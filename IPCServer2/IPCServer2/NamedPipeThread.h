#pragma once

#define WM_NPCONNECT	WM_USER + 0X0001

#include "NamedPipeServer.h"
#include "UsrMsg.h"

// CNamedPipeThread

typedef void (__stdcall *fpLog)(LPCWSTR Text);

class CNamedPipeThread : public CWinThread
{
	DECLARE_DYNCREATE(CNamedPipeThread)

public:
	CNamedPipeThread();
	CNamedPipeThread(HWND hWnd, CNamedPipeServer* Server);           // 動態建立所使用的保護建構函式
	virtual ~CNamedPipeThread();

	virtual int Run();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void ShowLog(LPCWSTR Text);

	int Function;
	byte* Data;
	int Length;

	int Result;

	fpLog LogCallBack;

protected:
	CNamedPipeServer* m_NPServer;
	HWND	m_hWnd;

protected:
	DECLARE_MESSAGE_MAP()
};


