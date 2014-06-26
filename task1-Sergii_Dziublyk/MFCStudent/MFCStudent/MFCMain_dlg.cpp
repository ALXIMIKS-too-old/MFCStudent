
// MFCStudentDlg.cpp : file of realization

#include "stdafx.h"
#include "MFCStudent.h"
#include "MFCMain_dlg.h"
#include "afxdialogex.h"
#include "AddStudent_dlg.h"	   

#include <GdiPlus.h>				// 	for GetThumbnailImage function
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif				 

//-------------------------------------------------------------------------------------------
// dialog CAboutDlg (information window)
//-------------------------------------------------------------------------------------------
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();	  

	enum { IDD = IDD_ABOUTBOX };						 // ID of dialog window

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);     // for DDX/DDV

protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

//-------------------------------------------------------------------------------------------
// dialog CStudentDlg	
//-------------------------------------------------------------------------------------------
CStudentDlg::CStudentDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStudentDlg::IDD, pParent)
{
	m_sortColumn = 1;
	m_sortParam = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStudentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list_ctrl);
}

BEGIN_MESSAGE_MAP(CStudentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

// Binding event handler functions and Objects ID
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CStudentDlg::OnButton_Add)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CStudentDlg::OnButton_Edit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CStudentDlg::OnButton_Del)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CStudentDlg::OnButton_About)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CStudentDlg::On_Dblclk_List) 
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST, &CStudentDlg::OnLvnKeydownList)
	ON_BN_CLICKED(IDC_ADD_RAND, &CStudentDlg::OnButton_AddRand)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CStudentDlg::OnHdnItemclickList)
END_MESSAGE_MAP()


//-------------------------------------------------------------------------------------------
// MYINIT FUNCTION	SYSTEM
//-------------------------------------------------------------------------------------------
BOOL CStudentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);

//-------------------------------------------------------------------------------------------
// MYINIT FUNCTION
//-------------------------------------------------------------------------------------------
	CString	 filename("1");		                        // for  *.jpg and *.bmp
	file_CArchive = "CArchive__Test.txt";				// name of serialization file
	int width = 16;										// width of image
	int length = 16;									// length of image
	

	// Create of colums in LIST
	m_list_ctrl.InsertColumn(0, _T("name"), LVCFMT_LEFT, 200);
	m_list_ctrl.InsertColumn(1, _T("Rating"), LVCFMT_CENTER, 100);
	
	// Set parameters of image
	m_image_list.Create(width, length, ILC_MASK | ILC_COLOR32, 1, 1);
	m_list_ctrl.SetImageList(&m_image_list, LVSIL_SMALL);  
	
	// Where to find image
	CString  folder;
	GetCurrentDirectory(_MAX_PATH, folder.GetBuffer(_MAX_PATH));
	folder.ReleaseBuffer();
	folder = folder + "\\" + filename;
	
	// Load image
	CImage image;	  
	if (image.Load(folder + ".jpg") != 0 && image.Load(folder + ".bmp") != 0){	
		CString err = CString("Don't found image: ") + folder;
		MessageBox(err, _T("Init errore"), MB_OK | MB_ICONERROR);			 // if don't load
	}
	else{
		// Change bitmap size of loading image
		HBITMAP hBit = image.Detach();

		Bitmap* bmp = Bitmap::FromHBITMAP(hBit, 0);						     //	 <GdiPlus.h>
		bmp = static_cast<Bitmap*> (bmp->GetThumbnailImage(width, length));	 //	 <GdiPlus.h>
		bmp->GetHBITMAP(Color(), &hBit);							         //	 <GdiPlus.h>

		CBitmap bitmap;
		bitmap.Attach(hBit);

		m_image_list.Add(&bitmap, RGB(0, 0, 0));	   // don't work with 	   m_image_list.Add(&bitmap, &bitmap);
	}

	// serialization
	Serialize(file_CArchive, CArchive::load);

	//	Rebuild of LIST
	FillList();
	return TRUE;
}
//-------------------------------------------------------------------------------------------
// SYSTEM Functions
//-------------------------------------------------------------------------------------------
void CStudentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CStudentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CStudentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}	   

void CStudentDlg::OnButton_About()
{
	CAboutDlg about;
	about.DoModal();
}

//-------------------------------------------------------------------------------------------
// Realization of  CStudentDlg
//-------------------------------------------------------------------------------------------

//event handler: Clicked on button  Add
void CStudentDlg::OnButton_Add(){
	add_student add_std;
	if (add_std.DoModal() == IDOK){		 // do dialog  add_student
		// get entered data
		CString smark;
		stud st; 
		smark.Format(_T("%g"), add_std.m_mark);
		st.name = add_std.m_name;
		st.mark = add_std.m_mark;

		// add new student to LIST and to students (CList)
		POSITION insert_pos = m_students.AddTail(st);
		Serialize(file_CArchive, CArchive::store);

		int nItem = m_list_ctrl.InsertItem(m_list_ctrl.GetItemCount(), add_std.m_name);					
		m_list_ctrl.SetItemText(nItem, 1, smark);
		m_list_ctrl.SetItemData(nItem, DWORD_PTR(insert_pos));

		//set LVIS_SELECTED  and  LVIS_FOCUSED	for added Item
		m_list_ctrl.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_list_ctrl.EnsureVisible(nItem, FALSE);
	}	 	
}	 

