# 深入理解 RCU 实现 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [lvyilong316](http://www.jobbole.com/members/lvyilong316) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
# **深入理解RCU实现**
**——基于****内核2.6.21**** RCU实现(lvyilong316)**
RCU(Read-Copy Update)，顾名思义就是读-拷贝修改，它是基于其原理命名的。对于被RCU保护的共享数据结构，**读者不需要获得任何锁就可以访问它，但写者在访问它时首先拷贝一个副本，然后对副本进行修改，最后使用一个回调（callback）机制在****适当的时机****把指向原来数据的指针重新指向新的被修改的数据**。那么这个“适当的时机”是怎么确定的呢？这是由内核确定的，也是我们后面讨论的重点。
## **RCU原理**
RCU实际上是一种改进的rwlock，读者几乎没有什么同步开销，它不需要锁，不使用原子指令，而且在除alpha的所有架构上也不需要内存栅（Memory Barrier），因此不会导致锁竞争，内存延迟以及流水线停滞。不需要锁也使得使用更容易，因为死锁问题就不需要考虑了。写者的同步开销比较大，它需要延迟数据结构的释放，复制被修改的数据结构，它也必须使用某种锁机制同步并行的其它写者的修改操作。
读者必须提供一个信号给写者以便写者能够确定数据可以被安全地释放或修改的时机。有一个专门的垃圾收集器来探测读者的信号，一旦所有的读者都已经发送信号告知它们都不在使用被RCU保护的数据结构，垃圾收集器就调用回调函数完成最后的数据释放或修改操作。
RCU与rwlock的不同之处是：它既允许多个读者同时访问被保护的数据，又允许多个读者和多个写者同时访问被保护的数据（注意：是否可以有多个写者并行访问取决于写者之间使用的同步机制），读者没有任何同步开销，而写者的同步开销则取决于使用的写者间同步机制。但RCU不能替代rwlock，因为如果写比较多时，对读者的性能提高不能弥补写者导致的损失。
读者在访问被RCU保护的共享数据期间不能被阻塞，这是RCU机制得以实现的一个基本前提，也就说当读者在引用被RCU保护的共享数据期间，读者所在的CPU不能发生上下文切换，spinlock和rwlock都需要这样的前提。写者在访问被RCU保护的共享数据时不需要和读者竞争任何锁，只有在有多于一个写者的情况下需要获得某种锁以与其他写者同步。
写者修改数据前首先拷贝一个被修改元素的副本，然后在副本上进行修改，修改完毕后它向垃圾回收器注册一个回调函数以便在适当的时机执行真正的修改操作。等待适当时机的这一时期称为grace period，而**CPU发生了上下文切换称为经历一个quiescent state，grace period就是所有CPU都经历一次quiescent state所需要的等待的时间**。垃圾收集器就是在grace period之后调用写者注册的回调函数来完成真正的数据修改或数据释放操作的。
要想使用好RCU，就要知道RCU的实现原理。我们拿linux 2.6.21 kernel的实现开始分析，为什么选择这个版本的实现呢？因为这个版本的实现相对较为单纯，也比较简单。当然之后内核做了不少改进，如抢占RCU、可睡眠RCU、分层RCU。但是基本思想都是类似的。所以先从简单入手。
首先，上一节我们提到，**写者在访问它时首先拷贝一个副本，然后对副本进行修改，最后使用一个回调（callback）机制在****适当的时机****把指向原来数据的指针重新指向新的被修改的数据****。**而这个“适当的时机”就是所有CPU经历了一次进程切换（也就是一个grace period）。为什么这么设计？因为RCU读者的实现就是关抢占执行读取，读完了当然就可以进程切换了，也就等于是写者可以操作临界区了。那么就自然可以想到，内核会设计两个元素，来分别表示写者被挂起的起始点，以及每cpu变量，来表示该cpu是否经过了一次进程切换(quies state)。就是说，当写者被挂起后，
1）重置每cpu变量，值为0。
2）当某个cpu经历一次进程切换后，就将自己的变量设为1。
3）当所有的cpu变量都为1后，就可以唤醒写者了。
下面我们来分别看linux里是如何完成这三步的。
## **从一个例子开始**
我们从一个例子入手，这个例子来源于linux kernel文档中的whatisRCU.txt。这个例子使用RCU的核心API来保护一个指向动态分配内存的全局指针。

C++
```
struct foo {
int a;
    char b;
    long c;
};
DEFINE_SPINLOCK(foo_mutex);
struct foo *gbl_foo;
void foo_update_a(int new_a)
{
  struct foo *new_fp;
struct foo *old_fp;
new_fp = kmalloc(sizeof(*new_fp), GFP_KERNEL);
spin_lock(&foo_mutex);
old_fp = gbl_foo;
*new_fp = *old_fp;
new_fp->a = new_a;
rcu_assign_pointer(gbl_foo, new_fp);
spin_unlock(&foo_mutex);
synchronize_rcu();
kfree(old_fp);
}
int foo_get_a(void)
{
  int retval;
rcu_read_lock();
retval = rcu_dereference(gbl_foo)->a;
rcu_read_unlock();
return retval;
 }
```
如上代码所示,RCU被用来保护全局指针struct foo *gbl_foo。foo_get_a()用来从RCU保护的结构中取得gbl_foo的值。而foo_update_a()用来更新被RCU保护的gbl_foo的值（更新其a成员）。
首先,我们思考一下,为什么要在foo_update_a()中使用自旋锁foo_mutex呢? 假设中间没有使用自旋锁.那foo_update_a()的代码如下:

