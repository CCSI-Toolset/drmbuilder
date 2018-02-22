// CDRMParameterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRMBuilder.h"
#include "DRMParameterDlg.h"


// CDRMParameterDlg dialog

IMPLEMENT_DYNAMIC(CDRMParameterDlg, CDialog)

CDRMParameterDlg::CDRMParameterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDRMParameterDlg::IDD, pParent)
	, m_nneuron_hid(0)
	, m_norder(0)
	, m_pole(0)
	, m_nmax_iter_lag(0)
	, m_weight_init(0)
	, m_nmax_iter_red(0)
	, m_ipole_opt(0)
	, m_itrain_lag_opt(0)
	, m_itrain_red_opt(0)
{

}

CDRMParameterDlg::~CDRMParameterDlg()
{
}

void CDRMParameterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_INPUT, m_input_list);
	DDX_Text(pDX, IDC_EDIT_NNEURON_HID, m_nneuron_hid);
	DDV_MinMaxInt(pDX, m_nneuron_hid, 1, 100);
	DDX_Text(pDX, IDC_EDIT_NORDER, m_norder);
	DDV_MinMaxInt(pDX, m_norder, 2, 30);
	DDX_Text(pDX, IDC_EDIT_POLE, m_pole);
	DDV_MinMaxFloat(pDX, m_pole, 0, 1);
	DDX_Control(pDX, IDC_BUTTON_DEFAULT, m_button_default);
	DDX_Text(pDX, IDC_EDIT_NMAX_ITER_LAG, m_nmax_iter_lag);
	DDV_MinMaxInt(pDX, m_nmax_iter_lag, 100, 1000000);
	DDX_Text(pDX, IDC_EDIT_WEIGHT_INIT, m_weight_init);
	DDV_MinMaxFloat(pDX, m_weight_init, 0, 100);
	DDX_Text(pDX, IDC_EDIT_NMAX_ITER_RED, m_nmax_iter_red);
	DDV_MinMaxInt(pDX, m_nmax_iter_red, 100, 1000000);
	DDX_Radio(pDX, IDC_RADIO_USER, m_ipole_opt);
	DDX_Control(pDX, IDC_LIST_OUTPUT, m_output_list);
	DDX_Radio(pDX, IDC_RADIO_BP_LAG, m_itrain_lag_opt);
	DDX_Radio(pDX, IDC_RADIO_BP_RED, m_itrain_red_opt);
}


