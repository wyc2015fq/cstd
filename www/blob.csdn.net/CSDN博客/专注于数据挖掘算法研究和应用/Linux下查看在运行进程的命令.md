# Linux下查看在运行进程的命令 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年05月26日 10:59:59[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2392








ps命令


输入下面的ps命令，显示所有运行中的进程：
# ps aux | less

按q退出；


其中，


-A：显示所有进程


a：显示终端中包括其它用户的所有进程


x：显示无控制终端的进程

任务：查看系统中的每个进程。
# ps -A
# ps -e
任务：查看非root运行的进程
# ps -U root -u root -N
任务：查看用户vivek运行的进程
# ps -u vivek
任务：top命令


top命令提供了运行中系统的动态实时视图。



