#pragma once
#ifdef ENABLE_THREADING

#include "QueryProcessor.h"

/*
void Worker::runNextSame(boost::function<std::vector<int>()> f) {
	returnVector = f();
}

void Worker::runNextDiff(boost::function<std::vector<std::vector<std::string>>()> f) {
	returnTuple = f();
}

void Worker::runAffectsSame(boost::function<std::vector<int>()> f) {
	returnVector = f();
}

void Worker::runAffectsDiff(boost::function<std::vector<std::vector<std::string>>()> f) {
	returnTuple = f();
}

std::vector<int> Worker::getReturnVector() {
	return returnVector;
}

std::vector<std::vector<std::string>> Worker::getReturnTuple() {
	return returnTuple;
}
*/

void Worker::operator()() { pool.service.run(); }
 
ThreadPool::ThreadPool(int threads) : work(new boost::asio::io_service::work(service))
{
    for(int i = 0;i<threads; i++)
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

Threading::Threading(int nThreads) {
	this->nThreads = nThreads;
	//this->tList = new boost::thread*[nThreads];
	//for (int i=0; i<nThreads; i++) {
	//	this->tList[i] = new boost::thread();
	//}
	//this->workers = new Worker[nThreads];
	this->threadPool(nThreads);
}

Threading::~Threading() {
	//delete[] workers;
	//delete[] tList;
}

///////////////////////////////////////
// [Next

// Next(s1, s1)
void Threading::processNextSameVarStart(std::vector<int>& result, std::vector<int>& para1Val, PKB& pkb, int i) {
    if (pkb.isNext(para1Val[i], para1Val[i])) {
        result.push_back(i);
    }
}

bool Threading::processNextSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
    for (int i = 0; i < (int)para1Val.size(); i++) {
		if (i%nThreads == 0 && i > 0) {
			if (!join_all()) {
				terminate_all();
				return false;
			}
			//tGroup.join_all();
			for (int j=0; j<nThreads; j++) {
				std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
				for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
					temp.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}

		boost::function<std::vector<int>()> f2 = boost::bind(&Threading::processNextSameVarStart, this, para1Val, pkb, i);
		boost::function<void()> f = boost::bind(&Worker::runNextSame, &(workers[i%nThreads]), f2);
		tList[i%nThreads] = new boost::thread(f);
		tGroup.add_thread(tList[i%nThreads]);
    }

	if (!join_all()) {
		terminate_all();
		return false;
	}
	//tGroup.join_all();
	for (int j=0; j<nThreads; j++) {
		if (tGroup.is_thread_in(tList[j])) {
			std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
			for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
				temp.push_back(*it);
			}
			tGroup.remove_thread(tList[j]);
		}
	}
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
}

bool Threading::processNextDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
        for (int i = 0; i < (int)para1ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processNextDiffVarStart, this, para1ValString, para1ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runNextDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
    else {
        for (int i = 0; i < (int)para2ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processNextDiffVarEnd, this, para2ValString, para2ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runNextDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
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
	std::vector<int> temp2 = pkb.getNextT(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (para1Val[i] == temp2[j]) {
            found = true;
            break;
        }
    }
    if (found)
        result.push_back(para1Val[i]);
}

bool Threading::processNextTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
    for (int i = 0; i < (int)para1Val.size(); i++) {
		if (i%nThreads == 0 && i > 0) {
			if (!join_all()) {
				terminate_all();
				return false;
			}
			//tGroup.join_all();
			for (int j=0; j<nThreads; j++) {
				std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
				for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
					temp.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}

		boost::function<std::vector<int>()> f2 = boost::bind(&Threading::processNextTSameVarStart, this, para1Val, pkb, i);
		boost::function<void()> f = boost::bind(&Worker::runNextSame, &(workers[i%nThreads]), f2);
		tList[i%nThreads] = new boost::thread(f);
		tGroup.add_thread(tList[i%nThreads]);
    }

	if (!join_all()) {
		terminate_all();
		return false;
	}
	//tGroup.join_all();
	for (int j=0; j<nThreads; j++) {
		if (tGroup.is_thread_in(tList[j])) {
			std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
			for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
				temp.push_back(*it);
			}
			tGroup.remove_thread(tList[j]);
		}
	}
	return true;
}

// Next*(s1, s2) from start
void Threading::processNextTDiffVarStart(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	temp = pkb.getNextT(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]); 
        result.push_back(holder);
    }
}

