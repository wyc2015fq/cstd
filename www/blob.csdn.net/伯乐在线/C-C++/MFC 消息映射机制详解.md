# MFC 消息映射机制详解 - 文章 - 伯乐在线
原文出处： [Ocean2006](http://blog.csdn.net/ocean2006/article/details/5498265)
Windows程序和MFC程序是靠消息驱动的，他们对于消息的处理本质上是相同的。只是Windows程序对于消息处理的过程十分清晰明了，MFC程序则掩盖了消息处理的过程，以消息映射的方式呈现在开发者面前，使得开发消息的处理十分简单。用多了mfc就想对它的消息映射机制有一个本质的了解，下面将对消息映射做详细的分析。当然，在分析MFC消息映射之前首先对Windows程序的消息处理过程进行一个简单的描述。
**1、Windows应用程序消息处理**
Windows程序都维护有自己的消息队列，保存了队列消息（当然也有非队列消息，它们直接发给窗口），并用过消息循环对消息进行处理。消息循环首先通过GetMessage取得消息并从队列中移走，对于加速键会调用TranslateAccelerator函数，对其进行翻译和处理，如果处理成功就不在调用TranslateMessage。如果不是加速键，就进行消息的转换和派发，让目的窗口的窗口过程来处理消息。示例代码：


```
// 主消息循环:  
 while (GetMessage(&msg, NULL, 0, 0))  
 {  
   if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))  
   {  
      TranslateMessage(&msg);  
      DispatchMessage(&msg);  
   }  
 }
```
真正处理消息的是所谓的窗口过程（LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)），这个函数的参数记录了过程对应的窗口、消息的ID以及参数，在其内部开发者可以实现自己需要的消息处理功能。那消息分发是如何发送给窗口过程的呢？我们知道窗口创建过程中有一个注册窗口类的步骤，如下：


```
ATOM MyRegisterClass(HINSTANCE hInstance)  
{  
   WNDCLASSEX wcex;  
  
   wcex.cbSize = sizeof(WNDCLASSEX);  
  
   wcex.style   = CS_HREDRAW | CS_VREDRAW;  
   wcex.lpfnWndProc = WndProc;   // 啊！！！原来在这里  
   wcex.cbClsExtra  = 0;  
   wcex.cbWndExtra  = 0;  
   wcex.hInstance  = hInstance;  
   wcex.hIcon   = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINDOWSP));  
   wcex.hCursor  = LoadCursor(NULL, IDC_ARROW);  
   wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);  
   wcex.lpszMenuName = MAKEINTRESOURCE(IDC_WINDOWSP);  
   wcex.lpszClassName = szWindowClass;  
   wcex.hIconSm  = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));  
  
   return RegisterClassEx(&wcex);  
}
```
相信你看到这段代码就会立即明了！
**2、MFC消息映射**
MFC窗口使用同一窗口过程，通过消息映射隐藏了消息处理的过程，更加详细点是隐藏了，那消息映射如何实现的呢？
首先，我们先对MFC的消息映射做一个简单介绍。MFC为了实现消息映射在响应消息的类内部自动做了如下两方面的处理：
a、消息映射声明和实现
在类的定义（头文件）里，添加声明消息映射的宏DECLARE_MESSAGE_MAP，在类的实现（源文件）里，通过BEGIN_MESSAGE_MAP和END_MESSAGE_MAP()实现消息映射。
b、消息响应函数的声明和实现
当通过ClassWizard添加消息响应函数时就会自动添加函数的声明和实现，代码如下：
声明：


```
//{{AFX_MSG  
afx_msg void OnTimer(UINT nIDEvent);  
afx_msg void OnPaint();  
//}}AFX_MSG  
DECLARE_MESSAGE_MAP()
```
映射：


```
BEGIN_MESSAGE_MAP(CTestDialog, CDialog)  
//{{AFX_MSG_MAP(CTestDialog)  
ON_WM_TIMER()  
ON_WM_PAINT()  
//}}AFX_MSG_MAP  
END_MESSAGE_MAP()
```
实现：


```
void CTestDialog::OnPaint()   
{  
}  
  
void CTestDialog::OnTimer(UINT nIDEvent)   
{     
    CDialog::OnTimer(nIDEvent);  
}
```
仅仅这些工作就能实现对消息处理的简化吗？当然，我们需要对这几步有更深入的探讨！首先，需要了解的是消息映射的声明和实现。消息映射声明的代码如下：


```
#define DECLARE_MESSAGE_MAP()   
protected:  
    static const AFX_MSGMAP* PASCAL GetThisMessageMap(); // 获得当前类和基类的映射信息  
    virtual const AFX_MSGMAP* GetMessageMap() const;     // 实际上调用了上一个函数
```
消息映射实现的代码如下：


