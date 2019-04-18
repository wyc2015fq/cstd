# Linux Top 命令解析 比较详细 - z69183787的专栏 - CSDN博客
2016年11月26日 16:26:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：372
TOP是一个动态显示过程,即可以通过用户按键来不断刷新当前状态.如果在前台执行该命令,它将独占前台,直到用户终止该程序为止.比较准确的说,top命令提供了实时的对系统处理器的状态监视.它将显示系统中CPU最“敏感”的任务列表.该命令可以按CPU使用.内存使用和执行时间对任务进行排序；而且该命令的很多特性都可以通过交互式命令或者在个人定制文件中进行设定.
top - 12:38:33 up 50 days, 23:15,  7 users,  load average: 60.58, 61.14, 61.22
Tasks: 203 total,  60 running, 139 sleeping,   4 stopped,   0 zombie
Cpu(s)  : 27.0%us, 73.0%sy,  0.0%ni,  0.0%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st
Mem:   1939780k total,  1375280k used,   564500k free,   109680k buffers
Swap:  4401800k total,   497456k used,  3904344k free,   848712k cached
PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND                                
 4338 oracle    25   0  627m 209m 207m R    0 11.0 297:14.76 oracle                                
 4267 oracle    25   0  626m 144m 143m R    6  7.6  89:16.62 oracle                                
 3458 oracle    25   0  672m 133m 124m R    0  7.1   1283:08 oracle                                
 3478 oracle    25   0  672m 124m 115m R    0  6.6   1272:30 oracle                                
 3395 oracle    25   0  672m 122m 113m R    0  6.5   1270:03 oracle                                 
 3480 oracle    25   0  672m 122m 109m R    8  6.4   1274:13 oracle                                
 3399 oracle    25   0  672m 121m 110m R    0  6.4   1279:37 oracle                                
 4261 oracle    25   0  634m 100m  99m R    0  5.3  86:13.90 oracle                                
25737 oracle    25   0  632m  81m  74m R    0  4.3 272:35.42 oracle                                
 7072 oracle    25   0  626m  72m  71m R    0  3.8   6:35.68 oracle                                
16073 oracle    25   0  630m  68m  63m R    8  3.6 175:20.36 oracle                                
16140 oracle    25   0  630m  66m  60m R    0  3.5 175:13.42 oracle                                
16122 oracle    25   0  630m  66m  60m R    0  3.5 176:47.73 oracle                                
  786 oracle    25   0  627m  63m  63m R    0  3.4   1:54.93 oracle                                
 4271 oracle    25   0  627m  59m  58m R    8  3.1  86:09.64 oracle                                
 4273 oracle    25   0  627m  57m  56m R    8  3.0  84:38.20 oracle                                
22670 oracle    25   0  626m  50m  49m R    0  2.7  84:55.82 oracle     
一.  TOP前五行统计信息
统计信息区前五行是系统整体的统计信息。
1. 第一行是任务队列信息
同 uptime  命令的执行结果:
[root@localhost ~]# uptime
 13:22:30 up 8 min,  4 users,  load average: 0.14, 0.38, 0.25
