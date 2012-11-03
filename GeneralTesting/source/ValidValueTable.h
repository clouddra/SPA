#ifndef VV_HEAD
#define VV_HEAD
#include "ValidValue.h"
#endif

// Table of valid values, note: does not support tuples
class ValidValueTable {
private:
    std::vector<ValidValue> vvTable;
    int findVariable(std::string var);

public:
    int insert(std::string variable, std::vector<std::string> values);
    std::vector<std::string> getValues(std::string variable);
};