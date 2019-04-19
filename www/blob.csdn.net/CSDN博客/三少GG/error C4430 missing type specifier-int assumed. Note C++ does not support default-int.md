# error C4430: missing type specifier - int assumed. Note: C++ does not support default-int - 三少GG - CSDN博客
2012年05月02日 19:27:19[三少GG](https://me.csdn.net/scut1135)阅读数：3780标签：[c																[c++																[function																[constructor																[parameters																[mfc](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=parameters&t=blog)](https://so.csdn.net/so/search/s.do?q=constructor&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[重构C/C++](https://blog.csdn.net/scut1135/article/category/621651)
 1.
### [error C4430: missing type specifier - int assumed. Note: C++ does not support default-int](http://blog.csdn.net/codejoker/article/details/4771938)
分类： [DirectShow](http://blog.csdn.net/codejoker/article/category/614817)2009-11-05
 15:193960人阅读[评论](http://blog.csdn.net/CodeJoker/article/details/4771938#comments)(8)[收藏](http://blog.csdn.net/CodeJoker/article/details/4771938)[举报](http://blog.csdn.net/CodeJoker/article/details/4771938#report)
BaseClasses/ctlutil.h(278) : error C4430: missing type specifier - int assumed. Note: C++ does not support default-int
错误发生在：operator=(LONG); 函数定义中，这是因为在VC6中，如果没有显示的指定返回值类型，编译器将其视为默认整型；但是vs2005不支持默认整型. 这个错误，解决方法如下：
打开project->BaseClasses properties->configuration->**C/C++ ->Command Line，增加/wd4430选项。**
2. 
![](https://img-my.csdn.net/uploads/201205/02/1335961826_4058.jpg)
解决办法： **在linker->input里加入strmbase.lib WINMM.lib，并在项目工程下添加上面两个库（模仿salad）**(为啥不是系统自带捏T T)。
或者参考：
error LNK2001: unresolved external symbol _IID_IGraphBuilder
时间:2007-06-18[www.jubao163.com](http://www.jubao163.com/)  来源:
 不详
所属分类：.NET技术 VC.NET
-----------------------------------------
请问各位大虾，Linking... 
AVIPlayerDoc.obj : error LNK2001: unresolved external symbol _IID_IMediaEvent 
AVIPlayerDoc.obj : error LNK2001: unresolved external symbol _IID_IMediaControl 
AVIPlayerDoc.obj : error LNK2001: unresolved external symbol _CLSID_FilterGraph 
AVIPlayerDoc.obj : error LNK2001: unresolved external symbol _IID_IGraphBuilder 
Debug/AVIPlayer.exe : fatal error LNK1120: 4 unresolved externals 
怎么解决啊？急！！！！！ 
----------------------------------------------------------------------
有什么静态库没有包含？ 
--------------------------------------------------------
我的这个程序是基于DirectShowSDK的，要包含什么库啊？ 
--------------------------------------------------------
**呵呵，我加了strmiids.lib这个库就好了，开心啊！ **
**///////////////**************************************//////////////////////////////////**
**unresolved_external_symbol__main解决办法.doc**
**原因：是在建项目或工程时把Win32 Application与Win32 Console Application弄错了。**
**详见 VC++中的Win32 Application和Win32 Console Application区别**
**[http://lihuaxiong-001.blog.163.com/blog/static/2165829920099206263119/edit/](http://lihuaxiong-001.blog.163.com/blog/static/2165829920099206263119/edit/)**
**在创建项目时, 不使用MFC AppWizard向导, 如果没有设置好项目参数, 就会在编译时产生很多连接错误, 如error LNK2001错误, 典型的错误提示有:**
**libcmtd.lib(crt0.obj) : error LNK2001: unresolvedexternal symbol _main**
**LIBCD.lib(wincrt0.obj) : error LNK2001: unresolvedexternal symbol _WinMain@16**
**msvcrtd.lib(crtexew.obj) : error LNK2001: unresolvedexternal symbol _WinMain@16**
**nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolvedexternal symbol __beginthreadex**
**nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolvedexternal symbol __endthreadex**
**下面介绍解决的方法:**
**1. Windows子系统设置错误, 提示:**
**libcmtd.lib(crt0.obj) : error LNK2001: unresolvedexternal symbol _main**
**Windows项目要使用Windows子系统, 而不是Console, 可以这样设置:**
**[Project] --> [Settings] --> 选择"Link"属性页,**
**在Project Options中将/subsystem:console改成/subsystem:windows**
**2. Console子系统设置错误, 提示:**
**LIBCD.lib(wincrt0.obj) : error LNK2001: unresolvedexternal symbol _WinMain@16**
**控制台项目要使用Console子系统, 而不是Windows, 设置:**
**[Project] --> [Settings] --> 选择"Link"属性页,**
**在Project Options中将/subsystem:windows改成/subsystem:console**
**注意:后边的/incremental:yes也得去掉**
**3. 程序入口设置错误, 提示:**
**msvcrtd.lib(crtexew.obj) : error LNK2001: unresolvedexternal symbol _WinMain@16**
**通常, MFC项目的程序入口函数是WinMain, 如果编译项目的Unicode版本, 程序入口必须改为wWinMainCRTStartup, 所以需要重新设置程序入口:**
**[Project] --> [Settings] --> 选择"Link"属性页,**
**在Category中选择Output,**
**再在Entry-point symbol中填入wWinMainCRTStartup,即可**
**4. 线程运行时库设置错误, 提示:**
**nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolvedexternal symbol __beginthreadex**
**nafxcwd.lib(thrdcore.obj) : error LNK2001: unresolvedexternal symbol __endthreadex**
**这是因为MFC要使用多线程时库, 需要更改设置:**
**[Project] --> [Settings] --> 选择"C/C++"属性页,**
**在Category中选择Code Generation,**
**再在Use run-time library中选择DebugMultithreaded或者multithreaded**
**其中,**
**Single-Threaded 单线程静态链接库(release版本)**
**Multithreaded 多线程静态链接库(release版本)**
**multithreaded DLL 多线程动态链接库(release版本)**
**Debug Single-Threaded 单线程静态链接库(debug版本)**
**Debug Multithreaded 多线程静态链接库(debug版本)**
**Debug Multithreaded DLL 多线程动态链接库(debug版本)**
**单线程: 不需要多线程调用时, 多用在DOS环境下**
**多线程: 可以并发运行**
**静态库: 直接将库与程序Link, 可以脱离MFC库运行**
**动态库: 需要相应的DLL动态库, 程序才能运行**
**release版本: 正式发布时使用**
**debug版本: 调试阶段使用**
**学习侯老师的<<深入浅出MFC>>时，第三章的Frame1程序，开始新建Win32 一个空的Console Application，然后把书中的代码如实写上去，编译错误如下：**
**my.obj : error LNK2001: unresolved external symbol"class CWinApp * __cdecl AfxGetApp(void)"(?AfxGetApp@@YAPAVCWinApp@@XZ)  **
**导致原因是：未将MFC.CPP加到工程中去。**
** +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++**
## [Visual C++ Error Messages](http://averia.unm.edu/VisualCPPErrorMessages.html)
This page contains a listing of "difficult to diagnose" error messages and possible fixes. I haven't taught a programming class that uses Visual C++ in several years so this list is probably out of date by now. 
 It was valid for Microsoft Visual C++ version 6.0 service pack 3.
**C1001: INTERNAL COMPILER ERROR (compiler file 'msc1.cpp', line 1786) **Please choose the Technical Support command on the Visual C++ Help menu, or open the Technical Support help file for more information
This error results from leaving off the parentheses immediately following the function name in a function header.  To correct the error simply add () to the end of the function name.
**C1010: unexpected end of file while looking for precompiled header directive**
If your project is an MFC AppWizard created project then this error results from not #including StdAfx.h as the first #include statement (before any other #includes, data declarations, or executable program code).
**C1083: Cannot open precompiled header file: 'Debug/<Project-Name>.pch': No such file or directory**
This error results from a missing file - the compiled version of StdAfx.cpp. Visual C++ does a poor job of keeping track of this file and frequently "forgets" how to build it. This problem often occurs after restoring
 a saved workspace from diskette without the Debug directory. To fix the error select StdAfx.cpp from the workspace file list them choose Compile from the Build menu.  If that doesn't work the go to Project -> Settings, select the C/C++ tab, and click the radio
 button labeled Create Precompiled Headers.
**C2001: newline in constant**
This error is usually caused by a string or character constant that is missing its closing ' or " symbol.
**C2065: '<data-member name>' : undeclared identifier**
If this error occurs in one of your member functions then it is generally the result of forgetting the class scope operator in front of the function name in your .cpp file.
**C2143: syntax error : missing ';' before 'PCH creation point'**
Check each of the #include files to ensure that the closing brace of each class declaration is followed by a semicolon.
**C2143: syntax error : missing ';' before '*'**
If this error is followed by two C2501 errors then the problem is an undeclared class name within a pointer declaration.
For example, the declaration:
CClass *pObject;
will generate the above error message followed by a C2501 error message for 'CClass' and another C2501 message for 'pObject'.  The problem is that the compiler isn't recognizing CClass as a valid class/type name. 
 To correct the problem add a #include of the file containing the declaration of CClass (e.g., #include CClass.h)
**C2447: missing function header (old-style formal list?)**
This error usually results from a missing { or use of a ; instead of a { following the parameter list of a function header.
**C2511: '<function-name>' : overloaded member function not found in '<class-name>'**
This error results from a mismatch in the parameter list of a member function declaration (.h file) and definition (.ccp file). Check the forward declaration of the function in the .h file and its definition in
 the .cpp file and ensure that the number of parameters and the type of each parameter match exactly.
**C2512: '<constructor-function-name>' : no appropriate default constructor available**
This error usually occurs when you implement the constructor function of a derived class and forget to include parameter passing to the base class constructor function.   For example assume that CDerived is derived
 from CBase and that the CBase constructor function requires one parameter (e.g., int A).  If you define the CDerived constructor function as:
CDerived::CDerived(int A, int B) { ... }
the compiler will issue the above error message on the line containing the function header of CDerived::CDerived() because you haven't provided instructions for routing the parameter A to CBase::CBase().  Because
 you didn't provide instructions the compiler assumes that CBase::CBase() requires no arguments and it complains because no version of CBase::CBase() has been defined that accepts zero arguments.
If you intended to provide a version of CBase::CBase() that requires no arguments then the error message indicates that you forgot to declare that function in your base class declaration (e.g., in CBase.h).
If CBase::CBase() does require one or more arguments then you must correct the problem by including explicit instructions for passing parameters from the derived class constructor function to the base class constructor
 function.  The correction for the example above is:
CDerived::CDerived(int A, int B) : CBase(A) { ... }
**C2556: '<function-name>' : overloaded functions only differ by return typeC2371: '<function-name>' : redefinition; different basic types**
These errors usually result from a mismatch of function type between a .h and .cpp file. Check the forward declaration of the function in the .h file and its definition in the .cpp file and make the function return
 type identical in both files.
**C2601: '<function-name>' : local function definitions are illegal**
This error results from defining one function inside the body of another function.   It usually means that you omitted one or more } symbols in the function just before the function named in the error message.
**C2653: '<Class-Name>' : is not a class or namespace name**
This error usually results from not having #include "StdAfx.h" as the first #include statement in your class.cpp file.  It can also occur if your class definition is in a .h file and you forget to #include that
 .h file in another file that refers to the class name.
**C2661: '<Class-Name>::<Function-Name>' : no overloaded function takes n parameters**
This error indicates a mismatch between the parameters used in a function call (e.g., from main.cpp) and the declaration of the function.  The function call is passing n parameters and there is no function declaration
 that uses that number of parameters.
**LNK1104: Cannot open file nafxcwd.lib**
This error sometimes occurs when a project uses a class from the MFC but the project settings don't explicitly tell the link editor to look in the MFC libraries. 
Go to Project --> Settings (Build --> Settings in Visual C++ 4.0). On the General tab check the box that says "Use MFC in a Shared DLL".
**LNK1168: cannot open Debug\<Project-Name>.exe for writing**
This error occurs when the link editor attempts to write to a .exe file that is currently in use. The .exe file of an executing program is write protected until the program is terminated. Look at the status bar
 at the bottom of your screen and find the icon representing your executable application. Open the application and exit from it. Then select Build.
**LNK2001: unresolved external symbol __endthreadexLNK2001: unresolved external symbol __beginthreadex**
These errors result from using an MFC object or function without telling the link editor to search the MFC libraries.
Go to Project --> Settings (Build --> Settings in Visual C++ 4.0). On the General tab check the box that says "Use MFC in a Shared DLL".
**LNK2001: unresolved external symbol _main**
Your project doesn't contain a function called main().  The error usually results from forgeting to add main.cpp to the project workspace.
**<File>.obj : error LNK2001: unresolved external symbol "public: void __thiscall <Class1>::<Function1>(<Type>)"**
This a generic form of a LNK2001 error where <File>.obj can be any object file in your project and <Class1>::<Function1>(<Type>) can be any function in any class.  Substitute the specific <File>, <Class>, <Function>,
 and <Type> in your message into the instructions below to diagnose and correct the problem.
An LNK2001 error means that the link editor is looking for a compiled function and can't find it.  The call to the "missing function" is somewhere in <File>.cpp. Unfortunately, double-clicking on the error message
 won't take you to the point in <File.cpp> where the function is called but you can search for it with Find or Find In Files.  The function the link editor can't find is a member of <Class>, its name is <Function1>, and its return type is <Type>.
There are two common reasons for a LNK2001 error:
- The call in <File>.cpp doesn't match the function prototype in <Class>.h and/or the implementation in <Class>.cpp.  The mismatch may be in the**function name**, **return type**, or **number and/or type of parameters**.
   Correction strategies include:
- Check that the function name is spelled the same (case sensitive) in all three files (File.cpp, Class.h, and Class.cpp).
- Check that the function is actually declared and defined within <Class> - perhaps you defined it as a member of a different class or perhaps you tried to call the function (in <File>.cpp) using an object or object pointer of a different class.
- Check that the number and type of parameters in the function implementation (in <Class>.cpp) matches the number and type of parameters declared in the function declaration in <Class>.h.
- Check that the number and type of parameters in the function call (in <File>.cpp) matches the number and type of parameters declared in the function header in <Class>.cpp.
- The function was never declared or was declared but never defined.  To see if either is the case go to the ClassView window of the Workspace view.  Click the + next to <Class> and find <Function> in the list of member functions.
- **If <Function> is NOT in the list** then it was never declared or defined - add a declaration to the class declaraion in <Class>.h and implement the function in <Class>.cpp.
- **If <Function> is in the list** then right click on it and select Go To Definition from the pop-up menu.  If you get the error message *Cannot find definition (implementation) of this function* then the function was declared but never
 defined (implemented).  Implement the function to <Class>.cpp.
**LNK2005: <some-long-string-of-mostly-garbage> already defined in <name>.lib(<name>.obj)**
This error usually results from including a source code file multiple times. If you recognize any of the names in the message then it probably results from multiple inclusion of one of your own header files. Check
 to be sure that you've used #ifndef/#define/#endif properly your header files. If you don't recognize the name then it's probably multiple inclusion of a system file (e.g., afxwin.h). Make sure that you haven't explicitly included something in main.cpp that
 is already included in one of your own header files.   Also check that you haven't #included a .cpp file where you should have #included a .h file.
This page was last updated on 12/30/04
[Click here](http://averia/MGT331/Default.html) to return to the top level of this web site.
