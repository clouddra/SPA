#pragma once
#ifdef ENABLE_THREADING

#include "QueryProcessor.h"

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

Threading::Threading(int nThreads) {
	this->nThreads = nThreads;
	this->tList = new boost::thread*[nThreads];
	for (int i=0; i<nThreads; i++) {
		this->tList[i] = new boost::thread();
	}
	this->workers = new Worker[nThreads];
}

Threading::~Threading() {
	delete[] workers;
	delete[] tList;
}

///////////////////////////////////////
// [Affects

// Affects(a1, a1)
std::vector<int> Threading::processAffectsSameVarStart(std::vector<int>& para1Val, PKB& pkb, int i) {
	std::vector<int> result;
	std::vector<int> temp2;
    temp2 = pkb.getAffectsStart(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	return result;
}

void Threading::processAffectsSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
    for (int i = 0; i < (int)para1Val.size(); i++) {
		if (i%nThreads == 0 && i > 0) {
			tGroup.join_all();
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

	tGroup.join_all();
	for (int j=0; j<nThreads; j++) {
		if (tGroup.is_thread_in(tList[j])) {
			std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
			for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
				temp.push_back(*it);
			}
			tGroup.remove_thread(tList[j]);
		}
	}
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

void Threading::processAffectsDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
        for (int i = 0; i < (int)para1ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				tGroup.join_all();
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

		tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
    }
    else {
        for (int i = 0; i < (int)para2ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				tGroup.join_all();
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

		tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
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
    temp2 = pkb.getAffectsTStart(para1Val[i]);
    for (int j = 0; j < (int)temp2.size(); j++) {
        if (temp2[j] == para1Val[i]) {
            result.push_back(para1Val[i]);
            break;
        }
    }
	return result;
}

void Threading::processAffectsTSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB& pkb) {
    for (int i = 0; i < (int)para1Val.size(); i++) {
		if (i%nThreads == 0 && i > 0) {
			tGroup.join_all();
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

	tGroup.join_all();
	for (int j=0; j<nThreads; j++) {
		if (tGroup.is_thread_in(tList[j])) {
			std::vector<int> returnedVector = workers[j%nThreads].getReturnVector();
			for (std::vector<int>::iterator it=returnedVector.begin(); it<returnedVector.end(); it++) {
				temp.push_back(*it);
			}
			tGroup.remove_thread(tList[j]);
		}
	}
}

// Affects*(a1, a2) from start
std::vector<std::vector<std::string>> Threading::processAffectsTDiffVarStart(std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, PKB& pkb, int i) {
	std::vector<int> temp;
	std::vector<std::string> toStore;
	std::vector<std::vector<std::string>> toStoreTuple;
	temp = pkb.getAffectsTStart(para1ValInt[i]);
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
    temp = pkb.getAffectsTEnd(para2ValInt[i]);
    toStore = intVecToStringVec(temp);
    for (int j = 0; j < (int)toStore.size(); j++) {
        std::vector<std::string> holder;
        holder.push_back(toStore[j]); 
        holder.push_back(para2ValString[i]);
        toStoreTuple.push_back(holder);
    }
	return toStoreTuple;
}

void Threading::processAffectsTDiffVarDriver(std::vector<std::vector<std::string>>& toStoreTuple, std::vector<std::string>& para1ValString, std::vector<int>& para1ValInt, 
							std::vector<std::string>& para2ValString, std::vector<int>& para2ValInt, bool isPara1, PKB& pkb) {
    if (isPara1) {
        for (int i = 0; i < (int)para1ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				tGroup.join_all();
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

		tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
    }
    else {
        for (int i = 0; i < (int)para2ValInt.size(); i++) {
			if (i%nThreads == 0 && i > 0) {
				tGroup.join_all();
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

		tGroup.join_all();
		for (int j=0; j<nThreads; j++) {
			if (tGroup.is_thread_in(tList[j])) {
				std::vector<std::vector<std::string>> returnedTuple = workers[j%nThreads].getReturnTuple();
				for (std::vector<std::vector<std::string>>::iterator it=returnedTuple.begin(); it<returnedTuple.end(); it++) {
					toStoreTuple.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}
    }
}
// Affects*]
////////////////////////////////////////////
std::vector<std::string> Threading::intVecToStringVec(std::vector<int> input) {
    std::vector<std::string> output;
    for (int i = 0; i < (int)input.size(); i++) {
        output.push_back(std::to_string((long long)input[i]));
    }
    return output;
}

#endif