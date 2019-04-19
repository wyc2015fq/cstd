# Win32 API串口编程 - xqhrs232的专栏 - CSDN博客
2013年01月18日 21:23:52[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：817
原文地址::[http://blog.csdn.net/zhuyonghao123/article/details/8162250](http://blog.csdn.net/zhuyonghao123/article/details/8162250)
相关网帖
1.Win32串口编程----[http://www.vckbase.com/index.php/wv/1439](http://www.vckbase.com/index.php/wv/1439)
通常windows下串口编程大家会选择MFC使用SCOM控件进行串口操作，
方便而且简单，我也是这样的，PC端要进行串口编程就选择MFC做一个
简单的界面，添加SCOM控件。但有时我们需要在Win32下进行串口编程，
根据我在网上查的资料以及编程调试经验做一个简单的例子，需要深入的
可以此为基础。
串口编程的过程基本是这样的：
       串口打开、串口读操作、串口写操作、串口关闭
包含头文件：
#include <windows.h>
#include <stdio.h>
一、串口的同步方式(等待操作完成后才能返回)：
1、  打开串口—同步方式
HANDLE Open_driver_T(char *name)
{
       //打开串口
       HANDLE m_hCom = CreateFile(name,GENERIC_READ | GENERIC_WRITE, 0, NULL,
            OPEN_EXISTING, 0, NULL);
       if(m_hCom == INVALID_HANDLE_VALUE)
       {
               printf("Create File faile\n");
               return NULL;
       }
       //设置缓冲区大小
       if(!SetupComm(m_hCom,1024,1024))
      {
             printf("SetupComm fail!\n");
             CloseHandle(m_hCom);
             return NULL;
       }
       //设置超时
       COMMTIMEOUTS TimeOuts;
       memset(&TimeOuts,0,sizeof(TimeOuts));
       TimeOuts.ReadIntervalTimeout = 100;
       TimeOuts.ReadTotalTimeoutConstant = 1000;
       TimeOuts.ReadTotalTimeoutMultiplier = 100;
       TimeOuts.WriteTotalTimeoutConstant = 2000;
       TimeOuts.WriteTotalTimeoutMultiplier = 50;
       SetCommTimeouts(m_hCom,&TimeOuts);
       PurgeComm(m_hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);
       //设置串口参数
       DCB dcb = {0};
       if (!GetCommState(m_hCom,&dcb))
       {
               printf("GetCommState fail\n");
               return NULL;
       }
       dcb.DCBlength = sizeof(dcb);
       if (!BuildCommDCB("1228800,n,8,1",&dcb))//填充ＤＣＢ的数据传输率、奇偶校验类型、数据位、停止位
       {
               printf("BuileCOmmDCB fail\n");
               CloseHandle(m_hCom);
               return NULL;
       }
       if(SetCommState(m_hCom,&dcb))
       {
              printf("SetCommState OK!\n");
       }
       return m_hCom;
}
2、串口发送(写操作)--同步方式
unsigned char Send_driver_T(HANDLE fd, BYTE *data)
{
       DWORD dwError;
       DWORD dwExpectSend = 20;
       DWORD dwRealSend = 0;
       BYTE *pSendBuffer;
       pSendBuffer = data;
       if(ClearCommError(fd,&dwError,NULL))
       {
              PurgeComm(fd,PURGE_TXABORT | PURGE_TXCLEAR);
       }
       if(!WriteFile(fd,pSendBuffer,dwExpectSend ,&dwRealSend,NULL))
       {    
              //写串口失败
              printf("发送失败!\n");
              return 1;
       }
       return 0;
}
3、串口读取(读操作)--同步方式
unsigned char Receive_driver_T(HANDLE fd, BYTE *data)
{
       DWORD dwError;
       DWORD dwWantRead = 20;
       DWORD dwRealRead = 0;
       BYTE* pReadBuf;
       int i;
       BYTE crc;
       pReadBuf = data;
       if (ClearCommError(fd,&dwError,NULL))
       {
              PurgeComm(fd,PURGE_TXABORT | PURGE_TXCLEAR);
       }
       if(!ReadFile(fd,pReadBuf,dwWantRead ,&dwRealRead,NULL))    //成功返回非0 //失败返回0
       {
              return 1;
       }
       if(dwRealRead>0)
              printf("recv_len = %d\n",dwRealRead);
       printf("接收数据: ");
       for(i=0; i<20; i++)
       {
              printf("%x ",data[i]);
       }
       printf("\n");
       return 0;
}
4、串口关闭
int Close_driver(void *fd)
{
       CloseHandle(fd);
       return 0;
}
5、编程应用程序
void main()
{
       HANDLE hCom;
       BYTE SendData[20] = {0};
       BYTE RecvData[100] = {0};
       unsigned int SendCnt=0;
       unsigned int RecvCnt=0;
       int i;
       hCom = Open_driver_T("COM3");
       if(!hCom)
       {
              printf("串口端口打开失败\n");
              return ;
       }
       else
              printf("串口端口打开成功!\n");
       SendCnt=0;
       for(i=0; i<5; i++)
       {
              SendCnt++;
              if(Send_driver_T(hCom, SendData))
              {
                     printf("数据(%4d) 发送失败!\n",SendCnt);
              }
              else
              {
                     printf("数据(%4d) 发送成功!\n",SendCnt);
              }
              Sleep(1000);
       }
       printf("\n");
       RecvCnt=0;
       for(i=0; i<5; i++)
       {
              if(Receive_driver_T(hCom, RecvData))
              {
                     printf("数据(%4d) 接收失败!\n",RecvCnt);
              }
              else
              {
                     RecvCnt++;
                     printf("数据(%4d) 接收成功!\n",RecvCnt);
              }
              Sleep(1000);
       }
}
二、串口的异步方式(操作结束立即返回)
1、打开串口—异步方式
HANDLE Open_driver_Y(char *name)
{
       //打开串口
       HANDLE m_hCom = CreateFile(name,GENERIC_READ | GENERIC_WRITE, 0, NULL,
              OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
       if(m_hCom == INVALID_HANDLE_VALUE)
       {
              printf("Create File faile\n");
              return NULL;
       }
       //设置缓冲区大小
       if(!SetupComm(m_hCom,1024,1024))
       {
              printf("SetupComm fail!\n");
              CloseHandle(m_hCom);
              return NULL;
       }
       //设置超时
       COMMTIMEOUTS TimeOuts;
       memset(&TimeOuts,0,sizeof(TimeOuts));
       TimeOuts.ReadIntervalTimeout = 100;
       TimeOuts.ReadTotalTimeoutConstant = 1000;
       TimeOuts.ReadTotalTimeoutMultiplier = 100;
       TimeOuts.WriteTotalTimeoutConstant = 2000;
       TimeOuts.WriteTotalTimeoutMultiplier = 50;
       SetCommTimeouts(m_hCom,&TimeOuts);
       PurgeComm(m_hCom,PURGE_TXCLEAR|PURGE_RXCLEAR);
       //设置串口参数
       DCB dcb = {0};
       if (!GetCommState(m_hCom,&dcb))
       {
              printf("GetCommState fail\n");
              return NULL;
       }
       dcb.DCBlength = sizeof(dcb);
       if (!BuildCommDCB("1228800,n,8,1",&dcb))//填充ＤＣＢ的数据传输率、奇偶校验类型、数据位、停止位
       {
              printf("BuileCOmmDCB fail\n");
              CloseHandle(m_hCom);
              return NULL;
       }
       if(SetCommState(m_hCom,&dcb))
       {
              printf("SetCommState OK!\n");
       }
       //建立并初始化重叠结构
       ZeroMemory(&wrOverlapped,sizeof(wrOverlapped));
       if (wrOverlapped.hEvent != NULL)
       {
              ResetEvent(wrOverlapped.hEvent);
              wrOverlapped.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
       }
       return m_hCom;
}
2、  串口发送(写操作)—异步方式
unsigned char Send_driver_Y(HANDLE fd, BYTE *data)
{
       DWORD dwError;
       DWORD dwExpectSend = 20;
       DWORD dwRealSend = 0;
       BYTE *pSendBuffer;
       pSendBuffer = data;
       if(ClearCommError(fd,&dwError,NULL))
       {
              PurgeComm(fd,PURGE_TXABORT | PURGE_TXCLEAR);
       }
       if(!WriteFile(fd,pSendBuffer,dwExpectSend ,&dwRealSend,&wrOverlapped))
       {    
              if(GetLastError() == ERROR_IO_PENDING)
              {
                     while(!GetOverlappedResult(fd,&wrOverlapped,&dwRealSend,FALSE))  
                     //成功返回非0，失败返回0
                     {
                            if(GetLastError() == ERROR_IO_INCOMPLETE)
                            {
                                   continue;
                            }
                            else
                            {
                                   printf("发送失败!\n");
                                   ClearCommError(fd,&dwError,NULL);
                                   break;
                                   return 1;
                            }
                     }
              }
       }
       return 0;
}
3、  串口接收(读操作)--异步方式
unsigned char Receive_driver_Y(HANDLE fd, BYTE *data)
{
       DWORD dwError;
       DWORD dwWantRead = 20;
       DWORD dwRealRead = 0;
       BYTE* pReadBuf;
       int i;
       BYTE crc;
       pReadBuf = data;
       if (ClearCommError(fd,&dwError,NULL))
       {
              PurgeComm(fd,PURGE_TXABORT | PURGE_TXCLEAR);
       }
       if(!ReadFile(fd,pReadBuf,dwWantRead,&dwRealRead,&wrOverlapped))
       {
              dwError=GetLastError();
              if(dwError == ERROR_IO_PENDING)
              {
                     while(!GetOverlappedResult(fd,&wrOverlapped,&dwRealRead,TRUE))   
                     //成功返回非0, 失败返回0
                     {
                            return 1;
                     }
              }
       }
       if(dwRealRead>0)
              printf("recv_len = %d\n",dwRealRead);      
       printf("接收数据\n");
       for(i=0; i<20; i++)
       {
              printf("%x ",data[i]);
       }
       return 0;
}
4、  关闭串口
int Close_driver(void *fd)
{
       CloseHandle(fd);
       return 0;
}
5、  编程应用程序--异步方式
OVERLAPPED wrOverlapped;
void main()
{
       HANDLE hCom;
       BYTE SendData[20] = {0};
       BYTE RecvData[100] = {0};
       unsigned int SendCnt=0;
       unsigned int RecvCnt=0;
       int i;
       hCom = Open_driver_Y("COM3");
       if(!hCom)
       {
              printf("串口端口打开失败\n");
              return ;
       }
       else
              printf("串口端口打开成功!\n");
       SendCnt=0;
       for(i=0; i<5; i++)
       {
              SendCnt++;
              if(Send_driver_Y(hCom, SendData))
              {
                     printf("数据(%4d) 发送失败!\n",SendCnt);
              }
              else
              {
                     printf("数据(%4d) 发送成功!\n",SendCnt);
              }
              Sleep(1000);
       }
       printf("\n");
       RecvCnt=0;
       for(i=0; i<5; i++)
       {
              if(Receive_driver_Y(hCom, RecvData))
              {
                     printf("数据(%4d) 接收失败!\n",RecvCnt);
              }
              else
              {
                     RecvCnt++;
                     printf("数据(%4d) 接收成功!\n",RecvCnt);
              }
              Sleep(1000);
       }
}
相关函数原型以及参数定义请参考MSDN
//===================================================
备注::
1.串口的操作可以有两种操作方式：同步操作方式和重叠操作方式（又称为异步操作方式）。同步操作时，API函数会阻塞直到操作完成以后才能返回（在多线程方式中，虽然不会阻塞主线程，但是仍然会阻塞监听线程）；而重叠操作方式，API函数会立即返回，操作在后台进行，避免线程的阻塞。
2.怎么都不见WaitCommEvent函数的使用？？？
3.怎么在一些书籍上看到在什么NT内核的WINDOWS系统上不支持串口的同步操作方式
4.我的ClearCommError函数老是获不得正确的数据个数，不知道为什么，不知道那里没设置好
5.串口通信出错网帖
   1》串口收数据丢失----[http://bbs.csdn.net/topics/120014380](http://bbs.csdn.net/topics/120014380)
   2》串口通信接收数据丢失----[http://bbs.csdn.net/topics/70081340](http://bbs.csdn.net/topics/70081340)
  3》MSDN Serial
 Communications----[http://msdn.microsoft.com/en-us/library/ff802693.aspx](http://msdn.microsoft.com/en-us/library/ff802693.aspx)
  4》ClearCommError函数问题----[http://bbs.csdn.net/topics/390147651?page=1#post-394097054](http://bbs.csdn.net/topics/390147651?page=1#post-394097054)

