#ifndef TT_HEAD
#define TT_HEAD
#include "TupleTable.h"
#endif

TupleTable::TupleTable(std::vector<std::string> vars, std::vector<std::vector<std::string>> vals) {
    variables = vars;
    values = vals; 
}

std::vector<std::string> TupleTable::getVars() {
    return variables;
}

int TupleTable::merge(TupleTable other) {
    int selfMatch1, otherMatch1;
    int selfMatch2, otherMatch2;
    bool match1Found = false;
    bool match2Found = false;
    int i, j, k;

    for (i = 0; i < (int)variables.size(); i++) {
        for (j = 0; j < (int)other.variables.size(); j++) {
            if (variables[i].compare(other.variables[j]) == 0) {
                if (!match1Found) {
                    selfMatch1 = i;
                    otherMatch1 = j;
                    match1Found = true;
                }
                else if (!match2Found) {
                    selfMatch2 = i;
                    otherMatch2 = j;
                    match2Found = true;
                }
                else {
                    // Intersection of more than 2 variables, error
                    return -1;
                }
                break;
            }
        }
    }

    std::vector<std::vector<std::string>> newValues;
    if (match2Found) {
        for (i = 0; i < (int)other.variables.size(); i++) {
            if (i != otherMatch1 && i != otherMatch2) {
                variables.push_back(other.variables[i]);
            }
        }

        for (i = 0; i < (int)values.size(); i++) {
            for (j = 0; j < (int)other.values.size(); j++) {
                if (values[i][selfMatch1].compare(other.values[j][otherMatch1]) == 0) {
                    if (values[i][selfMatch2].compare(other.values[j][otherMatch2]) == 0) {
                        std::vector<std::string> temp = values[i];
                        for (k = 0; k < (int)other.variables.size(); k++) {
                            if (k != otherMatch1 && k != otherMatch2) {
                                temp.push_back(other.values[j][k]);
                            }
                        }
                        newValues.push_back(temp);
                    }
                }
            }
        }
        values = newValues;
    }
    else if (match1Found) {
        for (i = 0; i < (int)other.variables.size(); i++) {
            if (i != otherMatch1) {
                variables.push_back(other.variables[i]);
            }
        }

        for (i = 0; i < (int)values.size(); i++) {
            for (j = 0; j < (int)other.values.size(); j++) {
                if (values[i][selfMatch1].compare(other.values[j][otherMatch1]) == 0) {
                    std::vector<std::string> temp = values[i];
                    for (k = 0; k < (int)other.variables.size(); k++) {
                        if (k != otherMatch1) {
                            temp.push_back(other.values[j][k]);
                        }
                    }
                    newValues.push_back(temp);
                }
            }
        }
        values = newValues;
    }
    else {
        return -2; // No intersections, return -2 as this is not a exiting error
    }

    if (values.size() == 0) {
        return -1; // No values remaining
    }
    return 0;
}

std::set<std::string> TupleTable::getValuesFor(std::string var) {
    std::set<std::string> ret;
    int match = -1;

    for (int i = 0; i < (int)variables.size(); i++) {
        if (variables[i].compare(var) == 0) {
            match = i;
            break;
        }
    }

    if (match != -1) {
        for (int i = 0; i < (int)values.size(); i++) {
            ret.insert(values[i][match]);
        }
    }
    return ret;
}