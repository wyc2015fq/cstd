# Socket编程知识必学/SELECT 编程 - maopig的专栏 - CSDN博客
2012年02月19日 16:56:01[maopig](https://me.csdn.net/maopig)阅读数：9039
                
 Select在Socket编程中还是比较重要的，可是对于初学Socket的人来说都不太爱用Select写程序，他们只是习惯写诸如 connect、accept、recv或recvfrom这样的阻塞程序（所谓阻塞方式block，顾名思义，就是进程或是线程执行到这些函数时必须等待某个事件的发生，如果事件没有发生，进程或线程就被阻塞，函数不能立即返回）。可是使用Select就可以完成非阻塞（所谓非阻塞方式non- block，就是进程或线程执行此函数时不必非要等待事件的发生，一旦执行肯定返回，以返回值的不同来反映函数的执行情况，如果事件发生则与阻塞方式相同，若事件没有发生则返回一个代码来告知事件未发生，而进程或线程继续执行，所以效率较高）方式工作的程序，它能够监视我们需要监视的文件描述符的变化情况读写或是异常。下面详细介绍一下！
Select的函数格式(我所说的是Unix系统下的伯克利socket编程，和windows下的有区别，一会儿说明)： 
int select(int maxfdp,fd_set *readfds,fd_set *writefds,fd_set *errorfds,struct timeval *timeout);
先说明两个结构体： 
第一，struct fd_set可以理解为一个集合，这个集合中存放的是文件描述符(file descriptor)，即文件句柄，这可以是我们所说的普通意义的文件，当然Unix下任何设备、管道、FIFO等都是文件形式，全部包括在内，所以毫无疑问一个socket就是一个文件，socket句柄就是一个文件描述符。fd_set集合可以通过一些宏由人为来操作，比如清空集合 FD_ZERO(fd_set *)，将一个给定的文件描述符加入集合之中FD_SET(int ,fd_set *)，将一个给定的文件描述符从集合中删除FD_CLR(int
 ,fd_set*)，检查集合中指定的文件描述符是否可以读写FD_ISSET(int ,fd_set* )。一会儿举例说明。 
第二，struct timeval是一个大家常用的结构，用来代表时间值，有两个成员，一个是秒数，另一个是毫秒数。 
具体解释select的参数： 
int maxfdp是一个整数值，是指集合中所有文件描述符的范围，即所有文件描述符的最大值加1，不能错！在Windows中这个参数的值无所谓，可以设置不正确。 
fd_set *readfds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的读变化的，即我们关心是否可以从这些文件中读取数据了，如果这个集合中有一个文件可读，select就会返回一个大于0的值，表示有文件可读，如果没有可读的文件，则根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的读变化。
fd_set *writefds是指向fd_set结构的指针，这个集合中应该包括文件描述符，我们是要监视这些文件描述符的写变化的，即我们关心是否可以向这些文件中写入数据了，如果这个集合中有一个文件可写，select就会返回一个大于0的值，表示有文件可写，如果没有可写的文件，则根据timeout参数再判断是否超时，若超出timeout的时间，select返回0，若发生错误返回负值。可以传入NULL值，表示不关心任何文件的写变化。
fd_set *errorfds同上面两个参数的意图，用来监视文件错误异常。 
struct timeval* timeout是select的超时时间，这个参数至关重要，它可以使select处于三种状态，第一，若将NULL以形参传入，即不传入时间结构，就是将select置于阻塞状态，一定等到监视文件描述符集合中某个文件描述符发生变化为止；第二，若将时间值设为0秒0毫秒，就变成一个纯粹的非阻塞函数，不管文件描述符是否有变化，都立刻返回继续执行，文件无变化返回0，有变化返回一个正值；第三，timeout的值大于0，这就是等待的超时时间，即
 select在timeout时间内阻塞，超时时间之内有事件到来就返回了，否则在超时后不管怎样一定返回，返回值同上述。 
返回值： 
负值：select错误 正值：某些文件可读写或出错 0：等待超时，没有可读写或错误的文件 
在有了select后可以写出像样的网络程序来！举个简单的例子，就是从网络上接受数据写入一个文件中。 
例子： 
main() 
{ 
int sock; 
FILE *fp; 
struct fd_set fds; 
struct timeval timeout={3,0}; //select等待3秒，3秒轮询，要非阻塞就置0
char buffer[256]={0}; //256字节的接收缓冲区 
/* 假定已经建立UDP连接，具体过程不写，简单，当然TCP也同理，主机ip和port都已经给定，要写的文件已经打开 
sock=socket(...); 
bind(...); 
fp=fopen(...); */ 
while(1) 
{ 
FD_ZERO(&fds); //每次循环都要清空集合，否则不能检测描述符变化 
FD_SET(sock,&fds); //添加描述符 
FD_SET(fp,&fds); //同上 
maxfdp=sock>fp?sock+1:fp+1; //描述符最大值加1 
switch(select(maxfdp,&fds,&fds,NULL,&timeout)) //select使用
{ 
case -1: exit(-1);break; //select错误，退出程序 
case 0:break; //再次轮询 
default: 
if(FD_ISSET(sock,&fds)) //测试sock是否可读，即是否网络上有数据 
{ 
recvfrom(sock,buffer,256,.....);//接受网络数据 
if(FD_ISSET(fp,&fds)) //测试文件是否可写 
fwrite(fp,buffer...);//写入文件 
buffer清空; 
}// end if break; 
}// end switch 
}//end while 
}//end main
----------------------------------------------------------
**Linux select()详解**
Linux select()详解       select系统调用是用来让我们的程序监视多个文件句柄(file descriptor)的状态变化的。程序会停在select这里等待，直到被监视的文件句柄有某一个或多个发生了状态改变。
    文件在句柄在Linux里很多，如果你man某个函数，在函数返回值部分说到成功后有一个文件句柄被创建的都是的，如man socket可以看到“On success, a file descriptor for the new socket is returned.”而man 2 open可以看到“open() and creat() return the new file descriptor”，其实文件句柄就是一个整数，看socket函数的声明就明白了：
    int socket(int domain, int type, int protocol);
当然，我们最熟悉的句柄是0、1、2三个，0是标准输入，1是标准输出，2是标准错误输出。0、1、2是整数表示的，对应的FILE *结构的表示就是stdin、stdout、stderr，0就是stdin，1就是stdout，2就是stderr。
比如下面这两段代码都是从标准输入读入9个字节字符：
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char ** argv)
{
        char buf[10] = "";
        read(0, buf, 9); /* 从标准输入 0 读入字符 */
        fprintf(stdout, "%s\n", buf); /* 向标准输出 stdout 写字符 */
        return 0;
}
/* **上面和下面的代码都可以用来从标准输入读用户输入的9个字符** */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char ** argv)
{
        char buf[10] = "";
        fread(buf, 9, 1, stdin); /* 从标准输入 stdin 读入字符 */
        write(1, buf, strlen(buf));
        return 0;
}
   继续上面说的select，就是用来监视某个或某些句柄的状态变化的。select函数原型如下：
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
函数的最后一个参数timeout显然是一个超时时间值，其类型是struct timeval *，即一个struct timeval结构的变量的指针，所以我们在程序里要申明一个struct timeval tv;然后把变量tv的地址&tv传递给select函数。struct timeval结构如下：
struct timeval {
             long    tv_sec;         /* seconds */
             long    tv_usec;        /* microseconds */
         };
   第2、3、4三个参数是一样的类型： fd_set *，即我们在程序里要申明几个fd_set类型的变量，比如rdfds, wtfds, exfds，然后把这个变量的地址&rdfds, &wtfds, &exfds 传递给select函数。这三个参数都是一个句柄的集合，第一个rdfds是用来保存这样的句柄的：当句柄的状态变成可读的时系统就会告诉select函数返回，同理第二个wtfds是指有句柄状态变成可写的时系统就会告诉select函数返回，同理第三个参数exfds是特殊情况，即句柄上有特殊情况发生时系统会告诉select函数返回。特殊情况比如对方通过一个socket句柄发来了紧急数据。如果我们程序里只想检测某个socket是否有数据可读，我们可以这样：
fd_set rdfds; /* 先申明一个 fd_set 集合来保存我们要检测的 socket句柄 */
struct timeval tv; /* 申明一个时间变量来保存时间 */
int ret; /* 保存返回值 */
FD_ZERO(&rdfds); /* 用select函数之前先把集合清零 */
FD_SET(socket, &rdfds); /* 把要检测的句柄socket加入到集合里 */
tv.tv_sec = 1;
tv.tv_usec = 500; /* 设置select等待的最大时间为1秒加500毫秒 */
ret = select(socket + 1, &rdfds, NULL, NULL, &tv); /* 检测我们上面设置到集合rdfds里的句柄是否有可读信息 */
if(ret < 0) perror("select");/* 这说明select函数出错 */
else if(ret == 0) printf("超时\n"); /* 说明在我们设定的时间值1秒加500毫秒的时间内，socket的状态没有发生变化 */
else { /* 说明等待时间还未到1秒加500毫秒，socket的状态发生了变化 */
    printf("ret=%d\n", ret); /* ret这个返回值记录了发生状态变化的句柄的数目，由于我们只监视了socket这一个句柄，所以这里一定ret=1，如果同时有多个句柄发生变化返回的就是句柄的总和了 */
    /* 这里我们就应该从socket这个句柄里读取数据了，因为select函数已经告诉我们这个句柄里有数据可读 */
    if(FD_ISSET(socket, &rdfds)) { /* 先判断一下socket这外被监视的句柄是否真的变成可读的了 */
        /* 读取socket句柄里的数据 */
        recv(...);
    }
}
   注意select函数的第一个参数，是所有加入集合的句柄值的最大那个值还要加1。比如我们创建了3个句柄：
/************关于本文档********************************************
*filename: Linux网络编程一步一步学-select详解
*purpose: 详细说明select的用法
*wrote by: zhoulifa([zhoulifa@163.com](mailto:zhoulifa@163.com)) 周立发([http://zhoulifa.bokee.com](http://zhoulifa.bokee.com/))
Linux爱好者 Linux知识传播者 SOHO族 开发者 最擅长C语言
*date time:2007-02-03 19:40
*Note: 任何人可以任意复制代码并运用这些文档，当然包括你的商业用途
* 但请遵循GPL
*Thanks to:Google
*Hope:希望越来越多的人贡献自己的力量，为科学技术发展出力
* 科技站在巨人的肩膀上进步更快！感谢有开源前辈的贡献！
*********************************************************************/
int sa, sb, sc;
sa = socket(...); /* 分别创建3个句柄并连接到服务器上 */
connect(sa,...);
sb = socket(...);
connect(sb,...);
sc = socket(...);
connect(sc,...);
FD_SET(sa, &rdfds);/* 分别把3个句柄加入读监视集合里去 */
FD_SET(sb, &rdfds);
FD_SET(sc, &rdfds);
   在使用select函数之前，一定要找到3个句柄中的最大值是哪个，我们一般定义一个变量来保存最大值，取得最大socket值如下：
int maxfd = 0;
if(sa > maxfd) maxfd = sa;
if(sb > maxfd) maxfd = sb;
if(sc > maxfd) maxfd = sc;
   然后调用select函数：
ret = select(maxfd + 1, &rdfds, NULL, NULL, &tv); /* 注意是最大值还要加1 */
   同样的道理，如果我们要检测用户是否按了键盘进行输入，我们就应该把标准输入0这个句柄放到select里来检测，如下：
