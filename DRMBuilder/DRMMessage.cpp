//DRMMessage.cpp
#include "stdafx.h"
#include "DRMBuilder.h"
#include "DRMMessage.h"

CDRMMessage::CDRMMessage(void)
{
	pview = NULL;
	nline = 0;
}

CDRMMessage::~CDRMMessage(void)
{
}

void CDRMMessage::AddMessageLine(CString str)
{
	CTime t = CTime::GetCurrentTime();
	if (str.GetAt(str.GetLength()-1)!='\n')
		str += "\n";
	slog_cur +=  t.Format(_T("[%I:%M:%S %p] ")) + str;
	nline++;
	UpdateWindowText();
}

void CDRMMessage::UpdateWindowText()
{
	int i;
	int ireturn;
	int istart = 0;
	if (nline>=200)		//trim 100 lines, Window's client area can hold up to 200 lines
	{
		for (i=0; i<100; i++)
		{
			ireturn = slog_cur.Find('\n',istart);
			istart = ireturn + 1;
		}
		slog_acc += slog_cur.Left(istart);
		slog_cur = slog_cur.Mid(istart);
		nline -= 100;
	}
	pview->Invalidate();
	pview->SendMessage(WM_COMMAND,ID_VIEW_SCROLLTOBOTTOM);
}

void CDRMMessage::WriteLogFile(FILE* pf)
{
	CString str = slog_acc + slog_cur;
	fprintf(pf,"%s",str);
}