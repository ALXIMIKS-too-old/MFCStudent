
// MFCStudentDlg.cpp : file of realization

#include "stdafx.h"
#include "MFCStudent.h"
#include "MFCMain_dlg.h"
#include "afxdialogex.h"
#include "AddStudent_dlg.h"	   
#include "DelStudent_dlg.h"

#include "pugixml.hpp"				//  XML DOM-type parser
#include <GdiPlus.h>				// 	for GetThumbnailImage function

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
, m_storage_name(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStudentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_list_ctrl);
	DDX_Text(pDX, IDC_STORAGE, m_storage_name);
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
	ON_BN_CLICKED(IDC_BUTTON_ADD_RAND, &CStudentDlg::OnButton_AddRand)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CStudentDlg::OnHdnItemclickList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, &CStudentDlg::OnNMClickList)
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
	CString	 config_name("config.ini");						// name of config file
	CString  default_config_name("config_default.ini");		// name of  default config file	 	
	CString DataStorageFile_default("MFCStudent_default.db"); // name of default data storage
	DataStorageFile = "MFCStudent.db";						// name of data storage	 	
	CString IconFile("icon.jpg");							// name of photofile
	int IconW³dth = 16;										// IconW³dth of image
	int IconLength = 16;									// IconLength of image
	int RandItemsNumber = 10;								// count of new Items fo first start	
//--------------------------------------------------------------------------------------------------------------
	// read init file
	if (read_init(config_name, default_config_name, DataStorageFile, DataStorageFile_default,
		IconFile, IconW³dth, IconLength, RandItemsNumber) == -1){
		MessageBox(_T("Wrong compilation parameters of program"), _T("Errore"), MB_OK | MB_ICONERROR);
	}

	m_storage_name = _T("Storage Name: ") + DataStorageFile;  
	UpdateData(FALSE);

	// Create of colums in LIST
	m_list_ctrl.InsertColumn(0, _T("    Student Name"), LVCFMT_LEFT, 300);
	m_list_ctrl.InsertColumn(1, _T("Student Mark"), LVCFMT_CENTER, 100);

	// Set parameters of image
	m_image_list.Create(IconW³dth, IconLength, ILC_MASK | ILC_COLOR32, 1, 1);
	m_list_ctrl.SetImageList(&m_image_list, LVSIL_SMALL);

	// Where to find image
	CString  folder;
	GetCurrentDirectory(_MAX_PATH, folder.GetBuffer(_MAX_PATH));
	folder.ReleaseBuffer();
	folder = folder + "\\" + IconFile;

	// Load image
	CImage image;
	if (image.Load(folder) != 0){
		CString err = CString("Icon file not found: ") + folder + CString("\nProgram will start without icon.");
		MessageBox(err, _T("Initialization errore"), MB_OK | MB_ICONERROR);			 // if don't load
	}
	else{
		// Change bitmap size of loading image
		HBITMAP hBit = image.Detach();

		Gdiplus::Bitmap* bmp = Gdiplus::Bitmap::FromHBITMAP(hBit, 0);					 //	 <GdiPlus.h>
		bmp = static_cast<Gdiplus::Bitmap*> (bmp->GetThumbnailImage(IconW³dth, IconLength));	 //	 <GdiPlus.h>
		bmp->GetHBITMAP(Gdiplus::Color(), &hBit);										 //	 <GdiPlus.h>

		CBitmap bitmap;
		bitmap.Attach(hBit);

		m_image_list.Add(&bitmap, RGB(0, 0, 0));	   // don't work with 	   m_image_list.Add(&bitmap, &bitmap);
	}

	// serialization
	Serialize(DataStorageFile, CArchive::load, RandItemsNumber);

	//	Rebuild of LIST
	FillList();
				 
	// disable 	 BUTTON_EDIT and   BUTTON_DEL
	GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);
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
	m_list_ctrl.SetFocus();
}

//-------------------------------------------------------------------------------------------
// Realization of  CStudentDlg
//-------------------------------------------------------------------------------------------

