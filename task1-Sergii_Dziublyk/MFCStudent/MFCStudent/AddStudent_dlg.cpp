// AddStudent_dlg.cpp: realization
// get name and mark of student

#include "stdafx.h"
#include "MFCStudent.h"
#include "AddStudent_dlg.h"
#include "afxdialogex.h"	

// dialog add_student	
IMPLEMENT_DYNAMIC(add_student, CDialogEx)

add_student::add_student(CWnd* pParent /*=NULL*/)
	: CDialogEx(add_student::IDD, pParent)
	, m_name(_T("Enter name of student"))			   // default name of student
	, m_mark(1)										   // default mark of student
{	 
}

add_student::~add_student()
{
}

void add_student::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);					 // binding EDIT place ID and m_name variable
	DDX_Text(pDX, IDC_EDIT2, m_mark);					 // binding EDIT place ID and m_mark variable
	DDV_MinMaxDouble(pDX, m_mark, 1, 100);				 // min and max mark
}	 

BEGIN_MESSAGE_MAP(add_student, CDialogEx)
END_MESSAGE_MAP()

