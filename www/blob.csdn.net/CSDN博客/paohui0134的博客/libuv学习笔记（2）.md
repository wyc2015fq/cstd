
# libuv学习笔记（2） - paohui0134的博客 - CSDN博客


2016年06月11日 15:32:54[paohui0134](https://me.csdn.net/paohui0134)阅读数：5278


# libuv学习笔记（2）
大致上以libuv文档中的顺序，对重要的结构体以及API深入学习。
## uv_loop_t结构体以及相关函数(windows平台)
loop的许多细节与具体的handle对象以及req相关，在学习完相关的内容之后再对loop进行补充与完善。
### 结构体定义
在uv.h文件中定义，定义如下：
```python
typedef
```
```python
struct
```
```python
uv_loop_s uv_loop_t;
```
```python
struct
```
```python
uv_loop_s{
```
```python
void
```
```python
*   data;
```
```python
//指向用户自定义数据
```
```python
unsigned
```
```python
int
```
```python
active_handles;
```
```python
//循环引用计数
```
```python
void
```
```python
*   handle_queue[
```
```python
2
```
```python
];
```
```python
//handle队列
```
```python
void
```
```python
*   active_reqs[
```
```python
2
```
```python
];
```
```python
//请求队列
```
```python
unsigned
```
```python
int
```
```python
stop_flag;
```
```python
//告知循环停止的内部标记
```
```python
UV_LOOP_PRIVATE_FIELDS;
```
```python
//私有成员宏
```
```python
}
```
私有成员宏UV_LOOP_PRIVATE_FIELDS展开如下：
```python
#define UV_LOOP_PRIVATE_FIELDS            \
```
```python
/*循环的完成端口句柄*/
```
```python
\
  HANDLE iocp;                              \
```
```python
/*当前时间，毫秒*/
```
```python
\
  uint_64 time;                             \
```
```python
/*等待处理的请求的单向循环队列的尾结点*/
```
```python
\
```
```python
/*如果队列为空，节点为空，如果只有一个节点*/
```
```python
\
```
```python
/*那么tail_->next_req == tail_*/
```
```python
\
  uv_req_t*  pending_req_tail;              \
```
```python
/*已关闭的句柄的单向列表的头节点*/
```
```python
\
  uv_handle_t* endgame_handles;             \
```
```python
/*定时器红黑树的头节点*/
```
```python
\
```
```python
struct
```
```python
uv_timer_tree_s timers;            \
```
```python
/*事件循环的3种监控handle列表*/
```
```python
\
  uv_prepare_t* prepare_handles;            \
  uv_check_t* check_handles;                \
  uv_idle_t* idle_handles;                  \
```
```python
/*指向即将被调用的handle。要能够再本次循环*/
```
```python
\
```
```python
/*正在结束时安全的移除*/
```
```python
\
  uv_prepare_t* next_prepare_handle;        \
```
```python
/*This handle holds the peer sockets for the fast variant of uv_poll_t*/
```
```python
\
  SOCKET poll_peer_sockets[UV_MSAFD_PROVIDER_COUNT]\
```
```python
/*活动的tcp流的计数*/
```
```python
\
```
```python
unsigned
```
```python
int
```
```python
active_tcp_stream;           \
```
```python
/*活动的udp流计数*/
```
```python
\
```
```python
unsigned
```
```python
int
```
```python
active_udp_stream;           \
```
```python
/*启动的定时器的计数*/
```
```python
\
  uint64_t timer_counter;                   \
```
```python
/*线程池*/
```
```python
\
```
```python
//所有需要在线程池中处理的请求结束之后或者被成功取消(uv_cancel)，会添加到该列表\
```
```python
void
```
```python
* wq[
```
```python
2
```
```python
];
  uv_mutex_t wq_mutex;                      \
  uv_async_t wq_async;
```
```python
//线程池中的请求处理结束之后，通过这个请求来wakeup  loop\
```
### 相关函数
#### 循环初始化，导出函数，在uv.h中声明，core.c中定义
```python
int
```
```python
uv_loop_init(uv_loop_t*  loop)
{
```
```python
int
```
```python
err;
```
```python
/*先初始化libuv本身*/
```
```python
uv__once_init();
```
```python
//本函数只会初始化一次，内部调用uv_once(),回调函数只会执行一次。
```
```python
/*创建iocp句柄*/
```
```python
loop->iocp=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,
```
```python
0
```
```python
,
```
```python
1
```
```python
);
```
```python
if
```
```python
(loop->iocp == null)
    Return uv_translate_sys_error(GetLastError());
```
```python
//将系统错误转换为UV_E*错误
```
```python
//为了防止使用未初始化的内存，在第一次调用uv_updata_time之前loop->timer必  须被初始化为0
```
```python
loop->time =
```
```python
0
```
```python
;
    uv_updata_time(loop);
```
```python
//内部调用uv_hrtime(UV__MILLISEC)来获取高精度时间
```
```python
QUEUE_INIT(&loop->wq);
```
```python
//初始化loop->wq双向链表
```
```python
QUEUE_INIT(&loop->handle_queue);
```
```python
//初始化loop->handle_queue双向链表
```
```python
QUEUE_INIT(&loop->active_reqs);
```
```python
//初始化loop->active_req双向链表
```
```python
loop->active_handles =
```
```python
0
```
```python
;
    loop->pending_req_tail = NULL;
    loop->endgame_handles = NULL;
    RB_INIT(&loop->timers);
```
```python
//初始化定时器红黑树，将头节点设为null
```
```python
loop->check_handles = NULL;
    loop->prepare_handle = NULL;
    loop->idle_handles = NULL;
    loop->next_prepare_handle = NULL;
    loop->next_check_handle = NULL;
    loop->next_idle_handle = NULL;
```
```python
memset
```
```python
(&loop->poll_peer_sockets,
```
```python
0
```
```python
,
```
```python
sizeof
```
```python
loop->poll_peer_sockets);
    loop->active_tcp_streams =
```
```python
0
```
```python
;
    loop->active_udp_streams =
```
```python
0
```
```python
;
    loop->timer_counter =
```
```python
0
```
```python
;
    loop->stop_flag =
```
```python
0
```
```python
;
    err = uv_mutex_init(&loop->wq_mutex);
```
```python
//初始化互斥量  底层通过临界区实现
```
```python
if
```
```python
(err)
        Goto  fail_mutex_init;
```
```python
//初始化异步handle
```
```python
err = uv_async_init(loop, &loop->wq_async, uv__work_done);
```
```python
if
```
```python
(err)
        Goto fail_async_init;
    uv__handle_unref(&loop->async);
```
```python
//uv_async_init改变了wq_async.flags = UV__HANDLE_REF
```
```python
loop->wq_async.flags |= UV__HANDLE_INTERNAL;
```
```python
return
```
```python
0
```
```python
;
    fail_async_init:
        uv_mutex_destroy(&loop->mutex);
    fail_mutex_inif:
        CloseHandle(loop->iocp);
        loop->iocp = INVALID_HANDLE_VALUE;
```
```python
return
```
```python
err;
}
```
#### 设置loop配置，导出函数，在uv.h中声明，uv-common.c中定义
windows平台不支持
```python
int
```
```python
uv_loop_configure(uv_loop_t*  loop,  uv_loop_option  option, ...)
{
    va_list  ap;
```
```python
int
```
```python
err;
    va_start(ap,  option);
```
```python
/*所有平台无关的配置都在此处理*/
```
```python
err = uv__loop_configure(loop,  option,  ap);
```
```python
//windows平台直接返回UV_ENOSYS(不支持)
```
```python
va_end(ap);
```
```python
return
```
```python
err;
}
```
#### 关闭loop，导出函数，在uv.h中声明，uv-common.c中定义
```python
int
```
```python
uv_loop_close(uv_loop_t*  loop)
{
    QUEUE*  q;
```
```python
//typedef void *QUEUE[2], q指向内含两个void*的数组
```
```python
uv_handle_t*  h;
```
```python
//判断loop中保存的请求双向列表是否为空
```
```python
if
```
```python
(!QUEUE_EMPTY(&(loop)->active_reqs))
        retirn  UV_EBUSY;
```
```python
//不为空
```
```python
QUEUE_FOREACH(q,  &loop->handle_queue)
    {
```
```python
//遍历队列
```
```python
//根据变量的偏移获取结构体指针
```
```python
//#define QUEUE_DATA(ptr, type, field)   \
```
```python
//((type *) ((char *) (ptr) - offsetof(type, field)))
```
```python
//handle队列中用到的是uv_handle_t的handle_queue成员
```
```python
h = QUEUE_DATA(q,  uv_handle_t,  handle_queue);
```
```python
//有任何不是内部使用的handle，表示还有handle出于激活状态，返回UV_EBUSY
```
```python
if
```
```python
(!(h->flags & UV_HANDLE_INTERNAL))
```
```python
return
```
```python
UV_EBUSY;
    }
```
```python
//内部调用uv__loop_close(loop)
```
```python
uv__loop_close(loop);
```
```python
//如果没有定义NDEBUG  ，也就是release版本
```
```python
#ifndef  NDEBUG
```
```python
memset
```
```python
(loop,  -
```
```python
1
```
```python
,
```
```python
sizeof
```
```python
(*loop));
```
```python
#endif
```
```python
if
```
```python
(loop == default_loop_ptr)
        default_loop_ptr = NULL;
```
```python
return
```
```python
0
```
```python
;
}
```
内部关闭函数，在uv-common.h中声明，core.c中定义
```python
void
```
```python
uv__loop_close(uv_loop_t*  loop)
{         
    size_t  i;
```
```python
/*关闭不需要额外循环迭代的异步handle*/
```
```python
//表示loop已经发出了一个异步消息（PostQueuedCompletionStatus），但是消息还没有被处理，需要在
```
```python
//下一个循环迭代中才能处理
```
```python
assert(!loop->wq_async.async_sent);
    loop->wq_async.close_cb = NULL;
    uv__handle_closing(&loop->wq_async);
    uv__handle_close(&loop->wq_async);
```
```python
for
```
```python
(i =
```
```python
0
```
```python
; i < ARRAY_SIZE(loop->poll_peer_sockets); i++)
    {
        SOCKET  sock = loop->poll_peer_sockets[i];
```
```python
if
```
```python
(sock !=
```
```python
0
```
```python
&& sock != INVALID_SOCKET)
            Closesocket(sock);
    }
    uv_mutex_lock(&loop->wq_mutex);
```
```python
//进入临界区
```
```python
assert(QUEUE_EMPTY(&loop->wq) &&
```
```python
"thread pool work queue not empty!"
```
```python
);
    assert(!uv__has_active_reqs(loop));
    uv_mutex_unlock(&loop->wq_mutex);
    uv_mutex_destroy(&loop->wq_mutex);
    CloseHandle(loop->iocp);
}
```
#### 获取默认loop，导出函数， 在uv.h中声明，在uv-common.c中定义
```python
uv_loop_t*  uv_default_loop(
```
```python
void
```
```python
)
{
```
```python
if
```
```python
(default_loop_ptr != NULL)
        Return loop_ptr;
```
```python
//初始化默认的loop
```
```python
if
```
```python
(uv_loop_init(&default_loop_struct))
        Return null;
```
```python
//返回默认loop的指针
```
```python
default_loop_ptr = &default_loop_struct;
```
```python
return
```
```python
default_loop_ptr;
}
```
#### 运行事件循环，导出函数，在uv.h文件中声明，在core.c文件中定义
与文档设计中的i/o循环流程相符
```python
int
```
```python
uv_run(uv_loop_t*  loop,   uv_run_mode  mode)
{
    DEWORD  timeout;
```
```python
int
```
```python
r;
```
```python
int
```
```python
ran_pending;
```
```python
void
```
```python
(*poll)(uv_loop_t* loop,  DWORD timeout);
```
```python
if
```
```python
(pGetQueuedCompletionStatusEx)
```
```python
//在uv_winapi_init函数中赋值
```
```python
poll = &uv_poll_ex;
```
```python
//使用pGetQueuedCompletionStatusEx
```
```python
else
```
```python
poll = &uv_poll;
```
```python
//使用GetQueuedCompletionStatus
```
```python
r = uv__loop_alive(loop);
```
```python
//判断是否存活，存活返回true
```
```python
if
```
```python
(!r)
        uv_update_time(loop);
```
```python
//更新当前时间
```
```python
while
```
```python
(r !=
```
```python
0
```
```python
&& loop->stop_flag ==
```
```python
0
```
```python
)
    {
```
```python
//循环alive
```
```python
uv_update_time(loop);
```
```python
//更新时间
```
```python
uv_process_timer(loop);
```
```python
//运行定时器
```
```python
ran_pending = uv_process_reqs(loop);
```
```python
//处理请求  详细步骤在req中再说明
```
```python
uv_idle_invoke(loop);
```
```python
//空转回调
```
```python
uv_prepare_invoke(loop);
```
```python
//预处理回调
```
```python
//设置超时
```
```python
timeout =
```
```python
0
```
```python
;
```
```python
if
```
```python
((mode == UV_RUN_ONCE && !ran_pending) || mode == UV_RUN_DEFAULT)
                timrout = uv_backend_timeout(loop);
```
```python
//获取超时
```
```python
//轮询IO，并将结果以req的形式添加到loop的pending列表，这点与API文档上的说明有点不同。
```
```python
(*poll)(loop,  timeout);       
        uv_check_invoke(loop);
```
```python
//check回调调用
```
```python
uv_process_endgames(loop);
```
```python
//处理所有关闭的handle，调用对应的uv_xxx_endgame
```
```python
if
```
```python
(mode == UV_RUN_ONCE)
        {
```
```python
/* UV_RUN_ONCE 意味着向前运行: 在返回之前至少调用一次回调。
            * uv__io_poll()可能在超时之后直接返回而并不进行I/O操作(没有回调函数)
            * 这意味着我们有一些满足继续运行条件的等待调用的定时器 
            *
            * UV_RUN_NOWAIT 不保证后续运行，所以check之后的步骤省略。
            */
```
```python
uv_process_timers(loop);
        }
        r = uv_loop_alive(loop);
```
```python
if
```
```python
(mode == UV_RUN_ONCE || mode == UV_RUN_NOWAIT)
            Break;
    }
```
```python
if
```
```python
(loop->stro_flag !=
```
```python
0
```
```python
)
        Loop->stop_flag =
```
```python
0
```
```python
;
```
```python
return
```
```python
r;
}
```
判断uv_loop_t是否是alive状态
```python
static
```
```python
int
```
```python
uv__loop_alive(
```
```python
const
```
```python
uv_loop_t*  loop)
{
```
```python
//判断loop是否存活  内部函数
```
```python
Return loop->active_handle >
```
```python
0
```
```python
||
```
```python
//有活动的handle
```
```python
!QUEUE_EMPTY(&loop->active_reqs) ||
```
```python
//有活动的请求
```
```python
Loop->endgame_handle != NULL;
```
```python
//有待关闭的handle
```
```python
}
```
处理所有定时器
```python
void
```
```python
uv_process_timer(uv_loop_t*  loop)
{
```
```python
//处理所有定时器
```
```python
uv_timer_t*  timer;
```
```python
for
```
```python
(timer = RB_MIN(uv_timer_tree_s,  &loop->timers);
```
```python
//定时器红黑树的最小节点
```
```python
timer != NULL && timer->due <= loop->time;
```
```python
//时间条件符合
```
```python
timer = RB_MIN(uv_timer_tree_s,  &loop->timers)) 
        {
            uv_timer_stop(timer);
```
```python
//停止定时器，从红黑树种删除
```
```python
uv_timer_again(timer);
```
```python
//再开定时器，如果有定时器repeat值不为0，调用start，否则不做任何操作
```
```python
timer->timer_cb((uv_timer_t*) timer);
```
```python
//调用回调函数
```
```python
}
}
```
#### 遍历循环中的handle，导出函数，uv.h中声明，uv-common.c中定义
```python
uv_walk(uv_loop_t
```
```python
*
```
```python
loop
```
```python
,  uv_walk_cb,
```
```python
void
```
```python
*
```
```python
arg)
{
```
```python
//遍历循环中的handle
```
```python
QUEUE
```
```python
queue
```
```python
;
```
```python
QUEUE
```
```python
*
```
```python
q;
    uv_handle_t
```
```python
*
```
```python
h;
    QUEUE_MOVE(
```
```python
&
```
```python
loop
```
```python
->
```
```python
handle_queue,
```
```python
&
```
```python
queue
```
```python
);
```
```python
//通过定义的宏遍历队列
```
```python
while
```
```python
(
```
```python
!
```
```python
QUEUE_EMPTY(
```
```python
&
```
```python
queue
```
```python
))
    {
        q
```
```python
=
```
```python
QUEUE_HEAD(
```
```python
&
```
```python
queue
```
```python
);
        h
```
```python
=
```
```python
QUEUE_DATA(q, uv_handle_t,  handle_queue);
        QUEUE_REMOVE(q);
        QUEUE_INSERT_TAIL(
```
```python
&
```
```python
loop
```
```python
->
```
```python
handle_queue, q);
```
```python
if
```
```python
(h
```
```python
->
```
```python
flags
```
```python
&
```
```python
UV__HANDLE_INTERNAL) continue;
```
```python
//所有非内部的handle，调用回调
```
```python
walk_cb(h,  arg);
    }
}
```
通过以上的一些函数可以简单的了解loop的运行流程，主要是一个单线程的循环，不断的从初始化时创建的完成端口中获取事件，同时为了兼顾定时器以及idle等监视器的回调，对于轮询（通过完成端口获取事件）的等待超时做了处理，根据不同的情况会有不同的超时值，有些类似于windows窗口的消息循环。
loop同时与libuv的线程池有关系，这个会学习线程池时分析。
不同的handle以及requite也会在之后的学习中具体分析，此处并没有深入研究。

