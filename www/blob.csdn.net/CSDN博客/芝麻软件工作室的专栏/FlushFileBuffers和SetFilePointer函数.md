
# FlushFileBuffers和SetFilePointer函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:38:35[seven-soft](https://me.csdn.net/softn)阅读数：275


在PC硬件体系结构里，速度最快的存储器是CPU里面的寄存器，接着到二级缓存，再到系统RAM内存，最后才到硬盘。由于这样的体系结构，就决定了操作系统对文件的操作方式，或者说是最优化的算法。比如操作系统接收到写文件的数据时，就会先把数据保存到RAM里，然后在合适的时间或者合适的数据量时再写到硬盘里。但有时候我们希望数据一定要保存到硬盘里，而不是保存在RAM里，这时就需要使用函数FlushFileBuffers来把RAM里的数据保存到硬盘里。文件的结构是一个有序的队列，有头有尾，当读写文件后，就会移动文件里的文件指针。有时候想移动到特定的位置读取数据。比如读取一个BMP的文件，它有文件头和数据块组成，就需要先读取文件头，然后根据文件头里指示数据块开始位置去读取图片显示数据，这时就需要使用到SetFilePointer函数。

函数FlushFileBuffers和SetFilePointer声明如下：
WINBASEAPI
BOOL
WINAPI
FlushFileBuffers(
__in HANDLE hFile
);
WINBASEAPI
DWORD
WINAPI
SetFilePointer(
__in     HANDLE hFile,
__in     LONG lDistanceToMove,
__in_opt PLONG lpDistanceToMoveHigh,
__in     DWORD dwMoveMethod
);
**hFile**是文件句柄。
**lDistanceToMove**是文件指针距离头或尾的长度。
**lpDistanceToMoveHigh**是文件指针距离头或尾的长度高位长度。
**dwMoveMethod**是相对文件头、文件尾或者当前位置的方式。
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
**\#032        //****先把写文件缓冲区的数据强制写入磁盘。**
**\#033        FlushFileBuffers(hFile);**
\#034
\#035         //
\#036         //从文件里读取数据。
\#037         LONG lDistance = 0;
**\#038        DWORD dwPtr = SetFilePointer(hFile, lDistance, NULL, FILE_BEGIN);**
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
\#063  }


