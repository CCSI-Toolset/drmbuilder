// DRMBuilderView.h : interface of the CDRMBuilderView class
//


#pragma once


class CDRMBuilderView : public CScrollView
{
protected: // create from serialization only
	CDRMBuilderView();
	DECLARE_DYNCREATE(CDRMBuilderView)

// Attributes
public:
	CDRMBuilderDoc* GetDocument() const;

	//mydata
	bool bbusy;				//set to true when doing something slow
	int htext;				//height of text, depending on the number of lines displayed
	CRect wrect;			//window size rect, used to initialize the scroll view, fixed once assigned
	CFont afont;			//specify the text fond in main client window
	//end of my data

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CDRMBuilderView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetupInputvariables();
	afx_msg void OnUpdateSetupInputvariables(CCmdUI *pCmdUI);
	afx_msg void OnSetupChooseAcm();
	afx_msg void OnUpdateSetupChooseAcm(CCmdUI *pCmdUI);
	afx_msg void OnSetupOutputvariables();
	afx_msg void OnUpdateSetupOutputvariables(CCmdUI *pCmdUI);
	afx_msg void OnSetupPrepareTrainingSequence();
	afx_msg void OnUpdateSetupPrepareTrainingSequence(CCmdUI *pCmdUI);
	afx_msg void OnSetupPrepareValidationSequence();
	afx_msg void OnUpdateSetupPrepareValidationSequence(CCmdUI *pCmdUI);
	afx_msg void OnBuildLaunchTrainingSimulation();
	afx_msg void OnUpdateBuildLaunchTrainingSimulation(CCmdUI *pCmdUI);
	afx_msg void OnBuildLaunchValidationSimulation();
	afx_msg void OnUpdateBuildLaunchValidationSimulation(CCmdUI *pCmdUI);
	afx_msg void OnBuildGenerateReducedModel();
	afx_msg void OnUpdateBuildGenerateReducedModel(CCmdUI *pCmdUI);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSetupChooseDml();
	afx_msg void OnUpdateSetupChooseDml(CCmdUI *pCmdUI);
	afx_msg void OnPostprocessPredictTraining();
	afx_msg void OnUpdatePostprocessPredictTraining(CCmdUI *pCmdUI);
	afx_msg void OnPostprocessPredictValidation();
	afx_msg void OnUpdatePostprocessPredictValidation(CCmdUI *pCmdUI);
	afx_msg void OnPostprocessCompareTraining();
	afx_msg void OnUpdatePostprocessCompareTraining(CCmdUI *pCmdUI);
	afx_msg void OnPostprocessCompareValidation();
	afx_msg void OnUpdatePostprocessCompareValidation(CCmdUI *pCmdUI);
	afx_msg void OnViewScrolltobottom();
	afx_msg void OnFileExportDrm();
	afx_msg void OnUpdateFileExportDrm(CCmdUI *pCmdUI);
	afx_msg void OnPostprocessUsebalancedmodel();
	afx_msg void OnUpdatePostprocessUsebalancedmodel(CCmdUI *pCmdUI);
	afx_msg void OnFileExportlaguerreweightmatrices();
	afx_msg void OnUpdateFileExportlaguerreweightmatrices(CCmdUI *pCmdUI);
	afx_msg void OnDDabnet();
	afx_msg void OnUpdateDDabnet(CCmdUI *pCmdUI);
	afx_msg void OnDNarma();
	afx_msg void OnUpdateDNarma(CCmdUI *pCmdUI);
	afx_msg void OnChoosehighAcmsnapshotfile();
	afx_msg void OnUpdateChoosehighAcmsnapshotfile(CCmdUI *pCmdUI);
	afx_msg void OnFileExportLog();
	afx_msg void OnUpdateFileExportLog(CCmdUI *pCmdUI);
	afx_msg void OnFileExporttrainingdata();
	afx_msg void OnUpdateFileExporttrainingdata(CCmdUI *pCmdUI);
	afx_msg void OnFileExportvalidationdata();
	afx_msg void OnUpdateFileExportvalidationdata(CCmdUI *pCmdUI);
	afx_msg void OnFileExportacmfile();
	afx_msg void OnUpdateFileExportacmfile(CCmdUI *pCmdUI);
	afx_msg void OnFileExportacmsnapshotfile();
	afx_msg void OnUpdateFileExportacmsnapshotfile(CCmdUI *pCmdUI);
	afx_msg void OnFileExportsimulinkfile();
	afx_msg void OnUpdateFileExportsimulinkfile(CCmdUI *pCmdUI);
	afx_msg void OnPostprocessUsehighfidelityhistory();
	afx_msg void OnUpdatePostprocessUsehighfidelityhistory(CCmdUI *pCmdUI);
	afx_msg void OnUqCalculate();
	afx_msg void OnUpdateUqCalculate(CCmdUI *pCmdUI);
	afx_msg void OnUqSetnoise();
	afx_msg void OnUpdateUqSetnoise(CCmdUI *pCmdUI);
	afx_msg void OnExportCovariancematrices();
	afx_msg void OnUpdateExportCovariancematrices(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in DRMBuilderView.cpp
inline CDRMBuilderDoc* CDRMBuilderView::GetDocument() const
   { return reinterpret_cast<CDRMBuilderDoc*>(m_pDocument); }
#endif

