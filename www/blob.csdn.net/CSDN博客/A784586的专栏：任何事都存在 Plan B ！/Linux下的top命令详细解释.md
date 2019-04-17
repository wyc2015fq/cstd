# Linux下的top命令详细解释 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年06月08日 13:21:27[QuJack](https://me.csdn.net/A784586)阅读数：406








## **Linux下的top命令详细解释**

## 1.top 

该命令可以按CPU使用.内存使用和执行时间对任务进行排序；而且该命令的很多特性都可以通过交互式命令或者在个人定制文件中进行设定.

top - 12:38:33 up 50 days, 23:15,  7 users,  load average: 60.58, 61.14, 61.22

Tasks: 203 total,  60 running, 139 sleeping,   4 stopped,   0 zombie

Cpu(s)  : 27.0%us, 73.0%sy,  0.0%ni,  0.0%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st

Mem:   1939780k total,  1375280k used,   564500k free,   109680k buffers

Swap:  4401800k total,   497456k used,  3904344k free,   848712k cached

PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND                                

 4338 oracle    25   0  627m 209m 207m R    0 11.0 297:14.76 oracle                                

 4267 oracle    25   0  626m 144m 143m R    6  7.6  89:16.62 oracle    

含义：

当前时间，系统运行时间（分），登录用户数，系统负载（任务队列平均长）1,5,15分钟到现在的平均值；       

同 uptime  命令的执行结果:

[root@localhost ~]# uptime

 13:22:30 up 8 min,  4 users,  load average: 0.14, 0.38, 0.25

第二、三行为进程和CPU的信息

当有多个CPU时，这些内容可能会超过两行。

进程总数，正在运行的进程数，睡眠进程数 ，停止进程数，讲师进程数

用户空间占用cpu百分比，内核空间占用CPU百分比，用户进程空间内改变过优先级的进程占用CPU百分比，空间CPU百分比，等待输入输出的CPU时间比

第四五行为内存信息

物理内存总量，使用的物理内存总量，空间内存总量，内核缓存的内存量

交换区的总量，使用的交换区总量，空间交换区总量，缓冲区的交换区总量

 进程信息

|列名|含义|
|----|----|
|PID|进程id|
|PPID|父进程id|
|RUSER|Real user name|
|UID|进程所有者的用户id|
|USER|进程所有者的用户名|
|GROUP|进程所有者的组名|
|TTY|启动进程的终端名。不是从终端启动的进程则显示为 ?|
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
|S|进程状态。D=不可中断的睡眠状态R=运行S=睡眠T=跟踪/停止Z=僵尸进程|
|COMMAND|命令名/命令行|
|WCHAN|若该进程在睡眠，则显示睡眠中的系统函数名|
|Flags|任务标志，参考 sched.h|


top 的man 命令查看解释

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



