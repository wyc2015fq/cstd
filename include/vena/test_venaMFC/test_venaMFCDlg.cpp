// test_venaMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "test_venaMFC.h"
#include "test_venaMFCDlg.h"

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
// CTest_venaMFCDlg dialog

CTest_venaMFCDlg::CTest_venaMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest_venaMFCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest_venaMFCDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest_venaMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest_venaMFCDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTest_venaMFCDlg, CDialog)
	//{{AFX_MSG_MAP(CTest_venaMFCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_venaMFCDlg message handlers

void CTest_venaMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTest_venaMFCDlg::OnPaint() 
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
HCURSOR CTest_venaMFCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

#include "cstd.h"
#include "libvena.h"
#pragma comment(lib,"libvenadll.lib")
//#include "../vena.inl"
cap_t cap[1] = {0};
img_t im[10] = {0};
uchar im3_data[VENA_STDH * VENA_STDW];
img_t im3[1] = {0};
int i, k=0;
buf_t bf[1] = {0};
uchar* featlib = NULL;
enum {pic_per_man = 10};
int nman = 0, npic = 0, ch=0;
int isrecmode = 1;// 识别模式
typedef struct venafeat_t {
  uchar feat[VENA_FEATLEN];
  char name[64];
} venafeat_t;
venafeat_t feat[1];
char username[64];
#define FEAT_STEP  sizeof(venafeat_t)

BOOL CTest_venaMFCDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
  ::SetTimer(m_hWnd, 11, 30, 0);
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
  
  bfsetsize(bf, 10*1000*1000);
  if (cap_num(CAP_DSHOW)>0) {
    cap_open(cap, 0, CAP_DSHOW);
  }
  // TODO: Add extra initialization here
  return TRUE;  // return TRUE  unless you set the focus to a control
}

//#include "ui/window.inl"
void CTest_venaMFCDlg::OnTimer(UINT nIDEvent) 
{
  //img_t im[1] = {0};
  //FillSolidRect(hDC, &rc, RGB(100, 0, 0));
  HDC hDC = ::GetDC(m_hWnd);
  int id=-1, dis=0;
  char buf[256];
  cap_getframe(cap, im, T_RGB);
  if (1) {
    utime_start(_start_time);
    k = vena_recog_bf(bf->data, bf->len, im->h, im->w, im->tt.data, im->s, im->c,
      pic_per_man*nman, isrecmode ? featlib : NULL, FEAT_STEP, im3_data, feat->feat, &id, &dis);
    printf("%f\n", utime_elapsed(_start_time));
    if (!k)  {
      //imdraw_fill(im3, _rgb(0, 0, 0));
      SetDlgItemText(IDC_STATIC1, "");
    } else {
      if (isrecmode) {
        if (featlib) {
          venafeat_t* f = (venafeat_t*)featlib;
          _snprintf(buf, 256, "识别: id = %d dis = %d username = %s", id, dis, dis<4000 ? f[id].name : "陌生人");
          //RECT rc = {0, 0, 100, 100};
          //imdraw_text_v(im, 0, buf, 100, 100, NULL, dis>4000 ? _rgb(1, 0, 0) : _rgb(0, 1, 0));
          SetDlgItemText(IDC_STATIC1, buf);
        }
      } else {
        if (npic<(nman+1)*pic_per_man) {
          _snprintf(buf, 256, "注册: 第%d张/共%d张", npic - (nman)*pic_per_man, pic_per_man);
          SetDlgItemText(IDC_STATIC1, buf);
          strcpy(feat->name, username);
          mem_push_back(&featlib, &npic, feat, 1, FEAT_STEP);
        } else {
          isrecmode = 1; // 切换回识别模式
          ++nman;
        }
      }
    }
    {
      //IRECT rc = iRECT(0, 0, im3->w, im3->h);
      //imdraw_image_corner(im, &rc, rc, im3, 0, 0, 0);
      //imdraw_rect(im, &rc, rc, 0, isrecmode ? _rgba(1,0,0,0.4) : _rgba(1,1,1,0.4), 10, BDM_ALL);
    }
  }
  {
    RECT rc = {0, 0, 100, 100};
    HDC dc = CreateCompatibleDC(hDC);
    cvShowDC1(dc, im->h, im->w, im->tt.data, im->s, im->c, 0);
    BitBlt(hDC, 0, 0, im->w, im->h, dc, 0, 0, SRCCOPY);
    DeleteDC(dc);
  }
    ::ReleaseDC(m_hWnd, hDC);
  //imshow(im);cvWaitKey(-1);
  //::InvalidateRect(m_hWnd, 0, 0);
	CDialog::OnTimer(nIDEvent);
}

void CTest_venaMFCDlg::OnOK() 
{
	// TODO: Add extra validation here
  GetDlgItemText(IDC_EDIT1, username, 64);
  if (username[0]) {
    npic = nman*pic_per_man;
    isrecmode = 0;
  } else {
    MessageBox("用户名不能为空");
  }
	//CDialog::OnOK();
}
