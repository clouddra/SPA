#ifndef TESTWRAPPER_H
#define TESTWRAPPER_H

#ifndef IOSTREAM_HEAD
#define IOSTREAM_HEAD
#include <iostream>
#endif

#ifndef STRING_HEAD
#define STRING_HEAD
#include <string>
#endif

#include <list>

// include your other headers here
#include "AbstractWrapper.h"

#ifndef CONTROLLER_HEAD
#define CONTROLLER_HEAD
#include "SPAController.h"
#endif

class TestWrapper : public AbstractWrapper {

protected:
	SPAController controller ;

public:
  // default constructor
  TestWrapper();
  
  // destructor
  ~TestWrapper();
  
  // method for parsing the SIMPLE source
  virtual void parse(std::string filename);
  
  // method for evaluating a query
  virtual void evaluate(std::string query, std::list<std::string>& results);
};

#endif
