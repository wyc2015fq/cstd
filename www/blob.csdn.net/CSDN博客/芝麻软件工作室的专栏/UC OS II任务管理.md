
# UC-OS II任务管理 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月13日 09:57:55[seven-soft](https://me.csdn.net/softn)阅读数：437个人分类：[uCos-II																](https://blog.csdn.net/softn/article/category/6305029)



UC/OS 初始化
在调用μC/OS-Ⅱ的任何其它服务之前，μC/OS-Ⅱ要求用户首先调用系统初始化函数OSIint()。
OSIint()初始化μC/OS-Ⅱ所有的变量和数据结构（见OS_CORE.C）。
OSInit()建立空闲任务OSTaskIdle（） ，优先级总是最低的
如果统计任务允许OS_TASK_STAT_EN和任务建立扩展允许都设为1，则OSInit()还得建立统计任务OSTaskStat()并且让其进入就绪态。
OSTaskStat的优先级总是设为OS_LOWEST_PRIO-1。
在文件OS_CFG.H中，OS_TASK_STAT_EN是设为1的。           // 是否使能任务统计
在文件OS_CFG.H中，OS_LOWEST_PRIO是设为63的。           // 设置最低优先级
在文件OS_CFG.H中, 最多任务数OS_MAX_TASKS是设成大于2的。// 设置最大任务数（UC/OS II最大为56）
UC/OS 启动
多任务的启动是用户通过调用OSStart()实现的。然而，启动μC/OS-Ⅱ之前，用户至少要建立一个应用任务
程序清单  初始化和启动μC/OS-Ⅱ
void main (void)
{
OSInit();           /* 初始化uC/OS-II                            */
.
.
通过调用OSTaskCreate()或OSTaskCreateExt()创建至少一个任务;
.
.
OSStart();          /* 开始多任务调度!OSStart()永远不会返回 */
}

