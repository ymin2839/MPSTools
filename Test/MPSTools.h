#pragma once

#include "atlstr.h"
#include <string>

/// <summary>
/// 
/// mp string tools namespace
/// 
/// writer : ymin
/// version : 2.0
/// version desc :
///   - change string class
///	  - data type theorem
/// 
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


		CString		u8_wstr();		// utf8 to unicode
		const char* u8_cstr();		// utf8 to ansi
		std::string to_utf8();


		CString		wstr();							// return unicode
		const char* cstr() { return c_str(); }		// return char*

		long long to_ll();

		const char* format(const char* fmt, ...);

		const char* replace_all(const char* old_str, const char* new_str);


		const char* operator=(const wchar_t* right);
		const char* operator=(const CString& right);
		const char* operator=(const long long& num);
		const bool operator==(const long long& num);
	};

	namespace strutil
	{
		// format string
		LPCTSTR			Printf(LPCTSTR fmt, ...);
		LPCSTR			Printf(LPCSTR fmt, ...);

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

	namespace file
	{
		// path
		// get path return "c:\rootdir\bin\app.exe"
		CString		GetAppPath();

		// get return app.exe
		CString		GetAppName();

		// get dir return "c:\rootdir\bin\"
		CString		GetBinDir();

		// get dir return "c:\rootdir\ini\"
		CString		GetIniDir();

		// get dir return "c:\rootdir\"
		CString		GetRootDir();

		// in "c:\dir, app.exe" >> out "c:\dir\app.exe"
		CString		GetFullPath(CString dir, CString file);

		// in "c:\dir\app.exe" >> out app.exe
		CString		GetFileNameByPath(LPCTSTR path);

		// in "c:\dir\app.exe" >> out "c:\dir\"
		CString		GetDirectoryByPath(LPCTSTR path);



		// all directory create, remove
		BOOL		CreateDirs(LPCTSTR dir_path);
		BOOL 		RemoveDirs(LPCTSTR dir_path);


		int			GetFileNameList(LPCTSTR dir, OUT CStringList& slFileName, BOOL bIncFolder = FALSE);
		// in app.exe >> c:\app.exe
		CString		GetDirByFileName(LPCTSTR file_name);




		// file info
		BOOL		GetWriteTime(CString path, OUT CTime& time);

		LONGLONG	GetFileSize(CString path);
	}

	namespace sys
	{
		// create process
		HWND  CreateProcess(CString pathProcess, CString cmdLine = L"");
		DWORD CreateProcessToPid(CString pathProcess, CString cmdLine = L"");

		// find handle
		HWND FindWindowByProcessName(CString processName);
		HWND FindWindowByProcID(DWORD processId);

		CString GetCPUName();
		CString GetOSName();

		// Clipboard
		BOOL CopyClipboard(HWND hWnd, CString strCopy);

		// registry
		CString ReadRegistryValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey);
		BOOL	WriteRegistryValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue);
		void	ReadRegistryKeyAll(HKEY hKey, CString subKey, OUT CStringList& listKeyPath);
		void	ReadRegistryValueAll(HKEY hKey, CString subKey, OUT CMapStringToString& mapValue);

		// time
		int		GetSystemDate();
		int		GetSystemTime();
		int		TimeToMinute(int nTime);
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