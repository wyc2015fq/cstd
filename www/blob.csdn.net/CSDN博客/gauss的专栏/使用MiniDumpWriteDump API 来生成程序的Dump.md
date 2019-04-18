# 使用MiniDumpWriteDump API 来生成程序的Dump - gauss的专栏 - CSDN博客
2013年01月04日 00:24:12[gauss](https://me.csdn.net/mathlmx)阅读数：406
个人分类：[开发工具/调试技巧](https://blog.csdn.net/mathlmx/article/category/1317877)
**MiniDumpWriteDump**是MS DbgHelp.dll 中一个API, 用于导出当前运行的程序的Dump. 这个dll程序系统中就有, 但是很多软件, 都在自己的安装目录下保存了这个.dll的最新的版本.
为了测试这个API, 参考网上一些资料, 写了一个简单的C++ 程序. 目的是当有异常发生的时候, 自动生成Dump文件供之后的分析. 有了Dump文件, 我们就可以使用WinDBG等调试器来分析异常发生时的情况. 其实这个功能很多软件都有, 比如QQ, 魔兽世界, 等等. 它们在出现了异常的时候会弹出一个对话框, 让用户输入异常发生时的情况, 然后把异常的dump文件用email发回, 供开发者们分析修改bug.
不过有一点, 这里需要程序的调试符号文件(pdb文件). 对于Debug版来说, 是生成的, 但是Release版来说默认是不生成的. 可以设置VC的编译器, 让它在Release版的时候也生成调试信息. 这带来一个新的问题, 因为.pdb里面是保存了源文件的信息的, 为了避免泄密, 可以采用VS中的CVPack工具, 从中去除敏感的信息.
程序需要使用**Dbghelp.h** 和 **Dbghelp.lib** . 它们可以从MSDN找到.
//最主要的函数,生成Dump
static void DumpMiniDump(HANDLE hFile, PEXCEPTION_POINTERS excpInfo)
{
if (excpInfo == NULL) //如果没有传入异常,比如是在程序里面调用的, 生成一个异常
{
// Generate exception to get proper context in dump
__try 
{
OutputDebugString(_T("raising exception\r\n"));
RaiseException(EXCEPTION_BREAKPOINT, 0, 0, NULL);
} 
__except(DumpMiniDump(hFile, GetExceptionInformation()),
EXCEPTION_CONTINUE_EXECUTION) 
{
}
} 
else
{
OutputDebugString(_T("writing minidump\r\n"));
MINIDUMP_EXCEPTION_INFORMATION eInfo;
eInfo.ThreadId = GetCurrentThreadId(); //把需要的信息添进去
eInfo.ExceptionPointers = excpInfo;
eInfo.ClientPointers = FALSE;
    // 调用,生成Dump. 98不支持
// Dump的类型是小型的, 节省空间. 可以参考MSDN生成更详细的Dump.
MiniDumpWriteDump(
GetCurrentProcess(),
GetCurrentProcessId(),
hFile,
MiniDumpNormal,
excpInfo ? &eInfo : NULL,
NULL,
NULL);
}
}
下面的是程序部分:
int _tmain(int argc, _TCHAR* argv[])
{
// 创建一个Dump文件
HANDLE hFile = CreateFile( _T("MiniDump.dmp"), GENERIC_READ | GENERIC_WRITE, 
0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ); 
int code;
__try 
{   
// 把自己实现的main函数包装一下, 放在try .. except 块中. 这样出现了异常可以自动生成dump
main_wrapper(argc, argv); 
}
__except( code=GetExceptionCode(), DumpMiniDump(hFile, GetExceptionInformation() ),       EXCEPTION_EXECUTE_HANDLER )//出现了异常,记录异常的code, 生成dump!!
{
printf("%x\n", code);
wchar_t msg[512];
wsprintf(msg, L"Exception happened. Exception code is %x", code);
MessageBox(NULL, msg, L"Exception", MB_OK); //显示消息给用户
}
CloseHandle( hFile ); //关闭Dump文件
getchar();
return 0; 
}
最下面是两个测试的函数, main_wrapper函数将调用test1, test1将会生成一个异常(非法内存写)
void test1() {
int *p;
p = (int*)0x100; 
*p = 0; //写0x100地址, 这个是非法的
}
void main_wrapper(int argc, _TCHAR* argv[]) {
test1();
}
运行, 异常被捕获了:
![clip_image001](http://www.cppblog.com/images/cppblog_com/woaidongmao/WindowsLiveWriter/MiniDumpWriteDumpAPIDump_9BF6/clip_image001_thumb.jpg)
同时, dump文件也生成了:
![clip_image002](http://www.cppblog.com/images/cppblog_com/woaidongmao/WindowsLiveWriter/MiniDumpWriteDumpAPIDump_9BF6/clip_image002_thumb.jpg)
用WinDBG打开Dump文件, 可以清楚的看出异常出现的情况:
![clip_image003](http://www.cppblog.com/images/cppblog_com/woaidongmao/WindowsLiveWriter/MiniDumpWriteDumpAPIDump_9BF6/clip_image003_thumb.jpg)
从中可以比较清楚的看到异常发生的情况(Exception code), 异常出现的地址(test1函数, 偏移0x28). 因为这次测试的是Debug版, 有保存了源代码的.pdb文件, 所以WinDbg把源代码也列出来了. 这样可以非常容易的发现问题.
============================================
参考:
DbgHelp中的DumpAPI例子: http://www.debuginfo.com/examples/src/effminidumps/MiniDump.cpp
CrashReport: 程序出现异常的时候显示发送错误的对话框, 并把Dump文件发送到指定的地址. http://code.google.com/p/crashrpt/
XCrashReport: 与上面的类似的一个开源项目. http://www.codeproject.com/KB/debug/XCrashReportPt1.aspx