获取当前UC/ＯＳII 版本号
应用程序调用OSVersion()[程序清单L3.26]可以得到当前μC/OS-Ⅱ的版本号。
OSVersion()函数返回版本号值乘以100。换言之，200表示版本号2.00。
任务管理
任务看起来与任何C函数一样，具有一个返回类型和一个参数，只是它从不返回。
任务的返回类型必须被定义成void型。
函数可以在OS_TASK文件中找到。如前所述，任务必须是以下两种结构之一：
void YourTask (void *pdata)
{
for (;;) {
/* 用户代码 */
调用μC/OS-Ⅱ的服务例程之一:
OSMboxPend();   // 发送邮箱
OSQPend();      //
OSSemPend();
OSTaskDel(OS_PRIO_SELF);      // 任务删除
OSTaskSuspend(OS_PRIO_SELF);  // 任务挂起
OSTimeDly();                  // 延时
OSTimeDlyHMSM();              //
/* 用户代码 */
}
}
void YourTask (void *pdata)
{
/* 用户代码 */
OSTaskDel(OS_PRIO_SELF);   // 删除任务
}
1.建立任务OSTaskCreate()
用 户可以通过传递任务地址和其它参数到以下两个函数之一来建立任务：OSTaskCreate() 或 OSTaskCreateExt()。OSTaskCreate()与μC/OS是向下兼容的，OSTaskCreateExt()是 OSTaskCreate()的扩展版本，提供了一些附加的功能。用两个函数中的任何一个都可以建立任务。任务可以在多任务调度开始前建立，也可以在其它 任务的执行过程中被建立。在开始多任务调度(即调用OSStart())前，用户必须建立至少一个任务。任务不能由中断服务程序(ISR)来建立。
OSTaskCreate()的定义如程序如下所述。从中可以知道，OSTaskCreate()需要四个参数：task是任务代码的指针，pdata是 当任务开始执行时传递给任务的参数的指针，ptos是分配给任务的堆栈的栈顶指针(参看4.02，任务堆栈)，prio是分配给任务的优先级。
现在我们不需要函数代码，只要知道怎么应用就可以了。
INT8U OSTaskCreate (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio)
{
……………
}
2.建立任务，OSTaskCreateExt()
用OSTaskCreateExt()函数来建立任务会更加灵活，但会增加一些额外的开销。
我们可以看到OSTaskCreateExt()需要九个参数！前四个参数(task,pdata,ptos和prio)与 OSTaskCreate()的四个参数完全相同，连先后顺序都一样。这样做的目的是为了使用户能够更容易地将用户的程序从OSTaskCreate() 移植到OSTaskCreateExt()上去。
id参数为要建立的任务创建一个特殊的标识符。该参数在?C/OS以后的升级版本中可能会用到，但在?C/OS-Ⅱ中还未使用。这个标识符可以扩 展?C/OS-Ⅱ功能，使它可以执行的任务数超过目前的64个。但在这里，用户只要简单地将任务的id设置成与任务的优先级一样的值就可以了。
pbos是指向任务的堆栈栈底的指针，用于堆栈的检验。
Stk??_size用于指定堆栈成员数目的容量。也就是说，如果堆栈的入口宽度为4字节宽，那么stk??_size为10000是指堆栈有40000个字节。该参数与pbos一样，也用于堆栈的检验。
pext是指向用户附加的数据域的指针，用来扩展任务的OS_TCB。例如，用户可以为每个任务增加一个名字(参看实例3)，或是在任务切换过程中将浮点寄存器的内容储存到这个附加数据域中，等等。
opt用于设定OSTaskCreateExt()的选项，指定是否允许堆栈检验，是否将堆栈清零，任务是否要进行浮点操作等等。?COS_Ⅱ.H文件 中有一个所有可能选项(OS_TASK_OPT_STK_CHK,OS_TASK_OPT_STK_CLR和OS_TASK_OPT_SAVE_FP)的 常数表。每个选项占有opt的一位，并通过该位的置位来选定(用户在使用时只需要将以上OS_TASK_OPT_???选项常数进行位或(OR)操作就可 以了)。
INT8U OSTaskCreateExt (void (*task)(void *pd),void *pdata, OS_STK *ptos, INT8U prio, INT16U id,
OS_STK  *pbos,INT32U stk_size, void  *pext, INT16U opt)
{
……………………
}
任务堆栈
每个任务都有自己的堆栈空间。堆栈必须声明为OS_STK类型，并且由连续的内存空间组成。用户可以静态分配堆栈空间(在编译的时候分配)也可以动态地分配堆栈空间(在运行的时候分配)。静态堆栈声明如程序清单下，这两种声明应放置在函数的外面。
OS_STK类型定义在 OS_CPU.h 头文件中定义
typedef INT32U         OS_STK;                  /* 堆栈是32位宽度
static OS_STK  MyTaskStack[stack_size];         // 静态堆栈设定，具体大小视程序定
或者
OS_STK  MyTaskStack[stack_size];
用户可以用C编译器提供的malloc()函数来动态地分配堆栈空间。在动态分配中，用户要时刻注意内存碎片问题。特别是当用户反复地建立和删除任务时， 内存堆中可能会出现大量的内存碎片，导致没有足够大的一块连续内存区域可用作任务堆栈，这时malloc()便无法成功地为任务分配堆栈空间。（一般不建 议使用）
如果用户并不会去删除任务，使用malloc()是非常可行的。
OS_STK  *pstk;
pstk = (OS_STK *)malloc(stack_size);
if (pstk != (OS_STK *)0) {            /* 确认malloc()能得到足够地内存空间 */
Create the task； }
堆栈增长问题：
UC/OS支持两种堆栈方式，从上(高地址)往下(低地址)长也可以从下往上长，当OS_CPU.H文件中的OS_STK_GROWTH置为0时，用户需要将堆栈的最低内存地址传递给任务创建函数
对两个方向增长的堆栈都提供支持代码如下
OS_STK  TaskStack[TASK_STACK_SIZE];
\#if OS_STK_GROWTH == 0
OSTaskCreate(task, pdata, &TaskStack[0], prio);
\#else
OSTaskCreate(task, pdata, &TaskStack[TASK_STACK_SIZE-1], prio);
\#endif
任务所需的堆栈的容量是由应用程序指定的。用户在指定堆栈大小的时候必须考虑用户的任务所调用的所有函数的嵌套情况，任务所调用的所有函数会分配的局部变量的数目，以及所有可能的中断服务例程嵌套的堆栈需求。另外，用户的堆栈必须能储存所有的CPU寄存器。
删除任务，OSTaskDel()
有 时候删除任务是很有必要的。删除任务,是说任务将返回并处于休眠状态，并不是说任务的代码被删除了，只是任务的代码不再被?C/OS-Ⅱ调用。通过调用 OSTaskDel()就可以完成删除任务的功能。OSTaskDel()一开始应确保用户所要删除的任务并非是空闲任务，因为删除空闲任务是不允许的。 不过，用户可以删除statistic任务。接着，OSTaskDel()还应确保用户不是在ISR例程中去试图删除一个任务，因为这也是不被允许的。调 用此函数的任务可以通过指定OS_PRIO_SELF参数来删除自己。接下来OSTaskDel()会保证被删除的任务是确实存在的。如果指定的参数是
 OS_PRIO_SELF的话，这一判断过程(任务是否存在)自然是可以通过的，但为这种情况单独写一段代码，只会增加代码并延长程序的执行时间。
INT8U OSTaskDel (INT8U prio)               //  函数原形
{}
请求删除任务，OSTaskDelReq()
函数原形　INT8U OSTaskDelReq (INT8U prio)｛｝
有时候，如果任务A拥有内存缓冲区或信号量之类的资源，而任务B想删除该任务，这些资源就可能由于没被释放而丢失。在这种情况下，用户可以想法子让拥有这些资源的任务在使用完资源后，先释放资源，再删除自己。用户可以通过OSTaskDelReq()函数来完成该功能。
发 出删除任务请求的任务(任务B)和要删除的任务(任务A)都需要调用OSTaskDelReq()函数。任务B需要决定在怎样的情况下请求删除任务A。换 句话说，用户的应用程序需要决定在什么样的情况下删除任务。如果任务需要被删除，可以通过传递被删除任务的优先级来调用OSTaskDelReq()（也 就是任务的ＩＤ号）如果要被删除的任务不存在(即任务已被删除或是还没被建立)，OSTaskDelReq()返回OS_TASK_NOT_EXIST。 如果OSTaskDelReq()的返回值为OS_NO_ERR，则表明请求已被接受但任务还没被删除。用户可能希望任务B等到任务A删除了自己以后才继
 续进行下面的工作，这时用户可以象笔者一样，通过让任务B延时一定时间来达到这个目的
