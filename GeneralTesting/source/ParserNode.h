#ifndef VARIANT_HEAD
#define VARIANT_HEAD
#include <boost/variant/recursive_variant.hpp>
#endif   

struct commonNode;
struct expressionNode;
struct binaryOp;
struct nil {};

typedef boost::variant<
	boost::recursive_wrapper<commonNode>,
	boost::recursive_wrapper<expressionNode>
> combinedNode;

struct commonNode
{
	std::string name;                       // Name
	std::string value;						// Value
	std::vector<combinedNode> children;    // Children
};

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
