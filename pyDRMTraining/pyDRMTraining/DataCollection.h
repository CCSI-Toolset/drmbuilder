#ifndef __DATACOLLECTION_H__
#define __DATACOLLECTION_H__

#include <vector>
#include <stdio.h>

class CDataCollection
{
//currently for single input and single output without D matrix
public:
	int npair;					//total number of number of data pairs
	int nset;					//number of data sets in the colloction
	std::vector<int> v_npair;	//vector of number of data pairs in individual data set
	int nin;					//number of inputs for each data pair
	int nout;					//number of outputs for each data pair
	double** ppdata;			//data as 2-D array, for each pair, input data followed by output data
	double* pmean;				//mean value
	double* psigma;				//standard deviation
	
	CDataCollection(void);
	virtual ~CDataCollection(void);
	CDataCollection(const CDataCollection &t);
	CDataCollection& operator=(const CDataCollection& t);
	virtual void AllocateMemory();
	virtual void DeleteMemory();
	void SetSize(int np, int ni, int no);
	void SetNset(int ns) {nset = ns;}
	void SetVnpair(std::vector<int> v) {v_npair = v;}
	void AssignInputOutputData(double* pin, double* pout);
	void CalcMeanAndSigma();
	void ScaleInputData();
	void ScaleOutputData();
	void UnscaleInputData();
	void UnscaleOutputData();
};

#endif