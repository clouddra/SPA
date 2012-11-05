#ifndef PQLPARSER_HEAD
#define PQLPARSER_HEAD
#include "PqlParser.h"
#endif

#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

#ifndef STD_HEAD
#define STD_HEAD
#include "common.hpp"
#endif

namespace pqlparser
{
    namespace fusion = boost::fusion;
    namespace phoenix = boost::phoenix;
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    ///////////////////////////////////////////////////////////////////////////
    // [Internal AST representation
    struct common_node;
	struct expression_node;

    typedef boost::variant<
			boost::recursive_wrapper<common_node>,
			boost::recursive_wrapper<expression_node>,
			std::string
    > combined_type;
	
    struct common_node
    {
        std::string name;                       // Name
		std::string value;						// Value
        std::vector<combined_type> children;    // Children
    };
    
	// Expression representation
	struct binary_op;
    struct nil {};
    struct expression_node
    {
        typedef
            boost::variant<
                nil
              , std::string
              , boost::recursive_wrapper<expression_node>
              , boost::recursive_wrapper<binary_op>
            >
        type;
		
        expression_node()
          : expr(nil()) {}

        template <typename Expr>
        expression_node(Expr const& expr)
          : expr(expr) {}

        expression_node& operator+=(expression_node const& rhs);
        expression_node& operator-=(expression_node const& rhs);
        expression_node& operator*=(expression_node const& rhs);
        expression_node& operator/=(expression_node const& rhs);

        type expr;
    };

    struct binary_op
    {
        binary_op(
            char op
          , expression_node const& left
          , expression_node const& right)
        : op(op), left(left), right(right) {}

        char op;
        expression_node left;
        expression_node right;
    };

    expression_node& expression_node::operator+=(expression_node const& rhs)
    {
        expr = binary_op('+', expr, rhs);
        return *this;
    }

    expression_node& expression_node::operator-=(expression_node const& rhs)
    {
        expr = binary_op('-', expr, rhs);
        return *this;
    }

    expression_node& expression_node::operator*=(expression_node const& rhs)
    {
        expr = binary_op('*', expr, rhs);
        return *this;
    }

    expression_node& expression_node::operator/=(expression_node const& rhs)
    {
        expr = binary_op('/', expr, rhs);
        return *this;
    }
}

// A Random Access Sequence is a Bidirectional Sequence whose iterators model Random Access Iterator. 
// It guarantees constant time access to arbitrary sequence elements.
BOOST_FUSION_ADAPT_STRUCT(
    pqlparser::common_node,
    (std::string, name)
	(std::string, value)
    (std::vector<pqlparser::combined_type>, children)
)
//]

namespace pqlparser
{
    ///////////////////////////////////////////////////////////////////////////
    // [Print the tree
    int const tabsize = 4;

    void tab(int indent)
    {
        for (int i = 0; i < indent; ++i)
            std::cout << ' ';
    }

    struct common_node_printer : boost::static_visitor<>
    {
        common_node_printer(int indent = 0)
          : indent(indent)
        {
        }

        void operator()(common_node const& node) const
		{
			tab(indent);
			std::cout << "name: " << node.name << ", value: " << node.value << std::endl;

			tab(indent);
			std::cout << '{' << std::endl;

			BOOST_FOREACH(combined_type const& each_node, node.children)
			{
				boost::apply_visitor(common_node_printer(indent+tabsize), each_node);
			}

			tab(indent);
			std::cout << '}' << std::endl;
		}

		void operator()(expression_node const& ast) const
        {
            boost::apply_visitor(*this, ast.expr);
        }

		void operator()(binary_op const& expr) const
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
	struct common_node_inserter : boost::static_visitor<>
    {
        explicit common_node_inserter(QueryProcessor* myQP, int par) {
            queryProcessor = myQP;
            parent = par;
        }

        void operator()(common_node const& node) const
		{
            int newParent = parent;

			newParent = queryProcessor->insertNode(node.name, node.value, parent);

			BOOST_FOREACH(combined_type const& each_node, node.children)
			{
				boost::apply_visitor(common_node_inserter(queryProcessor, newParent), each_node);
			}
		}