请求删除其它任务的任务(任务B)
void RequestorTask (void *pdata)
{
INT8U err;
pdata = pdata;
for (;;)
{
/* 应用程序代码 */
if ('TaskToBeDeleted()' 需要被删除) {           //确定删除A任务的条件
while (OSTaskDelReq(TASK_TO_DEL_PRIO) != OS_TASK_NOT_EXIST) {   // 等待删除成功
OSTimeDly(1);                                                   (3)
}
}
/*应用程序代码*/                                                       (4)
}
}
需要删除自己的任务(任务A)
void TaskToBeDeleted (void *pdata)
{
INT8U err;
pdata = pdata;
for (;;) {
/*应用程序代码*/
If (OSTaskDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ) {            (1)
释放所有占用的资源;                                                  (2)
释放所有动态内存;
OSTaskDel(OS_PRIO_SELF);                                           (3)
} else
{
/*应用程序代码*/
}
}
}
需 要删除自己的任务(任务A)的代码如程序清单如上。在OS_TAB中存有一个标志，任务通过查询这个标志的值来确认自己是否需要被删除。这个标志的值是通 过调用OSTaskDelReq(OS_PRIO_SELF)而得到的。当OSTaskDelReq()返回给调用者OS_TASK_DEL_REQ时， 则表明已经有另外的任务请求该任务被删除了。在这种情况下，被删除的任务会释放它所拥有的所用资源，并且调用 OSTaskDel(OS_PRIO_SELF)来删除自己。前面曾提到过，任务的代码没有被真正的删除，而只是?C/OS-Ⅱ不再理会该任务代码，换句
 话说，就是任务的代码不会再运行了。但是，用户可以通过调用OSTaskCreate()或OSTaskCreateExt()函数重新建立该任务。

改变任务的优先级，OSTaskChangePrio()
函数原形　INT8U OSTaskChangePrio (INT8U oldprio, INT8U newprio)｛｝
指名参数为当前优先级或OS_PRIO_SELF，用户还必须指定任务的新(即想要的)优先级
OSTaskChangePrio()完成了关键性的步骤后，在新的优先级高于旧的优先级或新的优先级高于调用本函数的任务的优先级情况下，任务调度程序就会被调用。
挂起任务，OSTaskSuspend()
函数原形　INT8U OSTaskSuspend (INT8U prio)｛｝
有时候将任务挂起是很有用的。挂起任务可通过调用OSTaskSuspend()函数来完成。被挂起的任务只能通过调用OSTaskResume() 函数来恢复。任务挂起是一个附加功能。也就是说，如果任务在被挂起的同时也在等待延时的期满，那么，挂起操作需要被取消，而任务继续等待延时期满，并转入 就绪状态。任务可以挂起自己或者其它任务。
恢复任务，OSTaskResume()
函数原形 INT8U OSTaskResume (INT8U prio)
恢复任务不能恢复调用本函数的任务，也就是说ID号不能为OS_PRIO_SELF
获得有关任务的信息，OSTaskQuery()
函数原形 INT8U OSTaskQuery (INT8U prio, OS_TCB *pdata)
用户的应用程序可以通过调用OSTaskQuery()来获得自身或其它应用任务的信息。实际上，OSTaskQuery()获得的是对应任务的 OS_TCB中内容的拷贝。用户能访问的OS_TCB的数据域的多少决定于用户的应用程序的配置(参看OS_CFG.H)。由于?C/OS-Ⅱ是可裁剪 的，它只包括那些用户的应用程序所要求的属性和功能。
要调用OSTaskQuery()，用户的应用程序必须要为OS_TCB分配存储空间。这个OS_TCB与?C/OS-Ⅱ分配的OS_TCB是完全不同的 数据空间。在调用了OSTaskQuery()后，这个OS_TCB包含了对应任务的OS_TCB的副本。用户必须十分小心地处理OS_TCB中指向其它 OS_TCB的指针(即OSTCBNext与OSTCBPrev)；用户不要试图去改变这些指针！一般来说，本函数只用来了解任务正在干什么——本函数是 有用的调试工具。
得到任务的信息
OS_TCB MyTaskData;
void MyTask (void *pdata)
{
INT8U err;
pdata = pdata;
for (;;) {
/*  用户代码                   */
err = OSTaskQuery(10, &MyTaskData);
/* Examine error code ..        */
/*  用户代码                    */
}

