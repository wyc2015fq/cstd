# Linux ps命令详解与示例说明 - xqhrs232的专栏 - CSDN博客
2017年06月02日 14:25:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：177
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/lsbhjshyn/article/details/18549869](http://blog.csdn.net/lsbhjshyn/article/details/18549869)
相关文章
1、[ps命令详解](http://www.cnblogs.com/wangkangluo1/archive/2011/09/23/2185938.html)----[http://www.cnblogs.com/wangkangluo1/archive/2011/09/23/2185938.html](http://www.cnblogs.com/wangkangluo1/archive/2011/09/23/2185938.html)
**ps**：要对进程进行监测和控制,首先必须要了解当前进程的情况,也就是需要查看当前进程,而ps命令就是最基本同时也是非常强大的进程查看命令.使用该命令可以确定有哪些进程正在运行和运行的状态、进程是否结束、进程有没有僵尸、哪些进程占用了过多的资源等等.总之大部分信息都是可以通过执行该命令得到的.
**ps**命令最常用的还是用于监控后台进程的工作情况,因为后台进程是不和屏幕键盘这些标准输入/输出设备进行通信的,所以如果需要检测其情况,便可以使用ps命令了.
注意：ps是显示瞬间进程的状态，并不动态连续；如果想对进程进行实时监控应该用top命令。
[root@lsbhjshyn ~]# man ps
![](https://img-blog.csdn.net/20140120112200421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNiaGpzaHlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
通过man ps可以获得ps的详细参数用法。下面可以通过示例看看效果：
[root@lsbhjshyn ~]# ps aux
[root@ lsbhjshyn ~]# ps -lA
[root@ lsbhjshyn ~]# ps axjf
参数：
-A ：所有的进程均显示出来，与 -e 具有同样的效用；
-a ： 显示现行终端机下的所有进程，包括其他用户的进程；
-u ：以用户为主的进程状态 ；
x ：通常与 a 这个参数一起使用，可列出较完整信息。
输出格式规划：
l ：较长、较详细的将该 PID 的的信息列出；
j ：工作的格式 (jobs format)
-f ：做一个更为完整的输出。
特别说明：
由于 ps 能够支持的系统类型相当的多，所以他的参数多的离谱！
而且有没有加上 - 差很多！详细的用法应该要参考 man ps 喔！
例1：将目前属于您自己这次登入的 PID 与相关信息列示出来
![](https://img-blog.csdn.net/20140120112259140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNiaGpzaHlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 各相关信息的意义为：
# F 代表这个程序的旗标 (flag)， 4 代表使用者为 superuser；
# S 代表这个程序的状态 (STAT)；
#UID 代表执行者身份
# PID 进程的ID号！底下的 PPID 则父进程的ID；
# C CPU 使用的资源百分比
# PRI指进程的执行优先权(Priority的简写)，其值越小越早被执行；
# NI 这个进程的nice值，其表示进程可被执行的优先级的修正数值。
# ADDR 这个是内核函数，指出该程序在内存的那个部分。如果是个执行
# 的程序，一般就是『 - 』
# SZ 使用掉的内存大小；
# WCHAN 目前这个程序是否正在运作当中，若为 - 表示正在运作；
# TTY 登入者的终端机位置啰；
# TIME 使用掉的 CPU 时间。
# CMD 所下达的指令名称
例2：列出目前所有的正在内存当中的程序
![](https://img-blog.csdn.net/20140120112322156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNiaGpzaHlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
• USER：该进程属于那个使用者账号的？
• PID ：该进程的进程ID号。
• %CPU：该进程使用掉的 CPU 资源百分比；
• %MEM：该进程所占用的物理内存百分比；
• VSZ ：该进程使用掉的虚拟内存量 (Kbytes)
• RSS ：该进程占用的固定的内存量 (Kbytes)
• TTY ：该进程是在那个终端机上面运作，若与终端机无关，则显示 ?，另外， tty1-tty6 是本机上面的登入者程序，若为 pts/0 等等的，则表示为由网络连接进主机的程序。
• STAT：该程序目前的状态，主要的状态有：
   R ：该程序目前正在运作，或者是可被运作；
   S ：该程序目前正在睡眠当中 (可说是 idle 状态啦！)，但可被某些讯号(signal) 唤醒。
   T ：该程序目前正在侦测或者是停止了；
   Z ：该程序应该已经终止，但是其父程序却无法正常的终止他，造成 zombie (疆尸) 程序的状态
• START：该进程被触发启动的时间；
• TIME ：该进程实际使用 CPU 运作的时间。
• COMMAND：该程序的实际指令为什么？
例3：以例一的显示内容，显示出所有的程序
![](https://img-blog.csdn.net/20140120112426156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNiaGpzaHlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
例4：列出类似程序树的程序显示
![](https://img-blog.csdn.net/20140120112441343?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNiaGpzaHlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
例5：找出与 cron 与 syslog 这两个服务有关的 PID 号码
![](https://img-blog.csdn.net/20140120112504281?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbHNiaGpzaHlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
