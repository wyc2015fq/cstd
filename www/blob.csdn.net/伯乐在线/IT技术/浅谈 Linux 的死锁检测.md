# 浅谈 Linux 的死锁检测 - 文章 - 伯乐在线
原文出处： [alex-huang](http://blog.chinaunix.net/uid-25942458-id-3823545.html?nsukey=r07dXgYlQEe4o1klwtl2ItalMphRPjEY7CIF%2B528cVnxlqVRdoE5%2FqsOLSYfPiGzJID8xKJlVzpCefwsRghcJHtOF849LA9TCGkbK1uXrNczBOCn2bVUDMxlPyzPjbqLskk1kf3nDdXKO7XEyGTwOvqIWY2gZ0HezHNUkuR8Mu9LP7ORrOfKCOBO4yAzo%2F%2F0)
死锁：就是多个进程（≥2）因为争夺资源而相互等待的一种现象，若无外力推动，将无法继续运行下去。注意，只有在多进程或者多线程之间或者他们与中断之间相互通讯或者共享资源才有可能发生死锁，单线程或者进程之间没有联系的话，一般不会发生死锁。锁的种类比较多，这里主要说自旋锁和信号量。两者的差别就在于前者获得不到资源时的动作是不断的资源（即忙转浪费cpu的cycles）而后者则表现为睡眠等待。
死锁的基本情况如下：
### （一）、自旋锁
1 递归使用：同一个进程或线程中，申请自旋锁，但没有释放之前又再次申请，一定产生死锁。
2 进程得到自旋锁后阻塞，睡眠：在获得自旋锁之后调用copy_from_user()、copy_to_ser()、和kmalloc（）等有可能引起阻塞的函数。
3 中断中没有关中断，或着因为申请未释放的自旋锁：在中断中使用自旋锁是可以的，应该在进入中断的时候关闭中断，不然中断再次进入的时候，中断处理函数会自旋等待自旋锁可以再次使用。或者在进程中申请了自旋锁，释放前进入中断处理函数，中断处理函数又申请同样的自旋锁，这将导致死锁。
4 中断与中断下半部共享资源和中断与进程共享资源死锁出现的情况类似。
5 中断下半部与进程共享资源和中断与进程共享资源死锁出现的情况类似。
自旋锁三种状态：
自旋锁保持期间是抢占失效的（内核不允许被抢占）。
1 单CPU且内核不可抢占：
自旋锁的所有操作都是空。不会引起死锁，内核进程间不存在并发操作进程，进程与中断仍然可能共享数据，存在并发操作，此时内核自旋锁已经失去效果。
2 单CPU且内核可抢占：
当获得自旋锁的时候，禁止内核抢占直到释放锁为止。此时可能存在死锁的情况是参考自旋锁可能死锁的一般情况。
禁止内核抢占并不代表不会进行内核调度，如果在获得自旋锁后阻塞或者主动调度，内核会调度其他进程运行，被调度的内核进程返回用户空间时，会进行用户抢占，此时调用的进程再次申请上次未释放的自旋锁时，会一直自旋。但是内核被禁止抢占，从而造成死锁。
内核被禁止抢占，但此时中断并没被禁止，内核进程可能因为中断申请自旋锁而死锁。
3 多CPU且内核可抢占：
这才是是真正的SMP的情况。当获得自旋锁的时候，禁止内核抢占直到释放锁为止。
### （二）信号量
1 递归使用： 同理，在同一个进程或线程中，申请了信号量，但没有释放之前又再次申请，进程会一直睡眠，这种情况一定死锁。
2 进程得到信号量后阻塞，睡眠：由于获取到信号量的进程阻塞或者随眠，其他在获取不到后信号量也会进入睡眠等待，这种情况可能造成死锁。
3 中断中申请信号量：由于信号量在获取不到自旋锁后会进入睡眠等待，中断处理函数不允许睡眠，如果睡眠，中断将无法返回。
4 中断下半部申请信号量：中断下半部允许睡眠，这种情况不会造成死锁。
5 俩个进程相互等待资源：进程1获得信号量A，需要信号量B，在进程1需要信号量B之前进程2获得信号量B，需要信号量A。进程1、2因相互等待资源而死锁。
上面是死锁的基本情况和类型。linux本身就提供了检测死锁的机制，如下：
### （一）D状态死锁检测
所谓D状态死锁：进程长时间（系统默认配置120秒）处于TASK_UNINTERRUPTIBLE 睡眠状态，这种状态下进程不响应异步信号。如：进程与外设硬件的交互（如read），通常使用这种状态来保证进程与设备的交互过程不被打断，否则设备可能处于不可控的状态。
对于这种死锁的检测linux提供的是hungtask机制，主要内容集中在Hung_task.c文件中。具体实现原理如下：
1）、系统创建normal级别的khungtaskd内核线程，内核线程每120秒检查一次，检查的内容：遍历所有的线程链表，发现D状态的任务，就判断自最近一次切换以来是否还有切换发生，若是有，则返回。若没有发生切换，则把任务的所有调用栈等信息打印出来。
2）、具体实现过程如下：
首先，hung_task_init创建一个名为khungtaskd的内核线程，内核线程的工作由watchdog来完成。

C++
```
static int __init hung_task_init(void)
{
    atomic_notifier_chain_register(&panic_notifier_list, &panic_block);
    watchdog_task = kthread_run(watchdog, NULL, "khungtaskd");
    return 0;
}
```
其次，我们看watchdog的实现：

C++
```
static int watchdog(void *dummy)
{
        //将内核线程设置为normal级别
    set_user_nice(current, 0);
    for ( ; ; ) {
//设置hungtask的校验时间间隔，用户可以修改这个时间，默认为120秒
        unsigned long timeout = sysctl_hung_task_timeout_secs;
        while (schedule_timeout_interruptible(timeout_jiffies(timeout)))
            timeout = sysctl_hung_task_timeout_secs;
//核心的检查代码在下面的函数中实现。        check_hung_uninterruptible_tasks(timeout);
    }
    return 0;
}
```
最后，我们分析一下hungtask的核心实现check_hung_uninterruptible_tasks，如下：

C++
```
static void check_hung_uninterruptible_tasks(unsigned long timeout)
{
    int max_count = sysctl_hung_task_check_count;
    int batch_count = HUNG_TASK_BATCHING;
    struct task_struct *g, *t;
    /*
     * If the system crashed already then all bets are off,
     * do not report extra hung tasks:
     */
//判断系统是否已经die、oops或者panic了，若是系统已经crash了，就无需再做hungtask了。
    if (test_taint(TAINT_DIE) || did_panic)
        return;
    rcu_read_lock();
//检查进程的列表，寻找D状态的任务
    do_each_thread(g, t) {
//判断用户是否设置了检查进程的数量，若是已经达到用户设置的限制，就跳出循环。
        if (!max_count--)
            goto unlock;
//判断是否到达批处理的个数，做这个批处理的目的就是因为整个检查是在关抢占的前提下进行的，可能进程列表的进程数很多，为了防止hungtask垄断cpu，所以，做了一个批处理的限制，到达批处理的数量后，就放一下权，给其他的进程运行的机会。
        if (!--batch_count) {
            batch_count = HUNG_TASK_BATCHING;
            rcu_lock_break(g, t);
            /* Exit if t or g was unhashed during refresh. */
            if (t->state == TASK_DEAD || g->state == TASK_DEAD)
                goto unlock;
        }
        /* use "==" to skip the TASK_KILLABLE tasks waiting on NFS */
//如果进程处于D状态，就开始把相关的信息显示出来了。
        if (t->state == TASK_UNINTERRUPTIBLE)
            check_hung_task(t, timeout);
    } while_each_thread(g, t);
 unlock:
    rcu_read_unlock();
}
```


C++
```
static void check_hung_task(struct task_struct *t, unsigned long timeout)
{
//统计进程的切换次数=主动切换次数+被动切换次数
    unsigned long switch_count = t->nvcsw + t->nivcsw;
    /*
     * Ensure the task is not frozen.
     * Also, when a freshly created task is scheduled once, changes
     * its state to TASK_UNINTERRUPTIBLE without having ever been
     * switched out once, it musn
```
怎么样，经过上面的分析，可以发现其实原理很简单。
### （二）R状态死锁检测
所谓R状态死锁：进程长时间（系统默认配置60秒）处于**TASK_RUNNING** 状态垄断cpu而不发生切换，一般情况下是进程关抢占后长时候干活，有时候可能进程关抢占后处于死循环或者睡眠后，这样就造成系统异常。
对于这种死锁的检测机制linux提供的机制是softlockup。主要集中在softlockup.c文件中。
1）、系统创建一个fifo的进程，此进程周期性的清一下时间戳（per cpu），而系统的时钟中断中会被softlockup挂入一个钩子（softlockup_tick）,这个钩子就是每个时钟中断到来的时候都检查是否每cpu的时间戳被touch了，若在阀值60秒内都没有被touch，系统就打印调试信息。
2）、让我们分析一下具体的实现：
首先，系统初始化的时候为每个cpu创建一个watchdog线程，这个线程是fifo的。具体实现如下：

C++
```
static int __init spawn_softlockup_task(void)
{
    void *cpu = (void *)(long)smp_processor_id();
    int err;
//可以通过启动参数禁止softlockup
    if (nosoftlockup)
        return 0;
//下面两个回调函数就是为每个cpu创建一个watchdog线程
    err = cpu_callback(&cpu_nfb, CPU_UP_PREPARE, cpu);
    if (err == NOTIFY_BAD) {
        BUG();
        return 1;
    }
    cpu_callback(&cpu_nfb, CPU_ONLINE, cpu);
    register_cpu_notifier(&cpu_nfb);
    atomic_notifier_chain_register(&panic_notifier_list, &panic_block);
    return 0;
}
```


C++
```
static int __cpuinit
cpu_callback(struct notifier_block *nfb, unsigned long action, void *hcpu)
{
    int hotcpu = (unsigned long)hcpu;
    struct task_struct *p;
    switch (action) {
    case CPU_UP_PREPARE:
    case CPU_UP_PREPARE_FROZEN:
        BUG_ON(per_cpu(softlockup_watchdog, hotcpu));
//创建watchdog内核线程
        p = kthread_create(watchdog, hcpu, "watchdog/%d", hotcpu);
        if (IS_ERR(p)) {
            printk(KERN_ERR "watchdog for %i failed\n", hotcpu);
            return NOTIFY_BAD;
        }
//将时间戳清零
        per_cpu(softlockup_touch_ts, hotcpu) = 0;
//设置watchdog
        per_cpu(softlockup_watchdog, hotcpu) = p;
//绑定cpu
        kthread_bind(p, hotcpu);
        break;
    case CPU_ONLINE:
    case CPU_ONLINE_FROZEN:
//唤醒watchdog这个内核线程        wake_up_process(per_cpu(softlockup_watchdog, hotcpu));
        break;
......
    
    return NOTIFY_OK;
}
```
其次，我们看一下内核线程watchdog的实现，如下：

C++
```
static int watchdog(void *__bind_cpu)
{
//将watchdog设置为fifo
    struct sched_param param = { .sched_priority = MAX_RT_PRIO-1 };
    sched_setscheduler(current, SCHED_FIFO, ?m);
//清狗，即touch时间戳
    /* initialize timestamp */
    __touch_softlockup_watchdog();
//当前进程可以被信号打断
    set_current_state(TASK_INTERRUPTIBLE);
    /*
     * Run briefly once per second to reset the softlockup timestamp.
     * If this gets delayed for more than 60 seconds then the
     * debug-printout triggers in softlockup_tick().
     */
    while (!kthread_should_stop()) {
//核心实现就是touch时间戳，让出cpu
        __touch_softlockup_watchdog();
        schedule();
        if (kthread_should_stop())
            break;
        set_current_state(TASK_INTERRUPTIBLE);
    }
    __set_current_state(TASK_RUNNING);
    return 0;
}
```
最后，softlockup在时钟中断中挂上一个钩子softlockup_tick，每个时钟中断都来检查watchdog这个fifo进程是否有touch过时间戳，若是60秒都没有touch过，就向系统上报异常信息了，如下：

C++
```
void softlockup_tick(void)
{
//取得当前的cpu
    int this_cpu = smp_processor_id();
//获得当前cpu的时间戳
    unsigned long touch_ts = per_cpu(softlockup_touch_ts, this_cpu);
    unsigned long print_ts;
//获得进程当前的寄存器组
    struct pt_regs *regs = get_irq_regs();
    unsigned long now;
//如果没有打开softlockup，就将这个cpu对应的时间戳清零
    /* Is detection switched off? */
    if (!per_cpu(softlockup_watchdog, this_cpu) || softlockup_thresh <= 0) {
        /* Be sure we don
```
怎么样，说白了，也不难，就是两条线并行工作：一条线是fifo级别的内核线程负责清时间戳，另一条线是时钟中断定期检查时间戳是否有被清过，若是到了阀值都没有被请过，则打印softlockup的信息。
### （三）长时间关中断检测
长时间关中断检测可以有几种实现机制，而利用nmi watchdog来检查这种长时间关中断情况，是比较简单的。其原理是需要软硬件配合，硬件通常提供一个计数器（可以递增也可以递减），当记数到某个值得时候，系统就硬件复位。而nmi watchdog就定期（小于这个计数到达系统复位的时间）的去清一下系统的计数，若是某个进程长时间关中断，则可能导致nmi watchdog得不到清，最终系统复位。
