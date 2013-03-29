#ifndef CONTROLLER_HEAD
#define CONTROLLER_HEAD
#include "SPAController.h"
#endif

SPAController::SPAController()
{
	qp = QueryProcessor(); 
	pqlParser = PQLParser();
	codeParser = Parser() ;
	pkb  = PKB();

}

PKB SPAController::parseSource(std::string program) {

	codeParser.parseCode(program, &pkb);
	de = DesignExtractor(&pkb);
	de.populateTables();
    pkb.startBuildCfg();
	pkb.startBuildCfgBip();
	return pkb;
}

std::list<std::string> SPAController::evaluateQuery(std::string query){
	qp = QueryProcessor();  // Reset qp to empty for next query
	int temp = pqlParser.parseQuery(query, &qp);
    if (temp == -1)
        return std::list<std::string> ();
    qp.processQuery(pkb);
    return qp.getResult();
}

std::string SPAController::runSPA(std::string program) {
	return "lol" ;
}
