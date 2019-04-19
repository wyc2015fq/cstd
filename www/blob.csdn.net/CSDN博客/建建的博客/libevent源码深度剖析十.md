# libevent源码深度剖析十 - 建建的博客 - CSDN博客
2017年03月07日 15:31:27[纪建](https://me.csdn.net/u013898698)阅读数：87
个人分类：[Libevent](https://blog.csdn.net/u013898698/article/category/6771433)
# libevent源码深度剖析十
——支持I/O多路复用技术
张亮
      Libevent的核心是事件驱动、同步非阻塞，为了达到这一目标，必须采用系统提供的I/O多路复用技术，而这些在Windows、[Linux](http://lib.csdn.net/base/linux)、Unix等不同平台上却各有不同，如何能提供优雅而统一的支持方式，是首要关键的问题，这其实不难，本节就来分析一下。
### 1 统一的关键
      Libevent支持多种I/O多路复用技术的关键就在于结构体eventop，这个结构体前面也曾提到过，它的成员是一系列的函数指针, 定义在event-internal.h文件中：
**[cpp]**[view
 plain](http://blog.csdn.net/sparkliang/article/details/5105392#)[copy](http://blog.csdn.net/sparkliang/article/details/5105392#)
- struct eventop {  
- constchar *name;  
- void *(*init)(struct event_base *); // 初始化
- int (*add)(void *, struct event *); // 注册事件
- int (*del)(void *, struct event *); // 删除事件
- int (*dispatch)(struct event_base *, void *, struct timeval *); // 事件分发
- void (*dealloc)(struct event_base *, void *); // 注销，释放资源
- /* set if we need to reinitialize the event base */
- int need_reinit;  
- };  
      在libevent中，每种I/O demultiplex机制的实现都必须提供这五个函数接口，来完成自身的初始化、销毁释放；对事件的注册、注销和分发。
比如对于epoll，libevent实现了5个对应的接口函数，并在初始化时并将eventop的5个函数指针指向这5个函数，那么程序就可以使用epoll作为I/O demultiplex机制了。
### 2 设置I/O demultiplex机制
      Libevent把所有支持的I/O demultiplex机制存储在一个全局静态数组eventops中，并在初始化时选择使用何种机制，数组内容根据优先级顺序声明如下：
**[cpp]**[view
 plain](http://blog.csdn.net/sparkliang/article/details/5105392#)[copy](http://blog.csdn.net/sparkliang/article/details/5105392#)
- /* In order of preference */
- staticconststruct eventop *eventops[] = {  
- #ifdef HAVE_EVENT_PORTS
-     &evportops,  
- #endif
- #ifdef HAVE_WORKING_KQUEUE
-     &kqops,  
- #endif
- #ifdef HAVE_EPOLL
-     &epollops,  
- #endif
- #ifdef HAVE_DEVPOLL
-     &devpollops,  
- #endif
- #ifdef HAVE_POLL
-     &pollops,  
- #endif
- #ifdef HAVE_SELECT
-     &selectops,  
- #endif
- #ifdef WIN32
-     &win32ops,  
- #endif
-     NULL  
- };   
     然后libevent根据系统配置和编译选项决定使用哪一种I/O demultiplex机制，这段代码在函数event_base_new()中：
**[cpp]**[view
 plain](http://blog.csdn.net/sparkliang/article/details/5105392#)[copy](http://blog.csdn.net/sparkliang/article/details/5105392#)
- base->evbase = NULL;  
- for (i = 0; eventops[i] && !base->evbase; i++) {  
-         base->evsel = eventops[i];  
-         base->evbase = base->evsel->init(base);  
-     }   
    可以看出，libevent在编译阶段选择系统的I/O demultiplex机制，而不支持在运行阶段根据配置再次选择。
    以Linux下面的epoll为例，实现在源文件epoll.c中，eventops对象epollops定义如下：
**[cpp]**[view
 plain](http://blog.csdn.net/sparkliang/article/details/5105392#)[copy](http://blog.csdn.net/sparkliang/article/details/5105392#)
- conststruct eventop epollops = {  
- "epoll",  
-     epoll_init,  
-     epoll_add,  
-     epoll_del,  
-     epoll_dispatch,  
-     epoll_dealloc,  
-     1 /* need reinit */
- };  
变量epollops中的函数指针具体声明如下，注意到其返回值和参数都和eventop中的定义严格一致，这是函数指针的语法限制。
**[cpp]**[view
 plain](http://blog.csdn.net/sparkliang/article/details/5105392#)[copy](http://blog.csdn.net/sparkliang/article/details/5105392#)
- staticvoid *epoll_init    (struct event_base *);  
- staticint epoll_add    (void *, struct event *);  
- staticint epoll_del    (void *, struct event *);  
- staticint epoll_dispatch(struct event_base *, void *, struct timeval *);  
- staticvoid epoll_dealloc    (struct event_base *, void *);  
      那么如果选择的是epoll，那么调用结构体eventop的init和dispatch函数指针时，实际调用的函数就是epoll的初始化函数epoll_init()和事件分发函数epoll_dispatch()了；
     关于epoll的具体用法这里就不多说了，可以参见介绍epoll的文章（本人的哈哈）：
[http://blog.csdn.net/sparkliang/archive/2009/11/05/4770655.aspx](http://blog.csdn.net/sparkliang/archive/2009/11/05/4770655.aspx)
     C++语言提供了虚函数来实现多态，在[C语言](http://lib.csdn.net/base/c)中，这是通过函数指针实现的。对于各类函数指针的详细说明可以参见文章：
[http://blog.csdn.net/sparkliang/archive/2009/06/09/4254115.aspx](http://blog.csdn.net/sparkliang/archive/2009/06/09/4254115.aspx)
同样的，上面epollops以及epoll的各种函数都直接定义在了epoll.c源文件中，对外都是不可见的。对于libevent的使用者而言，完全不会知道它们的存在，对epoll的使用也是通过eventop来完成的，达到了信息隐藏的目的。
### 3 小节
    支持多种I/O demultiplex机制的方法其实挺简单的，借助于函数指针就OK了。通过对源代码的分析也可以看出，Libevent是在编译阶段选择系统的I/O demultiplex机制的，而不支持在运行阶段根据配置再次选择。
