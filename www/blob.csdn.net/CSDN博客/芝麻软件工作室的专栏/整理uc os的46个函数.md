
# 整理uc-os的46个函数 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月13日 09:26:08[seven-soft](https://me.csdn.net/softn)阅读数：569


Void OSInit(void);
所属文件 OS_CORE.C     调用者 启动代码    开关量 无
OSinit（）初始化μC/OS-Ⅱ，对这个函数的调用必须在调用OSStart（）函数之前，而OSStart（）函数真正开始运行多任务。
Void OSIntEnter(void);
所属文件 OS_CORE.C     调用者 中断      开关量 无
OSIntEnter（）通知μC/OS-Ⅱ一个中断处理函数正在执行，这有助于μC/OS-Ⅱ掌握中断嵌套的情况。OSIntEnter（）函数通常和OSIntExit（）函数联合使用。
注意/警告：在任务级不能调用该函数。如果系统使用的处理器能够执行自动的独立执行读取-修改-写入的操作，那么就可以直接递增中断嵌套层数（OSIntNesting），这样可以避免调用函数所带来的额外的开销。
Void OSIntExit(void);
所属文件 OS_CORE.C    调用者 中断    开关量 无
OSIntExit（） 通知μC/OS-Ⅱ一个中断服务已执行完毕，这有助于μC/OS-Ⅱ掌握中断嵌套的情况。通常OSIntExit（）和OSIntEnter（）联合使 用。当最后一层嵌套的中断执行完毕后，如果有更高优先级的任务准备就绪，μC/OS-Ⅱ会调用任务调度函数，在这种情况下，中断返回到更高优先级的任务而 不是被中断了的任务。
注意/警告：在任务级不能调用该函数。并且即使没有调用OSIntEnter（）而是使用直接递增OSIntNesting的方法，也必须调用OSIntExit（）函数。
Void OSSchedLock(void);
所属文件 OS_CORE.C    调用者 任务或中断   开关量 N/A
OSSchedLock（） 函数停止任务调度，只有使用配对的函数OSSchedUnlock（）才能重新开始内核的任务调度。调用OSSchedLock（）函数的任务独占 CPU，不管有没有其他高优先级的就绪任务。在这种情况下，中断仍然可以被接受和执行（中断必须允许）。OSSchedLock（）函数和 OSSchedUnlock（）函数必须配对使用。μC/OS-Ⅱ可以支持多达254层的OSSchedLock（）函数嵌套，必须调用同样次数的
 OSSchedUnlock（）函数才能恢复任务调度。
注意/警告:任务调用了OSSchedLock（）函数后，决不能再调用可能导致当前任务 挂起的系统函数：OSTimeDly（），OSTimeDlyHMSM（），OSSemPend（），OSMboxPend（），OSQPend（）。因 为任务调度已经被禁止，其他任务不能运行，这会导致系统死锁。
Void OSSchedUnlock(void);
所属文件 OS_CORE.C    调用者 任务或中断   开关量 N/A
在调用了OSSchedLock（）函数后，OSSchedUnlock（）函数恢复任务调度。
注 意/警告:任务调用了OSSchedLock（）函数后，决不能再调用可能导致当前任务挂起的系统函 数：OSTimeDly（），OSTimeDlyHMSM（），OSSemPend（），OSMboxPend（），OSQPend（）。因为任务调度已 经被禁止，其他任务不能运行，这会导致系统死锁。
void OSStart(void);
所属文件 OS_CORE.C    调用者 初始代码  开关量 无
OSStart( )启动μC/OS-II的多任务环境。
注意/警告：在调用OSStart( )之前必须先调用OSInit ( )。在用户程序中OSStart( )只能被调用一次。第二次调用OSStart( )将不进行任何操作。
void OSStatInit (void);
所属文件 OS_CORE.C    调用者 初始代码  开关量 OS_TASK_STAT_EN &&OS_TASK_CREATE_EXT_EN
OSStatInit（）获取当系统中没有其他任务运行时，32位计数器所能达到的最大值。OSStatInit（）的调用时机是当多任务环境已经启动，且系统中只有一个任务在运行。也就是说，该函数只能在第一个被建立并运行的任务中调用。
INT8U OSTaskChangePrio (INT8U oldprio, INT8U newprio);
所属文件 OS_TASK.C    调用者 任务  开关量 OS_TASK_CHANGE_PRIO_EN
OSTaskChangePrio（）改变一个任务的优先级。
参数
oldprio是任务原先的优先级。
newprio 是任务的新优先级。
返回值
OSTaskChangePrio（）的返回值为下述之一：
OS_NO_ERR：任务优先级成功改变。
OS_PRO_INVALID：参数中的任务原先优先级或新优先级大于或等于OS_LOWEST_PRIO。
OS_PRIO_EXIST：参数中的新优先级已经存在。
OS_PRIO_ERR：参数中的任务原先优先级不存在。
注意/警告：参数中的新优先级必须是没有使用过的，否则会返回错误码。在OSTaskChangePrio（）中还会先判断要改变优先级的任务是否存在
INT8U OSTaskCreate(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio);
所属文件 OS_TASK.C    调用者 任务或初始化代码  开关量 无
OSTaskCreate（）建立一个新任务。任务的建立可以在多任务环境启动之前，也可以在正在运行的任务中建立。中断处理程序中不能建立任务。一个任务必须为无限循环结构，且不能有返回点。
OSTaskCreate（）是为与先前的μC/OS版本保持兼容，新增的特性在OSTaskCreateExt（）函数中。
无论用户程序中是否产生中断，在初始化任务堆栈时，堆栈的结构必须与CPU中断后寄存器入栈的顺序结构相同。详细说明请参考所用处理器的手册。
参数
task是指向任务代码的指针。
Pdata指向一个数据结构，该结构用来在建立任务时向任务传递参数。
ptos 为指向任务堆栈栈顶的指针。任务堆栈用来保存局部变量，函数参数，返回地址以及任务被中断时的CPU寄存器内容。任务堆栈的大小决定于任务的需要及预计的 中断嵌套层数。计算堆栈的大小，需要知道任务的局部变量所占的空间，可能产生嵌套调用的函数，及中断嵌套所需空间。如果初始化常量 OS_STK_GROWTH设为1，堆栈被设为从内存高地址向低地址增长，此时ptos应该指向任务堆栈空间的最高地址。反之，如果
 OS_STK_GROWTH设为0，堆栈将从内存的低地址向高地址增长。
