// InputVariableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRMBuilder.h"
#include "InputVariableDlg.h"


// CInputVariableDlg dialog

IMPLEMENT_DYNAMIC(CInputVariableDlg, CDialog)

CInputVariableDlg::CInputVariableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInputVariableDlg::IDD, pParent)
	, m_name(_T(""))
	, m_desc(_T(""))
	, m_xdefault(0)
	, m_xlower(0)
	, m_xupper(0)
	, m_bvaried(FALSE)
	, m_nx(0)
	, m_unit(_T(""))
	, m_ramp_rate(0)
	, m_bramp(FALSE)
	, m_unit_ramp_rate(_T(""))
	, m_dt_sampling(0)
	, m_dt_min_solver(0)
	, m_unit_dt_sampling(_T(""))
	, m_unit_dt_min_solver(_T(""))
{

}

CInputVariableDlg::~CInputVariableDlg()
{
}

void CInputVariableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INPUT, m_input_list);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_DESC, m_desc);
	DDX_Text(pDX, IDC_EDIT_DEFAULT, m_xdefault);
	DDX_Text(pDX, IDC_EDIT_LOWER_LIMIT, m_xlower);
	DDX_Text(pDX, IDC_EDIT_UPPER_LIMIT, m_xupper);
	DDX_Check(pDX, IDC_CHECK_VARIED, m_bvaried);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT, m_button_default);
	DDX_Control(pDX, IDC_EDIT_LOWER_LIMIT, m_edit_lower);
	DDX_Control(pDX, IDC_EDIT_UPPER_LIMIT, m_edit_upper);
	DDX_Text(pDX, IDC_EDIT_NX, m_nx);
	DDX_Text(pDX, IDC_EDIT_UNIT, m_unit);
	DDX_Text(pDX, IDC_EDIT_RAMP_RATE, m_ramp_rate);
	DDV_MinMaxFloat(pDX, m_ramp_rate, 0, 1e20f);
	DDX_Check(pDX, IDC_CHECK_RAMP, m_bramp);
	DDX_Control(pDX, IDC_EDIT_RAMP_RATE, m_edit_ramp_rate);
	DDX_Control(pDX, IDC_CHECK_RAMP, m_check_ramp);
	DDX_Text(pDX, IDC_STATIC_RAMP_RATE, m_unit_ramp_rate);
	DDX_Text(pDX, IDC_EDIT_TIME_SAMPLE, m_dt_sampling);
	DDV_MinMaxFloat(pDX, m_dt_sampling, 0, 1e10);
	DDX_Text(pDX, IDC_EDIT_TIME_MIN_SOLVER, m_dt_min_solver);
	DDX_Text(pDX, IDC_STATIC_TIME_SAMPLE, m_unit_dt_sampling);
	DDX_Text(pDX, IDC_STATIC_TIME_MIN_SOLVER, m_unit_dt_min_solver);
}


BEGIN_MESSAGE_MAP(CInputVariableDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CInputVariableDlg::OnBnClickedButtonDefault)
	ON_LBN_SELCHANGE(IDC_LIST_INPUT, &CInputVariableDlg::OnLbnSelchangeListInput)
	ON_EN_KILLFOCUS(IDC_EDIT_DESC, &CInputVariableDlg::OnEnKillfocusEditDesc)
	ON_EN_KILLFOCUS(IDC_EDIT_LOWER_LIMIT, &CInputVariableDlg::OnEnKillfocusEditLowerLimit)
	ON_EN_KILLFOCUS(IDC_EDIT_UPPER_LIMIT, &CInputVariableDlg::OnEnKillfocusEditUpperLimit)
	ON_BN_CLICKED(IDC_CHECK_VARIED, &CInputVariableDlg::OnBnClickedCheckVaried)
	ON_BN_CLICKED(IDOK, &CInputVariableDlg::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_RAMP_RATE, &CInputVariableDlg::OnEnKillfocusEditRampRate)
	ON_BN_CLICKED(IDC_CHECK_RAMP, &CInputVariableDlg::OnBnClickedCheckRamp)
	ON_EN_KILLFOCUS(IDC_EDIT_TIME_SAMPLE, &CInputVariableDlg::OnEnKillfocusEditTimeSample)
END_MESSAGE_MAP()


// CInputVariableDlg message handlers

BOOL CInputVariableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	std::list<CInputVariable>::iterator ite;
	m_nx = 0;
	for (ite=m_poldlist->begin(); ite!=m_poldlist->end(); ite++)
	{
		m_newlist.push_back(*ite);
		if ((*ite).bvaried)
			m_nx++;
		m_input_list.AddString((*ite).name.c_str());
	}
	m_input_list.SetCurSel(0);
	if (m_newlist.size()>0)
	{
		ite = m_newlist.begin();
		m_bvaried = (*ite).bvaried;
		m_name = (*ite).name.c_str();
		m_desc = (*ite).desc.c_str();
		m_unit = (*ite).unit.c_str();
		m_xdefault = (float)(*ite).xdefault;
		m_xlower = (float)(*ite).xlower;
		m_xupper = (float)(*ite).xupper;
		m_ramp_rate = (float)(*ite).ramp_rate;
		m_bramp = (*ite).bramp;
		if (m_unit==" ")
			m_unit_ramp_rate = "1/" + m_time_unit_acm;
		else
			m_unit_ramp_rate = m_unit + "/" + m_time_unit_acm;
	}
	else
	{
		m_bvaried = FALSE;
		m_bramp = FALSE;
		m_name = "";
		m_desc = "";
		m_unit = "";
		m_xdefault = 0;
		m_xlower = 0;
		m_xupper = 0;
		m_ramp_rate = 0;
		m_unit_ramp_rate = "";
	}
	m_edit_lower.EnableWindow(m_bvaried);
	m_edit_upper.EnableWindow(m_bvaried);
	m_edit_ramp_rate.EnableWindow(m_bvaried && m_bramp);
	m_check_ramp.EnableWindow(m_bvaried);
	m_unit_dt_sampling = m_time_unit_acm;
	m_unit_dt_min_solver = m_time_unit_acm;
	UpdateData(FALSE);
	// end of my code

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CInputVariableDlg::OnBnClickedButtonDefault()
{
	// TODO: Add your control notification handler code here
	m_button_default.SetFocus();
}

