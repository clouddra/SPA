#ifndef PARSER_HEAD
#define PARSER_HEAD
#include "Parser.h"
#endif

#ifndef GRAMMAR_HEAD
#define GRAMMA_HEAD
#include "SimpleGrammar.h"
#endif

#ifndef STD_HEAD
#define STD_HEAD
#include "common.hpp"
#endif

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


// A Random Access Sequence is a Bidirectional Sequence whose iterators model Random Access Iterator. 
// It guarantees constant time access to arbitrary sequence elements.
BOOST_FUSION_ADAPT_STRUCT(
    common_node,
    (std::string, name)
	(std::string, value)
    (std::vector<combined_type>, children)
)
//]


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

    struct common_node_inserter : boost::static_visitor<>
    {
        explicit common_node_inserter(PKB* myPKB, int par) {
            pkb = myPKB;
            parent = par;
        }

        void operator()(common_node const& node) const
		{
            int nodeType = -1;
            bool valueIsVar = false;
			
            if (node.name.compare("program") == 0)
            {
                nodeType = Node::programNode;
            }
            else if (node.name.compare("procedure") == 0)
            {
                nodeType = Node::procedureNode;
            }
            else if (node.name.compare("stmtLst") == 0)
            {
                nodeType = Node::stmtLstNode;
            }
            else if (node.name.compare("assign") == 0)
            {
                nodeType = Node::assignNode;
                valueIsVar = true;
            }
            else if (node.name.compare("while") == 0)
            {
                nodeType = Node::whileNode;
                valueIsVar = true;
            }
            else if (node.name.compare("if") == 0)
            {
                nodeType = Node::ifNode;
                valueIsVar = true;
            }
            else if (node.name.compare("call") == 0)
            {
                nodeType = Node::callNode;
            }

            int newParent = parent;
            if (nodeType != -1) {
                newParent = pkb->insertNode(nodeType, node.value, parent);
            }
            if (valueIsVar) {
                pkb->insertNode(Node::varNode, node.value, newParent);
            }

			BOOST_FOREACH(combined_type const& each_node, node.children)
			{
				boost::apply_visitor(common_node_inserter(pkb, newParent), each_node);
			}
		}

		void operator()(expression_node const& ast) const
        {
            boost::apply_visitor(*this, ast.expr);
        }

		void operator()(binary_op const& expr) const
        {
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

            int newParent = pkb->insertNode(nodeType, value, parent);
            boost::apply_visitor(common_node_inserter(pkb, newParent), expr.left.expr);
            boost::apply_visitor(common_node_inserter(pkb, newParent), expr.right.expr);
        }

        void operator()(std::string n) const { 
            std::istringstream convert(n);
            int temp, nodeType;
            if (!(convert >> temp)) {
                nodeType = Node::varNode;
            }
            else {
                nodeType = Node::constNode;
            }

            pkb->insertNode(nodeType, n, parent); 
        }

    private:
        PKB* pkb;
        int parent;
    };

int Parser::parseCode(std::string storage, PKB* pkb) {
    typedef simple_grammar<std::string::const_iterator> simple_grammar;
	simple_grammar simple; // Our grammar
    common_node ast; // Our tree

    using boost::spirit::ascii::space;
    std::string::const_iterator iter = storage.begin();
    std::string::const_iterator end = storage.end();
	bool r = phrase_parse(iter, end, simple, space, ast);
	
    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Parsing succeeded\n";
        std::cout << "-------------------------\n";
        //common_node_printer printer;
        //printer(ast);
        common_node_inserter inserter(pkb, -1);
        inserter(ast);
        pkb->postParseCleanup();
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