//event handler: Clicked on button  Add
void CStudentDlg::OnButton_Add(){
	add_student add_std;
	add_std.m_dialog_name = "Add New Item";
	if (add_std.DoModal() == IDOK){		 // do dialog  add_student
		// get entered data
		CString smark;
		stud st;
		smark.Format(_T("%g"), add_std.m_mark);
		st.name = add_std.m_name;
		st.mark = add_std.m_mark;

		// add new student to LIST and to students (CList)
		POSITION insert_pos = m_students.AddTail(st);
		Serialize(DataStorageFile, CArchive::store);

		int nItem = m_list_ctrl.InsertItem(m_list_ctrl.GetItemCount(), add_std.m_name);
		m_list_ctrl.SetItemText(nItem, 1, smark);
		m_list_ctrl.SetItemData(nItem, DWORD_PTR(insert_pos));

		//set LVIS_SELECTED  and  LVIS_FOCUSED	for added Item
		m_list_ctrl.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_list_ctrl.EnsureVisible(nItem, FALSE);

		m_list_ctrl.SetFocus();
		OnNMClickList(NULL, NULL);
	}
	m_list_ctrl.SetFocus();
}

// event handler: Clicked on button  Edit
void CStudentDlg::OnButton_Edit(){
	if (m_list_ctrl.GetSelectedCount() > 1){
		MessageBox(_T("Select only one Item"), _T("Edit Item"), MB_OK | MB_ICONINFORMATION);
		return;
	}
	// count of selected Items of LIST == 1	 or  0
	POSITION pos = m_list_ctrl.GetFirstSelectedItemPosition();
	
	int nItem = m_list_ctrl.GetNextSelectedItem(pos);
	POSITION pos_in_list = reinterpret_cast<POSITION> (m_list_ctrl.GetItemData(nItem));	 // get  POSITION of stud in students (CList)

	// init for add_student dialog
	
	add_student add_std;
	add_std.m_dialog_name = "Edit Item";
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
			Serialize(DataStorageFile, CArchive::store); 		
	}

	m_list_ctrl.SetFocus();
	OnNMClickList(NULL, NULL);
}

