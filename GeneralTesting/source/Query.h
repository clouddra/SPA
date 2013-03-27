#ifndef STD_HEAD
#define STD_HEAD
#include "Common.hpp"
#endif

class Query {
public:
    std::vector<int> connections;
    std::string para1;
    std::string para2;
    int weight;
    bool evaluated;

    void setWeight(std::string a, int inputWeight) {
       if (a.compare(para1) == 0 || a.compare(para2) == 0) {
           if (inputWeight < weight)
               weight = inputWeight;
        }
    }
};