# 最好的epoll讲解--转自”知乎“ - 三少GG - CSDN博客
2013年12月26日 14:26:28[三少GG](https://me.csdn.net/scut1135)阅读数：2897
2013-10-27更新：由于此文陆陆续续收到赞同，而且其中有些地方并不完全正确，特在本文最后予以订正
我不了解楼主的层次，我必须从很多基础的概念开始构建这个答案，并且可能引申到很多别的问题。
首先我们来定义流的概念，一个流可以是文件，socket，pipe等等可以进行I/O操作的内核对象。
不管是文件，还是套接字，还是管道，我们都可以把他们看作流。
之后我们来讨论I/O的操作，通过read，我们可以从流中读入数据；通过write，我们可以往流写入数据。现在假定一个情形，我们需要从流中读数据，但是流中还没有数据，（典型的例子为，客户端要从socket读如数据，但是服务器还没有把数据传回来），这时候该怎么办？
- 阻塞。阻塞是个什么概念呢？比如某个时候你在等快递，但是你不知道快递什么时候过来，而且你没有别的事可以干（或者说接下来的事要等快递来了才能做）；那么你可以去睡觉了，因为你知道快递把货送来时一定会给你打个电话（假定一定能叫醒你）。
- 非阻塞**忙**轮询。接着上面等快递的例子，如果用忙轮询的方法，那么你需要知道快递员的手机号，然后每分钟给他挂个电话：“你到了没？”
很明显一般人不会用第二种做法，不仅显很无脑，浪费话费不说，还占用了快递员大量的时间。
大部分程序也不会用第二种做法，因为第一种方法经济而简单，经济是指消耗很少的CPU时间，如果线程睡眠了，就掉出了系统的调度队列，暂时不会去瓜分CPU宝贵的时间片了。
为了了解阻塞是如何进行的，我们来讨论缓冲区，以及内核缓冲区，最终把I/O事件解释清楚。缓冲区的引入是为了减少频繁I/O操作而引起频繁的系统调用（你知道它很慢的），当你操作一个流时，更多的是以缓冲区为单位进行操作，这是相对于用户空间而言。对于内核来说，也需要缓冲区。
假设有一个管道，进程A为管道的写入方，Ｂ为管道的读出方。
- 假设一开始内核缓冲区是空的，B作为读出方，被阻塞着。然后首先A往管道写入，这时候内核缓冲区由空的状态变到非空状态，内核就会产生一个事件告诉Ｂ该醒来了，这个事件姑且称之为“缓冲区非空”。
- 但是“缓冲区非空”事件通知B后，B却还没有读出数据；且内核许诺了不能把写入管道中的数据丢掉这个时候，Ａ写入的数据会滞留在内核缓冲区中，如果内核也缓冲区满了，B仍未开始读数据，最终内核缓冲区会被填满，这个时候会产生一个I/O事件，告诉进程A，你该等等（阻塞）了，我们把这个事件定义为“缓冲区满”。
- 假设后来Ｂ终于开始读数据了，于是内核的缓冲区空了出来，这时候内核会告诉A，内核缓冲区有空位了，你可以从长眠中醒来了，继续写数据了，我们把这个事件叫做“缓冲区非满”
- 也许事件Y1已经通知了A，但是A也没有数据写入了，而Ｂ继续读出数据，知道内核缓冲区空了。这个时候内核就告诉B，你需要阻塞了！，我们把这个时间定为“缓冲区空”。
这四个情形涵盖了四个I/O事件，缓冲区满，缓冲区空，缓冲区非空，缓冲区非满（注都是说的内核缓冲区，且这四个术语都是我生造的，仅为解释其原理而造）。这四个I/O事件是进行阻塞同步的根本。（如果不能理解“同步”是什么概念，请学习操作系统的锁，信号量，条件变量等任务同步方面的相关知识）。
然后我们来说说阻塞I/O的缺点。但是阻塞I/O模式下，一个线程只能处理一个流的I/O事件。如果想要同时处理多个流，要么多进程(fork)，要么多线程(pthread_create)，很不幸这两种方法效率都不高。
于是再来考虑非阻塞忙轮询的I/O方式，我们发现我们可以同时处理多个流了（把一个流从阻塞模式切换到非阻塞模式再此不予讨论）：
while true {
for i in stream[]; {
if i has data
read until unavailable
}
}
我们只要不停的把所有流从头到尾问一遍，又从头开始。这样就可以处理多个流了，但这样的做法显然不好，因为如果所有的流都没有数据，那么只会白白浪费CPU。这里要补充一点，阻塞模式下，内核对于I/O事件的处理是阻塞或者唤醒，而非阻塞模式下则把I/O事件交给其他对象（后文介绍的select以及epoll）处理甚至直接忽略。
为了避免CPU空转，可以引进了一个代理（一开始有一位叫做select的代理，后来又有一位叫做poll的代理，不过两者的本质是一样的）。这个代理比较厉害，可以同时观察许多流的I/O事件，在空闲的时候，会把当前线程阻塞掉，当有一个或多个流有I/O事件时，就从阻塞态中醒来，于是我们的程序就会轮询一遍所有的流（于是我们可以把“忙”字去掉了）。代码长这样:
while true {
select(streams[])
for i in streams[] {
if i has data
read until unavailable
}
}
于是，如果没有I/O事件产生，我们的程序就会阻塞在select处。但是依然有个问题，我们从select那里仅仅知道了，有I/O事件发生了，但却并不知道是那几个流（可能有一个，多个，甚至全部），我们只能无差别轮询所有流，找出能读出数据，或者写入数据的流，对他们进行操作。
但是使用select，我们有O(n)的无差别轮询复杂度，同时处理的流越多，每一次无差别轮询时间就越长。再次
说了这么多，终于能好好解释epoll了
epoll可以理解为event poll，不同于忙轮询和无差别轮询，epoll之会把哪个流发生了怎样的I/O事件通知我们。此时我们对这些流的操作都是有意义的。（复杂度降低到了O(k)，k为产生I/O事件的流的个数，也有认为O(1)的[更新 1]）
在讨论epoll的实现细节之前，先把epoll的相关操作列出[更新 2]：
- epoll_create 创建一个epoll对象，一般epollfd = epoll_create()
- epoll_ctl （epoll_add/epoll_del的合体），往epoll对象中增加/删除某一个流的某一个事件
比如
epoll_ctl(epollfd, EPOLL_CTL_ADD, socket, EPOLLIN);//有缓冲区内有数据时epoll_wait返回
epoll_ctl(epollfd, EPOLL_CTL_DEL, socket, EPOLLOUT);//缓冲区可写入时epoll_wait返回
- epoll_wait(epollfd,...)等待直到注册的事件发生
（注：当对一个非阻塞流的读写发生缓冲区满或缓冲区空，write/read会返回-1，并设置errno=EAGAIN。而epoll只关心缓冲区非满和缓冲区非空事件）。
一个epoll模式的代码大概的样子是：
while true {
active_stream[] = epoll_wait(epollfd)
for i in active_stream[] {
read or write till unavailable
}
}
限于篇幅，我只说这么多，以揭示原理性的东西，至于epoll的使用细节，请参考man和google，实现细节，请参阅linux kernel source。
======================================
[更新1]: 原文为O(1)，但实际上O(k)更为准确
[更新2]: 原文所列第二点说法让人产生EPOLLIN/EPOLLOUT等同于“缓冲区非空”和“缓冲区非满”的事件，但并非如此，详细可以Google关于epoll的边缘触发和水平触发。
# [epoll精髓](http://www.cnblogs.com/OnlyXP/archive/2007/08/10/851222.html)
在linux的网络编程中，很长的时间都在使用select来做事件触发。在linux新的内核中，有了一种替换它的机制，就是epoll。
相比于select，epoll最大的好处在于它不会随着监听fd数目的增长而降低效率。因为在内核中的select实现中，它是采用轮询来处理的，轮询的fd数目越多，自然耗时越多。并且，在linux/posix_types.h头文件有这样的声明：
#define __FD_SETSIZE    1024
表示select最多同时监听1024个fd，当然，可以通过修改头文件再重编译内核来扩大这个数目，但这似乎并不治本。
epoll的接口非常简单，一共就三个函数：
1. int epoll_create(int size);
创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大。这个参数不同于select()中的第一个参数，给出最大监听的fd+1的值。需要注意的是，当创建好epoll句柄后，它就是会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须调用close()关闭，否则可能导致fd被耗尽。
2. int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
epoll的事件注册函数，它不同与select()是在监听事件时告诉内核要监听什么类型的事件，而是在这里先注册要监听的事件类型。第一个参数是epoll_create()的返回值，第二个参数表示动作，用三个宏来表示：
EPOLL_CTL_ADD：注册新的fd到epfd中；
EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
EPOLL_CTL_DEL：从epfd中删除一个fd；
第三个参数是需要监听的fd，第四个参数是告诉内核需要监听什么事，struct epoll_event结构如下：
struct epoll_event {
  __uint32_t events;  /* Epoll events */
  epoll_data_t data;  /* User data variable */
};
events可以是以下几个宏的集合：
EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
3. int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
等待事件的产生，类似于select()调用。参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size，参数timeout是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。该函数返回需要处理的事件数目，如返回0表示已超时。
### IOCP , kqueue , epoll ... 有多重要？
设计 mmo 服务器，我听过许多老生常谈，说起处理大量连接时， select 是多么低效。我们应该换用 iocp (windows), kqueue(freebsd), 或是 epoll(linux) 。的确，处理大量的连接的读写，select 是够低效的。因为 kernel 每次都要对 select 传入的一组 socket 号做轮询，那次在上海，以陈榕的说法讲，这叫鬼子进村策略。一遍遍的询问“鬼子进村了吗？”，“鬼子进村了吗？”...
 大量的 cpu 时间都耗了进去。（更过分的是在 windows 上，还有个万恶的 64 限制。）
使用 kqueue 这些，变成了派一些个人去站岗，鬼子来了就可以拿到通知，效率自然高了许多。不过最近我在反思，真的需要以这些为基础搭建服务器吗？
刚形成的一个思路是这样的：
我们把处理外部连接和处理游戏逻辑分摊到两个服务器上处理，为了后文容易表述，暂时不太严谨的把前者称为连接服务器，后者叫做逻辑服务器。
连接服务器做的事情可以非常简单，只是把多个连接上的数据汇集到一起。假设同时连接总数不超过 65536 个，我们只需要把每个连接上的数据包加上一个两字节的数据头就可以表识出来。这个连接服务器再通过单个连接和逻辑服务器通讯就够了。
那么连接服务器尽可以用最高效的方式处理数据，它的逻辑却很简单，代码量非常的小。而逻辑服务器只有一个外部连接，无论用什么方式处理都不会慢了。
进一步，我们可以把这个方法扩展开。假定我们逻辑以 10Hz 的频率处理逻辑。我们就让连接服务器以 10Hz 的脉冲把汇总的数据周期性的发送过去，先发一个长度信息再发数据包。即使一个脉冲没有外部数据，也严格保证至少发一个 0 的长度信息。额外的，连接服务器还需要控制每个脉冲的数据总流量，不至于一次发送数据超过逻辑服务器处理的能力。
那么，逻辑服务器甚至可以用阻塞方式调用 recv 收取这些数据，连 select 也省了。至于数据真的是否会被接收方阻塞，就由连接服务器的逻辑保证了。
说到阻塞接收，我跟一个同事讨论的时候，他严重担心这个的可靠性，不希望因为意外把逻辑服务器挂在一个 system call 上。他列举了许多可能发生的意外情况，不过我个人是不太担心的，原因不想在这里多解释。当然我这样设计，主要不是为了节省一个 select 的调用，而是希望方便调试。（当然，如果事实证明这样不可行，修改方案也很容易）
因为阻塞接收可以保证逻辑服务器的严格时序性，当我们把两个服务器中的通讯记录下来，以后可以用这些数据完全重现游戏逻辑的过程，无论怎么调试运行，都可以保证逻辑服务器的行为是可以完全重现的。即，每 0.1s 接受已知的数据包，然后处理它们。
这样做，逻辑服务器对网络层的代码量的需求也大大减少了，可以更专心的构建逻辑。
云风 提交于 April 19, 2006 11:09 AM|[固定链接](http://blog.codingnow.com/2006/04/iocp_kqueue_epoll.html)
### COMMENTS
哈哈哈，老文章了，还这么火。我公司也是样这架构的服务器，用libevent写的连接服务器，客户端和逻辑服务端的网络代码是同一套代码，都是用select实现的，现在发现select没必要了，直接阻塞也行的。反正是逻辑服务器单进程的。
Posted by: 涵曦 | (45) [December 10, 2013 04:19 PM](http://blog.codingnow.com/2006/04/iocp_kqueue_epoll.html#comment-44488)
不知深浅一句。
我理解，是不是建立一个事件池?这样，在连接服务器和逻辑服务器中间其实是不介意用什么通信方式的，目标只有两个：正确传递事件和处理结果，保证效率。
如此这样，连接服务器要等着处理结果？
Posted by: [luo](http://www.cnblogs.com/pied) | (44) [April 11,
 2013 04:53 PM](http://blog.codingnow.com/2006/04/iocp_kqueue_epoll.html#comment-43861)
说到阻塞接收，我跟一个同事讨论的时候，他严重担心这个的可靠性，不希望因为意外把逻辑服务器挂在一个 system call 上。他列举了许多可能发生的意外情况
-----------
我对他列出的意外情况很关心
Posted by: Anonymous | (43) [February 15, 2012 12:17 PM](http://blog.codingnow.com/2006/04/iocp_kqueue_epoll.html#comment-42361)
大神的文章都是很多人赞同的.不同的声音,就要被当作异类枪决.
好吧,我不是不同意大神的说法.我只是有点疑惑.我在游戏公司也有7年了.自己带的游戏也有5款.我们公司用的却是select,而看过许多国外的游戏服务端,都是iocp,由于商业机密问题,我不便透露我公司是什么.
如果真的没必要用iocp...那那些国外的游戏公司为啥要用iocp,总有个其他原因吧...当然,已知的可以防部分dos攻击是知道的了.
Posted by: ☆随心の风 | (42) [December 8, 2011 10:38 AM](http://blog.codingnow.com/2006/04/iocp_kqueue_epoll.html#comment-41979)
连接服务器？
这真是云风在2006年写的文章么？
2006年已经有很多网络游戏设计都是有连接服务器了。网易照理说也应该有才对。
不过这个连接服务器一般叫做GateWay服务器。作用不仅仅是转发了，同时也起到隔绝网络的作用。 
Posted by: CPP | (41) [November 24, 2011 03:28 PM](http://blog.codingnow.com/2006/04/iocp_kqueue_epoll.html#comment-41831)
很多年前看的这篇，最后记录信息调试那句话相当有帮助，我在我们的游戏服务端也实现了录像和重放的功能，调试诡异错误的时候非常方便，重放一下就可以了
## 关于select/poll对比epoll的一次精彩问答
[judezhan](http://lonelycoder.org/?author=1) 发布于 2013
 年 7 月 31 日
[暂无评论](http://lonelycoder.org/?p=403#respond)[发表评论](http://lonelycoder.org/?p=403#respond)
Epoll是Linux内核在2.5.44版本引进的一个新特性，旨在替换之前系统中老的select, poll等系统请求。这是Linux I/O系统一次质的飞跃。关于Epoll的详细的介绍见 [Wikipedia](http://en.wikipedia.org/wiki/Epoll)。
Epoll在绝大多数情况下性能都远超select或者poll，但是除了速度之外，三者之间的CPU开销，内存消耗情况又怎么样呢？
本文的内容来自Stackoverflow上一次精彩的问答，除了比较poll， select和epoll在性能，系统资源消耗等方面的差异之外，还指出了epoll在对普通文件支持方面相对于select/poll的不足之处（当然，这三者本身都不支持普通文件，只是作者认为epoll对这类问题的处理机制不好，这是个见仁见智的事情，不代表作者的观点是正确的）。本来想翻译成中文再写出来，但自己翻译水平有限，翻译了几句之后发现自己都不大看得懂了…所以还是放弃了。如果有NB的同学愿意翻译，可以联系我，质量OK的话，可以在原文后面带上翻译版。Anyway，还是希望对这个Topic感兴趣的同学能够看完这篇文章，相信能使你对epoll有个更深的了解。
问：
> 
Everything I’ve read and experienced ( Tornado based apps ) leads me to believe that ePoll is a natural replacement for Select and Poll based networking, especially with Twisted. Which makes me paranoid, its pretty rare for a better technique or methodology
 not to come with a price.
Reading a couple dozen comparisons between epoll and alternatives shows that epoll is clearly the champion for speed and scalability, specifically that it scales in a linear fashion which is fantastic. That said, what about processor and memory utilization,
 is epoll still the champ?
答:
> 
For very small numbers of sockets (varies depending on your hardware, of course, but we’re talking about something on the order of 10 or fewer), select can beat epoll in memory usage and runtime speed. Of course, for such small numbers of sockets, both mechanisms
 are so fast that you don’t really care about this difference in the vast majority of cases.
One clarification, though. Both select and epoll scale linearly. A big difference, though, is that the userspace-facing APIs have complexities that are based on different things. The cost of a select call goes roughly with the value of the highest numbered
 file descriptor you pass it. If you select on a single fd, 100, then that’s roughly twice as expensive as selecting on a single fd, 50. Adding more fds below the highest isn’t quite free, so it’s a little more complicated than this in practice, but this is
 a good first approximation for most implementations.
The cost of epoll is closer to the number of file descriptors that actually have events on them. If you’re monitoring 200 file descriptors, but only 100 of them have events on them, then you’re (very roughly) only paying for those 100 active file descriptors.
 This is where epoll tends to offer one of its major advantages over select. If you have a thousand clients that are mostly idle, then when you use select you’re still paying for all one thousand of them. However, with epoll, it’s like you’ve only got a few
 – you’re only paying for the ones that are active at any given time.
All this means that epoll will lead to less CPU usage for most workloads. As far as memory usage goes, it’s a bit of a toss up. select does manage to represent all the necessary information in a highly compact way (one bit per file descriptor). And the FD_SETSIZE
 (typically 1024) limitation on how many file descriptors you can use with select means that you’ll never spend more than 128 bytes for each of the three fd sets you can use with select (read, write, exception). Compared to those 384 bytes max, epoll is sort
 of a pig. Each file descriptor is represented by a multi-byte structure. However, in absolute terms, it’s still not going to use much memory. You can represent a huge number of file descriptors in a few dozen kilobytes (roughly 20k per 1000 file descriptors,
 I think). And you can also throw in the fact that you have to spend all 384 of those bytes with select if you only want to monitor one file descriptor but its value happens to be 1024, wheras with epoll you’d only spend 20 bytes. Still, all these numbers are
 pretty small, so it doesn’t make much difference.
And there’s also that other benefit of epoll, which perhaps you’re already aware of, that it is not limited to FD_SETSIZE file descriptors. You can use it to monitor as many file descriptors as you have. And if you only have one file descriptor, but its value
 is greater than FD_SETSIZE, epoll works with that too, but select does not.
Randomly, I’ve also recently discovered one slight drawback to epoll as compared to select or poll. While none of these three APIs supports normal files (ie, files on a file system), select and poll present this lack of support as reporting such descriptors
 as always readable and always writeable. This makes them unsuitable for any meaningful kind of non-blocking filesystem I/O, a program which uses select or poll and happens to encounter a file descriptor from the filesystem will at least continue to operate
 (or if it fails, it won’t be because of select or poll), albeit it perhaps not with the best performance.
On the other hand, epoll will fail fast with an error (EPERM, apparently) when asked to monitor such a file descriptor. Strictly speaking, this is hardly incorrect. It’s merely signalling its lack of support in an explicit way. Normally I would applaud explicit
 failure conditions, but this one is undocumented (as far as I can tell) and results in a completely broken application, rather than one which merely operates with potentially degraded performance.
In practice, the only place I’ve seen this come up is when interacting with stdio. A user might redirect stdin or stdout from/to a normal file. Whereas previously stdin and stdout would have been a pipe — supported by epoll just fine — it then becomes a normal
 file and epoll fails loudly, breaking the application.
该问答的地址为 [http://stackoverflow.com/questions/2032598/caveats-of-select-poll-vs-epoll-reactors-in-twisted](http://stackoverflow.com/questions/2032598/caveats-of-select-poll-vs-epoll-reactors-in-twisted)
Enjoy！