// Next*(s1, s2) from end
void Threading::processNextTDiffVarEnd(std::vector<std::vector<std::string>>& result, std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
    temp = pkb.getPrevT(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        result.push_back(holder);
    }
}

bool Threading::processNextTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
        for (int i = 0; i < (int)para1ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processNextTDiffVarStart, this, para1ValString, para1ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runNextDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
    else {
        for (int i = 0; i < (int)para2ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processNextTDiffVarEnd, this, para2ValString, para2ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runNextDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
}

// Next*]
////////////////////////////////////////////

///////////////////////////////////////
// [Affects

// Affects(a1, a1)
std::vector<int> Threading::processAffectsSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i) {
	std::vector<int> result;
	std::vector<int> temp2;
    temp2 = pkb.getAffectsStartAPI(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	return result;
}

bool Threading::processAffectsSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
    for (int i = 0; i < (int)para1Val.size(); i++) {
		if (i%nThreads == 0 && i > 0) {
			if (!join_all()) {
				terminate_all();
				return false;
			}
			//tGroup.join_all();
			for (int j=0; j<nThreads; j++) {
				std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
				for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
					temp.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}

		boost::function<std::vector<int>()> f2 = boost::bind(&Threading::processAffectsSameVarStart, this, para1Val, pkb, i);
		boost::function<void()> f = boost::bind(&Worker::runAffectsSame, &(workers[i%nThreads]), f2);
		tList[i%nThreads] = new boost::thread(f);
		tGroup.add_thread(tList[i%nThreads]);
    }

	if (!join_all()) {
		terminate_all();
		return false;
	}
	//tGroup.join_all();
	for (int j=0; j<nThreads; j++) {
		if (tGroup.is_thread_in(tList[j])) {
			std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
			for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
				temp.push_back(*it);
			}
			tGroup.remove_thread(tList[j]);
		}
	}
	return true;
}

// Affects(a1, a2) from start
std::vector<std::vector<std::string>> Threading::processAffectsDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	std::vector<std::vector<std::string>> toStoreTuple;
	temp = pkb.getAffectsStartAPI(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]); 
        toStoreTuple.push_back(holder);
    }
	return toStoreTuple;
}

// Affects(a1, a2) from end
std::vector<std::vector<std::string>> Threading::processAffectsDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	std::vector<std::vector<std::string>> toStoreTuple;
    temp = pkb.getAffectsEndAPI(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        toStoreTuple.push_back(holder);
    }
	return toStoreTuple;
}

bool Threading::processAffectsDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
        for (int i = 0; i < (int)para1ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processAffectsDiffVarStart, this, para1ValString, para1ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runAffectsDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
    else {
        for (int i = 0; i < (int)para2ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processAffectsDiffVarEnd, this, para2ValString, para2ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runAffectsDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
}

// Affects]
////////////////////////////////////////////

////////////////////////////////////////////
// [Affects*

// Affects*(a1, a1)
std::vector<int> Threading::processAffectsTSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i) {
	std::vector<int> result;
	std::vector<int> temp2;
    temp2 = pkb.getAffectsTStartAPI(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	return result;
}

bool Threading::processAffectsTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
    for (int i = 0; i < (int)para1Val.size(); i++) {
		if (i%nThreads == 0 && i > 0) {
			if (!join_all()) {
				terminate_all();
				return false;
			}
			//tGroup.join_all();
			for (int j=0; j<nThreads; j++) {
				std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
				for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
					temp.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}

		boost::function<std::vector<int>()> f2 = boost::bind(&Threading::processAffectsTSameVarStart, this, para1Val, pkb, i);
		boost::function<void()> f = boost::bind(&Worker::runAffectsSame, &(workers[i%nThreads]), f2);
		tList[i%nThreads] = new boost::thread(f);
		tGroup.add_thread(tList[i%nThreads]);
    }

	if (!join_all()) {
		terminate_all();
		return false;
	}
	//tGroup.join_all();
	for (int j=0; j<nThreads; j++) {
		if (tGroup.is_thread_in(tList[j])) {
			std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
			for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
				temp.push_back(*it);
			}
			tGroup.remove_thread(tList[j]);
		}
	}
	return true;
}

// Affects*(a1, a2) from start
std::vector<std::vector<std::string>> Threading::processAffectsTDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	std::vector<std::vector<std::string>> toStoreTuple;
	temp = pkb.getAffectsTStartAPI(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]); 
        toStoreTuple.push_back(holder);
    }
	return toStoreTuple;
}

