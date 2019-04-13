
# Windows static控件（静态文本框控件） -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:37:27[seven-soft](https://me.csdn.net/softn)阅读数：6198


使用 TextOut 和 DrawText 函数有时候会不方便，例如：文本不能自动换行，超出窗口范围会被隐藏；
每次更改文本都要先擦除背景再重新输出，比较麻烦。
实际开发中一般使用静态文本框控件来输出文本。静态文本框是Windows 的一种标准控件，可以用来在窗口上显示一段文本，并且文本容易受到控制。除了静态文本框，Windows的标准控件还有很多种，例如按钮、下拉菜单、单选按钮、复选框等。
其实，控件也是一种窗口，也使用 CreateWindow 函数来创建。但是它们使用的窗口类的名字比较特殊，是由Windows预定义的；静态文本框控件的窗口类名是`static`。
与前面创建的独立窗口不同，控件是子窗口，创建时必须指定父窗口，这样控件才能有“归属”。
我们先来回顾一下 CreateWindow 函数的原型：HWNDCreateWindow(
LPCWSTRlpClassName,//窗口类名
LPCWSTRlpWindowName,//窗体标题（或控件文本）
DWORDdwStyle,//窗口/控件样式
intx,//窗口相对桌面（或子窗口相对父窗口）的 X 坐标
inty,//窗口相对桌面（或子窗口相对父窗口）的 Y 坐标
intnWidth,//窗体宽度
intnHeight,//窗体高度
HWNDhWndParent,//父窗口句柄
HMENUhMenu,//菜单句柄
HINSTANCEhInstance,//当前程序实例句柄
LPVOIDlpParam//一个指向某数值的指针
);

几点说明：
1) 对于参数 lpClassName 和 lpWindowName，一般使用宽字符，请加前缀`L`或使用`TEXT()`。
lpClassName 为窗口类的名字，可以是 RegisterClass 注册的类名，也可以是 Windows 预定义的控件类名。
如果你创建的是独立窗口，则 lpWindowName 应传入窗口的标题，若你希望创建控件，则应传入控件的文本。
2) dwStyle 表示窗口样式或控件样式。窗口样式以 WS 开头，详情请查看《[CreateWindow窗口风格取值](http://c.biancheng.net/cpp/html/2963.html)》。这些样式既可以用于独立窗口，也可以用于控件（子窗口）。
除了窗口样式，不同的控件也有自己特有的样式。对于 static 控件，它的样式以 SS 开头，常用的有：
|样式|说明|
|SS_LEFT|文本居左。|
|SS_RIGHT|文本居右。|
|SS_CENTER|文本居中。|
|SS_CENTERIMAGE|文本垂直居中。设置该样式后只能显示一行文本，即使有 '\n' 也不会换行。|
|SS_LEFTNOWORDWRAP|文本居左，不自动换行（有 '\n' 才会换行），超出控件范围的文本将被隐藏。|
|SS_SIMPLE|只显示一行文本（有 '\n' 也不换行），且不自动换行，超出控件范围的文本将被隐藏。|
3) 对于参数 hWndParent，如果是独立窗口，那么为 NULL，如果是控件，那么就需要父窗口的句柄。
4) 参数 hMenu 十分重要，在后续介绍的需要处理控件消息的控件中，这是他们的唯一标识符。每个控件的 hMenu 参数值都应不同，并且需要强制转换到 HMENU 类型，如 (HMENU)1 ，再次强调，每个控件的(HMENU)后的值都应不同，可以从1往下递推。
下面的代码，会在 WM_CREATE 事件中创建 static 控件：//窗口过程
LRESULTCALLBACKWndProc(
HWNDhWnd,
UINTmessage,
WPARAMwParam,
LPARAM lParam
){
PAINTSTRUCTps;
HDChdc;
HWNDhStatic;
switch(message){
caseWM_CREATE:
hStatic=CreateWindow(
L"static",//静态文本框的类名
L"C语言中文网",//控件的文本
WS_CHILD/*子窗口*/|WS_VISIBLE/*创建时显示*/|WS_BORDER/*带边框*/|SS_CENTER/*水平居中*/|SS_CENTERIMAGE/*垂直居中*/,
20/*X坐标*/,20/*Y坐标*/,200/*宽度*/,100/*高度*/,
hWnd,//父窗口句柄
(HMENU)1,//为控件指定一个唯一标识符
hInst,//当前程序实例句柄
NULL
);
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_DESTROY:
PostQuitMessage(0);
break;
}
returnDefWindowProc(hWnd,message,wParam,lParam);
}

