
# SetEndOfFile和GetFileSizeEx函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:37:37[seven-soft](https://me.csdn.net/softn)阅读数：262


有一天，我正在开发BT软件，它有这样的一个功能，就是先把文件的大小分配好，然后再慢慢地往里面对应的位置写入相应的数据。这样的好处，就是可以先把磁盘空间占用起来，以便后面的下载顺利进行。要实现这个功能，就需要创建一个空的文件，然后把文件指针设置到相应大小的位置，然后再调用函数SetEndOfFile来设置文件的结束位置，这样文件就有相应的大小了。在BT软件的开发里，也发现目录的处理时需要详细地记录目录里的文件大小，这就需要使用GetFileSizeEx函数来获取文件的大小。由于BT里的视频文件比较大，有可能几G的，一定要使用GetFileSizeEx函数来处理，这样就可以获取比较大的文件而不出错。
函数FlushFileBuffers和SetFilePointer声明如下：
WINBASEAPI
BOOL
WINAPI
SetEndOfFile(
__in HANDLE hFile
);
BOOL
WINAPI
GetFileSizeEx(
__in HANDLE hFile,
__out PLARGE_INTEGER lpFileSize
);
**hFile**是文件句柄。
**lpFileSize**是获取文件返回的大小。
调用函数的例子如下：
\#001  //创建、写入、读取文件。
\#002  //
\#003  void CreateFileDemo(void)
\#004  {
\#005         //
\#006         HANDLE hFile = ::CreateFile(_T("CreateFileDemo.txt"),     //创建文件的名称。
\#007               GENERIC_WRITE|GENERIC_READ,          //写和读文件。
\#008               0,                      //不共享读写。
\#009               NULL,                   //缺省安全属性。
\#010               CREATE_ALWAYS,          //如果文件存在，也创建。
\#011               FILE_ATTRIBUTE_NORMAL, //一般的文件。
\#012               NULL);                 //模板文件为空。
\#013
\#014         if (hFile == INVALID_HANDLE_VALUE)
\#015         {
\#016               //
\#017               OutputDebugString(_T("CreateFile fail!/r/n"));
\#018         }
\#019
\#020         //往文件里写数据。
\#021         const int BUFSIZE = 4096;
\#022         char chBuffer[BUFSIZE];
\#023         memcpy(chBuffer,"Test",4);
\#024         DWORD dwWritenSize = 0;
\#025         BOOL bRet = ::WriteFile(hFile,chBuffer,4,&dwWritenSize,NULL);
\#026         if (bRet)
\#027         {
\#028               //
\#029               OutputDebugString(_T("WriteFile写文件成功/r/n"));
\#030         }
\#031
\#032         //先把写文件缓冲区的数据强制写入磁盘。
\#033         FlushFileBuffers(hFile);
\#034
\#035         //
\#036         //从文件里读取数据。
\#037         LONG lDistance = 0;
\#038         DWORD dwPtr = SetFilePointer(hFile, lDistance, NULL, FILE_BEGIN);
\#039         if (dwPtr == INVALID_SET_FILE_POINTER)
\#040         {
\#041               //获取出错码。
\#042               DWORD dwError = GetLastError() ;
\#043               //处理出错。
\#044         }
\#045
\#046         DWORD dwReadSize = 0;
\#047         bRet = ::ReadFile(hFile,chBuffer,4,&dwReadSize,NULL);
\#048         if (bRet)
\#049         {
\#050               //
\#051               OutputDebugString(_T("ReadFile读文件成功/r/n"));
\#052         }
\#053         else
\#054         {
\#055               //获取出错码。
\#056               DWORD dwError = GetLastError();
\#057               //处理出错。
\#058               TCHAR chErrorBuf[1024];
\#059               wsprintf(chErrorBuf,_T("GetLastError()=%d/r/n"),dwError);
\#060               OutputDebugString(chErrorBuf);
\#061         }
\#062
\#063         //
\#064         //
\#065         //移动文件指针到新的位置。
\#066         lDistance = 3;
\#067         dwPtr = SetFilePointer(hFile, lDistance, NULL, FILE_BEGIN);
\#068
**\#069        //****设置文件新的结束位置。**
**\#070        ::SetEndOfFile(hFile);**
\#071
\#072         //获取文件的大小。
**\#073        LARGE_INTEGER liFileSize;**
**\#074        ::GetFileSizeEx(hFile,&liFileSize);**
\#075
\#076         TCHAR chTemp[128];
\#077         wsprintf(chTemp,_T("GetFileSizeEx()=%d/r/n"),liFileSize);
\#078         OutputDebugString(chTemp);
\#079
\#080
\#081         //关闭文件。
\#082         if (hFile != INVALID_HANDLE_VALUE)
\#083         {
\#084               //
\#085               CloseHandle(hFile);
\#086         }
\#087
\#088  }


