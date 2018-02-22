// DRMBuilderView.cpp : implementation of the CDRMBuilderView class
//

#include "stdafx.h"
#include "DRMBuilder.h"

#include "InputVariableDlg.h"
#include "OutputVariableDlg.h"
#include "SolverInputDlg.h"
#include "DRMParameterDlg.h"
#include "NarmaParameterDlg.h"
#include "PlotResultDlg.h"
#include "AcmPathNameDlg.h"
#include "NoiseDlg.h"

#include "DRMBuilderDoc.h"
#include "DRMBuilderView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDRMBuilderView

IMPLEMENT_DYNCREATE(CDRMBuilderView, CScrollView)

BEGIN_MESSAGE_MAP(CDRMBuilderView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(ID_SETUP_INPUTVARIABLES, &CDRMBuilderView::OnSetupInputvariables)
	ON_UPDATE_COMMAND_UI(ID_SETUP_INPUTVARIABLES, &CDRMBuilderView::OnUpdateSetupInputvariables)
	ON_COMMAND(ID_SETUP_CHOOSE_ACM, &CDRMBuilderView::OnSetupChooseAcm)
	ON_UPDATE_COMMAND_UI(ID_SETUP_CHOOSE_ACM, &CDRMBuilderView::OnUpdateSetupChooseAcm)
	ON_COMMAND(ID_SETUP_OUTPUTVARIABLES, &CDRMBuilderView::OnSetupOutputvariables)
	ON_UPDATE_COMMAND_UI(ID_SETUP_OUTPUTVARIABLES, &CDRMBuilderView::OnUpdateSetupOutputvariables)
	ON_COMMAND(ID_SETUP_PREPARE_TRAINING_SEQUENCE, &CDRMBuilderView::OnSetupPrepareTrainingSequence)
	ON_UPDATE_COMMAND_UI(ID_SETUP_PREPARE_TRAINING_SEQUENCE, &CDRMBuilderView::OnUpdateSetupPrepareTrainingSequence)
	ON_COMMAND(ID_SETUP_PREPARE_VALIDATION_SEQUENCE, &CDRMBuilderView::OnSetupPrepareValidationSequence)
	ON_UPDATE_COMMAND_UI(ID_SETUP_PREPARE_VALIDATION_SEQUENCE, &CDRMBuilderView::OnUpdateSetupPrepareValidationSequence)
	ON_COMMAND(ID_BUILD_LAUNCH_TRAINING_SIMULATION, &CDRMBuilderView::OnBuildLaunchTrainingSimulation)
	ON_UPDATE_COMMAND_UI(ID_BUILD_LAUNCH_TRAINING_SIMULATION, &CDRMBuilderView::OnUpdateBuildLaunchTrainingSimulation)
	ON_COMMAND(ID_BUILD_LAUNCH_VALIDATION_SIMULATION, &CDRMBuilderView::OnBuildLaunchValidationSimulation)
	ON_UPDATE_COMMAND_UI(ID_BUILD_LAUNCH_VALIDATION_SIMULATION, &CDRMBuilderView::OnUpdateBuildLaunchValidationSimulation)
	ON_COMMAND(ID_BUILD_GENERATE_REDUCED_MODEL, &CDRMBuilderView::OnBuildGenerateReducedModel)
	ON_UPDATE_COMMAND_UI(ID_BUILD_GENERATE_REDUCED_MODEL, &CDRMBuilderView::OnUpdateBuildGenerateReducedModel)
	ON_WM_SETCURSOR()
	ON_COMMAND(ID_SETUP_CHOOSE_DML, &CDRMBuilderView::OnSetupChooseDml)
	ON_UPDATE_COMMAND_UI(ID_SETUP_CHOOSE_DML, &CDRMBuilderView::OnUpdateSetupChooseDml)
	ON_COMMAND(ID_POSTPROCESS_PREDICT_TRAINING, &CDRMBuilderView::OnPostprocessPredictTraining)
	ON_UPDATE_COMMAND_UI(ID_POSTPROCESS_PREDICT_TRAINING, &CDRMBuilderView::OnUpdatePostprocessPredictTraining)
	ON_COMMAND(ID_POSTPROCESS_PREDICT_VALIDATION, &CDRMBuilderView::OnPostprocessPredictValidation)
	ON_UPDATE_COMMAND_UI(ID_POSTPROCESS_PREDICT_VALIDATION, &CDRMBuilderView::OnUpdatePostprocessPredictValidation)
	ON_COMMAND(ID_POSTPROCESS_COMPARE_TRAINING, &CDRMBuilderView::OnPostprocessCompareTraining)
	ON_UPDATE_COMMAND_UI(ID_POSTPROCESS_COMPARE_TRAINING, &CDRMBuilderView::OnUpdatePostprocessCompareTraining)
	ON_COMMAND(ID_POSTPROCESS_COMPARE_VALIDATION, &CDRMBuilderView::OnPostprocessCompareValidation)
	ON_UPDATE_COMMAND_UI(ID_POSTPROCESS_COMPARE_VALIDATION, &CDRMBuilderView::OnUpdatePostprocessCompareValidation)
	ON_COMMAND(ID_VIEW_SCROLLTOBOTTOM, &CDRMBuilderView::OnViewScrolltobottom)
	ON_COMMAND(ID_FILE_EXPORT_DRM, &CDRMBuilderView::OnFileExportDrm)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_DRM, &CDRMBuilderView::OnUpdateFileExportDrm)
	ON_COMMAND(ID_POSTPROCESS_USEBALANCEDMODEL, &CDRMBuilderView::OnPostprocessUsebalancedmodel)
	ON_UPDATE_COMMAND_UI(ID_POSTPROCESS_USEBALANCEDMODEL, &CDRMBuilderView::OnUpdatePostprocessUsebalancedmodel)
	ON_COMMAND(ID_FILE_EXPORTLAGUERREWEIGHTMATRICES, &CDRMBuilderView::OnFileExportlaguerreweightmatrices)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTLAGUERREWEIGHTMATRICES, &CDRMBuilderView::OnUpdateFileExportlaguerreweightmatrices)
	ON_COMMAND(ID_D_DABNET, &CDRMBuilderView::OnDDabnet)
	ON_UPDATE_COMMAND_UI(ID_D_DABNET, &CDRMBuilderView::OnUpdateDDabnet)
	ON_COMMAND(ID_D_NARMA, &CDRMBuilderView::OnDNarma)
	ON_UPDATE_COMMAND_UI(ID_D_NARMA, &CDRMBuilderView::OnUpdateDNarma)
	ON_COMMAND(ID_CHOOSEHIGH_ACMSNAPSHOTFILE, &CDRMBuilderView::OnChoosehighAcmsnapshotfile)
	ON_UPDATE_COMMAND_UI(ID_CHOOSEHIGH_ACMSNAPSHOTFILE, &CDRMBuilderView::OnUpdateChoosehighAcmsnapshotfile)
	ON_COMMAND(ID_FILE_EXPORT_LOG, &CDRMBuilderView::OnFileExportLog)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_LOG, &CDRMBuilderView::OnUpdateFileExportLog)
	ON_COMMAND(ID_FILE_EXPORTTRAININGDATA, &CDRMBuilderView::OnFileExporttrainingdata)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTTRAININGDATA, &CDRMBuilderView::OnUpdateFileExporttrainingdata)
	ON_COMMAND(ID_FILE_EXPORTVALIDATIONDATA, &CDRMBuilderView::OnFileExportvalidationdata)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTVALIDATIONDATA, &CDRMBuilderView::OnUpdateFileExportvalidationdata)
	ON_COMMAND(ID_FILE_EXPORTACMFILE, &CDRMBuilderView::OnFileExportacmfile)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTACMFILE, &CDRMBuilderView::OnUpdateFileExportacmfile)
	ON_COMMAND(ID_FILE_EXPORTACMSNAPSHOTFILE, &CDRMBuilderView::OnFileExportacmsnapshotfile)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTACMSNAPSHOTFILE, &CDRMBuilderView::OnUpdateFileExportacmsnapshotfile)
	ON_COMMAND(ID_FILE_EXPORTSIMULINKFILE, &CDRMBuilderView::OnFileExportsimulinkfile)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTSIMULINKFILE, &CDRMBuilderView::OnUpdateFileExportsimulinkfile)
	ON_COMMAND(ID_POSTPROCESS_USEHIGHFIDELITYHISTORY, &CDRMBuilderView::OnPostprocessUsehighfidelityhistory)
	ON_UPDATE_COMMAND_UI(ID_POSTPROCESS_USEHIGHFIDELITYHISTORY, &CDRMBuilderView::OnUpdatePostprocessUsehighfidelityhistory)
	ON_COMMAND(ID_UQ_CALCULATE, &CDRMBuilderView::OnUqCalculate)
	ON_UPDATE_COMMAND_UI(ID_UQ_CALCULATE, &CDRMBuilderView::OnUpdateUqCalculate)
	ON_COMMAND(ID_UQ_SETNOISE, &CDRMBuilderView::OnUqSetnoise)
	ON_UPDATE_COMMAND_UI(ID_UQ_SETNOISE, &CDRMBuilderView::OnUpdateUqSetnoise)
	ON_COMMAND(ID_EXPORT_COVARIANCEMATRICES, &CDRMBuilderView::OnExportCovariancematrices)
	ON_UPDATE_COMMAND_UI(ID_EXPORT_COVARIANCEMATRICES, &CDRMBuilderView::OnUpdateExportCovariancematrices)
