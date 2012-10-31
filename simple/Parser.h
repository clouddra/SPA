#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

class Parser {

private:

public:
    Parser();
    int parseCode(char const* filename, PKB* pkb);
};