C++
```
void foo_update_a(int new_a) 
{ 
struct foo *new_fp; 
struct foo *old_fp; 
new_fp = kmalloc(sizeof(*new_fp), GFP_KERNEL); 
old_fp = gbl_foo; 
1:------------------------- 
*new_fp = *old_fp; 
new_fp->a = new_a; 
rcu_assign_pointer(gbl_foo, new_fp); 
synchronize_rcu(); 
kfree(old_fp); 
}
```
假设A进程在上图—-标识处被B进程抢点.B进程也执行了goo_ipdate_a().等B执行完后，再切换回A进程.此时,A进程所持的old_fd实际上已经被B进程给释放掉了.此后A进程对old_fd的操作都是非法的。所以在此我们得到一个重要结论：**RCU****允许多个读者同时访问被保护的数据，****也****允许多个读者****在有****写者时访问被保护的数据（****但是****注意：是否可以有多个写者并行访问取决于写者之间使用的同步机制）**。
说明：本文中说的进程不是用户态的进程，而是内核的调用路径，也可能是内核线程或软中断等。
## **RCU的核心API**
另外,我们在上面也看到了几个有关RCU的核心API。它们为别是:

C++
```
rcu_read_lock() 
rcu_read_unlock() 
synchronize_rcu() 
rcu_assign_pointer() 
rcu_dereference()
```
其中,rcu_read_lock()和rcu_read_unlock()用来保持一个读者的RCU临界区.在该临界区内不允许发生上下文切换。为什么不能发生切换呢？因为内核要根据“是否发生过切换”来判断读者是否已结束读操作，我们后面再分析。
rcu_dereference()：读者调用它来获得一个被RCU保护的指针。
rcu_assign_pointer()：写者使用该函数来为被RCU保护的指针分配一个新的值。
synchronize_rcu()：这是RCU的核心所在，它挂起写者，等待读者都退出后释放老的数据。
## **RCU API实现分析 **
l **r****cu_read_lock()和rcu_read_unlock()**
rcu_read_lock()和rcu_read_unlock()的实现如下:

C++
```
#define rcu_read_lock() __rcu_read_lock() 
#define rcu_read_unlock() __rcu_read_unlock() 
#define __rcu_read_lock() 
do { 
    preempt_disable(); 
    __acquire(RCU); 
    rcu_read_acquire(); 
} while (0) 
#define __rcu_read_unlock() 
do { 
    rcu_read_release(); 
    __release(RCU); 
    preempt_enable(); 
} while (0)
```
其中__acquire(),rcu_read_acquire(),rcu_read_release(),__release()都是一些选择编译函数,可以忽略不可看。因此可以得知：**rcu_read_lock(),rcu_read_unlock()只是禁止和启用抢占.因为在读者临界区****，****不允许发生上下文切换**。
l **rcu_dereference()和rcu_assign_pointer()**
rcu_dereference()和rcu_assign_pointer()的实现如下:

C++
```
rcu_dereference()和rcu_assign_pointer()的实现如下: 
#define rcu_dereference(p) ({ 
    typeof(p) _________p1 = ACCESS_ONCE(p); 
    smp_read_barrier_depends(); 
    (_________p1); 
}) 
#define rcu_assign_pointer(p, v) 
({ 
    if (!__builtin_constant_p(v) || ((v) != NULL)) 
        smp_wmb(); 
    (p) = (v); 
})
```
它们的实现也很简单.因为它们本身都是原子操作。只是为了cache一致性，插上了内存屏障。可以让其它的读者/写者可以看到保护指针的最新值.
l **synchronize_rcu()**
synchronize_rcu()在RCU中是一个最核心的函数,它用来等待之前的读者全部退出.我们后面的大部份分析也是围绕着它而进行.实现如下:

C++
```
void synchronize_rcu(void) 
{ 
    struct rcu_synchronize rcu; 
    init_completion(&rcu.completion); 
/* Will wake me after RCU finished */ 
    call_rcu(&rcu.head, wakeme_after_rcu); 
    /* Wait for it */ 
    wait_for_completion(&rcu.completion); 
}
```
我们可以看到,它初始化了一个本地变量,它的类型为struct rcu_synchronize.调用call_rcu()之后.一直等待条件变量rcu.competion的满足。
在这里看到了RCU的另一个核心API,它就是call_run()。它的定义如下:

C++
```
void call_rcu(struct rcu_head *head,  void (*func)(struct rcu_head *rcu)) 
{ 
    unsigned long flags; 
    struct rcu_data *rdp; 
    head->func = func; 
    head->next = NULL; 
    local_irq_save(flags); 
    rdp = &__get_cpu_var(rcu_data); 
    *rdp->nxttail = head; 
    rdp->nxttail = &head->next; 
    if (unlikely(++rdp->qlen > qhimark)) { 
        rdp->blimit = INT_MAX; 
        force_quiescent_state(rdp, &rcu_ctrlblk); 
    } 
    local_irq_restore(flags); 
}
```
该函数也很简单,就是将参数传入的回调函数fun赋值给一个struct rcu_head变量，再将这个struct rcu_head加在了per_cpu变量rcu_data的nxttail 链表上。
rcu_data定义如下，是个每cpu变量:
DEFINE_PER_CPU(struct rcu_data, rcu_data) = { 0L };
接着我们看下call_rcu注册的函数，我们也可以看到,在synchronize_rcu()中,传入call_rcu的函数为wakeme_after_rcu()，其实现如下:

