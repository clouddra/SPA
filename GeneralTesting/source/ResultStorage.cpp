#ifndef RS_HEAD
#define RS_HEAD
#include "ResultStorage.h"
#endif

ResultStorage::ResultStorage() {

}

int ResultStorage::insertResult(std::string var, std::vector<std::string> vals) {
    std::vector<std::string> tempVars;
    tempVars.push_back(var);
    std::vector<std::vector<std::string>> tempVals;
    for (int i = 0; i < (int)vals.size(); i++) {
        std::vector<std::string> tempVec;
        tempVec.push_back(vals[i]);
        tempVals.push_back(tempVec);
    }
    return insertResult(tempVars, tempVals);
}

int ResultStorage::insertResult(std::string var1, std::string var2, std::vector<std::vector<std::string>> vals) {
    std::vector<std::string> tempVars;
    tempVars.push_back(var1);
    tempVars.push_back(var2);
    return insertResult(tempVars, vals);
}

int ResultStorage::insertResult(std::vector<std::string> vars, std::vector<std::vector<std::string>> vals) {
    TupleTable tempTT = TupleTable(vars, vals);
    int ret, merge1 = -1, toDelete = -1;

    for (int i = 0; i < (int)ttVector.size(); i++) {
        ret = ttVector[i].merge(tempTT);
        if (ret == -1) {
            return -1;
        }
        else if (ret == 0) {
            if (toDelete != -1) {
                return -1; // Something went wrong
            }

            if (merge1 == -1) {
                merge1 = i;
            }
            else {
                toDelete = i;
                ret = ttVector[merge1].merge(ttVector[i]);
                if (ret == -1) {
                    return -1;
                }
                if (ret == -2) {
                    return -1; // Something went wrong
                }
            }

            // Update vvVector
            std::vector<std::string> vars = ttVector[merge1].getVars();
            for (int j = 0; j < (int)vars.size(); j++) {
                ret = -1;
                std::set<std::string> vals = ttVector[merge1].getValuesFor(vars[j]);
                for (int k = 0; k < (int)vvVector.size(); k++) {
                    ret = vvVector[k].restrictTo(vars[j], vals);
                    if (ret == 0)
                        break;
                }
                if (ret != 0) {
                    ValidValue temp = ValidValue(vars[j], vals);
                    vvVector.push_back(temp);
                }
            }
        }
    }
    if (toDelete != -1)
        ttVector.erase(ttVector.begin() + toDelete);
    if (merge1 == -1) {
        // No merge occured, new TupleTable
        ttVector.push_back(tempTT);
        
        // Update vvVector
        std::vector<std::string> vars = tempTT.getVars();
        for (int j = 0; j < (int)vars.size(); j++) {
            std::set<std::string> vals = tempTT.getValuesFor(vars[j]);
            ValidValue temp = ValidValue(vars[j], vals);
            vvVector.push_back(temp);
        }
    }

    return 0;
}

std::vector<std::string> ResultStorage::getValuesFor(std::string var) {
    std::vector<std::string> ret;
    for (int i = 0; i < (int)vvVector.size(); i++) {
        if (vvVector[i].getVariable().compare(var) == 0) {
            std::set<std::string> tempSet = vvVector[i].getValues(); 
            ret = std::vector<std::string> (tempSet.begin(), tempSet.end());
            return ret;
        }
    }
    return ret;
}