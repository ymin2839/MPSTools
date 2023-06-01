/// <summary>
/// 
/// mp string tools namespace
/// 
/// writer : ymin
/// version : 1.6
/// version desc :
///   - change string class
///	  - data type theorem
///   - delete file,sys namespace
///   - name chage strutil >> util
///   - util::printf return type change
/// 
/// </summary>

#pragma once

#include "atlstr.h"
#include <string>

namespace mps
{
	class string : public std::string
	{
	public:
		string() {}
		string(const char* src);
		string(const wchar_t* src);
		string(const double& num);
		string(const std::string& src);


		CStringW	u8_wstr();		// utf8 to unicode
		const char* u8_cstr();		// utf8 to ansi
		std::string to_utf8();

		CStringW	wstr();							// return unicode
		const char* cstr() { return c_str(); }		// return char*

		double to_ll();

		const char* format(const char* fmt, ...);

		const char* replace_all(const char* old_str, const char* new_str);


		const char* operator=(const wchar_t* right);
		const char* operator=(const CStringW& right);
		const char* operator=(const double& num);
		const bool operator==(const double& num);
	};

	namespace util
	{
		// format string
		CStringW		Printf(LPCTSTR fmt, ...);
		CStringA		Printf(LPCSTR fmt, ...);

		// convert string
		std::string		WtA(LPCTSTR wstr);	// convert unicode->string
		CStringW		AtW(LPCSTR astr);	// convert string->unicode
		CStringW		U8tW(LPCSTR u8str);	// convert utf8->CStringW
		std::string		U8tA(LPCSTR u8str);	// convert utf8->string
		std::string		WtU8(LPCTSTR wstr);	// convert unicode->utf8

		// string parser
		int				StringParser(IN LPCTSTR src, OUT CStringList& sl, LPCTSTR key = L";");
		int				StringParser(IN LPCTSTR src, OUT CStringArray& sa, LPCTSTR key = L";");

		// number to string
		CString			Num2Str(LONGLONG llNumber, LPCTSTR strSeperator = _T(","));	// number to string
		CString			Flt2Str(double dNumber, BYTE cDecimal = 0, LPCTSTR strSeperator = _T(","));
	}
	
#if 0
	class cstring : public CString
	{
	public:
		cstring() {}
		cstring(LPCTSTR src);
		cstring(LPCSTR src);
		LPCSTR c_str();

		LPCTSTR operator=(LPCSTR right);
		LPCTSTR operator=(std::string right);
	};
#endif
}