// OutputVariableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRMBuilder.h"
#include "OutputVariableDlg.h"


// COutputVariableDlg dialog

IMPLEMENT_DYNAMIC(COutputVariableDlg, CDialog)

COutputVariableDlg::COutputVariableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutputVariableDlg::IDD, pParent)
	, m_name(_T(""))
	, m_desc(_T(""))
	, m_bvaried(FALSE)
	, m_nx(0)
	, m_unit(_T(""))
{

}

COutputVariableDlg::~COutputVariableDlg()
{
}

void COutputVariableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_output_list);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_DESC, m_desc);
	DDX_Check(pDX, IDC_CHECK_VARIED, m_bvaried);
	DDX_Text(pDX, IDC_EDIT_NX, m_nx);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT, m_button_default);
	DDX_Text(pDX, IDC_EDIT_UNIT1, m_unit);
}


BEGIN_MESSAGE_MAP(COutputVariableDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &COutputVariableDlg::OnBnClickedButtonDefault)
	ON_LBN_SELCHANGE(IDC_LIST_OUTPUT, &COutputVariableDlg::OnLbnSelchangeListOutput)
	ON_EN_KILLFOCUS(IDC_EDIT_DESC, &COutputVariableDlg::OnEnKillfocusEditDesc)
	ON_BN_CLICKED(IDC_CHECK_VARIED, &COutputVariableDlg::OnBnClickedCheckVaried)
	ON_BN_CLICKED(IDOK, &COutputVariableDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// COutputVariableDlg message handlers

BOOL COutputVariableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	std::list<COutputVariable>::iterator ite;
	m_nx = 0;
	for (ite=m_poldlist->begin(); ite!=m_poldlist->end(); ite++)
	{
		m_newlist.push_back(*ite);
		if ((*ite).bvaried)
			m_nx++;
		m_output_list.AddString((*ite).name.c_str());
	}
	m_output_list.SetCurSel(0);
	if (m_newlist.size()>0)
	{
		ite = m_newlist.begin();
		m_bvaried = (*ite).bvaried;
		m_name = (*ite).name.c_str();
		m_desc = (*ite).desc.c_str();
		m_unit = (*ite).unit.c_str();
	}
	else
	{
		m_bvaried = FALSE;
		m_name = "";
		m_desc = "";
		m_unit = "";
	}
	UpdateData(FALSE);
	// end of my code
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void COutputVariableDlg::OnBnClickedButtonDefault()
{
	// TODO: Add your control notification handler code here
	m_button_default.SetFocus();
}

void COutputVariableDlg::OnLbnSelchangeListOutput()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_output_list.GetCurSel();
	std::list<COutputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	m_bvaried = (*ite).bvaried;
	m_name = (*ite).name.c_str();
	m_desc = (*ite).desc.c_str();
	m_unit = (*ite).unit.c_str();
	UpdateData(FALSE);
}

void COutputVariableDlg::OnEnKillfocusEditDesc()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_output_list.GetCurSel();
	if (isel<0)
	{
		AfxMessageBox("Please select an output variable!");
		return;
	}
	std::list<COutputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	(*ite).desc = m_desc;
}

void COutputVariableDlg::OnBnClickedCheckVaried()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_output_list.GetCurSel();
	if (isel<0)
	{
		AfxMessageBox("Please select an output variable!");
		return;
	}
	std::list<COutputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	(*ite).bvaried = m_bvaried==TRUE?true:false;
	if (m_bvaried)
		m_nx++;
	else
		m_nx--;
	UpdateData(FALSE);
}

void COutputVariableDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString str;
	std::list<COutputVariable>::iterator ite;
	//at least one variable needs to be selected
	if (m_output_list.GetCount()>0 && m_nx==0)
	{
		AfxMessageBox("Please choose at least one output variable!");
		return;
	}
	m_poldlist->clear();
	for (ite=m_newlist.begin(); ite!=m_newlist.end(); ite++)
		m_poldlist->push_back(*ite);
	m_newlist.clear();
	//end of my code
	OnOK();
}
