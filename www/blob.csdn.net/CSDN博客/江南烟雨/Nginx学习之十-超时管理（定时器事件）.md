# Nginx学习之十-超时管理（定时器事件） - 江南烟雨 - CSDN博客
2013年07月09日 19:03:37[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：12576
所属专栏：[Nginx模块开发与原理剖析](https://blog.csdn.net/column/details/nginxroad.html)
Nginx事件管理主要是网络事件和定时器事件。下面介绍定时器事件管理，即超时管理。
## 为什么要进行超时管理
Nginx有必要对可能发生超时的事件进行统一管理，并在事件超时时作出相应的处理，比如回收资源，返回错误等。举例来说，当客户端对nginx发出请求连接后，nginx机会accept()并建立对应的连接对象、读取额护短请求的头部信息。而读取这个头部信息显然是要在一定的时间内完成的。如果在一个有限的时间内没有读取到头部信息或者读取读取的头部信息不完整，那么nginx就无法进行正常处理，并且认为这是一个错误/非法的请求，直接返回错误信息并释放相应资源，如果nginx不这样做，那么针对如此的恶意攻击就和容易实施。
## 如何进行超时管理
对于超时管理，要解决两个问题：
（1）超时事件对象的组织
nginx采用的是红黑树。
（2）超时事件对象的超时检测
有两种方案：
一种是定时检测机制，通过设置定时器，没过一定时间就对红黑树管理的所有超时时间进行一次超级扫描。
另一种是先计算出距离当前最快发生超时的时间是多久。然后等待这个时间之后再去进行一次超时检测。
### 超时事件对象的组织
nginx中封装事件的结构体ngx_event_s中有几个字段与超时管理联系紧密：
```cpp
//超时
    unsigned         timedout:1;
    //事件存在于定时器中
    unsigned         timer_set:1;
    //定时器节点
    ngx_rbtree_node_t   timer;
```
Nginx设置了两个全局变量以便在程序的任何地方都可以哀诉的访问到这棵红棵树（src/event/ngx_event_timer.c）：
```cpp
ngx_thread_volatile ngx_rbtree_t  ngx_event_timer_rbtree;//超时管理的红黑树结构
static ngx_rbtree_node_t          ngx_event_timer_sentinel;//红黑树中的哨兵节点
```
红黑树结构的初始化
红黑树的初始化函数ngx_event_timer_init()是在函数ngx_event_process_init()函数内被调用的。即每个worker进程会在自身的初始化时建立这棵红黑树。
源代码（src/eventngx_event.c）：
```cpp
static ngx_int_t
ngx_event_process_init(ngx_cycle_t *cycle)
{
    ....
    if (ngx_event_timer_init(cycle->log) == NGX_ERROR) {
        return NGX_ERROR;
    }
    ...
}
```
函数ngx_event_timer_init完成了红黑树结构的初始化：
```cpp
/*
 * the event timer rbtree may contain the duplicate keys, however,
 * it should not be a problem, because we use the rbtree to find
 * a minimum timer value only
 */
ngx_int_t
ngx_event_timer_init(ngx_log_t *log)
{
    //红黑树初始化
    ngx_rbtree_init(&ngx_event_timer_rbtree, &ngx_event_timer_sentinel,
                    ngx_rbtree_insert_timer_value);
    //多线程处理
#if (NGX_THREADS)
    if (ngx_event_timer_mutex) {
        ngx_event_timer_mutex->log = log;
        return NGX_OK;
    }
    ngx_event_timer_mutex = ngx_mutex_init(log, 0);
    if (ngx_event_timer_mutex == NULL) {
        return NGX_ERROR;
    }
#endif
    return NGX_OK;
}
```
而ngx_rbtree_init(tree, s, i) 是宏定义（src/core/ngx_rbtree.h），即新建了一棵空的红黑树：
```cpp
#define ngx_rbtree_init(tree, s, i)                                           \
    ngx_rbtree_sentinel_init(s);                                              \
    (tree)->root = s;                                                         \
    (tree)->sentinel = s;                                                     \
    (tree)->insert = i
```
### 对事件进行超时监控
当需要对某个事件进行超时监控时，就会把它加入到这个红黑树内。比如，nginx调用accept接受到客户端的请求并建立对应的连接对象connection后，在连接对象的初始化函数ngx_http_init_connection()内，可以找到这样的代码：
```cpp
void
ngx_http_init_connection(ngx_connection_t *c)
{
...
(358L)ngx_add_timer(rev, c->listening->post_accept_timeout);
...
}
```
ngx_add_timer的第一个参数是事件对象，第二个参数是超时时限。
### 超时检测
Nginx具体使用哪一种超时检测方案取决于配置项timer_resolution，比如：
timer_resolution 100ms；
反应到nginx代码中就是全局变量ngx_timer_resolution的值为100；
下面我们看worker进程的核心处理函数ngx_process_events_and_timers（ngx_event.c）：
```cpp
void
ngx_process_events_and_timers(ngx_cycle_t *cycle)
{
    ngx_uint_t  flags;
    ngx_msec_t  timer, delta;
    if (ngx_timer_resolution) {
        timer = NGX_TIMER_INFINITE;
        flags = 0;
    } else {
        timer = ngx_event_find_timer();//将超时检测时间设置为最快发生超时的事件对象的超时时刻与当前时刻之差
        flags = NGX_UPDATE_TIME;
...
(void) ngx_process_events(cycle, timer, flags);
...
}
```
可以看到，ngx_timer_resolution是否为0影响了两个值timer和flags。
当ngx_timer_resolution不为0时，即方案1.timer为无限大。timer在函数ngx_process_events()内被用作事件机制被阻塞的最长时间。那么timer为无限大会不会导致事件处理机制无限等待而超时事件得不到及时处理呢？不会。因为正常情况下事件处理机制会监控到某些I/O事件的发生。即便是服务器太闲，没有任何I/O事件发生，工作进程也不会无限等待。因为工作进程一开始就设置好了一个定时器，这实现在初始化函数ngx_event_process_init()内，看代码：
```cpp
static ngx_int_t
ngx_event_process_init(ngx_cycle_t *cycle)
{
        ...
        sa.sa_handler = ngx_timer_signal_handler;
        sigemptyset(&sa.sa_mask);
        itv.it_interval.tv_sec = ngx_timer_resolution / 1000;
        itv.it_interval.tv_usec = (ngx_timer_resolution % 1000) * 1000;
        itv.it_value.tv_sec = ngx_timer_resolution / 1000;
        itv.it_value.tv_usec = (ngx_timer_resolution % 1000 ) * 1000;
        if (setitimer(ITIMER_REAL, &itv, NULL) == -1) {
            ngx_log_error(NGX_LOG_ALERT, cycle->log, ngx_errno,
                          "setitimer() failed");
        }
        ....
}
```
回调函数ngx_timer_signal_handler：
```cpp
static void
ngx_timer_signal_handler(int signo)
{
    ngx_event_timer_alarm = 1;
#if 1
    ngx_log_debug0(NGX_LOG_DEBUG_EVENT, ngx_cycle->log, 0, "timer signal");
#endif
}
```
可以看出它仅仅是将标志变量ngx_event_timer_alarm 设置为1.
只有在ngx_event_timer_alarm 为1 的情况下，工作进程才会更新时间。函数ngx_epoll_process_events中的代码（ngx_epoll_module.c）：
```cpp
static ngx_int_t
ngx_epoll_process_events(ngx_cycle_t *cycle, ngx_msec_t timer, ngx_uint_t flags)
{
...
    events = epoll_wait(ep, event_list, (int) nevents, timer);
    err = (events == -1) ? ngx_errno : 0;
    if (flags & NGX_UPDATE_TIME || ngx_event_timer_alarm) {
        ngx_time_update();
    }
...
}
```
在方案一的情况下，||前面的式子为假，那么ngx_event_timer_alarm 不为1 的情况下，更新函数ngx_time_update()不会被执行。那么会导致超时检测函数ngx_event_expire_timers不会被执行。看ngx_process_events_and_timers函数的代码（ngx_event.c）：
```cpp
void
ngx_process_events_and_timers(ngx_cycle_t *cycle)
{
...
    delta = ngx_current_msec;
    (void) ngx_process_events(cycle, timer, flags);//事件处理函数
    delta = ngx_current_msec - delta;
...
if (delta) {
        ngx_event_expire_timers();//超时检测函数
    }
...
}
```
当ngx_timer_resolution为0时，执行方案2。timer设置为最快发生超时的事件对象的超时时刻与当前时刻的时间差。具体计算时在函数ngx_event_find_timer内（ngx_event_timer.c）。
```cpp
ngx_msec_t
ngx_event_find_timer(void)
{
    ngx_msec_int_t      timer;
    ngx_rbtree_node_t  *node, *root, *sentinel;
    if (ngx_event_timer_rbtree.root == &ngx_event_timer_sentinel) {
        return NGX_TIMER_INFINITE;
    }
    ngx_mutex_lock(ngx_event_timer_mutex);
    root = ngx_event_timer_rbtree.root;
    sentinel = ngx_event_timer_rbtree.sentinel;
    node = ngx_rbtree_min(root, sentinel);
    ngx_mutex_unlock(ngx_event_timer_mutex);
    timer = (ngx_msec_int_t) (node->key - ngx_current_msec);
    return (ngx_msec_t) (timer > 0 ? timer : 0);
}
```
该函数从红黑树中找到key值最小的节点，然后用key值减去当前时刻即得到预期timer值。这个值可能是负数，表示已经有事件超时了。因此直接将其设置为0.那么事件处理 机制在开始监控I/O事件时会立即返回，以便马上处理这些超时事件。同时flags被设置为NGX_UPDATE_TIME。从ngx_epoll_process_events函数的代码中可以看出ngx_time_update()将被执行，事件被更新。即事件处理机制每次返回都会更新时间。如果I/O事件比较多，那么会导致比较频繁地调用gettimeofday()系统函数，这也可以说是超时检测方案2对性能的最大影响。这个时候超时检测函数ngx_event_expire_timers()函数会被执行。
下面看看超时检测函数ngx_event_expire_timers。其完成的主要任务是对超时事件对象是否超时进行扫描检测以及对已超时事件对象的处理。
检测是否有事件对象超时不需遍历扫描所有超时时间对象，而是找到最近的即将超时的超时事件对象。判断其是否超时，如果超时，则将其移出红黑树，设置超时标记，调用回调函数进行处理。之后再判断第二近的即将超时的超时事件对象，如此反复，知道某个超时事件对象还未超时或所有超时事件对象都已超时并处理完毕就结束检测。
此函数流程图：
![](https://img-blog.csdn.net/20130709185924921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhanVuMDcwNjEyMjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
下面是其核心代码：
```cpp
void
ngx_event_expire_timers(void)
{
    ngx_event_t        *ev;
    ngx_rbtree_node_t  *node, *root, *sentinel;
    sentinel = ngx_event_timer_rbtree.sentinel;
    //循环检测
    for ( ;; ) {
        ngx_mutex_lock(ngx_event_timer_mutex);
        root = ngx_event_timer_rbtree.root;
        if (root == sentinel) {
            return;
        }
        //找到最近的即将超时的超时事件对象
        node = ngx_rbtree_min(root, sentinel);
        /* node->key <= ngx_current_time */
        //如果已经超时
        if ((ngx_msec_int_t) (node->key - ngx_current_msec) <= 0) {
            ev = (ngx_event_t *) ((char *) node - offsetof(ngx_event_t, timer));
            ngx_log_debug2(NGX_LOG_DEBUG_EVENT, ev->log, 0,
                           "event timer del: %d: %M",
                           ngx_event_ident(ev->data), ev->timer.key);
            //从红黑树中移除这个已超时的超时事件对象
            ngx_rbtree_delete(&ngx_event_timer_rbtree, &ev->timer);
            ngx_mutex_unlock(ngx_event_timer_mutex);
#if (NGX_DEBUG)
            ev->timer.left = NULL;
            ev->timer.right = NULL;
            ev->timer.parent = NULL;
#endif
            //标记：是否已加入红黑树超时管理
            ev->timer_set = 0;
            //标记：是否超时
            ev->timedout = 1;
            //调用回调函数
            ev->handler(ev);
            continue;
        }
        break;
    }
    ngx_mutex_unlock(ngx_event_timer_mutex);
}
```
## 参考资料：
《深入剖析Nginx》
