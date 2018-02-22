//SolverInput.h
#ifndef __SOLVERINPUT_H__
#define __SOLVERINPUT_H__

#include <string>
#include <list>
#include <vector>
#include "TrainSequence.h"
#include "InputVariable.h"

class CSolverInput
{
private:
	int ireverse;									//add reverse order points, 1 if true
	int	ndim;										//number of dimension
	int npoint;										//number of points
	int nduration;									//number of durations
	int duration0;									//initial condition duration in term of number of samplings
	int nstep;										//number of time steps in the training sequence
	double dt_sampling;								//sampling time interval in sec
	double dt_min_solver;							//minimum time step size of the solver
	std::vector<int> vduration;						//duration in term of number of samplings, size = nduration
	std::list<CInputVariable>* pinput_list_all;		//pointer to all input list
	std::list<CInputVariable>* pinput_list_sel;		//pointer to selected input list
	std::vector<double> vsequence;					//vector represnting 2-D input sequence array, to be used by MATLAB for ACM
	CTrainSequence ts;								//train sequence

public:
	CSolverInput();
	CSolverInput(int ndi, int np, int ndu);
	virtual ~CSolverInput();
	void EnableReverse(int i) {ireverse = i;}
	void SetDimension(int n) {ndim = n;}
	void SetNumberOfPoints(int n) {npoint = n;}
	void SetInitialDuration(int n) {duration0 = n;}
	void SetSamplingTimeInterval(double t) {dt_sampling = t;}
	void SetSolverMinimumTimeStep(double t) {dt_min_solver = t;}
	void SetDurationVector(std::vector<int>* pd);
	void SetSelectedInputVariableList(std::list<CInputVariable>* pi) {pinput_list_sel = pi; ndim = pi->size();}
	void SetAllInputVariableList(std::list<CInputVariable>* pi) {pinput_list_all = pi;}
	double GetSamplingTimeInterval() {return dt_sampling;}
	double GetSolverMinimumTimeStep() {return dt_min_solver;}
	int GetReverseFlag() {return ireverse;}
	int GetNumberOfPoints() {return npoint;}
	std::vector<int>* GetDurationVector() {return &vduration;}
	double* GetSequenceVectorAsMemoryPointer() {return &vsequence[0];}
	int GetNumberOfTrainingSteps() {return nstep;}
	void GenerateTrainingSequence();
	void WriteAPMonitorInputFile(FILE* pf, int nostep=0);
	void WriteACMInputFile(FILE* pf);
	void PrepareSequenceVectorForACMInputArray();
	void Write(FILE* pf);
	void Read(FILE* pf);
};

#endif