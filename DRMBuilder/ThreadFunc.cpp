//ThreadFun.cpp

#include "stdafx.h"
#include "DRMBuilder.h"
#include "DRMBuilderDoc.h"
#include "ThreadFunc.h"

UINT LaunchHighFidelityModelForTraining(LPVOID pParam)
{
	CDRMBuilderDoc* pDoc = (CDRMBuilderDoc*)pParam;
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->bthread = true;
	pmanager->RunMatlabHighFidelityModel(0);
	pmanager->bthread = false;
	pDoc->UpdateCursorAndViews();
	pDoc->SetModifiedFlag();
	return 0;
}

UINT LaunchHighFidelityModelForValidation(LPVOID pParam)
{
	CDRMBuilderDoc* pDoc = (CDRMBuilderDoc*)pParam;
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->bthread = true;
	pmanager->RunMatlabHighFidelityModel(1);
	pmanager->bthread = false;
	pDoc->UpdateCursorAndViews();
	pDoc->SetModifiedFlag();
	return 0;
}

UINT GenerateDRM(LPVOID pParam)
{
	CDRMBuilderDoc* pDoc = (CDRMBuilderDoc*)pParam;
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->bthread = true;
	pmanager->GenerateDRM();
	pmanager->bthread = false;
	pDoc->UpdateCursorAndViews();
	pDoc->SetModifiedFlag();
	return 0;
}

UINT PlotDataForTraining(LPVOID pParam)
{
	CDRMBuilderDoc* pDoc = (CDRMBuilderDoc*)pParam;
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->bthread = true;
	pmanager->RunMatlabPlotScript(0);
	pmanager->bthread = false;
	pDoc->UpdateCursorAndViews();
	return 0;
}

UINT PlotDataForValidation(LPVOID pParam)
{
	CDRMBuilderDoc* pDoc = (CDRMBuilderDoc*)pParam;
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->bthread = true;
	pmanager->RunMatlabPlotScript(1);
	pmanager->bthread = false;
	pDoc->UpdateCursorAndViews();
	return 0;
}

UINT CalculateDRMUQ(LPVOID pParam)
{
	CDRMBuilderDoc* pDoc = (CDRMBuilderDoc*)pParam;
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->bthread = true;
	pmanager->RunMatlabUQScript();
	pmanager->bthread = false;
	pDoc->UpdateCursorAndViews();
	return 0;
}