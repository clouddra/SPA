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
#include "Common.hpp"
#endif

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

// A Random Access Sequence is a Bidirectional Sequence whose iterators model Random Access Iterator. 
// It guarantees constant time access to arbitrary sequence elements.
BOOST_FUSION_ADAPT_STRUCT(
    commonNode,
    (std::string, name)
	(std::string, value)
    (std::vector<combinedNode>, children)
)
//]

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

struct CommonNodeInserter : boost::static_visitor<>
{
    explicit CommonNodeInserter(PKB* myPKB, int par) {
        pkb = myPKB;
        parent = par;
    }

    void operator()(commonNode const& node) const
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

		BOOST_FOREACH(combinedNode const& each_node, node.children)
		{
			boost::apply_visitor(CommonNodeInserter(pkb, newParent), each_node);
		}
	}

	void operator()(expressionNode const& ast) const
    {
        boost::apply_visitor(*this, ast.expr);
    }

	void operator()(binaryOp const& expr) const
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
        boost::apply_visitor(CommonNodeInserter(pkb, newParent), expr.left.expr);
        boost::apply_visitor(CommonNodeInserter(pkb, newParent), expr.right.expr);
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
    typedef SimpleGrammar<std::string::const_iterator> SimpleGrammar;
	SimpleGrammar simple; // Our grammar
    commonNode ast; // Our tree

    using boost::spirit::ascii::space;
    std::string::const_iterator iter = storage.begin();
    std::string::const_iterator end = storage.end();
	bool r = phrase_parse(iter, end, simple, space, ast);
	
    if (r && iter == end)
    {
        std::cout << "-------------------------\n";
        std::cout << "Simple parsing succeeded\n";
        std::cout << "-------------------------\n";
        //CommonNodePrinter printer;
        //printer(ast);
        CommonNodeInserter inserter(pkb, -1);
        inserter(ast);
        pkb->postParseCleanup();
        return 0;
    }
    else
    {
        //std::string::const_iterator some = iter+30;
        //std::string context(iter, (some>end)?end:some);
        std::cout << "-------------------------\n";
        std::cout << "Simple parsing failed\n";
        //std::cout << "stopped at: \": " << context << "...\"\n";
        std::cout << "-------------------------\n";
        return -1;
    }
}