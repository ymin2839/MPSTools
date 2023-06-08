#pragma once

#include "atlstr.h"
#include <string>

/// <summary>
/// 
/// mp string tools namespace
/// 
/// writer : ymin
/// version : 2.7
/// version desc :
/// - simple utf8_to_str func name u8_utf
/// - constructor for utf8
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

		// input string to ansi
		// ex string(utf8_c_str(), CP_UTF8) save memory to ansi string
		string(const std::string& src, int src_codepage = CP_UTF8);


		CStringW		utf8_to_wstr();		// utf8 to unicode
		std::string		utf8_to_cstr();		// utf8 to ansi		
		std::string		to_utf8str();
		
		CStringW		u8_wstr() { return utf8_to_wstr(); }	// simple utf8 to unicode
		std::string		u8_cstr() { return utf8_to_cstr(); }	// simple utf8 to ansi

		std::string		SetUtf8String(const char* src);


		CStringW		wstr();							// return unicode
		const char*		cstr() { return c_str(); }		// return char*

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
}