//Util.cpp
#include "stdafx.h"
#include "Util.h"

void CUtil::WriteString(std::string& str, FILE* pf)
{
	int strlen = str.size();
	fwrite(&strlen,sizeof(int),1,pf);
	fwrite(str.c_str(),sizeof(char),strlen,pf);
}

void CUtil::ReadString(std::string& str, FILE* pf)
{
	int strlen;
	char* pbuffer;
	fread(&strlen,sizeof(int),1,pf);
	pbuffer = new char [strlen+1];
	pbuffer[strlen] = '\0';
	fread(pbuffer,sizeof(char),strlen,pf);
	str = pbuffer;
	delete [] pbuffer;
}

void CUtil::WriteStringText(std::string& str, FILE* pf)
{
	fprintf(pf,"%s",str.c_str());
}