C++
```
static void wakeme_after_rcu(struct rcu_head *head) 
{ 
    struct rcu_synchronize *rcu; 
    rcu = container_of(head, struct rcu_synchronize, head); 
    complete(&rcu->completion); 
}
```
我们可以看到,该函数将条件变量置真,然后唤醒了在条件变量上等待的进程。
由此,我们可以得知,每一个CPU都有一个rcu_data.每个调用call_rcu()/synchronize_rcu()进程的进程都会将一个rcu_head都会挂到rcu_data的nxttail链表上（这个rcu_head其实就相当于这个进程在RCU机制中的体现），然后挂起。当读者都完成读操作后（经过一个grace period后）就会触发这个rcu_head上的回调函数来唤醒写者。整个过程如下图所示：
![28541347_14766201931o1h](http://jbcdn2.b0.upaiyun.com/2016/10/2479eb93edab2c92288bb3393498af74.png)
看到这里，也就到了问题的关键，内核是如何判断当前读者都已经完成读操作了呢（经过了一个grace period）？又是由谁来触发这个回调函数wakeme_after_rcu呢？下一小节再来分析。
## **从RCU的初始化说起 **
那究竟怎么去判断当前的读者已经操作完了呢？我们在之前看到,不是读者在调用rcu_read_lock()的时候要禁止抢占么？因此,我们只需要判断所有的CPU都进过了一次上下文切换,就说明所有读者已经退出了。要彻底弄清楚这个问题,我们得从RCU的初始化说起。
RCU的初始化开始于start_kernel()–>rcu_init()。而其主要是对每个cpu调用了rcu_online_cpu函数。
l **rcu_online_cpu**

C++
```
static void __cpuinit rcu_online_cpu(int cpu) 
{ 
    struct rcu_data *rdp = &per_cpu(rcu_data, cpu); 
    struct rcu_data *bh_rdp = &per_cpu(rcu_bh_data, cpu); 
    rcu_init_percpu_data(cpu, &rcu_ctrlblk, rdp); 
    rcu_init_percpu_data(cpu, &rcu_bh_ctrlblk, bh_rdp); 
    open_softirq(RCU_SOFTIRQ, rcu_process_callbacks, NULL); 
}
```
这个函数主要完成两个操作：初始化两个per cpu变量；注册RCU_SOFTIRQ软中断处理函数rcu_process_callbacks。我们从这里又看到了另一个per cpu变量：rcu_bh_data.有关bh的部份之后再来分析.在这里略过这些部分。 下面看下rcu_init_percpu_data()的实现。
l **r****cu_init_percpu_data**

C++
```
static void rcu_init_percpu_data(int cpu, struct rcu_ctrlblk *rcp, struct rcu_data *rdp) 
{ 
    memset(rdp, 0, sizeof(*rdp)); 
    rdp->curtail = &rdp->curlist; 
    rdp->nxttail = &rdp->nxtlist; 
    rdp->donetail = &rdp->donelist; 
    rdp->quiescbatch = rcp->completed; 
    rdp->qs_pending = 0; 
    rdp->cpu = cpu; 
    rdp->blimit = blimit; 
}
```
调用这个函数的第二个参数是一个全局变量rcu_ctlblk。 在继续向下分析之前我们先看下这些函数用到的一些重要数据结构。
## **重要数据结构**
说明：下列数据结构只列出了主要成员。

C++
```
struct rcu_ctrlblk {  
    long cur;  
    long completed;  
    cpumask_t  cpumask;   
};
```
struct rcu_ctrlblk的主要作用就是定义上节提到的**全局变量****rcu_ctlblk****，这个变量在系统中是唯一的**。另外说明一下，为了记录方便，内核将从启动开始的每个grace period对应一个数字表示。
这里的cpumask是为了标识当前系统中的所有cpu，以便标记哪些cpu发生过上下文切换(经历过一个quiescent state)。而cur，completed，则用来同步。我们可以这样理解，**cur和completed是系统****级别的记录信息**，也即系统实时经历的grace编号，一般情况下，新开一个graceperiod等待周期的话，cur会加1，当graceperiod结束后，会将completed置为cur，所以通常情况下，都是completed追着cur跑。
那么我们可能会猜测，是不是如果complete= curr -1 的时候，就表示系统中graceperiod还没有结束？当completed= curr的时候，就表示系统中不存在graceperiod等待周期了？我们姑且这么理解，实际上有些许差别，但设计思想都是一样的。

C++
```
struct rcu_data {  
    long quiescbatch;       
    int passed_quiesc;  
    long            batch;           
    struct rcu_head *nxtlist;  
    struct rcu_head **nxttail;  
    struct rcu_head *curlist;  
    struct rcu_head **curtail;  
    struct rcu_head *donelist;  
    struct rcu_head **donetail;  
};
```
上面的结构，要达到的作用是，**跟踪单个CPU上的****RCU****事务**。
(1) passed_quiesc：这是一个flag标志，表示当前cpu是否已经发生过抢占了(经历过一个quiescent state)，0表示为未发送过切换，1表示发生过切换；
**说明：我们一直强调发生过一次cpu抢占就是经历过****一个quiescent state****，其实这是不严格的说法。为什么呢？因为自系统启动，各种进程频繁调度，肯定每个****cpu都会发生过抢占。所以我们这里说的“发生过抢占”是指从某个特殊的时间点开始发生过抢占。那么这个特殊的时间点是什么时候呢？就是我们调用****synchronize_rcu****将rcu_head挂在每cpu变量上并挂起进程时，我们后面分析就会证实这一点。**
(2) batch：表示一个grace periodid，表示本次被阻塞的写者，需要在哪个graceperiod之后被激活；
(3) quiescbatch：表示一个grace periodid，用来比较当前cpu是否处于等待进程切换完成。
下面我们介绍剩下的三组指针，但首先**注意这三组链表上的元素都是****struct rcu_head ****类型**。这三个链表的作用还要结合整个RCU写者从阻塞到唤醒的过程来看。
rcu写者的整体流程，假设系统中出现rcu写者阻塞，那么流程如下：
(1) 调用synchronize_rcu，将代表写者的rcu_head添加到CPU[n]每cpu变量rcu_data的nxtlist，这个链表代表有需要提交给rcu处理的回调(但还没有提交)；
(2) CPU[n]每次时钟中断时检测自己的nxtlist是否为null，若不为空，因此则唤醒rcu软中断；
(3) RCU的软中断处理函数rcu_process_callbacks会挨个检查本CPU的三个链表。
下面分析RCU的核心处理函数，也就是软中断处理函数rcu_process_callbacks。
## **RCU软中断**
在“RCU API实现分析”一节我们知道调用synchronize_rcu，将代表写者的rcu_head添加到了CPU[n]每cpu变量rcu_data的nxtlist。
而另一方面，在每次时钟中断中，都会调用update_process_times函数。
l **update_process_times **

C++
```
void update_process_times(int user_tick) 
{ 
    //......
    if (rcu_pending(cpu)) 
        rcu_check_callbacks(cpu, user_tick); 
//......
}
```
在每一次时钟中断,都会检查rcu_pending(cpu)是否为真,如果是,就会为其调用rcu_check_callbacks()。
l **r****cu_pending**
rcu_pending()的代码如下:

C++
```
int rcu_pending(int cpu) 
{ 
    return __rcu_pending(&rcu_ctrlblk, &per_cpu(rcu_data, cpu)) || 
    __rcu_pending(&rcu_bh_ctrlblk, &per_cpu(rcu_bh_data, cpu)); 
}
```
同上面一样,忽略bh的部份，我们看 __rcu_pending。
l ** __rcu_pending**

C++
```
static int __rcu_pending(struct rcu_ctrlblk *rcp, struct rcu_data *rdp) 
{ 
    /* This cpu has pending rcu entries and the grace period 
     * for them has completed. 
     */ 
    if (rdp->curlist && !rcu_batch_before(rcp->completed, rdp->batch)) 
        return 1; 
    /* This cpu has no pending entries, but there are new entries */ 
    if (!rdp->curlist && rdp->nxtlist) 
        return 1; 
    /* This cpu has finished callbacks to invoke */ 
    if (rdp->donelist) 
        return 1; 
    /* The rcu core waits for a quiescent state from the cpu */ 
    if (rdp->quiescbatch != rcp->cur || rdp->qs_pending) 
        return 1; 
    /* nothing to do */ 
    return 0; 
}
```
可以上面有四种情况会返回1，分别对应:
A. 该CPU上有等待处理的回调函数,且已经经过了一个batch(grace period).rdp->datch表示rdp在等待的batch序号;
B. 上一个等待已经处理完了,又有了新注册的回调函数;
C. 等待已经完成,但尚末调用该次等待的回调函数;
D. 在等待quiescent state.
如果rcu_pending返回1,就会进入到rcu_check_callbacks()，rcu_check_callbacks()中主要工作就是调用raise_softirq(RCU_SOFTIRQ)，触发RCU软中断。而RCU软中断的处理函数为rcu_process_callbacks，其中分别针对每cpu变量rcu_bh_data和rcu_data调用__rcu_process_callbacks。我们主要分析针对rcu_data的调用。
l **__****rcu_process_callbacks**
1) 先看nxtlist里有没有待处理的回调(rcu_head)，如果有的话，说明有写者待处理，那么还要分两种情况：
1.1)如果系统是第一次出现写者阻塞，也即之前的写者都已经处理完毕，那么此时curlist链表一定为空（curlist专门存放已被rcu检测到的写者请求），于是就把nxtlist里的所有成员都移动到curlist指向，并把当前CPU需要等待的grace period id：rdp->batch设置为当前系统处理的grace period的下一个grace周期，即rcp->cur+ 1。由于这算是一个新的grace period，即start_rcu_batch，所以还接着需要增加系统的grace period计数，即rcp->cur++，同时，将全局的cpusmask设置为全f，代表新的grace period开始，需要检测所有的cpu是否都经过了一次进程切换。代码如下：

