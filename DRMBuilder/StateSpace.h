#ifndef __STATESPACE_H__
#define __STATESPACE_H__

#include "CCSI.h"

class CStateSpace
{
//currently for single input and single output without D matrix
public:
	int nstate;				//number of state variables
	double**  ppa;			//A matrix
	double* pb;				//B matrix
	double* pc;				//C matrix
	double* px;				//current state vector
	
	CStateSpace(void);
	virtual ~CStateSpace(void);
	virtual void AllocateMemory();
	virtual void DeleteMemory();
	void InitStateVectorAsB();
	void InitStateVectorTo(double x);
	virtual void InitStateVectorToSteadyState(double u);
	void CalcNextStateVector(double u);
	double CalcOutput();
	void WriteStateSpaceMatrices(FILE* pf);
	void ReadStateSpaceMatrices(FILE* pf);
	virtual void Write(FILE* pf);
	virtual void Read(FILE* pf);
};

#endif