#ifndef PQLSPLIT_HEAD
#define PQLSPLIT_HEAD
#include "PqlSplitter.h"
#endif

// Splits input into multiple pql queries, appending the declaration to each of them
// No delimiter needed (Delimiter is "Select", IT IS CASE SENSITIVE)
std::vector<std::string> PqlSplitter::splitQuery(std::string input) {
    std::vector<std::string> queries;
    std::string declaration;
    std::string sel = "Select";
    int found = 0;
    
    found = input.find(sel);
    if (found != std::string::npos)
    {
        declaration = input.substr(0, found);
        input = input.substr(found);
        while (found != std::string::npos) {
            found = input.find(sel, 1);
            std::string query;
            if (found != std::string::npos) {
                query = declaration + input.substr(0, found);
                input = input.substr(found);
            }
            else
                query = declaration + input;
            queries.push_back(query);
        }
    }

    return queries;
}