C++
```
void __rcu_process_callbacks(struct rcu_ctrlblk *rcp,  
                    struct rcu_data *rdp)  
{  
    if (rdp->nxtlist && !rdp->curlist) {  
        move_local_cpu_nxtlist_to_curlist();  
  
        rdp->batch = rcp->cur + 1;  
  
        if (!rcp->next_pending) {  
            rcp->next_pending = 1;  
            rcp->cur++;  
            cpus_andnot(rcp->cpumask, cpu_online_map, nohz_cpu_mask);  
        }  
    }  
}
```
接着跳转至1.2。
1.2) 如果系统之前已经有写者在被rcu监控着，但还没来得及经过一个grace period，这个时候curlist不为空，nxtlist也不为空，写者会被加入nxtlist中。由于curlist不为空，说明上个rcu周期的写者还没有处理完，于是不会将本次阻塞的写者加入curlist，一直到上次的curlist里的rcu_head被处理完（都移动到了donelist），才会将后来的写者纳入RCU考虑（移动到curlist）**（假如这个期间又来了多个写者，则多个写者的****rcu_head共享下一个****grace****period****，也就是下一个****grace****period****结束后这多个写者都会被唤醒）**。进入下一步。
2) rcu_process_callbacks调用每CPU函数rcu_check_quiescent_state开始监控，检测所有的CPU是否会经历一个进程切换。 这个函数是如何得知需要开始监控的？ 答案在于quiescbatch与全局的rcp->cur比较。 初始化时rdp->quiescbatch =rcp->completed = rcp->cur。 由于1.1有新grace period开启，所以rcp->cur已经加1了，于是rdp->quiescbatch和rcp->curr不等，进而将此cpu的rdp->passed_quiesc设为0，表示这个周期开始，我要等待这个cpu经历一个进程切换，等待该CPU将passed_quiesc置为1。即与前面讲到的passed_quiesc标志置0的时机吻合。最后将rdp->quiescbatch置为 rcp->cur，以避免下次再进入软中断里将passed_quiesc重复置0。

