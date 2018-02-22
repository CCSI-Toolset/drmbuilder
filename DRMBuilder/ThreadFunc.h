//ThreadFunc.h

#ifndef __THREADFUNC_H__
#define __THREADFUNC_H__

UINT LaunchHighFidelityModelForTraining(LPVOID pParam);
UINT LaunchHighFidelityModelForValidation(LPVOID pParam);
UINT GenerateDRM(LPVOID pParam);
UINT PlotDataForTraining(LPVOID pParam);
UINT PlotDataForValidation(LPVOID pParam);
UINT CalculateDRMUQ(LPVOID pParam);

#endif