# Qt程式异常崩溃处理技巧(Win) - baidu_33570760的博客 - CSDN博客
2016年08月16日 15:43:39[carman_风](https://me.csdn.net/baidu_33570760)阅读数：14555
     文章转载来自     http://www.cnblogs.com/lcchuguo/p/5177715.html     作者 [lcchuguo](http://www.cnblogs.com/lcchuguo/)
[](http://www.cnblogs.com/lcchuguo/)
[这篇文章谈的是 Qt4 程式在视窗系统下的异常崩溃处理技巧。所以须要在头文件里包括“#include <Windows.h>”。](http://www.cnblogs.com/lcchuguo/)
首先，程式难免会有异常崩溃的时候。重要的是在崩溃时能及时把重要的数据保存好，将损失减少。
SetUnhandledExceptionFilter函数是Win32API的异常捕获函数，在程式异常结束前。会调用该函数注冊的回调函数，这样就能在进程终止前运行指定的代码，达到比如保存数据的功能。
```cpp
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException){//程式异常捕获
    /*
      ***保存数据代码***
    */
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((uint)record->ExceptionAddress,16)),errMod;
    QMessageBox::critical(NULL,"程式崩溃","<FONT size=4><div><b>对于发生的错误，表示诚挚的歉意</b><br/></div>"+
        QString("<div>错误代码：%1</div><div>错误地址：%2</div></FONT>").arg(errCode).arg(errAdr),
        QMessageBox::Ok);
    return EXCEPTION_EXECUTE_HANDLER;
}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
    QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
    QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注冊异常捕获函数
    MainWindow w;
    w.showMaximized();
    return a.exec();
}
```
保存数据仅是拯救措施，更重要的是找到错误的根源。若能在崩溃的同一时候，程式自己主动记录下崩溃时的执行信息，将有助于修正工作。微软提供了“DbgHelp”错误调试技术。调用相关功能就可以保存程式崩溃时的信息，然后借助WinDbg软件就能分析出当时的执行状况。
调用“DbgHelp”的MiniDumpWriteDump函数保存以“.dmp”为后缀的Dump文件，该文件能被WinDbg读取并分析。
你须要加入头文件“#include <DbgHelp.h>”，在Pro文件里加入“LIBS += -lDbgHelp”。目的是链接DbgHelp库。
```cpp
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException){//程式异常捕获
    /*
      ***保存数据代码***
    */
    //创建 Dump 文件
    HANDLE hDumpFile = CreateFile(QTime::currentTime().toString("HH时mm分ss秒zzz.dmp").utf16(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if( hDumpFile != INVALID_HANDLE_VALUE){
        //Dump信息
        MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
        dumpInfo.ExceptionPointers = pException;
        dumpInfo.ThreadId = GetCurrentThreadId();
        dumpInfo.ClientPointers = TRUE;
        //写入Dump文件内容
        MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
    }
    //这里弹出一个错误对话框并退出程序
    EXCEPTION_RECORD* record = pException->ExceptionRecord;
    QString errCode(QString::number(record->ExceptionCode,16)),errAdr(QString::number((uint)record->ExceptionAddress,16)),errMod;
    QMessageBox::critical(NULL,"程式崩溃","<FONT size=4><div><b>对于发生的错误，表示诚挚的歉意</b><br/></div>"+
        QString("<div>错误代码：%1</div><div>错误地址：%2</div></FONT>").arg(errCode).arg(errAdr),
        QMessageBox::Ok);
    return EXCEPTION_EXECUTE_HANDLER;
}
```
当被错误困扰得焦头烂额的时候。若是老天能直接告诉错误在哪一行代码该有多好呀。其实WinDbg就能做到。
在项目的proproject文件里增加：QMAKE_LFLAGS_RELEASE = /INCREMENTAL:NO /DEBUG
这句话的目的是Release版也将生成“.pdb”后缀的调试信息文件。在使用WinDbg导入Dump前。指定好源代码与pdb文件的位置。就可以在错误报告内看到罪魁祸首是哪一行代码。
