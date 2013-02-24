#ifndef VV_HEAD
#define VV_HEAD
#include "ValidValue.h"
#endif

ValidValue::ValidValue(std::string var, std::set<std::string> val) {
    variable = var;
    values = val;
}

std::string ValidValue::getVariable() {
    return variable;
}

std::set<std::string> ValidValue::getValues() {
    return values;
}

// Straight swap, intersection no longer done here
int ValidValue::restrictTo(std::string var, std::set<std::string> valid) { 
    if (variable.compare(var) != 0)
        return -1;

    values = valid;
    return 0;
}