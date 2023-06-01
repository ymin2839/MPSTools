#include "pch.h"
#include "mpstring.h"
#include <memory>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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

	CString string::utf8_to_wstr()
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

	const char* string::to_utf8()
	{
		int len = WideCharToMultiByte(CP_UTF8, 0, wstr(), -1, NULL, 0, NULL, NULL);
		if (len)
		{
			auto buffer = std::make_unique<char[]>(len + 1);

			WideCharToMultiByte(CP_UTF8, 0, wstr(), -1, buffer.get(), len, NULL, NULL);

			std::string res(buffer.get());

			return res.c_str();
		}

		return "";
	}

	const char* string::utf8_to_cstr()
	{
		std::wstring wstr = utf8_to_wstr().operator LPCWSTR();

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

	string string::format(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);

		int len = _vscprintf(fmt, args); // terminating '\0'
		if (len > 0)
		{
			auto buffer = std::make_unique<char[]>(len + 1);

			vsprintf_s(buffer.get(), len + 1, fmt, args);

			clear();
			append(buffer.get());
		}

		va_end(args);

		return *this;
	}

	string string::format(const wchar_t* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);

		int len = _vscwprintf(fmt, args) + 1;
		if (len > 0)
		{
			auto buffer = std::make_unique<wchar_t[]>(len + 1);

			vswprintf_s(buffer.get(), len, fmt, args);

			*this = (wchar_t*)buffer.get();
		}

		va_end(args);

		return *this;
	}

	string string::replace_all(const char* old_str, const char* new_str)
	{
		size_t pos = 0;
		while ((pos = find(old_str, pos)) != std::string::npos) {
			erase(pos, strlen(old_str));
			insert(pos, new_str);
			pos++;
		}
		return c_str();
	}

	string string::ltrim()
	{
		erase(0, find_first_not_of(" \n\r\t\f\v"));
		return c_str();
	}

	string string::rtrim()
	{
		erase(find_last_not_of(" \n\r\t\f\v") + 1);
		return c_str();
	}

	string string::trim()
	{
		ltrim();
		return rtrim();
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
		return format("%I64d", num).c_str();
	}

	const bool string::operator==(const long long& num)
	{
		return atoll(c_str()) == num ? true : false;
	}

	namespace util {
		CStringA Printf(LPCSTR format, ...)
		{
			CStringA rt;

			va_list args;
			va_start(args, format);

			int len = _vscprintf(format, args); // terminating '\0'
			if (len > 0)
			{
				auto buffer = std::make_unique<char[]>(len + 1);

				vsprintf_s(buffer.get(), len + 1, format, args);

				rt.Append(buffer.get());
			}

			va_end(args);

			return rt;
		}

		CStringW Printf(LPCTSTR format, ...)
		{
			CStringW rt;

			va_list args;
			va_start(args, format);

			int len = _vscwprintf(format, args);
			if (len > 0)
			{
				auto buffer = std::make_unique<TCHAR[]>(len + 1);

				vswprintf_s(buffer.get(), len + 1, format, args);

				rt.Append(buffer.get());
			}

			va_end(args);

			return rt;
		}

		CStringA WtA(LPCTSTR wstr)
		{
			int len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
			if (len > 0)
			{
				auto buffer = std::make_unique<char[]>(len + 1);

				WideCharToMultiByte(CP_ACP, 0, wstr, -1, buffer.get(), len, NULL, NULL);

				return CStringA(buffer.get());
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

				return CStringW(buffer.get());
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

				return CStringW(buffer.get());
			}

			return _T("");
		}

		CStringA U8tA(LPCSTR u8str)
		{
			return WtA(U8tW(u8str));
		}

		CStringA WtU8(LPCTSTR wstr)
		{
			int len = WideCharToMultiByte(CP_UTF8, 0, wstr, lstrlen(wstr), NULL, 0, NULL, NULL);
			if (len)
			{
				auto buffer = std::make_unique<char[]>(len + 1);

				WideCharToMultiByte(CP_UTF8, 0, wstr, -1, buffer.get(), len, NULL, NULL);

				return CStringA(buffer.get());
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