其内容如下：
|12:38:33|当前时间|
|----|----|
|up 50days|系统运行时间，格式为时:分|
|1 user|当前登录用户数|
|load average: 0.06, 0.60, 0.48|系统负载，即任务队列的平均长度。 三个数值分别为  1分钟、5分钟、15分钟前到现在的平均值。|
2. 第二、三行为进程和CPU的信息
当有多个CPU时，这些内容可能会超过两行。内容如下：
|Tasks: 29 total|进程总数|
|----|----|
|1 running|正在运行的进程数|
|28 sleeping|睡眠的进程数|
|0 stopped|停止的进程数|
|0 zombie|僵尸进程数|
|Cpu(s): 0.3% us|用户空间占用CPU百分比|
|1.0% sy|内核空间占用CPU百分比|
|0.0% ni|用户进程空间内改变过优先级的进程占用CPU百分比|
|98.7% id|空闲CPU百分比|
|0.0% wa|等待输入输出的CPU时间百分比|
|0.0% hi||
|0.0% si||
3. 第四五行为内存信息。
内容如下：
|Mem: 191272k total|物理内存总量|
|----|----|
|173656k used|使用的物理内存总量|
|17616k free|空闲内存总量|
|22052k buffers|用作内核缓存的内存量|
|Swap: 192772k total|交换区总量|
|0k used|使用的交换区总量|
|192772k free|空闲交换区总量|
|123988k cached|缓冲的交换区总量。 内存中的内容被换出到交换区，而后又被换入到内存，但使用过的交换区尚未被覆盖， 该数值即为这些**内容已存在于内存中**的交换区的大小。相应的内存再次被换出时可不必再对交换区写入。|
二.  进程信息
|列名|含义|
|----|----|
|PID|进程id|
|PPID|父进程id|
|RUSER|Real user name|
|UID|进程所有者的用户id|
|USER|进程所有者的用户名|
|GROUP|进程所有者的组名|
|TTY|启动进程的终端名。不是从终端启动的进程则显示为 ?|
|PR|优先级|
|NI|nice值。负值表示高优先级，正值表示低优先级|
|P|最后使用的CPU，仅在多CPU环境下有意义|
|%CPU|上次更新到现在的CPU时间占用百分比|
|TIME|进程使用的CPU时间总计，单位秒|
|TIME+|进程使用的CPU时间总计，单位1/100秒|
|%MEM|进程使用的物理内存百分比|
|VIRT|进程使用的虚拟内存总量，单位kb。VIRT=SWAP+RES|
|SWAP|进程使用的虚拟内存中，被换出的大小，单位kb。|
|RES|进程使用的、未被换出的物理内存大小，单位kb。RES=CODE+DATA|
|CODE|可执行代码占用的物理内存大小，单位kb|
|DATA|可执行代码以外的部分(数据段+栈)占用的物理内存大小，单位kb|
|SHR|共享内存大小，单位kb|
|nFLT|页面错误次数|
|nDRT|最后一次写入到现在，被修改过的页面数。|
|S|进程状态。            D=不可中断的睡眠状态            R=运行            S=睡眠            T=跟踪/停止            Z=僵尸进程|
|COMMAND|命令名/命令行|
|WCHAN|若该进程在睡眠，则显示睡眠中的系统函数名|
|Flags|任务标志，参考 sched.h|
top 的man 命令解释如下：
    Listed below are top's available fields.  They are always associated with the  letter  shown,  regardless  of the position you may have established for them with the 'o' (Order fields) interactive command.Any field is selectable as the sort field, and you
 control whether they are  sorted high-to-low  or  low-to-high.   For  additional  information on sort provisions see  topic 3c. TASK Area Commands.
a: PID  --  Process Id
       The task's unique process ID, which periodically wraps, though never  restarting at zero.
b: PPID  --  Parent Process Pid
       The process ID of a task's parent.
c: RUSER  --  Real User Name
       The real user name of the task's owner.
d: UID  --  User Id
       The effective user ID of the task's owner.
e: USER  --  User Name
       The effective user name of the task's owner.
f: GROUP  --  Group Name
       The effective group name of the task's owner.
g: TTY  --  Controlling Tty
       The  name of the controlling terminal.  This is usually the device (serial port, pty, etc.) from which the process was started, and which it uses  for  input  oroutput.   However,  a task need not be associated with a terminal, in which case you'll see
 '?' displayed.
h: PR  --  Priority
       The priority of the task.
i: NI  --  Nice value
       The nice value of the task.   A  negative  nice  value  means  higher  priority, whereas  a  positive nice value means lower priority.  Zero in this field simply means priority will not be adjusted in determining a task's dispatchability.
j: P  --  Last used CPU (SMP)
       A number representing the last used processor.  In a true SMP  environment  this will likely change frequently since the kernel intentionally uses weak affinity. Also, the very act of running top may break this weak affinity  and  cause  more processes  to
 change CPUs more often (because of the extra demand for cpu time).
k: %CPU  --  CPU usage
       The task's share of the elapsed CPU time since the last screen update, expressed as a percentage of total CPU time.  In a true SMP environment, if 'Irix mode' is Off, top will operate in 'Solaris mode' where a task's cpu usage will be divided by  the  total  number  of  CPUs.   You
 toggle 'Irix/Solaris' modes with the 'I' interactive command.
