#include "stdafx.h"
#include "MPSTools.h"
#include "atltime.h"
#include <memory>

#define MAX_PATH_LEN	260

namespace mps
{
	string::string(const char* str)
		: std::string(str)
	{
	}

	string::string(const wchar_t* wstr)
	{
		const int len(WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL));
		if (len > 0)
		{
			auto buffer = std::make_unique<char[]>(len + 1);

			WideCharToMultiByte(CP_ACP, 0, wstr, -1, buffer.get(), len, NULL, NULL);

			clear();
			append(buffer.get());
		}
	}

	string::string(const long long& num)
	{
		format("%I64d", num);
	}

	string::string(const std::string& src)
	{
		*this = src.c_str();
	}

	CString string::u8_wstr()
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, c_str(), -1, 0, 0);
		if (len > 0)
		{
			auto buffer = std::make_unique<TCHAR[]>(len + 1);

			MultiByteToWideChar(CP_UTF8, 0, c_str(), -1, buffer.get(), len);

			return CStringW(buffer.get());
		}

		return L"";
	}

	const char* string::u8_cstr()
	{
		std::wstring wstr = u8_wstr().operator LPCWSTR();

		int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
		if (len > 0)
		{
			auto buffer = std::make_unique<char[]>(len + 1);

			WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, buffer.get(), len, NULL, NULL);

			clear();
			append(buffer.get());
		}

		return c_str();
	}

	CString string::wstr()
	{
		int len = MultiByteToWideChar(CP_ACP, 0, c_str(), -1, NULL, NULL);
		if (len > 0)
		{
			auto buffer = std::make_unique<wchar_t[]>(len + 1);

			MultiByteToWideChar(CP_ACP, 0, c_str(), -1, buffer.get(), len);

			return CString(buffer.get());
		}

		return L"";
	}

	const char* string::format(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);

		int len = _vscprintf(fmt, args); // terminating '\0'
		if (len > 0)
		{
			auto buffer = std::make_unique<char[]>(len + 1);

			vsprintf_s(buffer.get(), len, fmt, args);

			clear();
			append(buffer.get());
		}

		va_end(args);

		return c_str();
	}

	const char* string::replace_all(const char* old_str, const char* new_str)
	{
		size_t pos = 0;
		while ((pos = find(old_str, pos)) != std::string::npos) {
			erase(pos, strlen(old_str));
			insert(pos, new_str);
			pos++;
		}
		return c_str();
	}

	long long string::to_ll()
	{
		return atoll(c_str());
	}

	const char* string::operator=(const wchar_t* right)
	{
		int len = WideCharToMultiByte(CP_ACP, 0, right, -1, NULL, 0, NULL, NULL);
		if (len > 0)
		{
			auto buffer = std::make_unique<char[]>(len + 1);

			WideCharToMultiByte(CP_ACP, 0, right, -1, buffer.get(), len, NULL, NULL);

			clear();
			append(buffer.get());
		}

		return c_str();
	}

	const char* string::operator=(const long long& num)
	{
		return this->format("%I64d", num);
	}

	const bool string::operator==(const long long& num)
	{
		return atoll(c_str()) == num ? true : false;
	}

	namespace strutil {
		LPCSTR Printf(LPCSTR format, ...)
		{
			std::string rt;

			va_list args;
			va_start(args, format);

			int len = _vscprintf(format, args); // terminating '\0'
			if (len > 0)
			{
				auto buffer = std::make_unique<char[]>(len + 1);

				vsprintf_s(buffer.get(), len, format, args);

				rt.append(buffer.get(), len);
			}

			va_end(args);

			return CStringA(rt.c_str());
		}

		LPCTSTR Printf(LPCTSTR format, ...)
		{
			CStringW rt;

			va_list args;
			va_start(args, format);

			int len = _vscwprintf(format, args) + 1;
			if (len > 0)
			{
				auto buffer = std::make_unique<TCHAR[]>(len + 1);

				vswprintf_s(buffer.get(), len, format, args);

				rt.Append(buffer.get());
			}

			va_end(args);

			return rt;
		}

		std::string WtA(LPCTSTR wstr)
		{
			int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
			if (len > 0)
			{
				auto buffer = std::make_unique<char[]>(len + 1);

				WideCharToMultiByte(CP_ACP, 0, wstr, -1, buffer.get(), len, NULL, NULL);

				return std::string(buffer.release());
			}

			return "";
		}

		CStringW AtW(LPCSTR astr)
		{
			int len = MultiByteToWideChar(CP_ACP, 0, astr, -1, NULL, NULL);
			if (len > 0)
			{
				auto buffer = std::make_unique<TCHAR[]>(len + 1);

				MultiByteToWideChar(CP_ACP, 0, astr, -1, buffer.get(), len);

				return CStringW(buffer.release());
			}

			return _T("");
		}

		CStringW U8tW(LPCSTR u8str)
		{
			int len = MultiByteToWideChar(CP_UTF8, 0, u8str, -1, 0, 0);
			if (len > 0)
			{
				auto buffer = std::make_unique<TCHAR[]>(len + 1);

				MultiByteToWideChar(CP_UTF8, 0, u8str, -1, buffer.get(), len);

				return CStringW(buffer.release());
			}

			return _T("");
		}

		std::string U8tA(LPCSTR u8str)
		{
			return WtA(U8tW(u8str));
		}

		std::string WtU8(LPCTSTR wstr)
		{
			int len = WideCharToMultiByte(CP_UTF8, 0, wstr, lstrlen(wstr), NULL, 0, NULL, NULL);
			if (len)
			{
				auto buffer = std::make_unique<char[]>(len + 1);

				WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer.get(), len, NULL, NULL);

				return std::string(buffer.release());
			}

			return "";
		}

		int StringParser(IN LPCTSTR data_src, OUT CStringList& sl, LPCTSTR key /*= L";"*/)
		{
			CString src = data_src;
			int posPrev = 0;
			int pos = src.Find(key);
			while (pos > -1)
			{
				sl.AddTail(src.Mid(posPrev, pos - posPrev));

				posPrev = pos + 1;

				pos = src.Find(key, posPrev);
			}

			if (src.GetLength() > posPrev)
			{
				sl.AddTail(src.Mid(posPrev));
			}

			return (int)sl.GetCount();
		}

		int StringParser(IN LPCTSTR data_src, OUT CStringArray& sa, LPCTSTR key /*= L";"*/)
		{
			CString src = data_src;
			int posPrev = 0;
			int pos = src.Find(key);
			while (pos > -1)
			{
				sa.Add(src.Mid(posPrev, pos - posPrev));

				posPrev = pos + 1;

				pos = src.Find(key, posPrev);
			}

			if (src.GetLength() > posPrev)
			{
				sa.Add(src.Mid(posPrev));
			}

			return (int)sa.GetCount();
		}

		CString Num2Str(LONGLONG llNumber, LPCTSTR strSeperator /* = _T(",") */)
		{
			CString	strNumber, strFormat;
			strNumber.Format(_T("%I64d"), llNumber);

			if (llNumber < 1000 && llNumber > -1000)
				return strNumber;

			if (lstrlen(strSeperator) == 0)
				return strNumber;

			BYTE cMinusFlag = 0;
			if (llNumber < 0)
				cMinusFlag = 1;

			int	nLength = strNumber.GetLength();
			strNumber.Insert(nLength - 3, strSeperator);
			nLength -= 3;
			while (nLength > 3 + cMinusFlag)
			{
				strNumber.Insert(nLength - 3, strSeperator);
				nLength -= 3;
			}

			return strNumber;
		}

		CString Flt2Str(double dNumber, BYTE cDecimal /* = 0 */, LPCTSTR strSeperator /* = _T(",") */)
		{
			CString	strNumber, strDecimal;

			strDecimal.Format(_T("%%.%df"), cDecimal);
			strNumber.Format(strDecimal, dNumber);

			if (dNumber < 1000.0f && dNumber > -1000.0f)
				return strNumber;

			if (lstrlen(strSeperator) == 0)
				return strNumber;

			BYTE cMinusFlag = 0;
			if (dNumber < 0.0f)
				cMinusFlag = 1;

			if (cMinusFlag == 1)
				strNumber.Delete(0);

			int nLength = strNumber.GetLength() - cDecimal - (cDecimal == 0 ? 0 : 1);
			CString	strFormat = strNumber.Left(nLength);

			strFormat.Insert(nLength - 3, strSeperator);
			nLength -= 3;
			while (nLength > 3)
			{
				strFormat.Insert(nLength - 3, strSeperator);
				nLength -= 3;
			}

			if (cDecimal != 0)
			{
				strNumber = strFormat + strNumber.Right(cDecimal + 1);
			}
			else
			{
				strNumber = strFormat;
			}

			if (cMinusFlag == 1)
			{
				strNumber = _T("-") + strNumber;
			}

			return strNumber;
		}
	}


	namespace file {

		CString GetAppPath()
		{
			TCHAR szPath[MAX_PATH_LEN] = L"";
			GetModuleFileNameW(NULL, szPath, MAX_PATH_LEN);

			CString strPath(szPath);

			return strPath;
		}

		CString GetAppName()
		{
			TCHAR szPath[MAX_PATH_LEN] = L"";
			GetModuleFileNameW(NULL, szPath, MAX_PATH_LEN);

			CString strPath(szPath);
			strPath = strPath.Mid(strPath.ReverseFind(L'\\') + 1);
			return strPath;
		}

		CString GetBinDir()
		{
			TCHAR szPath[MAX_PATH_LEN] = L"";
			GetModuleFileNameW(NULL, szPath, MAX_PATH_LEN);

			CString strPath(szPath);
			strPath = strPath.Left(strPath.ReverseFind(L'\\') + 1);

			return strPath;
		}

		CString GetIniDir()
		{
			TCHAR szPath[MAX_PATH_LEN] = L"";
			GetModuleFileNameW(NULL, szPath, MAX_PATH_LEN);

			CString strPath(szPath);
			strPath = strPath.Left(strPath.ReverseFind(L'\\'));
			strPath = strPath.Left(strPath.ReverseFind(L'\\') + 1);

			return GetRootDir() + _T("ini\\");
		}

		CString GetRootDir()
		{
			TCHAR szPath[MAX_PATH_LEN] = L"";
			GetModuleFileNameW(NULL, szPath, MAX_PATH_LEN);

			CString strPath(szPath);
			strPath = strPath.Left(strPath.ReverseFind(L'\\'));
			strPath = strPath.Left(strPath.ReverseFind(L'\\') + 1);

			return strPath;
		}

		CString GetFileNameByPath(LPCTSTR path_src)
		{
			CString path(path_src);
			// file style
			{
				int pos = path.ReverseFind('\\');
				if (pos > -1)
				{
					return path.Mid(pos + 1);
				}
			}

			// url style
			{
				int pos = path.ReverseFind('/');
				if (pos > -1)
				{
					return path.Mid(pos + 1);
				}
			}

			return L"";
		}

		CString GetDirectoryByPath(LPCTSTR path_stc)
		{
			CString path(path_stc);

			int pos = path.ReverseFind('\\');
			if (pos > -1)
			{
				return path.Left(pos + 1);
			}

			return L"";
		}

		BOOL CreateDirs(LPCTSTR path_src)
		{
			CString path(path_src);

			{
				int posLastDir = path.ReverseFind(L'\\');
				int posFile = path.ReverseFind(L'.');
				if (posLastDir < posFile && (posLastDir > -1 && posFile > -1))
				{
					path = path.Mid(0, posLastDir + 1);
				}
			}


			CString csPrefix;

			int nStart = 0;
			int nEnd = 0;

			while ((nEnd = path.Find('\\', nStart)) >= 0)
			{
				CString csToken = path.Mid(nStart, nEnd - nStart);
				CreateDirectory(csPrefix + csToken, NULL);

				csPrefix += csToken;
				csPrefix += _T("\\");
				nStart = nEnd + 1;
			}

			CString csToken = path.Mid(nStart);

			CreateDirectory(csPrefix + csToken, NULL);

			return TRUE;
		}

		BOOL RemoveDirs(LPCTSTR path_src)
		{
			CString rootDir(path_src);

			CFileFind find;

			BOOL vaild = find.FindFile(rootDir + _T("*.*"));
			while (vaild)
			{
				vaild = find.FindNextFile();

				if (find.IsDots() == FALSE)
				{
					if (find.IsDirectory())
					{
						RemoveDirs(find.GetFilePath() + L"\\");
					}
					else
					{
						vaild = DeleteFile(find.GetFilePath());
					}
				}
			}

			find.Close();

			return RemoveDirectory(rootDir);
		}

		int GetFileNameList(LPCTSTR dir_src, OUT CStringList& slFileName, BOOL bIncFolder /*= FALSE*/)
		{
			CString path(dir_src);
			path.Append(L"*.*");

			CFileFind finder;

			BOOL bWorking = finder.FindFile(path);
			while (bWorking)
			{
				bWorking = finder.FindNextFile();

				// file check
				if (finder.IsArchived())
				{
					CString fileNameFind = finder.GetFileName();

					if (finder.IsDots() || fileNameFind == _T("Thumbs.db"))
						continue;

					CString fileName = finder.GetFileName();

					slFileName.AddTail(fileName);
				}

				// directory check
				if (bIncFolder && finder.IsDirectory())
				{
					CString dirName = finder.GetFileName();

					slFileName.AddTail(dirName);
				}
			}

			return (int)slFileName.GetCount();
		}

		CString GetDirByFileName(LPCTSTR file_name)
		{
			CString procName(file_name);

			procName.MakeLower();
			if (procName.Find(L".exe") < 0)
			{
				procName.Append(L".exe");
			}

			TCHAR szName[MAX_PATH_LEN]{};
			TCHAR szPath[MAX_PATH_LEN]{};

			HWND hwnd = FindWindowEx(NULL, NULL, NULL, NULL);
			while (hwnd != NULL)
			{
				//if (IsWindow(hwnd) && IsWindowVisible((hwnd)))
				{
					GetWindowText(hwnd, szName, MAX_PATH_LEN);

					DWORD dwProcessId;
					GetWindowThreadProcessId(hwnd, &dwProcessId);
					HANDLE hHandle = OpenProcess(MAXIMUM_ALLOWED, FALSE, dwProcessId);
					if (hHandle)
					{
						::GetModuleFileName((HMODULE)hHandle, szPath, MAX_PATH_LEN);

						if (wcslen(szName))
						{
							CString procComp = szPath;
							int pos = procComp.ReverseFind(L'\\');
							if (pos > -1)
							{
								procComp = procComp.Mid(procComp.ReverseFind(L'\\') + 1);

								procComp.MakeLower();

								if (procComp == procName)
								{
									CString procDir = szPath;
									int pos = procDir.ReverseFind(L'\\') + 1;
									if (pos > 0)
									{
										procDir = procDir.Mid(0, pos);
										return procDir;
									}
								}
							}
						}
						CloseHandle(hHandle);
					}
				}

				hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
			}

			return L"";
		}

		CString	GetFullPath(CString dir, CString file)
		{
			int pos = dir.ReverseFind(L'\\');
			if (pos != dir.GetLength() - 1)
			{
				dir.AppendChar(L'\\');
			}

			return dir + file;
		}

		BOOL GetWriteTime(CString path, OUT CTime& time)
		{
			CFileFind finder;
			if (finder.FindFile(path))
			{
				finder.FindNextFile();

				finder.GetLastWriteTime(time);

				return TRUE;
			}

			return FALSE;
		}

		LONGLONG GetFileSize(CString path)
		{
			PLARGE_INTEGER pLint = nullptr;

			HANDLE hFile = ::CreateFile(path, 0, 0, NULL, OPEN_EXISTING, 0, NULL);
			if (INVALID_FILE_SIZE != ::GetFileSizeEx(hFile, pLint))
			{
				// 64bit인 경우
				//printf("64bit file size = %d\n", pLargeInteger.QuadPart);

				// 32bit인 경우
				//printf("32bit file size = %d\n", pLargeInteger.LowPart);

				return pLint->LowPart;
			}

			return -1;
		}
	}

	namespace sys {

		HWND FindWindowByProcessName(CString processName)
		{
			processName.MakeLower();
			if (processName.Find(L".exe") < 0)
			{
				processName.Append(L".exe");
			}

			TCHAR szName[MAX_PATH_LEN]{};
			TCHAR szPath[MAX_PATH_LEN]{};

			HWND hwnd = FindWindowEx(NULL, NULL, NULL, NULL);
			while (hwnd != NULL)
			{
				//if (IsWindow(hwnd) && IsWindowVisible((hwnd)))
				{
					GetWindowText(hwnd, szName, MAX_PATH_LEN);

					DWORD dwProcessId;
					GetWindowThreadProcessId(hwnd, &dwProcessId);
					HANDLE hHandle = OpenProcess(MAXIMUM_ALLOWED, FALSE, dwProcessId);
					if (hHandle)
					{
						::GetModuleFileName((HMODULE)hHandle, szPath, MAX_PATH_LEN);

						if (wcslen(szName))
						{
							CString procComp = szPath;
							int pos = procComp.ReverseFind(L'\\');
							if (pos > -1)
							{
								procComp = procComp.Mid(procComp.ReverseFind(L'\\') + 1);

								procComp.MakeLower();

								if (procComp == processName)
								{
									return hwnd;
								}
							}
						}
						CloseHandle(hHandle);
					}
				}
				hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
			}

			return NULL;
		}

		HWND FindWindow(LPCTSTR windowTitle)
		{
			HWND hWnd = ::FindWindow(NULL, windowTitle);
			if (hWnd)
			{
				return hWnd;
			}

			return NULL;
		}

		DWORD CreateProcessToPid(CString pathProcess, CString cmdLine/*=L""*/)
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(STARTUPINFO));

			if (CreateProcess(pathProcess, (LPWSTR)(LPCTSTR)cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				DWORD dwProcessId = pi.dwProcessId;

				return dwProcessId;
			}

			return 0;
		}

		HWND FindWindowByProcID(DWORD dwProcessId)
		{
			DWORD dwProcessIdFind = 0;
			HWND hwndFind = ::FindWindow(NULL, NULL);

			while (hwndFind != NULL)
			{
				if (GetParent(hwndFind) == NULL)
				{
					GetWindowThreadProcessId(hwndFind, &dwProcessIdFind);
					if (dwProcessId == dwProcessIdFind)
					{
						return hwndFind;
					}
				}

				hwndFind = GetWindow(hwndFind, GW_HWNDNEXT);
			}

			return NULL;
		}

		HWND CreateProcess(CString pathProcess, CString cmdLine/*=L""*/)
		{
			STARTUPINFO si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(STARTUPINFO));

			if (CreateProcess(pathProcess, (LPWSTR)(LPCTSTR)cmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
			{
				DWORD dwProcessId = pi.dwProcessId;

				return FindWindowByProcID(dwProcessId);
			}

			return NULL;
		}

		BOOL CopyClipboard(HWND hWnd, CString strCopy)
		{
			USES_CONVERSION_EX;

			CStringA strText = W2A_EX(strCopy, strCopy.GetLength());

			if (!OpenClipboard(hWnd))
			{
				return FALSE;
			}

			EmptyClipboard();

			int nLen = strText.GetAllocLength() + 1;
			HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, nLen);
			if (hMem)
			{
				LPSTR pClipData = (LPSTR)GlobalLock(hMem);
				pClipData[0] = 0;

				strcpy_s(pClipData, nLen, strText.GetBuffer());
				strText.ReleaseBuffer();

				GlobalUnlock(hMem);
				if (SetClipboardData(CF_TEXT, hMem) == NULL)
				{
					CloseClipboard();
					return FALSE;
				}
			}

			CloseClipboard();

			return TRUE;
		}

		BOOL Conv_Uni_Utf8(IN CString strUni, OUT std::string& utf8)
		{
			int nLen = WideCharToMultiByte(CP_UTF8, 0, strUni, -1, NULL, 0, NULL, NULL);
			if (nLen)
			{
				utf8.resize((size_t)nLen);

				WideCharToMultiByte(CP_UTF8, 0, strUni, lstrlen(strUni), (LPSTR)utf8.data(), nLen, NULL, NULL);

				return TRUE;
			}

			return FALSE;
		}

		CString ReadRegistryValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey)
		{
			HKEY hKeyValue = NULL;

			int ret = RegOpenKeyEx(hKey, lpSubKey, 0, KEY_QUERY_VALUE, &hKeyValue);
			if (ret == ERROR_SUCCESS)
			{
				DWORD data_size = 1024;
				LPTSTR data[1024]{};

				RegQueryValueEx(hKeyValue, lpValueKey, 0, NULL, (LPBYTE)data, &data_size);

				RegCloseKey(hKeyValue);

				return (LPCTSTR)data;
			}

			return L"";
		}

		BOOL WriteRegistryValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueKey, LPCTSTR lpValue)
		{
			HKEY hSubKey = NULL;

			if (::RegCreateKeyEx(hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hSubKey, NULL) == ERROR_SUCCESS)
			{
				DWORD cbSize = (DWORD)_tcslen(lpValue) * sizeof(TCHAR) + 1;

				::RegSetValueEx(hSubKey, lpValueKey, 0, REG_SZ, (const BYTE*)lpValue, cbSize);

				::RegCloseKey(hSubKey);

				return TRUE;
			}

			return FALSE;
		}

		void ReadRegistryKeyAll(HKEY hKey, CString subKey, OUT CStringList& listKeyPath)
		{
			if (::RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
			{
				DWORD dwIndex = 0;
				TCHAR subKeyName[MAX_PATH_LEN]{};

				while (::RegEnumKey(hKey, dwIndex, subKeyName, MAX_PATH_LEN) != ERROR_NO_MORE_ITEMS)
				{
					if (NULL == listKeyPath.Find(subKeyName))
					{
						listKeyPath.AddTail(subKeyName);
					}

					dwIndex++;
				}

				::RegCloseKey(hKey);
			}
		}

		void ReadRegistryValueAll(HKEY hKey, CString subKey, OUT CMapStringToString& mapValue)
		{
			enum { MAX_KEY_LENGTH = 255, MAX_VALUE_NAME = 16383 };

			TCHAR    achClass[MAX_PATH_LEN] = L"";  // buffer for class name
			DWORD    cchClassName = MAX_PATH_LEN;  // size of class string
			DWORD    cSubKeys = 0;               // number of subkeys
			DWORD    cbMaxSubKey;              // longest subkey size
			DWORD    cchMaxClass;              // longest class string
			DWORD    cValues;              // number of values for key
			DWORD    cchMaxValue;          // longest value name
			DWORD    cbMaxValueData;       // longest value data
			DWORD    cbSecurityDescriptor; // size of security descriptor
			FILETIME ftLastWriteTime;      // last write time

			TCHAR  achValue[MAX_VALUE_NAME];
			DWORD cchValue = MAX_VALUE_NAME;

			// open the key
			if (::RegOpenKeyEx(hKey, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
			{
				DWORD retCode = RegQueryInfoKey(
					hKey,                    // key handle
					achClass,                // buffer for class name
					&cchClassName,           // size of class string
					NULL,                    // reserved
					&cSubKeys,               // number of subkeys
					&cbMaxSubKey,            // longest subkey size
					&cchMaxClass,            // longest class string
					&cValues,                // number of values for this key
					&cchMaxValue,            // longest value name
					&cbMaxValueData,         // longest value data
					&cbSecurityDescriptor,   // security descriptor
					&ftLastWriteTime);       // last write time

				for (DWORD i = 0; i < cValues; i++)
				{
					cchValue = MAX_VALUE_NAME;
					achValue[0] = '\0';

					int retCode = RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL);
					if (retCode == ERROR_SUCCESS)
					{
						DWORD buf_size = 0;

						if (::RegQueryValueEx(hKey, achValue, NULL, NULL, NULL, &buf_size) == ERROR_SUCCESS)
						{
							TCHAR* pBuf = new TCHAR[buf_size + 1];

							if (::RegQueryValueEx(hKey, achValue, NULL, NULL, (LPBYTE)pBuf, &buf_size) == ERROR_SUCCESS)
							{
								pBuf[buf_size] = _T('\0');

								mapValue.SetAt(CString(achValue), CString(pBuf));
							}

							// to avoid leakage
							delete[] pBuf;
						}
					}
				}
			}

			::RegCloseKey(hKey);
		}

		CString GetCPUName()
		{
			TCHAR Cpu_info[100];
			HKEY hKey;
			int i = 0;
			long result = 0;
			DWORD c_size = sizeof(Cpu_info);

			RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
			RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)Cpu_info, &c_size);
			RegCloseKey(hKey);

			TCHAR num[8];
			SYSTEM_INFO systemInfo;
			GetSystemInfo(&systemInfo);
			swprintf(num, 8, L" * %d", systemInfo.dwNumberOfProcessors);
			wcscat_s(Cpu_info, 100, num);
			return Cpu_info;
		}

		CString GetOSName()
		{
			TCHAR ProductName[100];
			TCHAR CSDVersion[100];
			std::wstring Os_info;

			HKEY hKey;
			int i = 0;

			DWORD c_size = 100;

			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\", 0, KEY_QUERY_VALUE, &hKey) != ERROR_SUCCESS)
			{
				return L"Fail to Open OS_info";
			}
			if (RegQueryValueEx(hKey, L"ProductName", NULL, NULL, (LPBYTE)ProductName, &c_size) != ERROR_SUCCESS)
			{
				return L"Fail to Load the ProductName";
			}
			if (RegQueryValueEx(hKey, L"CSDVersion", NULL, NULL, (LPBYTE)CSDVersion, &c_size) != ERROR_SUCCESS)
			{
				RegCloseKey(hKey);
				return ProductName;
			}

			Os_info = ProductName;
			Os_info = L" ";
			Os_info = CSDVersion;

			RegCloseKey(hKey);

			return Os_info.c_str();
		}

		int	GetSystemDate()
		{
			CTime time = CTime::GetCurrentTime();
			return _ttoi(time.Format(_T("%Y%m%d")));
		}

		int	GetSystemTime()
		{
			CTime time = CTime::GetCurrentTime();
			return _ttoi(time.Format(_T("%H%M%S")));
		}

		int	TimeToMinute(int nTime)
		{
			int nHour = nTime / 10000;
			int nMinute = (nTime - (nHour * 10000)) / 100;

			int nResult = nHour * 60 + nMinute;

			return nResult;
		}
	}