prio为任务的优先级。每个任务必须有一个唯一的优先级作为标识。数字越小，优先级越高。
返回值
OSTaskCreate（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_EXIST：具有该优先级的任务已经存在。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
OS_NO_MORE_TCB：系统中没有OS_TCB可以分配给任务了。
注意/警告：任务堆栈必须声明为OS_STK类型。
在任务中必须调用μC/OS提供的下述过程之一：延时等待、任务挂起、等待事件发生（等待信号量，消息邮箱、消息队列），以使其他任务得到CPU。
用户程序中不能使用优先级0，1，2，3，以及OS_LOWEST_PRIO-3, OS_LOWEST_PRIO-2, OS_LOWEST_PRIO-1, OS_LOWEST_PRIO。这些优先级μC/OS系统保留，其余的56个优先级提供给应用程序。
INT8U OSTaskCreateExt(void (*task)(void *pd), void *pdata, OS_STK *ptos,INT8U prio, INT16U id, OS_STK *pbos, INT32U stk_size, void *pext, INT16U opt);
所属文件 OS_TASK.C    调用者 任务或初始化代码  开关量 无
OSTaskCreateExt（） 建立一个新任务。与OSTaskCreate（）不同的是，OSTaskCreateExt（）允许用户设置更多的细节内容。任务的建立可以在多任务环境 启动之前，也可以在正在运行的任务中建立，但中断处理程序中不能建立新任务。一个任务必须为无限循环结构（如下所示），且不能有返回点。
参数
task是指向任务代码的指针。
Pdata指针指向一个数据结构，该结构用来在建立任务时向任务传递参数。下例中说明μC/OS中的任务代码结构以及如何传递参数pdata：（如果在程序中不使用参数pdata，为了避免在编译中出现“参数未使用”的警告信息，可以写一句pdata= pdata；）
ptos 为指向任务堆栈栈顶的指针。任务堆栈用来保存局部变量，函数参数，返回地址以及中断时的CPU寄存器内容。任务堆栈的大小决定于任务的需要及预计的中断嵌 套层数。计算堆栈的大小，需要知道任务的局部变量所占的空间，可能产生嵌套调用的函数，及中断嵌套所需空间。如果初始化常量OS_STK_GROWTH设 为1，堆栈被设为向低端增长（从内存高地址向低地址增长）。此时ptos应该指向任务堆栈空间的最高地址。反之，如果OS_STK_GROWTH设为0，
 堆栈将从低地址向高地址增长。
prio为任务的优先级。每个任务必须有一个唯一的优先级作为标识。数字越小，优先级越高。id是任务的标识，目前这个参数没有实际的用途，但保留在OSTaskCreateExt（）中供今后扩展，应用程序中可设置id与优先级相同。
pbos 为指向堆栈底端的指针。如果初始化常量OS_STK_GROWTH设为1，堆栈被设为从内存高地址向低地址增长。此时pbos应该指向任务堆栈空间的最低 地址。反之，如果OS_STK_GROWTH设为0，堆栈将从低地址向高地址增长。pbos应该指向堆栈空间的最高地址。参数pbos用于堆栈检测函数 OSTaskStkChk（）。
stk_size 指定任务堆栈的大小。其单位由OS_STK定义：当OS_STK的类型定义为INT8U、INT16U、INT32U的时候， stk_size的单位为分别为字节（8位）、字（16位）和双字（32位）。pext是一个用户定义数据结构的指针，可作为TCB的扩展。例如，当任务 切换时，用户定义的数据结构中可存放浮点寄存器的数值，任务运行时间，任务切入次数等等信息。opt存放与任务相关的操作信息。opt的低8位由
 μC/OS保留，用户不能使用。用户可以使用opt的高8位。每一种操作由opt中的一位或几位指定，当相应的位被置位时，表示选择某种操作。
