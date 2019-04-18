# Linux 下Socket编程基础 - ljx0305的专栏 - CSDN博客
2008年07月17日 13:50:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：550标签：[socket																[linux																[tcp																[服务器																[internet																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=internet&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=tcp&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[Linux程序及使用](https://blog.csdn.net/ljx0305/article/category/394700)
                 Linux的兴起可以说是Internet创造的一个奇迹。Linux作为一个完全开放其原代码的免费的自由软件，兼容了各种UNIX标准（如POSIX、UNIX System V 和 BSD UNIX 等）的多用户、多任务的具有复杂内核的操作系统。在中国，随着Internet的普及，一批主要以高等院校的学生和ISP的技术人员组成的Linux爱好者队伍已经蓬勃成长起来。越来越多的编程爱好者也逐渐酷爱上这个优秀的自由软件。本文介绍了Linux下Socket的基本概念和函数调用。
2、 什么是Socket
Socket（套接字）是通过标准的UNIX文件描述符和其它程序通讯的一个方法。每一个套接字都用一个半相关描述：{协议，本地地址、本地端口}来表示；一个完整的套接字则用一个相关描述：{协议，本地地址、本地端口、远程地址、远程端口}，每一个套接字都有一个本地的由操作系统分配的唯一的套接字号。
3、 Socket的三种类型
（1） 流式Socket（SOCK_STREAM）
流式套接字提供可靠的、面向连接的通信流；它使用TCP协议，从而保证了数据传输的正确性和顺序的。
（2） 数据报Socket（SOCK_DGRAM）
数据报套接字定义了一种无连接的服务，数据通过相互独立的报文进行传输，是无序的，并且不保证可靠、无差错。它使用数据报协议UDP
（3） 原始Socket
原始套接字允许对底层协议如IP或ICMP直接访问，它功能强大但使用较为不便，主要用于一些协议的开发。
4、 利用套接字发送数据
1、 对于流式套接字用系统调用send（）来发送数据。
2、 对于数据报套接字，则需要自己先加一个信息头，然后调用sendto（）函数把数据发送出去。
5、 Linux中Socket的数据结构
（1） struct sockaddr { //用于存储套接字地址
unsigned short sa_family；//地址类型
char sa_data[14]； //14字节的协议地址
}；
（2） struct sockaddr_in{ //in 代表internet
short int sin_family； //internet协议族
unsigned short int sin_port；//端口号，必须是网络字节顺序
struct in_addr sin_addr；//internet地址，必须是网络字节顺序
unsigned char sin_zero；//添0（和struct sockaddr一样大小
}；
（3） struct in_addr{
unsigned long s_addr；
}；
6、 网络字节顺序及其转换函数
（1） 网络字节顺序
每一台机器内部对变量的字节存储顺序不同，而网络传输的数据是一定要统一顺序的。所以对内部字节表示顺序与网络字节顺序不同的机器，一定要对数据进行转换，从程序的可移植性要求来讲，就算本机的内部字节表示顺序与网络字节顺序相同也应该在传输数据以前先调用数据转换函数，以便程序移植到其它机器上后能正确执行。真正转换还是不转换是由系统函数自己来决定的。
（2） 有关的转换函数
* unsigned short int htons（unsigned short int hostshort）：
主机字节顺序转换成网络字节顺序，对无符号短型进行操作4bytes
* unsigned long int htonl（unsigned long int hostlong）：
主机字节顺序转换成网络字节顺序，对无符号长型进行操作8bytes
* unsigned short int ntohs（unsigned short int netshort）：
网络字节顺序转换成主机字节顺序，对无符号短型进行操作4bytes
* unsigned long int ntohl（unsigned long int netlong）：
网络字节顺序转换成主机字节顺序，对无符号长型进行操作8bytes
注：以上函数原型定义在netinet/in.h里
7、 IP地址转换
有三个函数将数字点形式表示的字符串IP地址与32位网络字节顺序的二进制形式的IP地址进行转换
（1） unsigned long int inet_addr(const char * cp)：该函数把一个用数字和点表示的IP地址的字符串转换成一个无符号长整型，如：struct sockaddr_in ina
ina.sin_addr.s_addr=inet_addr("202.206.17.101")
该函数成功时：返回转换结果；失败时返回常量INADDR_NONE，该常量=-1，二进制的无符号整数-1相当于255.255.255.255，这是一个广播地址，所以在程序中调用iner_addr（）时，一定要人为地对调用失败进行处理。由于该函数不能处理广播地址，所以在程序中应该使用函数inet_aton（）。
（2）int inet_aton（const char * cp,struct in_addr * inp）：此函数将字符串形式的IP地址转换成二进制形式的IP地址；成功时返回1，否则返回0，转换后的IP地址存储在参数inp中。
（3） char * inet_ntoa（struct in-addr in）：将32位二进制形式的IP地址转换为数字点形式的IP地址，结果在函数返回值中返回，返回的是一个指向字符串的指针。
8、 字节处理函数
Socket地址是多字节数据，不是以空字符结尾的，这和C语言中的字符串是不同的。Linux提供了两组函数来处理多字节数据，一组以b（byte）开头，是和BSD系统兼容的函数，另一组以mem（内存）开头，是ANSI C提供的函数。
以b开头的函数有：
（1） void bzero（void * s,int n）：将参数s指定的内存的前n个字节设置为0，通常它用来将套接字地址清0。
（2） void bcopy（const void * src，void * dest，int n）：从参数src指定的内存区域拷贝指定数目的字节内容到参数dest指定的内存区域。
（3） int bcmp（const void * s1，const void * s2，int n）：比较参数s1指定的内存区域和参数s2指定的内存区域的前n个字节内容，如果相同则返回0，否则返回非0。
注：以上函数的原型定义在strings.h中。
以mem开头的函数有：
（1） void * memset（void * s，int c，size_t n）：将参数s指定的内存区域的前n个字节设置为参数c的内容。
（2） void * memcpy（void * dest，const void * src，size_t n）：功能同bcopy（），区别：函数bcopy（）能处理参数src和参数dest所指定的区域有重叠的情况，memcpy（）则不能。
（4） int memcmp（const void * s1，const void * s2，size_t n）：比较参数s1和参数s2指定区域的前n个字节内容，如果相同则返回0，否则返回非0。
注：以上函数的原型定义在string.h中。
9、 基本套接字函数
（1） socket（）
#include<sys/types.h>
#include<sys/socket.h>
int socket(int domain，int type，int protocol)
参数domain指定要创建的套接字的协议族，可以是如下值：
AF_UNIX //UNIX域协议族，本机的进程间通讯时使用
AF_INET //Internet协议族（TCP/IP）
AF_ISO //ISO协议族
参数type指定套接字类型，可以是如下值：
SOCK_STREAM //流套接字，面向连接的和可靠的通信类型
SOCK_DGRAM //数据报套接字，非面向连接的和不可靠的通信类型
SOCK_RAW //原始套接字，只对Internet协议有效，可以用来直接访问IP协议
参数protocol通常设置成0，表示使用默认协议，如Internet协议族的流套接字使用TCP协议，而数据报套接字使用UDP协议。当套接字是原始套接字类型时，需要指定参数protocol，因为原始套接字对多种协议有效，如ICMP和IGMP等。
Linux系统中创建一个套接字的操作主要是：在内核中创建一个套接字数据结构，然后返回一个套接字描述符标识这个套接字数据结构。这个套接字数据结构包含连接的各种信息，如对方地址、TCP状态以及发送和接收缓冲区等等，TCP协议根据这个套接字数据结构的内容来控制这条连接。
（2） 函数connect（）
#include<sys/types.h>
#include<sys/socket.h>
int connect（int sockfd，struct sockaddr * servaddr，int addrlen）
参数sockfd是函数socket返回的套接字描述符；参数servaddr指定远程服务器的套接字地址，包括服务器的IP地址和端口号；参数addrlen指定这个套接字地址的长度。成功时返回0，否则返回-1，并设置全局变量为以下任何一种错误类型：ETIMEOUT、ECONNREFUSED、EHOSTUNREACH或ENETUNREACH。
在调用函数connect之前，客户机需要指定服务器进程的套接字地址。客户机一般不需要指定自己的套接字地址（IP地址和端口号），系统会自动从1024至5000的端口号范围内为它选择一个未用的端口号，然后以这个端口号和本机的IP地址填充这个套接字地址。
客户机调用函数connect来主动建立连接。这个函数将启动TCP协议的3次握手过程。在建立连接之后或发生错误时函数返回。连接过程可能出现的错误情况有：
（1） 如果客户机TCP协议没有接收到对它的SYN数据段的确认，函数以错误返回，错误类型为ETIMEOUT。通常TCP协议在发送SYN数据段失败之后，会多次发送SYN数据段，在所有的发送都高中失败之后，函数以错误返回。
注：SYN（synchronize）位：请求连接。TCP用这种数据段向对方TCP协议请求建立连接。在这个数据段中，TCP协议将它选择的初始序列号通知对方，并且与对方协议协商最大数据段大小。SYN数据段的序列号为初始序列号，这个SYN数据段能够被确认。当协议接收到对这个数据段的确认之后，建立TCP连接。
（2） 如果远程TCP协议返回一个RST数据段，函数立即以错误返回，错误类型为ECONNREFUSED。当远程机器在SYN数据段指定的目的端口号处没有服务进程在等待连接时，远程机器的TCP协议将发送一个RST数据段，向客户机报告这个错误。客户机的TCP协议在接收到RST数据段后不再继续发送SYN数据段，函数立即以错误返回。
注：RST（reset）位：表示请求重置连接。当TCP协议接收到一个不能处理的数据段时，向对方TCP协议发送这种数据段，表示这个数据段所标识的连接出现了某种错误，请求TCP协议将这个连接清除。有3种情况可能导致TCP协议发送RST数据段：（1）SYN数据段指定的目的端口处没有接收进程在等待；（2）TCP协议想放弃一个已经存在的连接；（3）TCP接收到一个数据段，但是这个数据段所标识的连接不存在。接收到RST数据段的TCP协议立即将这条连接非正常地断开，并向应用程序报告错误。
（3） 如果客户机的SYN数据段导致某个路由器产生“目的地不可到达”类型的ICMP消息，函数以错误返回，错误类型为EHOSTUNREACH或ENETUNREACH。通常TCP协议在接收到这个ICMP消息之后，记录这个消息，然后继续几次发送SYN数据段，在所有的发送都告失败之后，TCP协议检查这个ICMP消息，函数以错误返回。
注：ICMP：Internet 消息控制协议。Internet的运行主要是由Internet的路由器来控制，路由器完成IP数据包的发送和接收，如果发送数据包时发生错误，路由器使用ICMP协议来报告这些错误。ICMP数据包是封装在IP数据包的数据部分中进行传输的，其格式如下：
类型
码 
校验和
数据
0 8 16 24 31
类型：指出ICMP数据包的类型。
代码：提供ICMP数据包的进一步信息。
校验和：提供了对整个ICMP数据包内容的校验和。
ICMP数据包主要有以下类型：
（1） 目的地不可到达：A、目的主机未运行；B、目的地址不存在；C、路由表中没有目的地址对应的条目，因而路由器无法找到去往目的主机的路由。
（2） 超时：路由器将接收到的IP数据包的生存时间（TTL）域减1，如果这个域的值变为0，路由器丢弃这个IP数据包，并且发送这种ICMP消息。
（3） 参数出错：当IP数据包中有无效域时发送。
（4） 重定向：将一条新的路径通知主机。
（5） ECHO请求、ECHO回答：这两条消息用语测试目的主机是否可以到达。请求者向目的主机发送ECHO请求ICMP数据包，目的主机在接收到这个ICMP数据包之后，返回ECHO回答ICMP数据包。
（6） 时戳请求、时戳回答：ICMP协议使用这两种消息从其他机器处获得其时钟的当前时间。
调用函数connect的过程中，当客户机TCP协议发送了SYN数据段的确认之后，TCP状态由CLOSED状态转为SYN_SENT状态，在接收到对SYN数据段的确认之后，TCP状态转换成ESTABLISHED状态，函数成功返回。如果调用函数connect失败，应该用close关闭这个套接字描述符，不能再次使用这个套接字描述符来调用函数connect。
注：TCP协议状态转换图：
被动OPEN CLOSE 主动OPEN
（建立TCB） （删除TCB） （建立TCB，
发送SYN）
接收SYN SEND
（发送SYN，ACK） （发送SYN）
接收SYN的ACK（无动作） 
接收SYN的ACK 接收SYN，ACK 
（无动作） （发送ACK）
CLOSE
（发送FIN） CLOSE 接收FIN
（发送FIN） （发送FIN）
接收FIN
接收FIN的ACK（无动作） （发送ACK） CLOSE（发送FIN）
接收FIN 接收FIN的ACK 接收FIN的ACK
（发送ACK） （无动作） （无动作）
2MSL超时（删除TCB） 
（3） 函数bind（）
函数bind将本地地址与套接字绑定在一起，其定义如下：
#include<sys/types.h>
#include<sys/socket.h>
int bind(int sockfd,struct sockaddr * myaddr,int addrlen)；
参数sockfd是函数sockt返回的套接字描述符；参数myaddr是本地地址；参数addrlen是套接字地址结构的长度。执行成功时返回0，否则，返回-1，并设置全局变量errno为错误类型EADDRINUSER。
服务器和客户机都可以调用函数bind来绑定套接字地址，但一般是服务器调用函数bind来绑定自己的公认端口号。绑定操作一般有如下几种组合方式：
表1
程序类型
IP地址
端口号
说明
服务器
INADDR_ANY
非零值
指定服务器的公认端口号
服务器
本地IP地址
非零值
指定服务器的IP地址和公认端口号
客户机
INADDR_ANY
非零值
指定客户机的连接端口号
客户机
本地IP地址
非零值
指定客户机的IP地址连接端口号
客户机
本地IP地址
零
指定客户机的IP地址
分别说明如下：
（1） 服务器指定套接字地址的公认端口号，不指定IP地址：即服务器调用bind时，设置套接字的IP地址为特殊的INADDE-ANY，表示它愿意接收来自任何网络设备接口的客户机连接。这是服务器最常用的绑定方式。
（2） 服务器指定套接字地址的公认端口号和IP地址：服务器调用bind时，如果设置套接字的IP地址为某个本地IP地址，这表示这台机器只接收来自对应于这个IP地址的特定网络设备接口的客户机连接。当服务器有多块网卡时，可以用这种方式来限制服务器的接收范围。
（3） 客户机指定套接字地址的连接端口号：一般情况下，客户机调用connect函数时不用指定自己的套接字地址的端口号。系统会自动为它选择一个未用的端口号，并且用本地的IP地址来填充套接字地址中的相应项。但有时客户机需要使用一个特定的端口号（比如保留端口号），而系统不会未客户机自动分配一个保留端口号，所以需要调用函数bind来和一个未用的保留端口号绑定。
（4） 指定客户机的IP地址和连接端口号：表示客户机使用指定的网络设备接口和端口号进行通信。
（5） 指定客户机的IP地址：表示客户机使用指定的网络设备接口和端口号进行通信，系统自动为客户机选一个未用的端口号。一般只有在主机有多个网络设备接口时使用。
我们一般不在客户机上使用固定的客户机端口号，除非是必须使用的情况。在客户机上使用固定的端口号有以下不利：
（1） 服务器执行主动关闭操作：服务器最后进入TIME_WAIT状态。当客户机再次与这个服务器进行连接时，仍使用相同的客户机端口号，于是这个连接与前次连接的套接字对完全一样，但是一呢、为前次连接处于TIME_WAIT状态，并未消失，所以这次连接请求被拒绝，函connect以错误返回，错误类型为ECONNREFUSED
（2） 客户机执行主动关闭操作：客户机最后进入TIME_WAIT状态。当马上再次执行这个客户机程序时，客户机将继续与这个固定客户机端口号绑定，但因为前次连接处于TIME_WAIT状态，并未消失，系统会发现这个端口号仍被占用，所以这次绑定操作失败，函数bind以错误返回，错误类型为EADDRINUSE。
（4） 函数listen（）
函数listen将一个套接字转换为征听套接字，定义如下；
#include<sys/socket,h>
int listen(int sockfd，int backlog)
参数sockfd指定要转换的套接字描述符；参数backlog设置请求队列的最大长度；执行成功时返回0， 否则返回-1。函数listen功能有两个：
（1） 将一个尚未连接的主动套接字（函数socket创建的可以用来进行主动连接但不能接受连接请求的套接字）转换成一个被动连接套接字。执行listen之后，服务器的TCP状态由CLOSED转为LISTEN状态。
（2） TCP协议将到达的连接请求队列，函数listen的第二个参数指定这个队列的最大长度。
注：参数backlog的作用：
TCP协议为每一个征听套接字维护两个队列：
（1） 未完成连接队列：每个尚未完成3次握手操作的TCP连接在这个队列中占有一项。TCP希望仪在接收到一个客户机SYN数据段之后，在这个队列中创建一个新条目，然后发送对客户机SYN数据段的确认和自己的SYN数据段（ACK+SYN数据段），等待客户机对自己的SYN数据段的确认。此时，套接字处于SYN_RCVD状态。这个条目将保存在这个队列中，直到客户机返回对SYN数据段的确认或者连接超时。
（2） 完成连接队列：每个已经完成3次握手操作，但尚未被应用程序接收（调用函数accept）的TCP连接在这个队列中占有一项。当一个在未完成连接队列中的连接接收到对SYN数据段的确认之后，完成3次握手操作，TCP协议将它从未完成连接队列移到完成连接队列中。此时，套接字处于ESTABLISHED状态。这个条目将保存在这个队列中，直到应用程序调用函数accept来接收它。
参数backlog指定某个征听套接字的完成连接队列的最大长度，表示这个套接字能够接收的最大数目的未接收连接。如果当一个客户机的SYN数据段到达时，征听套接字的完成队列已经满了，那么TCP协议将忽略这个SYN数据段。对于不能接收的SYN数据段，TCP协议不发送RST数据段，
（5） 函数accept（）
函数accept从征听套接字的完成队列中接收一个已经建立起来的TCP连接。如果完成连接队列为空，那么这个进程睡眠。
#include<sys/socket.h>
int accept(int sockfd，struct sockaddr * addr，int * addrlen)
参数sockfd指定征听套接字描述符；参数addr为指向一个Internet套接字地址结构的指针；参数addrlen为指向一个整型变量的指针。执行成功时，返回3个结果：函数返回值为一个新的套接字描述符，标识这个接收的连接；参数addr指向的结构变量中存储客户机地址；参数addrlen指向的整型变量中存储客户机地址的长度。失败时返回-1。
征听套接字专为接收客户机连接请求，完成3次握手操作而用的，所以TCP协议不能使用征听套接字描述符来标识这个连接，于是TCP协议创建一个新的套接字来标识这个要接收的连接，并将它的描述符发挥给应用程序。现在有两个套接字，一个是调用函数accept时使用的征听套接字，另一个是函数accept返回的连接套接字（connected socket）。一个服务器通常只需创建一个征听套接字，在服务器进程的整个活动期间，用它来接收所有客户机的连接请求，在服务器进程终止前关闭这个征听套接字；对于没一个接收的（accepted）连接，TCP协议都创建一个新的连接套接字来标识这个连接，服务器使用这个连接套接字与客户机进行通信操作，当服务器处理完这个客户机请求时，关闭这个连接套接字。
当函数accept阻塞等待已经建立的连接时，如果进程捕获到信号，函数将以错误返回，错误类型为EINTR。对于这种错误，一般重新调用函数accept来接收连接。
（6） 函数close（）
函数close关闭一个套接字描述符。定义如下：
#include<unistd.h>
int close(int sockfd)；
执行成功时返回0，否则返回-1。与操作文件描述符的close一样，函数close将套接字描述符的引用计数器减1，如果描述符的引用计数大于0，则表示还有进程引用这个描述符，函数close正常返回；如果为0，则启动清除套接字描述符的操作，函数close立即正常返回。
调用close之后，进程将不再能够访问这个套接字，但TCP协议将继续使用这个套接字，将尚未发送的数据传递到对方，然后发送FIN数据段，执行关闭操作，一直等到这个TCP连接完全关闭之后，TCP协议才删除该套接字。
（7） 函数read（）和write（）
用于从套接字读写数据。定义如下：
int read(int fd，char * buf，int len)
int write(int fd，char * buf，int len)
函数执行成功时，返回读或写的数据量的大小，失败时返回-1。
每个TCP套接字都有两个缓冲区：套接字发送缓冲区、套接字接收缓冲区，分别处理发送和接收任务。从网络读、写数据的操作是由TCP协议在内核中完成的：TCP协议将从网络上接收到的数据保存在相应套接字的接收缓冲区中，等待用户调用函数将它们从接收缓冲区拷贝到用户缓冲区；用户将要发送的数据拷贝到相应套接字的发送缓冲区中，然后由TCP协议按照一定的算法处理这些数据。
读写连接套接字的操作与读写文件的操作类似，也可以使用函数read和write。函数read完成将数据从套接字接收缓冲区拷贝到用户缓冲区：当套接字接收缓冲区有数据可读时，1：可读数据量大于函数read指定值，返回函数参数len指定的数据量；2：了度数据量小于函数read指定值，函数read不等待请求的所有数据都到达，而是立即返回实际读到的数据量；当无数据可读时，函数read将阻塞不返回，等待数据到达。
当TCP协议接收到FIN数据段，相当于给读操作一个文件结束符，此时read函数返回0，并且以后所有在这个套接字上的读操作均返回0，这和普通文件中遇到文件结束符是一样的。
当TCP协议接收到RST数据段，表示连接出现了某种错误，函数read将以错误返回，错误类型为ECONNERESET。并且以后所有在这个套接字上的读操作均返回错误。错误返回时返回值小于0。
函数write完成将数据从用户缓冲区拷贝到套接字发送缓冲区的任务：到套接字发送缓冲区有足够拷贝所有用户数据的空间时，函数write将数据拷贝到这个缓冲区中，并返回老辈的数量大小，如果可用空间小于write参数len指定的大小时，函数write将阻塞不返回，等待缓冲区有足够的空间。
当TCP协议接收到RST数据段（当对方已经关闭了这条连接之后，继续向这个套接字发送数据将导致对方TCP协议返回RST数据段），TCP协议接收到RST数据段时，函数write将以错误返回，错误类型为EINTR。以后可以继续在这个套接字上写数据。
（8） 函数getsockname（）和getpeername（）
函数getsockname返回套接字的本地地址；函数getpeername返回套接字对应的远程地址。
10、 结束语
网络程序设计全靠套接字接收和发送信息。上文主要讲述了Linux 下Socket的基本概念、Sockets API以及Socket所涉及到的TCP常识。            
