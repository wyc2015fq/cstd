# Linux 进程管理之四大名捕 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [JamesXin](http://www.jobbole.com/members/JamesXin) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
**一、四大名捕**
四大名捕，最初出现于温瑞安创作的武侠小说，是朝廷中正义力量诸葛小花的四大徒弟，四人各怀绝技，分别是轻功暗器高手“无情”、内功卓越的高手“铁手”、腿功惊人的“追命”和剑法一流的“冷血”。
本文四大名捕由 linux 命令所出演：
无情：ps     出演
铁手：dstat  出演
追命：top    出演
冷血：htop   出演
**二、进程相关基础知识**
介绍四大名捕之前先介绍一下进程相关的基础知识，话不多说，看图。
![wKiom1b0HEmz_B-JAAEH4-y6Flw822.png](http://s3.51cto.com/wyfs02/M01/7D/FB/wKiom1b0HEmz_B-JAAEH4-y6Flw822.png)
（[查看大图](http://s3.51cto.com/wyfs02/M01/7D/FB/wKiom1b0HEmz_B-JAAEH4-y6Flw822.png)）
**三、轻功暗器高手“无情” [PS]**
ps：用于显示当前进程的状态（非动态）
ps [options]:
选项有三种风格：
1、UNIX风格，必须在选项前面加“-”
2、BSD风格，选项前不能加“-”
3、GNU风格，选项前为两个“-”
**常用组合之一：aux**
- a：所有与终端相关的进程
- x：所有与终端无关的进程
- u：以用户为中心组织进程状态信息显示
![wKiom1bz_fCDm5fcAACOPdnBEGM148.png](http://s2.51cto.com/wyfs02/M02/7D/FA/wKiom1bz_fCDm5fcAACOPdnBEGM148.png)
CPU%：cpu时间占用比率
MEM%：内存占用百分比
VSZ：virtual size虚拟内存集；
RSS：Resident Size，常驻内存集；
STAT：
- R：running 运行
- S：interruptable sleeping 可中断睡眠
- D：uninterruptable sleeping 不可中断睡眠
- T：Stopped 停止
- Z：zombie 僵死态
- +：前台进程
- l：多线程进程
- N：低优先级进程
- <：高优先级进程
- s：session leader  进程领导者
**常用组合之二：-ef**
- -e：显示所有进程
- -f：显示完整格式的进程信息
![wKiom1bz_1CQXTlqAACxHoJ0v6w854.png](http://s5.51cto.com/wyfs02/M01/7D/FA/wKiom1bz_1CQXTlqAACxHoJ0v6w854.png)
** 常用组合之三：-eFH**
-F：显示完整格式的进程信息；
- C： cpu utilization cpu占用百分比
- PSR：运行于哪颗CPU之上
-H：以层级结构显示进程的相关信息；
![wKiom1bz_42Tl3jjAACloBinZ60735.png](http://s5.51cto.com/wyfs02/M00/7D/FA/wKiom1bz_42Tl3jjAACloBinZ60735.png)
**常用组合之四：-eo, axo**
o  field1, field2,…：自定义要显示的字段列表，以逗号分隔
常用的field：pid, ni, priority, psr, pcpu, stat, comm, tty, ppid, rtprio
- pid：进程的pid号
- ni：nice值
- priority：优先级
- psr：运行在那颗cpu
- pcpu：cpu利用率
- ppid：父进程的id号
- rtprio：实时优先级
**四、内功卓越的高手“铁手”[dstat]**
> 
dstat：系统资源统计命令（动态）
dstat [-afv] [options..] [delay [count]]
![wKioL1b0BXyDh5J8AABZTKRRf7Q065.png](http://s2.51cto.com/wyfs02/M01/7D/F7/wKioL1b0BXyDh5J8AABZTKRRf7Q065.png)
常用选项：
- -c， –cpu：显示cpu相关信息；
- -C #,#,…,total：显示第一个cpu，第二个cpu或者总共的
- -d, –disk：显示磁盘的相关信息
- -D sda,sdb,…,tobal：显示指定硬盘设备，总空间
- -g：显示page相关的速率数据；
- -m：Memory的相关统计数据
- -n：Interface的相关统计数据；
- -p：显示process的相关统计数据；
- -r：显示io请求的相关的统计数据；
- -s：显示swapped的相关统计数据；
![wKioL1b0B-jj2vSgAABq2jc0wqw724.png](http://s5.51cto.com/wyfs02/M01/7D/F7/wKioL1b0B-jj2vSgAABq2jc0wqw724.png)
–tcp：显示tcp套接字
–udp：显示udp连接
–raw：显示裸套接字
–socket：套接字
–ipc：进程间通信信息
![wKioL1b0CDyA-MqNAAAfDT7iFjo365.png](http://s3.51cto.com/wyfs02/M02/7D/F7/wKioL1b0CDyA-MqNAAAfDT7iFjo365.png)
–top-cpu：显示最占用CPU的进程；
–top-io：最占用io的进程；
–top-mem：最占用内存的进程；
![wKiom1b0B_HBUD1SAACKboR7wwE308.png](http://s2.51cto.com/wyfs02/M00/7D/FA/wKiom1b0B_HBUD1SAACKboR7wwE308.png)
**五、腿功惊人的“追命”[top]**
> 
top：列出inux进程
top为动态显示进程
![wKioL1b0EZPiTlUMAACPN-loCGE523.png](http://s1.51cto.com/wyfs02/M00/7D/F7/wKioL1b0EZPiTlUMAACPN-loCGE523.png)
**top命令个参数具体含义：**
**top – 14:58:34 up  5:28,  1 user,  load average: 0.01, 0.02, 0.05**
- 14:58:34：当前时间
- up  5:28：运行时长
- 1 user：登录当前系统上的用户数
- load average: 0.01, 0.02, 0.05：平均负载（等待运行的队列长度的负载）
**Tasks: 353 total,   2 running, 351 sleeping,   0 stopped,   0 zombie**
- Tasks:任务
- 353 total：一共运行多少进程
- 2 running：几个处于运行
- 351 sleeping：多少个睡眠
- 0 stopped：多少个停止
- 0 zombie：多少个僵死
**%Cpu(s):  0.0 us,  0.7 sy,  0.0 ni, 99.3 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st**
- %Cpu：cpu占用百分比
- 0.0 us：用户空间占用的百分比
- 0.7 sy：内核空间占用时间的百分比
- 0.0 ni：对nice调整占用的内存百分比
- 99.3 id：空闲百分比
- 0.0 wa（wait）：等待IO完成所消耗的百分比
- 0.0 hi：处理硬件中断所占用的百分比
- 0.0 si：处理软件中断所占用的百分比
- 0.0 st：被偷走的百分比（虚拟化程序）
**KiB Mem :  1001332 total,   681052 free,   139844 used,   180436 buff/cache**
- KiB Mem：内存空间占用，以KB为单位：
- 1001332 total：总内存空间
- 681052 free：剩余内存空间
- 139844 used：已用内存空间
- 180436 buff/cache：用于缓存和缓冲的内存空间
**KiB Swap:  2098172 total,  2098172 free,        0 used.   698100 avail Mem **
- KiB Swap：swap空间占用，以KB为单位
- 2098172 total：总空间
- 2098172 free：剩余空间
- 0 used：已用空间
- 698100 avail Mem ：有效swap大小
** PID USER      PR  NI    VIRT    RES    SHR S %CPU %MEM     TIME+ COMMAND             **
**3077 root      20   0  146276   2256   1420 R  1.7  0.2   0:02.91 top  **
- PID:用户pid
- USER:用户名称
- PR:优先级
- NI:nice值
- VIRT:virtual size虚拟内存集
- RES:常驻内存集
- SHR:共享内存空间
- S:当前状态
- %CPU:占据CPU百分比
- %MEM:占据MEM百分比
- TIME+:运行时长
- COMMAND:命令
top内排序：
- P：以占据CPU百分比排序
- M：以占据内存百分比排序
- T：累积占用CPU时间排序
首部信息：
uptime信息：l命令
# 第一行没有显示
![wKiom1b0EzCxhK5vAACLCyJgSgY960.png](http://s1.51cto.com/wyfs02/M02/7D/FB/wKiom1b0EzCxhK5vAACLCyJgSgY960.png)
tasks及cpu信息：t命令
# 可以禁用显示硬盘及cpu相关消息
![wKioL1b0FHOhKiz6AACLNMrytRY331.png](http://s4.51cto.com/wyfs02/M00/7D/F7/wKioL1b0FHOhKiz6AACLNMrytRY331.png)
内存信息：m命令
# 可以将内存使用率用|||显示 或者白空格显示
![wKiom1b0E6WAVNZuAACJWkgmwyE381.png](http://s1.51cto.com/wyfs02/M00/7D/FB/wKiom1b0E6WAVNZuAACJWkgmwyE381.png)
退出命令：q
修改刷新时间间隔：s
![wKioL1b0FPvgOdHoAACUd8JOMfo790.png](http://s2.51cto.com/wyfs02/M01/7D/F7/wKioL1b0FPvgOdHoAACUd8JOMfo790.png)
终止指定的进程：k
![wKiom1b0FSaS1bqrAACXP-vrgBM154.png](http://s5.51cto.com/wyfs02/M00/7D/FB/wKiom1b0FSaS1bqrAACXP-vrgBM154.png)
选项：
- -d #：指定刷新时间间隔，默认为3秒；
- -b：以批次方式显示；
- -n #：显示多少批次；
**六、剑法一流“冷血”[htop]**
> 
htop:交互式进程查看器
htop [-dus]
![wKiom1b0Fymzsoz9AADt-sQHyz8347.png](http://s4.51cto.com/wyfs02/M00/7D/FB/wKiom1b0Fymzsoz9AADt-sQHyz8347.png)
# htop 是一个非常强大的工具，下面从F1到F10可以看到具体的参数信息。
# F1 ：帮助信息
![wKiom1b0F6zw60K9AACMkOrFuC0689.png](http://s2.51cto.com/wyfs02/M01/7D/FB/wKiom1b0F6zw60K9AACMkOrFuC0689.png)
选项：
- -d #：指定延迟时间间隔
- -u UserName：仅显示指定用户的进程
- -s COLUME：以指定字段进行排序
常用子命令：
- l：显示选定的进程打开的文件列表
- s：跟踪选定的进程的系统调用
- t：以层级关系显示各进程状态
- a：将选定的进程绑定至某指定的CPU核心
# 此处可以添加指定项到显示屏幕上面，显示方式可以是[Bar] [Text] [Graph] [LED]
![wKiom1b0GhSh4kTbAABwzLSgqKs830.png](http://s4.51cto.com/wyfs02/M01/7D/FB/wKiom1b0GhSh4kTbAABwzLSgqKs830.png)
进程管理相关命令就介绍到这了，如果有不同意见可以慷慨提出！