l: TIME  --  CPU Time
       Total CPU time the task has used since it started.  When  'Cumulative  mode'  is On,  each  process is listed with the cpu time that it and its dead children has used.  You toggle 'Cumulative mode' with 'S', which is a command-line option and an interactive
 command.  See the 'S' interactive command for additional information regarding this mode.
m: TIME+  --  CPU Time, hundredths
       The same as 'TIME', but reflecting more granularity through hundredths of a sec          ond.
n: %MEM  --  Memory usage (RES)
       A task's currently used share of available physical memory.
o: VIRT  --  Virtual Image (kb)
       The total amount of virtual memory used by the task.  It includes all code, data and shared libraries plus pages that have  been  swapped  out.  (Note:  you  can define  the STATSIZE=1 environment variable and the VIRT will be calculated from the /proc/#/state
 VmSize field.)
       VIRT = SWAP + RES.
p: SWAP  --  Swapped size (kb)
       The swapped out portion of a task's total virtual memory image.
q: RES  --  Resident size (kb)
       The non-swapped physical memory a task has used.
       RES = CODE + DATA.
r: CODE  --  Code size (kb)
       The amount of physical memory devoted to executable  code,  also  known  as  the'text resident set' size or TRS.
s: DATA  --  Data+Stack size (kb)
       The  amount of physical memory devoted to other than executable code, also known the 'data resident set' size or DRS.
t: SHR  --  Shared Mem size (kb)
       The amount of shared memory used by a task.   It  simply  reflects  memory  that could be potentially shared with other processes.
u: nFLT  --  Page Fault count
       The  number  of  major  page faults that have occurred for a task.  A page fault occurs when a process attempts to read from or write to a virtual page  that  is not  currently  present  in  its address space.  A major page fault is when disk access
 is involved in making that page available.
v: nDRT  --  Dirty Pages count
       The number of pages that have been modified since  they  were  last  written  to disk.   Dirty  pages  must  be written to disk before the corresponding physical memory location can be used for some other virtual page.
w: S  --  Process Status
       The status of the task which can be one of:
             'D' = uninterruptible sleep
             'R' = running
             'S' = sleeping
             'T' = traced or stopped
             'Z' = zombie
       Tasks shown as running should be more properly thought of as 'ready to run'  --their  task_struct is simply represented on the Linux run-queue.  Even without a true SMP machine, you may see numerous tasks in this state  depending  on  top's delay interval
 and nice value.
x: Command  --  Command line or Program name
       Display the command line used to start a task or the name of the associated program.  You toggle between command line and name with 'c', which is both  a  command-line option and an interactive command. When  you've  chosen  to display command lines,
 processes without a command line (like kernel threads) will be shown with only the program name  in  parentheses, as in this example:                ( mdrecoveryd ) Either  form  of  display is subject to potential truncation if it's too long to fit in this
 field's  current  width.   That  width  depends  upon  other  fields  selected, their order and the current screen width.
       Note: The 'Command' field/column is unique, in that it is not fixed-width.  When displayed, this column will be allocated all remaining screen width (up  to  the maximum  512  characters)  to  provide for the potential growth of program names into command
 lines.
y: WCHAN  --  Sleeping in Function
       Depending on the availability of the kernel link map ('System.map'), this  field will  show  the  name or the address of the kernel function in which the task is currently sleeping.  Running tasks will display a dash ('-') in this column.
       Note: By displaying this field, top's own working set will be increased by  over 700Kb.   Your  only  means of reducing that overhead will be to stop and restart          top.
z: Flags  --  Task Flags
       This column represents the task's current scheduling flags which  are  expressed in  hexadecimal  notation and with zeros suppressed.  These flags are officially documented in <linux/sched.h>.  Less formal documentation can also be  found  on the 'Fields
 select' and 'Order fields' screens.
       默认情况下仅显示比较重要的  PID、USER、PR、NI、VIRT、RES、SHR、S、%CPU、%MEM、TIME+、COMMAND  列。
2.1 用快捷键更改显示内容。
（1）更改显示内容通过 f键可以选择显示的内容。
       按 f 键之后会显示列的列表，按 a-z  即可显示或隐藏对应的列，最后按回车键确定。
