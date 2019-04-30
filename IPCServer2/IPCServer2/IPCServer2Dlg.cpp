
// IPCServer2Dlg.cpp: 實作檔案
//

#include "stdafx.h"
#include "IPCServer2.h"
#include "IPCServer2Dlg.h"
#include "afxdialogex.h"
#include "UsrMsg.h"
#include <tchar.h>
#include <strsafe.h>

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


// CIPCServer2Dlg 對話方塊



CIPCServer2Dlg::CIPCServer2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IPCSERVER2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIPCServer2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Text);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}

BEGIN_MESSAGE_MAP(CIPCServer2Dlg, CDialogEx)	
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(WM_THREAD_DONE, OnThreadDone)
END_MESSAGE_MAP()


// CIPCServer2Dlg 訊息處理常式

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
	
	////if (!m_NPServer.Connect()) {
		////ShowText(L"Create Named Pipe Connection Failed\r\n");
		////return TRUE;
	////}
	////m_ThreadConnectNP = AfxBeginThread(threadConnectNamedPipe, this);
	m_NPThread = new CNamedPipeThread(m_hWnd, &m_NPServer);	
	////m_NPThread->m_bAutoDelete = false;
	m_NPThread->Function = 0;
	////m_NPThread->LogCallBack = ShowText;
	m_NPThread->CreateThread();
	

	////m_NPThread = (CNamedPipeThread*)AfxBeginThread(RUNTIME_CLASS(CNamedPipeThread));
	/*
	if (CreateNamedPipeConnection() == 0) {
		ShowText(L"Create Named Pipe Connection Failed\r\n");
		return TRUE;
	}
	m_pThread = AfxBeginThread(threadHandleEvenbts, this);
	*/
	m_Connected = true;


	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

LRESULT CIPCServer2Dlg::OnThreadDone(WPARAM wParam, LPARAM lParam)
{
	int function = wParam;
	int result = lParam;

	if (function == 0) {
		if (result) {
			ShowText(L"Connect successfully.\r\n");
		}
		else {
			ShowText(L"Connection failed.\r\n");
		}
	}
	else if (function == 1) {
		if (result) {
			ShowText(L"Write data successfully.\r\n");
		}
		else {
			ShowText(L"Write data failed.\r\n");
		}
	}

	return 0;
}

UINT threadConnectNamedPipe(LPVOID pVar)
{	
	CIPCServer2Dlg* pDlg = (CIPCServer2Dlg*)pVar;	
	pDlg->m_Connected = false;

	UINT result = pDlg->m_NPServer.Connect();	

	if (result) {
		pDlg->ShowText(L"Connect Successfully\r\n");
		pDlg->m_Connected = true;
	}
	else {
		pDlg->ShowText(L"Connect Failed\r\n");
	}
	return result;
}


UINT CIPCServer2Dlg::CreateNamedPipeConnection()
{
	DWORD i, dwWait, cbRet, dwErr;
	BOOL fSuccess;
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");
	CString msg;

	for (i = 0; i < INSTANCES; i++) {
		// Create an event object for this instance. 

		m_hEvents[i] = CreateEvent(
			NULL,    // default security attribute 
			TRUE,    // manual-reset event 
			TRUE,    // initial state = signaled 
			NULL);   // unnamed event object 

		if (m_hEvents[i] == NULL) {
			msg.Format(L"CreateEvent failed with %d.\n", GetLastError());
			ShowText(msg);
			return 0;
		}

		m_Pipes[i].oOverlap.hEvent = m_hEvents[i];

		m_Pipes[i].hPipeInst = CreateNamedPipe(
			lpszPipename,            // pipe name 
			PIPE_ACCESS_DUPLEX |     // read/write access 
			FILE_FLAG_OVERLAPPED,    // overlapped mode 
			PIPE_TYPE_MESSAGE |      // message-type pipe 
			PIPE_READMODE_MESSAGE |  // message-read mode 
			PIPE_WAIT,               // blocking mode 
			INSTANCES,               // number of instances 
			BUFSIZE,   // output buffer size
			BUFSIZE,   // input buffer size 
			PIPE_TIMEOUT,            // client time-out 
			NULL);                   // default security attributes 

		if (m_Pipes[i].hPipeInst == INVALID_HANDLE_VALUE) {
			msg.Format(L"CreateNamedPipe failed with %d.\n", GetLastError());
			ShowText(msg);
			return 0;
		}
		// Call the subroutine to connect to the new client

		m_Pipes[i].fPendingIO = ConnectToNewClient(
			m_Pipes[i].hPipeInst,
			&m_Pipes[i].oOverlap);

		m_Pipes[i].dwState = m_Pipes[i].fPendingIO ?
			CONNECTING_STATE : // still connecting 
			READING_STATE;     // ready to read 
	}

	return 1;
}

