# MFC框架简介 - fanyun的博客 - CSDN博客
2016年09月30日 09:07:32[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：527
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
MFC框架简介
一、MFC框架
        MFC库是开发Windows应用程序的C++接口。MFC提供了面向对象的框架，采用面向对象技术，将大部分的Windows API 封装到C++类中，以类成员函数的形式提供给程序开发人员调用。
![](https://img-blog.csdn.net/20160929092058340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
[](http://img238.ph.126.net/4XrDNANbI2HdKfJMDoldBw==/1424826332111042115.png)
        CWnd类是MFC中一个非常重要的类，它封装了与窗口相关的操作。
在前面中，曾经有构建一个Windwos程序的几个部分：
[http://blog.163.com/zhoumhan_0351/blog/static/3995422720103401415721](http://blog.163.com/zhoumhan_0351/blog/static/3995422720103401415721)       MFC中的WinMain函数实际上是调用AfxWinMain函数来实现的。在VC给我们MFC向导生成的代码中，我们发现，在AfxWinMain函数中，调用了三个函数：
1、pApp->InitApplication()//完成内存管理方面的工作
2、pApp->InitInstance()//此函数是虚函数，调用派生类的InitInstance函数，完成窗口类的创建，注册，更新等操作。窗口类的注册是由AfxEndDeferRegisterClass完成。如果该窗口类已经注册，则直接返回一个真值;如果尚未注册，就调用RegisterClass函数注册该窗口类。
        其中，函数PreCreateWindow是为程序员修改窗口类参数准备的。其参数是CREATESTRUCT的引用，如果其值发生了变化，在调用CreateWindowEx函数时，其参数会发生相应的改变。
       在MFC中，窗口的创建功能由Cwnd类的CreateEx函数实现（从父类继承来的，调用CWnd::CreateEx()）。
3、pApp->Run()//完成消息循环。
默认的窗口过程函数采用消息映射机制来处理各种消息的。
二、MFC程序的运行过程如下：
1）利用全局应用程序对象theApp启动应用程序。
2）调用全局应用程序对象的构造函数，从而调用基类（CWinApp）的构造函数,完成应用程序的一些初始化工作，并将应用程序对象的指针保存起来。
3）进入WinMain函数。在AfxWinMain函数中获取子类的指针，利用指针实现上述的三个函数，从而完成窗口的创建注册等工作。
4）进入消息循环，一直到WM_QUIT。
说明：
1、this指针
在CWinApp::CWinApp函数的代码时，有：
pThreadState->m_pCurrentWinThread = this;那么this到底是什么？
我们先看看MSDN中this的定义：
        The this pointer is a pointer accessible onlywithin the member functions of a class, struct, or union type. It points to theobject for which the member function is called. Static member functions do nothave a this pointer. When a nonstatic member
 function is called for an object,the address of the object is passed as a hidden argument to the function. Forexample, the following function call.
     （译文：this指针是仅在类，结构或联合类型的成员函数访问的指针。它指向该成员函数被调用的对象。静态成员函数没有this指针。当一个非静态成员函数被调用的对象，该对象的地址被作为隐藏的参数传递给函数。例如，下面的函数调用。
）
      由上定义可见，this指针所指的应当是对象，而与类没有必然的关联关系。也就是说，在调用对象的成员函数时，将指向当前对象的指针一起传递给了成员函数，如
myDate.setMonth( 3 );//can be interpreted this way:setMonth( &myDate, 3 );
       而不管是在基类中（通过指针）调用子类的成员函数，还是在派生类中调用其成员函数。这样，如果是定义了派生类的对象，而通过基类的构造函数进行初始化与基类相关的部分时，实质上是派生类对象调用基类的构造函数在初始化，很明显的，会将派生类对象的指针传给基类。
       而在举例当时，很明显的，定义的是派生类对象：
CTestApp theApp;
       所以，在调用基类的构造函数初始化基类时，传给基类的指针是指向派生类的；而如果定义的是基类的对象，那么this指针就应当是指向基类对象了。
2、无论是全局变量，还是全局对象，程序在运行时，在加载main函数之前，就已经为全局变量或全局对象分配了内存空间。
3、对MFC程序来说，通过产生一个应用程序类的对象来惟一标识应用程序的实例。每一个MFC程序有且仅有一个从应用程序类(CWinApp)派生的类。每一个MFC程序实例有且仅有一个该派生类的实例化对象，也就是theApp全局对象。该对象就表示了应用程序本身。
4、Afx前缀的函数代表应用程序框架（Application Framework）函数。应用程序框架（Application Framework）辅助我们生成应用程序的框架模型，该模型把多个类进行了有机的集成。在MFC中，以Afx为前缀的函数都是全局函数。后缀Ex的函数都是扩展函数。
5、MicroSoft在设计类库时，采用文档/视结构。数据的存储和加载由文档类来完成，数据的显示和修改由视类来完成。
6、::开始表示全局方法。
7、C++窗口类对象与窗口并不是一回事，它们之间惟一的关系就是C++窗口类对象内部定义了一个窗口句柄变量，保存了与这个C++窗口类对象相关的那个窗口的句柄。窗口销毁时，与之对应的C++窗口类对象销毁与否，要看其生命周期是否结束，但C++窗口类对象销毁时，与之相关的窗口也将销毁。窗口创建时都会产生WM_CREATE消息。
8、文件存放
1）MFC中的WinMain函数在APPMODUL.cpp中，在编译的时候才会连接进来。
2）CWinApp在文件appcore.cpp中。
3）WinMain函数实际上是调用AfxWinMain函数来实现的。AfxWinMain位于WINMAIN.cpp中。
4）在MFC程序中，窗口的创建功能由Cwnd类的CreateEx函数实现（被位于WINFRM.CPP当中的Create调用，注意CreateEx不是虚函数），该函数的声明位于AFXWin.h文件中。实现代码位于WINCORE.CPP文件中。注册函数AfxEndDeferRegisterClass也在这个文件里。
5）CWinThread类的Run函数定义位于THRDCORE.CPP中。
6）CFrameWnd的PreCreateWindow函数位于WINFRM.CPP中。
