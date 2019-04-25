
// IPCClient1Dlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "IPCClient1.h"
#include "IPCClient1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 對 App About 使用 CAboutDlg 對話方塊

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

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CIPCClient1Dlg 對話方塊



CIPCClient1Dlg::CIPCClient1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IPCCLIENT1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPCClient1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}

BEGIN_MESSAGE_MAP(CIPCClient1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CIPCClient1Dlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CIPCClient1Dlg 訊息處理常式

BOOL CIPCClient1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	ConnectToNamedPipe();

	m_pThread = AfxBeginThread(threadHandleDataIn, this);

	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

UINT CIPCClient1Dlg::ConnectToNamedPipe()
{
	LPTSTR lpvMessage = TEXT("Default message from client.");
	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");

	CString msg;

	// Try to open a named pipe; wait for it, if necessary. 

	while (1) {
		m_hPipe = CreateFile(
			lpszPipename,   // pipe name 
			GENERIC_READ |  // read and write access 
			GENERIC_WRITE,
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			FILE_FLAG_OVERLAPPED,              // default attributes 
			NULL);          // no template file 

	  // Break if the pipe handle is valid. 
		if (m_hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY) {
			msg.Format(L"Could not open pipe. GLE=%d\n", GetLastError());
			return -1;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(lpszPipename, 20000)) {
			msg.Format(L"Could not open pipe: 20 second wait timed out.");
			return -1;
		}
	}
	// The pipe connected; change to message-read mode. 
	/*
	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		m_hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess) {
		msg.Format(L"SetNamedPipeHandleState failed. GLE=%d\n", GetLastError());
		return -1;
	}*/
}

UINT threadHandleDataIn(LPVOID pVar)
{
	CIPCClient1Dlg* pDlg = (CIPCClient1Dlg*)pVar;
	return pDlg->ListenDataIn();
}

UINT CIPCClient1Dlg::ListenDataIn()
{
	DWORD i, dwWait, cbRead, dwErr;
	BOOL fSuccess;
	CString msg;

	byte buffer[256];

	while (1) {
		fSuccess = ReadFile(
			m_hPipe,    // pipe handle 
			buffer,    // buffer to receive reply 
			256,  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);

		if (cbRead > 0) {
			ShowText(L"Got data\r\n");
			DumpBuffer(buffer, cbRead);
		}
	}

	return 1;
}

UINT CIPCClient1Dlg::WriteDataToPipe()
{
	bool fSuccess;
	DWORD cbWritten;

	byte buffer[256];

	memset(buffer, 0x5a, sizeof(buffer));

	fSuccess = WriteFile(
		m_hPipe,                  // pipe handle 
		buffer,             // message 
		sizeof(buffer),              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess)
	{
		_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		return -1;
	}
}

void CIPCClient1Dlg::DumpBuffer(BYTE* Buffer, UINT Size)
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

void CIPCClient1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CIPCClient1Dlg::OnPaint()
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

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CIPCClient1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIPCClient1Dlg::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	////CString CStrMessage = _T("Test message.");                // 測試傳送的字串
	WriteDataToPipe();
	////CDialogEx::OnOK();
}

void CIPCClient1Dlg::ShowText(LPCWSTR Text)
{
	m_Edit.SetSel(-1, -1);
	m_Edit.ReplaceSel(Text);
}