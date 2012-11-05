#ifndef VVTUPLE_HEAD
#define VVTUPLE_HEAD
#include "ValidValueTuple.h"
#endif

// Table of valid values for tuples
class ValidValueTupleTable {
private:
    std::vector<ValidValueTuple> vvTupleTable;
    int findVariable(std::string var1, std::string var2);

public:
    int insert(std::string var1, std::string var2, std::vector<std::pair<std::string,std::string>> val);
    std::vector<std::string> getValues(std::string variable);
    int restrictTo(ValidValueTable vvTable);
    int reconcile();
};