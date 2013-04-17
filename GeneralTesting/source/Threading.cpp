#pragma once
#ifdef ENABLE_THREADING

#include "QueryProcessor.h"

void Worker::operator()() { pool.service.run(); }
 
ThreadPool::ThreadPool(int nThreads) : work(new boost::asio::io_service::work(service))
{
    for(int i = 0;i<nThreads; i++)
        workers.push_back(
            std::unique_ptr<boost::thread>(
                new boost::thread(Worker(*this))
            )
        );
}

template<class F>
void ThreadPool::enqueue(F f)
{
    service.post(f);
}

ThreadPool::~ThreadPool()
{
	work.reset();
    for(size_t i = 0;i<workers.size();++i)
        workers[i]->join();
}

Threading::Threading(int nThreads) : threadPool(nThreads) {
	this->nThreads = nThreads;
}

Threading::~Threading() {

}

///////////////////////////////////////
// [Next

// Next(s1, s1)
void Threading::processNextSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i) {
    if (pkb.isNext(para1Val[i], para1Val[i])) {
        result.push_back(i);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processNextSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
	boost::interprocess::named_semaphore::remove(SEMNAME);
	boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
	std::vector<std::vector<int>> results((int)para1Val.size());
    for (int i = 0; i < (int)para1Val.size(); i++) {
		boost::function<void()> f = boost::bind(&Threading::processNextSameVarStart, this, boost::ref(results.at(i)), para1Val, boost::ref(pkb), i);
		this->threadPool.enqueue(f);
    }
	for (int i = 0; i < (int)para1Val.size(); i++) {
		sem.wait();
	}
	for (int i=0; i<(int)para1Val.size(); i++) {
		for (std::vector<int>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
			temp.push_back(*it);
		}
	}
	boost::interprocess::named_semaphore::remove(SEMNAME);
	return true;
}

// Next(s1, s2) from start
void Threading::processNextDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	temp = pkb.getNext(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]); 
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

// Next(s1, s2) from end
void Threading::processNextDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
    temp = pkb.getPrev(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processNextDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para1ValInt.size());
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processNextDiffVarStart, this, boost::ref(results.at(i)), para1ValString, para1ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para1ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
    else {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para2ValInt.size());
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processNextDiffVarEnd, this, boost::ref(results.at(i)), para2ValString, para2ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para2ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
}

// Next]
////////////////////////////////////////////

///////////////////////////////////////
// [Next*

// Next*(s1, s1)
void Threading::processNextTSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i) {
	bool found = false;
	std::vector<int> temp2 = pkb.getNextTAPI(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (para1Val[i] == temp2[j]) {
            found = true;
            break;
        }
    }
    if (found)
        result.push_back(para1Val[i]);
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processNextTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
	boost::interprocess::named_semaphore::remove(SEMNAME);
	boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
	std::vector<std::vector<int>> results((int)para1Val.size());
    for (int i = 0; i < (int)para1Val.size(); i++) {
		boost::function<void()> f = boost::bind(&Threading::processNextTSameVarStart,  this, boost::ref(results.at(i)), para1Val, boost::ref(pkb), i);
		this->threadPool.enqueue(f);
    }
	for (int i = 0; i < (int)para1Val.size(); i++) {
		sem.wait();
	}
	for (int i=0; i<(int)para1Val.size(); i++) {
		for (std::vector<int>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
			temp.push_back(*it);
		}
	}
	boost::interprocess::named_semaphore::remove(SEMNAME);
	return true;
}

// Next*(s1, s2) from start
void Threading::processNextTDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	temp = pkb.getNextTAPI(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]); 
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

// Next*(s1, s2) from end
void Threading::processNextTDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
    temp = pkb.getPrevTAPI(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processNextTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para1ValInt.size());
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processNextTDiffVarStart, this, boost::ref(results.at(i)), para1ValString, para1ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para1ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
    else {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para2ValInt.size());
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processNextTDiffVarEnd, this, boost::ref(results.at(i)), para2ValString, para2ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para2ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
}

