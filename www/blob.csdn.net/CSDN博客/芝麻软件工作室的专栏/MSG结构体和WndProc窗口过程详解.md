
# MSG结构体和WndProc窗口过程详解 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:39:48[seven-soft](https://me.csdn.net/softn)阅读数：2239


MSG结构体和WndProc窗口过程对于Windows编程非常重要，如果不了解它们，可以说就没有学会Windows编程。
## MSG结构体
MSG 结构体用来表示一条消息，各个字段的含义如下：typedefstructtagMSG{
HWNDhwnd;//窗口句柄
UINTmessage;//消息类型
WPARAMwParam;//附加消息1
LPARAMlParam;//附加消息2
DWORDtime;//消息被传递时候的时间
POINTpt;//消息被传递时光标在屏幕上的位置
}MSG;

对各个字段的说明：
1) 最后两个字段 time 和 pt 一般由系统使用，我们很少用到。
2) message 为消息类型，也就是以 WM 开头的消息（WM 是 Window Message 的缩写 ），例如 WM_CREATE、WM_PAINT、WM_DESTROY、WM_COMMAND 等。
2) wParam 和 lParam 是要重点说明的，它们都表示附加消息。例如，当收到一个字符消息的时，message 的值为 WM_CHAR，但用户到底输入的是什么字符，那么就由 wParam 和 lParam 来说明。wParam、lParam
 表示的信息随消息类型的不同而不同，具体细节可以到MSDN中查看。
WPARAM 和 LPARAM 这两种数据类型的定义分别为：typedef  UINT_PTR  WPARAM;
typedef  LONG_PTR  LPARAM;在现代操作系统中（32位和64位操作系统），它们一般都表示 32 位的整数。
但在16位操作系统中，WPARAM 表示16位整数，而 LPARAM 表示32位整数，根据匈牙利命名法，16 位的变量通常以`W`开头，32
 位变量通常以`L`开头。升级到32位操作系统后，WPARAM也被扩展到32位，此时
 WPARAM 和 LPARAM 的大小完全相同。
在 Win32 API 的早期，由于还有很多 Win16 API 的软件，为了保证和 Win16 API 的代码可移植性，微软依然保留了 WPARAM 和 LPARAM 两个宏。
不过16位系统早已成为历史，现在你可以认为这两个参数的长度相同。读者可以在 VC / VS 中通过 F12 键或者“转到定义”菜单查看。
#### wParam 和 lParam 到底表示什么信息
可以肯定的是，消息类型 message 不同，wParam 和 lParam 两个字段表示的附加消息一般也不同。那么，它们到底表示什么呢？其实这个真的不一定，没有什么规律，它们本来就是一个历史遗留问题，也不知道微软到底怎么安排的，只能根据不同的消息类型去
 MSDN 中查找。
一般约定，wParam 用来表示控件的ID，或者高 16 位和低 16 位组合起来分别表示鼠标的位置，如果发送消息时需要附带某种结构的指针或者是某种类型的句柄时，习惯上用 lParam。
不过这也不一定是对的，例如对于一个鼠标左键按下的消息 WM_LBUTTONDOW，就用 lParam 来表示鼠标的坐标：//point 是一个结构体，表示鼠标坐标
//msg 表示一条消息
point.x=LOWORD(msg.lParam);
point.y=HIWORD(msg.lParam);

LOWORD 和 HIWORD 都是宏定义：LOWORD 用来取一个 32 位数的低 16 位，HIWORD 用来取一个32 位数的高 16 位。对于 WM_LBUTTONDOW
 消息，低 16 位存放的是 x 坐标，而高 16 位存放的是 y 坐标。
## WndProc 窗口过程
窗口过程一般定义为如下的形式：LRESULTCALLBACKWndProc(
HWNDhwnd,//窗口句柄
UINTmessage,//消息类型
WPARAMwParam,//附加消息1
LPARAMlParam//附加消息2
){
// TODO
}

WndProc 的各个参数和 MSG 结构体的前四个字段是一一对应的。需要铭记的是：每产生一条消息，都会调用一次
 WndProc 函数。
当用户点击按钮、编辑框、下拉列表框等控件的时候，会产生WM_COMMAND消息。对于不同来源的 WM_COMMAND 消息，wParam、lParam 参数也不同，见下表：
|消息来源|wParam (高16位)|wParam (低16位)|lParam|
|菜单|0|菜单ID|0|
|快捷键|1|快捷键ID|0|
|控件|控件通知码|控件ID|控件句柄|
注意：上面说的菜单是指窗口标题下方、客户区上方的下拉菜单，而不是客户区的下拉列表框控件。
控件通知码用来识别控件的操作。例如 Button 控件一般有两种通知码，BN_CLICKED 和 BN_DOUBLECLICKED，前者表示 Button 被单击，后者表示 Button 被双击。
对于 Button 控件，我们可以通过`LOWORD(wParam)`来获取它的
 ID，这就是上节最后一个例子的原理，请参考上节的代码体会。

