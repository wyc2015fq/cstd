# [CareerCup] 16.1 Thread and Process 线程和进程 - Grandyang - 博客园







# [[CareerCup] 16.1 Thread and Process 线程和进程](https://www.cnblogs.com/grandyang/p/5385421.html)







16.1 What's the difference between a thread and a process?



进程Process是程序执行时的一个实例。一个进程是被分配系统资源的独立单元，每个进程在独立的地址空间上执行，如果需要使用其他进程的资源，需要使用进程间通讯，包括管道Pipes，文件Files，套接字Sockets，或者其他形式。

线程Thread存在于进程之中并分享进程的资源(包括堆空间)。同一个进程中的多个线程分享同一个堆地址。这是和进程区别很大的地方，进程之间不能直接访问内存。每个线程仍然有自己的栈，但是其他线程可以读写堆内存。

一个线程是一个进程的特殊执行路径，当一个线程修改了进程的资源，其他线程能立马察觉到改变。



[CareerCup All in One 题目汇总](http://www.cnblogs.com/grandyang/p/5162994.html)












