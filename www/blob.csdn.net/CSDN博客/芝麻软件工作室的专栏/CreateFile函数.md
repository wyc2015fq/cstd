
# CreateFile函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:40:31[seven-soft](https://me.csdn.net/softn)阅读数：400


在软件的需求里，把有用的数据保存起来是非常重要的功能。比如每天的股票行情数据需要保存起来，以便生成K线图。比如游戏客户端的LOG需要保存起，以便客户端出错时可以把LOG发送回来分析它出错的原因。比如银行每天进行交易时，也需要把所有交易的数据保存到文件备份起来，以便进行结算。还有在数据采集领域更是需要保存更多的数据，比如从DV里读取视频和语音数据出来，就会生成12G的巨型文件。比如读DVD光盘里，把光盘做成虚拟光驱也有9G大小。因此，创建文件是非常普通的功能，这个肯定是掌握，并且非常会使用的。当然这个CreateFile函数不但可以创建文件，还可以打串口、并口、网络、USB设备等功能。

函数CreateFile声明如下：
WINBASEAPI
__out
HANDLE
WINAPI
CreateFileA(
__in     LPCSTR lpFileName,
__in     DWORD dwDesiredAccess,
__in     DWORD dwShareMode,
__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
__in     DWORD dwCreationDisposition,
__in     DWORD dwFlagsAndAttributes,
__in_opt HANDLE hTemplateFile
);
WINBASEAPI
__out
HANDLE
WINAPI
CreateFileW(
__in     LPCWSTR lpFileName,
__in     DWORD dwDesiredAccess,
__in     DWORD dwShareMode,
__in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
__in     DWORD dwCreationDisposition,
__in     DWORD dwFlagsAndAttributes,
__in_opt HANDLE hTemplateFile
);
\#ifdef UNICODE
\#define CreateFile CreateFileW
\#else
\#define CreateFile CreateFileA
\#endif // !UNICODE
**lpFileName**是文件或设备的名称。
**dwDesiredAccess**是访问属性。
**dwShareMode**是共享属性。
**lpSecurityAttributes**是安全属性。
**dwCreationDisposition**是创建属性。
**dwFlagsAndAttributes**是文件标志和属性。
**hTemplateFile**是文件模板。
调用函数的例子如下：
\#001  //创建文件。
\#002  //
\#003  void CreateFileDemo(void)
\#004  {
\#005         //
\#006**HANDLE hFile = ::CreateFile(_T("CreateFileDemo.txt"),     //****创建文件的名称。**
**\#007              GENERIC_WRITE,          //****写文件。**
**\#008              0,                      //****不共享读写。**
**\#009              NULL,                   //****缺省安全属性。**
**\#010              CREATE_ALWAYS,          //****如果文件存在，也创建。**
**\#011              FILE_ATTRIBUTE_NORMAL, //****一般的文件。**
**\#012              NULL);                 //****模板文件为空。**
\#013
\#014         if (hFile == INVALID_HANDLE_VALUE)
\#015         {
\#016               //
\#017               OutputDebugString(_T("CreateFile fail!/r/n"));
\#018         }
\#019  }