当前的μC/OS版本支持下列操作：
OS_TASK_OPT_STK_CHK：决定是否进行任务堆栈检查。
OS_TASK_OPT_STK_CLR：决定是否清空堆栈。
OS_TASK_OPT_SAVE_FP：决定是否保存浮点寄存器的数值。此项操作仅当处理器有浮点硬件时有效。保存操作由硬件相关的代码完成。
其他操作请参考文件uCOS_II.H。
返回值
OSTaskCreateExt（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_EXIST：具有该优先级的任务已经存在。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
OS_NO_MORE_TCB：系统中没有OS_TCB可以分配给任务了。
注意/警告：任务堆栈必须声明为OS_STK类型。
在任务中必须进行μC/OS提供的下述过程之一：延时等待、任务挂起、等待事件发生（等待信号量，消息邮箱、消息队列），以使其他任务得到CPU。
用户程序中不能使用优先级0，1，2，3，以及OS_LOWEST_PRIO-3, OS_LOWEST_PRIO-2, OS_LOWEST_PRIO-1, OS_LOWEST_PRIO。这些优先级μC/OS系统保留，其余56个优先级提供给应用程序。
INT8U OSTaskDel (INT8U prio);
所属文件 OS_TASK.C 调用者 任务 开关量 OS_TASK_DEL_EN
OSTaskDel（） 函数删除一个指定优先级的任务。任务可以传递自己的优先级给OSTaskDel（），从而删除自身。如果任务不知道自己的优先级，还可以传递参数 OS_PRIO_SELF。被删除的任务将回到休眠状态。任务被删除后可以用函数OSTaskCreate（）或OSTaskCreateExt（）重新 建立。
参数
prio为指定要删除任务的优先级，也可以用参数OS_PRIO_SELF代替，此时，下一个优先级最高的就绪任务将开始运行。
返回值
OSTaskDel（）的返回值为下述之一： OS_NO_ERR：函数调用成功。
OS_TASK_DEL_IDLE：错误操作，试图删除空闲任务（Idle task）。
OS_TASK_DEL_ ERR：错误操作，指定要删除的任务不存在。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
OS_TASK_DEL_ISR：错误操作，试图在中断处理程序中删除任务。
注意/警告
OSTaskDel（）将判断用户是否试图删除μC/OS中的空闲任务（Idle task）。
在删除占用系统资源的任务时要小心，此时，为安全起见可以用另一个函数OSTaskDelReq（）。
INT8U OSDelReq (INT8U prio);
所属文件 OS_TASK.C 调用者 任务 开关量 OS_TASK_DEL_EN
OSTaskDelReq（） 函数请求一个任务删除自身。通常OSTaskDelReq（）用于删除一个占有系统资源的任务（例如任务建立了信号量）。对于此类任务，在删除任务之前应 当先释放任务占用的系统资源。具体的做法是：在需要被删除的任务中调用OSTaskDelReq（）检测是否有其他任务的删除请求，如果有，则释放自身占 用的资源，然后调用OSTaskDel（）删除自身。例如，假设任务5要删除任务10，而任务10占有系统资源，此时任务5不能直接调用
 OSTaskDel（10）删除任务10，而应该调用OSTaskDelReq（10）向任务10发送删除请求。在任务10中调用 OSTaskDelReq（OS_PRIO_SELF），并检测返回值。如果返回OS_TASK_DEL_REQ，则表明有来自其他任务的删除请求，此时 任务10应该先释放资源，然后调用OSTaskDel（OS_PRIO_SELF）删除自己。任务5可以循环调用OSTaskDelReq（10）并检测 返回值，如果返回OS_TASK_NOT_EXIST，表明任务10已经成功删除。
