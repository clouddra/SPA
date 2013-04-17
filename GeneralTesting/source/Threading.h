#ifdef ENABLE_THREADING
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>
#include <Windows.h>

/*
class Worker {
private:
	std::vector<int> returnVector;
	std::vector<std::vector<std::string>> returnTuple;

public:
	void runNextSame(boost::function<std::vector<int>()> f);
	void runNextDiff(boost::function<std::vector<std::vector<std::string>>()> f);
	void runAffectsSame(boost::function<std::vector<int>()> f);
	void runAffectsDiff(boost::function<std::vector<std::vector<std::string>>()> f);
	std::vector<int> getReturnVector();
	std::vector<std::vector<std::string>> getReturnTuple();
};
*/
class Worker {
public:
    Worker(ThreadPool &s) : pool(s) { }
    void operator()();
private:
    ThreadPool &pool; 
};

class Threading {
private:
	int nThreads;
	boost::thread_group tGroup;
	boost::thread **tList;
	Worker *workers;

public:
	Threading(int nThreads = boost::thread::hardware_concurrency()*4);
	~Threading();
	// Next
	std::vector<int> processNextSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i);
	bool processNextSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	std::vector<std::vector<std::string>> processNextDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	std::vector<std::vector<std::string>> processNextDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processNextDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	// Next*
	std::vector<int> processNextTSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i);
	bool processNextTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	std::vector<std::vector<std::string>> processNextTDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	std::vector<std::vector<std::string>> processNextTDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processNextTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

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

class ThreadPool {
public:
    ThreadPool(int nThreads);
    template<class F>
		void enqueue(F f);
    ~ThreadPool();

private:
    std::vector<std::unique_ptr<boost::thread>> workers;
    boost::asio::io_service service;
    std::unique_ptr<boost::asio::io_service::work> work;
    friend class Worker;
};
#endif