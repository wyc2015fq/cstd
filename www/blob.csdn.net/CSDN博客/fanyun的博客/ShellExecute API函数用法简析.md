# ShellExecute API函数用法简析 - fanyun的博客 - CSDN博客
2016年05月31日 09:55:38[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2849
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
          在VC/C++编程中，我们会经常遇到打开文件、网页、可执行程序的应用场景，ShellExecute API函数就可以做到这一点。现在我们看看它的强大！
**  ShellExecute函数原型：**
HINSTANCE ShellExecute(
                           HWND hwnd, 
                           LPCTSTR lpOperation,
                           LPCTSTR lpFile, 
                           LPCTSTR lpParameters, 
                           LPCTSTR lpDirectory,
                           INT nShowCmd
);   
**ShellExecute函数参数说明：**
       hwnd: 
用于指定父窗口句柄。当函数调用过程出现错误时，它将作为Windows消息窗口的父窗口。
lpOperation: 
用于指定要进行的操作。
“open”操作表示执行由lpFile参数指定的程序，或打开由lpFile参数指定的文件或文件夹；
“print”操作表示打印由lpFile参数指定的文件；
“explore”操作表示浏览由lpFile参数指定的文件夹。
当参数设为NULL时，表示执行默认操作“open”。
      lpFile:
用于指定要打开的文件名、要执行的程序文件名或要浏览的文件夹名。（设置该参数用相对路径时，应将调用的文件与源程序放在同一目录下，否则不出错，但也打不开文件）
     lpParameters:
若lpFile参数是一个可执行程序，则此参数指定命令行参数，否则此参数应为NULL.
     lpDirectory:
用于指定默认目录.
    nShowCmd:
若lpFile参数是一个可执行程序，则此参数指定程序窗口的初始显示方式，否则此参数应设置为0。
这个参数常用的常数：
SW_HIDE 隐藏窗口，活动状态给令一个窗口 
SW_MINIMIZE 最小化窗口，活动状态给令一个窗口 
SW_RESTORE 用原来的大小和位置显示一个窗口，同时令其进入活动状态 
SW_SHOW 用当前的大小和位置显示一个窗口，同时令其进入活动状态 
SW_SHOWMAXIMIZED 最大化窗口，并将其激活 
SW_SHOWMINIMIZED 最小化窗口，并将其激活 
SW_SHOWMINNOACTIVE 最小化一个窗口，同时不改变活动窗口 
SW_SHOWNA 用当前的大小和位置显示一个窗口，不改变活动窗口 
SW_SHOWNOACTIVATE 用最近的大小和位置显示一个窗口，同时不改变活动窗口 
SW_SHOWNORMAL 与SW_RESTORE相同
若ShellExecute函数调用成功，则返回值为被执行程序的实例句柄。若返回值小于32，则表示出现错误。  
**  ShellExecute函数使用方法：**
例如：
        ShellExecute(NULL,"open","iloveu.bmp",NULL,NULL,SW_SHOWNORMAL);      
用缺省的位图编辑器打开一个叫iloveu.bmp的位图文件，这个缺省的位图编辑器可能是 Microsoft Paint, Adobe Photoshop,或者Corel
 PhotoPaint。
这个函数能打开任何文件，甚至是桌面和URL快捷方式（.ink或.url）。ShellExecute解析系统注册表HKEY_CLASSES_ROOT中所有的内容，判断启动那一个执行程序，并且启动一个新的实例或使用DDE将文件名连到一打开的实例。然后，ShellExecute返回打开文件的应用的实例句柄。
ShellExecute(NULL, "open", "http://www.microsoft.com",NULL, NULL, SW_SHOWNORMAL); 
这个代码使你能访问微软的主页。当ShellExecute遇到文件名前面的“http:”时，可以判断出要打开的文件是Web文件，随之启动Internet
 Explorer 或者 Netscape Navigator或者任何你使用的别的浏览器打开文件。
ShellExecute还能识别其它协议，象FTP、GOPHER。甚至识别“mailto”，如果文件名指向“mailto:zxn@hq.cninfo.net”,它启动电子邮件程序并打开一个待编辑的新邮件，例如：
       ShellExecute(NULL,"open",“mailto:zxn@hq.cninfo.net”, NULL, NULL, SW_SHOWNORMAL);打开新邮件窗口。
总之，ShellExecute函数就是如此简单地打开磁盘文件和Internet文件。如果将第二个参数“OPEN”改为“PRINT”或者“EXPLORE”，ShellExecute将能打印文件和打开文件夹。ShellExecute还有一个扩展函数ShellExecuteEx，所带参数中有一个特殊的结构，功能更强，或者任何你使用的别的浏览器打开文件。
**Q: 如何打开一个应用程序？**
ShellExecute(this->m_hWnd,"open","calc.exe","","", SW_SHOW ); 或 ShellExecute(this->m_hWnd,"open","notepad.exe","c:\\MyLog.log","",SW_SHOW );正如您所看到的，我并没有传递程序的完整路径。
**Q: ****如何打开一个同系统程序相关连的文档？**
ShellExecute(this->m_hWnd,"open","c:\\abc.txt","","",SW_SHOW );
**Q: ****如何打开一个网页？**
ShellExecute(this->m_hWnd,"open","[http://www.google.com](http://www.google.com/)","","", SW_SHOW );
**Q: ****如何激活相关程序，发送EMAIL？**
ShellExecute(this->m_hWnd,"open","mailto:nishinapp@yahoo.com","","", SW_SHOW );
**Q: ****如何用系统打印机打印文档？**
ShellExecute(this->m_hWnd,"print","c:\\abc.txt","","", SW_HIDE);
**Q: ****如何用系统查找功能来查找指定文件？**
ShellExecute(m_hWnd,"find","d:\\nish",NULL,NULL,SW_SHOW);
**Q: ****如何启动一个程序，直到它运行结束？**
SHELLEXECUTEINFO ShExecInfo = {0};
ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
ShExecInfo.hwnd = NULL;
ShExecInfo.lpVerb = NULL;
ShExecInfo.lpFile = "c:\\MyProgram.exe";
ShExecInfo.lpParameters = "";
ShExecInfo.lpDirectory = NULL;
ShExecInfo.nShow = SW_SHOW;
ShExecInfo.hInstApp = NULL;
ShellExecuteEx(&ShExecInfo);
WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
或：
PROCESS_INFORMATION ProcessInfo;
STARTUPINFO StartupInfo; //This is an [in] parameter
ZeroMemory(&StartupInfo, sizeof(StartupInfo));
StartupInfo.cb = sizeof StartupInfo ; //Only compulsory field
if(CreateProcess("c:\\winnt\\notepad.exe", NULL,
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
**Q: ****如何显示文件或文件夹的属性？**
SHELLEXECUTEINFO ShExecInfo ={0};
ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
ShExecInfo.fMask = SEE_MASK_INVOKEIDLIST ;
ShExecInfo.hwnd = NULL;
ShExecInfo.lpVerb = "properties";
ShExecInfo.lpFile = "c:\\"; //can be a file as well
ShExecInfo.lpParameters = "";
ShExecInfo.lpDirectory = NULL;
ShExecInfo.nShow = SW_SHOW;
ShExecInfo.hInstApp = NULL;
ShellExecuteEx(&ShExecInfo);
