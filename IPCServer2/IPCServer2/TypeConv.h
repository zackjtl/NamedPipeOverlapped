#ifndef TypeConvH
#define TypeConvH
//-----------------------------------------------------------------------------
#include <minwindef.h>
#include <string>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "Error.h"

#pragma warning( disable : 4290)

using namespace std;
//-----------------------------------------------------------------------------
int     StrToInt(const string& Text) throw (CError);
int     StrToInt(const wstring& Text) throw (CError);
UINT64  StrToInt64(const string& Text) throw (CError);
UINT64  StrToInt64(const wstring& Text) throw (CError);
int     HexStrToInt(const string& Text) throw (CError);
int     HexStrToInt(const wstring& Text) throw (CError);
UINT64  HexStrToInt64(const string& Text) throw (CError);
UINT64  HexStrToInt64(const wstring& Text) throw (CError);
string  IntToStr(int Value);
wstring IntToWStr(int Value, UINT FieldWidth);
wstring IntToWStr(int Value);
string  Int64ToStr(UINT64 Value);
wstring Int64ToWStr(UINT64 Value);
string  IntToStr(int Value, UINT FieldWitdh);
string  IntToHexStr(UINT Value, UINT Digits);
wstring IntToHexWStr(UINT Value, UINT Digits);
string  Int64ToHexStr(UINT64 Value, UINT Digits);
wstring Int64ToHexWStr(UINT64 Value, UINT Digits);
wstring DecimalStringToDolloarForm(const wstring& Text);
UINT    OctalStrToInt(const string& Text);
UINT    OctalStrToInt(const wstring& Text);
string IntToOctalString(UINT Value, UINT Digits);
wstring IntToOctalWStr(UINT Value, UINT Digits);

UINT    OctToDec(UINT Oct);
UINT    DecToOct(UINT Dec);

std::time_t GetPosixTime();

wstring GetDateTimeString();

void   SplitString(const string&    Source,
                   const string&    PartitionChar,
                   vector<string>&  OutBuf);
void   SplitString(const wstring&    Source,
                   const wstring&    PartitionChar,
                   vector<wstring>&  OutBuf);
wstring ByteArrayToHexStr(byte* InBuf, UINT Size);

bool   IsDigit(const string& Text);
bool   IsDigit(const wstring& Text);
bool   IsHexDigit(const string& Text);
bool   IsHexDigit(const wstring& Text);
bool   IsAsciiString(const wstring& Text);
wstring EraseLineEnd(const wstring& Text);
wstring ElapsedTimeMsToStr(UINT Ms);
wstring ExtractMainFileName(const wstring& FileName);
//-----------------------------------------------------------------------------
enum { DE_LITTLE_ENDIAN = 0, DE_BIG_ENDIAN };
class CDataBuf
{
public:
       CDataBuf(UINT Columns = 16,
                UINT FieldBytes = 1,
                UINT Endian = DE_BIG_ENDIAN,
                bool AsciiCharEn = true,
                UINT IndentSpaces = 1)
         : _Columns(Columns),
           _FieldBytes(FieldBytes),
           _Endian(Endian),
           _AsciiCharEn(AsciiCharEn),
           _IndentSpaces(IndentSpaces)
       {
       }
  void SetColumns(UINT Value)      { _Columns = Value;      }
  void SetEndian(UINT Value)       { _Endian = Value;       }
  void SetFieldBytes(UINT Value)   { _FieldBytes = Value;   }
  void SetIndentSpaces(UINT Value) { _IndentSpaces = Value; }
  void EnableAsciiChar(bool Value) { _AsciiCharEn = Value;  }

