# Linux查看CPU和内存使用情况 - z69183787的专栏 - CSDN博客
2016年11月25日 10:35:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：22635
CPU占用率查看命令：top
图中红色部分即为占用百分率，下面是每个进程的CPU占用率，如果服务器是多核CPU可能在下方看到有些进程CPU占用超过100%，这种一般是该进程使用了多核。
![QQ图片20141023171421.jpg](http://bbs.jiasule.com/data/attachment/forum/201411/07/150526gb0a6abbsg8qyh5p.jpg)
内存占用率：free -m
图中红色部分即为占用，即used、free即为剩余。（注意中在第二行数字中找哦。）
![QQ图片20141023171421.jpg](http://bbs.jiasule.com/data/attachment/forum/201411/07/150733j7slpk44x58ss5l4.jpg)
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
**1.top**
使用权限：所有使用者
使用方式：top [-] [d delay] [q] [c] [S] [s] [i] [n] [b]
说明：即时显示process的动态
d :改变显示的更新速度，或是在交谈式指令列( interactive command)按s
q :没有任何延迟的显示速度，如果使用者是有superuser的权限，则top将会以最高的优先序执行
c :切换显示模式，共有两种模式，一是只显示执行档的名称，另一种是显示完整的路径与名称S :累积模式，会将己完成或消失的子行程( dead child process )的CPU time累积起来
s :安全模式，将交谈式指令取消,避免潜在的危机
i :不显示任何闲置(idle)或无用(zombie)的行程
n :更新的次数，完成后将会退出top
b :批次档模式，搭配"n"参数一起使用，可以用来将top的结果输出到档案内
范例：
显示更新十次后退出;
top -n 10
使用者将不能利用交谈式指令来对行程下命令:
top -s
将更新显示二次的结果输入到名称为top.log的档案里:
top -n 2 -b < top.log   
另附一个命令简介[**linux**](http://www.51testing.com/?uid-139579-action-viewspace-itemid-142106) traceroutewindows tracert两个命令相当，跟踪网络路由
**2.vmstat**
正如我们之前讨论的任何系统的性能比较都是基于基线的，并且监控CPU的性能就是以上3点，运行队列、CPU使用率和上下文切换。以下是一些对于CPU很普遍的性能要求：
1.对于每一个CPU来说运行队列不要超过3，例如，如果是双核CPU就不要超过6；
2.如果CPU在满负荷运行，应该符合下列分布，
a) User Time：65%～70%
b) System Time：30%～35%
c) Idle：0%～5%
**3. mpstat**
对于上下文切换要结合CPU使用率来看，如果CPU使用满足上述分布，大量的上下文切换也是可以接受的。
常用的监视工具有：vmstat, top,dstat和mpstat.
# vmstat 1
procs -----------memory---------- ---swap-- -----io---- --system-- ----cpu----
r b swpd free buff cache si so bi bo in cs us sy id wa
0 0 104300 16800 95328 72200 0 0 5 26 7 14 4 1 95 0
0 0 104300 16800 95328 72200 0 0 0 24 1021 64 1 1 98 0
0 0 104300 16800 95328 72200 0 0 0 0 1009 59 1 1 98 0
r表示运行队列的大小，
b表示由于IO等待而block的线程数量，
in表示中断的数量，
cs表示上下文切换的数量，
us表示用户CPU时间，
sys表示系统CPU时间，
wa表示由于IO等待而是CPU处于idle状态的时间，
id表示CPU处于idle状态的总时间。
dstat可以给出每一个设备产生的中断数：
# dstat -cip 1
----total-cpu-usage---- ----interrupts--- ---procs---
usr sys idl wai hiq siq| 15 169 185 |run blk new
6 1 91 2 0 0| 12 0 13  | 0 0 0
1 0 99 0 0 0| 0    0  6 | 0 0 0
0 0 100 0 0 0| 18 0  2 | 0 0 0
0 0 100 0 0 0| 0    0  3 | 0 0 0
我们可以看到这里有3个设备号15，169和185.设备名和设备号的关系我们可以参考文件/proc/interrupts,这里185代表网卡eth1.
# cat /proc/interrupts
CPU0
0: 1277238713 IO-APIC-edge timer
6: 5 IO-APIC-edge floppy
7: 0 IO-APIC-edge parport0
8: 1 IO-APIC-edge rtc
9: 1 IO-APIC-level acpi
14: 6011913 IO-APIC-edge ide0
15: 15761438 IO-APIC-edge ide1
169: 26 IO-APIC-level Intel 82801BA-ICH2
185: 16785489 IO-APIC-level eth1
193: 0 IO-APIC-level uhci_hcd:usb1
mpstat可以显示每个CPU的运行状况，比如系统有4个CPU。我们可以看到：
# mpstat –P ALL 1
Linux 2.4.21-20.ELsmp (localhost.localdomain) 05/23/2006
05:17:31 PM CPU %user %nice %system %idle intr/s
05:17:32 PM all 0.00 0.00 3.19 96.53 13.27
05:17:32 PM 0 0.00 0.00 0.00 100.00 0.00
05:17:32 PM 1 1.12 0.00 12.73 86.15 13.27
05:17:32 PM 2 0.00 0.00 0.00 100.00 0.00
05:17:32 PM 3 0.00 0.00 0.00 100.00 0.00
总结的说，CPU性能监控包含以下方面：
检查系统的运行队列，确保每一个CPU的运行队列不大于3.
确保CPU使用分布满足70/30原则（用户70%，系统30%）。
如果系统时间过长，可能是因为频繁的调度和改变优先级。
CPU Bound进程总是会被惩罚（降低优先级）而IO Bound进程总会被奖励（提高优先级）。
**4.prstat命令**
要显示系统上当前运行的进程和项目的各种统计信息，请使用带有**-J**选项的prstat命令：
%**prstat -J**
        PID USERNAME SIZE  RSS STATE PRI NICE     TIME CPU PROCESS/NLWP
 21634 jtd     5512K 4848K cpu0   44   0  0:00.00 0.3% prstat/1
  324 root      29M  75M sleep  59   0  0:08.27 0.2% Xsun/1
 15497 jtd       48M  41M sleep  49   0  0:08.26 0.1% adeptedit/1
  328 root    2856K 2600K sleep  58   0  0:00.00 0.0% mibiisa/11
 1979 jtd     1568K 1352K sleep  49   0  0:00.00 0.0% csh/1
 1977 jtd     7256K 5512K sleep  49   0  0:00.00 0.0% dtterm/1
  192 root    3680K 2856K sleep  58   0  0:00.36 0.0% automountd/5
 1845 jtd       24M  22M sleep  49   0  0:00.29 0.0% dtmail/11
 1009 jtd     9864K 8384K sleep  49   0  0:00.59 0.0% dtwm/8
  114 root    1640K 704K sleep  58   0  0:01.16 0.0% in.routed/1
  180 daemon  2704K 1944K sleep  58   0  0:00.00 0.0% statd/4
  145 root    2120K 1520K sleep  58   0  0:00.00 0.0% ypbind/1
  181 root    1864K 1336K sleep  51   0  0:00.00 0.0% lockd/1
  173 root    2584K 2136K sleep  58   0  0:00.00 0.0% inetd/1
  135 root    2960K 1424K sleep   0   0  0:00.00 0.0% keyserv/4
