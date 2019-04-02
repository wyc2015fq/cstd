# Libuv笔记

官网简介：http://docs.libuv.org/en/v1.x/guide.html

官网API：http://docs.libuv.org/en/v1.x

Github代码位置：https://github.com/libuv/libuv

libuv帮助文档:http://docs.libuv.org/en/v1.x/

源码压缩包地址：https://dist.libuv.org/dist/  （1.18.0版本，window有安装包，之后的没有）





# 第一节:概述



**本文基于libuv-v1.22.0进行解释说明，适合于异步调度使用者。要求有C语言基础。**

 

node.js 最初发起于 2009 年, 是一个可以让 Javascript 代码脱离浏览器的执行环境, libuv 使用了 Google 的 V8 执行引擎 和 Marc Lehmann 的 libev. Node.js 将事件驱动的 I/O 模型与适合该模型的编程语言(Javascript)融合在了一起, 随着 node.js 的日益流行, node.js 的开发者们也意识到应该让 node.js 在 Windows 平台下也能工作, 但是 libev 只能在 Unix 环境下运行. Windows 平台上与 kqueue(FreeBSD) 或者 (e)poll(Linux) 等内核事件通知相应的机制 是 IOCP, libuv 依据不同平台的特性(Unix 平台为 libev, Windows 平台为 IOCP) 给上层应用提供了统一基于 libev API 的抽象, 不过 node-v0.9.0 版本的 libuv 中 libev 的依赖已被移除.

# 编译：

## window编译：

执行代码目录的vcbuild.bat脚本。如果无法访问google网站，需要提前下载gyp放在build目录下。

Libuv的window编译参考博客：

<https://blog.csdn.net/valleyradio/article/details/73479756>

依赖:python2.7以及gyp,具体下载地址如下：

<https://www.python.org/downloads/windows/>

<https://github.com/adblockplus/gyp>

gyp解压后修改目录名称为gyp，放在libuv下的build目录(build没有则创建之)

## linux编译：

cd libuv

./autogen.sh

./configure

Make

 

参考资料：

