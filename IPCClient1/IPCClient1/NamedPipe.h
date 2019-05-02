#pragma once

#include <string>
#include "Error.h"
using namespace std;
//---------------------------------------------------------------------------
enum eNamedPipeType{NamedPipeServer, NamedPipeClient};
//---------------------------------------------------------------------------
class CNamedPipe
{
public:
	CNamedPipe();
	~CNamedPipe();

	BOOL Create(const wstring& Name, UINT BufferSize, BYTE Type);

	BOOL Connect();
	BOOL WriteData(BYTE* Buffer, DWORD Length);
	BOOL ReadData(BYTE* Buffer, DWORD Length, DWORD* BytesRead);

	BOOL IsConnected();
	BOOL DisConnect();
	BOOL Reconnect();

protected:
	BOOL ConnectToNewClient(HANDLE Pipe, LPOVERLAPPED LPOV);
	BOOL WaitEventAndGetResult(DWORD* Ret1);

protected:
	UINT			m_BuffSize;
	wstring			m_Name;
	HANDLE			m_hEvent;
	OVERLAPPED		m_Overlapped;
	HANDLE			m_hPipe;
	CError			m_Error;

	BOOL			m_Connected;

	BYTE			m_Type;


	static const int PIPE_TIMEOUT = 5000;
};
//---------------------------------------------------------------------------
