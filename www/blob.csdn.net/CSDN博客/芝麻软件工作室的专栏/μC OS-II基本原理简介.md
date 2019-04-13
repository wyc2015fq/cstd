
# μC-OS-II基本原理简介 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:15:47[seven-soft](https://me.csdn.net/softn)阅读数：1135


μC/OS-II，意为“微控制器操作系统版本”。其不仅提供了一个完整的嵌入式实时 内核的源代码，而且对这些代码的细节进行了详尽的解释。他不仅告诉读者该实时内核是怎么写的，还解释了为什幺这么写，而且该实时内核可以被方便的移植到几 乎所有的嵌入式应用类CPU上。对于商业应用中哪些昂贵的实时操作系统软件，μC/OS-II是一个很好的学习平台。
说到μC/OS-II，这里先介绍一下嵌入式应用程序的开发模式。基于 单片机的嵌入式系统在软件设计时没有操作系统，而是将应用程序设计成一个死循环程序，系统轮流处理事件，对于事件响应比较高的事件，则使用中断来处理。这 种方法的好处是代码简单、代码量小、事件处理单一，但是在事件响应上得不到保障。建立在操作系统之上的软件设计方法有别于单片机的这种循环系统。 μC/OS-II 是以任务为最小程序单元的，它在内核环境下被分割为独立的且可并行执行的基本逻辑单位的程序(这是其最大的特点)，任何任务不用关心资源的具体管理方法，
 而是交给操作系统，有操作系统来决定，这使得代码的层次很清晰，可以提高程序的执行效率。下面是μC/OS-II 软件设计的基本代码结构：
void Task_one(void *pdata)                                     //任务1的代码
{
}
void Task_two(void *pdata)                                     //任务2的代码
{
}
void Task_three(void *pdata)                                    //任务3的代码
{
}
void main(void)
{
OSInit();                                                             //初始化
OsTaskCreate(task_one...);                               //创建任务1
OsTaskCreate(task_two...);                               //创建任务2
OsTaskCreate(task_three...);                             //创建任务3
......
OSStart();                                                           //启动系统
}
由代码结构可以看出，其层次结构很清晰。
μC/OS-II 微小内核提供了大约14个用户接口函数，如下：
void OSInit(void)；            //初始化μC/OS-II
void OSStart(void);           //启动μC/OS-II 的多任务环境
INT8U OSTaskCreate(void(*task)(void *pd),void *pdata,OS_STK *ptos,INT8U prio);              //建立一个新任务
void OSTimeDly(INT16U ticks);        //将一个任务延时若干个时钟节拍
void OSTimeTick(void);                    //在每次时钟节拍中断服务程序被调用
INT8U OSTaskDel（INT8U prio）;    //删除一个指定优先级的任务
void OSIntEnter(void);                    //通知μC/OS-II 一个中断服务已开始执行
void OSIntExit(void);                       //通知μC/OS-II 一个中断服务已执行完毕
OS_ENTER_CRITICAL和OS_EXIT_CRITICAL()       //函数原型由移植代码决定，是描述进入临界状态所完成的操作和退出临界状态的方法
OS_EVENT *OSSemCreate(INT16U cnt);             //建立并初始化一个信号量
void OSSemPend(OS_EVENT *pevent,INT16U timeout,INT8U *err);            //等待信号量
INT8U OSSemPost(OS_EVENT *PEVENT);            //发送信号量
OS_EVENT *OSSemDel(OS_EVENT *pevent,INT8U opt,INT8U *err);            //删除信号量
具体的接口函数的实现可以参看μC/OS-II 的源码，上面有非常详细的说明。
在μC/OS-II 中，任务有5种状态，分别为睡眠状态、就绪状态、运行状态、等待状态和被中断状态
睡眠状态：任务驻留在程序空间之中，还没有交给μC/OS-II 管理的状态，而将任务交给μC/OS-II 管理是通过调用函数OSTaskCreate()实现的。
就绪状态：任务已经准备好，可以运行了，但由于该任务的优先级比正在运行的任务的优先级低，还暂时不能运行的状态。在μC/OS-II 中，当任务一旦建立，这个任务紧进入了就绪态准备运行。
运行状态：任务掌握了CPU的控制权，正在运行中的状态。
等待状态：任务在等待某一事件的发生还不能运行的状态。
被中断状态：因为中断服务程序正在执行而得不到CPU控制权的状态。
在任意给定时刻，任务的状态一定是这五种状态之一。
以上就是μC/OS-II 系统的一些基本概念和相应的一些机制。

