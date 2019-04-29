
// IPCServer2Dlg.h: 標頭檔
//
#include "NamedPipeServer.h"
#include "NamedPipeThread.h"

#pragma once

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2 
#define INSTANCES 4 
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096

typedef struct
{
	OVERLAPPED oOverlap;
	HANDLE hPipeInst;
	BYTE u8ReadBuff[BUFSIZE];
	DWORD cbRead;
	BYTE u8Reply[BUFSIZE];
	DWORD cbToWrite;
	DWORD dwState;
	BOOL fPendingIO;
} PIPEINST, * LPPIPEINST;

// CIPCServer2Dlg 對話方塊
class CIPCServer2Dlg : public CDialogEx
{
// 建構
public:
	CIPCServer2Dlg(CWnd* pParent = nullptr);	// 標準建構函式
	~CIPCServer2Dlg();

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IPCSERVER2_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void ShowText(LPCWSTR Text);
	UINT CreateNamedPipeConnection();
	BOOL ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo);
	VOID DisconnectAndReconnect(DWORD i);
	UINT HandleEvents();

	void DumpBuffer(BYTE* Buffer, UINT Size);

public:
	HANDLE	m_hPipe;
	bool	m_Connected;

	CEdit	m_Edit;

	CString m_Text;
	CWinThread*	m_pThread;
	CWinThread* m_ThreadConnectNP;

	PIPEINST m_Pipes[INSTANCES];
	HANDLE m_hEvents[INSTANCES];

	CNamedPipeServer	m_NPServer;
	CNamedPipeThread*	m_NPThread;

public:
	afx_msg void OnBnClickedOk();
};

UINT threadConnectNamedPipe(LPVOID pVar);

VOID GetAnswerToRequest(LPPIPEINST pipe);
UINT threadHandleEvenbts(LPVOID pVar);
UINT threadWaitForConnection(LPVOID pVar);