FD_ZERO(&rdfds);
FD_SET(0, &rdfds);
tv.tv_sec = 1;
tv.tv_usec = 0;
ret = select(1, &rdfds, NULL, NULL, &tv); /* 注意是最大值还要加1 */
if(ret < 0) perror("select");/* 出错 */
else if(ret == 0) printf("超时\n"); /* 在我们设定的时间tv内，用户没有按键盘 */
else { /* 用户有按键盘，要读取用户的输入 */
    scanf("%s", buf);
}----------------------------------------------------------
Linux select学习笔记
select系统调用是用来让我们的程序监视多个文件描述符(file descrīptor)的状态变化的。程序会停在select这里等待，直到被监视的文件描述符有某一个或多个发生了状态改变。select()的机制中提供一fd_set的数据结构，实际上是一long类型的数组， 每一个数组元素都能与一打开的文件描述符（不管是Socket描述符,还是其他 文件或命名管道或设备描述符）建立联系，建立联系的工作由程序员完成，
当调用select()时，由内核根据IO状态修改fd_set的内容，由此来通知执 行了select()的进程哪一Socket或文件可读，
select函数原型如下：
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
函数的最后一个参数timeout显然是一个超时时间值，其类型是struct timeval *，即一个struct timeval结构的变量的指针，所以我们在程序里要申明一个struct timeval tv;然后把变量tv的地址&tv传递给select函数。struct timeval结构如下：
struct timeval {
long   tv_sec;     /* seconds */
long   tv_usec;    /* microseconds */
};
第2、3、4三个参数的类型是一样的： fd_set *，即我们在程序里要申明几个fd_set类型的变量，比如定义了rfds, wfds, efds。
另外关于fd_set类型的变量，还有一组标准的宏定义来处理此类变量：
FD_ZERO(fd_set *fdset)：清空fdset与所有文件描述符的联系。
FD_SET(int fd, fd_set *fdset)：建立文件描述符fd与fdset的联系。
FD_CLR(int fd, fd_set *fdset)：清除文件描述符fd与fdset的联系。
FD_ISSET(int fd, fd_set *fdset)：检查fd_set联系的文件描述符fd是否可读写，>0表示可读写。
（关于fd_set及相关宏的定义见/usr/include/sys/types.h）定义的这三个参数都是描述符的集合，第一个rfds是用来保存这样的描述符的：当描述符的状态变成可读的时系统就会告诉select函数返回，第二个wfds是指有描述符状态变成可写的时系统就会告诉select函数返回，第三个参数efds是特殊情况，即描述符上有特殊情况发生时系统会告诉select函数返回。下面以一个输入为例来说明：
int fd1, fd2;         /* 在定义两个描述符*/
fd1 = socket(...);    /* 创建socket连接*/
fd2 = open(“/dev/tyS0”,O_RDWR); /* 打开一个串口*/
FD_ZERO(&rfds);       /* 用select函数之前先把集合清零 */
FD_SET(fd1, &rfds);   /* 分别把2个描述符加入读监视集合里去 */
FD_SET(fd2, &rfds);
int maxfd = 0;
maxfd = (fd1>fd2)?(fd1+1):(fd2+1);           /* 注意是最大值还要加1 */
ret = select(maxfd, &rfds, NULL, NULL, &tv); /*然后调用select函数*/
这样就可以使用一个开关语句（switch语句）来判断到底是哪一个输入源在输入数据。具体判断如下：
switch（ret）{
case -1：perror("select");/* 这说明select函数出错 */ 
case 0：printf("超时\n"); /* 说明在设定的时间内，socket的状态没有发生变化 */
default：
if（FD_ISSET(fd1, &rfds)） 处理函数1（）；/*socket有数据来*/
if（FD_ISSET(fd2, &rfds)） 处理函数2（）；/*ttyS0有数据来*/
}
以下来自网络搜索：
Linux下select调用的过程:
1.用户层应用程序调用select(),底层调用poll())
2.核心层调用sys_select() ------> do_select()
最终调用文件描述符fd对应的struct file类型变量的struct file_operations *f_op的poll函数。
poll指向的函数返回当前可否读写的信息。
1)如果当前可读写，返回读写信息。
2)如果当前不可读写，则阻塞进程，并等待驱动程序唤醒，重新调用poll函数，或超时返回。
3.驱动需要实现poll函数。
当驱动发现有数据可以读写时，通知核心层，核心层重新调用poll指向的函数查询信息。
poll_wait(filp,&wait_q,wait) // 此处将当前进程加入到等待队列中，但并不阻塞
在中断中使用wake_up_interruptible(&wait_q)唤醒等待队列
----------------------------------------------------------
Socket编程中select()的妙用
用过 WinSock API 网友们知道：WinSock 编程中有一很方便的地方便是其 
息驱动机制，不管是底层 API 的 WSAAsyncSelect() 还是 MFC 的异步Socket类： 
CAsyncSocket，都提供了诸如 FD_ACCEPT、FD_READ、FD_CLOSE 之类的消息 
供编程人员捕捉并处理。FD_ACCEPT 通知进程有客户方Socket请求连接， 
FD_READ通知进程本地Socket有东东可读，FD_CLOSE通知进程对方Socket已 
关闭。那么，BSD Socket 是不是真的相形见拙呢？ 
非也！ 'cause cpu love unix so. 
BSD UNIX中有一系统调用芳名select()完全可以提供类似的消息驱动机制。 
cpu郑重宣布：WinSock的WSAAsyncSeclet()不过是此select()的fork版！ 
bill也是fork出来的嘛，xixi. 
select()的机制中提供一fd_set的数据结构，实际上是一long类型的数组， 
每一个数组元素都能与一打开的文件句柄（不管是Socket句柄,还是其他 
文件或命名管道或设备句柄）建立联系，建立联系的工作由程序员完成， 
当调用select()时，由内核根据IO状态修改fd_set的内容，由此来通知执 
行了select()的进程哪一Socket或文件可读，下面具体解释： 
#include  <sys/types.h> 
#include  <sys/times.h> 
#include  <sys/select.h> 
int select(nfds, readfds, writefds, exceptfds, timeout) 
int nfds; 
fd_set *readfds, *writefds, *exceptfds; 
struct timeval *timeout; 
ndfs：select监视的文件句柄数，视进程中打开的文件数而定,一般设为呢要监视各文件 
      中的最大文件号加一。 
readfds：select监视的可读文件句柄集合。 
writefds: select监视的可写文件句柄集合。 
exceptfds：select监视的异常文件句柄集合。 
timeout：本次select()的超时结束时间。（见/usr/sys/select.h， 
        可精确至百万分之一秒！） 
当readfds或writefds中映象的文件可读或可写或超时，本次select() 
就结束返回。程序员利用一组系统提供的宏在select()结束时便可判 
断哪一文件可读或可写。对Socket编程特别有用的就是readfds。 
几只相关的宏解释如下： 
FD_ZERO(fd_set *fdset)：清空fdset与所有文件句柄的联系。 
FD_SET(int fd, fd_set *fdset)：建立文件句柄fd与fdset的联系。 
FD_CLR(int fd, fd_set *fdset)：清除文件句柄fd与fdset的联系。 
FD_ISSET(int fd, fdset *fdset)：检查fdset联系的文件句柄fd是否 
                                可读写，>0表示可读写。 
（关于fd_set及相关宏的定义见/usr/include/sys/types.h） 
这样，你的socket只需在有东东读的时候才读入，大致如下： 
... 
int     sockfd; 
fd_set  fdR; 
struct  timeval timeout = ..; 
... 
for(;;) { 
        FD_ZERO(&fdR); 
        FD_SET(sockfd, &fdR); 
        switch (select(sockfd + 1, &fdR, NULL, &timeout)) { 
                case -1: 
                        error handled by u; 
                case 0: 
                        timeout hanled by u; 
                default: 
                        if (FD_ISSET(sockfd)) { 
                                now u read or recv something; 
                                /* if sockfd is father and  
                                server socket, u can now 
                                accept() */ 
                        } 
        } 
} 
所以一个FD_ISSET(sockfd)就相当通知了sockfd可读。 
至于struct timeval在此的功能，请man select。不同的timeval设置 
使使select()表现出超时结束、无超时阻塞和轮询三种特性。由于 
timeval可精确至百万分之一秒，所以Windows的SetTimer()根本不算 
什么。你可以用select()做一个超级时钟。 
FD_ACCEPT的实现？依然如上，因为客户方socket请求连接时，会发送 
连接请求报文，此时select()当然会结束，FD_ISSET(sockfd)当然大 
于零，因为有报文可读嘛！至于这方面的应用，主要在于服务方的父 
Socket，你若不喜欢主动accept()，可改为如上机制来accept()。 
至于FD_CLOSE的实现及处理，颇费了一堆cpu处理时间，未完待续。 
-- 
讨论关于利用select()检测对方Socket关闭的问题： 
仍然是本地Socket有东东可读，因为对方Socket关闭时，会发一个关闭连接 
通知报文，会马上被select()检测到的。关于TCP的连接（三次握手）和关 
闭（二次握手）机制，敬请参考有关TCP/IP的书籍。 
不知是什么原因，UNIX好象没有提供通知进程关于Socket或Pipe对方关闭的 
信号，也可能是cpu所知有限。总之，当对方关闭，一执行recv()或read()， 
马上回返回-1，此时全局变量errno的值是115，相应的sys_errlist[errno] 
为"Connect refused"（请参考/usr/include/sys/errno.h）。所以，在上 
篇的for(;;)...select()程序块中，当有东西可读时，一定要检查recv()或 
read()的返回值，返回-1时要作出关断本地Socket的处理，否则select()会 
一直认为有东西读，其结果曾几令cpu伤心欲断针脚。不信你可以试试：不检 
查recv()返回结果，且将收到的东东（实际没收到）写至标准输出... 
在有名管道的编程中也有类似问题出现。具体处理详见拙作：发布一个有用 
的Socket客户方原码。 
至于主动写Socket时对方突然关闭的处理则可以简单地捕捉信号SIGPIPE并作 
出相应关断本地Socket等等的处理。SIGPIPE的解释是：写入无读者方的管道。 
在此不作赘述，请详man signal。 
以上是cpu在作tcp/ip数据传输实验积累的经验，若有错漏，请狂炮击之。 
唉，昨天在hacker区被一帮孙子轰得差点儿没短路。ren cpu(奔腾的心) z80 
补充关于select在异步(非阻塞)connect中的应用,刚开始搞socket编程的时候 
我一直都用阻塞式的connect,非阻塞connect的问题是由于当时搞proxy scan 
而提出的呵呵 
通过在网上与网友们的交流及查找相关FAQ,总算知道了怎么解决这一问题.同样 
用select可以很好地解决这一问题.大致过程是这样的: 
1.将打开的socket设为非阻塞的,可以用fcntl(socket, F_SETFL, O_NDELAY)完 
成(有的系统用FNEDLAY也可). 
2.发connect调用,这时返回-1,但是errno被设为EINPROGRESS,意即connect仍旧 
在进行还没有完成. 
3.将打开的socket设进被监视的可写(注意不是可读)文件集合用select进行监视, 
如果可写,用 
        getsockopt(socket, SOL_SOCKET, SO_ERROR, &error, sizeof(int)); 
来得到error的值,如果为零,则connect成功. 
在许多unix版本的proxyscan程序你都可以看到类似的过程,另外在solaris精华 
区->编程技巧中有一个通用的带超时参数的connect模块.  
**端口号常识:**
    端口号被从1 开始分配。
    通常端口号超出255 的部分被本地主机保留为私有用途。
    1到255 之间的号码被用于远程应用程序所请求的进程和网络服务。
    每个网络通信循环地进出主计算机的TCP 应用层。它被两个所连接的号码唯一地识别。这两个号码合起来叫做套接字.
        组成套接字的这两个号码就是机器的IP 地址和TCP 软件所使用的端口号。
**套接字的三种类型**
    流式套接字（SOCK_STREAM），数据报套接字（SOCK_DGRAM）及原始套接字(RAW)。
**  流式套接字（SOCK_STREAM）**
    流式的套接字可以提供可靠的、面向连接的通讯流。
    如果你通过流式套接字发送了顺序的数据：“1”、“２”。那么数据到达远程时候的顺序也是“１”、“２”。
    Telnet 应用程序、BBS服务、以及系统的远程登陆都是通过Telnet 协议连接的。Telnet 就是一个流式连接。你是否希望你在Telnet 应用程序上输入的字符（或汉字）在到达远程应用程序的时候是以你输入的顺序到达的？答案应该是肯定的吧。
    还有WWW 浏览器，它使用的HTTP 协议也是通过流式套接字来获取网页的。事实上，如果你Telnet 到一个Web Site 的80 端口上，然后输入 “GET 网页路径名”然后按两下回车（或者是两下Ctrl+回车）然后你就得到了“网页路径名”所代表的网页！
**  数据报套接字（SOCK_DGRAM）**
    数据报套接字定义了一种无连接的服务，数据通过相互独立的报文进行传输，是无序的，并且不保证可靠，无差错。
       如果你发送了一个数据报，它可能不会到达。
       它可能会以不同的顺序到达。
       如果它到达了，它包含的数据中可能存在错误。
       数据报套接字也使用IP，但是它不使用TCP，它使用使用者数据报协议UDP（User Datagram Protocol 可以参考RFC 768）
   为什么说它们是“无连接”的呢？因为它（UDP）不像流式套接字那样维护一个打开的连接，你只需要把数据打成一个包，把远程的IP 贴上去，然后把这个包发送出去。这个过程是不需要建立连接的。
       UDP 的应用例子有： tftp, bootp 等。
   那么，数据包既然会丢失，怎样能保证程序能够正常工作呢？
       事实上，每个使用UDP的程序都要有自己的对数据进行确认的协议。
           比如， TFTP 协议定义了对于每一个发送出去的数据包，远程在接受到之后都要回送一个数据包告诉本地程序：“我已经拿到了！”（一个 “ACK” 包）。如果数据包发的送者在5 秒内没有的得到回应，它就会重新发送这个数据包直到数据包接受者回送了 “ACK” 信号。这些知识对编写一个使用UDP 协议的程序员来说是非常必要的。
   无连接服务器一般都是面向事务处理的，一个请求一个应答就完成了客户程序与服务程序之间的相互作用。
   面向连接服务器处理的请求往往比较复杂，不是一来一去的请求应答所能解决的，而且往往是并发服务器
   套接字工作过程如下:
         服务器首先启动
         通过调用socket()建立一个套接字，
         然后调用bind()将该套接字和本地网络地址联系在一起，
         再调用listen()使套接字做好侦听的准备，并规定它的请求队列的长度,
         之后就调用accept()来接收连接。
         客户在建立套接字
         然后就可调用connect()和服务器建立连接。
         客户机和服务器之间就可以通过调用read()和write()来发送和接收数据。
         最后，待数据传送结束后，双方调用close()关闭套接字。