END_MESSAGE_MAP()

// CDRMBuilderView construction/destruction

CDRMBuilderView::CDRMBuilderView()
{
	// TODO: add construction code here
	bbusy = false;
	htext = 0;
	int hfont = 14;
	int hline = hfont + 2;
	int hclient = hline*200;	//total log line should not allowed to exceed 200, otherwise ScrollToBottom() will not work
	wrect.SetRect(0,0,2000,hclient);
	afont.CreateFont(hline,0,0,0,500,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_MODERN,"Courier New");
}

CDRMBuilderView::~CDRMBuilderView()
{
}

BOOL CDRMBuilderView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CDRMBuilderView drawing

void CDRMBuilderView::OnDraw(CDC* pDC)
{
	CDRMBuilderDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	pDC->SelectObject(&afont);
	htext = pDC->DrawText(pDoc->m_manager.msg.slog_cur,wrect,DT_LEFT);
}


// CDRMBuilderView printing

BOOL CDRMBuilderView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDRMBuilderView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDRMBuilderView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CDRMBuilderView diagnostics

#ifdef _DEBUG
void CDRMBuilderView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDRMBuilderView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDRMBuilderDoc* CDRMBuilderView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDRMBuilderDoc)));
	return (CDRMBuilderDoc*)m_pDocument;
}
#endif //_DEBUG


// CDRMBuilderView message handlers

