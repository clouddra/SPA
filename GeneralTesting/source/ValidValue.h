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
    /**
	 * Constructor with data members intialised to the given values
	 */
    ValidValue(std::string var, std::set<std::string> val);

    /**
	 * Getter function for variable
	 */
    std::string getVariable();

    /**
	 * Getter function for values set
	 */
    std::set<std::string> getValues();

    /**
	 * If parameter var matches variable, do a overwrite of the values set with the given set
	 */
    int restrictTo(std::string var, std::set<std::string> valid);
};