// Next*]
////////////////////////////////////////////

///////////////////////////////////////
// [Affects

// Affects(a1, a1)
void Threading::processAffectsSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i) {
	std::vector<int> temp2;
    temp2 = pkb.getAffectsStartAPI(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();	
}

bool Threading::processAffectsSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
	boost::interprocess::named_semaphore::remove(SEMNAME);
	boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
	std::vector<std::vector<int>> results((int)para1Val.size());
    for (int i = 0; i < (int)para1Val.size(); i++) {
		boost::function<void()> f = boost::bind(&Threading::processAffectsSameVarStart, this, boost::ref(results.at(i)), para1Val, boost::ref(pkb), i);
		this->threadPool.enqueue(f);
    }
	for (int i = 0; i < (int)para1Val.size(); i++) {
		sem.wait();
	}
	for (int i=0; i<(int)para1Val.size(); i++) {
		for (std::vector<int>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
			temp.push_back(*it);
		}
	}
	boost::interprocess::named_semaphore::remove(SEMNAME);
	return true;
}

// Affects(a1, a2) from start
void Threading::processAffectsDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	temp = pkb.getAffectsStartAPI(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]); 
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

// Affects(a1, a2) from end
void Threading::processAffectsDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
    temp = pkb.getAffectsEndAPI(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processAffectsDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para1ValInt.size());
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processAffectsDiffVarStart, this, boost::ref(results.at(i)), para1ValString, para1ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para1ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
    else {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para2ValInt.size());
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processAffectsDiffVarEnd, this, boost::ref(results.at(i)), para2ValString, para2ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para2ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
}

// Affects]
////////////////////////////////////////////

////////////////////////////////////////////
// [Affects*

// Affects*(a1, a1)
void Threading::processAffectsTSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i) {
	std::vector<int> temp2;
    temp2 = pkb.getAffectsTStartAPI(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processAffectsTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
	boost::interprocess::named_semaphore::remove(SEMNAME);
	boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
	std::vector<std::vector<int>> results((int)para1Val.size());
    for (int i = 0; i < (int)para1Val.size(); i++) {
		boost::function<void()> f = boost::bind(&Threading::processAffectsTSameVarStart, this, boost::ref(results.at(i)), para1Val, boost::ref(pkb), i);
		this->threadPool.enqueue(f);
    }
	for (int i = 0; i < (int)para1Val.size(); i++) {
		sem.wait();
	}
	for (int i=0; i<(int)para1Val.size(); i++) {
		for (std::vector<int>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
			temp.push_back(*it);
		}
	}
	boost::interprocess::named_semaphore::remove(SEMNAME);
	return true;
}

// Affects*(a1, a2) from start
void Threading::processAffectsTDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	temp = pkb.getAffectsTStartAPI(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]); 
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

// Affects*(a1, a2) from end
void Threading::processAffectsTDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
    temp = pkb.getAffectsTEndAPI(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processAffectsTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para1ValInt.size());
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processAffectsTDiffVarStart, this, boost::ref(results.at(i)), para1ValString, para1ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para1ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
    else {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para2ValInt.size());
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processAffectsTDiffVarEnd, this, boost::ref(results.at(i)), para2ValString, para2ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para2ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
}
// Affects*]
////////////////////////////////////////////


///////////////////////////////////////
// [AffectsBip

// AffectsBip(a1, a1)
void Threading::processAffectsBipSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i) {
	std::vector<int> temp2;
    temp2 = pkb.getAffectsBipStartAPI(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processAffectsBipSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
	boost::interprocess::named_semaphore::remove(SEMNAME);
	boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
	std::vector<std::vector<int>> results((int)para1Val.size());
    for (int i = 0; i < (int)para1Val.size(); i++) {
		boost::function<void()> f = boost::bind(&Threading::processAffectsBipSameVarStart, this, boost::ref(results.at(i)), para1Val, boost::ref(pkb), i);
		this->threadPool.enqueue(f);
    }
	for (int i = 0; i < (int)para1Val.size(); i++) {
		sem.wait();
	}
	for (int i=0; i<(int)para1Val.size(); i++) {
		for (std::vector<int>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
			temp.push_back(*it);
		}
	}
	boost::interprocess::named_semaphore::remove(SEMNAME);
	return true;
}

// AffectsBip(a1, a2) from start
void Threading::processAffectsBipDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	temp = pkb.getAffectsBipStartAPI(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]); 
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

