# 获取一个event_base - u013366022的专栏 - CSDN博客
2016年11月30日 15:02:19[slitaz](https://me.csdn.net/u013366022)阅读数：364
原文链接：http://www.wangafu.net/~nickm/libevent-book/Ref2_eventbase.html
## 创建一个event_base
在你使用任何有意思的Libevent函数之前,你需要分配一个或多个event_base结构.每一个event_base结构含有一组events，并且可以告知你哪一些events是就绪的.
如果一个event_base是以加锁模式创建的,那么在多个线程访问它是安全的.然而,事件循环却只能跑在一个线程内.所以说,如果你想要对IO事件有多线程的轮询,你需要为每一个线程都分配一个event_base。
  Tip:未来版本的Libevent可能会支持多线程的IO事件轮询.
每一个event_base都有一个"方法"或者说后台,event_base根据这个来判断哪一个event已经就绪.目前确认的方法有：
- 
select
- 
poll
- 
epoll
- 
kqueue
- 
devpoll
- 
evport
- 
win32
用户可以通过设置环境变量的方式禁用特定的后台方法.比如你不想使用kqueue,那么设置一个叫EVENT_NOKQUEUE的环境变量.如果你想禁用程序内部的后台方法,看下面对event_config_avoid_method()的说明.
### 创建一个默认的event_base
event_base_new()函数分配并返回一个新的默认配置的event_base。该函数检测环境变量并返回一个指向新的event_base的指针.如果出错的话则返回NULL.
当在众多方法(指上文中提到的事件多路分离的方法:select,poll,epoll等)中选择时,它会选择当前操作系统支持的最快的方法.
Interface
```
struct event_base *event_base_new(void);
```
对大多数程序来讲,这就是你所需全部了.
event_base_new()函数在<event2/event.h>中声明.它是在Libevent 1.4.3中第一次引入的.
### 创建一个复杂的event_base
 如果你想对获取一个什么样的event_base有更多的控制权,你需要用到event_config.一个event_config是一个不透明的结构(opaque structure),其中含有你对event_base的预设信息.当你想获取一个event_base,你向event_base_new_with_config()即可.
Interface
```
struct event_config *event_config_new(void);
struct event_base *event_base_new_with_config(const struct event_config *cfg);
void event_config_free(struct event_config *cfg);
```
为了使用这些函数分配一个event_base,你需要先调用event_config_new()分配一个新的event_config.然后你调用一些别的函数以告知event_config你的需求.最后,调用event_base_new_with_config()来获取一个新的event_base.当你完成之后,你可以调用event_config_free()释放掉event_config.
Interface
```
int event_config_avoid_method(struct event_config *cfg, const char *method);
enum event_method_feature {
    EV_FEATURE_ET = 0x01,
    EV_FEATURE_O1 = 0x02,
    EV_FEATURE_FDS = 0x04,
};
int event_config_require_features(struct event_config *cfg,
                                  enum event_method_feature feature);
enum event_base_config_flag {
    EVENT_BASE_FLAG_NOLOCK = 0x01,
    EVENT_BASE_FLAG_IGNORE_ENV = 0x02,
    EVENT_BASE_FLAG_STARTUP_IOCP = 0x04,
    EVENT_BASE_FLAG_NO_CACHE_TIME = 0x08,
    EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST = 0x10,
    EVENT_BASE_FLAG_PRECISE_TIMER = 0x20
};
int event_config_set_flag(struct event_config *cfg,
    enum event_base_config_flag flag);
```
调用event_config_avoid_method告知Libevent根据名字避开一个特定的可用后台方法.调用event_config_require_feature()告知Libevent不要使用那些不支持全部一组特性的后台方法.调用event_config_set_falg()告知Libevent在创建event_base时设置一个或多个下述的运行时标志.
已知的用来传给event_config_require_feature()的特征值有：
EV_FEATURE_ET
      要求一个支持边沿触发(edge-triggered)的后台方法.
EV_FEATURE_O1
     要求一个增加事件,删除事件,激活事件的复杂度为O(1)的后台方法.
EV_FEATURE_FDS
　　要求一个支持任意文件描述符类型,而不仅仅是sockets的后台方法.
已知的用来传给event_config_set_flag()的可选值有：
- EVENT_BASE_FLAG_NOLOCK
不为event_base分配锁.设置这个选项可以为你对event_base加锁和释放节约一点时间,但是可能导致多线程下访问event_base无效或是不安全.
- EVENT_BASE_FLAG_IGNORE_ENV
在决定用哪一种后台方法时,不去检查形如EVENT_*的环境变量.慎用该标志：使用该标志可能导致你的程序与Libevent的交互非常难以调试.
- EVENT_BASE_FLAG_STARTUP_IOCP
仅在Windows下使用,这个标志使得Libevent在启动时就启用IOCP的分派逻辑,而不是在需要时才启用.(On Windows only, this flag makes Libevent enable any necessary IOCP dispatch logic on startup, rather than on-demand.)
- EVENT_BASE_FLAG_NO_CACHE_TIME
在每一次超时调用后检测当前时间，而不是在事件循环就绪可以运行超时回调函数时检测当前时间。注意：这可能导致程序消耗更多的CPU。
- EVENT_BASE_FLAG_EPOLL_USE_CHANGELIST
这个标志告知Libevent如果要用epoll的话,用基于"changelist"的后台方法会更快,也是安全的.如果在多次后台调度函数调用之间同一个文件描述符的状态发生了多次改变,使用epoll-changelist后台方法可以避免不必要的系统调用,当你向Libevent传递任何一个由dup()或是类似函数复制的文件描述符时,这样(指使用epoll-changelist)会触发一个导致很多错误结果的内核bug。
- EVENT_BASE_FLAG_PRECISE_TIMER
默认地，Libevent尝试使用当前操作系统所能提供的最快的计时机制.如果操作系统有一种慢一些的但是精度更高的计时机制的话,设置这个标志将告知Libevent使用这种更慢但精度更高的机制,如果没有,这个标志将不起作用.
上述所有对event_config操作的函数都有两个返回值：0代表成功,-1代表失败. 
|Note|很容易创建这样一个event_config:它要求你的操作系统并未提供的后台方法.以Libevent 2.0.1-alpha为例,在Windows下没有O(1)的后台方法,在Linux下没有同时满足 EV_FEATURE_FDS and EV_FEATURE_O1的后台方法.如果你要求一个Libevent无法满足的配置,event_base_new_with_config()会返回空.|
 Interface
```
int event_config_set_num_cpus_hint(struct event_config *cfg, int cpus)
```
这个函数目前只在windows平台下使用IOCP时有用,尽管未来在别的平台可能也会有用.调用这个函数告诉event_base在多线程处理时要充分利用给定数量的cpu.注意这只是一个(该函数对event_base的)暗示:event base可能使用更多或更少的CPU.
Interface
```
int event_config_set_max_dispatch_interval(struct event_config *cfg,
    const struct timeval *max_interval, int max_callbacks,
    int min_priority);
```
这个函数通过在检测高优先级事件之前限制可被触发的低优先级事件数量的方式避免了优先级翻转问题(注：[http://baike.baidu.com/view/2422471.htm?fr=aladdin](http://baike.baidu.com/view/2422471.htm?fr=aladdin)).如果max_interval是非空的,事件循环在每一次回调之后检测时间,如果超出max_interval指定的时间,则重新扫描高优先级事件.如果max_callbacks是非负的,在max_callbacks次回调被触发后,事件循环还会检测更多事件(If
 max_callbacks is nonnegative, the event loop also checks for more events after max_callbacks callbacks have been invoked).这些规则对任何优先级的事件都适用.
Example: Preferring edge-triggered backends
```
struct event_config *cfg;
struct event_base *base;
int i;
/* My program wants to use edge-triggered events if at all possible.  So
   I'll try to get a base twice: Once insisting on edge-triggered IO, and
   once not. */
for (i=0; i<2; ++i) {
    cfg = event_config_new();
    /* I don't like select. */
    event_config_avoid_method(cfg, "select");
    if (i == 0)
        event_config_require_features(cfg, EV_FEATURE_ET);
    base = event_base_new_with_config(cfg);
    event_config_free(cfg);
    if (base)
        break;
    /* If we get here, event_base_new_with_config() returned NULL.  If
       this is the first time around the loop, we'll try again without
       setting EV_FEATURE_ET.  If this is the second time around the
       loop, we'll give up. */
}
```
Example: Avoiding priority-inversion
```
struct event_config *cfg;
struct event_base *base;
cfg = event_config_new();
if (!cfg)
   /* Handle error */;
/* I'm going to have events running at two priorities.  I expect that
   some of my priority-1 events are going to have pretty slow callbacks,
   so I don't want more than 100 msec to elapse (or 5 callbacks) before
   checking for priority-0 events. */
struct timeval msec_100 = { 0, 100*1000 };
event_config_set_max_dispatch_interval(cfg, &msec_100, 5, 1);
base = event_base_new_with_config(cfg);
if (!base)
   /* Handle error */;
event_base_priority_init(base, 2);
```
这些函数和类型在<event2/event.h>中声明.
EVENT_BASE_FLAG_IGNORE_ENV标志始于Libevent 2.0.2-alpha.EVENT_BASE_FLAG_PRECISE_TIMER标志始于Libevent 2.1.2-alpha.event_config_set_num_cpus_hint()始于Libevent 2.0.7-rc,event_config_set_max_dispatch_interval()始于Libevent 2.1.1-alpha.这一部分提到的其它函数或类型始于Libevent
 2.0.1-alpha.
### 检测event_base的后台方法
有时你可能想知道一个event_base究竟含有哪些特征或者使用了哪种后台方法.
Interface
```
const char **event_get_supported_methods(void);
```
event_get_supported_methods()函数返回一个指向数组的指针,数组内存放的是当前版本Libevent支持的方法.数据内的最后一个元素为空.
Example
```
int i;
const char **methods = event_get_supported_methods();
printf("Starting Libevent %s.  Available methods are:\n",
    event_get_version());
for (i=0; methods[i] != NULL; ++i) {
    printf("    %s\n", methods[i]);
}
```
|Note|这个函数返回一个方法列表,Libevent是被编译成支持这些方法的.然而,你的操作系统可能实际上并不支持这些方法.比如,在某些OSX版本下kqueue使用起来是很多BUG的|
Interface
```
const char *event_base_get_method(const struct event_base *base);
enum event_method_feature event_base_get_features(const struct event_base *base);
```
event_base_get_method()返回一个在event_base中真正使用的方法名.event_base_get_features()返回一个它所支持的特性的位掩码.
Example
```
struct event_base *base;
enum event_method_feature f;
base = event_base_new();
if (!base) {
    puts("Couldn't get an event_base!");
} else {
    printf("Using Libevent with backend method %s.",
        event_base_get_method(base));
    f = event_base_get_features(base);
    if ((f & EV_FEATURE_ET))
        printf("  Edge-triggered events are supported.");
    if ((f & EV_FEATURE_O1))
        printf("  O(1) event notification is supported.");
    if ((f & EV_FEATURE_FDS))
        printf("  All FD types are supported.");
    puts("");
}
```
这些函数定义于<event2/event.h>.event_base_get_method()第一次出现于Libevent 1.4.3.其余的始于Libevent 2.0.1-alpha.
### 释放event_base
当你不再需要event_base,你可以用event_base_free()释放它.
Interface
```
void event_base_free(struct event_base *base);
```
注意这个函数不会释放掉任何正与event_base关联的事件,或是关闭与这些事件相关的sockets,或是释放它们的指针所指空间.
event_base_free()函数定义在<event2/event.h>中.始于Libevent 1.2.
### 设置优先级
Libevent支持对一个event设置多个优先级.默认地,一个event_base只支持一个优先级.你可以通过调用event_base_priority_init()为一个event_base设置优先级的数量.
Interface
```
int event_base_priority_init(struct event_base *base, int n_priorities);
```
该函数成功返回0,失败返回-1.参数base是待修改的event_base,n_priorties是支持的优先级的数量.n_priorities至少要是1.优先级从0(最重要)到n_priorities-1(最不重要).
有一个常量,EVENT_MAX_PRIORITIES,标识了n_priorities的最大值.调用上述函数时,传一个超过EVENT_MAX_PRIORITIES的n_priorities参数是错误的行为.
Note:你必须在任何事件变为活动的(active,即事件已触发)之前调用该函数.最好是在创建event_base之后就立刻调用该函数.
你可以通过调用event_base_getnpriorities()来查看当前的base支持的优先级数量.
Interface
```
int event_base_get_npriorities(struct event_base *base);
```
该函数的返回值为当前base中配置的优先级数量.所以如果event_base_get_npriorities()返回3,那么可选的优先级的值为0,1,2.
Example
```
For an example, see the documentation for event_priority_set below.
```
默认地,所有和base相关的事件会被初始化为优先级为n_priorities/2.
event_base_priority_init()定义于<event2/event.h>.它始于Libevent 1.0.event_base_get_npriorities()始于Libevent 2.1.1-alpha
### 在fork()之后重新初始化event_base
 不是所有的事件后台在fork()之后还可以保持干净.(Not all event backends persist cleanly after a call to fork()).所以,如果你的程序使用了fork()或相关的系统调用来开启一个新的进程,而你想继续使用之前的event_base,你需要重新初始化它.
Interface
```
int event_reinit(struct event_base *base);
```
该函数成功返回0,失败返回-1.
Example
```
struct event_base *base = event_base_new();
/* ... add some events to the event_base ... */
if (fork()) {
    /* In parent */
    continue_running_parent(base); /*...*/
} else {
    /* In child */
    event_reinit(base);
    continue_running_child(base); /*...*/
}
```
event_reinit()定义于<event2/event.h>.始于Libevent 1.4.3-alpha.
### 废弃的event_base函数
旧版的Libevent严重依赖于“当前”event_base这个概念.所谓的“当前”event_base,是一个被所有线程共享的全局设置.如果你忘了指定你想使用哪一个event_base,那么你得到的就是"当前"的.而由于event_base不是线程安全的,这种行为非常容易导致错误.
相较于如今版本的event_base_new(),过去有：
Interface
```
struct event_base *event_init(void);
```
这个函数类似于event_base_new(),它将分配的base设置为当前base.除此之外,没有别的改变当前base的方式了.
这部分提到的作用于当前base的函数有许多变体.这些(旧版本中的)函数和现在的函数功能类似,除了它们不接受base参数.
|当前版本|已弃用的版本|
|----|----|
|event_base_priority_init()|event_priority_init()|
|event_base_get_method()|event_get_method()|

