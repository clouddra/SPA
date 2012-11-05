#ifndef STD_HEAD
#define STD_HEAD
#include "common.hpp"
#endif

#ifndef VVTABLE_HEAD
#define VVTABLE_HEAD
#include "ValidValueTable.h"
#endif

// Maps a variable tuple to the values it can take, used in query processor
// Note that values are strings

class ValidValueTuple {
private:
    std::string variable1;
    std::string variable2;
    std::vector<std::pair<std::string,std::string>> values;

public:
    ValidValueTuple();
    ValidValueTuple(std::string var1, std::string var2, std::vector<std::pair<std::string,std::string>> val);
    std::string getVariable1();
    std::string getVariable2();
    std::vector<std::string> getValuesForVar(std::string target);
    int restrictTo(std::string inputVar, std::vector<std::string> inputVal);
    int restrictTo(ValidValueTable vvTable);
};