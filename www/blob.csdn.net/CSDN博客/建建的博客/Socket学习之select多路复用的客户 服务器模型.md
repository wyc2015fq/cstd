# Socket学习之select多路复用的客户/服务器模型 - 建建的博客 - CSDN博客
2017年01月29日 22:23:22[纪建](https://me.csdn.net/u013898698)阅读数：147
# 1. 阻塞与非阻塞
首先介绍几个基本的概念。
阻塞方式（ block ），顾名思义，就是进程或是线程执行到这些函数时必须等待某个事件的发生，假如事件没有发生，进程或线程就被阻塞，函数不能立即返回。
非阻塞方式（ non-block ），就是进程或线程执行此函数时不必非要等待事件的发生，一旦执行肯定返回，以返回值的不同来反映函数的执行情况，假如事件发生则和阻塞方式相同，若事件没有发生则返回一个代码来告知事件未发生，而进程或线程继续执行，所以效率较高。
套接字相关函数默认时采用阻塞方式操作，而大多数情况下，程序不仅仅只拥有一个套接字。当进程以阻塞方式操作其中之一时，必将导致不能使用其他的套接字。如果希望这些套接字同时工作，就必须设计并发的套接字程序，即在一个套接字读写的同时保证另一个套接字也能正常地操作。
多路复用函数 select 把一些文件描述符集合在一起，如果某个文件描述符的状态发生变化，比如进入“写就绪”或者“读就绪”状态，函数 select 会立即返回，并且通知进程读取或写入数据；如果没有 I/O 到达，进程将进入阻塞，直到函数 select 超时退出为止。
套接字也是文件，函数 select 也支持套接字描述符，并且可以应用在 TCP 协议和 UDP 协议的套接字中。利用多路复用，进程就可以同时监控多个套接字信息，在多个套接字上并发地执行操作。
# 2.select介绍
select的大概思想：将多个套接字放在一个集合里，然后统一检查这些套接字的状态（可读、可写、异常等），调用select后，会更新这些套接字的状态，然后做判断，如果套接字可读，就执行read操作。这样就巧妙地避免了阻塞，达到同时处理多个连接的目的。当然如果没有事件发生，select会一直阻塞，如果不想一直让它等待，想去处理其它事情，可以设置一个最大的等待时间。
**[cpp]**[view
 plain](http://blog.csdn.net/u012501459/article/details/48162367#)[copy](http://blog.csdn.net/u012501459/article/details/48162367#)
- int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout);  
- 
- /*参数列表
- int maxfdp是一个整数值，是指集合中所有文件描述符的范围，即所有文件描述符的最大值加1，不能错！在Windows中这个参数的值无所谓，可以设置不正确。 
- 
- fd_set *readfds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的读变化的，即我们关心是否可以从这些文件中读取数据了，如果这个集合中有一个文件可读，select就会返回一个大于0的值，表示有文件可读，如果没有可读的文件，则根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的读变化。 
- 
- fd_set *writefds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的写变化的，即我们关心是否可以向这些文件中写入数据了，如果这个集合中有一个文件可写，select就会返回一个大于0的值，表示有文件可写，如果没有可写的文件，则根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的写变化。 
- 
- fd_set *errorfds同上面两个参数的意图，用来监视文件错误异常。 
- 
- struct timeval* timeout是select的超时时间，这个参数至关重要，它可以使select处于三种状态：
- 第一，若将NULL以形参传入，即不传入时间结构，就是将select置于阻塞状态，一定等到监视文件描述符集合中某个文件描述符发生变化为止；
- 第二，若将时间值设为0秒0毫秒，就变成一个纯粹的非阻塞函数，不管文件描述符是否有变化，都立刻返回继续执行，文件无变化返回0，有变化返回一个正值；
- 第三，timeout的值大于0，这就是等待的超时时间，即 select在timeout时间内阻塞，超时时间之内有事件到来就返回了，否则在超时后不管怎样一定返回，返回值同上述。
- */
- 
- /*
- 返回值： 
- 
- 
- 负值：select错误
- 
- 正值：某些文件可读写或出错
- 
- 0：等待超时，没有可读写或错误的文件
- */
上面有两个结构体。
第一， struct timeval 代表时间值
**[cpp]**[view
 plain](http://blog.csdn.net/u012501459/article/details/48162367#)[copy](http://blog.csdn.net/u012501459/article/details/48162367#)
- struct timeval{  
- 
- long tv_sec;/* 秒 */
- 
- long tv_usec;/* 毫秒 */
- 
- 
};  
第二， struct fd_set 能够理解为文件描述符的集合
**[cpp]**[view
 plain](http://blog.csdn.net/u012501459/article/details/48162367#)[copy](http://blog.csdn.net/u012501459/article/details/48162367#)
- #ifndef FD_SETSIZE
- #define FD_SETSIZE      64
- #endif /* FD_SETSIZE */
- 
- typedefstruct fd_set {  
- 
        u_int fd_count;               /* how many are SET? */
- 
        SOCKET  fd_array[FD_SETSIZE];   /* an array of SOCKETs */
- 
} fd_set;  
fd_set 集合能够通过一些宏由人为来操作，比如清空集合 FD_ZERO(fd_set *) ，将一个给定的文件描述符加入集合之中 FD_SET(int ,fd_set*) ，将一个给定的文件描述符从集合中删除 FD_CLR(int,fd_set*)，检查集合中指定的文件描述符是否能够读写 FD_ISSET(int ,fd_set* ) 。
# 3.操作fd_set的宏
下面是这些宏的详细描述：
- FD_ZERO(fd_set * set)，是把集合清空（初始化为0，确切的说，是把集合中的元素个数初始化为0，并不修改描述符数组).使用集合前，必须用FD_ZERO初始化，否则集合在栈上作为自动变量分配时，fd_set分配的将是随机值，导致不可预测的问题。
- FD_SET(int s,fd_set * set)，向集合中加入一个套接口描述符（如果该套接口描述符s没在集合中，并且数组中已经设置的个数小于最大个数时，就把该描述符加入到集合中，集合元素个数加1）。这里是将s的值直接放入数组中。
- FD_ISSET(int s,fd_set * set)，检查描述符是否在集合中，如果在集合中返回非0值，否则返回0. 它的宏定义并没有给出具体实现，但实现的思路很简单，就是搜索集合，判断套接字s是否在数组中。
- FD_CLR(int s,fd_set * set)，从集合中移出一个套接口描述符（比如一个套接字连接中断后，就应该移除它）。实现思路是，在数组集合中找到对应的描述符，然后把后面的描述依次前移一个位置，最后把描述符的个数减1。
下面是使用这些宏的基本方式。
- 调用FD_ZERO来初始化fd_set；
- 调用FD_SET将感兴趣的套接字描述符加入fd_set集合中（每次循环都要重新加入，因为select更新后，会将一些没有满足条件的套接字移除队列）；
- 设置等待时间后，调用select函数--更新套接字的状态；
- 调用FD_ISSET，来判断套接字是否有相应状态，然后做相应操作，比如，如果套接字可读，就调用recv函数去接收数据。
# 4.自定义管理套接字的集合类SocketList
从上面可以看出，我们需要维护一个套接字集合，这个套接字的集合里存放的是和服务器建立连接的套接字，服务器每次循环调用select()时首先需要使用FD_ZERO宏来初始化fd_set对象，然后调用FD_SET将我们维护的这个套接字集合中的套接字加入fd_set这个集合中，然后就可以调用select函数了，调用完select函数后，再次遍历我们维护的这个套接字集合，通过FD_ISSET宏来判断这个套接字是否是我们需要进行处理的，如果是需要进行处理的，那么就对它进行处理。
所以现在这里的关键就是要维护一个套接字的集合，我把它定义为SocketList这个类，下面是我实现的C++类。这个套接字的集合类可以很方便地实现下面的功能：
- 添加socket
- 删除socket
- 将前类中的所有感兴趣的socket加入到fd_set中
# 5.服务器端的实现
仍然是在原来服务端的第6步进行更改，但是在这之前启动了一个新的线程workThread，并且服务端也只需要启动一个这样的线程用来访问SocketList的对象。此时第6步就只需要将当前建立的socket添加到SocketList这个对象中。在workThread这个线程中访问SocketList的对象，在这个线程中调用select()函数即可。
服务端代码：
**[cpp]**[view
 plain](http://blog.csdn.net/u012501459/article/details/48162367#)[copy](http://blog.csdn.net/u012501459/article/details/48162367#)
- #include <stdio.h>
- #include <stdlib.h>
- #include <WinSock2.h>
- #include <iostream>
- 
- #pragma comment(lib, "ws2_32.lib")
- 
- 
- usingnamespace std;  
- 
- #define  PORT 6000
- //#define  IP_ADDRESS "10.11.163.113"  //表示服务器端的地址
- #define  IP_ADDRESS "127.0.0.1"  //直接使用本机地址
- 
- class SocketList  
- 
{  
- private:  
- int num;//记录socket的真实数目
- 
    SOCKET socketArray[FD_SETSIZE];//存放socket的数组
- 
- public:  
- 
    SOCKET getSocket(int i)  
- 
    {  
- return socketArray[i];  
- 
    }  
- 
- //构造函数中对两个成员变量进行初始化
- 
    SocketList()  
- 
    {  
- 
        num=0;  
- for (int i=0;i<FD_SETSIZE;i++)  
- 
        {  
- //因为socket的值是一个非负整数值，所以可以将socketArray初始化为0，让它来表示数组中的这一个元素有没有被使用
- 
            socketArray[i]=0;  
- 
        }  
- 
    }  
- 
- //往socketArray中添加一个socket
- void insertSocket(SOCKET s)  
- 
    {  
- for (int i=0;i<FD_SETSIZE;i++)  
- 
        {  
- //如果某一个socketArray[i]为0，表示哪一个位可以放入socket
- if (socketArray[i]==0)  
- 
            {  
- 
                socketArray[i]=s;  
- 
                num++;  
- break;//这里一定要加上break，不然一个socket会放在socketArray的多个位置上
- 
            }  
- 
        }  
- 
    }  
- 
- //从socketArray中删除一个socket
- void deleteSocket(SOCKET s)  
- 
    {  
- for (int i=0;i<FD_SETSIZE;i++)  
- 
        {  
- if (socketArray[i]==s)  
- 
            {  
- 
                socketArray[i]=0;  
- 
                num--;  
- break;  
- 
            }  
- 
        }  
- 
    }  
- 
- //将socketArray中的套接字放入fd_list这个结构体中
- void makefd(fd_set * fd_list)  
- 
    {  
- 
            FD_ZERO(fd_list);//首先将fd_list清0
- 
- //将每一个socket加入fd_list中
- for (int i=0;i<FD_SETSIZE;i++)  
- 
            {  
- if (socketArray[i]>0)  
- 
                {  
- 
                    FD_SET(socketArray[i],fd_list);  
- 
                }  
- 
            }  
- 
    }  
- 
- 
};  
- 
- 
- //使用这一个线程来通过select来处理多个socket
- DWORD WINAPI workThread(LPVOID lpParam)  
- 
{  
- //传递进来的socketList指针
- 
        SocketList * socketList=(SocketList *)lpParam;  
- int err=0;  
- 
        fd_set fdread;//存在读文件的set，select会检测这个set中是否可以从某些socket中读入信息
- 
- struct timeval timeout;//设置select超时的时间
- 
        timeout.tv_sec=6;  
- 
        timeout.tv_usec=0;  
- 
- //输入输出缓冲区
- char receBuff[MAX_PATH];  
- char sendBuf[MAX_PATH];  
- 
- 
        SOCKET socket;  
- 
- while(true)  
- 
        {  
- 
                socketList->makefd(&fdread);  
- 
                err=select(0,&fdread,NULL,NULL,&timeout);  
- if (err==0)//select返回0表示超时
- 
                {  
- 
                    cout<<"select() is time-out!"<<endl;  
- continue;  
- 
                }  
- else
- 
                {  
- //遍历socketList中的每一个socket，查看那些socket是可读的，处理可读的socket
- //从中读取数据到缓冲区，并发送数据给客户端
- for (int i=0;i<FD_SETSIZE;i++)  
- 
                        {  
- //读取有效的socket
- if (socketList->getSocket(i)==0)  
- continue;  
- 
                            socket=socketList->getSocket(i);  
- 
- //判断哪些socket是可读的，如果这个socket是可读的，从它里面读取数据
- if (FD_ISSET(socket,&fdread))  
- 
                            {  
- 
                                err=recv(socket,receBuff,MAX_PATH,0);  
- 
- //如果返回值表示要关闭这个连接，那么关闭它，并将它从sockeList中去掉
- if (err==0||err==SOCKET_ERROR)  
- 
                                {  
- 
                                    closesocket(socket);  
- 
                                    cout<<"断开连接!"<<endl;  
- 
                                    socketList->deleteSocket(socket);  
- 
                                }  
- else
- 
                                {  
- 
                                    cout<<"message from client:"<<receBuff<<endl;  
- 
- 
                                    strcpy(sendBuf,"server receive a message:");  
- 
                                    strcat(sendBuf,receBuff);  
- //发送数据
- 
                                    send(socket,sendBuf,strlen(sendBuf)+1,0);   //第三个参数加上1是为了将字符串结束符'\0'也发送过去   
- 
                                }  
- 
- 
                            }//end if
- 
- 
                        }//end for
- 
- 
                }//end if (err==0)
- 
- 
        }//end while
- 
- 
- return 0;  
- 
}  
- 
- void main()  
- 
{  
- 
- 
    WSADATA wsaData;  
- int err;  
- 
- //1.加载套接字库
- 
    err=WSAStartup(MAKEWORD(1,1),&wsaData);  
- if (err!=0)  
- 
    {  
- 
        cout<<"Init Windows Socket Failed::"<<GetLastError()<<endl;  
- return ;  
- 
    }  
- 
- //2.创建socket
- //套接字描述符,SOCKET实际上是unsigned int
- 
    SOCKET serverSocket;  
- 
    serverSocket=socket(AF_INET,SOCK_STREAM,0);  
- if (serverSocket==INVALID_SOCKET)  
- 
    {  
- 
        cout<<"Create Socket Failed::"<<GetLastError()<<endl;  
- return ;  
- 
    }  
- 
- 
- //服务器端的地址和端口号
- struct sockaddr_in serverAddr,clientAdd;  
- 
    serverAddr.sin_addr.s_addr=inet_addr(IP_ADDRESS);  
- 
    serverAddr.sin_family=AF_INET;  
- 
    serverAddr.sin_port=htons(PORT);  
- 
- //3.绑定Socket，将Socket与某个协议的某个地址绑定
- 
    err=bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));  
- if (err!=0)  
- 
    {  
- 
        cout<<"Bind Socket Failed::"<<GetLastError()<<endl;  
- return ;  
- 
    }  
- 
- 
- //4.监听,将套接字由默认的主动套接字转换成被动套接字
- 
    err=listen(serverSocket,10);  
- if (err!=0)  
- 
    {  
- 
        cout<<"listen Socket Failed::"<<GetLastError()<<endl;  
- return ;  
- 
    }  
- 
- 
    cout<<"服务器端已启动......"<<endl;  
- 
- int addrLen=sizeof(clientAdd);  
- 
    SOCKET sockConn;  
- 
    SocketList socketList;  
- HANDLE hThread=CreateThread(NULL,0,workThread,&socketList,0,NULL);  
- if (hThread==NULL)  
- 
    {  
- 
        cout<<"Create Thread Failed!"<<endl;  
- 
    }  
- 
    CloseHandle(hThread);  
- 
- while(true)  
- 
    {  
- //5.接收请求，当收到请求后，会将客户端的信息存入clientAdd这个结构体中，并返回描述这个TCP连接的Socket
- 
        sockConn=accept(serverSocket,(struct sockaddr*)&clientAdd,&addrLen);  
- if (sockConn==INVALID_SOCKET)  
- 
        {  
- 
            cout<<"Accpet Failed::"<<GetLastError()<<endl;  
- return ;  
- 
        }  
- 
        cout<<"客户端连接："<<inet_ntoa(clientAdd.sin_addr)<<":"<<clientAdd.sin_port<<endl;  
- 
- //将之前的第6步替换成了上面启动workThread这个线程函数和下面这一行代码
- //将socket放入socketList中
- 
        socketList.insertSocket(sockConn);  
- 
    }  
- 
- 
    closesocket(serverSocket);  
- //清理Windows Socket库
- 
    WSACleanup();  
- 
}  
