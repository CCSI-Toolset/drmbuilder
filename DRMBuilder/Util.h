//Util.h

#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

class CUtil
{
public:
	CUtil() {}
	virtual ~CUtil() {}
	static void WriteString(std::string& str, FILE* pf);
	static void ReadString(std::string& str, FILE* pf);
	static void WriteStringText(std::string& str, FILE* pf);
};

#endif