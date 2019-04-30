//---------------------------------------------------------------------------
#ifndef IoFileH
#define IoFileH
//---------------------------------------------------------------------------
#include <Windows.h>
////#include "BaseTypes.h"
#include "Error.h"
#include "TypeConv.h"
#include <string>

using namespace std;
//---------------------------------------------------------------------------
class CIoFile
{
public:
  CIoFile();
  CIoFile(const string&  FileName,
          UINT           DesiredAccess = GENERIC_READ | GENERIC_WRITE,
          UINT           CreationDisposition = OPEN_EXISTING,
          UINT           ShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
  CIoFile(const wstring& FileName,
          UINT          DesiredAccess = GENERIC_READ | GENERIC_WRITE,
          UINT          CreationDisposition = OPEN_EXISTING,
          UINT          ShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
  ~CIoFile();

  void Open(const string&  FileName,
            UINT           DesiredAccess = GENERIC_READ | GENERIC_WRITE,
            UINT           CreationDisposition = OPEN_EXISTING,
            UINT           ShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
  void Open(const wstring& FileName,
            UINT           DesiredAccess = GENERIC_READ | GENERIC_WRITE,
            UINT           CreationDisposition = OPEN_EXISTING,
            UINT           ShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE);
  void Close();
  void Write(byte* InBuf, UINT Count);
  void WriteByte(byte Value);
  void WriteWord(WORD Value);
  void WriteDWord(DWORD Value);
  virtual void Read(byte* OutBuf, UINT Count);
  byte ReadByte();
  WORD ReadWord();
  DWORD ReadDWord();
  void Flush();
  void Seek(UINT64 Position, int From);
  virtual void ReadString(string& Text);
  virtual void ReadString(wstring& Text);
  void WriteString(const string& Text);
  void WriteString(const wstring& Text);
  void WriteByteOrderMarkOfLittleEndian();
  bool IsUTF16();
  bool IsValidFileHandle();
  CIoFile& operator<<(const wstring& Value);
  CIoFile& operator<<(wchar_t Value);
  CIoFile& operator<<(int Value);
  CIoFile& operator<<(unsigned int Value);
  CIoFile& operator<<(long Value);
  CIoFile& operator<<(unsigned long Value);
  UINT64 GetSize();
protected:
  HANDLE    _FileHandle;
  wstring   _FileName;
};
//---------------------------------------------------------------------------
class CNewFile : public CIoFile
{
public:
  CNewFile(const wstring& FileName);
  ~CNewFile();
};
//---------------------------------------------------------------------------
#endif
