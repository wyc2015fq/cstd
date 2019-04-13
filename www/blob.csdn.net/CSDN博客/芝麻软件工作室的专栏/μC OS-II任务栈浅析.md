
# μC-OS-II任务栈浅析 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月12日 22:16:50[seven-soft](https://me.csdn.net/softn)阅读数：217个人分类：[uCos-II																](https://blog.csdn.net/softn/article/category/6305029)



和Linux采用进程单元一样，μC/OS-II采用的是任务的方式，而当任务运行被 加载到内存单元时，将占用的内存区域分为代码段、初始化数据段、未初始化数据段、堆、栈五个区域。而其中，栈段是用来存放函数的参数值、局部变量的值和任 务进行上下文切换时存储当前的任务。
栈在实现上是一段连续的内存空间，μC/OS-II采用数组来实现，当一个任务被创建时，需要为该任务创建相应的栈段，并指定该任务的栈顶位置
INT8U  OSTaskCreate (void(*task)(void*p_arg),void*p_arg, OS_STK *ptos, INT8U prio)

第三个参数便是其指定的栈顶位置。而OS_STK便是任务栈空间的数据类型，它的定义如下：
typedefunsignedshortOS_STK;
由此可见，其类型实际上就是短整型。
如何为任务声明一段空间作为任务的栈呢？
\#define TaskStkLengh    64          //定义用户任务堆栈长度
OS_STK  TaskStk[TaskStkLengh];// 定义用户任务的堆栈

当然，栈的长度需要根据具体的任务来进行定义。
在不同的处理器中，数据的存放有采用小端法的，也有采用大端法，μC/OS-II中采用了一个宏来对其进行选择
\#define  OS_STK_GROWTH        1
当值为1时，即栈的增长方向是从高地址到低地址，值为0时，相反；一般X86的处理器都选择的是1，而ARM怎需要根据具体的情况来进行选择，也可以通过编写一小段代码来确定到底是采用了大端法还是小端法。
其中在struct os_tcb中
OS_STK          *OSTCBStkPtr;/* Pointer to current top of stack                              */
其指向任务栈栈顶的指针，可以通过它就能很方便的找到栈顶的位置，进而确定数据的存放的顺序。
而任务控制块TCB将在后面进行详细的讲解。
下面是一个在LPC2131上的一个小例子：
\#include "config.h"
\#define   LED   (1 << 18)             // P1.18控制LED
\#define TaskLEDStkSize  128             // 定义任务TaskLED的堆栈长度
OS_STK  TaskLEDStk [TaskLEDStkSize];// 定义任务TaskLED的堆栈
voidTaskLED(void*data);// 任务TaskLED
intmain (void)
{
OSInit ();
OSTaskCreate (TaskLED,(void*)0, &TaskLEDStk[TaskLEDStkSize - 1], 2);
OSStart ();
return0;
}
voidTaskLED(void*pdata)
{
pdata = pdata;//避免编译警告
TargetInit ();
PINSEL2 = PINSEL2 & (~0x08);// P1[25:16]选择GPIO
IO1DIR |= LED;// 设置LED控制口输出
IO1SET  = LED;
for(;;)
{
IO1CLR = LED;
OSTimeDly(OS_TICKS_PER_SEC / 4);
IO1SET = LED;
OSTimeDly(OS_TICKS_PER_SEC / 2);
}
}
上面就是一个小的程序示例，里面设计的函数后面将进行详细的讲解。

