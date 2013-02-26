#ifndef TT_HEAD
#define TT_HEAD
#include "TupleTable.h"
#endif

#ifndef VV_HEAD
#define VV_HEAD
#include "ValidValue.h"
#endif

class ResultStorage {

private:
    std::vector<TupleTable> ttVector;
    std::vector<ValidValue> vvVector;

public:
    ResultStorage();
    int insertResult(std::vector<std::string> vars, std::vector<std::vector<std::string>> vals);
    int insertResult(std::string var, std::vector<std::string> vals);
    std::set<std::string> getValuesFor(std::string var);
};