C++
```
void rcu_check_quiescent_state(struct rcu_ctrlblk *rcp,  
                    struct rcu_data *rdp)  
{  
    if (rdp->quiescbatch != rcp->cur) {  
        /* start new grace period: */  
        rdp->qs_pending = 1;  
        rdp->passed_quiesc = 0;  
        rdp->quiescbatch = rcp->cur;  
        return;  
    }  
}
```
3) 本次软中断结束，下次软中断到来，再次进入rcu_check_quiescent_state进行检测，如果本CPU的rdp->passed_quiesc已经置1，则需要cpu_quiet将本CPU标志位从全局的rcp->cpumask中清除，如果cpumask为0了，则说明自上次RCU写者被挂起以来，所有CPU都已经历了一次进程切换，于是本次rcu等待周期结束，将rcp->completed置为rcp->cur，重置cpumask为全f，并尝试重新开启一个新的grace period。我们可以看到RCU用了如此多的同步标志，却少用spinlock锁，是多么巧妙的设计，不过这也提高了理解的难度。

C++
```
void rcu_check_quiescent_state(struct rcu_ctrlblk *rcp,  
                    struct rcu_data *rdp)  
{  
    if (rdp->quiescbatch != rcp->cur) {  
        /* start new grace period: */  
        rdp->qs_pending = 1;  
        rdp->passed_quiesc = 0;  
        rdp->quiescbatch = rcp->cur;  
        return;  
    }  
  
    if (!rdp->passed_quiesc)  
        return;  
    /*this cpu has passed a quies state*/  
    if (likely(rdp->quiescbatch == rcp->cur)) {  
        cpu_clear(cpu, rcp->cpumask);  
        if (cpus_empty(rcp->cpumask))   
            rcp->completed = rcp->cur;  
    }  
}
```
4)下次再进入rcu软中断__rcu_process_callbacks，发现rdp->batch已经比rcp->completed小了（因为上一步骤中，后者增大了），则将rdp->curlist上的回调移动到rdp->donelist里，接着还会再次进入rcu_check_quiescent_state，但是由于当前CPU的rdp->qs_pending已经为1了，所以不再往下清除cpu掩码。__rcu_process_callbacks
代码变成了：

C++
```
void __rcu_process_callbacks(struct rcu_ctrlblk *rcp,  
                    struct rcu_data *rdp)  
{  
    if (rdp->curlist && !rcu_batch_before(rcp->completed, rdp->batch)) {  
        *rdp->donetail = rdp->curlist;  
        rdp->donetail = rdp->curtail;  
        rdp->curlist = NULL;  
        rdp->curtail = &rdp->curlist;  
    }  
  
    if (rdp->nxtlist && !rdp->curlist) {  
        move_local_cpu_nxtlist_to_curlist();  
  
        rdp->batch = rcp->cur + 1;  
  
        if (!rcp->next_pending) {  
            rcp->next_pending = 1;  
            rcp->cur++;  
            cpus_andnot(rcp->cpumask, cpu_online_map, nohz_cpu_mask);  
        }  
    }  
    if (rdp->donelist)  
        rcu_do_batch(rdp);  
}
```
5) 经过千山万水终于来到rcu_do_batch（如果rdp->donelist有的话）在此函数里，执行RCU写者挂载的回调，即wakeme_after_rcu。
l **rcu_do_batch**

C++
```
static void rcu_do_batch(struct rcu_data *rdp) 
{ 
    struct rcu_head *next, *list; 
    int count = 0; 
    list = rdp->donelist; 
    while (list) { 
        next = list->next; 
        prefetch(next); 
        list->func(list); 
        list = next; 
        if (++count >= rdp->blimit) 
            break; 
    } 
    rdp->donelist = list; 
    local_irq_disable(); 
    rdp->qlen -= count; 
    local_irq_enable(); 
    if (rdp->blimit == INT_MAX && rdp->qlen)
        blimit = blimit; 
    if (!rdp->donelist) 
        rdp->donetail = &rdp->donelist; 
    else 
        raise_rcu_softirq(); 
}
```
它遍历处理挂在链表上的回调函数.在这里,注意每次调用的回调函数有最大值限制.这样做主要是防止一次调用过多的回调函数而产生不必要系统负载.如果donelist中还有没处理完的数据,打开RCU软中断,在下次软中断到来的时候接着处理.
**注意：**
**仅当系统检测到一个grace period的所有CPU都经历了进程切换后，才会给系统一个信息要求启动新batch，在此期间的所有写者请求，都暂存在本地CPU的nxtlist链表里。**
## **进程切换**
在每一次进程切换的时候,都会调用rcu_qsctr_inc().如下代码片段如示:

