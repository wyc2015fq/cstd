// DiagOut.cpp : implementation file
//


#include "rubixcube.h"
#include "DiagOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// CDiagOut dialog


CDiagOut::CDiagOut(const CString& text, CWnd* pParent)
  : CDialog(CDiagOut::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDiagOut)
  m_edit = text;
  //}}AFX_DATA_INIT
}


void CDiagOut::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDiagOut)
  DDX_Text(pDX, IDC_EDIT, m_edit);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDiagOut, CDialog)
  //{{AFX_MSG_MAP(CDiagOut)
  // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

//
// CDiagOut message handlers
