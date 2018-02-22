//InputVariable.cpp

#include "stdafx.h"
#include "InputVariable.h"
#include "Util.h"

CInputVariable::CInputVariable()
{
	name = "";
	desc = "";
	unit = "";
	bvaried = false;
	bramp = false;
	xdefault = 0;
	xlower = 0;
	xupper = 0;
	ramp_rate = 0;
}

CInputVariable::~CInputVariable()
{
}

CInputVariable::CInputVariable(const CInputVariable &t)
{
	name = t.name;
	desc = t.desc;
	unit = t.unit;
	bvaried = t.bvaried;
	bramp = t.bramp;
	xdefault = t.xdefault;
	xlower = t.xlower;
	xupper = t.xupper;
	ramp_rate = t.ramp_rate;
}

CInputVariable& CInputVariable::operator=(const CInputVariable& t)
{
	if (this==&t)
		return *this;
	name = t.name;
	desc = t.desc;
	unit = t.unit;
	bvaried = t.bvaried;
	bramp = t.bramp;
	xdefault = t.xdefault;
	xlower = t.xlower;
	xupper = t.xupper;
	ramp_rate = t.ramp_rate;
	return *this;
}

void CInputVariable::Write(FILE* pf)
{
	int iversion = 0;
	fwrite(&iversion,sizeof(int),1,pf);
	CUtil::WriteString(name,pf);
	CUtil::WriteString(desc,pf);
	CUtil::WriteString(unit,pf);
	fwrite(&bvaried,sizeof(bool),1,pf);
	fwrite(&bramp,sizeof(bool),1,pf);
	fwrite(&xdefault,sizeof(double),1,pf);
	fwrite(&xlower,sizeof(double),1,pf);
	fwrite(&xupper,sizeof(double),1,pf);
	fwrite(&ramp_rate,sizeof(double),1,pf);
}

void CInputVariable::Read(FILE* pf)
{
	int iversion;
	fread(&iversion,sizeof(int),1,pf);
	CUtil::ReadString(name,pf);
	CUtil::ReadString(desc,pf);
	CUtil::ReadString(unit,pf);
	fread(&bvaried,sizeof(bool),1,pf);
	fread(&bramp,sizeof(bool),1,pf);
	fread(&xdefault,sizeof(double),1,pf);
	fread(&xlower,sizeof(double),1,pf);
	fread(&xupper,sizeof(double),1,pf);
	fread(&ramp_rate,sizeof(double),1,pf);
}