C++
```
asmlinkage void __sched schedule(void) 
{ 
//......
rcu_qsctr_inc(cpu); 
//...... 
}
```
rcu_qsctr_inc()代码如下:

C++
```
static inline void rcu_qsctr_inc(int cpu) 
{ 
    struct rcu_data *rdp = &per_cpu(rcu_data, cpu); 
    rdp->passed_quiesc = 1; 
}
```
该函数将对应CPU上的rcu_data的passed_quiesc成员设为了1。或许你已经发现了,这个过程就标识该CPU经过了一次quiescent state，和之前在软中断的初始化为0相呼应。
## **几种RCU情况分析 **
1. 如果CPU 1上有进程调用rcu_read_lock进入临界区,之后退出来,发生了进程切换,新进程又通过rcu_read_lock进入临界区.由于RCU软中断中只判断一次上下文切换,因此,在调用回调函数的时候,仍然有进程处于RCU的读临界区,这样会不会有问题呢?
这样是不会有问题的.还是上面的例子:

C++
```
spin_lock(&foo_mutex); 
    old_fp = gbl_foo; 
    *new_fp = *old_fp; 
    new_fp->a = new_a; 
    rcu_assign_pointer(gbl_foo, new_fp); 
    spin_unlock(&foo_mutex); 
    synchronize_rcu(); 
    kfree(old_fp);
```
使用synchronize_rcu ()只是为了等待持有old_fd(也就是调用rcu_assign_pointer ()更新之前的gbl_foo)的进程退出.而不需要等待所有的读者全部退出.这是因为,在rcu_assign_pointer ()之后的读取取得的保护指针,已经是更新好的新值了.
2. 如果一个CPU连续调用synchronize_rcu()或者call_rcu()它们会有什么影响呢?
如果当前有请求在等待,就会新请提交的回调函数挂到taillist上,一直到前一个等待完成,再将taillist的数据移到curlist,并开启一个新的等待,因此,也就是说,在前一个等待期间提交的请求,都会放到一起处理.也就是说,他们会共同等待所有CPU切换完成.
举例说明如下: 
## **带bh的RCU API**
在上面的代码分析的时候,经常看到带有bh的RCU代码.现在来看一下这些带bh的RCU是什么样的。

C++
```
#define rcu_read_lock_bh() __rcu_read_lock_bh() 
#define rcu_read_unlock_bh() __rcu_read_unlock_bh() 
#define __rcu_read_lock_bh() 
do { 
    local_bh_disable(); 
    __acquire(RCU_BH); 
    rcu_read_acquire(); 
}while (0) 
#define __rcu_read_unlock_bh() 
do { 
    rcu_read_release(); 
    __release(RCU_BH); 
    local_bh_enable(); 
} while (0)
```
根据上面的分析：bh RCU跟普通的RCU相比不同的是,普通RCU是禁止内核抢占,而bh RCU是禁止下半部.
**其实,带bh的RCU一般在软中断使用,不过计算quiescent state并不是发生一次上下文切换**。而是发生一次softirq.我们在后面的分析中可得到印证.
l **c****all_rcu_bh() **

C++
```
void call_rcu_bh(struct rcu_head *head, void (*func)(struct rcu_head *rcu)) 
{ 
    unsigned long flags; 
    struct rcu_data *rdp; 
    head->func = func; 
    head->next = NULL; 
    local_irq_save(flags); 
    rdp = &__get_cpu_var(rcu_bh_data); 
    *rdp->nxttail = head; 
    rdp->nxttail = &head->next; 
    if (unlikely(++rdp->qlen > qhimark)) { 
        rdp->blimit = INT_MAX; 
        force_quiescent_state(rdp, &rcu_bh_ctrlblk); 
    } 
    local_irq_restore(flags); 
}
```
它跟call_rcu()不相同的是,rcu是取per_cpu变量rcu__data和全局变量rcu_ctrlblk.而bh RCU是取rcu_bh_data,rcu_bh_ctrlblk.他们的类型都是一样的,这样做只是为了区分BH和普通RCU的等待.
对于rcu_bh_qsctr_inc

C++
```
static inline void rcu_bh_qsctr_inc(int cpu) 
{ 
    struct rcu_data *rdp = &per_cpu(rcu_bh_data, cpu); 
    rdp->passed_quiesc = 1; 
}
```
它跟rcu_qsctr_inc()机同,也是更改对应成员. 所不同的是,调用rcu_bh_qsctr_inc()的地方发生了变化.

C++
```
asmlinkage void __do_softirq(void) 
{ 
    //......
    do { 
        if (pending & 1) { 
            h->action(h); 
            rcu_bh_qsctr_inc(cpu); 
        } 
        h++; 
        pending >>= 1; 
     } while (pending); 
    //...... 
}
```
也就是说，**在发生软中断的时候，才会认为是经过了一次quiescent state**.
## **RCU链表操作**
为了操作链表，在include/linux/rculist.h有一套专门的RCU API。如：list_entry_rcu、list_add_rcu、list_del_rcu、list_for_each_entry_rcu等。即对所有kernel 的list的操作都有一个对应的RCU操作。那么这些操作和原始的list操作有哪些不同呢？我们先对比几个看下。
l **list_entry_rcu**

C++
```
#define list_entry_rcu(ptr, type, member) \
container_of(rcu_dereference(ptr), type, member)
#define list_entry(ptr, type, member) \
container_of(ptr, type, member)
```
l **__list_for_each_rcu**