PROJID   NPROC SIZE  RSS MEMORY     TIME CPU PROJECT
   10      52 400M 271M   68%  0:11.45 0.4% booksite
    0      35 113M 129M   32%  0:10.46 0.2% system
Total: 87 processes, 205 lwps, load averages: 0.05, 0.02, 0.02
要显示系统上当前运行的进程和任务的各种统计信息，请使用带有**-T**选项的prstat命令：
%**prstat -T**
  PID USERNAME SIZE  RSS STATE PRI NICE     TIME CPU PROCESS/NLWP
 23023 root      26M  20M sleep  59   0  0:03:18 0.6% Xsun/1
 23476 jtd       51M  45M sleep  49   0  0:04:31 0.5% adeptedit/1
 23432 jtd     6928K 5064K sleep  59   0  0:00:00 0.1% dtterm/1
 28959 jtd       26M  18M sleep  49   0  0:00:18 0.0% .netscape.bin/1
 23116 jtd     9232K 8104K sleep  59   0  0:00:27 0.0% dtwm/5
 29010 jtd     5144K 4664K cpu0   59   0  0:00:00 0.0% prstat/1
  200 root    3096K 1024K sleep  59   0  0:00:00 0.0% lpsched/1
  161 root    2120K 1600K sleep  59   0  0:00:00 0.0% lockd/2
  170 root    5888K 4248K sleep  59   0  0:03:10 0.0% automountd/3
  132 root    2120K 1408K sleep  59   0  0:00:00 0.0% ypbind/1
  162 daemon  2504K 1936K sleep  59   0  0:00:00 0.0% statd/2
  146 root    2560K 2008K sleep  59   0  0:00:00 0.0% inetd/1
  122 root    2336K 1264K sleep  59   0  0:00:00 0.0% keyserv/2
  119 root    2336K 1496K sleep  59   0  0:00:02 0.0% rpcbind/1
  104 root    1664K 672K sleep  59   0  0:00:03 0.0% in.rdisc/1
TASKID   NPROC SIZE  RSS MEMORY     TIME CPU PROJECT                    
  222      30 229M 161M   44%  0:05:54 0.6% group.staff                
  223       1  26M  20M  5.3%  0:03:18 0.6% group.staff                
   12       1  61M  33M  8.9%  0:00:31 0.0% group.staff                
    1      33  85M  53M   14%  0:03:33 0.0% system                     
