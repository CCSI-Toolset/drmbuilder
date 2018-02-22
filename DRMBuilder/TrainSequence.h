//TrainSequence.h

#ifndef __TRAINSEQUENCE_H__
#define __TRAINSEQUENCE_H__

#include "LHS.h"

class CTrainSequence
{
private:
	int	ndim;				//number of dimension
	int npoint;				//number of points
	int nduration;			//number of time durations
	int* pistart;			//starting point, could be steady state point
	CLHS* plhs;				//an array of LHS of size nduration
	void DeleteArray();

public:
	CTrainSequence();
	CTrainSequence(int ndi, int np, int ndu);
	virtual ~CTrainSequence();
	CTrainSequence(const CTrainSequence &t);
	CTrainSequence& operator=(const CTrainSequence& t);
	void CopySamples(CTrainSequence& t);
	void AllocateArray();
	void SetDimension(int n);
	void SetNumberOfPoints(int n);
	void SetNumberOfDurations(int n);
	void SetSizeAll(int ndi, int np, int ndu);
	void SetDefaultStartingPoint();
	void SetRandomStartingPoint();
	void SetStartingPoint(int* pi);
	void SimpleSampling();
	void SelectBestSampling(int n);
	int GetDimension() {return ndim;}
	int GetNumberOfPoints() {return npoint;}
	int GetNumberOfDurations() {return nduration;}
	int GetSizeOfTrainSequence() {return nduration*(npoint-1);}
	CLHS* GetSampleList() {return plhs;}
	void GetTrainSequence(int** ppx);
	double CalcQualityPhi(double p);
	void Write(FILE* pf);
	void Read(FILE* pf);
};

#endif