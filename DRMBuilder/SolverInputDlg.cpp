// SolverInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRMBuilder.h"
#include "SolverInputDlg.h"


// CSolverInputDlg dialog

IMPLEMENT_DYNAMIC(CSolverInputDlg, CDialog)

CSolverInputDlg::CSolverInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSolverInputDlg::IDD, pParent)
	, m_dt_sampling(0)
	, m_npoint(0)
	, m_nduration(0)
	, m_ireverse(0)
	, m_ndt(0)
	, m_time_unit_acm(_T(""))
	, m_dt_min_solver(0)
	, m_unit_dt_min_solver(_T(""))
{

}

CSolverInputDlg::~CSolverInputDlg()
{
}

void CSolverInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DT, m_dt_sampling);
	DDX_Text(pDX, IDC_EDIT_NPOINT, m_npoint);
	DDV_MinMaxInt(pDX, m_npoint, 2, 100);
	DDX_Text(pDX, IDC_EDIT_NDURATION, m_nduration);
	DDV_MinMaxInt(pDX, m_nduration, 1, 1000);
	DDX_Check(pDX, IDC_CHECK_IREVERSE, m_ireverse);
	DDX_Control(pDX, IDC_LIST_DURATION, m_duration_list);
	DDX_Text(pDX, IDC_EDIT_NDT, m_ndt);
	DDV_MinMaxInt(pDX, m_ndt, 1, 1000);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT, m_button_default);
	DDX_Text(pDX, IDC_STATIC_TIME, m_time_unit_acm);
	DDX_Text(pDX, IDC_EDIT_DT_MIN_SOLVER, m_dt_min_solver);
	DDX_Text(pDX, IDC_STATIC_TIME_MIN, m_unit_dt_min_solver);
}


BEGIN_MESSAGE_MAP(CSolverInputDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CSolverInputDlg::OnBnClickedButtonDefault)
	ON_EN_KILLFOCUS(IDC_EDIT_NPOINT, &CSolverInputDlg::OnEnKillfocusEditNpoint)
	ON_EN_KILLFOCUS(IDC_EDIT_NDURATION, &CSolverInputDlg::OnEnKillfocusEditNduration)
	ON_BN_CLICKED(IDC_CHECK_IREVERSE, &CSolverInputDlg::OnBnClickedCheckIreverse)
	ON_LBN_SELCHANGE(IDC_LIST_DURATION, &CSolverInputDlg::OnLbnSelchangeListDuration)
	ON_EN_KILLFOCUS(IDC_EDIT_NDT, &CSolverInputDlg::OnEnKillfocusEditNdt)
END_MESSAGE_MAP()


// CSolverInputDlg message handlers

BOOL CSolverInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	int i;
	CString str;
	m_nduration = m_pvduration->size();
	m_vduration.resize(m_nduration);
	for (i=0; i<m_nduration; i++)
	{
		m_vduration[i] = m_pvduration->at(i);
		str.Format("LHS Set %d",i+1);
		m_duration_list.AddString(str);
	}
	m_duration_list.SetCurSel(0);
	m_ndt = m_vduration[0];
	if (m_bvalidation)
		SetWindowText("Validation Sequence Dialog");
	else
		SetWindowText("Training Sequence Dialog");
	UpdateData(FALSE);
	//end of my code
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSolverInputDlg::OnBnClickedButtonDefault()
{
	// TODO: Add your control notification handler code here
	m_button_default.SetFocus();
}

void CSolverInputDlg::OnEnKillfocusEditNpoint()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CSolverInputDlg::OnEnKillfocusEditNduration()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int i;
	CString str;
	int nduration_save = m_vduration.size();
	std::vector<int> vduration_save = m_vduration;
	m_vduration.resize(m_nduration,10);
	for (i=0; i<m_nduration && i<nduration_save; i++)
		m_vduration[i] = vduration_save[i];
	m_duration_list.ResetContent();
	for (i=0; i<m_nduration; i++)
	{
		str.Format("LHS Set %d",i+1);
		m_duration_list.AddString(str);
	}
	m_duration_list.SetCurSel(0);
	m_ndt = m_vduration[0];
	UpdateData(FALSE);
}

void CSolverInputDlg::OnBnClickedCheckIreverse()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CSolverInputDlg::OnLbnSelchangeListDuration()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_duration_list.GetCurSel();
	m_ndt = m_vduration[isel];
	UpdateData(FALSE);
}

void CSolverInputDlg::OnEnKillfocusEditNdt()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_duration_list.GetCurSel();
	m_vduration[isel] = m_ndt;
}
