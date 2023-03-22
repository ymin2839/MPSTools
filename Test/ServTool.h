#pragma once

#include <string>

// 
// mpserver namespace name
// writter : ymin
// ver 1
//
namespace mps
{
	class cstring : public CString
	{
	public:
		cstring() {}
		cstring(LPCTSTR src);
		LPCSTR c_str();

		LPCTSTR operator=(LPCSTR right);
		LPCTSTR operator=(std::string right);
	};

	namespace strutil
	{
		// format string
		CStringW		Printf(LPCTSTR fmt, ...);
		std::string		Printf(LPCSTR fmt, ...);

		// convert string
		std::string		WtA(LPCTSTR wstr);	// convert CStringW->string
		CStringW		AtW(LPCSTR astr);	// convert string->CStringW
		CStringW		U8tW(LPCSTR u8str);	// convert utf8->CStringW
		std::string		WtU8(LPCTSTR wstr);	// convert CStringW->utf8

		// string parser
		BOOL			StringParser(IN LPCTSTR src, OUT CStringList& sl, LPCTSTR key = L";");

		// number to string
		CString			Num2Str(LONGLONG llNumber, LPCTSTR strSeperator = _T(","));	// number to string
		CString			Flt2Str(double dNumber, BYTE cDecimal = 0, LPCTSTR strSeperator = _T(","));
	}

	namespace file
	{
		// get path return "c:\dir"
		CString		GetCurrPath();

		// get return app.exe
		CString		GetCurrAppName();

		// get dir return "c:\dir\"
		CString		GetCurrDirectory();

		// in "c:\dir\app.exe" >> out app.exe
		CString		GetFileNameByPath(LPCTSTR path);

		// in "c:\dir\app.exe" >> out "c:\dir\"
		CString		GetDirectoryByPath(LPCTSTR path);

		// all directory create, remove
		BOOL		CreateDirs(LPCTSTR path);
		BOOL 		RemoveDirs(LPCTSTR path);


		int			GetFileNameList(LPCTSTR dir, OUT CStringList& slFileName, BOOL bIncFolder = FALSE);

		CString		GetDirByProcName(LPCTSTR processName);
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
	}
}