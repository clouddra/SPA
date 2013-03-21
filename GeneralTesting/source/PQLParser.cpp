#ifndef PQLPARSER_HEAD
#define PQLPARSER_HEAD
#include "PQLParser.h"
#endif

#ifndef BOOST_HEAD
#define BOOST_HEAD
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>
#endif


#ifndef STD_HEAD
#define STD_HEAD
#include "Common.hpp"
#endif

namespace pqlparser
{
    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    ///////////////////////////////////////////////////////////////////////////
    // [Internal AST representation
    struct commonNode;
	struct expressionNode;

    typedef boost::variant<
			boost::recursive_wrapper<commonNode>,
			boost::recursive_wrapper<expressionNode>,
			std::string
    > combinedNode;
	
    struct commonNode
    {
        std::string name;                       // Name
		std::string value;						// Value
        std::vector<combinedNode> children;    // Children
    };

	// Used by boost spirit to roll back partial push_backs.
	void swap(commonNode &a, commonNode &b)
	{
		commonNode t;
		t.name = a.name;
		t.value = a.value;
		t.children = a.children;
		a.name = b.name;
		a.value = b.value;
		a.children = b.children;
		b.name = t.name;
		b.value = t.value;
		b.children = t.children;
	}

	// Expression representation
	struct binaryOp;
    struct nil {};
    struct expressionNode
    {
        typedef
            boost::variant<
                nil
              , std::string
              , boost::recursive_wrapper<expressionNode>
              , boost::recursive_wrapper<binaryOp>
            >
        type;
		
        expressionNode()
          : expr(nil()) {}

        template <typename Expr>
        expressionNode(Expr const& expr)
          : expr(expr) {}

        expressionNode& operator+=(expressionNode const& rhs);
        expressionNode& operator-=(expressionNode const& rhs);
        expressionNode& operator*=(expressionNode const& rhs);
        expressionNode& operator/=(expressionNode const& rhs);

        type expr;
    };

    struct binaryOp
    {
        binaryOp(
            char op
          , expressionNode const& left
          , expressionNode const& right)
        : op(op), left(left), right(right) {}

        char op;
        expressionNode left;
        expressionNode right;
    };

    expressionNode& expressionNode::operator+=(expressionNode const& rhs)
    {
        expr = binaryOp('+', expr, rhs);
        return *this;
    }

    expressionNode& expressionNode::operator-=(expressionNode const& rhs)
    {
        expr = binaryOp('-', expr, rhs);
        return *this;
    }

    expressionNode& expressionNode::operator*=(expressionNode const& rhs)
    {
        expr = binaryOp('*', expr, rhs);
        return *this;
    }

    expressionNode& expressionNode::operator/=(expressionNode const& rhs)
    {
        expr = binaryOp('/', expr, rhs);
        return *this;
    }
}

// A Random Access Sequence is a Bidirectional Sequence whose iterators model Random Access Iterator. 
// It guarantees constant time access to arbitrary sequence elements.
BOOST_FUSION_ADAPT_STRUCT(
    pqlparser::commonNode,
    (std::string, name)
	(std::string, value)
    (std::vector<pqlparser::combinedNode>, children)
)
//]

namespace pqlparser
{
    ///////////////////////////////////////////////////////////////////////////
    // [Print the tree
    int const tabSize = 4;

    void tab(int indent)
    {
        for (int i = 0; i < indent; ++i)
            std::cout << ' ';
    }

    struct CommonNodePrinter : boost::static_visitor<>
    {
        CommonNodePrinter(int indent = 0)
          : indent(indent)
        {
        }

        void operator()(commonNode const& node) const
		{
			tab(indent);
			std::cout << "name: " << node.name << ", value: " << node.value << std::endl;

			tab(indent);
			std::cout << '{' << std::endl;

			BOOST_FOREACH(combinedNode const& each_node, node.children)
			{
				boost::apply_visitor(CommonNodePrinter(indent+tabSize), each_node);
			}

			tab(indent);
			std::cout << '}' << std::endl;
		}

		void operator()(expressionNode const& ast) const
        {
            boost::apply_visitor(*this, ast.expr);
        }

		void operator()(binaryOp const& expr) const
        {
            std::cout << "op:" << expr.op << "(";
            boost::apply_visitor(*this, expr.left.expr);

            std::cout << ", ";
            boost::apply_visitor(*this, expr.right.expr);

			std::cout << ')';
        }

		void operator()(std::string n) const { std::cout << n; }

        int indent;
    };
	//]



