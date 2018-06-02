// testA20Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "testA20.h"
#include "testA20Dlg.h"
#include "SerialPort.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestA20Dlg dialog

CTestA20Dlg::CTestA20Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestA20Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestA20Dlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestA20Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestA20Dlg)
	DDX_Control(pDX, IDC_LIST2, m_EditLogger);
	DDX_Control(pDX, IDC_CHECK1, m_ctrlReceiveHex);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTestA20Dlg, CDialog)
	//{{AFX_MSG_MAP(CTestA20Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_COMM_RXCHAR, OnCommunication)
	ON_BN_CLICKED(IDC_ADD_ID, OnAddId)
	ON_BN_CLICKED(IDC_DEL_ID_ALL, OnDelIdAll)
	ON_BN_CLICKED(IDC_ADD_ID_X, OnAddIdX)
	ON_BN_CLICKED(IDC_UP_CHAR_ALL, OnUpCharAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestA20Dlg message handlers

	CSerialPort m_Comm;

BOOL CTestA20Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
  int m_intPort = 3;
  int m_intBaudRate = 115200;
  int m_intDataBits = 8;
  int m_intStopBits = 0;
  char m_cParity = 'N';
	if (m_Comm.InitPort(m_hWnd,m_intPort,m_intBaudRate,m_cParity, m_intDataBits,m_intStopBits))
	{
		m_Comm.StartMonitoring();
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTestA20Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestA20Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestA20Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

CString g_str1;
LONG CTestA20Dlg::OnCommunication(WPARAM ch, LPARAM port)
{
  BOOL m_IsViewReceiveData = true;
  if (m_IsViewReceiveData)
  {
    CString str;
    if (m_ctrlReceiveHex.GetCheck()) {
      str.Format("%02X ",ch);
    }
    else{
      str.Format("%c",ch);
    }
    if ('\n'==ch || '\r'==ch ) {
      if (g_str1.GetLength()>0) {
        m_EditLogger.AddString(g_str1);
      }
      g_str1 = "";
    } else {
      g_str1 += str;
    }
    UpdateData(FALSE);
  }
#if 0
  GetDocument()->m_RXCount++;
  CMyCommApp * myApp = (CMyCommApp *)AfxGetApp();
  CString str;
  str.Format("RX:%d",GetDocument()->m_RXCount);
  myApp->DoSetStautsBarText(SBSRX,str);
  m_ctrlReciveData.LineScroll(m_ctrlReciveData.GetLineCount());
#endif
  return 0;
}

void CTestA20Dlg::OnAddId() 
{
  char buf[256];
  char curid[256];
  GetDlgItemTextA(IDC_EDIT1, curid, 256);
  _snprintf(buf, 256, "ADD_ID=%s\n", curid);
  m_Comm.WriteToPort(buf, strlen(buf));
}

void CTestA20Dlg::OnAddIdX() 
{
  char buf[256];
  char curid[256];
  GetDlgItemTextA(IDC_EDIT1, curid, 256);
  _snprintf(buf, 256, "ADD_ID_X=%s\n", curid);
  m_Comm.WriteToPort(buf, strlen(buf));
}

void CTestA20Dlg::OnDelIdAll() 
{
  char buf[256];
  _snprintf(buf, 256, "DEL_ID_ALL=%d\n", 1);
  m_Comm.WriteToPort(buf, strlen(buf));
}

void CTestA20Dlg::OnUpCharAll() 
{
  char buf[256];
  _snprintf(buf, 256, "UP_CHAR_ALL=%d\n", 1);
  m_Comm.WriteToPort(buf, strlen(buf));
}
