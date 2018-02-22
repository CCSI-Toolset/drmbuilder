// NarmaParameterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRMBuilder.h"
#include "NarmaParameterDlg.h"


// CNarmaParameterDlg dialog

IMPLEMENT_DYNAMIC(CNarmaParameterDlg, CDialog)

CNarmaParameterDlg::CNarmaParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNarmaParameterDlg::IDD, pParent)
	, m_nhistory(0)
	, m_nneuron_hid(0)
	, m_nmax_iter_bp(0)
{

}

CNarmaParameterDlg::~CNarmaParameterDlg()
{
}

void CNarmaParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NHISTORY, m_nhistory);
	DDV_MinMaxInt(pDX, m_nhistory, 1, 4);
	DDX_Text(pDX, IDC_EDIT_NNEURON_HID, m_nneuron_hid);
	DDV_MinMaxInt(pDX, m_nneuron_hid, 2, 50);
	DDX_Text(pDX, IDC_EDIT_NMAX_ITER_BP, m_nmax_iter_bp);
	DDV_MinMaxInt(pDX, m_nmax_iter_bp, 1000, 200000);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT, m_button_default);
}


BEGIN_MESSAGE_MAP(CNarmaParameterDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CNarmaParameterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CNarmaParameterDlg::OnBnClickedButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CNarmaParameterDlg::OnBnClickedButtonReset)
END_MESSAGE_MAP()


// CNarmaParameterDlg message handlers

void CNarmaParameterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	OnOK();
}

void CNarmaParameterDlg::OnBnClickedButtonDefault()
{
	// TODO: Add your control notification handler code here
	m_button_default.SetFocus();
}

void CNarmaParameterDlg::OnBnClickedButtonReset()
{
	// TODO: Add your control notification handler code here
	m_nhistory = 2;
	m_nneuron_hid = 10;
	m_nmax_iter_bp = 10000;
	UpdateData(FALSE);
}
