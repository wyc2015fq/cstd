
# libuv学习(四) : Threads - Wesley's Blog - CSDN博客


2018年08月21日 17:17:53[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：85


## Introduction
当下主要有两种流行的线程库:`Windows Threads`和`Posix`实现. libuv的thread API类似于`pthread`库.
**libuv 允许你通过开启线程并且在结束时收集结果来模拟异步操作,而实际上是阻塞操作**
所以 libuv 提供自己实现的 thread 方法,是为了能够让某些在 loop 中的 handle 能够在独立的线程中运行, 也提供方法来与他们进行线程间通信.
接下来的例子假设: libuv 只有一个`event loop`, 并且只在单线程上运行.
## 核心 Thread 操作
创建线程:
`int uv_thread_create(uv_thread_t* tid, uv_thread_cb entry, void* arg)`等待线程结束:
`int uv_thread_join(uv_thread_t *tid)`龟兔赛跑完整代码:
`#include <stdio.h>
#include <unistd.h>
#include <uv.h>
void hare(void *arg) {
    int tracklen = *((int *) arg);
    while (tracklen) {
        tracklen--;
        sleep(1);
        fprintf(stderr, "Hare ran another step\n");
    }
    fprintf(stderr, "Hare done running!\n");
}
void tortoise(void *arg) {
    int tracklen = *((int *) arg);
    while (tracklen) {
        tracklen--;
        fprintf(stderr, "Tortoise ran another step\n");
        sleep(3);
    }
    fprintf(stderr, "Tortoise done running!\n");
}
int main() {
    int tracklen = 10;
    uv_thread_t hare_id;
    uv_thread_t tortoise_id;
    uv_thread_create(&hare_id, hare, &tracklen);
    uv_thread_create(&tortoise_id, tortoise, &tracklen);
    uv_thread_join(&hare_id);
    uv_thread_join(&tortoise_id);
    return 0;
}`
## 线程同步
libuv也提供了相应的同步机制,包括`Mutex`,`rwlocks`,`semaphores`,`condition variables`,`barriers`等
以最经典的读者写者问题为例:
`#include <stdio.h>
#include <uv.h>
uv_barrier_t blocker;
uv_rwlock_t numlock;
int shared_num;
void reader(void *n)
{
    int num = *(int *)n;
    int i;
    for (i = 0; i < 20; i++) {
        uv_rwlock_rdlock(&numlock);
        printf("Reader %d: acquired lock\n", num);
        printf("Reader %d: shared num = %d\n", num, shared_num);
        uv_rwlock_rdunlock(&numlock);
        printf("Reader %d: released lock\n", num);
    }
    uv_barrier_wait(&blocker);
}
void writer(void *n)
{
    int num = *(int *)n;
    int i;
    for (i = 0; i < 20; i++) {
        uv_rwlock_wrlock(&numlock);
        printf("Writer %d: acquired lock\n", num);
        shared_num++;
        printf("Writer %d: incremented shared num = %d\n", num, shared_num);
        uv_rwlock_wrunlock(&numlock);
        printf("Writer %d: released lock\n", num);
    }
    uv_barrier_wait(&blocker);
}
int main()
{
    uv_barrier_init(&blocker, 4);
    shared_num = 0;
    uv_rwlock_init(&numlock);
    uv_thread_t threads[3];
    int thread_nums[] = {1, 2, 1};
    uv_thread_create(&threads[0], reader, &thread_nums[0]);
    uv_thread_create(&threads[1], reader, &thread_nums[1]);
    uv_thread_create(&threads[2], writer, &thread_nums[2]);
    uv_barrier_wait(&blocker);
    uv_barrier_destroy(&blocker);
    uv_rwlock_destroy(&numlock);
    return 0;
}`到这里为止, 使用libuv的线程操作可以说与 pthread 太大区别.
## libuv work queue
uv_queue_work() 是一个很方便的函数允许应用在一个隔离的线程运行,并且结束之后调用 callback 函数.
`int uv_queue_work(uv_loop_t* loop, uv_work_t* req, uv_work_cb work_cb, uv_after_work_cb after_work_cb)`用起来十分简单, 可以通过在 uv_work_t._data 添加自己的数据来实现与线程通信.
调用 uv_queue_work 一次就类似于之前的调用了 uv_TYPE_t 一次,在loop中添加了一个`handle`(或者说事件), 在`uv_run`的时候,就会被取出并且进行处理
举个栗子, 异步版斐波那契:
`#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uv.h>
#define FIB_UNTIL 25
uv_loop_t *loop;
uv_work_t fib_reqs[FIB_UNTIL];
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
    if (status == UV_ECANCELED)
        fprintf(stderr, "Calculation of %d cancelled.\n", *(int *) req->data);
}
void signal_handler(uv_signal_t *req, int signum)
{
    printf("Signal received!\n");
    int i;
    for (i = 0; i < FIB_UNTIL; i++) {
        uv_cancel((uv_req_t*) &fib_reqs[i]);
    }
    uv_signal_stop(req);
}
int main() {
    loop = uv_default_loop();
    int data[FIB_UNTIL];
    int i;
    for (i = 0; i < FIB_UNTIL; i++) {
        data[i] = i;
        fib_reqs[i].data = (void *) &data[i];
        uv_queue_work(loop, &fib_reqs[i], fib, after_fib);
    }
    uv_signal_t sig;
    uv_signal_init(loop, &sig);
    uv_signal_start(&sig, signal_handler, SIGINT);
    return uv_run(loop, UV_RUN_DEFAULT);
}`其中, uv_cancel()用来取消还没有正在运行的 task, 如果已经开始运行,将会取消失败.
## 线程间通信
异步线程通信通过 loop 来沟通, 任何一个线程可以作为 message sender, 但是只有在 loop 中的线程能够作为 receiver. 需要注意的是, 由于`uv_async_send`是异步的,**它作用是只能保证回调函数会被调用一次, 如果有多个 pending 的 uv_async_send 的话,也只会调用一次**
举个栗子,就以浏览器下载文件,需要把下载进度实时汇报给主进程:
`#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <uv.h>
uv_loop_t *loop;
uv_async_t async;
double percentage;
void fake_download(uv_work_t *req) {
    int size = *((int*) req->data);
    int downloaded = 0;
    while (downloaded < size) {
        percentage = downloaded*100.0/size;
        async.data = (void*) &percentage;
        uv_async_send(&async);
        sleep(1);
        downloaded += (200+random())%1000; // can only download max 1000bytes/sec,
                                           // but at least a 200;
    }
}
void after(uv_work_t *req, int status) {
    fprintf(stderr, "Download complete\n");
    uv_close((uv_handle_t*) &async, NULL);
}
void print_progress(uv_async_t *handle) {
    double percentage = *((double*) handle->data);
    fprintf(stderr, "Downloaded %.2f%%\n", percentage);
}
int main() {
    loop = uv_default_loop();
    uv_work_t req;
    int size = 10240;
    req.data = (void*) &size;
    uv_async_init(loop, &async, print_progress);
    uv_queue_work(loop, &req, fake_download, after);
    return uv_run(loop, UV_RUN_DEFAULT);
}`本文简化翻译自[uvbook](https://github.com/nikhilm/uvbook)