对流式套接字你所需要做的只是调用send() 函数来发送数据。而对于数据报套接字，你需要自己加个信息头，然后调用sendto() 函数把数据发送出去
**  原始套接字**
     原始套接字主要用于一些协议的开发，可以进行比较底层的操作。它功能强大，但是没有上面介绍的两种套接字使用方便，一般的程序也涉及不到原始套接字
**套接字结构**
struct sockaddr
这个结构用来存储套接字地址。
数据定义：
struct sockaddr 
{
    unsigned short sa_family;  /* address族, AF_xxx */
    char sa_data[14];              /* 14 bytes的协议地址 */
};
sa_family  一般来说，都是 “AF_INET”。
sa_data    包含了一些远程电脑的地址、端口和套接字的数目，它里面的数据是杂溶在一切的。
为了处理struct sockaddr， 程序员建立了另外一个相似的结构struct sockaddr_in (“in” 代表 “Internet”):
struct sockaddr_in 
{
   short     int       sin_family;           /* Internet地址族 */
   unsigned  short int sin_port;       /* 端口号 */
   struct    in_addr   sin_addr;         /* Internet地址 */
   unsigned  char      sin_zero[8];   /* 添0（和struct sockaddr一样大小）*/
};
注意:
    1)这个结构提供了方便的手段来访问socket address（struct sockaddr）结构中的每一个元素。
    2)sin_zero[8] 是为了是两个结构在内存中具有相同的尺寸
                  要把sin_zero 全部设成零值（使用bzero()或memset()函数）。
    3)一个指向struct sockaddr_in 的指针可以声明指向一个sturct sockaddr 的结构。所以虽然socket() 函数需要一个structaddr * ，你也可以给他一个sockaddr_in * 。
    4)在struct  sockaddr_in 中，sin_family 相当于 在struct sockaddr 中的sa_family，需要设成 “AF_INET”。
    5)一定要保证sin_port 和sin_addr 必须是网络字节顺序（见下节）！
2．struct in_addr  ( 因特网地址 (a structure for historical reasons) )
   struct in_addr 
   {
       unsigned long s_addr;
   };
   如果你声明了一个 "ina" 作为一个struct sockaddr_in 的结构， 那么“ina.sin_addr.s_addr”就是4 个字节的IP 地址（按网络字节顺序排放）。
   需要注意的是，即使你的系统仍然使用联合而不是结构来表示struct in_addr，你仍然可以用上面的方法得到4 个字节的IP 地址（一些 #defines 帮了你的忙）
**网络字节顺序**
   因为每一个机器内部对变量的字节存储顺序不同（有的系统是高位在前，底位在后，而有的系统是底位在前，高位在后），而网络传输的数据大家是一定要统一顺序的。
   所以对与内部字节表示顺序和网络字节顺序不同的机器，就一定要对数据进行转换（比如IP 地址的表示，端口号的表示）。
   但是内部字节顺序和网络字节顺序相同的机器该怎么办呢？是这样的：它们也要调用转换函数，但是真正转换还是不转换是由系统函数自己来决定的。
**有关的转化函数**      我们通常使用的有两种数据类型：短型（两个字节）和长型（四个字节）。
      下面介绍的这些转换函数对于这两类的无符号整型变量都可以进行正确的转换。
      如果你想将一个短型数据从主机字节顺序转换到网络字节顺序的话，有这样一个函数htons：
         它是以“h”开头的（代表“主机”）；
         紧跟着它的是“to”，代表“转换到”；
         然后是“n”代表“网络”；
         最后是“s”，代表“短型数据”。
         H-to-n-s，就是htons() 函数（可以使用Hostto Network Short 来助记）
      你可以使用 “n”，“h”，“to”，“s”，“l”的任意组合.当然，你要在可能的情况下进行组合。比如，系统是没有stolh() 函数的（Short to Long Host?）。
        下面给出套接字字节转换程序的列表：
          htons()——“Host to Network Short” 主机字节顺序转换为网络字节顺序（对无符号短型进行操作4 bytes）
          htonl()——“Host to Network Long”　主机字节顺序转换为网络字节顺序（对无符号长型进行操作8 bytes）
          ntohs()——“Network to Host Short “　网络字节顺序转换为主机字节顺序（对无符号短型进行操作4 bytes）
          ntohl()——“Network to Host Long “　网络字节顺序转换为主机字节顺序（对无符号长型进行操作8 bytes）
     在struct sockaddr_in 中的sin_addr 和sin_port 他们的字节顺序都是网络字节顺序，而sin_family 却不是网络字节顺序的。为什么呢？
        这个是因为sin_addr 和sin_port 是从IP 和UDP 协议层取出来的数据，而在IP 和UDP协议层，是直接和网络相关的，所以，它们必须使用网络字节顺序。
        然而， sin_family 域只是内核用来判断struct sockaddr_in 是存储的什么类型的数据，并且， sin_family 永远也不会被发送到网络上，所以可以使用主机字节顺序来存储
**socket() 函数**
   /**
   * 
   * 取得套接字描述符！（记得我们以前说过的吗？它其实就是一个文件描述符）
   *
   * domain   需要被设置为 “AF_INET”，就像上面的struct sockaddr_in。
   * type     参数告诉内核这个socket 是什么类型，“SOCK_STREAM”或是“SOCK_DGRAM”。
   * protocol 通常为0
   *
   * return   如果发生错误，socket()函数返回 –1 。全局变量errno 将被设置为错误代码。
   *
   */
   #include 
   #include 
   int socket（int domain , int type , int protocol）
   示例:
   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
        perror("create sock");
        return -1;
   }
   else
   {
      // printf("socket created...\n");
   }
**bind() 函数**
/**
*
* 为套接字绑定一个端口号
*
* 当你需要进行端口监听listen()操作，等待接受一个连入请求的时候，
* 一般都需要经过这一步。比如网络泥巴(MUD)，Telnet a.b.c.d 4000
* 
* 如果你只是想进行连接一台服务器，也就是进行connect() 操作的时候，这一步并不是必须的。 
*
*
* sockfd    是由socket()函数返回的套接字描述符
* my_addr   是一个指向struct sockaddr 的指针，包含有关你的地址的信息：名称、端口和IP 地址。
* addrlen   可以设置为sizeof(struct sockaddr)
* 
* return    调用错误的时候，返回 -1 作为错误发生的标志。errno 的值为错误代码。
*
*/
#include 
#include 
int bind (int sockfd , struct sockaddr *my_addr , int addrlen) ;
示例:
#include 
#include 
#include 
#define  MYPORT 4000
main()
{
  int sockfd ;
  struct sockaddr_in my_addr ;
  sockfd                  = socket(AF_INET, SOCK_STREAM, 0); /* 在你自己的程序中要进行错误检查！! */
  my_addr.sin_family      = AF_INET ;        /* 主机字节顺序 */
  my_addr.sin_port        = htons(MYPORT);   /* 网络字节顺序，短整型 */
  my_addr.sin_addr.s_addr = inet_addr("166.111.69.52") ;
  bzero(&(my_addr.sin_zero), 8);             /* 将整个结构剩余部分数据设为0 */
  bind (sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));  /* 不要忘记在你自己的程序中加入判断bind 错误的代码！！ */
  注意：
    my_addr.sin_port         是网络字节顺序, 短整型
    my_addr.sin_addr.s_addr  也是网络字节顺序。
  最后，bind()可以在程序中自动获取你自己的IP 地址和端口。
    my_addr.sin_port = 0 ;                 /* 随机选择一个端口 */
    my_addr.sin_addr.s_addr = INADDR_ANY ; /* 使用自己的地址 */
    如上，通过设置my_addr.sin_port 为0，bind()可以知道你要它帮你选择合适的端口；
    通过设置my_addr.sin_addr.s_addr 为INADDR_ANY，bind()知道你要它将s_addr 填充为运行这个进程的机器的IP。
    这一切都可以要求bind()来自动的帮助你完成。
    如果你注意到了一些细节的话，你可能会发现我并没有将INADDR_ANY 转换为网络字节顺序！是这样的，INADDR_ANY的值为0，0 就是0，无论用什么顺序排列位的顺序，它都是不变的。
    有读者会想了，因为我用的INADDR_ANY 是一个#define，那么如果将我的程序移植到另外一个系统，假如那里的INADDR_ANY是这样定义的：
       #define INADDR_ANY 100，
       那么我的程序不是就会不运行了吗？那么下面这段代码就OK 了:
       my_addr.sin_port = htons(0);                   /* 随机选择一个未用的端口 */
       my_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;  /* 使用自己的IP地址 */
    现在我们已经是这么的严谨，对于任何数值的INADDR_ANY调用bind 的时候就都不会有麻烦了。
   另外一件必须指出的事情是：
       当你调用bind()的时候，不要把端口数设置的过小！小于1024 的所有端口都是保留下来作为系统使用端口的，没有root 权利无法使用。你可以使用1024 以上的任何端口，一直到65535 ：你所可能使用的最大的端口号（当然，你还要保证你所希望使用的端口没有被其他程序所使用）。
   最后注意有关bind()的是：
       有时候你并不一定要调用bind()来建立网络连接。比如你只是想连接到一个远程主机上面进行通讯，你并不在乎你究竟是用的自己机器上的哪个端口进行通讯（比如Telnet），那么你可以简单的直接调用connect()函数，connect()将自动寻找出本地机器上的一个未使用的端口，然后调用 bind()来将其socket 绑定到那个端口上。
