
# 漫画-Linux中断子系统综述 - 嵌入式Linux - CSDN博客

2019年01月21日 09:12:20[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：83



# 1、中断引发的面试教训
![](https://img-blog.csdnimg.cn/20190121202656190.png)
![](https://img-blog.csdnimg.cn/20190120225544887.png)
![](https://img-blog.csdnimg.cn/20190121200552243.png)
![](https://img-blog.csdnimg.cn/20190121172710366.png)![](https://img-blog.csdnimg.cn/20190120230515496.png)
[
](https://img-blog.csdnimg.cn/20190121172710366.png)![](https://img-blog.csdnimg.cn/20190120232646458.png)
# 2、什么是中断？
**中断：**（英语：Interrupt）指当出现需要时，CPU暂时停止当前程序的执行转而执行处理新情况的程序和执行过程。
即在程序运行过程中，系统出现了一个必须由CPU立即处理的情况，此时，CPU暂时中止程序的执行转而处理这个新的情况的过程就叫做中断。
**中断**在嵌入式软件中几乎不能离开它，你看到的几乎所有的芯片都有中断功能，不管是裸机程序STC89C51还是嵌入式Linux系统，还是其他的RTOS系统都有中断。每个外接设备申请一个唯一的中断号，让外设发生中断时，向CPU报告自己的中断号，CPU知道是哪个设备发生中断，然后执行相对应的操作。
![](https://img-blog.csdnimg.cn/20190121203016727.png)
# 3、为什么要用中断？
![](https://img-blog.csdnimg.cn/20190123082842163.png)
# 4、Linux中断子系统
![](http://plck4ljzb.bkt.clouddn.com/Fms1NYRZ9RuAVqN3MUZCbG8Fv9ms)
## 4.1、中断系统相关硬件描述
![](https://img-blog.csdnimg.cn/20190121111639264.png)
![](https://img-blog.csdnimg.cn/20190121202418724.png)
![](https://img-blog.csdnimg.cn/20190121113638534.png)
### 4.1.1、多个Interrupt controller和多个cpu之间的拓扑结构
![](https://img-blog.csdnimg.cn/20190121160950280.png)
第一种，就是有一个Root GIC(Generic Interrupt Controller)连接多个CPU，然后second GIC 接在 Root GIC上，Sencond GIC只负责上报中断信息给Root GIC，Root GIC负责接收Second GIC中断信息然后给CPU汇报中断信息。
![](https://img-blog.csdnimg.cn/20190121161324767.png)
第二种就是Root GIC 负责给cpu0-cpu4汇报工作，Second GIC负责给cpu3-cpu7汇报工作。这样设计的弊端就是Second GIC上的中断信号不能传给Cpu0-Cpu3，这样的设计理论上不是很完美。
![](https://img-blog.csdnimg.cn/20190121161506186.png)
4.1.2、Interrupt controller把中断事件送给哪个CPU？
![](https://img-blog.csdnimg.cn/20190121161840976.png)
在 SMP 体系结构中（cpu三大架构 numa smp mpp 之一），我们可以通过调用系统调用和一组相关的宏来设置 CPU 亲和力（CPU affinity），将一个或多个进程绑定到一个或多个处理器上运行。
中断在这方面也毫不示弱，也具有相同的特性。中断亲和力是指将一个或多个中断源绑定到特定的 CPU 上运行。中断亲和力最初由Ingo Molnar设计并实现。
在 /proc/irq 目录中，对于已经注册中断处理程序的硬件设备，都会在该目录下存在一个以该中断号命名的目录IRQ\#，IRQ\#目录下有一个smp_affinity文件（SMP 体系结构才有该文件），它是一个 CPU 的位掩码，可以用来设置该中断的亲和力， 默认值为 0xffffffff，表明把中断发送到所有的 CPU 上去处理。如果中断控制器不支持 IRQ affinity,不能改变此默认值，同时也不能关闭所有的 CPU 位掩码，即不能设置成 0x0。
举个栗子
我们以网卡（eth1，中断号 44 ）为例，在具有 8 个 CPU 的服务器上来设置网卡中断的亲和力（以下数据出自内核源码 Documentation\IRQ-affinity.txt）：
[root@moon 44]\# cat smp_affinity
ffffffff
[root@moon 44]\# echo 0f > smp_affinity
[root@moon 44]\# cat smp_affinity
0000000f
[root@moon 44]\# ping -f h
PING hell (195.4.7.3): 56 data bytes
...
--- hell ping statistics ---
6029 packets transmitted, 6027 packets received, 0% packet loss
round-trip min/avg/max = 0.1/0.1/0.4 ms
[root@moon 44]\# cat /proc/interrupts | grep 44:
 44:   0   1785   1785   1783   1783   1   1   0   IO-APIC-level   eth1
[root@moon 44]\# echo f0 > smp_affinity
[root@moon 44]\# ping -f h
PING hell (195.4.7.3): 56 data bytes
..
--- hell ping statistics ---
2779 packets transmitted, 2777 packets received, 0% packet loss
round-trip min/avg/max = 0.1/0.5/585.4 ms
[root@moon 44]\# cat /proc/interrupts | grep 44:
 44:  1068  1785  1785  1784   1784   1069   1070   1069   IO-APIC-level  eth1
[root@moon 44]\#在上例中，我们首先只允许在 CPU0-3 上处理网卡中断，接着运行 ping 程序，不难发现在 CPU4-7 上并没有对网卡中断进行处理。然后只在 CPU4-7 上对网卡中断进行处理， CPU0-3 不对网卡中断进行任何处理，运行 ping 程序之后，再次查看/proc/interrupts文件时，不难发现 CPU4~7 上的中断次数明显增加，而 CPU0~3 上的中断次数没有太大的变化。
![](https://img-blog.csdnimg.cn/20190123163943633.png)
4.2、中断子系统相关的软件框架
![](https://img-blog.csdnimg.cn/20190121163235395.png)
4.2.1、中断上半部分和下半部分
中断处理程序的这两个目标相互冲突：
快点执行
执行大量工作
由于这些竞争目标，中断的处理分为两部分或一半：
上半部分。中断处理程序是上半部分。上半部分在收到中断后立即运行，仅执行对时间要求严格的工作，例如确认收到中断或重置硬件。
下半部分。可以在以后执行的工作推迟到下半部分。下半部分将在更方便的时间运行，并启用所有中断。
4.2.2、中断分类
中断如果分为两大类，那就是同步中断和异步中断，如果根据中断原因来分，可以分为四种。具体如下图
![](https://img-blog.csdnimg.cn/20190121162713286.png)
![](https://img-blog.csdnimg.cn/20190121162929881.png)
4.2.3、中断上下文
执行中断处理程序时，内核处于中断上下文中。
与进程上下文的区别：
进程上下文是内核在代表进程执行时所处的操作模式，例如执行系统调用或运行内核线程。
在进程上下文中，current宏（在内核中，可以通过current宏来获得当前执行进程的task_struct指针）指向关联的任务。
由于进程在进程上下文中耦合到内核，因此进程上下文可以休眠或以其他方式调用调度程序。
current宏在发生中断的时候，它指向中断的进程。
没有后备进程，中断上下文无法休眠，无法重新安排。因此，您无法从中断上下文中调用某些函数。如果函数休眠，则不能从中断处理程序中使用它：这限制了可以从中断处理程序调用的函数。中断上下文是时间关键的，因为中断处理程序会中断其他代码。
应该记住，中断处理程序已经中断了其他代码（可能甚至是另一行上的另一个中断处理程序）。由于异步性质，所有中断处理程序必须尽可能快速和简单。应尽可能地将工作从中断处理程序中推出，并在下半部分执行，后者在更方便的时间运行。
4.2.4、linux kernel的中断子系统相关的软件框
架图如下所示：
![](http://plck4ljzb.bkt.clouddn.com/FsTsy8VpoHz_ECvffRk46mD7I-a7)
由上面的block图，我们可知linux kernel的中断子系统分成4个部分：
[
](http://plck4ljzb.bkt.clouddn.com/FsTsy8VpoHz_ECvffRk46mD7I-a7)（1）硬件无关的代码，我们称之Linux kernel通用中断处理模块。无论是哪种CPU，哪种controller，其中断处理的过程都有一些相同的内容，这些相同的内容被抽象出来，和HW无关。此外，各个外设的驱动代码中，也希望能用一个统一的接口实现irq相关的管理（不和具体的中断硬件系统以及CPU体系结构相关）这些“通用”的代码组成了linux kernel interrupt subsystem的核心部分。
（2）CPU architecture相关的中断处理。 和系统使用的具体的CPU architecture相关。
（3）Interrupt controller驱动代码 。和系统使用的Interrupt controller相关。
（4）普通外设的驱动。这些驱动将使用Linux kernel通用中断处理模块的API来实现自己的驱动逻辑。
[
](http://plck4ljzb.bkt.clouddn.com/FsTsy8VpoHz_ECvffRk46mD7I-a7)4.2.5、中断调试
[
](http://plck4ljzb.bkt.clouddn.com/FsTsy8VpoHz_ECvffRk46mD7I-a7)用示波器查看硬件是否真实产生了中断
cat /proc/interrupts  里面列举了系统申请的所有中断，查看自己注册的中断是否在里面
cat cat /proc/irq/[num]/  里面很多可以查看和调试的参数
[
](http://plck4ljzb.bkt.clouddn.com/FsTsy8VpoHz_ECvffRk46mD7I-a7)4.2.6、Linux中断是否可以嵌套吗？
[
](http://plck4ljzb.bkt.clouddn.com/FsTsy8VpoHz_ECvffRk46mD7I-a7)![](https://img-blog.csdnimg.cn/20190121172435423.png)
![](https://img-blog.csdnimg.cn/20190121171952144.png)
它的commit log清晰地解释中断嵌套可能引入的一些risk，比如stack溢出等。也就是说，从这个commit开始，实际Linux已经不再支持中断的嵌套, 也没有快慢中断的概念了，IRQF_DISABLED标记也作废了。在IRQ HANDLER里面，无论一个中断设置还是不设置IRQF_DISABLED, 内核都不会开启CPU对中断的响应
具体查看链接：
[https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=e58aa3d2d0cc](https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/commit/?id=e58aa3d2d0cc)
5、后续
![](http://plck4ljzb.bkt.clouddn.com/FglsATm4fFxLqg9taa_zVrmq3UOn)
![](https://img-blog.csdnimg.cn/20190121204057131.png)
6、参考
[http://www.wowotech.net/irq_subsystem/interrupt_subsystem_architecture.html/comment-page-2](http://www.wowotech.net/irq_subsystem/interrupt_subsystem_architecture.html/comment-page-2)
[https://www.ibm.com/developerworks/cn/linux/l-cn-linuxkernelint/index.html](https://www.ibm.com/developerworks/cn/linux/l-cn-linuxkernelint/index.html)
[https://notes.shichao.io/lkd/ch7/\#interrupt-control](https://notes.shichao.io/lkd/ch7/#interrupt-control)

[
  ](http://plck4ljzb.bkt.clouddn.com/FglsATm4fFxLqg9taa_zVrmq3UOn)