void CDRMBuilderView::OnSetupInputvariables()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CInputVariableDlg dlg;
	dlg.m_time_unit_acm = pDoc->m_manager.time_unit_acm;
	dlg.m_dt_sampling = (float)pDoc->m_manager.si_train.GetSamplingTimeInterval();
	dlg.m_dt_min_solver = (float)pDoc->m_manager.si_train.GetSolverMinimumTimeStep();
	dlg.m_poldlist = &pDoc->m_manager.input_list_all;
	if (dlg.DoModal()==IDOK)
	{
		//edit the DML file to set the sampling time interval
		pDoc->m_manager.SetMDLSamplingTimeInterval((double)dlg.m_dt_sampling);
		pDoc->m_manager.si_train.SetSamplingTimeInterval((double)dlg.m_dt_sampling);
		pDoc->m_manager.si_valid.SetSamplingTimeInterval((double)dlg.m_dt_sampling);
		pDoc->m_manager.UpdateSelectedInputList();
		pDoc->m_manager.msg.AddMessageLine("Input variables to be built in D-RM have been specified.\n");
		pDoc->m_manager.binputready = true;
		pDoc->m_manager.bsequence_train = false;
		pDoc->m_manager.bsequence_valid = false;
		pDoc->m_manager.brun_train = false;
		pDoc->m_manager.brun_valid = false;
		pDoc->m_manager.bbuilt_dabnet = false;
		pDoc->m_manager.bbuilt_narma = false;
		pDoc->m_manager.bpredict_train = false;
		pDoc->m_manager.bpredict_valid = false;
		pDoc->SetModifiedFlag();
	}
}

void CDRMBuilderView::OnUpdateSetupInputvariables(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.bhfm_file && pDoc->m_manager.bmdl_file);
}

void CDRMBuilderView::OnSetupChooseAcm()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CString filter = "acm Files (*.acmf)|*.acmf||";
	CFileDialog dlg(TRUE,"acmf",NULL,OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		pDoc->m_manager.filename_acm = dlg.GetFileName();
		pDoc->m_manager.ReadTextFileToString(dlg.GetPathName(),pDoc->m_manager.cstring_acm);
		pDoc->m_manager.msg.AddMessageLine("ACM file " + dlg.GetPathName() + " has been read.\n");
		pDoc->m_manager.bhfm_file = true;
		pDoc->m_manager.basnp_file = false;
		pDoc->m_manager.bmdl_file = false;
		pDoc->m_manager.binputready = false;
		pDoc->m_manager.boutputready = false;
		pDoc->m_manager.bsequence_train = false;
		pDoc->m_manager.bsequence_valid = false;
		pDoc->m_manager.brun_train = false;
		pDoc->m_manager.brun_valid = false;
		pDoc->m_manager.bbuilt_dabnet = false;
		pDoc->m_manager.bbuilt_narma = false;
		pDoc->m_manager.bpredict_train = false;
		pDoc->m_manager.bpredict_valid = false;
		pDoc->SetModifiedFlag();
	}
}

void CDRMBuilderView::OnUpdateSetupChooseAcm(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread); 
}

void CDRMBuilderView::OnSetupOutputvariables()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	COutputVariableDlg dlg;
	dlg.m_poldlist = &pDoc->m_manager.output_list_all;
	if (dlg.DoModal()==IDOK)
	{
		pDoc->m_manager.UpdateSelectedOutputList();
		pDoc->m_manager.msg.AddMessageLine("Output variables to be built in D-RM have been specified.\n");
		pDoc->m_manager.boutputready = true;
		pDoc->m_manager.bbuilt_dabnet = false;
		pDoc->m_manager.bbuilt_narma = false;
		pDoc->m_manager.bpredict_train = false;
		pDoc->m_manager.bpredict_valid = false;
		pDoc->SetModifiedFlag();
	}
}

void CDRMBuilderView::OnUpdateSetupOutputvariables(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.bhfm_file && pDoc->m_manager.bmdl_file);
}

void CDRMBuilderView::OnSetupPrepareTrainingSequence()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CSolverInputDlg dlg;
	dlg.m_bvalidation = false;
	dlg.m_time_unit_acm = pDoc->m_manager.time_unit_acm;
	dlg.m_unit_dt_min_solver = pDoc->m_manager.time_unit_acm;	//assume same unit as communication time
	dlg.m_dt_sampling = (float)pDoc->m_manager.si_train.GetSamplingTimeInterval();
	dlg.m_dt_min_solver = (float)pDoc->m_manager.si_train.GetSolverMinimumTimeStep();
	dlg.m_ireverse = pDoc->m_manager.si_train.GetReverseFlag();
	dlg.m_npoint = pDoc->m_manager.si_train.GetNumberOfPoints();
	dlg.m_pvduration = pDoc->m_manager.si_train.GetDurationVector();
	if (dlg.DoModal()==IDOK)
	{
		pDoc->m_manager.si_train.EnableReverse(dlg.m_ireverse);
		pDoc->m_manager.si_train.SetNumberOfPoints(dlg.m_npoint);
		pDoc->m_manager.si_train.SetDurationVector(&dlg.m_vduration);
		pDoc->m_manager.PrepareTrainingSequence();
		pDoc->m_manager.msg.AddMessageLine("Step change sequence for training has been generated.\n");
		pDoc->m_manager.bsequence_train = true;
		pDoc->m_manager.bsequence_valid = false;
		pDoc->m_manager.brun_train = false;
		pDoc->m_manager.brun_valid = false;
		pDoc->m_manager.bbuilt_dabnet = false;
		pDoc->m_manager.bbuilt_narma = false;
		pDoc->m_manager.bpredict_train = false;
		pDoc->m_manager.bpredict_valid = false;
		pDoc->SetModifiedFlag();
	}
}

void CDRMBuilderView::OnUpdateSetupPrepareTrainingSequence(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.binputready);
}