**connect() 函数**
/*
*
* sockfd     套接字文件描述符，由socket()函数返回的
* serv_addr  是一个存储远程计算机的IP 地址和端口信息的结构
* addrlen    应该是sizeof(struct sockaddr)
*
* return     如果发生了错误（比如无法连接到远程主机，或是远程主机的指定端口无法进行连接等）它将会返回错误值 -1
*            全局变量errno将会存储错误代码
*
*
*/
#include 
#include 
int connect (int sockfd, struct sockaddr *serv_addr, int addrlen);
示例:
#include 
#include 
#include 
#define DEST_IP “166.111.69.52”
#define DEST_PORT 23
main()
{
   int sockfd ;
   /* 将用来存储远程信息 */
   struct sockaddr_in dest_addr ;
   /* 注意在你自己的程序中进行错误检查！！ */
   sockfd = socket（AF_INET, SOCK_STREAM, 0）;
   /* 主机字节顺序 */
   dest_addr.sin_family = AF_INET ;
   /* 网络字节顺序，短整型 */
   dest_addr.sin_port = htons(DEST_PORT(;
   dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
   /* 将剩下的结构中的空间置0 */
   bzero(&(dest_addr.sin_zero), 8）;
   /* 不要忘记在你的代码中对connect()进行错误检查！！ */
   connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
   注意我们没有调用bind()函数。基本上，我们并不在乎我们本地用什么端口来通讯，是不是？我们在乎的是我们连到哪台主机上的哪个端口上。Linux 内核自动为我们选择了一个没有被使用的本地端口。
**listen() 函数**
/*
*
* 等待别人连接，进行系统侦听请求
* 当有人连接你的时候，你有两步需要做：
*     通过listen()函数等待连接请求
*     然后使用accept()函数来处理
*
* 那么我们需要指定本地端口了，因为我们是等待别人的连接。所以，在listen()函数调用之前，我们需要使用bind() 函数来指定使用本地的哪一个端口数值
* 如果你想在一个端口上接受外来的连接请求的话，那么函数的调用顺序为：
*  socket();
*  bind();
*  listen();
*
* sockfd    是一个套接字描述符，由socket()系统调用获得
* backlog   是未经过处理的连接请求队列可以容纳的最大数目(每一个连入请求都要进入一个连入请求队列，等待listen 的程序调用accept()函数来接受这个连接。当系统还没有调用accept()函数的时候，如果有很多连接，那么本地能够等待的最大数目就是 backlog 的数值。你可以将其设成5 到10 之间的数值(推荐))
*
*
* return    错误返回-1, 并设置全局错误代码变量errno
*/
#include 
int listen(int sockfd, int backlog);
**accept()函数**
/*
* 当调用它的时候，大致过程是下面这样的：
*     有人从很远很远的地方尝试调用connect()来连接你的机器上的某个端口（当然是你已经在listen()的）
*     他的连接将被listen 加入等待队列等待accept()函数的调用
*     你调用accept()函数，告诉他你准备连接
*
* sockfd   是正在listen() 的一个套接字描述符
* addr     一般是一个指向struct sockaddr_in 结构的指针；里面存储着远程连接过来的计算机的信息（比如远程计算机的IP 地址和端口）
* addrlen  是一个本地的整型数值，在它的地址传给accept() 前它的值应该是sizeof(struct sockaddr_in)；accept()不会在addr 中存储多余addrlen bytes 大小的数据。如果accept()函数在addr 中存储的数据量不足addrlen，则accept()函数会改变addrlen 的值来反应这个情况。
*
*
* return   accept()函数将回返回一个新的套接字描述符，这个描述符就代表了这个连接 
*          这时候你有了两个套接字描述符:
*             返回给你的那个就是和远程计算机的连接, 这时候你所得到的那个新的套接字描述符就可以进行send()操作和recv()操作了。
*             而第一个套接字描述符仍然在你的机器上原来的那个端口上listen()
*
*          -1 来表明调用失败，同时全局变量errno 将会存储错误代码
*/
#include 
int accept(int sockfd, void *addr, int *addrlen);
示例:
#include 
#include 
#include 
/* 用户连接的端口号 */
#define MYPORT 4000
/* 等待队列中可以存储多少个未经过accept()处理的连接 */
#define BACKLOG 10
main()
{
   /* 用来监听网络连接的套接字sock_fd，用户连入的套接字使用new_fd */
   int sockfd, new_fd ;
   /* 本地的地址信息 */
   struct sockaddr_in my_addr ;
   /* 连接者的地址信息 */
   struct sockaddr_in their_addr ;
   int sin_size;
   /* 记得在自己的程序中这部分要进行错误检查！ */
   sockfd = socket(AF_INET, SOCK_STREAM, 0) ;
   /* 主机字节顺序 */
   my_addr.sin_family = AF_INET ;
   /* 网络字节顺序，短整型 */
   my_addr.sin_port = htons(MYPORT) ;
   /* 自动赋值为自己的IP */
   my_addr.sin_addr.s_addr = INADDR_ANY ;
   /* 将结构中未使用部分全部清零 */
   bzero(&(my_addr.sin_zero), 8) ;
   /* 不要忘记在你自己的程序中下面的程序调用需要进行错误检测！！*/
   bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
   listen(sockfd, BACKLOG);
   sin_size = sizeof(struct sockaddr_in);
   new_fd = accept(sockfd, &their_addr, &sin_size);
**send()、recv()函数**/*
*
*  这两个函数是最基本的，通过连接的套接字流进行通讯的函数
*  如果你想使用无连接的使用者数据报的话，请参sendto() 和recvfrom() 函数。
* 
*  sockfd 是代表你与远程程序连接的套接字描述符。 
*  msg    是一个指针，指向你想发送的信息的地址。
*  len    是你想发送信息的长度
*  flags  发送标记。一般都设为0（你可以查看send 的man pages 来获得其他的参数值并且明白各个参数所代表的含义）
*  
*  return  函数在调用后会返回它真正发送数据的长度
*          -1  如果发生错误，错误代码存储在全局变量errno
*
*/
#include 
#include 
int send(int sockfd, const void *msg, int len, int flags);
示例:
char *msg = "Hello! World! ";
int len, bytes_sent;
len = strlen(msg);
bytes_sent = send(sockfd, msg, len, 0);
注意：
   send() 所发送的数据可能少于你给它的参数所指定的长度！
   因为如果你给send()的参数中包含的数据的长度远远大于send()所能一次发送的数据，则send()函数只发送它所能发送的最大数据长度，然后它相信你会把剩下的数据再次调用它来进行第二次发送。
   所以，记住如果send()函数的返回值小于len 的话，则你需要再次发送剩下的数据。
   幸运的是，如果包足够小（小于1K），那么send()一般都会一次发送光的。
**recv()函数**
/*
*
*  sockfd  是你要读取数据的套接字描述符
*  buf     是一个指针，指向你能存储数据的内存缓存区域
*  len     是缓存区的最大尺寸
*  flags   是recv() 函数的一个标志，一般都为0 （具体的其他数值和含义请参考recv()的man pages）
*
*
*  返回    它所真正收到的数据的长度（也就是存到buf 中数据的长度）
*
*          -1    则代表发生了错误（比如网络以外中断、对方关闭了套接字连接等），全局变量errno 里面存储了错误代码
*/
#include 
#include 
int recv(int sockfd, void *buf, int len, unsigned int flags）;
**sendto() 和recvfrom() 函数**
/*
* 这两个函数是进行无连接的UDP 通讯时使用的。
* 使用这两个函数，则数据会在没有建立过任何连接的网络上传输。因为数据报套接字无法对远程主机进行连接
* 
* sockfd  是代表你与远程程序连接的套接字描述符
* msg     是一个指针，指向你想发送的信息的地址
* len     是你想发送信息的长度
* flags   发送标记。一般都设为0 (你可以查看send 的man pages 来获得其他的参数值并且明白各个参数所代表的含义)
* to      是一个指向struct sockaddr 结构的指针，里面包含了远程主机的IP 地址和端口数据
* tolen   只是指出了struct sockaddr 在内存中的大小sizeof(struct sockaddr)
* 
* return  sendto()返回它所真正发送的字节数（当然也和send()一样，它所真正发送的字节数可能小于你所给它的数据的字节数）
*         -1 表示出错   同时全局变量errno 存储了错误代码
*/
#include 
#include 
int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, int tolen);
**recvfrom()函数**/*
*
* sockfd  是你要读取数据的套接字描述符
* buf     是一个指针，指向你能存储数据的内存缓存区域
* len     是缓存区的最大尺寸
* flags   是recv() 函数的一个标志，一般都为0 （具体的其他数值和含义请参考recv()的man pages）
* from    是一个本地指针，指向一个struct sockaddr 的结构（里面存有源IP 地址和端口数）
* fromlen 是一个指向一个int 型数据的指针，它的大小应该是sizeof (struct sockaddr)．当函数返回的时候，formlen 指向的数据是form 指向的struct sockaddr 的实际大小．
*         如果一个信息大得缓冲区都放不下，那么附加信息将被砍掉。该调用可以立即返回,也可以永久的等待。这取决于你把flags 设置成什么类型。你甚至可以设置超时(timeout)值。
*
*
* return  返回它接收到的字节数，如果发生了错误，它就返回–1 ，全局变量errno存储了错误代码
*
*/
#include 
#include 
int recvfrom(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen);
注意：
   如果你使用cnnect()连接到了一个数据报套接字的服务器程序上，那么你就可以使用send() 和recv() 函数来传输你的数据．
   不要以为你在使用一个流式的套接字，你所使用的仍然是一个使用者数据报的套接字，只不过套接字界面在send() 和recv()的时候自动帮助你加上了目标地址，目标端口的信息．
**close()和shutdown()函数**
/*
* 程序进行网络传输完毕后，你需要关闭这个套接字描述符所表示的连接。实现这个非常简单，只需要使用标准的关闭文件的函数：close()。
* 执行close()之后，套接字将不会在允许进行读操作和写操作。任何有关对套接字描述符进行读和写的操作都会接收到一个错误。
* 
*/
close(sockfd);
/*
* 如果你想对网络套接字的关闭进行进一步的操作的话，你可以使用函数shutdown()
* 它允许你进行单向的关闭操作，或是全部禁止掉。
* 
* 如果你在一个未连接的数据报套接字上使用shutdown() 函数，它将什么也不做．
*
* sockfd 是一个你所想关闭的套接字描述符
* how 可以取下面的值。
*     0 表示不允许以后数据的接收操；
*     1 表示不允许以后数据的发送操作；
*     2 表示和close()一样，不允许以后的任何操作（包括接收，发送数据）
*
* return  0 执行成功
*        -1 执行失败, 全局变量errno 中存储了错误代码．
*   
*/
#include 
int shutdown(int sockfd, int how);
**setsockopt() 和getsockopt() 函数**Linux 所提供的socket 库含有一个错误（bug）。
  此错误表现为你不能为一个套接字重新启用同一个端口号，即使在你正常关闭该套接字以后。
  例如，比方说，你编写一个服务器在一个套接字上等待的程序．服务器打开套接字并在其上侦听是没有问题的。无论如何，总有一些原因（不管是正常还是非正常的结束程序）使你的程序需要重新启动。然而重启动后你就不能把它绑定在原来那个端口上了。从bind()系统调用返回的错误代码总是报告说你试图连接的端口已经被别的进程所绑定。
  问题就是Linux 内核在一个绑定套接字的进程结束后从不把端口标记为未用。在大多数Linux/UNIX 系统中，端口可以被一个进程重复使用，甚至可以被其它进程使用。
  在Linux 中绕开这个问题的办法是，当套接字已经打开但尚未有连接的时候用setsockopt()系统调用在其上设定选项（options）。
  setsockopt() 调用设置选项而getsockopt()从给定的套接字取得选项。
这里是这些调用的语法：
/*
*
*
* sockfd 必须是一个已打开的套接字
* level 是函数所使用的协议标准（protocol level）（TCP/IP 协议使用IPPROTO_TCP，套接字标准的选项实用SOL_SOCKET）
* name 选项在套接字说明书中（man page）有详细说明
* value 指向为getsockopt()函数所获取的值，setsockopt()函数所设置的值的地址
* optlen 指针指向一个整数，该整数包含参数以字节计算的长度
*
*
*
*
*
*/
#include
#include
int getsockopt(int sockfd, int level, int name, char *value, int *optlen);
int setsockopt(int sockfd, int level, int name, char *value, int *optlen);
当你打开一个套接字时必须同时用下面的代码段来调用setsockopt()函数：
/* 设定参数数值 */
opt = 1; 
len = sizeof(opt);
/* 设置套接字属性 */
setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,&len);
**getpeername()函数**
/*
* 这个函数可以取得一个已经连接上的套接字的远程信息（比如IP 地址和端口），告诉你在远程和你连接的究竟是谁
*
* 当你拥有了远程连接用户的IP 地址，你就可以使用inet_ntoa() 或gethostbyaddr()来输出信息或是做进一步的处理
*
* sockfd  是你想取得远程信息的那个套接字描述符
* addr    是一个指向struct sockaddr （或是struct sockaddr_in）的指针
* addrlen 是一个指向int 的指针，应该赋于sizeof(struct sockaddr)的大小
*
*
* return  错误，函数将返回 –1 ，并且错误代码储存在全局变量errno 中
*
*/
#include 
int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);
**gethostname()函数**
/*
* 可以取得本地主机的信息,它返回正在执行它的计算机的名字
* 返回的这个名字可以被gethostbyname()函数使用，由此可以得到本地主机的IP 地址
*
* hostname  是一个指向字符数组的指针，当函数返回的时候，它里面的数据就是本地的主机的名字
* size      是hostname 指向的数组的长度
*
*
* return    成功执行，它返回0
*           错误，则返回–1，全局变量errno 中存储着错误代码
*
*/
#include 
int gethostname(char *hostname, size_t size);
**gethostbyname()函数**
/*
*
* 网络地址是以网络字节顺序存储的
*
*
* return    成功则返回指向结构struct hostent的指针
*
            #define h_addr h_addr_list[0]    //h_addr 是 h_addr_list 数组的第一个成员
            struct hostent 
            {
               char *h_name;       //是这个主机的正式名称
               char **h_aliases;   //是一个以NULL（空字符）结尾的数组，里面存储了主机的备用名称
               int  h_addrtype;    //是返回地址的类型，一般来说是“AF_INET”
               int  h_length;      //是地址的字节长度
               char **h_addr_list; //是一个以0 结尾的数组，存储了主机的网络地址
            };
*
*
*           如果发生错误，它将会返回NULL（但是errno 并不代表错误代码，h_errno 中存储的才识错误代码。参考下面的herror()函数
*/
struct hostent *gethostbyname(const char *name);
**五种I/O 模式**
----------------------------------------
在Linux/UNIX 下，有下面这五种I/O 操作方式：
   阻塞I/O
   非阻塞I/O
   I/O 多路复用
   信号驱动I/O（SIGIO）
   异步I/O
**程序进行输入操作有两步：**   等待有数据可以读
   将数据从系统内核中拷贝到程序的数据区。
**对于一个对套接字的输入操作:**     第一步一般来说是,等待数据从网络上传到本地,当数据包到达的时候，数据将会从网络层拷贝到内核的缓存中；
     第二步是从内核中把数据拷贝到程序的数据区中
**.阻塞I/O 模式**     简单的说，阻塞就是＂睡眠＂的同义词
         如你运行上面的listener 的时候，它只不过是简单的在那里等待接收数据。它调用recvfrom()函数，但是那个时候（listener 调用recvfrom()函数的时候），它并没有数据可以接收．所以recvfrom()函数阻塞在那里（也就是程序停在recvfrom()函数处睡大觉）直到有数据传过来阻塞．你应该明白它的意思。
     阻塞I/O 模式是最普遍使用的I/O 模式。大部分程序使用的都是阻塞模式的I/O 。
     缺省的，一个套接字建立后所处于的模式就是阻塞I/O 模式。
     对于一个UDP 套接字来说，数据就绪的标志比较简单：
         已经收到了一整个数据报 
         没有收到。
     而TCP 这个概念就比较复杂，需要附加一些其他的变量
         一个进程调用recvfrom ，然后系统调用并不返回知道有数据报到达本地系统，然后系统将数据拷贝到进程的缓存中。
        （如果系统调用收到一个中断信号，则它的调用会被中断）我们称这个进程在调用recvfrom 一直到从recvfrom 返回这段时间是阻塞的。
         当recvfrom正常返回时，我们的进程继续它的操作。
**.非阻塞模式I/O**
    当我们将一个套接字设置为非阻塞模式，我们相当于告诉了系统内核：“当我请求的I/O 操作不能够马上完成，你想让我的进程进行休眠等待的时候，不要这么做，请马上返回一个错误给我。”
    如我们开始对recvfrom 的三次调用，因为系统还没有接收到网络数据，所以内核马上返回一个EWOULDBLOCK的错误。
    第四次我们调用recvfrom 函数，一个数据报已经到达了，内核将它拷贝到我们的应用程序的缓冲区中，然后recvfrom 正常返回，我们就可以对接收到的数据进行处理了。
    当一个应用程序使用了非阻塞模式的套接字，它需要使用一个循环来不听的测试是否一个文件描述符有数据可读（称做polling）。
    应用程序不停的polling 内核来检查是否I/O操作已经就绪。这将是一个极浪费CPU 资源的操作。这种模式使用中不是很普遍
**.I/O 多路复用 select()**    在使用I/O 多路技术的时候，我们调用select()函数和poll()函数，在调用它们的时候阻塞，而不是我们来调用recvfrom（或recv）的时候阻塞。
    当我们调用select 函数阻塞的时候，select 函数等待数据报套接字进入读就绪状态。当select 函数返回的时候，也就是套接字可以读取数据的时候。这时候我们就可以调用recvfrom函数来将数据拷贝到我们的程序缓冲区中。
    和阻塞模式相比较，select()和poll()并没有什么高级的地方，而且，在阻塞模式下只需要调用一个函数：读取或发送，在使用了多路复用技术后，我们需要调用两个函数了：先调用select()函数或poll()函数，然后才能进行真正的读写。
    多路复用的高级之处在于，它能同时等待多个文件描述符，而这些文件描述符（套接字描述符）其中的任意一个进入读就绪状态，select()函数就可以返回
    假设我们运行一个网络客户端程序，要同时处理套接字传来的网络数据又要处理本地的标准输入输出。在我们的程序处于阻塞状态等待标准输入的数据的时候，假如服务器端的程序被kill（或是自己Down 掉了），那么服务器程端的TCP 协议会给客户端（我们这端）的TCP 协议发送一个FIN 数据代表终止连接。但是我们的程序阻塞在等待标准输入的数据上，在它读取套接字数据之前（也许是很长一段时间），它不会看见结束标志．我们就不能够使用阻塞模式的套接字。
    I/O多路技术一般在下面这些情况中被使用：
       当一个客户端需要同时处理多个文件描述符的输入输出操作的时候（一般来说是标准的输入输出和网络套接字）， I/O 多路复用技术将会有机会得到使用。
       当程序需要同时进行多个套接字的操作的时候。
       如果一个TCP 服务器程序同时处理正在侦听网络连接的套接字和已经连接好的套接字。
       如果一个服务器程序同时使用TCP 和UDP 协议。
       如果一个服务器同时使用多种服务并且每种服务可能使用不同的协议（比如inetd就是这样的）。
    I/O 多路服用技术并不只局限与网络程序应用上。几乎所有的程序都可以找到应用I/O多路复用的地方。
**fcntl()函数**
     简单的说，阻塞就是＂睡眠＂的同义词.
     如你运行上面的listener 的时候，它只不过是简单的在那里等待接收数据。它调用recvfrom()函数，但是那个时候（listener 调用recvfrom()函数的时候），它并没有数据可以接收．所以recvfrom()函数阻塞在那里（也就是程序停在recvfrom()函数处睡大觉）直到有数据传过来阻塞．你应该明白它的意思。
     当你一开始建立一个套接字描述符的时候，系统内核就被设置为阻塞状态。如果你不想你的套接字描述符是处于阻塞状态的，那么你可以使用函数fcntl()。
     #include 
     #include 
     int fcntl (int fd, int cmd, long arg）;
示例:
     sockfd = socket（AF_INET, SOCK_STREAM, 0）;
     fcntl（sockfd, F_SETFL, O_NONBLOCK）;
     这样将一个套接字设置为无阻塞模式后，你可以对套接字描述符进行有效的“检测”．
     如果你尝试从一个没有接收到任何数据的无阻塞模式的套接字描述符那里读取数据，那么读取函数会马上返回–1 代表发生错误，全局变量errno 中的值为EWOULDBLOCK。
     一般来说，这种无阻塞模式在某些情况下不是一个好的选择。假如你的程序一直没有接收到传过来的数据，那么你的程序就会进行不停的循环来检查是否有数据到来，浪费了大量的CPU 时间，而这些CPU 时间本来可以做其他事情的。
     另外一个比较好的检测套接字描述符的方法是调用select()函数
**套接字选择项select()函数**
/*
*
*  这个技术有一点点奇怪但是它对我们的程序确是非常有用的。
*  假想一下下面的情况：
*      你写的服务器程序想监听客户端的连接，但是你同时又想从你以前已经建立过的连接中来读取数据。
*  你可能会说：“没有问题，我不就是需要使用一个accept()函数和一对儿recv()函数吗？”。
*  不要这么着急，你要想想，当你调用accept()函数阻塞的时候，你还能调用recv()函数吗？
*  “使用非阻塞套接字！”你可能会这么说。是的，你可以。但是如果你又不想浪费宝贵的CPU 时间，该怎么办呢？
*  Select()函数可以帮助你同时监视许多套接字。它会告诉你哪一个套接字已经可以读取数据，
*  哪个套接字已经可以写入数据，甚至你可以知道哪个套接字出现了错误，如果你想知道的话。
* 
*  
*
* 当select()函数返回的时候，readfds 将会被修改用来告诉你哪一个文件描述符你可以用来读取数据。
*
* numfds         是readfds，writefds，exceptfds 中fd 集合中文件描述符中最大的数字加上1 也就是sockfd+1（因为标准输入的文件描述符的值为0 ，所以其他任何的文件描述符都会比标准输入的文件描述符大）。
*
* readfds        中的fd 集合将由select 来监视是否可以读取,如果你想知道是是否可以从标准输入和一些套接字（sockfd）中读取数据，你就可以把文件描述符和sockfd 加入readfds 中。
* writefds       中的fds 集合将由select 来监视是否可以写入
* exceptfds      中的fds 集合将由select 来监视是否有例外发生
* struct timeval 超时设置。 
*                    一般来说，如果没有任何文件描述符满足你的要求，你的程序是不想永远等下去的．也许每隔1 分钟你就想在屏幕上输出信息：“hello！”。
*                这个代表时间的结构将允许你定义一个超时。
*                在调用select()函数中，如果时间超过timeval 参数所代表的时间长度，
*                而还没有文件描述符满足你的要求，那么select()函数将回返回，允许你进行下面的操作。
*                只需要将tv_sec 设置为你想等待的秒数，然后设置tv_usec 为想等待的微秒数
*                (真正的时间就是tv_sec 所表示的秒数加上tv_usec 所表示的微秒数）．注意，是微秒（百万分之一）而不是毫秒．
*                一秒有1,000 毫秒，一毫秒有1,000 微秒。所以，一秒有1,000,000 微秒． 
*                这个timeval 结构定义如下：
*                struct timeval
*                {
*                    int tv_sec ;   //秒数
*                    int tv_usec ;  //微秒
*                };
*                我们拥有了一个以微秒为单位的记时器！但是因为Linux 和UNIX 一样，最小的时间片是100 微秒，所以不管你将tv_usec 设置的多小，实质上记时器的最小单位是100微秒．
* 
*                如果你将struct timeval 设置为0，则select()函数将会立即返回，同时返回在你的集合中的文件描述符的状态。 
*
*                如果你将timeout 这个参数设置为NULL，则select()函数进入阻塞状态，除了等待到文件描述符的状态变化，否则select()函数不会返回。
*
*
* return        当select()函数返回的时候，timeval 中的时间将会被设置为执行为select()后还剩下的时间。
*
*
*
*/
#include 
#include 
#include 
int select(int numfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
**使用FD_ISSET() 宏，你可以选出select()函数执行的结果。**
在进行更深的操作前，我们来看一看怎样处理这些fd_sets。下面这些宏可以是专门进行这类操作的：
  FD_ZERO(fd_set *set)           将一个文件描述符集合清零
  FD_SET(int fd, fd_set *set)    将文件描述符fd 加入集合set 中。
  FD_CLR(int fd, fd_set *set)    将文件描述符fd 从集合set 中删除．
  FD_ISSET(int fd, fd_set *set)  测试文件描述符fd 是否存在于文件描述符set 中．
下面这段代码演示了从标准输入等待输入等待2.5 秒．
#include 
#include 
#include 
/* 标准输入的文件描述符数值 */
#define STDIN 0
main()
{
   fd_set readfds;
   struct timeval tv;
   /* 设置等待时间为2 秒零500,000 微秒 */
   tv.tv_sec  = 2;
   tv.tv_usec = 500000;
   FD_ZERO(&readfds);
   FD_SET(STDIN, &readfds);
   /* 因为我们只想等待输入，所以将writefds 和execeptfds 设为NULL */
   /* 程序将会在这里等待2 秒零500,000 微秒，除非在这段时间中标准输入有操作 */
   select(STDIN+1, &readfds, NULL, NULL, &tv);
   /* 测试ST
**Socket编程知识必学**
**/*** Author: cnscn* HOME:  [http://www.cnscn.org](http://www.cnscn.org/)* 整理自《Linux网络编程》*/端口号常识:**
    端口号被从1 开始分配。
    通常端口号超出255 的部分被本地主机保留为私有用途。
    1到255 之间的号码被用于远程应用程序所请求的进程和网络服务。
    每个网络通信循环地进出主计算机的TCP 应用层。它被两个所连接的号码唯一地识别。这两个号码合起来叫做套接字.
        组成套接字的这两个号码就是机器的IP 地址和TCP 软件所使用的端口号。
**套接字的三种类型**
    流式套接字（SOCK_STREAM），数据报套接字（SOCK_DGRAM）及原始套接字(RAW)。
**  流式套接字（SOCK_STREAM）**
    流式的套接字可以提供可靠的、面向连接的通讯流。
    如果你通过流式套接字发送了顺序的数据：“1”、“２”。那么数据到达远程时候的顺序也是“１”、“２”。
    Telnet 应用程序、BBS服务、以及系统的远程登陆都是通过Telnet 协议连接的。Telnet 就是一个流式连接。你是否希望你在Telnet 应用程序上输入的字符（或汉字）在到达远程应用程序的时候是以你输入的顺序到达的？答案应该是肯定的吧。
    还有WWW 浏览器，它使用的HTTP 协议也是通过流式套接字来获取网页的。事实上，如果你Telnet 到一个Web Site 的80 端口上，然后输入 “GET 网页路径名”然后按两下回车（或者是两下Ctrl+回车）然后你就得到了“网页路径名”所代表的网页！
**  数据报套接字（SOCK_DGRAM）**
    数据报套接字定义了一种无连接的服务，数据通过相互独立的报文进行传输，是无序的，并且不保证可靠，无差错。
       如果你发送了一个数据报，它可能不会到达。
       它可能会以不同的顺序到达。
       如果它到达了，它包含的数据中可能存在错误。
       数据报套接字也使用IP，但是它不使用TCP，它使用使用者数据报协议UDP（User Datagram Protocol 可以参考RFC 768）
   为什么说它们是“无连接”的呢？因为它（UDP）不像流式套接字那样维护一个打开的连接，你只需要把数据打成一个包，把远程的IP 贴上去，然后把这个包发送出去。这个过程是不需要建立连接的。
       UDP 的应用例子有： tftp, bootp 等。
   那么，数据包既然会丢失，怎样能保证程序能够正常工作呢？
       事实上，每个使用UDP的程序都要有自己的对数据进行确认的协议。
           比如， TFTP 协议定义了对于每一个发送出去的数据包，远程在接受到之后都要回送一个数据包告诉本地程序：“我已经拿到了！”（一个 “ACK” 包）。如果数据包发的送者在5 秒内没有的得到回应，它就会重新发送这个数据包直到数据包接受者回送了 “ACK” 信号。这些知识对编写一个使用UDP 协议的程序员来说是非常必要的。
   无连接服务器一般都是面向事务处理的，一个请求一个应答就完成了客户程序与服务程序之间的相互作用。
   面向连接服务器处理的请求往往比较复杂，不是一来一去的请求应答所能解决的，而且往往是并发服务器
   套接字工作过程如下:
         服务器首先启动
         通过调用socket()建立一个套接字，
         然后调用bind()将该套接字和本地网络地址联系在一起，
         再调用listen()使套接字做好侦听的准备，并规定它的请求队列的长度,
         之后就调用accept()来接收连接。
         客户在建立套接字
         然后就可调用connect()和服务器建立连接。
         客户机和服务器之间就可以通过调用read()和write()来发送和接收数据。
         最后，待数据传送结束后，双方调用close()关闭套接字。
对流式套接字你所需要做的只是调用send() 函数来发送数据。而对于数据报套接字，你需要自己加个信息头，然后调用sendto() 函数把数据发送出去
**  原始套接字**
     原始套接字主要用于一些协议的开发，可以进行比较底层的操作。它功能强大，但是没有上面介绍的两种套接字使用方便，一般的程序也涉及不到原始套接字
**套接字结构**
struct sockaddr
这个结构用来存储套接字地址。
数据定义：
struct sockaddr 
{
    unsigned short sa_family;  /* address族, AF_xxx */
    char sa_data[14];              /* 14 bytes的协议地址 */
};
sa_family  一般来说，都是 “AF_INET”。
sa_data    包含了一些远程电脑的地址、端口和套接字的数目，它里面的数据是杂溶在一切的。
为了处理struct sockaddr， 程序员建立了另外一个相似的结构struct sockaddr_in (“in” 代表 “Internet”):
struct sockaddr_in 
{
   short     int       sin_family;           /* Internet地址族 */
   unsigned  short int sin_port;       /* 端口号 */
   struct    in_addr   sin_addr;         /* Internet地址 */
   unsigned  char      sin_zero[8];   /* 添0（和struct sockaddr一样大小）*/
};
注意:
    1)这个结构提供了方便的手段来访问socket address（struct sockaddr）结构中的每一个元素。
    2)sin_zero[8] 是为了是两个结构在内存中具有相同的尺寸
                  要把sin_zero 全部设成零值（使用bzero()或memset()函数）。
    3)一个指向struct sockaddr_in 的指针可以声明指向一个sturct sockaddr 的结构。所以虽然socket() 函数需要一个structaddr * ，你也可以给他一个sockaddr_in * 。
    4)在struct  sockaddr_in 中，sin_family 相当于 在struct sockaddr 中的sa_family，需要设成 “AF_INET”。
    5)一定要保证sin_port 和sin_addr 必须是网络字节顺序（见下节）！
2．struct in_addr  ( 因特网地址 (a structure for historical reasons) )
   struct in_addr 
   {
       unsigned long s_addr;
   };
   如果你声明了一个 "ina" 作为一个struct sockaddr_in 的结构， 那么“ina.sin_addr.s_addr”就是4 个字节的IP 地址（按网络字节顺序排放）。
   需要注意的是，即使你的系统仍然使用联合而不是结构来表示struct in_addr，你仍然可以用上面的方法得到4 个字节的IP 地址（一些 #defines 帮了你的忙）
**网络字节顺序**
   因为每一个机器内部对变量的字节存储顺序不同（有的系统是高位在前，底位在后，而有的系统是底位在前，高位在后），而网络传输的数据大家是一定要统一顺序的。
   所以对与内部字节表示顺序和网络字节顺序不同的机器，就一定要对数据进行转换（比如IP 地址的表示，端口号的表示）。
   但是内部字节顺序和网络字节顺序相同的机器该怎么办呢？是这样的：它们也要调用转换函数，但是真正转换还是不转换是由系统函数自己来决定的。
**有关的转化函数**      我们通常使用的有两种数据类型：短型（两个字节）和长型（四个字节）。
      下面介绍的这些转换函数对于这两类的无符号整型变量都可以进行正确的转换。
      如果你想将一个短型数据从主机字节顺序转换到网络字节顺序的话，有这样一个函数htons：
         它是以“h”开头的（代表“主机”）；
         紧跟着它的是“to”，代表“转换到”；
         然后是“n”代表“网络”；
         最后是“s”，代表“短型数据”。
         H-to-n-s，就是htons() 函数（可以使用Hostto Network Short 来助记）
      你可以使用 “n”，“h”，“to”，“s”，“l”的任意组合.当然，你要在可能的情况下进行组合。比如，系统是没有stolh() 函数的（Short to Long Host?）。
        下面给出套接字字节转换程序的列表：
          htons()——“Host to Network Short” 主机字节顺序转换为网络字节顺序（对无符号短型进行操作4 bytes）
          htonl()——“Host to Network Long”　主机字节顺序转换为网络字节顺序（对无符号长型进行操作8 bytes）
          ntohs()——“Network to Host Short “　网络字节顺序转换为主机字节顺序（对无符号短型进行操作4 bytes）
          ntohl()——“Network to Host Long “　网络字节顺序转换为主机字节顺序（对无符号长型进行操作8 bytes）
     在struct sockaddr_in 中的sin_addr 和sin_port 他们的字节顺序都是网络字节顺序，而sin_family 却不是网络字节顺序的。为什么呢？
        这个是因为sin_addr 和sin_port 是从IP 和UDP 协议层取出来的数据，而在IP 和UDP协议层，是直接和网络相关的，所以，它们必须使用网络字节顺序。
        然而， sin_family 域只是内核用来判断struct sockaddr_in 是存储的什么类型的数据，并且， sin_family 永远也不会被发送到网络上，所以可以使用主机字节顺序来存储
**socket() 函数**
   /**
   * 
   * 取得套接字描述符！（记得我们以前说过的吗？它其实就是一个文件描述符）
   *
   * domain   需要被设置为 “AF_INET”，就像上面的struct sockaddr_in。
   * type     参数告诉内核这个socket 是什么类型，“SOCK_STREAM”或是“SOCK_DGRAM”。
   * protocol 通常为0
   *
   * return   如果发生错误，socket()函数返回 –1 。全局变量errno 将被设置为错误代码。
   *
   */
   #include 
   #include 
   int socket（int domain , int type , int protocol）
   示例:
   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
        perror("create sock");
        return -1;
   }
   else
   {
      // printf("socket created...\n");
   }
**bind() 函数**
/**
*
* 为套接字绑定一个端口号
*
* 当你需要进行端口监听listen()操作，等待接受一个连入请求的时候，
* 一般都需要经过这一步。比如网络泥巴(MUD)，Telnet a.b.c.d 4000
* 
* 如果你只是想进行连接一台服务器，也就是进行connect() 操作的时候，这一步并不是必须的。 
*
*
* sockfd    是由socket()函数返回的套接字描述符
* my_addr   是一个指向struct sockaddr 的指针，包含有关你的地址的信息：名称、端口和IP 地址。
* addrlen   可以设置为sizeof(struct sockaddr)
* 
* return    调用错误的时候，返回 -1 作为错误发生的标志。errno 的值为错误代码。
*
*/
#include 
#include 
int bind (int sockfd , struct sockaddr *my_addr , int addrlen) ;
示例:
#include 
#include 
#include 
#define  MYPORT 4000
main()
{
  int sockfd ;
  struct sockaddr_in my_addr ;
  sockfd                  = socket(AF_INET, SOCK_STREAM, 0); /* 在你自己的程序中要进行错误检查！! */
  my_addr.sin_family      = AF_INET ;        /* 主机字节顺序 */
  my_addr.sin_port        = htons(MYPORT);   /* 网络字节顺序，短整型 */
  my_addr.sin_addr.s_addr = inet_addr("166.111.69.52") ;
  bzero(&(my_addr.sin_zero), 8);             /* 将整个结构剩余部分数据设为0 */
  bind (sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));  /* 不要忘记在你自己的程序中加入判断bind 错误的代码！！ */
  注意：
    my_addr.sin_port         是网络字节顺序, 短整型
    my_addr.sin_addr.s_addr  也是网络字节顺序。
  最后，bind()可以在程序中自动获取你自己的IP 地址和端口。
    my_addr.sin_port = 0 ;                 /* 随机选择一个端口 */
    my_addr.sin_addr.s_addr = INADDR_ANY ; /* 使用自己的地址 */
    如上，通过设置my_addr.sin_port 为0，bind()可以知道你要它帮你选择合适的端口；
    通过设置my_addr.sin_addr.s_addr 为INADDR_ANY，bind()知道你要它将s_addr 填充为运行这个进程的机器的IP。
    这一切都可以要求bind()来自动的帮助你完成。
    如果你注意到了一些细节的话，你可能会发现我并没有将INADDR_ANY 转换为网络字节顺序！是这样的，INADDR_ANY的值为0，0 就是0，无论用什么顺序排列位的顺序，它都是不变的。
    有读者会想了，因为我用的INADDR_ANY 是一个#define，那么如果将我的程序移植到另外一个系统，假如那里的INADDR_ANY是这样定义的：
       #define INADDR_ANY 100，
       那么我的程序不是就会不运行了吗？那么下面这段代码就OK 了:
       my_addr.sin_port = htons(0);                   /* 随机选择一个未用的端口 */
       my_addr.sin_addr.s_addr = htonl(INADDR_ANY) ;  /* 使用自己的IP地址 */
    现在我们已经是这么的严谨，对于任何数值的INADDR_ANY调用bind 的时候就都不会有麻烦了。
   另外一件必须指出的事情是：
       当你调用bind()的时候，不要把端口数设置的过小！小于1024 的所有端口都是保留下来作为系统使用端口的，没有root 权利无法使用。你可以使用1024 以上的任何端口，一直到65535 ：你所可能使用的最大的端口号（当然，你还要保证你所希望使用的端口没有被其他程序所使用）。
   最后注意有关bind()的是：
       有时候你并不一定要调用bind()来建立网络连接。比如你只是想连接到一个远程主机上面进行通讯，你并不在乎你究竟是用的自己机器上的哪个端口进行通讯（比如Telnet），那么你可以简单的直接调用connect()函数，connect()将自动寻找出本地机器上的一个未使用的端口，然后调用 bind()来将其socket 绑定到那个端口上。
**connect() 函数**
/*
*
* sockfd     套接字文件描述符，由socket()函数返回的
* serv_addr  是一个存储远程计算机的IP 地址和端口信息的结构
* addrlen    应该是sizeof(struct sockaddr)
*
* return     如果发生了错误（比如无法连接到远程主机，或是远程主机的指定端口无法进行连接等）它将会返回错误值 -1
*            全局变量errno将会存储错误代码
*
*
*/
#include 
#include 
int connect (int sockfd, struct sockaddr *serv_addr, int addrlen);
示例:
#include 
#include 
#include 
#define DEST_IP “166.111.69.52”
#define DEST_PORT 23
main()
{
   int sockfd ;
   /* 将用来存储远程信息 */
   struct sockaddr_in dest_addr ;
   /* 注意在你自己的程序中进行错误检查！！ */
   sockfd = socket（AF_INET, SOCK_STREAM, 0）;
   /* 主机字节顺序 */
   dest_addr.sin_family = AF_INET ;
   /* 网络字节顺序，短整型 */
   dest_addr.sin_port = htons(DEST_PORT(;
   dest_addr.sin_addr.s_addr = inet_addr(DEST_IP);
   /* 将剩下的结构中的空间置0 */
   bzero(&(dest_addr.sin_zero), 8）;
   /* 不要忘记在你的代码中对connect()进行错误检查！！ */
   connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr));
   注意我们没有调用bind()函数。基本上，我们并不在乎我们本地用什么端口来通讯，是不是？我们在乎的是我们连到哪台主机上的哪个端口上。Linux 内核自动为我们选择了一个没有被使用的本地端口。
**listen() 函数**
/*
*
* 等待别人连接，进行系统侦听请求
* 当有人连接你的时候，你有两步需要做：
*     通过listen()函数等待连接请求
*     然后使用accept()函数来处理
*
* 那么我们需要指定本地端口了，因为我们是等待别人的连接。所以，在listen()函数调用之前，我们需要使用bind() 函数来指定使用本地的哪一个端口数值
* 如果你想在一个端口上接受外来的连接请求的话，那么函数的调用顺序为：
*  socket();
*  bind();
*  listen();
*
* sockfd    是一个套接字描述符，由socket()系统调用获得
* backlog   是未经过处理的连接请求队列可以容纳的最大数目(每一个连入请求都要进入一个连入请求队列，等待listen 的程序调用accept()函数来接受这个连接。当系统还没有调用accept()函数的时候，如果有很多连接，那么本地能够等待的最大数目就是 backlog 的数值。你可以将其设成5 到10 之间的数值(推荐))
*
*
* return    错误返回-1, 并设置全局错误代码变量errno
*/
#include 
int listen(int sockfd, int backlog);
**accept()函数**
/*
* 当调用它的时候，大致过程是下面这样的：
*     有人从很远很远的地方尝试调用connect()来连接你的机器上的某个端口（当然是你已经在listen()的）
*     他的连接将被listen 加入等待队列等待accept()函数的调用
*     你调用accept()函数，告诉他你准备连接
*
* sockfd   是正在listen() 的一个套接字描述符
* addr     一般是一个指向struct sockaddr_in 结构的指针；里面存储着远程连接过来的计算机的信息（比如远程计算机的IP 地址和端口）
* addrlen  是一个本地的整型数值，在它的地址传给accept() 前它的值应该是sizeof(struct sockaddr_in)；accept()不会在addr 中存储多余addrlen bytes 大小的数据。如果accept()函数在addr 中存储的数据量不足addrlen，则accept()函数会改变addrlen 的值来反应这个情况。
*
*
* return   accept()函数将回返回一个新的套接字描述符，这个描述符就代表了这个连接 
*          这时候你有了两个套接字描述符:
*             返回给你的那个就是和远程计算机的连接, 这时候你所得到的那个新的套接字描述符就可以进行send()操作和recv()操作了。
*             而第一个套接字描述符仍然在你的机器上原来的那个端口上listen()
*
*          -1 来表明调用失败，同时全局变量errno 将会存储错误代码
*/
#include 
int accept(int sockfd, void *addr, int *addrlen);
示例:
#include 
#include 
#include 
/* 用户连接的端口号 */
#define MYPORT 4000
/* 等待队列中可以存储多少个未经过accept()处理的连接 */
#define BACKLOG 10
main()
{
   /* 用来监听网络连接的套接字sock_fd，用户连入的套接字使用new_fd */
   int sockfd, new_fd ;
   /* 本地的地址信息 */
   struct sockaddr_in my_addr ;
   /* 连接者的地址信息 */
   struct sockaddr_in their_addr ;
   int sin_size;
   /* 记得在自己的程序中这部分要进行错误检查！ */
   sockfd = socket(AF_INET, SOCK_STREAM, 0) ;
   /* 主机字节顺序 */
   my_addr.sin_family = AF_INET ;
   /* 网络字节顺序，短整型 */
   my_addr.sin_port = htons(MYPORT) ;
   /* 自动赋值为自己的IP */
   my_addr.sin_addr.s_addr = INADDR_ANY ;
   /* 将结构中未使用部分全部清零 */
   bzero(&(my_addr.sin_zero), 8) ;
   /* 不要忘记在你自己的程序中下面的程序调用需要进行错误检测！！*/
   bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr));
   listen(sockfd, BACKLOG);
   sin_size = sizeof(struct sockaddr_in);
   new_fd = accept(sockfd, &their_addr, &sin_size);
**send()、recv()函数**/*
*
*  这两个函数是最基本的，通过连接的套接字流进行通讯的函数
*  如果你想使用无连接的使用者数据报的话，请参sendto() 和recvfrom() 函数。
* 
*  sockfd 是代表你与远程程序连接的套接字描述符。 
*  msg    是一个指针，指向你想发送的信息的地址。
*  len    是你想发送信息的长度
*  flags  发送标记。一般都设为0（你可以查看send 的man pages 来获得其他的参数值并且明白各个参数所代表的含义）
*  
*  return  函数在调用后会返回它真正发送数据的长度
*          -1  如果发生错误，错误代码存储在全局变量errno
*
*/
#include 
#include 
int send(int sockfd, const void *msg, int len, int flags);
示例:
char *msg = "Hello! World! ";
int len, bytes_sent;
len = strlen(msg);
bytes_sent = send(sockfd, msg, len, 0);
注意：
   send() 所发送的数据可能少于你给它的参数所指定的长度！
   因为如果你给send()的参数中包含的数据的长度远远大于send()所能一次发送的数据，则send()函数只发送它所能发送的最大数据长度，然后它相信你会把剩下的数据再次调用它来进行第二次发送。
   所以，记住如果send()函数的返回值小于len 的话，则你需要再次发送剩下的数据。
   幸运的是，如果包足够小（小于1K），那么send()一般都会一次发送光的。
**recv()函数**
/*
*
*  sockfd  是你要读取数据的套接字描述符
*  buf     是一个指针，指向你能存储数据的内存缓存区域
*  len     是缓存区的最大尺寸
*  flags   是recv() 函数的一个标志，一般都为0 （具体的其他数值和含义请参考recv()的man pages）
*
*
*  返回    它所真正收到的数据的长度（也就是存到buf 中数据的长度）
*
*          -1    则代表发生了错误（比如网络以外中断、对方关闭了套接字连接等），全局变量errno 里面存储了错误代码
*/
#include 
#include 
int recv(int sockfd, void *buf, int len, unsigned int flags）;
**sendto() 和recvfrom() 函数**
/*
* 这两个函数是进行无连接的UDP 通讯时使用的。
* 使用这两个函数，则数据会在没有建立过任何连接的网络上传输。因为数据报套接字无法对远程主机进行连接
* 
* sockfd  是代表你与远程程序连接的套接字描述符
* msg     是一个指针，指向你想发送的信息的地址
* len     是你想发送信息的长度
* flags   发送标记。一般都设为0 (你可以查看send 的man pages 来获得其他的参数值并且明白各个参数所代表的含义)
* to      是一个指向struct sockaddr 结构的指针，里面包含了远程主机的IP 地址和端口数据
* tolen   只是指出了struct sockaddr 在内存中的大小sizeof(struct sockaddr)
* 
* return  sendto()返回它所真正发送的字节数（当然也和send()一样，它所真正发送的字节数可能小于你所给它的数据的字节数）
*         -1 表示出错   同时全局变量errno 存储了错误代码
*/
#include 
#include 
int sendto(int sockfd, const void *msg, int len, unsigned int flags, const struct sockaddr *to, int tolen);
**recvfrom()函数**/*
*
* sockfd  是你要读取数据的套接字描述符
* buf     是一个指针，指向你能存储数据的内存缓存区域
* len     是缓存区的最大尺寸
* flags   是recv() 函数的一个标志，一般都为0 （具体的其他数值和含义请参考recv()的man pages）
* from    是一个本地指针，指向一个struct sockaddr 的结构（里面存有源IP 地址和端口数）
* fromlen 是一个指向一个int 型数据的指针，它的大小应该是sizeof (struct sockaddr)．当函数返回的时候，formlen 指向的数据是form 指向的struct sockaddr 的实际大小．
*         如果一个信息大得缓冲区都放不下，那么附加信息将被砍掉。该调用可以立即返回,也可以永久的等待。这取决于你把flags 设置成什么类型。你甚至可以设置超时(timeout)值。
*
*
* return  返回它接收到的字节数，如果发生了错误，它就返回–1 ，全局变量errno存储了错误代码
*
*/
#include 
#include 
int recvfrom(int sockfd, void *buf, int len, unsigned int flags, struct sockaddr *from, int *fromlen);
注意：
   如果你使用cnnect()连接到了一个数据报套接字的服务器程序上，那么你就可以使用send() 和recv() 函数来传输你的数据．
   不要以为你在使用一个流式的套接字，你所使用的仍然是一个使用者数据报的套接字，只不过套接字界面在send() 和recv()的时候自动帮助你加上了目标地址，目标端口的信息．
**close()和shutdown()函数**
/*
* 程序进行网络传输完毕后，你需要关闭这个套接字描述符所表示的连接。实现这个非常简单，只需要使用标准的关闭文件的函数：close()。
* 执行close()之后，套接字将不会在允许进行读操作和写操作。任何有关对套接字描述符进行读和写的操作都会接收到一个错误。
* 
*/
close(sockfd);
/*
* 如果你想对网络套接字的关闭进行进一步的操作的话，你可以使用函数shutdown()
* 它允许你进行单向的关闭操作，或是全部禁止掉。
* 
* 如果你在一个未连接的数据报套接字上使用shutdown() 函数，它将什么也不做．
*
* sockfd 是一个你所想关闭的套接字描述符
* how 可以取下面的值。
*     0 表示不允许以后数据的接收操；
*     1 表示不允许以后数据的发送操作；
*     2 表示和close()一样，不允许以后的任何操作（包括接收，发送数据）
*
* return  0 执行成功
*        -1 执行失败, 全局变量errno 中存储了错误代码．
*   
*/
#include 
int shutdown(int sockfd, int how);
**setsockopt() 和getsockopt() 函数**Linux 所提供的socket 库含有一个错误（bug）。
  此错误表现为你不能为一个套接字重新启用同一个端口号，即使在你正常关闭该套接字以后。
  例如，比方说，你编写一个服务器在一个套接字上等待的程序．服务器打开套接字并在其上侦听是没有问题的。无论如何，总有一些原因（不管是正常还是非正常的结束程序）使你的程序需要重新启动。然而重启动后你就不能把它绑定在原来那个端口上了。从bind()系统调用返回的错误代码总是报告说你试图连接的端口已经被别的进程所绑定。
  问题就是Linux 内核在一个绑定套接字的进程结束后从不把端口标记为未用。在大多数Linux/UNIX 系统中，端口可以被一个进程重复使用，甚至可以被其它进程使用。
  在Linux 中绕开这个问题的办法是，当套接字已经打开但尚未有连接的时候用setsockopt()系统调用在其上设定选项（options）。
  setsockopt() 调用设置选项而getsockopt()从给定的套接字取得选项。
这里是这些调用的语法：
/*
*
*
* sockfd 必须是一个已打开的套接字
* level 是函数所使用的协议标准（protocol level）（TCP/IP 协议使用IPPROTO_TCP，套接字标准的选项实用SOL_SOCKET）
* name 选项在套接字说明书中（man page）有详细说明
* value 指向为getsockopt()函数所获取的值，setsockopt()函数所设置的值的地址
* optlen 指针指向一个整数，该整数包含参数以字节计算的长度
*
*
*
*
*
*/
#include
#include
int getsockopt(int sockfd, int level, int name, char *value, int *optlen);
int setsockopt(int sockfd, int level, int name, char *value, int *optlen);
当你打开一个套接字时必须同时用下面的代码段来调用setsockopt()函数：
/* 设定参数数值 */
opt = 1; 
len = sizeof(opt);
/* 设置套接字属性 */
setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,&len);
**getpeername()函数**
/*
* 这个函数可以取得一个已经连接上的套接字的远程信息（比如IP 地址和端口），告诉你在远程和你连接的究竟是谁
*
* 当你拥有了远程连接用户的IP 地址，你就可以使用inet_ntoa() 或gethostbyaddr()来输出信息或是做进一步的处理
*
* sockfd  是你想取得远程信息的那个套接字描述符
* addr    是一个指向struct sockaddr （或是struct sockaddr_in）的指针
* addrlen 是一个指向int 的指针，应该赋于sizeof(struct sockaddr)的大小
*
*
* return  错误，函数将返回 –1 ，并且错误代码储存在全局变量errno 中
*
*/
#include 
int getpeername(int sockfd, struct sockaddr *addr, int *addrlen);
**gethostname()函数**
/*
* 可以取得本地主机的信息,它返回正在执行它的计算机的名字
* 返回的这个名字可以被gethostbyname()函数使用，由此可以得到本地主机的IP 地址
*
* hostname  是一个指向字符数组的指针，当函数返回的时候，它里面的数据就是本地的主机的名字
* size      是hostname 指向的数组的长度
*
*
* return    成功执行，它返回0
*           错误，则返回–1，全局变量errno 中存储着错误代码
*
*/
#include 
int gethostname(char *hostname, size_t size);
**gethostbyname()函数**
/*
*
* 网络地址是以网络字节顺序存储的
*
*
* return    成功则返回指向结构struct hostent的指针
*
            #define h_addr h_addr_list[0]    //h_addr 是 h_addr_list 数组的第一个成员
            struct hostent 
            {
               char *h_name;       //是这个主机的正式名称
               char **h_aliases;   //是一个以NULL（空字符）结尾的数组，里面存储了主机的备用名称
               int  h_addrtype;    //是返回地址的类型，一般来说是“AF_INET”
               int  h_length;      //是地址的字节长度
               char **h_addr_list; //是一个以0 结尾的数组，存储了主机的网络地址
            };
*
*
*           如果发生错误，它将会返回NULL（但是errno 并不代表错误代码，h_errno 中存储的才识错误代码。参考下面的herror()函数
*/
struct hostent *gethostbyname(const char *name);
**五种I/O 模式**
----------------------------------------
在Linux/UNIX 下，有下面这五种I/O 操作方式：
   阻塞I/O
   非阻塞I/O
   I/O 多路复用
   信号驱动I/O（SIGIO）
   异步I/O
**程序进行输入操作有两步：**   等待有数据可以读
   将数据从系统内核中拷贝到程序的数据区。
**对于一个对套接字的输入操作:**     第一步一般来说是,等待数据从网络上传到本地,当数据包到达的时候，数据将会从网络层拷贝到内核的缓存中；
     第二步是从内核中把数据拷贝到程序的数据区中
**.阻塞I/O 模式**     简单的说，阻塞就是＂睡眠＂的同义词
         如你运行上面的listener 的时候，它只不过是简单的在那里等待接收数据。它调用recvfrom()函数，但是那个时候（listener 调用recvfrom()函数的时候），它并没有数据可以接收．所以recvfrom()函数阻塞在那里（也就是程序停在recvfrom()函数处睡大觉）直到有数据传过来阻塞．你应该明白它的意思。
     阻塞I/O 模式是最普遍使用的I/O 模式。大部分程序使用的都是阻塞模式的I/O 。
     缺省的，一个套接字建立后所处于的模式就是阻塞I/O 模式。
     对于一个UDP 套接字来说，数据就绪的标志比较简单：
         已经收到了一整个数据报 
         没有收到。
     而TCP 这个概念就比较复杂，需要附加一些其他的变量
         一个进程调用recvfrom ，然后系统调用并不返回知道有数据报到达本地系统，然后系统将数据拷贝到进程的缓存中。
        （如果系统调用收到一个中断信号，则它的调用会被中断）我们称这个进程在调用recvfrom 一直到从recvfrom 返回这段时间是阻塞的。
         当recvfrom正常返回时，我们的进程继续它的操作。
**.非阻塞模式I/O**
    当我们将一个套接字设置为非阻塞模式，我们相当于告诉了系统内核：“当我请求的I/O 操作不能够马上完成，你想让我的进程进行休眠等待的时候，不要这么做，请马上返回一个错误给我。”
    如我们开始对recvfrom 的三次调用，因为系统还没有接收到网络数据，所以内核马上返回一个EWOULDBLOCK的错误。
    第四次我们调用recvfrom 函数，一个数据报已经到达了，内核将它拷贝到我们的应用程序的缓冲区中，然后recvfrom 正常返回，我们就可以对接收到的数据进行处理了。
    当一个应用程序使用了非阻塞模式的套接字，它需要使用一个循环来不听的测试是否一个文件描述符有数据可读（称做polling）。
    应用程序不停的polling 内核来检查是否I/O操作已经就绪。这将是一个极浪费CPU 资源的操作。这种模式使用中不是很普遍
**.I/O 多路复用 select()**    在使用I/O 多路技术的时候，我们调用select()函数和poll()函数，在调用它们的时候阻塞，而不是我们来调用recvfrom（或recv）的时候阻塞。
    当我们调用select 函数阻塞的时候，select 函数等待数据报套接字进入读就绪状态。当select 函数返回的时候，也就是套接字可以读取数据的时候。这时候我们就可以调用recvfrom函数来将数据拷贝到我们的程序缓冲区中。
    和阻塞模式相比较，select()和poll()并没有什么高级的地方，而且，在阻塞模式下只需要调用一个函数：读取或发送，在使用了多路复用技术后，我们需要调用两个函数了：先调用select()函数或poll()函数，然后才能进行真正的读写。
    多路复用的高级之处在于，它能同时等待多个文件描述符，而这些文件描述符（套接字描述符）其中的任意一个进入读就绪状态，select()函数就可以返回
    假设我们运行一个网络客户端程序，要同时处理套接字传来的网络数据又要处理本地的标准输入输出。在我们的程序处于阻塞状态等待标准输入的数据的时候，假如服务器端的程序被kill（或是自己Down 掉了），那么服务器程端的TCP 协议会给客户端（我们这端）的TCP 协议发送一个FIN 数据代表终止连接。但是我们的程序阻塞在等待标准输入的数据上，在它读取套接字数据之前（也许是很长一段时间），它不会看见结束标志．我们就不能够使用阻塞模式的套接字。
    I/O多路技术一般在下面这些情况中被使用：
       当一个客户端需要同时处理多个文件描述符的输入输出操作的时候（一般来说是标准的输入输出和网络套接字）， I/O 多路复用技术将会有机会得到使用。
       当程序需要同时进行多个套接字的操作的时候。
       如果一个TCP 服务器程序同时处理正在侦听网络连接的套接字和已经连接好的套接字。
       如果一个服务器程序同时使用TCP 和UDP 协议。
       如果一个服务器同时使用多种服务并且每种服务可能使用不同的协议（比如inetd就是这样的）。
    I/O 多路服用技术并不只局限与网络程序应用上。几乎所有的程序都可以找到应用I/O多路复用的地方。
**fcntl()函数**
     简单的说，阻塞就是＂睡眠＂的同义词.
     如你运行上面的listener 的时候，它只不过是简单的在那里等待接收数据。它调用recvfrom()函数，但是那个时候（listener 调用recvfrom()函数的时候），它并没有数据可以接收．所以recvfrom()函数阻塞在那里（也就是程序停在recvfrom()函数处睡大觉）直到有数据传过来阻塞．你应该明白它的意思。
     当你一开始建立一个套接字描述符的时候，系统内核就被设置为阻塞状态。如果你不想你的套接字描述符是处于阻塞状态的，那么你可以使用函数fcntl()。
     #include 
     #include 
     int fcntl (int fd, int cmd, long arg）;
示例:
     sockfd = socket（AF_INET, SOCK_STREAM, 0）;
     fcntl（sockfd, F_SETFL, O_NONBLOCK）;
     这样将一个套接字设置为无阻塞模式后，你可以对套接字描述符进行有效的“检测”．
     如果你尝试从一个没有接收到任何数据的无阻塞模式的套接字描述符那里读取数据，那么读取函数会马上返回–1 代表发生错误，全局变量errno 中的值为EWOULDBLOCK。
     一般来说，这种无阻塞模式在某些情况下不是一个好的选择。假如你的程序一直没有接收到传过来的数据，那么你的程序就会进行不停的循环来检查是否有数据到来，浪费了大量的CPU 时间，而这些CPU 时间本来可以做其他事情的。
     另外一个比较好的检测套接字描述符的方法是调用select()函数
**套接字选择项select()函数**
/*
*
*  这个技术有一点点奇怪但是它对我们的程序确是非常有用的。
*  假想一下下面的情况：
*      你写的服务器程序想监听客户端的连接，但是你同时又想从你以前已经建立过的连接中来读取数据。
*  你可能会说：“没有问题，我不就是需要使用一个accept()函数和一对儿recv()函数吗？”。
*  不要这么着急，你要想想，当你调用accept()函数阻塞的时候，你还能调用recv()函数吗？
*  “使用非阻塞套接字！”你可能会这么说。是的，你可以。但是如果你又不想浪费宝贵的CPU 时间，该怎么办呢？
*  Select()函数可以帮助你同时监视许多套接字。它会告诉你哪一个套接字已经可以读取数据，
*  哪个套接字已经可以写入数据，甚至你可以知道哪个套接字出现了错误，如果你想知道的话。
* 
*  
*
* 当select()函数返回的时候，readfds 将会被修改用来告诉你哪一个文件描述符你可以用来读取数据。
*
* numfds         是readfds，writefds，exceptfds 中fd 集合中文件描述符中最大的数字加上1 也就是sockfd+1（因为标准输入的文件描述符的值为0 ，所以其他任何的文件描述符都会比标准输入的文件描述符大）。
*
* readfds        中的fd 集合将由select 来监视是否可以读取,如果你想知道是是否可以从标准输入和一些套接字（sockfd）中读取数据，你就可以把文件描述符和sockfd 加入readfds 中。
* writefds       中的fds 集合将由select 来监视是否可以写入
* exceptfds      中的fds 集合将由select 来监视是否有例外发生
* struct timeval 超时设置。 
*                    一般来说，如果没有任何文件描述符满足你的要求，你的程序是不想永远等下去的．也许每隔1 分钟你就想在屏幕上输出信息：“hello！”。
*                这个代表时间的结构将允许你定义一个超时。
*                在调用select()函数中，如果时间超过timeval 参数所代表的时间长度，
*                而还没有文件描述符满足你的要求，那么select()函数将回返回，允许你进行下面的操作。
*                只需要将tv_sec 设置为你想等待的秒数，然后设置tv_usec 为想等待的微秒数
*                (真正的时间就是tv_sec 所表示的秒数加上tv_usec 所表示的微秒数）．注意，是微秒（百万分之一）而不是毫秒．
*                一秒有1,000 毫秒，一毫秒有1,000 微秒。所以，一秒有1,000,000 微秒． 
*                这个timeval 结构定义如下：
*                struct timeval
*                {
*                    int tv_sec ;   //秒数
*                    int tv_usec ;  //微秒
*                };
*                我们拥有了一个以微秒为单位的记时器！但是因为Linux 和UNIX 一样，最小的时间片是100 微秒，所以不管你将tv_usec 设置的多小，实质上记时器的最小单位是100微秒．
* 
*                如果你将struct timeval 设置为0，则select()函数将会立即返回，同时返回在你的集合中的文件描述符的状态。 
*
*                如果你将timeout 这个参数设置为NULL，则select()函数进入阻塞状态，除了等待到文件描述符的状态变化，否则select()函数不会返回。
*
*
* return        当select()函数返回的时候，timeval 中的时间将会被设置为执行为select()后还剩下的时间。
*
*
*
*/
#include 
#include 
#include 
int select(int numfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
**使用FD_ISSET() 宏，你可以选出select()函数执行的结果。**
在进行更深的操作前，我们来看一看怎样处理这些fd_sets。下面这些宏可以是专门进行这类操作的：
  FD_ZERO(fd_set *set)           将一个文件描述符集合清零
  FD_SET(int fd, fd_set *set)    将文件描述符fd 加入集合set 中。
  FD_CLR(int fd, fd_set *set)    将文件描述符fd 从集合set 中删除．
  FD_ISSET(int fd, fd_set *set)  测试文件描述符fd 是否存在于文件描述符set 中．
下面这段代码演示了从标准输入等待输入等待2.5 秒．
#include 
#include 
#include 
/* 标准输入的文件描述符数值 */
#define STDIN 0
main()
{
   fd_set readfds;
   struct timeval tv;
   /* 设置等待时间为2 秒零500,000 微秒 */
   tv.tv_sec  = 2;
   tv.tv_usec = 500000;
   FD_ZERO(&readfds);
   FD_SET(STDIN, &readfds);
   /* 因为我们只想等待输入，所以将writefds 和execeptfds 设为NULL */
   /* 程序将会在这里等待2 秒零500,000 微秒，除非在这段时间中标准输入有操作 */
   select(STDIN+1, &readfds, NULL, NULL, &tv);
   /* 测试STDIN 是否在readfds 集合中 */
   if (FD_ISSET(STDIN, &readfds))
   {
      /* 在，则在标准输入有输入 */
      printf(“A key was pressed!\n”);
   }
   else
   {
      /* 不在，则在标准输入没有任何输入 */
      printf(“Timed out.\n”);
   }
}
在标准输入上，你需要输入回车后终端才会将输入的信息传给你的程序。所以如果你没有输入回车的话，程序会一直等待到超时。
对select()函数需要注意的最后一点：如果你的套接字描述符正在通过listen()函数侦听等待一个外来的网络连接，则你可以使用select()函数（将套接字描述符加入readfds 集合中）来测试是否存在一个未经处理的新连接
DIN 是否在readfds 集合中 */
   if (FD_ISSET(STDIN, &readfds))
   {
      /* 在，则在标准输入有输入 */
      printf(“A key was pressed!\n”);
   }
   else
   {
      /* 不在，则在标准输入没有任何输入 */
      printf(“Timed out.\n”);
   }
}
在标准输入上，你需要输入回车后终端才会将输入的信息传给你的程序。所以如果你没有输入回车的话，程序会一直等待到超时。
对select()函数需要注意的最后一点：如果你的套接字描述符正在通过listen()函数侦听等待一个外来的网络连接，则你可以使用select()函数（将套接字描述符加入readfds 集合中）来测试是否存在一个未经处理的新连接