		void operator()(expression_node const& ast) const
        {
            boost::apply_visitor(*this, ast.expr);
        }

		void operator()(binary_op const& expr) const
        {
		/* We shall not handle expressions for PQL yet
            std::string value(1, expr.op);
            int nodeType;

            if (value.compare("*") == 0)
            {
                nodeType = Node::timesNode;
            }
            else if (value.compare("/") == 0)
            {
                nodeType = Node::divideNode;
            }
            else if (value.compare("+") == 0)
            {
                nodeType = Node::plusNode;
            }
            else if (value.compare("-") == 0)
            {
                nodeType = Node::minusNode;
            }

            int newParent = pkb->insertNode(nodeType, value, true, parent);
            boost::apply_visitor(common_node_inserter(pkb, newParent), expr.left.expr);
            boost::apply_visitor(common_node_inserter(pkb, newParent), expr.right.expr);
		*/
        }

        void operator()(std::string n) const {     
			queryProcessor->insertNode(n, "", parent);

        }

	private:
        QueryProcessor* queryProcessor;
        int parent;
    };



    ///////////////////////////////////////////////////////////////////////////
    // [Grammar for PQL

    template <typename Iterator>
    struct pql_grammar : qi::grammar<Iterator, common_node(), ascii::space_type>
    {
        pql_grammar() : pql_grammar::base_type(select_cl_)
        {
            using qi::lit;
            using qi::lexeme;
            using ascii::char_;
			using qi::uint_;
            using ascii::string;
            using namespace qi::labels;

            using phoenix::at_c;
            using phoenix::push_back;

			// Lexical Rules
			LETTER_ %= lexeme[char_("a-zA-Z")];
			DIGIT_ %= lexeme[char_("0-9")];
			INTEGER_ %= +DIGIT_;
			IDENT_ %= LETTER_ >> *(LETTER_ | DIGIT_ | lexeme[char_('#')]);

			// Auxiliary grammar rules
			synonym_ %= IDENT_;
			varRef_ %= synonym_ | char_('_') | (char_('"') >> IDENT_ >> char_('"'));
			entRef_ %= synonym_ | char_('_') | (char_('"') >> IDENT_ >> char_('"')) | INTEGER_;
			stmtRef_ %= synonym_ | char_('_') | INTEGER_;

			design_entity_ %= 
				string("stmt")
				| string("assign")
				| string("while")
				| string("variable")
				| string("constant")
				| string("prog_line");

			// Grammar rules for select clause
			
			select_cl_ = 
				+declaration_				[push_back(at_c<2>(_val), _1)]
				>> string("Select")			[at_c<0>(_val) = "select"]
				>> synonym_					[at_c<1>(_val) = _1]
				>> *(suchthat_cl_)			[push_back(at_c<2>(_val), _1)]
				;

			declaration_ = 
				design_entity_				[at_c<0>(_val) = "declaration"][at_c<1>(_val) = _1]
				>> synonym_					[push_back(at_c<2>(_val), _1)]
				>> *(','					
				>> synonym_)				[push_back(at_c<2>(_val), _1)]
				>> ';'
				;
			
			suchthat_cl_ = 
				string("such that")			[at_c<0>(_val) = "such that"]
				>> relRef_					[push_back(at_c<2>(_val), _1)]
				;
			//pattern_cl_ = string("pattern") >> syn_assign_ >> char_("(") >> entRef_ >> char_(",") >> (expression_spec_ | char_("_")) >> char_(")");

			relRef_ %= ModifiesS_ | ModifiesP_ | UsesS_ | UsesP_ | Parent_ | ParentT_ | Follows_ | FollowsT_;
			
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

			//syn_assign_ = synonym_; // This must be declared as synonym of assignment
			

			//expression_spec_ = (char_('"') >> expr_ >> char_('"')) | (char_("_") >> char_('"') >> expr_ >> char_('"') >> char_("_"));

			// Simple grammar used
			/*
			NAME_ = LETTER_ >> *(LETTER_ | DIGIT_);
			expr_ = (expr_ >> char_("+") >> term_) | (expr_ >> char_("-") >> term_) | term_;
			term_ = (term_ >> char_("*") >> factor_) | factor_;
			factor_ = var_name_ | const_value_ | (char_("(") >> expr_ >> char_(")"));
			var_name_ = NAME_;
			const_value_ = INTEGER_;
			*/
        }

