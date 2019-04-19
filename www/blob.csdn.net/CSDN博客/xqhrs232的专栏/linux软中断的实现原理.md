# linux软中断的实现原理 - xqhrs232的专栏 - CSDN博客
2012年06月11日 21:05:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1246标签：[linux																[struct																[action																[任务																[list																[数据结构](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=任务&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::http://blog.csdn.net/liangjingbo/article/details/2817939
中断服务程序往往都是在CPU关中断的条件下执行的，以避免中断嵌套而使控制复杂化。但是CPU关中断的时间不能太长，否则容易丢失中断信号。为此， Linux将中断服务程序一分为二，各称作“Top Half”和“Bottom Half”。前者通常对时间要求较为严格，必须在中断请求发生后立即或至少在一定的时间限制内完成。因此为了保证这种处理能原子地完成，Top
 Half通常是在CPU关中断的条件下执行的。具体地说，Top Half的范围包括：从在IDT中登记的中断入口函数一直到驱动程序注册在中断服务队列中的ISR。而Bottom Half则是Top Half根据需要来调度执行的，这些操作允许延迟到稍后执行，它的时间要求并不严格，因此它通常是在CPU开中断的条件下执行的。 
但是， Linux的这种Bottom Half（以下简称BH）机制有两个缺点，也即：（1）在任意一时刻，系统只能有一个CPU可以执行Bottom Half代码，以防止两个或多个CPU同时来执行Bottom Half函数而相互干扰。因此BH代码的执行是严格“串行化”的。（2）BH函数不允许嵌套。 
这两个缺点在单CPU系统中是无关紧要的，但在SMP系统中却是非常致命的。因为BH机制的严格串行化执行显然没有充分利用SMP系统的多CPU特点。为此，Linux2.4内核在BH机制的基础上进行了扩展，这就是所谓的“软中断请求”（softirq）机制。 
Linux 的softirq机制是与SMP紧密不可分的。为此，整个softirq机制的设计与实现中自始自终都贯彻了一个思想：“谁触发，谁执行”（Who marks，Who runs），也即触发软中断的那个CPU负责执行它所触发的软中断，而且每个CPU都由它自己的软中断触发与控制机制。这个设计思想也使得softirq 机制充分利用了SMP系统的性能和特点。 
Linux在include/linux/interrupt.h头文件中定义了数据结构softirq_action，来描述一个软中断请求，如下所示： 
/* softirq mask and active fields moved to irq_cpustat_t in 
* asm/hardirq.h to get better cache usage. KAO 
*/ 
struct softirq_action 
{ 
void (*action)(struct softirq_action *); 
void *data; 
}; 
其中，函数指针action指向软中断请求的服务函数，而指针data则指向由服务函数自行解释的数据。 
基于上述软中断描述符，Linux在kernel/softirq.c文件中定义了一个全局的softirq_vec[32]数组： 
static struct softirq_action softirq_vec[32] __cacheline_aligned; 
在 这里系统一共定义了32个软中断请求描述符。软中断向量i（0≤i≤31）所对应的软中断请求描述符就是softirq_vec［i］。这个数组是个系统 全局数组，也即它被所有的CPU所共享。这里需要注意的一点是：每个CPU虽然都由它自己的触发和控制机制，并且只执行他自己所触发的软中断请求，但是各 个CPU所执行的软中断服务例程却是相同的，也即都是执行softirq_vec［］数组中定义的软中断服务函数。 
要实现“谁触发， 谁执行”的思想，就必须为每个CPU都定义它自己的触发和控制变量。为此，Linux在include/asm-i386/hardirq.h头文件中定 义了数据结构irq_cpustat_t来描述一个CPU的中断统计信息，其中就有用于触发和控制软中断的成员变量。数据结构irq_cpustat_t 的定义如下： 
/* entry.S is sensitive to the offsets of these fields */ 
typedef struct { 
unsigned int __softirq_active; 
unsigned int __softirq_mask; 
unsigned int __local_irq_count; 
unsigned int __local_bh_count; 
unsigned int __syscall_count; 
unsigned int __nmi_count; /* arch dependent */ 
} ____cacheline_aligned irq_cpustat_t; 
结构中每一个成员都是一个32位的无符号整数。其中__softirq_active和__softirq_mask就是用于触发和控制软中断的成员变量。 
①__softirq_active变量：32位的无符号整数，表示软中断向量0～31的状态。如果bit［i］（0≤i≤31）为1，则表示软中断向量i在某个CPU上已经被触发而处于active状态；为0表示处于非活跃状态。 
②__softirq_mask变量：32位的无符号整数，软中断向量的屏蔽掩码。如果bit［i］（0≤i≤31）为1，则表示使能（enable）软中断向量i，为0表示该软中断向量被禁止（disabled）。 
根据系统中当前的CPU个数（由宏NR_CPUS表示），Linux在kernel/softirq.c文件中为每个CPU都定义了它自己的中断统计信息结构，如下所示： 
/* No separate irq_stat for s390, it is part of PSA */ 
#if !defined(CONFIG_ARCH_S390) 
irq_cpustat_t irq_stat[NR_CPUS]; 
#endif /* CONFIG_ARCH_S390 */ 
这 样，每个CPU都只操作它自己的中断统计信息结构。假设有一个编号为id的CPU，那么它只能操作它自己的中断统计信息结构irq_stat［id］ （0≤id≤NR_CPUS-1），从而使各CPU之间互不影响。这个数组在include/linux/irq_cpustat.h头文件中也作了原型 声明。 
l 触发软中断请求的操作函数 
函数__cpu_raise_softirq()用于在编号为cpu的处理器上触发软中断向量nr。它通过将相应的__softirq_active成员变量中的相应位设置为1来实现软中断触发。如下所示（include/linux/interrupt.h）： 
static inline void __cpu_raise_softirq(int cpu, int nr) 
{ 
softirq_active(cpu) |= (1<<nr); 
} 
为 了保证“原子”性地完成软中断的触发过程，Linux在interrupt.h头文件中对上述内联函数又作了高层封装，也即函数 raise_softirq()。该函数向下通过调用__cpu_raise_softirq()函数来实现软中断的触发，但在调用该函数之前，它先通过 local_irq_save()函数来关闭当前CPU的中断并保存标志寄存器的内容，如下所示： 
/* I do not want to use atomic variables now, so that cli/sti */ 
static inline void raise_softirq(int nr) 
{ 
unsigned long flags; 
local_irq_save(flags); 
__cpu_raise_softirq(smp_processor_id(), nr); 
local_irq_restore(flags); 
} 
在软中断向量0～31中，Linux内核仅仅使用了软中断向量0～3，其余被留待系统以后扩展。Linux在头文件include/linux/interrupt.h中对软中断向量0～3进行了预定义： 
/* PLEASE, avoid to allocate new softirqs, if you need not _really_ high 
frequency threaded job scheduling. For almost all the purposes 
tasklets are more than enough. F.e. all serial device BHs et 
al. should be converted to tasklets, not to softirqs. 
*/ 
enum 
{ 
HI_SOFTIRQ=0, 
NET_TX_SOFTIRQ, 
NET_RX_SOFTIRQ, 
TASKLET_SOFTIRQ 
}; 
其 中，软中断向量0（即HI_SOFTIRQ）用于实现高优先级的软中断，如：高优先级的tasklet（将在后面详细描述）。软中断向量1和2则分别用于 网络数据的发送与接收。软中断向量3（即TASKLET_SOFTIRQ）则用于实现诸如tasklet这样的一般性软中断。关于tasklet我们将在 后面详细描述。NOTE！Linix内核并不鼓励一般用户扩展使用剩余的软中断向量，因为它认为其预定义的软中断向量HI_SOFTIRQ和 TASKLET_SOFTIRQ已经足够应付绝大多数应用。 
函数softirq_init()完成softirq机制的初始化。该函数由内核启动例程start_kernel()所调用。函数源码如下所示（kernel/softirq.c）： 
void __init softirq_init() 
{ 
int i; 
for (i=0; i<32; i++) 
tasklet_init(bh_task_vec+i, bh_action, i); 
open_softirq(TASKLET_SOFTIRQ, tasklet_action, NULL); 
open_softirq(HI_SOFTIRQ, tasklet_hi_action, NULL); 
} 
初始化的过程如下： 
（1）先用一个for循环来初始化用于实现BH机制的bh_task_vec［32］数组。这一点我们将在后面详细解释。 
（2） 调用open_softirq()函数开启使用软中断向量TASKLET_SOFTIRQ和HI_SOFTIRQ，并将它们的软中断服务函数指针分别指向 tasklet_action()函数和tasklet_hi_action（）函数。函数open_softirq()的主要作用是初始化设置软中断请 求描述符softirq_vec［nr］。 
函数open_softirq()用于开启一个指定的软中断向量nr，也即适当地 初始化软中断向量nr所对应的软中断描述符softirq_vec［nr］。它主要做两件事情：（1）初始化设置软中断向量nr所对应的软中断描述符 softirq_vec［nr］。（2）将所有CPU的软中断屏蔽掩码变量__softirq_mask中的对应位设置为1，以使能该软中断向量。该函数 的源码如下所示（kernel/softirq.c）： 
void open_softirq(int nr, void (*action)(struct softirq_action*), void *data) 
{ 
unsigned long flags; 
int i; 
spin_lock_irqsave(&softirq_mask_lock, flags); 
softirq_vec[nr].data = data; 
softirq_vec[nr].action = action; 
for (i=0; i<NR_CPUS; i++) 
softirq_mask(i) |= (1<<nr); 
spin_unlock_irqrestore(&softirq_mask_lock, flags); 
} 
函 数do_softirq()负责执行数组softirq_vec［32］中设置的软中断服务函数。每个CPU都是通过执行这个函数来执行软中断服务的。由 于同一个CPU上的软中断服务例程不允许嵌套，因此，do_softirq()函数一开始就检查当前CPU是否已经正出在中断服务中，如果是则 do_softirq()函数立即返回。举个例子，假设CPU0正在执行do_softirq()函数，执行过程产生了一个高优先级的硬件中断，于是 CPU0转去执行这个高优先级中断所对应的中断服务程序。总所周知，所有的中断服务程序最后都要跳转到do_IRQ()函数并由它来依次执行中断服务队列
 中的ISR，这里我们假定这个高优先级中断的ISR请求触发了一次软中断，于是do_IRQ()函数在退出之前看到有软中断请求，从而调用 do_softirq()函数来服务软中断请求。因此，CPU0再次进入do_softirq()函数（也即do_softirq()函数在CPU0上被 重入了）。但是在这一次进入do_softirq()函数时，它马上发现CPU0此前已经处在中断服务状态中了，因此这一次do_softirq()函数 立即返回。于是，CPU0回到该开始时的do_softirq()函数继续执行，并为高优先级中断的ISR所触发的软中断请求补上一次服务。从这里可以看
 出，do_softirq()函数在同一个CPU上的执行是串行的。 
函数源码如下（kernel/softirq.c）： 
asmlinkage void do_softirq() 
{ 
int cpu = smp_processor_id(); 
__u32 active, mask; 
if (in_interrupt()) 
return; 
local_bh_disable(); 
local_irq_disable(); 
mask = softirq_mask(cpu); 
active = softirq_active(cpu) & mask; 
if (active) { 
struct softirq_action *h; 
restart: 
/* Reset active bitmask before enabling irqs */ 
softirq_active(cpu) &= ~active; 
local_irq_enable(); 
h = softirq_vec; 
mask &= ~active; 
do { 
if (active & 1) 
h->action(h); 
h++; 
active >>= 1; 
} while (active); 
local_irq_disable(); 
active = softirq_active(cpu); 
if ((active &= mask) != 0) 
goto retry; 
} 
local_bh_enable(); 
/* Leave with locally disabled hard irqs. It is critical to close 
* window for infinite recursion, while we help local bh count, 
* it protected us. Now we are defenceless. 
*/ 
return; 
retry: 
goto restart; 
} 
结合上述源码，我们可以看出软中断服务的执行过程如下： 
（1）调用宏in_interrupt()来检测当前CPU此次是否已经处于中断服务中。该宏定义在hardirq.h，请参见5.7节。 
（2）调用local_bh_disable()宏将当前CPU的中断统计信息结构中的__local_bh_count成员变量加1，表示当前CPU已经处在软中断服务状态。 
（3）由于接下来要读写当前CPU的中断统计信息结构中的__softirq_active变量和__softirq_mask变量，因此为了保证这一个操作过程的原子性，先用local_irq_disable()宏（实际上就是cli指令）关闭当前CPU的中断。 
（4） 然后，读当前CPU的__softirq_active变量值和__softirq_mask变量值。当某个软中断向量被触发时（即 __softirq_active变量中的相应位被置1），只有__softirq_mask变量中的相应位也为1时，它的软中断服务函数才能得到执行。 因此，需要将__softirq_active变量和__softirq_mask变量作一次“与”逻辑操作。 
（5）如果active变量非 0，说明需要执行软中断服务函数。因此：①先将当前CPU的__softirq_active中的相应位清零，然后用local_irq_enable ()宏（实际上就是sti指令）打开当前CPU的中断。②将局部变量mask中的相应位清零，其目的是：让do_softirq()函数的这一次执行不对 同一个软中断向量上的再次软中断请求进行服务，而是将它留待下一次do_softirq()执行时去服务，从而使do_sottirq()函数避免陷入无 休止的软中断服务中。③用一个do{}while循环来根据active的值去执行相应的软中断服务函数。④由于接下来又要检测当前CPU的
 __softirq_active变量，因此再一次调用local_irq_disable()宏关闭当前CPU的中断。⑤读取当前CPU的 __softirq_active变量的值，并将它与局部变量mask进行与操作，以看看是否又有其他软中断服务被触发了（比如前面所说的那种情形）。如 果有的话，那就跳转到entry程序段（实际上是跳转到restart程序段）重新执行软中断服务。如果没有的话，那么此次软中断服务过程就宣告结束。 
（6）最后，通过local_bh_enable()宏将当前CPU的__local_bh_count变量值减1，表示当前CPU已经离开软中断服务状态。宏local_bh_enable()也定义在include/asm-i386/softirq.h头文件中。 
Tasklet机制是一种较为特殊的软中断。Tasklet一词的原意是“小片任务”的意思，这里是指一小段可执行的代码，且通常以函数的形式出现。软中断向量HI_SOFTIRQ和TASKLET_SOFTIRQ均是用tasklet机制来实现的。 
从 某种程度上讲，tasklet机制是Linux内核对BH机制的一种扩展。在2.4内核引入了softirq机制后，原有的BH机制正是通过 tasklet机制这个桥梁来纳入softirq机制的整体框架中的。正是由于这种历史的延伸关系，使得tasklet机制与一般意义上的软中断有所不 同，而呈现出以下两个显著的特点： 
1. 与一般的软中断不同，某一段tasklet代码在某个时刻只能在一个CPU上运行，而不像一般的软中断服务函数（即softirq_action结构中的action函数指针）那样??在同一时刻可以被多个CPU并发地执行。 
2. 与BH机制不同，不同的tasklet代码在同一时刻可以在多个CPU上并发地执行，而不像BH机制那样必须严格地串行化执行（也即在同一时刻系统中只能有一个CPU执行BH函数）。 
Linux用数据结构tasklet_struct来描述一个tasklet。该数据结构定义在include/linux/interrupt.h头文件中。如下所示： 
struct tasklet_struct 
{ 
struct tasklet_struct *next; 
unsigned long state; 
atomic_t count; 
void (*func)(unsigned long); 
unsigned long data; 
}; 
各成员的含义如下： 
（1）next指针：指向下一个tasklet的指针。 
（2） state：定义了这个tasklet的当前状态。这一个32位的无符号长整数，当前只使用了bit［1］和bit［0］两个状态位。其中，bit［1］ ＝1表示这个tasklet当前正在某个CPU上被执行，它仅对SMP系统才有意义，其作用就是为了防止多个CPU同时执行一个tasklet的情形出 现；bit［0］＝1表示这个tasklet已经被调度去等待执行了。对这两个状态位的宏定义如下所示（interrupt.h）： 
enum 
{ 
TASKLET_STATE_SCHED, /* Tasklet is scheduled for execution */ 
TASKLET_STATE_RUN /* Tasklet is running (SMP only) */ 
}; 
（3） 原子计数count：对这个tasklet的引用计数值。NOTE！只有当count等于0时，tasklet代码段才能执行，也即此时tasklet是 被使能的；如果count非零，则这个tasklet是被禁止的。任何想要执行一个tasklet代码段的人都首先必须先检查其count成员是否为0。 
（4）函数指针func：指向以函数形式表现的可执行tasklet代码段。 
（5）data：函数func的参数。这是一个32位的无符号整数，其具体含义可供func函数自行解释，比如将其解释成一个指向某个用户自定义数据结构的地址值。 
Linux在interrupt.h头文件中又定义了两个用来定义tasklet_struct结构变量的辅助宏： 
#define DECLARE_TASKLET(name, func, data) / 
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data } 
#define DECLARE_TASKLET_DISABLED(name, func, data) / 
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(1), func, data } 
显 然，从上述源代码可以看出，用DECLARE_TASKLET宏定义的tasklet在初始化时是被使能的（enabled），因为其count成员为 0。而用DECLARE_TASKLET_DISABLED宏定义的tasklet在初始时是被禁止的（disabled），因为其count等于1。 
在这里，tasklet状态指两个方面：（1）state成员所表示的运行状态；（2）count成员决定的使能／禁止状态。 
（1）改变一个tasklet的运行状态 
state 成员中的bit［0］表示一个tasklet是否已被调度去等待执行，bit［1］表示一个tasklet是否正在某个CPU上执行。对于state变量 中某位的改变必须是一个原子操作，因此可以用定义在include/asm/bitops.h头文件中的位操作来进行。 
由于bit［1］这一位（即TASKLET_STATE_RUN）仅仅对于SMP系统才有意义，因此Linux在Interrupt.h头文件中显示地定义了对TASKLET_STATE_RUN位的操作。如下所示： 
#ifdef CONFIG_SMP 
#define tasklet_trylock(t) (!test_and_set_bit(TASKLET_STATE_RUN, &(t)->state)) 
#define tasklet_unlock_wait(t) while (test_bit(TASKLET_STATE_RUN, &(t)->state)) { /* NOTHING */ } 
#define tasklet_unlock(t) clear_bit(TASKLET_STATE_RUN, &(t)->state) 
#else 
#define tasklet_trylock(t) 1 
#define tasklet_unlock_wait(t) do { } while (0) 
#define tasklet_unlock(t) do { } while (0) 
#endif 
显 然，在SMP系统同，tasklet_trylock()宏将把一个tasklet_struct结构变量中的state成员中的bit［1］位设置成 1，同时还返回bit［1］位的非。因此，如果bit［1］位原有值为1（表示另外一个CPU正在执行这个tasklet代码），那么 tasklet_trylock()宏将返回值0，也就表示上锁不成功。如果bit［1］位的原有值为0，那么tasklet_trylock()宏将返 回值1，表示加锁成功。而在单CPU系统中，tasklet_trylock()宏总是返回为1。 
任何想要执行某个tasklet代码的程序 都必须首先调用宏tasklet_trylock()来试图对这个tasklet进行上锁（即设置TASKLET_STATE_RUN位），且只能在上锁 成功的情况下才能执行这个tasklet。建议！即使你的程序只在CPU系统上运行，你也要在执行tasklet之前调用tasklet_trylock ()宏，以便使你的代码获得良好可移植性。 
在SMP系统中，tasklet_unlock_wait()宏将一直不停地测试 TASKLET_STATE_RUN位的值，直到该位的值变为0（即一直等待到解锁），假如：CPU0正在执行tasklet A的代码，在此期间，CPU1也想执行tasklet A的代码，但CPU1发现tasklet A的TASKLET_STATE_RUN位为1，于是它就可以通过tasklet_unlock_wait()宏等待tasklet A被解锁（也即TASKLET_STATE_RUN位被清零）。在单CPU系统中，这是一个空操作。 
宏tasklet_unlock()用来对一个tasklet进行解锁操作，也即将TASKLET_STATE_RUN位清零。在单CPU系统中，这是一个空操作。 
（2）使能／禁止一个tasklet 
使能与禁止操作往往总是成对地被调用的，tasklet_disable()函数如下（interrupt.h）： 
static inline void tasklet_disable(struct tasklet_struct *t) 
{ 
tasklet_disable_nosync(t); 
tasklet_unlock_wait(t); 
} 
函数tasklet_disable_nosync()也是一个静态inline函数，它简单地通过原子操作将count成员变量的值减1。如下所示（interrupt.h）： 
static inline void tasklet_disable_nosync(struct tasklet_struct *t) 
{ 
atomic_inc(&t->count); 
} 
函数tasklet_enable()用于使能一个tasklet，如下所示（interrupt.h）： 
static inline void tasklet_enable(struct tasklet_struct *t) 
{ 
atomic_dec(&t->count); 
} 
函数tasklet_init()用来初始化一个指定的tasklet描述符，其源码如下所示（kernel/softirq.c）： 
void tasklet_init(struct tasklet_struct *t, 
void (*func)(unsigned long), unsigned long data) 
{ 
t->func = func; 
t->data = data; 
t->state = 0; 
atomic_set(&t->count, 0); 
} 
函数tasklet_kill()用来将一个已经被调度了的tasklet杀死，即将其恢复到未调度的状态。其源码如下所示（kernel/softirq.c）： 
void tasklet_kill(struct tasklet_struct *t) 
{ 
if (in_interrupt()) 
printk("Attempt to kill tasklet from interrupt/n"); 
while (test_and_set_bit(TASKLET_STATE_SCHED, &t->state)) { 
current->state = TASK_RUNNING; 
do { 
current->policy |= SCHED_YIELD; 
schedule(); 
} while (test_bit(TASKLET_STATE_SCHED, &t->state)); 
} 
tasklet_unlock_wait(t); 
clear_bit(TASKLET_STATE_SCHED, &t->state); 
} 
多个tasklet可以通过tasklet描述符中的next成员指针链接成一个单向对列。为此，Linux专门在头文件include/linux/interrupt.h中定义了数据结构tasklet_head来描述一个tasklet对列的头部指针。如下所示： 
struct tasklet_head 
{ 
struct tasklet_struct *list; 
} __attribute__ ((__aligned__(SMP_CACHE_BYTES))); 
尽 管tasklet机制是特定于软中断向量HI_SOFTIRQ和TASKLET_SOFTIRQ的一种实现，但是tasklet机制仍然属于 softirq机制的整体框架范围内的，因此，它的设计与实现仍然必须坚持“谁触发，谁执行”的思想。为此，Linux为系统中的每一个CPU都定义了一 个tasklet对列头部，来表示应该有各个CPU负责执行的tasklet对列。如下所示（kernel/softirq.c）： 
struct tasklet_head tasklet_vec[NR_CPUS] __cacheline_aligned; 
struct tasklet_head tasklet_hi_vec[NR_CPUS] __cacheline_aligned; 
其 中，tasklet_vec［］数组用于软中断向量TASKLET_SOFTIRQ，而tasklet_hi_vec［］数组则用于软中断向量 HI_SOFTIRQ。也即，如果CPUi（0≤i≤NR_CPUS-1）触发了软中断向量TASKLET_SOFTIRQ，那么对列 tasklet_vec［i］中的每一个tasklet都将在CPUi服务于软中断向量TASKLET_SOFTIRQ时被CPUi所执行。同样地，如果 CPUi（0≤i≤NR_CPUS-1）触发了软中断向量HI_SOFTIRQ，那么队列tasklet_vec［i］中的每一个tasklet都将
 CPUi在对软中断向量HI_SOFTIRQ进行服务时被CPUi所执行。 
队列tasklet_vec［I］和tasklet_hi_vec ［I］中的各个tasklet是怎样被所CPUi所执行的呢？其关键就是软中断向量TASKLET_SOFTIRQ和HI_SOFTIRQ的软中断服务程 序??tasklet_action()函数和tasklet_hi_action()函数。下面我们就来分析这两个函数。 
Linux 为软中断向量TASKLET_SOFTIRQ和HI_SOFTIRQ实现了专用的触发函数和软中断服务函数。其中，tasklet_schedule() 函数和tasklet_hi_schedule()函数分别用来在当前CPU上触发软中断向量TASKLET_SOFTIRQ和HI_SOFTIRQ，并 把指定的tasklet加入当前CPU所对应的tasklet队列中去等待执行。而tasklet_action()函数和 tasklet_hi_action()函数则分别是软中断向量TASKLET_SOFTIRQ和HI_SOFTIRQ的软中断服务函数。在初始化函数
 softirq_init()中，这两个软中断向量对应的描述符softirq_vec［0］和softirq_vec［3］中的action函数指针就 被分别初始化成指向函数tasklet_hi_action()和函数tasklet_action（）。 
（1）软中断向量TASKLET_SOFTIRQ的触发函数tasklet_schedule（） 
该函数实现在include/linux/interrupt.h头文件中，是一个inline函数。其源码如下所示： 
static inline void tasklet_schedule(struct tasklet_struct *t) 
{ 
if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state)) { 
int cpu = smp_processor_id(); 
unsigned long flags; 
local_irq_save(flags); 
t->next = tasklet_vec[cpu].list; 
tasklet_vec[cpu].list = t; 
__cpu_raise_softirq(cpu, TASKLET_SOFTIRQ); 
local_irq_restore(flags); 
} 
} 
该函数的参数t指向要在当前CPU上被执行的tasklet。对该函数的NOTE如下： 
① 调用test_and_set_bit()函数将待调度的tasklet的state成员变量的bit［0］位（也即 TASKLET_STATE_SCHED位）设置为1，该函数同时还返回TASKLET_STATE_SCHED位的原有值。因此如果bit［0］为的原 有值已经为1，那就说明这个tasklet已经被调度到另一个CPU上去等待执行了。由于一个tasklet在某一个时刻只能由一个CPU来执行，因此 tasklet_schedule()函数什么也不做就直接返回了。否则，就继续下面的调度操作。 
②首先，调用local_irq_save()函数来关闭当前CPU的中断，以保证下面的步骤在当前CPU上原子地被执行。 
③然后，将待调度的tasklet添加到当前CPU对应的tasklet队列的首部。 
④接着，调用__cpu_raise_softirq()函数在当前CPU上触发软中断请求TASKLET_SOFTIRQ。 
⑤最后，调用local_irq_restore()函数来开当前CPU的中断。 
（2）软中断向量TASKLET_SOFTIRQ的服务程序tasklet_action（） 
函 数tasklet_action()是tasklet机制与软中断向量TASKLET_SOFTIRQ的联系纽带。正是该函数将当前CPU的 tasklet队列中的各个tasklet放到当前CPU上来执行的。该函数实现在kernel/softirq.c文件中，其源代码如下： 
static void tasklet_action(struct softirq_action *a) 
{ 
int cpu = smp_processor_id(); 
struct tasklet_struct *list; 
local_irq_disable(); 
list = tasklet_vec[cpu].list; 
tasklet_vec[cpu].list = NULL; 
local_irq_enable(); 
while (list != NULL) { 
struct tasklet_struct *t = list; 
list = list->next; 
if (tasklet_trylock(t)) { 
if (atomic_read(&t->count) == 0) { 
clear_bit(TASKLET_STATE_SCHED, &t->state); 
t->func(t->data); 
/* 
* talklet_trylock() uses test_and_set_bit that imply 
* an mb when it returns zero, thus we need the explicit 
* mb only here: while closing the critical section. 
*/ 
#ifdef CONFIG_SMP 
smp_mb__before_clear_bit(); 
#endif 
tasklet_unlock(t); 
continue; 
} 
tasklet_unlock(t); 
} 
local_irq_disable(); 
t->next = tasklet_vec[cpu].list; 
tasklet_vec[cpu].list = t; 
__cpu_raise_softirq(cpu, TASKLET_SOFTIRQ); 
local_irq_enable(); 
} 
} 
注释如下： 
① 首先，在当前CPU关中断的情况下，“原子”地读取当前CPU的tasklet队列头部指针，将其保存到局部变量list指针中，然后将当前CPU的 tasklet队列头部指针设置为NULL，以表示理论上当前CPU将不再有tasklet需要执行（但最后的实际结果却并不一定如此，下面将会看到）。 
②然后，用一个while{}循环来遍历由list所指向的tasklet队列，队列中的各个元素就是将在当前CPU上执行的tasklet。循环体的执行步骤如下： 
l 用指针t来表示当前队列元素，即当前需要执行的tasklet。 
l 更新list指针为list->next，使它指向下一个要执行的tasklet。 
l 用tasklet_trylock()宏试图对当前要执行的tasklet（由指针t所指向）进行加锁，如果加锁成功（当前没有任何其他CPU正在执行这 个tasklet），则用原子读函数atomic_read()进一步判断count成员的值。如果count为0，说明这个tasklet是允许执行 的，于是：（1）先清除TASKLET_STATE_SCHED位；（2）然后，调用这个tasklet的可执行函数func；（3）执行barrier ()操作；（4）调用宏tasklet_unlock()来清除TASKLET_STATE_RUN位。（5）最后，执行continue语句跳过下面的
 步骤，回到while循环继续遍历队列中的下一个元素。如果count不为0，说明这个tasklet是禁止运行的，于是调用 tasklet_unlock()清除前面用tasklet_trylock()设置的TASKLET_STATE_RUN位。 
l 如果tasklet_trylock()加锁不成功，或者因为当前tasklet的count值非0而不允许执行时，我们必须将这个tasklet重新放 回到当前CPU的tasklet队列中，以留待这个CPU下次服务软中断向量TASKLET_SOFTIRQ时再执行。为此进行这样几步操作：（1）先关 CPU中断，以保证下面操作的原子性。（2）把这个tasklet重新放回到当前CPU的tasklet队列的首部；（3）调用 __cpu_raise_softirq()函数在当前CPU上再触发一次软中断请求TASKLET_SOFTIRQ；（4）开中断。 
l 最后，回到while循环继续遍历队列。 
（3）软中断向量HI_SOFTIRQ的触发函数tasklet_hi_schedule() 
该函数与tasklet_schedule()几乎相同，其源码如下（include/linux/interrupt.h）： 
static inline void tasklet_hi_schedule(struct tasklet_struct *t) 
{ 
if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state)) { 
int cpu = smp_processor_id(); 
unsigned long flags; 
local_irq_save(flags); 
t->next = tasklet_hi_vec[cpu].list; 
tasklet_hi_vec[cpu].list = t; 
__cpu_raise_softirq(cpu, HI_SOFTIRQ); 
local_irq_restore(flags); 
} 
} 
（4）软中断向量HI_SOFTIRQ的服务函数tasklet_hi_action（） 
该函数与tasklet_action()函数几乎相同，其源码如下（kernel/softirq.c）： 
static void tasklet_hi_action(struct softirq_action *a) 
{ 
int cpu = smp_processor_id(); 
struct tasklet_struct *list; 
local_irq_disable(); 
list = tasklet_hi_vec[cpu].list; 
tasklet_hi_vec[cpu].list = NULL; 
local_irq_enable(); 
while (list != NULL) { 
struct tasklet_struct *t = list; 
list = list->next; 
if (tasklet_trylock(t)) { 
if (atomic_read(&t->count) == 0) { 
clear_bit(TASKLET_STATE_SCHED, &t->state); 
t->func(t->data); 
tasklet_unlock(t); 
continue; 
} 
tasklet_unlock(t); 
} 
local_irq_disable(); 
t->next = tasklet_hi_vec[cpu].list; 
tasklet_hi_vec[cpu].list = t; 
__cpu_raise_softirq(cpu, HI_SOFTIRQ); 
local_irq_enable(); 
} 
} 
Bottom Half机制在新的softirq机制中被保留下来，并作为softirq框架的一部分。其实现也似乎更为复杂些，因为它是通过tasklet机制这个中 介桥梁来纳入softirq框架中的。实际上，软中断向量HI_SOFTIRQ是内核专用于执行BH函数的。 
原有的32个BH函数指针被保留，定义在kernel/softirq.c文件中： 
static void (*bh_base[32])(void); 
但是，每个BH函数都对应有一个tasklet，并由tasklet的可执行函数func来负责调用相应的bh函数（func函数的参数指定调用哪一个BH函数）。与32个BH函数指针相对应的tasklet的定义如下所示（kernel/softirq.c）： 
struct tasklet_struct bh_task_vec[32]; 
上述tasklet数组使系统全局的，它对所有的CPU均可见。由于在某一个时刻只能有一个CPU在执行BH函数，因此定义一个全局的自旋锁来保护BH函数，如下所示（kernel/softirq.c）： 
spinlock_t global_bh_lock = SPIN_LOCK_UNLOCKED; 
在softirq 机制的初始化函数softirq_init()中将bh_task_vec［32］数组中的每一个tasklet中的func函数指针都设置为指向同一个 函数bh_action，而data成员（也即func函数的调用参数）则被设置成该tasklet在数组中的索引值，如下所示： 
void __init softirq_init() 
{ 
…… 
for (i=0; i<32; i++) 
tasklet_init(bh_task_vec+i, bh_action, i); 
…… 
} 
因此，bh_action()函数将负责相应地调用参数所指定的bh函数。该函数是连接tasklet机制与Bottom Half机制的关键所在。 
该函数的源码如下（kernel/softirq.c）： 
static void bh_action(unsigned long nr) 
{ 
int cpu = smp_processor_id(); 
if (!spin_trylock(&global_bh_lock)) 
goto resched; 
if (!hardirq_trylock(cpu)) 
goto resched_unlock; 
if (bh_base[nr]) 
bh_base[nr](); 
hardirq_endlock(cpu); 
spin_unlock(&global_bh_lock); 
return; 
resched_unlock: 
spin_unlock(&global_bh_lock); 
resched: 
mark_bh(nr); 
} 
对该函数的注释如下： 
① 首先，调用spin_trylock()函数试图对自旋锁global_bh_lock进行加锁，同时该函数还将返回自旋锁global_bh_lock 的原有值的非。因此，如果global_bh_lock已被某个CPU上锁而为非0值（那个CPU肯定在执行某个BH函数），那么 spin_trylock()将返回为0表示上锁失败，在这种情况下，当前CPU是不能执行BH函数的，因为另一个CPU正在执行BH函数，于是执行 goto语句跳转到resched程序段，以便在当前CPU上再一次调度该BH函数。 
②调用hardirq_trylock()函数锁定当前CPU，确保当前CPU不是处于硬件中断请求服务中，如果锁定失败，跳转到resched_unlock程序段，以便先对global_bh_lock解锁，在重新调度一次该BH函数。 
③此时，我们已经可以放心地在当前CPU上执行BH函数了。当然，对应的BH函数指针bh_base［nr］必须有效才行。 
④从BH函数返回后，先调用hardirq_endlock()函数（实际上它什么也不干，调用它只是为了保此加、解锁的成对关系），然后解除自旋锁global_bh_lock，最后函数就可以返回了。 
⑤resched_unlock程序段：先解除自旋锁global_bh_lock，然后执行reched程序段。 
⑥resched程序段：当某个CPU正在执行BH函数时，当前CPU就不能通过bh_action（）函数来调用执行任何BH函数，所以就通过调用mark_bh()函数在当前CPU上再重新调度一次，以便将这个BH函数留待下次软中断服务时执行。 
（1）init_bh()函数 
该函数用来在bh_base［］数组登记一个指定的bh函数，如下所示（kernel/softirq.c）： 
void init_bh(int nr, void (*routine)(void)) 
{ 
bh_base[nr] = routine; 
mb(); 
} 
（2）remove_bh()函数 
该函数用来在bh_base［］数组中注销指定的函数指针，同时将相对应的tasklet杀掉。如下所示（kernel/softirq.c）： 
void remove_bh(int nr) 
{ 
tasklet_kill(bh_task_vec+nr); 
bh_base[nr] = NULL; 
} 
（3）mark_bh()函数 
该 函数用来向当前CPU标记由一个BH函数等待去执行。它实际上通过调用tasklet_hi_schedule()函数将相应的tasklet加入到当前 CPU的tasklet队列tasklet_hi_vec［cpu］中，然后触发软中断请求HI_SOFTIRQ，如下所示 （include/linux/interrupt.h）： 
static inline void mark_bh(int nr) 
{ 
tasklet_hi_schedule(bh_task_vec+nr); 
} 
在32个BH函数指针中，大多数已经固定用于一些常见的外设，比如：第0个BH函数就固定地用于时钟中断。Linux在头文件include/linux/interrupt.h中定义了这些已经被使用的BH函数所引，如下所示： 
enum { 
TIMER_BH = 0, 
TQUEUE_BH, 
DIGI_BH, 
SERIAL_BH, 
RISCOM8_BH, 
SPECIALIX_BH, 
AURORA_BH, 
ESP_BH, 
SCSI_BH, 
IMMEDIATE_BH, 
CYCLADES_BH, 
CM206_BH, 
JS_BH, 
MACSERIAL_BH, 
ISICOM_BH 
}; 
任务队列是与Bottom Half机制紧密相连的。因为Bottom Half机制只有有限的32个函数指针，而且大部分都已被系统预定义使用，所以早期版本的Linux内核为了扩展Bottom Half机制，就设计了任务队列机制。 
所谓任务队列就是指以双向队列形式连接起来的任务链表，每一个链表元数都描述了一个可执行的任务（以函数的形式表现）。如下图所示： 
任务队列机制实现在include/linux/tqueue.h头文件中。 
Linux用数据结构tq_struct来描述任务队列中的每一个链表元数（即一个可执行的任务）： 
struct tq_struct { 
struct list_head list; /* linked list of active bh's */ 
unsigned long sync; /* must be initialized to zero */ 
void (*routine)(void *); /* function to call */ 
void *data; /* argument to function */ 
}; 
这个数据结构很简单，在此就不详述。 
然后，Linux定义了数据结构task_queue来描述任务队列的头部，其实task_queue就是结构类型list_head，如下： 
typedef struct list_head task_queue; 
但是Linux又定义了一个宏DECLARE_TASK_QUEUE()来辅助我们更方便地定义任务队列的链表表头： 
#define DECLARE_TASK_QUEUE(q) LIST_HEAD(q) 
一个任务队列是否处于active状态主要取决于其链表表头（即task_queue结构）是否为空，因此Linux定义宏TQ_ACTIVE（）来判断一个任务队列是否有效： 
#define TQ_ACTIVE(q) (!list_empty(&q)) 
显然，只要任务队列表头q不为空，该任务队列就是有效的。 
（1）保护自旋锁 
由于任务队列是系统全局的共享资源，所以面临竞争的问题。为了实现对任务队列链表的互斥访问，Linux在kernel/timer.c文件中定义了一个任务队列保护自旋锁tqueue_lock，如下： 
spinlock_t tqueue_lock = SPIN_LOCK_UNLOCKED; 
该自旋锁在tqueue.h头文件中也有原型声明： 
extern spinlock_t tqueue_lock; 
任何想要访问任务队列的代码都首先必须先持有该自旋锁。 
（2）queue_task（）函数 
实现在tqueue.h头文件中的内联函数queue_task()用来将一个指定的任务添加到某指定的任务队列的尾部，如下： 
/* 
* Queue a task on a tq. Return non-zero if it was successfully 
* added. 
*/ 
static inline int queue_task(struct tq_struct *bh_pointer, task_queue *bh_list) 
{ 
int ret = 0; 
if (!test_and_set_bit(0,&bh_pointer->sync)) { 
unsigned long flags; 
spin_lock_irqsave(&tqueue_lock, flags); 
list_add_tail(&bh_pointer->list, bh_list); 
spin_unlock_irqrestore(&tqueue_lock, flags); 
ret = 1; 
} 
return ret; 
} 
函数run_task_queue()用于实现指定的任务队列。它只有一个参数：指针list??指向待运行的任务队列头部task_queue结构变量。该函数实现在tqueue.h头文件中： 
static inline void run_task_queue(task_queue *list) 
{ 
if (TQ_ACTIVE(*list)) 
__run_task_queue(list); 
} 
显然，函数首先调用宏TQ_ACTIVE()来判断参数list指定的待运行任务队列是否为空。如果不为空，则调用__run_task_queue()函数来实际运行这个有效的任务队列。 
函数__run_task_queue()实现在kernel/softirq.c文件中。该函数将依次遍历任务队列中的每一个元数，并调用执行每一个元数的可执行函数。其源码如下： 
void __run_task_queue(task_queue *list) 
{ 
struct list_head head, *next; 
unsigned long flags; 
spin_lock_irqsave(&tqueue_lock, flags); 
list_add(&head, list); 
list_del_init(list); 
spin_unlock_irqrestore(&tqueue_lock, flags); 
next = head.next; 
while (next != &head) { 
void (*f) (void *); 
struct tq_struct *p; 
void *data; 
p = list_entry(next, struct tq_struct, list); 
next = next->next; 
f = p->routine; 
data = p->data; 
wmb(); 
p->sync = 0; 
if (f) 
f(data); 
} 
} 
对该函数的注释如下： 
（1） 首先，用一个局部的表头head来代替参数list所指向的表头。这是因为：在__run_task_queue（）函数的运行期间可能还会有新的任务加 入到list任务队列中来，但是__run_task_queue()函数显然不想陷入无休止的不断增加的任务处理中，因此它用局部的表头head来代替 参数list所指向的表头，以使要执行的任务个数固定化。为此：①先对全局的自旋锁tqueue_lock进行加锁，以实现对任务队列的互斥访问；②将局 部的表头head加在表头（＊list）和第一个元数之间。③将（＊list）表头从队列中去除，并将其初始化为空。④解除自旋锁
 tqueue_lock。 
