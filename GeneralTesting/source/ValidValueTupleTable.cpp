#ifndef VVTUPLETABLE_HEAD
#define VVTUPLETABLE_HEAD
#include "ValidValueTupleTable.h"
#endif

int ValidValueTupleTable::findVariable(std::string var1, std::string var2) {
    for (int i = 0; i < (int)vvTupleTable.size(); i++) {
        if (vvTupleTable[i].getVariable1().compare(var1) == 0)
        {
            if (vvTupleTable[i].getVariable2().compare(var2) == 0)
                return i;
        }
        else if (vvTupleTable[i].getVariable1().compare(var2) == 0)
        {
            if (vvTupleTable[i].getVariable2().compare(var1) == 0)
                return i;
        }
    }
    return -1;
}

int ValidValueTupleTable::insert(std::string var1, std::string var2, std::vector<std::pair<std::string,std::string>> val) {
    if ((int)val.size() == 0) {
        // std::cout << var1 << " and " << var2 << " have no valid values\n"; 
        return -1;
    }
    int index = findVariable(var1, var2);
    if (index == -1) {
        vvTupleTable.push_back(ValidValueTuple(var1, var2, val));
    }
    else {
        ValidValueTuple temp = ValidValueTuple(var1, var2, val);
        return vvTupleTable[index].restrictTo(temp, -1);
    }
    return 0;
}

std::vector<std::string> ValidValueTupleTable::getValues(std::string variable) {
    std::vector<std::string> temp;
    for (int i = 0; i < (int)vvTupleTable.size(); i++) {
        temp = vvTupleTable[i].getValuesForVar(variable);
        if (temp.size() != 0)
        {
            return temp;
        }
    }
    return temp;  // Empty vector if not found
}

int ValidValueTupleTable::restrictTo(ValidValueTable vvTable) {
    int ret;
    for (int i = 0; i < (int)vvTupleTable.size(); i++) {
        ret = vvTupleTable[i].restrictTo(vvTable);
        if (ret == -1)
            return -1;
    }
    return 0;
}

int ValidValueTupleTable::reconcile() {
    std::set<int> notFinalized;
    for (int i = 0; i < (int)vvTupleTable.size(); i++) {
        notFinalized.insert(i);
        for (int j = 0; j < (int)vvTupleTable.size(); j++) {
            if (i != j) {
                int ret = vvTupleTable[i].restrictTo(vvTupleTable[j], j);
                if (ret == -1) 
                    return -1;
            }
        }
    }

    while ((int)notFinalized.size() > 0) {
        int curr = *notFinalized.begin();
        notFinalized.erase(notFinalized.begin());
        std::vector<int> temp = vvTupleTable[curr].getLinked();
        for (int i = 0; i < (int)temp.size(); i++) {
            int ret = vvTupleTable[curr].restrictTo(vvTupleTable[temp[i]], temp[i]);
            if (ret == -1) 
                return -1;
            if (ret == 1) {
                for (int j = 0; j < (int)temp.size(); j++) {
                    if (i != j)
                        notFinalized.insert(temp[j]);
                }
            }
        }
    }
    return 0;
}