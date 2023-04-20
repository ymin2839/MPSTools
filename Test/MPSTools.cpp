#include "pch.h"
#include "MPSTools.h"
#include <memory>

#define MAX_PATH_LEN	260

namespace mps
{
	cstring::cstring(LPCTSTR src)
	{
		Append(src);
	}

	cstring::cstring(LPCSTR astr)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, astr, -1, NULL, NULL);
		if (len > 0)
		{
			auto buffer = std::make_unique<TCHAR[]>(len + 1);

			MultiByteToWideChar(CP_ACP, 0, astr, -1, buffer.get(), len);

			Append(buffer.get());
		}
	}

	LPCSTR cstring::c_str()
	{
		int len = WideCharToMultiByte(CP_ACP, 0, GetString(), -1, NULL, 0, NULL, NULL);
		if (len > 0)
		{
			auto buffer = std::make_unique<char[]>(len + 1);

			WideCharToMultiByte(CP_ACP, 0, GetString(), -1, buffer.get(), len, NULL, NULL);

			return CStringA(buffer.get());
		}

		return "";
	}

	LPCTSTR cstring::operator=(LPCSTR right)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, right, -1, NULL, NULL);
		if (len > 0)
		{
			auto buffer = std::make_unique<TCHAR[]>(len + 1);

			MultiByteToWideChar(CP_ACP, 0, right, -1, buffer.get(), len);

			return CStringW (buffer.get());
		}

		return _T("");
	}

	LPCTSTR cstring::operator=(std::string right)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, right.c_str(), -1, NULL, NULL);
		if (len > 0)
		{
			auto buffer = std::make_unique<TCHAR[]>(len + 1);

			MultiByteToWideChar(CP_ACP, 0, right.c_str(), -1, buffer.get(), len);

			return CStringW(buffer.get());
		}

		return _T("");
	}

	namespace strutil {
		LPCSTR Printf(LPCSTR format, ...)
		{
			std::string rs;

			va_list args;
			va_start(args, format);
			int len = _vscprintf(format, args) + 1; // terminating '\0'
			if (len > 0)
			{
				char* buffer = new char[len] {};

				vsprintf_s(buffer, len, format, args);

				rs.append(buffer, len);

				delete[] buffer;
			}
			va_end(args);

			return rs.c_str();
		}

		CStringW Printf(LPCTSTR format, ...)
		{
			CStringW rs;

			va_list args;

			va_start(args, format);
			int len = _vscwprintf(format, args) + 1;
			if (len > 0)
			{
				TCHAR* buffer = new TCHAR[len] {};

				vswprintf_s(buffer, len, format, args);

				rs.Append(buffer);

				delete[] buffer;
			}
			va_end(args);

			return rs;
		}

		LPCSTR WtA(LPCTSTR wstr)
		{
			int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
			if (len > 0)
			{
				char* buffer = new char[len + 1] {};

				WideCharToMultiByte(CP_ACP, 0, wstr, -1, buffer, len, NULL, NULL);

				std::string rs(buffer);

				delete[] buffer;

				return rs.c_str();
			}

			return "";
		}

		CStringW AtW(LPCSTR astr)
		{
			int len = MultiByteToWideChar(CP_ACP, 0, astr, -1, NULL, NULL);
			if (len > 0)
			{
				TCHAR* buffer = new TCHAR[len + 1]{};

				MultiByteToWideChar(CP_ACP, 0, astr, -1, buffer, len);

				CStringW rs(buffer);

				delete[] buffer;

				return rs;
			}

			return _T("");
		}

		CStringW U8tW(LPCSTR u8str)
		{
			int len = MultiByteToWideChar(CP_UTF8, 0, u8str, -1, 0, 0);
			if (len > 0)
			{
				TCHAR* buffer = new TCHAR[len + 1]{};

				MultiByteToWideChar(CP_UTF8, 0, u8str, -1, buffer, len);

				CStringW rs(buffer);

				delete[] buffer;

				return rs;
			}
			
			return _T("");
		}

		LPCSTR WtU8(LPCTSTR wstr)
		{
			int len = WideCharToMultiByte(CP_UTF8, 0, wstr, lstrlen(wstr), NULL, 0, NULL, NULL);
			if (len)
			{
				std::string rs(len, 0);

				WideCharToMultiByte(CP_UTF8, 0, wstr, -1, (LPSTR)rs.data(), len, NULL, NULL);

				return rs.c_str();
			}

			return "";
		}

		BOOL StringParser(IN LPCTSTR data_src, OUT CStringList& sl, LPCTSTR key /*= L";"*/)
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

			return TRUE;
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

		CString GetCurrPath()
		{
			TCHAR szPath[MAX_PATH_LEN] = L"";
			GetModuleFileNameW(NULL, szPath, MAX_PATH_LEN);

			CString strPath(szPath);

			return strPath;
		}

		CString GetCurrAppName()
		{
			TCHAR szPath[MAX_PATH_LEN] = L"";
			GetModuleFileNameW(NULL, szPath, MAX_PATH_LEN);

			CString strPath(szPath);
			strPath = strPath.Mid(strPath.ReverseFind(L'\\') + 1);
			return strPath;
		}

		CString GetCurrDirectory()
		{
			TCHAR szPath[MAX_PATH_LEN] = L"";
			GetModuleFileNameW(NULL, szPath, MAX_PATH_LEN);

			CString strPath(szPath);
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

				if (finder.IsArchived())
				{
					CString fileNameFind = finder.GetFileName();

					if (fileNameFind == _T(".") || fileNameFind == _T("..") || fileNameFind == _T("Thumbs.db"))
						continue;

					CString fileName = finder.GetFileTitle();

					slFileName.AddTail(fileName);
				}

				if (bIncFolder && finder.IsDirectory())
				{
					CString dirName = finder.GetFileName();

					slFileName.AddTail(dirName);
				}
			}

			return slFileName.GetCount();
		}

		CString GetDirByProcName(LPCTSTR procName_src)
		{
			CString procName(procName_src);

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
	}
}