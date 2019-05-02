#include "stdafx.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include "IoFile.h"
#include "TypeConv.h"
#include "FileError.h"
#include "randomc.h"

#ifdef _WIN32
#include <windows.h>
#endif
//---------------------------------------------------------------------------
CIoFile::CIoFile()
  : _FileHandle(INVALID_HANDLE_VALUE)
{
}
//---------------------------------------------------------------------------
CIoFile::CIoFile(const string&  FileName,
                 UINT           DesiredAccess,
                 UINT           CreationDisposition,
                 UINT           ShareMode)
{
  Open(FileName, DesiredAccess, CreationDisposition, ShareMode);
}
//---------------------------------------------------------------------------
CIoFile::CIoFile(const wstring&  FileName,
                 UINT            DesiredAccess,
                 UINT            CreationDisposition,
                 UINT            ShareMode)
{
  Open(FileName, DesiredAccess, CreationDisposition, ShareMode);
}
//---------------------------------------------------------------------------
CIoFile::~CIoFile()
{
  Close();
}
//---------------------------------------------------------------------------
CIoFile& CIoFile::operator<<(const wstring& Value)
{
  WriteString(Value);
  return *this;
}
//---------------------------------------------------------------------------
CIoFile& CIoFile::operator<<(wchar_t Value)
{
  Write((byte*) &Value, sizeof(Value));
  return *this;
}
//---------------------------------------------------------------------------
CIoFile& CIoFile::operator<<(int Value)
{
  WriteString(IntToWStr(Value));
  return *this;
}
//---------------------------------------------------------------------------
CIoFile& CIoFile::operator<<(unsigned int Value)
{
  WriteString(IntToWStr(Value));
  return *this;
}
//---------------------------------------------------------------------------
CIoFile& CIoFile::operator<<(long Value)
{
  WriteString(IntToWStr(Value));
  return *this;
}
//---------------------------------------------------------------------------
CIoFile& CIoFile::operator<<(unsigned long Value)
{
  WriteString(IntToWStr(Value));
  return *this;
}
//---------------------------------------------------------------------------
void CIoFile::Open(const string&  FileName,
                   UINT           DesiredAccess,
                   UINT           CreationDisposition,
                   UINT           ShareMode)
{
  _FileName = StrToWStr(FileName);

#ifndef _WIN32
    _FileHandle = open( FileName.c_str(), DesiredAccess | CreationDisposition, ShareMode);
    /*
    if (_FileHandle == INVALID_HANDLE_VALUE) {
      if (DesiredAccess & O_CREAT) {
        remove(FileName.c_str());
        _FileHandle = open( FileName.c_str(), O_CREAT | O_RDWR, ShareMode);
      }
    }*/
#else
  _FileHandle = CreateFileA(
                  FileName.c_str(),
                  DesiredAccess,
                  ShareMode,
                  NULL,
                  CreationDisposition,
                  FILE_ATTRIBUTE_NORMAL,
									NULL);
#endif

	if (_FileHandle == INVALID_HANDLE_VALUE) {
		throw COpenFileError(_FileName);
	}
}
//---------------------------------------------------------------------------
void CIoFile::Open(const wstring&  FileName,
                   UINT            DesiredAccess,
                   UINT            CreationDisposition,
                   UINT            ShareMode)
{
	_FileName = FileName;
#ifndef _WIN32
    std::string fn = w2c( FileName.c_str() );
    _FileHandle = open( fn.c_str(), DesiredAccess | CreationDisposition, ShareMode);
    /*
    if (_FileHandle == INVALID_HANDLE_VALUE) {
      if (DesiredAccess & O_CREAT) {
        remove(fn.c_str());
        _FileHandle = open( fn.c_str(), O_CREAT | O_RDWR, ShareMode);
      }
    }*/
#else
  _FileHandle = CreateFileW(
                  FileName.c_str(),
                  DesiredAccess,
                  ShareMode,
                  NULL,
                  CreationDisposition,
                  FILE_ATTRIBUTE_NORMAL,
                  NULL);
#endif

	if (_FileHandle == INVALID_HANDLE_VALUE) {
    throw COpenFileError(_FileName);
	}
}
//---------------------------------------------------------------------------
void CIoFile::Close()
{
  if (INVALID_HANDLE_VALUE == _FileHandle) {
    return;
  }
#ifndef _WIN32
    close( _FileHandle );
#else
  CloseHandle(_FileHandle);
#endif
  _FileHandle = INVALID_HANDLE_VALUE;
}
//---------------------------------------------------------------------------
void CIoFile::Write(byte* InBuf, UINT Count)
{
  DWORD      bytesWrite;

#ifndef _WIN32
    int status = ( write( _FileHandle, InBuf, Count ) == -1 ? false : true );
    bytesWrite = Count;
#else
  bool status = ::WriteFile(_FileHandle, InBuf, Count, &bytesWrite, NULL);
#endif
  if (status) {
    if (bytesWrite != Count) {
      status = false;
    }
  }
  if (!status) {
    throw CError(L"[Error] : Cannot write file, " + _FileName + L".");
  }
}
//---------------------------------------------------------------------------
void CIoFile::WriteByte(byte Value)
{
  Write(&Value, sizeof(Value));
}
//---------------------------------------------------------------------------
void CIoFile::WriteWord(WORD Value)
{
  Write((byte*) &Value, sizeof(Value));
}
//---------------------------------------------------------------------------
void CIoFile::WriteDWord(DWORD Value)
{
  Write((byte*) &Value, sizeof(Value));
}
//---------------------------------------------------------------------------
void CIoFile::Read(byte* OutBuf, UINT Count)
{
  DWORD      bytesRead;
    
#ifndef _WIN32
    bool status = ( read( _FileHandle, OutBuf, Count ) == -1 ? false : true );
    bytesRead = Count;
#else
  bool status = ::ReadFile(_FileHandle, OutBuf, Count, &bytesRead, NULL);
#endif
  if (status) {
    if (bytesRead != Count) {
      status = false;
    }
  }
  if (!status) {
    throw CError(L"[Error] : Cannot read file, " + _FileName + L".");
  }
}
//---------------------------------------------------------------------------
byte CIoFile::ReadByte()
{
  byte  value;

  Read(&value, sizeof(value));

  return value;
}
//---------------------------------------------------------------------------
WORD CIoFile::ReadWord()
{
  WORD  value;

  Read(reinterpret_cast<byte*>(&value), sizeof(value));

  return value;
}
//---------------------------------------------------------------------------
DWORD CIoFile::ReadDWord()
{
  DWORD  value;

  Read(reinterpret_cast<byte*>(&value), sizeof(value));

  return value;
}
//---------------------------------------------------------------------------
void CIoFile::Flush()
{
#ifndef _WIN32
    fsync (_FileHandle );
#else
  if (!FlushFileBuffers(_FileHandle)) {
    throw CError(L"[Error] : Cannot flush file, " + _FileName + L".");
  }
#endif
}
//---------------------------------------------------------------------------
UINT64 CIoFile::GetSize()
{
  UINT64  size;
#ifndef _WIN32
    std::string fn = w2c( _FileName.c_str() );
    struct stat st;
    stat( fn.c_str(), &st);
    size = st.st_size;
#else
    
  if (!GetFileSizeEx(_FileHandle, (PLARGE_INTEGER)&size)) {
    throw CError(L"[Error] : Cannot get file size, " + _FileName + L".");
  }
#endif
  return size;
}
//---------------------------------------------------------------------------
void CIoFile::Seek(UINT64 Position, int From)
{
#ifndef _WIN32
    lseek( _FileHandle, Position, From );
#else
  if (!SetFilePointerEx(_FileHandle, *((PLARGE_INTEGER) &Position), NULL, From)) {
    throw CError(L"[Error] : Cannot set file pointer, " + _FileName + L".");
  }
#endif
}
//---------------------------------------------------------------------------
void CIoFile::WriteByteOrderMarkOfLittleEndian()
{
  byte  bom[2] = { 0xff, 0xfe };

  Write(bom, sizeof(bom));
}
//---------------------------------------------------------------------------
void CIoFile::WriteString(const string& Value)
{
  Write((byte*) Value.c_str(), Value.size());
}
//---------------------------------------------------------------------------
void CIoFile::WriteString(const wstring& Value)
{
#ifndef _WIN32
  /* Convert from four-byte to two-byte */

  int stringSize = Value.size();
  int bufferSize = stringSize * 2;

  byte* buffer = new byte[bufferSize];

  byte* ptr = &buffer[0];
  byte* pStr = (byte*)&Value[0];

  for (int pos = 0; pos < stringSize; ++pos) {
    memcpy(ptr, pStr, 2);

    pStr += 4;
    ptr += 2;
  }

  try {
    Write(buffer, bufferSize);
  }
  catch (CError& Error) {
    delete[] buffer;
    throw Error;
  }
  delete[] buffer;
#else
  Write((byte*) Value.c_str(), Value.size() * sizeof(wchar_t));
#endif
}
//---------------------------------------------------------------------------
void CIoFile::ReadString(string& Value)
{
  UINT64  fileSize = GetSize();

#ifndef _WIN32
  if (fileSize) {
		Seek(0, FILE_BEGIN);
		Read((byte*) &Value[0], fileSize);
  }
  else {
    Value.clear();
  }
#else
  if (fileSize) {
    Value.resize(fileSize);

    Seek(0, FILE_BEGIN);

    Read((byte*) &Value[0], fileSize);
  }
  else {
    Value.clear();
	}
#endif
}
//---------------------------------------------------------------------------
void CIoFile::ReadString(wstring& Value)
{
  UINT64  fileSize = GetSize();
#ifndef _WIN32
  byte* buffer;

  if (IsUTF16()) {
    /* Only for UTF16 Format */
    int strLen = (fileSize / 2) - 1;

    buffer = new byte[fileSize];

    Value.resize(strLen);

    Seek(2, FILE_BEGIN);

    Read(buffer, fileSize - 2);

    byte* ptr = buffer;
    byte* pStr = (byte*)&Value[0];

    for (int pos = 0; pos < (strLen * 4); pos += 4) {
      memcpy((byte*)&pStr[pos], ptr, 2);
      memset((byte*)&pStr[pos+2], 0, 2);
      ptr += 2;
    }
    delete[] buffer;
  }
  else {
    Value.clear();
  }
#else
  if (fileSize >= 4) {
    Value.resize(fileSize / 2 - 1);

    Seek(2, FILE_BEGIN);

    Read((byte*) &Value[0], fileSize - 2);
  }
  else {
    Value.clear();
  }
#endif
}
//---------------------------------------------------------------------------
bool CIoFile::IsUTF16()
{
  if (GetSize() < 2) {
    return false;
  }
  Seek(0, FILE_BEGIN);

  byte bom[2];

  Read(bom, 2);

  if ((bom[0] == 0xff) && (bom[1] == 0xfe)) {
    return true;
  }
  else {
     return false;
  }
}
//---------------------------------------------------------------------------
bool CIoFile::IsValidFileHandle()
{
  return _FileHandle == INVALID_HANDLE_VALUE ? false : true;
}
//---------------------------------------------------------------------------
CNewFile::CNewFile(const wstring& FileName)
  : CIoFile(FileName, GENERIC_READ | GENERIC_WRITE, CREATE_ALWAYS)
{
}
//---------------------------------------------------------------------------
CNewFile::~CNewFile()
{
}
//---------------------------------------------------------------------------
