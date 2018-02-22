#ifndef __LAGUERRESTATESPACE_H__
#define __LAGUERRESTATESPACE_H__

#include "StateSpace.h"
#include "Matrix.h"
#include "DRMMessage.h"

class CLaguerreStateSpace : public CStateSpace
{
//currently for single input and single output without D matrix
//minimum number of states is 2 (at least one low-pass and one all-pass)
public:
	int irealization;		//realization method, 0=Larry, 1=Wang
	double a;				//pole, 1-T/tau
	CMatrix mweight;		//weight matrix from Laguerre neural network
	CDRMMessage* pmsg;		//a pointer to message object for message display
	
	CLaguerreStateSpace(void);
	virtual ~CLaguerreStateSpace(void);
	virtual void InitStateVectorToSteadyState(double u);
	void CreateUnbalancedStateSpace();
	void PrepareWeightMatrix(int nneuron, double** ppweight, double* psigma, int iscale);
	void RealizeBalancedStateSpace(CStateSpace* pss);
	virtual void Write(FILE* pf);
	virtual void Read(FILE* pf);
};

#endif