#pragma once
#include<string>
#include <sstream>
#include <list>
#include <iostream>
#include <vector>

using namespace std;

void string_tokenize(const string& str, vector<string>& tokens, const string& delimiters);

string loadFileContent(const string& filepath);

inline string intToString(int n) {
	std::stringstream ss;
	ss << n;
	return ss.str();
}

inline int stringToInt(const string& s) {
  istringstream i(s);
  int x;
  if (!(i >> x)) return -1;
  return x;
}

//bool stringVectorContains( string str, vector<string> v );

template <typename T>
inline int findItemPos(const vector<T>& v, T& item) {
	for (auto i = 0; i < v.size(); i++) {
		if (v[i] == item)
			return i;
	}
	return -1;
}

template<typename IT>
inline string implode(IT first, IT last, char delim=' ') {
	stringstream ss;
  while (first != last) {
		ss << *first;
    if (++first != last)
			ss << delim;
  }
	return ss.str();
}

inline bool isInteger(const string & s)
{
  if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;
  char * p ;
  strtol(s.c_str(), &p, 10);
  return (*p == 0) ;
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);