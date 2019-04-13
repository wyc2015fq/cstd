
# WriteFile和ReadFile函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月06日 15:39:34[seven-soft](https://me.csdn.net/softn)阅读数：256标签：[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[磁盘																](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)[file																](https://so.csdn.net/so/search/s.do?q=file&t=blog)[网络																](https://so.csdn.net/so/search/s.do?q=网络&t=blog)[bt																](https://so.csdn.net/so/search/s.do?q=bt&t=blog)[游戏																](https://so.csdn.net/so/search/s.do?q=游戏&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=bt&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=网络&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)
[
																								](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=file&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=file&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=磁盘&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=null&t=blog)
读写文件是每个Windows软件开发人员都需要做的工作。可见这项工作是非常重要的，毕竟各种各样的数据都需要保存起来，以便作各种各样的分析，或者通过网络传送给别人。像大家用BT下载的电影，在那个BT软件里，就需要不断从网络里接收到数据，然后再把这些数据保存到文件里合适的位置，就可以生成跟发行者那里一样的文件，这样才可以播放出来。又比如我在玩《征途》的游戏里，刚刚打开游戏时，它就不断从服务器上下载更新的文件下来，然后保存到硬盘。WriteFile函数是用来写数据到文件，ReadFile函数是从文件里读取数据出来。但这两个函数不但可以读取写磁盘的文件，也可以接收和发送网络的数据，还有读写串口、USB、并口等设备的数据。在读写文件里，首先就是先打开文件，然后判断打开是否成功。在写文件时，同时要注意磁盘的空间是否满等问题。在读取文件时，往往需要读取不同位置的文件，比如要读取一个4G的视频文件，就不可能完全把它读取到内存里，因此就需要对文件进行定位读取。
函数WriteFile和ReadFile声明如下：
WINBASEAPI
BOOL
WINAPI
WriteFile(
__in        HANDLE hFile,
__in_bcount(nNumberOfBytesToWrite) LPCVOID lpBuffer,
__in        DWORD nNumberOfBytesToWrite,
__out_opt   LPDWORD lpNumberOfBytesWritten,
__inout_opt LPOVERLAPPED lpOverlapped
);
WINBASEAPI
BOOL
WINAPI
ReadFile(
__in        HANDLE hFile,
__out_bcount_part(nNumberOfBytesToRead, *lpNumberOfBytesRead) LPVOID lpBuffer,
__in        DWORD nNumberOfBytesToRead,
__out_opt   LPDWORD lpNumberOfBytesRead,
__inout_opt LPOVERLAPPED lpOverlapped
);
**hFile**是文件句柄。
**lpBuffer**是读写数据缓冲区。
**nNumberOfBytesToWrite**是多少数据要写入。
**lpNumberOfBytesWritten**是已经写入多少数据。
**nNumberOfBytesToRead**是多少数据要读取。
**nNumberOfBytesToRead**是已经读取多少数据。
**lpOverlapped**是异步读写的结构。

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
**\#025        BOOL bRet = ::WriteFile(hFile,chBuffer,4,&dwWritenSize,NULL);**
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
**\#047        bRet = ::ReadFile(hFile,chBuffer,4,&dwReadSize,NULL);**
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