C++
```
#define __list_for_each_rcu(pos, head) \
for (pos = rcu_dereference((head)->next); \
pos != (head); \
pos = rcu_dereference(pos->next))
#define __list_for_each(pos, head) \
for (pos = (head)->next; pos != (head); pos = pos->next)
```
从__list_for_each_rcu和list_entry_rcu的实现可以看出，其将指针的获取替换为使用rcu_dereference。
l **list_replace_rcu**

C++
```
static inline void list_replace_rcu(struct list_head *old,
struct list_head *new)
{
new->next = old->next;
new->prev = old->prev;
rcu_assign_pointer(new->prev->next, new);
new->next->prev = new;
old->prev = LIST_POISON2;
}
static inline void list_replace(struct list_head *old,
struct list_head *new)
{
new->next = old->next;
new->next->prev = new;
new->prev = old->prev;
new->prev->next = new;
}
```
从list_replace_rcu的实现可以看出，RCU API的实现将指针的赋值替换为rcu_assign_pointer。
l **list_del_rcu**

C++
```
static inline void list_del_rcu(struct list_head *entry)
{
__list_del(entry->prev, entry->next);
entry->prev = LIST_POISON2;
}
static inline void list_del(struct list_head *entry)
{
__list_del(entry->prev, entry->next);
entry->next = LIST_POISON1;
entry->prev = LIST_POISON2;
}
```
从list_del_rcu的实现，可以看出RCU API的实现没有将删除项的next指针置为无效。这样实现是为了防止删除节点时，读者还在遍历该节点。
## **RCU链表API使用**
下面看下RCU list API的几个应用示例。
### **只有增加和删除的链表操作**
在这种应用情况下，绝大部分是对链表的遍历，即读操作，而很少出现的写操作只有增加或删除链表项，并没有对链表项的修改操作，这种情况使用RCU非常容易，从rwlock转换成RCU非常自然。路由表的维护就是这种情况的典型应用，对路由表的操作，绝大部分是路由表查询，而对路由表的写操作也仅仅是增加或删除，因此使用RCU替换原来的rwlock顺理成章。系统调用审计也是这样的情况。
这是一段使用rwlock的系统调用审计部分的读端代码：

C++
```
static enum audit_state audit_filter_task(struct task_struct *tsk)
        {
                struct audit_entry *e;
                enum audit_state   state;
                read_lock(&auditsc_lock);
                /* Note: audit_netlink_sem held by caller. */
                list_for_each_entry(e, &audit_tsklist, list) {
                        if (audit_filter_rules(tsk, &e->rule, NULL, &state)) {
                                read_unlock(&auditsc_lock);
                                return state;
                        }
                }
                read_unlock(&auditsc_lock);
                return AUDIT_BUILD_CONTEXT;
        }
```
使用RCU后将变成：

C++
```
static enum audit_state audit_filter_task(struct task_struct *tsk)
        {
                struct audit_entry *e;
                enum audit_state   state;
                rcu_read_lock();
                /* Note: audit_netlink_sem held by caller. */
                list_for_each_entry_rcu(e, &audit_tsklist, list) {
                        if (audit_filter_rules(tsk, &e->rule, NULL, &state)) {
                                rcu_read_unlock();
                                return state;
                        }
                }
                rcu_read_unlock();
                return AUDIT_BUILD_CONTEXT;
        }
```
这种转换非常直接，使用rcu_read_lock和rcu_read_unlock分别替换read_lock和read_unlock，链表遍历函数使用_rcu版本替换就可以了。
使用rwlock的写端代码：

C++
```
static inline int audit_del_rule(struct audit_rule *rule,
                                         struct list_head *list)
        {
                struct audit_entry  *e;
                write_lock(&auditsc_lock);
                list_for_each_entry(e, list, list) {
                        if (!audit_compare_rule(rule, &e->rule)) {
                                list_del(&e->list);
                                write_unlock(&auditsc_lock);
                                return 0;
                        }
                }
                write_unlock(&auditsc_lock);
                return -EFAULT;         /* No matching rule */
        }
        static inline int audit_add_rule(struct audit_entry *entry,
                                         struct list_head *list)
        {
                write_lock(&auditsc_lock);
                if (entry->rule.flags & AUDIT_PREPEND) {
                        entry->rule.flags &= ~AUDIT_PREPEND;
                        list_add(&entry->list, list);
                } else {
                        list_add_tail(&entry->list, list);
                }
                write_unlock(&auditsc_lock);
                return 0;
        }
```
使用RCU后写端代码变成为：

C++
```
static inline int audit_del_rule(struct audit_rule *rule,
                                         struct list_head *list)
        {
                struct audit_entry  *e;
                /* Do not use the _rcu iterator here, since this is the only
                 * deletion routine. */
                list_for_each_entry(e, list, list) {
                        if (!audit_compare_rule(rule, &e->rule)) {
                                list_del_rcu(&e->list);
                                call_rcu(&e->rcu, audit_free_rule, e);
                                return 0;
                        }
                }
                return -EFAULT;         /* No matching rule */
        }
        static inline int audit_add_rule(struct audit_entry *entry,
                                         struct list_head *list)
        {
                if (entry->rule.flags & AUDIT_PREPEND) {
                        entry->rule.flags &= ~AUDIT_PREPEND;
                        list_add_rcu(&entry->list, list);
                } else {
                        list_add_tail_rcu(&entry->list, list);
                }
                return 0;
        }
```
对于链表删除操作，list_del替换为list_del_rcu和call_rcu，这是因为被删除的链表项可能还在被别的读者引用，所以不能立即删除，必须等到所有读者经历一个quiescent state才可以删除。另外，list_for_each_entry并没有被替换为list_for_each_entry_rcu，这是因为，只有一个写者在做链表删除操作，因此没有必要使用_rcu版本。
**通常情况下，write_lock和write_unlock应当分别替换成spin_lock和spin_unlock，但是对于只是对链表进行增加和删除操作而且只有一个写者的写端，在使用了_rcu版本的链表操作API后，rwlock可以完全消除，不需要spinlock来同步读者的访问。**对于上面的例子， 由于已经有audit_netlink_sem被调用者保持，所以spinlock就没有必要了。
这种情况允许修改结果延后一定时间才可见，而且写者对链表仅仅做增加和删除操作，所以转换成使用RCU非常容易。
### **写端需要对链表条目进行修改操作**
如果写者需要对链表条目进行修改，那么就需要首先拷贝要修改的条目，然后修改条目的拷贝，等修改完毕后，再使用条目拷贝取代要修改的条目，要修改条目将被在经历一个grace period后安全删除。
对于系统调用审计代码，并没有这种情况。这里假设有修改的情况，那么使用rwlock的修改代码应当如下：

