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
	, m_dialog_name("")								   // dialog name
	, m_text_range(TRUE)							   // mark range - 1...100
{
}

BOOL add_student::OnInitDialog()
{
	BOOL ret = CDialogEx::OnInitDialog();				// default 	OnInitDialog
	if (m_dialog_name != "")
		SetWindowText(m_dialog_name);					// Set dialog name
	return ret;
}

add_student::~add_student()
{
}

void add_student::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_name);					 // binding EDIT place ID and m_name variable
	DDX_Text(pDX, IDC_EDIT2, m_mark);					 // binding EDIT place ID and m_mark variable
	DDX_Control(pDX, IDC_RANGE, m_range_text);			 // binding STATIC TEXT and m_range_text control object
}

BEGIN_MESSAGE_MAP(add_student, CDialogEx)
	ON_EN_UPDATE(IDC_EDIT2, &add_student::OnUpdateStudentMark)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()	 

void add_student::OnUpdateStudentMark()					 // 
{
	UpdateData(TRUE);
	if (m_mark > 100 || m_mark < 1){
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		m_text_range = FALSE;
		m_range_text.Invalidate(1);
	}
	else{						
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		m_text_range = TRUE;
		m_range_text.Invalidate(1);
	}
}	  

HBRUSH add_student::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{	
	HBRUSH hBrush = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// Selecting of color for STATIC TEXT with saving background color
	if ((nCtlColor == CTLCOLOR_STATIC) && (pWnd->GetSafeHwnd() == GetDlgItem(IDC_RANGE)->GetSafeHwnd())){
		if (m_text_range == TRUE)
			pDC->SetTextColor(RGB(0, 150, 0));			// green
		if (m_text_range == FALSE)
			pDC->SetTextColor(RGB(255, 0, 0));			// red
		return (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	return hBrush;
}
