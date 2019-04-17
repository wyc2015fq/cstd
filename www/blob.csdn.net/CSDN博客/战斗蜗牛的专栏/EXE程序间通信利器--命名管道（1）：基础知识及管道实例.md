# EXE程序间通信利器--命名管道（1）：基础知识及管道实例 - 战斗蜗牛的专栏 - CSDN博客





2012年11月12日 22:16:32[vbskj](https://me.csdn.net/vbskj)阅读数：2232










        命名管道作为一种[网络编程](http://baike.baidu.com/view/1317473.htm)方案使用时,它实际上建立一个简单的客户机/服务器数据通信体系,可在其中可靠地传输数据。当你的应用只是对于本机的多个exe程序之间通信时，命名管道是一个节省资源、小型化的解决方案。

         命名管道"或"命名管线"(Named Pipes)是一种简单的[进程间通信](http://baike.baidu.com/view/1492468.htm)(I P C)机制,Microsoft Windows NT,Windows 2000,Windows 95以及Windows 98均提供了对它的支持(但不包括Windows
 CE).命名管道可在同一台计算机的不同进程之间,或在跨越一个网络的不同计算机的不同进程之间,支持可靠的,单向或双向的数据通信.用命名管道来设计应用程序实际非常简单,并不需要事先深入掌握基层网络传送协议(如T C P / I P或I P X)的知识.这是由于命名管道利用了[微软](http://baike.baidu.com/view/2353.htm)网络提供者(M S N P)[重定向器](http://baike.baidu.com/view/1156161.htm),通过一个网络,在各进程间建立通信.这样一来,应用程序便不必关心网络协议的细节.之所以要用命名管道作为自己的网络通信方案,一项重要的原因是它们充分利用了Windows
 NT及Windows 2000内建的安全特性.

++++++++++++++++++++++++++++++++++++++++++++++

废话不多说，下面说说管道通信的实现办法，有socket基础的更容易理解：




首先，需要新建两个windows console application工程，一个为server端，一个为client端。

（1）Server端

#include <stdio.h>

#include <windows.h>//pipe所依赖的头文件，或者Winbase.h

void main(int argc,char* argv[])

{

HANDLE hPipe = NULL;//首先，声明管道句柄


char buf[100];
DWORD dwRead;





  /*1. 创建命名管道，并等待client的连接*/

hPipe=CreateNamedPipe("\\\\.\\pipe\\TestPipe",
PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
0,1,1024,1024,0,NULL);          //创建命名管道TestPipe
if(INVALID_HANDLE_VALUE==hPipe)
{
printf("创建命名管道失败！");
hPipe=NULL;
return;
}
HANDLE hEvent;
hEvent=CreateEvent(NULL,TRUE,FALSE,NULL);
if(!hEvent)
{
printf("创建事件对象失败！");
CloseHandle(hPipe);
hPipe=NULL;
return;
}
OVERLAPPED ovlap;
ZeroMemory(&ovlap,sizeof(OVERLAPPED));
ovlap.hEvent=hEvent;
if(!ConnectNamedPipe(hPipe,&ovlap))//在此阻塞的等待client来连接命名管道
{
if(ERROR_IO_PENDING!=GetLastError())
{
printf("等待客户端连接失败！");
CloseHandle(hPipe);
CloseHandle(hEvent);
hPipe=NULL;
return;
}
}
if(WAIT_FAILED==WaitForSingleObject(hEvent,INFINITE))
{
printf("等待对象失败！");
CloseHandle(hPipe);
CloseHandle(hEvent);
hPipe=NULL;
return;
}
CloseHandle(hEvent);





printf(“管道连接成功。。。。。。。。。。。\n”);




 /*2. 等待client发送消息，接收后，发送终止消息“123”给client*/（假定client收到“123”字符串后，终止程序，退出）


while(ReadFile(hPipe,buf,100,&dwRead,NULL))

{
printf(buf);
char buf[]="123";
DWORD dwWrite;
if(!WriteFile(hPipe,buf,strlen(buf)+1,&dwWrite,NULL))
{
printf("写入数据失败！");
return;
}

}


return 0;

}




*************************************************************

(2) Client 端

主要流程是启动后，主动连接服务器的管道，并发送字符串“I'm ok”，阻塞的等待接收服务器发送的字符串，当判定为“123”后退出。




#include <stdio.h>


#include <windows.h>//pipe所依赖的头文件，或者Winbase.h


void main(int argc,char* argv[])


{

HANDLE hPipe = NULL;//首先，声明管道句柄


char buf[100];
DWORD dwRead;
if(!WaitNamedPipe("\\\\.\\pipe\\TestPipe",NMPWAIT_WAIT_FOREVER))//等待一个可用的命名管道实例
{
printf("当前没有可利用的命名管道实例！");
return 1;
}
hPipe=CreateFile("\\\\.\\pipe\\TestPipe",GENERIC_READ | GENERIC_WRITE,
0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);//打开服务器创建的命名管道TestPipe
if(INVALID_HANDLE_VALUE==hPipe)
{
printf("打开命名管道失败！");
hPipe=NULL;
return 1;
}





char buf1[]="I'm ok";
DWORD dwWrite;
if(!WriteFile(hPipe,buf1,strlen(buf1)+1,&dwWrite,NULL))
{
printf("写入数据失败！");
return;
}




while(1)

{

while(ReadFile(hPipe,buf,100,&dwRead,NULL))
{
printf(buf);
if(strcmp(buf,"123")==0)
{
DisconnectNamedPipe(hPipe);
exit(-1);
}


}


}

return ;

}




多线程版本的命名管道通信、重叠IO的命名管道通信待续。。。












