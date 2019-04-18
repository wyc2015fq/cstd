# WM_NOTIFY消息 - L_Andy的专栏 - CSDN博客

2012年11月26日 14:26:04[卡哥](https://me.csdn.net/L_Andy)阅读数：919


这个技术文章介绍了关于新**WM_NOTIFY**消息， 还描述了建议使用的一种在你的MFC应用程序中处理**WM_NOTIFY**消息的方法。

**Windows3.x的 通告消息**

在Windows 3.x下，控件通过发送一个消息给它的父窗口来告知诸如目标点击，内容的变化与选中，控件绘制等等之类的事件。简单的通告消息以特殊的**WM_COMMAND**消息形式来发送，通知码(如**BN_CLICKED**)与控件ID存放在*wParam*里，*lParam*保存控件的句柄。此时注意，*wParam*与*lParam*已经装满了数据，再也传递不了别的数据了，这些消息只能是简单的通告消息。举个例子，**BN_CLICKED**通告消息，无法发送按下鼠标按键时鼠标的位置信息。

当Windows 3.x下的控件需要发送包括额外数据的通告消息时，它们使用各种特殊目的的消息，包括**WM_CTLCOLOR**,
**WM_VSCROLL**,**WM_HSCROLL**,**WM_DRAWITEM**,**WM_MEASUREITEM**,**WM_COMPAREITEM**,**WM_DELETEITEM**,**WM_CHARTOITEM**,**WM_VKEYTOITEM**等等。这些消息能够被反射回给发送它们的的控件，要看更多信息，查阅[TN062:
 Message Reflection for Windows Controls](http://msdn.microsoft.com/zh-cn/library/eeah46xd.aspx). ([TN062:Windows控件的消息反射](http://msdn.microsoft.com/zh-cn/library/eeah46xd.aspx))

**Win32下的通告消息**

对于那些Windows 3.1的控件， Win32 API使用那些曾在Windows3.x有的绝大部分通告消息。However, Win32 also adds a number of sophisticated, complex controlsto those supported in Windows 3.x.这些控件经常发送带附加数据的通告消息。设计者们没有为每一个需要附加数据的通告消息增加一个新的**WM_***
 消息，而是只增加了一个消息，**WM_NOTIFY**，这个消息可以通过一标准化格式传递任意多的额外数据。

**WM_NOTIFY**消息包括 保存发送消息控件ID的*wParam*和保存一个结构指针的*lParam*两部分。那个结构可以是一个**NMHDR**结构或者某些更大点的、以**NMHDR**结构为第一个成员的结构。这样的话，一个指向该结构的指针可以是**NMHDR**结构指针，也可以是那个更大点的结构指针，看你怎么转换他了。

在大多数情况下，那个指针会指向更大点的结构，当你用到的时候就需要转换它。 只有几个通告消息，如 common通告消息(名字以**NM_**开始)，工具提示控件的**TTN_SHOW**与**TTN_POP**，是实际上用到**NMHDR**结构的。

那个**NMHDR**结构或者**为首成员**的结构，包含发送消息的控件句柄和ID，还有通知码(如**TTN_SHOW**)。**NMHDR**结构格式如下：
`typedef struct tagNMHDR {`` `                 HWND hwndFrom;
`                  UINT idFrom;``                  UINT code;``} NMHDR;`
对于**TTN_SHOW**消息，成员**code**应被设置成**TTN_SHOW**。

大多数通告消息传递一个指向更大的把**NMHDR**结构作为第一个成员的结构的指针。举个例子，看看list view控件的**LVN_KEYDOWN**通告消息所使用的结构，在list view控件里键盘按键被按下时发送这个消息。那个指针就指向**LV_KEYDOWN**结构体，定义如下：
`typedef struct tagLV_KEYDOWN {`` `                 NMHDR hdr;                 
`                  WORD wVKey;  ``                  UINT flags;  ``} LV_KEYDOWN;`
这样，因为**NMHDR**是这个结构的第一个成员，那个指针既可以转换为**NMHDR**型指针也可以转换为**LV_KEYDOWN**型指针。

(笔者注：标准命名约定，**LV_KEYDOWN**已经改名为：**NMLVKEYDOWN**。MSDN Library - October 2001上是这么说的呵呵。)

**对新Windows控件通用的通告消息**

一些通告消息对所有新的Windows控件来说是通用的，这些通告消息传递一个指向**NMHDR**结构体的指针。
|**Notification code**|**Sent because**|
|----|----|
|**NM_CLICK**|用户在控件内单击鼠标左键|
|**NM_DBLCLK**|用户在控件内双击鼠标左键|
|**NM_RCLICK**|用户在控件内单击鼠标右键|
|**NM_RDBLCLK**|用户在控件内双击鼠标右键|
|**NM_RETURN**|用户在控件具有输入焦点的时候按下回车|
|**NM_SETFOCUS**|控件获得输入焦点|
|**NM_KILLFOCUS**|控件失去输入焦点|
|**NM_OUTOFMEMORY**|控件因为没有足够的可用内存而不能完成一项操作|

**ON_NOTIFY:在MFC应用程序里处理 WM_NOTIFY 消息**

函数**CWnd::OnNotify**处理通告消息。其默认实现是检查通告消息处理函数的消息映射，然后调用。(checks the message map for notification handlers to call.)一般说来，你不用重载**OnNotify**。你可以写一个处理函数，然后在你自己的窗口类的消息映射表里添加一个该函数的消息映射入口。

ClassWizard，通过ClassWizard属性页或者WizardBar工具条，能够创建**ON_NOTIFY**消息映射入口，并且给您提供了一个处理函数的框架。更多关于通过ClassWizard使得添加消息映射更容易的信息，请看*Visual C++ Programmer's Guide*的*MappingMessages to Functions* 。

**ON_NOTIFY**消息映射宏的语法如下：

**ON_NOTIFY(***wNotifyCode***,***id***,***memberFxn***)**

斜体字的参数被替换为：

*wNotifyCode*

要被处理的通告消息代码，如**LVN_KEYDOWN**。

*id*

发送通告消息的控件ID。

*memberFxn*

通告消息发送后被调用的成员函数。

你的成员函数必须按照如下形式声明：

**afx_msg****void***memberFxn***( NMHDR ****pNotifyStruct***, LRESULT ****result***);**

斜体字参数为：:

*pNotifyStruct*

指向通告消息结构的指针，类型如上声明。

*result*

指向函数返回之前要被设置结果值的变量指针。

**代码实例**

现指定你要成员函数OnKeydownList1函去处理ID为IDC_LIST1的**CListCtrl**控件的**LVN_KEYDOWN**消息，你可以通过ClassWizard把下面的内容加入到你的消息映射表里：
`ON_NOTIFY( LVN_KEYDOWN, IDC_LIST1, OnKeydownList1 )`
在上面的例子里，ClassWizard提供的函数是：
`void CMessageReflectionDlg::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult)``{``   LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;``   // TODO: Add your control notification handler``   //                 code here``   ``   *pResult = 0;``}`
ClassWizard自动生成合适类型的指针。你可以通过pNMHDR或者pLVKeyDow访问通告消息结构体。

**ON_NOTIFY_RANGE**

如果你需要处理一组控件的同一个**WM_NOTIFY**消息，你可使用**ON_NOTIFY_RANGE**而不是**ON_NOTIFY**。例如，你有一组按钮，想让它们对某一通告消息执行相同的动作。

When you use **ON_NOTIFY_RANGE**,you specify a contiguous range of child identifiers for which to handle thenotification message by specifying the beginning and ending child identifiersof
 the range.

(不太会翻译，大意就是，使用**ON_NOTIFY_RANGE**的时候，要指定一个你所需要相同相同消息处理函数控件的ID范围)

ClassWizard不去处理**ON_NOTIFY_RANGE**的使用，要用它，就自己在消息映射表里编辑。

**ON_NOTIFY_RANGE**的消息映射入口与函数原型如下表示：

**ON_NOTIFY_RANGE(***wNotifyCode***,***id***,***idLast***,***memberFxn*** )**

斜体字参数被替换为：

*wNotifyCode*

要被处理的通告消息代码，如**LVN_KEYDOWN**。

*id*

连续ID范围里的第一个。

*idLast*

连续ID范围里的最后一个。

*memberFxn*

通告消息发送后被调用的成员函数。

你的成员函数必须按照如下形式声明：

**afx_msg****void***memberFxn***( UINT ***id***, NMHDR ****pNotifyStruct***, LRESULT ****result***);**

斜体字参数为：

*id*

发送通告消息的控件ID。

*pNotifyStruct*

指向通告消息结构的指针，类型如上声明。

*result*

指向函数返回之前要被设置结果值的变量指针。

**ON_NOTIFY_EX,ON_NOTIFY_EX_RANGE(这部分不理解，先放一放)**

If you wantmore than one object in the notification routing to handle a message, you canuse
**ON_NOTIFY_EX**(or **ON_NOTIFY_EX_RANGE**)rather than
**ON_NOTIFY**(or **ON_NOTIFY_RANGE**).The only difference between the
**EX** version and the regular version is that the memberfunction called for the
**EX** version returns a **BOOL** that indicates whether ornot message processing should continue. Returning
**FALSE** fromthis function allows you to process the same message in more than one object.

ClassWizarddoes not handle
**ON_NOTIFY_EX**or **ON_NOTIFY_EX_RANGE**;if you want to use either of them, you need to edit your message map yourself.

The message-mapentry and function prototype for
**ON_NOTIFY_EX** and **ON_NOTIFY_EX_RANGE**are as follows. The meanings of the parameters are the same as for the non-**EX** versions.

**ON_NOTIFY_EX(***nCode***,***id***,***memberFxn***)**

**ON_NOTIFY_EX_RANGE(***wNotifyCode***,***id***,***idLast***,***memberFxn*** )**

The prototypefor both of the above is the same:

**afx_msg****BOOL***memberFxn***( UINT ***id***, NMHDR ****pNotifyStruct***, LRESULT ****result***);**

In bothcases, *id*holds the child identifier of the control that sent the notification.

Your functionmust return **TRUE**if the notification message has been completely handled or **FALSE** ifother objects in the command routing should have a chance to handle the message.

