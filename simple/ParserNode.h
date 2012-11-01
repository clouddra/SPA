#ifndef VARIANT_HEAD
#define VARIANT_HEAD
#include <boost/variant/recursive_variant.hpp>
#endif   

    struct common_node;
	struct expression_node;
		struct binary_op;
    struct nil {};

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