（2）接下来，用一个while循环来遍历整个队列head，并调用执行每一个队列元素中的函数。注意！任务队列是一个双向循环队列。 
Bottom Half机制与任务队列是紧密相连的。大多数BH函数都是通过调用run_task_queue()函数来执行某个预定义好的任务队列。最常见的内核预定义任务队列有： 
l tq_timer：对应于TQUEUE_BH。 
l tq_immediate：对应于IMMEDIATE_BH。 
l tq_disk：用于块设备任务。 
任务队列tq_timer和tq_immediate都定义在kernel/timer.c文件中，如下所示： 
DECLARE_TASK_QUEUE(tq_timer); 
DECLARE_TASK_QUEUE(tq_immediate); 
BH 向量TQUEUE_BH和IMMEDIATE_BH的BH函数分别是：queue_bh（）函数和immediate_bh()函数，它们都仅仅是简单地 调用run_task_queue()函数来分别运行任务队列tq_timer和tq_immediate，如下所示（kernel/timer.c）： 
void tqueue_bh(void) 
{ 
run_task_queue(&tq_timer); 
} 
void immediate_bh(void) 
{ 
run_task_queue(&tq_immediate); 
} 
出处：
[http://hi.baidu.com/romashell/blog/item/fba9100ea1b479e136d12292.html](http://hi.baidu.com/romashell/blog/item/fba9100ea1b479e136d12292.html)
