#pragma once
#include "afxwin.h"

#include <vector>
#include <list>
#include "OutputVariable.h"

// CNoiseDlg dialog

class CNoiseDlg : public CDialog
{
	DECLARE_DYNAMIC(CNoiseDlg)

public:
	CNoiseDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNoiseDlg();

//my data
	std::vector<double> m_vfr_output;
	std::list<COutputVariable>* m_poutput_list_sel;
//end of my data

// Dialog Data
	enum { IDD = IDD_DIALOG_NOISE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton m_button_default;
	float m_fq_state;
	int m_noutput;
	float m_fr_output;
	CListBox m_list_output;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnEnKillfocusEditFqState();
	afx_msg void OnEnKillfocusEditFrOutput();
	afx_msg void OnLbnSelchangeListOutput();
};
