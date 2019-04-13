
# libuv源码剖析(三): 事件循环loop - Wesley's Blog - CSDN博客


2018年08月28日 17:05:16[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：539


## Introduction
libuv中的 loop 是 libuv 最为核心的事件队列. 在熟悉了 libuv 的高效事件队列 queue, 和基本数据结构 handle 之后, 就需要来了解一下 loop 的实现.
下面通过一个uvbook中具体的实例来讲解.
## uv_loop_t 数据结构
我们首先看一下 uv_loop_t 的数据结构:
`struct uv_loop_s {
  /* User data - use this for whatever. */
  void* data;
  /* Loop reference counting. */
  unsigned int active_handles;
  void* handle_queue[2];
  void* active_reqs[2];
  /* Internal flag to signal loop stop. */
  unsigned int stop_flag;
  UV_LOOP_PRIVATE_FIELDS
};`其中`void *data`用来存放任意的用户数据,`void *handle_queue[2]`就是 handle 的队列, 为什么用两个空指针来表示可以看我之前的源码剖析文章.`void *active_reqs[2]`是 request 队列,`stop_flag`表示是否运行, 在run和close的时候会观察这个变量.
再然后`UV_LOOP_PRIVATE_FIELDS`是用户不应该操纵的变量, 其中变量较多, 包括了很多队列:`idle_handles`,`pending_queue`,`watcher_queue`等等, 这里就不列举出来了
## 举个例子
下面以一个较为简单的sample代码为例, 例子来源于`uvbook/code/idle-basic`, 稍稍修改了一下, 多加了一个`idle_handle`
`#include <stdio.h>
#include <uv.h>
int64_t counter = 0;
void wait_for_a_while(uv_idle_t* handle) {
    counter++;
    if (counter >= 10e6)
        uv_idle_stop(handle);
}
void do_something(uv_idle_t* handle){
    printf("Do something\n");
    uv_idle_stop(handle);
}
int main() {
    uv_idle_t idler;
    uv_idle_t idler2;
    uv_idle_init(uv_default_loop(), &idler);
    uv_idle_init(uv_default_loop(), &idler2);
    uv_idle_start(&idler, wait_for_a_while);
    uv_idle_start(&idler2, do_something);
    printf("Idling...\n");
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
    uv_loop_close(uv_default_loop());
    return 0;
}`上面代码的输出是:
`Idling...
Do something`下面我们来一部分一部分来看
### 第一部分, uv_idle_init
`uv_idle_t idler;
uv_idle_init(uv_default_loop(), &idler);`上面代码声明一个 handle, 并且注册到 loop 中, 那么是如何注册的呢? 我们来看一下`uv_idle_init`的代码:
`#define UV_LOOP_WATCHER_DEFINE(name, type)                                    \
  int uv_##name##_init(uv_loop_t* loop, uv_##name##_t* handle) {              \
    uv__handle_init(loop, (uv_handle_t*)handle, UV_##type);                   \
    handle->name##_cb = NULL;                                                 \
    return 0;                                                                 \
  }         
UV_LOOP_WATCHER_DEFINE(idle, IDLE)`就是调用了 uv__handle_init:
`#define uv__handle_init(loop_, h, type_)                                      \
  do {                                                                        \
    (h)->loop = (loop_);                                                      \
    (h)->type = (type_);                                                      \
    (h)->flags = UV__HANDLE_REF;  /* Ref the loop when active. */             \
    QUEUE_INSERT_TAIL(&(loop_)->handle_queue, &(h)->handle_queue);            \
    uv__handle_ platform_init(h);                                              \
  }                                                                           \
  while (0)`整个代码使用**宏定义**来实现, 保证了效率. 至于为啥要有个`do {} while(0)`我确实不太理解, 希望有高手能评论解释以下. 上面代码最重要的就是:
`QUEUE_INSERT_TAIL(&(loop_)->handle_queue, &(h)->handle_queue);`这段代码相当于**将句柄队列节点注册到了整个 loop 的队列中**.
### 第二部分, uv_idle_start
`uv_idle_start(&idler, wait_for_a_while)`这段代码一看就知道是给 handle 注册回调函数, 实际实现也很简单
`int uv_##name##_start(uv_##name##_t* handle, uv_##name##_cb cb) {           \
    if (uv__is_active(handle)) return 0;                                      \
    if (cb == NULL) return -EINVAL;                                           \
    QUEUE_INSERT_HEAD(&handle->loop->name##_handles, &handle->queue);         \
    handle->name##_cb = cb;                                                   \
    uv__handle_start(handle);                                                 \
    return 0;                                                                 \
  }`但是我们仔细看, 除了设置回调函数之外还做了另一个操作:
`QUEUE_INSERT_HEAD(&handle->loop->name##_handles, &handle->queue);`其实这段代码是因为`uv_idle_handle`的特殊性,`idle_handle`是会在每个 turn 都会执行的(之后也能看到), 所以loop中有专门的`idle_handles`队列, 在run的过程中每个turn都会把`idle_handles`队列中的所有`handles`回调函数都执行一遍.
所以说,**这个 uv_idle_start 的作用是, 设置回调函数 + 把idle句柄注册到队列的 idle 队列中**
### 第三部分, uv_run
`uv_run(uv_default_loop(), UV_RUN_DEFAULT);`开始运行 loop, 完整代码在此:
`int uv_run(uv_loop_t* loop, uv_run_mode mode) {
  int timeout;
  int r;
  int ran_pending;
  r = uv__loop_alive(loop);
  if (!r)
    uv__update_time(loop);
  while (r != 0 && loop->stop_flag == 0) {
    uv__update_time(loop);
    uv__run_timers(loop);
    ran_pending = uv__run_pending(loop);
    uv__run_idle(loop);
    uv__run_prepare(loop);
    timeout = 0;
    if ((mode == UV_RUN_ONCE && !ran_pending) || mode == UV_RUN_DEFAULT)
      timeout = uv_backend_timeout(loop);
    uv__io_poll(loop, timeout);
    uv__run_check(loop);
    uv__run_closing_handles(loop);
    if (mode == UV_RUN_ONCE) {
      /* UV_RUN_ONCE implies forward progress: at least one callback must have
       * been invoked when it returns. uv__io_poll() can return without doing
       * I/O (meaning: no callbacks) when its timeout expires - which means we
       * have pending timers that satisfy the forward progress constraint.
       *
       * UV_RUN_NOWAIT makes no guarantees about progress so it's omitted from
       * the check.
       */
      uv__update_time(loop);
      uv__run_timers(loop);
    }
    r = uv__loop_alive(loop);
    if (mode == UV_RUN_ONCE || mode == UV_RUN_NOWAIT)
      break;
  }
  /* The if statement lets gcc compile it to a conditional store. Avoids
   * dirtying a cache line.
   */
  if (loop->stop_flag != 0)
    loop->stop_flag = 0;
  return r;
}`这段代码也对应着官方文档的 libuv I/O loop order:
![这里写图片描述](https://img-blog.csdn.net/2018082817045293?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
调用`uv__run_idle(loop);`执行idle句柄的回调函数,`uv__run_idle`作用就是遍历所有的 idle_handle, 取出数据并且调用回调函数:
`void uv__run_##name(uv_loop_t* loop) {                                      \
    uv_##name##_t* h;                                                         \
    QUEUE* q;                                                                 \
    QUEUE_FOREACH(q, &loop->name##_handles) {                                 \
      h = QUEUE_DATA(q, uv_##name##_t, queue);                                \
      h->name##_cb(h);                                                        \
    }                                                                         \
  }`到这里, 就开始真正运行回调函数了.
## 总结
感觉 libuv 最大优势在于纯用c来实现, 并且使用了很多宏定义来替换了c++本来需要通过继承的方式实现的内容, 达到了高效的效果. 整个事件驱动异步IO的整体思路还是与其他的实现没有太大区别的.