UINT CIPCServer2Dlg::HandleEvents()
{
	DWORD i, dwWait, cbRet, dwErr;
	BOOL fSuccess;
	CString msg;

	while (1) {
		// Wait for the event object to be signaled, indicating 
		// completion of an overlapped read, write, or 
		// connect operation. 

		dwWait = WaitForMultipleObjects(
			INSTANCES,    // number of event objects 
			m_hEvents,      // array of event objects 
			FALSE,        // does not wait for all 
			INFINITE);    // waits indefinitely 

	  // dwWait shows which pipe completed the operation. 

		i = dwWait - WAIT_OBJECT_0;  // determines which pipe 
		if (i < 0 || i >(INSTANCES - 1)) {
			msg.Format(L"Index out of range.\n");
			ShowText(msg);
			return 0;
		}
		// Get the result if the operation was pending. 

		if (m_Pipes[i].fPendingIO) {
			fSuccess = GetOverlappedResult(
				m_Pipes[i].hPipeInst, // handle to pipe 
				&m_Pipes[i].oOverlap, // OVERLAPPED structure 
				&cbRet,            // bytes transferred 
				FALSE);            // do not wait 

			switch (m_Pipes[i].dwState) {
				// Pending connect operation 
				case CONNECTING_STATE:
					if (!fSuccess) {
						msg.Format(L"Error %d.\n", GetLastError());
						ShowText(msg);
						return 0;
					}
					m_Pipes[i].dwState = READING_STATE;
					ShowText(L"Connceting To Reading State\r\n");
					break;

				// Pending read operation 
				case READING_STATE:
					if (!fSuccess || cbRet == 0) {
						DisconnectAndReconnect(i);
						continue;
					}
					m_Pipes[i].cbRead = cbRet;
					////m_Pipes[i].dwState = WRITING_STATE;

					ShowText(L"Read Data Done\r\n");
					DumpBuffer(m_Pipes[i].u8ReadBuff, cbRet);

					////msg.Format(L"Reading To Writing State, cbRead=%d\r\n", cbRet);

					ShowText(msg);
					break;

				// Pending write operation 
				case WRITING_STATE:
					if (!fSuccess || cbRet != m_Pipes[i].cbToWrite) {
						DisconnectAndReconnect(i);
						continue;
					}
					ShowText(L"Writing To Reading State\r\n");
					m_Pipes[i].dwState = READING_STATE;
					break;

				default: {
					msg.Format(L"Invalid pipe state.\n");
					ShowText(msg);
					return 0;
				}
			}
		}

		// The pipe state determines which operation to do next. 

		switch (m_Pipes[i].dwState)
		{
			// READING_STATE: 
			// The pipe instance is connected to the client 
			// and is ready to read a request from the client. 

			case READING_STATE:
				ShowText(L"Read Data..\r\n");
				fSuccess = ReadFile(
					m_Pipes[i].hPipeInst,
					m_Pipes[i].u8ReadBuff,
					BUFSIZE,
					&m_Pipes[i].cbRead,
					&m_Pipes[i].oOverlap);

				// The read operation completed successfully. 

				if (fSuccess && (m_Pipes[i].cbRead != 0)) {
					m_Pipes[i].fPendingIO = FALSE;
					m_Pipes[i].dwState = WRITING_STATE;
					ShowText(L"Read Operation Completed Successfully\r\n");
					continue;
				}

				// The read operation is still pending. 
				ShowText(L"Read Operation Still Pending..\r\n");

				dwErr = GetLastError();
				if (!fSuccess && (dwErr == ERROR_IO_PENDING)) {
					m_Pipes[i].fPendingIO = TRUE;
					ShowText(L"Error IO Pending\r\n");
					continue;
				}

				// An error occurred; disconnect from the client. 

				DisconnectAndReconnect(i);
				break;

				// WRITING_STATE: 
				// The request was successfully read from the client. 
				// Get the reply data and write it to the client. 

			case WRITING_STATE:
				////GetAnswerToRequest(&m_Pipes[i]);
				ShowText(L"Write Data..\r\n");
				fSuccess = WriteFile(
					m_Pipes[i].hPipeInst,
					m_Pipes[i].u8Reply,
					m_Pipes[i].cbToWrite,
					&cbRet,
					&m_Pipes[i].oOverlap);

				// The write operation completed successfully. 

				if (fSuccess && cbRet == m_Pipes[i].cbToWrite) {
					m_Pipes[i].fPendingIO = FALSE;
					m_Pipes[i].dwState = READING_STATE;
					ShowText(L"Write Operation Completed Successfully\r\n");
					continue;
				}
				ShowText(L"Write Operation Still Pending..\r\n");
				// The write operation is still pending. 

				dwErr = GetLastError();
				if (!fSuccess && (dwErr == ERROR_IO_PENDING)) {
					m_Pipes[i].fPendingIO = TRUE;
					ShowText(L"Error IO Pending (w)\r\n");
					continue;
				}

				// An error occurred; disconnect from the client. 

				DisconnectAndReconnect(i);
				break;

			default: {
				msg.Format(L"Invalid pipe state.\n");
				ShowText(msg);
				return 0;
			}
		}
	}
}

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

