#include "HelperFunctions.h"
#include<fstream>
#include<string>
#include<vector>
#include<sstream>

using namespace std;

void string_tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


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


/*bool stringVectorContains( string str, vector<string> v ) {
	bool returnValue = false;
	for(int i = 0; i < v.size(); i++)
	{
		if (v[i] == str) {
			returnValue = true;
			break;
		}
	}
	return returnValue;
}*/

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}