参数
prio为要求删除任务的优先级。如果参数为OS_PRIO_SELF，则表示调用函数的任务正在查询是否有来自其他任务的删除请求。
返回值
OSTaskDelReq（）的返回值为下述之一：
OS_NO_ERR：删除请求已经被任务记录。
OS_TASK_NOT_EXIST：指定的任务不存。发送删除请求的任务可以等待此返回值，看删除是否成功。
OS_TASK_DEL_IDLE：错误操作，试图删除空闲任务（Idle task）。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO或没有设定OS_PRIO_SELF的值。
OS_TASK_DEL_REQ：当前任务收到来自其他任务的删除请求。
注意/警告
OSTaskDelReq（）将判断用户是否试图删除μC/OS中的空闲任务（Idle task）。
INT8U OSTaskQuery ( INT8U prio, OS_TCB *pdata);
所属文件 OS_TASK.C 调用者 任务或中断 开关量 无
OSTaskQuery（） 用于获取任务信息，函数返回任务TCB的一个完整的拷贝。应用程序必须建立一个OS_TCB类型的数据结构容纳返回的数据。需要提醒用户的是，在对任务 OS_TCB对象中的数据操作时要小心，尤其是数据项OSTCBNext和OSTCBPrev。它们分别指向TCB链表中的后一项和前一项。
参数
prio为指定要获取TCB内容的任务优先级，也可以指定参数OS_PRIO_SELF，获取调用任务的信息。
pdata指向一个OS_TCB类型的数据结构，容纳返回的任务TCB的一个拷贝。
返回值
OSTaskQuery（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_ERR：参数指定的任务非法。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
注意/警告
任务控制块（TCB）中所包含的数据成员取决于下述开关量在初始化时的设定（参见OS_CFG.H）
OS_TASK_CREATE_EN
OS_Q_EN
OS_MBOX_EN
OS_SEM_EN
OS_TASK_DEL_EN
INT8U OSTaskResume ( INT8U prio);
所属文件 OS_TASK.C 调用者 任务 开关量 OS_TASK_SUSPEND_EN
OSTaskResume （）唤醒一个用OSTaskSuspend（）函数挂起的任务。OSTaskResume（）也是唯一能“解挂”挂起任务的函数。
参数
prio指定要唤醒任务的优先级。
返回值
OSTaskResume （）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_TASK_RESUME_PRIO：要唤醒的任务不存在。
OS_TASK_NOT_SUSPENDED：要唤醒的任务不在挂起状态。
OS_PRIO_INVALID：参数指定的优先级大于或等于OS_LOWEST_PRIO。
INT8U OSTaskStkChk ( INT8U prio, OS_STK_DATA *pdata);
所属文件 OS_TASK.C 调用者 任务 开关量 OS_TASK_CREATE_EXT
OSTaskStkChk（）检查任务堆栈状态，计算指定任务堆栈中的未用空间和已用空间。使用OSTaskStkChk（）函数要求所检查的任务是被OSTaskCreateExt（）函数建立的，且opt参数中OS_TASK_OPT_STK_CHK操作项打开。
计 算堆栈未用空间的方法是从堆栈底端向顶端逐个字节比较，检查堆栈中0的个数，直到一个非0的数值出现。这种方法的前提是堆栈建立时已经全部清零。要实现清 零操作，需要在任务建立初始化堆栈时设置OS_TASK_OPT_STK_CLR为1。如果应用程序在初始化时已经将全部RAM清零，且不进行任务删除操 作，也可以设置OS_TASK_OPT_STK_CLR为0，这将加快OSTaskCreateExt（）函数的执行速度。
参数
prio为指定要获取堆栈信息的任务优先级，也可以指定参数OS_PRIO_SELF，获取调用任务本身的信息。
pdata指向一个类型为OS_STK_DATA的数据结构，其中包含如下信息：
INT32U OSFree;        /* 堆栈中未使用的字节数       */
INT32U OSUsed;        /* 堆栈中已使用的字节数       */
返回值
OSTaskStkChk（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO，或未指定OS_PRIO_SELF。
OS_TASK_NOT_EXIST：指定的任务不存在。
OS_TASK_OPT_ERR：任务用OSTaskCreateExt（）函数建立的时候没有指定OS_TASK_OPT_STK_CHK操作，或者任务是用OSTaskCreate（）函数建立的。
注意/警告
函数的执行时间是由任务堆栈的大小决定的，事先不可预料。
在应用程序中可以把OS_STK_DATA结构中的数据项OSFree和OSUsed相加，可得到堆栈的大小。虽然原则上该函数可以在中断程序中调用，但由于该函数可能执行很长时间，所以实际中不提倡这种做法。
INT8U OSTaskSuspend ( INT8U prio);
所属文件 OS_TASK.C 调用者 任务 开关量 OS_TASK_SUSPEND_EN
OSTaskSuspend（） 无条件挂起一个任务。调用此函数的任务也可以传递参数OS_PRIO_SELF，挂起调用任务本身。当前任务挂起后，只有其他任务才能唤醒。任务挂起后， 系统会重新进行任务调度，运行下一个优先级最高的就绪任务。唤醒挂起任务需要调用函数OSTaskResume （）。
任务的挂起是可以叠加到其他操作上的。例如，任务被挂起时正在进行延时操作，那么任务的唤醒就需要两个条件：延时的结束以及其他任务的唤醒操作。又如，任务被挂起时正在等待信号量，当任务从信号量的等待对列中清除后也不能立即运行，而必须等到唤醒操作后。
参数
prio为指定要获取挂起的任务优先级，也可以指定参数OS_PRIO_SELF，挂起任务本身。此时，下一个优先级最高的就绪任务将运行。
返回值
OSTaskSuspend（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_TASK_ SUSPEND_IDLE：试图挂起μC/OS-II中的空闲任务（Idle task）。此为非法操作。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO或没有设定OS_PRIO_SELF的值。
OS_TASK_ SUSPEND _PRIO：要挂起的任务不存在。
注意/警告
在程序中OSTaskSuspend（）和OSTaskResume （）应该成对使用。
用OSTaskSuspend（）挂起的任务只能用OSTaskResume （）唤醒。
void OSTimeDly ( INT16U ticks);
所属文件 OS_TIMC.C 调用者 任务 开关量 无
OSTimeDly（） 将一个任务延时若干个时钟节拍。如果延时时间大于0，系统将立即进行任务调度。延时时间的长度可从0到65535个时钟节拍。延时时间0表示不进行延时， 函数将立即返回调用者。延时的具体时间依赖于系统每秒钟有多少时钟节拍（由文件SO_CFG.H中的常量OS_TICKS_PER_SEC设定）。
参数
ticks为要延时的时钟节拍数。
注意/警告
注意到延时时间0表示不进行延时操作，而立即返回调用者。为了确保设定的延时时间，建议用户设定的时钟节拍数加1。例如，希望延时10个时钟节拍，可设定参数为11。
void OSTimeDlyHMSM( INT8U hours，INT8U minutes，INT8U seconds，INT8U milli);
所属文件 OS_TIMC.C 调用者 任务 开关量 无
OSTimeDlyHMSM（）将一个任务延时若干时间。延时的单位是小时、分、秒、毫秒。所以使用OSTimeDlyHMSM（）比OSTimeDly（）更方便。调用OSTimeDlyHMSM（）后，如果延时时间不为0，系统将立即进行任务调度。
参数
hours为延时小时数，范围从0-255。
minutes为延时分钟数，范围从0-59。
seconds为延时秒数，范围从0-59
milli 为延时毫秒数，范围从0-999。需要说明的是，延时操作函数都是以时钟节拍为为单位的。实际的延时时间是时钟节拍的整数倍。例如系统每次时钟节拍间隔是 10ms，如果设定延时为5ms，将不产生任何延时操作，而设定延时15ms，实际的延时是两个时钟节拍，也就是20ms。
返回值
OSTimeDlyHMSM（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_TIME_INVALID_MINUTES：参数错误，分钟数大于59。
OS_TIME_INVALID_SECONDS：参数错误，秒数大于59。
OS_TIME_INVALID_MILLI：参数错误，毫秒数大于999。
OS_TIME_ZERO_DLY：四个参数全为0。
注意/警告
OSTimeDlyHMSM（0，0，0，0）表示不进行延时操作，而立即返回调用者。另外，如果延时总时间超过65535个时钟节拍，将不能用OSTimeDlyResume（）函数终止延时并唤醒任务。
void OSTimeDlyResume( INT8U prio);
所属文件 OS_TIMC.C 调用者 任务 开关量 无
OSTimeDlyResume（）唤醒一个用OSTimeDly（）或OSTimeDlyHMSM（）函数延时的任务。
参数
prio为指定要唤醒任务的优先级。
返回值
OSTimeDlyResume（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
OS_TIME_NOT_DLY：要唤醒的任务不在延时状态。
OS_TASK_NOT_EXIST：指定的任务不存在。
注意/警告
用户不应该用OSTimeDlyResume（）去唤醒一个设置了等待超时操作，并且正在等待事件发生的任务。操作的结果是使该任务结束等待，除非的确希望这么做。
OSTimeDlyResume（） 函数不能唤醒一个用OSTimeDlyHMSM（）延时，且延时时间总计超过65535个时钟节拍的任务。例如，如果系统时钟为 100Hz，OSTimeDlyResume（）不能唤醒延时OSTimeDlyHMSM（0，10，55，350）或更长时间的任务。
（OSTimeDlyHMSM（0，10，55，350）共延时     [ 10 minutes *60 + （55+0.35）seconds ] *100 =65,535次时钟节拍）
INT32U OSTimeGet (void);
所属文件 OS_TIMC.C 调用者 任务或中断 开关量 无
OSTimeGet（）获取当前系统时钟数值。系统时钟是一个32位的计数器，记录系统上电后或时钟重新设置后的时钟计数。
返回值
当前时钟计数（时钟节拍数）。
void OSTimeSet (INT32U ticks);
所属文件 OS_TIMC.C 调用者 任务或中断 开关量 无
OSTimeSet（）设置当前系统时钟数值。系统时钟是一个32位的计数器，记录系统上电后或时钟重新设置后的时钟计数。
参数
ticks要设置的时钟数，单位是时钟节拍数。
void OSTimeTick (void);
所属文件 OS_TIMC.C 调用者 任务或中断 开关量 无
每次时钟节拍，μC/OS-II 都将执行OSTimeTick（）函数。OSTimeTick（）检查处于延时状态的任务是否达到延时时间（用OSTimeDly（）或OSTimeDlyHMSM（）函数延时），或正在等待事件的任务是否超时。
注意/警告
OSTimeTick（）的运行时间和系统中的任务数直接相关，在任务或中断中都可以调用。如果在任务中调用，任务的优先级应该很高（优先级数字很小），这是因为OSTimeTick（）负责所有任务的延时操作。
INT16U OSVersion (void);
所属文件 OS_CORE.C 调用者 任务或中断 开关量 无
OSVersion（）获取当前μC/OS-II的版本。
返回值
当前版本，格式为x.yy，返回值为乘以100后的数值。例如当前版本2.00，则返回200。
void OS_ENTER_CRITICAL(void);
void OS_EXIT_CRITICAL(void);
所属文件 OS_CPU.C 调用者 任务或中断 开关量 无
OS_ENTER_CRITICAL（）和OS_EXIT_CRITICAL（）为定义的宏，用来关闭、打开CPU的中断。
注意/警告
OS_ENTER_CRITICAL（）和OS_EXIT_CRITICAL（）必须成对使用。
INT32U  OSNUsed;              /*使用的内存块数目          */
返回值
OSMemQuery（）函数返回值总是OS_NO_ERR。
注意/警告：必须首先建立内存区，然后使用。
Void *OSMboxAccept(OS_EVENT *pevent);
所属文件 OS_MBOX.C    调用者 任务或中断    开关量 OS_MBOX_EN
参数:pevent 是指向需要查看的消息邮箱的指针。当建立消息邮箱时，该指针返回到用户程序。
返回值:如果消息已经到达，返回指向该消息的指针；如果消息邮箱没有消息，返回空指针。
注意/警告:必须先建立消息邮箱，然后使用。
OS_EVENT *OSMboxCreate(void *msg);
所属文件 OS_MBOX.C    调用者 任务或启动代码   开关量 OS_MBOX_EN
OSMboxCreate（）建立并初始化一个消息邮箱。消息邮箱允许任务或中断向其他一个或几个任务发送消息。
参数
msg 参数用来初始化建立的消息邮箱。如果该指针不为空，建立的消息邮箱将含有消息。
返回值
指向分配给所建立的消息邮箱的事件控制块的指针。如果没有可用的事件控制块，返回空指针。
注意/警告：必须先建立消息邮箱，然后使用。
OSMboxPend(OS_EVENT *pevent,INT16U timeout,int8u *err);
所属文件 OS_MBOX.C    调用者 任务   开关量 OS_MBOX_EN
OSMboxPend（） 用于任务等待消息。消息通过中断或另外的任务发送给需要的任务。消息是一个以指针定义的变量，在不同的程序中消息的使用也可能不同。如果调用 OSMboxPend（）函数时消息邮箱已经存在需要的消息，那么该消息被返回给OSMboxPend（）的调用者，消息邮箱中清除该消息。如果调用 OSMboxPend（）函数时消息邮箱中没有需要的消息，OSMboxPend（）函数挂起当前任务直到得到需要的消息或超出定义等待超时的时间。如果
 同时有多个任务等待同一个消息，μC/OS-Ⅱ默认最高优先级的任务取得消息并且任务恢复执行。一个由OSTaskSuspend（）函数挂起的任务也可 以接受消息，但这个任务将一直保持挂起状态直到通过调用OSTaskResume（）函数恢复任务的运行。