（2）按o键可以改变列的显示顺序。
       按小写的 a-z 可以将相应的列向右移动，而大写的 A-Z  可以将相应的列向左移动。最后按回车键确定。
       按大写的 F 或 O 键，然后按 a-z 可以将进程按照相应的列进行排序。而大写的  R 键可以将当前的排序倒转。
       设置完按回车返回界面。
三.  命令使用
详细内容可以参考MAN 帮助文档。这里列举部分内容：
命令格式：
top [-] [d] [p] [q] [c] [C] [S]    [n]
参数说明：
d：  指定每两次屏幕信息刷新之间的时间间隔。当然用户可以使用s交互命令来改变之。
p：  通过指定监控进程ID来仅仅监控某个进程的状态。
q：该选项将使top没有任何延迟的进行刷新。如果调用程序有超级用户权限，那么top将以尽可能高的优先级运行。
S： 指定累计模式
s ： 使top命令在安全模式中运行。这将去除交互命令所带来的潜在危险。
i：  使top不显示任何闲置或者僵死进程。
c：  显示整个命令行而不只是显示命令名
在top命令的显示窗口，我们还可以输入以下字母，进行一些交互：
帮助文档如下：
Help for Interactive Commands - procps version 3.2.7
Window 1:Def: Cumulative mode Off.  System: Delay 4.0 secs; Secure mode Off.
  Z,B       Global: 'Z' change color mappings; 'B' disable/enable bold
  l,t,m     Toggle Summaries: 'l' load avg; 't' task/cpu stats; 'm' mem info
  1,I       Toggle SMP view: '1' single/separate states; 'I' Irix/Solaris mode
  f,o     . Fields/Columns: 'f' add or remove; 'o' change display order
  F or O  . Select sort field
  <,>     . Move sort field: '<' next col left; '>' next col right
  R,H     . Toggle: 'R' normal/reverse sort; 'H' show threads
  c,i,S   . Toggle: 'c' cmd name/line; 'i' idle tasks; 'S' cumulative time
  x,y     . Toggle highlights: 'x' sort field; 'y' running tasks
  z,b     . Toggle: 'z' color/mono; 'b' bold/reverse (only if 'x' or 'y')
  u       . Show specific user only
  n or #  . Set maximum tasks displayed
  k,r       Manipulate tasks: 'k' kill; 'r' renice
  d or s    Set update interval
  W         Write configuration file
  q         Quit
          ( commands shown with '.' require a visible task display window )
Press 'h' or '?' for help with Windows,
h或者?  : 显示帮助画面，给出一些简短的命令总结说明。
k  ：终止一个进程。系统将提示用户输入需要终止的进程PID，以及需要发送给该进程什么样的信号。一般的终止进程可以使用15信号；如果不能正常结束那就使用信号9强制结束该进程。默认值是信号15。在安全模式中此命令被屏蔽。
i：忽略闲置和僵死进程。这是一个开关式命令。
q：  退出程序。
r：  重新安排一个进程的优先级别。系统提示用户输入需要改变的进程PID以及需要设置的进程优先级值。输入一个正值将使优先级降低，反之则可以使该进程拥有更高的优先权。默认值是10。
S：切换到累计模式。
s :  改变两次刷新之间的延迟时间。系统将提示用户输入新的时间，单位为s。如果有小数，就换算成ms。输入0值则系统将不断刷新，默认值是5 s。需要注意的是如果设置太小的时间，很可能会引起不断刷新，从而根本来不及看清显示的情况，而且系统负载也会大大增加。
f或者F :从当前显示中添加或者删除项目。
o或者O  :改变显示项目的顺序。
l: 切换显示平均负载和启动时间信息。即显示影藏第一行
m： 切换显示内存信息。即显示影藏内存行
t ： 切换显示进程和CPU状态信息。即显示影藏CPU行
c：  切换显示命令名称和完整命令行。 显示完整的命令。 这个功能很有用。
M ： 根据驻留内存大小进行排序。
P：根据CPU使用百分比大小进行排序。
T： 根据时间/累计时间进行排序。
W：  将当前设置写入~/.toprc文件中。这是写top配置文件的推荐方法。
