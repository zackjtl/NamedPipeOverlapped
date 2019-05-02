//---------------------------------------------------------------------------
#include "stdafx.h"
#include <sstream>
#include <iomanip>
#include <iostream>
#include <cctype>
#include <cstdlib>

#include <stdlib.h>
#include <stdio.h>
////#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __BORLANDC__
#include <vcl.h>
#endif

#include "TypeConv.h"
#include "Bulk.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
int StrToInt(const string& Text) throw (CError)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
  int            value;
  istringstream  is(Text);

  if (Text.size() > 1) {
    switch (Text[1]) {
      case 'x' : case 'X' :
        is >> hex >> ((UINT&) value);
        break;
      default :
        is >> dec >> value;
        break;
    }
  }
  else {
    is >> dec >> value;
  }
  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << StrToWStr(Text)
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;
}
//---------------------------------------------------------------------------
int StrToInt(const wstring& Text) throw (CError)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
  int             value;
  wistringstream  is(Text);

  if (Text.size() > 1) {
    switch (Text[1]) {
      case L'x' : case L'X' :
        is >> hex >> ((UINT&) value);
        break;
      default :
        is >> dec >> value;
        break;
    }
  }
  else {
    is >> dec >> value;
  }
  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << Text
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;
}
//---------------------------------------------------------------------------
UINT64 StrToInt64(const string& Text) throw (CError)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
  UINT64          value;
  istringstream  is(Text);

  if (Text.size() > 1) {
    switch (Text[1]) {
      case 'x' : case 'X' :
        is >> hex >> ((UINT64&) value);
        break;
      default :
        is >> dec >> value;
        break;
    }
  }
  else {
    is >> dec >> value;
  }
  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << StrToWStr(Text)
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;
}
//---------------------------------------------------------------------------
UINT64 StrToInt64(const wstring& Text) throw (CError)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
  UINT64           value;
  wistringstream  is(Text);

  if (Text.size() > 1) {
    switch (Text[1]) {
      case L'x' : case L'X' :
        is >> hex >> ((UINT64&) value);
        break;
      default :
        is >> dec >> value;
        break;
    }
  }
  else {
    is >> dec >> value;
  }
  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << Text
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;
}
//---------------------------------------------------------------------------
int HexStrToInt(const string& Text) throw (CError)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
   int            value;
   istringstream  is(Text);

  is >> hex >> ((UINT&) value);

  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << StrToWStr(Text)
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;
}
//---------------------------------------------------------------------------
int HexStrToInt(const wstring& Text) throw (CError)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
   int             value;
   wistringstream  is(Text);

  is >> hex >> ((UINT&) value);

  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << Text
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;
}
//---------------------------------------------------------------------------
UINT64 HexStrToInt64(const string& Text) throw (CError)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
  UINT64          value;
  istringstream  is(Text);

  is >> hex >> ((UINT64&) value);

  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << StrToWStr(Text)
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;
}
//---------------------------------------------------------------------------
UINT64 HexStrToInt64(const wstring& Text) throw (CError)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
  UINT64           value;
  wistringstream  is(Text);

  is >> hex >> ((UINT64&) value);

  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << Text
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;
}
//---------------------------------------------------------------------------
string IntToStr(int Value, UINT FieldWidth)
{
  ostringstream  text;

  text << setw(FieldWidth) << uppercase  << setfill('0') << right << Value;

  return text.str();
}
//---------------------------------------------------------------------------
string IntToStr(int Value)
{
	//char   temp[33];

  //_itoa_s(Value, temp, 33, 10);
  ostringstream  text;
  text << uppercase  << setfill('0') << right << Value;

	return text.str();
}
//---------------------------------------------------------------------------
wstring IntToWStr(int Value, UINT FieldWidth)
{
  wostringstream  text;

  text << setw(FieldWidth) << uppercase  << setfill(L'0') << right << Value;

  return text.str();
}
//---------------------------------------------------------------------------
wstring IntToWStr(int Value)
{
	//wchar_t   temp[33];

	//_itow_s(Value, temp, (size_t)33, 10);
  wostringstream  text;
  text << uppercase  << setfill(L'0') << right << Value;

	return text.str();
}
//---------------------------------------------------------------------------
string Int64ToStr(UINT64 Value)
{
  ostringstream  text;

  text << Value;

  return text.str();
}
//---------------------------------------------------------------------------
wstring Int64ToWStr(UINT64 Value)
{
  wostringstream  text;

  text << Value;

  return text.str();
}
//---------------------------------------------------------------------------
string IntToHexStr(UINT Value, UINT Digits)
{
  ostringstream  text;

  text << hex << setw(Digits) << uppercase  << setfill('0') << right << Value;

  return string(text.str());
}
//---------------------------------------------------------------------------
wstring IntToHexWStr(UINT Value, UINT Digits)
{
  wostringstream  text;

  text << hex << setw(Digits) << uppercase  << setfill(L'0') << right << Value;

  return text.str();
}
//---------------------------------------------------------------------------
string Int64ToHexStr(UINT64 Value, UINT Digits)
{
  ostringstream  text;

  text << hex << setw(Digits) << uppercase  << setfill('0') << right << Value;

  return text.str();
}
//---------------------------------------------------------------------------
wstring Int64ToHexWStr(UINT64 Value, UINT Digits)
{
  wostringstream  text;

  text << hex << setw(Digits) << uppercase  << setfill(L'0') << right << Value;

	return text.str();
}
//---------------------------------------------------------------------------
wstring DecimalStrToDolloarForm(const wstring& Text)
{
	wstring temp = Text;
	int pos = temp.length();

	while (pos >= 4) {
		int insertPos = pos - 3;
		temp.insert(insertPos, L",");
		pos = insertPos;
	}
  return temp;
}
//---------------------------------------------------------------------------
UINT OctalStrToInt(const string& Text)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
   int            value;
   istringstream  is(Text);

  is >> oct >> ((UINT&) value);

  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << StrToWStr(Text)
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;   
}
//---------------------------------------------------------------------------
UINT OctalStrToInt(const wstring& Text)
{
  if (Text.empty()) {
    throw CError(L"[ Error] : Can't convert empty string to integer.");
  }
  int             value;
  wistringstream  is(Text);

  is >> oct >> ((UINT&) value);

  if (!is.good() && !is.eof()) {
    wostringstream  os;
    os << L"[Error] : Can't convert \""
       << Text
       << L"\" to integer.";
    throw CError(os.str());
  }
  return value;   
}
//---------------------------------------------------------------------------
string IntToOctalString(UINT Value, UINT Digits)
{
  ostringstream  text;

  text << oct << setw(Digits) << uppercase  << setfill('0') << right << Value;

  return string(text.str());
}
//---------------------------------------------------------------------------
wstring IntToOctalWideString(UINT Value, UINT Digits)
{
  wostringstream  text;

  text << oct << setw(Digits) << uppercase  << setfill(L'0') << right << Value;

  return text.str();
}
//---------------------------------------------------------------------------
UINT OctToDec(UINT Oct)
{
  string strOct = IntToStr(Oct);
  return OctalStrToInt(strOct);
}
//---------------------------------------------------------------------------
UINT DecToOct(UINT Dec)
{
  string strOct = IntToOctalString(Dec, 8);
  return StrToInt(strOct);
}
//---------------------------------------------------------------------------
std::time_t GetPosixTime()
{
  std::time_t base_time, local_time, gm_time;
  struct tm* gm_tm = NULL;
  base_time   = time(NULL);

#if defined(_MSC_VER)
  struct tm local_tm;
  errno_t rc = localtime_s(&local_tm, &base_time);
	if (rc == 0) {
		local_time = mktime(&local_tm);
    rc = gmtime_s(gm_tm, &base_time);
		if (rc == 0) {
			gm_time = mktime(gm_tm);
    }
  }
#elif defined(__BORLANDC__)
  struct tm local_tm;
  struct tm* rc = localtime_s((long*)&base_time, &local_tm);
	if (rc == 0) {
		local_time = mktime(&local_tm);
    rc = gmtime_s((long*)&base_time, gm_tm);
		if (rc == 0) {
			gm_time = mktime(gm_tm);
    } 
  }   
#else
  struct tm* local_tm = NULL;

	local_tm = localtime(&base_time);
	local_time = mktime(local_tm);
	gm_tm = gmtime(&base_time);
	gm_time = mktime(gm_tm);
#endif
  std::time_t diff = base_time - gm_time;

  return base_time;//// + diff;
}
//---------------------------------------------------------------------------
void SplitString(const string&    Source,
                 const string&    PartitionChar,
                 vector<string>&  OutBuf)
{
  OutBuf.clear();

  string::size_type start = Source.find_first_not_of(PartitionChar, 0);

  while (start != string::npos) {
    string::size_type end = Source.find_first_of(PartitionChar, start + 1);

    if (end == string::npos) {
      OutBuf.push_back(Source.substr(start, Source.size() - start));
      break;
    }
    else {
      OutBuf.push_back(Source.substr(start, end - start));
      start = Source.find_first_not_of(PartitionChar, end + 1);
    }
  }
}
//---------------------------------------------------------------------------
void SplitString(const wstring&    Source,
                 const wstring&    PartitionChar,
                 vector<wstring>&  OutBuf)
{
  OutBuf.clear();

  wstring::size_type start = Source.find_first_not_of(PartitionChar, 0);

  while (start != wstring::npos) {
    wstring::size_type end = Source.find_first_of(PartitionChar, start + 1);

    if (end == wstring::npos) {
      OutBuf.push_back(Source.substr(start, Source.size() - start));
      break;
    }
    else {
      OutBuf.push_back(Source.substr(start, end - start));
      start = Source.find_first_not_of(PartitionChar, end + 1);
    }
  }
}
//---------------------------------------------------------------------------
wstring ByteArrayToHexStr(byte* InBuf, UINT Size)
{
 wostringstream os;

  os << hex << right;

  for (; Size; --Size) {
    os << setw(2) << setfill(L'0')
       <<static_cast<int>(*InBuf)
       << L' ';
    ++InBuf;
  }
  return os.str();
}
//---------------------------------------------------------------------------
bool ErasePartitionChar(string& Text)
{
  string::size_type  pos = Text.find_first_of(":+*", 0);

  while (pos != string::npos) {
    if (0 == pos) {
      return false;
    }
    string::size_type end = Text.find_first_not_of(" \t", pos + 1);

    if (end == string::npos) {
      return false;
    }
    UINT right = end - pos - 1;

    if (right) {
      Text.erase(pos + 1, right);
    }
    string::size_type start = Text.find_last_not_of(" \t", pos - 1);

    if (start == string::npos) {
      return false;
    }
    UINT left = pos - start - 1;

    if (left) {
      Text.erase(start + 1, left);
    }
    pos = Text.find_first_of(":+*", pos - left + 2);
  }
  return true;
}
//-------------------------------------------------------------------------
bool ErasePartitionChar(wstring& Text)
{
  wstring::size_type  pos = Text.find_first_of(L":+*", 0);

  while (pos != wstring::npos) {
    if (0 == pos) {
      return false;
    }
    wstring::size_type end = Text.find_first_not_of(L" \t", pos + 1);

    if (end == wstring::npos) {
      return false;
    }
    UINT right = end - pos - 1;

    if (right) {
      Text.erase(pos + 1, right);
    }
    wstring::size_type start = Text.find_last_not_of(L" \t", pos - 1);

    if (start == wstring::npos) {
      return false;
    }
    UINT left = pos - start - 1;

    if (left) {
      Text.erase(start + 1, left);
    }
    pos = Text.find_first_of(L":+*", pos - left + 2);
  }
  return true;
}
//-------------------------------------------------------------------------
bool IsDigit(const string& Text)
{
  for (UINT idx = 0; idx < Text.size(); ++idx) {
    if (!isdigit(Text[idx])) {
      return false;
    }
  }
  return true;
}
//---------------------------------------------------------------------------
bool IsDigit(const wstring& Text)
{
  for (UINT idx = 0; idx < Text.size(); ++idx) {
    if (!iswdigit(Text[idx])) {
      return false;
    }
  }
  return true;
}
//---------------------------------------------------------------------------
bool IsHexDigit(const string& Text)
{
  for (UINT idx = 0; idx < Text.size(); ++idx) {
    if (!isxdigit(Text[idx])) {
      return false;
    }
  }
  return true;
}
//---------------------------------------------------------------------------
bool IsHexDigit(const wstring& Text)
{
  for (UINT idx = 0; idx < Text.size(); ++idx) {
    if (!iswxdigit(Text[idx])) {
      return false;
    }
  }
  return true;
}
//---------------------------------------------------------------------------
bool IsAsciiString(const wstring& Text)
{
  UINT size = Text.size();

  for (UINT idx = 0; idx < size; ++idx) {
    if ((int) Text[idx] > 0x7f) {
      return false;
    }
  }
  return true;
}
//---------------------------------------------------------------------------
wstring EraseLineEnd(const wstring& Text)
{
  return Text.substr(0, Text.rfind(L"\r\n"));
}
//-----------------------------------------------------------------------------
wstring ElapsedTimeMsToString(UINT Ms)
{
  if (Ms < 10000) {
    return IntToWStr(Ms) + L" ms";
  }
  else {
    UINT sec = Ms / 1000;
    return IntToWStr(sec) + L" sec";
  }
  return L"";
}
//---------------------------------------------------------------------------
wstring CDataBuf::ToText(byte* InBuf, UINT Length)
{
  wostringstream  text;

  text << hex << setfill(L'0');

  if (_FieldBytes == 1) {
		for (; Length; --Length) {
			text << setw(2) << (int)*InBuf++ << L' ';
    }
  }
  else {
    UINT  count;

    if (_Endian == DE_LITTLE_ENDIAN) {
      while (Length) {
        count = Length > _FieldBytes ? _FieldBytes : Length;
        Length -= count;

        for (; count; --count) {
          text << setw(2) << (int)InBuf[count - 1];
        }
        text << L' ';
        InBuf += _FieldBytes;
      }
    }
    else {
      while (Length) {
        count = Length > _FieldBytes ? _FieldBytes : Length;
        Length -= count;

        for (; count; ++count) {
          text << setw(2) << (int)*InBuf++;
        }
        text << L' ';
      }
    }
  }
  return text.str();
}
//---------------------------------------------------------------------------
wstring CDataBuf::ToAscii(const byte* InBuf, UINT Length)
{
  wchar_t         ch;
  wostringstream  text;

  for (; Length; --Length) {
    ch = *InBuf++;

    if (isprint(ch)) {
      text << ch;
    }
    else {
      text << L'.';
    }
  }
  return text.str();
}
//---------------------------------------------------------------------------
wstring CDataBuf::Dump(byte* InBuf, UINT Length, bool SkipFirstIndent)
{
  UINT            count;
  wostringstream  text;
  wstring         ident(_IndentSpaces, L' ');

  if (0 == Length) {
    return L"\r\n";
  }

  for (UINT rows = Length / _Columns; rows; rows--) {
    if (SkipFirstIndent) {
      text << ToText(InBuf, _Columns);
      SkipFirstIndent = false;
    }
    else {
      text << ident << ToText(InBuf, _Columns);
    }
    if (_AsciiCharEn) {
      text << L" ;" << ToAscii(InBuf, _Columns);
    }
    text << L"\r\n";
    InBuf += _Columns;
  }

  UINT remain = Length % _Columns;

  if (remain > 0) {
    if (!SkipFirstIndent) {
      text << ident;
    }
    text << ToText(InBuf, remain); // << space;

    if (_AsciiCharEn) {
      text << wstring((_Columns - remain) * (_FieldBytes * 2 + 1), L' ')
           << L" ;"
           << ToAscii(InBuf, remain);
    }
    text << L"\r\n";
  }

  return text.str();
}
//---------------------------------------------------------------------------
wstring CDataBuf::Dump(UINT Address, UINT Length, byte* InBuf)
{
  UINT            count;
  wostringstream  text;
  wstring         ident(_IndentSpaces, L' ');
  UINT            offset = _Columns / _FieldBytes;
  UINT            width((Address + Length) > 65535 ? 8 : 4);

  text << hex;

  for (UINT rows = Length / _Columns; rows; rows--) {
    text << ident << setfill(L'0') << setw(width) << Address << L" : " <<  ToText(InBuf, _Columns);

    if (_AsciiCharEn) {
      text << L" ;" << ToAscii(InBuf, _Columns);
    }
    text << L"\r\n";
    InBuf += _Columns;
    Address += offset;
  }

  UINT remain = Length % _Columns;

  if (remain > 0) {
    wstring space((_Columns - remain) * (_FieldBytes * 2 + 1), L' ');

    text << ident << setfill(L'0') << setw(width) << Address
         << L" : " << ToText(InBuf, remain) << space;
    if (_AsciiCharEn) {
      text << L" ;" << ToAscii(InBuf, remain);
    }
    text << L"\r\n";
  }
  return text.str();
}
//---------------------------------------------------------------------------
void CEndian::Swap(WORD* Buffer, UINT Length)
{
  SwapWords(Buffer, Length);
}
//---------------------------------------------------------------------------
void CEndian::Swap(DWORD* Buffer, UINT Length)
{
  SwapDWords(Buffer, Length);
}
//---------------------------------------------------------------------------
void CEndian::SwapWords(WORD* Buffer, UINT Length)
{
  for(; Length; Length--) {
    *Buffer = SwapWord(*Buffer);
    ++Buffer;
  }
}
//---------------------------------------------------------------------------
void CEndian::SwapDWords(DWORD* Buffer, UINT Length)
{
	for(; Length; Length--) {
		*Buffer = SwapDWord(*Buffer);
		++Buffer;
  }
}
//---------------------------------------------------------------------------
wstring StrToWStr(const string& Source)
{
#ifndef _WIN32
    return c2w( Source.c_str() );
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
string WStrToStr(const wstring& Source)
{
#ifndef _WIN32
    return w2c( Source.c_str() );
#else
  const UINT  size = Source.size() * 2 + 1;
  Bulk<char>  buffer(size);

  UINT pos = WideCharToMultiByte(CP_ACP,
                                 0,
                                 Source.c_str(),
                                 Source.size(),
                                 &buffer[0],
                                 size,
                                 0,
                                 0);
  buffer[pos] = 0;

  return string(&buffer[0]);
#endif
}//---------------------------------------------------------------------------
wstring GetDateTimeString()
{
  wostringstream  text;
#ifndef _WIN32
    tm localTime;
    std::chrono::system_clock::time_point t = std::chrono::system_clock::now();
    time_t now = std::chrono::system_clock::to_time_t( t );
    
    localtime_r(&now, &localTime);
    
    const std::chrono::duration<double> tse = t.time_since_epoch();
    std::chrono::seconds::rep milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(tse).count() % 1000;
    
    text << (int) localTime.tm_year+1900 << L'/'
    << setfill(L'0') << setw(2) << (int) localTime.tm_mon  << L'/'
    << setfill(L'0') << setw(2) << (int) localTime.tm_wday    << L"  "
    << setfill(L'0') << setw(2) << (int) localTime.tm_hour   << L':'
    << setfill(L'0') << setw(2) << (int) localTime.tm_min << L':'
    << setfill(L'0') << setw(2) << (int) localTime.tm_sec;
#else
  SYSTEMTIME      systemTime;
  GetLocalTime(&systemTime);

  text << (int) systemTime.wYear << L'/'
       << setfill(L'0') << setw(2) << (int) systemTime.wMonth  << L'/'
       << setfill(L'0') << setw(2) << (int) systemTime.wDay    << L"  "
       << setfill(L'0') << setw(2) << (int) systemTime.wHour   << L':'
			 << setfill(L'0') << setw(2) << (int) systemTime.wMinute << L':'
       << setfill(L'0') << setw(2) << (int) systemTime.wSecond;
#endif

  return text.str();
}
//---------------------------------------------------------------------------
wstring ExtractMainFileName(const wstring& FileName)
{
  wstring::size_type  pos = FileName.find_last_of(L'.');

  if (pos == wstring::npos) {
    return FileName;
  }
  else {
    return FileName.substr(0, pos);
  }
}
//---------------------------------------------------------------------------
