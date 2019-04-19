# Linux下基于C/C++的Socket编程基础 - maopig的专栏 - CSDN博客
2011年08月18日 23:31:27[maopig](https://me.csdn.net/maopig)阅读数：1392
什么是Socket
Socket接口是TCP/IP网络的API，Socket接口定义了许多函数或例程，程序员可以用它们来开发TCP/IP网络上的应用程序。要学Internet上的TCP/IP网络编程，必须理解Socket接口。
　　 Socket接口设计者最先是将接口放在Unix操作系统里面的。如果了解Unix系统的输入和输出的话，就很容易了解Socket了。网络的 Socket数据传输是一种特殊的I/O，Socket也是一种文件描述符。Socket也具有一个类似于打开文件的函数调用Socket()，该函数返回一个整型的Socket描述符，随后的连接建立、数据传输等操作都是通过该Socket实现的。常用的Socket类型有两种：流式 Socket（SOCK_STREAM）和数据报式Socket（SOCK_DGRAM）。流式是一种面向连接的Socket，针对于面向连接的TCP服务应用；数据报式Socket是一种无连接的Socket，对应于无连接的UDP服务应用。
Socket建立
　　为了建立Socket，程序可以调用Socket函数，该函数返回一个类似于文件描述符的句柄。socket函数原型为：
 int socket(int domain, int type, int protocol);
domain指明所使用的协议族，通常为AF_INET，表示互联网协议族（TCP/IP协议族）；type参数指定socket的类型：SOCK_STREAM 或SOCK_DGRAM，Socket接口还定义了原始Socket（SOCK_RAW），允许程序使用低层协议；protocol通常赋值"0"。Socket()调用返回一个整型socket描述符，你可以在后面的调用使用它。
　　 Socket描述符是一个指向内部数据结构的指针，它指向描述符表入口。调用Socket函数时，socket执行体将建立一个Socket，实际上"建立一个Socket"意味着为一个Socket数据结构分配存储空间。Socket执行体为你管理描述符表。
　　两个网络程序之间的一个网络连接包括五种信息：通信协议、本地协议地址、本地主机端口、远端主机地址和远端协议端口。Socket数据结构中包含这五种信息。
Socket配置
　　通过socket调用返回一个socket描述符后，在使用socket进行网络传输以前，必须配置该socket。面向连接的socket客户端通过调用connect函数在socket数据结构中保存本地和远端信息。无连接socket的客户端和服务端以及面向连接socket的服务端通过调用bind函数来配置本地信息。
bind函数将socket与本机上的一个端口相关联，随后你就可以在该端口监听服务请求。bind函数原型为：
　　 int bind(int sockfd,struct sockaddr *my_addr, int addrlen);
sockfd是调用socket函数返回的socket描述符,my_addr是一个指向包含有本机IP地址及端口号等信息的sockaddr类型的指针；addrlen常被设置为sizeof(struct sockaddr)。
struct sockaddr结构类型是用来保存socket信息的：
　　 struct sockaddr {
　　 unsigned short sa_family; /* 地址族， AF_xxx */
      char sa_data[14]; /* 14 字节的协议地址 */
      };
sa_family一般为AF_INET，代表Internet（TCP/IP）地址族；sa_data则包含该socket的IP地址和端口号。
另外还有一种结构类型：
　 struct sockaddr_in {
　　 short int sin_family; /* 地址族 */
　　 unsigned short int sin_port; /* 端口号 */
　　 struct in_addr sin_addr; /* IP地址 */
　　 unsigned char sin_zero[8]; /* 填充0 以保持与struct sockaddr同样大小 */
　　 };
　　这个结构更方便使用。sin_zero用来将sockaddr_in结构填充到与struct sockaddr同样的长度，可以用bzero()或memset()函数将其置为零。指向sockaddr_in 的指针和指向sockaddr的指针可以相互转换，这意味着如果一个函数所需参数类型是sockaddr时，你可以在函数调用的时候将一个指向 sockaddr_in的指针转换为指向sockaddr的指针；或者相反。
　　使用bind函数时，可以用下面的赋值实现自动获得本机IP地址和随机获取一个没有被占用的端口号：
　　 my_addr.sin_port = 0; /* 系统随机选择一个未被使用的端口号 */
　　 my_addr.sin_addr.s_addr = INADDR_ANY; /* 填入本机IP地址 */
　　通过将my_addr.sin_port置为0，函数会自动为你选择一个未占用的端口来使用。同样，通过将my_addr.sin_addr.s_addr置为INADDR_ANY，系统会自动填入本机IP地址。
　　注意在使用bind函数时需要将sin_port和s_addr转换成为网络字节优先顺序；而sin_addr则不需要转换。
　　计算机数据存储有两种字节优先顺序：高位字节优先和低位字节优先。Internet上数据以高位字节优先顺序在网络上传输，所以对于在内部是以低位字节优先方式存储数据的机器，在Internet上传输数据时就需要进行转换，否则就会出现数据不一致。
　　 下面是几个字节顺序转换函数：
　　　　·htonl()：把32位值从主机字节序转换成网络字节序
　　　　·htons()：把16位值从主机字节序转换成网络字节序
　　　　·ntohl()：把32位值从网络字节序转换成主机字节序
　　　　·ntohs()：把16位值从网络字节序转换成主机字节序
bind()函数在成功被调用时返回0；出现错误时返回"-1"并将errno置为相应的错误号。需要注意的是，在调用bind函数时一般不要将端口号置为小于1024的值，因为1到1024是保留端口号，你可以选择大于1024中的任何一个没有被占用的端口号。
连接建立
　面向连接的客户程序使用connect函数来配置socket并与远端服务器建立一个TCP连接，其函数原型为：
　　 int connect(int sockfd, struct sockaddr *serv_addr,int addrlen);
　　sockfd是socket函数返回的socket描述符；serv_addr是包含远端主机IP地址和端口号的指针；addrlen是远端地址结构的长度。connect函数在出现错误时返回-1，并且设置errno为相应的错误码。进行客户端程序设计无须调用bind()，因为这种情况下只需知道目的机器的IP地址，而客户通过哪个端口与服务器建立连接并不需要关心，socket执行体为你的程序自动选择一个未被占用的端口，并通知你的程序数据什么时候到达端口。
　　 connect函数启动和远端主机的直接连接。只有面向连接的客户程序使用socket时才需要将此socket与远端主机相连。无连接协议从不建立直接连接。面向连接的服务器也从不启动一个连接，它只是被动的在协议端口监听客户的请求。
 listen函数使socket处于被动的监听模式，并为该socket建立一个输入数据队列，将到达的服务请求保存在此队列中，直到程序处理它们。
　　 int listen(int sockfd， int backlog);
　　 sockfd是Socket系统调用返回的socket 描述符；backlog指定在请求队列中允许的最大请求数，进入的连接请求将在队列中等待accept()它们（参考下文）。backlog对队列中等待服务的请求的数目进行了限制，大多数系统缺省值为20。如果一个服务请求到来时，输入队列已满，该socket将拒绝连接请求，客户将收到一个出错信息。
　　 当出现错误时listen函数返回-1，并置相应的errno错误码。
accept()函数让服务器接收客户的连接请求。在建立好输入队列后，服务器就调用accept函数，然后睡眠并等待客户的连接请求。
　　 int accept(int sockfd, void *addr, int *addrlen);
　　 sockfd是被监听的socket描述符，addr通常是一个指向sockaddr_in变量的指针，该变量用来存放提出连接请求服务的主机的信息（某台主机从某个端口发出该请求）；addrten通常为一个指向值为sizeof(struct sockaddr_in)的整型指针变量。出现错误时accept函数返回-1并置相应的errno值。
　　首先，当accept函数监视的socket收到连接请求时，socket执行体将建立一个新的socket，执行体将这个新socket和请求连接进程的地址联系起来，收到服务请求的初始socket仍可以继续在以前的 socket上监听，同时可以在新的socket描述符上进行数据传输操作。
数据传输
 send()和recv()这两个函数用于面向连接的socket上进行数据传输。
　　 send()函数原型为：
   　 int send(int sockfd, const void *msg, int len, int flags);
　　 sockfd是你想用来传输数据的socket描述符；msg是一个指向要发送数据的指针；len是以字节为单位的数据的长度；flags一般情况下置为0（关于该参数的用法可参照man手册）。
 send()函数返回实际上发送出的字节数，可能会少于你希望发送的数据。在程序中应该将send()的返回值与欲发送的字节数进行比较。当send()返回值与len不匹配时，应该对这种情况进行处理。
　　 　　 char *msg = "Hello!";
　　 　　 int len, bytes_sent;
　　 　　 ……
　　 　　 len = strlen(msg);
　　 　　 bytes_sent = send(sockfd, msg,len,0);
　　 　　 ……
recv()函数原型为：
　　 int recv(int sockfd,void *buf,int len,unsigned int flags);
　　 sockfd是接收数据的socket描述符；buf 是存放接收数据的缓冲区；len是缓冲的长度。flags也被置为0。recv()返回实际上接收的字节数，当出现错误时，返回-1并置相应的errno值。
sendto()和recvfrom()用于在无连接的数据报socket方式下进行数据传输。由于本地socket并没有与远端机器建立连接，所以在发送数据时应指明目的地址。
　　 sendto()函数原型为：
　　 int sendto(int sockfd, const void *msg,int len,unsigned int flags,const struct sockaddr *to, int tolen);
　　该函数比send()函数多了两个参数，to表示目地机的IP地址和端口号信息，而tolen常常被赋值为sizeof (struct sockaddr)。sendto 函数也返回实际发送的数据字节长度或在出现发送错误时返回-1。
　　 recvfrom()函数原型为：
　　 int recvfrom(int sockfd,void *buf,int len,unsigned int flags,struct sockaddr *from,int *fromlen);
　　 from是一个struct sockaddr类型的变量，该变量保存源机的IP地址及端口号。fromlen常置为sizeof (struct sockaddr)。当recvfrom()返回时，fromlen包含实际存入from中的数据字节数。recvfrom()函数返回接收到的字节数或当出现错误时返回-1，并置相应的errno。
　　 如果你对数据报socket调用了connect()函数时，你也可以利用send()和recv()进行数据传输，但该socket仍然是数据报socket，并且利用传输层的UDP服务。但在发送或接收数据报时，内核会自动为之加上目地和源地址信息。
结束传输
当所有的数据操作结束以后，你可以调用close()函数来释放该socket，从而停止在该socket上的任何数据操作：
　　 close(sockfd);
　　你也可以调用shutdown()函数来关闭该socket。该函数允许你只停止在某个方向上的数据传输，而一个方向上的数据传输继续进行。如你可以关闭某socket的写操作而允许继续在该socket上接受数据，直至读入所有数据。
　　 int shutdown(int sockfd,int how);
　　 sockfd是需要关闭的socket的描述符。参数 how允许为shutdown操作选择以下几种方式：
　　 ·0-------不允许继续接收数据
　　 ·1-------不允许继续发送数据
　　 ·2-------不允许继续发送和接收数据
　　 ·均为允许则调用close ()
　　 shutdown在操作成功时返回0，在出现错误时返回-1并置相应errno。
本篇文原文链接：[http://www.linuxidc.com/Linux/2011-06/37951.htm](http://www.linuxidc.com/Linux/2011-06/37951.htm)
