// CNamedPipeThread.cpp: 實作檔案
//
#include "stdafx.h"
#include "IPCServer2.h"
#include "NamedPipeThread.h"
// CNamedPipeThread

IMPLEMENT_DYNCREATE(CNamedPipeThread, CWinThread)

CNamedPipeThread::CNamedPipeThread()
	: LogCallBack(NULL), m_hWnd(NULL)
{

}

CNamedPipeThread::CNamedPipeThread(HWND hWND, CNamedPipeServer* Server)
	:	m_hWnd(hWND), m_NPServer(Server), LogCallBack(NULL)
{
}

CNamedPipeThread::~CNamedPipeThread()
{
}

BOOL CNamedPipeThread::InitInstance()
{
	// TODO:     在此執行任何個別執行緒的初始設定
	return TRUE;
}

int CNamedPipeThread::Run()
{
	if (Function == 0) {
		Result = m_NPServer->Connect();
	}
	else if (Function == 1) {
		Result = m_NPServer->WriteData(Data, Length);	
	}
	else {
		Result = 0;
	}

	PostMessage(m_hWnd, WM_THREAD_DONE, Function, Result);

	return 1;
}

void CNamedPipeThread::ShowLog(LPCWSTR Text)
{
	if (LogCallBack != NULL) {
		LogCallBack(Text);
	}
}

int CNamedPipeThread::ExitInstance()
{
	// TODO:     在此執行任何個別執行緒的初始設定
	return CWinThread::ExitInstance();
}
BEGIN_MESSAGE_MAP(CNamedPipeThread, CWinThread)
END_MESSAGE_MAP()


// CNamedPipeThread 訊息處理常式
