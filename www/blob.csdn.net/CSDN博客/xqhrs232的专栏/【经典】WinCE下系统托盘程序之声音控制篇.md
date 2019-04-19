# 【经典】WinCE下系统托盘程序之声音控制篇 - xqhrs232的专栏 - CSDN博客
2015年01月15日 10:59:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1113
原文地址::[http://blog.mcuol.com/User/Urbetter/Article/16136_1.htm](http://blog.mcuol.com/User/Urbetter/Article/16136_1.htm)
相关文章
1、[MFC
 程序最小化到系统托盘区的一个简单实例](http://blog.csdn.net/jarvischu/article/details/5781294)----[http://blog.csdn.net/jarvischu/article/details/5781294](http://blog.csdn.net/jarvischu/article/details/5781294)
2、如何建立 WinCE 上的系统托盘图标 [转自 MSDN]----[http://blog.sina.com.cn/s/blog_546badf1010008a2.html](http://blog.sina.com.cn/s/blog_546badf1010008a2.html)
3、 如何关闭时隐藏程序而不退出？点拖盘图标后再显示出来----[http://bbs.csdn.net/topics/210068798](http://bbs.csdn.net/topics/210068798)
程序在以下平台测试通过：
操作系统：Wince6.0
软件平台：EVC4.0
硬件平台：深圳友坚恒天科技有限公司ARM11开发板UT-S3C6410
[http://www.urbetter.com](http://www.urbetter.com/)
![](http://www.mcuol.com/download/uppic/200903011134371111.JPG)
在XP下我们能经常看到系统托盘的图标，也就是右下角的小图标，比如：QQ图标、音量控制图标等，那么Wince下的如何实现系统托盘呢？本文通过图解＋代码的方式详细介绍系统托盘程序实现的整个步骤。
先来看看程序实现后的效果图：
1、  开机自动运行系统托盘程序
![](http://www.mcuol.com/download/uppic/200903011136281.JPG)
2、  左键单击图标（小喇叭），上下拖动调整音量大小
![](http://www.mcuol.com/download/uppic/200903011137492.JPG)
3、  点击“Mute“静音
![](http://www.mcuol.com/download/uppic/200903011138433.JPG)
4、  双击打开系统音量控制界面
![](http://www.mcuol.com/download/uppic/200903011139274.JPG)
步骤：
1、  EVC下建立一个WCE Application Project
点击ok
![](http://www.mcuol.com/download/uppic/200903011140025.JPG)
2、  选择A Sample Windows CE Application
点击“Finish”
![](http://www.mcuol.com/download/uppic/200903011142156.JPG)
3、  创建并编辑对话框资源
![](http://www.mcuol.com/download/uppic/200903011143367.JPG)
4、  插入两个托盘ICON
一个是正常状态，一个是静音状态
![](http://www.mcuol.com/download/uppic/200903011144138.JPG)
程序很简单，主要包括几个方面：
1、  自定义一个消息用来处理系统托盘的操作
2、  初始化NOTIFYICONDATA结构
3、  调用API函数Shell_NotifyIcon
4、  系统托盘消息处理
5、 将编译生成的SoundSystemTray.exe拷贝至Wince 工程目录下
6、 将在SoundSystemTray.exe 加到NK中
platform.bib中添加
SoundSystemTray.exe $(_FLATRELEASEDIR)\SoundSystemTray.exe  NK S
7、 设置开机自动运行SoundSystemTray.exe
platform.reg中添加
[HKEY_LOCAL_MACHINE\Init]
"Launch80"="SoundSystemTray.exe"
"Depend80"=hex:1e,00
下面贴出所有代码：
// VIA_Volume.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "resource.h"
#include
#define  VOLUME_DEBUG 1
#define  WM_SHELL_NOTIFYICON (WM_USER +11) //自定义消息
BOOL CALLBACK DlgProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
LRESULT OnShowIcon(HWND hwnd,WPARAM wParam,LPARAM lParam);
DWORD CurrentVolume;
BOOL bIsMute;
NOTIFYICONDATA nid;
HICON hIcon;
HICON hIcon1;
////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(    HINSTANCE hInstance,
                            HINSTANCE hPrevInstance,
                            LPTSTR    lpCmdLine,
                            int       nCmdShow)
{
     // TODO: Place code here.
      MSG msg;
      InitCommonControls();
      HWND hwnd = CreateDialog(hInstance,MAKEINTRESOURCE(IDD_VOLUME),NULL,DlgProc);
      if (!hwnd)
      {
           RETAILMSG(VOLUME_DEBUG,(TEXT("CreateDialog fail !\r\n")));
           return FALSE;
      }
      waveOutGetVolume(0,&CurrentVolume);//得到当前系统消息值
      SendMessage(GetDlgItem(hwnd,IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (0xFFFF-CurrentVolume));
      //加载ICON及静音ICON
      hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON_ON));
      hIcon1 = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON_OFF));
      //初始化NOTIFYICONDATA结构
      nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
      nid.hWnd = hwnd;
      wcscpy(nid.szTip,TEXT("volume"));
      nid.uID = IDI_ICON_ON;
      nid.uFlags = NIF_MESSAGE | NIF_TIP | NIF_ICON;
      nid.uCallbackMessage = WM_SHELL_NOTIFYICON;
      nid.hIcon = hIcon;
      // 调用API Shell_ NotifyIcon加载系统托盘图标
      Shell_NotifyIcon(NIM_ADD,&nid);
      SetWindowLong(hwnd,GWL_EXSTYLE,WS_EX_TOOLWINDOW);
      while(GetMessage(&msg,NULL,0,0))
      {
           if (!IsDialogMessage(hwnd,&msg))
           {
                 TranslateMessage(&msg);
                 DispatchMessage(&msg);
            }
      }
      return msg.wParam;
}
BOOL CALLBACK DlgProc(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
      DWORD x,y;
      switch (uMsg)
      {
      case WM_INITDIALOG://窗口初始化
           {
                 RETAILMSG(VOLUME_DEBUG,(TEXT("OnInitDialog !\r\n")));
                SendMessage(GetDlgItem(hwnd,IDC_CHECK1),BM_SETCHECK,BST_UNCHECKED,0);
                 SendMessage(GetDlgItem(hwnd,IDC_SLIDER1), TBM_SETRANGEMIN, (WPARAM)TRUE, (LPARAM)0x0);//设置最小音量值
                 SendMessage(GetDlgItem(hwnd,IDC_SLIDER1), TBM_SETRANGEMAX, (WPARAM)TRUE, (LPARAM)0xffff); //设置最大音量值
                 x = SendMessage(GetDlgItem(hwnd,IDC_SLIDER1), TBM_GETRANGEMAX, 0, 0);
                 y = SendMessage(GetDlgItem(hwnd,IDC_SLIDER1), TBM_GETRANGEMIN, 0, 0);
                 RETAILMSG(VOLUME_DEBUG,(TEXT("set volume rang :0x%x--0x%x\r\n"),y,x));
           }
           break;
      case WM_COMMAND:
           {
                 if(SendMessage(GetDlgItem(hwnd,IDC_CHECK1),BM_GETCHECK,0,0) == BST_CHECKED)          //静音
                 {
                      bIsMute = TRUE;
                      waveOutSetVolume(0,0);
                      RETAILMSG(VOLUME_DEBUG,(TEXT("Mute,CurrentVolume =0x%x\r\n"),CurrentVolume));
                      //PostQuitMessage(0);
                      nid.hIcon = hIcon1;
                      Shell_NotifyIcon(NIM_MODIFY,&nid);
                 }
                 else
                 {
                      bIsMute = FALSE;
                      waveOutSetVolume(0,CurrentVolume);
                       RETAILMSG(VOLUME_DEBUG,(TEXT("unMute:CurrentVolume:0x%x\r\n"),CurrentVolume));
                      //PostQuitMessage(0);
                      nid.hIcon = hIcon;
                      Shell_NotifyIcon(NIM_MODIFY,&nid);
                 }
           }
           break;
      case WM_SHELL_NOTIFYICON:  //处理系统托盘消息
           {
                 OnShowIcon(hwnd,wParam,lParam);
           }
           break;
      case WM_ACTIVATE://点击不在窗口时的处理
           {
                 RETAILMSG(VOLUME_DEBUG,(TEXT("WM_ACTIVATE !\r\n")));
                 if (wParam == WA_INACTIVE)
                 {
                      RETAILMSG(VOLUME_DEBUG,(TEXT("WA_INACTIVE !\r\n")));
                      ShowWindow(hwnd,SW_HIDE);
                 }              
           }
           break;
      case WM_VSCROLL://音量控制处理
           {
                 CurrentVolume = SendMessage(GetDlgItem(hwnd,IDC_SLIDER1),TBM_GETPOS,(WPARAM)TRUE,0);
                 CurrentVolume = 0xFFFF-CurrentVolume;
                 if (bIsMute)
                 {
                      //waveOutSetVolume(0,0);
                      RETAILMSG(VOLUME_DEBUG,(TEXT("Mute:CurrentVolume:0x%x\r\n"),CurrentVolume));
                      break;
                 }
                 else
                 {
                      waveOutSetVolume(0,CurrentVolume);
                      RETAILMSG(VOLUME_DEBUG,(TEXT("waveOutSetVolume CurrentVolume=0x%x\r\n"),CurrentVolume));
                 }
           }
           break;
      default:
           break;
      }
      return 0;
}
LRESULT OnShowIcon(HWND hwnd,WPARAM wParam,LPARAM lParam)
{
      if (wParam!=IDI_ICON_ON && wParam!=IDI_ICON_OFF)
      {
           RETAILMSG(VOLUME_DEBUG,(TEXT(" icon err...wParam =%d \r\n"),wParam));
           return FALSE;
      }
      switch (lParam)
      {
      case WM_LBUTTONDBLCLK: //双击，打开系统音量控制程序
           {
                 PROCESS_INFORMATION pi;
                 CreateProcess(_T("\\Windows\\ctlpnl.exe"), _T("\\Windows\\cplmain.cpl,10"),
                      NULL, NULL, FALSE, 0, NULL, NULL, NULL, &pi);
           }
           break;
      case WM_LBUTTONUP: //单击，打开音量控制窗口
           {
                 POINT pt;                        
                 GetCursorPos(&pt);
                 ShowWindow(hwnd,SW_SHOW);
                 SetForegroundWindow(hwnd);
                 MoveWindow(hwnd,pt.x,324,58,128,TRUE);//设置窗口显示位置
           }
           break;
      default:
            break;
      }   
      return 0;
}