	/// Copied from Shihan's Parser.cpp
	struct CommonNodeInserter : boost::static_visitor<>
    {
        explicit CommonNodeInserter(QueryProcessor* myQP, int par) {
            queryProcessor = myQP;
            parent = par;
        }

        void operator()(commonNode const& node) const
		{
            int newParent = parent;

			newParent = queryProcessor->insertNode(node.name, node.value, parent);

			BOOST_FOREACH(combinedNode const& each_node, node.children)
			{
				boost::apply_visitor(CommonNodeInserter(queryProcessor, newParent), each_node);
			}
		}

		void operator()(expressionNode const& ast) const
        {
            boost::apply_visitor(*this, ast.expr);
        }

		void operator()(binaryOp const& expr) const
        {
            std::string value(1, expr.op);

            int newParent = queryProcessor->insertNode(value, value, parent);
            boost::apply_visitor(CommonNodeInserter(queryProcessor, newParent), expr.left.expr);
            boost::apply_visitor(CommonNodeInserter(queryProcessor, newParent), expr.right.expr);
        }

        void operator()(std::string n) const {   
            std::istringstream convert(n);
            int temp;
            std::string type;

            if (!(convert >> temp)) {
                type = "variable";
            }
            else {
                type = "constant";
            }

			queryProcessor->insertNode(n, type, parent);
        }

	private:
        QueryProcessor* queryProcessor;
        int parent;
    };



    ///////////////////////////////////////////////////////////////////////////
    // [Grammar for PQL

