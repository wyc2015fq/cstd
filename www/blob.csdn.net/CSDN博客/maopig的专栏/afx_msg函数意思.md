# afx_msg函数意思 - maopig的专栏 - CSDN博客
2012年05月03日 09:47:40[maopig](https://me.csdn.net/maopig)阅读数：9147
应用程序框架产生的消息映射函数
例如：afx_msg void OnBnClickedButton1(); 其中 afx_msg为消息标志，它向系统声明：有消息映射到函数实现体；
而在map宏定义中，就有具体消息和此函数的映射定义（可以是自定义，也可以是系统自动完成的）
afx是 application framework
如定义一个Edit的Change处理函数：
一、在类的头文件(*.h)中增加 :
  //{{AFX_MSG(CDialogDemo)   
  afx_msg  void OnChangeEdit1();
  //{{AFX_MSG   
二、在类的实现文件(*.cpp)中增加:
1.消息定义( ON_EN_CHANGE)：
BEGIN_MESSAGE_MAP(CDialogDemo, CDialog)
//{{AFX_MSG_MAP()   
ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
 //}}AFX_MSG_MAP   
END_MESSAGE_MAP()   
2.执行函数：
void CDialogDemo::OnChangeEdit1() 
{
 // TODO: Add your control notification handler code here
……
}   
在afxwin.h中afx_msg的解释：  
  #ifndef   afx_msg  
  #define   afx_msg                   //   intentional   placeholder  
  #endif  
    没什么意思.只是定义了这个符号而已.   这个对编译器来说,相当于什么都没有,对于人来说,我们可以看到这样的符号.   
对于类向导来说.这个符号才是有意义的.它是一个消息处理函数的前缀.   类向导生成的消息函数,分发函数,事件响应函数都以这个为前缀.  
 如果去掉了,向导将不能识别
**DECLARE_MESSAGE_MAP的理解**
在一些资料上，有这么一段文字： 
**DECLARE_MESSAGE_MAP**()
说明： 
用户程序中的每个CCmdTarget派生类必须提供消息映射以处理消息。在类定义的末尾使用DECLARE_MESSAGE_MAP宏。接着，在定义类成员函数的.CPP文件中，使用BEGIN_MESSAGE_MAP宏，每个用户消息处理函数的宏项下面的列表以及END_MESSAGE_MAP宏。
注释： 
如果在DECLARE_MESSAGE_MAP之后定义任何一个成员，那么必须为他们指定一个新存取类型（公共的，私有的，保护的）。 
我觉得他描述得欠妥，我的理解是： 
只要有：只要是CCmdTarget(用于所有能够消息映射的基类)派生类,必有消息映射以处理消息，则在类的说明文件的尾部有DECLARE_MESSAGE_MAP宏，在类的定义文件中有BEGIN_MESSAGE_MAP宏和END_MESSAGE_MAP宏以处理用户消息。
其中BEGIN_MESSAGE_MAP(参数1,参数2) ,参数1为该类的类名，参数2为该类基类的类名。
其中ON_MESSAGE(参数1，参数2），参数1为响应的消息，参数2为该消息对应的处理的函数名。
BEGIN_MESSAGE_MAP(CH264PlayerDlg, CDialog)
//{{AFX_MSG_MAP(CH264PlayerDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_SIZE()
ON_BN_CLICKED(IDC_PLAY, OnPlay)
ON_COMMAND(IDR_FILE_OPEN, OnFileOpen)
ON_BN_CLICKED(IDC_STOP, OnStop)
ON_COMMAND(IDR_VIEW_FULLSCRREN, OnViewFullscrren)
ON_COMMAND(IDR_VIEW_ORIGINALSIZE, OnViewOriginalsize)
ON_WM_LBUTTONDBLCLK()
ON_BN_CLICKED(IDC_CUTPIC, OnCutPicture)
ON_COMMAND(IDR_ABOUTBOX, OnAboutbox)
ON_WM_ERASEBKGND()
ON_WM_CTLCOLOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
MFC BEGIN_MESSAGE_MAP()
### 简单用法：
　　BEGIN_MESSAGE_MAP(CpassApp, CWinApp) 
　　ON_COMMAND(ID_HELP, CWinApp::OnHelp) 
　　END_MESSAGE_MAP() 
这些都是宏定义，不是函数。 
　　在BEGIN_MESSAGE_MAP()和END_MESSAGE_MAP()之间添加你的消息响应函数,为每个消息处理函数加入一个入口 
　　BEGIN_MESSAGE_MAP( theClass, baseClass ) 
### 参数：
|theClass|指定消息映射所属的类的名字。|
|----|----|
|baseClass|指定theClass的基类的名字。|
### 说明：
　　使用BEGIN_MESSAGE_MAP宏开始你的消息映射的定义。 
　　在你的类的成员函数的实现文件（.CPP）中，使用BEGIN_MESSAGE_MAP宏开始消息映射，然后为每个消息处理函数加入一个入口，最后用END_MESSAGE_MAP宏结束消息映射。
　　每个消息映射入口的格式如下： 
　　ON_Notification(id, memberFxn) 
　　其中id指定了发送通知的控件的子窗口的ID，而memberFxn指定了处理该通知的父对象中的成员函数名。 
　　父对象的函数原型格式如下： 
　　afx_msg void memberFxn( ); 
　　可能的消息映射入口如下： 
|映射入口|何时向父对象发送消息|
|----|----|
|ON_BN_CLICKED|用户单击按钮时|
|ON_BN_DOUBLECLICKED|用户双击按钮时|
