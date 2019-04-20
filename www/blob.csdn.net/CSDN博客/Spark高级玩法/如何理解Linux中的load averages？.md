# 如何理解Linux中的load averages？ - Spark高级玩法 - CSDN博客
2018年06月24日 00:01:49[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：613
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/UicsouxJOkBfoicz6BR0hthzDgOCOPGZ8ia7G6ZJumEia7NCSuV9yknpprTQbZ080xMVOPcWIdxdrTFppGsFhzcw9Q/640?wx_fmt=jpeg)
经常和 Linux 打交道的童鞋都知道，load averages 是衡量机器负载的关键指标，但是这个指标是怎样定义出来的呢？
和其他系统不同，Linux 上的 load averages 不仅追踪可运行的任务，还追踪处于不可中断睡眠状态的任务，为什么是这样呢？这篇文章就来聊聊这方面的知识。
Linux 的 load averages 是系统负载平均值，这个值将正在运行线程（任务）对于系统的需求，作为处于运行和等待状态的线程的平均数量。大多数工具会显示 1 分钟，5 分钟和 15 分钟的平均值：
`$ uptime 17:30:01 up 13 days, 20:30,  3 users,  load average: 1.66, 2.03, 2.08$ cat /proc/loadavg1.48 1.98 2.06 4/3587 117385`
对上面的输出信息稍稍做些解释
- 
如果平均值是 0.0，说明系统处于空闲状态
- 
如果 1 分钟的平均值大于 5 分钟或者 15 分钟，说明系统负载正在增加
- 
如果 1 分钟的平均值小于 5 分钟或者 15 分钟，说明系统负载正在减小
- 
如果这些值大于 CPU 的核数，说明可能遇到了性能问题
利用这三个值，我们可以判断系统的负载是在增加还是在减小，这在实践中很有用。这三个中的任意一个拿出来也很有用，比如为云服务的自动伸缩设置阈值。不过，在缺少其他信息的情况下，单看这些值是没有意义的。比如 1 分钟的 load averages 值在 23 到 25 之间，就没有任何意义；但如果知道 CPU 核数并且知道运行的任务是计算密集型，那这个值就很有意义。
历史
最开始的时候，load averages 只显示对系统 CPU 相关的需求：运行的进程数加上等待的进程数。如 RFC 546 描述的：
TENEX load averages 是衡量 CPU 需求的指标。这个值是给定时间内可运行进程数量的平均值。例如，对于单核 CPU 系统，每小时平均 10 次意思是在该小时内可以期望看到一个进程正在运行和另外九个等待 CPU(即没有被 I/O 阻塞) 处于 ready 状态的进程。
下图是 1973 年绘制的监控图：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/UicsouxJOkBfoicz6BR0hthzDgOCOPGZ8iarEe0X2sDarjnnsqtMVNA7OP3xQCOiciajWoExibdJlPQhHxauiaDgmwDzg/640?wx_fmt=jpeg)
以前操作系统的代码还可以找到，下面是 TENEX) 定义的一些宏：
`NRJAVS==3               ;NUMBER OF LOAD AVERAGES WE MAINTAINGS RJAV,NRJAVS          ;EXPONENTIAL AVERAGES OF NUMBER OF ACTIVE PROCESSES[...];UPDATE RUNNABLE JOB AVERAGESDORJAV: MOVEI 2,^D5000        MOVEM 2,RJATIM          ;SET TIME OF NEXT UPDATE        MOVE 4,RJTSUM           ;CURRENT INTEGRAL OF NBPROC+NGPROC        SUBM 4,RJAVS1           ;DIFFERENCE FROM LAST UPDATE        EXCH 4,RJAVS1        FSC 4,233               ;FLOAT IT        FDVR 4,[5000.0]         ;AVERAGE OVER LAST 5000 MS[...];TABLE OF EXP(-T/C) FOR T = 5 SEC.EXPFF:  EXP 0.920043902 ;C = 1 MIN        EXP 0.983471344 ;C = 5 MIN        EXP 0.994459811 ;C = 15 MIN`
Linux 中定义的宏长下面这样（代码出处 include/linux/sched/loadavg.h）:
`#define EXP_1           1884            /* 1/exp(5sec/1min) as fixed-point */#define EXP_5           2014            /* 1/exp(5sec/5min) */#define EXP_15          2037            /* 1/exp(5sec/15min) */`指标的三个粒度
load averages 有 1 分钟，5 分钟，15 分钟三个粒度的结果。不过事实上，他们并不是真正的平均值，统计的粒度也不是 1，5，15 分钟。从上面的代码中可以看出，1，5 和 15 都是常量，用于计算指数衰减的 5 秒平均移动和。由此算出的 1 分钟，5 分钟和 15 分钟的 load averages 所反应的负载远远超过 1，5，15 分钟。
假设在一个空闲的系统上，开启一个单线程来跑 CPU 密集任务，60 秒后 1 分钟的 load averages 是多少呢？如果 load averages 按普通平均值来算，这个值将是 1.0. 下面是一个绘制成图的实验结果：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/UicsouxJOkBfoicz6BR0hthzDgOCOPGZ8iaoicUAQcTWmGaRc8jslzQU8mwelWoMSEKDzks39icRgFdef7RtYibr0p5Q/640?wx_fmt=png)
在上面的实验中，所谓的“1 分钟 load averages”在一分钟内只能达到 0.62 左右。
Linux 不可中断任务
Linux 中刚引入 load averages 时，和其他系统一样将其作为衡量 CPU 需求的指标，后来将其更改为不仅包含可运行任务，还包含处于不可中断状态的任务（TASK_UNINTERRUPTIBLE 或 nr_uninterruptible）。这种状态由希望避免信号中断的代码使用，其中包括阻塞在磁盘 I/O 和一些锁上的任务。在`ps`和`top`的输出中，这种状态被标志为“D”。ps(1) 的 man page 将其称为"不可中断睡眠状态（通常被 IO 阻塞）"
`# man ps.....PROCESS STATE CODES       Here are the different values that the s, stat and state output specifiers (header "STAT" or "S") will display to describe the       state of a process:               D    uninterruptible sleep (usually IO)...`
为什么 Linux 中的 load averages 要加入不可中断状态呢，而不是像其他系统一样只计算 CPU 的需求呢？
加入不可中断的起源
在 oldlinux.org 找到了一封 1993 年的邮件：
`From: Matthias Urlichs <urlichs@smurf.sub.org>Subject: Load average broken ?Date: Fri, 29 Oct 1993 11:37:23 +0200The kernel only counts "runnable" processes when computing the load average.I don't like that; the problem is that processes which are swapping orwaiting on "fast", i.e. noninterruptible, I/O, also consume resources.It seems somewhat nonintuitive that the load average goes down when youreplace your fast swap disk with a slow swap disk...Anyway, the following patch seems to make the load average much moreconsistent WRT`` the subjective speed of the system. And, most important, theload is still zero when nobody is doing anything. ;-)--- kernel/sched.c.orig Fri Oct 29 10:31:11 1993+++ kernel/sched.c  Fri Oct 29 10:32:51 1993@@ -414,7 +414,9 @@    unsigned long nr = 0;    for(p = &LAST_TASK; p > &FIRST_TASK; --p)-       if (*p && (*p)->state == TASK_RUNNING)+       if (*p && ((*p)->state == TASK_RUNNING) ||+                  (*p)->state == TASK_UNINTERRUPTIBLE) ||+                  (*p)->state == TASK_SWAPPING))            nr += FIXED_1;    return nr; }--Matthias Urlichs         XLink-POP N|rnberg   | EMail: urlichs@smurf.sub.orgSchleiermacherstra_e 12    Unix+Linux+Mac     | Phone: ...please use email.90491 N|rnberg (Germany)     Consulting+Networking+Programming+etc'ing      42`
看到这么久之前的想法还是很令人惊叹的。
这也证明了 Linux 改变 load averages 的含义，使其不仅体现对 CPU 的需要，是有意的，这让 load averages 从“CPU 负载均衡”变成了“系统负载均衡”。
邮件中举交换磁盘速度慢的例子是有道理的：通过降低系统性能，系统需求（运行和排队的进程数）应该增加；但是如果仅仅根据 CPU 运行状态，那么 load averages 值应该会下降。Matthias 认为这是不直观的，所以修改了代码。
现代系统的不可中断
但是难道不会出现磁盘 I/O 不能解释 Linux load averages 过高的情况吗？这种情况是会出现的，这是因为在现代 Linux（4.12）版本中，有将近 400 处代码设置了`TASK_UNINTERRUPTIBLE`状态，包括一些锁原语中。其中部分代码可能不需要统计在 load averages 中。
既然`TASK_UNINTERRUPTIBLE`在更多的地方被用到，那么是否应该将 load averages 改成只统计 CPU 和磁盘需求呢？Linux 调度程序的维护者 Peter Zijstra 有一个想法：将`TASK_UNINTERRUPTIBLE`替换成`task_struct->in_iowait`，这样 load averages 就更贴近磁盘 I/O 的需求。这样又引入了另外一个问题，我们到底想要从 load averages 中得到什么？我们是需要用线程对系统的需求来衡量负载，还是只通过物理资源的使用情况来衡量负载呢？如果是前者的话，那么应该包含等待不间断锁的线程，因为这些线程并没有闲置。所以也许 Linux 的 load averages 已经按我们需要的方式工作了。
理解 Linux 的 load averages
也许真正的问题在于“load averages”这个词和“I/O”一样含糊不清。到底是哪种 I/O 呢？是磁盘 I/O？文件系统 I/O？还是网络 I/O。类似的，到底是哪种 load averages 呢？是 CPU 平均负载？还是系统平均负载？下面做一个总结吧：
- 
在 Linux 上，load averages 的真实含义是“系统平均负载”，即对整个系统，测量正在工作并等待工作的线程数（CPU，磁盘，不可中断锁）。换句话说，这种方式测量的是不完全空闲的线程数量。这种方式的优势在于包括了对不同资源的需求。
- 
在其他的系统上，load averages 的含义是“CPU 平均负载”，这组值用于测量正在占有 CPU 执行权的线程数量加上等待 CPU 的线程数量。
还有另一种可能的类型：“物理资源负载平均值”，其中包括仅用于物理资源（CPU+ 磁盘）的负载。
更精确的测量数据
当 Linux 的 load averages 值增加时，可以判断任务对系统资源（CPU，磁盘和锁）有了更高的需求，但是到底是对哪种资源的需求增长了呢？这时可以用其他的指标来进行判断。比如，CPU 资源有如下指标：
- 
单个 CPU 使用率：可以用命令`mpstat -P ALL 1`查看
- 
每个进程的 CPU 使用率：可用命令`top`，`pidstat 1`查看
- 
每个线程运行队列（调度程序）延迟：可用命令`perf sched`查看，也可以查看文件`/proc/PID/schedstats`
- 
CPU 运行队列延迟：可用命令`perf sched`查看，也可以查看文件`/proc/schedstat`
- 
CPU 运行队列长度：可用`vmstat 1`命令查看。
上面提供的指标中，前两个用来衡量使用率，后三个用来度量系统饱和度。利用率指标对于衡量工作负载很有用，而饱和度指标可用来识别性能问题。衡量 CPU 饱和度的最佳指标是运行队列（或调度程序）的延迟，延迟是指任务或者线程处于可运行状态，但必须等待 CPU 的时间。通过这样的指标可以用来衡量性能问题的严重程度，比如线程等待调度的时间在运行时间中占的百分比。通过观察运行队列长度可以很方便判断是否存在问题，但比较难定位到问题产生的原因。
`schedstats`功能在 Linux 4.6 中成为内核可调参数（sysctl.kernel.sched_schedstats），默认是关闭的。
尽管有更明确的指标，但并不意味着 load averages 是无用的。这组指标已经成功用于云计算微服务的扩展策略，微服务根据不同的负载值做出反应。有了这些判断的依据，即使在自动扩容时犯错也保险多了：扩容实例会花更多的钱，不扩容则会损失用户。如果扩容太多，后来调查一下纠正就是了。
总结
在 1993 年，一位 Linux 工程师发现了一个非直观的 load averages 情况，于是提交了三行代码的补丁将 load averages 的含义由“CPU 负载平均值”变成了“系统负载平均值”。这次的变动在统计中包括了不可中断状态下的任务，所以 load averages 值不仅反映了对 CPU 的需求，还反映了对磁盘资源的需求。系统平均负载计算正在工作和等待工作的线程的数量，并且统计 1 分钟，5 分钟，15 分钟指数衰减的移动总和平均值。通过这三个值，能够知道系统的负载是在增加还是在减小。
Linux 中对不可中断状态的使用越来越多，现在已经包括了不可中断的锁原语。如果需要衡量处于运行状态和等待状态的线程对于系统的需求，那么 load averages 依然是很好的指标。
最后引用 kernel/sched/loadavg.c 头部的注释来结束吧。
- 
This file contains the magic bits required to compute the global loadavg
- 
figure. Its a silly number but people think its important. We go through
- 
great pains to make it work on big machines and tickless kernels.
如果，Google 早已解决不了你的问题。
如果，你还想知道 Apple、Facebook、IBM、阿里等国内外名企的核心架构设计。
来，我们在深圳准备了知识星球，想助你成长：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFU7Oic5ibSSV5sJGB2RgzSeqQpuBicxOPgCBglwQPt9rnatXN0VblKhw9WicqSGsuTU9o0Sj12698QpuQ/640?wx_fmt=jpeg)
