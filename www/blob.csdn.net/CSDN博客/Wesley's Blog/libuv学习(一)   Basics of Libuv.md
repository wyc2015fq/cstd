
# libuv学习(一) : Basics of Libuv - Wesley's Blog - CSDN博客


2018年08月19日 20:07:39[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：108标签：[libuv																](https://so.csdn.net/so/search/s.do?q=libuv&t=blog)个人分类：[计算机网络																](https://blog.csdn.net/yinanmo5569/article/category/7951171)



## Introduction
**libuv强制异步,事件驱动编程.**它的核心功能是提供一个事件队列和回调机制基于IO提醒和其他活动. libuv 提供核心的功能包括 timers, non-blocking networking support, asynchronous file system access, child processes等.
## Hello world
以下代码建立了一个`uv_loop_t`来监听实践, 因为这个loop并没有任何监听事件,所以会马上结束.
也可以直接使用`uv_default_loop()`来直接获取使用 libuv 默认的 loop
`#include <stdio.h>
#include <stdlib.h>
#include <uv.h>
int main() {
    uv_loop_t *loop = malloc(sizeof(uv_loop_t));
    uv_loop_init(loop);
    printf("Now quitting.\n");
    // 下面可替换为 uv_run(uv_default_loop(), UV_RUN_DEFAULT); 
    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    free(loop);
    return 0;
}`
## Handles and Requests
用户需要通过注册`Handles`来监听自己所感兴趣的**事件(event)**. 这里的 handles 其实与 c\# 中的 event 是有些相似的, 就是一种事件. libuv 中的 handles 是一种数据结构, 都是使用`uv_TYPE_t`来命名.
**libuv watchers:**
`/* Handle types. */
typedef struct uv_loop_s uv_loop_t;
typedef struct uv_handle_s uv_handle_t;
typedef struct uv_stream_s uv_stream_t;
typedef struct uv_tcp_s uv_tcp_t;
typedef struct uv_udp_s uv_udp_t;
typedef struct uv_pipe_s uv_pipe_t;
typedef struct uv_tty_s uv_tty_t;
typedef struct uv_poll_s uv_poll_t;
typedef struct uv_timer_s uv_timer_t;
typedef struct uv_prepare_s uv_prepare_t;
typedef struct uv_check_s uv_check_t;
typedef struct uv_idle_s uv_idle_t;
typedef struct uv_async_s uv_async_t;
typedef struct uv_process_s uv_process_t;
typedef struct uv_fs_event_s uv_fs_event_t;
typedef struct uv_fs_poll_s uv_fs_poll_t;
typedef struct uv_signal_s uv_signal_t;
/* Request types. */
typedef struct uv_req_s uv_req_t;
typedef struct uv_getaddrinfo_s uv_getaddrinfo_t;
typedef struct uv_getnameinfo_s uv_getnameinfo_t;
typedef struct uv_shutdown_s uv_shutdown_t;
typedef struct uv_write_s uv_write_t;
typedef struct uv_connect_s uv_connect_t;
typedef struct uv_udp_send_s uv_udp_send_t;
typedef struct uv_fs_s uv_fs_t;
typedef struct uv_work_s uv_work_t;
/* None of above. */
typedef struct uv_cpu_info_s uv_cpu_info_t;
typedef struct uv_interface_address_s uv_interfac;
typedef struct uv_dirent_s uv_dirent_t;`其中上面的`Handles`代表`long-lived`的 objects, 而`Requests`代表`short-lived`, 并且通常是 I/O 操作的 handle
Handles 通过固定的函数来**初始化**(也是向`loop`**注册**自己):
`uv_TYPE_init(uv_loop_t *, uv_TYPE_t *)`
## Idling
下面以一个`空闲句柄(idle handle)`为例, idle handle 的回调函数会在每次`loop turn`都会被执行一次.
`#include <stdio.h>
#include <uv.h>
int64_t counter = 0;
void wait_for_a_while(uv_idle_t * handle) {
    counter++;
    if (counter >= 10e6)
        uv_idle_stop(handle);
}
int main() {
    uv_idle_t idler;
    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_start(&idler, wait_for_a_while);
    printf("Idling...\n");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    uv_loop_close(uv_default_loop());
    return 0;
}`在上面这段代码中, 先是通过`uv_idle_init`来向 loop 注册了 idle handle, 然后再通过`uv_idle_start`来向 idle 这个事件注册对应的**回调函数**.
在这个过程中, 是通过`uv_TYPE_t`来传递事件**上下文(context)**的, 所有的 handles 和 request 都有一个 void *data 的成员变量用于保存上下文. 在之后的文章中会提到.
本文简化翻译自[uvbook](https://github.com/nikhilm/uvbook)

