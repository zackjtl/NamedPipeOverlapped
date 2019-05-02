//---------------------------------------------------------------------------
#ifndef IniFileH
#define IniFileH
//---------------------------------------------------------------------------
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "Error.h"
#include "ErrorType.h"
////#include "BaseTypes.h"
#include "TypeConv.h"
using namespace std;
//---------------------------------------------------------------------------
enum {
  IE_SYNTAX = ERROR_INI_FILE << 24, IE_NO_SECTION, IE_NO_KEY, IE_INVALID_INTEGER,
  IE_ITEM_COUNT
};
enum {
   FILE_OPEN_READ_WRITE = 0, FILE_CREATE_NEW
};
//---------------------------------------------------------------------------
class CIniFileError : public CError
{
public :
   CIniFileError(UINT Code, const wstring& Text, const wstring& FileName)
      : CError(Code, Text),
        _FileName(FileName)
   {
   }
   ~CIniFileError() throw() {}
   wstring GetFileName() const { return _FileName; }
private :
   wstring  _FileName;
};
//---------------------------------------------------------------------------
template<class T>
struct DisableCompare :public std::binary_function<T,T,bool>
{
  bool operator()(T lhs,T rhs)  const
  {
    return true;
  }
};
//---------------------------------------------------------------------------
typedef map<wstring, wstring>           CKeyMap;
typedef map<wstring, CKeyMap>   				CSectionMap;
//---------------------------------------------------------------------------
class CIniFileLoader
{
public :
	CIniFileLoader();
  CIniFileLoader(const wstring& FileName);
  CIniFileLoader(const wstring& FileName, UINT RandomSeed);

  bool  End() { return _Position < _Text.size() ? false : true; }
  void  GetPositionOfComment();

  wstring::size_type Find(wchar_t Ch, wstring::size_type Index);
  wstring::size_type FindFirstNotOf(const wchar_t* Str, wstring::size_type Index);

  void  GetItem(wstring& Term, wchar_t EndToken);
  void  GetKeyMap(CKeyMap& KeyMap);

  wstring   GetText() {return _Text; }
  string   GetTextC() {return _TextC; }
protected :
  string               _TextC;
  wstring              _Text;
  const wstring&       _FileName;
  wstring::size_type   _Position;
  wstring::size_type   _StartOfComment;
  wstring::size_type   _EndOfComment;
};
//---------------------------------------------------------------------------
class CIniFile
{
public:
  CIniFile() {}
  CIniFile(const wstring& FileName, UINT FileMode = FILE_OPEN_READ_WRITE)
  {
    Open(FileName, FileMode);
  }
  CIniFile(UINT RandomSeed, const wstring& FileName, UINT FileMode = FILE_OPEN_READ_WRITE)
  {
    Open(FileName, FileMode, RandomSeed);
  }
  CIniFile(const CIniFile& Source);

