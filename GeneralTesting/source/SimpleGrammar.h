#ifndef SPIRIT_HEAD
#define SPIRIT_HEAD
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/foreach.hpp>
#endif

#ifndef PARSE_NODE_HEAD
#define PARSE_NODE_HEAD
#include "ParserNode.h"
#endif

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template <typename Iterator>
struct SimpleGrammar : qi::grammar<Iterator, commonNode(), ascii::space_type>
{
    SimpleGrammar() : SimpleGrammar::base_type(program_)
    {
        using qi::lit;
        using qi::lexeme;
        using ascii::char_;
		using qi::uint_;
        using ascii::string;
        using namespace qi::labels;

        using phoenix::at_c;
        using phoenix::push_back;

		text_ = lexeme[+(char_ - '{' - '}' - ';')				[_val += qi::_1]];
		name_ = lexeme[+(char_ - ')' - '(' - qi::space - '+' - '-' - '*' - '/' - '=' - ';' - '"' - '{' - '}')			[_val += qi::_1]];

		call_	=
				string("call")				[at_c<0>(_val) = qi::_1]
			>>	name_						[at_c<1>(_val) = qi::_1]
			>>	';'
		;
			
		while_	=
				string("while")				[at_c<0>(_val) = qi::_1]
			>>	name_						[at_c<1>(_val) = qi::_1]
			>>	'{'
			>>	stmtLst_					[push_back(at_c<2>(_val), qi::_1)]
			>>	'}'
		;

		if_		=
				string("if")				[at_c<0>(_val) = qi::_1]
			>>	name_						[at_c<1>(_val) = qi::_1]
			>>	then_						[push_back(at_c<2>(_val), qi::_1)]
			>>	else_						[push_back(at_c<2>(_val), qi::_1)]
		;

		then_	=
				string("then")				[at_c<0>(_val) = qi::_1]
			>>	'{'
			>>	stmtLst_					[push_back(at_c<2>(_val), qi::_1)]
			>>	'}'
		;

		else_	=
				string("else")				[at_c<0>(_val) = qi::_1]
			>>	'{'
			>>	stmtLst_					[push_back(at_c<2>(_val), qi::_1)]
			>>	'}'
		;

		assign_	=
				name_						[at_c<0>(_val) = "assign"][at_c<1>(_val) = qi::_1]
			>>	'='
			>>	expr_						[push_back(at_c<2>(_val), qi::_1)]
			>>	';'
		;

		expr_ =	term_                       [_val = qi::_1]
            >> *(   ('+' >> term_           [_val += qi::_1])
            |   ('-' >> term_			    [_val -= qi::_1])
                )
        ;

		term_ =	factor_                     [_val = qi::_1]
            >> *(   ('*' >> factor_         [_val *= qi::_1])
                |   ('/' >> factor_         [_val /= qi::_1])
                )
        ;

		factor_ =
				name_                          [_val = qi::_1]
            |   '(' >> expr_                [_val = qi::_1] >> ')'
        ;
			
		stmt_	 %= (call_ | while_ | if_ | assign_);

		stmtLst_
				= +stmt_					[at_c<0>(_val) = "stmtLst"][push_back(at_c<2>(_val), qi::_1)]
		;
			
		procedure_
				= 
				string("procedure")			[at_c<0>(_val) = qi::_1]
			>>	name_						[at_c<1>(_val) = qi::_1]
			>>	'{'
			>>	stmtLst_					[push_back(at_c<2>(_val), qi::_1)]
			>>	'}'
		;
			
        program_
				=	+procedure_				[at_c<0>(_val) = "program"][push_back(at_c<2>(_val), qi::_1)]
		;
			
    }

    qi::rule<Iterator, commonNode(), ascii::space_type> program_;
	qi::rule<Iterator, commonNode(), ascii::space_type> procedure_;
	qi::rule<Iterator, commonNode(), ascii::space_type> stmtLst_;
	qi::rule<Iterator, commonNode(), ascii::space_type> stmt_;
	qi::rule<Iterator, commonNode(), ascii::space_type> call_;			
	qi::rule<Iterator, commonNode(), ascii::space_type> while_;
	qi::rule<Iterator, commonNode(), ascii::space_type> if_;
	qi::rule<Iterator, commonNode(), ascii::space_type> then_;
	qi::rule<Iterator, commonNode(), ascii::space_type> else_;
	qi::rule<Iterator, commonNode(), ascii::space_type> assign_;
	qi::rule<Iterator, expressionNode(), ascii::space_type> expr_;
	qi::rule<Iterator, expressionNode(), ascii::space_type> term_;
	qi::rule<Iterator, expressionNode(), ascii::space_type> factor_;
    qi::rule<Iterator, std::string(), ascii::space_type> text_;
	qi::rule<Iterator, std::string()> name_;
};