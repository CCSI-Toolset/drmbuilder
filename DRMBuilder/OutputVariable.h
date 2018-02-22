//Parameter.h

#ifndef __OUTPUTVARIABLE_H__
#define __OUTPUTVARIABLE_H__

#include <string>

class COutputVariable
{
public:
	std::string name;			//variable name
	std::string desc;			//variable description
	std::string unit;			//unit
	bool	bvaried;			//flag, true if varied
	double xdefault;			//variable default value

	COutputVariable();
	~COutputVariable();
	COutputVariable(const COutputVariable &t);
	COutputVariable& operator=(const COutputVariable& t);
	void Write(FILE* pf);
	void Read(FILE* pf);
};

#endif