  wstring ToText(byte* InBuf, UINT Length);
  wstring ToAscii(const byte* InBuf, UINT Length);
  wstring Dump(byte *InBuf, UINT Length, bool SkipFirstIndent = false);
  wstring Dump(UINT Address, UINT Length, byte *Buffer);

private :
  UINT   _Columns;
  UINT   _IndentSpaces;
  UINT   _FieldBytes;
  bool   _AsciiCharEn;
  UINT   _Endian;
};
//-----------------------------------------------------------------------------
template <class T>
void HexStrToInts(const string& Text, vector<T>& Integers) throw (CError)
{
  Integers.clear();

  bool              stop = false;
  string::size_type start = Text.find_first_not_of(" \t", 0);

  while (start != string::npos) {
    string::size_type end = Text.find_first_of(" \t,", start + 1);
    if (end == string::npos) {
      stop = true;
      end = Text.size();
    }
    Integers.push_back(
      HexStrToInt(Text.substr(start, end - start)));
    if (stop) {
      break;
    }
    start = Text.find_first_not_of(", \t\n", end);
  }
}
//-----------------------------------------------------------------------------
template <class T>
void HexStrToInts(const wstring& Text, vector<T>& Integers) throw (CError)
{
  Integers.clear();

  bool               stop = false;
  wstring::size_type start = Text.find_first_not_of(L" \t", 0);

  while (start != wstring::npos) {
    wstring::size_type end = Text.find_first_of(L" \t,", start + 1);
    if (end == wstring::npos) {
      stop = true;
      end = Text.size();
    }
    Integers.push_back(
      HexStrToInt(Text.substr(start, end - start)));
    if (stop) {
      break;
    }
    start = Text.find_first_not_of(L", \t\n", end);
  }
}
//-----------------------------------------------------------------------------
template <class T>
void StrToInts(const string& Text, vector<T>& Integers) throw (CError)
{
  Integers.clear();

  bool              stop = false;
  string::size_type start = Text.find_first_not_of(" \t", 0);

  while (start != string::npos) {
    string::size_type end = Text.find_first_of(" \t,", start + 1);

    if (end == string::npos) {
      stop = true;
      end = Text.size();
    }
    Integers.push_back(StrToInt(Text.substr(start, end - start)));

    if (stop) {
      break;
    }
    start = Text.find_first_not_of(", \t\n", end);
  }
}
//-----------------------------------------------------------------------------
template <class T>
void StrToInts(const wstring& Text, vector<T>& Integers) throw (CError)
{
  Integers.clear();

  bool               stop = false;
  wstring::size_type start = Text.find_first_not_of(L" \t", 0);

  while (start != wstring::npos) {
    wstring::size_type end = Text.find_first_of(L" \t,", start + 1);

    if (end == wstring::npos) {
      stop = true;
      end = Text.size();
    }
    Integers.push_back(StrToInt(Text.substr(start, end - start)));

    if (stop) {
      break;
    }
    start = Text.find_first_not_of(L", \t\n", end);
  }
}
//-----------------------------------------------------------------------------
template <class T>
void ColonStrToInts(const string& Text, vector<T>& Items)
{
  Items.clear();

  string::size_type  token = Text.find(':', 0);

  if (token != string::npos) {
    string::size_type start = Text.find_first_not_of(" \t", 0);
    string::size_type end = Text.find_first_of(": \t,", start + 1);

    if (end == string::npos) {
      return;
    }
    UINT begin = StrToInt(Text.substr(start, end - start));

    start = Text.find_first_not_of(" \t", token + 1);

    if (start == string::npos) {
      return;
    }
    end = Text.find_first_of(" \t,", start + 1);

    if (end == string::npos) {
      end = Text.size();
    }
    UINT final = StrToInt(Text.substr(start, end - start));

    if (final >= begin) {
      UINT idx = 0;

      Items.resize(final - begin + 1);

      for (; begin <= final; ++begin) {
        Items[idx++] = begin;
      }
    }
  }
  else {
    if (!Text.empty()) {
      Items.push_back(StrToInt(Text));
    }
  }
}
//-----------------------------------------------------------------------------
template <class T>
void ColonStrToInts(const wstring& Text, vector<T>& Items)
{
  Items.clear();

  wstring::size_type  token = Text.find(L':', 0);

  if (token != wstring::npos) {
    wstring::size_type start = Text.find_first_not_of(L" \t", 0);
    wstring::size_type end = Text.find_first_of(L": \t,", start + 1);

    if (end == wstring::npos) {
      return;
    }
    UINT begin = StrToInt(Text.substr(start, end - start));

    start = Text.find_first_not_of(L" \t", token + 1);

    if (start == wstring::npos) {
      return;
    }
    end = Text.find_first_of(L" \t,", start + 1);

    if (end == wstring::npos) {
      end = Text.size();
    }
    UINT final = StrToInt(Text.substr(start, end - start));

    if (final >= begin) {
      UINT idx = 0;

      Items.resize(final - begin + 1);

      for (; begin <= final; ++begin) {
        Items[idx++] = begin;
      }
    }
  }
  else {
    if (!Text.empty()) {
      Items.push_back(StrToInt(Text));
    }
  }
}
//-----------------------------------------------------------------------------
template <class T>
void ColonHexStrToInts(const string& Text, vector<T>& Items)
{
  Items.clear();

  string::size_type  token = Text.find(':', 0);

  if (token != string::npos) {
    string::size_type start = Text.find_first_not_of(" \t", 0);
    string::size_type end = Text.find_first_of(": \t,", start + 1);

    if (end == string::npos) {
      return;
    }
    UINT begin = HexStrToInt(Text.substr(start, end - start));

    start = Text.find_first_not_of(" \t", token + 1);

    if (start == string::npos) {
      return;
    }
    end = Text.find_first_of(" \t,", start + 1);

    if (end == string::npos) {
      end = Text.size();
    }
    UINT final = HexStrToInt(Text.substr(start, end - start));

    if (final >= begin) {
      UINT idx = 0;

      Items.resize(final - begin + 1);

      for (; begin <= final; ++begin) {
        Items[idx++] = begin;
      }
    }
  }
  else {
    if (!Text.empty()) {
      Items.push_back(HexStrToInt(Text));
    }
  }
}
//-----------------------------------------------------------------------------
template <class T>
void ColonHexStrToInts(const wstring& Text, vector<T>& Items)
{
  Items.clear();

  wstring::size_type  token = Text.find(L':', 0);

  if (token != wstring::npos) {
    wstring::size_type start = Text.find_first_not_of(L" \t", 0);
    wstring::size_type end = Text.find_first_of(L": \t,", start + 1);

    if (end == wstring::npos) {
      return;
    }
    UINT begin = HexStrToInt(Text.substr(start, end - start));

    start = Text.find_first_not_of(L" \t", token + 1);

    if (start == wstring::npos) {
      return;
    }
    end = Text.find_first_of(L" \t,", start + 1);

    if (end == wstring::npos) {
      end = Text.size();
    }
    UINT final = HexStrToInt(Text.substr(start, end - start));

    if (final >= begin) {
      UINT idx = 0;

      Items.resize(final - begin + 1);

      for (; begin <= final; ++begin) {
        Items[idx++] = begin;
      }
    }
  }
  else {
    if (!Text.empty()) {
      Items.push_back(HexStrToInt(Text));
    }
  }
}
//-----------------------------------------------------------------------------
template <class T>
void AddOpStrToInts(const string& Text, vector<T>& Items)
{
  Items.clear();

  string::size_type  token = Text.find('+', 0);

  if (token != string::npos) {
    string::size_type start = Text.find_first_not_of(" \t", 0);
    string::size_type end = Text.find_first_of("+ \t,", start + 1);

    if (end == string::npos) {
      return;
    }
    vector<T>  begin;

    ColonStrToInts(Text.substr(start, end - start), begin);

    start = Text.find_first_not_of(" \t", token + 1);

    if (start == string::npos) {
      return;
    }
    end = Text.find_first_of("* \t,", start + 1);

    if (end == string::npos) {
      return;
    }
    UINT inc = StrToInt(Text.substr(start, end - start));

    token = Text.find('*', 0);

    if (token == string::npos) {
      return;
    }

    start = Text.find_first_not_of(" \t", token + 1);

    if (start == string::npos) {
      return;
    }
    end = Text.find_first_of(" \t,", start + 1);

    if (end == string::npos) {
      end = Text.size();
    }
    UINT iter  = 0;
    UINT size  = begin.size();
    UINT count = StrToInt(Text.substr(start, end - start));

    Items.resize(count * size);

    for (UINT idx = 0; idx < size; ++idx) {
      start = begin[idx];

      for (UINT num = count; num; --num, start += inc) {
        Items[iter++] = start;
      }
    }
  }
}
//-----------------------------------------------------------------------------
template <class T>
void AddOpStrToInts(const wstring& Text, vector<T>& Items)
{
  Items.clear();

  wstring::size_type  token = Text.find(L'+', 0);

  if (token != wstring::npos) {
    wstring::size_type start = Text.find_first_not_of(L" \t", 0);
    wstring::size_type end = Text.find_first_of(L"+ \t,", start + 1);

    if (end == wstring::npos) {
      return;
    }
    vector<T>  begin;

    ColonStrToInts(Text.substr(start, end - start), begin);

    start = Text.find_first_not_of(L" \t", token + 1);

    if (start == wstring::npos) {
      return;
    }
    end = Text.find_first_of(L"* \t,", start + 1);

    if (end == wstring::npos) {
      return;
    }
    UINT inc = StrToInt(Text.substr(start, end - start));

    token = Text.find(L'*', 0);

    if (token == wstring::npos) {
      return;
    }

    start = Text.find_first_not_of(L" \t", token + 1);

    if (start == wstring::npos) {
      return;
    }
    end = Text.find_first_of(L" \t,", start + 1);

    if (end == wstring::npos) {
      end = Text.size();
    }
    UINT iter  = 0;
    UINT size  = begin.size();
    UINT count = StrToInt(Text.substr(start, end - start));

    Items.resize(count * size);

    for (UINT idx = 0; idx < size; ++idx) {
      start = begin[idx];

      for (UINT num = count; num; --num, start += inc) {
        Items[iter++] = start;
      }
    }
  }
}
//-----------------------------------------------------------------------------
template <class T>
void AddOpHexStrToInts(const string& Text, vector<T>& Items)
{
  Items.clear();

  string::size_type  token = Text.find('+', 0);

  if (token != string::npos) {
    string::size_type start = Text.find_first_not_of(" \t", 0);
    string::size_type end = Text.find_first_of("+ \t,", start + 1);

    if (end == string::npos) {
      return;
    }
    vector<T>  begin;

    ColonHexStrToInts(Text.substr(start, end - start), begin);

    start = Text.find_first_not_of(" \t", token + 1);

    if (start == string::npos) {
      return;
    }
    end = Text.find_first_of("* \t,", start + 1);

    if (end == string::npos) {
      return;
    }
    UINT inc = HexStrToInt(Text.substr(start, end - start));

    token = Text.find('*', 0);

    if (token == string::npos) {
      return;
    }

    start = Text.find_first_not_of(" \t", token + 1);

    if (start == string::npos) {
      return;
    }
    end = Text.find_first_of(" \t,", start + 1);

    if (end == string::npos) {
      end = Text.size();
    }
    UINT iter  = 0;
    UINT size  = begin.size();
    UINT count = HexStrToInt(Text.substr(start, end - start));

    Items.resize(count * size);

    for (UINT idx = 0; idx < size; ++idx) {
      start = begin[idx];

      for (UINT num = count; num; --num, start += inc) {
        Items[iter++] = start;
      }
    }
  }
}
//-----------------------------------------------------------------------------
template <class T>
void AddOpHexStrToInts(const wstring& Text, vector<T>& Items)
{
  Items.clear();

  wstring::size_type  token = Text.find('+', 0);

  if (token != wstring::npos) {
    wstring::size_type start = Text.find_first_not_of(L" \t", 0);
    wstring::size_type end = Text.find_first_of(L"+ \t,", start + 1);

    if (end == wstring::npos) {
      return;
    }
    vector<T>  begin;

    ColonHexStrToInts(Text.substr(start, end - start), begin);

    start = Text.find_first_not_of(L" \t", token + 1);

    if (start == wstring::npos) {
      return;
    }
    end = Text.find_first_of(L"* \t,", start + 1);

    if (end == wstring::npos) {
      return;
    }
    UINT inc = HexStrToInt(Text.substr(start, end - start));

    token = Text.find('*', 0);

    if (token == wstring::npos) {
      return;
    }

    start = Text.find_first_not_of(L" \t", token + 1);

    if (start == string::npos) {
      return;
    }
    end = Text.find_first_of(L" \t,", start + 1);

    if (end == wstring::npos) {
      end = Text.size();
    }
    UINT iter  = 0;
    UINT size  = begin.size();
    UINT count = HexStrToInt(Text.substr(start, end - start));

    Items.resize(count * size);

    for (UINT idx = 0; idx < size; ++idx) {
      start = begin[idx];

      for (UINT num = count; num; --num, start += inc) {
        Items[iter++] = start;
      }
    }
  }
}
//-----------------------------------------------------------------------------
bool ErasePartitionChar(string& Text);
bool ErasePartitionChar(wstring& Text);
//-----------------------------------------------------------------------------
template <class T>
void SimpleRegExStrToInts(const string& Text, vector<T>& Ints)
{
  Ints.clear();

  string  str(Text);

  if (!ErasePartitionChar(str)) {
    throw CError(L"RegExStrToInts() fail.");
  }
  vector<T>       temp;
  vector<string>  items;

  SplitString(str, ", \t", items);

  UINT itemCount = items.size();

  for (UINT idx = 0; idx < itemCount; ++idx) {
    string&  subStr = items[idx];

    if (subStr.find('+') != string::npos) {
      AddOpStrToInts(subStr, temp);
    }
    else {
      if (subStr.find(':') != string::npos) {
        ColonStrToInts(subStr, temp);
      }
      else {
        temp.resize(1);
        temp[0] = StrToInt(subStr);
      }
    }
    UINT size = Ints.size();

    Ints.resize(size + temp.size());

    copy(temp.begin(), temp.end(), Ints.begin() + size);
  }
  sort(Ints.begin(), Ints.end());
  Ints.erase(unique(Ints.begin(), Ints.end()), Ints.end());
}
//-----------------------------------------------------------------------------
template <class T>
void RemoveVector(vector<T>& Source, const vector<T>& Discard)
{
	typename vector<T>::const_iterator it = Discard.begin();
  typename vector<T>::const_iterator end = Discard.end();

  while (it != end) {
		typename vector<T>::iterator endPos = Source.end();
    typename vector<T>::iterator pos = find(Source.begin(), endPos, *it);

    if (pos != endPos) {
      Source.erase(pos);
    }
    ++it;
  }
}
//-----------------------------------------------------------------------------
template <class T>
void RegExStrToInts(const string& Text, vector<T>& Ints)
{
  Ints.clear();

  string  str(Text);

  if (!ErasePartitionChar(str)) {
    throw CError(L"RegExStrToInts() fail.");
  }

  vector<T>       discardInts;
  vector<string>  discardItems;

  SplitString(str, "-", discardItems);

  UINT end = discardItems.size();

  if (end > 2) {
    end = 2;
  }
  for (UINT from = 0; from < end; ++from) {
    vector<T>       temp;
    vector<string>  items;

    SplitString(discardItems[from], ", \t", items);

    UINT itemCount = items.size();

    for (UINT idx = 0; idx < itemCount; ++idx) {
      string&  subStr = items[idx];

      if (subStr.find('+') != string::npos) {
        AddOpStrToInts(subStr, temp);
      }
      else {
        if (subStr.find(':') != string::npos) {
          ColonStrToInts(subStr, temp);
        }
        else {
          temp.resize(1);
          temp[0] = StrToInt(subStr);
        }
      }
      if (from) {
        UINT size = discardInts.size();

        discardInts.resize(size + temp.size());

        copy(temp.begin(), temp.end(), discardInts.begin() + size);
      }
      else {
        UINT size = Ints.size();

        Ints.resize(size + temp.size());

        copy(temp.begin(), temp.end(), Ints.begin() + size);
      }
    }
  }
  sort(Ints.begin(), Ints.end());
  Ints.erase(unique(Ints.begin(), Ints.end()), Ints.end());
  RemoveVector(Ints, discardInts);
}
//-----------------------------------------------------------------------------
template <class T>
void RegExStrToInts(const wstring& Text, vector<T>& Ints)
{
  Ints.clear();

  wstring  str(Text);

  if (!ErasePartitionChar(str)) {
    throw CError(L"RegExStrToInts() fail.");
  }

  vector<T>        discardInts;
  vector<wstring>  discardItems;

  SplitString(str, L"-", discardItems);

  UINT end = discardItems.size();

  if (end > 2) {
    end = 2;
  }
  for (UINT from = 0; from < end; ++from) {
    vector<T>        temp;
    vector<wstring>  items;

    SplitString(discardItems[from], L", \t", items);

    UINT itemCount = items.size();

    for (UINT idx = 0; idx < itemCount; ++idx) {
      wstring&  subStr = items[idx];

      if (subStr.find(L'+') != wstring::npos) {
        AddOpStrToInts(subStr, temp);
      }
      else {
        if (subStr.find(L':') != wstring::npos) {
          ColonStrToInts(subStr, temp);
        }
        else {
          temp.resize(1);
          temp[0] = StrToInt(subStr);
        }
      }
      if (from) {
        UINT size = discardInts.size();

        discardInts.resize(size + temp.size());

        copy(temp.begin(), temp.end(), discardInts.begin() + size);
      }
      else {
        UINT size = Ints.size();

        Ints.resize(size + temp.size());

        copy(temp.begin(), temp.end(), Ints.begin() + size);
      }
    }
  }
  sort(Ints.begin(), Ints.end());
  Ints.erase(unique(Ints.begin(), Ints.end()), Ints.end());
  RemoveVector(Ints, discardInts);
}
//-----------------------------------------------------------------------------
template <class T>
void RegExHexStrToInts(const string& Text, vector<T>& Ints, bool SortInts = true)
{
  Ints.clear();

  string  str(Text);

  if (!ErasePartitionChar(str)) {
    throw CError(L"RegExHexStrToInts() fail.");
  }

  vector<T>       discardInts;
  vector<string>  discardItems;

  SplitString(str, "-", discardItems);

  UINT end = discardItems.size();

  if (end > 2) {
    end = 2;
  }
  for (UINT from = 0; from < end; ++from) {
    vector<T>       temp;
    vector<string>  items;

    SplitString(discardItems[from], ", \t", items);

    UINT itemCount = items.size();

    for (UINT idx = 0; idx < itemCount; ++idx) {
      string&  subStr = items[idx];

      if (subStr.find('+') != string::npos) {
        AddOpHexStrToInts(subStr, temp);
      }
      else {
        if (subStr.find(':') != string::npos) {
          ColonHexStrToInts(subStr, temp);
        }
        else {
          temp.resize(1);
          temp[0] = HexStrToInt(subStr);
        }
      }
      if (from) {
        UINT size = discardInts.size();

        discardInts.resize(size + temp.size());

        copy(temp.begin(), temp.end(), discardInts.begin() + size);
      }
      else {
        UINT size = Ints.size();

        Ints.resize(size + temp.size());

        copy(temp.begin(), temp.end(), Ints.begin() + size);
      }
    }
  }
  if (SortInts) {
    sort(Ints.begin(), Ints.end());
    Ints.erase(unique(Ints.begin(), Ints.end()), Ints.end());
  }
  RemoveVector(Ints, discardInts);
}
//-----------------------------------------------------------------------------
template <class T>
void RegExHexStrToInts(const wstring& Text, vector<T>& Ints, bool SortInts = true)
{
  Ints.clear();

  wstring  str(Text);

  if (!ErasePartitionChar(str)) {
    throw CError(L"RegExHexStrToInts() fail.");
  }

  vector<T>        discardInts;
  vector<wstring>  discardItems;

  SplitString(str, L"-", discardItems);

  UINT end = discardItems.size();

  if (end > 2) {
    end = 2;
  }
  for (UINT from = 0; from < end; ++from) {
    vector<T>        temp;
    vector<wstring>  items;

    SplitString(discardItems[from], L", \t", items);

    UINT itemCount = items.size();

    for (UINT idx = 0; idx < itemCount; ++idx) {
      wstring&  subStr = items[idx];

      if (subStr.find(L'+') != wstring::npos) {
        AddOpHexStrToInts(subStr, temp);
      }
      else {
        if (subStr.find(L':') != wstring::npos) {
          ColonHexStrToInts(subStr, temp);
        }
        else {
          temp.resize(1);
          temp[0] = HexStrToInt(subStr);
        }
      }
      if (from) {
        UINT size = discardInts.size();

        discardInts.resize(size + temp.size());

        copy(temp.begin(), temp.end(), discardInts.begin() + size);
      }
      else {
        UINT size = Ints.size();

        Ints.resize(size + temp.size());

        copy(temp.begin(), temp.end(), Ints.begin() + size);
      }
    }
  }
  if (SortInts) {
    sort(Ints.begin(), Ints.end());
    Ints.erase(unique(Ints.begin(), Ints.end()), Ints.end());
  }
  RemoveVector(Ints, discardInts);
}
//-----------------------------------------------------------------------------
class CEndian
{
public :
	WORD Swap(WORD Value)
  {
    return SwapWord(Value);
  }
  DWORD Swap(DWORD Value)
  {
    return SwapWord(Value);
  }
  WORD SwapWord(WORD Value)
  {
    return ((Value >> 8) + (Value << 8));
  }
  DWORD SwapDWord(DWORD Value)
  {
    return ((Value << 24) +
            (Value >> 24) +
            ((Value << 8) & 0x00FF0000) +
            ((Value >> 8) & 0x0000FF00));
  }
  void Swap(WORD* Buffer, UINT Length);
  void Swap(DWORD* Buffer, UINT Length);
  void SwapWords(WORD* Buffer, UINT Length);
  void SwapDWords(DWORD* Buffer, UINT Length);
};
//-----------------------------------------------------------------------------
wstring StrToWStr(const string& Source);
string  WStrToStr(const wstring& Source);
//-----------------------------------------------------------------------------
enum TSModes{TSMode_Short, TSMode_Normal, TSMode_Full, TSMode_FileName};
//-----------------------------------------------------------------------------
static void time_to_gmtime(time_t* time, tm& tm)
{
#if defined(__BORLANDC__)
		localtime_s(time, &tm);
#elif defined(_MSC_VER)
		localtime_s(&tm, time);
#else
		tm = *localtime(time);
#endif  
}
//-----------------------------------------------------------------------------
#define CTimeStamp class_TimeStamp::GetInstance()
class class_TimeStamp
{
public:
	// Singleton
	static class_TimeStamp& GetInstance()
	{
		static class_TimeStamp Inst;
		return Inst;
	}

public:
	class_TimeStamp() {}

