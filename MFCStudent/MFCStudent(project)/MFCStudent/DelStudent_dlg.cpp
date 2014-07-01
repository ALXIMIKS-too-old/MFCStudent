// DelStudent_dlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCStudent.h"
#include "DelStudent_dlg.h"
#include "afxdialogex.h"


// диалоговое окно del_student

IMPLEMENT_DYNAMIC(del_student, CDialogEx)

del_student::del_student(CWnd* pParent /*=NULL*/)
	: CDialogEx(del_student::IDD, pParent)
	, m_text(_T(""))
	, m_dialog_name("")
{

}


BOOL del_student::OnInitDialog()
{
	BOOL ret = CDialogEx::OnInitDialog();
	if (m_dialog_name != "")
		SetWindowText(m_dialog_name);
	return ret;
}

del_student::~del_student()
{
}

void del_student::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT, m_text);
}


BEGIN_MESSAGE_MAP(del_student, CDialogEx)
END_MESSAGE_MAP()

