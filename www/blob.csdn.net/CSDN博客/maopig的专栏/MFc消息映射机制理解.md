# MFc消息映射机制理解 - maopig的专栏 - CSDN博客
2012年05月08日 10:35:09[maopig](https://me.csdn.net/maopig)阅读数：1297
何谓消息、消息处理函数、消息映射？
消息简单的说就是指通过输入设备向程序发出指令要执行某个操作。具体的某个操作是你的一系列代码。称为消息处理函数。在SDK中消息其实非常容易理解，当窗口建立后便会有一个函数（窗口处理函数）开始执行一个消息循环，我们还可以清楚的看到消息处理的脉络。一个switch case语句就可以搞定，消息循环直到遇到WM_QUIT消息才会结束，其余的消息均被拦截后调用相应的处理函数。但在封装了API的MFC中，消息似乎变的有些复杂了，我们看不到熟悉的switch case语句了，取而代之的是一个叫消息映射的东西。为什么MFC要引入消息映射机制，你可以想象一下，在现在的程序开发活动中，你的一个程序是否拥有多个窗体，主窗口就算只有一个，那菜单、工具条、控件这些都是子窗口，那我们需要写多少个switch
 case，并且还要为每个消息分配一个消息处理函数，这样做是多么的复杂呀。因此MFC采用了一种新的机制。利用一个数组，将窗口消息和相对应的消息处理函数进行映射，你可以理解成这是一个表。这种机制就是消息映射。这张表在窗口基类CWnd定义，派生类的消息映射表如果你没有动作它是空的，也就是说如果你不手工的增加消息处理函数，则当派生窗口接受一个消息时会执行父类的消息处理函数。这样做显然是高效的。
MFC提供的消息结构
同时MFC定义了下面的两个主要结构:
AFX_MSGMAP_ENTRY
struct AFX_MSGMAP_ENTRY{
UINT nMessage;   // Windows消息的ID号
UINT nCode;  // 控制消息的通知
UINT nID;    // Windows控制消息的ID
UINT nLastID;   //表示是一个指定范围的消息被映射的范围
UINT nSig;  //表示消息的动作标识
AFX_PMSG pfn;    // 指向消息处理函数的指针
};
AFX_MSGMAP
struct AFX_MSGMAP{
#ifdef _AFXDLL
const AFX_MSGMAP* (PASCAL* pfnGetBaseMap)();
#else
const AFX_MSGMAP* pBaseMap;
#endif
const AFX_MSGMAP_ENTRY* lpEntries;
};
///AFX_MSGMAP可以得到基类的消息映射入口地址和得到本身的消息映射入口地址。
MFC下一个消息的处理过程是一般是这样的。
1、_AfxCbtFilterHook截获消息（这是一个钩子函数）
2、_AfxCbtFilterHook把窗口过程设定为AfxWndProc。
3、函数AfxWndProc接收Windows操作系统发送的消息。
4、函数AfxWndProc调用函数AfxCallWndProc进行消息处理。
5、函数AfxCallWndProc调用CWnd类的方法WindowProc进行消息处理。
如何添加自己的消息？
我们已经了解了WINDOW的消息机制，如何加入我们自己的消息呢？好我们来看
一个标准的消息处理程序是这个样子的
在 CWnd 类中预定义了标准 Windows 消息 (WM_XXXX  WM是WINDOW MESSAGE的缩写) 的默认处理程序。类库基于消息名命名这些处理程序。例如，WM_PAINT 消息的处理程序在 CWnd 中被声明为：
afx_msg void OnPaint();
afx_msg 关键字通过使这些处理程序区别于其他 CWnd 成员函数来表明 C++ virtual 关键字的作用。但是请注意，这些函数实际上并不是虚拟的，而是通过消息映射实现的。我们在本文的一开始便说明了为什么要这样做。
所有能够进行消息处理的类都是基于CCmdTarget类的，也就是说CCmdTarget类是所有可以进行消息处理类的父类。CCmdTarget类是MFC处理命令消息的基础和核心。
若要重写基类中定义的处理程序，只需在派生类中定义一个具有相同原型的函数，并创建此处理程序的消息映射项。我们通过ClassWizard可以建立大多数窗口消息或自定义的消息，通过ClassWizard可以自动建立消息映射，和消息处理函数的框架，我们只需要把我们要做的事情填空,添加你要做的事情到处理函数。这个非常简单，就不细说了。但是也许我们需要添加一些ClassWizard不支持的窗口消息或自定义消息，那么就需要我们亲自动手建立消息映射和消息处理的框架，通常步骤如下：
第一步：定义消息。Microsoft推荐用户自定义消息至少是WM_USER+100，因为很多新控件也要使用WM_USER消息。
#define WM_MYMESSAGE (WM_USER + 100)
第二步：实现消息处理函数。该函数使用WPRAM和LPARAM参数并返回LPESULT。
LPESULT CMainFrame::OnMyMessage(WPARAM wParam, LPARAM lParam)
{
// TODO: 处理用户自定义消息，填空就是要填到这里。
return 0;
}
第三步：在类头文件的AFX_MSG块中说明消息处理函数：
// {{AFX_MSG(CMainFrame)
afx_msg LRESULT OnMyMessage(WPARAM wParam, LPARAM lParam);
//}}AFX_MSG
DECLARE_MESSAGE_MAP()
第四步：在用户类的消息块中，使用ON_MESSAGE宏指令将消息映射到消息处理函数中。
ON_MESSAGE( WM_MYMESSAGE, OnMyMessage )
可以看出，用户自定义的消息和我们通过ClassWizard添加的消息一样，都是利用了ON_MESSAGE宏,建立的消息映射。
其实消息类别可以分成多种，上面说的只是其中之一。有三种主要的消息类别：（以下部分摘自MSDN）
1、Windows 消息
此类消息主要包括以前缀 WM_ 开头的消息，WM_COMMAND 除外。Windows 消息由窗口和视图处理。此类消息往往带有用于确定如何处理消息的参数。
2、控件通知
此类消息包括从控件和其他子窗口发送到其父窗口的 WM_COMMAND 通知消息。例如，当用户在编辑控件 (Edit Control) 中执行可能更改文本的操作后，该编辑控件 (Edit Control) 将向其父级发送包含 EN_CHANGE 控件通知代码的 WM_COMMAND 消息。该消息的窗口处理程序以某种适当的方式响应此通知消息，例如在控件中检索该文本。
框架像传送其他 WM_ 消息一样传送控件通知消息。但是有一个例外的情况，即当用户单击按钮时由按钮发送的 BN_CLICKED 控件通知消息。该消息被作为命令消息特别处理，并像其他命令一样传送。
3、命令消息
此类消息包括用户界面对象（菜单、工具栏按钮和快捷键）发出的 WM_COMMAND 通知消息。框架处理命令的方式与处理其他消息不同，可以使用更多种类的对象处理命令。
Windows 消息和控件通知消息由窗口来处理（窗口是从 CWnd 类派生的类的对象）。包括 CFrameWnd、CMDIFrameWnd、CMDIChildWnd、CView、CDialog 以及从这些基类派生的您自己的类。这些对象封装了 HWND——Windows 窗口的句柄。
命令消息可以由范围更广的对象（文档、文档模板以及应用程序对象本身）处理，而不仅仅由窗口和视图处理。当某一命令直接影响到某个特定对象时，应当让该对象处理此命令。例如，“文件”菜单中的“打开”命令在逻辑上与应用程序相关联：该应用程序接收到此命令时会打开指定的文档。因此“打开”命令的处理程序是应用程序类的成员函数。
命令消息我们比较常见的便是菜单项和工具条了，大家可以看到他的消息映射宏和窗口消息不太一样，一般的形式是这样的
ON_COMMAND(id,memberFxn)
第一个参数是命令ID，一个ID号对应一个消息处理，当然你可以让多个ID共用一个处理函数。常见的应用例如：菜单项打开文档的ID和工具条按钮打开文档的ID同时使用一个处理函数，或者直接将它们的ID设成相同的。
还有一种消息叫通知消息。例如树型控件的等一些复杂的控件在单击后需要传递更多的信息，例如光标的位置和当前项的一个结构，所以MFC为控件的每个通知消息也定义了一个宏，它长成了这个样子：
ON_CONTROL(EN_CHANGE,id,memberFxn)
还有很多种消息存在于MFC，宏定义有区别，大家可以触类旁通。
窗口消息有上百个。你可以从MSDN上查到WM_开头的，或者查看CWnd的成员函数，会给你列出很多，别忘了还有很多非窗口消息。雷神无法一一列出，也没有必要。大家查一下就行了。不过对一些常用的、新的控件消息和特殊的通知消息我还是把他们列出几个表，大家做个参考吧。
