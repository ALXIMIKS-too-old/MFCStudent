
// MFCStudent.h : ������� ���� ��������� ��� ���������� PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�������� stdafx.h �� ��������� ����� ����� � PCH"
#endif

#include "resource.h"		// �������� �������


// CMFCStudentApp:
// � ���������� ������� ������ ��. MFCStudent.cpp
//

class CMFCStudentApp : public CWinApp
{
public:
	CMFCStudentApp();

// ���������������
public:
	virtual BOOL InitInstance();

// ����������

	DECLARE_MESSAGE_MAP()
};

extern CMFCStudentApp theApp;