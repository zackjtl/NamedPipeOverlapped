#pragma once

#include <string>
#include "Error.h"
using namespace std;
//---------------------------------------------------------------------------
class CNamedPipeServer
{
public:
	CNamedPipeServer();
	~CNamedPipeServer();

	BOOL Create(const wstring& Name, UINT BufferSize);

	BOOL Connect();
	BOOL WriteData(BYTE* Buffer, UINT Length);
	BOOL ReadData(BYTE* Buffer, UINT Length);

protected:
	BOOL ConnectToNewClient(HANDLE Pipe, LPOVERLAPPED LPOV);
	BOOL WaitEventAndGetResult(DWORD* Ret1);
	BOOL Reconnect();
protected:
	UINT			m_BuffSize;
	wstring			m_Name;
	HANDLE			m_hEvent;
	OVERLAPPED		m_Overlapped;
	HANDLE			m_hPipe;
	CError			m_Error;

	BOOL			m_Connected;


	static const int PIPE_TIMEOUT = 5000;
};
//---------------------------------------------------------------------------
