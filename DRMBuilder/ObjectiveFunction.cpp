//ObjectiveFunction.cpp
#include "stdafx.h"
#include "ObjectiveFunction.h"
#include "DRMManager.h"

CObjectiveFunction::CObjectiveFunction()
{
	ncount = 0;
	pmsg = NULL;
}

CObjectiveFunction::~CObjectiveFunction()
{
}

double CObjectiveFunction::CalcObjectiveFunction(double* pvar)
{
	CString str;
	int i;
	int ninput = pmodel->ninput;
	//assign pvar to pole_list
	for (i=0; i<ninput; i++)
		pmodel->pole_list[i] = pvar[i];
	pmodel->UpdateLaguerreStateSpaceList();
	pmodel->ProcessIdentificationDataForLaguerreTraining();
	pmodel->TrainLaguerreNeuralNetwork();
	ncount++;
	str.Format("Objective function has been called for %d times.\n",ncount);
	pmsg->AddMessageLine(str);
	return pmodel->ann_lag.GetBatchError();
}