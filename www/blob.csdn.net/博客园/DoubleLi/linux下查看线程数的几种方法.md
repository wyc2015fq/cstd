# linux下查看线程数的几种方法 - DoubleLi - 博客园






**1、** cat /proc/${pid}/status

**2、**pstree -p ${pid}

**3、**top -p ${pid} 再按H   或者直接输入 top -bH -d 3 -p  ${pid}

**top -H**
手册中说：-H : Threads toggle
加上这个选项启动top，top一行显示一个线程。否则，它一行显示一个进程。

**4、ps xH**
手册中说：H Show threads as if they were processes
这样可以查看所有存在的线程。

**5、ps -mp <PID>**
手册中说：m Show threads after processes
这样可以查看一个进程起的线程数。









