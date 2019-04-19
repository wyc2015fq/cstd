# linux简易socket server - 三少GG - CSDN博客
2013年08月09日 17:16:56[三少GG](https://me.csdn.net/scut1135)阅读数：834
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)

[http://starlight36.com/post/linux_c_language_muti_thread_socket](http://starlight36.com/post/linux_c_language_muti_thread_socket)
[http://www.cnblogs.com/forfuture1978/archive/2010/09/12/1824443.html](http://www.cnblogs.com/forfuture1978/archive/2010/09/12/1824443.html)
  Linux系统网络服务器模型主要有两种：并发服务器和循环服务器。所谓并发服务器就是在同一个时刻可以处理来自多个客户端的请求；循环服务器是指服务器在同一时刻指可以响应一个客户端的请求。而且对于TCP和UDP套接字，这两种服务器的实现方式也有不同的特点。[http://www.91linux.com/html/article/program/cpp/20071105/7988_2.html](http://www.91linux.com/html/article/program/cpp/20071105/7988_2.html)
大致了解下的多客户端服务器程序，要么是单或者少量线程的多路复用的非阻塞式，要么是一个客户端一个线程。当然两种方式各有相应的应用场景，不能谁打死谁。按照一般大学教科书上介绍的服务器带多客户端，大多数是后者，即来个请求，起一个线程为其服务。由此也能推断，这种适合于，数据通讯及时性强，长期连接的应用场景。这次介绍的就是多线程的Socket服务器端的示例。
其中：
select()--多路同步 I/O 
　　虽然这个函数有点奇怪，但是它很有用。假设这样的情况：你是个服 务器，你一边在不停地从连接上读数据，一边在侦听连接上的信息。没问题，你可能会说，不就是一个 accept() 和两个 recv() 吗? 这么 容易吗，朋友? 如果你在调用 accept() 的时候阻塞呢? 你怎么能够同时接
 受 recv() 数据? “用非阻塞的套接字啊！” 不行！你不想耗尽所有的 CPU 吧? 那么，该如何是好? 
select() 让你可以同时监视多个套接字。如果你想知道的话，那么它就 会告诉你哪个套接字准备读，哪个又准备写，哪个套接字又发生了例外 (exception)。 
闲话少说，下面是 select()： 
#include <sys/time.h> 
　　#include <sys/types.h> 
　　#include <unistd.h> 
int select(int numfds, fd_set *readfds, fd_set *writefds,fd_set  
*exceptfds, struct timeval *timeout); 
这个函数监视一系列文件描述符，特别是 readfds、writefds 和 exceptfds。如果你想知道你是否能够从标准输入和套接字描述符 sockfd 读入数据，你只要将文件描述符 0 和 sockfd 加入到集合 readfds 中。参 数 numfds 应该等于最高的文件描述符的值加1。在这个例子中，你应该
 设置该值为 sockfd+1。因为它一定大于标准输入的文件描述符 (0)。 当函数 select() 返回的时候，readfds 的值修改为反映你选择的哪个 文件描述符可以读。你可以用下面讲到的宏 FD_ISSET() 来测试。 在我们继续下去之前，让我来讲讲如何对这些集合进行操作。每个集 合类型都是 fd_set。下面有一些宏来对这个类型进行操作：  
FD_ZERO(fd_set *set) – 清除一个文件描述符集合 
　　FD_SET(int fd, fd_set *set) - 添加fd到集合  
　　FD_CLR(int fd, fd_set *set) – 从集合中移去fd  
　　FD_ISSET(int fd, fd_set *set) – 测试fd是否在集合中  
最后，是有点古怪的数据结构 struct timeval。有时你可不想永远等待别人发送数据过来。也许什么事情都没有发生的时候你也想每隔96秒在终 端上打印字符串 "Still Going..."。这个数据结构允许你设定一个时间，如果 时间到了，而 select() 还没有找到一个准备好的文件描述符，它将返回让你继续处理。  

