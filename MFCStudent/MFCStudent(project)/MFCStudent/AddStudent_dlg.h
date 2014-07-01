#pragma once	  
#include "afxwin.h"

// dialog AddStudent_dlg 
// get name and mark of student

class add_student : public CDialogEx
{
	DECLARE_DYNAMIC(add_student)

public:
	add_student(CWnd* pParent = NULL);
	virtual ~add_student();
	virtual BOOL add_student::OnInitDialog();

	afx_msg void OnUpdateStudentMark();	  							   //  event handler: input character in edit place
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);   //  for change color

	enum { IDD = IDD_ADD_STUDENT };		// dialog window ID

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // for DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_text_range;				 // TRUE  0 < m_mark < 101
	CStatic m_range_text;			 // STATIC TEXT control
	CString m_name;					 // name of student
	double m_mark;					 // mark of student
	CString m_dialog_name;			 // name of student

};
