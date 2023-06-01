#pragma once

#include "atlstr.h"
#include <string>

/// <summary>
/// 
/// mp string tools namespace
/// 
/// writer : ymin
/// version : 2.6
/// version desc :
///   - change string class
///	  - data type theorem
///   - delete file,sys namespace
///   - change name is strutil to util
///   - util::printf return type change : std::string to CStringA
/// </summary>


namespace mps
{
	class string : public std::string
	{
	public:
		string() {}
		string(const char* src);
		string(const wchar_t* src);
		string(const long long& num);
		string(const std::string& src);


		CStringW	utf8_to_wstr();		// utf8 to unicode
		const char* utf8_to_cstr();		// utf8 to ansi
		const char* to_utf8();


		CStringW	wstr();							// return unicode
		const char* cstr() { return c_str(); }		// return char*

		long long to_ll();

		string format(const char* fmt, ...);
		string format(const wchar_t* fmt, ...);

		string replace_all(const char* old_str, const char* new_str);
		string ltrim();
		string rtrim();
		string trim();

		const char* operator=(const wchar_t* right);
		const char* operator=(const CStringW& right);
		const char* operator=(const long long& num);
		const bool operator==(const long long& num);
	};

	namespace util
	{
		// format string
		CStringW		Printf(LPCTSTR fmt, ...);
		CStringA		Printf(LPCSTR fmt, ...);

		// convert string
		CStringA		WtA(LPCTSTR wstr);	// convert unicode->string
		CStringW		AtW(LPCSTR astr);	// convert string->unicode
		CStringW		U8tW(LPCSTR u8str);	// convert utf8->CStringW
		CStringA		U8tA(LPCSTR u8str);	// convert utf8->string
		CStringA		WtU8(LPCTSTR wstr);	// convert unicode->utf8

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