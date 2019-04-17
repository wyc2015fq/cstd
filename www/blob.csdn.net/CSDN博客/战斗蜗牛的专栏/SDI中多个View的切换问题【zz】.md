# SDI中多个View的切换问题【zz】 - 战斗蜗牛的专栏 - CSDN博客





2011年10月03日 08:22:36[vbskj](https://me.csdn.net/vbskj)阅读数：1577







我在MSDN上看到一篇文章，是关于，如何在SDI中切换View的，我按照它的方法做了 


可以切换，但是新建的那个View，在画面上看起来，比老的那个大一些，把框架的边都 


给挡住了，请问这个是怎么回事。 


除了在切换和OnPain的时候，改变View的大小，还有什么别的方法么？？ 


文章如下： 

-------------------------------------------------------------------------- 


Visual   C++   概念：添加功能       


将多个视图添加到单个文档 

在用   Microsoft   基础类库   (MFC)   创建的单文档接口   (SDI)   应用程序中，每个文档类型与单个视图类型关联。在一些情况中，最好具有在当前文档视图与新视图之间切换的能力。 


提示       有关实现单文档的多个视图的其他过程，请参见   CDocument::AddView   和下列   MFC   示例：ENROLL   和   COLLECT。 

通过添加新的   CView   派生类和用于将视图动态切换到现有   MFC   应用程序的附加代码，可以实现此功能。 


步骤如下：   


修改现有应用程序类   

创建和修改新视图类   

创建并附加新视图   

实现切换函数   

添加视图切换支持   

本主题的其余部分假定下列情况：   


CWinApp   派生的对象名是   CMyWinApp，并且   CMyWinApp   在   MYWINAPP.H   和   MYWINAPP.CPP   中声明和定义。   

CNewView   是新的   CView   派生对象名，并且   CNewView   在   NEWVIEW.H   和   NEWVIEW.CPP   中声明和定义。   

修改现有应用程序类 

为使应用程序能在视图间切换，需要修改应用程序类，方法是添加存储视图的成员变量和添加切换视图的方法。 


将下列代码添加到   MYWINAPP.H   中的   CMyWinApp   声明： 


CView*   m_pOldView; 

CView*   m_pNewView; 

CView*   SwitchView(   ); 

新成员变量   m_pOldView   和   m_pNewView   指向当前视图和新创建的视图。新方法   (SwitchView)   在用户请求时切换视图。该方法主体以后将在实现切换函数的本主题中讨论。 


对应用程序类的最后修改要求包括一个新的头文件来定义用在切换函数中的   Windows   消息   (WM_INITIALUPDATE)。 


将下行插到   MYWINAPP.CPP   的   include   部分： 


#include   <AFXPRIV.H> 

保存所做更改后继续进行下一步操作。 


创建和修改新视图类 

通过使用类视图中可用的新建类命令使创建新视图类变得容易。对此类的唯一要求是它从   CView   派生。将此新类添加到应用程序。有关将新类添加到项目的特定信息，请参见添加类。 


将类添加到项目后，需要更改一些视图类成员的可访问性。 


修改   NEWVIEW.H，将构造函数和析构函数的访问说明符从   protected   更改为   public。这使得可以动态创建和销毁类，并且允许类在视图可见前修改视图外观。 


保存所做更改后继续进行下一步操作。 


创建并附加新视图 

若要创建和附加新视图，需要修改应用程序类的   InitInstance   函数。该修改添加创建新视图对象的新代码，然后用现有的两个视图对象初始化   m_pOldView   和   m_pNewView。 


因为新视图是在   InitInstance   函数内创建的，因此新视图和现有视图在应用程序的生存期内一直保持。但是，应用程序可以很容易地动态创建新视图。 


在调用   ProcessShellCommand   后插入此代码： 


... 

CView*   pActiveView   =   ((CFrameWnd*)   m_pMainWnd)-> GetActiveView(); 

m_pOldView   =   pActiveView; 

m_pNewView   =   (CView*)   new   CNewView; 


CDocument*   pCurrentDoc   =   ((CFrameWnd*)m_pMainWnd)-> GetActiveDocument(); 


//   Initialize   a   CCreateContext   to   point   to   the   active   document. 

//   With   this   context,   the   new   view   is   added   to   the   document 

//   when   the   view   is   created   in   CView::OnCreate(). 

CCreateContext   newContext; 

newContext.m_pNewViewClass   =   NULL; 

newContext.m_pNewDocTemplate   =   NULL; 

newContext.m_pLastView   =   NULL; 

newContext.m_pCurrentFrame   =   NULL; 

newContext.m_pCurrentDoc   =   pCurrentDoc; 


//   The   ID   of   the   initial   active   view   is   AFX_IDW_PANE_FIRST. 

//   Incrementing   this   value   by   one   for   additional   views   works 

//   in   the   standard   document/view   case   but   the   technique   cannot 

//   be   extended   for   the   CSplitterWnd   case. 

UINT   viewID   =   AFX_IDW_PANE_FIRST   +   1; 

CRect   rect(0,   0,   0,   0);   //   Gets   resized   later. 


//   Create   the   new   view.   In   this   example,   the   view   persists   for 

//   the   life   of   the   application.   The   application   automatically 

//   deletes   the   view   when   the   application   is   closed. 

m_pNewView-> Create(NULL,   "AnyWindowName ",   WS_CHILD,   rect,   m_pMainWnd,   viewID,   &newContext); 


//   When   a   document   template   creates   a   view,   the   WM_INITIALUPDATE 

//   message   is   sent   automatically.   However,   this   code   must 

//   explicitly   send   the   message,   as   follows. 

m_pNewView-> SendMessage(WM_INITIALUPDATE,   0,   0); 

... 

保存所做更改后继续进行下一步操作。 


实现切换函数 

在上一步中，您添加了创建和初始化新视图对象的代码。最后一步的主要工作是实现切换方法   SwitchView。 


在应用程序类   (MYWINAPP.CPP)   的实现文件的末尾，添加下列方法定义： 


CView*   CMyWinApp::SwitchView(   ) 

{ 

      CView*   pActiveView   = 

            ((CFrameWnd*)   m_pMainWnd)-> GetActiveView(); 


      CView*   pNewView=   NULL; 

      if(pActiveView   ==   m_pOldView) 

            pNewView=   m_pNewView; 

      else 

            pNewView=   m_pOldView; 


      //   Exchange   view   window   IDs   so   RecalcLayout()   works. 

      #ifndef   _WIN32 

      UINT   temp   =   ::GetWindowWord(pActiveView-> m_hWnd,   GWW_ID); 

      ::SetWindowWord(pActiveView-> m_hWnd,   GWW_ID,   ::GetWindowWord(pNewView-> m_hWnd,   GWW_ID));

      ::SetWindowWord(pNewView-> m_hWnd,   GWW_ID,   temp); 

      #else 

      UINT   temp   =   ::GetWindowLong(pActiveView-> m_hWnd,   GWL_ID); 

      ::SetWindowLong(pActiveView-> m_hWnd,   GWL_ID,   ::GetWindowLong(pNewView-> m_hWnd,   GWL_ID));

      ::SetWindowLong(pNewView-> m_hWnd,   GWL_ID,   temp); 

      #endif 


      pActiveView-> ShowWindow(SW_HIDE); 

      pNewView-> ShowWindow(SW_SHOW); 

      ((CFrameWnd*)   m_pMainWnd)-> SetActiveView(pNewView); 

      ((CFrameWnd*)   m_pMainWnd)-> RecalcLayout(); 

      pNewView-> Invalidate(); 

      return   pActiveView; 

}   

保存所做更改后继续进行下一步操作。 


添加视图切换支持 

最后一步是添加当应用程序需要在视图间切换时调用   SwitchView   方法的代码。该操作可以通过几种方式来完成：添加供用户选择的新菜单项，或在符合某些条件时从内部切换视图。 


有关添加新菜单项和命令处理函数的更多信息，请参见命令和控件通知的处理程序。 


请参见 

文档/视图结构 







m_pNewView-> Create(NULL,   "AnyWindowName ",   WS_CHILD,   rect,   m_pMainWnd,   viewID,   &newContext);中的样式再并上一个WS_BORDER，就可以了！ 

知道了吗。




