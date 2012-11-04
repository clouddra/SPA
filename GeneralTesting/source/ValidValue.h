#ifndef STD_HEAD
#define STD_HEAD
#include "common.hpp"
#endif

// Maps a variable to the values it can take, used in query processor
// Note that values are strings
// For performance might want to change values to set type

class ValidValue {
private:
    std::string variable;
    std::vector<std::string> values;

public:
    ValidValue(std::string var, std::vector<std::string> val);
    std::string getVariable();
    std::vector<std::string> getValues();
    int restrictTo(std::vector<std::string> valid);
};