参数
pevent  是指向即将接受消息的消息邮箱的指针。该指针的值在建立该消息邮箱时可以得到。（参考OSMboxCreate（）函数）。
Timeout  允许一个任务在经过了指定数目的时钟节拍后还没有得到需要的消息时恢复运行。如果该值为零表示任务将持续的等待消息。最大的等待时间为65,535个时钟 节拍。这个时间长度并不是非常严格的，可能存在一个时钟节拍的误差，因为只有在一个时钟节拍结束后才会减少定义的等待超时时钟节拍。
Err 是指向包含错误码的变量的指针。OSMboxPend（）函数返回的错误码可能为下述几种：OS_NO_ERR ：消息被正确的接受。
OS_TIMEOUT ：消息没有在指定的周期数内送到。
OS_ERR_PEND_ISR ：从中断调用该函数。虽然规定了不允许从中断调用该函数，但μC/OS-Ⅱ仍然包含了检测这种情况的功能。
OS_ERR_EVENT_TYPE  ：pevent 不是指向消息邮箱的指针。
返回值
OSMboxPend（）函数返回接受的消息并将 *err置为OS_NO_ERR。如果没有在指定数目的时钟节拍内接受到需要的消息，OSMboxPend（）函数返回空指针并且将 *err设置为OS_TIMEOUT。
注意/警告:必须先建立消息邮箱，然后使用。不允许从中断调用该函数。
INT8U OSMboxPost(OS_EVENT *pevent, void *msg);
所属文件 OS_MBOX.C    调用者 任务或中断   开关量 OS_MBOX_EN
OSMboxPost（） 函数通过消息邮箱向任务发送消息。消息是一个指针长度的变量，在不同的程序中消息的使用也可能不同。如果消息邮箱中已经存在消息，返回错误码说明消息邮箱 已满。OSMboxPost（）函数立即返回调用者，消息也没有能够发到消息邮箱。如果有任何任务在等待消息邮箱的消息，最高优先级的任务将得到这个消 息。如果等待消息的任务优先级比发送消息的任务优先级高，那么高优先级的任务将得到消息而恢复执行，也就是说，发生了一次任务切换。
参数
pevent  是指向即将接受消息的消息邮箱的指针。该指针的值在建立该消息邮箱时可以得到。（参考OSMboxCreate（）函数）。
Msg 是即将实际发送给任务的消息。消息是一个指针长度的变量，在不同的程序中消息的使用也可能不同。不允许传递一个空指针，因为这意味着消息邮箱为空。
返回值
OSMboxPost（）函数的返回值为下述之一：
OS_NO_ERR ：消息成功的放到消息邮箱中。
OS_MBOX_FULL ：消息邮箱已经包含了其他消息，不空。
OS_ERR_EVENT_TYPE  ：pevent 不是指向消息邮箱的指针。
注意/警告:必须先建立消息邮箱，然后使用。
不允许传递一个空指针，因为这意味着消息邮箱为空。
INT8U OSMboxQuery(OS_EVENT *pevent, OS_MBOX_DATA *pdata);
所属文件 OS_MBOX.C    调用者 任务或中断   开关量 OS_MBOX_EN
OSMboxQuery（） 函数用来取得消息邮箱的信息。用户程序必须分配一个OS_MBOX_DATA的数据结构，该结构用来从消息邮箱的事件控制块接受数据。通过调用 OSMboxQuery（）函数可以知道任务是否在等待消息以及有多少个任务在等待消息，还可以检查消息邮箱现在的消息。
参数
pevent  是指向即将接受消息的消息邮箱的指针。该指针的值在建立该消息邮箱时可以得到。（参考OSMboxCreate（）函数）。
Pdata  是指向OS_MBOX_DATA数据结构的指针，该数据结构包含下述成员：
Void  *OSMsg;            /* 消息邮箱中消息的复制 */
INT8U  OSEventTbl[OS_EVENT_TBL_SIZE];   /*消息邮箱等待队列的复制*/
INT8U OSEventGrp；
返回值
OSMboxQuery（）函数的返回值为下述之一：
OS_NO_ERR ：调用成功
OS_ERR_EVENT_TYPE  ：pevent 不是指向消息邮箱的指针。
注意/警告:必须先建立消息邮箱，然后使用。
Void  *OSQAccept(OS_EVENT  *pevent);
所属文件 OS_Q.C    调用者 任务或中断   开关量 OS_Q_EN
OSQAccept（）函数检查消息队列中是否已经有需要的消息。不同于OSQPend（）函数，如果没有需要的消息，OSQAccept（）函数并不挂起任务。如果消息已经到达，该消息被传递到用户任务。通常中断调用该函数，因为中断不允许挂起等待消息。
参数
pevent 是指向需要查看的消息队列的指针。当建立消息队列时，该指针返回到用户程序。
返回值
如果消息已经到达，返回指向该消息的指针；如果消息队列没有消息，返回空指针。
注意/警告：必须先建立消息队列，然后使用。
OS_EVENT  *OSQCreate( void **start, INT8U size);
所属文件 OS_Q.C    调用者 任务或启动代码   开关量 OS_Q_EN
OSQCreate（）函数建立一个消息队列。任务或中断可以通过消息队列向其他一个或多个任务发送消息。消息的含义是和具体的应用密切相关的。
参数
start  是消息内存区的基地址，消息内存区是一个指针数组。
Size  是消息内存区的大小。
返回值
OSQCreate（）函数返回一个指向消息队列事件控制块的指针。如果没有空余的事件空闲块，OSQCreate（）函数返回空指针。
注意/警告：必须先建立消息队列，然后使用。
INT8U *SOQFlush(OS_EVENT *pevent);
所属文件 OS_Q.C    调用者 任务或中断   开关量 OS_Q_EN
OSQFlush（）函数清空消息队列并且忽略发送往队列的所有消息。不管队列中是否有消息，这个函数的执行时间都是相同的。
参数
pevent  是指向消息队列的指针。该指针的值在建立该队列时可以得到。
返回值
OSQFlush（）函数的返回值为下述之一：
OS_NO_ERR ：消息队列被成功清空
OS_ERR_EVENT_TYPE ：试图清除不是消息队列的对象
注意/警告:必须先建立消息队列，然后使用。
Void *OSQPend( OS_EVENT *pevent, INT16U timeout, INT8U *err);
所属文件 OS_Q.C    调用者 任务   开关量 OS_Q_EN
OSQPend（） 函数用于任务等待消息。消息通过中断或另外的任务发送给需要的任务。消息是一个以指针定义的变量，在不同的程序中消息的使用也可能不同。如果调用 OSQPend（）函数时队列中已经存在需要的消息，那么该消息被返回给OSQPend（）函数的调用者，队列中清除该消息。如果调用OSQPend（） 函数时队列中没有需要的消息，OSQPend（）函数挂起当前任务直到得到需要的消息或超出定义的超时时间。如果同时有多个任务等待同一个消
 息，μC/OS-Ⅱ默认最高优先级的任务取得消息并且任务恢复执行。一个由OSTaskSuspend（）函数挂起的任务也可以接受消息，但这个任务将一 直保持挂起状态直到通过调用OSTaskResume（）函数恢复任务的运行。
