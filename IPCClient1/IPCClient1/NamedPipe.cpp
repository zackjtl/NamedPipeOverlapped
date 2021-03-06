#include "stdafx.h"
#include "NamedPipe.h"
#include "Error.h"
#include "TypeConv.h"
//---------------------------------------------------------------------------
CNamedPipe::CNamedPipe()
{
	m_Connected = false;
}
//---------------------------------------------------------------------------
CNamedPipe::~CNamedPipe()
{
}
//---------------------------------------------------------------------------
BOOL CNamedPipe::IsConnected()
{
	return m_Connected;
}
//---------------------------------------------------------------------------
BOOL CNamedPipe::Create(const wstring& Name, UINT BufferSize, BYTE Type)
{
	m_Name = Name;
	m_BuffSize = BufferSize;
	m_Connected = false;
	m_Type = Type;

	m_hEvent = CreateEvent(
		NULL,    // default security attribute 
		TRUE,    // manual-reset event 
		TRUE,    // initial state = signaled 
		NULL);   // unnamed event object 

	if (m_hEvent == NULL) {
		m_Error.SetText(L"CreateEvent failed with error 0x" + IntToHexWStr(GetLastError(), 8));
		m_Error.SetCode(GetLastError());
		return FALSE;
	}
	m_Overlapped.hEvent = m_hEvent;

	m_hPipe = CreateNamedPipe(
		Name.c_str(),            // pipe name 
		PIPE_ACCESS_DUPLEX |     // read/write access 
		FILE_FLAG_OVERLAPPED,    // overlapped mode 
		PIPE_TYPE_MESSAGE |      // message-type pipe 
		PIPE_READMODE_MESSAGE |  // message-read mode 
		PIPE_WAIT,               // blocking mode 
		1,						// number of instances 
		BufferSize,				// output buffer size
		BufferSize,				// input buffer size 
		PIPE_TIMEOUT,            // client time-out 
		NULL);                   // default security attributes 

	if (m_hPipe == INVALID_HANDLE_VALUE) {
		m_Error.SetText(L"CreateNamedPipe failed with error 0x" + IntToHexWStr(GetLastError(), 8));
		m_Error.SetCode(GetLastError());
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL CNamedPipe::Connect()
{
	DWORD cbRet;
	// Call the subroutine to connect to the new client
	if (!ConnectToNewClient(m_hPipe, &m_Overlapped)) {
		return FALSE;
	}
	if (!WaitEventAndGetResult(&cbRet)) {
		return FALSE;
	}
	m_Connected = TRUE;
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL CNamedPipe::ConnectToNewClient(HANDLE Pipe, LPOVERLAPPED LPOV)	
{
	BOOL connected;
	BOOL ioPending = FALSE;
	
	// Start an overlapped connection for this pipe instance. 
	connected = ConnectNamedPipe(Pipe, LPOV);

	// Overlapped ConnectNamedPipe should return zero. 
	if (connected != 0) {
		m_Error.SetText(L"ConnectNamedPipe failed with error 0x" + IntToHexWStr(GetLastError(), 8));
		m_Error.SetCode(GetLastError());
		return FALSE;
	}
	switch (GetLastError()) {
		// The overlapped connection in progress. 
		case ERROR_IO_PENDING:
			ioPending = TRUE;
			break;

		// Client is already connected, so signal an event. 
		case ERROR_PIPE_CONNECTED:
			if (SetEvent(LPOV->hEvent))
				break;

		// If an error occurs during the connect operation... 
		default:
			m_Error.SetText(L"ConnectNamedPipe failed with error 0x" + IntToHexWStr(GetLastError(), 8));
			m_Error.SetCode(GetLastError());
			return 0;
	
	}
	return ioPending;
}
//---------------------------------------------------------------------------
BOOL CNamedPipe::WaitEventAndGetResult(DWORD* Ret)
{
	DWORD dwWait;
	BOOL success;

	dwWait = WaitForSingleObject(m_hEvent, INFINITE);

	if (dwWait == WAIT_FAILED) {
		m_Error.SetText(L"WaitForSingleObject failed with error 0x" + IntToHexWStr(GetLastError(), 8));
		m_Error.SetCode(GetLastError());
		return FALSE;
	}
	success = GetOverlappedResult(
		m_hPipe, // handle to pipe 
		&m_Overlapped, // OVERLAPPED structure 
		Ret,            // bytes transferred 
		FALSE);            // do not wait 

	return success;
}
//---------------------------------------------------------------------------
BOOL CNamedPipe::DisConnect()
{
	return DisconnectNamedPipe(m_hPipe);
}
//---------------------------------------------------------------------------
BOOL CNamedPipe::Reconnect()
{
	m_Connected = FALSE;
	if (!DisConnect()) {
		m_Error.SetText(L"Error DisconnectNamedPipe with error 0x" + IntToHexWStr(GetLastError(), 8));
		m_Error.SetCode(GetLastError());
	}

	if (!ConnectToNewClient(m_hPipe, &m_Overlapped)) {
		return FALSE;
	}
	DWORD ret;

	if (!WaitEventAndGetResult(&ret)) {
		return FALSE;
	}
	m_Connected = TRUE;
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL CNamedPipe::WriteData(BYTE* Buffer, DWORD Length)
{
	////ShowText(L"Write Data..\r\n");
	BOOL success;
	DWORD cbRet, dwErr;

	success = WriteFile(
	m_hPipe,
	Buffer,
	Length,
	&cbRet,
	&m_Overlapped);

	if (success && (Length == cbRet)) {
		return TRUE;
	}
	
	dwErr = GetLastError();
	if (!success && (dwErr == ERROR_IO_PENDING)) {
		success = WaitEventAndGetResult(&cbRet);

		if (!success || (cbRet != Length)) {
			Reconnect();
			return  FALSE;
		}
	}
	return TRUE;
}
//---------------------------------------------------------------------------
BOOL CNamedPipe::ReadData(BYTE* Buffer, DWORD Length, DWORD* BytesRead)
{
	BOOL success;
	DWORD dwErr;

	success = ReadFile(
		m_hPipe,
		Buffer,
		Length,
		BytesRead,
		&m_Overlapped);

	if (success && (BytesRead != 0)) {
		return TRUE;
	}

	dwErr = GetLastError();
	if (!success && (dwErr == ERROR_IO_PENDING)) {
		success = WaitEventAndGetResult(BytesRead);

		if (!success || (BytesRead == 0)) {
			Reconnect();
			return  FALSE;
		}
	}
	return TRUE;
}
//---------------------------------------------------------------------------