    template <typename Iterator>
    struct pql_grammar : qi::grammar<Iterator, commonNode(), ascii::space_type>
    {
        pql_grammar() : pql_grammar::base_type(select_cl_)
        {
            using qi::lit;
            using qi::lexeme;
            using ascii::char_;
			using qi::uint_;
            using ascii::string;
			using boost::spirit::qi::hold;
            using namespace qi::labels;
			
            using phoenix::at_c;
            using phoenix::push_back;

			// Lexical Rules
			LETTER_ %= lexeme[char_("a-zA-Z")];
			DIGIT_ %= lexeme[char_("0-9")];
			INTEGER_ %= +DIGIT_;
			IDENT_ %= LETTER_ >> *(LETTER_ | DIGIT_ | lexeme[char_('#')]);
			NAME_ = lexeme[+(char_ - ')' - '(' - qi::space - '+' - '-' - '*' - '/' - '=' - ';' - '"' - '{' - '}')			[_val += _1]];

			// Auxiliary grammar rules
			tuple_ = elem_					[at_c<0>(_val) = "elem"][push_back(at_c<2>(_val), _1)]
				| '<' >> elem_				[at_c<0>(_val) = "elem"][push_back(at_c<2>(_val), _1)]
				>> *(',' >> elem_			[at_c<0>(_val) = "tuple"][push_back(at_c<2>(_val), _1)]) 
				>> '>';
			elem_ = hold[synonym_[at_c<0>(_val) = "synonym"][push_back(at_c<2>(_val), _1)]] 
				| attrRef_ [at_c<0>(_val) = "attrRef"][push_back(at_c<2>(_val), _1)];
			synonym_ %= IDENT_;
			attrName_ %= string("procName") | string("varName") | string("value") | string("stmt#");
			varRef_ %= synonym_ | char_('_') | (char_('"') >> IDENT_ >> char_('"'));
			entRef_ %= synonym_ | char_('_') | (char_('"') >> IDENT_ >> char_('"')) | INTEGER_;
			stmtRef_ %= synonym_ | char_('_') | INTEGER_;
			lineRef_ %= synonym_ | char_('_') | INTEGER_;

			design_entity_ %= 
				string ("procedure")
                | string("stmt")
				| string("assign")
                | string ("call")
				| string("while")
                | string("if")
				| string("variable")
				| string("constant")
				| string("prog_line");

			// Grammar rules for select clause
			
			select_cl_ = 
				*declaration_				[push_back(at_c<2>(_val), _1)]
				>> string("Select")			[at_c<0>(_val) = "select"]
				>> result_cl_				[push_back(at_c<2>(_val), _1)]
				>> *(suchthat_cl_ | with_cl_ | pattern_cl_)	[push_back(at_c<2>(_val), _1)]
				;

			result_cl_ = 
				tuple_						[at_c<0>(_val) = "result"][push_back(at_c<2>(_val), _1)]
				;

			declaration_ = 
				design_entity_				[at_c<0>(_val) = "declaration"][at_c<1>(_val) = _1]
				>> synonym_					[push_back(at_c<2>(_val), _1)]
				>> *(','					
				>> synonym_)				[push_back(at_c<2>(_val), _1)]
				>> ';'
				;
			
				/*
			suchthat_cl_ = 
				string("such that")			[at_c<0>(_val) = "such that"]
				>> relRef_					[push_back(at_c<2>(_val), _1)]
				;
				*/
			suchthat_cl_ = 
				string("such that")			[at_c<0>(_val) = "such that"]
				>> relCond_					[push_back(at_c<2>(_val), _1)]
				;

			pattern_cl_ = 
				string("pattern")			[at_c<0>(_val) = "pattern"]
				>> patternCond_				[push_back(at_c<2>(_val), _1)]
				;

			with_cl_ = 
				string("with")				[at_c<0>(_val) = "with"]
				>> attrCond_				[push_back(at_c<2>(_val), _1)]
				;

			attrCond_ = 
				attrCompare_				[at_c<0>(_val) = "attrCompare"][push_back(at_c<2>(_val), _1)]
				>> *("and" >> attrCompare_  [push_back(at_c<2>(_val), _1)])		
				;

			attrCompare_ = 
				hold[
				(attrRef_					[at_c<0>(_val) = "attrCompare_attrRef"][push_back(at_c<2>(_val), _1)]
				>> string("=")
				>> ref_						[push_back(at_c<2>(_val), _1)]
				)
				]
				|
				(
				synonym_					[at_c<0>(_val) = "attrCompare_synonym"][push_back(at_c<2>(_val), _1)]
				>> string("=")
				>> ref_pl_					[push_back(at_c<2>(_val), _1)]
				)
				;

			ref_pl_ =
				INTEGER_					[at_c<0>(_val) = "ref_pl_integer"][push_back(at_c<2>(_val), _1)]
				| attrRef_					[at_c<0>(_val) = "ref_pl_attrRef"][push_back(at_c<2>(_val), _1)]
				;

			attrRef_ =
				synonym_					[at_c<0>(_val) = "attrRef"][push_back(at_c<2>(_val), _1)]
				>> string(".")
				>> attrName_				[push_back(at_c<2>(_val), _1)]
				;
			
			ref_ =
				('"' >> IDENT_ >> '"')		[at_c<0>(_val) = "ref_ident"][push_back(at_c<2>(_val), _1)]
				| INTEGER_					[at_c<0>(_val) = "ref_integer"][push_back(at_c<2>(_val), _1)]
				| attrRef_					[at_c<0>(_val) = "ref_attrRef"][push_back(at_c<2>(_val), _1)]
				;
				
			patternCond_ =
				pattern_					[at_c<0>(_val) = "patternCond"][push_back(at_c<2>(_val), _1)]
				>> *("and" >> pattern_      [push_back(at_c<2>(_val), _1)])	
				;

			pattern_ %= hold[if_] | assign_or_while_;
			
			expression_spec_ = 
				(('"' >> expr_ [at_c<0>(_val) = "expr_no_underscore"][push_back(at_c<2>(_val), _1)] >> '"') 
				| (lit('_') >> '"' >> expr_ [at_c<0>(_val) = "expr_with_underscore"][push_back(at_c<2>(_val), _1)] >> '"' >> lit('_')))
				;

			expr_ =	term_                       [_val = _1]
                >> *(   ('+' >> term_           [_val += _1])
                |   ('-' >> term_			    [_val -= _1])
                    )
				;

			term_ =	factor_                     [_val = _1]
                >> *(   ('*' >> factor_         [_val *= _1])
                    |   ('/' >> factor_         [_val /= _1])
                    )
				;

			factor_ =
				 NAME_                          [_val = _1]
                |   '(' 
				>> expr_						[_val = _1] 
				>> ')'
				;

			assign_or_while_ = 
				synonym_					[at_c<0>(_val) = "pattern_assign_or_while_"][push_back(at_c<2>(_val), _1)]
				>> '('
				>> varRef_					[push_back(at_c<2>(_val), _1)]
				>> ','
				>> (expression_spec_ | string("_")) 	[push_back(at_c<2>(_val), _1)]
				>> ')'
				;
				
			if_ = 
				synonym_					[at_c<0>(_val) = "pattern_if_"][push_back(at_c<2>(_val), _1)]
				>> '('
				>> varRef_					[push_back(at_c<2>(_val), _1)]
				>> ','
				>> string("_")				[push_back(at_c<2>(_val), _1)]
				>> ','
				>> string("_")				[push_back(at_c<2>(_val), _1)]
				>> ')'
				;

			relCond_ =
				relRef_						[at_c<0>(_val) = "relcond_"][push_back(at_c<2>(_val), _1)]
				>> *("and" >> relRef_       [push_back(at_c<2>(_val), _1)])		
				;

			relRef_ %= ModifiesS_ | ModifiesP_ | UsesS_ | UsesP_ | Parent_ | ParentT_ | Follows_ | FollowsT_ | Calls_ | CallsT_ | Next_ | NextT_;
			
			ModifiesS_ = 
				string("Modifies")			[at_c<0>(_val) = "modifiess"]
				>> '('
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]
				>> ',' 
				>> varRef_					[push_back(at_c<2>(_val), _1)]
				>> ')'
				;