Total: 65 processes, 154 lwps, load averages: 0.04, 0.05, 0.06      
**注–**
**-J**和**-T**选项不能一起使用。
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
在系统维护的过程中，随时可能有需要查看 CPU 使用率，并根据相应信息分析系统状况的需要。在 CentOS 中，可以通过 top 命令来查看 CPU 使用状况。运行 top 命令后，CPU 使用状态会以全屏的方式显示，并且会处在对话的模式 -- 用基于 top 的命令，可以控制显示方式等等。退出 top 的命令为 q （在 top 运行中敲 q 键一次）。
top命令是Linux下常用的性能分析工具，能够实时显示系统中各个进程的资源占用状况，类似于Windows的任务管理器
　　可以直接使用top命令后，查看%MEM的内容。可以选择按进程查看或者按用户查看，如想查看oracle用户的进程内存使用情况的话可以使用如下的命令：
　　$ top -u oracle
内容解释：
> 
　　PID：进程的ID
　　USER：进程所有者
　　PR：进程的优先级别，越小越优先被执行
　　NInice：值
　　VIRT：进程占用的虚拟内存
　　RES：进程占用的物理内存
　　SHR：进程使用的共享内存
　　S：进程的状态。S表示休眠，R表示正在运行，Z表示僵死状态，N表示该进程优先值为负数
　　%CPU：进程占用CPU的使用率
　　%MEM：进程使用的物理内存和总内存的百分比
　　TIME+：该进程启动后占用的总的CPU时间，即占用CPU使用时间的累加值。
　　COMMAND：进程启动命令名称
　　操作实例:
　　在命令行中输入 “top”
　　即可启动 top
　　top 的全屏对话模式可分为3部分：系统信息栏、命令输入栏、进程列表栏。
　　第一部分 -- 最上部的 系统信息栏 ：
　　第一行（top）：
　　　　“00:11:04”为系统当前时刻；
　　　　“3:35”为系统启动后到现在的运作时间；
　　　　“2 users”为当前登录到系统的用户，更确切的说是登录到用户的终端数 -- 同一个用户同一时间对系统多个终端的连接将被视为多个用户连接到系统，这里的用户数也将表现为终端的数目；
　　　　“load average”为当前系统负载的平均值，后面的三个值分别为1分钟前、5分钟前、15分钟前进程的平均数，一般的可以认为这个数值超过 CPU 数目时，CPU 将比较吃力的负载当前系统所包含的进程；
　　第二行（Tasks）：
　　　　“59 total”为当前系统进程总数；
　　　　“1 running”为当前运行中的进程数；
　　　　“58 sleeping”为当前处于等待状态中的进程数；
　　　　“0 stoped”为被停止的系统进程数；
　　　　“0 zombie”为被复原的进程数；
　　第三行（Cpus）：
　　　　分别表示了 CPU 当前的使用率；
　　第四行（Mem）：
　　　　分别表示了内存总量、当前使用量、空闲内存量、以及缓冲使用中的内存量；
　　第五行（Swap）：
　　　　表示类别同第四行（Mem），但此处反映着交换分区（Swap）的使用情况。通常，交换分区（Swap）被频繁使用的情况，将被视作物理内存不足而造成的。
　　第二部分 -- 中间部分的内部命令提示栏：
　　top 运行中可以通过 top 的内部命令对进程的显示方式进行控制。内部命令如下表：
　　s
　　- 改变画面更新频率
　　l - 关闭或开启第一部分第一行 top 信息的表示
　　t - 关闭或开启第一部分第二行 Tasks 和第三行 Cpus 信息的表示
　　m - 关闭或开启第一部分第四行 Mem 和 第五行 Swap 信息的表示
　　N - 以 PID 的大小的顺序排列表示进程列表（第三部分后述）
　　P - 以 CPU 占用率大小的顺序排列进程列表 （第三部分后述）
　　M - 以内存占用率大小的顺序排列进程列表 （第三部分后述）
　　h - 显示帮助
　　n - 设置在进程列表所显示进程的数量
　　q - 退出 top
　　s -
　　改变画面更新周期
　　第三部分 -- 最下部分的进程列表栏：
　　以 PID 区分的进程列表将根据所设定的画面更新时间定期的更新。通过 top 内部命令可以控制此处的显示方式
**pmap**
可以根据进程查看进程相关信息占用的内存情况，(进程号可以通过ps查看)如下所示：
　　$ pmap -d 5647
**ps**
　　如下例所示：
　　$ ps -e -o 'pid,comm,args,pcpu,rsz,vsz,stime,user,uid'  其中rsz是是实际内存
　　$ ps -e -o 'pid,comm,args,pcpu,rsz,vsz,stime,user,uid' | grep oracle |  sort -nrk
　　其中rsz为实际内存，上例实现按内存排序，由大到小
在Linux下查看内存我们一般用free命令：
[root@scs-2 tmp]# free
             total       used       free     shared    buffers     cached
Mem:       3266180    3250004      16176          0     110652    2668236
-/+ buffers/cache:     471116    2795064
Swap:      2048276      80160    1968116
下面是对这些数值的解释：
total:总计物理内存的大小。
used:已使用多大。
free:可用有多少。
Shared:多个进程共享的内存总额。
Buffers/cached:磁盘缓存的大小。
第三行(-/+ buffers/cached):
used:已使用多大。
free:可用有多少。
第四行就不多解释了。
区别：第二行(mem)的used/free与第三行(-/+ buffers/cache) used/free的区别。 这两个的区别在于使用的角度来看，第一行是从OS的角度来看，因为对于OS，buffers/cached 都是属于被使用，所以他的可用内存是16176KB,已用内存是3250004KB,其中包括，内核（OS）使用+Application(X, oracle,etc)使用的+buffers+cached.
第三行所指的是从应用程序角度来看，对于应用程序来说，buffers/cached 是等于可用的，因为buffer/cached是为了提高文件读取的性能，当应用程序需在用到内存的时候，buffer/cached会很快地被回收。
所以从应用程序的角度来说，可用内存=系统free memory+buffers+cached。
如上例：
2795064=16176+110652+2668236
接下来解释什么时候内存会被交换，以及按什么方交换。 当可用内存少于额定值的时候，就会开会进行交换。
如何看额定值：
cat /proc/meminfo
[root@scs-2 tmp]# cat /proc/meminfo
MemTotal:      3266180 kB
MemFree:         17456 kB
Buffers:        111328 kB
Cached:        2664024 kB
SwapCached:          0 kB
Active:         467236 kB
Inactive:      2644928 kB
HighTotal:           0 kB
HighFree:            0 kB
LowTotal:      3266180 kB
LowFree:         17456 kB
SwapTotal:     2048276 kB
SwapFree:      1968116 kB
Dirty:               8 kB
Writeback:           0 kB
Mapped:         345360 kB
Slab:           112344 kB
Committed_AS:   535292 kB
PageTables:       2340 kB
VmallocTotal: 536870911 kB
VmallocUsed:    272696 kB
VmallocChunk: 536598175 kB
HugePages_Total:     0
HugePages_Free:      0
Hugepagesize:     2048 kB
用free -m查看的结果：
[root@scs-2 tmp]# free -m 
             total       used       free     shared    buffers     cached
