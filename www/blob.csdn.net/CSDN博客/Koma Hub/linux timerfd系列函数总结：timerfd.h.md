# linux timerfd系列函数总结：timerfd.h - Koma Hub - CSDN博客
2019年04月10日 21:31:48[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：7
个人分类：[Glibc																[Linux](https://blog.csdn.net/Rong_Toa/article/category/7156254)](https://blog.csdn.net/Rong_Toa/article/category/8605571)
**Table of Contents**
[一、timerfd系列函数](#%E4%B8%80%E3%80%81timerfd%E7%B3%BB%E5%88%97%E5%87%BD%E6%95%B0)
[（1）timerfd_create()函数](#%EF%BC%881%EF%BC%89timerfd_create()%E5%87%BD%E6%95%B0)
[（2）timerfd_settime()函数](#%EF%BC%882%EF%BC%89timerfd_settime()%E5%87%BD%E6%95%B0)
[（3）timerfd_gettime()函数](#%EF%BC%883%EF%BC%89timerfd_gettime()%E5%87%BD%E6%95%B0)
[（4）下面贴出一个timerfd配合epoll函数的简单例子](#%EF%BC%884%EF%BC%89%E4%B8%8B%E9%9D%A2%E8%B4%B4%E5%87%BA%E4%B8%80%E4%B8%AAtimerfd%E9%85%8D%E5%90%88epoll%E5%87%BD%E6%95%B0%E7%9A%84%E7%AE%80%E5%8D%95%E4%BE%8B%E5%AD%90)
[参考资料：](#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99%EF%BC%9A)
网上关于timerfd的文章很多，在这儿归纳总结一下方便以后使用，顺便贴出一个timerfd配合epoll使用的简单例子
## 一、timerfd系列函数
　　timerfd是Linux为用户程序提供的一个定时器接口。这个接口基于文件描述符，通过文件描述符的可读事件进行超时通知，因此可以配合select/poll/epoll等使用。
下面对timerfd系列函数先做一个简单的介绍：
### **（1）timerfd_create()函数**
```cpp
#include <sys/timerfd.h>
int timerfd_create(int clockid, int flags);
/* 
timerfd_create（）函数创建一个定时器对象，同时返回一个与之关联的文件描述符。
clockid：clockid标识指定的时钟计数器，可选值（CLOCK_REALTIME、CLOCK_MONOTONIC。。。）
CLOCK_REALTIME:系统实时时间,随系统实时时间改变而改变,即从UTC1970-1-1 0:0:0开始计时,中间时刻如果系统时间被用户改成其他,则对应的时间相应改变
CLOCK_MONOTONIC:从系统启动这一刻起开始计时,不受系统时间被用户改变的影响
flags：参数flags（TFD_NONBLOCK(非阻塞模式)/TFD_CLOEXEC（表示当程序执行exec函数时本fd将被系统自动关闭,表示不传递）
*/
```
### **（2）timerfd_settime()函数**
```cpp
#include <sys/timerfd.h>
struct timespec {
    time_t tv_sec;                /* Seconds */
    long   tv_nsec;               /* Nanoseconds */
};
struct itimerspec {
    struct timespec it_interval;  /* Interval for periodic timer （定时间隔周期）*/
    struct timespec it_value;     /* Initial expiration (第一次超时时间)*/
};
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
/*
    timerfd_settime()此函数用于设置新的超时时间，并开始计时,能够启动和停止定时器;
    fd: 参数fd是timerfd_create函数返回的文件句柄
    flags：参数flags为1代表设置的是绝对时间（TFD_TIMER_ABSTIME 表示绝对定时器）；为0代表相对时间。
    new_value: 参数new_value指定定时器的超时时间以及超时间隔时间
    old_value: 如果old_value不为NULL, old_vlaue返回之前定时器设置的超时时间，具体参考timerfd_gettime()函数
    
    ** it_interval不为0则表示是周期性定时器。
       it_value和it_interval都为0表示停止定时器
*/
```
### **（3）timerfd_gettime()函数**
```cpp
int timerfd_gettime(int fd, struct itimerspec *curr_value);
/*
    timerfd_gettime()函数获取距离下次超时剩余的时间
    curr_value.it_value 字段表示距离下次超时的时间，如果改值为0，表示计时器已经解除
    改字段表示的值永远是一个相对值，无论TFD_TIMER_ABSTIME是否被设置
    curr_value.it_interval 定时器间隔时间
*/
uint64_t exp = 0;
read(fd, &exp, sizeof(uint64_t)); 
//可以用read函数读取计时器的超时次数，改值是一个8字节无符号的长整型
```
### **（4）下面贴出一个timerfd配合epoll函数的简单例子**
```cpp
/********************************************************
* Filename: timerfd.c
* Author: zhangwj
* Desprition: a sample program of timerfd
* Date: 2017-04-17
* Warnning:
********************************************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/timerfd.h>
#if 0
struct timespec {
    time_t tv_sec;                /* Seconds */
    long   tv_nsec;               /* Nanoseconds */
};
struct itimerspec {
    struct timespec it_interval;  /* Interval for periodic timer */
    struct timespec it_value;     /* Initial expiration */
};
#endif
#define EPOLL_LISTEN_CNT        256
#define EPOLL_LISTEN_TIMEOUT    500
#define LOG_DEBUG_ON 1
#ifdef LOG_DEBUG_ON 
#define LOG_DEBUG(fmt, args...) \
    do {  \
        printf("[DEBUG]:");\
        printf(fmt "\n", ##args); \
    } while(0);
#define LOG_INFO(fmt, args...) \
    do { \
        printf("[INFO]:");\
        printf(fmt "\n", ##args); \
    } while(0);
#define LOG_WARNING(fmt, args...) \
    do { \
        printf("[WARNING]:");\
        printf(fmt "\n", ##args); \
    } while(0);
#else
#define LOG_DEBUG(fmt, args...) 
#define LOG_INFO(fmt, args...) 
#define LOG_WARNING(fmt, args...) 
#endif
#define LOG_ERROR(fmt, args...) \
    do{ \
        printf("[ERROR]:");\
        printf(fmt "\n", ##args);\
    }while(0);
#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)
static int g_epollfd = -1;
static int g_timerfd = -1;
uint64_t tot_exp = 0;
static void help(void)
{
    exit(0);
}
static void print_elapsed_time(void)
{
    static struct timespec start;
    struct timespec curr;
    static int first_call = 1;
    int secs, nsecs;
    
    if (first_call) {
        first_call = 0;
        if (clock_gettime(CLOCK_MONOTONIC, &start) == -1) 
            handle_error("clock_gettime");
    }   
    
    if (clock_gettime(CLOCK_MONOTONIC, &curr) == -1) 
        handle_error("clock_gettime");
    
    secs = curr.tv_sec - start.tv_sec;
    nsecs = curr.tv_nsec - start.tv_nsec;
    if (nsecs < 0) {
        secs--;
        nsecs += 1000000000;
    }   
    printf("%d.%03d: ", secs, (nsecs + 500000) / 1000000);
}
void timerfd_handler(int fd)
{
    uint64_t exp = 0;
    
    read(fd, &exp, sizeof(uint64_t)); 
    tot_exp += exp;
    print_elapsed_time();
    printf("read: %llu, total: %llu\n", (unsigned long long)exp, (unsigned long long)tot_exp);
    return;
}
void epoll_event_handle(void)
{
    int i = 0;
    int fd_cnt = 0;
    int sfd;
    struct epoll_event events[EPOLL_LISTEN_CNT];    
    memset(events, 0, sizeof(events));
    while(1) 
    {   
        /* wait epoll event */
        fd_cnt = epoll_wait(g_epollfd, events, EPOLL_LISTEN_CNT, EPOLL_LISTEN_TIMEOUT); 
        for(i = 0; i < fd_cnt; i++) 
        {   
            sfd = events[i].data.fd;
            if(events[i].events & EPOLLIN) 
            {   
                if (sfd == g_timerfd) 
                {
                    timerfd_handler(sfd); 
                }   
            }   
        } 
    }   
}
int epoll_add_fd(int fd)
{
    int ret;
    struct epoll_event event;
    memset(&event, 0, sizeof(event));
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    ret = epoll_ctl(g_epollfd, EPOLL_CTL_ADD, fd, &event);
    if(ret < 0) {
        LOG_ERROR("epoll_ctl Add fd:%d error, Error:[%d:%s]", fd, errno, strerror(errno));
        return -1;
    }
    LOG_DEBUG("epoll add fd:%d--->%d success", fd, g_epollfd);
    return 0;    
}
int epollfd_init()
{
    int epfd;
    /* create epoll fd */
    epfd = epoll_create(EPOLL_LISTEN_CNT); 
    if (epfd < 0) {
        LOG_ERROR("epoll_create error, Error:[%d:%s]", errno, strerror(errno));
        return -1;
    }
    g_epollfd = epfd;
    LOG_DEBUG("epoll fd:%d create success", epfd);
    return epfd;
}
int timerfd_init()
{
    int tmfd;
    int ret;
    struct itimerspec new_value;
    new_value.it_value.tv_sec = 2;
    new_value.it_value.tv_nsec = 0;
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_nsec = 0;
    
    tmfd = timerfd_create(CLOCK_MONOTONIC, 0);
    if (tmfd < 0) {
        LOG_ERROR("timerfd_create error, Error:[%d:%s]", errno, strerror(errno));
        return -1;
    }
    ret = timerfd_settime(tmfd, 0, &new_value, NULL);
    if (ret < 0) {
        LOG_ERROR("timerfd_settime error, Error:[%d:%s]", errno, strerror(errno));
        close(tmfd);
        return -1;
    }
    if (epoll_add_fd(tmfd)) {
        close(tmfd);
        return -1;
    }
    g_timerfd = tmfd;
    return 0;
}
int main(int argc, char **argv)
{
    if (epollfd_init() < 0) {
        return -1;
    }
    if (timerfd_init()) {
        return -1;
    }
    /* event handle */
    epoll_event_handle();
    return 0;
}
```
## 参考资料：
[https://www.cnblogs.com/wenqiang/p/6698371.html](https://www.cnblogs.com/wenqiang/p/6698371.html)
[http://www.man7.org/linux/man-pages/man2/timerfd_create.2.html](http://www.man7.org/linux/man-pages/man2/timerfd_create.2.html)
[http://blog.csdn.net/walkingman321/article/details/6162055](http://blog.csdn.net/walkingman321/article/details/6162055)

