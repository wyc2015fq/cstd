# 在MFC类中获取别的类的指针 - ljx0305的专栏 - CSDN博客
2010年03月03日 23:00:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：444标签：[mfc																[文档																[null																[class																[框架																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[MFC																[C++](https://blog.csdn.net/ljx0305/article/category/380566)](https://blog.csdn.net/ljx0305/article/category/403913)
======================================================
     MFC应用程序中指针的使用
======================================================
      1)       在View中获得Doc指针     
      CYouSDIDoc       *pDoc       =       GetDocument();一个视只能有一个文档。   
      2)       在App中获得MainFrame指针   
      CWinApp       中的       m_pMainWnd变量就是MainFrame的指针   
      也可以：     
      CMainFrame       *pMain       =       (CMainFrame*)AfxGetMainWnd();   
      3)       在View中获得MainFrame指针     
      CMainFrame       *pMain       =       (CMaimFrame       *)AfxGetApp()->m_pMainWnd;   
      4)       获得View（已建立）指针     
      CMainFrame       *pMain       =       (CMaimFrame       *)AfxGetApp()->m_pMainWnd;   
      CYouView       *pView       =       (CYouView       *)pMain->GetActiveView();   
      5)       获得当前文档指针     
      CDocument       *       pCurrentDoc       =   
      (CFrameWnd       *)m_pMainWnd->GetActiveDocument();   
      6)       获得状态栏与工具栏指针     
      CStatusBar       *       pStatusBar       ＝(CStatusBar       *)   
      AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR);   
      CToolBar       *       pToolBar       =       (CtoolBar       *)   
      AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_TOOLBAR);   
      7)       如果框架中加入工具栏和状态栏变量还可以这样     
      (CMainFrame       *)GetParent()->m_wndToolBar;   
      (CMainFrame       *)GetParent()->m_wndStatusBar;   
      8)       在Mainframe获得菜单指针     
      CMenu       *pMenu       =       m_pMainWnd->GetMenu();   
      9)       在任何类中获得应用程序类   
      用MFC全局函数AfxGetApp()获得。   
      10)       从文档类取得视图类的指针   
      我是从[http://download.cqcnc.com/soft/program/article/vc/vc405.html](http://download.cqcnc.com/soft/program/article/vc/vc405.html)
      学到的，从文档获得视图类指针目的一般为了控制同一文档的多个视图的定位   
      问题，我的体会特别是文字处理CEditView当产生多个视图类时，这个功能是   
      非常需要的。     
      CDocument类提供了两个函数用于视图类的定位：   
      GetFirstViewPosition()和GetNextView()     
      virtual       POSITION       GetFirstViewPosition()       const;   
      virtual       CView*       GetNextView(POSITION&       rPosition)       const;   
      注意：GetNextView()括号中的参数用的是引用方式，因此执行后值可能改变。   
      GetFirstViewPosition()用于返回第一个视图位置（返回的并非视图类指针，   
      而是一个POSITION类型值），GetNextView()有两个功能：返回下一个视图类   
      的指针以及用引用调用的方式来改变传入的POSITION类型参数的值。很明显，   
      在Test程序中，只有一个视图类，因此只需将这两个函数调用一次即可得到   
      CTestView的指针如下（需定义一个POSITION结构变量来辅助操作）：     
      CTestView*       pTestView;   
      POSITION       pos       =       GetFirstViewPosition();   
      pTestView       =       GetNextView(pos);   
      这样，便可到了CTestView类的指针pTestView.执行完几句后，变量pos=NULL,   
      因为没有下一个视图类，自然也没有下一个视图类的POSITION.但是这几条语句   
      太简单，不具有太强的通用性和安全特征；当象前面说的那样，当要在多个视图   
      为中返回某个指定类的指针时，我们需要遍历所有视图类，直到找到指定类为止。   
      判断一个类指针指向的是否某个类的实例时，可用IsKindOf()成员函数时行检查，   
      如：     
      pView->IsKindOf(RUNTIME_CLASS(CTestView));   
      即可检查pView所指是否是CTestView类。   
      有了以上基础，我们已经可以从文档类取得任何类的指针。为了方便，我们将   
      其作为一个文档类的成员函数，它有一个参数，表示要获得哪个类的指针。实   
      现如下：     
      CView*       CTestDoc::GetView(CRuntimeClass*       pClass)   
      {   
      CView*       pView;   
      POSITION       pos       =       GetFirstViewPosition();   
      while       (pos!       =       NULL)   
      {   
      pView       =       GetNextView(pos);   
      if(pView->IsKindOf(pClass))       //找到了
      break;   
      }   
      if       (!pView->IsKindOf(pClass))   
      {   
      AfxMessageBox("Connt       Locate       the       View./r/n");   
      return       NULL;   
      }   
      return       pView;   
      }   
      其中用了两次视图类的成员函数IsKindOf()来判断，是因为退出while循环有三   
      种可能：   
      1.pos为NULL，即已经不存在下一个视图类供操作；   
      2.pView已符合要求。   
      1和2同是满足。这是因为GetNextView()的功能是将当前视图指针改变成一个视   
      图的位置同时返回当前视图指针，因此pos是pView的下一个视图类的POSITION,   
      完全有可能既是pos==NULL又是pView符合需要。当所需的视图是最后一个视图   
      是最后一个视图类时就如引。因此需采用两次判断。   
      使用该函数应遵循如下格式（以取得CTestView指针为例）：   
      CTestView*       pTestView       =       (CTestView*)GetView(RUNTIME_CLASS(CTestView));   
      RUNTIME_CLASS是一个宏，可以简单地理解它的作用：将类的名字转化为   
      CRuntimeClass为指针。   
      至于强制类型转换也是为了安全特性考虑的，因为从同一个基类之间的指针类型   
      是互相兼容的。这种强制类型转换也许并不必要，但能避免一些可能出现的麻烦。   
      3.从一个视图类取得另一视图类的指针       综合1和2，很容易得出视图类之间互相获   
      得指针的方法：就是用文档类作中转，先用1的方法得到文档类的指针，再用2的方   
      法，以文档类的视图定位函数取得另一个视图类。同样，可以实现成一个函数：   
      （假设要从CTestAView中取得指向其它视图类的指针）   
      CView*       CTestAView::GetView(CRuntimeClass*       pClass)   
      {   
      CTestDoc*       pDoc       =       (CTestDoc*)GetDocument();   
      CView*       pView;   
      POSITION       pos       =       pDoc->GetFirstViewPosition();   
      while       (pos       !=       NULL)   
      {   
      pView       =       pDoc->GetNextView(pos);   
      if       (!pView->IsKindOf(pClass))   
      break;   
      }   
      if       (!pView->IsKindOf(pClass))   
      {   
      AfxMessageBox("Connt       Locate       the       View.");   
      return       NULL;   
      }   
      return       pView;   
      }   
      这个函数和2中的GetView()相比，一是多了第一句以取得文档类指针，二是在   
      GetFirstViewPosition()和GetNextView()前加上了文档类指针，以表示它们是   
      文档类成员函数。   
      有了此函数；当要从CTestAView中取得CTestBView的指针时，只需如下：   
      CTestBView*       pTestbView=(CTestView*)GetView(RUNTIME_CLASS(CTestBView));   
      11）对于单文档中也可以加入多个文档模板，但是一般的开发就使用MDI方式开发   
      多文档模板，其方法与上述视图的获取方法很接近，这里稍做解释，如果不清楚，   
      请查阅MSDN    
      可以用CWinApp::GetFirstDocTemplatePostion获得应用程序注册的第一个文档   
      模板的位置；   
      利用该值来调用CWinApp::GetNextDocTemplate函数，获得第一个CDocTemplate   
      对象指针。   
      POSITION       GetFirstDocTemplate(       )       const;     
      CDocTemplate       *GetNextDocTemplate(       POSITION       &       pos       )       const;   
      第二个函数返回由pos       标识的文档模板。POSITION是MFC定义的一个用于迭代或   
      对象指针检索的值。通过这两个函数，应用程序可以遍历整个文档模板列表。如   
      果被检索的文档模板是模板列表中的最后一个，则pos参数被置为NULL。   
      12）一个文档模板可以有多个文档，每个文档模板都保留并维护了一个所有对应   
      文档的指针列表。     
      用CDocTemplate::GetFirstDocPosition函数获得与文档模板相关的文档集合中   
      第一个文档的位置，并用POSITION值作为CDocTemplate::GetNextDoc的参数来重   
      复遍历与模板相关的文档列表。函数原形为：     
      viaual       POSITION       GetFirstDocPosition(       )       const       =       0;     
      visual       CDocument       *GetNextDoc(POSITION       &       rPos)       const       =       0;     
      如果列表为空，则rPos被置为NULL.     
      13）在文档中可以调用CDocument::GetDocTemplate获得指向该文档模板的指针。   
      函数原形如下：     
      CDocTemplate       *       GetDocTemplate       (       )       const;     
      如果该文档不属于文档模板管理，则返回值为NULL。     
      14)一个文档可以有多个视。每一个文档都保留并维护一个所有相关视的列表。   
      CDocument::AddView将一个视连接到文档上，将该视加入到文档相联系的视的列   
      表中，并将视的文档指针指向该文档。当有File/New、File/Open、Windows/New   
      或Window/Split的命令而将一个新创建的视的对象连接到文档上时，       MFC会自动   
      调用该函数，框架通过文档/视的结构将文档和视联系起来。当然，程序员也可   
      以根据自己的需要调用该函数。     
      Virtual       POSITION       GetFirstViewPosition(       )       const;     
      Virtual       CView       *       GetNextView(       POSITION       &rPosition)       cosnt;     
      应用程序可以调用CDocument::GetFirstViewPosition返回与调用文档相联系的视   
      的列表中的第一个视的位置，并调用CDocument::GetNextView返回指定位置的视，   
      并将rPositon的值置为列表中下一个视的POSITION值。如果找到的视为列表中的   
      最后一个视，则将rPosition置为NULL.     
      15)从一个视图类取得另一视图类的指针这个应用在多视的应用程序中很多见，一   
      般如果自己在主程序或者主框架中做好变量记号，也可以获得，还有比较通用的   
      就是用文档类作中转，以文档类的视图遍历定位，取得另一个视图类。这个功能   
      从本文第10项中可以得到。 
