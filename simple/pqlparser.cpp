#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/foreach.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

			/*
			text_ = lexeme[+(char_ - '{' - '}' - ';')				[_val += _1]];
			name_ = lexeme[+(char_ - ')' - '(' - qi::space - '+' - '-' - '*' - '/' - '=' - ';')			[_val += _1]];

			
			call_	=
					string("call")				[at_c<0>(_val) = _1]
				>>	name_						[at_c<1>(_val) = _1]
				>>	';'
			;
			
			while_	=
					string("while")				[at_c<0>(_val) = _1]
				>>	name_						[at_c<1>(_val) = _1]
				>>	'{'
				>>	stmtLst_					[push_back(at_c<2>(_val), _1)]
				>>	'}'
			;

			if_		=
					string("if")				[at_c<0>(_val) = _1]
				>>	name_						[at_c<1>(_val) = _1]
				>>	then_						[push_back(at_c<2>(_val), _1)]
				>>	else_						[push_back(at_c<2>(_val), _1)]
			;

			then_	=
					string("then")				[at_c<0>(_val) = _1]
				>>	'{'
				>>	stmtLst_					[push_back(at_c<2>(_val), _1)]
				>>	'}'
			;

			else_	=
					string("else")				[at_c<0>(_val) = _1]
				>>	'{'
				>>	stmtLst_					[push_back(at_c<2>(_val), _1)]
				>>	'}'
			;

			assign_	=
					name_						[at_c<0>(_val) = "assign"][at_c<1>(_val) = _1]
				>>	'='
				>>	expr_						[push_back(at_c<2>(_val), _1)]
				>>	';'
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
				 name_                          [_val = _1]
                |   '(' >> expr_                [_val = _1] >> ')'
            ;
			
			stmt_	 %= (call_ | while_ | if_ | assign_);

			stmtLst_
					= *stmt_					[at_c<0>(_val) = "stmtLst"][push_back(at_c<2>(_val), _1)]
			;
			
			procedure_
					= 
					string("procedure")			[at_c<0>(_val) = _1]
				>>	name_						[at_c<1>(_val) = _1]
				>>	'{'
				>>	stmtLst_					[push_back(at_c<2>(_val), _1)]
				>>	'}'
			;
			
            program_
					=	+procedure_				[at_c<0>(_val) = "program"][push_back(at_c<2>(_val), _1)]
			;
			*/

			// Lexical Rules
			LETTER_ %= lexeme[char_("a-zA-Z")];
			DIGIT_ %= lexeme[char_("0-9")];
			INTEGER_ %= +DIGIT_;
			IDENT_ %= LETTER_ >> *(LETTER_ | DIGIT_ | lexeme[char_('#')]);

			// Auxiliary grammar rules
			synonym_ %= IDENT_;
			varRef_ %= synonym_ | char_('_') | ('"' >> IDENT_ >> '"') ;
			entRef_ %= synonym_ | char_('_') | ('"' >> IDENT_ >> '"') | INTEGER_;
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
			
			suchthat_cl_ %= "such that" >> relRef_;
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

		/*
        qi::rule<Iterator, common_node(), ascii::space_type> program_;
		qi::rule<Iterator, common_node(), ascii::space_type> procedure_;
		qi::rule<Iterator, common_node(), ascii::space_type> stmtLst_;
		qi::rule<Iterator, common_node(), ascii::space_type> stmt_;
		qi::rule<Iterator, common_node(), ascii::space_type> call_;			
		qi::rule<Iterator, common_node(), ascii::space_type> while_;
		qi::rule<Iterator, common_node(), ascii::space_type> if_;
		qi::rule<Iterator, common_node(), ascii::space_type> then_;
		qi::rule<Iterator, common_node(), ascii::space_type> else_;
		qi::rule<Iterator, common_node(), ascii::space_type> assign_;
		qi::rule<Iterator, expression_node(), ascii::space_type> expr_;
		qi::rule<Iterator, expression_node(), ascii::space_type> term_;
		qi::rule<Iterator, expression_node(), ascii::space_type> factor_;
        qi::rule<Iterator, std::string(), ascii::space_type> text_;
		qi::rule<Iterator, std::string()> name_;
		*/

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

/*
///////////////////////////////////////////////////////////////////////////////
// [Main program
int main(int argc, char **argv)
{
    char const* filename;
    if (argc > 1)
    {
        filename = argv[1];
    }
    else
    {
        //std::cerr << "Error: No input file provided." << std::endl;
        //return 1;
		filename = "C:\\pql.txt";
    }

    std::ifstream in(filename, std::ios_base::in);

    if (!in)
    {
        std::cerr << "Error: Could not open input file: "
            << filename << std::endl;
		int x;
		std::cin >> x;
        return 1;
    }

    std::string storage; // We will read the contents here.
    in.unsetf(std::ios::skipws); // No white space skipping!
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(storage));

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
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";
        pqlparser::common_node_printer printer;
        printer(pql_root);
        return 0;
    }
    else
    {
        std::string::const_iterator some = iter+30;
        std::string context(iter, (some>end)?end:some);
        std::cout << "-------------------------\n";
        std::cout << "Parsing failed\n";
        std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return 1;
    }
}
//]
*/
