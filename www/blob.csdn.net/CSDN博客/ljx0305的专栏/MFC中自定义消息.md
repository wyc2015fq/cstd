# MFC中自定义消息  - ljx0305的专栏 - CSDN博客
2008年03月11日 20:57:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：795
消息映射、循环机制是Windows程序运行的基本方式。VC++ MFC 中有许多现成的消息句柄，可当我们需要完成其它的任务，需要自定义消息，
就遇到了一些困难。在MFC ClassWizard中不允许添加用户自定义消息，所以我们必须手动在程序中添加相应代码，以便可以象处理其它消息一样处理自定义消息。
自定义消息的步骤如下：
（1）建立Single Document的MFC Application，工程名为：MyMessage
（2）自定义消息：
第一步：定义消息
在StdAfx.h中添加如下代码：第二步：声明消息处理函数
选择CMainFrame类中添加消息处理函数
在MainFrm.h文件中，类CMainFrame内，声明消息处理函数，代码如下:第三步：实现消息处理函数
在MainFrm.cpp文件中添加如下代码：第四步：在CMainFrame类的消息块中，使用ON_MESSAGE宏指令将消息映射到消息处理函数中如果用户需要一个定义整个系统唯一的消息,可以调用SDK函数RegisterWindowMessage定义消息:
在StdAfx.h中将代码
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)//推荐用户自定义消息至少是WM_USER+100，因为很多新控件也要使用WM_USER消息。
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)#define WM_MY_MESSAGE (WM_USER+100)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)protect:
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam); 
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)LRESULT CMainFrame::OnMyMessage(WPARAM wParam, LPARAM lParam)
![](http://www.cnblogs.com/Images/dot.gif){
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)//TODO: Add your message handle code
![](http://www.cnblogs.com/Images/OutliningIndicators/InBlock.gif)return0;
![](http://www.cnblogs.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    ON_WM_CREATE()
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    ON_MESSAGE(WM_MY_MESSAGE,OnMyMessage)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)//ON_REGISTERED_MESSAGE (WM_MY_MESSAGE,OnMyMessage)
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)END_MESSAGE_MAP()
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)#define WM_MY_MESSAGE (WM_USER+100)
替换为：
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)static UINT WM_MY_MESSAGE=RegisterWindowMessage(_T("User"));
并使用ON_REGISTERED_MESSAGE宏指令取代ON_MESSAGE宏指令,其余步骤同上。
注：如果仍然使用ON_MESSAGE宏指令，compile可以通过，但是无法响应消息。
当需要使用自定义消息时,可以在相应类中的函数中调用函数PostMessage或SendMessage发送消息PoseMessage(WM_MY_MESSAGE,O,O)。
附：RegisterWindowMessage函数说明
RegisterWindowMessage Function
--------------------------------------------------------------------------------
The RegisterWindowMessage function defines a new window message that is guaranteed to be unique throughout the system. 
The message value can be used when sending or posting messages.
Syntax lpString
 [in] Pointer to a null-terminated string that specifies the message to be registered.
Return Value
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif) UINT RegisterWindowMessage(          LPCTSTR lpString
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)    );
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)Parameters
If the message is successfully registered, the return value is a message identifier in the range 0xC000 through 0xFFFF.
If the function fails, the return value is zero. To get extended error information, call GetLastError.
Remarks
The RegisterWindowMessage function is typically used to register messages for communicating between two cooperating applications. 
If two different applications register the same message string, the applications return the same message value. 
The message remains registered until the session ends. 
//如果两个Application使用相同的string注册message，他们将等到相同的message值，也就是得到相同的message
 引用:http://www.cnblogs.com/xulei/archive/2007/11/22/968170.html            
