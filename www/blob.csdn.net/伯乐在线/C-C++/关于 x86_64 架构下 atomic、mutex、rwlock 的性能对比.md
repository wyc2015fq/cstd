# 关于 x86_64 架构下 atomic、mutex、rwlock 的性能对比 - 文章 - 伯乐在线
原文出处： [瀚海书香](http://blog.chinaunix.net/uid-20662820-id-4211959.html)
这里以多线程操作long类型变量，进行加法运算1亿次的时间作为性能对比的标准。
测试使用SLES 11SP2操作系统，3.0.80内核，CPU使用Xeon 5506（2 socket, 4 cores, 1thread）
![](http://blog.chinaunix.net/attachment/201404/16/20662820_1397616910w8K8.jpg)
由于针对64位类型的atomic glibc没有提供相应的库，将内核实现代码移植到应用层
atomic64.h


```
#ifndef __HI_ATOMIC64_H__
#define __HI_ATOMIC64_H__
#include <stdio.h>
#include <getopt.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
/* Learn from kernel */
#ifdef __x86_64__
#define LOCK_PREFIX "lock ;"
typedef struct {
        long long counter;
} atomic64_t;
/**
 * atomic64_read - read atomic64 variable
 * @v: pointer of type atomic64_t
 *
 * Atomically reads the value of <a href="http://www.jobbole.com/members/q1317827412">@v.</a>
 * Doesn't imply a read memory barrier.
 */
static inline long atomic64_read(const atomic64_t *v)
{
        return (*(volatile long *)&(v)->counter);
}
/**
 * atomic64_set - set atomic64 variable
 * @v: pointer to type atomic64_t
 * @i: required value
 *
 * Atomically sets the value of <a href="http://www.jobbole.com/members/shoujiliuyi6455">@v</a> to @i.
 */
static inline void atomic64_set(atomic64_t *v, long i)
{
        v->counter = i;
}
/**
 * atomic64_add - add integer to atomic64 variable
 * @i: integer value to add
 * @v: pointer to type atomic64_t
 *
 * Atomically adds <a href="http://www.jobbole.com/members/zhoann">@i</a> to <a href="http://www.jobbole.com/members/q1317827412">@v.</a>
 */
static inline void atomic64_add(long i, atomic64_t *v)
{
        asm volatile(LOCK_PREFIX "addq %1,%0"
                     : "=m" (v->counter)
                     : "er" (i), "m" (v->counter));
}
/**
 * atomic64_sub - subtract the atomic64 variable
 * @i: integer value to subtract
 * @v: pointer to type atomic64_t
 *
 * Atomically subtracts <a href="http://www.jobbole.com/members/zhoann">@i</a> from <a href="http://www.jobbole.com/members/q1317827412">@v.</a>
 */
static inline void atomic64_sub(long i, atomic64_t *v)
{
        asm volatile(LOCK_PREFIX "subq %1,%0"
                     : "=m" (v->counter)
                     : "er" (i), "m" (v->counter));
}
#else /* __x86_64__ */
/*FIXME:
 * This program will run on x86_64 machine in the expected future, we
 * do _not_ need to care other cpu architecture.
 */
#endif
#endif
```
测试代码performance.c


```
/*******************************************************************************
  Copyright(c) 2008-2014 
  This program is free software; you can redistribute it and/or modify it
  under the terms and conditions of the GNU General Public License,
  version 2, as published by the Free Software Foundation.
  This program is distributed in the hope it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
  more details.
  You should have received a copy of the GNU General Public License along with
  this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
  The full GNU General Public License is included in this distribution in
  the file called "COPYING".
  Date: 2014-04-15 21:27:57 CST
  Contact Information:
  Tony <tingw.liu@gmail.com>
  Home, Qingdao, China. 
*******************************************************************************/
#include "atomic64.h"
atomic64_t num;
long mutexnum = 0;
long maxnum;
struct timeval tv;
long starttime;
//FIXME: gettimeofday is a non-thread safe sysycall
static pthread_mutex_t timelock = PTHREAD_MUTEX_INITIALIZER;
#define TIME_LOCK() pthread_mutex_lock(&timelock)
#define TIME_UNLOCK() pthread_mutex_unlock(&timelock)
static pthread_mutex_t numlock = PTHREAD_MUTEX_INITIALIZER;
#define MUTEX_LOCK() pthread_mutex_lock(&numlock)
#define MUTEX_UNLOCK() pthread_mutex_unlock(&numlock)
static pthread_rwlock_t rwnumlock = PTHREAD_RWLOCK_INITIALIZER;
#define RW_LOCK() pthread_rwlock_wrlock(&rwnumlock)
#define RW_UNLOCK() pthread_rwlock_unlock(&rwnumlock);
static void * add_func(void *arg)
{
        long stoptime;
        while(1) {
                atomic64_add(1, &num);
                if (atomic64_read(&num) > maxnum) {
                        TIME_LOCK();
                        gettimeofday(&tv, 0);
                        TIME_UNLOCK();
                        stoptime= (long)tv.tv_sec * (long)1000000 +
                                (long)tv.tv_usec;
                        printf("Used %ld microseconds\n", stoptime - starttime);
                        break;
                }
        }
}
static void *add_func_rwlock(void *arg)
{
        long stoptime;
        while(1) {
                RW_LOCK();
                ++mutexnum;
                if (mutexnum > maxnum) {
                        RW_UNLOCK();
                        TIME_LOCK();
                        gettimeofday(&tv, 0);
                        TIME_UNLOCK();
                        stoptime= (long)tv.tv_sec * (long)1000000 +
                                (long)tv.tv_usec;
                        printf("Used %ld microseconds\n", stoptime - starttime);
                        break;
                }
                RW_UNLOCK();
        }
}
static void *add_func_mutex(void *arg)
{
        long stoptime;
        while(1) {
                MUTEX_LOCK();
                ++mutexnum;
                if (mutexnum > maxnum) {
                        MUTEX_UNLOCK();
                        TIME_LOCK();
                        gettimeofday(&tv, 0);
                        TIME_UNLOCK();
                        stoptime= (long)tv.tv_sec * (long)1000000 +
                                (long)tv.tv_usec;
                        printf("Used %ld microseconds\n", stoptime - starttime);
                        break;
                }
                MUTEX_UNLOCK();
        }
}
#define ATOMIC_TYPE 0
#define MUTEX_TYPE 1
#define RW_TYPE 2
int main(int argc, char **argv)
{
        pthread_t thread;
        pthread_attr_t thread_attr;
        int threadnum, i, type;
        if (argc != 4) {
                printf("Usage: %s threadnum maxnum type[0-atomic, 1-mutex, 2-rwlock]\n",
                                argv[0]);
                exit(0);
        }
        threadnum = atoi(argv[1]);
        maxnum = atoll(argv[2]);
        type = atoi(argv[3]);
        printf("Use %d threads add num from 0 to %ld\n", threadnum, maxnum);
        gettimeofday(&tv, 0);
        starttime= (long)tv.tv_sec * (long)1000000 + (long)tv.tv_usec;
        atomic64_set(&num, 0);
        pthread_attr_init(&thread_attr);
        //pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED);
        for (i = 0; i < threadnum; i++) {
                if (type == ATOMIC_TYPE)
                        pthread_create(&thread, &thread_attr, add_func, 0);
                else if (type == MUTEX_TYPE)
                        pthread_create(&thread, &thread_attr, add_func_mutex, 0);
                else if (type == RW_TYPE)
                        pthread_create(&thread, &thread_attr, add_func_rwlock, 0);
        }
        while(1)
                sleep(10);
}
```
运行结果这里就不详细列出来了，可以直接看这个曲线图。
![](http://blog.chinaunix.net/attachment/201404/16/20662820_13976175948T1t.png)
横坐标是线程数，纵坐标是运算1亿次耗费的微秒数。
从这个图中可以看出，对于频繁写操作的情况atomic > mutex > rwlock
对于同一种同步类型，并不是随着线程数的增加而一直增加，不过因为只有8个核心，所以没有测试更多线程的情况。