Mem:          3189       3173         16          0        107       2605
-/+ buffers/cache:        460       2729
Swap:         2000         78       1921
查看/proc/kcore文件的大小（内存镜像）：
[root@scs-2 tmp]# ll -h /proc/kcore 
-r-------- 1 root root 4.1G Jun 12 12:04 /proc/kcore
备注：
占用内存的测量
测量一个进程占用了多少内存，linux为我们提供了一个很方便的方法，/proc目录为我们提供了所有的信息，实际上top等工具也通过这里来获取相应的信息。
/proc/meminfo 机器的内存使用信息
/proc/pid/maps pid为进程号，显示当前进程所占用的虚拟地址。
/proc/pid/statm 进程所占用的内存
[root@localhost ~]# cat /proc/self/statm
654 57 44 0 0 334 0
输出解释
CPU 以及CPU0。。。的每行的每个参数意思（以第一行为例）为：
参数 解释 /proc//status
Size (pages) 任务虚拟地址空间的大小 VmSize/4
Resident(pages) 应用程序正在使用的物理内存的大小 VmRSS/4
Shared(pages) 共享页数 0
Trs(pages) 程序所拥有的可执行虚拟内存的大小 VmExe/4
Lrs(pages) 被映像到任务的虚拟内存空间的库的大小 VmLib/4
Drs(pages) 程序数据段和用户态的栈的大小 （VmData+ VmStk ）4
dt(pages) 04
查看机器可用内存
/proc/28248/>free
total used free shared buffers cached
Mem: 1023788 926400 97388 0 134668 503688
-/+ buffers/cache: 288044 735744
Swap: 1959920 89608 1870312
我们通过free命令查看机器空闲内存时，会发现free的值很小。这主要是因为，在linux中有这么一种思想，内存不用白不用，因此它尽可能的cache和buffer一些数据，以方便下次使用。但实际上这些内存也是可以立刻拿来使用的。
所以 空闲内存=free+buffers+cached=total-used
top命令 是Linux下常用的性能 分析工具 ，能够实时显示系统 中各个进程的资源占用状况，类似于Windows的任务管理 器。下面详细介绍它的使用方法。
top - 02:53:32 up 16 days,  6:34, 17 users,  load average: 0.24, 0.21, 0.24
Tasks: 481 total,   3 running, 474 sleeping,   0 stopped,   4 zombie
Cpu(s): 10.3%us,  1.8%sy,  0.0%ni, 86.6%id,  0.5%wa,  0.2%hi,  0.6%si,  0.0%st
Mem:   4042764k total,  4001096k used,    41668k free,   383536k buffers
Swap:  2104472k total,     7900k used,  2096572k free,  1557040k cached
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
32497 jacky     20   0  669m 222m  31m R   10  5.6       29:27.62 firefox
 4788 yiuwing   20   0  257m  18m  13m S    5  0.5          5:42.44 konsole
 5657 Liuxiaof  20   0  585m 159m  30m S    4  4.0          5:25.06 firefox
 4455 xiefc      20   0  542m  124m  30m R    4  3.1         7:23.03 firefox
 6188 Liuxiaof  20   0  191m   17m  13m S    4  0.5          0:01.16 konsole
统计信息区前五行是系统整体的统计信息。第一行是任务队列信息，同 uptime  命令的执行结果。其内容如下：
01:06:48  当前时间  
up 1:22  系统运行 时间，格式为时:分  
1 user  当前登录用户 数  
load average: 0.06, 0.60, 0.48  系统负载 ，即任务队列的平均长度。
            三个数值分别为  1分钟、5分钟、15分钟前到现在的平均值。 
第二、三行为进程和CPU的信息。当有多个CPU时，这些内容可能会超过两行。内容如下：
Tasks: 29 total  进程总数  
1 running  正在运行的进程数  
28 sleeping  睡眠的进程数  
0 stopped  停止的进程数  
0 zombie  僵尸进程数  
Cpu(s): 0.3% us  用户空间占用CPU百分比  
1.0% sy  内核 空间占用CPU百分比  
0.0% ni  用户进程空间内改变过优先级的进程占用CPU百分比  
98.7% id  空闲CPU百分比  
0.0% wa  等待输入输出的CPU时间百分比  
0.0% hi     
0.0% si    
最后两行为内存 信息。内容如下：
Mem: 191272k total  物理内存总量  
173656k used  使用的物理内存总量  
17616k free  空闲内存总量  
22052k buffers  用作内核缓存 的内存量  
Swap: 192772k total  交换区总量  
0k used  使用的交换区总量  
192772k free  空闲交换区总量  
123988k cached  缓冲的交换区总量。
            内存中的内容被换出到交换区，而后又被换入到内存，但使用过的交换区尚未被覆盖，
            该数值即为这些内容已存在于内存中 的交换区的大小。
            相应的内存再次被换出时可不必再对交换区写入。 
