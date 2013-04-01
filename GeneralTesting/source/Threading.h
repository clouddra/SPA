#ifdef ENABLE_THREADING
#include <boost/thread.hpp>

class Worker {
private:
	std::vector<int> returnVector;

public:
	void run(boost::function<std::vector<int>()> f);
	std::vector<int> getReturnVector();
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
	std::vector<int> processAffectsStart(std::vector<int>& para1Val, PKB pkb, int i);
	void processAffectsSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB pkb);
};
#endif