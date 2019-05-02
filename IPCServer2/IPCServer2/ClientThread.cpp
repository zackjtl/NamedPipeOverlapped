// ClientThread.cpp: 實作檔案
//
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include "IPCServer2.h"
#include "ClientThread.h"
//-----------------------------------------------------------------------------
// CClientThread
IMPLEMENT_DYNCREATE(CClientThread, CWinThread)
//-----------------------------------------------------------------------------
CClientThread::CClientThread()
{
}
//-----------------------------------------------------------------------------
CClientThread::CClientThread(HWND hWnd, CString ProgramPath, CString Parameters, BYTE PresentType)
	:	Terminated(false),
		m_hWnd(hWnd),
		m_ProgramPath(ProgramPath),
		m_Parameters(Parameters),
		m_PresentType(PresentType)
{
}
//-----------------------------------------------------------------------------
CClientThread::~CClientThread()
{
}
//-----------------------------------------------------------------------------
BOOL CClientThread::InitInstance()
{
	// TODO:     在此執行任何個別執行緒的初始設定
	return TRUE;
}
//-----------------------------------------------------------------------------
int CClientThread::ExitInstance()
{
	// TODO:     在此執行任何個別執行緒的初始設定
	return CWinThread::ExitInstance();
}
//-----------------------------------------------------------------------------
int CClientThread::Run()
{
	memset((void*)& m_rSEI, 0, sizeof(SHELLEXECUTEINFO));
	m_rSEI.cbSize = sizeof(m_rSEI);
	m_rSEI.lpVerb = L"open";
	m_rSEI.lpFile = m_ProgramPath;		// Program name

	//rSEI.lpParameters = (tempFilePath + L" 0 D 0.5 5 1.0 1 0x12345678").c_str();
	m_rSEI.lpParameters = m_Parameters;

	if (m_PresentType == WindowMinimize) {
		m_rSEI.nShow = SW_MINIMIZE;
	}
	else if (m_PresentType == WindowHide) {
		m_rSEI.nShow = SW_HIDE;
	}
	else {
		m_rSEI.nShow = SW_NORMAL;
	}
	m_rSEI.fMask = SEE_MASK_NOCLOSEPROCESS;

	ShellExecuteEx(&m_rSEI);   // you should check for an error here

	WaitShellExecuteProcessDone(m_rSEI);

	DWORD dwCode;
	GetExitCodeProcess(m_rSEI.hProcess, &dwCode);  // ERRORLEVEL value\

	return TRUE;
}
//---------------------------------------------------------------------------
void CClientThread::WaitShellExecuteProcessDone(SHELLEXECUTEINFO& rSEI)
{
	while (TRUE) {
		DWORD nStatus = MsgWaitForMultipleObjects(
			1, &rSEI.hProcess, FALSE,
			INFINITE, QS_ALLINPUT   // drop through on user activity
		);
		if (nStatus == WAIT_OBJECT_0) {  // done: the program has ended
			break;
		}
		MSG msg;     // else process some messages while waiting...
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) && (!Terminated)) {
			DispatchMessage(&msg);
		}
		if (Terminated)
			break;
	}  // launched process has exited
}
//-----------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CClientThread, CWinThread)
END_MESSAGE_MAP()
//-----------------------------------------------------------------------------
// CClientThread 訊息處理常式