// AffectsBip(a1, a2) from end
void Threading::processAffectsBipDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
    temp = pkb.getAffectsBipEndAPI(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processAffectsBipDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para1ValInt.size());
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processAffectsBipDiffVarStart, this, boost::ref(results.at(i)), para1ValString, para1ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para1ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
    else {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para2ValInt.size());
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processAffectsBipDiffVarEnd, this, boost::ref(results.at(i)), para2ValString, para2ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para2ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
}

// AffectsBip]
////////////////////////////////////////////

////////////////////////////////////////////
// [AffectsBip*

// AffectsBip*(a1, a1)
void Threading::processAffectsBipTSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i) {
	std::vector<int> temp2;
    temp2 = pkb.getAffectsBipTStartAPI(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processAffectsBipTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
	boost::interprocess::named_semaphore::remove(SEMNAME);
	boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
	std::vector<std::vector<int>> results((int)para1Val.size());
    for (int i = 0; i < (int)para1Val.size(); i++) {
		boost::function<void()> f = boost::bind(&Threading::processAffectsBipTSameVarStart, this, boost::ref(results.at(i)), para1Val, boost::ref(pkb), i);
		this->threadPool.enqueue(f);
    }
	for (int i = 0; i < (int)para1Val.size(); i++) {
		sem.wait();
	}
	for (int i=0; i<(int)para1Val.size(); i++) {
		for (std::vector<int>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
			temp.push_back(*it);
		}
	}
	boost::interprocess::named_semaphore::remove(SEMNAME);
	return true;
}

// AffectsBip*(a1, a2) from start
void Threading::processAffectsBipTDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	temp = pkb.getAffectsBipTStartAPI(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]);
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

// AffectsBip*(a1, a2) from end
void Threading::processAffectsBipTDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
    temp = pkb.getAffectsBipTEndAPI(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        result.push_back(holder);
    }
	boost::interprocess::named_semaphore sem(boost::interprocess::open_only_t(), SEMNAME);
	sem.post();
}

bool Threading::processAffectsBipTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para1ValInt.size());
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processAffectsBipTDiffVarStart, this, boost::ref(results.at(i)), para1ValString, para1ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para1ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para1ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
    else {
		boost::interprocess::named_semaphore::remove(SEMNAME);
		boost::interprocess::named_semaphore sem(boost::interprocess::create_only_t(), SEMNAME, 0);
		std::vector<std::vector<std::vector<std::string>>> results((int)para2ValInt.size());
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			boost::function<void()> f = boost::bind(&Threading::processAffectsBipTDiffVarEnd, this, boost::ref(results.at(i)), para2ValString, para2ValInt, boost::ref(pkb), i);
			this->threadPool.enqueue(f);
		}
		for (int i = 0; i < (int)para2ValInt.size(); i++) {
			sem.wait();
		}
		for (int i=0; i<(int)para2ValInt.size(); i++) {
			for (std::vector<std::vector<std::string>>::iterator it=results.at(i).begin(); it<results.at(i).end(); it++) {
				toStoreTuple.push_back(*it);
			}
		}
		boost::interprocess::named_semaphore::remove(SEMNAME);
		return true;
    }
}
// AffectsBip*]
////////////////////////////////////////////

std::vector<std::string> Threading::intVecToStringVec(std::vector<int> input) {
    std::vector<std::string> output;
    for (int i = 0; i < (int)input.size(); i++) {
        output.push_back(std::to_string((long long)input[i]));
    }
    return output;
}
#endif