参数
pevent  是指向即将接受消息的队列的指针。该指针的值在建立该队列时可以得到。（参考OSMboxCreate（）函数）。
Timeout  允许一个任务在经过了指定数目的时钟节拍后还没有得到需要的消息时恢复运行状态。如果该值为零表示任务将持续的等待消息。最大的等待时间为65535个时 钟节拍。这个时间长度并不是非常严格的，可能存在一个时钟节拍的误差，因为只有在一个时钟节拍结束后才会减少定义的等待超时时钟节拍。
Err 是指向包含错误码的变量的指针。OSQPend（）函数返回的错误码可能为下述几种：
OS_NO_ERR ：消息被正确的接受。
OS_TIMEOUT ：消息没有在指定的周期数内送到。
OS_ERR_PEND_ISR ：从中断调用该函数。虽然规定了不允许从中断调用该函数，但μC/OS-Ⅱ仍然包含了检测这种情况的功能。
OS_ERR_EVENT_TYPE  ：pevent 不是指向消息队列的指针。
返回值
OSQPend（）函数返回接受的消息并将 *err置为OS_NO_ERR。如果没有在指定数目的时钟节拍内接受到需要的消息，OSQPend（）函数返回空指针并且将 *err设置为OS_TIMEOUT。
注意/警告:必须先建立消息邮箱，然后使用。
不允许从中断调用该函数。
INT8U OSQPost(OS_EVENT *pevent, void *msg);
所属文件 OS_Q.C    调用者 任务或中断   开关量 OS_Q_EN
OSQPost（） 函数通过消息队列向任务发送消息。消息是一个指针长度的变量，在不同的程序中消息的使用也可能不同。如果队列中已经存满消息，返回错误码。 OSQPost（）函数立即返回调用者，消息也没有能够发到队列。如果有任何任务在等待队列中的消息，最高优先级的任务将得到这个消息。如果等待消息的任 务优先级比发送消息的任务优先级高，那么高优先级的任务将得到消息而恢复执行，也就是说，发生了一次任务切换。消息队列是先入先出（FIFO）机制的，先
 进入队列的消息先被传递给任务。