			ModifiesP_ = 
				string("Modifies")			[at_c<0>(_val) = "modifiesp"]
				>> '('
				>> entRef_					[push_back(at_c<2>(_val), _1)]
				>> ',' 
				>> varRef_					[push_back(at_c<2>(_val), _1)]
				>> ')'
				;

			UsesS_ = 
				string("Uses")				[at_c<0>(_val) = "usess"]
				>> '(' 
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]					
				>> ',' 
				>> varRef_					[push_back(at_c<2>(_val), _1)]
				>> ')'
				;

			UsesP_ = 
				string("Uses")				[at_c<0>(_val) = "usesp"]
				>> '(' 
				>> entRef_					[push_back(at_c<2>(_val), _1)]					
				>> ',' 
				>> varRef_					[push_back(at_c<2>(_val), _1)]
				>> ')'
				;

			Parent_ = 
				string("Parent")			[at_c<0>(_val) = "parent"]
				>> '(' 
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]	
				>> ',' 
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]	
				>> ')'
				;

			ParentT_ = 
				string("Parent*")			[at_c<0>(_val) = "parentt"]
				>> '('
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]	
				>> ','
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]	
				>> ')'
				;

			Follows_ = 
				string("Follows")			[at_c<0>(_val) = "follows"]
				>> char_('(') 
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]	
				>> char_(',') 
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]	
				>> char_(')')
				;

			FollowsT_ = 
				string("Follows*")			[at_c<0>(_val) = "followst"]
				>> char_('(') 
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]	
				>> char_(',')				
				>> stmtRef_					[push_back(at_c<2>(_val), _1)]	
				>> char_(')')
				;

			Calls_ =
				string("Calls")				[at_c<0>(_val) = "calls"]
				>> '('
				>> entRef_					[push_back(at_c<2>(_val), _1)]	
				>> ','
				>> entRef_					[push_back(at_c<2>(_val), _1)]	
				>> ')'
				;

			CallsT_ =
				string("Calls*")			[at_c<0>(_val) = "callst"]
				>> '('
				>> entRef_					[push_back(at_c<2>(_val), _1)]	
				>> ','
				>> entRef_					[push_back(at_c<2>(_val), _1)]	
				>> ')'
				;

			Next_ =
				string("Next")				[at_c<0>(_val) = "next"]
				>> '('
				>> lineRef_					[push_back(at_c<2>(_val), _1)]
				>> ','
				>> lineRef_					[push_back(at_c<2>(_val), _1)]
				>> ')'
				;

			NextT_ =
				string("Next*")				[at_c<0>(_val) = "nextt"]
				>> '('
				>> lineRef_					[push_back(at_c<2>(_val), _1)]
				>> ','
				>> lineRef_					[push_back(at_c<2>(_val), _1)]
				>> ')'
				;
        }

		// Lexical Rules
		qi::rule<Iterator, char()> LETTER_;
		qi::rule<Iterator, char()> DIGIT_;
		qi::rule<Iterator, std::string()> INTEGER_;
		qi::rule<Iterator, std::string()> IDENT_;
		qi::rule<Iterator, std::string()> NAME_;

		// Auxiliary grammar rules
		qi::rule<Iterator, commonNode(), ascii::space_type> tuple_;
		qi::rule<Iterator, commonNode(), ascii::space_type> elem_;
		qi::rule<Iterator, std::string(), ascii::space_type> synonym_;
		qi::rule<Iterator, std::string(), ascii::space_type> attrName_;
		qi::rule<Iterator, std::string(), ascii::space_type> entRef_;
		qi::rule<Iterator, std::string(), ascii::space_type> stmtRef_;
		qi::rule<Iterator, std::string(), ascii::space_type> varRef_;
		qi::rule<Iterator, std::string(), ascii::space_type> lineRef_;
		qi::rule<Iterator, std::string(), ascii::space_type> design_entity_;

		// Grammar rules for select clause
		qi::rule<Iterator, commonNode(), ascii::space_type> select_cl_;
		qi::rule<Iterator, commonNode(), ascii::space_type> result_cl_;
		qi::rule<Iterator, commonNode(), ascii::space_type> declaration_;
		qi::rule<Iterator, commonNode(), ascii::space_type> with_cl_;
		qi::rule<Iterator, commonNode(), ascii::space_type> attrCond_;
		qi::rule<Iterator, commonNode(), ascii::space_type> attrCompare_;
		qi::rule<Iterator, commonNode(), ascii::space_type> attrRef_;
		qi::rule<Iterator, commonNode(), ascii::space_type> ref_pl_;
		qi::rule<Iterator, commonNode(), ascii::space_type> ref_;
		qi::rule<Iterator, commonNode(), ascii::space_type> suchthat_cl_;
		qi::rule<Iterator, commonNode(), ascii::space_type> pattern_cl_;
		qi::rule<Iterator, commonNode(), ascii::space_type> patternCond_;
		qi::rule<Iterator, commonNode(), ascii::space_type> pattern_;
		qi::rule<Iterator, commonNode(), ascii::space_type> if_;
		qi::rule<Iterator, commonNode(), ascii::space_type> assign_or_while_;
		//qi::rule<Iterator, commonNode(), ascii::space_type> while_;
		//qi::rule<Iterator, commonNode(), ascii::space_type> assign_;
		qi::rule<Iterator, commonNode(), ascii::space_type> expression_spec_;
		qi::rule<Iterator, expressionNode(), ascii::space_type> expr_;
		qi::rule<Iterator, expressionNode(), ascii::space_type> term_;
		qi::rule<Iterator, expressionNode(), ascii::space_type> factor_;
		qi::rule<Iterator, commonNode(), ascii::space_type> relRef_;
		qi::rule<Iterator, commonNode(), ascii::space_type> relCond_;
		qi::rule<Iterator, commonNode(), ascii::space_type> ModifiesS_;
		qi::rule<Iterator, commonNode(), ascii::space_type> ModifiesP_;
		qi::rule<Iterator, commonNode(), ascii::space_type> UsesS_;
		qi::rule<Iterator, commonNode(), ascii::space_type> UsesP_;
		qi::rule<Iterator, commonNode(), ascii::space_type> Parent_;
		qi::rule<Iterator, commonNode(), ascii::space_type> ParentT_;
		qi::rule<Iterator, commonNode(), ascii::space_type> Follows_;
		qi::rule<Iterator, commonNode(), ascii::space_type> FollowsT_;
		qi::rule<Iterator, commonNode(), ascii::space_type> Calls_;
		qi::rule<Iterator, commonNode(), ascii::space_type> CallsT_;
		qi::rule<Iterator, commonNode(), ascii::space_type> Next_;
		qi::rule<Iterator, commonNode(), ascii::space_type> NextT_;
    };

    //]
}

