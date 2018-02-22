#pragma once
#include "afxwin.h"

#include <list>
#include "OutputVariable.h"

// COutputVariableDlg dialog

class COutputVariableDlg : public CDialog
{
	DECLARE_DYNAMIC(COutputVariableDlg)

public:
	COutputVariableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COutputVariableDlg();

// my additional data
	std::list<COutputVariable>* m_poldlist;
	std::list<COutputVariable> m_newlist;
// end of my data

// Dialog Data
	enum { IDD = IDD_DIALOG_OUTPUT_VARIABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_output_list;
	CString m_name;
	CString m_desc;
	BOOL m_bvaried;
	int m_nx;
	CButton m_button_default;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnLbnSelchangeListOutput();
	afx_msg void OnEnKillfocusEditDesc();
	afx_msg void OnBnClickedCheckVaried();
	afx_msg void OnBnClickedOk();
	CString m_unit;
};
