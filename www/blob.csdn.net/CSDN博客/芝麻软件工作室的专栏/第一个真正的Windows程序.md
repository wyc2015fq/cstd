
# 第一个真正的Windows程序 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月20日 06:29:07[seven-soft](https://me.csdn.net/softn)阅读数：2580


编写Windows程序，首先要包含 windows.h 头文件。windows.h 还包含了其他一些Windows头文件，例如：windef.h：基本类型定义
winbase.h：内核函数
wingdi.h：用户接口函数
winuser.h： 图形设备接口函数
这些头文件定义了Windows的所有数据类型、函数原型、数据结构和符号常量，也就是说，所有的Windows API都在这些头文件中声明。
在C语言中，程序都是“黑屏”的，称为控制台程序(Console
 Application)。这套教程要讲的是带界面的Windows程序(Windows
 Application)，也称为GUI程序(GUI
 Application)。
控制台程序以 main() 为入口函数，Windows程序以 WinMain() 为入口函数，动态链接库(DLL)以 DllMain() 为入口函数（请查看[动态链接库DLL教程](http://c.biancheng.net/cpp/html/2753.html)），不同的入口函数决定了不同类型的程序。
WinMain() 函数的原型为：intWINAPIWinMain(
HINSTANCEhInstance,// 当前窗口句柄
HINSTANCEhPrevInstance,// 前一个窗口句柄，Win32下为NULL（Win16留下的废物，目前已弃用）
LPSTRlpCmdLine,// 命令行参数
intnCmdShow// 窗口显示方式
);

先不要急于理解这些参数的含义，我先给大家写一个简单的不带黑屏的、真正的Windows程序：\#include<windows.h>
intWINAPIWinMain(
HINSTANCEhInstance,
HINSTANCEhPrevInstance,
LPSTRlpCmdLine,
intnCmdShow
){
// 调用API 函数MessageBox
intnSelect=MessageBox(NULL,TEXT("你好，欢迎来到C语言中文网！"),TEXT("Welcome"),MB_OKCANCEL);
if(nSelect==IDOK){
MessageBox(NULL,TEXT("你点击了“确定”按钮"),TEXT("提示"),MB_OK);
}else{
MessageBox(NULL,TEXT("你点击了“取消”按钮"),TEXT("提示"),MB_OK);
}
return0;
}

编译并运行，会弹出一个对话框，如下所示：
![](http://c.biancheng.net/cpp/uploads/allimg/150713/1-150G3095500E1.png)
点击“确定”或“取消”按钮，又会弹出一个新的提示框。大家可以亲自运行一下，会有真实的体验。
原来没有`main()`函数，没有`#include
 <stdio.h>`的C语言程序也是可以运行的！
MessageBox() 函数是众多API中的一个，用于弹出一个指定风格的对话框，其原型为：int WINAPI MessageBox( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType );WINAPI 为宏定义`#define 
 WINAPI  __stdcall`，表示函数调用方式，暂时可以不理会，不影响代码编写，只需要知道MessageBox()返回值为 int，表示按下的按钮。有兴趣的读者可以查看：[__stdcall，__cdecl，__pascal，__fastcall的区别](http://c.biancheng.net/cpp/html/2847.html)
参数说明：hWnd：该消息框的父窗口句柄，如果此参数为NULL，则该消息框没有拥有父窗口。大家不用急于理解这个参数，后续会详细讲解。
lpText：消息框的内容。LPCTSTR 是自定义数据类型，等价于 const char *。
lpCaption：消息框的标题。
uType：对话框的按钮样式和图标。
uType 支持的按钮样式：
|按钮|含义|
|MB_OK|默认值，有一个“确认”按钮在里面|
|MB_YESNO|有“是”和“否”两个按钮在里面|
|MB_ABORTRETRYIGNORE|有“中止”，“重试”和“跳过”三个按钮在里面|
|MB_YESNOCANCEL|有“是”，“否”和“取消”三个按钮在里面|
|MB_RETRYCANCEL|有“重试”和“取消”两个按钮在里面|
|MB_OKCANCEL|有“确定”和“取消”两个按钮在里面|
这些按钮都是宏定义：\#defineMB_OK0x00000000L
\#defineMB_OKCANCEL0x00000001L
\#defineMB_ABORTRETRYIGNORE0x00000002L
\#defineMB_YESNOCANCEL0x00000003L
\#defineMB_YESNO0x00000004L
\#defineMB_RETRYCANCEL0x00000005L

你也可以尝试用数字来表示按钮，例如：MessageBox(NULL, TEXT("你好，欢迎来到C语言中文网！"), TEXT("Welcome"), 1);也会生成与上面相同的对话框。
除了按钮，uType 还支持图标（图标对用户有提醒作用）：|图标|含义|
|MB_ICONEXCLAMATION|一个惊叹号出现在消息框：![](http://c.biancheng.net/cpp/uploads/allimg/141108/1-14110Q0425B47.png)|
|MB_ICONWARNING|一个惊叹号出现在消息框（同上）|
|MB_ICONINFORMATION|一个圆圈中小写字母i组成的图标出现在消息框：![](http://c.biancheng.net/cpp/uploads/allimg/141108/1-14110Q0431R22.png)|
|MB_ICONASTERISK|一个圆圈中小写字母i组成的图标出现在消息框（同上）|
|MB_ICONQUESTION|一个问题标记图标出现在消息框：![](http://c.biancheng.net/cpp/uploads/allimg/141108/1-14110Q043302b.png)|
|MB_ICONSTOP|一个停止消息图标出现在消息框：![](http://c.biancheng.net/cpp/uploads/allimg/141108/1-14110Q04345J6.png)|
|MB_ICONERROR|一个停止消息图标出现在消息框（同上）|
|MB_ICONHAND|一个停止消息图标出现在消息框（同上）|

这些图标也都是宏定义：\#defineMB_ICONHAND0x00000010L
\#defineMB_ICONQUESTION0x00000020L
\#defineMB_ICONEXCLAMATION0x00000030L
\#defineMB_ICONASTERISK0x00000040L

如果希望同时定义按钮和图标的样式，可以使用或运算`|`，例如：MessageBox(
NULL,
TEXT("你好，欢迎来到C语言中文网！"),
TEXT("Welcome"),
MB_OKCANCEL|MB_ICONINFORMATION
);

会弹出如下的对话框：
![](http://c.biancheng.net/cpp/uploads/allimg/150713/1-150G3100240611.png)
与上面的对话框相比，多出了一个图标，同时还能听到提示音（Win7 有，XP 和 Win8 读者可以亲自测试）。
大家有没有发现，按钮都是用十六进制的第1位（二进制前4位）来表示，图标都是使用十六进制第2位（二进制第5~8位）来表示，进行或运算，每个位都不会改变，如下图所示：
![](http://c.biancheng.net/cpp/uploads/allimg/141108/1-14110Q126035L.png)
Windows 通过检测第1位的值来确定按钮的样式，检测第2位的值来确定图标样式。
再看来一下MessageBox() 的返回值。
MessageBox() 返回被按下的按钮，以数字表示，这些数字都被定义成了宏，如下所示：
|返回值|含义|
|IDOK|用户按下了“确认”按钮|
|IDCANCEL|用户按下了“取消”按钮|
|IDABORT|用户按下了“中止”按钮|
|IDRETRY|用户按下了“重试”按钮|
|IDIGNORE|用户按下了“忽略”按钮|
|IDYES|用户按下了“是”按钮|
|IDNO|用户按下了“否”按钮|
对应的宏定义为：[复制](http://c.biancheng.net/cpp/html/2848.html#)[纯文本](http://c.biancheng.net/cpp/html/2848.html#)[新窗口](http://c.biancheng.net/cpp/html/2848.html#)
\#defineIDOK1
\#defineIDCANCEL2
\#defineIDABORT3
\#defineIDRETRY4
\#defineIDIGNORE5
\#defineIDYES6
\#defineIDNO7


