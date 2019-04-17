# MFC中获取各个指针 - 战斗蜗牛的专栏 - CSDN博客





2011年10月03日 09:02:37[vbskj](https://me.csdn.net/vbskj)阅读数：575标签：[mfc																[文档																[框架																[file																[class																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[C/C++/Visual C++](https://blog.csdn.net/vbskj/article/category/630758)








### MFC中获取各种类指针

2008-07-30 11:47:36|  分类：[程序设计](http://caswei.blog.163.com/blog/#m=0&t=1&c=fks_087069081082083067084085083095092081081075083083083075)|字号订阅









> [柚子](http://pomelox.blog.163.com/) 的 [MFC中获取各种类指针](http://pomelox.blog.163.com/blog/static/204769332007542426816)

1、获取应用程序指针

CMyApp* pApp=(CMyApp*)AfxGetApp();


2、获取主框架指针

CWinApp 中的公有成员变量 m_pMainWnd 就是主框架的指针

CMainFrame* pMainFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);

或者

CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();


３、获取菜单指针

CMenu* pMenu = AfxGetMainWnd()->GetMenu();


４、获取工具栏、状态栏指针

主框架中可以直接使用m_wndToolBar、m_wndStatusBar

其他：

CToolBar* pToolBar = (CToolBar*)AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_TOOLBAR);

CStatusBar* pStatusBar = (CStatusBar*)AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);


5、获取控件指针

先用 GetDlgItem() 再转换，如：

CButton* pButton = (CButton*)GetDlgItem(IDC_MYBUTTON);


6、获取文档、视图指针

> 

SDI:

CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

CYourDoc* pDoc = (CYourDoc*)pMainFrame->GetActiveDocument();

CYourView* pView = (CYourView*)pMainFrame->GetActiveView();


MDI:

CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

CChildFrame* pChildFrame = (CChildFrame*)pMainFrame->GetActiveFrame();

CYourDoc* pDoc = (CYourDoc*)pChildFrame->GetActiveDocument();

CYourView* pView = (CYourView*)pChildFrame->GetActiveView();



7、文档、视图

> 

从视图获取文档指针：

CYourDoc* pDoc = GetDocument();


从文档获取视图指针：

利用成员函数 GetFirstViewPosition() 和 GetNextView() 遍历

virtual POSITION GetFirstViewPosition() const;

virtual CView* GetNextView(POSITION& rPosition) const;


SDI:

CYourView* pView;

POSITION pos = GetFirstViewPosition();

pView = GetNextView(pos);


MDI:

定义函数

CView* CYourDoc::GetView(CRuntimeClass* pClass)

{

    CView* pView;

    POSITION pos=GetFirstViewPosition();

    while(pos!=NULL)

    {

        pView=GetNextView(pos);

         if(!pView->IsKindOf(pClass))

             break;

    }

    if(!pView->IsKindOf(pClass))

    {

        AfxMessageBox("Connt Locate the View.");

       return NULL;

    }

    return pView;

}

使用如下：

CYourView* pView=(CYourView*)GetView(RUNTIME_CLASS(CYourView));



8、文档模版、文档

> 

从文档获取文档模版指针：

CDocTemplate* GetDocTemplate() const;


从文档模版获取文档指针：

viaual POSITION GetFirstDocPosition( ) const = 0; 

visual CDocument* GetNextDoc(POSITION & rPos) const = 0;



9、获取分割视图中各个视图的指针

> 

主框架中定义：CSplitterWnd m_wndSplitter;


定义两个View类：CView1、CView2


框架类中重载：

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT, CCreateContext* pContext)

{

    VERIFY(m_splitter.CreateStatic(this,2,1)); //分割成两行一列

    VERIFY(m_splitter.CreateView(0,0,RUNTIME_CLASS(CView1),CSize(100,100),pContext));

    VERIFY(m_splitter.CreateView(1,0,RUNTIME_CLASS(CView2),CSize(100,100),pContext));

    return TRUE;

}


获取分割视图指针

CView1* pView1 = (CView1*)m_wndSplitter.GetPane(0,0);

CView2* pView2 = (CView2*)m_wndSplitter.GetPane(1,0);



10、通过鼠标获得子窗口指针

> 

CWnd* ChildWindowFromPoint(POINT point) const;

CWnd* ChildWindowFromPoint(POINT point,UINT nFlags) const;

用于确定包含指定点的子窗口

如果指定点在客户区之外，函数返回NULL；

如果指定点在客户区内，但是不属于任何一个子窗口，函数返回该CWnd的指针；

如果有多个子窗口包含指定点，则返回第一个子窗口的指针。

还要注意的是，该函数返回的是一个伪窗口指针，不能将它保存起来供以后使用。

对于第二个参数nFlags有几个含义：

CWP_ALL             file://不忽略任何子窗口

CWP_SKIPNIVSIBLE    file://忽略不可见子窗口

CWP_SKIPDISABLED    file://忽略禁止的子窗口

CWP_SKIPRANSPARENT file://忽略透明子窗口](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)