void CInputVariableDlg::OnLbnSelchangeListInput()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_input_list.GetCurSel();
	std::list<CInputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	m_bvaried = (*ite).bvaried;
	m_bramp = (*ite).bramp;
	m_name = (*ite).name.c_str();
	m_desc = (*ite).desc.c_str();
	m_unit = (*ite).unit.c_str();
	m_xdefault = (float)(*ite).xdefault;
	m_xlower = (float)(*ite).xlower;
	m_xupper = (float)(*ite).xupper;
	m_ramp_rate = (float)(*ite).ramp_rate;
	if (m_unit==" ")
		m_unit_ramp_rate = "1/" + m_time_unit_acm;
	else
		m_unit_ramp_rate = m_unit + "/" + m_time_unit_acm;
	m_edit_lower.EnableWindow(m_bvaried);
	m_edit_upper.EnableWindow(m_bvaried);
	m_edit_ramp_rate.EnableWindow(m_bvaried && m_bramp);
	m_check_ramp.EnableWindow(m_bvaried);
	UpdateData(FALSE);
}

void CInputVariableDlg::OnEnKillfocusEditDesc()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_input_list.GetCurSel();
	if (isel<0)
	{
		AfxMessageBox("Please select an input variable!");
		return;
	}
	std::list<CInputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	(*ite).desc = m_desc;
}

void CInputVariableDlg::OnEnKillfocusEditLowerLimit()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_input_list.GetCurSel();
	if (isel<0)
	{
		AfxMessageBox("Please select an input variable!");
		return;
	}
	std::list<CInputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	(*ite).xlower = m_xlower;
}

void CInputVariableDlg::OnEnKillfocusEditUpperLimit()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_input_list.GetCurSel();
	if (isel<0)
	{
		AfxMessageBox("Please select an input variable!");
		return;
	}
	std::list<CInputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	(*ite).xupper = m_xupper;
}

void CInputVariableDlg::OnBnClickedCheckVaried()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_input_list.GetCurSel();
	if (isel<0)
	{
		AfxMessageBox("Please select an input variable!");
		return;
	}
	std::list<CInputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	(*ite).bvaried = m_bvaried==TRUE?true:false;
	if (m_bvaried)
		m_nx++;
	else
		m_nx--;
	m_edit_lower.EnableWindow(m_bvaried);
	m_edit_upper.EnableWindow(m_bvaried);
	m_edit_ramp_rate.EnableWindow(m_bvaried && m_bramp);
	m_check_ramp.EnableWindow(m_bvaried);
	UpdateData(FALSE);
}

void CInputVariableDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	int i = 0;
	CString str;
	std::list<CInputVariable>::iterator ite;
	//check if lower and upper limits valid
	//at least one variable needs to be selected
	for (ite=m_newlist.begin(); ite!=m_newlist.end(); ite++)
	{
		i++;
		if ((*ite).bvaried)
		{
			if ((*ite).xlower > (*ite).xdefault)
			{
				str.Format("Variable %d's lower limit is greater than the current steady state value!",i);
				AfxMessageBox(str);
				return;
			}
			if ((*ite).xupper < (*ite).xdefault)
			{
				str.Format("Variable %d's upper limit is less than the current steady state value!",i);
				AfxMessageBox(str);
				return;
			}
		}
	}
	if (m_input_list.GetCount()>0 && m_nx==0)
	{
		AfxMessageBox("Please choose at least one input variable!");
		return;
	}
	//check to see if the ramp rate is reasonable
	i = 0;
	for (ite=m_newlist.begin(); ite!=m_newlist.end(); ite++)
	{
		i++;
		if ((*ite).bvaried && (*ite).bramp)
		{
			if (((*ite).xupper - (*ite).xlower)/(*ite).ramp_rate > m_dt_sampling/2)
			{
				str.Format("The ramp rate of %s is too low to ramp from minimum to maximum within half of the sampling time interval!",(*ite).name.c_str());
				AfxMessageBox(str);
				return;
			}
		}
	}
	m_poldlist->clear();
	for (ite=m_newlist.begin(); ite!=m_newlist.end(); ite++)
		m_poldlist->push_back(*ite);
	m_newlist.clear();
	//end of my code
	OnOK();
}

void CInputVariableDlg::OnEnKillfocusEditRampRate()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_input_list.GetCurSel();
	if (isel<0)
	{
		AfxMessageBox("Please select an input variable!");
		return;
	}
	std::list<CInputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	(*ite).ramp_rate = m_ramp_rate;
}

void CInputVariableDlg::OnBnClickedCheckRamp()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int isel = m_input_list.GetCurSel();
	if (isel<0)
	{
		AfxMessageBox("Please select an input variable!");
		return;
	}
	std::list<CInputVariable>::iterator ite;
	ite = m_newlist.begin();
	for (int i=0; i<isel; i++)
		ite++;
	(*ite).bramp = m_bramp==TRUE?true:false;
	m_edit_ramp_rate.EnableWindow(m_bvaried && m_bramp);
	UpdateData(FALSE);
}

void CInputVariableDlg::OnEnKillfocusEditTimeSample()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
