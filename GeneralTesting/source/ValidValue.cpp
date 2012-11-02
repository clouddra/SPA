#ifndef VV_HEAD
#define VV_HEAD
#include "ValidValue.h"
#endif

ValidValue::ValidValue(std::string var, std::vector<std::string> val) {
    variable = var;
    values = val;
}

std::string ValidValue::getVariable() {
    return variable;
}

std::vector<std::string> ValidValue::getValues() {
    return values;
}

// Does an intersection between valid and values, storing the result back in values
// A return value of -1 indicates all values have been deleted (No valid value for the parameter = query evaluates to false)
int ValidValue::restrictTo(std::vector<std::string> valid) {  
    std::vector<int> toDelete;
    for (int i = 0; i < (int)values.size(); i++) {
        bool found = false;
        for (int j = 0; j < (int)valid.size(); j++) {
            if (values[i].compare(valid[j]) == 0) {
                found = true;
                break;
            }
        }
        if (!found)
            toDelete.push_back(i);
    }

    for (int i = toDelete.size()-1; i > -1; i--) {
        values.erase(values.begin() + toDelete[i]);
    }

    if (values.size() == 0)
        return -1;
    return 0;
}