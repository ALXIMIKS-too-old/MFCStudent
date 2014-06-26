
// MFCStudent.h : главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CMFCStudentApp:
// О реализации данного класса см. MFCStudent.cpp
//

class CMFCStudentApp : public CWinApp
{
public:
	CMFCStudentApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CMFCStudentApp theApp;