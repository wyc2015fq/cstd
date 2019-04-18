# linux 进程几个宏定义 - 深之JohnChen的专栏 - CSDN博客

2007年09月28日 18:02:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2227


WIFEXITED:判断子进程退出值是非0

WEXITSTATUS:判断子进程的退出值(当子进程退出时非0).

WIFSIGNALED:子进程由于有没有获得的信号而退出.

WTERMSIG:子进程没有获得的信号号(在WIFSIGNALED为真时才有意义).

WIFSTOPPED：子进程终止

WSTOPSIG:子进程终止状态值

WCOREDUMP：调试coredump文件信息

