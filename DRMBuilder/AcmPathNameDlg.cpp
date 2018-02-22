// AcmPathNameDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DRMBuilder.h"
#include "AcmPathNameDlg.h"


// CAcmPathNameDlg dialog

IMPLEMENT_DYNAMIC(CAcmPathNameDlg, CDialog)

CAcmPathNameDlg::CAcmPathNameDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAcmPathNameDlg::IDD, pParent)
	, m_pathname_acm(_T(""))
{

}

CAcmPathNameDlg::~CAcmPathNameDlg()
{
}

void CAcmPathNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PATHNAME, m_pathname_acm);
}


BEGIN_MESSAGE_MAP(CAcmPathNameDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CAcmPathNameDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDOK, &CAcmPathNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAcmPathNameDlg message handlers

void CAcmPathNameDlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	CString filter = "ACM file (*.acmf)|*.acmf||";
	CFileDialog dlg(TRUE,"acmf",m_pathname_acm,OFN_NOCHANGEDIR,filter);
	if (dlg.DoModal()==IDOK)
	{
		m_pathname_acm = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CAcmPathNameDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	//end of my code
	OnOK();
}
