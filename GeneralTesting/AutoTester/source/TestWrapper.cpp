#include "TestWrapper.h"
// #include "HelperFunctions.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
	// create any objects here as instance variables of this class
	// as well as any initialization required for your spa program
	//PKB pkb = PKB();
	controller = SPAController() ;	
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	// call your parser to do the parsing
	std::ifstream in(filename, std::ios_base::in);
    std::string program; 
    in.unsetf(std::ios::skipws); 
    std::copy(
        std::istream_iterator<char>(in),
        std::istream_iterator<char>(),
        std::back_inserter(program));
	// std::cout << program << std::endl; // uncomment to print out program
	/*
	Parser temp = Parser();
	temp.parseCode(program, &pkb);
	DesignExtractor de = DesignExtractor(&pkb);
	de.populateTables();
	*/
	controller.parseSource(program) ;
	


}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string>& results){
// call your evaluator to evaluate the query here
	// ...code to evaluate query...
	
	// store the answers to the query in the results list (it is initially empty)
	// each result must be a string.
	/*
    QueryProcessor qp = QueryProcessor();
    PqlParser temp = PqlParser();
    temp.parseQuery(query, &qp);
    qp.processQuery(pkb);
    results = qp.getResult();
	*/

	results = controller.evaluateQuery(query);

}