```
#define BEGIN_MESSAGE_MAP(theClass, baseClass)              // 消息映射开始  
     PTM_WARNING_DISABLE                                   // pragma宏的处理，无关系  
     const AFX_MSGMAP* theClass::GetMessageMap() const     // 获得自身和基类的函数映射表   
     { return GetThisMessageMap(); }                        // 入口地址  
     const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() // 获得自身函数映射表入口地址  
     {  
        typedef theClass ThisClass;                         // 当前类  
    typedef baseClass TheBaseClass;                     // 基类  
    static const AFX_MSGMAP_ENTRY _messageEntries[] =    // 当前类信息实体数组，记录了  
    {                                                     // 该类所有的消息实体  
        // 该行之所以空出来，是因为所有的消息都要写在这里  
#define END_MESSAGE_MAP() /                                   // 映射消息的结束，也是消息实  
        {0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 }      // 体的最后一个元素，标志结束  
    }; 
    static const AFX_MSGMAP messageMap =                 // 消息映射变量（包含基类）  
    { &TheBaseClass::GetThisMessageMap, &_messageEntries[0] };  
       return &messageMap;                                  // 返回消息变量   
     } 
     PTM_WARNING_RESTORE                                  // pragma宏的处理，无关系
```
对于消息映射声明和实现需要特别说明四点：
**a、静态变量：消息映射实体数组AFX_MSGMAP_ENTRY _messageEntries[] ——记录了当前类的所有消息映射**。每一个消息是一个数组成员，AFX_MSGMAP_ENTRY 的定义如下：


```
struct AFX_MSGMAP_ENTRY  
{  
    UINT nMessage;   // windows message  
    UINT nCode;      // control code or WM_NOTIFY code  
    UINT nID;        // control ID (or 0 for windows messages)  
    UINT nLastID;    // used for entries specifying a range of control id's  
    UINT_PTR nSig;   // signature type (action) or pointer to message #  
    AFX_PMSG pfn;    // routine to call (or special value)  
};
```
从上述结构可以看出，每条映射有两部分的内容：第一部分是关于消息ID的，包括前四个域；第二部分是关于消息对应的执行函数，包括后两个域，pfn是一个指向CCmdTarger成员函数的指针。函数指针的类型定义如下：
typedef void (AFX_MSG_CALL CCmdTarget::*AFX_PMSG)(void);
当使用一条或者多条消息映射条目初始化消息映射数组时，各种不同类型的消息函数都被转换成这样的类型：不接收参数，也不返回参数的类型。因为所有可以有消息映射的类都是从CCmdTarge派生的，所以可以实现这样的转换。nSig是一个标识变量，用来标识不同原型的消息处理函数，每一个不同原型的消息处理函数对应一个不同的nSig。在消息分发时，MFC内部根据nSig把消息派发给对应的成员函数处理，实际上，就是根据nSig的值把pfn还原成相应类型的消息处理函数并执行它。
**b、静态变量：消息映射信息变量AFX_MSGMAP messageMap——记录了当前类和基类的消息映射实体数组的入口地址。**AFX_MSGMAP结构的定义如下：


```
struct AFX_MSGMAP  
{  
    const AFX_MSGMAP* (PASCAL* pfnGetBaseMap)(); // 基类消息映射入口地址  
    const AFX_MSGMAP_ENTRY* lpEntries;           // 当前类消息映射入口地址  
};
```
**c、消息映射实体数组**：从BEGIN_MESSAGE_MAP和END_MESSAGE_MAP宏定义来看，用户添加的消息映射实体会自动加入到_messageEntries数组中，在这里实现了对消息映射实体数组的初始化。
**d、虚函数GetMessageMap**：之所以设置成虚函数，就是为了实现多态，使当前类和基类能够调用正确的消息映射实体数组。
通过上面对消息映射宏的解析，我可以清晰的了解到**三个宏通过两个静态变量把类和基类、把消息和对应的消息处理函数关联起来**，这种关联保证了消息处理的顺序（当前类->基类），保证了消息能够正确的找到对应的函数。
讲到这里，总感觉少点什么。仔细想想这种映射是有啦，但是什么激发了这样映射（就像windows程序的窗口过程是有啦，是谁调用了这个过程使得每一个消息都能够实现自己的功能、达到想要的目的呢？）？——**消息循环**。通过消息循环派发消息到窗口，窗口类的会调用有关函数查找消息映射实体数组，首先查找当前类的再查找基类的，查找到后就会调用相应的消息处理函数，如果没有查到相应的处理函数程序会自动调用默认处理函数！
至于MFC具体是怎样实现消息的派送的，请参考：[http://blog.csdn.net/linzhengqun/archive/2007/11/28/1905671.aspx](http://blog.csdn.net/linzhengqun/archive/2007/11/28/1905671.aspx)。这篇文章清晰的阐释了MFC消息分发的原理和消息的部分传递过程！
MFC的完整传递过程（从消息循环开始到消息处理函数）有待以后熟悉，毕竟MFC封装的太好以至于不那么好理解，同时给予文档的应用程序和基于对话框的应用程序也是不一样。
