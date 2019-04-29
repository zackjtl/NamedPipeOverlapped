#pragma once

#define WM_NPCONNECT	WM_USER + 0X0001

#include "NamedPipeServer.h"

// CNamedPipeThread

class CNamedPipeThread : public CWinThread
{
	DECLARE_DYNCREATE(CNamedPipeThread)

public:
	CNamedPipeThread();
	CNamedPipeThread(CNamedPipeServer* Server);           // 動態建立所使用的保護建構函式
	virtual ~CNamedPipeThread();

	virtual int Run();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	int Function;
	byte* Data;
	int Length;

protected:
	CNamedPipeServer* m_NPServer;

protected:
	DECLARE_MESSAGE_MAP()
};