//  event handler: Clicked on button  Delete
void CStudentDlg::OnButton_Del(){
	int selected_count = m_list_ctrl.GetSelectedCount();	   	
	if (selected_count == 0) {
		MessageBox(_T("Select some Item"), _T("Delete Item"), MB_OK | MB_ICONINFORMATION);
		return;
	}

	// init for del_student dialog
	del_student del_std;
	if (selected_count == 1){
		del_std.m_text = "Do you want to delete selected Item?";
		del_std.m_dialog_name = "Delete Item";
	}
	else {
		del_std.m_text = "Do you want to delete selected items?";
		del_std.m_dialog_name = "Delete Items";
	}

	POSITION pos = m_list_ctrl.GetFirstSelectedItemPosition();
	if (IDOK == del_std.DoModal()){
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
		Serialize(DataStorageFile, CArchive::store);

		m_list_ctrl.SetItemState(num, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_list_ctrl.EnsureVisible(num, FALSE);
	}

	m_list_ctrl.SetFocus();
	OnNMClickList(NULL, NULL);
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
	Serialize(DataStorageFile, CArchive::store);

	int nItem = m_list_ctrl.InsertItem(m_list_ctrl.GetItemCount(), st.name);
	m_list_ctrl.SetItemText(nItem, 1, smark);
	m_list_ctrl.SetItemData(nItem, DWORD_PTR(insert_pos));

	//set LVIS_SELECTED  and  LVIS_FOCUSED	for added Item
	m_list_ctrl.SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	m_list_ctrl.EnsureVisible(nItem, FALSE);

	m_list_ctrl.SetFocus();
	OnNMClickList(NULL, NULL);
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

	 //sort
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
	Serialize(DataStorageFile, CArchive::store);
	CDialogEx::OnCancel();
}

// fucntion for Serialization of data and creating random data for first start
void CStudentDlg::Serialize(const CString &DataStorageFile, int flag, int RandItemsNumber)
{
	CFile theFile;
	BOOL open_result = TRUE;
	// open FILE for write
	if (flag == CArchive::store) {
		open_result = theFile.Open(DataStorageFile, CFile::modeCreate | CFile::modeWrite);
	}
	else {
		// open FILE for load
		if (!theFile.Open(DataStorageFile, CFile::modeRead)){
			// First start of program - load random items for LIST	  
			stud st;
			for (int i = 0; i < RandItemsNumber; ++i){
				st.name.Format(_T("Student %d"), rand() / 100 + 1);
				st.mark = rand() % 100 + 1;
				m_students.AddTail(st);
			}
			// for serialize random data
			flag = CArchive::store;
			open_result = theFile.Open(DataStorageFile, CFile::modeCreate | CFile::modeWrite);
		}
	}

	// problem with opening or creating file
	if (open_result == FALSE){			   
		static BOOL first_err = TRUE;
		if (first_err == TRUE){
			first_err = false;
			MessageBox(_T("Problem with storage file ") + DataStorageFile, _T("Errore"), MB_OK | MB_ICONERROR);
		}
		return;
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
	CStudentDlg* pThis = (CStudentDlg*)(lParamSort);
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

//  disable/enable  DEL and EDIT buttons
void CStudentDlg::OnNMClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int sel_count = m_list_ctrl.GetSelectedCount();

	// if selected 0 Item - disable DEL and EDIT buttons
	if (sel_count == 0){
		GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(FALSE);
	}
	// if selected 1 Item - enable DEL and EDIT buttons
	else if (sel_count == 1){
		GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(TRUE);
	}
	// if selected more than 1 Item - enable DEL and disable EDIT buttons
	else 
	{
		GetDlgItem(IDC_BUTTON_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(TRUE);
	}
	if (pResult != NULL)
	*pResult = 0;
}


//creating init file
BOOL create_init_file(const CString &config_name, const CString & DataStorageFile,
	const CString &IconFile, int IconW³dth, int IconLength, int RandItemsNumber){

	// from Unicode to Ansi
	CT2CA String_DataStorageFile(DataStorageFile);
	CT2CA String_IconFile(IconFile);

	pugi::xml_document doc;
	pugi::xml_node Profile = doc.append_child("Profile");
	Profile.append_attribute("FormatVersion") = 1;
	pugi::xml_node Tools = Profile.append_child("Tools");
	Tools.append_attribute("Version") = 1;
	pugi::xml_node Tool = Tools.append_child("Tool");
	Tool.append_attribute("DataStorageFile") = String_DataStorageFile;
	Tool.append_attribute("RandItemsNumber") = RandItemsNumber;
	Tool.append_attribute("IconFile") = String_IconFile;
	Tool.append_attribute("IconW³dth") = IconW³dth;
	Tool.append_attribute("IconLength") = IconLength;
	

	return doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
}

int  CStudentDlg::read_init(const CString &config_name, const CString &default_config_name, CString &DataStorageFile,
	 const CString & DataStorageFile_default, CString &IconFile, int &IconW³dth, int &IconLength, int &RandItemsNumber){

	CString DataStorageFile_tmp = DataStorageFile;						// name of data storage
	CString IconFile_tmp = IconFile;									// name of photofile
	int IconW³dth_tmp = IconW³dth;										// IconW³dth of image
	int IconLength_tmp = IconLength;									// IconLength of image
	int RandItemsNumber_tmp = RandItemsNumber;							// count of new Items fo first start		

	static int recursion_count = 0;
	++recursion_count;

	// load config from config.ini
	int result = 0;
	while (TRUE){
		pugi::xml_document doc;
		pugi::xml_parse_result res;

		// load from init file
		CString error_name;
		res = doc.load_file(config_name);
		if (res == FALSE){
			error_name = _T("Could not open \"") + config_name + _T("\" file.\n");
			CString descr = CString(res.description());
			error_name += CString("Description: ") + descr;
			if (descr == CString("File was not found") ||
				descr == CString("No document element found")){
				result = 1;
				error_name += _T("\nWill be ctreated standart config  \"") + config_name + _T("\".\n");
			}
			else{
				result = 2;
				error_name += _T("\nWill be used default config \"") + default_config_name + _T("\".\n");
			}				
			MessageBox(error_name, _T("Initialization Error"), MB_OK | MB_ICONERROR);
			break;
		}
		// get Node	  Profile
		pugi::xml_node profile_node = doc.child("Profile");
		if (profile_node == FALSE){
			result = 2;
			error_name = _T("Problem with \"") + config_name + _T("\" file.\n");
			error_name += _T("Will be used default config \"") + default_config_name + _T("\".\n");
			error_name += _T("Path: Root") + CString(doc.path().c_str());
			error_name += _T(":\nNode \"Profile\" was not fount.");
			MessageBox(error_name, _T("Initialization Error"), MB_OK | MB_ICONERROR);
			break;
		}
		// get atribute	  FormatVersion
		pugi::xml_attribute atribute = profile_node.attribute("FormatVersion");
		if (atribute == FALSE){
			result = 2;
			error_name = _T("Problem with \"") + config_name + _T("\" file.\n");
			error_name += _T("Will be used default config \"") + default_config_name + _T("\".\n");
			error_name += _T("Path: Root") + CString(profile_node.path().c_str());
			error_name += _T(":\nAtribute \"FormatVersion\" was not fount.");
			MessageBox(error_name, _T("Initialization Error"), MB_OK | MB_ICONERROR);
			break;
		}
		// find Node Tools 
		const pugi::char_t* char_atribute = atribute.as_string();
		pugi::xml_node tools = profile_node.find_child_by_attribute("Tools", "Version", char_atribute);
		if (tools == FALSE){
			result = 2;
			error_name = _T("Problem with \"") + config_name + _T("\" file.\n");
			error_name += _T("Will be used default config \"") + default_config_name + _T("\".\n");
			error_name += _T("Path: Root") + CString(profile_node.path().c_str());
			error_name += _T(":\nNode \"Tools\" with atribute \"Version\" = \"");
			error_name += CString(char_atribute) + _T(" \"was not fount.");
			MessageBox(error_name, _T("Initialization Error"), MB_OK | MB_ICONERROR);
			break;
		}
		// get Node	 Tool
		pugi::xml_node tool = tools.child("Tool");
		if (tool == FALSE){
			result = 2;
			error_name = _T("Problem with \"") + config_name + _T("\" file.\n");
			error_name += _T("Will be used default config \"") + default_config_name + _T("\".\n");
			error_name += _T("Path: Root") + CString(tools.path().c_str());
			error_name += _T(":\nNode \"Tool\" was not fount.");
			MessageBox(error_name, _T("Initialization Error"), MB_OK | MB_ICONERROR);
			break;
		}
		// get atribute  RandItemsNumber
		atribute = tool.attribute("RandItemsNumber");
		RandItemsNumber = atribute.as_int();
		if (atribute == FALSE){
			pugi::xml_attribute first_atr = tool.first_attribute();
			if (!first_atr.empty())
				tool.insert_attribute_before("RandItemsNumber", tool.first_attribute()) = RandItemsNumber_tmp;
			else
				tool.append_attribute("RandItemsNumber") = RandItemsNumber_tmp;
			doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
			RandItemsNumber = RandItemsNumber_tmp;
		}
		else{
			if (RandItemsNumber < 0){
				atribute.set_value(RandItemsNumber_tmp);
				doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
				RandItemsNumber = RandItemsNumber_tmp;
			}
		}
		// get atribute  DataStorageFile  		
		atribute = tool.attribute("DataStorageFile");
		DataStorageFile = atribute.as_string();
		if (atribute == FALSE || (DataStorageFile.Trim() == "")){
			error_name = _T("Problem with \"") + config_name + _T("\" file.\n");
			error_name += _T("Will be used standart storage file \"") + DataStorageFile_tmp + _T("\".\n");
			error_name += _T("Path: Root") + CString(tool.path().c_str());
			error_name += _T(":\nAtribute \"DataStorageFile\" was not fount.");
			MessageBox(error_name, _T("Initialization Error"), MB_OK | MB_ICONERROR);

			CT2CA pszCharacterString(DataStorageFile_tmp);
			pugi::xml_attribute first_atr = tool.first_attribute();
			if (!first_atr.empty())
				tool.insert_attribute_before("DataStorageFile", tool.first_attribute()) = pszCharacterString;
			else
				tool.append_attribute("DataStorageFile") = pszCharacterString;
			DataStorageFile = DataStorageFile_tmp;
			doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
		}
		else{
			if (DataStorageFile.Trim() == ""){
				CT2CA pszCharacterString(DataStorageFile_tmp);
				atribute.set_value(pszCharacterString);
				doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
				DataStorageFile = DataStorageFile_tmp;
			}
		}
		
		// check file of storage
		CFile check;
		if (check.Open(DataStorageFile, CFile::modeRead) == FALSE){		 // check opening of file
			if (check.Open(DataStorageFile, CFile::modeCreate | CFile::modeWrite) == TRUE){
				check.Close();
				stud st;
				for (int i = 0; i < RandItemsNumber; ++i){
					st.name.Format(_T("Student %d"), rand() / 100 + 1);
					st.mark = rand() % 100 + 1;
					m_students.AddTail(st);
				}
				Serialize(DataStorageFile, CArchive::store, RandItemsNumber);
				m_students.RemoveAll();
			}
			else{
				if (config_name == default_config_name || recursion_count == 2){
					result = -1;
					break;
				}
				error_name = _T("Problem with \"") + config_name + _T("\" file.\n");
				error_name += _T("Will be used default config \"") + DataStorageFile_default + _T("\".\n");
				error_name += _T("Description: could not open \"DataStorageFile\" \"") + DataStorageFile + _T("\"");
				MessageBox(error_name, _T("Initialization Error"), MB_OK | MB_ICONERROR);
				result = 2;
				break;
			}
		}
		else{
			check.Close();
		}
		
		//get atribute 	IconFile
		atribute = tool.attribute("IconFile");
		IconFile = atribute.as_string();
		if (atribute == false){
			CT2CA pszCharacterString(IconFile_tmp);
			pugi::xml_attribute first_atr = tool.first_attribute();
			if (!first_atr.empty())
				tool.insert_attribute_before("IconFile", tool.first_attribute()) = pszCharacterString;
			else
				tool.append_attribute("IconFile") = pszCharacterString;
			doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
			IconFile = IconFile_tmp;
		}
		else{
			if (IconLength < 0)	 {
				CT2CA pszCharacterString(IconFile_tmp);
				atribute.set_value(pszCharacterString);
				doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
				IconFile = IconFile_tmp;
			}
		}
		//get atribute 	IconW³dth
		atribute = tool.attribute("IconW³dth");
		IconW³dth = atribute.as_int();
		if (atribute == false){
			pugi::xml_attribute first_atr = tool.first_attribute();
			if (!first_atr.empty())
				tool.insert_attribute_before("IconW³dth", tool.first_attribute()) = IconW³dth_tmp;
			else
				tool.append_attribute("IconW³dth") = IconW³dth_tmp;
			doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
			IconW³dth = IconW³dth_tmp;
		}
		else{
			if (IconLength < 0)	 {
				atribute.set_value(IconW³dth_tmp);
				doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
				IconW³dth = IconW³dth_tmp;
			}
		}

		//get atribute 	IconLength
		atribute = tool.attribute("IconLength");
		IconLength = atribute.as_int();
		if (atribute == false){
			pugi::xml_attribute first_atr = tool.first_attribute();
			if (!first_atr.empty())
				tool.insert_attribute_before("IconLength", tool.first_attribute()) = IconLength_tmp;
			else
				tool.append_attribute("IconLength") = IconLength_tmp;
			doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
			IconLength = IconLength_tmp;
		}
		else{
			if (IconLength < 0)	 {
				atribute.set_value(IconLength_tmp);
				doc.save_file(config_name, "\t", 1U | 32U, pugi::xml_encoding::encoding_utf8);
				IconLength = IconLength_tmp;
			}
		}
		// exit
		break;
	}
	// finding what to do with result
	switch (result){
	case 1:
		// create new file with standart config
		if (!create_init_file(config_name, DataStorageFile_tmp, IconFile_tmp, IconW³dth_tmp, IconLength_tmp, RandItemsNumber_tmp))
			return -1;
		return read_init(config_name, default_config_name, DataStorageFile, DataStorageFile_default,
			IconFile, IconW³dth, IconLength, RandItemsNumber);
	case 2:
		// create new file with default config
		if (!create_init_file(default_config_name, DataStorageFile_default, IconFile_tmp, IconW³dth_tmp, IconLength_tmp, 0))
			return -1;
		return read_init(default_config_name, default_config_name, DataStorageFile, DataStorageFile_default,
			IconFile, IconW³dth, IconLength, RandItemsNumber);
	case -1:
		// problem with compiling configuration
		return -1;
	}		
	return result;	 
}

