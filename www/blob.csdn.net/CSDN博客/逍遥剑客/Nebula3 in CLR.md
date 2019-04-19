# Nebula3 in CLR - 逍遥剑客 - CSDN博客
2008年11月18日 22:44:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2262标签：[winform																[application																[microsoft																[system																[crash](https://so.csdn.net/so/search/s.do?q=crash&t=blog)](https://so.csdn.net/so/search/s.do?q=system&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=winform&t=blog)
个人分类：[引擎研究																[工具开发](https://blog.csdn.net/xoyojank/article/category/1858383)](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
有用N3 + CLR做界面的冲动
新建一个CLR WinForm工程, 直接引入N3的头文件和库进行编译........
编译不过, 找了半天才发现原因
晕死, .Net和N3都有个System命名空间, 没法改Microsoft的东西, 只好把N3的System改成了NSystem
然后就是链接不过
一是__fastcall不被CLR支持, 改成__cdecl (/Gd)重遍
二是Multi-threaded Debug (/MTd)跟/clr冲突, 改成Multi-threaded Debug DLL (/MDd)
终于链接过了.............
启动程序, Crash掉
拿着关键字就去问google, 没想到MSDN论坛上还真有解决方法(感谢我的先驱们, 我成功是了站在你们的"尸体"上)
原因是N3的对象系统在ImplementClass时定义了一些静态对象, 如果直接用CLR会导致不能正解地进行初始化
解决方案(引用原文):
- Workaround Steps:
- In the project properties:
- 
- 1. Set Linker/Advanced/Entry Point to "" (empty string)
- 2. Set Linker/System/Subsystem to Not Set
- 
- 
- Step 1: Makes sure that the CRT startup code is invoked. This is because, if no entry point is specified, the linker will automatically use mainCRTStartup, which is defined in the CRT libraries. mainCRTStartup will make sure that the global object is initialized correctly.
- 
- Step 2: Makes sure that the linker will look for the symbol “main”. The linker looks for “main” because mainCRTStartup calls main() in its body. The default option for a Winforms application is Subsystem:Windows and this makes the linker look for WinMain().
- 
- Thanks
- 
- Sarita Bafna
- 
- Visual C++ team
- 
测试程序:
- // N3CLR.cpp : main project file. 
- 
- #include "stdafx.h" 
- #include "MainForm.h" 
- #include "stdneb.h" 
- #include "core/coreserver.h" 
- #include "io/ioserver.h" 
- 
- usingnamespace N3CLR;
- 
- [STAThreadAttribute]
- int main(array<System::String ^> ^args)
- {
- // Enabling Windows XP visual effects before any controls are created 
-     Application::EnableVisualStyles();
-     Application::SetCompatibleTextRenderingDefault(false);
- 
-     Ptr<Core::CoreServer> coreServer = Core::CoreServer::Create();
-     coreServer->Open();
-     n_printf("Hello CLR!");
-     coreServer->Close();
-     coreServer = NULL;
- // Create the main window and run it 
-     Application::Run(gcnew MainForm());
- return 0;
- }
- 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081118/N3_HelloCLR.JPG)
如果想嵌入到WinForm中的话, 需要更改DisplayDevice中的hWnd, 我的做法是把DisplayDevice创建的窗口做为WinForm的子窗口.
注意InputDevice需要最顶层的窗口句柄来创建:
- // set the cooperative level of the device, we're friendly 
- // note: use Win32's FindWindow() to find our top level window because  
- // the DisplayDevice may be running in a different thread 
- HWND hWnd = FindWindow(NEBULA3_WINDOW_CLASS, NULL);
- if (0 == hWnd)
-     {
-         hWnd = DisplayDevice::Instance()->GetParentWnd();
-     }
-     n_assert(0 != hWnd);
- 
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20081119/N3_WinForm.JPG)
