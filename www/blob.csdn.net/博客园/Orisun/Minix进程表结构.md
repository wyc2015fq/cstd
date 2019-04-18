# Minix进程表结构 - Orisun - 博客园







# [Minix进程表结构](https://www.cnblogs.com/zhangchaoyang/articles/1896607.html)





# Minix进程表结构

**摘要：**本文通过分析Minix3操作系统原代码，详细说明了与进程相关的数据结构，并在此基础上描述了Minix进程间通信和进程调度的完整过程。

**一些预备知识**

首先要明白MINIX3本身就是一组进程的集合。

![](https://pic002.cnblogs.com/images/2010/103496/2010120421330090.png)

尽管系统任务和时钟任务被编译进了内核，但它仍然以一个独立的进程运行。

include/minix/sys_config.h中定义了宏变量_NR_RPOCS指示控制进程表的大小，该值默认是64，如果用户想使用MINIX3作为一个网络服务器以便多个远程用户或者服务器进程能够并发执行，那么需要增加此常量的值。

include/minix/type.h中定义了结构体kinfo，用来向系统的其他部分传递有关系统的消息。进程管理器在创建它的进程表的部分时使用的正是该结构。

include/minix/callnr.h中定义了所有允许的系统调用的序号。当一个进程执行MINIX3系统调用时，它向进程管理器（PM）或文件系统（FS）发送一条消息，每条消息中含有所有要求的系统调用序号。

在kernel/config.h中定义了一些宏，处理每个内核调用所需要的代码使用这些宏进行条件编译的，如果一些函数被取消，那么执行它的代码也会从系统二进制代码中删除。例如手机不需要产生新进程（fork），因而该部分代码可以从执行文件中删除，以便使得内存更小。

kernel/const.h中定义了宏函数vir2phys用于将内核的内存空间与基地址相关的虚拟地址转换为物理地址。

#definevir2phys(vir)(kinfo.data_base+ (vir_bytes) (vir))

进程状态字（PSW）是个CPU寄存器，其中I/OProtection Level（IOPL）位定义了是否允许访问中断系统和I/O端口，下面的代码决定了普通进程和特权进程对中断系统和I/O端口的访问权限。

#defineINIT_PSW      0x0200/*initial psw */

#defineINIT_TASK_PSW 0x1200/*initial psw for tasks (with IOPL 1) */

kernel/type.h中定义了memory结构，它使用基地址和存储区大小两个域来来唯一标识一块内存区域。

struct memory {

phys_clicksbase;/* startaddress of chunk */

phys_clickssize;/* sizeof memory chunk */

};

这里解释一下phys_clicks。内存的基本度量单位是块，Intel处理器平台上的MINIX3的块是1024字节。内存用参数phys_clicks来度量，内核通过它来访问系统任何地方的任何内存区域。而进程则通过vir_clicks来度量，一块vir_clicks内存引用是指分配给特定进程的一段内存的基址。

## 进程表数据结构

kernel/proc.h定义了内核进程表。一个进程的完整状态包括内存中的进程数据和进程表项中的信息。当一个进程没有执行时CPU寄存器内容就存储在这里，恢复执行时则重新存储。进程表中的每一项被定义为一个proc进程：

struct proc {

structstackframe_s p_reg;/*process' registers saved in stack frame */

reg_tp_ldt_sel;/*selector in gdt with ldt base and limit */

struct segdesc_sp_ldt[2+NR_REMOTE_SEGS]; /* CS, DS and remote segments */



proc_nr_tp_nr;/* numberof this process (for fast access) */

structpriv *p_priv;/*system privileges structure */

charp_rts_flags;/*SENDING, RECEIVING, etc. */



charp_priority;/*current scheduling priority */

charp_max_priority;/*maximum scheduling priority */

charp_ticks_left;/*number of scheduling ticks left */

charp_quantum_size;/*quantum size in ticks */



struct mem_mapp_memmap[NR_LOCAL_SEGS];   /* memory map (T, D, S) */



clock_tp_user_time;/*user time in ticks */

clock_tp_sys_time;/*sys time in ticks */



structproc *p_nextready;/*pointer to next ready process */

structproc *p_caller_q;/*head of list of procs wishing to send */

structproc *p_q_link;/*link to next proc wishing to send */

message*p_messbuf;/*pointer to passed message buffer */

proc_nr_tp_getfrom;/*from whom does process want to receive? */

proc_nr_tp_sendto;/* towhom does process want to send? */



sigset_tp_pending;/*bit map for pending kernel signals */



charp_name[P_NAME_LEN];/*name of the process, including \0 */

};

每一项包括进程寄存器、栈指针、状态值、内存映射、栈限制、进程号、计数值、alarm时间以及消息信息。进程表项的第一部分是stackframe_s，该结构体的定义在kernel/type.h中，内存中的进程通过将它的栈指针赋予进程表项的地址来投入运行，并且将该结构弹出到CPU寄存器。进程表项中还有一个指向priv结构的指针，priv结构在kerenl/priv.h中有定义，它包含了消息允许的源（source）和目的（destination），以及其他一些特权。每一个进程都有指向其副本的一个指针，用户的权限与指向同一副本的用户进程的指针相同。p_max_priority是一个进程允许的最大优先级，表示一个进程首次处于就绪状态时需要放入哪个调试队列。因为如果进程阻碍了其他进行运行，那么它的优先级就会降低。p_priority被初始化为与p_max_priority相等，每次进程就绪时实际上是由p_priority决定使用哪个队列。p_ticks_left记录进程剩余的时间片，p_quantum_size记录分配给进程的时间片。p_user_time和p_sys_time记录每个进程所用的时间。

p_nextready指向下一个准备就绪的进程。当进程由于目标未在等待不能完成send时，发送者将被放置在目标的p_caller_q指针指向的队列中，这样目标最终进行receive时，它就可以很容易地找到要向它发送消息的进程。p_q_link用于将队列成员连接在一起。

当一个进程执行了receive而没以被接收的消息时，它将阻塞，而它想要接收信号的进程号存储在p_getfrom中。同样地，当一个进程执行了send而没有接收者需要时，p_sendto存储目标进程号。消息地址缓存在p_messbuf中。p_pending使用一个位映射来跟踪那些没有被发送进程管理器的信号（因为进程管理器未在等待该进程）。数组p_name用来存储进程名称。

kernel/proc.h中接下来定义了一些常量，可以在p_rts_flags中设置各种标志。当p_rts_flags=0时说明进程是可运行的就绪状态，任何一位为1说明进程不可运行，不可运行的原因由下列代码说明：

/* Bits for the runtime flags.A process is runnable iff p_rts_flags == 0. */

#defineSLOT_FREE0x01/*process slot is free */

#defineNO_MAP0x02/*keeps unmapped forked child from running */

#defineSENDING0x04/*process blocked trying to SEND */

#defineRECEIVING0x08/*process blocked trying to RECEIVE */

#defineSIGNALED0x10/*set when new kernel signal arrives */

#defineSIG_PENDING0x20/*unready while signal being processed */

#defineP_STOP0x40/*set when process is being traced */

#defineNO_PRIV0x80/*keep forked system process from running */

如何上表中的任何一项被使用SLOT_FREE置1。fork之后NO_MAP置位以防止子进程还未建立内存映像就开始运行。SENDING和RECEIVING表示进程正在发送或接收消息而阻塞。SIGNALED和SIG_PENDING表示信号已被接收，P_STOP为追踪提供支持。NO_PRIV用来防止新的系统进程还未完全建立就开始运行。

EXTERNstruct proc proc[NR_TASKS + NR_PROCS];/*process table */

进程表本身被定义为一个proc结构体的数组proc[NR_TASKS+NR_PROCS],NR_TASKS在include/minix/com.h中被定义为4，常量NR_PROCS在include/minix/congfig.h中被定义为64。如果需要，NR_PROCS可以更改，以创建一个能够处理更多进程的系统（如在一个大型服务器上）。

EXTERN struct proc*pproc_addr[NR_TASKS + NR_PROCS];

EXTERN struct proc*rdy_head[NR_SCHED_QUEUES]; /* ptrs to ready list headers */

EXTERN struct proc*rdy_tail[NR_SCHED_QUEUES]; /* ptrs to ready list tails */

上面的这些宏定义用来提高访问速度。进程表被频繁读写，而在一个数组中计算地址需要用到速度很慢的乘法操作，所以提供了一个指向进程表元素的数组pproc_addr。rdy_head和rdy_tail用来维护调度队列，例如rdy_head[USER_Q]指向默认用户队列的第一个进程。

## 进程间通信

进程间通信的数据结构和函数原型在include/minix/ipc.h中，该文件中定义了一个message结构体：

typedef struct {

intm_source;/*who sent the message */

intm_type;/*what kind of message is it */

union {

mess_1m_m1;

mess_2m_m2;

mess_3m_m3;

mess_4m_m4;

mess_5m_m5;

mess_7m_m7;

mess_8m_m8;

} m_u;

} message;

其中包括消息源、消息的类型和以一个联合体m_u表示的消息的内容。消息内容一共有7种格式，比如要发送的消息包含3个整数和3个指针时使用m_m1，传递3个int，2个long，1个指针时使用m_m2，依次类推。

ipc.h中还定义了消息传递的原语的原型：

_PROTOTYPE(int echo, (message *m_ptr));

_PROTOTYPE(int notify, (int dest));

_PROTOTYPE(int sendrec, (int src_dest, message *m_ptr));

_PROTOTYPE(int receive, (int src, message *m_ptr));

_PROTOTYPE(int send, (int dest, message *m_ptr));

_PROTOTYPE(int nb_receive, (int src, message *m_ptr));

_PROTOTYPE(int nb_send, (int dest, message *m_ptr));

先解释一下_PROTOTYPE，这个include/ansi.h中定义一个宏:

#define_PROTOTYPE(function,params)function()

它允许使用如下形式写一个函数原型：

_PROTOTYPE(return-type function-name , (argument-type argument , ...) )

上面的消息原语中比较重要的是notify，sendrec，receive和send，非阻塞的nb_send和nb_receive调用大都被notify替换了。echo原型没有源和目的地址域，它对于生成代码没有任何帮助，但它可以检测发送和接收消息所用的时间。

在每次编译时都会用到kernel/ipc.h，它定义了用于进程通信的各种变量。

/* Masks and flags for systemcalls. */

#defineSYSCALL_FUNC0x0F/*mask for system call function */

#define SYSCALL_FLAGS   0xF0   /* mask for system call flags */

#defineNON_BLOCKING  0x10/* prevent blocking, return error */

/* The following bit masksdetermine what checks that should be done. */

#defineCHECK_PTR       0x0B/*1 0 1 1 : validate message buffer */

#defineCHECK_DST       0x05/*0 1 0 1 : validate message destination */

#defineCHECK_SRC       0x02/*0 0 1 0 : validate message source */

进程间的高层通信代码在kernel/proc.c中。

proc.c中首先是一个BuidMess函数，创建一条消息。内核的任务是将一个硬件中断或软件中断转换为一条消息，前者由硬件产生，后者是对一个系统调用向内核传递的途径。

#define BuildMess(m_ptr, src,dst_ptr) \

(m_ptr)->m_source= (src); /*消息的源*/\

(m_ptr)->m_type= NOTIFY_FROM(src);/*消息的类型*/\

(m_ptr)->NOTIFY_TIMESTAMP=get_uptime();/*给要发送的消息加上时间戳。get_uptime()在kernel/clock.c中定义，它的唯一功能就是获取当前时间。*/\

switch(src) {\

caseHARDWARE:/*消息源于硬件中断*/\

(m_ptr)->NOTIFY_ARG=priv(dst_ptr)->s_int_pending;/*m_ptr是指向一个message的指针，NOTIFY_ARG在include/minix/com.h中定义为m2_l1，即第2种消息类型的第1个long元素。priv宏函数在kernel/priv.h中被定义为获取一个进程的系统优先级别。s_int_pending是priv结构体的一个成员，它表示挂起的硬件中断*/\

priv(dst_ptr)->s_int_pending= 0;/*把中断位置0*/\

break;\

caseSYSTEM:/*消息源自于系统调用*/\

(m_ptr)->NOTIFY_ARG=priv(dst_ptr)->s_sig_pending;/*s_sig_pending也是priv结构体的成员，表示挂起的信号*/\

priv(dst_ptr)->s_sig_pending= 0;\

break;\

}

接下来是一个CopyMess函数两个消息缓冲区中内容的复制。

sys_call()函数的作用是将一个软中断转换为消息

PRIVATEint mini_send(caller_ptr, dst, m_ptr,flags)/*mini_send函数的参数要求给出消息的源、目标、指向消息的指针和一个标记。*/

registerstruct proc *caller_ptr;/*发送消息的源。proc结构体在proc.h中有定义*/

intdst;/*以进程号给出消息的目标进程。*/

message*m_ptr;/*指向进程消息缓冲区的指针。message结构体在include/minix/ipc.h中有定义*/

unsignedflags;/*系统调用的标记 */

{



registerstruct proc *dst_ptr = proc_addr(dst);/*把进程号转化为指向该进程的指针*/

register struct proc **xpp;

register struct proc *xp;



/* 检查死锁*/

xp= dst_ptr;/*XP指向目标进程 */

while(xp->p_rts_flags & SENDING) {/*检查目标进程是否正在进行send操作。p_rts_flags在proc.h中有定义*/

xp= proc_addr(xp->p_sendto);/* 把目标进程的send目标赋给xp*/

if(xp == caller_ptr) return(ELOCKED);/* 如果xp正好就是原消息的发送者，则发生死锁。ELOCKED常量定义在include/error.h中，这种错误表示由于发一死锁而不能执行sendmessage的操作*/

}



if ( (dst_ptr->p_rts_flags& (RECEIVING | SENDING)) == RECEIVING &&

(dst_ptr->p_getfrom== ANY || dst_ptr->p_getfrom == caller_ptr->p_nr)) {

/*目标进程正在receive一个消息，并且它等待消息的发送者正好就是消息源*/

CopyMess(caller_ptr->p_nr,caller_ptr, m_ptr, dst_ptr,

dst_ptr->p_messbuf);/*把消息从发送者的缓冲区复制到接收者的缓冲区*/

if((dst_ptr->p_rts_flags &= ~RECEIVING) == 0)enqueue(dst_ptr);/*如果目标进程没有在等待接收消息，则把源进程放入一个等待队列中*/

}else if ( ! (flags & NON_BLOCKING)){/*目标进程没有在等待。NON_BLOCKING在kernel/ipc.h中有定义*/

caller_ptr->p_messbuf= m_ptr;/*重新把消息内容赋给消息发送进程的消息缓冲区*/

if(caller_ptr->p_rts_flags == 0)dequeue(caller_ptr);/*发送者的p_rts_flags等于0说明发送进程处于就绪状态，把它压入队列*/

caller_ptr->p_rts_flags|= SENDING;/*把发送进程的SENDING位置1*/

caller_ptr->p_sendto= dst;



/*进程阻塞，把它放入目标队列*/

xpp= &dst_ptr->p_caller_q;/* p_caller_q是目标进程队列的队首*/

while(*xpp != NIL_PROC) xpp =&(*xpp)->p_q_link;/*NIL_PROC在proc.h中定义为空进程*/

*xpp= caller_ptr;

caller_ptr->p_q_link= NIL_PROC;/* 标记新的队尾*/

} else {

return(ENOTREADY);/*ENOTREADY错误表示源或目标进程没有准备就绪*/

}

return(OK);

}

接收消息与发送消息过程类似，当一个进程执行receive操作时，内核检查该队列中是否存在向它发送消息的进程。若有，则消息从被阻塞的发送进程缓冲区复制到接收进程缓冲区，并将两者均标记为就绪；若不存在这样的进程，则接收进程被阻塞，直到一条消息到达。

当进程A向进程B发送消息的同时，进程B也在向进程A发送消息，那么将导致死锁，应尽量避免。notify发送一条基本消息（bare-bones），如果接收者不在等待消息，发送者也不会阻塞。然而通知也不会阻塞，当目标进程下次运行时挂起的消息将先于普通消息处理。

PRIVATE intmini_notify(caller_ptr, dst)

registerstruct proc *caller_ptr;/*通知发起者*/

intdst;/*通知接收者*/

{

register struct proc*dst_ptr = proc_addr(dst);

intsrc_id;/*source id for late delivery */

messagem;/* thenotification message */



/*检查目标进程是否阻塞并且正在等待这个消息。一个进程进行SENDREC系统调用时，它可以同时处于sending和receiving两种状态。*/

if ((dst_ptr->p_rts_flags& (RECEIVING|SENDING)) == RECEIVING &&

!(priv(dst_ptr)->s_flags & SENDREC_BUSY) &&

(dst_ptr->p_getfrom== ANY || dst_ptr->p_getfrom == caller_ptr->p_nr)) {

BuildMess(&m,proc_nr(caller_ptr), dst_ptr);

CopyMess(proc_nr(caller_ptr),proc_addr(HARDWARE), &m, 

dst_ptr,dst_ptr->p_messbuf);

dst_ptr->p_rts_flags&= ~RECEIVING;/*deblock destination */

if (dst_ptr->p_rts_flags== 0) enqueue(dst_ptr);

return(OK);

} 

src_id =priv(caller_ptr)->s_id;

/*接收者没有在等待消息，那么它的s_notify_pending位图中的一位将被置位，它指示了有一个通知挂起，并表明了发送者的身份

set_sys_bit(priv(dst_ptr)->s_notify_pending,src_id); 

return(OK);

}

## 进程调度

kernel/glo.h中包含了与进程控制和内核执行有关的一些变量。

/* Process schedulinginformation and the kernel reentry count. */

EXTERNstruct proc *prev_ptr;/*previously running process */

EXTERNstruct proc *proc_ptr;/*pointer to currently running process */

EXTERNstruct proc *next_ptr;/*next process to run after restart() */

EXTERNstruct proc *bill_ptr;/*process to bill for clock ticks */

EXTERNchar k_reenter;/*kernel reentry count (entry count less 1) */

EXTERNunsigned lost_ticks;/*clock ticks counted outside clock task */

prev_ptr，proc_ptr，next_ptr分别指向之前、当前、之后的进程的进程表项。bill_ptr指示哪个进程来为时钟买单（billfor clockticks），举个例子，当用户进程调用文件系统，而文件系统正在运行时，proc_ptr指向该文件系统进程，而bill_ptr则指向进行调用的用户进程，那就是说用户进程要为时钟买单，即文件系统所用的CPU时间长度将从调用者拥有的系统时间中扣除。k_reenter记录重新进入内核的次数，即内核代码的嵌套执行次数。



kernel/protect.h中几乎所有内容都与支持保护模式的Intel处理器（80286、80386、80486、奔腾系列）体系结构细节相关。

/* Privileges. */

#defineINTR_PRIVILEGE       0/*kernel and interrupt handlers */

#defineTASK_PRIVILEGE       1/*kernel tasks */

#defineUSER_PRIVILEGE       3/*servers and user processes */

32位的Intel处理器提供了四种优先级别（privilegelevels），MINIX3使用其中的三种。内核的最中心部分即运行于中断处理期间的部分和切换进程的部分运行在INTR_PRIVILEGE特权级，它们可以访问全部的内存空间和全部的CPU寄存器。系统任务运行在TASK_PRIVILEGE特权级上，它们被允许访问I/O，但不能使用那些修改特殊寄存器（如指向描述符表的寄存器）的指令。服务器进程和用户进程运行在USER_PRIVILEGE特权级，它们不能访问I/O端口、改变内存分配状态或改变处理器运行级别等。

MINIX3使用一种多级调度算法，事实上调试器维护16个可运行的进程队列。时钟和系统任务拥有最高的优先级，设备驱动器获得低一些优先级，但它们的优先级不完全相同，用户进程的优先级最，并且在启动时被赋予相同的初始值，通过nice命令可以提高或降低某个进程的优先级。系统初始化的过程中初始进程的排除情况由table.c中的image表决定。在kernel/table.c中image表提供了初始化所有从引导映像加载的进程所需的细节部分。

PUBLIC struct boot_imageimage[] = {

/* process nr,   pc, flags,qs,  queue, stack, traps, ipcto, call,  name */ 

{ IDLE,  idle_task, IDL_F, 8, IDLE_Q, IDL_S,     0,     0,     0, "IDLE"  },

{ CLOCK,clock_task, TSK_F,64, TASK_Q, TSK_S, TSK_T,     0,     0, "CLOCK" },

{ SYSTEM, sys_task, TSK_F,64, TASK_Q, TSK_S, TSK_T,     0,     0, "SYSTEM"},

{ HARDWARE,      0, TSK_F,64, TASK_Q, HRD_S,     0,     0,     0, "KERNEL"},

{ PM_PROC_NR,    0, SRV_F,32,      3, 0,     SRV_T, SRV_M,  PM_C, "pm"    },

{ FS_PROC_NR,    0, SRV_F,32,      4, 0,     SRV_T, SRV_M,  FS_C, "fs"    },

{ RS_PROC_NR,    0, SRV_F, 4,      3, 0,     SRV_T, SYS_M,  RS_C, "rs"    },

{ TTY_PROC_NR,   0, SRV_F, 4,      1, 0,     SRV_T, SYS_M, DRV_C, "tty"   },

{ MEM_PROC_NR,   0, SRV_F, 4,      2, 0,     SRV_T, DRV_M, MEM_C, "memory"},

{ LOG_PROC_NR,   0, SRV_F, 4,      2, 0,     SRV_T, SYS_M, DRV_C, "log"   },

{ DRVR_PROC_NR,  0, SRV_F, 4,      2, 0,     SRV_T, SYS_M, DRV_C, "driver"},

{ INIT_PROC_NR,  0, USR_F, 8, USER_Q, 0,     USR_T, USR_M,     0, "init"  },

};

进程IDLE总是处于就绪状态，并且位于最低优先级别队列中。

“qs”域展示了分配给每个进程的时钟数。通常情况下用户进程如Init子进程运行8个时钟，而时钟和系统任务可以最长运行64个时钟。不像用户空间服务器和驱动程序，时钟和系统任务即使阻塞了其他进程的运行机会，其优先级也不会降低。

每个队列内部都采用时间片轮转调试算法。如果一个运行进程的时间片用完了，则它移到队列尾部并分配一个新的时间片。而当一个阻塞的队列被唤醒时，如果阻塞前没有用完时间片，则它将被放到队首。它并不会得到一个新的时间片，而只能得到阻塞时剩余的时间片。数组rdy_tail使得向队列尾部加一个进程变得非常简单。当一个运行的进程被阻塞或者被一个信号杀死时，它将被移出队列，调度队列中仅有可运行的进程。

enqueue和dequeue函数就是负责把一个进程插入某个优先级队列或从就绪队列中删除一个进程。

下面来看一下sched函数，这个函数决定一个新就绪的进程应该放到哪个队列，是放在队列的队首还是队尾。每个进程的进程表项内记录有它的时间片、剩余时间片、优先级和所允许的最大优先级。

PRIVATE void sched(rp, queue,front)

registerstruct proc *rp;/*被调度的进程 */

int*queue;/*使用的优先级队列 */

int*front;/*队首或队尾 */

{

staticstruct proc *prev_ptr = NIL_PROC;/*创建一个空进程 */

inttime_left = (rp->p_ticks_left > 0);/*判断被调度的进程还有没有剩余的时间片*/

intpenalty = 0;/*罚值*/



if( ! time_left) {/*没有时间片剩余*/

rp->p_ticks_left= rp->p_quantum_size; /*赋予新的时间片 */

/*检查这个进程是否可以在其他进程没有运行的情况下连续运行再次，这种情况被视为可能的无限循环或至少是一个足够长的循环，处以+1的罚值。*/ 

if(prev_ptr == rp) penalty ++;

/*如果其他进程获得运行的机会，处以-1的罚值*/ 

elsepenalty --; 

prev_ptr= rp;

}



/*决定进程处于应放入哪个调度队列。其取值范围在IDLE和允许的最大优先级之间。

* 内核任务和空闲进程的优先级别永远不变

*/

if (penalty != 0 &&! iskernelp(rp)) {/*罚值不为0并且不是内核任务*/

rp->p_priority+= penalty;/*优先级按罚值增加*/

if (rp->p_priority <rp->p_max_priority)  /* 优先能让不能小于上限*/

rp->p_priority=rp->p_max_priority;

elseif (rp->p_priority > IDLE_Q-1)   /*优先级不能大于IDLE_Q的优先级*/

 rp->p_priority = IDLE_Q-1;

}



/*如果时间片有剩余，直接把进程插入队首，这样该进程可以立刻得到运行。

* 此时要选择的优先级队列就是进程当前所在的队列。

*/

*queue = rp->p_priority;

*front = time_left;

}

pick_proc函数的主要功能是设置next_ptr值。队列中所有可能影响选择下一个运行进程的改变都将引起pick_proc重新调用。

PRIVATE void pick_proc()

{

registerstruct proc *rp;/*要运行的进程 */

intq;/* 迭代变量*/

for(q=0; q < NR_SCHED_QUEUES; q++){/*遍历调度队列，NR_SCHED_QUEUES在kernel.proc.h中定义为16*/

if ( (rp = rdy_head[q])!= NIL_PROC) {/*如果队首进程不为空*/

next_ptr= rp;/*就运行队首进程*/

if(priv(rp)->s_flags & BILLABLE)/*priv函数是获取一个进程的系统优先级结构，在proc.h中。而priv结构体的定义在priv.h中。如果其成员项s_flags被标记为BILLABLE*/

bill_ptr= rp;/*把要运行的进程赋给bill_ptr。这样保证系统根据最后运行的用户进程所做的工作为系统时钟买单*/

return;

}

}

}