void CDRMBuilderView::OnSetupPrepareValidationSequence()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CSolverInputDlg dlg;
	dlg.m_bvalidation = true;
	dlg.m_time_unit_acm = pDoc->m_manager.time_unit_acm;
	dlg.m_unit_dt_min_solver = pDoc->m_manager.time_unit_acm;	//assume same unit as communication time
	dlg.m_dt_sampling = (float)pDoc->m_manager.si_valid.GetSamplingTimeInterval();
	dlg.m_dt_min_solver = (float)pDoc->m_manager.si_train.GetSolverMinimumTimeStep();
	dlg.m_ireverse = pDoc->m_manager.si_valid.GetReverseFlag();
	dlg.m_npoint = pDoc->m_manager.si_valid.GetNumberOfPoints();
	dlg.m_pvduration = pDoc->m_manager.si_valid.GetDurationVector();
	if (dlg.DoModal()==IDOK)
	{
		//dt for sampling time interval is not updated
		pDoc->m_manager.si_valid.EnableReverse(dlg.m_ireverse);
		pDoc->m_manager.si_valid.SetNumberOfPoints(dlg.m_npoint);
		pDoc->m_manager.si_valid.SetDurationVector(&dlg.m_vduration);
		pDoc->m_manager.PrepareValidationSequence();
		pDoc->m_manager.msg.AddMessageLine("Step change sequence for validation has been generated.\n");
		pDoc->m_manager.bsequence_valid = true;
		pDoc->m_manager.brun_valid = false;
		pDoc->m_manager.bpredict_valid = false;
		pDoc->SetModifiedFlag();
	}
}

void CDRMBuilderView::OnUpdateSetupPrepareValidationSequence(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.binputready);
}

void CDRMBuilderView::OnBuildLaunchTrainingSimulation()
{
	// TODO: Add your command handler code here
	//first check if ID_Input_Training.csv exist
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->PrepareCommandParameterCellArray(0,0);		//for training, high-fidelity response
	pmanager->msg.AddMessageLine("Starting to perform high-fidelity model training simulation...\n");
	pmanager->msg.AddMessageLine("Please wait until commands in MATLAB script are completed. Then close MATLAB window by clicking the \"OK\" button on the pop-up message window.\n");
	bbusy = true;
	SendMessage(WM_SETCURSOR);
	pDoc->StartHighFidelityModelThreadForTraining();
	bbusy = false;
	SendMessage(WM_SETCURSOR);
	pmanager->brun_train = true;
	pmanager->bbuilt_dabnet = false;
	pmanager->bbuilt_narma = false;
	pmanager->bpredict_train = false;
	pmanager->bpredict_valid = false;
	pDoc->SetModifiedFlag();
}

void CDRMBuilderView::OnUpdateBuildLaunchTrainingSimulation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.bsequence_train);
}

void CDRMBuilderView::OnBuildLaunchValidationSimulation()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->PrepareCommandParameterCellArray(1,0);		//for validation, high-fidelity response
	pmanager->msg.AddMessageLine("Starting to perform high-fidelity model validation simulation...\n");
	pmanager->msg.AddMessageLine("Please wait until commands in MATLAB script are completed. Then close MATLAB window by clicking the \"OK\" button on the pop-up message window.\n");
	pDoc->StartHighFidelityModelThreadForValidation();
	pmanager->brun_valid = true;
	pmanager->bpredict_valid = false;
	pDoc->SetModifiedFlag();
}

void CDRMBuilderView::OnUpdateBuildLaunchValidationSimulation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.bsequence_valid);
}

void CDRMBuilderView::OnBuildGenerateReducedModel()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	if (pDoc->m_manager.drm_container.imodel_type==0)	//DABNet
	{
		CDRMParameterDlg dlg;
		dlg.m_pinput_list_sel = &pDoc->m_manager.input_list_sel;
		dlg.m_poutput_list_sel = &pDoc->m_manager.output_list_sel;
		dlg.m_dabnet_input = pDoc->m_manager.drm_container.dabnet_input;
		dlg.m_pdrm_dabnet = &pDoc->m_manager.drm_container.drm_dabnet;
		if (dlg.DoModal()==IDOK)
		{
			pDoc->m_manager.drm_container.dabnet_input = dlg.m_dabnet_input;
			//other parameters in drm_dabnet vector has been updated
			//generate model
			bbusy = true;
			SendMessage(WM_SETCURSOR);
			pDoc->StartDRMGenerationThread();
			bbusy = false;
			SendMessage(WM_SETCURSOR);
			pDoc->m_manager.bbuilt_dabnet = true;
			pDoc->m_manager.bpredict_train = false;
			pDoc->m_manager.bpredict_valid = false;
			pDoc->SetModifiedFlag();
		}
	}
	else	//NARMA model
	{
		CNarmaParameterDlg dlg;
		dlg.m_nneuron_hid = pDoc->m_manager.drm_container.drm_narma.nneuron_hid;
		dlg.m_nmax_iter_bp = pDoc->m_manager.drm_container.drm_narma.nmax_iter_bp;
		dlg.m_nhistory = pDoc->m_manager.drm_container.drm_narma.nhistory;
		if (dlg.DoModal()==IDOK)
		{
			pDoc->m_manager.drm_container.drm_narma.nneuron_hid = dlg.m_nneuron_hid;
			pDoc->m_manager.drm_container.drm_narma.nmax_iter_bp = dlg.m_nmax_iter_bp;
			pDoc->m_manager.drm_container.drm_narma.nhistory = dlg.m_nhistory;
			//generate model
			bbusy = true;
			SendMessage(WM_SETCURSOR);
			pDoc->StartDRMGenerationThread();
			bbusy = false;
			SendMessage(WM_SETCURSOR);
			pDoc->m_manager.bbuilt_narma = true;
			pDoc->m_manager.bpredict_train = false;
			pDoc->m_manager.bpredict_valid = false;
			pDoc->SetModifiedFlag();
		}
	}
}

