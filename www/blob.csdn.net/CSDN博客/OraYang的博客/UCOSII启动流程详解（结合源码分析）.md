
# UCOSII启动流程详解（结合源码分析） - OraYang的博客 - CSDN博客

2017年09月03日 14:02:30[OraYang](https://me.csdn.net/u010665216)阅读数：3199


**μC/OS-****Ⅱ****初始化**
在调用μC/OS-Ⅱ的任何其它服务之前，μC/OS-Ⅱ要求用户首先调用系统初始化函数
OSIint()。OSIint()初始化μC/OS-Ⅱ所有的变量和数据结构（见 OS_CORE.C）。OSInit()建立空闲任务 idle task，这个任务总是处于就绪态的。空闲任务 OSTaskIdle（）的优先级总是设成最低，即 OS_LOWEST_PRIO。如果统计任务允许 OS_TASK_STAT_EN 和
任务建立扩展允许都设为 1，则 OSInit()还得建立统计任务 OSTaskStat()并且让其进入就
绪态。OSTaskStat 的优先级总是设为 OS_LOWEST_PRIO-1。
μC/OS-Ⅱ还初始化了 4 个空数据结构缓冲区，每个缓冲区都是单向链
表，允许μC/OS-Ⅱ从缓冲区中迅速得到或释放一个缓冲区中的元素。注意，空任务控制块
在空缓冲区中的数目取决于最多任务数 OS_MAX_TASKS，这个最多任务数是在 OS_CFG.H 文件中定义的。μC/OS-Ⅱ自动安排总的系统任务数 OS_N_SYS_TASKS(见文件μC/OS-Ⅱ.H)。控制块 OS_TCB 的数目也就自动确定了。当然，包括足够的任务控制块分配给统计任务和空闲任务。
**μC/OS-****Ⅱ****的启动**
多任务的启动是用户通过调用 OSStart()实现的。然而，启动μC/OS-Ⅱ之前，用户至少
要建立一个应用任务，如下程序清单所示：
![](https://img-blog.csdn.net/20170903135759610)
OSStart()的代码如下所示：
![](https://img-blog.csdn.net/20170903135800791)
![](https://img-blog.csdn.net/20170903135802227)
当调用 OSStart()时，OSStart()从任务就绪表中找出那个用户建立的优先级最高任务的任务控制块[L3.25(1)]。然后，Start()调用高优先级就绪任务启动函数 OSStartHighRdy()[(2)]，(见汇编语言文件 OS_CPU_A.ASM)，这个文件与选择的微处理器有关。实质上，函数 OSStartHighRdy()是将任务栈中保存的值弹回到CPU 寄存器中，然后执行一条中断返回指令，中断返回指令强制执行该任务代码。OSStartHighRdy()将永远不返回到 OSStart()。