运行效果：
![](http://c.biancheng.net/cpp/uploads/allimg/150729/1-150H9120135559.png)
完整代码下载：[http://pan.baidu.com/s/1eQo8kNK](http://pan.baidu.com/s/1eQo8kNK)密码：excr
给 CreateWindow 函数传入的第三个参数为窗口样式或控件样式（子窗口样式）。不同的控件样式一般不同，而窗口样式则大同小异：WS_CHILD：表明是一个子窗口，也就是控件，不是独立窗口。
WS_VISIBLE：创建时显示，如果没有该样式，则需要调用 ShowWindow 函数来显示。
WS_BORDER：带边框。
给 CreateWindow 函数传入的倒数第二个参数为 hInst，表示当前程序的实例句柄。hInst 在 WndProc 函数中并不存在，因为当前实例句柄是通过 WinMain 函数的参数传入的，所以必须要定义一个全局变量
 hInst，然后在 WinMain 中给它赋值后才能使用。如下所示：\#include<windows.h>
HINSTANCEhInst;
intWINAPIWinMain(
HINSTANCEhInstance,
HINSTANCEhPrevInstance,
PSTRszCmdLine,
intiCmdShow
){
// TODO: 其他代码
hInst=hInstance;
// TODO: 其他代码
}

另外，你也可以通过`((LPCREATESTRUCT)lParam)->hInstance`语句获得当前程序实例句柄，有兴趣的朋友请自行Google或百度。
## 获取、修改控件文本
GetWindowText 函数用于将指定窗口的标题文本（如果存在）拷贝到一个缓存区内；如果指定的窗口是一个控件，则拷贝控件的文本。它的原型为：IntGetWindowText(
HWNDhWnd,//窗口/控件句柄
LPTSTRlpString,//指向接收文本的缓冲区指针
IntnMaxCount//要保存在缓冲区内的字符的最大个数
);

说明：nMaxCount 指定要保存在缓冲区内的字符的最大个数，其中包含NULL字符。如果文本超过界限，它就被截断。
函数执行成功，返回拷贝的字符的个数。
代码举例：//定义缓冲区
TCHARszStaticBuf[100];
//获取 static 控件的文本
GetWindowText(hStatic,szStaticBuf,100);

类似的，SetWindowText 函数可以用来设置窗口标题或控件文本，它的原型为：BOOLSetWindowText(
HWNDhwnd,
LPCTSTR lpString
);

下面的例子用来显示鼠标点击的次数：//窗口过程
LRESULTCALLBACKWndProc(
HWNDhWnd,
UINTmessage,
WPARAMwParam,
LPARAM lParam
){
PAINTSTRUCTps;
HDChdc;
//必须被设置为静态变量
staticintiClick=0;//鼠标单击次数
staticTCHARszTextBuf[20];//static 控件文本（缓冲区）
staticHWNDhStatic;//static 控件句柄
switch(message){
caseWM_CREATE:
hStatic=CreateWindow(
L"static",//静态文本框的类名
L"C语言中文网",//控件的文本
WS_CHILD/*子窗口*/|WS_VISIBLE/*创建时显示*/|WS_BORDER/*带边框*/|SS_CENTER/*水平居中*/|SS_CENTERIMAGE/*垂直居中*/,
20/*X坐标*/,20/*Y坐标*/,200/*宽度*/,100/*高度*/,
hWnd,//父窗口句柄
(HMENU)1,//为控件指定一个唯一标识符
hInst,//当前程序实例句柄
NULL
);
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
// TODO:  在此添加任意绘图代码...
EndPaint(hWnd,&ps);
break;
caseWM_LBUTTONDOWN://鼠标左键单击消息
iClick++;
wsprintf(szTextBuf,TEXT("鼠标被单击%d次"),iClick);
SetWindowText(hStatic,szTextBuf);
break;
caseWM_DESTROY:
PostQuitMessage(0);
break;
}
returnDefWindowProc(hWnd,message,wParam,lParam);
}

运行程序后，在客户区单击鼠标查看效果。下面是一张截图：
![](http://c.biancheng.net/cpp/uploads/allimg/150812/1-150Q215313D45.png)
完整代码下载：[http://pan.baidu.com/s/1bnudUVH](http://pan.baidu.com/s/1bnudUVH)提取密码：5pea
几点说明：
1) WM_LBUTTONDOWN 为鼠标左键单击消息。
2) wsprintf 与C语言中的 printf 类似，都是格式化输出函数，不过 wsprintf 一般将字符串输出到缓冲区，而 printf 输出到控制台。它的原型为：intwsprintf(
LPTSTRlpBuffer,//接收字符串的缓冲区的指针
LPCTSTRlpFormat,//格式控制字符串
[paramList]//参数列表
);

3) 声明 static 变量的目的是使局部变量持久化。第一次执行窗口函数会产生 WM_CREATE 消息，初始化 hStatic 变量。如果不声明为 static，那么当函数执行结束后 hStatic 变量就会被销毁，接下来单击鼠标执行窗口函数时，再也不会产生
 WM_CREATE 消息，也就意味着 hStatic 变量不会被赋值，将无法使用。
对于窗口函数中的变量，如果是在 WM_CREATE 消息中赋值，但在其他消息中使用，那么一般声明为静态变量，这样下次执行窗口函数时依然有效。
更多关于 static 变量的内容请查看：[C语言static变量和函数](http://c.biancheng.net/cpp/html/2863.html)

