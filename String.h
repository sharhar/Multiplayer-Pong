#ifndef STRING_H
#define STRING_H

#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <string.h>

class StringUtils{
public:

	static std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	    std::stringstream ss(s);
	    std::string item;
	    while (std::getline(ss, item, delim)) {
	        elems.push_back(item);
	    }
	    return elems;
	}


	static std::vector<std::string> split(const std::string &s, char delim) {
	    std::vector<std::string> elems;
	    split(s, delim, elems);
	    return elems;
	}

	static inline double parseDouble(std::string number) {
		return atof(number.c_str());
	}

	static inline float parseFloat(std::string number) {
		return (float) parseDouble(number);
	}

	static inline int parseInt(std::string number) {
		return (int) parseDouble(number);
	}
};

#endif