// Affects*(a1, a2) from end
std::vector<std::vector<std::string>> Threading::processAffectsTDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	std::vector<std::vector<std::string>> toStoreTuple;
    temp = pkb.getAffectsTEndAPI(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        toStoreTuple.push_back(holder);
    }
	return toStoreTuple;
}

bool Threading::processAffectsTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
        for (int i = 0; i < (int)para1ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processAffectsTDiffVarStart, this, para1ValString, para1ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runAffectsDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
    else {
        for (int i = 0; i < (int)para2ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processAffectsTDiffVarEnd, this, para2ValString, para2ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runAffectsDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
}
// Affects*]
////////////////////////////////////////////


///////////////////////////////////////
// [AffectsBip

// AffectsBip(a1, a1)
std::vector<int> Threading::processAffectsBipSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i) {
	std::vector<int> result;
	std::vector<int> temp2;
    temp2 = pkb.getAffectsBipStartAPI(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	return result;
}

bool Threading::processAffectsBipSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
    for (int i = 0; i < (int)para1Val.size(); i++) {
		if (i%nThreads == 0 && i > 0) {
			if (!join_all()) {
				terminate_all();
				return false;
			}
			//tGroup.join_all();
			for (int j=0; j<nThreads; j++) {
				std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
				for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
					temp.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}

		boost::function<std::vector<int>()> f2 = boost::bind(&Threading::processAffectsBipSameVarStart, this, para1Val, pkb, i);
		boost::function<void()> f = boost::bind(&Worker::runAffectsSame, &(workers[i%nThreads]), f2);
		tList[i%nThreads] = new boost::thread(f);
		tGroup.add_thread(tList[i%nThreads]);
    }

	if (!join_all()) {
		terminate_all();
		return false;
	}
	//tGroup.join_all();
	for (int j=0; j<nThreads; j++) {
		if (tGroup.is_thread_in(tList[j])) {
			std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
			for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
				temp.push_back(*it);
			}
			tGroup.remove_thread(tList[j]);
		}
	}
	return true;
}

// AffectsBip(a1, a2) from start
std::vector<std::vector<std::string>> Threading::processAffectsBipDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	std::vector<std::vector<std::string>> toStoreTuple;
	temp = pkb.getAffectsBipStartAPI(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]); 
        toStoreTuple.push_back(holder);
    }
	return toStoreTuple;
}

// AffectsBip(a1, a2) from end
std::vector<std::vector<std::string>> Threading::processAffectsBipDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	std::vector<std::vector<std::string>> toStoreTuple;
    temp = pkb.getAffectsBipEndAPI(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        toStoreTuple.push_back(holder);
    }
	return toStoreTuple;
}

