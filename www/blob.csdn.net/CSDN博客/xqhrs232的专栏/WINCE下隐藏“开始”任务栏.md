# WINCE下隐藏“开始”任务栏 - xqhrs232的专栏 - CSDN博客
2009年11月06日 17:52:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1324标签：[wince																[任务																[winapi																[null																[shell																[image](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
1。编程实现
//===========================================================================
//隐藏“开始”栏   
#if  1
   HWND   lpClassName;   
  lpClassName   =   ::FindWindow(TEXT("HHTaskBar"),   NULL);   
  ::ShowWindow(lpClassName,   SW_HIDE);   
 // 只隐藏Taskbar还不行，你得把work   area   设为整个屏幕：   
  int   screenx=GetSystemMetrics(SM_CXSCREEN);   
  int   screeny=GetSystemMetrics(SM_CYSCREEN);   
  CRect   rcWorkArea;  
  rcWorkArea.left=0;   
  rcWorkArea.right=screenx;  
  rcWorkArea.top=0;   
  rcWorkArea.bottom=screeny;   
  ::SystemParametersInfo(SPI_SETWORKAREA,   0,   &rcWorkArea,   SPIF_SENDCHANGE   );  
#else
int i=0; 
CenterWindow(GetDesktopWindow()); // center to the hpc screen 
LONG  lOldWindowStyle;  
RECT  rectOldTaskBarRect;  
HWND  hOldhTaskBar = ::FindWindow(_T("HHTaskBar"),NULL); 
while( hOldhTaskBar == NULL)  
{  
   ++i; 
   if(i>4) 
   { 
     break; 
    } 
  hOldhTaskBar = ::FindWindow(_T("HHTaskBar"),NULL);  
  Sleep(1000);  
}  
::GetWindowRect(hOldhTaskBar,&rectOldTaskBarRect);  
::SetWindowPos(hOldhTaskBar,HWND_BOTTOM,0,0,0,0,SWP_HIDEWINDOW|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE); 
lOldWindowStyle = GetWindowLong(hOldhTaskBar,GWL_STYLE);  
::SetWindowLong(hOldhTaskBar,GWL_STYLE,lOldWindowStyle|WS_DISABLED); 
#endif
2。修改注册表实现
在Platform.reg加上： 
[HKEY_LOCAL_MACHINE/Software/Microsoft/Shell/SlideTaskBar] 
"Default"="0" 
[HKEY_LOCAL_MACHINE/Software/Microsoft/Shell/AutoHide] 
"Default"=dword:1 
[HKEY_LOCAL_MACHINE/Software/Microsoft/Shell/OnTop] 
"Default"="0"
3。修改PUBLIC代码实现
CSDN相关贴子
[http://topic.csdn.net/u/20100117/20/05bb9c1e-eee7-4d48-99d4-d9315b197362.html](http://topic.csdn.net/u/20100117/20/05bb9c1e-eee7-4d48-99d4-d9315b197362.html)
AAAA
一、采用standard shell,  
去掉任务栏          代码%_winceroot%/public/shell/oak/hpc 
我尝试了以下两种方法： 
1、在taskbar.cpp中将函数BOOL CTaskBar::Register()的内容全部删除，直接return TRUE; 
2、在explorer.cpp中将函数DWORD WINAPI CreatTaskBar()的内容删除，直接return 0; 
两种方法都可以实现，只是不太清楚哪个方法更节约时间和空间。个人感觉第2个方法比较好。 
去除桌面“我的电脑”“回收站”的图标 
代码%_winceroot%/public/shell/oak/files 
在shell.reg文件中找到这两个键值,它们对应着桌面上的我的电脑和回收站,删除这两个键值: 
[HKEY_LOCAL_MACHINE/Explorer/Desktop] 
"{000214A0-0000-0000-C000-000000000046}"="My Computer" 
"{000214A1-0000-0000-C000-000000000046}"="Recycle Bin" 
注：也可只修改项目release目录下的shell.reg.之后只要make image就可以了。 
二、采用taskman shell，需设置环境变量__SYSGEN_TASKMAN=1.。 
taskman shell没有任务栏、开始菜单和桌面图标，但有桌面背景色。 
代码%_winceroot%/public/wceshellfe/oak/taskman/ 
[http://www.itwis.com/html/net/netother/20090425/3792.html](http://www.itwis.com/html/net/netother/20090425/3792.html)
BBBB
最简单的办法： 
打开E:/WINCE600/PUBLIC/SHELL/OAK/HPC/EXPLORER/INC/taskbar.hxx文件,将宏TASKBAR_HEIGHT定义为0
