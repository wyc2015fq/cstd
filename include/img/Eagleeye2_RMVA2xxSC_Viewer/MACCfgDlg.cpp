// MACCfgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Eagleeye2_Viewer.h"
#include "MACCfgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMACCfgDlg dialog


CMACCfgDlg::CMACCfgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMACCfgDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMACCfgDlg)
	m_strMacAdd1 = _T("08");
	m_strMacAdd2 = _T("00");
	m_strMacAdd3 = _T("28");
	m_strMacAdd4 = _T("");
	m_strMacAdd5 = _T("");
	m_strMacAdd6 = _T("");
	//}}AFX_DATA_INIT
}


void CMACCfgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMACCfgDlg)
	DDX_Text(pDX, IDC_MACADD1, m_strMacAdd1);
	DDV_MaxChars(pDX, m_strMacAdd1, 2);
	DDX_Text(pDX, IDC_MACADD2, m_strMacAdd2);
	DDV_MaxChars(pDX, m_strMacAdd2, 2);
	DDX_Text(pDX, IDC_MACADD3, m_strMacAdd3);
	DDV_MaxChars(pDX, m_strMacAdd3, 2);
	DDX_Text(pDX, IDC_MACADD4, m_strMacAdd4);
	DDV_MaxChars(pDX, m_strMacAdd4, 2);
	DDX_Text(pDX, IDC_MACADD5, m_strMacAdd5);
	DDV_MaxChars(pDX, m_strMacAdd5, 2);
	DDX_Text(pDX, IDC_MACADD6, m_strMacAdd6);
	DDV_MaxChars(pDX, m_strMacAdd6, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMACCfgDlg, CDialog)
	//{{AFX_MSG_MAP(CMACCfgDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMACCfgDlg message handlers

void CMACCfgDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	CDialog::OnOK();
}
