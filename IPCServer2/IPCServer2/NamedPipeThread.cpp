// CNamedPipeThread.cpp: 實作檔案
//
#include "stdafx.h"
#include "IPCServer2.h"
#include "NamedPipeThread.h"


// CNamedPipeThread

IMPLEMENT_DYNCREATE(CNamedPipeThread, CWinThread)

CNamedPipeThread::CNamedPipeThread()
{

}

CNamedPipeThread::CNamedPipeThread(CNamedPipeServer* Server)
	: m_NPServer(Server)
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
		if (m_NPServer->Connect()) {

		}
	}
	else if (Function == 1) {
		m_NPServer->WriteData(Data, Length);
	}
	return 0;
}

int CNamedPipeThread::ExitInstance()
{
	// TODO:     在此執行任何個別執行緒的初始設定
	return CWinThread::ExitInstance();
}
BEGIN_MESSAGE_MAP(CNamedPipeThread, CWinThread)
END_MESSAGE_MAP()


// CNamedPipeThread 訊息處理常式
