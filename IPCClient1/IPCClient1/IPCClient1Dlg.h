
// IPCClient1Dlg.h: 標頭檔
//

#pragma once


// CIPCClient1Dlg 對話方塊
class CIPCClient1Dlg : public CDialogEx
{
// 建構
public:
	CIPCClient1Dlg(CWnd* pParent = nullptr);	// 標準建構函式

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_IPCCLIENT1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	HANDLE m_hPipe;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void ShowText(LPCWSTR Text);

	UINT WriteDataToPipe();
	UINT HandleEvents();
	
	void DumpBuffer(BYTE* Buffer, UINT Size);

	static const int BUFSIZE = 512;

	CEdit	m_Edit;
	HANDLE  m_hEvent;

	CWinThread* m_pThread;
	

public:
	UINT ConnectToNamedPipe();
	UINT ListenDataIn();

public:
	afx_msg void OnBnClickedOk();
};

UINT threadHandleDataIn(LPVOID pVar);