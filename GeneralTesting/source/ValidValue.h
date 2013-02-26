#ifndef STD_HEAD
#define STD_HEAD
#include "Common.hpp"
#endif

// Maps a variable to the values it can take, used in query processor
// Note that values are strings

class ValidValue {
private:
    std::string variable;
    std::set<std::string> values;

public:
    ValidValue(std::string var, std::set<std::string> val);
    std::string getVariable();
    std::set<std::string> getValues();
    int restrictTo(std::string var, std::set<std::string> valid);
};