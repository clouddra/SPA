#ifndef TT_HEAD
#define TT_HEAD
#include "TupleTable.h"
#endif

#ifndef VV_HEAD
#define VV_HEAD
#include "ValidValue.h"
#endif

class ResultStorage {

private:
    std::vector<TupleTable> ttVector;
    std::vector<ValidValue> vvVector;
    int insertResult(std::vector<std::string> vars, std::vector<std::vector<std::string>> vals);

public:
    ResultStorage();

    /**
	 * Insert the intermediate results for 4 variable into ResultStorage
	 */
    int insertResult(std::string var1, std::string var2, std::string var3, std::string var4, std::vector<std::vector<std::string>> vals);

    /**
	 * Insert the intermediate results for 3 variable into ResultStorage
	 */
    int insertResult(std::string var1, std::string var2, std::string var3, std::vector<std::vector<std::string>> vals);

    /**
	 * Insert the intermediate results for 2 variable into ResultStorage
	 */
    int insertResult(std::string var1, std::string var2, std::vector<std::vector<std::string>> vals);

    /**
	 * Insert the intermediate results for 1 variable into ResultStorage
	 */
    int insertResult(std::string var, std::vector<std::string> vals);

    /**
	 * Get values for 1 variable
	 */
    std::vector<std::string> getValuesFor(std::string var);

    /**
	 * Get tuple values for muliple variables
	 */
    std::vector<std::vector<std::string>> getValuesFor(std::vector<std::string> vars);

    /**
	 * Getter function for the vvVector
	 */
    std::vector<ValidValue> getVVVector();
};