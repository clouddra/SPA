#include "HelperFunctions.h"
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

string loadFileContent(const string& filepath) {
	string ans, line;
	ifstream myfile (filepath);
	if (!myfile.is_open()) return "";
	while ( myfile.good() ) {
		getline (myfile,line);
		ans.append(line);
	}
	myfile.close();
	return ans;
}