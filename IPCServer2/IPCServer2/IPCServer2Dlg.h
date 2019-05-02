
// IPCServer2Dlg.h: 標頭檔
//
#include "NamedPipeServer.h"
#include "NamedPipeThread.h"
#include "ClientThread.h"
#include "ListCtrlEx.h"

#pragma once

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2 
#define INSTANCES 4 
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096

#define IDT_TIMER1 1003 

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
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();	
	afx_msg LRESULT OnThreadDone(WPARAM wParam, LPARAM lParam);
	
	void OnNMClickList(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnNotifyDescriptionEdited(WPARAM wParam, LPARAM lParam);


	DECLARE_MESSAGE_MAP()

public:
	void __stdcall ShowText(LPCWSTR Text);

	void DumpBuffer(BYTE* Buffer, UINT Size);

protected:
	void InitialListControl();
	void InitialGUIConfig();
	void InitCameraParamList(PVOID IniFile);
	void SaveGUIConfig();

	CPoint InterviewListCursorPosition() const;

public:
	HANDLE	m_hPipe;
	bool	m_Connected;

	CEdit	m_LogEdit;
	CEdit   m_ClientEdit;
	CEdit   m_NPNameEdit;

	CEdit   m_StationEdit;
	CEdit   m_CamIPEdit[4];
	CString m_Text;

	CListCtrlEx m_ListCtrl;

	PIPEINST m_Pipes[INSTANCES];
	HANDLE m_hEvents[INSTANCES];

	CNamedPipeServer	m_NPServer;
	CNamedPipeThread*	m_NPThread;
	CClientThread*		m_ClientThread;

public:
	afx_msg void OnBnClickedBtnbrowse();
	afx_msg void OnBnClickedBtnopenclient();
	afx_msg void OnBnClickedBtnsendconfig();
};