void CDRMBuilderView::OnUpdateBuildGenerateReducedModel(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.brun_train && pDoc->m_manager.boutputready);
}

BOOL CDRMBuilderView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default
	CDRMBuilderDoc* pDoc = GetDocument();
	CWinApp* pApp = AfxGetApp();
	HICON hIcon;
	if (pDoc->m_manager.bthread || bbusy)
	{
		hIcon = pApp->LoadStandardCursor(IDC_WAIT);
		SetCursor(hIcon);
		return TRUE;
	}
	//end of my code
	return CScrollView::OnSetCursor(pWnd, nHitTest, message);
}

void CDRMBuilderView::OnSetupChooseDml()
{
	// TODO: Add your command handler code here
	int i;
	CString str;
	CDRMBuilderDoc* pDoc = GetDocument();
	CString filter = "mdl Files (*.mdl)|*.mdl||";
	CFileDialog dlg(TRUE,"mdl",NULL,OFN_FILEMUSTEXIST,filter);	//without OFN_NOCHANGEDIR option, will cause working directory change
	if (dlg.DoModal()==IDOK)
	{
		pDoc->m_manager.ReadTextFileToString(dlg.GetPathName(),pDoc->m_manager.cstring_mdl);
		str = dlg.GetPathName();
		pDoc->m_manager.msg.AddMessageLine("Simulink file " + str + " has been read.\n");
		i = str.ReverseFind('\\');
		pDoc->m_manager.foldername = str.Left(i);
		pDoc->m_manager.bmdl_file = true;
		pDoc->m_manager.binputready = false;
		pDoc->m_manager.boutputready = false;
		pDoc->m_manager.bsequence_train = false;
		pDoc->m_manager.bsequence_valid = false;
		pDoc->m_manager.brun_train = false;
		pDoc->m_manager.brun_valid = false;
		pDoc->m_manager.bbuilt_dabnet = false;
		pDoc->m_manager.bbuilt_narma = false;
		pDoc->m_manager.bpredict_train = false;
		pDoc->m_manager.bpredict_valid = false;
		pDoc->m_manager.SetAllAvailableInputOutputList();
		pDoc->SetModifiedFlag();
	}
}

void CDRMBuilderView::OnUpdateSetupChooseDml(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.bhfm_file && pDoc->m_manager.basnp_file);
}

void CDRMBuilderView::OnPostprocessPredictTraining()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->PredictUsingTrainingData();
	pmanager->msg.AddMessageLine("Training response prediction has been completed.\n");
	pmanager->bpredict_train = true;
	pDoc->SetModifiedFlag();
}

void CDRMBuilderView::OnUpdatePostprocessPredictTraining(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	bool benable = !pDoc->m_manager.bthread && pDoc->m_manager.brun_train;
	if (pDoc->m_manager.drm_container.imodel_type==0)
		benable &= pDoc->m_manager.bbuilt_dabnet;
	else
		benable &= pDoc->m_manager.bbuilt_narma;
	pCmdUI->Enable(benable);
}

void CDRMBuilderView::OnPostprocessPredictValidation()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	pmanager->PredictUsingValidationData();
	pmanager->msg.AddMessageLine("Validation response prediction has been completed.\n");
	pmanager->bpredict_valid = true;
	pDoc->SetModifiedFlag();
}

void CDRMBuilderView::OnUpdatePostprocessPredictValidation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	bool benable = !pDoc->m_manager.bthread && pDoc->m_manager.brun_valid;
	if (pDoc->m_manager.drm_container.imodel_type==0)
		benable &= pDoc->m_manager.bbuilt_dabnet;
	else
		benable &= pDoc->m_manager.bbuilt_narma;
	pCmdUI->Enable(benable);
}

void CDRMBuilderView::OnPostprocessCompareTraining()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CPlotResultDlg dlg;
	dlg.m_pinput_list_all = &pDoc->m_manager.input_list_all;
	dlg.m_poutput_list_all = &pDoc->m_manager.output_list_all;
	dlg.m_berror = pDoc->m_manager.bplot_error;
	dlg.m_bstep_change = pDoc->m_manager.bplot_step_change;
	dlg.m_bcorrelation = pDoc->m_manager.bplot_correlation;
	if (dlg.DoModal()==IDOK)
	{
		pDoc->m_manager.input_to_plot = dlg.m_selected_inputs;
		pDoc->m_manager.output_to_plot = dlg.m_selected_outputs;
		pDoc->m_manager.bplot_error = dlg.m_berror;
		pDoc->m_manager.bplot_step_change = dlg.m_bstep_change;
		pDoc->m_manager.bplot_correlation = dlg.m_bcorrelation;
		pDoc->m_manager.PrepareCommandParameterCellArray(0,1);
		pDoc->m_manager.msg.AddMessageLine("Starting to plot training response in MATLAB...\n");
		pDoc->m_manager.msg.AddMessageLine("Please wait until input and output profiles are plotted. Then close MATLAB window by clicking the \"OK\" button on the pop-up message window.\n");
		bbusy = true;
		SendMessage(WM_SETCURSOR);
		pDoc->StartMatlabPlotThreadForTraining();
		bbusy = false;
		SendMessage(WM_SETCURSOR);
	}
}

void CDRMBuilderView::OnUpdatePostprocessCompareTraining(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.brun_train && pDoc->m_manager.bpredict_train);
}

