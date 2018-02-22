#pragma once
#include "afxwin.h"

#include <list>
#include <vector>
#include "InputVariable.h"
#include "OutputVariable.h"
#include "Dabnet.h"

// CDRMParameterDlg dialog

class CDRMParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CDRMParameterDlg)

public:
	CDRMParameterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDRMParameterDlg();

	//my data
	std::list<CInputVariable>* m_pinput_list_sel;
	std::list<COutputVariable>* m_poutput_list_sel;
	std::vector<CDabnet>* m_pdrm_dabnet;
	CDabnetInput m_dabnet_input;
	int* p_ipole_opt;
	int* p_nneuron_hid;
	int** pp_norder;
	float** pp_pole;
	//end of my data

// Dialog Data
	enum { IDD = IDD_DIALOG_MODEL_PARAMETER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_input_list;
	int m_nneuron_hid;
	int m_norder;
	float m_pole;
	CButton m_button_default;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnLbnSelchangeListInput();
	afx_msg void OnEnKillfocusEditNneuronHid();
	afx_msg void OnEnKillfocusEditNorder();
	afx_msg void OnEnKillfocusEditPole();
	afx_msg void OnBnClickedOk();
	int m_nmax_iter_lag;
	float m_weight_init;
	int m_nmax_iter_red;
	int m_ipole_opt;
	CListBox m_output_list;
	afx_msg void OnLbnSelchangeListOutput();
	afx_msg void OnBnClickedRadioUser();
	afx_msg void OnBnClickedRadioOptimized();
	int m_itrain_lag_opt;
	int m_itrain_red_opt;
	afx_msg void OnEnKillfocusEditNmaxIterLag();
	afx_msg void OnEnKillfocusEditNmaxIterRed();
	afx_msg void OnBnClickedRadioBpLag();
	afx_msg void OnBnClickedRadioIpoptLag();
	afx_msg void OnBnClickedRadioBpRed();
	afx_msg void OnBnClickedRadioIpoptRed();
	afx_msg void OnBnClickedButtonUseDefault();
	afx_msg void OnBnClickedCancel();
};
