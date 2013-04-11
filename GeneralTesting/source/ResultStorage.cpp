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

int ResultStorage::insertResult(std::string var1, std::string var2, std::string var3, std::vector<std::vector<std::string>> vals) {
    int ret;
    std::vector<std::string> tempVars;
    std::vector<std::vector<std::string>> tempVals;
    tempVars.push_back(var1);
    tempVars.push_back(var2);
    tempVars.push_back(var3);
    TupleTable tempTT = TupleTable(tempVars, vals);

    tempVars.clear();
    tempVars.push_back(var1);
    tempVars.push_back(var2);
    tempVals = tempTT.getValuesFor(tempVars);
    ret = insertResult(tempVars, tempVals);
    if (ret == -1)
        return -1;

    tempVars.clear();
    tempVars.push_back(var2);
    tempVars.push_back(var3);
    tempVals = tempTT.getValuesFor(tempVars);
    ret = insertResult(tempVars, tempVals);
    if (ret == -1)
        return -1;

    tempVars.clear();
    tempVars.push_back(var1);
    tempVars.push_back(var3);
    tempVals = tempTT.getValuesFor(tempVars);
    ret = insertResult(tempVars, tempVals);
    if (ret == -1)
        return -1;

    return 0;
}

int ResultStorage::insertResult(std::string var1, std::string var2, std::string var3, std::string var4, std::vector<std::vector<std::string>> vals) {
    int ret;
    std::vector<std::string> tempVars;
    std::vector<std::vector<std::string>> tempVals;
    tempVars.push_back(var1);
    tempVars.push_back(var2);
    tempVars.push_back(var3);
    tempVars.push_back(var4);
    TupleTable tempTT = TupleTable(tempVars, vals);

    tempVars.clear();
    tempVars.push_back(var1);
    tempVars.push_back(var2);
    tempVals = tempTT.getValuesFor(tempVars);
    ret = insertResult(tempVars, tempVals);
    if (ret == -1)
        return -1;

    tempVars.clear();
    tempVars.push_back(var1);
    tempVars.push_back(var3);
    tempVals = tempTT.getValuesFor(tempVars);
    ret = insertResult(tempVars, tempVals);
    if (ret == -1)
        return -1;

    tempVars.clear();
    tempVars.push_back(var1);
    tempVars.push_back(var4);
    tempVals = tempTT.getValuesFor(tempVars);
    ret = insertResult(tempVars, tempVals);
    if (ret == -1)
        return -1;

    tempVars.clear();
    tempVars.push_back(var2);
    tempVars.push_back(var3);
    tempVals = tempTT.getValuesFor(tempVars);
    ret = insertResult(tempVars, tempVals);
    if (ret == -1)
        return -1;

    tempVars.clear();
    tempVars.push_back(var2);
    tempVars.push_back(var4);
    tempVals = tempTT.getValuesFor(tempVars);
    ret = insertResult(tempVars, tempVals);
    if (ret == -1)
        return -1;

    tempVars.clear();
    tempVars.push_back(var3);
    tempVars.push_back(var4);
    tempVals = tempTT.getValuesFor(tempVars);
    ret = insertResult(tempVars, tempVals);
    if (ret == -1)
        return -1;

    return 0;
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

std::vector<std::vector<std::string>> ResultStorage::getValuesFor(std::vector<std::string> vars) {
    std::vector<std::vector<std::string>> temp;
    std::vector<std::vector<std::vector<std::string>>> intResults;
    std::vector<int> order;

    for (int i = 0; i < (int)ttVector.size(); i++) {
        std::vector<std::string> match;
        std::vector<std::string> tempVec = ttVector[i].getVars();
        for (int j = 0; j < (int)vars.size(); j++) {
            for (int k = 0; k < (int)tempVec.size(); k++) {
                if (vars[j].compare(tempVec[k]) == 0) {
                    match.push_back(vars[j]);
                    order.push_back(j);
                    break;
                }
            }
        }
        if (match.size() > 0)
            intResults.push_back(ttVector[i].getValuesFor(match));
    }
    if (intResults.size() == 0) 
        return temp;

    while ((int)intResults.size() > 1) {
        temp.clear();
        for (int i = 0; i < (int)intResults[0].size(); i++) {
            for (int j = 0; j < (int)intResults[1].size(); j++) {
                std::vector<std::string> tempVec = intResults[0][i];
                for (int k = 0; k < (int)intResults[1][j].size(); k++) {
                    tempVec.push_back(intResults[1][j][k]);
                }
                temp.push_back(tempVec);
            }
        }
        intResults[0] = temp;
        intResults.erase(intResults.begin()+1);
    }

    std::vector<std::vector<std::string>> toRet;
    for (int i = 0; i < (int)intResults[0].size(); i++) {
        std::vector<std::string> holder;
        holder.resize((int)vars.size());
        for (int j = 0; j < (int)order.size(); j++) {
            holder[order[j]] = intResults[0][i][j];
        }
        toRet.push_back(holder);
    }
    return toRet;
}

std::vector<ValidValue> ResultStorage::getVVVector() {
    return vvVector;
}