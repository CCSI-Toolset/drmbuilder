#pragma once
#include "afxwin.h"


// CNarmaParameterDlg dialog

class CNarmaParameterDlg : public CDialog
{
	DECLARE_DYNAMIC(CNarmaParameterDlg)

public:
	CNarmaParameterDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNarmaParameterDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODEL_PARAMETER_NARMA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nhistory;
	int m_nneuron_hid;
	int m_nmax_iter_bp;
	CButton m_button_default;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnBnClickedButtonReset();
};
