#pragma once
//-----------------------------------------------------------------------------
#include <string>
#include <exception>
#include <minwindef.h>
//-----------------------------------------------------------------------------
using namespace std;
//-----------------------------------------------------------------------------
class CError
{
public:
	CError() throw()
		: _Code(0)
	{
	}
	CError(UINT Code) throw()
		: _Code(Code)
	{
	}
	CError(const wstring& Text) throw()
		: _Code(-1),
		_Text(Text)
	{
	}
	CError(UINT Code, const wstring& Text) throw()
		: _Code(Code),
		_Text(Text)
	{
	}
	CError(const CError& rhs) throw()
		: _Code(rhs._Code),
		_Text(rhs._Text)
	{
	}
	virtual ~CError() throw()
	{
	}
	CError& operator=(const CError& rhs) throw()
	{
		if (&rhs != this) {
			_Code = rhs._Code;
			_Text = rhs._Text;
		}
		return *this;
	}
	virtual const wchar_t* what() const throw()
	{
		return _Text.c_str();
	}
	void    SetCode(UINT Value) { _Code = Value; }
	void    SetText(const wstring& Value) { _Text = Value; }
	void    AppendText(const wstring& Value) { _Text += Value; }
	UINT    GetCode() const { return _Code; }
	wstring GetText() const { return _Text; }
protected:
	UINT      _Code;
	wstring   _Text;
};
//-----------------------------------------------------------------------------
