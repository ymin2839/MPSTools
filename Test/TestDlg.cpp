﻿#include "pch.h"
#include "framework.h"
#include "Test.h"
#include "TestDlg.h"
#include "afxdialogex.h"
#include "atlconv.h"
#include <string>
#include "ServTool.h"

//#pragma warning(disable : 4996)

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
		CString wstr = mps::strutil::Printf(L"hangle ok? 한글되니?");
		std::string cstr = mps::strutil::Printf("hangle ok? 한글되니?");
	}
	
	// uni >> mul
	{
		std::string ustr = mps::strutil::WtU8(L"hangle ok? 한글되니?");
		CString wstr = mps::strutil::U8tW(ustr.c_str());
	}
	
	// 
	{
		char buff[1024 + 1]{};
		buff[0] = 'a';
		buff[1024] = '\0';

		CString rs;
		rs.Format(L"%s", (LPCTSTR)mps::strutil::AtW(buff));
	}
	
	{
		WCHAR buff[1024 + 1]{};
		buff[0] = L'a';
		buff[1024] = L'\0';

		CStringA rs;
		rs.Format("%s", mps::strutil::WtA(buff).c_str());
	}

	{
		mps::cstring cstr(L"hihi");

		std::string astr("hangle ok? 한글되니?");
	}

	return TRUE;
}