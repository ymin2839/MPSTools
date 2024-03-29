﻿#include "pch.h"
#include "framework.h"
#include "Test.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include "mpstring.h"

#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CTestDlg::CTestDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_DIALOG, pParent)
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// printf
	{
		CString wstr = mps::util::Printf(L"hangle ok? 한글되니?");
		std::string cstr = mps::util::Printf("hangle ok? 한글되니?");
	}
	
	// uni to u8, u8 to uni
	{
		std::string ustr = mps::util::WtU8(L"hangle ok? 한글되니?");
		CString wstr = mps::util::U8tW(ustr.c_str());
	}
	
	// conv ansi to unicode
	{
		char buff[1024 + 1]{};
		buff[0] = 'a';
		buff[1024] = '\0';

		CString rs;
		rs.Format(L"%s", (LPCTSTR)mps::util::AtW(buff));
	}
	
	// conv unicode to ansi
	{
		WCHAR buff[1024 + 1]{};
		buff[0] = L'a';
		buff[1024] = L'\0';

		CStringA rs;
		rs.Format("%s", mps::util::WtA(buff));
	}

	// mps::string
	{
		mps::string wstr(L"hangle ok? 한글되니?");
		mps::string cstr("hangle ok? 한글되니?");
		mps::string ustr(std::string(u8R"(hangle ok? 한글되니?)"));
	}

	{
		mps::string wstr_oper = { L"hangle ok? 한글되니?" };
		mps::string cstr_oper = { "hangle ok? 한글되니?" };
	}

	{
		mps::string mpstr = u8R"({"한글도 되나요?":"네","hangle ok?":"yes"})";
		const char* strptr = mpstr.data();
	}

	{
		mps::string utf8_fmt;
		std::string utf8_ret = utf8_fmt.format(u8R"({"한글도 되나요?":"%s","hangle ok?":"yes"})", "네");

		mps::string cstr_fmt;
		std::string cstr_ret = cstr_fmt.format(R"({"한글도 되나요?":"%s","hangle ok?":"yes"})", "네");

		mps::string wstr_fmt;
		std::wstring wstr_ret = wstr_fmt.format(LR"({"한글도 되나요?":"%s","hangle ok?":"yes"})", L"네").wstr();
	}

	{
		mps::string mpstr = u8R"({"한글도 되나요?":"네","hangle ok?":"yes"})";
		CString wstr = mpstr.utf8_to_wstr();
		std::string cstr = mpstr.utf8_to_cstr();
	}

	{
		mps::string str = "  hihi  ";
		CString res = str.trim().wstr();
	}

	return TRUE;
}