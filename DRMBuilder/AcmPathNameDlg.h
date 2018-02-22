#pragma once


// CAcmPathNameDlg dialog

class CAcmPathNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CAcmPathNameDlg)

public:
	CAcmPathNameDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAcmPathNameDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ACM_PATHNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_pathname_acm;
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedOk();
};
