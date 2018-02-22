// NoiseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRMBuilder.h"
#include "NoiseDlg.h"


// CNoiseDlg dialog

IMPLEMENT_DYNAMIC(CNoiseDlg, CDialog)

CNoiseDlg::CNoiseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoiseDlg::IDD, pParent)
	, m_fq_state(0)
	, m_noutput(0)
	, m_fr_output(0)
{

}

CNoiseDlg::~CNoiseDlg()
{
}

void CNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT, m_button_default);
	DDX_Text(pDX, IDC_EDIT_FQ_STATE, m_fq_state);
	DDV_MinMaxFloat(pDX, m_fq_state, 1e-10f, 1);
	DDX_Text(pDX, IDC_EDIT_NOUTPUT, m_noutput);
	DDV_MinMaxInt(pDX, m_noutput, 1, 100);
	DDX_Text(pDX, IDC_EDIT_FR_OUTPUT, m_fr_output);
	DDV_MinMaxFloat(pDX, m_fr_output, 1e-10f, 1);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_list_output);
}


BEGIN_MESSAGE_MAP(CNoiseDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CNoiseDlg::OnBnClickedButtonDefault)
	ON_EN_KILLFOCUS(IDC_EDIT_FQ_STATE, &CNoiseDlg::OnEnKillfocusEditFqState)
	ON_EN_KILLFOCUS(IDC_EDIT_FR_OUTPUT, &CNoiseDlg::OnEnKillfocusEditFrOutput)
	ON_LBN_SELCHANGE(IDC_LIST_OUTPUT, &CNoiseDlg::OnLbnSelchangeListOutput)
END_MESSAGE_MAP()


// CNoiseDlg message handlers

BOOL CNoiseDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString str;
	int i;
	m_noutput = m_poutput_list_sel->size();
	std::list<COutputVariable>::iterator ite_out;
	i = 0;
	for (ite_out=m_poutput_list_sel->begin(); ite_out!=m_poutput_list_sel->end(); ++ite_out)
	{
		i++;
		str.Format("%d: %s",i,(*ite_out).name.c_str());
		m_list_output.AddString(str);
	}
	m_list_output.SetCurSel(0);
	m_fr_output = (float)m_vfr_output[0];
	UpdateData(FALSE);
	// end of my code
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CNoiseDlg::OnBnClickedButtonDefault()
{
	// TODO: Add your control notification handler code here
	m_button_default.SetFocus();
}

void CNoiseDlg::OnEnKillfocusEditFqState()
{
	// TODO: Add your control notification handler code here
	UpdateData();
}

void CNoiseDlg::OnEnKillfocusEditFrOutput()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int index = m_list_output.GetCurSel();
	m_vfr_output[index] = m_fr_output;
}

void CNoiseDlg::OnLbnSelchangeListOutput()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int index = m_list_output.GetCurSel();
	m_fr_output = (float)m_vfr_output[index];
	UpdateData(FALSE);
}
