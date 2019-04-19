# 高性能高并发网络库：StateThreads - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月01日 17:58:22[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：87
StateThreads是一个C的网络程序开发库，提供了编写高性能、高并发、高可读性的网络程序的开发库，轻量级网络应用框架
共也就3000行C代码
网络程序（Internet Application）（IA）
现有的架构
IAs(Internet Applications)有一些常见的被广泛使用的架构，包括基于进程的架构（Multi-Process）,基于线程的架构（Multi-Threaded）, 和事件驱动的状态机架构（Event-Driven State Machine）。
Event-Driven State Machine：EDSM，基于事件驱动的状态机架构
优点：
1. 能设计出高效的IA系统，包括很高的负载能力和系统能力。
2. 简化了编程和调试，因为没有同步锁，可以使用静态变量和不可重入函数。
协程(coroutine)顾名思义就是“协作的例程”（co-operative routines）
协程是在用户空间利用程序语言的语法语义就能实现逻辑上类似多任务的编程技巧。
ST的核心思想就是利用multi-threading的简单优雅范式胜过传统异步回调的复杂晦涩实现，又利用EDSM的性能和解耦架构避免了multi-threading在系统上的开销和暗礁。
开启10K和30K个线程的程序
#include <stdio.h>
/*
build and execute
gcc -I../obj -g huge_threads.c ../obj/libst.a -o huge_threads;
./huge_threads 10000
10K report:
10000 threads, running on 1 CPU 512M machine,
CPU 6%, MEM 8.2% (~42M = 42991K = 4.3K/thread)
30K report:
30000 threads, running on 1CPU 512M machine,
CPU 3%, MEM 24.3% (4.3K/thread)
*/
#include <st.h>
void* do_calc(void* arg){
int sleep_ms = (int)(long int)(char*)arg * 10;
for(;;){
printf("in sthread #%dms\n", sleep_ms);
st_usleep(sleep_ms * 1000);
}
return NULL;
}
int main(int argc, char** argv){
if(argc <= 1){
printf("Test the concurrence of state-threads!\n"
"Usage: %s <sthread_count>\n"
"eg. %s 10000\n", argv[0], argv[0]);
return -1;
}
if(st_init() < 0){
printf("error!");
return -1;
}
int i;
int count = atoi(argv[1]);
for(i = 1; i <= count; i++){
if(st_thread_create(do_calc, (void*)i, 0, 0) == NULL){
printf("error!");
return -1;
}
}
st_thread_exit(NULL);
return 0;
}
[https://coolshell.cn/articles/10975.html](https://coolshell.cn/articles/10975.html)
[https://coolshell.cn/articles/12012.html](https://coolshell.cn/articles/12012.html)
[https://blog.csdn.net/win_lin/article/details/8242653](https://blog.csdn.net/win_lin/article/details/8242653) 国外人推荐的，说明国外程序员能力不是一般强啊。
