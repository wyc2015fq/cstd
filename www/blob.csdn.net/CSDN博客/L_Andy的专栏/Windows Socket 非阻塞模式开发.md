# Windows Socket 非阻塞模式开发 - L_Andy的专栏 - CSDN博客

2015年03月11日 11:12:51[卡哥](https://me.csdn.net/L_Andy)阅读数：767
个人分类：[网络工程](https://blog.csdn.net/L_Andy/article/category/1925965)



非阻塞套接字      

     非阻塞模式是指：套接字在执行操作时，调用的函数不管操作是否完成都会立即返回的工作模式。

    非阻塞套接字在处理同时建立的多个连接等方面具有明显的优势。但是使用过程中有一定的难度。由于函数在操作没有完成后也仍然会返回，为了实现某些功能必须循环调用，直到完成功能为止。因此非阻塞模式会使程序效率非常低。

    把套接字设置为非阻塞模式，即告诉系统：在调用Windows socket API时，不让主调线程睡眠，而让函数立即返回。比如在调用recv函数时，即使此时接受缓冲区没有数据，也不会导致线程在recv处等待，recv函数会立即返回。如果没有调用成功函数会返回WSAEROULDBLOCK错误代码。为了接收到数据必须循环调用recv,这也是非阻塞与阻塞模式的主要区别。

    默认情况下，使用socket或是WSASocket函数创建的套接字都是阻塞的。在创建套接字之后可以调用ioctsocket函数将套接字设置为非阻塞模式。

**[cpp]**[view
 plain](http://blog.csdn.net/ithzhang/article/details/8274596#)[copy](http://blog.csdn.net/ithzhang/article/details/8274596#)

- <span style="font-size:18px;">SOCKET s;  
- 
- unsigned long ul=1;  
- 
- int ret;  
- 
- s=socket(AF_INET,SOCK_STREAM,0);  
- 
- ret=ioctlsocket(s,FIONBIO,(unsigned long *)&ul);//设置成非阻塞模式。
- 
- if(ret==SOCKET_ERROR)//设置失败。
- 
- {  
- 
- }  
- 
- 
-     如果Windows socket api函数在返回时，却没有完成功能，它将返回WSAEWOULDBLOCK错误代码。说明请求的操作在调用期间内没有完成。通常情况下应用程序需要重复调用此函数直到返回值标识为成功为止。  
- 
- 
- while(true)  
- 
- {  
- 
-    ret=recv(s,buff,num,0);  
- 
- if(ret==SOCKET_ERROR)  
- 
-    {  
- 
-       err=WSAGetLastError();  
- 
- if(err==WSAEWOULDBLOCK)  
- 
-       {  
- 
- continue;  
- 
-        }  
- 
- elseif(err==WSAETIMEDOUT)//超时。
- 
-       ｛  
- 
-        ｝  
- 
- elseif(err==WSAENETDOWN)//连接断开。
- 
-        {  
- 
-        }  
- 
- else//其他错误。
- 
- break;  
- 
-    }  
- 
- else
- 
- break;//接受成功。
- 
- ｝  
- 
- </span>  

    在上面的代码中，WSAGetLastError函数返回WSAEWOULDBLOCK错误码时，说明此时套接字缓冲区还没有数据。需要继续调用。除了WSAEWOULDBLOCK错误码之外，还有WSAETIMEDOUT和WSAENETDOWN错误，这些错误说明由于网络原因，与对方已经断开了连接。

     不同的Windows socket api虽然都返回WSAEWOULDBLOCK但是它们所表示的错误原因却不尽相同:

    对于accept和WSAAccept，WSAEWOULDBLOCK表示没有收到连接请求。

    recv和WSARecv、recvfrom、WSARecvfrom，表示接受缓冲区没有收到数据。

    send、WSASend、sendfrom和WSASendfrom标识发送缓冲区不可用。

    connecte、WSAConnect表示连接未能立即完成。

    以上这些函数，在未完成任务时都会返回WSAEWOULDBLOCK，但是bind和WSAStartup函数，却不会返回该错误代码。除了调用ioctlsocket函数，将套接字设置为非阻塞模式之外，还可以调用WSAAsyncSelect和WSAEventselect函数。这会在后面介绍。

    由于使用非阻塞套接字，函数会频繁返回WSAEWOULDBLOCK错误。因此任何时候都应该仔细检查返回代码。

    看一个完整的例子：该例分为服务器程序和客户端程序。客户端向服务器发送任意一段话，服务器在接收到时会返回给客户端当前时间和日期。客户端和服务器都采用非阻塞套接字实现。

服务器程序：

    服务器程序：由主线程、接受客户端请求线程、接收数据线程和发送数据线程。

    主线程负责显示界面，创建其他线程，同时初始化socket库、创建socket监听端口等基本操作。由于不知道客户端何时会发起连接，因此需要不停的循环调用accept，当有客户端连接到来时，接受客户端的连接请求。为了提高性能，创建一个新的线程来执行此操作。此线程为接受客户端请求线程。

由于recv和send函数，在未完成任务时也会返回。因此需要循环调用它们，直到执行成功为止。同样为了提高性能，我们也分别创建两个线程用于循环调用recv和send，接收和发送数据。注意由于每一个连接都既可以发送又可以接受数据，因此为每个连接都创建了这两个线程。为了便于管理，我们为将创建一个类，每个连接都是该类的对象。每个连接都包括接收线程和发送线程。

    CClient类头文件。Client类管理每个连接。

**[cpp]**[view
 plain](http://blog.csdn.net/ithzhang/article/details/8274596#)[copy](http://blog.csdn.net/ithzhang/article/details/8274596#)

- <span style="font-size:18px;">#pragma once  
- #include<iostream>
- #include"windows.h"
- #include"time.h"
- class CClient  
- {  
- public:  
-     CClient(void);  
-     CClient(SOCKET s,sockaddr_in addr);  
-     ~CClient(void);  
- public:  
- bool IsConnected();//判断连接是否中断。
- bool DisConnect();//中断与服务器的连接。
- bool calc();//计算当前时间，并复制到发送缓冲区内。
- bool startRunning();//开始运行发送和接收线程。
- staticDWORD WINAPI sendThread(void*param);//发送线程入口函数。
- staticDWORD WINAPI recvThread(void*param);//接收线程入口函数。
- private:  
- HANDLE m_hSendThread;//发送线程句柄。
- HANDLE m_hRecvThread;//接受线程句柄。
- HANDLE m_hEvent;//发送线程和接收线程同步事件对象。接收客户端请求后通知发送线程发送当前时间。
-     SOCKET m_socket;//与客户端连接套接字。
-     sockaddr_in m_addr;//客户端地址。
- bool m_IsConnected;  
- char *m_pRecvData;//接收缓冲区。
- char *m_pSendData;//发送缓冲区。
- bool m_IsSendData;//由于只有接收到客户端请求后才需要发送，该变量控制是否发送数据。
- };  
- 
- </span>  

CClient类

**[cpp]**[view
 plain](http://blog.csdn.net/ithzhang/article/details/8274596#)[copy](http://blog.csdn.net/ithzhang/article/details/8274596#)

- <span style="font-size:18px;">#include "Client.h"
- 
- 
- CClient::CClient(void)  
- {  
- }  
- 
- CClient::CClient( SOCKET s,sockaddr_in addr )  
- {//初始化各成员变量。
-     m_socket=s;  
-     m_addr=addr;  
-     m_hRecvThread=NULL;  
-     m_hSendThread=NULL;  
-     m_IsConnected=true;  
-     m_IsSendData=true;  
-     m_hEvent=CreateEvent(NULL,true,false,NULL);  
-     m_pRecvData=newchar[1024];  
-     m_pSendData=newchar[1024];  
-     memset(m_pSendData,0,1024);  
-     memset(m_pRecvData,0,1024);  
- }  
- 
- 
- CClient::~CClient(void)  
- {  
- delete []m_pRecvData;  
- delete []m_pSendData;  
- }  
- 
- bool CClient::IsConnected()  
- {  
- return m_IsConnected;  
- }  
- 
- 
- bool CClient::calc()  
- {  
- time_t t;   
- structtm *local;  
- char T[256];  
-      memset(T,0,256);  
-      t=time(NULL);  
-      local=localtime(&t);  
-      sprintf(T,"%d/%d/%d %d:%d:%d",local->tm_year+1900,local->tm_mon+1,local->tm_mday,local->tm_hour,local->tm_min,local->tm_sec);  
-      strcpy(m_pSendData,T);  
- returntrue;  
- }  
- 
- DWORD WINAPI CClient::sendThread( void*param )//发送线程入口函数。
- {  
-     std::cout<<"发送数据线程开始运行！！"<<std::endl;  
-     CClient *pClient=static_cast<CClient*>(param);//获得CClient对象指针。以便操纵成员变量。
-     WaitForSingleObject(pClient->m_hEvent,INFINITE);//等待接收数据线程通知。</span><span style="font-size:18px;">
- while(pClient->m_IsConnected)  
-     {  
- while(pClient->m_IsSendData)//可以发送数据。
-         {  
-             std::cout<<"等待接收数据线程通知！！"<<std::endl;  
- //
- //ResetEvent(pClient->m_hEvent);
- int ret=send(pClient->m_socket,pClient->m_pSendData,1024,0);  
- if(ret==SOCKET_ERROR)  
-             {  
- int r=WSAGetLastError();  
- if(r==WSAEWOULDBLOCK)  
-                 {  
- continue;  
-                 }  
- else
-                 {  
- return 0;  
-                 }  
-             }  
- else
-             {  
-                 std::cout<<"结果发送成功！！"<<std::endl;  
-                 pClient->m_IsSendData=false;  
- break;  
-             }  
- 
-         }  
-         Sleep(1000);//未收到发送通知，睡眠1秒。
- 
-     }  
- }  
- 
- DWORD WINAPI CClient::recvThread( void*param )//接收数据线程入口函数。</span><span style="font-size:18px;">
- {  
-     std::cout<<"接收数据线程开始运行！！"<<std::endl;  
-     CClient *pClient=static_cast<CClient*>(param);  
- while(pClient->m_IsConnected)  
-     {  
-         memset(pClient->m_pRecvData,0,1024);  
- int ret=recv(pClient->m_socket,pClient->m_pRecvData,1024,0);  
- if(ret==SOCKET_ERROR)  
-         {  
- int r=WSAGetLastError();  
- if(r==WSAEWOULDBLOCK)  
-             {  
- //std::cout<<"没有收到来自客户端的数据！！"<<std::endl;
-                 Sleep(20);  
- continue;  
-             }  
- elseif(r==WSAENETDOWN)  
-             {  
-                 std::cout<<"接收数据线程出现错误,连接中断！"<<std::endl;  
- break;  
-             }  
- else
-             {  
-                 std::cout<<"接收数据线程出现错误！"<<std::endl;  
- break;  
-             }  
-         }  
- else
-         {  
-             std::cout<<"恭喜，收到来自客户端的数据:"<<pClient->m_pRecvData<<std::endl;  
-             pClient->calc();  
-             std::cout<<"通知发送线程发送结果！！"<<std::endl;  
-             SetEvent(pClient->m_hEvent);  
-             pClient->m_IsSendData=true;  
-         }  
-     }  
- return 0;  
- }  
- bool CClient::startRunning()//开始为连接创建发送和接收线程。
- {  
-     m_hRecvThread=CreateThread(NULL,0,recvThread,(void*)this,0,NULL);//由于static成员函数，无法访问类成员。因此传入this指针。
- if(m_hRecvThread==NULL)  
-     {  
- returnfalse;  
-     }  
-     m_hSendThread=CreateThread(NULL,0,sendThread,(void*)this,0,NULL);  
- if(m_hSendThread==NULL)  
-     {  
- returnfalse;  
-     }  
- returntrue;  
- 
- }  
- 
- bool CClient::DisConnect()  
- {  
-     m_IsConnected=false;//接收和发送线程退出。资源释放交由资源释放线程。
- returntrue;  
- }  
- </span>  

服务器主文件：

**[cpp]**[view
 plain](http://blog.csdn.net/ithzhang/article/details/8274596#)[copy](http://blog.csdn.net/ithzhang/article/details/8274596#)

- <span style="font-size:18px;">#include"windows.h"
- #include<iostream>
- #include<list>
- #include"Client.h"
- #pragma comment(lib,"wsock32.lib")
- 
- HANDLE hAcceptHandle;  
- HANDLE hCleanHandle;  
- HANDLE hEvent;  
- SOCKET servSocket;  
- CRITICAL_SECTION cs;  
- bool IsServerRunning;  
- std::list<CClient*> clientlist;//该链表中存储与服务器建立的各连接。清理线程将执行对此链表进行删除操作。
- 
- bool InitMemember();//初始化成员变量。
- bool InitSocket();//初始化套接字，设置为非阻塞模式。绑定并监听。
- bool StartService();//开始运行接收客户端请求线程。</span><span style="font-size:18px;">
- bool StopService();//终止服务器运行。
- DWORD WINAPI CleanThread(void*param);//资源清理线程。
- DWORD WINAPI AcceptThread(void*param);//接受客户端请求线程。
- 
- 
- int main(int argc,char**argv)  
- {  
-     InitMemember();  
-     InitSocket();  
- do
-     {  
- char c;  
-         std::cout<<"请选择操作："<<std::endl;  
- 
-         std::cin>>c;  
- if(c=='e')  
-         {  
-             std::cout<<"即将退出服务器程序。"<<std::endl;  
-             StopService();  
-         }  
- elseif(c=='y')  
-         {  
- if(IsServerRunning)  
-             {  
-                 std::cout<<"服务器已经开启，请不要重复开启！！"<<std::endl;  
- 
-             }  
- else
-             {  
-                 StartService();  
-             }  
- 
- 
-         }  
- elseif(c=='n')  
-         {  
- if(!IsServerRunning)  
-             {  
-                 std::cout<<"服务器未开启，无法关闭！！"<<std::endl;  
- 
-             }  
- else
-             {  
-                 StopService();  
- 
-             }  
-         }  
- else
-         {  
- 
-         }  
-         getchar();  
-     }while(IsServerRunning);  
- 
-     Sleep(3000);  
-     WaitForSingleObject(CleanThread,INFINITE);  
- 
- 
- return 0;  
- }  
- 
- bool InitMemember()  
- {  
-     std::cout<<"初始化变量。"<<std::endl;  
- 
-     IsServerRunning=false;  
-     hEvent=NULL;  
-     hCleanHandle=NULL;  
-     hAcceptHandle=NULL;  
-     InitializeCriticalSection(&cs);  
-     servSocket=INVALID_SOCKET;  
- return 0;  
- }  
- 
- bool InitSocket()  
- {  
-     std::cout<<"初始化套接字。"<<std::endl;  
- 
-     WSADATA wsadata;  
-     WSAStartup(MAKEWORD(2,2),&wsadata);  
- 
-     servSocket=socket(AF_INET,SOCK_STREAM,0);  
- if(servSocket==INVALID_SOCKET)  
-     {  
- returnfalse;  
-     }  
-     unsigned long ul=1;  
- int r=ioctlsocket(servSocket,FIONBIO,&ul);  
- if(r==SOCKET_ERROR)  
-     {  
- returnfalse;  
-     }  
-     sockaddr_in addr;  
-     addr.sin_addr.S_un.S_addr=INADDR_ANY;  
-     addr.sin_family=AF_INET;  
-     addr.sin_port=htons(5000);  
-     r=bind(servSocket,(sockaddr*)&addr,sizeof(addr));  
- if(r==SOCKET_ERROR)  
-     {  
- returnfalse;  
-     }  
- int ret=listen(servSocket,10);  
- if(ret==SOCKET_ERROR)  
-     {  
- returnfalse;  
-     }  
- returntrue;  
- }  
- 
- bool StartService()  
- {  
-     std::cout<<"开启服务器。"<<std::endl;  
- 
-     IsServerRunning=true;  
- 
-     hAcceptHandle=CreateThread(NULL,0,AcceptThread,NULL,0,NULL);  
- if(hAcceptHandle==NULL)  
-     {  
- returnfalse;  
-     }  
-     CloseHandle(hCleanHandle);  
-     CloseHandle(hAcceptHandle);  
- }  
- 
- bool StopService()  
- {  
-     std::cout<<"关闭服务器。"<<std::endl;  
-     hCleanHandle=CreateThread(NULL,0,CleanThread,NULL,0,NULL);  
- if(hCleanHandle==NULL)  
-     {  
-         std::cout<<"清理线程创建失败！"<<std::endl;  
- returnfalse;  
-     }  
-     IsServerRunning=false;  
- return 0;  
- }  
- 
- DWORD WINAPI CleanThread( void*param )  
- {  
-     std::cout<<"资源清理线程已运行"<<std::endl;  
- //中断所有连接。退出所有接收和发送线程循环。各线程将会退出。
- for(std::list<CClient*>::iterator iter=clientlist.begin();iter!=clientlist.end();iter++)  
-     {  
-         (*iter)->DisConnect();  
-     }  
-     Sleep(100);  
- for(std::list<CClient*>::iterator iter=clientlist.begin();iter!=clientlist.end();iter++)  
-     {  
- delete *iter;  
-     }  
-     clientlist.clear();  
-     SetEvent(hEvent);  
-     std::cout<<"资源清理完毕，资源清理线程退出！！"<<std::endl;  
- return 0;  
- }  
- 
- DWORD WINAPI AcceptThread( void*param )  
- {  
-     std::cout<<"接受客户端连接线程开始运行。"<<std::endl;  
- 
- while(IsServerRunning)  
-     {  
-         sockaddr_in addr;  
-         SOCKET s;  
- int len=sizeof(addr);  
-         s=accept(servSocket,(sockaddr*)&addr,&len);  
- if(s==SOCKET_ERROR)  
-         {  
- int r=WSAGetLastError();  
- if(r==WSAEWOULDBLOCK)  
-             {  
- //std::cout<<"未收到客户端的连接请求。"<<std::endl;
- 
-                 Sleep(1000);  
- continue;  
-             }  
- else
-             {  
-                 std::cout<<"未知错误，接受客户端连接线程退出。"<<std::endl;  
-                 getchar();  
- returnfalse;  
-             }  
-         }  
- else//收到客户端请求。
-         {  
-             std::cout<<"收到客户端的连接请求。"<<std::endl;  
- 
-             CClient*pClient=new CClient(s,addr);  
-             pClient->startRunning();//该链接接受和发送线程开始执行。
-             clientlist.push_back(pClient);  
-         }  
-     }  
-     std::cout<<"接受客户端连接线程退出。"<<std::endl;  
- 
- return 0;  
- }  
- 
- 
- 
- </span>  


客户端：

    客户端程序：由主线程、接收数据线程和发送数据线程组成。

主线程：负责界面显示，初始化socket库、创建套接字，连接服务器、接收用户输入、创建发送和接收数据线程。

发送线程：向服务器发送数据。

接收线程：接收从服务器发送的时间日期。

客户端主文件：

**[cpp]**[view
 plain](http://blog.csdn.net/ithzhang/article/details/8274596#)[copy](http://blog.csdn.net/ithzhang/article/details/8274596#)

- <span style="font-size:18px;">#include<iostream>  
- #include"windows.h"
- #pragma  comment (lib,"wsock32.lib")
- SOCKET clientSocket;  
- HANDLE hRecvThread;  
- HANDLE hSendThread;  
- bool IsConnected;  
- char sendBuff[1024];  
- char recvBuff[1024];  
- HANDLE hEvent;  
- HANDLE hSendEvent;  
- bool InitMember();  
- bool InitSocket();  
- bool startConnect();  
- DWORD WINAPI recvThread(void*param);  
- DWORD WINAPI sendThread(void*param);  
- 
- 
- int main(int argc,char**argv)  
- {  
-     InitMember();  
-     InitSocket();  
-     startConnect();  
- char buff[256];  
- while(IsConnected)  
-     {  
-         memset(buff,0,256);  
-         std::cout<<"请输入表达式：";  
-         std::cin>>sendBuff;  
- if(!strcmp(buff,"exit"))  
-         {  
-             std::cout<<"即将推出！"<<std::endl;  
-             IsConnected=false;  
- HANDLE hHandleArray[2];  
-             hHandleArray[0]=hRecvThread;  
-             hHandleArray[1]=hSendThread;  
-             SetEvent(hEvent);  
-             WaitForMultipleObjects(2,hHandleArray,true,INFINITE);  
-             getchar();  
- return 0;  
-         }  
- else
-         {  
-             SetEvent(hEvent);  
-         }  
-     }  
- 
- 
- return 0;  
- }  
- //初始化套接字。
- bool InitSocket()  
- {  
-     WSAData wsadata;  
-     WSAStartup(MAKEWORD(2,2),&wsadata);  
-     clientSocket=socket(AF_INET,SOCK_STREAM,0);  
- if(clientSocket==INVALID_SOCKET)  
-     {  
- returnfalse;  
-     }  
-     unsigned long ul=1;  
-     ioctlsocket(clientSocket,FIONBIO,&ul);  
- 
-     sockaddr_in addr;  
-     addr.sin_family=AF_INET;  
-     addr.sin_port=htons(5000);  
-     addr.sin_addr.S_un.S_addr=inet_addr("192.168.1.100");  
- while(true)  
-     {  
- int ret=connect(clientSocket,(sockaddr*)&addr,sizeof(addr));  
- if(ret==SOCKET_ERROR)  
-         {  
- int r=WSAGetLastError();  
- if(r==WSAEWOULDBLOCK||r==WSAEINVAL)  
-             {  
-                 Sleep(20);  
- continue;  
-             }  
- elseif(r==WSAEISCONN)//套接字原来已经连接！！
-             {  
- break;  
-             }  
- else
-             {  
-                 std::cout<<"发生错误"<<std::endl;  
- returnfalse;  
-             }  
-         }  
- if(ret==0)  
-         {  
- break;  
-         }  
-     }  
-     IsConnected=true;  
- returntrue;  
- }  
- 
- bool InitMember()  
- {  
-     hSendThread=NULL;  
-     hRecvThread=NULL;  
-     hEvent=CreateEvent(NULL,true,false,NULL);  
-     hSendEvent=CreateEvent(NULL,true,false,NULL);  
-     IsConnected=false;  
-     memset(recvBuff,0,1024);  
-     memset(sendBuff,0,1024);  
- returntrue;  
- }  
- //创建接收和发送数据线程。
- bool startConnect()  
- {  
-     hRecvThread=CreateThread(NULL,0,recvThread,NULL,0,NULL);  
- if(hRecvThread==NULL)  
-     {  
- returnfalse;  
-     }  
-     hSendThread=CreateThread(NULL,0,sendThread,NULL,0,NULL);  
- if(hSendThread==NULL)  
-     {  
- returnfalse;  
-     }  
- return 0;  
- }  
- //接收数据线程入口函数。
- DWORD WINAPI recvThread( void*param )  
- {  
-     std::cout<<"数据接收线程已开始运行！"<<std::endl;  
- while(IsConnected)  
-     {  
-         WaitForSingleObject(hSendEvent,INFINITE);  
- //ResetEvent(hSendEvent);
- int ret=recv(clientSocket,recvBuff,1024,0);  
- if(ret==SOCKET_ERROR)  
-         {  
- int r=WSAGetLastError();  
- if(r==WSAEWOULDBLOCK)  
-             {  
- //std::cout<<"没有收到服务器返回的数据！！"<<std::endl;
-                 Sleep(10);  
- continue;  
-             }  
- elseif(r==WSAENETDOWN)  
-             {  
-                 std::cout<<"数据发送失败！"<<std::endl;  
- returnfalse;  
-             }  
-         }  
- else
-         {  
-             std::cout<<"接收成功！"<<std::endl;  
- 
-             std::cout<<recvBuff<<std::endl;  
-         }  
-     }  
- returntrue;  
- }  
- //发送数据线程入口函数。
- DWORD WINAPI sendThread( void*param )  
- {  
-     std::cout<<"数据发送线程已开始运行！！"<<std::endl;  
- 
- while(IsConnected)//是否与服务器连接
-     {  
-         WaitForSingleObject(hEvent,INFINITE);//等待接收数据线程通知。
-         ResetEvent(hEvent);  
- int ret=send(clientSocket,sendBuff,256,0);  
- if(ret==SOCKET_ERROR)  
-         {  
- int r=WSAGetLastError();  
- if(r==WSAEWOULDBLOCK)  
-             {  
-                 std::cout<<"数据发送失败！"<<std::endl;  
-                 Sleep(20);  
- continue;  
-             }  
- else
-             {  
-                 std::cout<<"数据发送失败！"<<std::endl;  
- break;  
-             }  
-         }  
- else
-         {  
-             std::cout<<"发送成功！！"<<std::endl;  
-             SetEvent(hSendEvent);  
-         }  
-     }  
- returntrue;  
- }  
- </span>  

运行结果： 
![](https://img-my.csdn.net/uploads/201212/09/1355024678_6610.jpg)

