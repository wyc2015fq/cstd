
# 中断下半部机制-softirq-Tasklet-工作队列 - 嵌入式Linux - CSDN博客

2019年03月05日 09:55:35[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：36



## 中断上半部和下半部
在中断上半部，就是中断处理函数里面，是关中断的，所以为了快速执行，不能放太多的处理代码，只能执行非常短，这就衍生出了中断下半部。下半部主要是为了方便执行大量和本次中断相关的代码。
## 中断下半部的三种机制
|下半部|上下文|顺序执行保障|
|---|---|---|
|softirq|中断|随意，同类型都可以在不同处理器同时执行，代码执行效率非常高|
|tasklet|中断|用softirq机制实现，同类型不能同时执行|
|workqueue|进程|不保障，可能被调度和抢占|
### softirq
**具体示例查看内核代码**
kernel/kernel/time/timer.c
**注册softirq**
`void open_softirq(int nr, void (*action)(struct softirq_action *)) 
{ 
    softirq_vec[nr].action = action; 
}`**触发softirq**
`void raise_softirq(unsigned int nr) 
{ 
    unsigned long flags;
    local_irq_save(flags); 
    raise_softirq_irqoff(nr); 
    local_irq_restore(flags); 
}`**disable/enable softirq**
`//disable
static inline void local_bh_disable(void) 
{ 
    __local_bh_disable_ip(_THIS_IP_, SOFTIRQ_DISABLE_OFFSET); 
}
static __always_inline void __local_bh_disable_ip(unsigned long ip, unsigned int cnt) 
{ 
    preempt_count_add(cnt); 
    barrier(); 
}
//enable
static inline void local_bh_enable(void) 
{ 
    __local_bh_enable_ip(_THIS_IP_, SOFTIRQ_DISABLE_OFFSET); 
}
void __local_bh_enable_ip(unsigned long ip, unsigned int cnt) 
{ 
    WARN_ON_ONCE(in_irq() || irqs_disabled());－－－－－－－－－－－（1） 
    preempt_count_sub(cnt - 1); －－－－－－－－－－－－－－－－－－（2）
    if (unlikely(!in_interrupt() && local_softirq_pending())) { －－－－－－－（3） 
        do_softirq(); 
    }
    preempt_count_dec(); －－－－－－－－－－－－－－－－－－－－－（4） 
    preempt_check_resched(); 
}`
### Tasklet
**定义一个Tasklet**
`#define DECLARE_TASKLET(name, func, data) \ 
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(0), func, data }
#define DECLARE_TASKLET_DISABLED(name, func, data) \ 
struct tasklet_struct name = { NULL, 0, ATOMIC_INIT(1), func, data }`**调度一个Tasklet**
`static inline void tasklet_schedule(struct tasklet_struct *t) 
{ 
    if (!test_and_set_bit(TASKLET_STATE_SCHED, &t->state)) 
        __tasklet_schedule(t); 
}`
### 工作队列workqueue
**声明一个workqueue**
`int irq2;           
    struct work_struct irq2_work;                                                                                                  
    struct workqueue_struct *irq2_work_queue;`**初始化一个workqueue**
`INIT_WORK(&acc->irq2_work, lis3dh_acc_irq2_work_func);                                                                         
    acc->irq2_work_queue = create_singlethread_workqueue("lis3dh_acc_wq2");
    if (!acc->irq2_work_queue) {
        err = -ENOMEM;
        dev_err(&client->dev, "cannot create work queue2: %d\n", err);
        goto err_destoyworkqueue1;
    }`**调度执行一个workqueue**
`queue_work(acc->irq2_work_queue, &acc->irq2_work);                                                                             
printk(KERN_INFO "%s: isr2 queued\n", LIS3DH_ACC_DEV_NAME);`**销毁一个workqueue**
`destroy_workqueue(acc->irq2_work_queue);`只是简单记录~面试的时候会问他们的差别，还有实现下半部的方式，主要性能方面softirq可以多cpu执行，性能会最快，workqueue被调度执行，性能会比较低下。后续有时间再继续更新。

