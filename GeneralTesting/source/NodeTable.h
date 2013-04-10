#ifndef VECTOR_HEAD
#define VECTOR_HEAD
#include <vector>
#endif

class NodeTable {

private:
    std::vector<int> plus;
    std::vector<int> minus;
    std::vector<int> times;
    std::vector<int> divide;
    std::vector<int> constants;
    std::vector<int> stmtLst;
    std::vector<int> procedures;
    std::vector<int> variables;

public:
    std::vector<int> getPlus();
    std::vector<int> getMinus();
    std::vector<int> getTimes();
    std::vector<int> getDivide();
    std::vector<int> getConstants();
    std::vector<int> getProcs();
    std::vector<int> getStmtLst();
    std::vector<int> getVariables();
    void insert(int nodeIndex, int type);
};