参数
pevent  是指向即将接受消息的消息队列的指针。该指针的值在建立该队列时可以得到。（参考OSQCreate（）函数）。
Msg 是即将实际发送给任务的消息。消息是一个指针长度的变量，在不同的程序中消息的使用也可能不同。不允许传递一个空指针。
返回值
OSQPost（）函数的返回值为下述之一：
OS_NO_ERR ：消息成功的放到消息队列中。
OS_MBOX_FULL ：消息队列已满。
OS_ERR_EVENT_TYPE  ：pevent 不是指向消息队列的指针。
注意/警告:必须先建立消息队列，然后使用。
不允许传递一个空指针。
INT8U OSQPostFront(OS_EVENT *pevent, void *msg);
所属文件 OS_Q.C    调用者 任务或中断   开关量 OS_Q_EN
OSQPostFront（） 函数通过消息队列向任务发送消息。OSQPostFront（）函数和OSQPost（）函数非常相似，不同之处在于OSQPostFront（）函数将 发送的消息插到消息队列的最前端。也就是说，OSQPostFront（）函数使得消息队列按照后入先出（LIFO）的方式工作，而不是先入先出 （FIFO）。消息是一个指针长度的变量，在不同的程序中消息的使用也可能不同。如果队列中已经存满消息，返回错误码。OSQPost（）函数立即返回调
 用者，消息也没能发到队列。如果有任何任务在等待队列中的消息，最高优先级的任务将得到这个消息。如果等待消息的任务优先级比发送消息的任务优先级高，那 么高优先级的任务将得到消息而恢复执行，也就是说，发生了一次任务切换
