# TCP通信示例程序 - xqhrs232的专栏 - CSDN博客
2013年06月28日 10:12:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：654
个人分类：[3G/RIL/TAPI/网络技术](https://blog.csdn.net/xqhrs232/article/category/906928)
原文地址::[http://blog.csdn.net/durongjian/article/details/5445657](http://blog.csdn.net/durongjian/article/details/5445657)
服务器端程序：
**[cpp]**[view plain](http://blog.csdn.net/durongjian/article/details/5445657#)[copy](http://blog.csdn.net/durongjian/article/details/5445657#)[print](http://blog.csdn.net/durongjian/article/details/5445657#)[?](http://blog.csdn.net/durongjian/article/details/5445657#)
- /*
- 程序名称：TCP通信示例程序服务器端
- 作者：天行健
- 完成时间：2010-4-2  17：00
- */
- #include <iostream>
- #include <WINSOCK2.h>
- **using****namespace** std;  
- #pragma comment(lib,"wsock32.lib")
- #define BUF_SIZE 64
- 
- **void** main()  
- {  
-     WSADATA wsd;                //WSADATA变量
-     SOCKET sServer,sClient;        //服务器端和客户端套接字
- **int** retVal;                    //返回值
- **char** buf[BUF_SIZE];            //存放客户端发来的字符的缓冲区
- 
- //第一步：初始化套接字动态库
- **if**(WSAStartup(MAKEWORD(2,2),&wsd)!=0)  
-     {  
-         cout<<"WSAStartup failed!"<<WSAGetLastError()<<endl;  
- **return**;  
-     }  
- 
- 
-     sServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
- **if**(sServer==INVALID_SOCKET)  
-     {  
-         cout<<"socket failed:"<<WSAGetLastError()<<endl;  
-         WSACleanup();            //释放套接字资源
- **return**;  
-     }  
- 
- //第二步：绑定套接字
-     SOCKADDR_IN addrServer;        //服务器套接字地址信息
-     addrServer.sin_family=AF_INET;  
-     addrServer.sin_port=htons(4999);  
-     addrServer.sin_addr.s_addr=INADDR_ANY;  
- 
-     retVal=bind(sServer,(LPSOCKADDR)&addrServer,**sizeof**(SOCKADDR_IN));  
- **if**(SOCKET_ERROR==retVal)  
-     {  
-         cout<<"bind failed:"<<WSAGetLastError()<<endl;  
-         closesocket(sServer);  
-         WSACleanup();  
- **return**;  
-     }  
- 
- //第三步：监听 
-     retVal=listen(sServer,5);  
- **if**(SOCKET_ERROR==retVal)  
-     {  
-         cout<<"listen failed:"<<WSAGetLastError()<<endl;  
-         closesocket(sServer);    //半闭套接字
-         WSACleanup();            //释放套接字资源
- **return**;  
-     }  
- 
- //第四步：接受客户端连接
-     sockaddr_in addrClient;  
- **int** addrClientlen=**sizeof**(addrClient);  
-     sClient=accept(sServer,(sockaddr FAR*)&addrClient,&addrClientlen);  
- **if**(INVALID_SOCKET==sClient)  
-     {  
-         cout<<"accept failed:"<<WSAGetLastError()<<endl;  
-         closesocket(sServer);  
-         WSACleanup();  
- **return**;  
-     }  
- 
- //第五步：接收并显示数据
-     ZeroMemory(buf,BUF_SIZE);  
-     retVal=recv(sClient,buf,BUF_SIZE,0);  
- **if**(SOCKET_ERROR==retVal)  
-     {  
-         cout<<"recv failed:"<<WSAGetLastError()<<endl;  
-         closesocket(sServer);  
-         WSACleanup();  
- **return**;  
-     }  
- 
-     cout<<buf<<endl;  
- 
- //第六步：退出
-     closesocket(sServer);        //半闭套接字
-     closesocket(sClient);        //半闭套接字
-     WSACleanup();                //释放套接字资源
- }  
/*
程序名称：TCP通信示例程序服务器端
作者：天行健
完成时间：2010-4-2  17：00
*/
#include <iostream>
#include <WINSOCK2.h>
using namespace std;
#pragma comment(lib,"wsock32.lib")
#define BUF_SIZE 64
void main()
{
    WSADATA wsd;                //WSADATA变量
    SOCKET sServer,sClient;        //服务器端和客户端套接字
    int retVal;                    //返回值
    char buf[BUF_SIZE];            //存放客户端发来的字符的缓冲区
    //第一步：初始化套接字动态库
    if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
    {
        cout<<"WSAStartup failed!"<<WSAGetLastError()<<endl;
        return;
    }
    
    sServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sServer==INVALID_SOCKET)
    {
        cout<<"socket failed:"<<WSAGetLastError()<<endl;
        WSACleanup();            //释放套接字资源
        return;
    }
    //第二步：绑定套接字
    SOCKADDR_IN addrServer;        //服务器套接字地址信息
    addrServer.sin_family=AF_INET;
    addrServer.sin_port=htons(4999);
    addrServer.sin_addr.s_addr=INADDR_ANY;
    retVal=bind(sServer,(LPSOCKADDR)&addrServer,sizeof(SOCKADDR_IN));
    if(SOCKET_ERROR==retVal)
    {
        cout<<"bind failed:"<<WSAGetLastError()<<endl;
        closesocket(sServer);
        WSACleanup();
        return;
    }
    //第三步：监听
    retVal=listen(sServer,5);
    if(SOCKET_ERROR==retVal)
    {
        cout<<"listen failed:"<<WSAGetLastError()<<endl;
        closesocket(sServer);    //半闭套接字
        WSACleanup();            //释放套接字资源
        return;
    }
    
    //第四步：接受客户端连接
    sockaddr_in addrClient;
    int addrClientlen=sizeof(addrClient);
    sClient=accept(sServer,(sockaddr FAR*)&addrClient,&addrClientlen);
    if(INVALID_SOCKET==sClient)
    {
        cout<<"accept failed:"<<WSAGetLastError()<<endl;
        closesocket(sServer);
        WSACleanup();
        return;
    }
    //第五步：接收并显示数据
    ZeroMemory(buf,BUF_SIZE);
    retVal=recv(sClient,buf,BUF_SIZE,0);
    if(SOCKET_ERROR==retVal)
    {
        cout<<"recv failed:"<<WSAGetLastError()<<endl;
        closesocket(sServer);
        WSACleanup();
        return;
    }
    cout<<buf<<endl;
    //第六步：退出
    closesocket(sServer);        //半闭套接字
    closesocket(sClient);        //半闭套接字
    WSACleanup();                //释放套接字资源
}
 客户端程序：
**[cpp]**[view plain](http://blog.csdn.net/durongjian/article/details/5445657#)[copy](http://blog.csdn.net/durongjian/article/details/5445657#)[print](http://blog.csdn.net/durongjian/article/details/5445657#)[?](http://blog.csdn.net/durongjian/article/details/5445657#)
- /*
- 程序名称：TCP通信示例程序客户端
- 作者：天行健
- 完成时间：2010-4-2  17：00
- */
- #include <iostream>
- #include <WINSOCK2.h>
- **using****namespace** std;  
- #pragma comment(lib,"wsock32.lib")
- #define BUF_SIZE 64
- 
- **void** main()  
- {  
-     WSADATA wsd;  
-     SOCKET sServer;  
-     SOCKADDR_IN addrServer;  
- **int** retVal;  
- **char** buf[BUF_SIZE];  
- 
- //第一步：创建套接字
- **if**(WSAStartup(MAKEWORD(2,2),&wsd)!=0)  
-     {  
-         cout<<"WSAStartup failed!"<<WSAGetLastError()<<endl;  
- **return**;  
-     }  
- 
-     sServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
- **if**(sServer==INVALID_SOCKET)  
-     {  
-         cout<<"socket failed:"<<WSAGetLastError()<<endl;  
-         WSACleanup();  
- **return**;  
-     }  
- 
- //第二步：连接服务器
-     addrServer.sin_family=AF_INET;  
-     addrServer.sin_port=htons(4999);  
-     addrServer.sin_addr.s_addr=inet_addr("127.0.0.1");  
- 
-     retVal=connect(sServer,(**struct** sockaddr*)&addrServer,**sizeof**(addrServer));  
- **if**(SOCKET_ERROR==retVal)  
-     {  
-         cout<<"connect failed:"<<WSAGetLastError()<<endl;  
-         closesocket(sServer);  
-         WSACleanup();  
- **return**;  
-     }  
- 
- //第三步：发送数据
-     ZeroMemory(buf,BUF_SIZE);  
-     strcpy(buf,"MY TCP");  
-     retVal=send(sServer,buf,strlen(buf),0);  
- **if**(SOCKET_ERROR==retVal)  
-     {  
-         cout<<"send failed:"<<WSAGetLastError()<<endl;  
-         closesocket(sServer);  
-         WSACleanup();  
- **return**;  
-     }  
- 
- //第四步：退出
-     closesocket(sServer);  
-     WSACleanup();  
- }  
/*
程序名称：TCP通信示例程序客户端
作者：天行健
完成时间：2010-4-2  17：00
*/
#include <iostream>
#include <WINSOCK2.h>
using namespace std;
#pragma comment(lib,"wsock32.lib")
#define BUF_SIZE 64
void main()
{
    WSADATA wsd;
    SOCKET sServer;
    SOCKADDR_IN addrServer;
    int retVal;
    char buf[BUF_SIZE];
    //第一步：创建套接字
    if(WSAStartup(MAKEWORD(2,2),&wsd)!=0)
    {
        cout<<"WSAStartup failed!"<<WSAGetLastError()<<endl;
        return;
    }
    sServer=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(sServer==INVALID_SOCKET)
    {
        cout<<"socket failed:"<<WSAGetLastError()<<endl;
        WSACleanup();
        return;
    }
    //第二步：连接服务器
    addrServer.sin_family=AF_INET;
    addrServer.sin_port=htons(4999);
    addrServer.sin_addr.s_addr=inet_addr("127.0.0.1");
    retVal=connect(sServer,(struct sockaddr*)&addrServer,sizeof(addrServer));
    if(SOCKET_ERROR==retVal)
    {
        cout<<"connect failed:"<<WSAGetLastError()<<endl;
        closesocket(sServer);
        WSACleanup();
        return;
    }
    //第三步：发送数据
    ZeroMemory(buf,BUF_SIZE);
    strcpy(buf,"MY TCP");
    retVal=send(sServer,buf,strlen(buf),0);
    if(SOCKET_ERROR==retVal)
    {
        cout<<"send failed:"<<WSAGetLastError()<<endl;
        closesocket(sServer);
        WSACleanup();
        return;
    }
    //第四步：退出
    closesocket(sServer);
    WSACleanup();
}
运行方法：
在DOS窗口中运行服务器端代码编译后生成的EXE程序，然后双击编译客户端代码后生成的EXE程序，即可在DOS窗口中看到 MY TCP 字符，即表示运行成功
//======================================================================================================================
备注::
1>客户端跟服务器建立SOCKET连接后，客户端要收服务器发过来的数据就要对建立的SOCKET连接进行监视-----用SELECT来判断有没数据过来，客户端不用LISTEN来监听；服务器端初始化后就用LISTEN来监听客户端有没要连接，有连接就ACCEPT然后再用SOCKET收发数据就可以了
