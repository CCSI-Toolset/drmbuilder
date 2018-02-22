// DRMBuilderDoc.h : interface of the CDRMBuilderDoc class
//


#pragma once

#include "DRMManager.h"

class CDRMBuilderDoc : public CDocument
{
protected: // create from serialization only
	CDRMBuilderDoc();
	DECLARE_DYNCREATE(CDRMBuilderDoc)

// Attributes
public:
	CDRMManager m_manager;		//contains all data

// Operations
public:
	//my function
	void UpdateCursorAndViews();
	void StartHighFidelityModelThreadForTraining();
	void StartHighFidelityModelThreadForValidation();
	void StartDRMGenerationThread();
	void StartMatlabPlotThreadForTraining();
	void StartMatlabPlotThreadForValidation();
	void StartMatlabUQThreadForValidation();
	//end of my function

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CDRMBuilderDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
};


