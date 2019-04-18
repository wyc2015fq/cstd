# accept函数  - ljx0305的专栏 - CSDN博客
2010年02月04日 22:42:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：567标签：[socket																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Linux																[网络编程](https://blog.csdn.net/ljx0305/article/category/403915)](https://blog.csdn.net/ljx0305/article/category/364133)
accept函数
SOCKET accept( 
SOCKET s, 
struct sockaddr FAR *addr, 
int FAR *addrlen 
); 
服务程序调用accept函数从处于监听状态的流套接字s的客户连接请求队列中取出排在最前的一个客户请求，并且创建一个新的套接字来与客户套接字创建连接通道，如果连接成功，就返回新创建的套接字的描述符，以后与客户套接字交换数据的是新创建的套接字；如果失败就返回 INVALID_SOCKET。该函数的第一个参数指定处于监听状态的流套接字；操作系统利用第二个参数来返回新创建的套接字的地址结构；操作系统利用第三个参数来返回新创建的套接字的地址结构的长度。下面是一个调用accept的例子： 
struct sockaddr_in ServerSocketAddr; 
int addrlen; 
addrlen=sizeof(ServerSocketAddr); 
ServerSocket=accept(ListenSocket,(struct sockaddr *)&ServerSocketAddr,&addrlen); SOCKET accept( 
SOCKET s, 
struct sockaddr FAR *addr, 
int FAR *addrlen 
); 
服务程序调用accept函数从处于监听状态的流套接字s的客户连接请求队列中取出排在最前的一个客户请求，并且创建一个新的套接字来与客户套接字创建连接通道，如果连接成功，就返回新创建的套接字的描述符，以后与客户套接字交换数据的是新创建的套接字；如果失败就返回 INVALID_SOCKET。该函数的第一个参数指定处于监听状态的流套接字；操作系统利用第二个参数来返回新创建的套接字的地址结构；操作系统利用第三个参数来返回新创建的套接字的地址结构的长度。下面是一个调用accept的例子： 
struct sockaddr_in ServerSocketAddr; 
int addrlen; 
addrlen=sizeof(ServerSocketAddr); 
ServerSocket=accept(ListenSocket,(struct sockaddr *)&ServerSocketAddr,&addrlen);  
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/ljw787/archive/2007/10/11/1820414.aspx](http://blog.csdn.net/ljw787/archive/2007/10/11/1820414.aspx)
