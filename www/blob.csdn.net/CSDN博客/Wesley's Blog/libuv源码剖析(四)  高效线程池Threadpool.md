
# libuv源码剖析(四): 高效线程池Threadpool - Wesley's Blog - CSDN博客


2018年08月29日 14:36:35[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：371


## Introduction
在网络编程中, 始终都是基于`Reactor`模型的变种, 无论怎么演化,**核心组件都包括: Reactor实例(事件注册, 注销, 通知); 多路复用器(由操作系统提供, 比如kqueue, select, epoll); 事件处理器(handler)以及事件源(linux中这就是描述符)这四个组件.**
一般，会单独启动一个线程运行Reactor实例来实现真正的异步操作。但是，依赖操作系统提供的系统调用来实现异步是有局限的，比如在Reactor模型中我们只能监听到：网络IO事件、signel(信号)、超时事件以及一些管道事件等，但这些事件也只是通知我们资源可读或者可写，**真正的读写操作（read和write）还是同步的**（也就是你必须等到read或者write返回，虽然linux提供了aio，但是其有诸多槽点），那么**libuv的全异步**是如何做到的呢？你可能会很快想到，就是**启用单独的线程来做同步**的事情，这也是libuv的设计思路，借用官网的一张图，说明一切：
![这里写图片描述](https://img-blog.csdn.net/20180829143602787?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
由上图可以看到，libuv实现了一套自己的线程池来处理所有同步操作（从而模拟出异步的效果），下面就来看一下该线程池的具体实现吧！
## 线程池模型
几乎所有的线程池都遵守着下面这个模型(任务队列+线程池):
![这里写图片描述](https://img-blog.csdn.net/20180829143552368?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
在libuv中, 事件队列借助自身的高线队列实现, 具体实现可参考我的另一篇博文:[libuv源码剖析(一): 高效队列 Queue](https://blog.csdn.net/yinanmo5569/article/details/82085461)
接下来我们来看`threadpool`部分的实现.
首先, libuv对于 task 的定义:
`struct uv__work {
  void (*work)(struct uv__work *w);
  void (*done)(struct uv__work *w, int status);
  struct uv_loop_s* loop;
  void* wq[2];
};`两个回调函数指针(一个是实际任务, 一个是任务做完之后的回调),`void *wq[2]`是`work queue`中的节点, 通过这个节点组成一条链.
至于loop用来标明在哪个loop中.
再来看下 threadpool 初始化的过程:
`#define MAX_THREADPOOL_SIZE 128
static uv_once_t once = UV_ONCE_INIT;
static uv_cond_t cond;
static uv_mutex_t mutex;
static unsigned int idle_threads;//当前空闲的线程数
static unsigned int nthreads;
static uv_thread_t* threads;
static uv_thread_t default_threads[4];
static QUEUE exit_message;
static QUEUE wq;//线程池全部会检查这个queue，一旦发现有任务就执行，但是只能有一个线程抢占到
static volatile int initialized;

static void init_once(void) {
  unsigned int i;
  const char* val;
  // 线程池中的线程数，默认值为4
  nthreads = ARRAY_SIZE(default_threads);
  val = getenv("UV_THREADPOOL_SIZE");
  if (val != NULL)
    nthreads = atoi(val);
  if (nthreads == 0)
    nthreads = 1;
  if (nthreads > MAX_THREADPOOL_SIZE)
    nthreads = MAX_THREADPOOL_SIZE;
  threads = default_threads;
  if (nthreads > ARRAY_SIZE(default_threads)) {
    // 分配线程句柄
    threads = uv__malloc(nthreads * sizeof(threads[0]));
    if (threads == NULL) {
      nthreads = ARRAY_SIZE(default_threads);
      threads = default_threads;
    }
  }
  // 初始化条件变量
  if (uv_cond_init(&cond))
    abort();
  // 初始化互斥锁
  if (uv_mutex_init(&mutex))
    abort();
  // 初始化任务队列
  QUEUE_INIT(&wq);
  // 创建nthreads个线程
  for (i = 0; i < nthreads; i++)
    if (uv_thread_create(threads + i, worker, NULL))
      abort();
  initialized = 1;
}`上面的代码中，一共创建了nthreads个线程，那么每个线程的执行代码是什么呢？由线程创建代码：`uv_thread_create(threads + i, worker, NULL)`，可以看到，每一个线程都是执行worker函数，下面看看worker函数都在做什么：
`/* To avoid deadlock with uv_cancel() it's crucial that the worker
 * never holds the global mutex and the loop-local mutex at the same time.
 */
static void worker(void* arg) {
  struct uv__work* w;
  QUEUE* q;
  (void) arg;
  for (;;) {
    // 因为是多线程访问，因此需要加锁同步
    uv_mutex_lock(&mutex);
    // 如果任务队列是空的
    while (QUEUE_EMPTY(&wq)) {
      // 空闲线程数加1
      idle_threads += 1;
      // 等待条件变量
      uv_cond_wait(&cond, &mutex);
      // 被唤醒之后，说明有任务被post到队列，因此空闲线程数需要减1
      idle_threads -= 1;
    }
    // 取出队列的头部节点（第一个task）
    q = QUEUE_HEAD(&wq);
    if (q == &exit_message)
      uv_cond_signal(&cond);
    else {
      // 从队列中移除这个task
      QUEUE_REMOVE(q);
      QUEUE_INIT(q);  /* Signal uv_cancel() that the work req is
                             executing. */
    }
    uv_mutex_unlock(&mutex);
    if (q == &exit_message)
      break;
    // 取出uv__work首地址
    w = QUEUE_DATA(q, struct uv__work, wq);
    // 调用task的work，执行任务
    w->work(w);
    uv_mutex_lock(&w->loop->wq_mutex);
    w->work = NULL;  /* Signal uv_cancel() that the work req is done
                        executing. */
    QUEUE_INSERT_TAIL(&w->loop->wq, &w->wq);
    uv_async_send(&w->loop->wq_async);
    uv_mutex_unlock(&w->loop->wq_mutex);
  }
}`可以看到，多个线程都会在worker方法中等待在conn条件变量上，一旦有任务加入队列，线程就会被唤醒，然后只有一个线程会得到任务的执行权，其他的线程只能继续等待。
那么如何向队列提交一个task呢？看以下代码：
`void uv__work_submit(uv_loop_t* loop,
                 struct uv__work* w,
                 void (*work)(struct uv__work* w),
                 void (*done)(struct uv__work* w, int status)) {
  uv_once(&once, init_once);
  // 构造一个task
  w->loop = loop;
  w->work = work;
  w->done = done;
  // 将其插入任务队列
  post(&w->wq);
}`接着看post做了什么：
`static void post(QUEUE* q) {
  // 同步队列操作
  uv_mutex_lock(&mutex);
  // 将task插入队列尾部
  QUEUE_INSERT_TAIL(&wq, q);
  // 如果当前有空闲线程，就向条件变量发送信号
  if (idle_threads > 0)
    uv_cond_signal(&cond);
  uv_mutex_unlock(&mutex);
}`有提交任务，就肯定会有取消一个任务的操作，是的，他就是`uv__work_cancel`，代码如下：
`static int uv__work_cancel(uv_loop_t* loop, uv_req_t* req, struct uv__work* w) {
  int cancelled;
  uv_mutex_lock(&mutex);
  uv_mutex_lock(&w->loop->wq_mutex);
  // 只有当前队列不为空并且要取消的uv__work有效时才会继续执行
  cancelled = !QUEUE_EMPTY(&w->wq) && w->work != NULL;
  if (cancelled)
    QUEUE_REMOVE(&w->wq);// 从队列中移除task
  uv_mutex_unlock(&w->loop->wq_mutex);
  uv_mutex_unlock(&mutex);
  if (!cancelled)
    return UV_EBUSY;
  // 更新这个task的状态
  w->work = uv__cancelled;
  uv_mutex_lock(&loop->wq_mutex);
  QUEUE_INSERT_TAIL(&loop->wq, &w->wq);
  uv_async_send(&loop->wq_async);
  uv_mutex_unlock(&loop->wq_mutex);
  return 0;
}`至此，一个线程池的组成以及实现原理都说完了，可以看到，libuv几乎是用了最少的代码完成了高效的线程池，这对于我们平时写代码时具有很好的借鉴意义，文中涉及到uv_req_t以及uv_loop_t等结构我都直接跳过，因为这牵扯到libuv的其他组件，我将在以后的源码剖析中逐步阐述，谢谢你能看到这里。
## 举个例子
接下来,我们再通过一个最简单的栗子看libuv是如何使用这个线程池的.我们来看 uvbook/queue-work/main.c 中的示例代码:
`#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uv.h>
#define FIB_UNTIL 5
uv_loop_t *loop;
long fib_(long t) {
    if (t == 0 || t == 1)
        return 1;
    else
        return fib_(t-1) + fib_(t-2);
}
void fib(uv_work_t *req) {
    int n = *(int *) req->data;
    if (random() % 2)
        sleep(1);
    else
        sleep(3);
    long fib = fib_(n);
    fprintf(stderr, "%dth fibonacci is %lu\n", n, fib);
}
void after_fib(uv_work_t *req, int status) {
    fprintf(stderr, "Done calculating %dth fibonacci\n", *(int *) req->data);
}
int main() {
    loop = uv_default_loop();
    int data[FIB_UNTIL];
    uv_work_t req[FIB_UNTIL];
    int i;
    for (i = 0; i < FIB_UNTIL; i++) {
        data[i] = i;
        req[i].data = (void *) &data[i];
        uv_queue_work(loop, &req[i], fib, after_fib);
    }
    return uv_run(loop, UV_RUN_DEFAULT);
}`上面的代码的输出如下:
`0th fibonacci is 1
2th fibonacci is 2
Done calculating 0th fibonacci
Done calculating 2th fibonacci
3th fibonacci is 3
Done calculating 3th fibonacci
4th fibonacci is 5
Done calculating 4th fibonacci
1th fibonacci is 1
Done calculating 1th fibonacci`其中,`uv_queue_work`是一个很方便的函数允许应用在一个隔离的线程运行,并且结束之后调用 callback 函数, 我们来看下它的实现:
`int uv_queue_work(uv_loop_t* loop,
                  uv_work_t* req,
                  uv_work_cb work_cb,
                  uv_after_work_cb after_work_cb) {
  if (work_cb == NULL)
    return UV_EINVAL;
  uv__req_init(loop, req, UV_WORK);
  req->loop = loop;
  req->work_cb = work_cb;
  req->after_work_cb = after_work_cb;
  uv__work_submit(loop, &req->work_req, uv__queue_work, uv__queue_done);
  return 0;
}`可以看出,就是通过这个函数调用了`uv__work_submit`来将任务提交到任务队列中. 在使用 gdb 打断点调试的时候, 能发现在第一次执行到`uv_queue_work`的时候, 就会立马创建出 4 个线程, 并且立刻执行`work_cb`任务. 而`after_work_cb`则需要到`uv_run`的时候才会执行
非常感谢大牛的博客, 参考链接:
[https://my.oschina.net/fileoptions/blog/1036609](https://my.oschina.net/fileoptions/blog/1036609)