进程信息区统计信息区域的下方显示了各个进程的详细信息。首先来认识一下各列的含义。
序号  列名  含义  
a  PID  进程id  
b  PPID  父进程id  
c  RUSER  Real user name  
d  UID  进程所有者的用户id  
e  USER  进程所有者的用户名  
f  GROUP  进程所有者的组名  
g  TTY  启动进程的终端名。不是从终端启动的进程则显示为 ?  
h  PR  优先级  
i  NI  nice值。负值表示高优先级，正值表示低优先级  
j  P  最后使用的CPU，仅在多CPU环境 下有意义  
k  %CPU  上次更新到现在的CPU时间占用百分比  
l  TIME  进程使用的CPU时间总计，单位秒  
m  TIME+  进程使用的CPU时间总计，单位1/100秒  
n  %MEM  进程使用的物理内存 百分比  
o  VIRT  进程使用的虚拟内存总量，单位kb。VIRT=SWAP+RES  
p  SWAP  进程使用的虚拟内存中，被换出的大小，单位kb。  
q  RES  进程使用的、未被换出的物理内存大小，单位kb。RES=CODE+DATA  
r  CODE  可执行代码占用的物理 内存大小，单位kb  
s  DATA  可执行代码以外的部分(数据 段+栈)占用的物理 内存大小，单位kb  
t  SHR  共享内存大小，单位kb  
u  nFLT  页面错误次数  
v  nDRT  最后一次写入到现在，被修改过的页面数。  
w  S  进程状态。
            D =不可中断的睡眠状态
            R =运行
            S =睡眠
            T =跟踪/停止
            Z =僵尸进程  
x  COMMAND  命令名/命令行  
y  WCHAN  若该进程在睡眠，则显示睡眠中的系统函数名  
z  Flags  任务标志，参考 sched.h 
默认情况下仅显示比较重要的  PID、USER、PR、NI、VIRT、RES、SHR、S、%CPU、%MEM、TIME+、COMMAND  列。可以通过下面的快捷键来更改显示内容。
更改显示内容通过 f 键可以选择显示的内容。按 f 键之后会显示列的列表，按 a-z  即可显示或隐藏对应的列，最后按回车键确定。
按 o 键可以改变列的显示顺序。按小写的 a-z 可以将相应的列向右移动，而大写的 A-Z  可以将相应的列向左移动。最后按回车键确定。
按大写的 F 或 O 键，然后按 a-z 可以将进程按照相应的列进行排序。而大写的  R 键可以将当前的排序倒转。
==============================
top命令使用过程中，还可以使用一些交互的命令来完成其它参数的功能。这些命令是通过快捷键启动的。
＜空格＞：立刻刷新。
P：根据CPU使用大小进行排序。
T：根据时间、累计时间排序。
q：退出top命令。
m：切换显示内存信息。
t：切换显示进程和CPU状态信息。
c：切换显示命令名称和完整命令行。
M：根据使用内存大小进行排序。
W：将当前设置写入~/.toprc文件中。这是写top配置文件的推荐方法。
可以看到，top命令是一个功能十分强大的监控系统的工具，对于系统管理员而言尤其重要。但是，它的缺点是会消耗很多系统资源。
 应用实例 
