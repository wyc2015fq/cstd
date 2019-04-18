# 关于SIGPIPE导致的程序退出 - ljx0305的专栏 - CSDN博客
2013年07月03日 15:11:02[ljx0305](https://me.csdn.net/ljx0305)阅读数：3046
## [关于SIGPIPE导致的程序退出](http://www.cppblog.com/elva/archive/2008/09/10/61544.html)
收集一些网上的资料，以便参考：
[http://blog.chinaunix.net/u2/69143/showart_1087349.html](http://blog.chinaunix.net/u2/69143/showart_1087349.html)
     当服务器close一个连接时，若client端接着发数据。根据TCP协议的规定，会收到一个RST响应，client再往这个服务器发送数据时，系统会发出一个SIGPIPE信号给进程，告诉进程这个连接已经断开了，不要再写了。
    根据信号的默认处理规则SIGPIPE信号的默认执行动作是terminate(终止、退出),所以client会退出。若不想客户端退出可以把SIGPIPE设为SIG_IGN
    如:    signal(SIGPIPE,SIG_IGN);
    这时SIGPIPE交给了系统处理。
  服务器采用了fork的话，要收集垃圾进程，防止僵尸进程的产生，可以这样处理：
  signal(SIGCHLD,SIG_IGN);　交给系统init去回收。
   这里子进程就不会产生僵尸进程了。
http://www.cublog.cn/u/31357/showart_242605.html
好久没做过C开发了，最近重操旧业。
听说另外一个项目组socket开发遇到问题，发送端和接受端数据大小不一致。建议他们采用writen的重发机制，以避免信号中断错误。采用后还是有问题。PM让我帮忙研究下。
UNP n年以前看过，很久没做过底层开发，手边也没有UNP vol1这本书，所以做了个测试程序，研究下实际可能发生的情况了。
测试环境：AS3和redhat 9(缺省没有nc)
先下载unp源码:
wget http://www.unpbook.com/unpv13e.tar.gz
tar xzvf *.tar.gz;
configure;make lib.
然后参考str_cli.c和tcpcli01.c，写了测试代码client.c
```
#include    "unp.h"
#define MAXBUF 40960
void processSignal(int signo)
{
    printf("Signal is %d\n", signo);
    signal(signo, processSignal);
}
void
str_cli(FILE
*fp, 
int sockfd)
{
    char    sendline[MAXBUF], recvline[MAXBUF];
    while (1)
{
        memset(sendline,
'a', 
sizeof(sendline));
        printf("Begin send %d data\n", MAXBUF);
        Writen(sockfd, sendline,
sizeof(sendline));
        sleep(5);
    }
}
int
main(int argc,
char **argv)
{
    int                    sockfd;
    struct sockaddr_in    servaddr;
    signal(SIGPIPE,
SIG_IGN);
    //signal(SIGPIPE, processSignal);
    if (argc
!= 2)
        err_quit("usage: tcpcli [port]");
    sockfd = Socket(AF_INET,
SOCK_STREAM, 0);
    bzero(&servaddr,
sizeof(servaddr));
    servaddr.sin_family 
= AF_INET;
    servaddr.sin_port 
= htons(atoi(argv[1]));
    Inet_pton(AF_INET,
"127.0.0.1",
&servaddr.sin_addr);
    Connect(sockfd,
(SA *)
&servaddr, 
sizeof(servaddr));
    str_cli(stdin, sockfd);        /* do it all */
    exit(0);
}
```
为了方便观察错误输出，lib/writen.c也做了修改，加了些日志：
```
/* include writen */
#include    "unp.h"
ssize_t                        /* Write "n" bytes to a descriptor. */
writen(int fd,
const void 
*vptr, size_t n)
{
    size_t        nleft;
    ssize_t        nwritten;
    const char    *ptr;
    ptr = vptr;
    nleft = n;
    while (nleft
> 0) 
{
        printf("Begin Writen %d\n", nleft);
        if (
(nwritten =
write(fd, ptr, nleft))
<= 0)
{
            if (nwritten
< 0 &&
errno == EINTR)
{
                printf("intterupt\n");
                nwritten = 0;        /* and call write() again */
            }
            else
                return(-1);            /* error */
        }
        nleft -= nwritten;
        ptr += nwritten;
        printf("Already write %d, left %d, errno=%d\n", nwritten, nleft,
errno);
    }
    return(n);
}
/* end writen */
void
Writen(int fd,
void *ptr,
size_t nbytes)
{
    if (writen(fd, ptr, nbytes)
!= nbytes)
        err_sys("writen error");
}
```
client.c放在tcpclieserv目录下，修改了Makefile，增加了client.c的编译目标
```
client: client.c
                ${CC} ${CFLAGS}
-o $@ $< ${LIBS}
```
接着就可以开始测试了。
## 测试1 忽略SIGPIPE信号，writen之前，对方关闭接受进程
本机服务端：
nc -l -p 30000
本机客户端:
./client 30000
Begin send 40960 data
Begin Writen 40960
Already write 40960, left 0, errno=0
Begin send 40960 data
Begin Writen 40960
Already write 40960, left 0, errno=0
执行到上步停止服务端,client会继续显示:
Begin send 40960 data
Begin Writen 40960
writen error: Broken pipe(32)
**结论:可见write之前，对方socket中断，发送端write会返回-1,errno号为EPIPE(32)**
## 测试2 catch SIGPIPE信号，writen之前，对方关闭接受进程
修改客户端代码，catch sigpipe信号
```
//signal(SIGPIPE, SIG_IGN);
        signal(SIGPIPE, processSignal);
```
本机服务端：
nc -l -p 30000
本机客户端:
make client
./client 30000
Begin send 40960 data
Begin Writen 40960
Already write 40960, left 0, errno=0
Begin send 40960 data
Begin Writen 40960
Already write 40960, left 0, errno=0
执行到上步停止服务端,client会继续显示:
Begin send 40960 data
Begin Writen 40960
Signal is 13
writen error: Broken pipe(32)
**结论:可见write之前，对方socket中断，发送端write时，会先调用SIGPIPE响应函数，然后write返回-1,errno号为EPIPE(32)**
## 测试3 writen过程中，对方关闭接受进程
为了方便操作，加大1次write的数据量，修改MAXBUF为4096000
本机服务端：
nc -l -p 30000
本机客户端:
make client
./client 30000
Begin send 4096000 data
Begin Writen 4096000
执行到上步停止服务端,client会继续显示:
Already write 589821, left 3506179, errno=0
Begin Writen 3506179
writen error: Connection reset by peer(104)
**结论:可见socket write中，对方socket中断，发送端write会先返回已经发送的字节数,再次write时返回-1,errno号为ECONNRESET(104)**
为什么以上测试，都是对方已经中断socket后，发送端再次write，结果会有所不同呢。从后来找到的UNP5.12,5.13能找到答案
The client's call to readline may happen before the server's RST is received by the client, or it may happen after. If the readline happens before the RST is received, as we've shown in our example, the result is an unexpected
 EOF in the client. But if the RST arrives first, the result is an ECONNRESET ("Connection reset by peer") error return from readline.
以上解释了测试3的现象，write时，收到RST.
What happens if the client ignores the error return from readline and writes more data to the server? This can happen, for example, if the client needs to perform two writes to the server before reading anything back,
 with the first write eliciting the RST.
The rule that applies is: When a process writes to a socket that has received an RST, the SIGPIPE signal is sent to the process. The default action of this signal is to terminate the process, so the process must catch
 the signal to avoid being involuntarily terminated.
If the process either catches the signal and returns from the signal handler, or ignores the signal, the write operation returns EPIPE.
以上解释了测试1，2的现象,write一个已经接受到RST的socket，系统内核会发送SIGPIPE给发送进程，如果进程catch/ignore这个信号，write都返回EPIPE错误.
因此,UNP建议应用根据需要处理SIGPIPE信号，至少不要用系统缺省的处理方式处理这个信号，系统缺省的处理方式是退出进程，这样你的应用就很难查处处理进程为什么退出。
[http://blog.csdn.net/shcyd/archive/2006/10/28/1354577.aspx](http://blog.csdn.net/shcyd/archive/2006/10/28/1354577.aspx)
在Unix系统下，如果send在等待协议传送数据时网络断开的话，调用send的进程会接收到一个SIGPIPE信号，进程对该信号的默认处理是进程终止。
在Unix系统下，如果recv函数在等待协议接收数据时网络断开了，那么调用recv的进程会接收到一个SIGPIPE信号，进程对该信号的默认处理是进程终止。
处理方法：
在初始化时调用**signal(SIGPIPE,SIG_IGN)**忽略该信号（只需一次）
其时send或recv函数将返回-1，errno为EPIPE，可视情况关闭socket或其他处理
gdb：
gdb默认收到sigpipe时中断程序，可调用**handle SIGPIPE nostop print**
相关
**(1)SIG_DFL信号专用的默认动作:**
　　(a)如果默认动作是暂停线程，则该线程的执行被暂时挂起。当线程暂停期间，发送给线程的任何附加信号都不交付，直到该线程开始执行，但是SIGKILL除外。
　　(b)把挂起信号的信号动作设置成SIG_DFL，且其默认动作是忽略信号 (SIGCHLD)。 
**(2)SIG_IGN忽略信号**
　　(a)该信号的交付对线程没有影响
　　(b)系统不允许把SIGKILL或SIGTOP信号的动作设置为SIG_DFL 
**(3)指向函数的指针--捕获信号**
　　(a)信号一经交付，接收线程就在指定地址上执行信号捕获程序。在信号捕 获函数返回后，接受线程必须在被中断点恢复执行。
　　(b)用C语言函数调用的方法进入信号捕捉程序:
　　　　void func (signo)
　　　　int signo;
　　　　func( )是指定的信号捕捉函数，signo是正被交付信号的编码
　　(c)如果SIGFPE,SIGILL或SIGSEGV信号不是由C标准定义的kill( )或raise( )函数所生成，则从信号SIGFPE,SIGILL,SIGSEGV的信号捕获函数正常返回后线程的行为是未定义的。
　　(d)系统不允许线程捕获SIGKILL和SIGSTOP信号。
　　(e)如果线程为SIGCHLD信号建立信号捕获函数，而该线程有未被等待的以终止的子线程时，没有规定是否要生成SIGCHLD信号来指明那个子线程。
每一种信号都被OSKit给予了一个符号名，对于32位的i386平台而言，一个字32位，因而信号有32种。下面的表给出了常用的符号名、描述和它们的信号值。 
符号名　　信号值 描述　　　　　　　　　　　　　　　　是否符合POSIX
SIGHUP　　1　　　在控制终端上检测到挂断或控制线程死亡　　是
SIGINT　　2　　　交互注意信号　　　　　　　　　　　　　　是
SIGQUIT　 3　　　交互中止信号　　　　　　　　　　　　　　是
SIGILL　　4　　　检测到非法硬件的指令　　　　　　　　　　是
SIGTRAP　 5　　　从陷阱中回朔　　　　　　　　　　　　　　否
SIGABRT　 6　　　异常终止信号　　　　　　　　　　　　　　是
SIGEMT　　7　　　EMT 指令　　　　　　　　　　　　　　　　否
SIGFPE　　8　　　不正确的算术操作信号　　　　　　　　　　是
SIGKILL　 9　　　终止信号　　　　　　　　　　　　　　　　是
SIGBUS　　10　　 总线错误　　　　　　　　　　　　　　　　否
SIGSEGV　 11　　 检测到非法的内存调用　　　　　　　　　　是
SIGSYS　　12　　 系统call的错误参数　　　　　　　　　　　否
SIGPIPE　 13　　 在无读者的管道上写　　　　　　　　　　　是
SIGALRM　 14　　 报时信号　　　　　　　　　　　　　　　　是
SIGTERM　 15　　 终止信号　　　　　　　　　　　　　　　　是
SIGURG　　16　　 IO信道紧急信号　　　　　　　　　　　　　否
SIGSTOP　 17　　 暂停信号　　　　　　　　　　　　　　　　是
SIGTSTP　 18　　 交互暂停信号　　　　　　　　　　　　　　是
SIGCONT　 19　　 如果暂停则继续　　　　　　　　　　　　　是
SIGCHLD　 20　　 子线程终止或暂停　　　　　　　　　　　　是
SIGTTIN　 21　　 后台线程组一成员试图从控制终端上读出　　是
SIGTTOU　 22　　 后台线程组的成员试图写到控制终端上　　　是
SIGIO　　 23　　 允许I/O信号 　　　　　　　　　　　　　　否
SIGXCPU　 24　　 超出CPU时限　　　　　　　　　　　　　　 否
SIGXFSZ　 25　　 超出文件大小限制　　　　　　　　　　　　否
SIGVTALRM 26　　 虚时间警报器　　　　　　　　　　　　　　否
SIGPROF　 27　　 侧面时间警报器　　　　　　　　　　　　　否
SIGWINCH　28　　 窗口大小的更改　　　　　　　　　　　　　否
SIGINFO　 29　　 消息请求　　　　　　　　　　　　　　　　否
SIGUSR1 　30　　 保留作为用户自定义的信号1　　　　　　　 是
SIGUSR2 　31　　 保留作为用户自定义的信号　　　　　　　　是 
注意：Linux信号机制基本上是从Unix系统中继承过来的。早期Unix系统中的信号机制比较简单和原始，后来在实践中暴露出一些问题，因此，把那些建立在早期机制上的信号叫做"不可靠信号"，信号值小于SIGRTMIN(Red hat 7.2中，SIGRTMIN=32，SIGRTMAX=63)的信号都是不可靠信号。这就是"不可靠信号"的来源。它的主要问题是：进程每次处理信号后，就将对信号的响应设置为默认动作。在某些情况下，将导致对信号的错误处理；因此，用户如果不希望这样的操作，那么就要在信号处理函数结尾再一次调用signal()，重新安装该信号。
另外，我再做一些补充，产生RST响应以至于系统发出SIGPIPE信号，应该分为两种情况:
1. 客户端到服务端之间网络断掉，或者服务端断电等，物理连接断掉了，这种情况下客户端不会退出，send函数正常执行，不会感觉到自己出错。因为由于物理网络断开，服务端不会给客户端回应错误消息，没有RST响应，自然也不会产生SIGPIPE信号。但是当服务端再恢复正常的时候，对客户端send来的消息会产生RST响应，客户端就收到SIGPIPE信号了，程序退出，但是这时send函数是能够返回 -1的。可以进行异常处理。
2.客户端到服务端的网络能通，服务程序挂掉，客户端程序会马上退出，因为服务端能正常返回错误消息，客户端收到，SIGPIPE信号就产生了。不过我不确定此时服务端返回是的RST响应，抓包来看没有RST标志。水平有限，只写到这了。
引用:[http://www.cppblog.com/elva/archive/2008/09/10/61544.html](http://www.cppblog.com/elva/archive/2008/09/10/61544.html)
