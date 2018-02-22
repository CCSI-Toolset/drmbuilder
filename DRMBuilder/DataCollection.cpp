//DataCollection.cpp
#include "stdafx.h"
#include <math.h>
#include "DataCollection.h"
#include "CCSI.h"

CDataCollection::CDataCollection(void)
{
	npair = 1;
	nin = 1;
	nout = 1;
	ppdata = NULL;
	pmean = NULL;
	psigma = NULL;
}

CDataCollection::~CDataCollection(void)
{
	DeleteMemory();
}

CDataCollection::CDataCollection(const CDataCollection &t)
{
	int i, j;
	npair = t.npair;
	nin = t.nin;
	nout = t.nout;
	ppdata = NULL;
	pmean = NULL;
	psigma = NULL;
	if (t.ppdata==NULL)
		return;
	AllocateMemory();
	for (i=0; i<npair; i++)
	{
		for (j=0; j<nin+nout; j++)
			ppdata[i][j] = t.ppdata[i][j];
	}
	for (j=0; j<nin+nout; j++)
	{
		pmean[j] = t.pmean[j];
		psigma[j] = t.psigma[j];
	}
}

CDataCollection& CDataCollection::operator=(const CDataCollection& t)
{
	if (this==&t)
		return *this;
	int i, j;
	DeleteMemory();
	npair = t.npair;
	nin = t.nin;
	nout = t.nout;
	if (t.ppdata!=NULL)
	{
		AllocateMemory();
		for (i=0; i<npair; i++)
		{
			for (j=0; j<nin+nout; j++)
				ppdata[i][j] = t.ppdata[i][j];
		}
		for (j=0; j<nin+nout; j++)
		{
			pmean[j] = t.pmean[j];
			psigma[j] = t.psigma[j];
		}
	}
	return *this;
}

void CDataCollection::AllocateMemory()
{
	int i;
	DeleteMemory();
	ppdata = new double* [npair];
	for (i=0; i<npair; i++)
		ppdata[i] = new double [nin+nout];
	pmean = new double [nin+nout];
	psigma = new double [nin+nout];
}

void CDataCollection::DeleteMemory()
{
	int i;
	if (ppdata!=NULL)
	{
		for (i=0; i<npair; i++)
			delete [] ppdata[i];
		delete [] ppdata;
		ppdata = NULL;
	}
	if (pmean!=NULL)
	{
		delete [] pmean;
		pmean = NULL;
	}
	if (psigma!=NULL)
	{
		delete [] psigma;
		psigma = NULL;
	}
}

void CDataCollection::SetSize(int np, int ni, int no)
{
	DeleteMemory();
	npair = np;
	nin = ni;
	nout = no;
	AllocateMemory();
}

void CDataCollection::AssignInputOutputData(double* pin, double* pout)
{
	//assign unscaled input and output data assuming SetSize() has been called
	//pin: input data generated by CSolverInput::PrepareSequenceVectorForACMInputArray(), mapped from 2-D array to 1-D array
	//pout: output data generated by MATLAB ProcessCommand.m script, mapped from 2-D array to 1-D array
	int i, j;
	for (i=0; i<npair; i++)
	{
		for (j=0; j<nin; j++)
			ppdata[i][j] = pin[i*nin+j];
		for (j=0; j<nout; j++)
			ppdata[i][j+nin] = pout[i*nout+j];
	}
}

void CDataCollection::CalcMeanAndSigma()
{
	//calculate mean and standard deviation
	int i, j;
	double tmp;
	double mean;
	double sigma;
	if (npair==1)		//if only one data point
	{
		for (j=0; j<nin+nout; j++)
		{
			pmean[j] = ppdata[0][j];
			psigma[j] = 0;
		}
		return;
	}
	for (j=0; j<nin+nout; j++)
	{
		mean = 0;
		for (i=0; i<npair; i++)
			mean += ppdata[i][j];
		mean /= npair;
		sigma = 0;
		for (i=0; i<npair; i++)
		{
			tmp = ppdata[i][j] - mean;
			sigma += tmp*tmp;
		}
		sigma = sqrt(sigma/(npair-1));
		pmean[j] = mean;
		psigma[j] = sigma;
	}
}