VOID CIPCServer2Dlg::DisconnectAndReconnect(DWORD i)
{
	// Disconnect the pipe instance. 
	CString msg;

	if (!DisconnectNamedPipe(m_Pipes[i].hPipeInst)) {
		msg.Format(L"DisconnectNamedPipe failed with %d.\n", GetLastError());
		ShowText(msg);
	}

	// Call a subroutine to connect to the new client. 

	m_Pipes[i].fPendingIO = ConnectToNewClient(
		m_Pipes[i].hPipeInst,
		&m_Pipes[i].oOverlap);

	m_Pipes[i].dwState = m_Pipes[i].fPendingIO ?
		CONNECTING_STATE : // still connecting 
		READING_STATE;     // ready to read 
}

BOOL CIPCServer2Dlg::ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
	BOOL fConnected, fPendingIO = FALSE;

	// Start an overlapped connection for this pipe instance. 
	fConnected = ConnectNamedPipe(hPipe, lpo);

	// Overlapped ConnectNamedPipe should return zero. 
	if (fConnected)
	{
		printf("ConnectNamedPipe failed with %d.\n", GetLastError());
		return 0;
	}

	switch (GetLastError()) {
			// The overlapped connection in progress. 
		case ERROR_IO_PENDING:
			fPendingIO = TRUE;
			break;

			// Client is already connected, so signal an event. 

		case ERROR_PIPE_CONNECTED:
			if (SetEvent(lpo->hEvent))
				break;

			// If an error occurs during the connect operation... 
		default:
		{
			printf("ConnectNamedPipe failed with %d.\n", GetLastError());
			return 0;
		}
	}

	return fPendingIO;
}

