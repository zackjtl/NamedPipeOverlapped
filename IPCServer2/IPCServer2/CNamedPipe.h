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

protected:
	BOOL ConnectToNewClient(HANDLE Pipe, LPOVERLAPPED LPOV);
	BOOL WaitEventAndGetResult();
	BOOL Reconnect();
protected:
	UINT			m_BuffSize;
	wstring			m_Name;
	HANDLE			m_hEvent;
	OVERLAPPED		m_Overlapped;
	HANDLE			m_hPipe;
	CError			m_Error;

	static const int PIPE_TIMEOUT = 5000;
};
//---------------------------------------------------------------------------
