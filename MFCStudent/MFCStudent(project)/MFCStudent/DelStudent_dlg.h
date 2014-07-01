#pragma once   
// dialog window - delete or not delete

class del_student : public CDialogEx
{
	DECLARE_DYNAMIC(del_student)

public:
	del_student(CWnd* pParent = NULL);   
	virtual ~del_student();
	virtual BOOL OnInitDialog();

	enum { IDD = IDD_DELETE };		// dialog window ID

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    

	DECLARE_MESSAGE_MAP()
public:
	CString m_text;				// static text
	CString	m_dialog_name;		// name of dialog window
};
