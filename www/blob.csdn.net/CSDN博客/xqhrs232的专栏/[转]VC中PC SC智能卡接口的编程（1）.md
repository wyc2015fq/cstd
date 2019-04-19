# [转]VC中PC/SC智能卡接口的编程（1） - xqhrs232的专栏 - CSDN博客
2017年04月18日 11:49:35[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：568
原文地址::[http://blog.chinaunix.net/uid-20613084-id-251326.html](http://blog.chinaunix.net/uid-20613084-id-251326.html)
相关文章
1、[[转]VC中PC/SC智能卡接口的编程（2）](http://blog.chinaunix.net/uid-20613084-id-251327.html) ----[http://blog.chinaunix.net/uid-20613084-id-251327.html](http://blog.chinaunix.net/uid-20613084-id-251327.html)
2、RFID读卡器SDK开发指南V3.5----[https://wenku.baidu.com/view/95f7b45a3b3567ec102d8ac6.html](https://wenku.baidu.com/view/95f7b45a3b3567ec102d8ac6.html)
3、关于rfid Desfire芯片的更改秘钥问题----[http://bbs.csdn.net/topics/392030984](http://bbs.csdn.net/topics/392030984)

到了PC/SC编程，这篇文章写的非常好。
另：有关PC/SC协议的网址：[http://www.gemplus.com.cn/techno/pcsc/](http://www.gemplus.com.cn/techno/pcsc/)
VC中PC/SC智能卡接口的编程
[摘要]本文介绍了如何在VC中通过PC/SC接口实现对智能卡读写器的操作，并给出了详细的例子代码。
[关键词] 智能卡、PC/SC、智能卡读写器
1 引言
完整的智能卡应用系统由后台服务程序、主机或终端应用程序和智能卡等组成，如图1所示。其中，后台服务程序提供了支持智能卡的服务。例如，在一个电子付款系统中，后台服务程序可以提供到信用卡和帐户信息的访问；主机或终端应用程序一般存在于台式机或者终端、电子付款终端、手机或者一个安全子系统中，终端应用程序要处理用户、智能卡和后台服务程序之间的通讯；智能卡则存储用户的一些信息。
终端应用程序需要通过读卡器来访问智能卡，在一个系统中，通常存在多家厂商提供的读卡器，因此需要一个统一的读卡器设备驱动接口。
随着智能卡的广泛应用，为解决计算机与各种读卡器之间的互操作性问题，人们提出了PC/SC（Personal Computer/Smart Card）规范，PC/SC规范作为读卡器和卡与计算机之间有一个标准接口，实现不同生产商的卡和读卡器之间的互操作性，其独立于设备的 API使得应用程序开发人员不必考虑当前实现形式和将来实现形式之间的差异，并避免了由于基本硬件改变而引起的应用程序变更，从而降低了软件开发成本。
Microsoft在其Platform SDK中实现了PC/SC，作为连接智能卡读卡器与计算机的一个标准模型，提供了独立于设备的 API，并与Windows平台集成。因此，我们可以用PC/SC接口来访问智能卡。
2 PC/SC概述
PC/SC接口包含30多个以Scard为前缀的函数，所有函数的原型都在winscard.h中声明，应用程序需要包含winscard.lib，所有函数的正常返回值都是SCARD_S_SUCCESS。在这30多个函数中，常用的函数只有几个，与智能卡的访问流程（图2）对应，下面将详细介绍这些常用函数。
3 PC/SC的使用
3.1建立资源管理器的上下文
函数ScardEstablishContext()用于建立将在其中进行设备数据库操作的资源管理器上下文（范围）。
函数原型：LONG SCardEstablishContext(DWORD dwScope,  LPCVOID pvReserved1,  LPCVOID pvReserved2,  LPSCARDCONTEXT phContext);
各个参数的含义：（1）dwScope：输入类型；表示资源管理器上下文范围，取值为：SCARD_SCOPE_USER（在用户域中完成设备数据库操作）、SCARD_SCOPE_SYSTEM（在系统域中完成设备数据库操作）。要求应用程序具有相应的操作权限。（2）pvReserved1：输入类型；保留，必须为NULL。（3）pvReserved2：输入类型；保留，必须为NULL。（4）phContext：输出类型；建立的资源管理器上下文的句柄。
下面是建立资源管理器上下文的代码：
SCARDCONTEXT        hSC;
LONG                lReturn;
lReturn = SCardEstablishContext(SCARD_SCOPE_USER, NULL, NULL, &hSC);
if ( lReturn!=SCARD_S_SUCCESS )
printf("Failed SCardEstablishContext\n");
3.2 获得系统中安装的读卡器列表
函数ScardListReaders()可以列出系统中安装的读卡器的名字。
函数原型：LONG SCardListReaders(SCARDCONTEXT hContext,  LPCTSTR mszGroups,  LPTSTR mszReaders,  LPDWORD pcchReaders);
各个参数的含义：（1）hContext：输入类型；ScardEstablishContext()建立的资源管理器上下文的句柄，不能为NULL。（2）mszGroups：输入类型；读卡器组名，为NULL时，表示列出所有读卡器。（3）mszReaders：输出类型；系统中安装的读卡器的名字，各个名字之间用’\0’分隔，最后一个名字后面为两个连续的’\0’。（4）pcchReaders：输入输出类型；mszReaders的长度。
系统中可能安装多个读卡器，因此，需要保存各个读卡器的名字，以便以后与需要的读卡器建立连接。
下面是获得系统中安装的读卡器列表的代码：
char            mszReaders[1024];
LPTSTR          pReader, pReaderName[2];
DWORD           dwLen=sizeof(mzsReaders);
int             nReaders=0;
lReturn = SCardListReaders(hSC, NULL, (LPTSTR)mszReaders, &dwLen);
if ( lReturn==SCARD_S_SUCCESS )
{
pReader = (LPTSTR)pmszReaders;
while (*pReader !='\0'  )
{
if ( nReaders<2 ) //使用系统中前2个读卡器
                pReaderName[nReaders++]=pReader;
printf("Reader: %S\n", pReader );
//下一个读卡器名
pReader = pReader + strlen(pReader) + 1;
}
}
3.3 与读卡器（智能卡）连接
函数ScardConnect()在应用程序与读卡器上的智能卡之间建立一个连接。
函数原型：LONG SCardConnect(SCARDCONTEXT hContext,  LPCTSTR szReader,  DWORD dwShareMode,  DWORD dwPreferredProtocols,  LPSCARDHANDLE phCard,  LPDWORD pdwActiveProtocol);
各个参数的含义：（1）hContext：输入类型；ScardEstablishContext()建立的资源管理器上下文的句柄。（2）szReader：输入类型；包含智能卡的读卡器名称（读卡器名称由ScardListReaders()给出）。（3）dwShareMode：输入类型；应用程序对智能卡的操作方式，SCARD_SHARE_SHARED（多个应用共享同一个智能卡）、SCARD_SHARE_EXCLUSIVE（应用独占智能卡）、SCARD_SHARE_DIRECT（应用将智能卡作为私有用途，直接操纵智能卡，不允许其它应用访问智能卡）。（4）dwPreferredProtocols：输入类型；连接使用的协议，SCARD_PROTOCOL_T0（使用T=0协议）、SCARD_PROTOCOL_T1（使用T=1协议）。（5）phCard：输出类型；与智能卡连接的句柄。（6）PdwActiveProtocol：输出类型；实际使用的协议。
下面是与智能卡建立连接的代码：
SCARDHANDLE     hCardHandle[2];
DWORD           dwAP;
lReturn = SCardConnect( hContext, pReaderName[0],    SCARD_SHARE_SHARED,
SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCardHandle[0], &dwAP );
if ( lReturn!=SCARD_S_SUCCESS )
{
printf("Failed SCardConnect\n");
exit(1);
}
与智能卡建立连接后，就可以向智能卡发送指令，与其交换数据了。