void CDataCollection::ScaleInputData()
{
	int i, j;
	for (i=0; i<npair; i++)
	{
		for (j=0; j<nin; j++)
			ppdata[i][j] = (ppdata[i][j]-pmean[j])/psigma[j];
	}
}

void CDataCollection::ScaleOutputData()
{
	int i, j;
	int ntotal = nin + nout;
	for (i=0; i<npair; i++)
	{
		for (j=nin; j<ntotal; j++)
			ppdata[i][j] = (ppdata[i][j]-pmean[j])/psigma[j];
	}
}

void CDataCollection::UnscaleInputData()
{
	int i, j;
	for (i=0; i<npair; i++)
	{
		for (j=0; j<nin; j++)
			ppdata[i][j] = pmean[j] + ppdata[i][j]*psigma[j];
	}
}

void CDataCollection::UnscaleOutputData()
{
	int i, j;
	int ntotal = nin + nout;
	for (i=0; i<npair; i++)
	{
		for (j=nin; j<ntotal; j++)
			ppdata[i][j] = pmean[j] + ppdata[i][j]*psigma[j];
	}
}

void CDataCollection::WriteInputData(FILE* pf)
{
	int i, j;
	for (i=0; i<npair; i++)
	{
		fprintf(pf,"%lg",ppdata[i][0]);
		for (j=1; j<nin; j++)
			fprintf(pf,"\t%lg",ppdata[i][j]);
		fprintf(pf,"\n");
	}
}

void CDataCollection::WriteOutputData(FILE* pf)
{
	int i, j;
	int ntotal = nin + nout;
	for (i=0; i<npair; i++)
	{
		fprintf(pf,"%lg",ppdata[i][nin]);
		for (j=nin+1; j<ntotal; j++)
			fprintf(pf,"\t%lg",ppdata[i][j]);
		fprintf(pf,"\n");
	}
}

void CDataCollection::WriteInputAndOutputData(FILE* pf)
{
	int i, j;
	int ntotal = nin + nout;
	for (i=0; i<npair; i++)
	{
		fprintf(pf,"%lg",ppdata[i][0]);
		for (j=1; j<ntotal; j++)
			fprintf(pf,"\t%lg",ppdata[i][j]);
		fprintf(pf,"\n");
	}
}

void CDataCollection::WriteDataInFANNFormat(FILE* pf)
{
	int i, j;
	int ntotal = nin + nout;
	fprintf(pf,"%d\t%d\t%d\n",npair,nin,nout);
	for (i=0; i<npair; i++)
	{
		fprintf(pf,"%lg",ppdata[i][0]);
		for (j=1; j<nin; j++)
			fprintf(pf,"\t%lg",ppdata[i][j]);
		fprintf(pf,"\n");
		fprintf(pf,"%lg",ppdata[i][nin]);
		for (j=nin+1; j<ntotal; j++)
			fprintf(pf,"\t%lg",ppdata[i][j]);
		fprintf(pf,"\n");
	}
}

void CDataCollection::Write(FILE* pf)
{
	int i;
	int iversion = 0;
	bool bppdata = ppdata!=NULL;
	fwrite(&iversion,sizeof(int),1,pf);
	fwrite(&npair,sizeof(int),1,pf);
	fwrite(&nin,sizeof(int),1,pf);
	fwrite(&nout,sizeof(int),1,pf);
	fwrite(&bppdata,sizeof(bool),1,pf);
	if (bppdata)
	{
		for (i=0; i<npair; i++)
			fwrite(ppdata[i],sizeof(double),nin+nout,pf);
		fwrite(pmean,sizeof(double),nin+nout,pf);
		fwrite(psigma,sizeof(double),nin+nout,pf);
	}
}

void CDataCollection::Read(FILE* pf)
{
	int i;
	int iversion;
	bool bppdata;
	DeleteMemory();
	fread(&iversion,sizeof(int),1,pf);
	fread(&npair,sizeof(int),1,pf);
	fread(&nin,sizeof(int),1,pf);
	fread(&nout,sizeof(int),1,pf);
	fread(&bppdata,sizeof(bool),1,pf);
	if (bppdata)
	{
		AllocateMemory();
		for (i=0; i<npair; i++)
			fread(ppdata[i],sizeof(double),nin+nout,pf);
		fread(pmean,sizeof(double),nin+nout,pf);
		fread(psigma,sizeof(double),nin+nout,pf);
	}
}