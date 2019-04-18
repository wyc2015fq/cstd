# 工作中常用的命令netstat、lsof、ps和top - Orisun - 博客园







# [工作中常用的命令netstat、lsof、ps和top](https://www.cnblogs.com/zhangchaoyang/articles/3751267.html)





netstat命令选项：

-a all 默认不显示LISTEN相关

-t tcp

-u udp

-n number 不显示别名，能显示数字的都用数字显示。由于不需要把数字转换成别名，所以显示的快一些

-l listen 仅列出正在监听的服务状态

-p program 显示建立连接的程序名

-r route 路由信息

-e extention 扩展信息，比如uid等



-s statistic 按各个协议进行统计

-c cycle 每隔一段时间周期执行netstat命令

实战应用1：公司内部的一个老服务运行在192.168.1.1:50060上，服务将于一周之后停用，再在要查一下本机上有没有进行在调用该服务。

使用命令netstat -pt可以输出PID及程序名：

Active Internet connections (w/o servers)
Proto Recv-Q Send-Q Local Address               Foreign Address             State       PID/Program name 
tcp        0      0 localhostname:16061192.168.1.1:50060       ESTABLISHED 22000/java

是本机的16061端口在调192.168.1.1:50060上的服务，且本机16061端口上跑的是一个java程序，进程ID是22000



 lsof可以查看系统打开的文件，这里的“文件”包括/proc文件、磁盘文件、网络IO等。

lsof命令常用选项：
lsof  filename 显示打开指定文件的所有进程
lsof -a 表示两个参数都必须满足时才显示结果
lsof -c string   显示COMMAND列中包含指定字符的进程所有打开的文件
lsof -u username  显示所属user进程打开的文件
lsof -g gid 显示归属gid的进程情况
lsof +d /DIR/ 显示目录下被进程打开的文件
lsof +D /DIR/ 同上，但是会搜索目录下的所有目录，时间相对较长
lsof -d FD 显示指定文件描述符的进程
lsof -n 不将IP转换为hostname，缺省是不加上-n参数
lsof -i 用以显示符合条件的进程情况lsof -i[46] [protocol][@hostname|hostaddr][:service|port]
            46 --> IPv4 or IPv6
            protocol --> TCP or UDP
            hostname --> Internet host name
            hostaddr --> IPv4地址
            service --> /etc/service中的 service name (可以不只一个)
            port --> 端口号 (可以不只一个)小例子：查看所属root用户进程所打开的文件类型为txt的文件: 
# lsof -a -u root -d txt

还实战应用1的例子，我们可以用lsof命令得到结果。

lsof -i:@192.168.1.1:50060，输出：

COMMAND PID USER FD TYPE DEVICE SIZE/OFF NODE NAME
java    22000 root  106u  IPv4 1592198833      0t0  TCP localhostname:16061->192.168.1.1:50060 (ESTABLISHED)

FD表示文件描述符，应用程序通过文件描述符来识别文件，文件描述符后有u表示程序是以读写的权限打开文件的（r只读w只写）。



ps命令列出进程状态。

在实战应用1中，我们想查看PID为22000的程序到底是哪个程序可以用：

ps -ef | grep 22000

输出：

UID        PID  PPID  C STIME TTY          TIME CMD

在CMD列你可以看到程序名

或者用ps aux | grep 22000

输出：

USER       PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND

aux比-ef输出的信息更多，比如%CPU和%MEM。



top命令用于实时查看服务器性能

# top

top - 16:31:10 up 493 days, 2:18, 1 user, load average: 3.79, 5.07, 5.00
Tasks: 134 total, 1 running, 133 sleeping, 0 stopped, 0 zombie
Cpu(s): 8.9%us, 0.7%sy, 0.0%ni, 89.9%id, 0.2%wa, 0.1%hi, 0.2%si, 0.0%st
Mem: 32949016k total, 32857500k used, 91516k free, 181172k buffers
Swap: 32764556k total, 759976k used, 32004580k free, 18778656k cached

PID USER PR NI VIRT RES SHR S %CPU %MEM TIME+ COMMAND 
21902 root 25 0 7732m 6.1g 11m S 73.2 19.4 3257:11 java 
22616 root 21 0 3184m 2.0g 12m S 1.7 6.3 45:44.93 java 
20028 root 22 0 3493m 1.7g 12m S 1.3 5.4 95:23.61 java 
21784 root 15 0 12756 1104 816 R 0.3 0.0 0:00.05 top 
24526 root 20 0 8408m 160m 9524 S 0.3 0.5 7:54.50 java 
1 root 15 0 10364 572 536 S 0.0 0.0 7:53.62 init


解释一下，第一行：16：31：10是当时时间，up 493 days是系统已经运行了这么长时间，1 user当前只有1个用户登录了系统，load average后面是1分钟、5分钟、15分钟系统的负载情况。

第二行：当前有134个进行在运行，133个在休眠，0个已停止，0个僵尸。

第三行：8.9%us是用户进行占用cpu的量，0.7%sy是内核进程占用cpu的量。

进行详细信息。PR是优先级，NI是nice值，真正的优先级实际上是PR+NI。

top命令的常用参数：

-d指定每两次屏幕信息刷新之间的时间间隔。
-p  通过指定监控进程ID来仅仅监控某个进程的状态。














