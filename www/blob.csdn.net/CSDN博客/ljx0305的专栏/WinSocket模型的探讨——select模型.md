# WinSocket模型的探讨——select模型 - ljx0305的专栏 - CSDN博客
2009年04月06日 10:24:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：1063标签：[socket																[null																[cmd																[struct																[服务器																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=cmd&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[VC																[网络编程](https://blog.csdn.net/ljx0305/article/category/403915)](https://blog.csdn.net/ljx0305/article/category/401790)
查找了很多资料都找不到select模型的详细用法，《Windows网络编程》这本书上也只是写了一个简单的回应服务器，就连writefds的用法都没讲，也不知道什么时候利用“可写”来发文件。这些都是我的疑问，相信很多研究网络编程的同路人也碰到了我的这些问题。这些疑问在这篇文章中都解决了！耗费了偶很多的精力去猜测去思考！ 
感觉一些已经得道的高人都不肯把这些问题说透彻点，唉，只能靠自己去摸索了，希望这篇文章能对你有用，也希望一些高人能出来指点指点！
SOCKET模型的出现是为了解决“一个客户端一线程”的问题，为了WINDOWS的线程切换不要太频繁，我们可以使用WINDOWS的SOCKET模型。但在论坛里又看到了一些文章说现在的计算机硬件相当发达，成万的线程切换根本不是什么问题。无论怎么样，既然这些模型被MS发明了出来，就有它的道理，我们还是好好来学习一下吧。
对于select模型，大家都知道用select函数，然后判断读集、写集。但如何使用select，最终写好的Server又是一个怎么样的结构呢？
select可以这样用，写成两个函数，SelectSend和SelectRecv，这两个函数和一般的send/recv不同的地方在于它是有超时值的，这样就不会把程序完全阻塞了。这两个函数大概如下（参考《PC网络游戏编程》）：
SelectRecv(...)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int SelectRecv(SOCKET hSocket, char*pszBuffer, int nBufferSize, 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)        DWORD dwTimeout)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    ASSERT(hSocket != NULL);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if(hSocket==NULL)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return ( SOCKET_ERROR );
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)    FD_SET fd =...{1, hSocket};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)    TIMEVAL tv =...{dwTimeout, 0};
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int nBytesReceived=0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if(select(0, &fd, NULL, NULL, &tv) ==0) 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)goto CLEAR;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if((nBytesReceived = recv(hSocket, pszBuffer, nBufferSize, 0)) == SOCKET_ERROR)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)goto CLEAR;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return nBytesReceived;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)CLEAR:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)    SetLastError(WSAGetLastError());//超时
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return(SOCKET_ERROR);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
SelectSend(...)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int SelectSend(SOCKET hSocket,charconst* pszBuffer, 
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int nBufferSize, DWORD dwTimeout)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if(hSocket==NULL)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return(SOCKET_ERROR);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int nBytesSent =0;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)int nBytesThisTime;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)constchar* pszTemp = pszBuffer;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedSubBlock.gif)do...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        nBytesThisTime = Send_Block(hSocket,pszTemp, nBufferSize-nBytesSent, dwTimeout);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)if(nBytesThisTime<0)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return(SOCKET_ERROR);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//如果一次没有发送成功
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        nBytesSent += nBytesThisTime;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)//改变当前字符指针
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)        pszTemp += nBytesThisTime;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedSubBlockEnd.gif)    }while(nBytesSent < nBufferSize);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return nBytesSent;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}
这样就可以利用上面两个函数写发送和接收程序了！我们下面来看看如何使用select模型建立我们的Server，下面分了4个文件，大家可以拷贝下来实际编译一下。首先有两个公用的文件：CommonSocket.h、CommonCmd.h。他们都是放在 Common Include 目录中的！然后就是SelectServer.cpp和SocketClient.cpp这两个文件，可以分别建立两个工程把他们加进去编译！有些关键的地方我都写在文件注释里了，可以自己看看。为了方便大家拷贝，我就不用“代码插入”的方式了。直接贴到下面！
/**********************************************************************************************************************/
第一个文件
/*/
文件：SelectServer.cpp
说明：
 此文件演示了如何使用select模型来建立服务器，难点是select的writefds在什么时候使用。
 好好看看代码就能很明白的了，可以说我写这些代码就是为了探索这个问题的！找了很多资料都找不到！！
 在这里我怀疑是否可以同时读写同一个SOCKET，结果发现是可以的，但是最好别这样做。因为会导致包的顺序不一致。
    这里说一下SELECT模型的逻辑：
 我们如果不使用select模型，在调用recv或者send时候会导致程序阻塞。如果使用了select
 就给我们增加了一层保护，就是说在调用了select函数之后，对处于读集合的socket进行recv操作
 是一定会成功的（这是操作系统给我们保证的）。对于判断SOCKET是否可写时也一样。
 而且就算不可读或不可写，使用了select也不会锁 死！因为 select 函数提供了超时！利用这个特性还可以
 做异步connect，也就是可以扫描主机，看哪个主机开了服务（远程控制软件经常这样干哦！）
 我们如何利用这种逻辑来设计我们的server呢？
 这里用的方法是建立一个SocketInfo，这个SocketInfo包括了对Socket当前进行的操作，我把它分为：
 {RecvCmd, RecvData, ExecCmd} 一开始socket是处于一个RecvCmd的状态，
 然后取到了CMD（也就是取到了指令，可想象一下CPU得到了指令后干什么），然后就要取数据了，取得指令
 知道要干什么，取得了数据就可以实际开始干了。实际开始干就是ExecCmd，在这个状态之后都是需要
 发送数据的了，所以把他们都放在判断SOCKET可写下面<就是 if(FD_ISSET(vecSocketInfo[i].sock, &fdWrite)) >，
 即当Socket可写就可以发送信息给客户端了。
 发送的根本协议是这样的：先发一个SCommand的结构体过去，这个结构体说明了指令和数据的长度。
 然后就根据这个长度接收数据。最后再给客户端做出相应的响应！
    根据这种代码结构，可以很方便的添加新的功能。
   错误处理做得不太好，以后再补充了。
 其他的如注释，结构，命名等的编码规范都用了个人比较喜欢的方式。
输出：
 ../Bin/SelectServer.exe
用法：
 直接启动就可以了
Todo:
 下一步首先完成各个SOCKET的模型，然后公开自己的研究代码。
 功能方面就是：
 1、服务器可以指定共享文件夹
 2、客户端可以列出服务器共享了哪些文件
 3、客户端可以列出哪些用户在线，并可以发命令和其他用户聊天
 4、加上界面
/*/
#include <winsock2.h>
#pragma comment(lib, "WS2_32")
#include <windows.h>
#pragma warning(disable: 4786)
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;
#include "../Common Include/CommonSocket.h"
#include "../Common Include/CommonCmd.h"
typedef struct tagSocketInfo
{
 SOCKET sock;
 ECurOp eCurOp;
 SCommand cmd;
 char *data;
}SSocketInfo;
// 登录用户的列表
map<string, SOCKET> g_LoginUsers;
// 注册用户的列表(用户名，密码)
map<string, string> g_RegUSers;
// 用于退出服务器
bool g_bExit = false;
void DoRecvCmd(vector<SSocketInfo> &vecSockInfo, int idx);
void DoRecvData(vector<SSocketInfo> &vecSockInfo, int idx);
void DoExecCmd(vector<SSocketInfo> &vecSockInfo, int idx);
bool DoAuthen(SOCKET sock, char *data, DWORD len);
bool DoGetFile(SOCKET sock, char *data, DWORD len);
bool DoRegister(SOCKET sock, char *data, DWORD len);
void GetRegUsers();
///////////////////////////////////////////////////////////////////////
//
// 函数名       : RemoveByIndex
// 功能描述     : 根据 index 来删除 VECTOR 里的元素
// 参数         : vector<T> &vec [in]
// 参数         : int nIdx   [in]
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
template<class T>
void EraseByIndex(vector<T> &vec, int nIdx)
{
 vector<T>::iterator it;
 it = vec.begin() + nIdx;
 vec.erase(it);
}
void main()
{
 InitWinsock();
 vector<SSocketInfo> vecSocketInfo;
 SOCKET sockListen = BindServer(PORT);
 ULONG NonBlock = 1;
 ioctlsocket(sockListen, FIONBIO, &NonBlock);
 SOCKET sockClient;
 GetRegUsers();
 FD_SET fdRead;
 FD_SET fdWrite;
 while(!g_bExit)
 {
  // 每次调用select之前都要把读集和写集清空
  FD_ZERO(&fdRead);
  FD_ZERO(&fdWrite);
  // 设置好读集和写集
  FD_SET(sockListen, &fdRead);
  for(int i = 0; i < vecSocketInfo.size(); i++)
  {
   FD_SET(vecSocketInfo[i].sock, &fdRead);
   FD_SET(vecSocketInfo[i].sock, &fdWrite);
  }
  // 调用select函数
  if(select(0, &fdRead, &fdWrite, NULL, NULL) == SOCKET_ERROR)
  {
   OutErr("select() Failed!");
   break;
  }
  // 说明可以接受连接了
  if(FD_ISSET(sockListen, &fdRead))
  {
   char szClientIP[50];
   sockClient = AcceptClient(sockListen, szClientIP);
   cout << szClientIP << " 连接上来" << endl;
   ioctlsocket(sockClient, FIONBIO, &NonBlock);
   SSocketInfo sockInfo;
   sockInfo.sock = sockClient;
   sockInfo.eCurOp = RecvCmd;
   // 把接收到的这个socket加入自己的队列中
   vecSocketInfo.push_back(sockInfo);
  }
  for(i = 0; i < vecSocketInfo.size(); i++)
  {
   // 如果可读
   if(FD_ISSET(vecSocketInfo[i].sock, &fdRead))
   {
    switch(vecSocketInfo[i].eCurOp)
    {
    case RecvCmd:
     DoRecvCmd(vecSocketInfo, i);
     break;
    case RecvData:
     DoRecvData(vecSocketInfo, i);
     break;
    default:
     break;
    }
   }
   // 如果可写
   if(FD_ISSET(vecSocketInfo[i].sock, &fdWrite))
   {
    switch(vecSocketInfo[i].eCurOp)
    {
    case ExecCmd:
     DoExecCmd(vecSocketInfo, i);
     break;
    default:
     break;
    }
   }
  }
 }
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : DoRecvCmd
// 功能描述     : 获取客户端传过来的cmd
// 参数         : vector<SSocketInfo> &vecSockInfo
// 参数         : int idx
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void DoRecvCmd(vector<SSocketInfo> &vecSockInfo, int idx)
{
 SSocketInfo *sockInfo = &vecSockInfo[idx];
 int nRet = RecvFix(sockInfo->sock, (char *)&(sockInfo->cmd), sizeof(sockInfo->cmd));
 // 如果用户正常登录上来再用 closesocket 关闭 socket 会返回0 
 // 如果用户直接关闭程序会返回 SOCKET_ERROR，强行关闭
 if(nRet == SOCKET_ERROR || nRet == 0)
 {
  OutMsg("客户端已退出。");
  closesocket(sockInfo->sock);
  sockInfo->sock = INVALID_SOCKET;     
  EraseByIndex(vecSockInfo, idx);
  return;
 }
 sockInfo->eCurOp = RecvData;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : DoRecvData
// 功能描述     : DoRecvCmd 已经获得了指令，接下来就要获得执行指令所需要的数据
// 参数         : vector<SSocketInfo> &vecSockInfo
// 参数         : int idx
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void DoRecvData(vector<SSocketInfo> &vecSockInfo, int idx)
{
 SSocketInfo *sockInfo = &vecSockInfo[idx];
 // 为数据分配空间，分配多一位用来放最后的0
 sockInfo->data = new char[sockInfo->cmd.DataSize + 1];
 memset(sockInfo->data, 0, sockInfo->cmd.DataSize + 1);
 // 接收数据
 int nRet = RecvFix(sockInfo->sock, sockInfo->data, sockInfo->cmd.DataSize);
 if(nRet == SOCKET_ERROR || nRet == 0)
 {
  OutMsg("客户端已退出。");
  closesocket(sockInfo->sock);
  sockInfo->sock = INVALID_SOCKET;     
  EraseByIndex(vecSockInfo, idx);
  return;
 }
 sockInfo->eCurOp = ExecCmd;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : DoExecCmd
// 功能描述     : 指令和执行指令所需数据都已经准备好了，接下来就可以执行命令
// 参数         : vector<SSocketInfo> &vecSockInfo
// 参数         : int idx
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void DoExecCmd(vector<SSocketInfo> &vecSockInfo, int idx)
{
 SSocketInfo *sockInfo = &vecSockInfo[idx];
 switch(sockInfo->cmd.CommandID) 
 {
 case CMD_AUTHEN:
  DoAuthen(sockInfo->sock, sockInfo->data, sockInfo->cmd.DataSize);
   break;
 case CMD_GETFILE:
  DoGetFile(sockInfo->sock, sockInfo->data, sockInfo->cmd.DataSize);
  break;
 case CMD_REGISTER:
  DoRegister(sockInfo->sock, sockInfo->data, sockInfo->cmd.DataSize);
  break;
 default:
  break;
 }
 // 执行完命令后就设置回接收指令状态
 sockInfo->eCurOp = RecvCmd;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : DoAuthen
// 功能描述     : 对用户名和密码做验证
// 参数         : SOCKET sock
// 参数         : char *data
// 参数         : DWORD len
// 返回值       : bool 
//
///////////////////////////////////////////////////////////////////////
bool DoAuthen(SOCKET sock, char *data, DWORD len)
{
 // 取得用户名和密码的字符串
 // 格式为 "dyl 123"
 char *pBuf = data;
 int nIdx = 0;
 char szName[10];
 memset(szName, 0, 10);
 char szPass[10];
 memset(szPass, 0, 10);
 while (*pBuf != ' ')
 {
  szName[nIdx++] = *pBuf++;
 }
 szName[nIdx] = '/0';
 *pBuf++;
 nIdx = 0;
 while (*pBuf != '/0')
 {
  szPass[nIdx++] = *pBuf++;
 }
 szPass[nIdx] = '/0';
 char szSend[30];
 memset(szSend, 0, 30);
 bool bUserExist = false;
 if( g_RegUSers.find(string(szName)) != g_RegUSers.end() )
 {
  if(strcmp(g_RegUSers[szName].c_str(), szPass) == 0)
  {
   strcpy(szSend, "UP OK!");
   g_LoginUsers[szName] = sock;
  }
  else
  {
   strcpy(szSend, "P Err!");
  }  
 }
 else
 {
 // 不存在这个用户
  strcpy(szSend, "U Err!");
 }
 int nRet = SendFix(sock, szSend, strlen(szSend));
 if(nRet == SOCKET_ERROR)
  return false;
 // 执行完了就释放data
 delete []data;
 return true;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : DoGetFile
// 功能描述     : 为用户提供文件
// 参数         : SOCKET sock
// 参数         : char *data
// 参数         : DWORD len
// 返回值       : bool 
//
///////////////////////////////////////////////////////////////////////
bool DoGetFile(SOCKET sock, char *data, DWORD len)
{
 // 打开文件，判断文件是否存在
 HANDLE hFile = CreateFile(data, GENERIC_READ, FILE_SHARE_READ, 
  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 if(hFile == INVALID_HANDLE_VALUE)
 {
  OutMsg("文件不存在！");
  DWORD dwSize = 0;
  SendFix(sock, (char *)&dwSize, sizeof(dwSize));
  return false;
 }
 else
 {// 发送文件信息
  // 发送文件大小，发送过去
  DWORD dwFileSize = GetFileSize(hFile, NULL);
  int nRet = SendFix(sock, (char *)&dwFileSize, sizeof(dwFileSize));
  if(nRet == SOCKET_ERROR)
   return false;
  // 读文件记录并发送
  DWORD nLeft = dwFileSize;
  char szBuf[1024];
  DWORD nCurrRead = 0;
  while(nLeft > 0)
  {
   if(!ReadFile(hFile, szBuf, 1024, &nCurrRead, NULL))
   {
    OutErr("ReadFile failed!");
    return false;
   }
   SendFix(sock, szBuf, nCurrRead);
   nLeft -= nCurrRead;
  }
  CloseHandle(hFile);
 }
 delete []data;
 return true;
}
bool DoRegister(SOCKET sock, char *data, DWORD len)
{
 // 取得用户名和密码的字符串
 // 格式为 "dyl 123"
 bool bReturn = true;
 char *pBuf = data;
 int nIdx = 0;
 char szName[10];
 memset(szName, 0, 10);
 char szPass[20];
 memset(szPass, 0, 20);
 while (*pBuf != ' ')
 {
  szName[nIdx++] = *pBuf++;
 }
 szName[nIdx] = '/0';
 *pBuf++;
 nIdx = 0;
 while (*pBuf != '/0')
 {
  szPass[nIdx++] = *pBuf++;
 }
 szPass[nIdx] = '/0';
 char szSend[30];
 memset(szSend, 0, 30); 
 HANDLE hFile = CreateFile("Users.lst", GENERIC_WRITE, FILE_SHARE_READ, NULL, 
  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 if(hFile == INVALID_HANDLE_VALUE)
 {
  hFile = CreateFile("Users.lst", GENERIC_WRITE, FILE_SHARE_READ, NULL, 
   CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
  if(hFile == INVALID_HANDLE_VALUE) 
  {
   OutMsg("创建文件Users.lst失败！");
   strcpy(szSend, "REG ERR!");
   bReturn = false;
  }
  else
  {
   // 在开始加
   SetFilePointer(hFile, 0, 0, FILE_BEGIN);
   DWORD dwWritten = 0;
   if(!WriteFile(hFile, szName, 10, &dwWritten, NULL))
   {
    OutMsg("WriteFile failed!");
    strcpy(szSend, "REG ERR!");
    bReturn = false;
   }
   if(!WriteFile(hFile, szPass, 20, &dwWritten, NULL))
   {
    OutMsg("WriteFile failed!");
    strcpy(szSend, "REG ERR!");
    bReturn = false;
   }
   CloseHandle(hFile);
   // 读回到已注册用户列表中
   GetRegUsers();
   strcpy(szSend, "REG OK!");
  }
 }
 else
 {
  // 移动到最后追加
  SetEndOfFile(hFile);
  DWORD dwWritten = 0;
  if(!WriteFile(hFile, szName, 10, &dwWritten, NULL))
  {
   OutMsg("WriteFile failed!");
   strcpy(szSend, "REG ERR!");
   bReturn = false;
  }
  if(!WriteFile(hFile, szPass, 20, &dwWritten, NULL))
  {
   OutMsg("WriteFile failed!");
   strcpy(szSend, "REG ERR!");
   bReturn = false;
  }
  CloseHandle(hFile);
  // 读回到已注册用户列表中
  GetRegUsers();
  strcpy(szSend, "REG OK!");  
 }
 int nRet = SendFix(sock, szSend, strlen(szSend));
 if(nRet == SOCKET_ERROR)
  bReturn = false;
 // 执行完了就释放data
 delete []data; 
 return bReturn;
}
void GetRegUsers()
{
 g_RegUSers.clear();
 char szName[10];
 char szPwd[20];
 HANDLE hFile = CreateFile("Users.lst", GENERIC_READ, FILE_SHARE_READ, NULL, 
  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 if(hFile == INVALID_HANDLE_VALUE)
 {
  OutMsg("用户列表不存在！");
 }
 else
 {
  DWORD dwFileSize = 0;
  dwFileSize = GetFileSize(hFile, NULL);
  SetFilePointer(hFile, 0, 0, FILE_BEGIN);
  DWORD dwRead = 0;
  DWORD dwLeft = dwFileSize;
  while(dwLeft > 0)
  {
   memset(szName, 0, 10);
   memset(szPwd, 0, 20);
   if(!ReadFile(hFile, szName, 10, &dwRead, NULL))
   {
    DWORD dwErr = GetLastError();
    OutMsg("ReadFile failed!");
   }
   dwLeft -= dwRead;
   if(!ReadFile(hFile, szPwd, 20, &dwRead, NULL))
   {
    DWORD dwErr = GetLastError();
    OutMsg("ReadFile failed!");
   }
   dwLeft -= dwRead;
   g_RegUSers[szName] = szPwd;
  } 
 }
 CloseHandle(hFile);
}
/**********************************************************************************************************************/
第二个文件
/*/
文件：SocketClient.cpp
说明：
 此文件是作为测试的客户端，实现了登录和取文件的功能。
 和服务端的交互就是采用了发送命令、数据长度，然后发送具体的数据这样的顺序。
 详细可看服务端的说明。
 基本逻辑是这样的，客户端要先登录服务端，然后登录成功之后，才能进行相应的操作。
 错误处理做得不太好，以后再补充了。
 其他的如注释，结构，命名等的编码规范都用了个人比较喜欢的方式。
输出：
 ../Bin/SocketClient.exe
用法：
 可以 SocketClient Server_IP
 或者直接启动SocketClient，会提示你输入服务端的IP
Todo:
 下一步首先完成各个SOCKET的模型，然后公开自己的研究代码。
 功能方面就是：
 1、服务器可以指定共享文件夹
 2、客户端可以列出服务器共享了哪些文件
 3、客户端可以列出哪些用户在线，并可以发命令和其他用户聊天
/*/
#include <winsock2.h>
#pragma comment(lib, "WS2_32")
#include <iostream>
using namespace std;
#include <stdlib.h>
#include "../Common Include/CommonSocket.h"
#include "../Common Include/CommonCmd.h"
bool g_bAuth = false;
void GetFile(SOCKET sock);
bool Auth(SOCKET sock, char *szName, char *szPwd);
bool RegisterUser(SOCKET sock, char *szName, char *szPwd);
///////////////////////////////////////////////////////////////////////
//
// 函数名       : Usage
// 功能描述     : 提示程序用法
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void Usage() 
{ 
 printf("*******************************************/n"); 
 printf("Socket Client                            /n"); 
 printf("Written by DYL                         /n"); 
 printf("Email: [dylgsy@163.com](mailto:dylgsy@163.com)                 /n"); 
 printf("Usage: SocketClient.exe Server_IP          /n"); 
 printf("*******************************************/n"); 
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : Menu
// 功能描述     : 选择服务的界面
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void Menu()
{
 system("cls");
 printf("********************************************/n"); 
 printf("请选择操作：        /n/n"); 
 printf("1、取得文件         /n"); 
 printf("2、退出          /n"); 
 printf("********************************************/n"); 
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : LoginMenu
// 功能描述     : 用户登录的界面
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void LoginMenu()
{
 cout << "请按任意键继续操作．" <<endl;
 getchar();
 system("cls");
 printf("********************************************/n"); 
 printf("请选择操作：        /n/n"); 
 printf("1、登录          /n"); 
 printf("2、注册          /n"); 
 printf("3、退出          /n"); 
 printf("********************************************/n"); 
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : Login
// 功能描述     : 用户登录的界面逻辑
// 参数         : SOCKET sock
// 返回值       : bool 
//
///////////////////////////////////////////////////////////////////////
bool Login(SOCKET sock)
{
 bool bGoOn = true;
 while(bGoOn)
 {
  LoginMenu();
  int nChoose = 0;
  cin >> nChoose;
  char szName[10];
  char szPwd[20];
  char szConfirmPwd[20];
  memset(szName, 0, 10);
  memset(szPwd, 0, 20);
  memset(szConfirmPwd, 0, 20);
  bool bGoOnLogin = true;
  switch(nChoose) 
  {
  case 1:
   while(bGoOnLogin)
   {
    cout << "请输入你的用户名：";
    cin >> szName;
    cout << "请输入你的密码：";
    cin >> szPwd;
    if(Auth(sock, szName, szPwd))
    {
     return true; 
    }
    else
    {
     char c;
     cout << "继续登录？y/n" << endl;
     cin >> c;
     switch(c) 
     {
     case 'y':
      bGoOnLogin = true;
      break;
     case 'n':
      bGoOnLogin = false;
      break;
     default:
      break;
     }
    }
   }
   break;
  case 2:
   cout << "请输入你的用户名：";
   cin >> szName;
   cout << "请输入你的密码：";
   cin >> szPwd;
   cout << "请再次输入你的密码：";
   cin >> szConfirmPwd;
   if(strcmp(szPwd, szConfirmPwd) != 0)
   {
    cout << "前后密码不一致" << endl;
   }
   else
   {
    if(!RegisterUser(sock, szName, szPwd))
    {
     cout << "注册用户失败!" << endl;
    }
   }
   break;
  case 3:
   bGoOn = false;
   return false;
  default:
   break;
  }
 }
 return false;
}
void main(int argc, char *argv[])
{
 system("cls");
 char szServerIP[20];
 memset(szServerIP, 0, 20);
 if(argc != 2)
 {
  cout << "请输入服务器IP：";
  cin >> szServerIP;
 }
 else
 {
  strcpy(szServerIP, argv[1]);
 }
 InitWinsock();
 SOCKET sockServer;
 sockServer = ConnectServer(szServerIP, PORT, 1);
 if(sockServer == NULL)
 {
  OutErr("连接服务器失败！");
  return;
 }
 else
 {
  OutMsg("已和服务器建立连接！");
 }
 // 要求用户登录
 if(!Login(sockServer))
  return;
 // 登录成功，让用户选择服务
 int nChoose = 0;
 bool bExit = false;
 while(!bExit)
 {
  Menu();
  cin >> nChoose;
  switch(nChoose)
  {
  case 1:  // 获取文件
   GetFile(sockServer);
   break;
  case 2:
   bExit = true;
   break;   
  default:
   break;
  }
 }
 shutdown(sockServer, SD_BOTH);
 closesocket(sockServer);
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : Auth
// 功能描述     : 用户登录认证
// 参数         : SOCKET sock
// 参数         : char *szName
// 参数         : char *szPwd
// 返回值       : bool 
//
///////////////////////////////////////////////////////////////////////
bool Auth(SOCKET sock, char *szName, char *szPwd)
{
 char szCmd[50];
 memset(szCmd, 0, 50);
 strcpy(szCmd, szName);
 strcat(szCmd, " ");
 strcat(szCmd, szPwd);
 SCommand cmd;
 cmd.CommandID = CMD_AUTHEN;
 cmd.DataSize = strlen(szCmd);
 int nRet;
 nRet = SendFix(sock, (char *)&cmd, sizeof(cmd));
 if(nRet == SOCKET_ERROR)
 {
  OutErr("SendFix() failed!");
  return false;
 }
 else
 {
  SendFix(sock, szCmd, strlen(szCmd));
  char szBuf[10];
  memset(szBuf, 0, 10);
  recv(sock, szBuf, 10, 0);
  if(strcmp(szBuf, "UP OK!") == 0)
  {
   cout << "登录成功。" << endl;
   g_bAuth = true;
  }
  else if(strcmp(szBuf, "U Err!") == 0)
  {
   cout << "此用户不存在。" << endl;
   g_bAuth = false;
  }
  else if(strcmp(szBuf, "P Err!") == 0)
  {
   cout << "密码错误。" << endl;
   g_bAuth = false;
  }
 }
 return g_bAuth;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : GetFile
// 功能描述     : 取得服务器的文件
// 参数         : SOCKET sock
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void GetFile(SOCKET sock)
{
 if(!g_bAuth)
 {
  OutMsg("用户还没登录！请先登录");
  return;
 }
 char szSrcFile[MAX_PATH];
 char szDstFile[MAX_PATH];
 memset(szSrcFile, 0, MAX_PATH);
 memset(szDstFile, 0, MAX_PATH);
 cout << "你要取得Server上的文件：";
 cin >> szSrcFile;
 cout << "你要把文件存在哪里：";
 cin >> szDstFile;
 SCommand cmd;
 cmd.CommandID = CMD_GETFILE;
 cmd.DataSize = strlen(szSrcFile);
 // 发送命令
 SendFix(sock, (char *)&cmd, sizeof(cmd));
 // 发送文件名
 SendFix(sock, szSrcFile, strlen(szSrcFile));
 // 接收文件长度
 DWORD dwFileSize = 0;
 RecvFix(sock, (char*)&dwFileSize, sizeof(dwFileSize));
 if(dwFileSize == 0)
 {
  OutMsg("文件不存在");
  return;
 }
 // 接收文件内容
 DWORD dwLeft = dwFileSize;
 char szBuf[1024];
 HANDLE hFile = CreateFile(szDstFile, GENERIC_WRITE, FILE_SHARE_READ, 
  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 if(hFile == INVALID_HANDLE_VALUE)
 {
  hFile = CreateFile(szDstFile, GENERIC_WRITE, FILE_SHARE_READ, 
   NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
  if(hFile == INVALID_HANDLE_VALUE)
  {
   OutErr("CreateFile failed!");
   return;
  }
 }
 while(dwLeft > 0)
 {
  memset(szBuf, 0, 1024);
  // 这里是不确定接收长度的，所以要用recv，不能用RecvFix
  int nRead = recv(sock, szBuf, 1024, 0);
  if(nRead == SOCKET_ERROR)
   OutErr("RecvFix Error!");
  DWORD dwWritten = 0;
  if(!WriteFile(hFile, szBuf, nRead, &dwWritten, NULL))
  {
   OutErr("WriteFile error!");
   return;
  }
  dwLeft -= dwWritten;
 }
 CloseHandle(hFile);
 OutMsg("接收文件成功！");
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : RegisterUser
// 功能描述     : 注册新用户
// 参数         : SOCKET sock
// 参数         : char *szName
// 参数         : char *szPwd
// 返回值       : bool 
//
///////////////////////////////////////////////////////////////////////
bool RegisterUser(SOCKET sock, char *szName, char *szPwd)
{
 char szCmd[50];
 memset(szCmd, 0, 50);
 strcpy(szCmd, szName);
 strcat(szCmd, " ");
 strcat(szCmd, szPwd);
 SCommand cmd;
 cmd.CommandID = CMD_REGISTER;
 cmd.DataSize = strlen(szCmd);
 // 发送命令
 int nRet = SendFix(sock, (char *)&cmd, sizeof(cmd));
 if(nRet == SOCKET_ERROR)
 {
  OutErr("SendFix() failed!");
  return false;
 }
 else
 {
  // 发送用户名和密码串
  SendFix(sock, szCmd, strlen(szCmd));
  char szBuf[10];
  memset(szBuf, 0, 10);
  recv(sock, szBuf, 10, 0);
  if(strcmp(szBuf, "REG OK!") == 0)
  {
   cout << "注册成功。" << endl;
   return true;
  }
  else if(strcmp(szBuf, "REG ERR!") == 0)
  {
   cout << "注册失败." << endl;
   return false;
  }
 }
 return false;
}
/**********************************************************************************************************************/
第三个文件，公用的
/*/
文件: CommonSocket.h
说明: 
 实现了服务端和客户端一些公用的函数!
/*/
#ifndef __COMMONSOCKET_H__
#define __COMMONSOCKET_H__
#include <iostream>
using namespace std;
#define OutErr(a) cout << (a) << endl /
      << "出错代码：" << WSAGetLastError() << endl /
      << "出错文件：" << __FILE__ << endl  /
      << "出错行数：" << __LINE__ << endl /
#define OutMsg(a) cout << (a) << endl;
///////////////////////////////////////////////////////////////////////
//
// 函数名       : InitWinsock
// 功能描述     : 初始化WINSOCK
// 返回值       : void 
//
///////////////////////////////////////////////////////////////////////
void InitWinsock()
{
 // 初始化WINSOCK
 WSADATA wsd;
 if( WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
 {
  OutErr("WSAStartup()");
 }
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : ConnectServer
// 功能描述     : 连接SERVER
// 参数         : char *lpszServerIP IP地址
// 参数         : int nPort    端口
// 返回值       : SOCKET    SERVER 的 Socket
//
///////////////////////////////////////////////////////////////////////
SOCKET ConnectServer(char *lpszServerIP, int nPort, ULONG NonBlock)
{
 SOCKET sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
 //ioctlsocket(sServer, FIONBIO, &NonBlock);
 struct hostent *pHost = NULL;
 struct sockaddr_in servAddr;
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = htons(nPort);
 servAddr.sin_addr.s_addr = inet_addr(lpszServerIP);
 // 如果给的是主机的名字而不是IP地址
 if(servAddr.sin_addr.s_addr == INADDR_NONE)
 {
  pHost = gethostbyname( lpszServerIP );
  if(pHost == NULL)
  {
   OutErr("gethostbyname Failed!");
   return NULL;
  }
  memcpy(&servAddr.sin_addr, pHost->h_addr_list[0], pHost->h_length);
 }
 int nRet = 0;
 nRet = connect(sServer, (struct sockaddr *)&servAddr, sizeof(servAddr));
 if( nRet == SOCKET_ERROR )
 {
  OutErr("connect failed!");
  return NULL;
 }
 return sServer;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : BindServer
// 功能描述     : 绑定端口
// 参数         : int nPort
// 返回值       : SOCKET 
//
///////////////////////////////////////////////////////////////////////
SOCKET BindServer(int nPort)
{
 // 创建socket 
 SOCKET sServer = socket(AF_INET, SOCK_STREAM, 0);
 // 绑定端口
 struct sockaddr_in servAddr;
 servAddr.sin_family = AF_INET;
 servAddr.sin_port = htons(nPort);
 servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
 if(bind(sServer, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
 {
  OutErr("bind Failed!");
  return NULL;
 }
 // 设置监听队列为200
 if(listen(sServer, 200) != 0)
 {
  OutErr("listen Failed!");
  return NULL;
 }
 return sServer;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : AcceptClient
// 功能描述     : 
// 参数         : SOCKET sServer [in]
// 参数         : LPSTR lpszIP  [out] 返回客户端的IP地址 
// 返回值       : SOCKET   [out] 返回客户端的socket
//
///////////////////////////////////////////////////////////////////////
SOCKET AcceptClient(SOCKET sListen, LPSTR lpszIP)
{
 struct sockaddr_in cliAddrTmp;
 int cliAddrSize = sizeof(struct sockaddr_in);
 SOCKET sClient = accept(sListen, (struct sockaddr *)&cliAddrTmp, &cliAddrSize);
 if(sClient == INVALID_SOCKET)
 {
  OutErr("accept failed!");
  return NULL;
 }
 sprintf(lpszIP, "%s", inet_ntoa(cliAddrTmp.sin_addr));
 return sClient;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : RecvFix
// 功能描述     : 接收指定长度的数据，考虑非阻塞socket的情况
// 参数         : SOCKET socket [in]
// 参数         : char *data [in]
// 参数         : DWORD len  [in]
// 参数         : DWORD *retlen [out]
// 返回值       : bool 
//
///////////////////////////////////////////////////////////////////////
int RecvFix(SOCKET socket, char *data, DWORD len)
{
 int retlen = 0;
 int nLeft = len;
 int nRead = 0;
 char *pBuf = data;
 while(nLeft > 0)
 {
  nRead = recv(socket, pBuf, nLeft, 0);
  if(nRead == SOCKET_ERROR || nRead == 0)
  {
   if(WSAEWOULDBLOCK == WSAGetLastError())
    continue;
   else
    return nRead;
  }
  nLeft -= nRead;
  retlen += nRead;
  pBuf += nRead;
 }
 return nRead;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : SendFix
// 功能描述     : 发送指定长度的数据，考虑非阻塞socket的情况
// 参数         : SOCKET socket
// 参数         : char *data
// 参数         : DWORD len
// 参数         : DWORD *retlen
// 返回值       : bool 
//
///////////////////////////////////////////////////////////////////////
int SendFix(SOCKET socket, char *data, DWORD len)
{
 int retlen = 0;
 int nLeft = len;
 int nWritten = 0;
 const char *pBuf = data;
 while(nLeft > 0)
 {
  nWritten = send(socket, data, nLeft, 0);
  if(nWritten == SOCKET_ERROR || nWritten == 0)
  {
   if(WSAEWOULDBLOCK == WSAGetLastError())
    continue;
   else
    return nWritten;
  }
  nLeft -= nWritten;
  retlen += nWritten;
  pBuf += nWritten;
 }
 return nWritten;
}
/*
///////////////////////////////////////////////////////////////////////
//
// 函数名       : SelectSend
// 功能描述     : 使用select模型来发送数据，没完成，所以注释掉了
// 参数         : SOCKET sock
// 参数         : FD_SET *wfds
// 参数         : char *data
// 参数         : DWORD len
// 返回值       : bool 
//
///////////////////////////////////////////////////////////////////////
bool SelectSend(SOCKET sock, FD_SET *wfds, char *data, DWORD len)
{
 FD_ZERO(wfds);
 FD_SET(sock, wfds);
 if(select(0, NULL, wfds, NULL, NULL) == SOCKET_ERROR)
 {
  OutErr("select() Failed!");
  return false;
 }
 // 如果是可以写的SOCKET，就一直写，直到返回WSAEWOULDBLOCK
 if( FD_ISSET(sock, wfds) )
 {
  int nLeft = len;
  while(nLeft > 0)
  {
   int nRet = send(sock, data, len, 0);
   if(nRet == SOCKET_ERROR)
    return false;
   nLeft -= nRet;
  }
 }
 return true;
}
///////////////////////////////////////////////////////////////////////
//
// 函数名       : SelectRecv
// 功能描述     : 使用select模型来接收数据，没完成，所以注释掉了
// 参数         : SOCKET sock
// 参数         : FD_SET *rfds
// 参数         : char *data
// 参数         : DWORD len
// 返回值       : bool 
//
///////////////////////////////////////////////////////////////////////
bool SelectRecv(SOCKET sock, FD_SET *rfds, char *data, DWORD len)
{
 FD_ZERO(rfds);
 FD_SET(sock, rfds);
 if(select(0, rfds, NULL, NULL, NULL) == SOCKET_ERROR)
 {
  OutErr("select() Failed!");
  return false;
 }
 if( FD_ISSET(sock, rfds) )
 {
  int nLeft = len;
  while(nLeft > 0)
  {
   int nRet = recv(sock, data, len, 0);
   if(nRet == SOCKET_ERROR)
    return false;
   nLeft -= nRet;
  }
 }
 return true;
}
*/
#endif //__COMMONSOCKET_H__
/**********************************************************************************************************************/
第四个文件，公用的
/*/
文件: CommonCmd.h
说明: 
 实现了服务端和客户端一些公用的数据结构，所以服务端和客户端都要包含。
 其中有命令、SOCKET的当前状态等的定义。
/*/
#ifndef __COMMONCMD_H__
#define __COMMONCMD_H__
#define PORT 5050
// 命令定义
#define CMD_AUTHEN 1 // 登录认证
#define CMD_GETFILE 2 // 获取文件
#define CMD_REGISTER 3  // 注册用户
typedef struct tagCommand
{
 int CommandID;  // 命令ID
 DWORD DataSize;  // 后接数据的大小
}SCommand;
// 标志目前的SOCKET该做什么
enum ECurOp
{RecvCmd, RecvData, ExecCmd};
#endif //__COMMONCMD_H__
 好了，上面四个文件都搞好了，有兴趣的朋友自己去弄吧，希望对你们有用，好累啊（写文章是一件很累的事情）。有什么问题欢迎探讨！
引用:http://blog.csdn.net/dylgsy/archive/2007/01/18/1487143.aspx