// event handler: Clicked on button  Edit
void CStudentDlg::OnButton_Edit(){	  
	// if count of selected Items of LIST == 0 or > 1
	int select_count = m_list_ctrl.GetSelectedCount();
	if (select_count == 0){
		MessageBox(_T("Select some item"), _T("Edit"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	if (select_count > 1){
		MessageBox(_T("Select only one item"), _T("Edit"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	// if count of selected Items of LIST == 1
	POSITION pos = m_list_ctrl.GetFirstSelectedItemPosition();
	int nItem = m_list_ctrl.GetNextSelectedItem(pos);
	POSITION pos_in_list = reinterpret_cast<POSITION> (m_list_ctrl.GetItemData(nItem));	 // get  POSITION of stud in students (CList)
	
	// init for add_student dialog
	add_student add_std;
	add_std.m_name = m_students.GetAt(pos_in_list).name;
	add_std.m_mark = m_students.GetAt(pos_in_list).mark;

	if (add_std.DoModal() == IDOK){	 						  // do dialog  add_student
		//  check entered data for changes
		BOOL serializ = FALSE;
		if (m_students.GetAt(pos_in_list).name != add_std.m_name){				
			m_students.GetAt(pos_in_list).name = add_std.m_name;

			m_list_ctrl.SetItemText(nItem, 0, add_std.m_name);
			serializ = TRUE;
		}
		if (m_students.GetAt(pos_in_list).mark != add_std.m_mark){
			m_students.GetAt(pos_in_list).mark = add_std.m_mark;

			CString dbl;
			dbl.Format(_T("%g"), add_std.m_mark);
			m_list_ctrl.SetItemText(nItem, 1, dbl);
			serializ = TRUE;
		}
		if (serializ == TRUE)
			Serialize(file_CArchive, CArchive::store);
	}		  
}

//  event handler: Clicked on button  Delete
void CStudentDlg::OnButton_Del(){	  
	POSITION pos = m_list_ctrl.GetFirstSelectedItemPosition();	  	
	if (pos == NULL) {
		MessageBox(_T("Select some item"), _T("Delete"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	if (IDYES == MessageBox(_T("Do you wont to delete selected items?"), _T("Delete"), MB_YESNO | MB_ICONQUESTION)){
		int nItem;				   // position of Items in LIST for delete
		CList<int> stack;		   // for satck position of Items in LIST for delete
		while ((nItem = m_list_ctrl.GetNextSelectedItem(pos)) != -1){
			POSITION pos_in_list = reinterpret_cast<POSITION> (m_list_ctrl.GetItemData(nItem));
			m_students.RemoveAt(pos_in_list);
			stack.AddTail(nItem);	  			
		}

		int num;											 //  number of Item for LVIS_SELECTED and LVIS_FOCUSED
		int last_element = stack.GetTail();
		if (m_list_ctrl.GetItemCount() == last_element + 1)	 // if selectes Item is last in LIST
			num = last_element - stack.GetSize();
		else
			num = last_element - stack.GetSize() + 1;

		// delete selected Items from stack
		POSITION stack_pos = stack.GetTailPosition();
		while (stack_pos){
			m_list_ctrl.DeleteItem(stack.GetAt(stack_pos));
			stack.GetPrev(stack_pos);
		}
		Serialize(file_CArchive, CArchive::store);

		m_list_ctrl.SetItemState(num, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_list_ctrl.EnsureVisible(num, FALSE);
	}
}

//event handler: Clicked on button Add_Rand
void CStudentDlg::OnButton_AddRand()
{
	CString smark;
	stud st;

	st.name.Format(_T("Student %d"), rand() / 100 + 1);
	st.mark = rand() % 100 + 1;			
	smark.Format(_T("%g"), st.mark);

	// add new student to LIST and to students (CList)
	POSITION insert_pos = m_students.AddTail(st);
	Serialize(file_CArchive, CArchive::store);

	int nItem = m_list_ctrl.InsertItem(m_list_ctrl.GetItemCount(), st.name);
	m_list_ctrl.SetItemText(nItem, 1, smark);
	m_list_ctrl.SetItemData(nItem, DWORD_PTR(insert_pos));

	//set LVIS_SELECTED  and  LVIS_FOCUSED	for added Item
	m_list_ctrl.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_list_ctrl.EnsureVisible(nItem, FALSE);
}

//event handler: DblClick on LIST
void CStudentDlg::On_Dblclk_List(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	OnOK();
	*pResult = 0;
}

//  event handler: Keydown on LIST	
void CStudentDlg::OnLvnKeydownList(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);	
	// Keydown	  Delete
	if (GetKeyState(VK_DELETE) & 0x8000){				//if (pLVKeyDow->wVKey == VK_DELETE){  
		OnButton_Del();
	}

	// Keydown	  Left CTRL + A
	if ((GetKeyState(VK_LCONTROL) & 0x8000) && (GetKeyState('A') & 0x8000)){
		int count = m_list_ctrl.GetItemCount();
		while (count >= 0){
			m_list_ctrl.SetItemState(count, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_list_ctrl.EnsureVisible(count, FALSE);
			--count;
		}
	}				
	*pResult = 0;
}		  

// event handler:  Click on head of LIST column
void CStudentDlg::OnHdnItemclickList(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	// get column and diraction
	if (m_sortColumn != phdr->iItem){
		m_sortColumn = phdr->iItem;
		m_sortParam = FALSE;
	}
	else
		m_sortParam = !m_sortParam;

	// sort
	m_list_ctrl.SortItems(CompareFunc, (DWORD_PTR) this);
	*pResult = 0;
}

// event handler:  Keydown ENTER
void CStudentDlg::OnOK(){
	if (m_list_ctrl.GetSelectedCount() == 0){
		OnButton_Add();
	}
	else {
		OnButton_Edit();
	} 
}

// event handler:  Keydown ESC and exit
void CStudentDlg::OnCancel(){	 
	Serialize(file_CArchive, CArchive::store);
	CDialogEx::OnCancel();
}		 

// fucntion for Serialization of data and creating random data for first start
void CStudentDlg::Serialize(const CString &file_CArchive, int flag, int rand_count)
{
	CFile theFile;
	// open FILE for write
	if (flag == CArchive::store) {
		theFile.Open(file_CArchive, CFile::modeCreate | CFile::modeWrite);
	}	  	
	else {
		// open FILE for load
		if (!theFile.Open(file_CArchive, CFile::modeRead)){
			// First start of program - load random items for LIST	   
			stud st;
			for (int i = 1; i < rand_count; ++i){
				st.name.Format(_T("Student %d"), rand() / 100 + 1);
				st.mark = rand() % 100 + 1;
				m_students.AddTail(st);
			}
			// for serialize random data
			flag = CArchive::store;
			theFile.Open(file_CArchive, CFile::modeCreate | CFile::modeWrite);
		}
	}
	// Serialization
	CArchive archive(&theFile, flag);

	if (archive.IsStoring()){
		archive << m_students.GetCount();
		archive << m_sortColumn;
		archive << m_sortParam;
		POSITION head_pos = m_students.GetHeadPosition();
		while (head_pos){
			stud x;
			x = m_students.GetNext(head_pos);
			archive << x.mark;
			archive << x.name;
		}
	}
	else{
		int nCount;
		archive >> nCount;
		archive >> m_sortColumn;
		archive >> m_sortParam;
		for (int i = 0; i<nCount; i++){				
			stud st;
			archive >> st.mark;
			archive >> st.name;
			m_students.AddTail(st);
		}
	}
}

// function for rebuild  of LIST
void CStudentDlg::FillList(){
	// delete all Items form LIST
	m_list_ctrl.DeleteAllItems();
	stud st;
	CString cstr;
	int nItem;

	// Insert new Items from CList 
	POSITION head_pos = m_students.GetHeadPosition();
	while (head_pos){			
		POSITION last = head_pos;
		st = m_students.GetNext(head_pos);

		nItem = m_list_ctrl.InsertItem(m_list_ctrl.GetItemCount(), st.name, 0);

		cstr.Format(_T("%g"), st.mark);
		m_list_ctrl.SetItemText(nItem, 1, cstr);
		m_list_ctrl.SetItemData(nItem, DWORD_PTR(last));  
	}
	m_list_ctrl.SortItems(CompareFunc, (DWORD_PTR) this);
	return;
}	

// compare using ItemDATA
int CStudentDlg::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort){
	// get Position from ItemDATA
	POSITION pos1 = reinterpret_cast<POSITION>  (lParam1);
	POSITION pos2 = reinterpret_cast<POSITION>  (lParam2);
	
	// get stud from Clist
	CStudentDlg* pThis = (CStudentDlg*) (lParamSort);
	stud st1 = pThis->m_students.GetAt(pos1);
	stud st2 = pThis->m_students.GetAt(pos2);

	// compare
	int nResult = 0;
	switch (pThis->m_sortColumn){
	case 0:
		nResult = st1.name.CompareNoCase(st2.name);
		break;
	case 1:
		if (st1.mark < st2.mark)
			nResult = -1;
		else if (st1.mark == st2.mark)
			nResult = 0;
		else if (st1.mark > st2.mark)
			nResult = 2;
		break;
	}
	// diraction
	if (pThis->m_sortParam == FALSE){
		nResult = nResult  * (-1);
	}
	return nResult;
}