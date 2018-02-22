//ObjectiveFunction.h

#ifndef __OBJECTIVEFUNCTION_H__
#define __OBJECTIVEFUNCTION_H__

#include "Dabnet.h"
#include "DRMMessage.h"

class CObjectiveFunction
{
public:
	int ncount;						//number of function call
	CDabnet* pmodel;				//currently a DABNet model pointer
	CDRMMessage* pmsg;				//a pointer to message object for message display

	CObjectiveFunction();
	virtual ~CObjectiveFunction();
	void SetModel(CDabnet* pm) {pmodel = pm;}
	virtual double CalcObjectiveFunction(double* pvar);
	void ResetCounter() {ncount = 0;}
};

#endif