bool Threading::processAffectsBipDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
        for (int i = 0; i < (int)para1ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processAffectsBipDiffVarStart, this, para1ValString, para1ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runAffectsDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
    else {
        for (int i = 0; i < (int)para2ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processAffectsBipDiffVarEnd, this, para2ValString, para2ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runAffectsDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
}

// AffectsBip]
////////////////////////////////////////////

////////////////////////////////////////////
// [AffectsBip*

// AffectsBip*(a1, a1)
std::vector<int> Threading::processAffectsBipTSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i) {
	std::vector<int> result;
	std::vector<int> temp2;
    temp2 = pkb.getAffectsBipTStartAPI(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	return result;
}

bool Threading::processAffectsBipTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
    for (int i = 0; i < (int)para1Val.size(); i++) {
		if (i%nThreads == 0 && i > 0) {
			if (!join_all()) {
				terminate_all();
				return false;
			}
			//tGroup.join_all();
			for (int j=0; j<nThreads; j++) {
				std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
				for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
					temp.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}

		boost::function<std::vector<int>()> f2 = boost::bind(&Threading::processAffectsBipTSameVarStart, this, para1Val, pkb, i);
		boost::function<void()> f = boost::bind(&Worker::runAffectsSame, &(workers[i%nThreads]), f2);
		tList[i%nThreads] = new boost::thread(f);
		tGroup.add_thread(tList[i%nThreads]);
    }

	if (!join_all()) {
		terminate_all();
		return false;
	}
	//tGroup.join_all();
	for (int j=0; j<nThreads; j++) {
		if (tGroup.is_thread_in(tList[j])) {
			std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
			for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
				temp.push_back(*it);
			}
			tGroup.remove_thread(tList[j]);
		}
	}
	return true;
}

// AffectsBip*(a1, a2) from start
std::vector<std::vector<std::string>> Threading::processAffectsBipTDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	std::vector<std::vector<std::string>> toStoreTuple;
	temp = pkb.getAffectsBipTStartAPI(para1ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(para1ValString[i]);
        holder.push_back(toStore[j]);
        toStoreTuple.push_back(holder);
    }
	return toStoreTuple;
}

// AffectsBip*(a1, a2) from end
std::vector<std::vector<std::string>> Threading::processAffectsBipTDiffVarEnd(std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	std::vector<std::vector<std::string>> toStoreTuple;
    temp = pkb.getAffectsBipTEndAPI(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        toStoreTuple.push_back(holder);
    }
	return toStoreTuple;
}

bool Threading::processAffectsBipTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
        for (int i = 0; i < (int)para1ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processAffectsBipTDiffVarStart, this, para1ValString, para1ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runAffectsDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
		return true;
    }
    else {
        for (int i = 0; i < (int)para2ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				if (!join_all()) {
					terminate_all();
					return false;
				}
				//tGroup.join_all();
				for (int j=0; j<nThreads; j++) {
					std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
					for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
						toStoreTuple.push_back(*it);
					}
					tGroup.remove_thread(tList[j]);
				}
			}

			boost::function<std::vector<std::vector<std::string>>()> f2 = boost::bind(&Threading::processAffectsBipTDiffVarEnd, this, para2ValString, para2ValInt, pkb, i);
			boost::function<void()> f = boost::bind(&Worker::runAffectsDiff, &(workers[i%nThreads]), f2);
			tList[i%nThreads] = new boost::thread(f);
			tGroup.add_thread(tList[i%nThreads]);
        }

		if (!join_all()) {
			terminate_all();
			return false;
		}
		//tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
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

/*
bool Threading::join_all() {
	while(1) {
		bool flag = true;
		for (int j=0; j<nThreads; j++) {
			if (tList[j]->joinable() && !tList[j]->try_join_for(boost::chrono::milliseconds(1000))) {
				flag = false;
				if (AbstractWrapper::GlobalStop) {
					return false;
				}
			}
			else flag = true & flag;
		}
		if (flag) break;
	}
	return true;
}
*/

bool Threading::join_all() {
	/*
	for (int j=0; j<nThreads; j++) {
		if (tList[j]->joinable()) {
			tList[j]->join();
		}
	}
	*/
	tGroup.join_all();
	return true;
}

// Not portable, windows only
void Threading::terminate_all() {
	for (int i=0; i<nThreads; i++) {
		TerminateThread(tList[i]->native_handle(), EXIT_SUCCESS);
	}
}
#endif