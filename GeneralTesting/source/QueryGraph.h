#ifndef QUERY_HEAD
#define QUERY_HEAD
#include "Query.h"
#endif

#ifndef VV_HEAD
#define VV_HEAD
#include "ValidValue.h"
#endif

class QueryGraph {
public:
    std::vector<Query> graph;

    /**
	 * Insert a query into the query graph, queries must have 2 synonyms
	 */
    void insertQuery(std::string a, std::string b);

    /**
	 * Set weights for all query in graph based on vvVector from resultStorage
	 */
    void setWeight(std::vector<ValidValue> vvVector);

    /**
	 * Find the query with the minimum weight and return its INDEX 
	 */
    int findMinWeight();
};