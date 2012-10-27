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

namespace simpleparser
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
			boost::recursive_wrapper<expression_node>
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
    simpleparser::common_node,
    (std::string, name)
	(std::string, value)
    (std::vector<simpleparser::combined_type>, children)
)
//]

namespace simpleparser
{
    ///////////////////////////////////////////////////////////////////////////
    // [Grammar for Simple

    template <typename Iterator>
    struct simple_grammar : qi::grammar<Iterator, common_node(), ascii::space_type>
    {
        simple_grammar() : simple_grammar::base_type(program_)
        {
            using qi::lit;
            using qi::lexeme;
            using ascii::char_;
			using qi::uint_;
            using ascii::string;
            using namespace qi::labels;

            using phoenix::at_c;
            using phoenix::push_back;

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
			
        }

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
    };

    //]
}