int PQLParser::parseQuery(std::string storage, QueryProcessor* qp)
{
	typedef pqlparser::pql_grammar<std::string::const_iterator> pql_grammar;
	pql_grammar pql; // Our grammar
    pqlparser::commonNode pql_root; // Our tree

    using boost::spirit::ascii::space;
    std::string::const_iterator iter = storage.begin();
    std::string::const_iterator end = storage.end();
	bool r = phrase_parse(iter, end, pql, space, pql_root);
	
    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "PQL parsing succeeded\n";
        std::cout << "-------------------------\n";
        //pqlparser::CommonNodePrinter printer;
        //printer(pql_root);
		pqlparser::CommonNodeInserter inserter(qp, -1);
		inserter(pql_root);
        return 0;
    }
    else
    {
        //std::string::const_iterator some = iter+30;
        //std::string context(iter, (some>end)?end:some);
        std::cout << "-------------------------\n";
        std::cout << "PQL parsing failed\n";
        //std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return -1;
    }
}

// Splits input into multiple pql queries, appending the declaration to each of them
// No delimiter needed (Delimiter is "Select", IT IS CASE SENSITIVE)
std::vector<std::string> PQLParser::splitQuery(std::string input) {
    std::vector<std::string> queries;
    std::string declaration;
    std::string sel = "Select";
    int found = 0;
    
    found = input.find(sel);
    if (found != std::string::npos)
    {
        declaration = input.substr(0, found);
        input = input.substr(found);
        while (found != std::string::npos) {
            found = input.find(sel, 1);
            std::string query;
            if (found != std::string::npos) {
                query = declaration + input.substr(0, found);
                input = input.substr(found);
            }
            else
                query = declaration + input;
            queries.push_back(query);
        }
    }

    return queries;
}