使用top命令可以监视指定用户，缺省情况是监视所有用户的进程。如果想查看指定用户的情况，在终端中按“U”键，然后输入用户名，系统就会切换为指定用户的进程运行界面。
ａ.作用
free命令用来显示内存的使用情况，使用权限是所有用户。
ｂ.格式
free [－b　－k　－m] [－o] [－s delay] [－t] [－V]
ｃ.主要参数
－b －k －m：分别以字节（KB、MB）为单位显示内存使用情况。
－s delay：显示每隔多少秒数来显示一次内存使用情况。
－t：显示内存总和列。
－o：不显示缓冲区调节列。
ｄ.应用实例
free命令是用来查看内存使用情况的主要命令。和top命令相比，它的优点是使用简单，并且只占用很少的系统资源。通过－S参数可以使用free命令不间断地监视有多少内存在使用，这样可以把它当作一个方便实时监控器。
＃free －b －s5
使用这个命令后终端会连续不断地报告内存使用情况（以字节为单位），每5秒更新一次。
参考 [http://www.cnblogs.com/gaojun/p/3406096.html](http://www.cnblogs.com/gaojun/p/3406096.html)
在系统维护的过程中，随时可能有需要查看 CPU 使用率，并根据相应信息分析系统状况的需要。在 CentOS 中，可以通过 top 命令来查看 CPU 使用状况。运行 top 命令后，CPU 使用状态会以全屏的方式显示，并且会处在对话的模式 -- 用基于 top 的命令，可以控制显示方式等等。退出 top 的命令为 q （在 top 运行中敲 q 键一次）。
top命令是Linux下常用的性能分析工具，能够实时显示系统中各个进程的资源占用状况，类似于Windows的任务管理器
　　可以直接使用top命令后，查看%MEM的内容。可以选择按进程查看或者按用户查看，如想查看oracle用户的进程内存使用情况的话可以使用如下的命令：
　　$ top -u oracle
内容解释：
> 
　　PID：进程的ID
　　USER：进程所有者
　　PR：进程的优先级别，越小越优先被执行
　　NInice：值
　　VIRT：进程占用的虚拟内存
　　RES：进程占用的物理内存
　　SHR：进程使用的共享内存
　　S：进程的状态。S表示休眠，R表示正在运行，Z表示僵死状态，N表示该进程优先值为负数
　　%CPU：进程占用CPU的使用率
　　%MEM：进程使用的物理内存和总内存的百分比
　　TIME+：该进程启动后占用的总的CPU时间，即占用CPU使用时间的累加值。
　　COMMAND：进程启动命令名称
　　操作实例:
　　在命令行中输入 “top”
　　即可启动 top
　　top 的全屏对话模式可分为3部分：系统信息栏、命令输入栏、进程列表栏。
　　第一部分 -- 最上部的 系统信息栏 ：
　　第一行（top）：
　　　　“00:11:04”为系统当前时刻；
　　　　“3:35”为系统启动后到现在的运作时间；
　　　　“2 users”为当前登录到系统的用户，更确切的说是登录到用户的终端数 -- 同一个用户同一时间对系统多个终端的连接将被视为多个用户连接到系统，这里的用户数也将表现为终端的数目；
　　　　“load average”为当前系统负载的平均值，后面的三个值分别为1分钟前、5分钟前、15分钟前进程的平均数，一般的可以认为这个数值超过 CPU 数目时，CPU 将比较吃力的负载当前系统所包含的进程；
　　第二行（Tasks）：
　　　　“59 total”为当前系统进程总数；
　　　　“1 running”为当前运行中的进程数；
　　　　“58 sleeping”为当前处于等待状态中的进程数；
　　　　“0 stoped”为被停止的系统进程数；
　　　　“0 zombie”为被复原的进程数；
　　第三行（Cpus）：
　　　　分别表示了 CPU 当前的使用率；
　　第四行（Mem）：
　　　　分别表示了内存总量、当前使用量、空闲内存量、以及缓冲使用中的内存量；
　　第五行（Swap）：
　　　　表示类别同第四行（Mem），但此处反映着交换分区（Swap）的使用情况。通常，交换分区（Swap）被频繁使用的情况，将被视作物理内存不足而造成的。
　　第二部分 -- 中间部分的内部命令提示栏：
　　top 运行中可以通过 top 的内部命令对进程的显示方式进行控制。内部命令如下表：
　　s
　　- 改变画面更新频率
　　l - 关闭或开启第一部分第一行 top 信息的表示
　　t - 关闭或开启第一部分第二行 Tasks 和第三行 Cpus 信息的表示
　　m - 关闭或开启第一部分第四行 Mem 和 第五行 Swap 信息的表示
　　N - 以 PID 的大小的顺序排列表示进程列表（第三部分后述）
　　P - 以 CPU 占用率大小的顺序排列进程列表 （第三部分后述）
　　M - 以内存占用率大小的顺序排列进程列表 （第三部分后述）
　　h - 显示帮助
　　n - 设置在进程列表所显示进程的数量
　　q - 退出 top
　　s -
　　改变画面更新周期
　　第三部分 -- 最下部分的进程列表栏：
　　以 PID 区分的进程列表将根据所设定的画面更新时间定期的更新。通过 top 内部命令可以控制此处的显示方式
**pmap**
可以根据进程查看进程相关信息占用的内存情况，(进程号可以通过ps查看)如下所示：
　　$ pmap -d 5647
**ps**
　　如下例所示：
　　$ ps -e -o 'pid,comm,args,pcpu,rsz,vsz,stime,user,uid'  其中rsz是是实际内存
　　$ ps -e -o 'pid,comm,args,pcpu,rsz,vsz,stime,user,uid' | grep oracle |  sort -nrk
　　其中rsz为实际内存，上例实现按内存排序，由大到小
在Linux下查看内存我们一般用free命令：
[root@scs-2 tmp]# free
             total       used       free     shared    buffers     cached
Mem:       3266180    3250004      16176          0     110652    2668236
-/+ buffers/cache:     471116    2795064
Swap:      2048276      80160    1968116
下面是对这些数值的解释：
total:总计物理内存的大小。
used:已使用多大。
free:可用有多少。
Shared:多个进程共享的内存总额。
Buffers/cached:磁盘缓存的大小。
第三行(-/+ buffers/cached):
used:已使用多大。
free:可用有多少。
第四行就不多解释了。
区别：第二行(mem)的used/free与第三行(-/+ buffers/cache) used/free的区别。 这两个的区别在于使用的角度来看，第一行是从OS的角度来看，因为对于OS，buffers/cached 都是属于被使用，所以他的可用内存是16176KB,已用内存是3250004KB,其中包括，内核（OS）使用+Application(X, oracle,etc)使用的+buffers+cached.
第三行所指的是从应用程序角度来看，对于应用程序来说，buffers/cached 是等于可用的，因为buffer/cached是为了提高文件读取的性能，当应用程序需在用到内存的时候，buffer/cached会很快地被回收。
所以从应用程序的角度来说，可用内存=系统free memory+buffers+cached。
如上例：
2795064=16176+110652+2668236
接下来解释什么时候内存会被交换，以及按什么方交换。 当可用内存少于额定值的时候，就会开会进行交换。
如何看额定值：
cat /proc/meminfo
[root@scs-2 tmp]# cat /proc/meminfo
MemTotal:      3266180 kB
MemFree:         17456 kB
Buffers:        111328 kB
Cached:        2664024 kB
SwapCached:          0 kB
Active:         467236 kB
Inactive:      2644928 kB
HighTotal:           0 kB
HighFree:            0 kB
LowTotal:      3266180 kB
LowFree:         17456 kB
SwapTotal:     2048276 kB
SwapFree:      1968116 kB
Dirty:               8 kB
Writeback:           0 kB
Mapped:         345360 kB
Slab:           112344 kB
Committed_AS:   535292 kB
PageTables:       2340 kB
VmallocTotal: 536870911 kB
VmallocUsed:    272696 kB
VmallocChunk: 536598175 kB
HugePages_Total:     0
HugePages_Free:      0
Hugepagesize:     2048 kB
用free -m查看的结果：
[root@scs-2 tmp]# free -m 
             total       used       free     shared    buffers     cached
Mem:          3189       3173         16          0        107       2605
-/+ buffers/cache:        460       2729
Swap:         2000         78       1921
查看/proc/kcore文件的大小（内存镜像）：
[root@scs-2 tmp]# ll -h /proc/kcore 
-r-------- 1 root root 4.1G Jun 12 12:04 /proc/kcore
备注：
占用内存的测量
测量一个进程占用了多少内存，linux为我们提供了一个很方便的方法，/proc目录为我们提供了所有的信息，实际上top等工具也通过这里来获取相应的信息。
/proc/meminfo 机器的内存使用信息
/proc/pid/maps pid为进程号，显示当前进程所占用的虚拟地址。
/proc/pid/statm 进程所占用的内存
[root@localhost ~]# cat /proc/self/statm
654 57 44 0 0 334 0
输出解释
CPU 以及CPU0。。。的每行的每个参数意思（以第一行为例）为：
参数 解释 /proc//status
Size (pages) 任务虚拟地址空间的大小 VmSize/4
Resident(pages) 应用程序正在使用的物理内存的大小 VmRSS/4
Shared(pages) 共享页数 0
Trs(pages) 程序所拥有的可执行虚拟内存的大小 VmExe/4
Lrs(pages) 被映像到任务的虚拟内存空间的库的大小 VmLib/4
Drs(pages) 程序数据段和用户态的栈的大小 （VmData+ VmStk ）4
dt(pages) 04
查看机器可用内存
/proc/28248/>free
total used free shared buffers cached
Mem: 1023788 926400 97388 0 134668 503688
-/+ buffers/cache: 288044 735744
Swap: 1959920 89608 1870312
我们通过free命令查看机器空闲内存时，会发现free的值很小。这主要是因为，在linux中有这么一种思想，内存不用白不用，因此它尽可能的cache和buffer一些数据，以方便下次使用。但实际上这些内存也是可以立刻拿来使用的。
所以 空闲内存=free+buffers+cached=total-used
top命令 是Linux下常用的性能 分析工具 ，能够实时显示系统 中各个进程的资源占用状况，类似于Windows的任务管理 器。下面详细介绍它的使用方法。
top - 02:53:32 up 16 days,  6:34, 17 users,  load average: 0.24, 0.21, 0.24
Tasks: 481 total,   3 running, 474 sleeping,   0 stopped,   4 zombie
Cpu(s): 10.3%us,  1.8%sy,  0.0%ni, 86.6%id,  0.5%wa,  0.2%hi,  0.6%si,  0.0%st
Mem:   4042764k total,  4001096k used,    41668k free,   383536k buffers
Swap:  2104472k total,     7900k used,  2096572k free,  1557040k cached
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
32497 jacky     20   0  669m 222m  31m R   10  5.6       29:27.62 firefox
 4788 yiuwing   20   0  257m  18m  13m S    5  0.5          5:42.44 konsole
 5657 Liuxiaof  20   0  585m 159m  30m S    4  4.0          5:25.06 firefox
 4455 xiefc      20   0  542m  124m  30m R    4  3.1         7:23.03 firefox
 6188 Liuxiaof  20   0  191m   17m  13m S    4  0.5          0:01.16 konsole
统计信息区前五行是系统整体的统计信息。第一行是任务队列信息，同 uptime  命令的执行结果。其内容如下：
01:06:48  当前时间  
up 1:22  系统运行 时间，格式为时:分  
1 user  当前登录用户 数  
load average: 0.06, 0.60, 0.48  系统负载 ，即任务队列的平均长度。
            三个数值分别为  1分钟、5分钟、15分钟前到现在的平均值。 
第二、三行为进程和CPU的信息。当有多个CPU时，这些内容可能会超过两行。内容如下：
Tasks: 29 total  进程总数  
1 running  正在运行的进程数  
28 sleeping  睡眠的进程数  
0 stopped  停止的进程数  
0 zombie  僵尸进程数  
Cpu(s): 0.3% us  用户空间占用CPU百分比  
1.0% sy  内核 空间占用CPU百分比  
0.0% ni  用户进程空间内改变过优先级的进程占用CPU百分比  
98.7% id  空闲CPU百分比  
0.0% wa  等待输入输出的CPU时间百分比  
0.0% hi     
0.0% si    
最后两行为内存 信息。内容如下：
Mem: 191272k total  物理内存总量  
173656k used  使用的物理内存总量  
17616k free  空闲内存总量  
22052k buffers  用作内核缓存 的内存量  
Swap: 192772k total  交换区总量  
0k used  使用的交换区总量  
192772k free  空闲交换区总量  
123988k cached  缓冲的交换区总量。
            内存中的内容被换出到交换区，而后又被换入到内存，但使用过的交换区尚未被覆盖，
            该数值即为这些内容已存在于内存中 的交换区的大小。
            相应的内存再次被换出时可不必再对交换区写入。 
进程信息区统计信息区域的下方显示了各个进程的详细信息。首先来认识一下各列的含义。
序号  列名  含义  
a  PID  进程id  
b  PPID  父进程id  
c  RUSER  Real user name  
d  UID  进程所有者的用户id  
e  USER  进程所有者的用户名  
f  GROUP  进程所有者的组名  
g  TTY  启动进程的终端名。不是从终端启动的进程则显示为 ?  
h  PR  优先级  
i  NI  nice值。负值表示高优先级，正值表示低优先级  
j  P  最后使用的CPU，仅在多CPU环境 下有意义  
k  %CPU  上次更新到现在的CPU时间占用百分比  
l  TIME  进程使用的CPU时间总计，单位秒  
m  TIME+  进程使用的CPU时间总计，单位1/100秒  
n  %MEM  进程使用的物理内存 百分比  
o  VIRT  进程使用的虚拟内存总量，单位kb。VIRT=SWAP+RES  
p  SWAP  进程使用的虚拟内存中，被换出的大小，单位kb。  
q  RES  进程使用的、未被换出的物理内存大小，单位kb。RES=CODE+DATA  
r  CODE  可执行代码占用的物理 内存大小，单位kb  
s  DATA  可执行代码以外的部分(数据 段+栈)占用的物理 内存大小，单位kb  
t  SHR  共享内存大小，单位kb  
u  nFLT  页面错误次数  
v  nDRT  最后一次写入到现在，被修改过的页面数。  
w  S  进程状态。
            D =不可中断的睡眠状态
            R =运行
            S =睡眠
            T =跟踪/停止
            Z =僵尸进程  
x  COMMAND  命令名/命令行  
y  WCHAN  若该进程在睡眠，则显示睡眠中的系统函数名  
z  Flags  任务标志，参考 sched.h 
默认情况下仅显示比较重要的  PID、USER、PR、NI、VIRT、RES、SHR、S、%CPU、%MEM、TIME+、COMMAND  列。可以通过下面的快捷键来更改显示内容。
更改显示内容通过 f 键可以选择显示的内容。按 f 键之后会显示列的列表，按 a-z  即可显示或隐藏对应的列，最后按回车键确定。
按 o 键可以改变列的显示顺序。按小写的 a-z 可以将相应的列向右移动，而大写的 A-Z  可以将相应的列向左移动。最后按回车键确定。
按大写的 F 或 O 键，然后按 a-z 可以将进程按照相应的列进行排序。而大写的  R 键可以将当前的排序倒转。
==============================
top命令使用过程中，还可以使用一些交互的命令来完成其它参数的功能。这些命令是通过快捷键启动的。
＜空格＞：立刻刷新。
P：根据CPU使用大小进行排序。
T：根据时间、累计时间排序。
q：退出top命令。
m：切换显示内存信息。
t：切换显示进程和CPU状态信息。
c：切换显示命令名称和完整命令行。
M：根据使用内存大小进行排序。
W：将当前设置写入~/.toprc文件中。这是写top配置文件的推荐方法。
可以看到，top命令是一个功能十分强大的监控系统的工具，对于系统管理员而言尤其重要。但是，它的缺点是会消耗很多系统资源。
 应用实例 
使用top命令可以监视指定用户，缺省情况是监视所有用户的进程。如果想查看指定用户的情况，在终端中按“U”键，然后输入用户名，系统就会切换为指定用户的进程运行界面。
ａ.作用
free命令用来显示内存的使用情况，使用权限是所有用户。
ｂ.格式
free [－b　－k　－m] [－o] [－s delay] [－t] [－V]
ｃ.主要参数
－b －k －m：分别以字节（KB、MB）为单位显示内存使用情况。
－s delay：显示每隔多少秒数来显示一次内存使用情况。
－t：显示内存总和列。
－o：不显示缓冲区调节列。
ｄ.应用实例
free命令是用来查看内存使用情况的主要命令。和top命令相比，它的优点是使用简单，并且只占用很少的系统资源。通过－S参数可以使用free命令不间断地监视有多少内存在使用，这样可以把它当作一个方便实时监控器。
＃free －b －s5
使用这个命令后终端会连续不断地报告内存使用情况（以字节为单位），每5秒更新一次。
参考 [http://www.cnblogs.com/gaojun/p/3406096.html](http://www.cnblogs.com/gaojun/p/3406096.html)
