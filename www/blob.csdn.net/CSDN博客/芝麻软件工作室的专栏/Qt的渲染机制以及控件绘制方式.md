
# Qt的渲染机制以及控件绘制方式 -  芝麻软件工作室的专栏 - CSDN博客


2016年06月16日 17:27:32[seven-soft](https://me.csdn.net/softn)阅读数：5276


在《[Windows编程](http://c.biancheng.net/cpp/windows/)》中，我们讲解了如何使用
 Windows  API 来创建控件，例如按钮、静态文本框、编辑框等，这些控件是 Windows 自带的，已经封装好了，只需要调用 API 即可。
Windows 支持的控件非常丰富，可以满足企业级应用程序的开发，Windows 本身也在使用这些控件。但这些控件是和 Windows 主题风格绑定的，比较传统，甚至略显丑陋，不能做出像QQ、360、迅雷等如此漂亮的界面。由于这些控件已经封装好了，想要修改它们的样式和行为非常困难，甚至无能为力。
如果不希望使用系统自带的控件，就必须自己完成控件的绘制，并为它们绑定事件，例如鼠标点击时改变颜色、鼠标移入是切换 Tab等。这些是非常复杂的工作，不但要保证控件尽量丰富、行为易于理解、样式便于修改等，还要保证绘制的高效以及动画的流畅。
任何平台都提供了图形绘制函数，例如画点、画线、画面等，Qt 就是利用这些基本函数将所有控件画出来。不同操作系统的控件样式不同，有时甚至连行为都有所差异，考虑到跨平台，Qt 要模拟不同风格的控件，例如在 Windows
 下的控件要尽量表现得像Windows原生控件，这样用户才不会觉得别扭，程序才更加 native。
其实，Qt 5 提供了两种控件的绘制方式：
1) 一种是上面所说的模拟原生控件，这种方式从 Qt 诞生以来就一直存在，程序员需要使用C++代码来创建界面、处理业务逻辑。虽然界面风格也比较传统，但控件样式容易修改。
2) 另外一种是 Qt 4.7 推出的 QML，使用它可以创建个性化的、大气漂亮的界面，例如QQ、360、迅雷等，适合现代的审美观。Qt 5 以后官方主推 QML。
QML 是 Qt 专门为界面设计推出的一种描述性的脚本语言，语法非常像CSS或JSON，但又支持JavaScript形式的编程控制。
> 如果你有Web开发经验，将很容易理解 QML，甚至会感到欣喜和熟悉；如果没有，上面的解释你可能感到晦涩，不过没关系，后续会详细讲解。
这种方式使前端和后台分开：QML 用来创建界面和处理事件，完成“用户看得到”的工作；C++ 用来处理业务逻辑，完成“用户看不到”的工作。
QML 是解释执行的脚本语言，程序在执行时将 .qml 文件加载进去，解析完成后再渲染成界面。这虽然比编译成本地程序慢，但它的好处是更新界面时不需要重新编译和更新整个程序，只要加载新的 QML 文件就可以。
> QML 和 JavaScript 虽然都是解释执行的脚本语言，但没有想象中的那么慢。浏览器渲染网页就是采用的这种机制，几乎是瞬间完成的，大家可以放心使用。
下面，我们使用 Windows API 来自己绘制一个简单的按钮控件，让它响应鼠标点击事件。代码如下：\#include<windows.h>
LRESULTCALLBACKWndProc(HWND,UINT,WPARAM,LPARAM);
intWINAPIWinMain(
HINSTANCEhInstance,
HINSTANCEhPrevInstance,
PSTRszCmdLine,
intiCmdShow
){
staticTCHARszClassName[]=TEXT("HelloWin");//窗口类名
HWNDhwnd;//窗口句柄
MSGmsg;//消息
WNDCLASSwndclass;//窗口类
//为窗口类的各个字段赋值
wndclass.style=CS_HREDRAW|CS_VREDRAW;//窗口风格
wndclass.lpfnWndProc=WndProc;//窗口过程
wndclass.cbClsExtra=0;//暂时不需要理解
wndclass.cbWndExtra=0;//暂时不需要理解
wndclass.hInstance=hInstance;//当前窗口句柄
wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);//窗口图标
wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);//鼠标样式
wndclass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);//窗口背景画刷
wndclass.lpszMenuName=NULL;//窗口菜单
wndclass.lpszClassName=szClassName;//窗口类名
//注册窗口
RegisterClass(&wndclass);
//创建窗口(并让窗口显示出来)
hwnd=CreateWindow(
szClassName,//窗口类的名字
TEXT("Welcome"),//窗口标题（出现在标题栏）
WS_OVERLAPPEDWINDOW,//窗口风格
CW_USEDEFAULT,//初始化时x轴的位置
CW_USEDEFAULT,//初始化时y轴的位置
500,//窗口宽度
300,//窗口高度
NULL,//父窗口句柄
NULL,//窗口菜单句柄
hInstance,//当前窗口的句柄
NULL//不使用该值
);
//显示窗口
ShowWindow(hwnd,iCmdShow);
//更新（绘制）窗口
UpdateWindow(hwnd);
//消息循环
while(GetMessage(&msg,NULL,0,0)){
TranslateMessage(&msg);//翻译消息
DispatchMessage(&msg);//分派消息
}
returnmsg.wParam;
}
//窗口过程
LRESULTCALLBACKWndProc(HWNDhWnd,UINTmessage,WPARAMwParam,LPARAMlParam){
PAINTSTRUCTps;
HDChdc;
TCHARszText[20]=TEXT("点击这里");//按钮文本
staticHBRUSHhSolidBrush;//画刷
staticHFONThFont;//字体
staticRECTrect={60,60,260,110};//按钮坐标
staticPOINTpt;//鼠标点击时的坐标
switch(message){
caseWM_CREATE:
//创建蓝色实心画刷
hSolidBrush=CreateSolidBrush(RGB(0,0,255));
//创建逻辑字体
hFont=CreateFont(
-15/*高度*/,-7.5/*宽度*/,0/*不用管*/,0/*不用管*/,400/*一般这个值设为400*/,
FALSE/*不带斜体*/,FALSE/*不带下划线*/,FALSE/*不带删除线*/,
DEFAULT_CHARSET,//这里我们使用默认字符集，还有其他以 _CHARSET 结尾的常量可用
OUT_CHARACTER_PRECIS,CLIP_CHARACTER_PRECIS,//这行参数不用管
DEFAULT_QUALITY,//默认输出质量
FF_DONTCARE,//不指定字体族*/
TEXT("微软雅黑")//字体名
);
break;
caseWM_PAINT:
hdc=BeginPaint(hWnd,&ps);
SelectObject(hdc,hFont);//将字体选入设备环境
SelectObject(hdc,hSolidBrush);//将画刷选入设备环境
Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);//绘制矩形
SetTextColor(hdc,RGB(0xff,0xff,0xff));//设置按钮文本颜色为白色
SetBkMode(hdc,TRANSPARENT);//设置按钮文本背景为透明
TextOut(hdc,128,75,szText,wcslen(szText));//输入按钮文本
EndPaint(hWnd,&ps);
break;
caseWM_LBUTTONDOWN:
pt.x=LOWORD(lParam);//点击鼠标时的x坐标
pt.y=HIWORD(lParam);//点击鼠标时的y坐标
if(PtInRect(&rect,pt)){//鼠标点击位置是否位于按钮内部
MessageBox(NULL,TEXT("感谢你的点击"),TEXT("提示"),MB_OK);
}
break;
caseWM_DESTROY:
DeleteObject(hSolidBrush);
PostQuitMessage(0);
break;
default:
returnDefWindowProc(hWnd,message,wParam,lParam);
}
return0;
}

在VS下创建Windows工程，运行上面的代码：![](http://c.biancheng.net/cpp/uploads/allimg/151121/1-151121225152920.png)
点击蓝色区域，会弹出提示框，这就是一个使用 GDI 函数绘制的按钮。
这个自己绘制的按钮和系统原生按钮风格不同，非常个性和自由，可以任意更换颜色、修饰边框等；如果加上美工和设计，它将会是一个非常漂亮的按钮。这就是自绘控件的优点。

