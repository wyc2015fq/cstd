# ShellExecute的妙用 - ljx0305的专栏 - CSDN博客
2009年12月25日 16:43:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：855标签：[null																[文档																[string																[printing																[dos																[delphi](https://so.csdn.net/so/search/s.do?q=delphi&t=blog)](https://so.csdn.net/so/search/s.do?q=dos&t=blog)](https://so.csdn.net/so/search/s.do?q=printing&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[VC																[MFC](https://blog.csdn.net/ljx0305/article/category/403913)](https://blog.csdn.net/ljx0305/article/category/401790)
ShellExecute的功能是运行一个外部程序（或者是打开一个已注册的文件、打开一个目录、打印一个文件等等），并对外部程序有一定的控制。
　　有几个API函数都可以实现这些功能，但是在大多数情况下ShellExecute是更多的被使用的，同时它并不是太复杂。下面举例说明它的用法。
开始一个新的应用程序
　　　ShellExecute(Handle, 'open', PChar('c:/test/app.exe'), nil, nil, SW_SHOW);
打开记事本，并打开一个文件(系统能识别记事本应用程序的路径，因此我们不必使用绝对路径）
　　　ShellExecute(Handle, 'open', PChar('notepad'), PChar('c:/test/readme.txt'), nil, SW_SHOW);
打印一个文档
　　　ShellExecute(Handle, 'print', PChar('c:/test/test.doc'), nil, nil, SW_SHOW);
　　　注意：可能你会看到word暂时的被打开，但它会自动关闭。
打开一个HTML页面
　　　ShellExecute(Handle, 'open', PChar('http://www.festra.com/'), nil, nil, SW_SHOW);
你能通过一个已经注册的文件类型来打开应用程序
　　　ShellExecute(Handle, 'open', PChar('c:/test/readme.txt'), nil, nil, SW_SHOW);
用windows Explorer 打开一个目录
　　　ShellExecute(Handle, 'explore', PChar('c:/windows)', nil, nil, SW_SHOW);
运行一个DOS命令并立即返回
　　　ShellExecute(Handle, 'open', PChar('command.com'), PChar('/c copy file1.txt file2.txt'), nil, SW_SHOW);
运行一个DOS命令并保持DOS窗口存在
　　　ShellExecute(Handle, 'open', PChar('command.com'), PChar('/k dir'), nil, SW_SHOW);
ShellExecute详解
ShellExecute函数用于打开或者打印一个指定的文件。指定的文件必须是可执行文件或者文档文件。
HINSTANCE ShellExecute(
     HWND hwnd, // handle to parent window 
     LPCTSTR lpOperation, // pointer to string that specifies operation to perform
     LPCTSTR lpFile, // pointer to filename or folder name string
     LPCTSTR lpParameters, // pointer to string that specifies executable-file parameters 
     LPCTSTR lpDirectory, // pointer to string that specifies default directory
     INT nShowCmd   // whether file is shown when opened
    )
* Params（参数解释）
Hwnd
指定父窗口句柄。这个窗口接收工程产生的所有消息。例如：一个工程可以通过消息框报告错误。
lpOperation
以一个以null结束的字符串描述操作的执行。以下的操作字符串是有效的：
行号 参数 含义
1 “open” 函数打开由lpFile指定的文件。这个文件必须是可执行文件或者文档文件。文件夹必须是可打开的文件夹。
2 “print” 函数打印由lpFile指定的文件。这个文件应该是文档文件。如果指定的文件是可执行文件，函数将以open的方式打开这个文件。
3 “explore” 函数将搜索由lpFile指定的文件夹。
lpOperation参数如果为null，函数将以open的方式打开由lpFile指定的文件。
lpFile
以一个null结束的字符串描述打开或者打印指定的文件，或者打开或者搜索指定的
文件夹。函数可以打开一个指定的文件或者文件夹，也可以打印一个文档文件。
lpParameters
如果lpFile指定的是一个可执行文件，lpParameters以一个以null结束的字符串指定该执行文件传给应用软件的参数。
行号 参数 含义
1 SW_HIDE 隐藏这个窗体，并激活其他窗体。
2 SW_MAXIMIZE 最大化指定的窗体。
3 SW_MINIMIZE 最小化指定的窗体，并按顺序激活最上层的窗体。
4 SW_RESTORE 激活并显示窗体。如果窗体为最小化或者最大化，窗体恢复到原始大小和位置。应用程序当恢复一个最小化的窗体时将指定标记。
5 SW_SHOW 以当前的大小和位置激活并显示窗体。
6 SW_SHOWDEFAULT 
7 SW_SHOWMAXIMIZED 激活并最大化显示窗体。
8 SW_SHOWMINIMIZED 激活并最小化现实窗体。
9 SW_SHOWMINNOACTIVE 最小化窗体，保持其激活状态。
10 SW_SHOWNA 以当前状态显示窗体，保持其激活状态。
11 SW_SHOWNOACTIVATE 以当前的大小和位置显示窗体，并保持其激活状态。
12 SW_SHOWNORMAL 激活并显示一个窗体。如果窗体为最大化或者最小化，窗体恢复到原始的大小和位置。当窗体第一次显示的时候，应用程序记录标记。
如果lpField指定的是一个文档文件，lpParameters应该为null。
lpDirectory
以一个以null结束的字符串指定文件开始运行的默认目录。
nShowCmd
如果lpField指定了一个可执行文件，nShowCmd指定这个文件在被打开时如何显示。这个参数可以是以下值中的任何一个：
如果lpField指定的是文档文件，nShowCmd应该为0。
* return Values（返回值）
如果函数执行成功，返回值是已经运行的工程的实例句柄，或者动态数据交换服务器应用程序的句柄。
（具体返回值列表从略，请参考Delphi在线帮助）
* Remarks（注释）
被lpFile参数指定的文件必须是一个可执行文件或者文档文件。如果文件是一个文档文件，ShellExecute函数将根据lpOperation参数决定打开或者打印这个文件。如果是可执行文件，ShellExecute函数将打开它，即使lpOperation参数被指定为printing。
你可以使用ShellExecute打开活着搜索一个文件夹。可以使用以下任意一种方式打开一个文件夹：
ShellExecute(handle, NULL, path_to_folder, NULL, NULL, SW_SHOWNORMAL);
或者
ShellExecute(handle, "open", path_to_folder, NULL, NULL, SW_SHOWNORMAL);
使用下面的调用搜索文件夹：
ShellExecute(handle, "explore", path_to_folder, NULL, NULL, SW_SHOWNORMAL)
如果lpOperation参数为null，函数将打开由lpFile指定的文件。如果lpOperation参数为”open”或者”explore”，函数将强制打开窗体或者资源管理器。
注：在新窗口打开网页的办法
ShellExecute(self.Handle, PChar('open'), PChar('explorer'), PChar(URL), nil, SW_MAXIMIZE); 
Q: 如何打开一个应用程序？
ShellExecute(this->m_hWnd,"open","calc.exe","","", SW_SHOW) ;
或
ShellExecute(this->m_hWnd,"open","notepad.exe",
"c:/MyLog.log","",SW_SHOW);
Q: 如何打开一个同系统程序相关连的文档？
ShellExecute(this->m_hWnd,"open",
"c:/abc.txt","","",SW_SHOW);
Q: 如何打开一个网页？
ShellExecute(this->m_hWnd,"open","[http://www.google.com](http://www.google.com)";,"","",   SW_SHOW);
Q: 如何激活相关程序，发送EMAIL？
ShellExecute(this->m_hWnd,"open",
"[mailto:nishinapp@yahoo.com](mailto:nishinapp@yahoo.com)","","", SW_SHOW);
Q: 如何用系统打印机打印文档？
ShellExecute(this->m_hWnd,"print",
"c:/abc.txt","","", SW_HIDE);
Q: 如何用系统查找功能来查找指定文件？
ShellExecute(m_hWnd,"find","d:/nish",
NULL,NULL,SW_SHOW);
Q: 如何启动一个程序，直到它运行结束？
SHELLEXECUTEINFO ShExecInfo = {0};
ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
ShExecInfo.hwnd = NULL;
ShExecInfo.lpVerb = NULL;
ShExecInfo.lpFile = "c:/MyProgram.exe"; 
ShExecInfo.lpParameters = ""; 
ShExecInfo.lpDirectory = NULL;
ShExecInfo.nShow = SW_SHOW;
ShExecInfo.hInstApp = NULL; 
ShellExecuteEx(&ShExecInfo);
WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
或：
PROCESS_INFORMATION ProcessInfo; 
STARTUPINFO StartupInfo; //入口参数
ZeroMemory(&StartupInfo, sizeof(StartupInfo));
StartupInfo.cb = sizeof StartupInfo ; //分配大小
if(CreateProcess("c:/winnt/notepad.exe", NULL, 
NULL,NULL,FALSE,0,NULL,
NULL,&StartupInfo,&ProcessInfo))
{ 
WaitForSingleObject(ProcessInfo.hProcess,INFINITE);
CloseHandle(ProcessInfo.hThread);
CloseHandle(ProcessInfo.hProcess);
} 
else
{
MessageBox("The process could not be started...");
}
Q: 如何显示文件或文件夹的属性？
SHELLEXECUTEINFO ShExecInfo ={0};
ShExecInfo.cbSize = sizeof
SHELLEXECUTEINFO);
ShExecInfo.fMask = SEE_MASK_INVOKEIDLIST ;
ShExecInfo.hwnd = NULL;
ShExecInfo.lpVerb = "properties";
ShExecInfo.lpFile = "c:/"; //也可以是文件
ShExecInfo.lpParameters = ""; 
ShExecInfo.lpDirectory = NULL;
ShExecInfo.nShow = SW_SHOW;
ShExecInfo.hInstApp = NULL
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/Silitex/archive/2009/02/18/3906697.aspx](http://blog.csdn.net/Silitex/archive/2009/02/18/3906697.aspx)
