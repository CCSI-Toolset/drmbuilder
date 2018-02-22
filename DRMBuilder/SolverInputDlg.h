#pragma once
#include "afxwin.h"

#include <vector>

// CSolverInputDlg dialog

class CSolverInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CSolverInputDlg)

public:
	CSolverInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSolverInputDlg();

//my data
	bool m_bvalidation;
	std::vector<int>* m_pvduration;
	std::vector<int> m_vduration;
//end of my data

// Dialog Data
	enum { IDD = IDD_DIALOG_SOLVER_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	float m_dt_sampling;
	int m_npoint;
	int m_nduration;
	int m_ireverse;
	CListBox m_duration_list;
	int m_ndt;
	CButton m_button_default;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnEnKillfocusEditNpoint();
	afx_msg void OnEnKillfocusEditNduration();
	afx_msg void OnBnClickedCheckIreverse();
	afx_msg void OnLbnSelchangeListDuration();
	afx_msg void OnEnKillfocusEditNdt();
	CString m_time_unit_acm;
	float m_dt_min_solver;
	CString m_unit_dt_min_solver;
};
