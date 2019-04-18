# 网络IO解决方案 — 协程框架的实现 - qq_42302962的博客 - CSDN博客
2019年04月10日 16:52:01[Linux高级开发](https://me.csdn.net/qq_42302962)阅读数：11
协程这个概念很久了，好多程序员是实现过这个组件的，网上关于协程的文章，博客，论坛都是汗牛充栋，在知乎，github上面也有很多大牛写了关于协程的心得体会。突发奇想，我也来实现一个这样的组件，并测试了一下性能。借鉴了很多大牛的思想，阅读了很多大牛的代码。于是把整个思考过程写下来。实现代码 [https://github.com/wangbojing/NtyCo](https://github.com/wangbojing/NtyCo)
代码简单易读，如果在你的项目中，NtyCo能够为你解决些许工程问题，那就荣幸之至。
下面将部分的NtyCo的代码贴出来。
NtyCo 支持多核多进程。
```
int process_bind(void) {
	int num = sysconf(_SC_NPROCESSORS_CONF);
	pid_t self_id = syscall(__NR_gettid);
	printf("selfid --> %d\n", self_id);
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(self_id % num, &mask);
	sched_setaffinity(0, sizeof(mask), &mask);
	mulcore_entry(9096 + (self_id % num) * 10);
}
```
**NtyCo 上下文切换**
首先来回顾一下x86_64寄存器的相关知识。x86_64 的寄存器有16个64位寄存器，分别是：%rax, %rbx, %rcx, %esi, %edi, %rbp, %rsp, %r8, %r9, %r10, %r11, %r12,
%r13, %r14, %r15。
%rax 作为函数返回值使用的。
%rsp 栈指针寄存器，指向栈顶
%rdi, %rsi, %rdx, %rcx, %r8, %r9 用作函数参数，依次对应第1参数，第2参数。。。
%rbx, %rbp, %r12, %r13, %r14, %r15 用作数据存储，遵循调用者使用规则，换句话说，就是随便用。调用子函数之前要备份它，以防它被修改
%r10, %r11 用作数据存储，就是使用前要先保存原值。
上下文切换，就是将CPU的寄存器暂时保存，再将即将运行的协程的上下文寄存器，分别mov到相对应的寄存器上。此时上下文完成切换。如下图所示：
![image.png](https://s1.51cto.com/images/20180803/1533294301880430.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
**代码如下**
```
__asm__ (
"    .text                                  \n"
"       .p2align 4,,15                                   \n"
".globl _switch                                          \n"
".globl __switch                                         \n"
"_switch:                                                \n"
"__switch:                                               \n"
"       movq %rsp, 0(%rsi)      # save stack_pointer     \n"
"       movq %rbp, 8(%rsi)      # save frame_pointer     \n"
"       movq (%rsp), %rax       # save insn_pointer      \n"
"       movq %rax, 16(%rsi)                              \n"
"       movq %rbx, 24(%rsi)     # save rbx,r12-r15       \n"
"       movq %r12, 32(%rsi)                              \n"
"       movq %r13, 40(%rsi)                              \n"
"       movq %r14, 48(%rsi)                              \n"
"       movq %r15, 56(%rsi)                              \n"
"       movq 56(%rdi), %r15                              \n"
"       movq 48(%rdi), %r14                              \n"
"       movq 40(%rdi), %r13     # restore rbx,r12-r15    \n"
"       movq 32(%rdi), %r12                              \n"
"       movq 24(%rdi), %rbx                              \n"
"       movq 8(%rdi), %rbp      # restore frame_pointer  \n"
"       movq 0(%rdi), %rsp      # restore stack_pointer  \n"
"       movq 16(%rdi), %rax     # restore insn_pointer   \n"
"       movq %rax, (%rsp)                                \n"
"       ret                                              \n"
);
```
**协程的调度器**
调度器的实现，有两种方案，一种是生产者消费者模式，另一种多状态运行。
![image.png](https://s1.51cto.com/images/20180803/1533294375525147.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
逻辑代码如下：
```
while (1) {
 
        //遍历睡眠集合，将满足条件的加入到ready
        nty_coroutine *expired = NULL;
        while ((expired = sleep_tree_expired(sched)) != ) {
            TAILQ_ADD(&sched->ready, expired);
        }
 
        //遍历等待集合，将满足添加的加入到ready
        nty_coroutine *wait = NULL;
        int nready = epoll_wait(sched->epfd, events, EVENT_MAX, 1);
        for (i = 0;i < nready;i ++) {
            wait = wait_tree_search(events[i].data.fd);
            TAILQ_ADD(&sched->ready, wait);
        }
 
        // 使用resume回复ready的协程运行权
        while (!TAILQ_EMPTY(&sched->ready)) {
            nty_coroutine *ready = TAILQ_POP(sched->ready);
            resume(ready);
        }
    }
```
多状态运行
![image.png](https://s1.51cto.com/images/20180803/1533294504189047.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
实现逻辑代码如下：
```
while (1) {
 
        //遍历睡眠集合，使用resume恢复expired的协程运行权
        nty_coroutine *expired = NULL;
        while ((expired = sleep_tree_expired(sched)) != ) {
            resume(expired);
        }
 
        //遍历等待集合，使用resume恢复wait的协程运行权
        nty_coroutine *wait = NULL;
        int nready = epoll_wait(sched->epfd, events, EVENT_MAX, 1);
        for (i = 0;i < nready;i ++) {
            wait = wait_tree_search(events[i].data.fd);
            resume(wait);
        }
 
        // 使用resume恢复ready的协程运行权
        while (!TAILQ_EMPTY(sched->ready)) {
            nty_coroutine *ready = TAILQ_POP(sched->ready);
            resume(ready);
        }
```
性能测试
测试环境：4台VMWare 虚拟机
1台服务器 6G内存，4核CPU
3台客户端 2G内存，2核CPU
操作系统：ubuntu 14.04
服务器端测试代码：[https://github.com/wangbojing/NtyCo](https://github.com/wangbojing/NtyCo)
客户端测试代码：[https://github.com/wangbojing/c1000k_test/blob/master/client_mutlport_epoll.c](https://github.com/wangbojing/c1000k_test/blob/master/client_mutlport_epoll.c)
按照每一个连接启动一个协程来测试。**协程启动数量能够达70W无异常。**
![image.png](https://s1.51cto.com/images/20180803/1533294608177009.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![image.png](https://s1.51cto.com/images/20180803/1533294589729711.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
**技术交流加微信：xuelin477**
