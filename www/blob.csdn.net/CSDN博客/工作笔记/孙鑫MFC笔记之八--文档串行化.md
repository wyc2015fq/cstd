# 孙鑫MFC笔记之八--文档串行化 - 工作笔记 - CSDN博客





2012年06月20日 08:54:27[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5912








1.CArchive在菜单打开保存时的代码

CFile file("1.txt",CFile::modeCreate | CFile::modeWrite);

CArchive ar(&file,CArchive::store);

int i=4;

char ch='a';

float f=1.3f;

CString str("[http://www.sunxin.org](http://www.sunxin.org/)");

ar<<i<<ch<<f<<str;以上是保存，打开略

CArchive类主要是将内存中的对象，保存在永久设备中，程序关闭后再次启动，可以直接从设备中读取。让对象保持持久性的过程称之为串行化。该类没有基类，它重载了提取和插入操作符，可以<<或>>任何数据类型。

注意点：

     1 在用CArchive前，必须构造一个文件或者派生文件类的对象。且一个文件对象，只能用一个活动的CArchive对象管理。



2.文档-视类结构简介

OnNewDocument是MFC提供的文件新建命令处理一部分，它是由框架类调用的虚函数。

   OnNewDocument在程序启动时被调用，此时可设置文档标题（settitle），也可以在String Table的IDR_MAINFRAME的第二个"\"后改变文档的标题。

   在WinAPP的InitInstance（）中完成DOC,View,MainFrame的归一。
  每当新建一个文档，都会产生一个视类对象，框架类对象，文档类对象，三位一体，为文档服务。

![](https://img-my.csdn.net/uploads/201206/20/1340158532_8307.jpg)







       当点击系统的打开和新建菜单时，有一系列的步骤，孙鑫老师给我们跟踪了代码的调用过程，此段跟踪我们略过。但我们要牢记住：CWinAPP负责管理文档管理器，文档管理器有一个指针链表，且来保存文档模板的指针，文档模板指针管理三个类DOC，VIEW，MAINFRAME，使其为某文件对象服务。



3.利用CArchive来保存一个类的对象，此类必须支持串行化，该类可串行化需要5个步骤。

  a.让类从CObject派生；

  b.覆盖Serialize()函数，在其中完成保存和读取功能；

  c.在类申明的.h中加入 DECLARE_SERIAL(CGraph)；参数为想成为可串行化的类类名

  d.定义一个不带参数的构造函数。

  e.在类实现的.cpp中加入IMPLEMENT_SERIAL(CGraph, CObject, 1 )；第三个参数为读取数据时，与所保存的数据的版本号。相同，加载成功，不同，加载失败。


保存绘画数据到文件的简单过程

  a.在CGraph中增加一个画图的成员函数，其实不增加也行。可以在View中完成相应功能。

  b.增加四个画图菜单，菜单可以从11课的代码中拷贝。

  c.在View中增加LButtonDown和UP的响应，在UP中画图，在DOWN中保存点

  d.利用CObArray集合类来保存绘画数据

  e.在CGraphicDOC::Serialize()中保存和读取数据

  f.然后在OnDraw中重绘。



n在MFC中，文档类负责管理数据，提供保存和加载数据的功能。视类负责数据的显示，以及给用户提供对数据的编辑和修改功能。

nMFC给我们提供Document/View结构，将一个应用程序所需要的“数据处理与显示”的函数空壳都设计好了，这些函数都是虚函数，我们可以在派生类中重写这些函数。有关文件读写的操作在CDocument的Serialize函数中进行，有关数据和图形显示的操作在CView的OnDraw函数中进行。我们在其派生类中，只需要去关注Serialize和OnDraw函数就可以了，其它的细节我们不需要去理会，程序就可以良好地运行。

n当我们按下“File/Open”，Application Framework会激活文件打开对话框，让你指定文件名，然后自动调用CGraphicDoc::Serialize读取文件。Application Framework还会调用CGraphicView::OnDraw，传递一个显示DC，让你重新绘制窗口内容。

nMFC给我们提供Document/View结构，是希望我们将精力放在数据结构的设计和数据显示的操作上，而不要把时间和精力花费在对象与对象之间、模块与模块之间的通信上。

n一个文档对象可以和多个视类对象相关联，而一个视类对象只能和一个文档对象相关联。





4.新建和打开文档时，要注意销毁原来的数据。在DOC的DeleteContents虚函数中是好时机。代码如下

int nCount;

nCount=m_obArray.GetSize();

/*for(int i=0;i<nCount;i++)

{

  delete m_obArray.GetAt(i);//释放指针指向的内存空间

  //m_obArray.RemoveAt(i);//移除链表中的元素。嘿嘿，别搞错了。但在此处不能这样用，会导致非法操作。要用下面的方法沙

}

m_obArray.RemoveAll();*/

while(nCount--)

{

  delete m_obArray.GetAt(nCount);

  m_obArray.RemoveAt(nCount);

}






