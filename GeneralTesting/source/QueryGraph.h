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

    void insertQuery(std::string a, std::string b);
    void setWeight(std::vector<ValidValue> vvVector);
    int findMinWeight();
};