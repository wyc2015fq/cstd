# VC++中的字体设置 - L_Andy的专栏 - CSDN博客

2013年04月10日 10:55:20[卡哥](https://me.csdn.net/L_Andy)阅读数：1693


VC++中static text字体改变 

窗口都有2个和字体有关的函数：

CWnd::GetFont()和SetFont(CFont*, BOOL);

1)CFont* pFont = m_static.GetFont();

2)LOGFONT LogFont;

pFont->GetLogFont(&LogFont);

3)对LogFont直接操纵修改里面的字体选项//如LogFont.lfUnderline = 1;设置下划线

 LogFont.lfHeight=30;       //字体大小设置

 strcpy(LogFont.lfFaceName, "楷体_GB2312");  //字体设置

4)pFont->Detach();

第四步的目的是将pFont里装有的HFONT解除关联，否则pFont无法调用紧接的Create函数。

5)pFont->CreateFontIndirect(&LogFont);

m_static.SetFont(pFont);

6)pFont->Detach();

必须再一次解除在pFont里装载的HFONT，原因是第5步已经将HFONT赋给了m_static。pFont的任务已完成，不应该持有HFONT资源，它也不能去销毁HFONT，因为m_static在使用这个HFONT，所以必须是Detach()来解除关联。 

VC++中字体颜色的改变 

在OnCtlColor函数中如下代码： 

 HBRUSH CDlg_SignIn::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)

{

 HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor); 

 // TODO:  Change any attributes of the DC here

   if(nCtlColor == CTLCOLOR_STATIC)

       {

       if(pWnd->GetDlgCtrlID()== IDC_REGARD)

           {

               pDC->SetTextColor(RGB(255,0,0));

               pDC->SetBkColor(RGB(251, 247, 200));//设置文本背景色

               pDC->SetBkMode(TRANSPARENT);//设置背景透明                   

           }

       } 

 // TODO:  Return a different brush if the default is not desired

 return hbr;

}  

其他控件的宏定义为： 

CTLCOLOR_BTN 按钮控件 

CTLCOLOR_DLG 对话框 

CTLCOLOR_EDIT 编辑框 

CTLCOLOR_LISTBOX 列表控件 

CTLCOLOR_MSGBOX 消息控件 

CTLCOLOR_SCROLLBAR 滚动条控件 

CTLCOLOR_STATIC 静态控件 

VC中动态改变控件和对话框字体. 

1 VC的对话框字体设置对所有控件都有效，你不能单独地改变某个静态文本的字体。对于你的问题，需要首先用CreateFont来建立一个字体对象，然后调用控件的SetFont，就可以了。

    例子： 

    1、改静态文体的ID，如:IDC_STATIC1 

    2、添加一个Edit控件，建立一个关联的控件m_editControl。 

    3、在OnInitDialog中添加如下代码：  

CFont * f; 

     f = new CFont; 

     f->CreateFont(16, // nHeight 

     0, // nWidth 

     0, // nEscapement 

     0, // nOrientation 

     FW_BOLD, // nWeight 

     TRUE, // bItalic 

     FALSE, // bUnderline 

     0, // cStrikeOut 

     ANSI_CHARSET, // nCharSet 

     OUT_DEFAULT_PRECIS, // nOutPrecision 

     CLIP_DEFAULT_PRECIS, // nClipPrecision 

     DEFAULT_QUALITY, // nQuality 

     DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily 

     _T("Arial")); // lpszFac    

     GetDlgItem(IDC_STATIC1)->SetFont(f); 

     CWnd *cWnd = GetDlgItem(IDC_STATIC1);

     cWnd->SetFont(&font); 

     cWnd->SetWindowTextW(L"设置需要的内容"); 

需要注意的是，这里我们使用的是CFont指针，而不是普通的CFont局部变量, 在非MFC程序，首先用CreateFont来建立一个字体句柄，然后再用SendMessage发给控件WM_SETFONT消息，将建立的字体句柄赋值过去，就可以了。  

2 但是整个对话框或窗口的字体的大小,使用对话框或窗口的SetFont()函数却没有任何的作用.可以在初始化时遍历每个控件分别设置来处理,但这里说另一种使用回调函数的简单方法:

   :调用系统的API:::EnumChildWindows(). ,传入回调函数和重新定义的字体.(第一个参数不用管啊,本来就有啊)

)

   1)在文档视图结构中CMainFrame::OnCreate().中调用::EnumChildWindows(). 实现所有窗口和子窗口字体改变

   2) 在对话框的OnInitDialog(). 中调用::EnumChildWindows(). 改变对话窗上的所有控件. 

回调函数如下: 

/ lParam is a pointer to CFont object

BOOL __stdcall SetChildFont(HWND hwnd, LPARAM lparam)

{

CFont *pFont = (CFont*)lparam;

CWnd *pWnd = CWnd::FromHandle(hwnd); 

pWnd->SetFont(pFont);

return TRUE; 

}

使用1: 

BOOL CAboutDlg::OnInitDialog() 

{

CDialog::OnInitDialog();

// TODO: Add extra initialization here

::EnumChildWindows(m_hWnd, ::SetChildFont, (LPARAM)g_Font.GetFont());

return TRUE;  // return TRUE unless you set the focus to a control

  // EXCEPTION: OCX Property Pages should return FALSE

} 

使用2: 

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)

{

 if (CFrameWnd::OnCreate(lpCreateStruct) == -1)

return -1;

 if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP

| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||

!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))

{

TRACE0("Failed to create toolbar\n");

 return -1;      // fail to create

}

if (!m_wndStatusBar.Create(this) ||

!m_wndStatusBar.SetIndicators(indicators,

sizeof(indicators)/sizeof(UINT)))

{

 TRACE0("Failed to create status bar\n");

return -1;      // fail to create

 }

m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

EnableDocking(CBRS_ALIGN_ANY);

DockControlBar(&m_wndToolBar);

::EnumChildWindows(m_hWnd, ::SetChildFont, (LPARAM)g_Font.GetFont());

return 0;

} 

(很好用,不像mfc中的那个垃圾setfont(),设置了对话框的没有一点反应!) 

3 如何在mfc中实现,当系统的字体变大的时候,对话框上面的字体也相应的变大?(非常感谢)

//IconFont

    LOGFONT logFont;

    int  size = sizeof(LOGFONT);

    bool isGood = SystemParametersInfo(SPI_GETICONTITLELOGFONT,size,&logFont,0);

    if(isGood == true)

{

 CFont * f;

f = new CFont;

const LOGFONT* pFont = new LOGFONT(logFont);

f->CreateFontIndirectW(pFont);

 //::EnumChildWindows(m_hWnd, ::SetChildFont, (LPARAM)f);

} 

//other Font

NONCLIENTMETRICS ncm = new NONCLIENTMETRICS();                

bool isGood = SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), ref ncm, 0);

if (isGood == true)

{

LOGFONT logFont2;

//logFont2=ncm.lfntCaptionFont);//CaptionFont

 //logFont2 =ncm.lfntSMCaptionFont;//CaptionFont_Small

//logFont2 = ncm.lfntMenuFont;//MenuFont

//logFont2 = ncm.lfntStatusFont;//StatusFont

 logFont2 = ncm.lfntMessageFont;//MessageFont

CFont * f;

 f = new CFont;

const LOGFONT* pFont = new LOGFONT(logFont2);

 f->CreateFontIndirectW(pFont);

 //::EnumChildWindows(m_hWnd, ::SetChildFont, (LPARAM)f);

 }

 以上是取得系统字体的大小,然后再调用上面的第二种方法。

窗体上的所有字体都会跟着系统字体的大小改变。

