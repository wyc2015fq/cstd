# VC中的消息的分类——详解ON_ONTIFY - L_Andy的专栏 - CSDN博客

2012年11月26日 14:22:39[卡哥](https://me.csdn.net/L_Andy)阅读数：2343



## [VC中的消息的分类](http://www.cnblogs.com/shangsrs/archive/2012/09/03/2669051.html)

张明奇（卡哥）

1.窗口消息，该消息的产生一般与创建窗口、绘制窗口、移动窗口、销毁窗口，以及在窗口中的操作（如移动鼠标）等操作窗口的动作有关。标准window消息有默认的处理函数，这些函数在CWnd类中进行了预定义。

2.命令消息，该消息一般与处理用户的请求有关，是由某个用户接口事件所发出的消息。例如，当用户与用户界面交互（单击菜单选项，工具栏按钮或按了某一加速键）时，命令消息就以WM_COMMAND的形式出现。该类消息意味着“使用者命令程序完成某些操作”，凡是由图形用户接口产生的消息都是这类消息。凡是从CCmdTarget派生的类（如文档、文档模板，应用程序对象、窗口和视图等）都能处理命令消息。

3.控件通知消息，控件是一个Windows的一个子窗口（如对话框中的按钮、编辑框等），控件通知消息是指在事件发生时，由控件或其它类型的子窗口发送到父窗口的消息。它通知父窗口，该控件接受了某操作，为父窗口进一步控制子窗口提供了条件。

4.自定义的消息。该消息的值必须大于WM_USER(vc提供的一个整数值)。

VC中的消息的分类有3种：窗口消息、命令消息和控件通知消息，我们这里要谈的是最后一种：控件通知消息。

　　控件通知消息，是指这样一种消息，一个窗口内的子控件发生了一些事情，需要通知父窗口。通知消息只适用于标准的窗口控件如按钮、列表框、组合框、编辑框，以及Windows公共控件如树状视图、列表视图等。例如，单击或双击一个控件、在控件中选择部分文本、操作控件的滚动条都会产生通知消息。她类似于命令消息，当用户与控件窗口交互时，那么控件通知消息就会从控件窗口发送到它的主窗口。但是这种消息的存在并不是为了处理用户命令，而是为了让主窗口能够改变控件，例如加载、显示数据。例如按下一个按钮，他向父窗口发送的消息也可以看作是一个控件通知消息；单击鼠标所产生的消息可以由主窗口直接处理，然后交给控件窗口处理。

　　控件通知消息主要由窗口类即直接或间接由CWND类派生类处理。
　　控件通知格式

　　控件通知经历了一个演变过程，因而SendMessage( )的变量Message、wParam和lParam有三种格式。

　　第一控件通知格式

　　第一控件通知格式只是窗口消息的子集。它的特征格式如下：WM_XXXX。它主要来自下面的3种消息类型：
(1)表示一个控件窗口要么已经被创建或销毁，要么已经被鼠标单击的消息：WM_PARENTNOTIFY；

(2)发送到父窗口，用来绘制自身窗口的消息，例如： WM_CTLCOLOR、WM_DRAWITEM、WM_MEASUREITEM、WM_DELETEITEM、WM_CHARTOITEM、WM_VKTOITEM、WM_COMMAND和WM_COMPAREITEM

(3)有滚动调控件发送，通知父窗口滚动窗口的消息：WM_VSCROLL和WM_HSCROLL

　　第二控件通知格式

　　第二控件通知格式与命令消息共享，它的特征格式如下：WM_COMMAND。

　　在WM_COMMAND中，lParam用来区分是命令消息还是控件通知消息：如果lParam为NULL，则这是个命令消息，否则lParam里面放的必然就是控件的句柄，是一个控件通知消息。对于wParam则是低位放的是控件ID，高位放的是相应的消息事件。

　　第三控件通知格式

　　这个才真正涉及到我们要讲的内容，同时他也是最为灵活的一种格式。它的特征格式如下：WM_NOTIFY。
在WM_NOTIFY中，lParam中放的是一个称为NMHDR结构的指针。在wParam中放的则是控件的ID。

NMHDR结构的由来

NMHDR结构是很值得一提的，该结构包括有关制作该通知的控件的任何内容，而不受空间和类型的限制，他的来历也是很有意思的。

　　在最初的windows3.x中，根本就不存在什么WM_NOTIFY，控件通知它们父窗口，如鼠标点击,控件背景绘制事件，通过发送一个消息到父窗口。简单的通知仅发送一个WM_COMMAND消息，包含一个通知码和一个在wParam中的控件ID及一个在lPraram中的控件句柄。这样一来，wParam和lParam就都被填充了，没有额外的空间来传递一些其它的消息，例如鼠标按下的位置和时间。

　　为了克服这个困难，windows3.x就提出了一个比较低级的解决策略，那就是给一些消息添加一些附加消息，最为明显的就是控件自画用到的DRAWITEMSTRUCT。不知道大家对这个结构熟悉不，不过，如果你是老手，你应该非常清楚这个结构，这个结构包含了9个内容，几乎你需要控制的信息都给你提供了。为什么说它比较低级呢？因为不同的消息附加的内容不同，结果就是一盘散沙，非常混乱。

　　在win32中，MS又提出了一个更好的解决方案：引进NMHDR结构。这个结构的引进就是消息统一起来，利用它可以传递复杂的信息。这个结构的布局如下：

NMHDR

{

HWnd hWndFrom ; 相当于原WM_COMMAND传递方式的lParam

UINT idFrom ; 相当于原WM_COMMAND传递方式的wParam（low-order）

UINT code ; 相当于原WM_COMMAND传递方式的NotifyCode(wParam"s high-order)

}；

　　对于这个结构的应用于WM_NOTIFY信息结构，结果WM_NOTIFY就变成了：

A、无附加信息。结构变得很简单，就是一个NMHDR结构。
B、有附加信息。定义一个大的结构，它的第一个元素就是NMHDR结构，它的后面放置附加信息。

WM_NOTIFY结构的好处

　　除了上面我们所说的好处外，WN_NOTIFY还有自己的独特的好处：

　　由于在大结构中，第一个成员为NMHDR,这样一来，我们就可以利用指向NMHDR的指针来传递结构地址，根据指针的特性，无论消息有没有附加信息，这个指针都适用，也能够很方便的进行强制转换。

　　分析ON_NOTIFY

　 类向导可以创建ON_NOTIFY消息映射入口并提供一个处理函数的框架，来处理WM_NOTIFY类型的消息。ON_NOTIFY消息映射宏有如下语法.

ON_NOTIFY(wNotifyCode,id,memberFxn)

　　其中：wNotifyCode:要处理的通知消息通知码。比如上面我们提到的LVN_KEYDOWN；Id:控件标识ID；MemberFxn:处理此消息的成员函数。

　　此成员函数有如下的原型声明:

afx_msgvoid memberFxn( NMHDR * pNotifyStruct, LRESULT * result);

　　比如:假设你想成员函数OnKeydownList1处理ClistCtrl(标识ID=IDC_LIST1）的 LVN_KEYDOWN消息,你可以使用类向导添加如下的消息映射:

ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, 

　　在上面的例子中,类向导提供如下函数:

voidCMessageReflectionDlg::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult)
{
LV_KEYDOWN* pLVKey= (LV_KEYDOWN*)pNMHDR;
*pResult = 0;
 }

　　这时类向导提供了一个适当类型的指针，你既可以通过pNMHDR，也可以通过 pLVKey来访问这个通知结构。

ON_NOTIFY_RANGE

　　有时我们可能需要为一组控件处理相同的WM_NOTIFY消息。这时需要使用ON_NOTIFY_RANGE而不是ON_NOTIFY。不过，很不幸的是，VC6的ClassWizard并不支持这个消息，所以我们必须手工添加。方法和一般的手工添加的消息一样，不过需要注意的是：

(1)当你使用 ON_NOTIFY_RANGE时,你需要指定控件的ID范围.其消息映射入口及函数原型如下:

ON_NOTIFY_RANGE( wNotifyCode, id, idLast, memberFxn )

　　其中：wNotifyCode:消息通知码.比如:LVN_KEYDOWN。id:第一控件的标识ID。
idLast:最后一个控件的标识ID。（标识值一定要连续）memberFxn:消息处理函数。

(2)成员函数必须有如下原型申明：afx_msg void memberFxn( UINT id, NMHDR * pNotifyStruct, LRESULT *result );

