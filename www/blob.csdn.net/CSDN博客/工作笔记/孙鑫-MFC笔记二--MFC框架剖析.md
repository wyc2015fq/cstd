# 孙鑫-MFC笔记二--MFC框架剖析 - 工作笔记 - CSDN博客





2012年05月02日 16:25:20[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5673








    MFC，Microsoft Foundation Class，微软基础类库，是微软为了简化程序员的开发工作所开发的一套C++类集合，是一套面向对象的函数库，以类的方式提供给用户使用。

    实际上，MFC同样也有WinMain函数，只是这个WinMain函数被微软封装，同时，也封装了每一个窗口都需要的步骤。

    在程序编译连接时，WinMain函数成为了程序的一部分。然而，MFC是如何组织起各个类的呢？

    这主要是通过CWinApp类的对象。

   应用程序的实例是由实例句柄来标识，对于MFC，通过产生一个应用程序类的对象来唯一表示应用程序实例、每个MFC程序，有且仅有一个从应用程序类派生的类，有且仅有一个该派生类的实例化对象。即theApp全局对象。由于该对象是全局的，因此，在WinMain函数前，它已经完成初始化工作。其初始化工作由类的构造函数完成。

    对于我们自己的应用程序，都会有且仅有一个CXXXAPP类，这个类派生于CWinApp类。于是，在CXXXAPP类生成一个全局对象theApp后，该对象要构造，则首先调用基类的构造函数，即CWinApp类的构造函数，然后，再调用子类的构造函数，即，它自身的构造函数，最后，构造出该全局对象。 

   然而，通过查询可知，我们自己的应用程序类，含有无参默认构造函数，而CWinApp类含有一个缺省的构造函数，因此，我们无需传递参数，显式调用基类构造函数。

   根据基类构造函数this指针，我们的theApp构造完成，在WinMain函数中的指针pThread，pApp就有了值。

MFC大致流程：

   实际上，WinMain函数功能由AfxWinMain函数完成，AFX表示应用程序框架函数，都是全局函数。

1 重新获取了指向theApp的指针，pThread和pApp所指内容一致。

CWinThread* pThread = AfxGetThread();

CWinApp* pApp = AfxGetApp(); 

2 pApp调用InitApplication()函数，完成MFC内部管理工作。

3 pThread->InitInstance() 完成设计窗口类，注册窗口类，创建窗口，显示窗口，更新窗口

4  nReturnCode = pThread->Run(); 消息循环



详细流程：

设计和注册窗口类

   MFC实际已经预先设计好了一些缺省的标准窗口类，只需要选择注册就可以了。注册窗口类，用函数AfxEndDeferRegisterClass。该函数根据窗口类型，选择相应的类名，这些类名都是MFC预先定义好的，之后，调用AfxRegisterClass函数注册窗口类。后者根据类的信息，判定是否已经注册，已经注册，返回真，否则，注册失败。

  实际上，MFC程序还有个框架窗口。该窗口注册前，先通过PreCreateWindow，调用AfxEndDeferRegisterClass，后者实际上是一个指向AfxEndDefer-RegisterClass函数的宏。根据程序调试可以知道。MFC执行过程也是在全局对象前，WindMain函数后，窗口产生前注册窗口类的。

但单文档中，窗口类的注册提前了，牵扯到文档管理，没有经过PreCreatWindow。

创建窗口：

   MFC的窗口创建，是通过CreateEx函数实现，该函数在CWnd类中，在底层代码中，CFrameWnd类的Create函数，调用了CWnd类中的CreateEx函数，而CFrameWnd派生于CWnd类。同时，Create函数，由CFrameWnd类的LoadFrame函数调用。由于CreateEx即不是虚函数，也没有被重写，因此，Create调用的就是基类CWnd的CreateEx函数。也就是说，我们应用程序调用的CreateEx函数，就是CWnd的成员函数。它内部实现时，又调用了PreCreateWindow虚函数。因此，这里实际就是调用子类的虚函数。在这里通过调用该函数，让程序员修改窗口外观，利用引用结构体。修改了子类窗口外观，实际也就是修改了底层窗口外观。

  MFC中的Ex后缀，表示扩展函数。

显示和更新窗口：

在应用程序类中，有一个继承而来的成员变量。它是CWnd类型指针，保存了应用程序框架窗口对象。即，指向CMainFrame对象的的指针。通过调用显示更新函数，显示更新窗口。

 m_pMainWnd->ShowWindow(SW_SHOW);

 m_pMainWnd->UpdateWindow();

消息循环

  nReturnCode = pThread->Run(); 消息循环

  窗口过程函数：

  消极映射机制



在BOOL CTestApp::InitInstance()中的代码

CSingleDocTemplate* pDocTemplate;

pDocTemplate = new CSingleDocTemplate(

  IDR_MAINFRAME,

  RUNTIME_CLASS(CTestDoc),

  RUNTIME_CLASS(CMainFrame),       // 主框架类

  RUNTIME_CLASS(CTestView));

AddDocTemplate(pDocTemplate);

完成了将这三个类关联起来的工作。



