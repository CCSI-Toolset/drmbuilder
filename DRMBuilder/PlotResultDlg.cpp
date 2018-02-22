// PlotResultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRMBuilder.h"
#include "PlotResultDlg.h"


// CPlotResultDlg dialog

IMPLEMENT_DYNAMIC(CPlotResultDlg, CDialog)

CPlotResultDlg::CPlotResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlotResultDlg::IDD, pParent)
	, m_berror(FALSE)
	, m_bstep_change(FALSE)
	, m_bcorrelation(FALSE)
{

}

CPlotResultDlg::~CPlotResultDlg()
{
}

void CPlotResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INPUT, m_list_input);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_list_output);
	DDX_Check(pDX, IDC_CHECK_ERROR, m_berror);
	DDX_Check(pDX, IDC_CHECK_STEP_CHANGE, m_bstep_change);
	DDX_Check(pDX, IDC_CHECK_CORRELATION, m_bcorrelation);
}


BEGIN_MESSAGE_MAP(CPlotResultDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL_IN, &CPlotResultDlg::OnBnClickedButtonSelectAllIn)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL_OUT, &CPlotResultDlg::OnBnClickedButtonSelectAllOut)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT_ALL_IN, &CPlotResultDlg::OnBnClickedButtonDeselectAllIn)
	ON_BN_CLICKED(IDC_BUTTON_DESELECT_ALL_OUT, &CPlotResultDlg::OnBnClickedButtonDeselectAllOut)
	ON_BN_CLICKED(IDOK, &CPlotResultDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CPlotResultDlg message handlers

BOOL CPlotResultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString str;
	std::list<CInputVariable>::iterator ite_in;
	std::list<COutputVariable>::iterator ite_out;
	for (ite_in=m_pinput_list_all->begin(); ite_in!=m_pinput_list_all->end(); ++ite_in)
	{
		str = (*ite_in).name.c_str();
		if ((*ite_in).bvaried)
			str = "* " + str;
		else
			str = "   " + str;
		m_list_input.AddString(str);
	}
	for (ite_out=m_poutput_list_all->begin(); ite_out!=m_poutput_list_all->end(); ++ite_out)
	{
		str = (*ite_out).name.c_str();
		if ((*ite_out).bvaried)
			str = "* " + str;
		else
			str = "   " + str;
		m_list_output.AddString(str);
	}
	m_list_input.SetCurSel(0);
	m_list_output.SetCurSel(0);
	UpdateData(FALSE);
	// end of my code
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPlotResultDlg::OnBnClickedButtonSelectAllIn()
{
	// TODO: Add your control notification handler code here
	int i;
	int ncount = m_list_input.GetCount();
	for (i=0; i<ncount; i++)
	{
		m_list_input.SetSel(i);
	}
	UpdateData(FALSE);
}

void CPlotResultDlg::OnBnClickedButtonSelectAllOut()
{
	// TODO: Add your control notification handler code here
	int i;
	int ncount = m_list_output.GetCount();
	for (i=0; i<ncount; i++)
	{
		m_list_output.SetSel(i);
	}
	UpdateData(FALSE);
}

void CPlotResultDlg::OnBnClickedButtonDeselectAllIn()
{
	// TODO: Add your control notification handler code here
	int i;
	int ncount = m_list_input.GetCount();
	for (i=0; i<ncount; i++)
	{
		m_list_input.SetSel(i,FALSE);
	}
	UpdateData(FALSE);
}

void CPlotResultDlg::OnBnClickedButtonDeselectAllOut()
{
	// TODO: Add your control notification handler code here
	int i;
	int ncount = m_list_output.GetCount();
	for (i=0; i<ncount; i++)
	{
		m_list_output.SetSel(i,FALSE);
	}
	UpdateData(FALSE);
}

void CPlotResultDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	int i;
	int ncount;
	UpdateData();
	m_selected_inputs.clear();
	m_selected_outputs.clear();
	ncount = m_list_input.GetCount();
	for (i=0; i<ncount; i++)
	{
		if (m_list_input.GetSel(i)>0)
			m_selected_inputs.push_back(i);
	}
	ncount = m_list_output.GetCount();
	for (i=0; i<ncount; i++)
	{
		if (m_list_output.GetSel(i)>0)
			m_selected_outputs.push_back(i);
	}
	if (m_selected_inputs.size() <= 0)
	{
		AfxMessageBox("Please select at least one input!");
		return;
	}
	if (m_selected_outputs.size() <= 0)
	{
		AfxMessageBox("Please select at least one output!");
		return;
	}
	//end of my code
	OnOK();
}