  CIniFile& operator=(const CIniFile& Source);
  CIniFile& operator+=(const CIniFile& Source);
  bool      operator==(const CIniFile& Source) const;
  bool      operator!=(const CIniFile& Source) const;
  void    Open(const wstring& FileName, UINT FileMode);
  void    Open(const wstring& FileName, UINT FileMode, UINT RandomSeed);
  void    SetFileName(const wstring& Value) { _FileName = Value; }
  wstring GetFileName() const { return _FileName; }
  void    Clear() { _SectionMap.clear(); }
  wstring ReadString(const wstring& Section, const wstring& Ident) const;
  wstring ReadString(const wstring& Section, const wstring& Ident, const wstring& Default) const;
  void   ReadStrings(const wstring&   Section,
                     const wstring&   Ident,
                     vector<wstring>& OutBuf) const;
  void   ReadStrings(const wstring&   Section,
                     const wstring&   Ident,
                     UINT             Count,
                     vector<wstring>& OutBuf) const;
  int    StringToInteger(const wstring& Text) const;
  int    ReadInteger(const wstring& Section, const wstring& Ident) const;
  int    ReadInteger(const wstring& Section, const wstring& Ident, int Default) const;
  int    ReadHexInteger(const wstring& Section, const wstring& Ident) const;
  int    ReadHexInteger(const wstring& Section, const wstring& Ident, int Default) const;
  void   WriteString(const wstring& Section, const wstring& Ident, const wstring& Value);
  void   WriteInteger(const wstring& Section, const wstring& Ident, int Value);
  template <class T>
  void ReadIntegers(const wstring& Section, const wstring& Ident, vector<T>& Integers) const
  {
    CKeyMap::const_iterator itKey = FindKey(Section, Ident);

    StrToInts(itKey->second, Integers);
  }
  template <class T>
  void ReadIntegers(const wstring& Section,
                    const wstring& Ident,
                    UINT           Count,
                    vector<T>&     Integers) const
  {
    ReadIntegers(Section, Ident, Integers);

    if (Integers.size() < Count) {
      wostringstream  os;
      os << L"[Ini File Error] " << _FileName
         << L" : Actual integer count = " << Integers.size()
         << L", not " << Count
         << L" when Read \"" << Ident
         << L"\" in the [" << Section << L"].";
      throw CError(IE_ITEM_COUNT, os.str());
    }
    else if (Integers.size() > Count) {
      Integers.erase(Integers.begin() + Count, Integers.end());
    }
  }
  template <class T>
  void ReadRegExInts(const wstring& Section, const wstring& Ident, vector<T>& Integers) const
  {
    CKeyMap::const_iterator itKey = FindKey(Section, Ident);

    RegExStrToInts(itKey->second, Integers);
  }
  template <class T>
  void ReadRegExInts(const wstring& Section,
                     const wstring& Ident,
                     UINT           Count,
                     vector<T>&     Integers) const
  {
    ReadRegExInts(Section, Ident, Integers);

    if (Integers.size() < Count) {
      wostringstream  os;
      os << L"[Ini File Error] " << _FileName
         << L" : Actual integer count = " << Integers.size()
         << L", not " << Count
         << L" when Read \"" << Ident
         << L"\" in the [" << Section << L"].";
      throw CError(IE_ITEM_COUNT, os.str());
    }
    else if (Integers.size() > Count) {
      Integers.erase(Integers.begin() + Count, Integers.end());
    }
  }
  template <class T>
  void ReadHexInts(const wstring& Section, const wstring& Ident, vector<T>& Integers) const
  {
    CKeyMap::const_iterator itKey = FindKey(Section, Ident);

    HexStrToInts(itKey->second, Integers);
  }
  template <class T>
  void ReadHexInts(const wstring& Section,
                   const wstring& Ident,
                   UINT           Count,
                   vector<T>&     Integers) const
  {
    ReadHexInts(Section, Ident, Integers);

    if (Integers.size() < Count) {
      wostringstream  os;
      os << L"[Ini File Error] " << _FileName
         << L" : Actual integer count = " << Integers.size()
         << L", not " << Count
         << L" when Read \"" << Ident
         << L"\" in the [" << Section << L"].";
      throw CError(IE_ITEM_COUNT, os.str());
    }
    else if (Integers.size() > Count) {
      Integers.erase(Integers.begin() + Count, Integers.end());
    }
  }
  template <class T>
  void ReadHexRegExInts(const wstring& Section, const wstring& Ident, vector<T>& Integers) const
  {
    CKeyMap::const_iterator itKey = FindKey(Section, Ident);

    RegExHexStrToInts(itKey->second, Integers);
  }

  template <class T>
  void ReadHexRegExInts(const wstring& Section,
                        const wstring& Ident,
                        UINT           Count,
                        vector<T>&    Integers) const
  {
    ReadHexRegExInts(Section, Ident, Integers);

    if (Integers.size() < Count) {
      wostringstream  os;
      os << L"[Ini File Error] " << _FileName
         << L" : Actual integer count = " << Integers.size()
         << L", not " << Count
         << L" when Read \"" << Ident
         << L"\" in the [" << Section << L"].";
      throw CError(IE_ITEM_COUNT, os.str());
    }
    else if (Integers.size() > Count) {
      Integers.erase(Integers.begin() + Count, Integers.end());
    }
  }
  template <class T>
  void WriteIntegers(const wstring& Section, const wstring& Ident, vector<T>& Integers)
  {
    CKeyMap&       keyValue = _SectionMap[Section];
    wostringstream  text;

    for (UINT idx = 0; idx < Integers.size(); ++idx) {
      text << (int)Integers[idx];

      if ((idx + 1) < Integers.size()) {
        text << L",";
      }
    }
    keyValue[Ident] = text.str();
  }
  template <class T>
  void WriteHexIntegers(const wstring& Section, const wstring& Ident, vector<T>& Integers, UINT Digit)
  {
    CKeyMap&       keyValue = _SectionMap[Section];
    wostringstream  text;

    for (UINT idx = 0; idx < Integers.size(); ++idx) {

      text << hex << setw(Digit) << setfill(L'0') << (int)Integers[idx];

      if ((idx + 1) < Integers.size()) {
        text << L",";
      }
    }
    keyValue[Ident] = text.str();
  }
  void DeleteSection(const wstring& SectionName);
  void DeleteKey(const wstring& Section, const wstring& Key);
  CKeyMap& GetKeyValue(const wstring& Section) { return _SectionMap[Section]; }
  void UpdateFile(const wstring& FileName);
  void UpdateFile(UINT RandomSeed);
  void UpdateFile();
  CSectionMap& GetSectionMap() { return _SectionMap; }
protected:
  void Load(CIniFileLoader& IniText);
  CKeyMap::const_iterator FindKey(const wstring& Section, const wstring& Ident) const;
protected:
  CSectionMap                          _SectionMap;
  wstring                              _FileName;
  mutable wstring                      _PrevSection;
  mutable CSectionMap::const_iterator  _PrevSectionIter;
};
//---------------------------------------------------------------------------
const CIniFile operator+(const CIniFile& lhs, const CIniFile& rhs);
//---------------------------------------------------------------------------
#endif