参数
pevent  是指向即将接受消息的消息队列的指针。该指针的值在建立该队列时可以得到。
Msg 是即将实际发送给任务的消息。消息是一个指针长度的变量，在不同的程序中消息的使用也可能不同。不允许传递一个空指针。
返回值
OSQPost（）函数的返回值为下述之一：
OS_NO_ERR ：消息成功的放到消息队列中。
OS_MBOX_FULL ：消息队列已满。
OS_ERR_EVENT_TYPE  ：pevent 不是指向消息队列的指针。
注意/警告:必须先建立消息队列，然后使用。
不允许传递一个空指针。
INT8U OSQQuery(OS_EVENT *pevent, OS_Q_DATA *pdata);
所属文件 OS_Q.C    调用者 任务或中断   开关量 OS_Q_EN
OSQQuery（） 函数用来取得消息队列的信息。用户程序必须建立一个OS_Q_DATA的数据结构，该结构用来保存从消息队列的事件控制块得到的数据。通过调用 OSQQuery（）函数可以知道任务是否在等待消息、有多少个任务在等待消息、队列中有多少消息以及消息队列可以容纳的消息数。OSQQuery（）函 数还可以得到即将被传递给任务的消息的信息。
参数
pevent  是指向即将接受消息的消息邮箱的指针。该指针的值在建立该消息邮箱时可以得到。（参考OSQCreate（）函数）。
Pdata  是指向OS_Q_DATA数据结构的指针，该数据结构包含下述成员：
Void           *OSMsg;          /* 下一个可用的消息*/
INT16U         OSNMsgs；                          /*  队列中的消息数目*/
INT16U         OSQSize；                            /*  消息队列的大小  */
INT8U          OSEventTbl[OS_EVENT_TBL_SIZE];  /*  消息队列的等待队列*/
INT8U          OSEventGrp；
返回值
OSQQuery（）函数的返回值为下述之一：
OS_NO_ERR ：调用成功
OS_ERR_EVENT_TYPE  ：pevent 不是指向消息队列的指针。
注意/警告:必须先建立消息队列，然后使用。
INT16U *OSSemAccept(OS_EVENT *pevent);
所属文件 OS_SEM.C    调用者 任务或中断   开关量 OS_SEM_EN
OSSemAccept（）函数查看设备是否就绪或事件是否发生。不同于OSSemPend（）函数，如果设备没有就绪，OSSemAccept（）函数并不挂起任务。中断调用该函数来查询信号量。
参数
pevent 是指向需要查询的设备的信号量。当建立信号量时，该指针返回到用户程序。
返回值
当调用OSSemAccept（）函数时，设备信号量的值大于零，说明设备就绪，这个值被返回调用者，设备信号量的值减一。如果调用OSSemAccept（）函数时，设备信号量的值等于零，说明设备没有就绪，返回零。
注意/警告:必须先建立信号量，然后使用。
OS_EVENT *OSSemCreate(WORD value);
所属文件 OS_SEM.C    调用者 任务  开关量 OS_SEM_EN
OSSemCreate（）函数建立并初始化一个信号量。信号量的作用如下：
允许一个任务和其他任务或者中断同步。
取得设备的使用权标志事件的发生
参数
value 参数是建立的信号量的初始值，可以取0到65535之间的任何值。
返回值
OSSemCreate（）函数返回指向分配给所建立的消息邮箱的事件控制块的指针。如果没有可用的事件控制块，OSSemCreate（）函数返回空指针。
注意/警告:必须先建立信号量，然后使用。
Void OSSemPend ( OS_EVNNT  *pevent, INT16U  timeout,  int8u *err );
所属文件 OS_SEM.C    调用者 任务  开关量 OS_SEM_EN
OSSemPend（） 函数用于任务试图取得设备的使用权，任务需要和其他任务或中断同步，任务需要等待特定事件的发生的场合。如果任务调用OSSemPend（）函数时，信号 量的值大于零，OSSemPend（）函数递减该值并返回该值。如果调用时信号量等于零，OSSemPend（）函数函数将任务加入该信号量的等待队列。 OSSemPend（）函数挂起当前任务直到其他的任务或中断置起信号量或超出等待的预期时间。如果在预期的时钟节拍内信号量被置起，μC/OS-Ⅱ默认
 最高优先级的任务取得信号量恢复执行。一个被OSTaskSuspend（）函数挂起的任务也可以接受信号量，但这个任务将一直保持挂起状态直到通过调用 OSTaskResume（）函数恢复任务的运行。
参数
pevent  是指向信号量的指针。该指针的值在建立该信号量时可以得到。
Timeout  允许一个任务在经过了指定数目的时钟节拍后还没有得到需要的信号量时恢复运行状态。如果该值为零表示任务将持续的等待信号量。最大的等待时间为65535 个时钟节拍。这个时间长度并不是非常严格的，可能存在一个时钟节拍的误差，因为只有在一个时钟节拍结束后才会减少定义的等待超时时钟节拍。
Err 是指向包含错误码的变量的指针。OSSemPend（）函数返回的错误码可能为下述几种：
OS_NO_ERR ：信号量不为零。
OS_TIMEOUT ：信号量没有在指定的周期数内置起。
OS_ERR_PEND_ISR ：从中断调用该函数。虽然规定了不允许从中断调用该函数，但μC/OS-Ⅱ仍然包含了检测这种情况的功能。
OS_ERR_EVENT_TYPE  ：pevent 不是指向信号量的指针。
注意/警告:必须先建立信号量，然后使用。
不允许从中断调用该函数。
INT8U OSSemPost(OS_EVENT *pevent);
所属文件 OS_SEM.C    调用者 任务或中断  开关量 OS_SEM_EN
OSSemPost（） 函数置起指定的信号量。如果指定的信号量是零或大于零，OSSemPost（）函数递增该信号量并返回。如果有任何任务在等待信号量，最高优先级的任务将 得到信号量并进入就绪状态。任务调度函数将进行任务调度，决定当前运行的任务是否仍然为最高优先级的就绪状态的任务。
参数
pevent  是指向信号量的指针。该指针的值在建立该信号量时可以得到。
返回值
OSSemPost（）函数的返回值为下述之一：
OS_NO_ERR ：信号量成功的置起
OS_SEM_OVF ：信号量的值溢出
OS_ERR_EVENT_TYPE  ：pevent 不是指向信号量的指针。
注意/警告：必须先建立信号量，然后使用。
INT8U OSSemQuery(OS_EVENT *pevent, OS_SEM_DATA *pdata);
所属文件 OS_SEM.C    调用者 任务或中断  开关量 OS_SEM_EN
OSSemQuery（） 函数用于获取某个信号量的信息。使用OSSemQuery（）之前，应用程序需要先创立类型为OS_SEM_DATA的数据结构，用来保存从信号量的事件 控制块中取得的数据。使用OSSemQuery（）可以得知是否有，以及有多少任务位于信号量的任务等待队列中（通过查询.OSEventTbl [ ]域），还可以获取信号量的标识号码。OSEventTbl
 [ ]域的大小由语句：
\#define constant OS_ENENT_TBL_ SIZE定义(参阅文件uCOS_II.H)。
参数
pevent是一个指向信号量的指针。该指针在信号量建立后返回调用程序[参见OSSemCreat（）函数]。
Pdata是一个指向数据结构OS_SEM_DATA的指针，该数据结构包含下述域：
INT16U OSCnt;                                /* 当前信号量标识号码 */
INT8U  OSEventTbl[OS_EVENT_TBL_SIZE];        /*信号量等待队列*/
INT8U  OSEventGrp;
返回值
OSSemQuery（）函数有下述两个返回值：
OS_NO_ERR 表示调用成功。
OS_ERR_EVENT_TYPE 表示未向信号量传递指针。
注意/警告：被操作的信号量必须是已经建立了的。

