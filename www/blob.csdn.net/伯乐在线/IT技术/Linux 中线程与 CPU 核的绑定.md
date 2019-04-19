# Linux 中线程与 CPU 核的绑定 - 文章 - 伯乐在线
原文出处： [wta670](http://blog.chinaunix.net/uid-26739406-id-3181199.html)
最近在对项目进行性能优化，由于在多核平台上，所以了解了些进程、线程绑定cpu核的问题，在这里将所学记录一下。
不管是线程还是进程，都是通过设置亲和性(affinity)来达到目的。对于进程的情况，一般是使用sched_setaffinity这个函数来实现，网上讲的也比较多，这里主要讲一下线程的情况。
与进程的情况相似，线程亲和性的设置和获取主要通过下面两个函数来实现：

```
int pthread_setaffinity_np(pthread_t thread, size_t cpusetsize，
const cpu_set_t *cpuset);
int pthread_getaffinity_np(pthread_t thread, size_t cpusetsize, 
cpu_set_t *cpuset);
```
从函数名以及参数名都很明了，唯一需要点解释下的可能就是cpu_set_t这个结构体了。这个结构体的理解类似于select中的fd_set，可以理解为cpu集，也是通过约定好的宏来进行清除、设置以及判断：

```
//初始化，设为空
void CPU_ZERO (cpu_set_t *set); 
//将某个cpu加入cpu集中 
void CPU_SET (int cpu, cpu_set_t *set); 
//将某个cpu从cpu集中移出 
void CPU_CLR (int cpu, cpu_set_t *set); 
//判断某个cpu是否已在cpu集中设置了 
int CPU_ISSET (int cpu, const cpu_set_t *set);
```
cpu集可以认为是一个掩码，每个设置的位都对应一个可以合法调度的 cpu，而未设置的位则对应一个不可调度的 CPU。换而言之，线程都被绑定了，只能在那些对应位被设置了的处理器上运行。通常，掩码中的所有位都被置位了，也就是可以在所有的cpu中调度.
以下为测试代码：

```
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
void *myfun(void *arg)
{
    cpu_set_t mask;
    cpu_set_t get;
    char buf[256];
    int i;
    int j;
    int num = sysconf(_SC_NPROCESSORS_CONF);
    printf("system has %d processor(s)\n", num);
    for (i = 0; i < num; i++) {
        CPU_ZERO(&mask);
        CPU_SET(i, &mask);
        if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0) {
            fprintf(stderr, "set thread affinity failed\n");
        }
        CPU_ZERO(&get);
        if (pthread_getaffinity_np(pthread_self(), sizeof(get), &get) < 0) {
            fprintf(stderr, "get thread affinity failed\n");
        }
        for (j = 0; j < num; j++) {
            if (CPU_ISSET(j, &get)) {
                printf("thread %d is running in processor %d\n", (int)pthread_self(), j);
            }
        }
        j = 0;
        while (j++ < 100000000) {
            memset(buf, 0, sizeof(buf));
        }
    }
    pthread_exit(NULL);
}
int main(int argc, char *argv[])
{
    pthread_t tid;
    if (pthread_create(&tid, NULL, (void *)myfun, NULL) != 0) {
        fprintf(stderr, "thread create failed\n");
        return -1;
    }
    pthread_join(tid, NULL);
    return 0;
}
```
这段代码将使myfun线程在所有cpu中依次执行一段时间，在我的四核cpu上，执行结果为  ：

```
system has 4 processor(s)        
thread 1095604544 is running in processor 0        
thread 1095604544 is running in processor 1        
thread 1095604544 is running in processor 2        
thread 1095604544 is running in processor 3
```
在一些嵌入式设备中，运行的进程线程比较单一，如果指定进程线程运行于特定的cpu核，减少进程、线程的核间切换，有可能可以获得更高的性能。