C++
```
static inline int audit_upd_rule(struct audit_rule *rule,
                                         struct list_head *list,
                                         __u32 newaction,
                                         __u32 newfield_count)
        {
                struct audit_entry  *e;
                struct audit_newentry *ne;
                write_lock(&auditsc_lock);
                /* Note: audit_netlink_sem held by caller. */
                list_for_each_entry(e, list, list) {
                        if (!audit_compare_rule(rule, &e->rule)) {
                                e->rule.action = newaction;
                                e->rule.file_count = newfield_count;
                                write_unlock(&auditsc_lock);
                                return 0;
                        }
                }
                write_unlock(&auditsc_lock);
                return -EFAULT;         /* No matching rule */
        }
```
如果使用RCU，修改代码应当为；

C++
```
static inline int audit_upd_rule(struct audit_rule *rule,
                                         struct list_head *list,
                                         __u32 newaction,
                                         __u32 newfield_count)
        {
                struct audit_entry  *e;
                struct audit_newentry *ne;
                list_for_each_entry(e, list, list) {
                        if (!audit_compare_rule(rule, &e->rule)) {
                                ne = kmalloc(sizeof(*entry), GFP_ATOMIC);
                                if (ne == NULL)
                                        return -ENOMEM;
                                audit_copy_rule(&ne->rule, &e->rule);
                                ne->rule.action = newaction;
                                ne->rule.file_count = newfield_count;
                                list_replace_rcu(e, ne);
                                call_rcu(&e->rcu, audit_free_rule, e);
                                return 0;
                        }
                }
                return -EFAULT;         /* No matching rule */
        }
```
### **修改操作立即可见**
前面两种情况，读者能够容忍修改可以在一段时间后看到，也就说读者在修改后某一时间段内，仍然看到的是原来的数据。在很多情况下，读者不能容忍看到旧的数据，这种情况下，需要使用一些新措施，如System V IPC，它在每一个链表条目中增加了一个deleted字段，标记该字段是否删除，如果删除了，就设置为真，否则设置为假，当代码在遍历链表时，核对每一个条目的deleted字段，如果为真，就认为它是不存在的。
还是以系统调用审计代码为例，如果它不能容忍旧数据，那么，读端代码应该修改为：

C++
```
static enum audit_state audit_filter_task(struct task_struct *tsk)
        {
                struct audit_entry *e;
                enum audit_state   state;
                rcu_read_lock();
                list_for_each_entry_rcu(e, &audit_tsklist, list) {
                        if (audit_filter_rules(tsk, &e->rule, NULL, &state)) {
                                spin_lock(&e->lock);
                                if (e->deleted) {
                                        spin_unlock(&e->lock);
                                        rcu_read_unlock();
                                        return AUDIT_BUILD_CONTEXT;
                                }
                                rcu_read_unlock();
                                return state;
                        }
                }
                rcu_read_unlock();
                return AUDIT_BUILD_CONTEXT;
        }
```
注意，对于这种情况，每一个链表条目都需要一个spinlock保护，因为删除操作将修改条目的deleted标志。此外，该函数如果搜索到条目，返回时应当保持该条目的锁。
写端的删除操作将变成：

C++
```
static inline int audit_del_rule(struct audit_rule *rule,
                                         struct list_head *list)
        {
                struct audit_entry  *e;
                /* Do not use the _rcu iterator here, since this is the only
                 * deletion routine. */
                list_for_each_entry(e, list, list) {
                        if (!audit_compare_rule(rule, &e->rule)) {
                                spin_lock(&e->lock);
                                list_del_rcu(&e->list);
                                e->deleted = 1;
                                spin_unlock(&e->lock);
                                call_rcu(&e->rcu, audit_free_rule, e);
                                return 0;
                        }
                }
                return -EFAULT;         /* No matching rule */
        }
```
删除条目时，需要标记该条目为已删除。这样读者就可以通过该标志立即得知条目是否已经删除。
## **小结**
RCU是2.6内核引入的新的锁机制，在绝大部分为读而只有极少部分为写的情况下，它是非常高效的，因此在路由表维护、系统调用审计、SELinux的AVC、dcache和IPC等代码部分中，使用它来取代rwlock来获得更高的性能。**但是，它也有缺点，延后的删除或释放将占用一些内存，尤其是对嵌入式系统，这可能是非常昂贵的内存开销。此外，写者的开销比较大，尤其是对于那些无法容忍旧数据的情况以及不只一个写者的情况，写者需要spinlock或其他的锁机制来与其他写者同步**。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/09/cf8e8b52a0e2fe66ce4100c5b2b99ed4.png)
