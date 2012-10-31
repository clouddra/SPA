#ifndef PKB_HEAD
#define PKB_HEAD
#include "PKB.h"
#endif

class SPA {

private:
    PKB pkb;

public:
    SPA();
    int parseCode(char const*);
};