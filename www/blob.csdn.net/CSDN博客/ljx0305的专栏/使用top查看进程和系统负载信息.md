# 使用top查看进程和系统负载信息 - ljx0305的专栏 - CSDN博客
2012年09月28日 17:02:31[ljx0305](https://me.csdn.net/ljx0305)阅读数：5908
**引言     **
使用top命令，可以查看正在运行的进程和系统负载信息，包括cpu负载、内存使用、各个进程所占系统资源等，top可以以一定频率更新这些统计信息。下面我们来学习top命令的具体使用方法。
**默认输出**
top命令的默认输出如下：
```
[]()
top - 08:27:48 up 21:43,  2 users,  load average: 1.48, 1.47, 1.39
Tasks: 228 total,   2 running, 226 sleeping,   0 stopped,   0 zombie
Cpu(s):  0.1%us,  2.5%sy,  0.0%ni, 97.2%id,  0.0%wa,  0.0%hi,  0.2%si,  0.0%st
Mem:     15945M total,     1726M used,    14219M free,       48M buffers
Swap:     8197M total,        0M used,     8197M free,     1196M cached
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
   22 root      20   0     0    0    0 R  1.9  0.0  60:23.34 ksoftirqd/9
 8689 root      20   0  273m  67m  11m S  0.0  0.4   2:04.01 java
11058 root      39  19     0    0    0 S  0.0  0.0   1:45.68 kipmi0
11771 root     -98   0 20388  19m 7256 S  0.0  0.1   0:16.06 had
    3 root      RT   0     0    0    0 S  0.0  0.0   0:00.00 migration/0
[]()
```
以上输出上下分成两部分，上半部分显示了整体系统负载情况，各行含义如下：
- **top一行**：从左到右依次为当前系统时间，系统运行的时间，系统在之前1min、5min和15min内cpu的平均负载值
- **Tasks一行**：该行给出进程整体的统计信息，包括统计周期内进程总数、运行状态进程数、休眠状态进程数、停止状态进程数和僵死状态进程数
- **Cpu(s)一行**：cpu整体统计信息，包括用户态下进程、系统态下进程占用cpu时间比，nice值大于0的进程在用户态下占用cpu时间比，cpu处于idle状态、wait状态的时间比，以及处理硬中断、软中断的时间比
- **Mem一行**：该行提供了内存统计信息，包括物理内存总量、已用内存、空闲内存以及用作缓冲区的内存量
- **Swap一行**：虚存统计信息，包括交换空间总量、已用交换区大小、空闲交换区大小以及用作缓存的交换空间大小
下半部分显示了各个进程的运行情况，各列含义如下：
- **PID**: 进程pid
- **USER**: 拉起进程的用户
- **PR**: 该列值加100为进程优先级，若优先级小于100，则该进程为实时(real-time)进程，否则为普通(normal)进程，实时进程的优先级更高，更容易获得cpu调度，以上输出结果中，java进程优先级为120，是普通进程，had进程优先级为2，为实时进程，migration 进程的优先级RT对应于0，为最高优先级
- **NI**: 进程的nice优先级值，该列中，实时进程的nice值为0，普通进程的nice值范围为-20~19
- **VIRT**: 进程所占虚拟内存大小（默认单位kB）
- **RES**: 进程所占物理内存大小（默认单位kB）
- **SHR**: 进程所占共享内存大小（默认单位kB）
- **S**: 进程的运行状态
- **%CPU**: 采样周期内进程所占cpu百分比
- **%MEM**: 采样周期内进程所占内存百分比
- **TIME+**: 进程使用的cpu时间总计
- **COMMAND**: 拉起进程的命令
top命令默认输出提供了很多信息，为获取我们关注的信息，可以使用top命令选项，也可以在top运行中使用快捷键过滤输出信息、根据cpu使用率等值对进程进行排序，下面给出具体使用例子。
**对输出结果进行排序**
top输出的下半部分有很多进程信息列，我们可以根据这些列对进程输出进行排序，默认情况下根据cpu使用率对进程排序。在top命令执行时，**按”O”**，可以看到排序项以及相应的按键：
```
[]()
Current Sort Field:  K  for window 1:Def
Select sort field via field letter, type any other key to return
  a: PID        = Process Id
  b: PPID       = Parent Process Pid
  c: RUSER      = Real user name
  d: UID        = User Id
  ……
[]()
```
各项默认以从大到小排序，top运行中，**按”R”**可反向显示输出结果。
**显示特定用户相关进程**
在top运行时，**按”u”**快捷键，可以过滤出特定用户拉起的进程：
```
Which user (blank for all): sshusr
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
15561 sshusr    20   0  3700  356  276 R  8.3  0.0   5:26.05 a.out
15492 sshusr    20   0 81932 1868 1088 S  0.0  0.0   0:00.00 sash
15493 sshusr    20   0 13840 2872 1580 S  0.0  0.0   0:00.04 bash
```
执行top命令时，使用**”top -u sshusr”**可以起到相同效果。
**显示特定进程**
执行top时，使用**”-p”选项**可以指定要显示的进程：
```
# top -p 15561,15492
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
15561 sshusr    20   0  3700  356  276 R  8.3  0.0  49:03.57 a.out
15492 sshusr    20   0 81932 1868 1088 S  0.0  0.0   0:00.00 sshd
```
**显示所有cpu核使用情况**
默认情况下，top上半部分资源负载信息中，显示的是cpu整体的使用情况：
```
top - 10:33:33 up 23:49,  7 users,  load average: 2.02, 2.12, 2.17
Tasks: 259 total,   3 running, 256 sleeping,   0 stopped,   0 zombie
**Cpu(s):  ****8.5%us,  2.4%sy,  0.0%ni, 88.9%id,  0.0%wa,  0.0%hi,  0.3%si,  0.0%st**
```
**按数字”1”**快捷键，可以开启/关闭显示各个cpu核的信息：
```
Cpu0  :  0.0%us,  0.3%sy,  0.0%ni, 99.7%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st
Cpu1  :  0.0%us,  0.0%sy,  0.0%ni,100.0%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st
Cpu2  :  0.3%us,  0.0%sy,  0.0%ni, 99.7%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st
Cpu3  :100.0%us,  0.0%sy,  0.0%ni,  0.0%id,  0.0%wa,  0.0%hi,  0.0%si,  0.0%st
```
**更改top命令显示频率**
默认情况下，top命令以3秒的频率更新输出结果，top命令执行时，**按”d”**可以更新top更新频率：
```
Change delay from 3.0 to: 1
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
15561 sshusr    20   0  3700  356  276 R  8.2  0.0  65:53.58 a.out
   26 root      20   0     0    0    0 R  2.8  0.0 106:13.28 ksoftirqd/11
```
执行top命令时，使用**”-d”选项**也可以指定top输出跟新频率。另外，按**空格键**，可以手动更新输出。
**显示命令的全路径和参数**
top运行时，**按”c”**快捷键，将显示命令的全路径以及命令参数：
```
15561 sshusr    20   0  3700  356  276 R  8.3  0.0  73:40.02 ./a.out
11088 root      20   0  220m 7196 3000 S  0.5  0.0  78:48.42 /opt/HPI/HPI_ALARM/runHpiAlarm
11974 root      20   0  9252 3244 1844 S  0.0  0.0   0:51.01 /opt/VRTSvcs/bin/HostMonitor -type HostMonitor -agdir /
```
**指定显示的进程数目**
top默认依据窗口大小显示进程数，有时我们对进程排序后，只想关注排在前面的几个进程，使用**”-n”**快捷键可以达到目的：
```
Maximum tasks = 0, change to (0 is unlimited): 2
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
    4 root      20   0     0    0    0 R  2.1  0.0  25:27.25 ksoftirqd/0
11088 root      20   0  220m 7196 3000 S  0.7  0.0  80:17.93 runHpiAlarm
```
**在top中向进程发送信号**
top运行时，**按”k”**快捷键，可向指定进程发送信号，默认信号为SIGTERM，该信号可中止进程：
```
PID to kill: 15561
Kill PID 15561 with signal [15]:
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
15561 sshusr    30  10  3700  356  276 R  8.2  0.0  95:33.45 a.out
   18 root      20   0     0    0    0 R  2.8  0.0  41:48.24 ksoftirqd/7
```
**更改进程的nice优先级值**
top运行时，**按”r”**快捷键，可以修改指定进程的nice优先级：
```
PID to renice: 17898
Renice PID 17898 to value: 10
  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
17898 sshusr    20   0  3700  356  276 R  8.3  0.0   0:03.96 a.out
    4 root      20   0     0    0    0 R  2.0  0.0  25:51.30 ksoftirqd/0
```
**保存top配置**
为看到我们关注的进程、信息，用了很多快捷键，若下次运行top时，想要看到之前关心的内容，又要敲一遍快捷键，这相当不方便。使用**”W”**快捷键，可以把当前显示配置保存到~/.toprc文件中。 
例如以1秒的频率查看前10个占用内存最多的进程，我们需要敲下列快捷键：
```
**d**: Change delay from 3.0 to: 1**n**: Maximum tasks = 0, change to (0 is unlimited): 10**M**: 快捷键”M”指示进程根据内存使用率从大到小排序
```
之后按下”W”可以对配置进行保存：
```
**W**: Wrote configuration to '/root/.toprc'
```
退出再执行top命令，可以看到top以1秒的频率查看前10个占用内存最多的进程，与我们之前保存的配置相符。
引用:[http://www.cnblogs.com/bangerlee/articles/2568776.html](http://www.cnblogs.com/bangerlee/articles/2568776.html)