BEGIN_MESSAGE_MAP(CDRMParameterDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, &CDRMParameterDlg::OnBnClickedButtonDefault)
	ON_LBN_SELCHANGE(IDC_LIST_INPUT, &CDRMParameterDlg::OnLbnSelchangeListInput)
	ON_EN_KILLFOCUS(IDC_EDIT_NNEURON_HID, &CDRMParameterDlg::OnEnKillfocusEditNneuronHid)
	ON_EN_KILLFOCUS(IDC_EDIT_NORDER, &CDRMParameterDlg::OnEnKillfocusEditNorder)
	ON_EN_KILLFOCUS(IDC_EDIT_POLE, &CDRMParameterDlg::OnEnKillfocusEditPole)
	ON_BN_CLICKED(IDOK, &CDRMParameterDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST_OUTPUT, &CDRMParameterDlg::OnLbnSelchangeListOutput)
	ON_BN_CLICKED(IDC_RADIO_USER, &CDRMParameterDlg::OnBnClickedRadioUser)
	ON_BN_CLICKED(IDC_RADIO_OPTIMIZED, &CDRMParameterDlg::OnBnClickedRadioOptimized)
	ON_EN_KILLFOCUS(IDC_EDIT_NMAX_ITER_LAG, &CDRMParameterDlg::OnEnKillfocusEditNmaxIterLag)
	ON_EN_KILLFOCUS(IDC_EDIT_NMAX_ITER_RED, &CDRMParameterDlg::OnEnKillfocusEditNmaxIterRed)
	ON_BN_CLICKED(IDC_RADIO_BP_LAG, &CDRMParameterDlg::OnBnClickedRadioBpLag)
	ON_BN_CLICKED(IDC_RADIO_IPOPT_LAG, &CDRMParameterDlg::OnBnClickedRadioIpoptLag)
	ON_BN_CLICKED(IDC_RADIO_BP_RED, &CDRMParameterDlg::OnBnClickedRadioBpRed)
	ON_BN_CLICKED(IDC_RADIO_IPOPT_RED, &CDRMParameterDlg::OnBnClickedRadioIpoptRed)
	ON_BN_CLICKED(IDC_BUTTON_USE_DEFAULT, &CDRMParameterDlg::OnBnClickedButtonUseDefault)
	ON_BN_CLICKED(IDCANCEL, &CDRMParameterDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDRMParameterDlg message handlers

BOOL CDRMParameterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString str;
	int i, j;
	int ninput = m_pinput_list_sel->size();
	int noutput = m_poutput_list_sel->size();
	std::list<CInputVariable>::iterator ite_in;
	std::list<COutputVariable>::iterator ite_out;
	i = 0;
	for (ite_in=m_pinput_list_sel->begin(); ite_in!=m_pinput_list_sel->end(); ++ite_in)
	{
		i++;
		str.Format("%d: %s",i,(*ite_in).name.c_str());
		m_input_list.AddString(str);
	}
	i = 0;
	for (ite_out=m_poutput_list_sel->begin(); ite_out!=m_poutput_list_sel->end(); ++ite_out)
	{
		i++;
		str.Format("%d: %s",i,(*ite_out).name.c_str());
		m_output_list.AddString(str);
	}
	m_input_list.SetCurSel(0);
	m_output_list.SetCurSel(0);
	//save arrays
	p_ipole_opt = new int [noutput];
	p_nneuron_hid = new int [noutput];
	pp_norder = new int* [noutput];
	pp_pole = new float* [noutput];
	for (i=0; i<noutput; i++)
	{
		pp_norder[i] = new int [ninput];
		pp_pole[i] = new float [ninput];
	}
	for (i=0; i<noutput; i++)
	{
		p_ipole_opt[i] = m_pdrm_dabnet->at(i).ipole_opt;
		p_nneuron_hid[i] = m_pdrm_dabnet->at(i).nneuron_hid;
		for (j=0; j<ninput; j++)
		{
			pp_norder[i][j] = m_pdrm_dabnet->at(i).norder_list[j];
			pp_pole[i][j] = (float)m_pdrm_dabnet->at(i).pole_list[j];
		}
	}
	m_ipole_opt = p_ipole_opt[0];
	m_nneuron_hid = p_nneuron_hid[0];
	m_norder = pp_norder[0][0];
	m_pole = pp_pole[0][0];
	//static data
	m_itrain_lag_opt = m_dabnet_input.itrain_lag_opt;
	m_itrain_red_opt = m_dabnet_input.itrain_red_opt;
	if (m_itrain_lag_opt)
		m_nmax_iter_lag = m_dabnet_input.nmax_iter_ipopt_lag;
	else
		m_nmax_iter_lag = m_dabnet_input.nmax_iter_bp_lag;
	if (m_itrain_red_opt)
		m_nmax_iter_red = m_dabnet_input.nmax_iter_ipopt_red;
	else
		m_nmax_iter_red = m_dabnet_input.nmax_iter_bp_red;
	m_weight_init = (float)m_dabnet_input.weight_init;
	UpdateData(FALSE);
	//end of my code
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDRMParameterDlg::OnBnClickedButtonDefault()
{
	// TODO: Add your control notification handler code here
	m_button_default.SetFocus();
}

void CDRMParameterDlg::OnLbnSelchangeListInput()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int iout = m_output_list.GetCurSel();
	int iin = m_input_list.GetCurSel();
	m_norder = pp_norder[iout][iin];
	m_pole = pp_pole[iout][iin];
	UpdateData(FALSE);
}

void CDRMParameterDlg::OnEnKillfocusEditNneuronHid()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int iout = m_output_list.GetCurSel();
	p_nneuron_hid[iout] = m_nneuron_hid;
}

void CDRMParameterDlg::OnEnKillfocusEditNorder()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int iout = m_output_list.GetCurSel();
	int iin = m_input_list.GetCurSel();
	pp_norder[iout][iin] = m_norder;
}

void CDRMParameterDlg::OnEnKillfocusEditPole()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int iout = m_output_list.GetCurSel();
	int iin = m_input_list.GetCurSel();
	pp_pole[iout][iin] = m_pole;
}

void CDRMParameterDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	int i, j;
	int ninput = m_pinput_list_sel->size();
	int noutput = m_poutput_list_sel->size();
	UpdateData();
	//update static data
	m_dabnet_input.itrain_lag_opt = m_itrain_lag_opt;
	m_dabnet_input.itrain_red_opt = m_itrain_red_opt;
	m_dabnet_input.weight_init = m_weight_init;
	//update output specific data
	for (i=0; i<noutput; i++)
	{
		m_pdrm_dabnet->at(i).ipole_opt = p_ipole_opt[i];
		m_pdrm_dabnet->at(i).nneuron_hid = p_nneuron_hid[i];
		for (j=0; j<ninput; j++)
		{
			m_pdrm_dabnet->at(i).norder_list[j] = pp_norder[i][j];
			m_pdrm_dabnet->at(i).pole_list[j] = pp_pole[i][j];
		}
		//update total number of Laguerre states
		m_pdrm_dabnet->at(i).UpdateTotalLaguerreOrders();
	}
	//delete pointers
	delete [] p_ipole_opt;
	delete [] p_nneuron_hid;
	for (i=0; i<noutput; i++)
	{
		delete [] pp_norder[i];
		delete [] pp_pole[i];
	}
	delete [] pp_norder;
	delete [] pp_pole;
	//end of my code
	OnOK();
}

