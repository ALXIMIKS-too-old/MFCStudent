
// MFCStudentDlg.h : head file
//

#pragma once
#include "afxcmn.h"

//------------------------------------------------------------
// struct for serialization of data
//------------------------------------------------------------
struct stud{
	CString name;								// name of student
	double	mark;								// mark of student
};

//------------------------------------------------------------
// dialog window  CStudentDlg    (MAIN)
//------------------------------------------------------------
class CStudentDlg : public CDialogEx
{

public:
	CStudentDlg(CWnd* pParent = NULL);	

	enum { IDD = IDD_MFCSTUDENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// for DDX/DDV


protected:
	HICON m_hIcon;
	int m_sortColumn;			 // number of column for sort
	BOOL m_sortParam;			 // diraction of sort

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	   	
	
	afx_msg void OnButton_Add();				 //  event handler: Clicked on button  Add
	afx_msg void OnButton_Edit();				 //  event handler: Clicked on button  Edit
	afx_msg void OnButton_Del();				 //  event handler: Clicked on button  Delete
	afx_msg void OnButton_AddRand();			 //  event handler: Clicked on button Add_Rand
	afx_msg void OnButton_About();				 //  event handler: Clicked on button  About

	afx_msg void On_Dblclk_List(NMHDR *pNMHDR, LRESULT *pResult); //  event handler: DblClick on LIST
	afx_msg void OnLvnKeydownList(NMHDR *pNMHDR, LRESULT *pResult);	//  event handler: Keydown on LIST		 	
	afx_msg void OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult); // event handler:  Click on head of LIST column


	CListCtrl m_list_ctrl;			 // CListCtrl of LIST
	CList<stud> m_students;			 // List of students
	CImageList m_image_list;		 // icons for LIST	
	CString  file_CArchive;          // name of serialization file

	void FillList();				 //  rebuild  of LIST
	void OnOK();					 //  event handler: Enter 
	void OnCancel();				 //  event handler: exit 
	void Serialize(const CString &file_CArchive, int flag, int rand_count = 20); //  Serialization of  CList<stud> m_students;	 	

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);	 // compare using ItemDATA
};
