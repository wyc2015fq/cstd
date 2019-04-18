# socket编程原理 - ljx0305的专栏 - CSDN博客
2008年07月17日 13:59:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：597标签：[socket																[服务器																[网络																[unix																[pascal																[网络应用](https://so.csdn.net/so/search/s.do?q=网络应用&t=blog)](https://so.csdn.net/so/search/s.do?q=pascal&t=blog)](https://so.csdn.net/so/search/s.do?q=unix&t=blog)](https://so.csdn.net/so/search/s.do?q=网络&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=socket&t=blog)
个人分类：[网络编程](https://blog.csdn.net/ljx0305/article/category/403915)
                 socket编程原理  
1 问题的引入  
UNIX系统的I/O命令集，是从Maltics和早期系统中的命令演变出来的，其模式为打开一读/写一关闭（open-write-read-close）。在一个用户进程进行I/O操作时，它首先调用“打开”获得对指定文件或设备的使用权，并返回称为文件描述符的整型数，以描述用户在打开的文件或设备上进行I/O操作的进程。然后这个用户进程多次调用“读/写”以传输数据。当所有的传输操作完成后，用户进程关闭调用，通知操作系统已经完成了对某对象的使用。  
TCP/IP协议被集成到UNIX内核中时，相当于在UNIX系统引入了一种新型的I/O操作。UNIX用户进程与网络协议的交互作用比用户进程与传统的I/O设备相互作用复杂得多。首先，进行网络操作的两个进程钥纪纪同机器上，如何建立它们之间的联系？其次，网络协议存在多种，如何建立一种通用机制以支持多种协议？这些都是网络应用编程界面所要解决的问题。  
在UNIX系统中，网络应用编程界面有两类：UNIX BSD的套接字（socket）和UNIX System V的TLI。由于Sun公司采用了支持TCP/IP的UNIX BSD操作系统，使TCP/IP的应用有更大的发展，其网络应用编程界面──套接字（socket）在网络软件中被广泛应用，至今已引进微机操作系统DOS和Windows系统中，成为开发网络应用软件的强有力工具，本章将要详细讨论这个问题。  
2 套接字编程基本概念  
钥纪纪始使用套接字编程之前，首先必须建立以下概念。  
2.1 网间进程通信  
进程通信的概念最初来源于单机系统。由于每个进程都在自己的地址范围内运行，为保证两个相互通信的进程之间既互不干扰又协调一致工作，操作系统为进程通信提供了相应设施，如UNIX BSD中的管道（pipe）、命名管道（named pipe）和软中断信号（signal），UNIX system V的消息（message）、共享存储区（shared memory）和信号量（semaphore)等，但都仅限于用在本机进程之间通信。网间进程通信要解决的是不同主机进程间的相互通信问题（可把同机进程通信看作是其中的特例）。为此，首先要解决的是网间进程标识问题。同一主机上，不同进程可用进程号（process ID）唯一标识。但在网络环境下，各主机独立分配的进程号不能唯一标识该进程。例如，主机A赋于某进程号5，在B机中也可以存在5号进程，因此，“5号进程”这句话就没有意义了。  
其次，操作系统支持的网络协议众多，不同协议的工作方式不同，地址格式也不同。因此，网间进程通信还要解决多重协议的识别问题。  
为了解决上述问题，TCP/IP协议引入了下列几个概念。  
端口  
网络中可以被命名和寻址的通信端口，是操作系统可分配的一种资源。  
按照OSI七层协议的描述，传输层与网络层在功能上的最大区别是传输层提供进程通信能力。从这个意义上讲，网络通信的最终地址就不仅仅是主机地址了，还包括可以描述进程的某种标识符。为此，TCP/IP协议提出了协议端口（protocol port，简称端口）的概念，用于标识通信的进程。  
端口是一种抽象的软件结构（包括一些数据结构和I/O缓冲区）。应用程序（即进程）通过系统调用与某端口建立连接（binding）后，传输层传给该端口的数据都被相应进程所接收，相应进程发给传输层的数据都通过该端口输出。在TCP/IP协议的实现中，端靠纪纪作类似于一般的I/O操作，进程获取一个端口，相当于获取本地唯一的I/O文件，可以用一般的读写原语访问之。  
类似于文件描述符，每个端口都拥有一个叫端口号（port number）的整数型标识符，用于区别不同端口。由于TCP/IP传输层的两个协议TCP和UDP是完全独立的两个软件模块，因此各自的端口号也相互独立，如TCP有一个255号端口，UDP也可以有一个255号端口，二者并不冲突。  
端口号的分配是一个重要问题。有两种基本分配方式：第一种叫全局分配，这是一种集中控制方式，由一个公认的中央机构根据用户需要进行统一分配，并将结果公布于众。第二种是本地分配，又称动态连接，即进程需要访问传输层服务时，向本地操作系统提出申请，操作系统返回一个本地唯一的端口号，进程再通过合适的系统调用将自己与该端口号联系起来（绑扎）。TCP/IP端口号的分配中综合了上述两种方式。TCP/IP将端口号分为两部分，少量的作为保留端口，以全局方式分配给服务进程。因此，每一个标准服务器都拥有一个全局公认的端口（即周知口，well-known port），即使钥纪纪同机器上，其端口号也相同。剩余的为自由端口，以本地方式进行分配。TCP和UDP均规定，小于256的端口号才能作保留端口。  
地址  
网络通信中通信的两个进程分别钥纪纪同的机器上。在互连网络中，两台机器可能位涌纪纪同的网络，这些网络通过网络互连设备（网关，网桥，路由器等）连接。因此需要三级寻址：  
1. 某一主机可与多个网络相连，必须指定一特定网络地址；  
2. 网络上每一台主机应有其唯一的地址；  
3. 每一主机上的每一进程应有在该主机上的唯一标识符。  
通常主机地址由网络ID和主机ID组成，在TCP/IP协议中用32位整数值表示；TCP和UDP均使用16位端口号标识用户进程。  
网络字节顺序  
不同的计算机存放多字节值的顺序不同，有的机器在起始地址存放低位字节（低价先存），有的存高位字节（高价先存）。为保证数据的正确性，在网络协议中须指定网络字节顺序。TCP/IP协议使用16位整数和32位整数的高价先存格式，它们均含在协议头文件中。  
连接  
两个进程间的通信链路称为连接。连接在目纪纪表现为一些缓冲区和一组协议机制，在外部表现出比无连接高的可靠性。  
半相关  
综上所述，网络中用一个三元组可以在全局唯一标志一个进程：  
（协议，本地地址，本地端口号）  
这样一个三元组，叫做一个半相关（half-association），它指定连接的每半部分。  
全相关  
一个完整的网间进程通信需要由两个进程组成，并且只能使用同一种高层协议。也就是说，不可能通信的一端用TCP协议，而另一端用UDP协议。因此一个完整的网间通信需要一个五元组来标识：  
（协议，本地地址，本地端口号，远地地址，远地端口号）  
这样一个五元组，叫做一个相关（association），即两个协议相同的半相关才能组合成一个合适的相关，或完全指定组成一连接。  
2.2 服务方式  
在网络分层结构中，各层之间是严格单向依赖的，各层次的分工和协作集中体现在相量纪纪之间的界面上。“服务”是描述相量纪纪之间关系的抽象概念，即网络中各层向紧邻上层提供的一组操作。下层是服务提供者，上层是请求服务的用户。服务的表现形式是原语（primitive），如系统调用或库函数。系统调用是操作系统内核向网络应用程序或高层协议提供的服务原语。网络中的n层总要向n+1层提供比n-1层更完备的服务，否则n层就没有存在的价值。  
在OSI的术语中，网络层及其以下各层又称为通信子网，只提供点到点通信，没有程序或进程的概念。而传输层实现的是“端到端”通信，引进网间进程通信概念，同时也要解决差错控制，流量控制，数据排序（报文排序），连接管理等问题，为此提供不同的服务方式：  
面向连接（虚电路）或无连接  
面向连接服务是电话系统服务模式的抽象，即每一次完整的数据传输都要经过建立连接，使用连接，终止连接的过程。在数据传输过程中，各数据分组不携带目的地址，而使用连接号（connect ID）。本质上，连接是一个管道，收发数据不但顺序一致，而且内容相同。TCP协议提供面向连接的虚电路。  
无连接服务是邮政系统服务的抽象，每个分组都携带完整的目的地址，各分组在系统中独立传送。无连接服务不能保证分组的先后顺序，不进行分组出错的恢复与重传，不保证传输的可靠性。UDP协议提供无连接的数据报服务。  
下面给出这两种服务的类型及应用中的例子：  
服务类型  
服 务  
例 子  
面向连接  
可靠的报文流  
可靠的字节流  
不可靠的连接  
文件传输（FTP）  
远程登录（Telnet）  
数字话音  
无连接  
不可靠的数据报  
有确认的数据报  
请求－应答  
电子邮件（E-mail）  
电子邮件中的挂号信  
网络数据库查询  
顺序  
在网络传输中，两个连续报文在端－端通信中可能经过不同路径，这样到达目的地时的顺序可能会与发送时不同。“顺序”是指接收数据顺序与发送数据顺序相同。TCP协议提供这项服务。  
差错控制  
保证应用程序接收的数据无差错的一种机制。检查差错的方法一般是采用检验“检查和（Checksum）”的方法。而保证传送无差错的方法是双方采用确认应答技术。TCP协议提供这项服务。  
流控制  
在数据传输过程中控制数据传输速率的一种机制，以保证数据不被丢失。TCP协议提供这项服务。  
字节流  
字节流方式指的是仅把传输中的报文看作是一个字节序列，不提供数据流的任何边界。TCP协议提供字节流服务。  
报文  
接收方要保存发送方的报文边界。UDP协议提供报文服务。  
全双工/半双工  
端－端间数据同时以两个方向/一个方向传送。  
缓存/带外数据  
在字节流服务中，由于没有报文边界，用户进程在某一时刻可以读或写任意数量的字节。为保证传输正确或采用有流控制的协议时，都要进行缓存。但对某些特殊的需求，如交互式应用程序，又会要求取消这种缓存。  
在数据传送过程中，希望不通过常规传输方式传送给用户以便及时处理的某一类信息，如UNIX系统的中断键（Delete或Control-c）、终端流控制符（Control-s和Control-q），称为带外数据。逻辑上看，好象用户进程使用了一个独立的通道传输这些数据。该通道与每对连接的流相联系。由于Berkeley Software Distribution中对带外数据的实现与RFC 1122中规定的Host Agreement不一致,为了将互操作中的问题减到最小，应用程序编写者除非与现有服务互操作时要求带外数据外，最好不使用它。  
2.3 客户/服务器模式  
在TCP/IP网络应用中，通信的两个进程间相互作用的主要模式是客户/服务器模式（Client/Server model），即客户向服务器发出服务请求，服务器接收到请求后，提供相应的服务。客户/服务器模式的建立基于以下两点：首先，建立网络的起因是网络中软硬件资源、运算能力和信息不均等，需要共享，从而造就拥有众多资源的主机提供服务，资源较少的客户请求服务这一非对等作用。其次，网间进程通信完全是异步的，相互通信的进程间既不存在父子关系，又不共享内存缓冲区，因此需要一种机制为希望通信的进程间建立联系，为二者的数据交换提供同步，这就是基涌纪纪户/服务器模式的TCP/IP。  
客户/服务器模式钥纪纪作过程中采取的是主动请求方式：  
首先服务器方要先启动，并根据请求提供相应服务：  
1. 打开一通信通道并告知本地主机，它愿意在某一公认地址上（周知口，如FTP为21）接收客户请求；  
2. 等待客户请求到达该端口；  
3. 接收到重复服务请求，处理该请求并发送应答信号。接收到并发服务请求，要激活一新进程来处理这个客户请求（如UNIX系统中用fork、exec）。新进程处理此客户请求，并不需要对其它请求作出应答。服务完成后，关闭此新进程与客户的通信链路，并终止。  
4. 返回第二步，等待另一客户请求。  
5. 关闭服务器  
客户方：  
1. 打开一通信通道，并连接到服务器所在主机的特定端口；  
2. 向服务器发服务请求报文，等待并接收应答；继续提出请求......  
3. 请求结束后关闭通信通道并终止。  
从上面所描述过程可知：  
1. 客户与服务器进程的作用是非对称的，因此编码不同。  
2. 服务进程一般是先涌纪纪户请求而启动的。只要系统运行，该服务进程一直存在，直到正常或强迫终止。  
2.4 套接字类型  
TCP/IP的socket提供下列三种类型套接字。  
流式套接字（SOCK_STREAM）  
提供了一个面向连接、可靠的数据传输服务，数据无差错、无重复地发送，且按发送顺序接收。内设流量控制，避免数据流超限；数据被看作是字节流，无长度限制。文件传送协议（FTP）即使用流式套接字。  
数据报式套接字（SOCK_DGRAM）  
提供了一个无连接服务。数据包以独立包形式被发送，不提供无错保证， 
数据可能丢失或重复，并且接收顺序混乱。网络文件系统（NFS）使用数据报式套接字。  
原始式套接字（SOCK_RAW）  
该接口允许对较低层协议，如IP、ICMP直接访问。常用于检验新的协议实现或访问现有服务中配置的新设备。  
3 基本套接字系统调用  
为了更好地说明套接字编程原理，下面给出几个基本套接字系统调用说明。  
3.1 创建套接字──socket()  
应用程序在使用套接字前，首先必须拥有一个套接字，系统调用socket()向应用程序提供创建套接字的手段，其调用格式如下：  
SOCKET PASCAL FAR socket(int af, int type, int protocol);  
该调用要接收三个参数：af、type、protocol。参数af指定通信发生的区域，UNIX系统支持的地址族有：AF_UNIX、AF_INET、AF_NS等，而DOS、WINDOWS中仅支持AF_INET，它是网际网区域。因此，地址族与协议族相同。参数type 描述要建立的套接字的类型。参数protocol说明该套接字使用的特定协议，如果调用者不希望特别指定使用的协议，则置为0，使用默认的连接模式。根据这三个参数建立一个套接字，并将相应的资源分配给它，同时返回一个整型套接字号。因此，socket()系统调用实际上指定了相关五元组中的“协议”这一元。  
有关socket()的详细描述参看5.2.23。  
3.2 指定本地地址──bind()  
当一个套接字用socket()创建后，存在一个名字空间(地址族),但它没有被命名。bind()将套接字地址（包括本地主机地址和本地端口地址）与所创建的套接字号联系起来，即将名字赋予套接字，以指定本地半相关。其调用格式如下：  
int PASCAL FAR bind(SOCKET s, const struct sockaddr FAR * name, int namelen);  
参数s是由socket()调用返回的并且未作连接的套接字描述符(套接字号)。参数name 是赋给套接字s的本地地址（名字），其长度可变，结构随通信域的不同而不同。namelen表明了name的长度。  
如果没有错误发生，bind()返回0。否则返回值SOCKET_ERROR。  
地址在建立套接字通信过程中起着重要作用，作为一个网络应用程序设计者对套接字地址结构必须有明确认识。例如，UNIX BSD有一组描述套接字地址的数据结构，其中使用TCP/IP协议的地址结构为：  
struct sockaddr_in{  
short sin_family; /*AF_INET*/  
u_short sin_port; /*16位端口号，网络字节顺序*/  
struct in_addr sin_addr; /*32位IP地址，网络字节顺序*/  
char sin_zero[8]; /*保留*/  
}  
有关bind()的详细描述参看5.2.2。  
3.3 建立套接字连接──connect()与accept()  
这两个系统调用用于完成一个完整相关的建立，其中connect()用于建立连接。无连接的套接字进程也可以调用connect()，但这时在进程之间没有实际的报文交换，调用将从本地操作系统直接返回。这样做的优点是程序员不必为每一数据指定目的地址，而且如果收到的一个数据报，其目的端口未与任何套接字建立“连接”，便能判断该端靠纪纪可操作。而accept()用于使服务器等待来自某客户进程的实际连接。  
connect()的调用格式如下：  
int PASCAL FAR connect(SOCKET s, const struct sockaddr FAR * name, int namelen);  
参数s是欲建立连接的本地套接字描述符。参数name指出说明对方套接字地址结构的指针。对方套接字地址长度由namelen说明。  
如果没有错误发生，connect()返回0。否则返回值SOCKET_ERROR。在面向连接的协议中，该调用导致本地系统和外部系统之间连接实际建立。  
由于地址族总被包含在套接字地址结构的前两个字节中，并通过socket()调用与某个协议族相关。因此bind()和connect()无须协议作为参数。  
有关connect()的详细描述参看5.2.4。  
accept()的调用格式如下：  
SOCKET PASCAL FAR accept(SOCKET s, struct sockaddr FAR* addr, int FAR* addrlen);  
参数s为本地套接字描述符，在用做accept()调用的参数前应该先调用过listen()。addr 指向客户方套接字地址结构的指针，用来接收连接实体的地址。addr的确切格式由套接字创建时建立的地址族决定。addrlen 为客户方套接字地址的长度（字节数）。如果没有错误发生，accept()返回一个SOCKET类型的值，表示接收到的套接字的描述符。否则返回值INVALID_SOCKET。  
accept()用于面向连接服务器。参数addr和addrlen存放客户方的地址信息。调用前，参数addr 指向一个初始值为空的地址结构，而addrlen 的初始值为0；调用accept()后，服务器等待从编号为s的套接字上接受客户连接请求，而连接请求是由客户方的connect()调用发出的。当有连接请求到达时，accept()调用将请求连接队列上的第一个客户方套接字地址及长度放入addr 和addrlen，并创建一个与s有相同特性的新套接字号。新的套接字可用于处理服务器并发请求。  
有关accept()的详细描述参看5.2.1。  
四个套接字系统调用，socket()、bind()、connect()、accept()，可以完成一个完全五元相关的建立。socket()指定五元组中的协议元，它的用法与是否为客户或服务器、是否面向连接无关。bind()指定五元组中的本地二元，即本地主机地址和端口号，其用法与是否面向连接有关：在服务器方，无论是否面向连接，均要调用bind()；钥纪纪户方，若采用面向连接，则可以不调用bind()，而通过connect()自动完成。若采用无连接，客户方必须使用bind()以获得一个唯一的地址。  
以上讨论仅对客户/服务器模式而言，实际上套接字的使用是非常灵活的，唯一需遵循的原则是进程通信之前，必须建立完整的相关。  
3.4 监听连接──listen()  
此调用用于面向连接服务器，表明它愿意接收连接。listen()需在accept()之前调用，其调用格式如下：  
int PASCAL FAR listen(SOCKET s, int backlog);  
参数s标识一个本地已建立、尚未连接的套接字号，服务器愿意从它上面接收请求。backlog表示请求连接队列的最大长度，用于限制排队请求的个数，目前允许的最大值为5。如果没有错误发生，listen()返回0。否则它返回SOCKET_ERROR。  
listen()在执行调用过程中可为没有调用过bind()的套接字s完成所必须的连接，并建立长度为backlog的请求连接队列。  
调用listen()是服务器接收一个连接请求的四个步骤中的第三步。它在调用socket()分配一个流套接字，且调用bind()给s赋于一个名字之后调用，而且一定要在accept()之前调用。  
有关listen()的详细描述参看5.2.13。  
2.3节中提到钥纪纪户/服务器模式中，有两种类型的服务：重复服务和并发服务。accept()调用为实现并发服务提供了极大方便，因为它要返回一个新的套接字号，其典型结构为：  
int initsockid, newsockid;  
if ((initsockid = socket(....)) ;%s/n”, buf);  
}while (rval != 0);  
closesocket(msgsock);  
} while (TRUE);  
/* 因为这个程序已经有了一个无限循环，所以套接字“sock”从来不显式关闭。然而，当进程被杀死或正常终止时，所有套接字都将自动地被关闭。*/  
exit(0);  
}  
客户方程序：  
/* File Name: streamc.c */  
#include  
#include  
#define DATA “half a league, half a league ...”  
/* 这个程序建立套接字，然后与命令行给出的套接字连接；连接结束时，在连接上发送  
一个消息，然后关闭套接字。命令行的格式是：streamc 主机名 端口号  
端口号要与服务器程序的端口号相同 */  
main(argc, argv)  
int argc;  
char *argv[ ];  
{  
int sock;  
struct sockaddr_in server;  
struct hostent *hp, *gethostbyname( );  
char buf[1024];  
/* 建立套接字 */  
sock = socket(AF_INET, SOCK_STREAM, 0);  
if (sock ;h_addr, hp->;h_length);  
sever.sin_port = htons(atoi(argv[2]));  
if (connect(sock, (struct sockaddr*)&server, sizeof(server)) ;s_port; /* Service Port in Network Byte Order */  
return(1);  
}  
int InitPassiveSock(char * servicename)  
/* Initialize Passive Socket. If succeed then return 1, else return error code (;s_port; /* Service Port in Network Byte Order */  
if((mainsock = socket(AF_INET, SOCK_STREAM, 0)) ;s_port;  
if (bind(mainsock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) ;s_addr;  
/* Set this socket as a Non-blocking socket. */  
if (ioctl(tmpsock, FIONBIO, &flag) == -1) {  
close(tmpsock);  
return(-2);  
}  
/* Connect to the server. */  
if (connect(tmpsock, (struct sockaddr *)&server, sizeof(server)) ;= 64) {  
close(tmpsock);  
return(-4); /* too many connections */  
}  
Mysock.Sockets = tmpsock;  
Mysock.SockNum++;  
return(i);  
}  
int AcceptConnection(struct in_addr *IPaddr)  
/* Accept a connection. If succeed, return the data sockets number, else return -1. */  
{  
int newsock, len, flag=1, i;  
struct sockaddr_in addr;  
len = sizeof(addr);  
bzero((char *)&addr, len);  
if ((newsock = accept(Mysock.DaemonSock, &addr, &len)) == -1)  
return(-1); /* Accept error. */  
/* Set this socket as a Non-blocking socket. */  
ioctl(newsock, FIONBIO, &flag);  
FD_SET(newsock, &Mysock.readfds);  
FD_SET(newsock, &Mysock.writefds);  
FD_SET(newsock, &Mysock.exceptfds);  
/* Return IP address in the Parameter. */  
IPaddr->;s_addr = addr.sin_addr.s_addr;  
i = 0;  
while (Mysock.Sockets != 0) i++; /* look for a blank sockets position */  
if (i >;= 64) {  
close(newsock);  
return(-4); /* too many connections */  
}  
Mysock.Sockets = newsock;  
Mysock.SockNum++;  
return(i);  
}  
int CloseConnection(int Sockno)  
/* Close a connection indicated by Sockno. */  
{  
int retcode;  
if ((Sockno >;= 64) || (Sockno ; 0) && (FD_ISSET(Mysock.Sockets, &rfds)))  
SockMsg.ReadQueue[SockMsg.ReadNum++] = i;  
}  
for (i=0; i; 0) && (FD_ISSET(Mysock.Sockets, &wfds)))  
SockMsg.WriteQueue[SockMsg.WriteNum++] = i;  
}  
if (FD_ISSET(Mysock.DaemonSock, &efds))  
SockMsg.AcceptNum = -1; /* server socket error. */  
for (i=0; i; 0) && (FD_ISSET(Mysock.Sockets, &efds)))  
SockMsg.ExceptQueue[SockMsg.ExceptNum++] = i;  
}  
return(retcode);  
}  
int SendPacket(int Sockno, void *buf, int len)  
/* Send a packet. If succeed return the number of send data, else return -1 */  
{  
int actlen;  
if ((Sockno >;= 64) || (Sockno ;= 64) || (Sockno ; 0)  
printf("sockno %d Recv string = %s /n", SockMsg.ReadQueue, buf);  
else /* 返回数据长度为零，指示连接中断，关闭套接字。*/  
CloseConnection(SockMsg.ReadQueue);  
}  
} /* end while */  
}  
2.5.4 简单客户程序示例  
/* File Name: client.c */  
/* 客户程序在执行时，先初始化数据结构，然后等待用户输入命令。它识别四个命令：  
conn(ect)： 和服务器建立连接；  
send： 给指定连接发送数据；  
clos(e)： 关闭指定连接；  
quit： 退出客户程序。  
*/  
#include "tcpsock.h"  
main(argc, argv)  
int argc;  
char **argv;  
{  
char cmd_buf[16];  
struct in_addr sin_addr;  
int sockno1, retcode;  
char *buf = "This is a string for test.";  
sin_addr.s_addr = inet_addr("166.111.5.249"); /* 运行服务器程序的主机的IP地址 */  
if ((retcode = InitSocketsStruct("TestService")) ;");  
gets(cmd_buf);  
if (!strncmp(cmd_buf, "conn", 4)) {  
retcode = CreateConnection(&sin_addr); /* 建立连接 */  
printf("return code: %d/n", retcode);  
}  
else if(!strncmp(cmd_buf, "send", 4)) {  
printf("Sockets Number:");  
scanf("%d", &sockno1);  
retcode = SendPacket(sockno1, buf, 26); /* 发送数据 */  
printf("return code: %d/n", retcode, sizeof(buf));  
}  
else if (!strncmp(cmd_buf, "close", 4)) {  
printf("Sockets Number:");  
scanf("%d", &sockno1);  
retcode = CloseConnection(sockno1); /* 关闭连接 */  
printf("return code: %d/n", retcode);  
}  
else if (!strncmp(cmd_buf, "quit", 4))  
exit(0);  
else  
putchar('/007');  
} /* end while */  
}  
