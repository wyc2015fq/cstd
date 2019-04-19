# 详解WINCE的控制面板 - xqhrs232的专栏 - CSDN博客
2013年01月23日 23:14:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：494
原文地址::[http://www.hzlitai.com.cn/article/ARM9-article/system/1624.html](http://www.hzlitai.com.cn/article/ARM9-article/system/1624.html)
//========================================================================
//TITLE:
// 详解WINCE的控制面板
//AUTHOR:
// norains
//DATE:
// Tuesday 29-August-2006
//========================================================================
曾经在我的一篇《控制面板的创建》中介绍过WinCE控制面板的结构,不过由于那篇文章其目的只在于控制面板如何调用EXE程序,故对控制面板也仅是泛泛而谈.不过在这篇文章中,我们将对WinCE的使用方法来个整体的详解.
1.控制面板的结构
控制面板其实也是个动态链接库,区别只在于后缀名为.cpl,以及对外接口为CPlApplet().
接口的原型为:
LONG CALLBACK CPlApplet(HWND hwndCPL,UINT message, LPARAM lParam1, LPARAM lParam2)
在接口函数中我们需要实现特定几个消息的响应,控制面板才能正常运作.为方便观察,在这里直接贴出消息的响应结构:
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// The entry point to the Control Panel application.
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
extern "C" LONG CALLBACK CPlApplet(HWND hwndCPL,UINT message, LPARAM lParam1, LPARAM lParam2)
{
switch (message)
{
case CPL_INIT:
// Perform global initializations, especially memory
// allocations, here.
// Return 1 for success or 0 for failure.
// Control Panel does not load if failure is returned.
return 1;
case CPL_GETCOUNT:
// The number of actions supported by this Control
// Panel application.
return 1;
case CPL_NEWINQUIRE:
{
// This message is sent once for each dialog box, as
// determined by the value returned from CPL_GETCOUNT.
// lParam1 is the 0-based index of the dialog box.
// lParam2 is a pointer to the NEWCPLINFO structure.
return 0; //means CPLApplet succeed
return 1; // Nonzero value means CPlApplet failed.
}
case CPL_DBLCLK:
{
// The user has double-clicked the icon for the
// dialog box in lParam1 (zero-based).
return 0; // CPlApplet succeed.
return 1; // CPlApplet failed.
}
case CPL_STOP:
// Called once for each dialog box. Used for cleanup.
case CPL_EXIT:
// Called only once for the application. Used for cleanup.
default:
return 0;
}
return 1; // CPlApplet failed.
} // CPlApplet
这几个消息的作用分别是:
CPL_INIT:初始化,可以在这分配内存等等.
CPL_GETCOUNT:返回控制面板程序的个数.因为一个.cpl文件中,可能包含多个Applets.
CPL_NEWINQUIRE:返回包含程序名和图标信息的NEWCPLINFO结构
CPL_DBLCLK:双击时响应
CPL_STOP:单个组件退出时响应,用来清除资源内存等
CPL_EXIT:退出整个cpl程序时响应
这些消息的定义可以在cpl.h中找到
2.在explorer中使用
如果定制的系统中带有explorer UI,只要把编译好的文件放进"\windows",然后再打开"控制面板"就可以看到.
3.如何调用.cpl
就像PC机中.dll文件不能直接运行,需要借助rundll32.exe来运行一样,WinCE下的.cpl也需要相应的程序来调用,这个程序就是:"ctlpnl.exe".
比如我们需要调用系统自带的cplmain.cpl的"电源管理",只要在开始菜单的运行中输入:"ctlpnl.exe \windows\cplmain.cpl,5".在路径后面紧跟的"5"只是因为"电源管理"在cplmain.cpl中输入第五个Applet而已.
由此我们不难得出在程序中运用ShellExecuteEx()函数调用控制面板的Applet的方法.以下这段代码,效果和在命令行中输入命令一样,也是调用"电源管理".
SHELLEXECUTEINFO info; 
TCHAR szControlPanelCmdLine[ 200 ];
swprintf( szControlPanelCmdLine, L"\\windows\\cplmain.cpl,5");
info.cbSize = sizeof( info );
info.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
info.lpVerb = NULL;
info.lpFile = TEXT("ctlpnl.exe");
info.lpParameters = szControlPanelCmdLine;
info.lpDirectory = NULL;
info.nShow = SW_SHOW;
info.hInstApp = NULL;
ShellExecuteEx( &info ); 
如果想调用cplmain.cpl的其它Applet,只要把数字改为相对应的即可.
以下所列的第一项序号为0,接下的为1,其后的继续递增.
0 CPL_Comm 
1 CPL_Dialing 
2 CPL_Keyboard 
3 CPL_Password
4 CPL_Owner 
5 CPL_Power 
6 CPL_System 
7 CPL_Screen 
8 CPL_Mouse 
9 CPL_Stylus 
10 CPL_Sounds 
11 CPL_SIP" 
12 CPL_Remove 
13 CPL_DateTime 
14 CPL_Certs 
15 CPL_Accessib 
4.系统自带控制面板的代码
微软对控制面板的代码是完全公开的,如果安装的是PB4.2,代码可以在"\PUBLIC\WCESHELLFE\OAK\CTLPNL"中找到.
CTLPNL文件夹中有九个文件夹,分别对应不同的CPL文件:
ADVBACKLIGHT:高级背光灯管理
BTHPNL:蓝牙
CONNPNL:拨号网络
CONTROL:主控制台,我们在explorer中看到的"控制面板"应该就是这个
CPLMAIN:会生成cplmain.cpl,控制面板的大多数选项均源自于此
CTLPNL:和CONTROL相关联的
DMPNL:没使用过,不太清楚.不过查看rc文件应该是设置上网的.
INTL2:区域和语言设置
STGUI:存储器管理
//======================================
备注::
1、控制面板的代码可以在"\PUBLIC\WCESHELLFE\OAK\CTLPNL"中找到
