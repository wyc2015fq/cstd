# strace跟踪线程调用 - DoubleLi - 博客园






方法一:
strace -fp pid , 可以跟踪所有线程， 进程的系统调用。


- [root@xxxx]strace -p 24091
- Process xxx attached - interrupt to quit
- [ Process PID=24091 runs in 32 bit mode. ]
- futex(0xc5bbbd8, FUTEX_WAIT, 24215 NULL



复制代码


你的问题不是strace不能跟踪，而是你的线程锁住了。 futex应该是内核里对pthread_mutex_lock实现的一个函数。

方法二(本质同方法一，但更明了)：
pstree -p pid 可以查看多线程程序的进程树。
在针对各个线程，逐个的strace -p pid

最后
man pstack
pstack - print a stack trace of a running process
相当于gdb内的bt，打出栈信息。









