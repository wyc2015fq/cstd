# Tasklet机制 - h13 - 博客园
一、tasklet使用
Tasklet的使用比较简单，只需要定义tasklet及其处理函数并将两者关联
例子：
Void my_tasklet_func(unsigned long)
DECLARE_TASKLET(my_tasklet.my_tasklet_func,data)
代码DECLARE_TASKLET实现了定义名称为my_tasklet的tasklet并将其与my_tasklet_func这个函数绑定，而传入这个函数的参数为data。
需要调度tasklet的时候引用一个tasklet_schedule()函数就能使系统在适当的时候进行调度，如下所示
Tasklet_schedule(&my_tasklet)
下面给出驱动模板
```
void xxx_do_tasklet(unsigned long);
DECLARE_TASKLET(xxx_tasklet,xxx_do_tasklet,0);
void xxx_do_tasklet(unsigned long)
{
……
}
irqreturn_t xxx_interrupt(int irq,void *dev_id,struct pt_regs *regs)
{
      ……
      tasklet_schedule(&xxx_tasklet);
      ……
}
int _init xxx_init(void)
{
      ……
      result=request_irq(xxx_irq,xxx_interrupt,SA_INTERRUPT,”xxx”,NULL)
      ……
}
void _exit xxx_exit(void)
{
      ……
      free_irq(xxx_irq,xxx_irq_interrupt);
      ……
}
```
二、tasklet函数详解
它对于中断处理特别有用：硬件中断必须尽快处理, 但大部分的数据管理可以延后到以后安全的时间执行。
tasklet 以一个数据结构形式存在，使用前必须被初始化。初始化能够通过调用一个特定函数或者通过使用某些宏定义声明结构: 
```
#include <linux/interrupt.h> 
struct tasklet_struct
{
    struct tasklet_struct *next;
    unsigned long state;
    atomic_t count;
    void (*func)(unsigned long);
    unsigned long data;
};
void tasklet_init(struct tasklet_struct *t,
 void (*func)(unsigned long), unsigned long data);
#define DECLARE_TASKLET(name, func, data) \
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data }
#define DECLARE_TASKLET_DISABLED(name, func, data) \
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(1), func, data }
void tasklet_disable(struct tasklet_struct *t); 
/*函数暂时禁止给定的 tasklet被 tasklet_schedule 调度，直到这个 tasklet 被再次被enable；
若这个 tasklet 当前在运行, 这个函数忙等待直到这个tasklet退出*/
void tasklet_disable_nosync(struct tasklet_struct *t); 
/*和tasklet_disable类似，但是tasklet可能仍然运行在另一个 CPU */
void tasklet_enable(struct tasklet_struct *t); 
/*使能一个之前被disable的 tasklet;若这个 tasklet 已经被调度, 它会很快运行。 tasklet_enable
 和tasklet_disable必须匹配调用, 因为内核跟踪每个 tasklet 的"禁止次数"*/ 
void tasklet_schedule(struct tasklet_struct *t); 
/*调度 tasklet 执行，如果tasklet在运行中被调度, 它在完成后会再次运行; 这保证了在其他事件被处理
当中发生的事件受到应有的注意. 这个做法也允许一个 tasklet 重新调度它自己*/
void tasklet_hi_schedule(struct tasklet_struct *t); 
/*和tasklet_schedule类似，只是在更高优先级执行。当软中断处理运行时, 它处理高优先级 tasklet 在
其他软中断之前，只有具有低响应周期要求的驱动才应使用这个函数, 可避免其他软件中断处理引入的附加周期*/
void tasklet_kill(struct tasklet_struct *t); 
/*确保了 tasklet 不会被再次调度来运行，通常当一个设备正被关闭或者模块卸载时被调用。如果 tasklet
 正在运行, 这个函数等待直到它执行完毕。若 tasklet 重新调度它自己，则必须阻止在调用 tasklet_kill 
前它重新调度它自己，如同使用 del_timer_sync*/
```
**工作队列**
一、       工作队列的使用
定义一个工作队列
Struct work_struct my_wq;
Void my_wq_func(unsigned long);
通过INIT_WORK可以初始化这个工作队列并将工作队列与处理函数绑定
INIT_WORK(&my_wq,(void(*)(void*))my_wq_func,NULL)
调度工作队列
Schedule_work(&my_wq)
使用模板
```
struct work_struct xxx_wq;
void xxx_do_work(unsigned long);
void xxx_do_work(unsigned long)
{
      ……
}
irqreturn_t xxx_interrupt(int irq,void *dev_id,struct pt_regs *regs)
{
      ……
      schedule_work(&xxx_wq);
      ……
}
int init(void)
{
      ……
      result=request_irq(xxx_irq,xxx_interrupt,SA_INTERRUPT,“xxx”,NULL)
      ……
      INIT_WORK(&xxx_wq,(void(*)(void*))xxx_do_work,NULL);
      …….
}
void xxx_exit(void)
{
      ……
      free_irq(xxx_irq,xxx_interrupt);
      ……
}
```
二、       工作队列函数详解
工作队列有 struct workqueue_struct 类型，在 <linux/workqueue.h> 中定义。一个工作队列必须明确的在使用前创建，宏为:
struct workqueue_struct *create_workqueue(const char *name);
struct workqueue_struct *create_singlethread_workqueue(const char *name);
每个工作队列有一个或多个专用的进程("内核线程"), 这些进程运行提交给这个队列的函数。 若使用 create_workqueue, 就得到一个工作队列它在系统的每个处理器上有一个专用的线程。在很多情况下，过多线程对系统性能有影响，如果单个线程就足够则使用 create_singlethread_workqueue 来创建工作队列。
提交一个任务给一个工作队列，在这里《LDD3》介绍的内核2.6.10和我用的新内核2.6.22.2已经有不同了，老接口已经不能用了，编译会出错。这里我只讲2.6.22.2的新接口，至于老的接口我想今后内核不会再有了。从这一点我们可以看出内核发展。 
```
/*需要填充work_struct或delayed_work结构，可以在编译时完成, 宏如下: */
struct work_struct {
    atomic_long_t data;
#define WORK_STRUCT_PENDING 0        /* T if work item pending execution */
#define WORK_STRUCT_FLAG_MASK (3UL)
#define WORK_STRUCT_WQ_DATA_MASK (~WORK_STRUCT_FLAG_MASK)
    struct list_head entry;
    work_func_t func;
};
struct delayed_work {
    struct work_struct work;
    struct timer_list timer;
};
DECLARE_WORK(n, f)    
/*n 是声明的work_struct结构名称, f是要从工作队列被调用的函数*/
DECLARE_DELAYED_WORK(n, f)
/*n是声明的delayed_work结构名称, f是要从工作队列被调用的函数*/
/*若在运行时需要建立 work_struct 或 delayed_work结构, 使用下面 2 个宏定义:*/
INIT_WORK(struct work_struct *work, void (*function)(void *)); 
PREPARE_WORK(struct work_struct *work, void (*function)(void *)); 
INIT_DELAYED_WORK(struct delayed_work *work, void (*function)(void *)); 
PREPARE_DELAYED_WORK(struct delayed_work *work, void (*function)(void *)); 
/* INIT_* 做更加全面的初始化结构的工作，在第一次建立结构时使用. PREPARE_* 做几乎同样的工作, 
但是它不初始化用来连接 work_struct或delayed_work 结构到工作队列的指针。如果这个结构已经被提
交给一个工作队列, 且只需要修改该结构,则使用 PREPARE_* 而不是 INIT_* */
/*有 2 个函数来提交工作给一个工作队列:*/
int queue_work(struct workqueue_struct *queue, struct work_struct *work);
int queue_delayed_work(struct workqueue_struct *queue, struct delayed_work *work, unsigned long delay);
/*每个都添加work到给定的workqueue。如果使用 queue_delay_work, 则实际的工作至少要经过指定的 
jiffies 才会被执行。 这些函数若返回 1 则工作被成功加入到队列; 若为0，则意味着这个 work 已经在
队列中等待，不能再次加入*/
```
在将来的某个时间, 这个工作函数将被传入给定的 data 值来调用。这个函数将在工作线程的上下文运行, 因此它可以睡眠 (你应当知道这个睡眠可能影响提交给同一个工作队列的其他任务) 工作函数不能访问用户空间，因为它在一个内核线程中运行, 完全没有对应的用户空间来访问。
取消一个挂起的工作队列入口项可以调用: 
int cancel_delayed_work(struct delayed_work *work); 
void cancel_work_sync(struct work_struct *work)
如果这个入口在它开始执行前被取消，则返回非零。内核保证给定入口的执行不会在调用 cancel_delay_work 后被初始化. 如果 cancel_delay_work 返回 0, 但是, 这个入口可能已经运行在一个不同的处理器, 并且可能仍然在调用 cancel_delayed_work 后在运行. 要绝对确保工作函数没有在 cancel_delayed_work 返回 0 后在任何地方运行, 你必须跟随这个调用来调用: 
void flush_workqueue(struct workqueue_struct *queue); 
在 flush_workqueue 返回后, 没有在这个调用前提交的函数在系统中任何地方运行。
而cancel_work_sync会取消相应的work，但是如果这个work已经在运行那么cancel_work_sync会阻塞，直到work完成并取消相应的work。
当用完一个工作队列，可以去掉它，使用: 
void destroy_workqueue(struct workqueue_struct *queue); 
**三、与tasklet****的联系**
工作队列类似 tasklets，允许内核代码请求在将来某个时间调用一个函数，不同在于:
（1）tasklet 在软件中断上下文中运行，所以 tasklet 代码必须是原子的。而工作队列函数在一个特殊内核进程上下文运行，有更多的灵活性，且能够休眠。
（2）tasklet 只能在最初被提交的处理器上运行，这只是工作队列默认工作方式。
（3）内核代码可以请求工作队列函数被延后一个给定的时间间隔。
（4）tasklet 执行的很快, 短时期, 并且在原子态, 而工作队列函数可能是长周期且不需要是原子的，两个机制有它适合的情形。
