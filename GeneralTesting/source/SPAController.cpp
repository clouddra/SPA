#ifndef CONTROLLER_HEAD
#define CONTROLLER_HEAD
#include "SPAController.h"
#endif

SPAController::SPAController()
{
	qp = QueryProcessor(); 
	pqlParser = PqlParser();
	codeParser = Parser() ;
	pkb  = PKB();

}

PKB SPAController::parseSource(std::string program) {

	codeParser.parseCode(program, &pkb);
	de = DesignExtractor(&pkb);
	de.populateTables();
	return pkb;
}

std::list<std::string> SPAController::evaluateQuery(std::string query){
	qp = QueryProcessor();  // Reset qp to empty for next query
	pqlParser.parseQuery(query, &qp);
    qp.processQuery(pkb);
    return qp.getResult();
}
	

std::string SPAController::runSPA(std::string program) {
	return "lol" ;
}