  struct tm get_tm_struct() {
    //struct tm tm;
		time(&_time);
    time_to_gmtime(&_time, _tm);
    return _tm;
  }

	wstring str(int mode = TSMode_Short) {
		time(&_time);
    time_to_gmtime(&_time, _tm);
		int year = _tm.tm_year + 1900;
		int month = _tm.tm_mon + 1;
		int date = _tm.tm_mday;
		int hour = _tm.tm_hour;
		int min = _tm.tm_min;
		int sec = _tm.tm_sec;

		wostringstream  text;

		if (mode == TSMode_Short) {
			year = year - (year / 100) * 100;
			text << setfill(L'0') << setw(2) << year
					 << setfill(L'0') <<setw(2) << month
					 << setfill(L'0') <<setw(2) << date
					 << setfill(L'0') <<setw(2) << hour
					 << setfill(L'0') <<setw(2) << min
					 << setfill(L'0') <<setw(2) << sec;
		}
		else if (mode == TSMode_FileName) {
			text << setfill(L'0') << setw(4) << year
					 << setfill(L'0') <<setw(2) << month
					 << setfill(L'0') <<setw(2) << date
					 << L"_"
					 << setfill(L'0') <<setw(2) << hour
					 << setfill(L'0') <<setw(2) << min
					 << setfill(L'0') <<setw(2) << sec;
		}
		else if (mode == TSMode_Normal){
			text << setfill(L'0') << setw(2) << year
					 << L"-" << setfill(L'0') <<setw(2) << month
					 << L"-" << setfill(L'0') <<setw(2) << date
					 << L" " << setfill(L'0') <<setw(2) << hour
					 << L":" << setfill(L'0') <<setw(2) << min
					 << L":" << setfill(L'0') <<setw(2) << sec;
		}
		return text.str();
	}

private:
	time_t		  _time;
  struct tm   _tm;
};
//-----------------------------------------------------------------------------

#endif
