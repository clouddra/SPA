#ifndef PARSER_HEAD
#define PARSER_HEAD
#include "Parser.h"
#endif

#ifndef PQLPARSER_HEAD
#define PQLPARSER_HEAD
#include "PqlParser.h"
#endif

#ifndef DE_HEAD
#define DE_HEAD
#include "DesignExtractor.h"
#endif

#ifndef STRING_HEAD
#define STRING_HEAD
#include <string>
#endif

#include <iostream>
#include <list>

class SPAController {

	private:
	QueryProcessor qp;
	PQLParser pqlParser; 
	DesignExtractor de;
	Parser codeParser ;
	PKB pkb;

	public:

	SPAController();	
	PKB parseSource(std::string program);
	std::list<std::string> evaluateQuery(std::string query);
	std::string runSPA(std::string program);

		  
				/*
		std::ifstream in(filename, std::ios_base::in);
    std::string program; 
    in.unsetf(std::ios::skipws); 
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(program));
	// std::cout << program << std::endl; // uncomment to print out program
	Parser temp = Parser();
	temp.parseCode(program, &pkb);
	DesignExtractor de = DesignExtractor(&pkb);
	de.populateTables();

	    QueryProcessor qp = QueryProcessor();
    PqlParser temp = PqlParser();
    temp.parseQuery(query, &qp);
    qp.processQuery(pkb);
    results = qp.getResult();

		PKB pkb = PKB();
		*/
};