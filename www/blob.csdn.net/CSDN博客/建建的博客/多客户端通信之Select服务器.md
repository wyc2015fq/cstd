# 多客户端通信之Select服务器 - 建建的博客 - CSDN博客
2017年01月30日 21:15:12[纪建](https://me.csdn.net/u013898698)阅读数：308
//主要是增加了一个连接管理类，以及增加了消息处理线程 
//消息处理线程其实是不断地在循环看列表中是否有新客户端添加进来 
//添加进来就加入到select的读队列中，然后设置select的等待时间为0 
//就是立即返回，非阻塞模式,select主要是一旦底层要有数据过来以后会根据IO的变化通知select的函数立即返回，然后遍历所有的socket，找到读变化的客户端，获取发送过来的休息，但是这个还有一个问题，会有半包的现象，因为tcp是数据流，他只保证不断流，但不保证水大小一样。/后续再继续解决 
// server3.cpp : 定义控制台应用程序的入口点。 
//
# include “stdafx.h”
# include 
# include 
# include “SocketList.h”
using namespace std;
DWORD WINAPI WorkThread(LPVOID lpParam) 
{ 
    SocketList* userlist=(SocketList*)lpParam; 
    int err=0; 
    fd_set fdread; 
    struct timeval timeout; 
    timeout.tv_sec=0; 
    timeout.tv_usec=0;
```
char receBuff[MAX_PATH];
char sendBuf[MAX_PATH];
SOCKET socket;
while (true)
{
    userlist->makefd(&fdread);
    err=select(0,&fdread,NULL,NULL,&timeout);
    if (err==0)
    {
    }else
    {
        for (int i=0;i<FD_SETSIZE;i++)
        {
            if (userlist->getSocket(i)==0)
            {
                continue;
            }
            socket=userlist->getSocket(i);
            if (FD_ISSET(socket,&fdread))
            {
                err=recv(socket,receBuff,MAX_PATH,0);  
                if (err==0||err==SOCKET_ERROR)
                {
                    closesocket(socket);  
                    std::cout<<"断开连接!"<<std::endl;  
                    userlist->deleteSocket(socket);  
                } 
                else
                {
                    printf("%s \n",receBuff);
                }
            }
        }
    }
}
return 1;
```
}
int _tmain(int argc, _TCHAR* argv[]) 
{ 
    //初始化Winsock 
    WSADATA wsaDATA; 
    WORD version = MAKEWORD(2, 0); 
    int ret = WSAStartup(version, &wsaDATA); 
    if(ret != 0) 
        cout << “初始化失败!” << endl;
```
SOCKET m_hSocket;
SOCKADDR_IN m_addr,clientAdd;
//创建新的套接字，这里是创建的流类型的套接字
m_hSocket = socket(AF_INET, SOCK_STREAM,0);
m_addr.sin_family = AF_INET;
m_addr.sin_addr.S_un.S_addr = INADDR_ANY;
m_addr.sin_port = 1234;
//绑定一个套接字到本机的地址
ret = 0;
int error = 0;
ret = bind(m_hSocket, (LPSOCKADDR)&m_addr, sizeof(m_addr));
if(ret == SOCKET_ERROR)
{
    cout << "绑定失败：" << (error = WSAGetLastError()) << endl;
    system("pause");
    return 0;
}
//开始一个侦听过程，等待用户的连接
ret = listen(m_hSocket, 5);
if(ret == SOCKET_ERROR)
{
    cout << "侦听失败：" << (error = WSAGetLastError()) << endl;
    system("pause");
    return 0;
}
//非堵塞等待客户的连接
SOCKET sock;                                                  //这个用来接受一个连接
fd_set rfd, wfd;                                              //描述符集 这个将用来测试有没有一个可用的连接
struct timeval timeout;
FD_ZERO(&rfd);                                                //总是这样先清空一个描述符集
timeout.tv_sec = 60;                                          //等下select用到这个
timeout.tv_usec = 0;
u_long ul = 1;
ioctlsocket(m_hSocket, FIONBIO, &ul);                         //用非阻塞的连接
SOCKET sockConn;  
SocketList socketList;  
HANDLE hThread=CreateThread(NULL,0,WorkThread,&socketList,0,NULL);
if (hThread==NULL)  
{  
    std::cout<<"Create Thread Failed!"<<std::endl;  
}  
CloseHandle(hThread);  
int addsize=sizeof(clientAdd);
while (true)
{
    //5.接收请求，当收到请求后，会将客户端的信息存入clientAdd这个结构体中，并返回描述这个TCP连接的Socket 
    sockConn=accept(m_hSocket,(struct sockaddr*)&clientAdd,&addsize);  
    if (sockConn==INVALID_SOCKET)  
    {  
        continue;  
        //return -1;  
    }  
    cout<<"客户端连接："<<inet_ntoa(clientAdd.sin_addr)<<":"<<clientAdd.sin_port<<endl;  
    //将之前的第6步替换成了上面启动workThread这个线程函数和下面这一行代码  
    //将socket放入socketList中  
    socketList.insertSocket(sockConn);  
}
system("pause");
return 0;
```
}
但是现在还没有解决半包和粘包的问题。后续继续更新，妈的写代码天天写有鸟用，干着没意思，还不如考公务员。草
