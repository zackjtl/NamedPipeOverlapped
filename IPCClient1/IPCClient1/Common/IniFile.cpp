#include "stdafx.h"
#pragma hdrstop
//---------------------------------------------------------------------------
#include <iterator>
#include <algorithm>
#include "IniFile.h"
#include "randomc.h"
#include "FileError.h"
#include "IoFile.h"
#include "TypeIterator.h"
//---------------------------------------------------------------------------
using namespace std;
//---------------------------------------------------------------------------
CIniFileLoader::CIniFileLoader()
	: _FileName(L""),
		_Position(0),
		_StartOfComment(0),
		_EndOfComment(-1)
{
}
//---------------------------------------------------------------------------
CIniFileLoader::CIniFileLoader(const wstring& FileName)
  : _FileName(FileName),
    _Position(0),
    _StartOfComment(0),
    _EndOfComment(-1)
{
  CIoFile  inFile(FileName, GENERIC_READ, OPEN_EXISTING);

  inFile.ReadString(_Text);

  GetPositionOfComment();
}
//---------------------------------------------------------------------------
CIniFileLoader::CIniFileLoader(const wstring& FileName, UINT RandomSeed)
  : _FileName(FileName),
    _Position(0),
    _StartOfComment(0),
    _EndOfComment(-1)
{

  CIoFile  inFile(FileName, GENERIC_READ, OPEN_EXISTING);

  inFile.ReadString(_Text);

  UINT             size = _Text.size();
  TRandomMersenne  pattern(RandomSeed);

  for(UINT idx = 0; idx < size; ++idx)  {
    wchar_t ch = _Text[idx];

    switch (ch) {
      case L'\r': case L'\n':
        break;
      default:
#if defined(__GNUC__) || defined(__LIBSDK__)
		if( ( wcscmp( FileName.c_str(), L".\\config" ) == 0 ) ||
				wcscmp( FileName.c_str(), L"./config" ) == 0 )
		{
			_Text[idx]= (ch & 0xfff0) + ((ch^pattern.IRandom(0, 255)) & 0x000f);
		}
		else
		{
			dword* dwptr = (dword*)&_Text[idx];
			word wc = (word)(*dwptr);
			*dwptr =(wc & 0xfff0) + (((wc^pattern.IRandom(0, 255)) & 0x000f));
		}
#else
			_Text[idx]= (ch & 0xfff0) + ((ch^pattern.IRandom(0, 255)) & 0x000f);
#endif
			break;
    }
  }
  GetPositionOfComment();
}
//---------------------------------------------------------------------------
void CIniFileLoader::GetPositionOfComment()
{
  _StartOfComment = _Text.find(L'/', _EndOfComment + 1);

  if (_StartOfComment == wstring::npos) {
    _StartOfComment = _EndOfComment = _Text.size();
    return;
  }

  wstring::size_type next = _StartOfComment + 1;

  if (next >= _Text.size()) {
    wostringstream  os;
    os << L"[Ini File Error] " << _FileName
       << L"\n : Missing '/' on line = "
       << (int) count(_Text.begin(), _Text.begin() + next, L'\n')
       << L'.';
    throw CIniFileError(IE_SYNTAX, os.str(), _FileName);
  }

  switch (_Text[next]) {
    case L'/' :
      _EndOfComment = _Text.find(L"\r\n", next + 1);

      if (_EndOfComment == wstring::npos) {
        _EndOfComment = _Text.size();
      }
      break;
    case L'*' :
      _EndOfComment = _Text.find(L'*', next + 1);

      if (_EndOfComment != wstring::npos) {
        _EndOfComment++;

        if (_EndOfComment < _Text.size()) {
          if (_Text[_EndOfComment] == L'/') {
            return;
          }
          else {
            wostringstream os;
            os << L"[Ini File Error] " << _FileName
               << L"\n : Missing '/' on line = " << (int) count(_Text.begin(),_Text.begin() + _EndOfComment, '\n')
               << L'.';
            throw CIniFileError(IE_SYNTAX, os.str(), _FileName);
          }
        }
      }
      else {
        wostringstream  os;
        os << L"[Ini File Error] " << _FileName
           << L"\n : missing '*' on line = "
           << count(_Text.begin(), _Text.begin() + next, '\n')
           << L'.';
        throw CIniFileError(IE_SYNTAX, os.str(), _FileName);
      }
    default:
      wostringstream os;
      os << L"[Ini File Error] " << _FileName
         << L"\n : Missing '/' on line = "
         << count(_Text.begin(), _Text.begin() + next, L'\n')
         << L'.';
      throw CIniFileError(IE_SYNTAX, os.str(), _FileName);
  }
}
//---------------------------------------------------------------------------
wstring::size_type CIniFileLoader::Find(wchar_t Ch, wstring::size_type Index)
{
  Index = _Text.find(Ch, Index);

  if (Index != wstring::npos) {
    while (Index >= _StartOfComment) {
      if (Index <= _EndOfComment) {
        Index = _Text.find(Ch, _EndOfComment + 1);

        if (Index == wstring::npos) {
          return Index;
        }
      }
      GetPositionOfComment();
    }
  }
  return Index;
}
//---------------------------------------------------------------------------
wstring::size_type CIniFileLoader::FindFirstNotOf(
  const wchar_t*     Str,
  string::size_type  Index)
{
  Index = _Text.find_first_not_of(Str, Index);

  if (Index != wstring::npos) {
    while (Index >= _StartOfComment) {
      if (Index <= _EndOfComment) {
        Index = _Text.find_first_not_of(Str, _EndOfComment + 1);
        if (Index == wstring::npos) { return Index; }
      }
      GetPositionOfComment();
    }
  }

  return Index;
}
//---------------------------------------------------------------------------
void CIniFileLoader::GetItem(wstring& Item, wchar_t EndToken)
{
  Item = L"";

  wstring::size_type start = _Position;

  if (EndToken == L']') {
    // Find start position of section.
    start = Find(L'[', start);

    if (start == wstring::npos) {
      _Position = _Text.size();
      return;
    }
    else {
      _Position = ++start;
    }
  }
  start = FindFirstNotOf(L" \t\r\n", start);

  if (start == wstring::npos) {
    _Position = _Text.size();
    return;
  }
  else {
    _Position = start;

    if((_Text[start] == L'[') && (EndToken != L']')) {
      return;
    }
  }

  wstring::size_type end = _Text.find(EndToken, start);

  if (end != wstring::npos) {
    if (start != end) {
      while (end >= _StartOfComment) {
        Item += _Text.substr(start, _StartOfComment - start);

        start = _EndOfComment + 1;

        if (end <= _EndOfComment) {
          end = _Text.find(EndToken, start);

          if (end == string::npos) {
            wostringstream os;
            os << L"[Ini File Error] " << _FileName
               << L"\n : Missing '"
               << EndToken
               << L"' on line = "
               << count(_Text.begin(), _Text.begin() + start, L'\n')
               << L'.';
            throw CIniFileError(IE_SYNTAX, os.str(), _FileName);
          }
        }

        GetPositionOfComment();
      }

      _Position = end + 1;
      end = _Text.find_last_not_of(L" \t\r\n", end - 1);
      Item += _Text.substr(start, end - start + 1);
      // �M�����C�r��
      Item.erase(remove(Item.begin(), Item.end(), L'\r'), Item.end());
      Item.erase(remove(Item.begin(), Item.end(), L'\n'), Item.end());
    }
    else {
      _Position = end + 1;
    }
  }
  else {
    wostringstream os;
    os << L"[Ini File Error] " << _FileName
       << L"\n : Missing '"
       << EndToken
       << L"' on line = "
       << count(_Text.begin(), _Text.begin() + _Position, L'\n')
       << L'.';

    throw CIniFileError(IE_SYNTAX, os.str(), _FileName);
  }
}
//---------------------------------------------------------------------------
void CIniFileLoader::GetKeyMap(CKeyMap& KeyMap)
{
  wstring  key;
  wstring  value;

  KeyMap.clear();

  while (!End()) {
    GetItem(key, L'=');

    if (!key.empty()) {
      GetItem(value, L';');
      KeyMap.insert(make_pair(key, value));
    }
    else {
      break;
    }
  }
}
//---------------------------------------------------------------------------
CIniFile::CIniFile(const CIniFile& Source)
  : _SectionMap(Source._SectionMap),
    _FileName(Source._FileName)
{
}
//---------------------------------------------------------------------------
CIniFile& CIniFile::operator=(const CIniFile& Source)
{
  if (&Source != this) {
    _SectionMap      = Source._SectionMap;
    _FileName        = Source._FileName;

    _PrevSection.clear();
  }
  return *this;
}
//---------------------------------------------------------------------------
bool CIniFile::operator==(const CIniFile& Source) const
{
  return _SectionMap == Source._SectionMap;
}
//---------------------------------------------------------------------------
bool CIniFile::operator!=(const CIniFile& Source) const
{
  return _SectionMap != Source._SectionMap;
}
//---------------------------------------------------------------------------
CIniFile& CIniFile::operator+=(const CIniFile& Source)
{
  CSectionMap::const_iterator srcEnd = Source._SectionMap.end();

  for (CSectionMap::const_iterator src = Source._SectionMap.begin();
        src != srcEnd;
        ++src) {
    CSectionMap::iterator dest = _SectionMap.find(src->first);

    if (dest != _SectionMap.end()) {
      CKeyMap::const_iterator itKeyEnd = src->second.end();
      CKeyMap& keyMap = dest->second;

      for(CKeyMap::const_iterator itKey = src->second.begin();
          itKey != itKeyEnd;
          ++itKey) {
        keyMap.insert(make_pair(itKey->first, itKey->second));
      }
    }
    else  {
      _SectionMap.insert(make_pair(src->first, src->second));
    }
  }
  _PrevSection.clear();

  return *this;
}
//---------------------------------------------------------------------------
void CIniFile::Load(CIniFileLoader& IniText)
{
  _SectionMap.clear();

  wstring   sectionName;
  CKeyMap  keyMap;

  while (!IniText.End()) {
    IniText.GetItem(sectionName, L']');

    if (!IniText.End()) {
      IniText.GetKeyMap(keyMap);
      _SectionMap.insert(make_pair(sectionName, keyMap));
    }
  }
  _PrevSection.clear();
}
//---------------------------------------------------------------------------
void CIniFile::Open(const wstring& FileName, UINT FileMode)
{
  _FileName = FileName;

  if (FileMode == FILE_CREATE_NEW) {
    return;
  }
  CIniFileLoader iniText(FileName);

  Load(iniText);
}
//---------------------------------------------------------------------------
void CIniFile::Open(const wstring& FileName, UINT FileMode, UINT RandomSeed)
{
  _FileName = FileName;

  if (FileMode == FILE_CREATE_NEW) {
     return;
  }

  CIniFileLoader iniText(FileName, RandomSeed);

  Load(iniText);
}
//---------------------------------------------------------------------------
CKeyMap::const_iterator CIniFile::FindKey(const wstring& Section,
                                          const wstring& Ident) const
{
  if (_PrevSection.empty() || (Section != _PrevSection)) {
    CSectionMap::const_iterator  itSection = _SectionMap.find(Section);

    if (itSection == _SectionMap.end()) {
      wostringstream   text;
      text << L"[Ini File Error] " << _FileName
           << L"\n : Can't find [" << Section << L"].";
      throw CIniFileError(IE_NO_SECTION, text.str(), _FileName);
    }
    _PrevSection     = Section;
    _PrevSectionIter = itSection;
  }

	CKeyMap::const_iterator itKey;

	itKey = find_map_const_iter(_PrevSectionIter->second, Ident);

	if (itKey == _PrevSectionIter->second.end()) {
    wostringstream  text;
		text << L"[Ini File Error] " << _FileName
         << L"\n : Can't find \"" << Ident
         << L"\" in the [" << Section << L"]." ;
		throw CIniFileError(IE_NO_KEY, text.str(), _FileName);
  }
	return itKey;
}
//---------------------------------------------------------------------------
wstring CIniFile::ReadString(const wstring& Section, const wstring& Ident) const
{
	CKeyMap::const_iterator itKey = FindKey(Section, Ident);

  const wstring& text = itKey->second;

  if (text[0] == L'"') {
    wstring::size_type pos = text.find(L'"', 1);

    if (pos != wstring::npos)
      return text.substr(1, pos - 1);
  }

  return text;
}
//---------------------------------------------------------------------------
wstring CIniFile::ReadString(const wstring& Section,
                             const wstring& Ident,
                             const wstring& Default) const
{
  try {
    return ReadString(Section, Ident);
  }
  catch (...) {
    return Default;
  }
}
//---------------------------------------------------------------------------
void CIniFile::ReadStrings(const wstring&   Section,
                           const wstring&   Ident,
                           vector<wstring>& OutBuf) const
{
  SplitString(ReadString(Section, Ident), L" \t", OutBuf);
}
//---------------------------------------------------------------------------
void CIniFile::ReadStrings(const wstring&   Section,
                           const wstring&   Ident,
                           UINT             Count,
                           vector<wstring>& OutBuf) const
{
  ReadStrings(Section, Ident, OutBuf);

  if (OutBuf.size() < Count) {
    wostringstream  os;
    os << L"[Ini File Error] " << _FileName
       << L"\n : Actual integer count = " << OutBuf.size()
       << L", not " << Count
       << L" when Read \"" << Ident
       << L"\" in the [" << Section << L"].";
    throw CIniFileError(IE_ITEM_COUNT, os.str(), _FileName);
  }
  else if (OutBuf.size() > Count) {
    OutBuf.erase(OutBuf.begin() + Count, OutBuf.end());
  }
}
//---------------------------------------------------------------------------
int CIniFile::StringToInteger(const wstring& Text) const
{
  if (Text.empty()) {
    wostringstream  os;
    os << L"[Ini File Error] " << _FileName
       << L"\n : Can't convert empty string to integer.";
    throw CIniFileError(IE_INVALID_INTEGER, os.str(), _FileName);
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
    os << L"[Ini File Error] " << _FileName
       << L"\n : Can't convert \""
       << Text
       << L"\" to integer.";
    throw CIniFileError(IE_INVALID_INTEGER, os.str(), _FileName);
  }
  return value;
}
//---------------------------------------------------------------------------
int CIniFile::ReadInteger(const wstring& Section, const wstring& Ident) const
{
  return StringToInteger(ReadString(Section, Ident));
}
//---------------------------------------------------------------------------
int CIniFile::ReadInteger(const wstring& Section,
                          const wstring& Ident,
                          int            Default) const
{
  try {
    return ReadInteger(Section, Ident);
  }
  catch (...) {
    return Default;
  }
}
//---------------------------------------------------------------------------
int CIniFile::ReadHexInteger(const wstring& Section,
                             const wstring& Ident) const
{
  return HexStrToInt(ReadString(Section, Ident));
}
//---------------------------------------------------------------------------
int CIniFile::ReadHexInteger(const wstring& Section,
                             const wstring& Ident,
                             int            Default) const
{
  try {
    return ReadHexInteger(Section, Ident);
  }
  catch (...) {
    return Default;
  }
}
//---------------------------------------------------------------------------
void CIniFile::WriteString(const wstring& Section,
                           const wstring& Ident,
                           const wstring& Value)
{
  if (_PrevSection.empty() || (Section != _PrevSection)) {
    CSectionMap::const_iterator  itSection = _SectionMap.find(Section);

    if (itSection == _SectionMap.end()) {
      _SectionMap[Section][Ident] = Value;
      _PrevSection.clear();
      return;
    }
    _PrevSection     = Section;
    _PrevSectionIter = itSection;
  }
  const_cast<CKeyMap&>(_PrevSectionIter->second)[Ident] = Value;
}
//---------------------------------------------------------------------------
void CIniFile::WriteInteger(const wstring& Section,
                            const wstring& Ident,
                            int            Value)
{
  wostringstream  text;

  text << Value;

  WriteString(Section, Ident, text.str());
}
//---------------------------------------------------------------------------
void CIniFile::DeleteSection(const wstring& Section)
{
  if (_PrevSection.empty() || (Section != _PrevSection)) {
    CSectionMap::iterator  pos = _SectionMap.find(Section);

    if (pos != _SectionMap.end()) {
      _SectionMap.erase(pos);
    }
  }
  else {
    CSectionMap::iterator  pos(_SectionMap.begin());

    advance(pos, distance(CSectionMap::const_iterator(pos), _PrevSectionIter));

    _SectionMap.erase(pos);
  }
  _PrevSection.clear();
}
//---------------------------------------------------------------------------
void CIniFile::DeleteKey(const wstring& Section, const wstring& Key)
{
  if (_PrevSection.empty() || (Section != _PrevSection)) {
    CSectionMap::const_iterator  itSection = _SectionMap.find(Section);

    if (itSection == _SectionMap.end()) {
       return;
    }
    _PrevSection     = Section;
    _PrevSectionIter = itSection;
  }
  CKeyMap&           keyMap = const_cast<CKeyMap&>(_PrevSectionIter->second);
  CKeyMap::iterator  itKey = keyMap.find(Key);

  if (itKey == keyMap.end()) {
    return;
  }
  keyMap.erase(itKey);
}
//---------------------------------------------------------------------------
void CIniFile::UpdateFile(const wstring& FileName)
{
  CIoFile                outFile(FileName, GENERIC_WRITE, CREATE_ALWAYS);
  CSectionMap::iterator  endSectionMap = _SectionMap.end();
//#ifndef __GNUC__
  outFile.WriteByteOrderMarkOfLittleEndian();
//#endif
  for (CSectionMap::iterator itSectionMap = _SectionMap.begin();
       itSectionMap != endSectionMap;
       ++itSectionMap) {
    CKeyMap::iterator endKeyValue = itSectionMap->second.end();

    outFile.WriteString(L"[");
    outFile.WriteString(itSectionMap->first);
    outFile.WriteString(L"]\r\n");

    for (CKeyMap::iterator itKeyMap = itSectionMap->second.begin();
         itKeyMap != endKeyValue;
         ++itKeyMap) {
      outFile.WriteString(itKeyMap->first);
      outFile.WriteString(L"=");
      outFile.WriteString(itKeyMap->second);
      outFile.WriteString(L";\r\n");
    }
    outFile.WriteString(L"\r\n");
  }
}
//---------------------------------------------------------------------------
void CIniFile::UpdateFile(UINT RandomSeed)
{
  CIoFile                outFile(_FileName, GENERIC_WRITE, CREATE_ALWAYS);
  CSectionMap::iterator  endSectionMap = _SectionMap.end();
  TRandomMersenne  pattern(RandomSeed);

  outFile.WriteByteOrderMarkOfLittleEndian();

  wstring text;

  for (CSectionMap::iterator itSectionMap = _SectionMap.begin();
       itSectionMap != endSectionMap;
       ++itSectionMap) {
    CKeyMap::iterator endKeyValue = itSectionMap->second.end();

    text += L"[";
    text += itSectionMap->first;
    text += L"]\r\n";

    for (CKeyMap::iterator itKeyMap = itSectionMap->second.begin();
         itKeyMap != endKeyValue;
         ++itKeyMap) {
      text += itKeyMap->first;
      text += L"=";
      text += itKeyMap->second;
      text += L";\r\n";
    }
    text += L"\r\n";
  }

  UINT size = text.size();
  wchar_t  ch;

  for (UINT pos = 0; pos < size; pos++) {
    ch = text[ pos ];

    switch (ch) {
      case L'\r': case L'\n':
        break;
      default:
        text[ pos ]= (ch & 0xfff0) + ((ch ^ pattern.IRandom(0, 255)) & 0x000f);
        break;
    }
  }
  outFile.WriteString(text);
}
//---------------------------------------------------------------------------
void CIniFile::UpdateFile()
{
  UpdateFile(_FileName);
}
//---------------------------------------------------------------------------
const CIniFile operator+(const CIniFile& lhs, const CIniFile& rhs)
{
  return CIniFile(lhs) += rhs;
}
//---------------------------------------------------------------------------
