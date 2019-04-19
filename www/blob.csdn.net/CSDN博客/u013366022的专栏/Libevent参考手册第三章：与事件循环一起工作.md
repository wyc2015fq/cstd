# Libevent参考手册第三章：与事件循环一起工作 - u013366022的专栏 - CSDN博客
2016年11月30日 15:18:03[slitaz](https://me.csdn.net/u013366022)阅读数：147
# Libevent参考手册第三章：与事件循环一起工作
## 1 运行循环
一旦有了一个已经注册了某些事件的event_base（关于如何创建和注册事件请看下一节），就需要让libevent等待事件并且通知事件的发生。
## 接口
#define EVLOOP_ONCE     0x01
#define EVLOOP_NONBLOCK 0x02
**int** event_base_loop(**struct** event_base
 *base, **int** flags);
默认情况下，event_base_loop（）函数运行event_base直到其中没有已经注册的事件为止。执行循环的时候，函数重复地检查是否有任何已经注册的事件被触发（比如说，读事件的文件描述符已经就绪，可以读取了；或者超时事件的超时时间即将到达）。如果有事件被触发，函数标记被触发的事件为“激活的”，并且执行这些事件。
在flags参数中设置一个或者多个标志就可以改变event_base_loop（）的行为。如果设置了EVLOOP_ONCE，循环将等待某些事件成为激活的，执行激活的事件直到没有更多的事件可以执行，然会返回。如果设置了EVLOOP_NONBLOCK，循环不会等待事件被触发：循环将仅仅检测是否有事件已经就绪，可以立即触发，如果有，则执行事件的回调。
完成工作后，如果正常退出，event_base_loop（）返回0；如果因为后端中的某些未处理错误而退出，则返回-1。
为帮助理解，这里给出event_base_loop（）的算法概要：
## 伪代码
**while** (any events are registered with the loop) {**if** (EVLOOP_NONBLOCK was set, or any events are already active)        If any registered events have triggered, mark them active.**else**        Wait until at least one event has triggered, and mark it active.**for** (p = 0; p < n_priorities; ++p {**if** (any event with priority of p is active) {          Run all active events with priority of p.**break**; /* Do not run any events of a less important priority */       }    }**if** (EVLOOP_ONCE was set or EVLOOP_NONBLOCK was set)**break**;}
为方便起见，也可以调用
## 接口
**int** event_base_dispatch(**struct** event_base *base);
event_base_dispatch（）等同于没有设置标志的event_base_loop（）。所以，event_base_dispatch（）将一直运行，直到没有已经注册的事件了，或者调用了event_base_loopbreak（）或者event_base_loopexit（）为止。
这些函数定义在<event2/event.h>中，从libevent 1.0版就存在了。
## 2 停止循环
如果想在移除所有已注册的事件之前停止活动的事件循环，可以调用两个稍有不同的函数。
## 接口
**int** event_base_loopexit(**struct** event_base *base,**const****struct** timeval *tv);**int** event_base_loopbreak(**struct** event_base *base);
event_base_loopexit（）让event_base在给定时间之后停止循环。如果tv参数为NULL，event_base会立即停止循环，没有延时。如果event_base当前正在执行任何激活事件的回调，则回调会继续运行，直到运行完所有激活事件的回调之才退出。
event_base_loopbreak（）让event_base立即退出循环。它与event_base_loopexit（base,NULL）的不同在于，如果event_base当前正在执行激活事件的回调，它将在执行完当前正在处理的事件后立即退出。
注意event_base_loopexit(base,NULL)和event_base_loopbreak(base)在事件循环没有运行时的行为不同：前者安排下一次事件循环在下一轮回调完成后立即停止（就好像带EVLOOP_ONCE标志调用一样）；后者却仅仅停止当前正在运行的循环，如果事件循环没有运行，则没有任何效果。
这两个函数都在成功时返回0，失败时返回-1。
## 示例：立即关闭
#include <event2/event.h>*/* Here's a callback function that calls loopbreak */***void** cb(**int** sock, **short** what, **void** *arg){**struct** event_base *base = arg;    event_base_loopbreak(base);}**void** main_loop(**struct** event_base *base, evutil_socket_t watchdog_fd){**struct** event *watchdog_event;*    /* Construct a new event to trigger whenever there are any bytes to**       read from a watchdog socket.  When that happens, we'll call the**       cb function, which will make the loop exit immediately without**       running any other active events at all.**     */*    watchdog_event = event_new(base, watchdog_fd, EV_READ, cb, base);    event_add(watchdog_event, NULL);    event_base_dispatch(base);}
## 示例：执行事件循环10秒，然后退出
#include <event2/event.h>**void** run_base_with_ticks(**struct** event_base *base){**struct** timeval ten_sec;  ten_sec.tv_sec = 10;  ten_sec.tv_usec = 0;*  /* Now we run the event_base for a series of 10-second intervals, printing**     "Tick" after each.  For a much better way to implement a 10-second**     timer, see the section below about persistent timer events. */***while** (1) {*     /* This schedules an exit ten seconds from now. */*     event_base_loopexit(base, &ten_sec);     event_base_dispatch(base);     puts("Tick");  }}
有时候需要知道对event_base_dispatch（）或者event_base_loop（）的调用是正常退出的，还是因为调用event_base_loopexit（）或者event_base_break（）而退出的。可以调用下述函数来确定是否调用了loopexit或者break函数。
## 接口
**int** event_base_got_exit(**struct** event_base *base);**int** event_base_got_break(**struct** event_base *base);
这两个函数分别会在循环是因为调用event_base_loopexit（）或者event_base_break（）而退出的时候返回true，否则返回false。下次启动事件循环的时候，这些值会被重设。
这些函数声明在<event2/event.h>中。event_break_loopexit()函数首次在libevent 1.0c版本中实现；event_break_loopbreak()首次在libevent 1.4.3版本中实现。
## 3 检查内部时间缓存
有时候需要在事件回调中获取当前时间的近似视图，但不想调用gettimeofday()（可能是因为OS将gettimeofday()作为系统调用实现，而你试图避免系统调用的开销）。
在回调中，可以请求libevent开始本轮回调时的当前时间视图。
## 接口
**int** event_base_gettimeofday_cached(**struct** event_base *base,**struct** timeval *tv_out);
如果event_base当前正在执行回调，event_base_gettimeofday_cached()函数设置tv_out参数的值为缓存的时间。否则，函数调用evutil_gettimeofday()获取真正的当前时间。成功时函数返回0，失败时返回负数。
注意，因为libevent在开始执行回调的时候时间值会被缓存，所以这个值至少是有一点不精确。如果回调执行很长时间，这个值将非常不精确。
这个函数是libevent 2.0.4-alpha新引入的。
# 4 转储event_base的状态
## 接口
**void** event_base_dump_events(**struct** event_base *base, FILE *f);
为帮助调试程序（或者调试libevent），有时候可能需要已经加入到event_base的所有事件及其状态的完整列表。调用event_base_dump_events()可以将这个列表输出到指定的文件中。
这个列表是人可读的，未来版本的libevent将会改变其格式。
这个函数在libevent 2.0.1-alpha版本中引入。
## 5 废弃的事件循环函数
前面已经讨论过，老版本的libevent 具有“当前”event_base的概念。
本文讨论的某些事件循环函数具有操作当前event_base的变体。除了没有base参数外，这些函数跟当前新版本函数的行为相同。
![](http://hi.csdn.net/attachment/201106/11/0_1307764569MuN8.gif)
## 注意
2.0版本之前的event_base是不支持锁的，所以这些函数并不是完全线程安全的：不允许在执行事件循环的线程之外的其他线程中调用_loopbreak()或者_loopexit()函数。