		// Lexical Rules
		qi::rule<Iterator, char()> LETTER_;
		qi::rule<Iterator, char()> DIGIT_;
		qi::rule<Iterator, std::string()> INTEGER_;
		qi::rule<Iterator, std::string()> IDENT_;

		// Auxiliary grammar rules
		qi::rule<Iterator, std::string(), ascii::space_type> synonym_;
		qi::rule<Iterator, std::string(), ascii::space_type> entRef_;
		qi::rule<Iterator, std::string(), ascii::space_type> stmtRef_;
		qi::rule<Iterator, std::string(), ascii::space_type> varRef_;
		qi::rule<Iterator, std::string(), ascii::space_type> design_entity_;

		// Grammar rules for select clause
		qi::rule<Iterator, common_node(), ascii::space_type> select_cl_;
		qi::rule<Iterator, common_node(), ascii::space_type> declaration_;
		qi::rule<Iterator, common_node(), ascii::space_type> suchthat_cl_;
		//qi::rule<Iterator, ascii::space_type> pattern_cl_;
		qi::rule<Iterator, common_node(), ascii::space_type> relRef_;
		qi::rule<Iterator, common_node(), ascii::space_type> ModifiesS_;
		qi::rule<Iterator, common_node(), ascii::space_type> ModifiesP_;
		qi::rule<Iterator, common_node(), ascii::space_type> UsesS_;
		qi::rule<Iterator, common_node(), ascii::space_type> UsesP_;
		qi::rule<Iterator, common_node(), ascii::space_type> Parent_;
		qi::rule<Iterator, common_node(), ascii::space_type> ParentT_;
		qi::rule<Iterator, common_node(), ascii::space_type> Follows_;
		qi::rule<Iterator, common_node(), ascii::space_type> FollowsT_;
		//qi::rule<Iterator, ascii::space_type> syn_assign_;

	//	qi::rule<Iterator, ascii::space_type> expression_spec_;

		/*
		qi::rule<Iterator> NAME_;
		qi::rule<Iterator, ascii::space_type> expr_;
		qi::rule<Iterator, ascii::space_type> term_;
		qi::rule<Iterator, ascii::space_type> factor_;
		qi::rule<Iterator> var_name_;
		qi::rule<Iterator> const_value_;
		*/
    };

    //]
}

int PqlParser::parseQuery(std::string storage, QueryProcessor* qp)
{
	typedef pqlparser::pql_grammar<std::string::const_iterator> pql_grammar;
	pql_grammar pql; // Our grammar
    pqlparser::common_node pql_root; // Our tree

    using boost::spirit::ascii::space;
    std::string::const_iterator iter = storage.begin();
    std::string::const_iterator end = storage.end();
	bool r = phrase_parse(iter, end, pql, space, pql_root);
	
    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "PQL parsing succeeded\n";
        std::cout << "-------------------------\n";
        //pqlparser::common_node_printer printer;
        //printer(pql_root);
		pqlparser::common_node_inserter inserter(qp, -1);
		inserter(pql_root);
        return 0;
    }
    else
    {
        std::string::const_iterator some = iter+30;
        std::string context(iter, (some>end)?end:some);
        std::cout << "-------------------------\n";
        std::cout << "PQL parsing failed\n";
        std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return -1;
    }
}

// Splits input into multiple pql queries, appending the declaration to each of them
// No delimiter needed (Delimiter is "Select", IT IS CASE SENSITIVE)
std::vector<std::string> PqlParser::splitQuery(std::string input) {
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