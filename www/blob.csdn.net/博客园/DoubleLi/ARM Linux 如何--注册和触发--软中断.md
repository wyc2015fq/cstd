# ARM Linux 如何--注册和触发--软中断 - DoubleLi - 博客园






**1. 注册软中断当然是通过open_softirq**

例子如下：



**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7064060)[copy](http://blog.csdn.net/myarrow/article/details/7064060)



- void __init init_timers(void)  
- {  
- int err = timer_cpu_notify(&timers_nb, (unsigned long)CPU_UP_PREPARE,  
-                 (void *)(long)smp_processor_id());  
- 
-     init_timer_stats();  
- 
-     BUG_ON(err == NOTIFY_BAD);  
-     register_cpu_notifier(&timers_nb);  
-     open_softirq(TIMER_SOFTIRQ, run_timer_softirq);  
- }  
- 
- void open_softirq(int nr, void (*action)(struct softirq_action *))  
- {  
-     softirq_vec[nr].action = action;  
- }  


软中断TIMER_SOFTIRQ的中断处理函数为：run_timer_softirq

之所以成为softirq，是因为这些中断是由硬件中断来间接触发的,如何间接触发的呢：
硬件中断处理函数-->对软中断的相应位置位-->唤醒ksoftirqd线程-->执行软中断的中断处理函数



**2. 硬件中断如何通过置位唤醒ksoftirqd线程**

timer interrupt handler->
timer_tick->
update_process_times->
run_local_timers->
hrtimer_run_queues()和raise_softirq(TIMER_SOFTIRQ)->
raise_softirq_irqoff->
__raise_softirq_irqoff { or_softirq_pending(1UL << (nr)); }
即(local_softirq_pending() |= (x))



**3. 如何执行软中断的action<中断处理函数>**

对于TIMER_SOFTIRQ来说，每次system clock产生中断时，即一个tick 到来时，在system clock的中断处理函数中会调用run_local_timers来设置TIMER_SOFTIRQ触发条件；也就是当前CPU对应的irq_cpustat_t结构体中的__softirq_pending成员的第TIMER_SOFTIRQ个BIT被置为1。 而当这个条件满足时，ksoftirqd线程(入口函数run_ksoftirqd,cpu_callback:kthread_create(run_ksoftirqd, hcpu, "ksoftirqd/%d", hotcpu);)会被唤醒，然后按照下面的流程调用TIMER_SOFTIRQ在数组softirq_vec中注册的action，即run_timer_softirq。
run_ksoftirqd--->do_softirq--->__do_softirq--->softirq_vec[TIMER_SOFTIRQ].action






**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7064060)[copy](http://blog.csdn.net/myarrow/article/details/7064060)



- static int run_ksoftirqd(void * __bind_cpu)  
- {  
-     set_current_state(TASK_INTERRUPTIBLE);  
- 
- while (!kthread_should_stop()) {  
-         preempt_disable();  
- if (!local_softirq_pending()) {  
-             preempt_enable_no_resched();  
-             schedule();  
-             preempt_disable();  
-         }  
- 
-         __set_current_state(TASK_RUNNING);  
- 
- while (local_softirq_pending()) {  
- /* Preempt disable stops cpu going offline. 
-                If already offline, we'll be on wrong CPU: 
-                don't process */  
- if (cpu_is_offline((long)__bind_cpu))  
- goto wait_to_die;  
-             do_softirq();  
-             preempt_enable_no_resched();  
-             cond_resched();  
-             preempt_disable();  
-             rcu_sched_qs((long)__bind_cpu);  
-         }  
-         preempt_enable();  
-         set_current_state(TASK_INTERRUPTIBLE);  
-     }  
-     __set_current_state(TASK_RUNNING);  
- return 0;  
- 
- wait_to_die:  
-     preempt_enable();  
- /* Wait for kthread_stop */  
-     set_current_state(TASK_INTERRUPTIBLE);  
- while (!kthread_should_stop()) {  
-         schedule();  
-         set_current_state(TASK_INTERRUPTIBLE);  
-     }  
-     __set_current_state(TASK_RUNNING);  
- return 0;  
- }  















