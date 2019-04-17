# minidump-DMP文件的生成和使用 - DoubleLi - 博客园






转载地址[点击打开链接](http://blog.csdn.net/eckelwei/article/details/7662826)

1、生成dmp的程序

#include  <dbghelp.h> 
#pragma comment(lib,  "dbghelp.lib")


//设置异常处理回调函数
SetUnhandledExceptionFilter(UnhandledExceptionFilter);


//异常处理代码
//EXCEPTION_EXECUTE_HANDLER equ 1 表示我已经处理了异常,可以优雅地结束了 
//EXCEPTION_CONTINUE_SEARCH equ 0 表示我不处理,其他人来吧,于是windows调用默认的处理程序显示一个错误框,并结束 
//EXCEPTION_CONTINUE_EXECUTION equ -1 表示错误已经被修复,请从异常发生处继续执行 
LONG UnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    CString   strDumpFile; 
    strDumpFile.Format("%d",CTime::GetCurrentTime().GetTickCount());
    strDumpFile   +=  ".dmp "; 
    HANDLE   hFile   =   CreateFile(strDumpFile,   GENERIC_WRITE,   FILE_SHARE_WRITE,   NULL,   CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,   NULL   );

    if   (hFile!=INVALID_HANDLE_VALUE)
    { 
        MINIDUMP_EXCEPTION_INFORMATION   ExInfo; 

        ExInfo.ThreadId   =   ::GetCurrentThreadId();
        ExInfo.ExceptionPointers   =   ExceptionInfo;
        ExInfo.ClientPointers   =   NULL;

        //   write   the   dump
        BOOL   bOK   =   MiniDumpWriteDump(GetCurrentProcess(),   GetCurrentProcessId(),   hFile,   MiniDumpNormal,  &ExInfo,   NULL,   NULL   );
        CloseHandle(hFile); 
    } 

    theApp.PostThreadMessage(WM_QUIT,0,0);
    return EXCEPTION_EXECUTE_HANDLER;
}

2、调试dump文件

    1）用windbg。

    2）用vc，把dmp文件和exe, pdb文件放在同一目录下, 然后用编译器(如vc)打开, 然后开始调试就会中断到刚才中断的地方.


3、以下是网上搜集到的条件，尚未验证是否是必需的，如果不能正确的生成dmp，可尝试以下方法

    1）SetUnhandledExceptionFilter()并不能处理所有的异常，比如CRT异常需要调用_set_invalid_parameter_handler()才行

    1）要设置c运行库异常处理函授。这样当出错后，会引发你的异常处理，你的处理中，可以直接丢掉或重新启动你的exe,或写入log文件。

    2）注意生成的release应用程序需要开启“生成调试信息”选项。

    2）使用VS2008的话，不需要开启“生成调试信息”选项就可以生成的。

    3）编译release版本的时候同时生成map文件，具体可以在网上查找一下关于map文件的使用方法。

    3）生成可执行文件时在工程属性设置里的“调试”选项里将“生成映射文件”设置为“是”。这主要是用来在出错时通过dump和map文件对错误定位。

4、其他工具

    1）生成dump文件需要Dr.Watson，它是Windows自带的调试工具,只要对它进行了设置，程序出错后就可以在相应目录下找到dump文件。可参见如下资料：[http://hi.baidu.com/mryongzhang/blog/item/11a774edf9b513d72e2e2167.html](http://hi.baidu.com/mryongzhang/blog/item/11a774edf9b513d72e2e2167.html)

    2）adplus是最有用的工具，[http://support.microsoft.com/kb/286350/](http://support.microsoft.com/kb/286350/)









