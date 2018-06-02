// DiagExtr.cpp : implementation file
//


#include "rubixcube.h"
#include "ChildView.h"
#include "MainFrm.h"
#include "VersionRes.h"
#include "DiagExtr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//
// CDiagExtr dialog

CDiagExtr::CDiagExtr(CWnd* pParent)
  : CDialog(CDiagExtr::IDD, pParent)
{
  //{{AFX_DATA_INIT(CDiagExtr)
  //}}AFX_DATA_INIT
  m_pWnd = NULL;
  m_wnd.acq = true;
}

CDiagExtr::CDiagExtr(const CString& fileName, CWnd* pParent)
  : CDialog(CDiagExtr::IDD, pParent)
{
  m_wnd.m_extract.FileRead(fileName);
  m_pWnd = pParent;
  m_wnd.acq = false;
}

BOOL CDiagExtr::Create(CWnd* pParent)
{
  if (!CDialog::Create(CDiagExtr::IDD, pParent)) {
    return FALSE;
  }

  m_pWnd = pParent;
  // load and init the TWAIN driver
  InitTwain(m_hWnd);

  if (!IsValidDriver()) {
    AfxMessageBox("Unable to load Twain Driver.");
  }

  return TRUE;
}

BOOL CDiagExtr::OnInitDialog()
{
  CDialog::OnInitDialog();

  ((CButton*)GetDlgItem(IDC_CHKHISTORY))->SetCheck(0);
  GetDlgItem(IDOK)->EnableWindow(m_wnd.m_extract.IsReady());
  SetTimer(IDD_TIMER, 500, NULL);

  return TRUE;  // return TRUE unless you set the focus to a control
}               // EXCEPTION: OCX Property Pages should return FALSE

void CDiagExtr::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDiagExtr)
  DDX_Control(pDX, IDC_EXTRACT, m_wnd);
  //}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CDiagExtr, CDialog)
  //{{AFX_MSG_MAP(CDiagExtr)
  ON_BN_CLICKED(IDC_CHKHISTORY, OnChkhistory)
  ON_WM_TIMER()
  ON_BN_CLICKED(IDC_OPTISOLVER, OnResolver)
  ON_BN_CLICKED(ID_ACQUIRE, OnAcquire)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CDiagExtr::OnOK()
{
  if (m_pWnd && m_wnd.m_extract.IsReady()) {
    ((CChildView*)m_pWnd)->GetCubeText(ToString());
  }

  CDialog::OnOK();
}

void CDiagExtr::OnCancel()
{
  if (m_wnd.acq) {
    CloseDS();
  }

  CDialog::OnCancel();
}

void CDiagExtr::OnResolver()
{
  if (m_pWnd && m_wnd.m_extract.IsReady()
      && ((CChildView*)m_pWnd)->GetCubeText(ToString())) {

    CDialog::OnOK();

    ((CMainFrame*)AfxGetMainWnd())->m_diagCmd.OptiSolverCmd();
  }
}
//
// CDiagExtract message handlers

void CDiagExtr::OnChkhistory()
{
  m_wnd.chkHistory = ((CButton*)GetDlgItem(IDC_CHKHISTORY))->GetCheck() == 1 ? TRUE : FALSE;
  m_wnd.Invalidate();
}

void CDiagExtr::OnTimer(UINT nIDEvent)
{
  BOOL rdy = m_wnd.m_extract.IsReady();
  GetDlgItem(IDOK)->EnableWindow(rdy);
  GetDlgItem(IDC_OPTISOLVER)->EnableWindow(rdy);

  CDialog::OnTimer(nIDEvent);
}
//
// driver TWAIN

void CDiagExtr::GetIdentity()
{
  CVersionRes version;
  // Expects all the fields in m_AppId to be set except for the id field.
  m_AppId.Id = 0; // Initialize to 0 (Source Manager will assign real value)
  m_AppId.Version.MajorNum = version.MajorNum();
  m_AppId.Version.MinorNum = version.MinorNum();
  m_AppId.Version.Language = TWLG_FRENCH;
  m_AppId.Version.Country = TWCY_FRANCE;
  strcpy(m_AppId.Version.Info, version.VersionInfo());
  m_AppId.ProtocolMajor = TWON_PROTOCOLMAJOR;
  m_AppId.ProtocolMinor = TWON_PROTOCOLMINOR;
  m_AppId.SupportedGroups = DG_IMAGE | DG_CONTROL;
  strcpy(m_AppId.Manufacturer, version.GetString("CompanyName"));
  strcpy(m_AppId.ProductFamily, "Games");
  strcpy(m_AppId.ProductName, version.GetString("InternalName"));
}

void CDiagExtr::CopyImage(HANDLE hBitmap, TW_IMAGEINFO& info)
{
  BITMAPINFOHEADER* pBih = (BITMAPINFOHEADER*)GlobalLock(hBitmap);
  Gdiplus::Bitmap* pBitmap;

  pBitmap = Gdiplus::Bitmap::FromBITMAPINFO((LPBITMAPINFO)pBih, pBih + 1);
  GlobalUnlock(pBih);

  if (!AddImage(pBitmap)) {
    CloseDS();
  }
}

BOOL CDiagExtr::PreTranslateMessage(MSG* pMsg)
{
  if (ProcessMessage(*pMsg)) {
    return TRUE;
  }

  return CDialog::PreTranslateMessage(pMsg);
}

void CDiagExtr::OnAcquire()
{
  // TODO: Add your control notification handler code here

}