void CDRMBuilderView::OnPostprocessCompareValidation()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CPlotResultDlg dlg;
	dlg.m_pinput_list_all = &pDoc->m_manager.input_list_all;
	dlg.m_poutput_list_all = &pDoc->m_manager.output_list_all;
	dlg.m_berror = pDoc->m_manager.bplot_error;
	dlg.m_bstep_change = pDoc->m_manager.bplot_step_change;
	dlg.m_bcorrelation = pDoc->m_manager.bplot_correlation;
	if (dlg.DoModal()==IDOK)
	{
		pDoc->m_manager.input_to_plot = dlg.m_selected_inputs;
		pDoc->m_manager.output_to_plot = dlg.m_selected_outputs;
		pDoc->m_manager.bplot_error = dlg.m_berror;
		pDoc->m_manager.bplot_step_change = dlg.m_bstep_change;
		pDoc->m_manager.bplot_correlation = dlg.m_bcorrelation;
		pDoc->m_manager.PrepareCommandParameterCellArray(1,1);
		pDoc->m_manager.msg.AddMessageLine("Starting to plot validation response in MATLAB...\n");
		pDoc->m_manager.msg.AddMessageLine("Please wait until input and output profiles are plotted. Then close MATLAB window by clicking the \"OK\" button on the pop-up message window.\n");
		bbusy = true;
		SendMessage(WM_SETCURSOR);
		pDoc->StartMatlabPlotThreadForValidation();
		bbusy = false;
		SendMessage(WM_SETCURSOR);
	}
}

void CDRMBuilderView::OnUpdatePostprocessCompareValidation(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.brun_valid && pDoc->m_manager.bpredict_valid);
}

void CDRMBuilderView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	CSize doc_size(wrect.Width(),wrect.Height());
	SetScrollSizes(MM_TEXT, doc_size);
	CDRMBuilderDoc* pDoc = GetDocument();
	pDoc->m_manager.msg.pview = this;
}

void CDRMBuilderView::OnViewScrolltobottom()
{
	// TODO: Add your command handler code here
	//htext should not exceed the scroll size defined in OnInitialUpdata()
	CRect rect;
	CPoint point(0,0);
	GetClientRect(rect);
	if (htext>rect.bottom-64)
		point.y = htext - rect.bottom + 64;
	ScrollToPosition(point);
}

void CDRMBuilderView::OnFileExportDrm()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CString str = pDoc->GetTitle();
	int idot = str.ReverseFind('.');
	if (idot!=-1)
		str = str.Left(idot);
	str += "_DRMParameters.m";
	CString filter = "DRM as Matlab m Files (*.m)|*.m||";
	char szCurPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	str = (CString)szCurPath + "\\" + str;
	CFileDialog dlg(FALSE,"m",str,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		str = dlg.GetPathName();
		FILE* pf = fopen(str,"w");
		if (pf!=NULL)
		{
			pDoc->m_manager.WriteDRMMatlabFile(pf);
			fclose(pf);
		}
	}
}

void CDRMBuilderView::OnUpdateFileExportDrm(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	bool benable = !pDoc->m_manager.bthread;
	if (pDoc->m_manager.drm_container.imodel_type==0)	//DABNet
		benable &= pDoc->m_manager.bbuilt_dabnet;
	else		//NARMA
		benable &= pDoc->m_manager.bbuilt_narma;
	pCmdUI->Enable(benable);
}

void CDRMBuilderView::OnPostprocessUsebalancedmodel()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDabnetInput* pdabnet_input = &pDoc->m_manager.drm_container.dabnet_input;
	if (pdabnet_input->ipredict_opt)
		pdabnet_input->ipredict_opt = 0;
	else
		pdabnet_input->ipredict_opt = 1;
	pDoc->SetModifiedFlag();
}

void CDRMBuilderView::OnUpdatePostprocessUsebalancedmodel(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->m_manager.drm_container.dabnet_input.ipredict_opt);
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.drm_container.imodel_type==0);
}

void CDRMBuilderView::OnFileExportlaguerreweightmatrices()
{
	
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	CString str = pDoc->GetTitle();
	int idot = str.ReverseFind('.');
	if (idot!=-1)
		str = str.Left(idot);
	str += ".txt";
	CString filter = "Laguerre weight matrix text files (*.txt)|*.txt||";
	char szCurPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	str = (CString)szCurPath + "\\" + str;
	CFileDialog dlg(FALSE,"txt",str,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();
		FILE* pf = fopen(pathname,"w");
		if (pf!=NULL)
		{
			pDoc->m_manager.drm_container.WriteWeightMatrixFile(pf);
			fclose(pf);
		}
	}
}

void CDRMBuilderView::OnUpdateFileExportlaguerreweightmatrices(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.drm_container.imodel_type==0 && pDoc->m_manager.bbuilt_dabnet);
}

void CDRMBuilderView::OnDDabnet()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pDoc->m_manager.drm_container.imodel_type = 0;
}

void CDRMBuilderView::OnUpdateDDabnet(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread);
	pCmdUI->SetCheck(pDoc->m_manager.drm_container.imodel_type==0);
}

void CDRMBuilderView::OnDNarma()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pDoc->m_manager.drm_container.imodel_type = 1;
}

void CDRMBuilderView::OnUpdateDNarma(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread);
	pCmdUI->SetCheck(pDoc->m_manager.drm_container.imodel_type==1);
}

