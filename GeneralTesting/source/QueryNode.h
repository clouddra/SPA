#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

#ifndef NODE_HEAD
#define NODE_HEAD
#include "Node.h"
#endif

class QueryNode {

private:
	std::string name;
	std::string value;
	std::vector<int> children; // Indexes of children
	int parent; // Parent's index

public:
    /**
	 * Constructor with name and value initialised
	 */
	QueryNode(std::string, std::string);

    /**
	 * Getter function for name
	 */
	std::string getName();

    /**
	 * Getter function for value
	 */
	std::string getValue();

    /**
	 * Getter function for children vector
	 */
	std::vector<int> getChildren();

    /**
	 * Add a child to the children vector
	 */
	void addChild(int);

    /**
	 * Delete every child except the 1st from the children vector (Used in reorgTree in QueryTree)
	 */
    void keep1Child();

    /**
	 * Setter function for parent
	 */
	void setParent(int);
};
