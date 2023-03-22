#pragma once

class CTestDlg : public CDialogEx
{
public:
	CTestDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
