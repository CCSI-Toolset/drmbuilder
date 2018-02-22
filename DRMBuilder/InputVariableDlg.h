#pragma once
#include "afxwin.h"

#include <list>
#include "InputVariable.h"

// CInputVariableDlg dialog

class CInputVariableDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputVariableDlg)

public:
	CInputVariableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputVariableDlg();

// my additional data
	CString m_time_unit_acm;
	std::list<CInputVariable>* m_poldlist;
	std::list<CInputVariable> m_newlist;
// end of my data

// Dialog Data
	enum { IDD = IDD_DIALOG_INPUT_VARIABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_input_list;
	CString m_name;
	CString m_desc;
	float m_xdefault;
	float m_xlower;
	float m_xupper;
	BOOL m_bvaried;
	int m_nx;
	CButton m_button_default;
	CEdit m_edit_lower;
	CEdit m_edit_upper;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnLbnSelchangeListInput();
	afx_msg void OnEnKillfocusEditDesc();
	afx_msg void OnEnKillfocusEditLowerLimit();
	afx_msg void OnEnKillfocusEditUpperLimit();
	afx_msg void OnBnClickedCheckVaried();
	afx_msg void OnBnClickedOk();
	CString m_unit;
	float m_ramp_rate;
	BOOL m_bramp;
	CEdit m_edit_ramp_rate;
	CButton m_check_ramp;
	CString m_unit_ramp_rate;
	afx_msg void OnEnKillfocusEditRampRate();
	afx_msg void OnBnClickedCheckRamp();
	float m_dt_sampling;
	float m_dt_min_solver;
	afx_msg void OnEnKillfocusEditTimeSample();
	CString m_unit_dt_sampling;
	CString m_unit_dt_min_solver;
};