VOID GetAnswerToRequest(LPPIPEINST pipe)
{
	////_tprintf(TEXT("[%d] %s\n"), pipe->hPipeInst, pipe->chRequest);
	////StringCchCopy(pipe->u8Reply, BUFSIZE, TEXT("Default answer from server"));
	////pipe->cbToWrite = (lstrlen(pipe->u8Reply) + 1) * sizeof(TCHAR);
}

UINT threadHandleEvenbts(LPVOID pVar)
{
	CIPCServer2Dlg* pDlg = (CIPCServer2Dlg*)pVar;
	return pDlg->HandleEvents();
}

UINT threadWaitForConnection(LPVOID pVar)
{	
	CIPCServer2Dlg* pDlg = (CIPCServer2Dlg*)pVar;
	if (!ConnectNamedPipe(pDlg->m_hPipe, NULL))             // 等待Client Connect
	{
		CloseHandle(pDlg->m_hPipe);                          // 失敗要關閉Handle
		::AfxMessageBox(_T("Connect Named Pipe fail."));
		return false;
	}
	pDlg->ShowText(L"Congraduation!\r\n");
	pDlg->ShowText(L"Connected.\r\n");

	unsigned char buffer[1024] = { 0 };
	DWORD ReadNum;

	while (1) {
		if (!ReadFile(pDlg->m_hPipe, buffer, 1024, &ReadNum, NULL))
		{
			CloseHandle(pDlg->m_hPipe);                          // 失敗要關閉Handle
			::AfxMessageBox(_T("Read Pipe fail."));
		}
		CString msg;
		msg.Format(L"Got Data: %02x %02x %02x %02x\r\n", buffer[0], buffer[1], buffer[2], buffer[3]);
		pDlg->ShowText(msg);
	}
	return true;
}

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

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

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

CIPCServer2Dlg::~CIPCServer2Dlg()
{
	if (m_Connected) {
		if (DisconnectNamedPipe(m_hPipe))                  // 終止Server連線
		{
			CloseHandle(m_hPipe);                          // 關閉Pipe
			m_hPipe = NULL;
		}
		else
			::AfxMessageBox(_T("Disconnect Named Pipe fail"));
	}
}

void __stdcall CIPCServer2Dlg::ShowText(LPCWSTR Text)
{
	m_Edit.SetSel(-1, -1);
	m_Edit.ReplaceSel(Text);
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CIPCServer2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIPCServer2Dlg::OnBnClickedOk()
{
	// TODO: 在此加入控制項告知處理常式程式碼
	
	////m_NPThread->Delete();
	m_NPThread = new CNamedPipeThread(m_hWnd, &m_NPServer);

	byte buffer[32];
	memset(buffer, 0x77, 32);
	m_NPThread->Function = 1;
	m_NPThread->Data = buffer;
	m_NPThread->Length = 32;
	m_NPThread->CreateThread();

	/*
	bool fSuccess;
	DWORD cbRet;

	byte buffer[32];

	memset(buffer, 0x77, 32);

	ShowText(L"Write Data..\r\n");
	fSuccess = WriteFile(
		m_Pipes[0].hPipeInst,
		buffer,
		32,
		&cbRet,
		&m_Pipes[0].oOverlap);

	// The write operation completed successfully. 

	if (fSuccess && cbRet == 32) {
		ShowText(L"Write Operation Completed Successfully\r\n");
	}
	else {
		ShowText(L"Write Data Failed\r\n");
	}*/
	
	
	////CDialogEx::OnOK();
}

UINT threadWriteData(LPVOID pVar)
{
	CIPCServer2Dlg* pDlg = (CIPCServer2Dlg*)pVar;
	pDlg->m_Connected = false;

	UINT result = pDlg->m_NPServer.Connect();

	if (result) {
		pDlg->ShowText(L"Connect Successfully\r\n");
		pDlg->m_Connected = true;
	}
	else {
		pDlg->ShowText(L"Connect Failed\r\n");
	}
	return result;
}