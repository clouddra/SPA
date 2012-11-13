#ifndef VVTABLE_HEAD
#define VVTABLE_HEAD
#include "ValidValueTable.h"
#endif

int ValidValueTable::findVariable(std::string var) {
    for (int i = 0; i < (int)vvTable.size(); i++) {
        if (vvTable[i].getVariable().compare(var) == 0)
            return i;
    }
    return -1;
}

int ValidValueTable::insert(std::string variable, std::vector<std::string> values) {
    if ((int)values.size() == 0) {
        // std::cout << variable << " has no valid values\n"; 
        return -1;
    }
    int index = findVariable(variable);
    if (index == -1) {
        vvTable.push_back(ValidValue(variable, values));
    }
    else {
        return vvTable[index].restrictTo(values);
    }
    return 0;
}

std::vector<std::string> ValidValueTable::getValues(std::string variable) {
    int index = findVariable(variable);
    if (index != -1)
        return vvTable[index].getValues();

    return std::vector<std::string>();  // Empty vector if not found
}