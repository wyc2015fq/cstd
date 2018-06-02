#pragma once


// ExampleDlg dialog

class ExampleDlg : public CDialog
{
	DECLARE_DYNAMIC(ExampleDlg)

public:
	ExampleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~ExampleDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