#if 0
	cstring::cstring(LPCTSTR src) { Append(src); }

	cstring::cstring(LPCSTR astr) {
		int len = MultiByteToWideChar(CP_ACP, 0, astr, -1, NULL, NULL);
		if (len > 0) {
			auto buffer = std::make_unique<TCHAR[]>(len + 1);
			MultiByteToWideChar(CP_ACP, 0, astr, -1, buffer.get(), len);
			Append(buffer.get());
		}
	}

	LPCSTR cstring::c_str() {
		int len = WideCharToMultiByte(CP_ACP, 0, GetString(), -1, NULL, 0, NULL, NULL);
		if (len > 0) {
			auto buffer = std::make_unique<char[]>(len + 1);
			WideCharToMultiByte(CP_ACP, 0, GetString(), -1, buffer.get(), len, NULL, NULL);
			return CStringA(buffer.get());
		}
		return "";
	}

	LPCTSTR cstring::operator=(LPCSTR right) {
		int len = MultiByteToWideChar(CP_ACP, 0, right, -1, NULL, NULL);
		if (len > 0) {
			auto buffer = std::make_unique<TCHAR[]>(len + 1);
			MultiByteToWideChar(CP_ACP, 0, right, -1, buffer.get(), len);
			return CStringW(buffer.get());
		}
		return _T("");
	}

	LPCTSTR cstring::operator=(std::string right) {
		int len = MultiByteToWideChar(CP_ACP, 0, right.c_str(), -1, NULL, NULL);
		if (len > 0) {
			auto buffer = std::make_unique<TCHAR[]>(len + 1);
			MultiByteToWideChar(CP_ACP, 0, right.c_str(), -1, buffer.get(), len);
			return CStringW(buffer.get());
		}
		return _T("");
	}
#endif
}