#ifndef QUERYGRAPH_HEAD
#define QUERYGRAPH_HEAD
#include "QueryGraph.h"
#endif

void QueryGraph::insertQuery(std::string a, std::string b) {
    Query temp = Query();
    std::vector<int> connections;
    int index = graph.size();

    temp.weight = INT_MAX;
    temp.para1 = a;
    temp.para2 = b;
    temp.evaluated = false;

    for (int i = 0; i < (int)graph.size(); i++) {
        if (a.compare(graph[i].para1) == 0 || a.compare(graph[i].para2) == 0) {
            connections.push_back(i);
            graph[i].connections.push_back(index);
        }
        else if (b.compare(graph[i].para1) == 0 || b.compare(graph[i].para2) == 0) {
            connections.push_back(i);
            graph[i].connections.push_back(index);
        }
    }
    temp.connections = connections;
    graph.push_back(temp);
}

void QueryGraph::setWeight(std::vector<ValidValue> vvVector) {
    for (int i = 0; i < (int)vvVector.size(); i++) {
        for (int j = 0; j < (int)graph.size(); j++) {
            graph[j].setWeight(vvVector[i].getVariable(), vvVector[i].getValues().size());
        }
    }
}
    
int QueryGraph::findMinWeight() {
    int min = INT_MAX, minIndex = -1;
    for (int i = 0; i < (int)graph.size(); i++) {
        if (!graph[i].evaluated && min >= graph[i].weight) {
            min = graph[i].weight;
            minIndex = i;
        }
    }
    return minIndex;
}