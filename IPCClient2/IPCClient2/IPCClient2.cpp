// IPCClient2.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//
//---------------------------------------------------------------------------
#include "pch.h"
#include <iostream>
#include <string>
#include <Windows.h>
#include "Bulk.h"
#include "RecognitionConfig.h"
#include "ipc_def.h"
using namespace std;
//---------------------------------------------------------------------------
static const int BUFSIZE = 65536;
HANDLE g_hPipe;
//---------------------------------------------------------------------------
BOOL ConnectToNamedPipe(const wstring& NPName);
void ListenDataIn();
void DumpPacket(BYTE* Data, UINT Length);
void DumpBuffer(BYTE* Buffer, UINT Size);
wstring GetOpCodeDescription(UINT OPCode);
BOOL WriteAck();
wstring StrToWStr(const string& Source);
//---------------------------------------------------------------------------
int main(int argc, const char** argv)
{
	wstring npName;

	if (argc < 2) {
		npName = L"000";
	}
	else {
		npName = StrToWStr(argv[1]);
	}
	if (ConnectToNamedPipe(L"\\\\.\\pipe\\vns" + npName)) {
		printf("Connect named pipe successfully.\r\n");
		ListenDataIn();
	}
	system("PAUSE");

	return 0;
}
//---------------------------------------------------------------------------
BOOL ConnectToNamedPipe(const wstring& NPName)
{
	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;

	wstring msg;

	// Try to open a named pipe; wait for it, if necessary. 

	g_hPipe = CreateFile(
		NPName.c_str(),   // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE,
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,  // opens existing pipe 
		FILE_FLAG_OVERLAPPED,              // default attributes 
		NULL);          // no template file 

	// Break if the pipe handle is valid. 
	if (g_hPipe != INVALID_HANDLE_VALUE)
		return TRUE;

	// Exit if an error other than ERROR_PIPE_BUSY occurs. 

	if (GetLastError() != ERROR_PIPE_BUSY) {
		wprintf(L"Could not open pipe. LE = %d\n", GetLastError());
		return FALSE;
	}

	// All pipe instances are busy, so wait for 20 seconds. 
	if (!WaitNamedPipe(NPName.c_str(), 20000)) {
		wprintf(L"Could not open pipe. LE = %d\n", GetLastError());
		return FALSE;
	}
	return TRUE;	// pipe not busy
}
//---------------------------------------------------------------------------
void ListenDataIn()
{
	DWORD i, dwWait, cbRead, dwErr;
	BOOL fSuccess;

	byte buffer[BUFSIZE];

	while (1) {
		fSuccess = ReadFile(
			g_hPipe,    // pipe handle 
			buffer,    // buffer to receive reply 
			BUFSIZE,  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);

		if (cbRead > 0) {
			wprintf(L"Got %d bytes data\r\n", cbRead);
			////DumpBuffer(buffer, cbRead);
			DumpPacket(buffer, cbRead);

			if (!WriteAck()) {
				break;
			}
		}
	}
}
//---------------------------------------------------------------------------
void DumpPacket(BYTE* Data, UINT Length)
{
	THeader* pHeader = (THeader*)Data;
	
	int totalSize = sizeof(THeader) + pHeader->AttachmentSize;

	if (totalSize > Length) {
		printf("Error Input Data Length\r\n");
		return;
	}
	wprintf(L"OPCode     :  %d (%s)\r\n", pHeader->Opcode, GetOpCodeDescription(pHeader->Opcode).c_str());
	wprintf(L"AttachSize :  %d\r\n", pHeader->AttachmentSize);
	wprintf(L"P0         :  %d\r\n", pHeader->P0);
	wprintf(L"P1         :  %d\r\n", pHeader->P0);

	if (pHeader->Opcode == OP_SEND_RECOGNITION_CONFIG) {
		CRecognitionConfig config;
		config.Config = *(TRecognitionConfig*)(Data + sizeof(THeader));
		wprintf(L"\r\n");
		wprintf(L"---- Recognition Config ----\r\n");
		wprintf(config.ToString(1).c_str());
	}
}
//---------------------------------------------------------------------------
wstring GetOpCodeDescription(UINT OPCode)
{
	if (OP_ACK == OPCode)
		return L"OP_ACK";
	if (OP_SEND_RECOGNITION_CONFIG == OPCode)
		return L"OP_SEND_RECOGNITION_CONFIG";
	if (OP_WRITE_BMP == OPCode)
		return L"OP_WRITE_BMP";
	if (OP_WRITE_BMP == OPCode)
		return L"OP_WRITE_BMP";
	if (OP_READ_BMP == OPCode)
		return L"OP_READ_BMP";
	if (OP_RECOGNIZE_START == OPCode)
		return L"OP_RECOGNIZE_START";
	if (OP_RECOGNIZE_RESULT == OPCode)
		return L"OP_RECOGNIZE_RESULT";

	return L"Unknown";
}
//---------------------------------------------------------------------------
void DumpRecognitionConfig(TRecognitionConfig* Config)
{

}
//---------------------------------------------------------------------------
BOOL WriteAck()
{
	BOOL fSuccess;
	DWORD cbWritten;
	char ack[] = "ack";

	fSuccess = WriteFile(
		g_hPipe,                  // pipe handle 
		ack,             // message 
		strlen(ack),              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess) {
		wprintf(L"WriteFile to pipe failed. LE=%d\n", GetLastError());
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------------------------
void DumpBuffer(BYTE* Buffer, UINT Size)
{
	wstring temp;
	wstring text;

	wprintf(L"Buffer Data (%d bytes)\r\n", Size);

	for (int i = 0; i < Size; ++i) {
		if (i != 0 && (i % 16 == 0)) {
			wprintf(L"\r\n");
		}
		wprintf(L"%02x ", Buffer[i]);
	}
	wprintf(L"\r\n");
}
//---------------------------------------------------------------------------
wstring StrToWStr(const string& Source)
{
#ifndef _WIN32
	return c2w(Source.c_str());
#else
	const UINT     size = Source.size() + 1;
	Bulk<wchar_t>  buffer(size);

	UINT pos = MultiByteToWideChar(CP_ACP,
		0,
		Source.c_str(),
		Source.size(),
		&buffer[0],
		size);
	buffer[pos] = 0;

	return wstring(&buffer[0]);
#endif
}
//---------------------------------------------------------------------------