void CDRMBuilderView::OnChoosehighAcmsnapshotfile()
{
	// TODO: Add your command handler code here
	CString str;
	CDRMBuilderDoc* pDoc = GetDocument();
	CString filter = "ACM Snap Shot Files (*.asnp)|*.asnp||";
	CFileDialog dlg(TRUE,"asnp",NULL,OFN_FILEMUSTEXIST|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		pDoc->m_manager.ReadTextFileToString(dlg.GetPathName(),pDoc->m_manager.cstring_asnp);
		str = "ACM snap shot file " + dlg.GetPathName() + " has been read.\n";
		pDoc->m_manager.msg.AddMessageLine(str);
		pDoc->m_manager.basnp_file = true;
		pDoc->m_manager.bmdl_file = false;
		pDoc->m_manager.binputready = false;
		pDoc->m_manager.boutputready = false;
		pDoc->m_manager.bsequence_train = false;
		pDoc->m_manager.bsequence_valid = false;
		pDoc->m_manager.brun_train = false;
		pDoc->m_manager.brun_valid = false;
		pDoc->m_manager.bbuilt_dabnet = false;
		pDoc->m_manager.bbuilt_narma = false;
		pDoc->m_manager.bpredict_train = false;
		pDoc->m_manager.bpredict_valid = false;
		pDoc->SetModifiedFlag();
	}
}

void CDRMBuilderView::OnUpdateChoosehighAcmsnapshotfile(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.bhfm_file);
}

void CDRMBuilderView::OnFileExportLog()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CString str = pDoc->GetTitle();
	int idot = str.ReverseFind('.');
	if (idot!=-1)
		str = str.Left(idot);
	str += ".log";
	CString filter = "D-RM Builder log files (*.log)|*.log||";
	char szCurPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	str = (CString)szCurPath + "\\" + str;
	CFileDialog dlg(FALSE,"log",str,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();
		FILE* pf = fopen(pathname,"w");
		if (pf!=NULL)
		{
			pDoc->m_manager.msg.WriteLogFile(pf);
			fclose(pf);
		}
	}
}

void CDRMBuilderView::OnUpdateFileExportLog(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->m_manager.bthread);
}

void CDRMBuilderView::OnFileExporttrainingdata()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CString str = pDoc->GetTitle();
	int idot = str.ReverseFind('.');
	if (idot!=-1)
		str = str.Left(idot);
	str += ".csv";
	CString filter = "Comma delimited Excel files (*.csv)|*.csv||";
	char szCurPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	str = (CString)szCurPath + "\\" + str;
	CFileDialog dlg(FALSE,"csv",str,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();
		FILE* pf = fopen(pathname,"w");
		if (pf!=NULL)
		{
			pDoc->m_manager.WriteTrainingDataToExcel(pf);
			fclose(pf);
		}
	}
}

void CDRMBuilderView::OnUpdateFileExporttrainingdata(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	pCmdUI->Enable(!pmanager->bthread && pmanager->brun_train);
}

void CDRMBuilderView::OnFileExportvalidationdata()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CString str = pDoc->GetTitle();
	int idot = str.ReverseFind('.');
	if (idot!=-1)
		str = str.Left(idot);
	str += ".csv";
	CString filter = "Comma delimited Excel files (*.csv)|*.csv||";
	char szCurPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	str = (CString)szCurPath + "\\" + str;
	CFileDialog dlg(FALSE,"csv",str,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();
		FILE* pf = fopen(pathname,"w");
		if (pf!=NULL)
		{
			pDoc->m_manager.WriteValidationDataToExcel(pf);
			fclose(pf);
		}
	}
}

void CDRMBuilderView::OnUpdateFileExportvalidationdata(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	pCmdUI->Enable(!pmanager->bthread && pmanager->brun_valid);
}

void CDRMBuilderView::OnFileExportacmfile()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CString str = pDoc->GetTitle();
	int idot = str.ReverseFind('.');
	if (idot!=-1)
		str = str.Left(idot);
	str += ".acmf";
	CString filter = "ACM file (*.acmf)|*.acmf||";
	char szCurPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	str = (CString)szCurPath + "\\" + str;
	CFileDialog dlg(FALSE,"acmf",str,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();
		FILE* pf = fopen(pathname,"w");
		if (pf!=NULL)
		{
			pDoc->m_manager.WriteACMFileInMemory(pf);
			fclose(pf);
		}
	}
}

void CDRMBuilderView::OnUpdateFileExportacmfile(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	pCmdUI->Enable(!pmanager->bthread && pmanager->bhfm_file);
}

void CDRMBuilderView::OnFileExportacmsnapshotfile()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CString str = pDoc->GetTitle();
	int idot = str.ReverseFind('.');
	if (idot!=-1)
		str = str.Left(idot);
	str += ".asnp";
	CString filter = "ACM snapshot file (*.asnp)|*.asnp||";
	char szCurPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	str = (CString)szCurPath + "\\" + str;
	CFileDialog dlg(FALSE,"asnp",str,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();
		FILE* pf = fopen(pathname,"w");
		if (pf!=NULL)
		{
			pDoc->m_manager.WriteASNPFileInMemory(pf);
			fclose(pf);
		}
	}
}

void CDRMBuilderView::OnUpdateFileExportacmsnapshotfile(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	pCmdUI->Enable(!pmanager->bthread && pmanager->basnp_file);
}

void CDRMBuilderView::OnFileExportsimulinkfile()
{
	// TODO: Add your command handler code here
	int i;
	CDRMBuilderDoc* pDoc = GetDocument();
	CString str = pDoc->GetTitle();
	int idot = str.ReverseFind('.');
	if (idot!=-1)
		str = str.Left(idot);
	str += ".mdl";
	CString filter = "MATLAB Simulink file (*.mdl)|*.mdl||";
	char szCurPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	str = (CString)szCurPath + "\\" + str;
	CFileDialog dlg(FALSE,"mdl",str,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();
		str = pathname;
		i = str.ReverseFind('.');
		str = str.Left(i+1) + "acmf";
		CAcmPathNameDlg dlg_acm;
		dlg_acm.m_pathname_acm = str;
		if (dlg_acm.DoModal()==IDOK)
		{
			FILE* pf = fopen(pathname,"w");
			if (pf!=NULL)
			{
				pDoc->m_manager.WriteMDLFileInMemory(pf,dlg_acm.m_pathname_acm);
				fclose(pf);
			}
		}
	}
}

