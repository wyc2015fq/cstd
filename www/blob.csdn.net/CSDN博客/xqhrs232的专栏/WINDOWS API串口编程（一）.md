# WINDOWS API串口编程（一） - xqhrs232的专栏 - CSDN博客
2013年04月30日 22:33:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：709
原文地址::[http://xiaozhekobe.blog.163.com/blog/static/175646098201211111556609/](http://xiaozhekobe.blog.163.com/blog/static/175646098201211111556609/)
相关网帖
1、WINDOWS API串口编程（二）----[http://xiaozhekobe.blog.163.com/blog/static/1756460982012111115722105/](http://xiaozhekobe.blog.163.com/blog/static/1756460982012111115722105/)
Win32操作系统中，在设置串口是有许多函数还会用到如设备控制DCB以及超市控制等结构，还有通信错误、通信状态以及通信事件等信息
1、DCB（Device Control Block）结构
在打开通信设备句柄后，常常需要对串口进行一些初始化工作，这需要通过一个DCB结构来进行，DCB结构包含了诸如波特率、每个字符的数据位数、奇偶校验和停止位数等信息。在查询或配置串口的属性时，都要用DCB结构来作为缓冲区
调用GetCommState函数可以获得串口的设置，该函数把当前配置填充到一个DCB结构。一般在用CreateFile打开串口后，可以调用GetCommState函数来获取串行口的初始配置，若要修改串口的配置，应该先修改DCB结构，然后再调用SetCommState函数用指定的DCB结构来设置串行口。
typedef struct _DCB {
  DWORD*DCBlength*;//DCB快大小
  DWORD*BaudRate*;//当前波特率
  DWORD *fBinary*:*1*;//二进制模式，不检测EOF
  DWORD*fParity*:*1*;//允许奇偶校验
  DWORD*fOutxCtsFlow*:*1*;//CTS输出流控制
  DWORD*fOutxDsrFlow*:*1*;//DSR输出流控制
  DWORD*fDtrControl*:*2*;//DTR流控制类型
  DWORD*fDsrSensitivity*:*1*;//对DSR信号线是否敏感
  DWORD*fTXContinueOnXoff*:*1*;//XOFF continues TX
  DWORD*fOutX*:*1*;
  DWORD*fInX*:*1*;
  DWORD*fErrorChar*:*1*;
  DWORD*fNull*:*1*;
  DWORD*fRtsControl*:*2*;
  DWORD*fAbortOnError*:*1*;
  DWORD*fDummy2*:*17*;
  WORD*wReserved*;
  WORD*XonLim*;
  WORD*XoffLim*;  
  BYTE*ByteSize*;
  BYTE*Parity*;
  BYTE*StopBits*;
  char*XonChar*;
  char*XoffChar*;
  char*ErrorChar*;
  char*EofChar*;
  char*EvtChar*;
  WORD*wReserved1*;
} DCB, *LPDCB;
具体的配置看说明
2、超时设置COMMTIMEOUTS结构
typedefstruct _COMMTIMEOUTS 
{ 
    DWORD ReadIntervalTimeout; //读间隔超时
    DWORD ReadTotalTimeoutMultiplier; //读事件系数
    DWORD ReadTotalTimeoutConstant; //读时间常量
    DWORD WriteTotalTimeoutMultiplier; //读时间系数
    DWORD WriteTotalTimeoutConstant; //读时间常量
} COMMTIMEOUTS, *LPCOMMTIMEOUTS 
在用ReadFile和WriteFile读写串行口时，需要考虑超时问题，如果在指定的时间内没有读出或写入指定数量的字节，那么ReadFile和WriteFile的操作就会结束，要查询当前的超市设置应调用GetCommTimeouts函数，该函数会填充COMMTIMEOUTS结构，调用SetCommTimeouts函数可以用某一个COMMTIMEOUTS结构的内容来设置超时
1、ReadIntervalTimeout读时间间隔超时
以毫秒为单位设置通信线路上两个字符到达之间最大时间间隔，在ReadFile操作期间，从接收到第一个字符开始计时，如果任意两个字符达到之间的时间间隔超过这个最大值，则ReadFile操作完成，返回缓冲数据，如果改制被置位0，则不实用时间间隔超时
2、ReadTotalTimeoutMultiplier读时间系数
以毫秒为单位设置一个用来计算读操作总超时时间的时间系数，改时间系数乘要求读出的字节数
3、ReadTotalTimeoutConstant读时间常量
以毫秒为单位设置一个用来计算读操作总超时时间的时间常量
4、WriteTotalTimeoutMultiplier写时间系数
以毫秒为单位设置一个用来计算写操作总超时时间的时间系数，该时间系数乘要求写入的字节数
5、WriteTotalTimeConstatnt写时间常量
以毫秒为单位设置一个用来计算读操作总超时的时间常量
总超时时间=读/写时间系数*要求度/写的字节数+读/写时间常量
超时有间隔超时和总超时两种类型，间隔超时是指在接受时两个字符之间的最大时延，用于从串口读取数据，当接受一个字节时，通信驱动程序启动一个内部定时器开始计时，在下一个字节到达之前，如果定时时间超过了间隔时间，读操作就会被放弃，总超时是指读写操作总共话费的最大时间，写操作只支持总超时，而读操作对两种超时均支持。