1. [官网文档](http://docs.libuv.org/en/v1.x/)
2. [网络库libevent、libev、libuv对比](https://blog.csdn.net/lijinqi1987/article/details/71214974)





# 第二节:异步调度



## 异步调度逻辑

libuv是一个**高性能事件驱动库**，屏蔽了各种操作系统的差异从而提供了**统一的API**。libuv严格使用**异步、事件驱动**的编程风格。其核心工作是提供**事件循环**及基于 I/O或其他活动事件的**回调机制**。**libuv库包含了诸如计时器、非阻塞网络支持、异步文件系统访问、线程创建、子进程等核心工具**。

libuv有二个主要功能，一个是循环调度模块，也就是异步IO的核心Loop模块，一个是全局的线程池Thread Pool，loop模块主要是用于异步通知，thread pool主要用于线程管理和调度。libuv库里边用到线程池的地方有DNS相关的二个函数getaddrinfo和getnameinfo，文件各种异步操作。其他暂时不使用thread pool，thread pool主要是给调度者使用。

loop的调度一般通过二种模式调度，一种是Handle，一种是Request。Handle一般关联一个长期任务，request关联一个一次性任务。比如一个socket的监听任务，就是uv_udp_t（handle）,而一次数据的发送就是一个请求任务，关联请求句柄uv_udp_send_t（request）。

handle和req每一个变量里边都关联了一个 void* data，用于关联上下文，下图是req和handle的分类。所有的req类型都有一个UV_REQ_FIELDS结构，所以都可以强转为uf_req_t。所有的handle都有一个UV_HANDLE_FIELDS结构，所以都可以强制转化为uv_handle_t.

![img](https://img-blog.csdn.net/20180831094929779?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tub3dsZWRnZWJhbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下图是官网提供的架构图，TCP、UDP、TTY、PIPE等依赖系统的uv__io_t或IOCP异步机制实现异步IO的功能，FILE/DNS/用户代码依赖Thread Pool来实现异步IO机制。

libuv提供了一个线程池，可用于运行用户代码并在循环线程中得到通知。此线程池在内部用于运行所有文件系统操作，以及getaddrinfo和getnameinfo请求。其默认大小为4，但可以通过将UV_THREADPOOL_SIZE环境变量设置为任何值（绝对最大值为128，*putenv*("UV_THREADPOOL_SIZE=128")）在启动时更改 。线程池是全局的，并在所有事件循环中共享。当特定函数使用线程池时（即使用时），libuv会预分配并初始化允许的最大线程数。这会导致相对较小的内存开销（128个线程约为1MB），但会增加运行时的线程性能。

 ![img](https://img-blog.csdn.net/20180831095012557?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tub3dsZWRnZWJhbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下图是loop的while循环流程，程序中可以有多个loop，每个loop实际上是一个while死循环，不停的检测各种事件，然后触发各种回调，TCP/UDP/TTY/PIPE等都是while死循环触发的回调，所以回调里边不要干时间太长的事。File I/O,DNS Ops,User cord是线程池触发的回调，可以适当干点长时间的事，具体loop循环，可以定位uv_run，跟进代码详细了解。需要注意的是方式依赖loop的事件，必须执行vu_run之后才能触发。因为uv_run是一个死循环，所以会占用独立的线程，此线程只处理loop的事件。tcp或udp的主动触发，需要另外线程去触发。

![img](https://img-blog.csdn.net/20180831101004767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2tub3dsZWRnZWJhbw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下边引用官网的话（<http://docs.libuv.org/en/v1.x/design.html>）：

The I/O (or event) loop is the central part of libuv. It establishes the content for all I/O operations, and it’s meant to be tied to a single thread. One can run multiple event loops as long as each runs in a different thread. The libuv event loop (or any other API involving the loop or handles, for that matter) **is not thread-safe** except where stated otherwise.

The event loop follows the rather usual single threaded asynchronous I/O approach: all (network) I/O is performed on non-blocking sockets which are polled using the best mechanism available on the given platform: epoll on Linux, kqueue on OSX and other BSDs, event ports on SunOS and IOCP on Windows. As part of a loop iteration the loop will block waiting for I/O activity on sockets which have been added to the pollerand callbacks will be fired indicating socket conditions (readable, writable hangup) so handles can read, write or perform the desired I/O operation.

In order to better understand how the event loop operates, the following diagram illustrates all stages of a loop iteration:

上图：

1，The loop concept of ‘now’ is updated. The event loop caches the current time at the start of the event loop tick in order to reduce the number of time-related system calls.为了减少系统时间依赖性，循环开始位置更新当前时间指。

2，If the loop is *alive* an iteration(循环) is started, otherwise the loop will exit immediately. So, when is a loop considered to be *alive*? If a loop has active and ref’d handles, active requests or closing handles it’s considered to be *alive*.当loop存活的时候，

3，Due timers are run. All active timers scheduled for a time before the loop’s concept of *now* get their callbacks called.

4，Pending callbacks are called. All I/O callbacks are called right after polling for I/O, for the most part. There are cases, however, in which calling such a callback is deferred for the next loop iteration. If the previous iteration deferred any I/O callback it will be run at this point.

5，Idle handle callbacks are called. Despite the unfortunate name, idle handles are run on every loop iteration, if they are active.

6，Prepare handle callbacks are called. Prepare handles get their callbacks called right before the loop will block for I/O.

7，Poll timeout is calculated. Before blocking for I/O the loop calculates for how long it should block. These are the rules when calculating the timeout:

>   If the loop was run with the UV_RUN_NOWAIT flag, the timeout is 0.
>
>   If the loop is going to be stopped ([uv_stop](http://docs.libuv.org/en/v1.x/loop.html)[()](http://docs.libuv.org/en/v1.x/loop.html) was called), the timeout is 0.
>
>   If there are no active handles or requests, the timeout is 0.
>
>   If there are any idle handles active, the timeout is 0.
>
>   If there are any handles pending to be closed, the timeout is 0.
>
>   If none of the above cases matches, the timeout of the closest timer is taken, or if there are no active timers, infinity.

8，The loop blocks for I/O. At this point the loop will block for I/O for the duration calculated in the previous step. All I/O related handles that were monitoring a given file descriptor for a read or write operation get their callbacks called at this point.

9，Check handle callbacks are called. Check handles get their callbacks called right after the loop has blocked for I/O. Check handles are essentially the counterpart of prepare handles.

10，Close callbacks are called. If a handle was closed by calling [uv_close](http://docs.libuv.org/en/v1.x/handle.html)[()](http://docs.libuv.org/en/v1.x/handle.html) it will get the close callback called.

11，Special case in case the loop was run with UV_RUN_ONCE, as it implies forward progress. It’s possible that no I/O callbacks were fired after blocking for I/O, but some time has passed so there might be timers which are due, those timers get their callbacks called.

12，Iteration ends. If the loop was run with UV_RUN_NOWAIT or UV_RUN_ONCE modes the iteration ends and [uv_run](http://docs.libuv.org/en/v1.x/loop.html)[()](http://docs.libuv.org/en/v1.x/loop.html) will return. If the loop was run with UV_RUN_DEFAULT it will continue from the start if it’s still *alive*, otherwise it will also end.

**Important：**

 libuv uses a thread pool to make asynchronous file I/O operations possible, but network I/O is **always** performed in a single thread, each loop’s thread.

**Note：**

 While the polling mechanism is different, libuv makes the execution model consistent across Unix systems and Windows.

采用最小堆算法。

## loop相关API

```cpp
uv_loop_t* uv_default_loop(void);
int uv_loop_init(uv_loop_t* loop);
int uv_loop_close(uv_loop_t* loop);
uv_loop_t* uv_loop_new(void);
void uv_loop_delete(uv_loop_t*);
size_t uv_loop_size(void);
int uv_loop_alive(const uv_loop_t* loop);
int uv_loop_configure(uv_loop_t* loop, uv_loop_option option, ...);
int uv_loop_fork(uv_loop_t* loop);
```

<http://docs.libuv.org/en/v1.x/loop.html>

**uv_default_loop****：**获取默认的队列

**uv_loop_init****：**队列初始化，参数loop必须allocate

**uv_loop_close****：**

Releases all internal loop resources. Call this function only when the loop has finished executing and all open handles and requests have been closed, or it will return UV_EBUSY. After this function returns, the user can free the memory allocated for the loop.

关闭队列，执行此函数的前提是所有任务必须都执行完成，如果有任务遗留，会返回UV_EBUSY.

uv_loop_new，uv_loop_delete：*malloc*+uv_loop_init，uv_loop_close+free，不推荐使用

uv_loop_size：

**uv_loop_alive****：**

Returns non-zero if there are referenced active handles, active requests or closing handles in the loop.

Loop里边有任务(不管uv_run是否启动)，返回非0，否则返回0。一般uv_run运行返回后以及添加任务区，返回0.

**uv_loop_configure****：**

Set additional loop options. You should normally call this before the first call to [uv_run](http://docs.libuv.org/en/v1.x/loop.html?highlight=uv_loop_configure)[()](http://docs.libuv.org/en/v1.x/loop.html?highlight=uv_loop_configure) unless mentioned otherwise.

Returns 0 on success or a UV_E* error code on failure. Be prepared to handle UV_ENOSYS; it means the loop option is not supported by the platform.

Supported options:

UV_LOOP_BLOCK_SIGNAL: Block a signal when polling for new events. The second argument to [uv_loop_configure](http://docs.libuv.org/en/v1.x/loop.html?highlight=uv_loop_configure)[()](http://docs.libuv.org/en/v1.x/loop.html?highlight=uv_loop_configure) is the signal number.

This operation is currently only implemented for SIGPROF signals, to suppress unnecessary wakeups when using a sampling profiler. Requesting other signals will fail with UV_EINVAL.

设置loop的属性，此接口在uv_loop之前调用。目前option仅支持SIGPROF，阻塞指定信号。

\#ifdef _WIN32

ASSERT(UV_ENOSYS == uv_loop_configure(&loop, UV_LOOP_BLOCK_SIGNAL, 0));

\#else

ASSERT(0 == uv_loop_configure(&loop, UV_LOOP_BLOCK_SIGNAL, SIGPROF));

\#endif

**uv_loop_fork****：**复制loop

## loop运行

```cpp
int uv_run(uv_loop_t*, uv_run_mode mode);
void uv_stop(uv_loop_t*);
 
void uv_update_time(uv_loop_t*);
uint64_t uv_now(const uv_loop_t*);
 
int uv_backend_fd(const uv_loop_t*);
int uv_backend_timeout(const uv_loop_t*);
```

参考网址：

> Libuv源码分析：<https://www.cnblogs.com/watercoldyi/p/5675180.html>
>
> IO模型及select、poll、epoll和kqueue的区别**:** <https://www.cnblogs.com/linganxiong/p/5583415.html>
>
> Reactor模式--VS--Proactor模式 :<https://blog.csdn.net/wenbingoon/article/details/9880365>

**uv_run****:**

运行loop

This function runs the event loop. It will act differently depending on the specified mode:

UV_RUN_DEFAULT: Runs the event loop until there are no more active and referenced handles or requests. Returns non-zero if [uv_stop](http://docs.libuv.org/en/v1.x/loop.html?highlight=uv_run)[()](http://docs.libuv.org/en/v1.x/loop.html?highlight=uv_run) was called and there are still active handles or requests. Returns zero in all other cases.阻塞运行到直到手动停止或没有被任何请求引用。

UV_RUN_ONCE: Poll for i/o once. Note that this function blocks if there are no pending callbacks. Returns zero when done (no active handles or requests left), or non-zero if more callbacks are expected (meaning you should run the event loop again sometime in the future). UV_ONCE,阻塞模式运行，直到处理了一次请求。

UV_RUN_NOWAIT: Poll for i/o once but don’t block if there are no pending callbacks. Returns zero if done (no active handles or requests left), or non-zero if more callbacks are expected (meaning you should run the event loop again sometime in the future).运行一次循环流程

**uv_stop****:**

uv_stop() can be used to stop an event loop. The earliest the loop will stop running is *on the next iteration*, possibly later. This means that events that are ready to be processed in this iteration of the loop will still be processed, so uv_stop() can’t be used as a kill switch. When uv_stop() is called, the loop **won’t** block for i/o on this iteration. The semantics of these things can be a bit difficult to understand, so let’s look at uv_run() where all the control flow occurs.

停止uv_run运行，停止运行后，loop里的任务还是有效的，只是不在执行，再次调用uv_run还可以继续运行。

**uv_now****：**

Return the current timestamp in milliseconds. The timestamp is cached at the start of the event loop tick, see [uv_update_time](http://docs.libuv.org/en/v1.x/loop.html?highlight=uv_now)[()](http://docs.libuv.org/en/v1.x/loop.html?highlight=uv_now) for details and rationale.

Note：Use [uv_hrtime](http://docs.libuv.org/en/v1.x/misc.html)[()](http://docs.libuv.org/en/v1.x/misc.html) if you need sub-millisecond granularity.

获取loop时间loop->time，单位毫秒，如果需要微妙进度，请使用uv_hrtime().loop在uv_run过程中，此值才会不断更新。

**uv_update_time****：**

Update the event loop’s concept of “now”. Libuv caches the current time at the start of the event loop tick in order to reduce the number of time-related system calls.

You won’t normally need to call this function unless you have callbacks that block the event loop for longer periods of time, where “longer” is somewhat subjective but probably on the order of a millisecond or more.

立刻刷新loop的当前时间loop->time，目的是使**uv_now**更精确

**uv_backend_fd：**

Get backend file descriptor. Only kqueue, epoll and event ports(端口事件) are supported.

This can be used in conjunction with *uv_run**(loop, UV_RUN_NOWAIT)* to poll in one thread and run the event loop’s callbacks in another see test/test-embed.c for an example.

Note:Embedding a kqueue fd in another kqueue pollset doesn’t work on all platforms. It’s not an error to add the fd but it never generates events.

**uv_backend_timeout：**

Get the poll timeout. The return value is in milliseconds, or -1 for no timeout.

下一个Loop到来时间。单位毫秒。

## handle相关：

```cpp
int uv_is_active(const uv_handle_t* handle);
int uv_is_closing(const uv_handle_t* handle);
void uv_close(uv_handle_t* handle, uv_close_cb close_cb);
 
void uv_ref(uv_handle_t*);
void uv_unref(uv_handle_t*);
int uv_has_ref(const uv_handle_t*);
 
size_t uv_handle_size(uv_handle_type type);
uv_handle_type uv_handle_get_type(const uv_handle_t* handle);
const char* uv_handle_type_name(uv_handle_type type);
 
uv_loop_t* uv_handle_get_loop(const uv_handle_t* handle);
void* uv_handle_get_data(const uv_handle_t* handle);
void uv_handle_set_data(uv_handle_t* handle, void* data);
 
int uv_send_buffer_size(uv_handle_t* handle, int* value);
int uv_recv_buffer_size(uv_handle_t* handle, int* value);
 
int uv_fileno(const uv_handle_t* handle, uv_os_fd_t* fd);
```

<http://docs.libuv.org/en/v1.x/handle.html>

The libuv event loop (if run in the default mode) will run until there are no active *and* referenced handles left. The user can force the loop to exit early by unreferencing handles which are active, for example by calling [uv_unref](http://docs.libuv.org/en/v1.x/handle.html?highlight=uv_ref)[()](http://docs.libuv.org/en/v1.x/handle.html?highlight=uv_ref) after calling [uv_timer_start](http://docs.libuv.org/en/v1.x/timer.html)[()](http://docs.libuv.org/en/v1.x/timer.html).

A handle can be referenced or unreferenced, the refcounting scheme doesn’t use a counter, so both operations are idempotent.

All handles are referenced when active by default, see [uv_is_active](http://docs.libuv.org/en/v1.x/handle.html?highlight=uv_ref)[()](http://docs.libuv.org/en/v1.x/handle.html?highlight=uv_ref) for a more detailed explanation on what being *active*involves.

 

**uv_is_active:**

Returns non-zero if the handle is active, zero if it’s inactive. What “active” means depends on the type of handle:

A uv_async_t handle is always active and cannot be deactivated, except by closing it with uv_close().

A uv_pipe_t, uv_tcp_t, uv_udp_t, etc. handle - basically any handle that deals with i/o - is active when it is doing something that involves i/o, like reading, writing, connecting, accepting new connections, etc.

A uv_check_t, uv_idle_t, uv_timer_t, etc. handle is active when it has been started with a call to uv_check_start(), uv_idle_start(), etc.

Rule of thumb: if a handle of type *uv_foo_t* has a *uv_foo_start**()* function, then it’s active from the moment that function is called. Likewise, *uv_foo_stop**()* deactivates the handle again.

请求是否处于活动状态，执行uv_*start后，就处于active状态，uv_*stop后，处于非active状态，此状态不可修改。

**uv_is_closing:** Returns non-zero if the handle is closing or closed, zero otherwise.

Note: This function should only be used between the initialization of the handle and the arrival of the close callback.

此接口仅在创建于结束之间有效，如果结束后，结果无实际意义.

**uv_close:** 从endgame队列中删除

Request handle to be closed. *close_cb* will be called asynchronously after this call. This MUST be called on each handle before memory is released.

Handles that wrap file descriptors are closed immediately but *close_cb* will still be deferred to the next iteration of the event loop. It gives you a chance to free up any resources associated with the handle.

In-progress requests, like uv_connect_t or uv_write_t, are cancelled and have their callbacks called asynchronously with status=UV_ECANCELED.

关闭指定句柄，回调可以为空（同步返回）,比如关闭定时器、idle、prepare等，一般和各种init成对出现。

**uv_ref:**

Reference the given handle. References are idempotent, that is, if a handle is already referenced calling this function again will have no effect.

指定句柄为ref状态，默认handle执行Start之后，默认含有此状态。

**uv_unref:**

Un-reference the given handle. References are idempotent, that is, if a handle is not referenced calling this function again will have no effect.

取消引用，多次无效(标志设置)，设置后不作为loop循环运行的条件，但是pool如果有其它事件的话，timer回调会继续触发。

一个应用场景就是，loop中，创建一个timer用于回收一些其他req或者handle的资源，创建timer之后，就可以通过uv_unref调用，减除timer的引用，当其他handle或者req全部退出的时候，loop也可以正常退出。

**uv_has_ref:**

Returns non-zero if the handle referenced, zero otherwise.

**uv_handle_get_type:**

Returns *handle->type**。*

*获取**handle**类型*

**uv_handle_size****:**

Returns the size of the given handle type. Useful for FFI binding writers who don’t want to know the structure layout.

获取handle类型的编号

**uv_handle_type_name****:**

Returns the name for the equivalent struct for a given handle type, e.g. *“pipe”* (as in [uv_pipe_t](http://docs.libuv.org/en/v1.x/pipe.html)) for *UV_NAMED_PIPE*.

If no such handle type exists, this returns *NULL*.

获取类型描述名称

**uv_handle_get_loop****：**

Returns *handle->loop*.

**uv_handle_get_data****：**

Returns *handle->data*.

**uv_handle_set_data****：**

Sets *handle->data* to *data*.

**uv_send_buffer_size****：**

Gets or sets the size of the send buffer that the operating system uses for the socket.

If **value* == 0, it will return the current send buffer size, otherwise it will use **value* to set the new send buffer size.

This function works for TCP, pipe and UDP handles on Unix and for TCP and UDP handles on Windows.

Note：Linux will set double the size and return double the size of the original set value.

设置/获取，发送缓存大小，注意：linux会设置二倍大小

**uv_recv_buffer_size****：**

Gets or sets the size of the receive buffer that the operating system uses for the socket.

If **value* == 0, it will return the current receive buffer size, otherwise it will use **value* to set the new receive buffer size.

This function works for TCP, pipe and UDP handles on Unix and for TCP and UDP handles on Windows.

Note：Linux will set double the size and return double the size of the original set value.

**uv_fileno****：**

Gets the platform dependent file descriptor equivalent.

The following handles are supported: TCP, pipes, TTY, UDP and poll. Passing any other handle type will fail with *UV_EINVAL*.

If a handle doesn’t have an attached file descriptor yet or the handle itself has been closed, this function will return *UV_EBADF*.

Warning：Be very careful when using this function. libuv assumes it’s in control of the file descriptor so any change to it may lead to malfunction.

提取句柄。比如：handle->io_watcher.fd;

## 线程通信

```cpp
int uv_async_init(uv_loop_t*,
                            uv_async_t* async,
                            uv_async_cb async_cb);
int uv_async_send(uv_async_t* async);
```

<http://docs.libuv.org/en/v1.x/async.html>

线程之间通信。

**uv_async_init****：**Initialize the handle. A NULL callback is allowed.

Returns:0 on success, or an error code < 0 on failure.**Note**

Unlike other handle initialization functions, it immediately starts the handle.

初始化async句柄。

**uv_async_send****：**

Wake up the event loop and call the async handle’s callback.

Returns:0 on success, or an error code < 0 on failure.**Note**

It’s safe to call this function from any thread. The callback will be called on the loop thread.

Warning：libuv will coalesce calls to [uv_async_send](http://docs.libuv.org/en/v1.x/async.html?highlight=uv_async_init)[()](http://docs.libuv.org/en/v1.x/async.html?highlight=uv_async_init), that is, not every call to it will yield an execution of the callback. For example: if [uv_async_send](http://docs.libuv.org/en/v1.x/async.html?highlight=uv_async_init)[()](http://docs.libuv.org/en/v1.x/async.html?highlight=uv_async_init) is called 5 times in a row before the callback is called, the callback will only be called once. If [uv_async_send](http://docs.libuv.org/en/v1.x/async.html?highlight=uv_async_init)[()](http://docs.libuv.org/en/v1.x/async.html?highlight=uv_async_init) is called again after the callback was called, it will be called again.

通知async句柄，此函数是线程安全的，但是多次发送**uv_async_send**有可能只触发一次uv_async_cb。

## 异步消息

```cpp
int uv_idle_init(uv_loop_t*, uv_idle_t* idle);
int uv_idle_start(uv_idle_t* idle, uv_idle_cb cb);
int uv_idle_stop(uv_idle_t* idle);
 
int uv_prepare_init(uv_loop_t*, uv_prepare_t* prepare);
int uv_prepare_start(uv_prepare_t* prepare, uv_prepare_cb cb);
int uv_prepare_stop(uv_prepare_t* prepare);
 
int uv_check_init(uv_loop_t*, uv_check_t* check);
int uv_check_start(uv_check_t* check, uv_check_cb cb);
int uv_check_stop(uv_check_t* check);
```

<http://docs.libuv.org/en/v1.x/prepare.html>

https://www.cnblogs.com/watercoldyi/p/5682344.html

uv__handle_init() -> uv__handle_start() -> uv__handle_stop() - > uv__handle_closing() -> uv_want_endgame()-> uv__handle_close()

1,  init，初始化，返回句柄。

2，start，添加句柄到执行队列，准备执行，设置回调，如果handle已经存在，添加无效。不管是否有效，都返回0

3，stop，从执行队列移到endgame队列。

一般idle作为辅助消息起作用，一般和其他消息一块运行，用来检测其他消息是否真正执行。或者是否执行完成。

 

参考资料：

1，[uv_loop_t — Event loop](http://docs.libuv.org/en/v1.x/loop.html)，[Design overview](http://docs.libuv.org/en/v1.x/design.html)

2，[libuv的源码分析(1)](https://www.cnblogs.com/watercoldyi/p/5675180.html)，[libuv源码分析(2)](https://www.cnblogs.com/watercoldyi/p/5682344.html)

3，[IO模型及select、poll、epoll和kqueue的区别](https://www.cnblogs.com/linganxiong/p/5583415.html)

4，[Reactor模式--VS--Proactor模式](https://blog.csdn.net/wenbingoon/article/details/9880365)

5，[uv_handle_t — Base handle](http://docs.libuv.org/en/v1.x/handle.html)，[uv_async_t — Async handle](http://docs.libuv.org/en/v1.x/async.html)，[uv_prepare_t — Prepare handle](http://docs.libuv.org/en/v1.x/prepare.html)

------







# 第三节:文件系统





libuv provides a wide variety of cross-platform sync and async file system operations. All functions defined in this document take a callback, which is allowed to be NULL. If the callback is NULL the request is completed synchronously, otherwise it will be performed asynchronously.

All file operations are run on the threadpool. See [Thread pool work scheduling](http://docs.libuv.org/en/v1.x/threadpool.html) for information on the threadpool size.

文件相关的函数以uv_fs_开头，结构统一使用uv_fs_t，文件操作和网络操作不同，网络操作是底层异步API支持非阻塞，而文件系统内部是阻塞调用，通过线程池实现异步通知。文件操作有两种模式：同步模式(回调为NULL)和异步模式。

所有操作回调可以设置为空，如果回调为空执行同步返回，一般对文件的频繁读写，需要使用同步模式。因为异步不保证内容写入的先后顺序，如果对一个文件多次写入有先后顺序要求的话，只能通过同步模式进行。文件的异步回调方式，每个函数的操作都会从线程池里关联一个线程进行操作。

API官网地址：<http://docs.libuv.org/en/v1.x/fs.html>

## 文件句柄操作相关

```cpp
void uv_fs_req_cleanup(uv_fs_t* req);
uv_buf_t uv_buf_init(char* base, unsigned int len);
 
int uv_fs_open(uv_loop_t* loop, uv_fs_t* req, const char* path, int flags, int mode, uv_fs_cb cb);
int uv_fs_read(uv_loop_t* loop,uv_fs_t* req,uv_file file,const uv_buf_t bufs[], unsigned int nbufs, int64_t offset,uv_fs_cb cb);
int uv_fs_write(uv_loop_t* loop,uv_fs_t* req,uv_file file,uv_buf_t bufs[],unsigned int nbufs,int64_t offset,uv_fs_cb cb);
int uv_fs_close(uv_loop_t* loop,uv_fs_t* req,uv_file file,uv_fs_cb cb);
 
int uv_fs_fsync(uv_loop_t* loop,uv_fs_t* req,uv_file file, uv_fs_cb cb);
int uv_fs_fdatasync(uv_loop_t* loop,uv_fs_t* req,uv_file file, uv_fs_cb cb);
int uv_fs_ftruncate(uv_loop_t* loop,uv_fs_t* req,uv_file file, int64_t offset, uv_fs_cb cb);
 
uv_fs_type uv_fs_get_type(const uv_fs_t*);
ssize_t uv_fs_get_result(const uv_fs_t*);
void* uv_fs_get_ptr(const uv_fs_t*);
const char* uv_fs_get_path(const uv_fs_t*);
uv_stat_t* uv_fs_get_statbuf(uv_fs_t*);
```

**uv_fs_req_cleanup：**

Cleanup request. Must be called after a request is finished to deallocate any memory libuv might have allocated.

清空请求，每次发起请求后，最后必须调用的函数，用于释放libuv可能申请的内存。

**uv_buf_init：**

功能：构造一个uv_buf_t结构，uv_buf_t结构含有数据和长度，一般用于uv_fs_write.

**uv_fs_open：**

功能：打开文件，linux下封装open，window下封装*CreateFileW* （window仅支持二进制模式打开）。

参数：

req：输入输出参数，req->result成功的话，返回文件句柄(>0)，否则返回错误码(<0),**下同**

Flags，mode：unix flags,libuv在window环境之中，会自动对应到window的参数。

**uv_fs_read：**

功能：读多组数据，等价于preadv.

参数：

file:打开的文件句柄，bufs：数据数组(含数据和长度)，nbufs：bufs个数，offset：偏移量

**uv_fs_write：**

功能：写多组数据，等同于pwritev.

参数：参考uv_fs_read

**uv_fs_close:**

功能：关闭文件，linux下封装close。

参数：参考uv_fs_open

**uv_fs_fsync:**刷新文件，类似flushes

**uv_fs_fdatasync:**同上

**uv_fs_ftruncate:**截取文件或者补充文件到指定大小。

**uv_fs_get_type****/uv_fs_get_result****/uv_fs_get_ptr****/uv_fs_get_path****/uv_fs_get_statbuf**

返回结构特uv_fs_t的各种参数，具体详见[uv_fs_t](http://docs.libuv.org/en/v1.x/fs.html?highlight=uv_fs_open)。

## 文件直接操作

```cpp
int uv_fs_rename(uv_loop_t* loop, uv_fs_t* req, const char* path, const char* new_path, uv_fs_cb cb);
int uv_fs_unlink(uv_loop_t* loop,uv_fs_t* req, const char* path, uv_fs_cb cb);
int uv_fs_copyfile(uv_loop_t* loop, uv_fs_t* req, const char* path, const char* new_path, int flags, uv_fs_cb cb);
int uv_fs_sendfile(uv_loop_t* loop, uv_fs_t* req, uv_file out_fd, uv_file in_fd, int64_t in_offset, size_t length, uv_fs_cb cb);
```

**uv_fs_rename:**改名字

**uv_fs_unlink:**

功能：删除文件，封装的[unlink(2)](http://linux.die.net/man/2/unlink).

参数：参考uv_fs_open

**uv_fs_copyfile:**拷贝文件，

参数：

flags：

*UV_FS_COPYFILE_EXCL*: 如果目标文件已经存在，返回*UV_EEXIST，不执行。*

*UV_FS_COPYFILE_FICLONE*: 如果目标文件存在，尝试创建copy-on-write reflink，如果平台不支持copy-on-write reflink，那就先删除，后拷贝

*UV_FS_COPYFILE_FICLONE_FORCE*:同上，但是，如果平台不支持copy-on-write reflink，那就返回失败

**uv_fs_sendfile:**等同于copyfile

## 目录操作相关

```cpp
int uv_fs_mkdir(uv_loop_t* loop,uv_fs_t* req,const char* path,int mode,uv_fs_cb cb);
int uv_fs_mkdtemp(uv_loop_t* loop,uv_fs_t* req,const char* tpl,uv_fs_cb cb);
int uv_fs_rmdir(uv_loop_t* loop,uv_fs_t* req,const char* path,uv_fs_cb cb);
int uv_fs_scandir(uv_loop_t* loop,uv_fs_t* req,const char* path,int flags,uv_fs_cb cb);
int uv_fs_scandir_next(uv_fs_t* req, uv_dirent_t* ent);
```

**uv_fs_mkdir:**创建目录

**uv_fs_mkdtemp:**The result can be found as a null terminated string at *req->path*.

**uv_fs_rmdir:**删除目录

**uv_fs_scandir:**浏览目录文件

**uv_fs_scandir_next:**目录下一个文件

## 文件属性相关

```cpp
int uv_fs_stat(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb);
int uv_fs_fstat(uv_loop_t* loop,uv_fs_t* req,uv_file file,uv_fs_cb cb);
int uv_fs_lstat(uv_loop_t* loop,uv_fs_t* req,const char* path,uv_fs_cb cb);
int uv_fs_access(uv_loop_t* loop, uv_fs_t* req, const char* path, int mode, uv_fs_cb cb);
int uv_fs_utime(uv_loop_t* loop,uv_fs_t* req,const char* path,double atime,double mtime,uv_fs_cb cb);
int uv_fs_futime(uv_loop_t* loop, uv_fs_t* req, uv_file file, double atime, double mtime, uv_fs_cb cb);
int uv_fs_chmod(uv_loop_t* loop,uv_fs_t* req,const char* path,int mode,uv_fs_cb cb);
int uv_fs_fchmod(uv_loop_t* loop, uv_fs_t* req, uv_file file, int mode, uv_fs_cb cb);
int uv_fs_chown(uv_loop_t* loop,uv_fs_t* req,const char* path,uv_uid_t uid,uv_gid_t gid,uv_fs_cb cb);
int uv_fs_fchown(uv_loop_t* loop, uv_fs_t* req, uv_file file, uv_uid_t uid, uv_gid_t gid, uv_fs_cb cb);
int uv_fs_lchown(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_uid_t uid, uv_gid_t gid, uv_fs_cb cb);
 
```

**uv_fs_stat:**文件状态，同linux的stat

**uv_fs_fstat:**文件状态，同linux的fstat

**uv_fs_lstat:**文件状态，同linux的lstat

**uv_fs_access:**检测是否可用。Equivalent to [access(2)](http://linux.die.net/man/2/access) on Unix. Windows uses GetFileAttributesW().

**uv_fs_utime:**修改文件时间

**uv_fs_futime:**同上

**uv_fs_chmod:**检测执行权限，等同于chmod

**uv_fs_fchmod:**检测执行权限，等同于fchmod

**uv_fs_chown:**更改所有者及所属组，等同于chown() 

**uv_fs_fchown:**更改所有者及所属组，等同于fchown() 

**uv_fs_lchown:**更改所有者及所属组，等同于lchown() 

## 文件链接相关

```cpp
int uv_fs_link(uv_loop_t* loop, uv_fs_t* req, const char* path, const char* new_path, uv_fs_cb cb);
int uv_fs_symlink(uv_loop_t* loop,uv_fs_t* req,const char* path,const char* new_path,int flags,uv_fs_cb cb);
int uv_fs_readlink(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb);
int uv_fs_realpath(uv_loop_t* loop, uv_fs_t* req, const char* path, uv_fs_cb cb);
```

**uv_fs_link:**创建硬链接，等同于link

**uv_fs_symlink:**创建符号链接,等同于symlink。

**uv_fs_readlink:**读取链接，结果放在r*eq->**ptr*.

**uv_fs_realpath:**相对路径转换为绝对路径。

Equivalent to [realpath](http://linux.die.net/man/3/realpath)[(3)](http://linux.die.net/man/3/realpath) on Unix. Windows uses [GetFinalPathNameByHandle](https://msdn.microsoft.com/en-us/library/windows/desktop/aa364962(v=vs.85).aspx). The resulting string is stored in *req->**ptr*.

## 文件监听相关

```cpp
int uv_fs_event_init(uv_loop_t* loop, uv_fs_event_t* handle);
int uv_fs_event_start(uv_fs_event_t* handle,uv_fs_event_cb cb, const char* path, unsigned int flags);
int uv_fs_event_stop(uv_fs_event_t* handle);
int uv_fs_event_getpath(uv_fs_event_t* handle, char* buffer, size_t* size);
int uv_fs_poll_init(uv_loop_t* loop, uv_fs_poll_t* handle);
int uv_fs_poll_start(uv_fs_poll_t* handle, uv_fs_poll_cb poll_cb, const char* path, unsigned int interval);
int uv_fs_poll_stop(uv_fs_poll_t* handle);
int uv_fs_poll_getpath(uv_fs_poll_t* handle, char* buffer, size_t* size);
 
```

官网API：<http://docs.libuv.org/en/v1.x/fs_event.html>

官网API：<http://docs.libuv.org/en/v1.x/fs_poll.html>

FS Poll handles allow the user to monitor a given path for changes. Unlike [uv_fs_event_t](http://docs.libuv.org/en/v1.x/fs_event.html), fs poll handles use *stat* to detect when a file has changed so they can work on file systems where fs event handles can’t.

FS Event handles allow the user to monitor a given path for changes, for example, if the file was renamed or there was a generic change in it. This handle uses the best backend for the job on each platform.

FS Event在有些系统(AIX、z/OS)使用有限制，而pool不受系统影响。

**uv_fs_\*_init:**初始化监听句柄

**uv_fs_\*_start:**开始监听

**uv_fs_\*_stop:**停止监听

**uv_fs_\*_getpath:**通过监听句柄，获取监听路径。buffer和size是输出参数，buffer需要提前申请好。

 



# 第四节:网络相关





Networking in libuv is not much different from directly using the BSD socket interface, some things are easier, all are non-blocking, but the concepts stay the same. In addition libuv offers utility functions to abstract the annoying, repetitive and low-level tasks like setting up sockets using the BSD socket structures, DNS lookup, and tweaking various socket parameters.The uv_tcp_t and uv_udp_t structures are used for network I/O.

libuv中的网络与直接使用BSD套接字接口没什么不同，有些东西更容易，都是非阻塞的，但概念保持不变。此外，libuv还提供实用程序功能来抽象恼人的，重复的和低级别的任务，例如使用BSD套接字结构设置套接字，DNS查找以及调整各种套接字参数。这里网络相关的主要涉及TCP和UDP，由于TCP是Stream流，而PIPE和TTY也是Stream类，所以他们有共同的结构体，libuv将TCP、PIPE、TTY归为Stream流所以这里一并对PIPE和TTY的相关API也做一个简单介绍。

下边是TCP、PIPE、TTY的结构体。他们有相同的结构  UV_HANDLE_FIELDS，UV_STREAM_FIELDS，所以TCP、PIPE、TTY可以强制转换成Stream类型。

```cpp
/* uv_stream_t is a subclass of uv_handle_t.
 * uv_stream is an abstract class.
 * uv_stream_t is the parent class of uv_tcp_t, uv_pipe_t and uv_tty_t. */
struct uv_stream_s {
  UV_HANDLE_FIELDS
  UV_STREAM_FIELDS
};
/*uv_pipe_t is a subclass of uv_stream_t.
 * Representing a pipe stream or pipe server. On Windows this is a Named
 * Pipe. On Unix this is a Unix domain socket.*/
struct uv_pipe_s {
  UV_HANDLE_FIELDS
  UV_STREAM_FIELDS
  int ipc; /* non-zero if this pipe is used for passing handles */
  UV_PIPE_PRIVATE_FIELDS
};
/*
 * uv_tty_t is a subclass of uv_stream_t.
 * Representing a stream for the console.
 */
struct uv_tty_s {
  UV_HANDLE_FIELDS
  UV_STREAM_FIELDS
  UV_TTY_PRIVATE_FIELDS
};
/*
 * uv_tcp_t is a subclass of uv_stream_t.
 * Represents a TCP stream or TCP server.
 */
struct uv_tcp_s {
  UV_HANDLE_FIELDS
  UV_STREAM_FIELDS
  UV_TCP_PRIVATE_FIELDS
};
```

## Stream基类相关

这里边的函数可以被PIPE、TTY、TCP调用，不同的类型可能会有一些差异，但是表示的功能相同。官网API：<http://docs.libuv.org/en/v1.x/stream.html>

```cpp
int uv_shutdown(uv_shutdown_t* req, uv_stream_t* handle, uv_shutdown_cb cb)
 
int uv_listen(uv_stream_t* stream, int backlog, uv_connection_cb cb);
int uv_accept(uv_stream_t* server, uv_stream_t* client);
 
int uv_read_start(uv_stream_t*,uv_alloc_cb alloc_cb,uv_read_cb read_cb);
int uv_read_stop(uv_stream_t*);
 
int uv_is_readable(const uv_stream_t* handle);
int uv_is_writable(const uv_stream_t* handle);
int uv_stream_set_blocking(uv_stream_t* handle, int blocking);
 
int uv_write(uv_write_t* req,uv_stream_t* handle,const uv_buf_t bufs[],unsigned int nbufs,uv_write_cb cb);
int uv_write2(uv_write_t* req,uv_stream_t* handle,const uv_buf_t bufs[],unsigned int nbufs,uv_stream_t* send_handle,uv_write_cb cb);
int uv_try_write(uv_stream_t* handle,const uv_buf_t bufs[],unsigned int nbufs);
 
size_t uv_stream_get_write_queue_size(const uv_stream_t* stream);
```

<http://docs.libuv.org/en/v1.x/stream.html>

**uv_shutdown****：**关闭连接，此关闭会等待写操作完成**。**

**uv_listen****：**开始监听，backlog表示监听个数。同linux的[listen(2)](http://linux.die.net/man/2/listen)。

**uv_accept****：**

接收请求，一般在[uv_connection_cb](http://docs.libuv.org/en/v1.x/stream.html?highlight=uv_connect_cb)回调中调用，只允许调用一次。多次会失败。

*此接口只能在**bind**的回调里边处理。**server* and *client* must be handles running on the same loop.

**uv_read_start****:**

Read data from an incoming stream. The [uv_read_cb](http://docs.libuv.org/en/v1.x/stream.html?highlight=uv_connect_cb) callback will be made several times until there is no more data to read or [uv_read_stop](http://docs.libuv.org/en/v1.x/stream.html?highlight=uv_connect_cb)[()](http://docs.libuv.org/en/v1.x/stream.html?highlight=uv_connect_cb) is called.

开始读数据

**uv_read_stop****:**

Stop reading data from the stream. The [uv_read_cb](http://docs.libuv.org/en/v1.x/stream.html?highlight=uv_connect_cb) callback will no longer be called.

This function is idempotent and may be safely called on a stopped stream.

停止读数据

**uv_is_readable**

Returns 1 if the stream is readable, 0 otherwise.

**uv_is_writable****：**

Returns 1 if the stream is writable, 0 otherwise.

**uv_stream_set_blocking****：**

Enable or disable blocking mode for a stream.

当block被设置为1后，所有的操作都会被置为同步，window仅pipe支持，不要过度依赖此接口，将来libuv可能废弃此接口。

uv_stre此接口在开始位置就应该被设置，当有reg在队列中时，设置可能失效。

**am_get_write_queue_size****：**

Returns *stream->**write_queue_size*.

**uv_write****：**

Write data to stream. Buffers are written in order. Example:

Note：The memory pointed to by the buffers must remain valid until the callback gets called. This also holds for[uv_write2()](http://docs.libuv.org/en/v1.x/stream.html?highlight=uv_connect_cb).

写数据，buffer空间在回调之前必须保持有效。

**u****v_write2****：**

Extended write function for sending handles over a pipe. The pipe must be initialized with *ipc* == 1.

Note： *send_handle* must be a TCP socket or pipe, which is a server or a connection (listening or connected state). Bound sockets or pipes will be assumed to be servers.

uv_write的扩展，多一个handle参数。通过pipe发送一个handle，此pipe必须使用ipc==1初始化。hand必须是tcpsocket server或者是pipe。

**uv_try_write****：**

Same as [uv_write](http://docs.libuv.org/en/v1.x/stream.html?highlight=uv_connect_cb)[()](http://docs.libuv.org/en/v1.x/stream.html?highlight=uv_connect_cb), but won’t queue a write request if it can’t be completed immediately.

Will return either:

\> 0: number of bytes written (can be less than the supplied buffer size).

< 0: negative error code (UV_EAGAIN is returned if no data can be sent immediately).

同uv_write, 并且不关联req的同步写操作，类似于直接调用系统API。

## TTY

<http://docs.libuv.org/en/v1.x/tty.html>

TTY handles represent a stream for the console.

[`uv_tty_t`](http://docs.libuv.org/en/v1.x/tty.html#c.uv_tty_t) is a ‘subclass’ of [`uv_stream_t`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_stream_t).

在[Linux](https://baike.baidu.com/item/Linux)中，TTY也许是跟终端有关系的最为混乱的术语。TTY是TeleTYpe的一个老缩写。Teletypes，或者teletypewriters，原来指的是[电传打字机](https://baike.baidu.com/item/%E7%94%B5%E4%BC%A0%E6%89%93%E5%AD%97%E6%9C%BA)，是通过串行线用打印机键盘通过阅读和发送信息的东西，和古老的[电报机](https://baike.baidu.com/item/%E7%94%B5%E6%8A%A5%E6%9C%BA)区别并不是很大。之后，当计算机只能以[批处理](https://baike.baidu.com/item/%E6%89%B9%E5%A4%84%E7%90%86)方式运行时（当时穿孔卡片[阅读器](https://baike.baidu.com/item/%E9%98%85%E8%AF%BB%E5%99%A8)是唯一一种使程序载入运行的方式），电传打字机成为唯一能够被使用的“实时”输入/[输出设备](https://baike.baidu.com/item/%E8%BE%93%E5%87%BA%E8%AE%BE%E5%A4%87)。最终，电传打字机被键盘和显示器终端所取代，但在终端或TTY接插的地方，操作系统仍然需要一个程序来监视[串行端口](https://baike.baidu.com/item/%E4%B8%B2%E8%A1%8C%E7%AB%AF%E5%8F%A3)。一个getty“Get TTY”的处理过程是：一个程序监视物理的TTY/终端接口。对一个[虚拟网络](https://baike.baidu.com/item/%E8%99%9A%E6%8B%9F%E7%BD%91%E7%BB%9C)控制台（[VNC](https://baike.baidu.com/item/VNC)）来说，一个伪装的TTY(Pseudo-TTY，即假冒的TTY，也叫做“PTY”）是等价的终端。当你运行一个[xterm](https://baike.baidu.com/item/xterm)([终端仿真](https://baike.baidu.com/item/%E7%BB%88%E7%AB%AF%E4%BB%BF%E7%9C%9F)程序）或GNOME终端程序时，PTY对虚拟的用户或者如xterm一样的[伪终端](https://baike.baidu.com/item/%E4%BC%AA%E7%BB%88%E7%AB%AF)来说，就像是一个TTY在运行。“Pseudo”的意思是“duplicating in a fake way”（用伪造的方法复制），它相比“virtual”或“emulated”更能真实的说明问题。而在的计算中，它却处于被放弃的阶段。

[串行端口](https://baike.baidu.com/item/%E4%B8%B2%E8%A1%8C%E7%AB%AF%E5%8F%A3)终端（Serial Port Terminal）是使用计算机串行端口连接的[终端设备](https://baike.baidu.com/item/%E7%BB%88%E7%AB%AF%E8%AE%BE%E5%A4%87)。计算机把每个串行端口都看作是一个字符设备。有段时间这些串行端口设备通常被称为终端设备，因为那时它的最大用途就是用来连接终端。这些串行端口所对应的设备名称是/dev/tts/0（或/dev/ttyS0),/dev/tts/1（或/dev/ttyS1）等，设备号分别是（4,0），（4,1）等，分别对应于DOS系统下的COM1、COM2等。若要向一个端口发送数据，可以在命令行上把标准输出重定向到这些特殊文件名上即可。例如，在命令行提示符下键入：echo test > /dev/ttyS1会把单词”test”发送到连接在ttyS1(COM2）端口的设备上。

```cpp
typedef enum {
  /* Initial/normal terminal mode */
  UV_TTY_MODE_NORMAL,
  /* Raw input mode (On Windows, ENABLE_WINDOW_INPUT is also enabled) */
  UV_TTY_MODE_RAW,
  /* Binary-safe I/O mode for IPC (Unix-only) */
  UV_TTY_MODE_IO
} uv_tty_mode_t;
 
int uv_tty_init(uv_loop_t*, uv_tty_t*, uv_file fd, int readable);
int uv_tty_set_mode(uv_tty_t*, uv_tty_mode_t mode);
int uv_tty_reset_mode(void);
int uv_tty_get_winsize(uv_tty_t*, int* width, int* height);
 
uv_handle_type uv_guess_handle(uv_file file);
```

http://docs.libuv.org/en/v1.x/tty.html

**uv_tty_init****:**

Initialize a new TTY stream with the given file descriptor. Usually the file descriptor will be:

0 = stdin

1 = stdout

2 = stderr

*readable*, specifies if you plan on calling [uv_read_start](http://docs.libuv.org/en/v1.x/stream.html)[()](http://docs.libuv.org/en/v1.x/stream.html) with this stream. stdin is readable, stdout is not.

On Unix this function will determine the path of the fd of the terminal using [ttyname_r](http://linux.die.net/man/3/ttyname_r)[(3)](http://linux.die.net/man/3/ttyname_r), open it, and use it if the passed file descriptor refers to a TTY. This lets libuv put the tty in non-blocking mode without affecting other processes that share the tty.

This function is not thread safe on systems that don’t support ioctl TIOCGPTN or TIOCPTYGNAME, for instance OpenBSD and Solaris.

Note: If reopening the TTY fails, libuv falls back to blocking writes for non-readable TTY streams.

*Changed in version 1.9.0::* the path of the TTY is determined by [ttyname_r](http://linux.die.net/man/3/ttyname_r)[(3)](http://linux.die.net/man/3/ttyname_r). In earlier versions libuvopened */dev/**tty*instead.

*Changed in version 1.5.0::* trying to initialize a TTY stream with a file descriptor that refers to a file returns *UV_EINVAL* on UNIX.

**uv_tty_set_mode****:**

the mode is specified as a [uv_tty_mode_t](http://docs.libuv.org/en/v1.x/tty.html) value.

Set the TTY using the specified terminal mode.

**uv_tty_reset_mode****:**

To be called when the program exits. Resets TTY settings to default values for the next process to take over.

This function is async signal-safe on Unix platforms but can fail with error code UV_EBUSY if you call it when execution is inside [uv_tty_set_mode](http://docs.libuv.org/en/v1.x/tty.html)[()](http://docs.libuv.org/en/v1.x/tty.html).

**uv_tty_get_winsize****:**

Gets the current Window size. On success it returns 0.

获取window大小。

## PIPE

Pipe handles provide an abstraction over local domain sockets on Unix and named pipes on Windows.

[`uv_pipe_t`](http://docs.libuv.org/en/v1.x/pipe.html#c.uv_pipe_t) is a ‘subclass’ of [`uv_stream_t`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_stream_t).

```cpp
int uv_pipe_init(uv_loop_t*, uv_pipe_t* handle, int ipc);
int uv_pipe_open(uv_pipe_t*, uv_file file);
int uv_pipe_bind(uv_pipe_t* handle, const char* name);
void uv_pipe_connect(uv_connect_t* req,
                               uv_pipe_t* handle,
                               const char* name,
                               uv_connect_cb cb);
int uv_pipe_getsockname(const uv_pipe_t* handle,
                                  char* buffer,
                                  size_t* size);
int uv_pipe_getpeername(const uv_pipe_t* handle,
                                  char* buffer,
                                  size_t* size);
void uv_pipe_pending_instances(uv_pipe_t* handle, int count);
int uv_pipe_pending_count(uv_pipe_t* handle);
uv_handle_type uv_pipe_pending_type(uv_pipe_t* handle);
int uv_pipe_chmod(uv_pipe_t* handle, int flags);
```

**uv_pipe_init**

Initialize a pipe handle. The *ipc* argument is a boolean to indicate if this pipe will be used for handle passing between processes.

初始化一个pipe，ipc表示是否跨进程。

**uv_pipe_open**

Open an existing file descriptor or HANDLE as a pipe.

打开以及已经存在的handle。

**uv_pipe_bind**

Bind the pipe to a file path (Unix) or a name (Windows).

绑定路径(Unix)或者名称(Windows)。

**uv_pipe_connect**:

Connect to the Unix domain socket or the named pipe.

连接pipe。

**uv_pipe_getsockname****：**

Get the name of the Unix domain socket or the named pipe.

获取pipe名字，buffer不含结束符，size不包含结束符。

**uv_pipe_getpeername**

Get the name of the Unix domain socket or the named pipe to which the handle is connected.

获取对方pipe的名称。

**uv_pipe_pending_instances**

Set the number of pending pipe instance handles when the pipe server is waiting for connections.

Note：This setting applies to Windows only.

设置待处理句柄个数，仅适用于window

**uv_pipe_pending_count****：**

**uv_pipe_pending_type****：**

Used to receive handles over IPC pipes.

First - call [uv_pipe_pending_count](http://docs.libuv.org/en/v1.x/pipe.html)[()](http://docs.libuv.org/en/v1.x/pipe.html), if it’s > 0 then initialize a handle of the given *type*, returned by [uv_pipe_pending_type](http://docs.libuv.org/en/v1.x/pipe.html)[()](http://docs.libuv.org/en/v1.x/pipe.html) and call uv_accept(pipe, handle).

Pipe待处理任务个数，uv_accept使用之前调用。

**uv_pipe_chmod****：**

Alters pipe permissions, allowing it to be accessed from processes run by different users. Makes the pipe writable or readable by all users. Mode can be UV_WRITABLE, UV_READABLE or UV_WRITABLE | UV_READABLE. This function is blocking.

*此函数是同步的，设置**pipe**的权限。*

## TCP

<http://docs.libuv.org/en/v1.x/tcp.html>

TCP handles are used to represent both TCP streams and servers.

[`uv_tcp_t`](http://docs.libuv.org/en/v1.x/tcp.html#c.uv_tcp_t) is a ‘subclass’ of [`uv_stream_t`](http://docs.libuv.org/en/v1.x/stream.html#c.uv_stream_t).

> Server sockets proceed by:
>
>   **uv_tcp_init** the TCP handle.
>
>   **uv_tcp_bind** it.
>
>   Call **uv_listen** on the handle to have a callback invoked whenever a new connection is   established by a client.
>
>   Use **uv_accept** to accept the connection.
>
>   Use **stream operations** to communicate with the client.

```cpp
int uv_tcp_init(uv_loop_t*, uv_tcp_t* handle);
int uv_tcp_init_ex(uv_loop_t*, uv_tcp_t* handle, unsigned int flags);
int uv_tcp_open(uv_tcp_t* handle, uv_os_sock_t sock);
int uv_tcp_nodelay(uv_tcp_t* handle, int enable);
int uv_tcp_keepalive(uv_tcp_t* handle,
                               int enable,
                               unsigned int delay);
int uv_tcp_simultaneous_accepts(uv_tcp_t* handle, int enable);
int uv_tcp_bind(uv_tcp_t* handle,
const struct sockaddr* addr, unsigned int flags);
int uv_tcp_getsockname(const uv_tcp_t* handle,
struct sockaddr* name, int* namelen);
int uv_tcp_getpeername(const uv_tcp_t* handle,
struct sockaddr* name, int* namelen);
int uv_tcp_connect(uv_connect_t* req,uv_tcp_t* handle,
const struct sockaddr* addr, uv_connect_cb cb);
```

**uv_tcp_init:**Initialize the handle. No socket is created as of yet.

**uv_tcp_init_ex:**Initialize the handle with the specified flags. At the moment only the lower 8 bits of the *flags* parameter are used as the socket domain. A socket will be created for the given domain. If the specified domain is AF_UNSPEC no socket is created, just like [uv_tcp_init](http://docs.libuv.org/en/v1.x/tcp.html?highlight=uv_tcp_init)[()](http://docs.libuv.org/en/v1.x/tcp.html?highlight=uv_tcp_init).

flags：AF_UNIX域、AF_INET域、AF_UNSPEC域等。

**uv_tcp_open:**Open an existing file descriptor or SOCKET as a TCP handle.

*Changed in version 1.2.1:* the file descriptor is set to non-blocking mode.

Note： The passed file descriptor or SOCKET is not checked for its type, but it’s required that it represents a valid stream socket.

**打开句柄**

**uv_tcp_nodelay:**Enable *TCP_NODELAY*, which disables Nagle’s algorithm.

https://www.cnblogs.com/wajika/p/6573014.html

**uv_tcp_keepalive:**Enable / disable TCP keep-alive. *delay* is the initial delay in seconds, ignored when *enable* is zero.

**uv_tcp_simultaneous_accepts:**Enable / disable simultaneous asynchronous accept requests that are queued by the operating system when listening for new TCP connections.

This setting is used to tune a TCP server for the desired performance. Having simultaneous accepts can significantly improve the rate of accepting connections (which is why it is enabled by default) but may lead to uneven load distribution in multi-process setups.

是否开启系统默认的同步接收。默认是开启的，同步接收accett

**uv_tcp_bind:**Bind the handle to an address and port. *addr* should point to an initialized struct sockaddr_in or struct sockaddr_in6.

When the port is already taken, you can expect to see an UV_EADDRINUSE error from either [uv_tcp_bind](http://docs.libuv.org/en/v1.x/tcp.html?highlight=uv_tcp_init)[()](http://docs.libuv.org/en/v1.x/tcp.html?highlight=uv_tcp_init), [uv_listen](http://docs.libuv.org/en/v1.x/stream.html)[()](http://docs.libuv.org/en/v1.x/stream.html)or [uv_tcp_connect](http://docs.libuv.org/en/v1.x/tcp.html?highlight=uv_tcp_init)[()](http://docs.libuv.org/en/v1.x/tcp.html?highlight=uv_tcp_init). That is, a successful call to this function does not guarantee that the call to [uv_listen](http://docs.libuv.org/en/v1.x/stream.html)[()](http://docs.libuv.org/en/v1.x/stream.html) or [uv_tcp_connect](http://docs.libuv.org/en/v1.x/tcp.html?highlight=uv_tcp_init)[()](http://docs.libuv.org/en/v1.x/tcp.html?highlight=uv_tcp_init) will succeed as well.

*flags*可以包含UV_TCP_IPV6ONLY，在这种情况下，禁用双栈支持并且仅使用IPv6。

**uv_tcp_getsockname:**Get the current address to which the handle is bound. *addr* must point to a valid and big enough chunk of memory, structsockaddr_storage is recommended for IPv4 and IPv6 support.

获取本地地址。

**uv_tcp_getpeername:**Get the address of the peer connected to the handle. *addr* must point to a valid and big enough chunk of memory, structsockaddr_storage is recommended for IPv4 and IPv6 support.

获取对方的地址。

**uv_tcp_connect:**Establish an IPv4 or IPv6 TCP connection. Provide an initialized TCP handle and an uninitialized [uv_connect_t](http://docs.libuv.org/en/v1.x/stream.html). *addr*should point to an initialized struct sockaddr_in or struct sockaddr_in6.

On Windows if the *addr* is initialized to point to an unspecified address (0.0.0.0 or ::) it will be changed to point to localhost. This is done to match the behavior of Linux systems.

客户端与tcp的Server建立连接。

## UDP

UDP handles encapsulate UDP communication for both clients and servers.

<http://docs.libuv.org/en/v1.x/udp.html>

```cpp
int uv_udp_init(uv_loop_t*, uv_udp_t* handle);
int uv_udp_init_ex(uv_loop_t*, uv_udp_t* handle, unsigned int flags);
int uv_udp_open(uv_udp_t* handle, uv_os_sock_t sock);
int uv_udp_bind(uv_udp_t* handle,
                          const struct sockaddr* addr,
                          unsigned int flags);
int uv_udp_send(uv_udp_send_t* req,uv_udp_t* handle, const uv_buf_t bufs[], unsigned int nbufs, const struct sockaddr* addr, uv_udp_send_cb send_cb);
int uv_udp_try_send(uv_udp_t* handle,const uv_buf_t bufs[], unsigned int nbufs, const struct sockaddr* addr);
size_t uv_udp_get_send_queue_size(const uv_udp_t* handle);
size_t uv_udp_get_send_queue_count(const uv_udp_t* handle);
 
int uv_udp_recv_start(uv_udp_t* handle,uv_alloc_cb alloc_cb, uv_udp_recv_cb recv_cb);
int uv_udp_recv_stop(uv_udp_t* handle);
 
```

**uv_udp_t****:**

/* uv_udp_t is a subclass of uv_handle_t. */

struct uv_udp_s {

  UV_HANDLE_FIELDS

  size_t send_queue_size;//待发送字节数

  size_t send_queue_count;//请求发送个数

  UV_UDP_PRIVATE_FIELDS

};

**uv_udp_init****:**

Initialize a new UDP handle. The actual socket is created lazily. Returns 0 on success.

**uv_udp_init_ex****:**

Initialize the handle with the specified flags. At the moment the lower 8 bits of the *flags* parameter are used as the socket domain. A socket will be created for the given domain. If the specified domain is AF_UNSPEC no socket is created, just like [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

**uv_udp_open****:**

Opens an existing file descriptor or Windows SOCKET as a UDP handle.

Unix only: The only requirement of the *sock* argument is that it follows the datagram contract (works in unconnected mode, supports sendmsg()/recvmsg(), etc). In other words, other datagram-type sockets like raw sockets or netlink sockets can also be passed to this function.

*Changed in version 1.2.1:* the file descriptor is set to non-blocking mode.

Note:The passed file descriptor or SOCKET is not checked for its type, but it’s required that it represents a valid datagram socket.

打开init后的udp句柄

**uv_udp_bind****：**
Bind the UDP handle to an IP address and port.

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

**addr** – *struct* *sockaddr_in* or *struct sockaddr_in6* with the address and port to bind to.

**flags** – Indicate how the socket will be bound, UV_UDP_IPV6ONLY and UV_UDP_REUSEADDR are supported.

Returns:0 on success, or an error code < 0 on failure.

**uv_udp_send****：**

Send data over the UDP socket. If the socket has not previously been bound with [uv_udp_bind](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html) it will be bound to 0.0.0.0 (the “all interfaces” IPv4 address) and a random port number.

On Windows if the *addr* is initialized to point to an unspecified address (0.0.0.0 or ::) it will be changed to point to localhost. This is done to match the behavior of Linux systems.

Parameters:**req** – UDP request handle. Need not be initialized（malloc）.

**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

**bufs** – List of buffers to send.

**nbufs** – Number of buffers in *bufs*.

**addr** – *struct* *sockaddr_in* or *struct sockaddr_in6* with the address and port of the remote peer.

**send_cb** – Callback to invoke when the data has been sent out.

Returns:0 on success, or an error code < 0 on failure.

*Changed in version 1.19.0:* added 0.0.0.0 and :: to localhost mapping

**uv_udp_try_send：**

Same as [uv_udp_send](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html), but won’t queue a send request if it can’t be completed immediately.

Returns:>= 0: number of bytes sent (it matches the given buffer size). < 0: negative error code (UV_EAGAIN is returned when the message can’t be sent immediately).

没有send_cb,不会加入发送队列，直接发送，其他同vu_udp_send.

**uv_udp_get_send_queue_size****:**

Returns *handle->**send_queue_size*.

Number of bytes queued for sending. This field strictly shows how much information is currently queued.

 

**uv_udp_get_send_queue_count****:**

Returns *handle->**send_queue_count*.

Number of send requests currently in the queue awaiting to be processed.

队列中的req请求数目。

 

**uv_udp_recv_start****：**

Prepare for receiving data. If the socket has not previously been bound with [uv_udp_bind](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html) it is bound to 0.0.0.0 (the “all interfaces” IPv4 address) and a random port number.

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

**alloc_cb** – Callback to invoke when temporary storage is needed.

**recv_cb** – Callback to invoke with received data.

Returns:0 on success, or an error code < 0 on failure.

 

**uv_udp_recv_stop****：**

Stop listening for incoming datagrams.

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

Returns:0 on success, or an error code < 0 on failure.

```cpp
int uv_udp_getsockname(const uv_udp_t* handle,struct sockaddr* name,int* namelen);
int uv_udp_set_membership(uv_udp_t* handle,const char* multicast_addr,const char* interface_addr,uv_membership membership);
int uv_udp_set_multicast_loop(uv_udp_t* handle, int on);
int uv_udp_set_multicast_ttl(uv_udp_t* handle, int ttl);
int uv_udp_set_multicast_interface(uv_udp_t* handle, const char* interface_addr);
int uv_udp_set_broadcast(uv_udp_t* handle, int on);
int uv_udp_set_ttl(uv_udp_t* handle, int ttl);
int uv_ip4_addr(const char* ip, int port, struct sockaddr_in* addr);
int uv_ip6_addr(const char* ip, int port, struct sockaddr_in6* addr);
int uv_ip4_name(const struct sockaddr_in* src, char* dst, size_t size);
int uv_ip6_name(const struct sockaddr_in6* src, char* dst, size_t size);
int uv_inet_ntop(int af, const void* src, char* dst, size_t size);
int uv_inet_pton(int af, const char* src, void* dst);
```

**uv_udp_getsockname****：**

Get the local IP and port of the UDP handle.

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html) and bound.

**name** – Pointer to the structure to be filled with the address data. In order to support IPv4 and IPv6 *struct* *sockaddr_storage* should be used.

**namelen** – On input it indicates the data of the *name* field. On output it indicates how much of it was filled.

Returns:0 on success, or an error code < 0 on failure.

**uv_udp_set_membership****：**

Set membership for a multicast address

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

**multicast_addr** – Multicast address to set membership for.

**interface_addr** – Interface address.

**membership** – Should be UV_JOIN_GROUP or UV_LEAVE_GROUP.

Returns:0 on success, or an error code < 0 on failure.

**uv_udp_set_multicast_loop****：**

Set IP multicast loop flag. Makes multicast packets loop back to local sockets.

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

本机是否接收组播包

**on** – 1 for on, 0 for off.

Returns:0 on success, or an error code < 0 on failure.

 **uv_udp_set_multicast_ttl****：**

Set the multicast ttl（ Time To Live）

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

**ttl** – 1 through 255.

Returns:0 on success, or an error code < 0 on failure.

**uv_udp_set_multicast_interface****：**

Set the multicast interface to send or receive data on.

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

**interface_addr** – interface address.

Returns:0 on success, or an error code < 0 on failure.

**uv_udp_set_broadcast****：**

Set broadcast on or off.

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

**on** – 1 for on, 0 for off.

Returns:0 on success, or an error code < 0 on failure.

**uv_udp_set_ttl****:**

Set the time to live. （ Time To Live）

Parameters:**handle** – UDP handle. Should have been initialized with [uv_udp_init](http://docs.libuv.org/en/v1.x/udp.html)[()](http://docs.libuv.org/en/v1.x/udp.html).

**ttl** – 1 through 255.

Returns:0 on success, or an error code < 0 on failure.

**uv_ip4_addr:**

Convert a string containing an IPv4 addresses to a binary structure.

ip到addr

**uv_ip6_addr****:**

Convert a string containing an IPv6 addresses to a binary structure.

**uv_ip4_name****:**

Convert a binary structure containing an IPv4 address to a string.

addr到ip

**uv_ip6_name****:**

Convert a binary structure containing an IPv6 address to a string.

**uv_inet_ntop****:***//**将点分十进制的**ip**地址转化为用于网络传输的数值格式*

**uv_inet_pton****:***//**将数值格式转化为点分十进制的**ip**地址格式*

Cross-platform IPv6-capable implementation of [inet_ntop](http://linux.die.net/man/3/inet_ntop)[(3)](http://linux.die.net/man/3/inet_ntop) and [inet_pton](http://linux.die.net/man/3/inet_pton)[(3)](http://linux.die.net/man/3/inet_pton). On success they return 0. In case of error the target *dst* pointer is unmodified





# 第五节:线程相关





------

Threads are used internally to fake the asynchronous nature of all of the system calls. libuv also uses threads to allow you, the application, to perform a task asynchronously that is actually blocking, by spawning a thread and collecting the result when it is done.

Today there are two predominant thread libraries: the Windows threads implementation and POSIX’s [pthreads](http://man7.org/linux/man-pages/man7/pthreads.7.html). libuv’s thread API is analogous to the pthreads API and often has similar semantics.（目前主要有二类线程，一种是window的POSIX，一种是linux的pthread，libuv封装的thread与pthread更相近）

A notable aspect of libuv’s thread facilities is that it is a self contained section within libuv. Whereas other features intimately depend on the event loop and callback principles, threads are complete agnostic, they block as required, signal errors directly via return values, and, as shown in the [first example](http://docs.libuv.org/en/v1.x/guide/threads.html#thread-create-example), don’t even require a running event loop.（线程在libuv中有被使用，很多情况和loop关联，当然线程也可以单独使用）

libuv’s thread API is also very limited since the semantics and syntax of threads are different on all platforms, with different levels of completeness.（因为不同线程的实现，封装的功能仅包含部分线程功能）

This chapter makes the following assumption: **There is only one event loop, running in one thread (the main thread)**. No other thread interacts with the event loop (except using `uv_async_send`).（libuv中使用loop的话，只允许使用一个loop）

```c
typedef void (*uv_thread_cb)(void* arg);
 
int uv_thread_create(uv_thread_t* tid, uv_thread_cb entry, void* arg);
uv_thread_t uv_thread_self(void);
int uv_thread_join(uv_thread_t *tid);
int uv_thread_equal(const uv_thread_t* t1, const uv_thread_t* t2);
int uv_key_create(uv_key_t* key);
void uv_key_delete(uv_key_t* key);
void* uv_key_get(uv_key_t* key);
void uv_key_set(uv_key_t* key, void* value);
```

**uv_thread_t：**Thread data type

void (*uv_thread_cb)(void* *arg*)：Callback that is invoked to initialize thread execution. *arg* is the same value that was passed to uv_thread_create().

**uv_thread_create****：**第一个参数是线程ID，为输出参数。

**uv_thread_t uv_thread_self****：**返回本线程ID，在线程内部调用。

**uv_thread_join****：**等待线程结束。

**uv_thread_equal****：**判断二个线程是否是同一个。如果相同，返回非0，不相同返回0

**uv_key_t：**Thread-local key data type.

**uv_key_create:**创建局部变量,第一个参数是返回值key

**uv_key_delete:**删除局部变量

**uv_key_get:**获取关联指针

**uv_key_set:**设置关联指针

```cpp
int uv_mutex_init(uv_mutex_t* handle);
int uv_mutex_init_recursive(uv_mutex_t* handle);
void uv_mutex_destroy(uv_mutex_t* handle);
void uv_mutex_lock(uv_mutex_t* handle);
int uv_mutex_trylock(uv_mutex_t* handle);
void uv_mutex_unlock(uv_mutex_t* handle);
int uv_rwlock_init(uv_rwlock_t* rwlock);
void uv_rwlock_destroy(uv_rwlock_t* rwlock);
void uv_rwlock_rdlock(uv_rwlock_t* rwlock);
int uv_rwlock_tryrdlock(uv_rwlock_t* rwlock);
void uv_rwlock_rdunlock(uv_rwlock_t* rwlock);
void uv_rwlock_wrlock(uv_rwlock_t* rwlock);
int uv_rwlock_trywrlock(uv_rwlock_t* rwlock);
void uv_rwlock_wrunlock(uv_rwlock_t* rwlock);
```

互斥锁

1，uv_mutex_init：初始化锁，handle是返回值

2，uv_mutex_init_recursive：初始化线程循环锁

3，uv_mutex_destroy：销毁锁

4，uv_mutex_lock：上锁

5，uv_mutex_trylock:尝试上锁

6，uv_mutex_unlock：解锁

**读写锁加锁操作**

假设当前临界区没有任何进程，这时候read进程或者write进程都可以进来，但是只能是其一

如果当前临界区只有一个read进程，这时候任意的read进程都可以进入，但是write进程不能进入

如果当前临界区只有一个write进程，这时候任何read/write进程都无法进入。只能自旋等待

如果当前当前临界区有好多个read进程，同时read进程依然还会进入，这时候进入的write进程只能等待。直到临界区一个read进程都没有，才可进入

**读写锁解锁操作**

如果在read进程离开临界区的时候，需要根据情况决定write进程是否需要进入。只有当临界区没有read进程了，write进程方可进入。

如果在write进程离开临界区的时候，无论write进程或者read进程都可进入临界区，因为write进程是排它的。

1，uv_rwlock_init:初始化读写锁

2，uv_rwlock_destroy：销毁读写锁

3，uv_rwlock_rdlock：读上锁

4，uv_rwlock_tryrdlock：读尝试上锁

5，rwlock_rdunlock：读解锁

6，uv_rwlock_wrlock：写上锁

7，uv_rwlock_trywrlock：写尝试上锁

8，uv_rwlock_wrunlock：写解锁

```cpp
int uv_sem_init(uv_sem_t* sem, unsigned int value);
void uv_sem_destroy(uv_sem_t* sem);
void uv_sem_post(uv_sem_t* sem);
void uv_sem_wait(uv_sem_t* sem);
int uv_sem_trywait(uv_sem_t* sem);
```

信号量允许多个线程同时进入临界区。

https://blog.csdn.net/qyz_og/article/details/47189219

1，uv_sem_init：初始化信号量，value：信号量值

2，uv_sem_destroy：销户

3，uv_sem_post：释放使value++

4，uv_sem_wait：等待获取，如果value>0,直接返回，并且value--。否则等待，知道value>0

5，uv_sem_trywait:尝试获取,if value>0,return 0;else return -1;

```cpp
int uv_barrier_init(uv_barrier_t* barrier, unsigned int count);
void uv_barrier_destroy(uv_barrier_t* barrier);
int uv_barrier_wait(uv_barrier_t* barrier);
```

https://blog.csdn.net/qq405180763/article/details/23919191

把先后到达的多个线程挡在同一栏杆前，直到所有线程到齐，然后撤下栏杆同时放行。

1，uv_barrier_init：初始化屏障，count等待线程个数

2，uv_barrier_destroy：销毁

3，uv_barrier_wait：等待，线程内部调用。

```cpp
int uv_cond_init(uv_cond_t* cond);
void uv_cond_destroy(uv_cond_t* cond);
void uv_cond_signal(uv_cond_t* cond);
void uv_cond_broadcast(uv_cond_t* cond);
void uv_cond_wait(uv_cond_t* cond, uv_mutex_t* mutex);
int uv_cond_timedwait(uv_cond_t* cond, uv_mutex_t* mutex,uint64_t timeout);
```

https://www.cnblogs.com/jiu0821/p/6424951.html

1，uv_cond_init：初始化条件变量，与pthread_cond_init的区别是缺少属性设置attr。

2，uv_cond_destroy：销毁条件变量

3，uv_cond_signal：释放被阻塞在指定条件变量上的一个线程。

4，uv_cond_broadcast：释放阻塞的所有线程

5，uv_cond_wait：函数将解锁mutex参数指向的互斥锁，并使当前线程阻塞在cond参数指向的条件变量上。

被阻塞的线程可以被uv_cond_signal函数，uv_cond_broadcast函数唤醒，也可能在被信号中断后被唤醒。

uv_cond_wait函数的返回并不意味着条件的值一定发生了变化，必须重新检查条件的值。

uv_cond_wait函数返回时，相应的互斥锁将被当前线程锁定，即使是函数出错返回。

一般一个条件表达式都是在一个互斥锁的保护下被检查。当条件表达式未被满足时，线程将仍然阻塞在这个条件变量上。当另一个线程改变了条件的值并向条件变量发出信号时，等待在这个条件变量上的一个线程或所有线程被唤醒，接着都试图再次占有相应的互斥锁。

阻塞在条件变量上的线程被唤醒以后，直到uv_cond_wait()函数返回之前条件的值都有可能发生变化。所以函数返回以后，在锁定相应的互斥锁之前，必须重新测试条件值。最好的测试方法是循环调用uv_cond_wait函数，并把满足条件的表达式置为循环的终止条件。如：

```c
uv_mutex_lock();
while (condition_is_false)
        uv_cond_wait();
uv_mutex_unlock();
```

阻塞在同一个条件变量上的不同线程被释放的次序是不一定的。

注意：uv_cond_wait()函数是退出点，如果在调用这个函数时，已有一个挂起的退出请求，且线程允许退出，这个线程将被终止并开始执行善后处理函数，而这时和条件变量相关的互斥锁仍将处在锁定状态。

6，uv_cond_timedwait：在uv_cond_wait的基础上增加超时功能。

7.唤醒丢失问题

在线程未获得相应的互斥锁时调用pthread_cond_signal或pthread_cond_broadcast函数可能会引起唤醒丢失问题。

唤醒丢失往往会在下面的情况下发生：

一个线程调用pthread_cond_signal或pthread_cond_broadcast函数；

另一个线程正处在测试条件变量和调用pthread_cond_wait函数之间；

没有线程正在处在阻塞等待的状态下。

```cpp
void uv_once(uv_once_t* guard, void (*callback)(void));
```

Multiple threads can attempt to call uv_once() with a given guard and a function pointer, **only the first one will win, the function will be called once and only once**

```cpp
/* Initialize guard */
static uv_once_t once_only = UV_ONCE_INIT;
int i = 0;
void increment() { i++;}
void thread1() {
    /* ... work */
    uv_once(once_only, increment);
}

void thread2() {
    /* ... work */
    uv_once(once_only, increment);
}

int main() {
    /* ... spawn threads */
}
```

After all threads are done, i == 1.

 

## 线程池调用

```cpp
int uv_queue_work(uv_loop_t* loop,
                            uv_work_t* req,
                            uv_work_cb work_cb,
                            uv_after_work_cb after_work_cb);
 
int uv_cancel(uv_req_t* req);
```

 

<http://docs.libuv.org/en/v1.x/guide/threads.html>

libuv provides a threadpool which can be used to run user code and get notified in the loop thread. This thread pool is internally used to run all file system operations, as well as getaddrinfo and getnameinfo requests.

Its default size is 4, but it can be changed at startup time by setting the UV_THREADPOOL_SIZE environment variable to any value (the absolute maximum is 128).

The threadpool is global and shared across all event loops. When a particular function makes use of the threadpool (i.e. when using [uv_queue_work](http://docs.libuv.org/en/v1.x/threadpool.html?highlight=uv_queue_work)[()](http://docs.libuv.org/en/v1.x/threadpool.html?highlight=uv_queue_work)) libuv preallocates and initializes the maximum number of threads allowed by UV_THREADPOOL_SIZE. This causes a relatively minor memory overhead (~1MB for 128 threads) but increases the performance of threading at runtime.

libuv提供了一个线程池，可用于运行用户代码并在循环线程中得到通知。此线程池在内部用于运行所有文件系统操作，以及getaddrinfo和getnameinfo请求。

其默认大小为4，但可以通过将UV_THREADPOOL_SIZE环境变量设置为任何值（绝对最大值为128，*putenv*("UV_THREADPOOL_SIZE=128")）在启动时更改 。

线程池是全局的，并在所有事件循环中共享。当特定函数使用线程池时（即使用时），libuv会预分配并初始化允许的最大线程数。这会导致相对较小的内存开销（128个线程约为1MB），但会增加运行时的线程性能。

**uv_queue_work****：**

Initializes a work request which will run the given *work_cb* in a thread from the threadpool. Once *work_cb* is completed, *after_work_cb* will be called on the loop thread.

This request can be cancelled with [uv_cancel](http://docs.libuv.org/en/v1.x/request.html)[()](http://docs.libuv.org/en/v1.x/request.html).

> Note that even though a global thread pool which is shared across all events loops is used, the functions are not thread safe.

从线程池中获取一个线程，执行work_cb任务。执行完成后，通过after_work_cb通知。work_cb不可以为空，否则after_work_cb不会执行。

Loop主要用来调用after_work_cb回调结果，如果想要after_work_cb返回，loop必须调用uv_run.

从线程池获取线程，然后执行对应的任务，无需自己创建线程。

**uv_cancel:**取请求执行

```cpp
/*
uv_work_t:Work request type.
void (*uv_work_cb)(uv_work_t* req)
void (*uv_after_work_cb)(uv_work_t* req, int status)
*/
int uv_queue_work(uv_loop_t* loop, uv_work_t* req, uv_work_cb work_cb, uv_after_work_cb after_work_cb)
```

注意事项：

创建子进程用spawn，不要fork，fork的子进程中调用uv_queue_work，不执行

 





# 第六节:进程相关





------

libuv offers considerable child process management, abstracting the platform differences and allowing communication with the child process using streams or named pipes.（libuv提供子进程的管理工作，抽象平台的差异性，允许通过stream和pipe进行线程之间的交互）

A common idiom in Unix is for every process to do one thing and do it well. In such a case, a process often uses multiple child processes to achieve tasks (similar to using pipes in shells). A multi-process model with messages may also be easier to reason about compared to one with threads and shared memory.（unix建议每个进程只做一件事并且做好。所以一个进程常常使用多个子进程来完成各种任务，类似于shell中使用多个pipe。与线程共享内存类似，含有消息的多进程模型也很简单。）

A common refrain against event-based programs is that they cannot take advantage of multiple cores in modern computers. In a multi-threaded program the kernel can perform scheduling and assign different threads to different cores, improving performance. But an event loop has only one thread. The workaround can be to launch multiple processes instead, with each process running an event loop, and each process getting assigned to a separate CPU core.（基于事件的程序想利用现代计算机的多核功能是有一定挑战性的。在多线程程序里，内核可以通过调度把不同的线程分配到不同的系统核core上来提供core的利用率。但是libuv的loop只有一个线程，通过变通的使用多进程，每个进程上运行一个loop，每个进程关联到不同的core上以达到利用多核系统。）

## 创建进程：

The simplest case is when you simply want to launch a process and know when it exits. This is achieved using `uv_spawn`.

使用uv_spawn来创建一个进程。代码如下：

```cpp
UV_EXTERN int uv_spawn(uv_loop_t* loop,uv_process_t* handle,const uv_process_options_t* options);

uv_loop_t *loop;
uv_process_t child_req;
uv_process_options_t options;
void on_exit(uv_process_t *req, int64_t exit_status, int term_signal) {
    fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
    uv_close((uv_handle_t*) req, NULL);
}
int main() {
    loop = uv_default_loop();
 
    char* args[3];
    args[0] = "mkdir";
    args[1] = "test-dir";
    args[2] = NULL;
 
    options.exit_cb = on_exit;
    options.file = "mkdir";
    options.args = args;
 
    int r;
    if ((r = uv_spawn(loop, &child_req, &options))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return 1;
    } else {
        fprintf(stderr, "Launched process with ID %d\n", child_req.pid);
    }
 
    return uv_run(loop, UV_RUN_DEFAULT);
}
```

options必须被初始化位0，上边代码因为options是全局的，所以默认会被置为0.

args的数组最后一个参数必须是NULL。

完成调用之后，`uv_process_t.pid`包含的就是进程ID.

on_exit()在进程退出时，会触发。

## 改变参数：

在进程运行前，可以改变参数uv_process_options_t。

Set `uv_process_options_t.cwd` to the corresponding directory.设置运行目录。

`uv_process_options_t.env` is a null-terminated array of strings, each of the form `VAR=VALUE` used to set up the environment variables for the process. Set this to `NULL` to inherit the environment from the parent (this) process.运行环境。

按位或flags参数。

Setting `uv_process_options_t.flags` to a bitwise OR of the following flags, modifies the child process behaviour:

- `UV_PROCESS_SETUID` - sets the child’s execution user ID to `uv_process_options_t.uid`.
- `UV_PROCESS_SETGID` - sets the child’s execution group ID to `uv_process_options_t.gid`.

Changing the UID/GID is only supported on Unix, `uv_spawn` will fail on Windows with `UV_ENOTSUP`.

- `UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS` - No quoting or escaping of `uv_process_options_t.args` is done on Windows. Ignored on Unix.
- `UV_PROCESS_DETACHED` - Starts the child process in a new session, which will keep running after the parent process exits. See example below.

## 分离过程

可以创建和父进程无关的子进程，使用UV_PROCESS_DETACHED标记。不受父进程的影响，父进程关闭不影响子进程的运行。可以用来写守候等不受父进程影响的子进程。举例如下：

```cpp
int main() {
    loop = uv_default_loop();
 
    char* args[3];
    args[0] = "sleep";
    args[1] = "100";
    args[2] = NULL;
 
    options.exit_cb = NULL;
    options.file = "sleep";
    options.args = args;
    options.flags = UV_PROCESS_DETACHED;
 
    int r;
    if ((r = uv_spawn(loop, &child_req, &options))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return 1;
    }
    fprintf(stderr, "Launched sleep with PID %d\n", child_req.pid);
    uv_unref((uv_handle_t*) &child_req);
 
    return uv_run(loop, UV_RUN_DEFAULT);
```

虽然使用了UV_PROCESS_DETACHED，但是句柄还在父进程那里，如果想完全失去读子进程的控制，需要调用uv_unref接口，完全释放对子进程的控制。

## 向进程发送信号

在进程执行的过程中，可以通过发送信号来控制进程的执行。当然 `SIGTERM`, `SIGINT` and `SIGKILL也会导致线程终止，具体参考`<https://blog.csdn.net/qq562029186/article/details/70132719>

```cpp
UV_EXTERN int uv_process_kill(uv_process_t*, int signum);
UV_EXTERN int uv_kill(int pid, int signum);
```

如果调用uv_process_kill关闭进程的话，别忘记调用uv_close函数。

## 信号

libuv针对Unix提供了一些信号，有些也适用于Windows.

使用`uv_signal_init()`初始化，它与一个循环关联。要侦听该处理程序上的特定信号，使用`uv_signal_start()`处理程序函数。每个处理程序只能与一个信号编号相关联，连续调用 `uv_signal_start()后边的`覆盖先前信号。使用`uv_signal_stop()停止信号监听。`下边提供一个小例子供参考：

```cpp
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uv.h>
 
uv_loop_t* create_loop()
{
    uv_loop_t *loop = malloc(sizeof(uv_loop_t));
    if (loop) {
      uv_loop_init(loop);
    }
    return loop;
}
 
void signal_handler(uv_signal_t *handle, int signum)
{
    printf("Signal received: %d\n", signum);
    uv_signal_stop(handle);
}
 
// two signal handlers in one loop
void thread1_worker(void *userp)
{
    uv_loop_t *loop1 = create_loop();
 
    uv_signal_t sig1a, sig1b;
    uv_signal_init(loop1, &sig1a);
    uv_signal_start(&sig1a, signal_handler, SIGUSR1);
 
    uv_signal_init(loop1, &sig1b);
    uv_signal_start(&sig1b, signal_handler, SIGUSR1);
 
    uv_run(loop1, UV_RUN_DEFAULT);
}
 
// two signal handlers, each in its own loop
void thread2_worker(void *userp)
{
    uv_loop_t *loop2 = create_loop();
    uv_loop_t *loop3 = create_loop();
 
    uv_signal_t sig2;
    uv_signal_init(loop2, &sig2);
    uv_signal_start(&sig2, signal_handler, SIGUSR1);
 
    uv_signal_t sig3;
    uv_signal_init(loop3, &sig3);
    uv_signal_start(&sig3, signal_handler, SIGUSR1);
 
    while (uv_run(loop2, UV_RUN_NOWAIT) || uv_run(loop3, UV_RUN_NOWAIT)) {
    }
}
 
int main()
{
    printf("PID %d\n", getpid());
 
    uv_thread_t thread1, thread2;
 
    uv_thread_create(&thread1, thread1_worker, 0);
    uv_thread_create(&thread2, thread2_worker, 0);
 
    uv_thread_join(&thread1);
    uv_thread_join(&thread2);
    return 0;
}
```

上述监听回调会触发4次监听。

## 子进程I/O

子进程有其自身的一组文件描述符，用0，1和2的分别表示`stdin`，`stdout`和`stderr`。有时我们可能希望子进程与父进程共享文件描述符。libuv支持 *继承*文件描述符。

```cpp
int main() {
    loop = uv_default_loop();
 
    /* ... */
 
    options.stdio_count = 3;
    uv_stdio_container_t child_stdio[3];
    child_stdio[0].flags = UV_IGNORE;
    child_stdio[1].flags = UV_IGNORE;
    child_stdio[2].flags = UV_INHERIT_FD;
    child_stdio[2].data.fd = 2;
    options.stdio = child_stdio;
 
    options.exit_cb = on_exit;
    options.file = args[0];
    options.args = args;
 
    int r;
    if ((r = uv_spawn(loop, &child_req, &options))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
        return 1;
    }
 
    return uv_run(loop, UV_RUN_DEFAULT);
}
```

UV_IGNORE被重定向到/dev/null

UV_INHERIT_FD继承父进程的I/O,需要在data.fd中，设置父进程的描述符。

同理，子进程可以继承父进程的Stream，举例如下：

```cpp
 
    args[1] = NULL;
 
    /* ... finding the executable path and setting up arguments ... */
 
    options.stdio_count = 3;
    uv_stdio_container_t child_stdio[3];
    child_stdio[0].flags = UV_IGNORE;
    child_stdio[1].flags = UV_INHERIT_STREAM;
    child_stdio[1].data.stream = (uv_stream_t*) client;
    child_stdio[2].flags = UV_IGNORE;
    options.stdio = child_stdio;
 
    options.exit_cb = cleanup_handles;
    options.file = args[0];
    options.args = args;
 
    // Set this so we can close the socket after the child process exits.
    child_req.data = (void*) client;
    int r;
    if ((r = uv_spawn(loop, &child_req, &options))) {
        fprintf(stderr, "%s\n", uv_strerror(r));
```

## 管道

管道不像linux下的符号“|”或者pipe，管道是用于两个进程之间进行通信的方法。具体如下：

### 父子管道：

父进程在创建子进程的时候uv_spawn，可以通过设置 `uv_stdio_container_t.flags` 参数，来设置父子进程之间的单向或双向通信， `uv_stdio_container_t.flags可以是``UV_CREATE_PIPE` 、 `UV_READABLE_PIPE` or `UV_WRITABLE_PIPE的一个或多个。可以同时设置多个，占不同的二进制位。UV_CREATE_PIPE` 、 `UV_READABLE_PIPE` or `UV_WRITABLE_PIPE都是从子进程的角度来看。`

### Arbitrary process IPC

Since domain sockets [[1\]](http://docs.libuv.org/en/v1.x/guide/processes.html#id3) can have a well known name and a location in the file-system they can be used for IPC between unrelated processes. The [D-BUS](http://www.freedesktop.org/wiki/Software/dbus) system used by open source desktop environments uses domain sockets for event notification. Various applications can then react when a contact comes online or new hardware is detected. The MySQL server also runs a domain socket on which clients can interact with it.

When using domain sockets, a client-server pattern is usually followed with the creator/owner of the socket acting as the server. After the initial setup, messaging is no different from TCP, so we’ll re-use the echo server example.

pipe-echo-server/main.c

```cpp
void remove_sock(int sig) {
    uv_fs_t req;
    uv_fs_unlink(loop, &req, PIPENAME, NULL);
    exit(0);
}
 
int main() {
    loop = uv_default_loop();
 
    uv_pipe_t server;
    uv_pipe_init(loop, &server, 0);
 
    signal(SIGINT, remove_sock);
 
    int r;
    if ((r = uv_pipe_bind(&server, PIPENAME))) {
        fprintf(stderr, "Bind error %s\n", uv_err_name(r));
        return 1;
    }
    if ((r = uv_listen((uv_stream_t*) &server, 128, on_new_connection))) {
        fprintf(stderr, "Listen error %s\n", uv_err_name(r));
        return 2;
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}
```

We name the socket `echo.sock` which means it will be created in the local directory. This socket now behaves no different from TCP sockets as far as the stream API is concerned. You can test this server using [socat](http://www.dest-unreach.org/socat/):

```cpp
$ socat - /path/to/socket
```

A client which wants to connect to a domain socket will use:

```cpp
void uv_pipe_connect(uv_connect_t *req, uv_pipe_t *handle, const char *name, uv_connect_cb cb);
```

where `name` will be `echo.sock` or similar. On Unix systems, `name` must point to a valid file (e.g. `/tmp/echo.sock`). On Windows, `name` follows a `\\?\pipe\echo.sock` format.

## 进程相关API

```cpp
/* uv_spawn() options. */
typedef enum {
  UV_IGNORE         = 0x00,
  UV_CREATE_PIPE    = 0x01,
  UV_INHERIT_FD     = 0x02,
  UV_INHERIT_STREAM = 0x04,
 
  /*
   * When UV_CREATE_PIPE is specified, UV_READABLE_PIPE and UV_WRITABLE_PIPE
   * determine the direction of flow, from the child process' perspective. Both
   * flags may be specified to create a duplex data stream.
   */
  UV_READABLE_PIPE  = 0x10,
  UV_WRITABLE_PIPE  = 0x20,
 
  /*
   * Open the child pipe handle in overlapped mode on Windows.
   * On Unix it is silently ignored.
   */
  UV_OVERLAPPED_PIPE = 0x40
} uv_stdio_flags;
 
typedef struct uv_stdio_container_s {
  uv_stdio_flags flags;
 
  union {
    uv_stream_t* stream;
    int fd;
  } data;
} uv_stdio_container_t;
 
typedef struct uv_process_options_s {
  uv_exit_cb exit_cb; /* Called after the process exits. */
  const char* file;   /* Path to program to execute. */
  /*
   * Command line arguments. args[0] should be the path to the program. On
   * Windows this uses CreateProcess which concatenates the arguments into a
   * string this can cause some strange errors. See the note at
   * windows_verbatim_arguments.
   */
  char** args;
  /*
   * This will be set as the environ variable in the subprocess. If this is
   * NULL then the parents environ will be used.
   */
  char** env;
  /*
   * If non-null this represents a directory the subprocess should execute
   * in. Stands for current working directory.
   */
  const char* cwd;
  /*
   * Various flags that control how uv_spawn() behaves. See the definition of
   * `enum uv_process_flags` below.
   */
  unsigned int flags;
  /*
   * The `stdio` field points to an array of uv_stdio_container_t structs that
   * describe the file descriptors that will be made available to the child
   * process. The convention is that stdio[0] points to stdin, fd 1 is used for
   * stdout, and fd 2 is stderr.
   *
   * Note that on windows file descriptors greater than 2 are available to the
   * child process only if the child processes uses the MSVCRT runtime.
   */
  int stdio_count;
  uv_stdio_container_t* stdio;
  /*
   * Libuv can change the child process' user/group id. This happens only when
   * the appropriate bits are set in the flags fields. This is not supported on
   * windows; uv_spawn() will fail and set the error to UV_ENOTSUP.
   */
  uv_uid_t uid;
  uv_gid_t gid;
} uv_process_options_t;
 
/*
 * These are the flags that can be used for the uv_process_options.flags field.
 */
enum uv_process_flags {
  /*
   * Set the child process' user id. The user id is supplied in the `uid` field
   * of the options struct. This does not work on windows; setting this flag
   * will cause uv_spawn() to fail.
   */
  UV_PROCESS_SETUID = (1 << 0),
  /*
   * Set the child process' group id. The user id is supplied in the `gid`
   * field of the options struct. This does not work on windows; setting this
   * flag will cause uv_spawn() to fail.
   */
  UV_PROCESS_SETGID = (1 << 1),
  /*
   * Do not wrap any arguments in quotes, or perform any other escaping, when
   * converting the argument list into a command line string. This option is
   * only meaningful on Windows systems. On Unix it is silently ignored.
   */
  UV_PROCESS_WINDOWS_VERBATIM_ARGUMENTS = (1 << 2),
  /*
   * Spawn the child process in a detached state - this will make it a process
   * group leader, and will effectively enable the child to keep running after
   * the parent exits.  Note that the child process will still keep the
   * parent's event loop alive unless the parent process calls uv_unref() on
   * the child's process handle.
   */
  UV_PROCESS_DETACHED = (1 << 3),
  /*
   * Hide the subprocess console window that would normally be created. This
   * option is only meaningful on Windows systems. On Unix it is silently
   * ignored.
   */
  UV_PROCESS_WINDOWS_HIDE = (1 << 4)
};
 
/*
 * uv_process_t is a subclass of uv_handle_t.
 */
struct uv_process_s {
  UV_HANDLE_FIELDS
  uv_exit_cb exit_cb;
  int pid;
  UV_PROCESS_PRIVATE_FIELDS
};
 
UV_EXTERN int uv_spawn(uv_loop_t* loop,
                       uv_process_t* handle,
                       const uv_process_options_t* options);
UV_EXTERN int uv_process_kill(uv_process_t*, int signum);
UV_EXTERN int uv_kill(int pid, int signum);
UV_EXTERN uv_pid_t uv_process_get_pid(const uv_process_t*);
 
```

**uv_spawn**：创建子进程。
**uv_process_kill**：杀掉子进程，通过进程结构体。
**uv_kill**：杀掉子进程，通过进程ID。
**uv_process_get_pid**：获取进程号。

参考文章：

<http://docs.libuv.org/en/v1.x/process.html>





# 第七节:高级调度





libuv的loop为提供了相当大的用户控制权，您可以通过处理多个循环来实现有趣的结果。您还可以将libuv的事件循环嵌入到另一个基于事件循环的库中 - 想象一下基于Qt的UI，以及Qt的事件循环驱动一个libuv后端，让它执行密集的系统级任务。

libuv提供一个uv_stop函数，用于停止loop的执行，当然uv_stop不会让loop立刻停止，而是通过设置stop_flag来控制loop的执行，具体可以参考<https://blog.csdn.net/knowledgebao/article/details/82251513>。

下边做一个简单说明：

```cpp
int uv_is_closing(const uv_handle_t* handle) {
  return uv__is_closing(handle);
}
 
 
int uv_backend_fd(const uv_loop_t* loop) {
  return loop->backend_fd;
}
 
 
int uv_backend_timeout(const uv_loop_t* loop) {
  if (loop->stop_flag != 0)
    return 0;
 
  if (!uv__has_active_handles(loop) && !uv__has_active_reqs(loop))
    return 0;
}
```

下边给出一个uv_stop的例子，可以看看是否和预期一致。

```cpp
#include <stdio.h>
#include <uv.h>
 
int64_t counter = 0;
 
void idle_cb(uv_idle_t *handle) {
    printf("Idle callback\n");
    counter++;
 
    if (counter >= 5) {
        uv_stop(uv_default_loop());
        printf("uv_stop() called\n");
    }
}
 
void prep_cb(uv_prepare_t *handle) {
    printf("Prep callback\n");
}
 
int main() {
    uv_idle_t idler;
    uv_prepare_t prep;
 
    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, idle_cb);
 
    uv_prepare_init(uv_default_loop(), &prep);
    uv_prepare_start(&prep, prep_cb);
 
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
 
    return 0;
}
```

 





# 第八节:其他





<http://docs.libuv.org/en/v1.x/timer.html>

<http://docs.libuv.org/en/v1.x/errors.html>

<http://docs.libuv.org/en/v1.x/misc.html>

这一章包含一些通用的工具类接口以及一些不适合单独列出来作为一章的东东。

## DNS：

```cpp
int uv_getaddrinfo(uv_loop_t* loop,
                             uv_getaddrinfo_t* req,
                             uv_getaddrinfo_cb getaddrinfo_cb,
                             const char* node,
                             const char* service,
                             const struct addrinfo* hints);
void uv_freeaddrinfo(struct addrinfo* ai);
typedef void (*uv_getaddrinfo_cb)(uv_getaddrinfo_t* req,
                                  int status,
                                  struct addrinfo* res);
 
int uv_getnameinfo(uv_loop_t* loop,
                             uv_getnameinfo_t* req,
                             uv_getnameinfo_cb getnameinfo_cb,
                             const struct sockaddr* addr,
                             int flags);
typedef void (*uv_getnameinfo_cb)(uv_getnameinfo_t* req,
                                  int status,
                                  const char* hostname,
                                  const char* service);
 
```

<https://linux.die.net/man/3/getaddrinfo>

**uv_getaddrinfo_t**：地址结构体,具体详见头文件

**uv_getaddrinfo**：获取地址

Either node or service may be NULL but not both.

Returns 0 on success or an error code < 0 on failure. If successful, the callback will get called sometime in the future with the lookup result, which is either:

status == 0, the res argument points to a valid *struct* *addrinfo*, or

status < 0, the res argument is NULL. See the UV_EAI_* constants.

Call [uv_freeaddrinfo](http://docs.libuv.org/en/v1.x/dns.html?highlight=uv_getaddrinfo)[()](http://docs.libuv.org/en/v1.x/dns.html?highlight=uv_getaddrinfo) to free the addrinfo structure.

*Changed in version 1.3.0:* the callback parameter is now allowed to be NULL, in which case the request will run **synchronously**.

**uv_freeaddrinfo**：释放地址

**uv_getaddrinfo_cb:**  回调，Callback which will be called with the getaddrinfo request result once complete. In case it was cancelled, status will have a value of UV_ECANCELED.

[http://docs.libuv.org/en/v1.x/dns.html](https://linux.die.net/man/3/getaddrinfo)

[https://baike.baidu.com/item/getaddrinfo/9021771](https://linux.die.net/man/3/getaddrinfo)

**uv_getnameinfo****：**获取名称

## 定时器：

```cpp
int uv_timer_init(uv_loop_t*, uv_timer_t* handle);
int uv_timer_start(uv_timer_t* handle,
                             uv_timer_cb cb,
                             uint64_t timeout,
                             uint64_t repeat);
int uv_timer_stop(uv_timer_t* handle);
int uv_timer_again(uv_timer_t* handle);
void uv_timer_set_repeat(uv_timer_t* handle, uint64_t repeat);
uint64_t uv_timer_get_repeat(const uv_timer_t* handle);
```

**uv_timer_init**

Initialize the handle.

初始化定时器，最后需要调用uv_close((uv_handle_t*)&timer_handle, *NULL*);释放只句柄；

**uv_timer_start**

开始计时器，timeout:首次触发时间，repeat：重复间隔时长(milliseconds)

如果连续调用二次start，第二次覆盖第一次。

如果两个定时间触发时间点相同，按照start执行的先后顺序触发。

cb不可以为空，否则失败

**uv_timer_stop：**停止计时器，可以使用uv_close(uv_handle_t*, NULL)代替。

**uv_timer_again****：**

Stop the timer, and if it is repeating restart it using the repeat value as the timeout.

If the timer has never been started before it returns UV_EINVAL.

重新开始，受限停止timer，如果当前timer的repeat不为0，timeout使用repeat代替，如果timer还没有uv_run,则执行失败，返回UV_EINVAL

**uv_timer_set_repeat****：**

Set the repeat interval value in milliseconds. The timer will be scheduled to run on the given interval, regardless of the callback execution duration, and will follow normal timer semantics in the case of a time-slice overrun.

For example, if a 50ms repeating timer first runs for 17ms, it will be scheduled to run again 33ms later. If other tasks consume more than the 33ms following the first timer callback, then the callback will run as soon as possible.

Note:

If the repeat value is set from a timer callback it does not immediately take effect. If the timer was non-repeating before, it will have been stopped. If it was repeating, then the old repeat value will have been used to schedule the next timeout.

设置重复， 单位：milliseconds

**uv_timer_get_repeat****：**

Get the timer repeat value.

获取重复

**uv_timer_cb****:**

回调函数，定时时间到，触发

## 系统错误：

```c
int uv_translate_sys_error(int sys_errno);
 
const char* uv_strerror(int err);
char* uv_strerror_r(int err, char* buf, size_t buflen);
 
const char* uv_err_name(int err);
char* uv_err_name_r(int err, char* buf, size_t buflen);
```

**uv_translate_sys_error**：转换为系统错误码

**uv_strerror**：返回错误描述信息，比如：operation canceled

**uv_strerror_r**：修改错误描述信息

**uv_err_name**：返回错误编码，比如：ECANCELED

**uv_err_name_r**：修改错误名称

## 程序相关

```cpp
char** uv_setup_args(int argc, char** argv);
int uv_get_process_title(char* buffer, size_t size);
int uv_set_process_title(const char* title);
int uv_resident_set_memory(size_t* rss);
int uv_uptime(double* uptime);
uv_os_fd_t uv_get_osfhandle(int fd);
```

**uv_setup_args：**Store the program arguments. Required for uv_get_process_title/uv_set_process_title.

**uv_get_process_title:**Gets the title of the current process. You *must* call *uv_setup_args* before calling this function. 

**uv_set_process_title**:Sets the current process title. You *must* call *uv_setup_args* before calling this function.

**uv_resident_set_memory**：Gets the resident set size (RSS) for the current process.

**uv_uptime**：Gets the current system uptime.获取系统运行时间

**uv_get_osfhandle**：For a file descriptor in the C runtime, get the OS-dependent handle. On UNIX, returns the fd intact. On Windows, this calls [_get_osfhandle](https://msdn.microsoft.com/en-us/library/ks2530z6.aspx). Note that the return value is still owned by the C runtime, any attempts to close it or to use it after closing the fd may lead to malfunction.获取系统剩余可用句柄

**VSS**- Virtual Set Size 虚拟耗用内存（包含共享库占用的内存）

**RSS**- Resident Set Size 实际使用物理内存（包含共享库占用的内存）

**PSS**- Proportional Set Size 实际使用的物理内存（比例分配共享库占用的内存）

**USS**- Unique Set Size 进程独自占用的物理内存（不包含共享库占用的内存）

## 内存相关：

```cpp
int uv_replace_allocator(uv_malloc_func malloc_func,
                                   uv_realloc_func realloc_func,
                                   uv_calloc_func calloc_func,
                                   uv_free_func free_func);
```

**uv_replace_allocator****：**

Override the use of the standard library’s [malloc(3)](http://linux.die.net/man/3/malloc), [calloc](http://linux.die.net/man/3/calloc)[(3)](http://linux.die.net/man/3/calloc), [realloc](http://linux.die.net/man/3/realloc)[(3)](http://linux.die.net/man/3/realloc), [free(3)](http://linux.die.net/man/3/free), memory allocation functions.

This function must be called before any other libuv function is called or after all resources have been freed and thus libuv doesn’t reference any allocated memory chunk.

On success, it returns 0, if any of the function pointers is NULL it returns UV_EINVAL.

重载系统的malloc, calloc, realloc, free函数

## 共享库相关

```cpp
UV_EXTERN int uv_dlopen(const char* filename, uv_lib_t* lib);
UV_EXTERN void uv_dlclose(uv_lib_t* lib);
UV_EXTERN int uv_dlsym(uv_lib_t* lib, const char* name, void** ptr);
UV_EXTERN const char* uv_dlerror(const uv_lib_t* lib);
```

[http://docs.libuv.org/en/v1.x/dll.html](http://docs.libuv.org/en/v1.x/dll.html?highlight=uv_dlopen)

**int uv_dlopen(const char\* filename, uv_lib_t\* lib)**

Opens a shared library. The filename is in utf-8. Returns 0 on success and -1 on error. Call [`uv_dlerror()`](http://docs.libuv.org/en/v1.x/dll.html?highlight=uv_dlopen#c.uv_dlerror) to get the error message.

**void uv_dlclose(uv_lib_t\* lib)**

Close the shared library.

**int uv_dlsym(uv_lib_t\* lib, const char\* name, void\** ptr)**

Retrieves a data pointer from a dynamic library. It is legal for a symbol to map to NULL. Returns 0 on success and -1 if the symbol was not found.

**const char\* uv_dlerror(const uv_lib_t\* lib)**

Returns the last uv_dlopen() or uv_dlsym() error message.

## **其它**

```cpp
UV_EXTERN int uv_exepath(char* buffer, size_t* size);
 
UV_EXTERN int uv_cwd(char* buffer, size_t* size);
 
UV_EXTERN int uv_chdir(const char* dir);
 
UV_EXTERN uint64_t uv_get_free_memory(void);
UV_EXTERN uint64_t uv_get_total_memory(void);
 
UV_EXTERN uint64_t uv_hrtime(void);
```

**int uv_exepath(char\* buffer, size_t\* size)**

Gets the executable path.

**int uv_cwd(char\* buffer, size_t\* size)**

Gets the current working directory, and stores it in buffer. If the current working directory is too large to fit in buffer, this function returns UV_ENOBUFS, and sets size to the required length, including the null terminator.

Changed in version 1.1.0: On Unix the path no longer ends in a slash.

Changed in version 1.9.0: the returned length includes the terminating null byte on UV_ENOBUFS, and the buffer is null terminated on success.

**int uv_chdir(const char\* dir)**

Changes the current working directory.

**uint64_t uv_get_total_memory(void)**

Gets memory information (in bytes).

**uint64_t uv_hrtime(void)**

Returns the current high-resolution real time. This is expressed in nanoseconds. It is relative to an arbitrary time in the past. It is not related to the time of day and therefore not subject to clock drift. The primary use is for measuring performance between intervals.

Note:Not every platform can support nanosecond resolution; however, this value will always be in nanoseconds.

**int uv_cpu_info(uv_cpu_info_t\** cpu_infos, int\* count)**

Gets information about the CPUs on the system. The cpu_infos array will have count elements and needs to be freed with [`uv_free_cpu_info()`](http://docs.libuv.org/en/v1.x/misc.html?highlight=uv_if_indextoname#c.uv_free_cpu_info).

**void uv_free_cpu_info(uv_cpu_info_t\* cpu_infos, int count)**

Frees the cpu_infos array previously allocated with [`uv_cpu_info()`](http://docs.libuv.org/en/v1.x/misc.html?highlight=uv_if_indextoname#c.uv_cpu_info).

详细参考<http://docs.libuv.org/en/v1.x/misc.html>

提供了CPU、内存、系统时间、IP格式转换、程序名称、进程IP、interface_address、执行路径、打印loop、优先级等通用API。

 