void CDRMParameterDlg::OnLbnSelchangeListOutput()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int iout = m_output_list.GetCurSel();
	m_input_list.SetCurSel(0);
	m_ipole_opt = p_ipole_opt[iout];
	m_nneuron_hid = p_nneuron_hid[iout];
	m_norder = pp_norder[iout][0];
	m_pole = pp_pole[iout][0];
	UpdateData(FALSE);
}

void CDRMParameterDlg::OnBnClickedRadioUser()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int iout = m_output_list.GetCurSel();
	p_ipole_opt[iout] = m_ipole_opt;
}

void CDRMParameterDlg::OnBnClickedRadioOptimized()
{
	// TODO: Add your control notification handler code here
	OnBnClickedRadioUser();
}

void CDRMParameterDlg::OnEnKillfocusEditNmaxIterLag()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_itrain_lag_opt)	//IPOPT
		m_dabnet_input.nmax_iter_ipopt_lag = m_nmax_iter_lag;
	else					//BP
		m_dabnet_input.nmax_iter_bp_lag = m_nmax_iter_lag;
}

void CDRMParameterDlg::OnEnKillfocusEditNmaxIterRed()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_itrain_red_opt)	//IPOPT
		m_dabnet_input.nmax_iter_ipopt_red = m_nmax_iter_red;
	else					//BP
		m_dabnet_input.nmax_iter_bp_red = m_nmax_iter_red;
}

void CDRMParameterDlg::OnBnClickedRadioBpLag()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_itrain_lag_opt)	//IPOPT
		m_nmax_iter_lag = m_dabnet_input.nmax_iter_ipopt_lag;
	else					//BP
		m_nmax_iter_lag = m_dabnet_input.nmax_iter_bp_lag;
	UpdateData(FALSE);
}

void CDRMParameterDlg::OnBnClickedRadioIpoptLag()
{
	// TODO: Add your control notification handler code here
	OnBnClickedRadioBpLag();
}

void CDRMParameterDlg::OnBnClickedRadioBpRed()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (m_itrain_red_opt)	//IPOPT
		m_nmax_iter_red = m_dabnet_input.nmax_iter_ipopt_red;
	else					//BP
		m_nmax_iter_red = m_dabnet_input.nmax_iter_bp_red;
	UpdateData(FALSE);
}

void CDRMParameterDlg::OnBnClickedRadioIpoptRed()
{
	// TODO: Add your control notification handler code here
	OnBnClickedRadioBpRed();
}

void CDRMParameterDlg::OnBnClickedButtonUseDefault()
{
	// TODO: Add your control notification handler code here
	int i, j;
	int ninput = m_pinput_list_sel->size();
	int noutput = m_poutput_list_sel->size();
	//static variables
	m_itrain_lag_opt = 0;
	m_itrain_red_opt = 0;
	m_dabnet_input.nmax_iter_bp_lag = 5000;
	m_dabnet_input.nmax_iter_ipopt_lag = 3000;
	m_dabnet_input.nmax_iter_bp_red = 10000;
	m_dabnet_input.nmax_iter_ipopt_red = 6000;
	m_nmax_iter_lag = m_dabnet_input.nmax_iter_bp_lag;
	m_nmax_iter_red = m_dabnet_input.nmax_iter_bp_red;
	m_weight_init = 0.01f;
	//parameter related to each output
	for (i=0; i<noutput; i++)
	{
		p_ipole_opt[i] = 0;
		p_nneuron_hid[i] = 10;
		for (j=0; j<ninput; j++)
		{
			pp_norder[i][j] = 6;
			pp_pole[i][j] = 0.5;
		}
	}
	i = m_output_list.GetCurSel();
	j = m_input_list.GetCurSel();
	m_ipole_opt = p_ipole_opt[i];
	m_nneuron_hid = p_nneuron_hid[i];
	m_norder = pp_norder[i][j];
	m_pole = pp_pole[i][j];
	UpdateData(FALSE);
}

void CDRMParameterDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//delete pointers
	int i;
	int noutput = m_poutput_list_sel->size();
	delete [] p_ipole_opt;
	delete [] p_nneuron_hid;
	for (i=0; i<noutput; i++)
	{
		delete [] pp_norder[i];
		delete [] pp_pole[i];
	}
	delete [] pp_norder;
	delete [] pp_pole;
	//end of my code
	OnCancel();
}
