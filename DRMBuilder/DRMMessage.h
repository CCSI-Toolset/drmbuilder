//DRMMessage.h
#ifndef __DRMMESSAGE_H__
#define __DRMMESSAGE_H__

class CDRMMessage
{
public:
	CView* pview;									//pointer to CDRMBuilderView object
	CString slog_cur;								//current log string to be displayed in client window of pvew
	CString slog_acc;								//accumulated log string to be saved to log file
	int nline;										//number of lines in current log string slog_cur

	CDRMMessage(void);
	virtual ~CDRMMessage(void);
	void AddMessageLine(CString str);
	void UpdateWindowText();
	void WriteLogFile(FILE* pf);
};

#endif