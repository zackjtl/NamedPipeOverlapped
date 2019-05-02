#ifndef FileErrorH
#define FileErrorH
//-----------------------------------------------------------------------------
#include <sstream>
#include <iomanip>
#include <string>
#include "Error.h"
#include "ErrorType.h"
using namespace std;
//-----------------------------------------------------------------------------
enum { ERROR_OPEN_FILE = ERROR_FILE << 24 };
//-----------------------------------------------------------------------------
class COpenFileError : public CError
{
public :
  COpenFileError(const wstring& FileName)
    : CError(ERROR_OPEN_FILE),
      _FileName(FileName)
  {
    wostringstream  os;

    os << L"[Error] : Can't open file, " << FileName << L'.';
    SetText(os.str());
  }
  ~COpenFileError() throw() {}
  wstring GetFileName() const { return _FileName; }
private :
  wstring  _FileName;
};
//-----------------------------------------------------------------------------
#endif
