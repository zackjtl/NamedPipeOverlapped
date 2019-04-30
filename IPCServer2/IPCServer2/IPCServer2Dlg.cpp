
// IPCServer2Dlg.cpp: 實作檔案
//
//---------------------------------------------------------------------------
#include "stdafx.h"
#include "IPCServer2.h"
#include "IPCServer2Dlg.h"
#include "afxdialogex.h"
#include "UsrMsg.h"
#include <tchar.h>
#include <strsafe.h>
#include "IniFile.h"
//---------------------------------------------------------------------------
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//---------------------------------------------------------------------------
// 對 App About 使用 CAboutDlg 對話方塊
//---------------------------------------------------------------------------
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支援

// 程式碼實作
protected:
	DECLARE_MESSAGE_MAP()
};
//---------------------------------------------------------------------------
CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}
//---------------------------------------------------------------------------
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
//---------------------------------------------------------------------------
// CIPCServer2Dlg 對話方塊
//---------------------------------------------------------------------------
CIPCServer2Dlg::CIPCServer2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IPCSERVER2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Text);
	DDX_Control(pDX, IDC_EDIT1, m_LogEdit);
	DDX_Control(pDX, IDC_EDIT2, m_ClientEdit);
}
//---------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CIPCServer2Dlg, CDialogEx)	
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(WM_THREAD_DONE, OnThreadDone)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_BTNBROWSE, &CIPCServer2Dlg::OnBnClickedBtnbrowse)
END_MESSAGE_MAP()
//---------------------------------------------------------------------------
// CIPCServer2Dlg 訊息處理常式
//---------------------------------------------------------------------------
BOOL CIPCServer2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	AfxInitRichEdit5();

	// 將 [關於...] 功能表加入系統功能表。

	// IDM_ABOUTBOX 必須在系統命令範圍之中。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定
	
	m_Connected = false;

	m_NPServer.Create(L"\\\\.\\pipe\\mynamedpipe", 16384);
	
	m_NPThread = new CNamedPipeThread(m_hWnd, &m_NPServer);	
	m_NPThread->m_bAutoDelete = false;
	m_NPThread->Function = 0;
	////m_NPThread->LogCallBack = ShowText;
	m_NPThread->CreateThread();
	
	m_Connected = true;

	if (PathFileExists(L"ui.ini")) {
		CIniFile iniFile(L"ui.ini");

		wstring clientFile = iniFile.ReadString(L"Main", L"Client File", L"");
		m_ClientEdit.SetWindowText(clientFile.c_str());
	}
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnCancel()
{
	if (!PathFileExists(L"ui.ini")) {
		CIniFile iniFile(L"ui.ini", FILE_CREATE_NEW);
		iniFile.UpdateFile();
	}
	CString winText;
	m_ClientEdit.GetWindowText(winText);

	CIniFile iniFile(L"ui.ini");
	iniFile.WriteString(L"Main", L"Client File", (LPCWSTR)winText);
	iniFile.UpdateFile();

	CDialog::OnCancel();
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::DumpBuffer(BYTE* Buffer, UINT Size)
{
	CString temp;
	CString text;

	text.Format(L"Buffer Data (%d bytes)\r\n", Size);
	ShowText(text);

	for (int i = 0; i < Size; ++i) {

		if (i != 0 && (i % 16 == 0)) {
			ShowText(L"\r\n");
		}
		temp.Format(L"%02x ", Buffer[i]);
		ShowText(temp);
	}
	ShowText(L"\r\n");
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//---------------------------------------------------------------------------
// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//---------------------------------------------------------------------------
CIPCServer2Dlg::~CIPCServer2Dlg()
{
	if (m_NPServer.IsConnected()) {
		m_NPServer.DisConnect();
	}
}
//---------------------------------------------------------------------------
void __stdcall CIPCServer2Dlg::ShowText(LPCWSTR Text)
{
	m_LogEdit.SetSel(-1, -1);
	m_LogEdit.ReplaceSel(Text);
}
//---------------------------------------------------------------------------
// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
//---------------------------------------------------------------------------
HCURSOR CIPCServer2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	
	////m_NPThread->Delete();
	m_NPThread = new CNamedPipeThread(m_hWnd, &m_NPServer);

	byte buffer[16384];
	memset(buffer, 0x77, 32);
	m_NPThread->m_bAutoDelete = false;
	m_NPThread->Function = NamedPipeWrite;
	m_NPThread->Data = buffer;
	m_NPThread->Length = 32;
	m_NPThread->CreateThread();

	////m_NPThread->Delete();
	m_NPThread = new CNamedPipeThread(m_hWnd, &m_NPServer);

	m_NPThread->m_bAutoDelete = false;
	m_NPThread->Function = NamedPipeRead;
	m_NPThread->Data = buffer;
	m_NPThread->Length = 16384;
	m_NPThread->CreateThread();

	////CDialogEx::OnOK();
}
//---------------------------------------------------------------------------
LRESULT CIPCServer2Dlg::OnThreadDone(WPARAM wParam, LPARAM lParam)
{
	CString text;
	int function = wParam;
	int result = lParam;

	if (function == NamedPipeConnect) {
		if (result) {
			ShowText(L"Connect successfully.\r\n");
		}
		else {
			ShowText(L"Connection failed.\r\n");
		}
	}
	else if (function == NamedPipeWrite) {
		if (result) {
			ShowText(L"Write data successfully.\r\n");
		}
		else {
			ShowText(L"Write data failed.\r\n");
		}
	}
	else if (function == NamedPipeRead) {
		if (result) {
			text.Format(L"Read data %d bytes successfully.\r\n", m_NPThread->BytesRead);
			ShowText(text);
			DumpBuffer(m_NPThread->Data, m_NPThread->BytesRead);
		}
		else {
			ShowText(L"Read data failed.\r\n");
		}
	}

	WaitForSingleObject(m_NPThread->m_hThread, 5000);

	m_NPThread->Delete();

	return 0;
}
//---------------------------------------------------------------------------
void CIPCServer2Dlg::OnBnClickedBtnbrowse()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	const TCHAR szFilter[] = L"executable file (*.exe)|*.exe|All Files (*.*)|*.*||";
	CFileDialog dlg(TRUE, L"exe", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		m_ClientEdit.SetWindowText(sFilePath);
	}
}
//---------------------------------------------------------------------------