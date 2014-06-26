#pragma once	  

// dialog AddStudent_dlg 
// get name and mark of student

class add_student : public CDialogEx
{
	DECLARE_DYNAMIC(add_student)	 

public:
	add_student(CWnd* pParent = NULL);  
	virtual ~add_student();

	enum { IDD = IDD_ADD_STUDENT };		// dialog window ID

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // for DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;					 // name of student
	double m_mark;					 // mark of student
};
