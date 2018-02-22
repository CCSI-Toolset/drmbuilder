// DRMBuilderDoc.cpp : implementation of the CDRMBuilderDoc class
//

#include "stdafx.h"
#include "DRMBuilder.h"

#include "ThreadFunc.h"

#include "DRMBuilderDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDRMBuilderDoc

IMPLEMENT_DYNCREATE(CDRMBuilderDoc, CDocument)

BEGIN_MESSAGE_MAP(CDRMBuilderDoc, CDocument)
END_MESSAGE_MAP()


// CDRMBuilderDoc construction/destruction

CDRMBuilderDoc::CDRMBuilderDoc()
{
	// TODO: add one-time construction code here

}

CDRMBuilderDoc::~CDRMBuilderDoc()
{
}

BOOL CDRMBuilderDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CDRMBuilderDoc serialization

void CDRMBuilderDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CDRMBuilderDoc diagnostics

#ifdef _DEBUG
void CDRMBuilderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDRMBuilderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDRMBuilderDoc commands
void CDRMBuilderDoc::UpdateCursorAndViews()
{
	POSITION pos = GetFirstViewPosition();
	CView* pView = NULL;
	while (pos!=NULL)
	{
		pView = GetNextView( pos );
		pView->Invalidate();
	}
	if (pView!=NULL) pView->SendMessage(WM_SETCURSOR);
}

void CDRMBuilderDoc::StartHighFidelityModelThreadForTraining()
{
	if (!m_manager.bthread)
		m_manager.pthread = AfxBeginThread(LaunchHighFidelityModelForTraining,(LPVOID)this,THREAD_PRIORITY_BELOW_NORMAL);
}

void CDRMBuilderDoc::StartHighFidelityModelThreadForValidation()
{
	if (!m_manager.bthread)
		m_manager.pthread = AfxBeginThread(LaunchHighFidelityModelForValidation,(LPVOID)this,THREAD_PRIORITY_BELOW_NORMAL);
}

void CDRMBuilderDoc::StartDRMGenerationThread()
{
	if (!m_manager.bthread)
		m_manager.pthread = AfxBeginThread(GenerateDRM,(LPVOID)this,THREAD_PRIORITY_BELOW_NORMAL);
}

void CDRMBuilderDoc::StartMatlabPlotThreadForTraining()
{
	if (!m_manager.bthread)
		m_manager.pthread = AfxBeginThread(PlotDataForTraining,(LPVOID)this,THREAD_PRIORITY_BELOW_NORMAL);
}

void CDRMBuilderDoc::StartMatlabPlotThreadForValidation()
{
	if (!m_manager.bthread)
		m_manager.pthread = AfxBeginThread(PlotDataForValidation,(LPVOID)this,THREAD_PRIORITY_BELOW_NORMAL);
}

void CDRMBuilderDoc::StartMatlabUQThreadForValidation()
{
	if (!m_manager.bthread)
		m_manager.pthread = AfxBeginThread(CalculateDRMUQ,(LPVOID)this,THREAD_PRIORITY_BELOW_NORMAL);
}

BOOL CDRMBuilderDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	int iversion;
	FILE* pf;
	if ((pf = fopen(lpszPathName,"rb"))==NULL)
		return FALSE;
	//read data
	fread(&iversion,sizeof(int),1,pf);
	m_manager.Read(pf);
	fclose(pf);
	//set current directory
	CString strPath = lpszPathName;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	::SetCurrentDirectory(strPath);
	m_manager.foldername = strPath;
	//end of my code
	return TRUE;
}

BOOL CDRMBuilderDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: Add your specialized code here and/or call the base class
	int iversion = 0;
	FILE* pf;
	if ((pf = fopen(lpszPathName,"wb"))==NULL)
		return FALSE;
	//write data
	fwrite(&iversion,sizeof(int),1,pf);
	m_manager.Write(pf);
	fclose(pf);
	//set current directory
	CString strPath = lpszPathName;
	strPath = strPath.Left(strPath.ReverseFind('\\'));
	::SetCurrentDirectory(strPath);
	m_manager.foldername = strPath;
	SetModifiedFlag(FALSE);
	return TRUE;
	//end of my code
	//skip default call below. otherwise, the code will call Serialize to purge what has been written
	//return CDocument::OnSaveDocument(lpszPathName);
}
