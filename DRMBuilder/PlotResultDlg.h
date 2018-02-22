#pragma once

#include <list>
#include <vector>
#include "InputVariable.h"
#include "OutputVariable.h"
#include "afxwin.h"


// CPlotResultDlg dialog

class CPlotResultDlg : public CDialog
{
	DECLARE_DYNAMIC(CPlotResultDlg)

public:
	CPlotResultDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlotResultDlg();

	//my data
	std::list<CInputVariable>* m_pinput_list_all;
	std::list<COutputVariable>* m_poutput_list_all;
	std::vector<int> m_selected_inputs;
	std::vector<int> m_selected_outputs;
	//end of my data

// Dialog Data
	enum { IDD = IDD_DIALOG_PLOT_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_list_input;
	CListBox m_list_output;
	afx_msg void OnBnClickedButtonSelectAllIn();
	afx_msg void OnBnClickedButtonSelectAllOut();
	afx_msg void OnBnClickedButtonDeselectAllIn();
	afx_msg void OnBnClickedButtonDeselectAllOut();
	afx_msg void OnBnClickedOk();
	BOOL m_berror;
	BOOL m_bstep_change;
	BOOL m_bcorrelation;
};
