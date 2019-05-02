#pragma once
// CClientThread
//-----------------------------------------------------------------------------
enum ePresentType{WindowMinimize, WindowNormal, WindowHide};
//-----------------------------------------------------------------------------
class CClientThread : public CWinThread
{
	DECLARE_DYNCREATE(CClientThread)

public:
	CClientThread(HWND hWnd, CString ProgramPath, CString Parameters, BYTE PresentType);           // 動態建立所使用的保護建構函式

protected:
	CClientThread();
	
	virtual ~CClientThread();

	void WaitShellExecuteProcessDone(SHELLEXECUTEINFO& rSEI);

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	virtual int Run();

public:
	bool Terminated;

	SHELLEXECUTEINFO m_rSEI;

	HWND        m_hWnd;
	CString		m_ProgramPath;
	CString		m_Parameters;
	BYTE		m_PresentType;

protected:
	DECLARE_MESSAGE_MAP()
};
//-----------------------------------------------------------------------------

