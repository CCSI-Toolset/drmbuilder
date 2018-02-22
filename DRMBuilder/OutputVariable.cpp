//OutputVariable.cpp

#include "stdafx.h"
#include "OutputVariable.h"
#include "Util.h"

COutputVariable::COutputVariable()
{
	name = "";
	desc = "";
	unit = "";
	bvaried = false;
	xdefault = 0;
}

COutputVariable::~COutputVariable()
{
}

COutputVariable::COutputVariable(const COutputVariable &t)
{
	name = t.name;
	desc = t.desc;
	unit = t.unit;
	bvaried = t.bvaried;
	xdefault = t.xdefault;
}

COutputVariable& COutputVariable::operator=(const COutputVariable& t)
{
	if (this==&t)
		return *this;
	name = t.name;
	desc = t.desc;
	unit = t.unit;
	bvaried = t.bvaried;
	xdefault = t.xdefault;
	return *this;
}

void COutputVariable::Write(FILE* pf)
{
	int iversion = 0;
	fwrite(&iversion,sizeof(int),1,pf);
	CUtil::WriteString(name,pf);
	CUtil::WriteString(desc,pf);
	CUtil::WriteString(unit,pf);
	fwrite(&bvaried,sizeof(bool),1,pf);
	fwrite(&xdefault,sizeof(double),1,pf);
}

void COutputVariable::Read(FILE* pf)
{
	int iversion;
	fread(&iversion,sizeof(int),1,pf);
	CUtil::ReadString(name,pf);
	CUtil::ReadString(desc,pf);
	CUtil::ReadString(unit,pf);
	fread(&bvaried,sizeof(bool),1,pf);
	fread(&xdefault,sizeof(double),1,pf);
}
