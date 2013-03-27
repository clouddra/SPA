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
    int insertResult(std::vector<std::string> vars, std::vector<std::vector<std::string>> vals);

public:
    ResultStorage();
    int insertResult(std::string var1, std::string var2, std::vector<std::vector<std::string>> vals);
    int insertResult(std::string var, std::vector<std::string> vals);
    std::vector<std::string> getValuesFor(std::string var);
    std::vector<std::vector<std::string>> getValuesFor(std::vector<std::string> vars);
    std::vector<ValidValue> getVVVector();
};