void CDRMBuilderView::OnUpdateFileExportsimulinkfile(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pmanager = &pDoc->m_manager;
	pCmdUI->Enable(!pmanager->bthread && pmanager->bmdl_file);
}

void CDRMBuilderView::OnPostprocessUsehighfidelityhistory()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CNarma* pnarma = &pDoc->m_manager.drm_container.drm_narma;
	if (pnarma->ipredict_opt)
		pnarma->ipredict_opt = 0;
	else
		pnarma->ipredict_opt = 1;
	pDoc->SetModifiedFlag();
}

void CDRMBuilderView::OnUpdatePostprocessUsehighfidelityhistory(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	pCmdUI->SetCheck(pDoc->m_manager.drm_container.drm_narma.ipredict_opt==0);
	pCmdUI->Enable(!pDoc->m_manager.bthread && pDoc->m_manager.drm_container.imodel_type==1);
}

void CDRMBuilderView::OnUqCalculate()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CPlotResultDlg dlg;
	dlg.m_pinput_list_all = &pDoc->m_manager.input_list_all;
	dlg.m_poutput_list_all = &pDoc->m_manager.output_list_all;
	dlg.m_berror = pDoc->m_manager.bplot_error;
	dlg.m_bstep_change = pDoc->m_manager.bplot_step_change;
	dlg.m_bcorrelation = pDoc->m_manager.bplot_correlation;
	if (dlg.DoModal()==IDOK)
	{
		pDoc->m_manager.input_to_plot = dlg.m_selected_inputs;
		pDoc->m_manager.output_to_plot = dlg.m_selected_outputs;
		pDoc->m_manager.bplot_error = dlg.m_berror;
		pDoc->m_manager.bplot_step_change = dlg.m_bstep_change;
		pDoc->m_manager.bplot_correlation = dlg.m_bcorrelation;
		pDoc->m_manager.PrepareCommandParameterCellArray(1,2);
		pDoc->m_manager.msg.AddMessageLine("Starting to perform UQ in MATLAB...\n");
		pDoc->m_manager.msg.AddMessageLine("Please wait until input and output profiles are plotted. Then close MATLAB window by clicking the \"OK\" button on the pop-up message window.\n");
		bbusy = true;
		SendMessage(WM_SETCURSOR);
		pDoc->StartMatlabUQThreadForValidation();
		bbusy = false;
		SendMessage(WM_SETCURSOR);
		if (pDoc->m_manager.brun_uq)	//UQ calculation successful
			pDoc->SetModifiedFlag();
	}
}

void CDRMBuilderView::OnUpdateUqCalculate(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pm = &pDoc->m_manager;
	pCmdUI->Enable(!pm->bthread && pm->brun_valid && pm->bpredict_valid && pm->bbuilt_dabnet && pm->drm_container.imodel_type==0 && pm->bnoise_set);
}

void CDRMBuilderView::OnUqSetnoise()
{
	// TODO: Add your command handler code here
	int i;
	int noutput;
	CNoiseDlg dlg;
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pm = &pDoc->m_manager;
	noutput = pm->drm_container.noutput;
	dlg.m_poutput_list_sel = &pm->output_list_sel;
	dlg.m_fq_state = (float)pm->drm_container.dabnet_input.fq_state;
	if (pm->drm_container.fr_output.size() != noutput)
	{
		pm->drm_container.fr_output.resize(noutput);
		for (i=0; i<noutput; i++)
			pm->drm_container.fr_output[i] = 0.01;	//set to default values
	}
	dlg.m_vfr_output = pm->drm_container.fr_output;
	if (dlg.DoModal()==IDOK)
	{
		pm->drm_container.dabnet_input.fq_state = dlg.m_fq_state;
		for (i=0; i<noutput; i++)
			pm->drm_container.fr_output[i] = dlg.m_vfr_output[i];
		pm->bnoise_set = true;
		pDoc->SetModifiedFlag();
	}
}

void CDRMBuilderView::OnUpdateUqSetnoise(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pm = &pDoc->m_manager;
	pCmdUI->Enable(!pm->bthread && pm->boutputready && pm->drm_container.imodel_type==0);
}

void CDRMBuilderView::OnExportCovariancematrices()
{
	// TODO: Add your command handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CString str = pDoc->GetTitle();
	int idot = str.ReverseFind('.');
	if (idot!=-1)
		str = str.Left(idot);
	str += "_uq.txt";
	CString filter = "Text file (*.txt)|*.txt||";
	char szCurPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,szCurPath);
	str = (CString)szCurPath + "\\" + str;
	CFileDialog dlg(FALSE,"txt",str,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		CString pathname = dlg.GetPathName();
		FILE* pf = fopen(pathname,"w");
		if (pf!=NULL)
		{
			pDoc->m_manager.WriteCovarianceMatrices(pf);
			fclose(pf);
		}
	}
}

void CDRMBuilderView::OnUpdateExportCovariancematrices(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	CDRMBuilderDoc* pDoc = GetDocument();
	CDRMManager* pm = &pDoc->m_manager;
	pCmdUI->Enable(!pm->bthread && pm->drm_container.imodel_type==0 && pm->brun_uq);
}
