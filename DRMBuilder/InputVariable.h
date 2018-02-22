//InputVariable.h

#ifndef __INPUTVARIABLE_H__
#define __INPUTVARIABLE_H__

#include <string>

class CInputVariable
{
public:
	std::string name;			//name of variables
	std::string desc;			//discription
	std::string unit;			//unit
	bool	bvaried;			//flag, true if varied
	bool	bramp;				//flag, true if ramp is need to replace step change for high-fidelity model simulation
	double	xdefault;			//default
	double	xlower;				//lower limit
	double	xupper;				//upper limit
	double	ramp_rate;			//ramp rate, unit is variable unit/time unit

	CInputVariable();
	virtual ~CInputVariable();
	CInputVariable(const CInputVariable &t);
	CInputVariable& operator=(const CInputVariable& t);
	void Write(FILE* pf);
	void Read(FILE* pf);
};

#endif