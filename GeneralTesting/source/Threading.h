#ifdef ENABLE_THREADING
#include <boost/thread.hpp>
#include <Windows.h>

class Worker {
private:
	std::vector<int> returnVector;
	std::vector<std::vector<std::string>> returnTuple;

public:
	void runAffectsSame(boost::function<std::vector<int>()> f);
	void runAffectsDiff(boost::function<std::vector<std::vector<std::string>>()> f);
	std::vector<int> getReturnVector();
	std::vector<std::vector<std::string>> getReturnTuple();
};

class Threading {
private:
	int nThreads;
	boost::thread_group tGroup;
	boost::thread **tList;
	Worker *workers;

public:
	Threading(int nThreads = boost::thread::hardware_concurrency());
	~Threading();
	// Affects
	std::vector<int> processAffectsSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	std::vector<std::vector<std::string>> processAffectsDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	std::vector<std::vector<std::string>> processAffectsDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processAffectsDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	// Affects*
	std::vector<int> processAffectsTSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	std::vector<std::vector<std::string>> processAffectsTDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	std::vector<std::vector<std::string>> processAffectsTDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processAffectsTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	// AffectsBip
	std::vector<int> processAffectsBipSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsBipSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	std::vector<std::vector<std::string>> processAffectsBipDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	std::vector<std::vector<std::string>> processAffectsBipDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processAffectsBipDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	// AffectsBip*
	std::vector<int> processAffectsBipTSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsBipTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	std::vector<std::vector<std::string>> processAffectsBipTDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	std::vector<std::vector<std::string>> processAffectsBipTDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processAffectsBipTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	std::vector<std::string> intVecToStringVec(std::vector<int> input);
	bool join_all();
	void terminate_all(); // Not portable, windows only
};
#endif