#ifndef VVTUPLE_HEAD
#define VVTUPLE_HEAD
#include "ValidValueTuple.h"
#endif

#include <set>

ValidValueTuple::ValidValueTuple() {

}

ValidValueTuple::ValidValueTuple(std::string var1, std::string var2, std::vector<std::pair<std::string,std::string>> val) {
    variable1 = var1;
    variable2 = var2;
    values = val;
}

std::string ValidValueTuple::getVariable1() {
    return variable1;
}

std::string ValidValueTuple::getVariable2() {
    return variable2;
}

std::vector<std::string> ValidValueTuple::getValuesForVar(std::string target) {
    std::vector<std::string> ret;
    std::set<std::string> temp;

    if (target.compare(variable1) == 0) {
        for (int i = 0; i < (int)values.size(); i++) {
            temp.insert(values[i].first);
        }
        std::set<std::string>::iterator it;
        for (it = temp.begin() ; it != temp.end(); it++ ) {
            ret.push_back(*it);
        }
    }
    else if (target.compare(variable2) == 0) {
        for (int i = 0; i < (int)values.size(); i++) {
            temp.insert(values[i].second);
        }
        std::set<std::string>::iterator it;
        for (it = temp.begin() ; it != temp.end(); it++ ) {
            ret.push_back(*it);
        }
    }
    return ret;
}

int ValidValueTuple::restrictTo(std::string inputVar, std::vector<std::string> inputVal) {
    std::vector<int> toDelete;
    if (inputVar.compare(variable1) == 0) {
        for (int i = 0; i < (int)values.size(); i++) {
            bool found = false;
            for (int j = 0; j < (int)inputVal.size(); j++) {
                if (values[i].first.compare(inputVal[j]) == 0) {
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
    }
    else if (inputVar.compare(variable2) == 0) {
        for (int i = 0; i < (int)values.size(); i++) {
            bool found = false;
            for (int j = 0; j < (int)inputVal.size(); j++) {
                if (values[i].second.compare(inputVal[j]) == 0) {
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
    }
    if (values.size() == 0) {
        std::cout << variable1 << " and " << variable2 << " have no valid values\n";
        return -1;
    }
    return 0;
}

int ValidValueTuple::restrictTo(ValidValueTable vvTable) {
    std::vector<std::string> valid1 = vvTable.getValues(variable1);
    std::vector<std::string> valid2 = vvTable.getValues(variable2);

    if (valid1.size() != 0) {
        int ret = restrictTo(variable1, valid1);
        if (ret == -1)
            return -1;
    }
    if (valid2.size() != 0) {
        int ret = restrictTo(variable2, valid2);
        if (ret == -1)
            return -1;
    }
    return 0;
}