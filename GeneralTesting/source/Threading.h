#ifdef ENABLE_THREADING
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/interprocess/sync/named_semaphore.hpp>

#define SEMNAME "sem"

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
	ThreadPool threadPool;

public:
	Threading(int nThreads = boost::thread::hardware_concurrency());
	~Threading();
	// Next
	void processNextSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processNextSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	void processNextDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	void processNextDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processNextDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	// Next*
	void processNextTSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processNextTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	void processNextTDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	void processNextTDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processNextTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	// NextBip
	void processNextBipSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processNextBipSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	void processNextBipDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	void processNextBipDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processNextBipDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	// NextBip*
	void processNextBipTSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processNextBipTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	void processNextBipTDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	void processNextBipTDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processNextBipTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	// Affects
	void processAffectsSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	void processAffectsDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	void processAffectsDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processAffectsDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);
	void processAffectsPara1IsPlaceholder(std::vector<int>& result, std::vector<int>& para2Val, PKB& pkb, int i);
	void processAffectsPara2IsPlaceholder(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsPara1IsPlaceholderDriver(std::vector<int>& temp, std::vector<int>& para2Val, PKB& pkb);
	bool processAffectsPara2IsPlaceholderDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);

	// Affects*
	void processAffectsTSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	void processAffectsTDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	void processAffectsTDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processAffectsTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);
	void processAffectsTPara1IsPlaceholder(std::vector<int>& result, std::vector<int>& para2Val, PKB& pkb, int i);
	void processAffectsTPara2IsPlaceholder(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsTPara1IsPlaceholderDriver(std::vector<int>& temp, std::vector<int>& para2Val, PKB& pkb);
	bool processAffectsTPara2IsPlaceholderDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);

	// AffectsBip
	void processAffectsBipSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsBipSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	void processAffectsBipDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	void processAffectsBipDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processAffectsBipDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	// AffectsBip*
	void processAffectsBipTSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i);
	bool processAffectsBipTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb);
	void processAffectsBipTDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i);
	void processAffectsBipTDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i);
	bool processAffectsBipTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb);

	std::vector<std::string> intVecToStringVec(std::vector<int> input);
};
#endif