# linux ps命令，查看某进程cpu和内存占用率情况， linux ps命令，查看进程cpu和内存占用率排序。  不指定 - DoubleLi - 博客园






背景：有时需要单看某个进程的CPU及占用情况，有时需要看整体进程的一个占用情况。
一、 linux ps命令，查看某进程cpu和内存占用率情况
[root@test vhost]# ps aux
USER       PID  %CPU    %MEM    VSZ   RSS TTY      STAT    START   TIME COMMAND
解释：
linux 下的ps命令
USER 进程运行用户
PID    进程编号
%CPU 进程的cpu占用率
%MEM 进程的内存占用率
VSZ 进程所使用的虚存的大小
RSS 进程使用的驻留集大小或者是实际内存的大小
TTY 与进程关联的终端（tty）
STAT 检查的状态：进程状态使用字符表示的，如R（running正在运行或准备运行）、S（sleeping睡眠）、I（idle空闲）、Z (僵死)、D（不可中断的睡眠，通常是I/O）、P（等待交换页）、W（换出,表示当前页面不在内存）、N（低优先级任务）T(terminate终止)、W has no resident pages
START （进程启动时间和日期）
TIME ;（进程使用的总cpu时间）
COMMAND （正在执行的命令行命令）
NI (nice)优先级
PRI 进程优先级编号
PPID 父进程的进程ID（parent process id）
SID 会话ID（session id）
WCHAN 进程正在睡眠的内核函数名称；该函数的名称是从/root/system.map文件中获得的。
FLAGS 与进程相关的数字标识
————————————由上面列的输出后于是可以通过awk进行对特定列进行输出————————————



[view plain](http://www.justwinit.cn/)[print](http://www.justwinit.cn/)[?](http://www.justwinit.cn/)

- ps aux |grep HttpPrograme| awk '{print($1" "$3" "$4" "$11);}'   



查看用户的一个进程运行情况参数o:

- ps -o user,%cpu,%mem,command  
- -o,o user-defined  -f full            --Group --User --pid --cols --ppid  



摘自：http://www.commandlinefu.com/commands/view/7244/micro-ps-aux-by-memcpu




二、 linux ps命令，查看进程cpu和内存占用率排序
使用以下命令查看：
ps -aux | sort -k4,4n
ps auxw --sort=rss
ps auxw --sort=%cpu

摘录自：http://blog.chinaunix.net/uid-21389973-id-3292358.html









