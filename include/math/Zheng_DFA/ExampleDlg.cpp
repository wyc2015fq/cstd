// ExampleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Zheng_DFA.h"
#include "ExampleDlg.h"


// ExampleDlg dialog

IMPLEMENT_DYNAMIC(ExampleDlg, CDialog)

ExampleDlg::ExampleDlg(CWnd* pParent /*=NULL*/)
  : CDialog(ExampleDlg::IDD, pParent)
{

}

ExampleDlg::~ExampleDlg()
{
}

void ExampleDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ExampleDlg, CDialog)
END_MESSAGE_MAP()
BOOL ExampleDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  GetDlgItem(IDC_EDIT1)->SetWindowTextA("(a|b)*abb");
  GetDlgItem(IDC_EDIT3)->SetWindowTextA("aaaabb");
  return true;
}

// ExampleDlg message handlers
