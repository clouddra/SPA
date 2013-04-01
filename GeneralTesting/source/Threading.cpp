#pragma once
#ifdef ENABLE_THREADING

#include "QueryProcessor.h"

void Worker::run(boost::function<std::vector<int>()> f) {
	returnVector = f();
}

std::vector<int> Worker::getReturnVector() {
	return returnVector;
}

Threading::Threading(int nThreads) {
	this->nThreads = nThreads;
	this->tList = new boost::thread*[nThreads];
	this->workers = new Worker[nThreads];
}

Threading::~Threading() {
	delete workers;
	delete tList;
}

std::vector<int> Threading::processAffectsStart(std::vector<int>& para1Val, PKB pkb, int i) {
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

void Threading::processAffectsSameVarDriver(std::vector<int>& temp, std::vector<int>& para1Val, PKB pkb) {
    for (int i = 0; i < (int)para1Val.size(); i++) {
		if (i%nThreads == 0 && i > 0) {
			tGroup.join_all();
			for (int j=0; j<nThreads; j++) {
				for (std::vector<int>::iterator it=workers[j%nThreads].getReturnVector().begin(); it<workers[j%nThreads].getReturnVector().end(); it++) {
					temp.push_back(*it);
				}
				tGroup.remove_thread(tList[j]);
			}
		}

		boost::function<std::vector<int>()> f2 = boost::bind(&Threading::processAffectsStart, this, para1Val, pkb, i);
		boost::function<void()> f = boost::bind(&Worker::run, &(workers[i%nThreads]), f2);
		tList[i%nThreads] = new boost::thread(f);
		tGroup.add_thread(tList[i%nThreads]);
    }

	tGroup.join_all();
	for (int j=0; j<nThreads; j++) {
		if (tGroup.is_thread_in(tList[j])) {
			for (std::vector<int>::iterator it=workers[j%nThreads].getReturnVector().begin(); it<workers[j%nThreads].getReturnVector().end(); it++) {
				temp.push_back(*it);
			}
			tGroup.remove_thread(tList[j]);
		}
	}
}

#endif