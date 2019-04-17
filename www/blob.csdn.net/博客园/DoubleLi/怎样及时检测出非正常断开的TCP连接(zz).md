# 怎样及时检测出非正常断开的TCP连接(zz) - DoubleLi - 博客园






此处的”非正常断开”指TCP连接不是以优雅的方式断开,如网线故障等物理链路的原因,还有突然主机断电等原因
有两种方法可以检测:1.TCP连接双方定时发握手消息 2.利用TCP协议栈中的KeepAlive探测
第二种方法简单可靠,只需对TCP连接两个Socket设定KeepAlive探测,所以本文只讲第二种方法在Linux,Window2000下的实现(在其它的平台上没有作进一步的测试)
Windows 2000平台下
//定义结构及宏
struct TCP_KEEPALIVE {
u_longonoff;
u_longkeepalivetime;
u_longkeepaliveinterval;
} ;

#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR,4)

//KeepAlive实现
TCP_KEEPALIVE inKeepAlive = {0}; //输入参数
unsigned long ulInLen = sizeof(TCP_KEEPALIVE);

TCP_KEEPALIVE outKeepAlive = {0}; //输出参数
unsigned long ulOutLen = sizeof(TCP_KEEPALIVE);

unsigned long ulBytesReturn = 0;

//设置socket的keep alive为5秒，并且发送次数为3次
inKeepAlive.onoff = 1; 
inKeepAlive.keepaliveinterval = 5000; //两次KeepAlive探测间的时间间隔
inKeepAlive.keepalivetime = 5000; //开始首次KeepAlive探测前的TCP空闭时间

if (WSAIoctl((unsigned int)s, SIO_KEEPALIVE_VALS,
(LPVOID)&inKeepAlive, ulInLen,
(LPVOID)&outKeepAlive, ulOutLen,
&ulBytesReturn, NULL, NULL) == SOCKET_ERROR) 
{ 
ACE_DEBUG ((LM_INFO,
ACE_TEXT ("(%P|%t) /WSAIoctl failed. error code(%d)!/n"),WSAGetLastError()));
}

Linux平台下
#include 
……
////KeepAlive实现
//下面代码要求有ACE,如果没有包含ACE,则请把用到的ACE函数改成linux相应的接口
int keepAlive = 1;//设定KeepAlive
int keepIdle = 5;//开始首次KeepAlive探测前的TCP空闭时间
int keepInterval = 5;//两次KeepAlive探测间的时间间隔
int keepCount = 3;//判定断开前的KeepAlive探测次数

if(setsockopt(s,SOL_SOCKET,SO_KEEPALIVE,(void*)&keepAlive,sizeof(keepAlive)) == -1)
{
ACE_DEBUG ((LM_INFO,
ACE_TEXT ("(%P|%t) setsockopt SO_KEEPALIVE error!/n")));

}

if(setsockopt(s,SOL_TCP,TCP_KEEPIDLE,(void *)&keepIdle,sizeof(keepIdle)) == -1)
{
ACE_DEBUG ((LM_INFO,
ACE_TEXT ("(%P|%t) setsockopt TCP_KEEPIDLE error!/n")));
}

if(setsockopt(s,SOL_TCP,TCP_KEEPINTVL,(void *)&keepInterval,sizeof(keepInterval)) == -1)
{
ACE_DEBUG ((LM_INFO,
ACE_TEXT ("(%P|%t) setsockopt TCP_KEEPINTVL error!/n")));
}

if(setsockopt(s,SOL_TCP,TCP_KEEPCNT,(void *)&keepCount,sizeof(keepCount)) == -1)
{
ACE_DEBUG ((LM_INFO,
ACE_TEXT ("(%P|%t)setsockopt TCP_KEEPCNT error!/n")));
}









