# Linux中断处理体系结构分析（二） - h13 - 博客园
1.中断处理的体系结构
   我们知道编写设备驱动程序一定要用到中断处理函数，这在驱动程序的编写中，占据很重要的一部分。在响应一个特定的中断的时候，内核会执行一个函数，该函数叫做中断处理程序（interrupt handler）或中断服务例程（interrupt service routine ,ISP）.产生中断的每个设备都有一个相应的中断处理程序，中断处理程序通常不和特定的设备关联，而是和特定的中断关联的，也就是说，如果一个设备可以产生多种不同的中断，那么该就可以对应多个中断处理程序，相应的，该设备的驱动程序也就要准备多个这样的函数。在Linux内核中处理中断是分为上半部（top half），和下半部（bottom half）之分的。上半部只做有严格时限的工作，例如对接收到的中断进行应答或复位硬件，这些工作是在所有的中断被禁止的
情况下完成的，能够被允许稍后完成的工作会推迟到下半部去。要想了解上半部和下半部的机制可以阅读一下《Linux内核设计与实现》的第七章的内容。
    Linux内核将所有的中断统一编号，使用一个irq_desc结构数组来描述这些中断;每个数组项对应一个中断，也可能是一组中断，它们共用相同的中断号，里面记录了中断的名称、中断状态、中断标记（比如中断类型、是否共享中断等），并提供了中断的低层硬件访问函数（清除、屏蔽、使能中断），提供了这个中断的处理函数入口，通过它可以调用用户注册的中断处理函数。
   通过irq_desc结构数组就可以了解中断处理体系结构，irq_desc结构的数据类型include/linux/irq.h
中定义，
```
struct irq_desc {
    unsigned int        irq;
    struct timer_rand_state *timer_rand_state;
    unsigned int *kstat_irqs;
#ifdef CONFIG_INTR_REMAP
    struct irq_2_iommu *irq_2_iommu;
#endif
    irq_flow_handler_t    handle_irq; // 当前中断的处理函数入口
    struct irq_chip        *chip; //低层的硬件访问
    struct msi_desc        *msi_desc;
    void            *handler_data;
    void            *chip_data;
    struct irqaction    *action;    // 用户提供的中断处理函数链表
    unsigned int        status;        //IRQ状态
                ........
    const char        *name; //中断的名称
} ____cacheline_internodealigned_in_smp;
```
   handle_irq是这个或这组中断的处理函数入口。发生中断时，总入口函数asm_do_IRQ将根据中断号调用相应irq_desc数组项中handle_irq.handle_irq使用chip结构中的函数清除、屏蔽或者重新使能中断，还要调用用户在action链表中注册的中断处理函数。
   irq_chip结构类型也是在include/linux/irq.h中定义，其中的成员大多用于操作底层硬件，比如设置寄存器以屏蔽中断，使能中断，清除中断等。
```
struct irq_chip {
    const char    *name;
    unsigned int    (*startup)(unsigned int irq);//启动中断，如果不设置，缺省为“enable
    void        (*shutdown)(unsigned int irq);/*关闭中断，如果不设置，缺省为"disable"*/
    void        (*enable)(unsigned int irq);// 使用中断，如果不设置，缺省为"unmask"
    void        (*disable)(unsigned int irq);//禁止中断，如果不设置，缺省为“mask”
    void        (*ack)(unsigned int irq);/*响应中断，通常是清除当前中断使得可以接收下一个中断*/
    void        (*mask)(unsigned int irq); //屏蔽中断源
    void        (*mask_ack)(unsigned int irq);//屏蔽和响应中断
    void        (*unmask)(unsigned int irq);//开启中断源
    void        (*eoi)(unsigned int irq);
    ........
    const char    *typename;
};
```
irq_desc结构中的irqaction结构类型在include/linux/iterrupt.h中定义。用户注册的每个中断
处理函数用一个irqaction结构来表示，一个中断比如共享中断可以有多个处理函数，它们的irqaction结
构链接成一个链表，以action为表头。irqation结构定义如下：
```
struct irqaction {
    irq_handler_t handler; //用户注册的中断处理函数
    unsigned long flags; //中断标志，比如是否共享中断，电平触发还是边沿触发
    const char *name; //用户注册的中断名字
    void *dev_id; //用户传给上面的handler的参数，还可以用来区分共享中断
    struct irqaction *next; //指向下一个用户注册函数的指针
    int irq; //中断号
    struct proc_dir_entry *dir;
    irq_handler_t thread_fn;
    struct task_struct *thread;
    unsigned long thread_flags;
};
```
   irq_desc结构数组、它的成员“struct irq_chip *chip” "struct irqaction *action",这3种数据结构构成了中断处理体系的框架。下图中描述了Linxu中断处理体系结构的关系图：
![](https://pic002.cnblogs.com/images/2011/121648/2011041323051396.jpg)
中断处理流程如下
(1)发生中断时，CPU执行异常向量vector_irq的代码
(2)在vector_irq里面，最终会调用中断处理的总入口函数asm_do_IRQ
(3)asm_do_IRQ根据中断号调用irq_desc数组项中的handle_irq。
(4)handle_irq会使用chip成员中的函数来设置硬件，比如清除中断、禁止中断、重新使能中断等
(5)handle_irq逐个调用用户在aciton链表中注册的处理函数
   中断体系结构的初始化就是构造这些数据结构，比如irq_desc数组项中的handle_irq、chip等成员；用户注册中断时就是构造action链表；用户卸载中断时就是从action链表中去除不需要的项。
2.中断处理体系结构的初始化
init_IRQ函数被用来初始化中断处理体系结构，代码在arch/arm/kernel/irq.c中
```
153 void __init init_IRQ(void)
154 {
155 int irq;
156 
157 for (irq = 0; irq < NR_IRQS; irq++)
158 irq_desc[irq].status |= IRQ_NOREQUEST | IRQ_NOPROBE;
159 
160 init_arch_irq();
161 }
```
157~~158行 初始化irq_desc结构数组中每一项的中断状态
第160行调用架构相关的中断初始化函数。对于S3C2440开发板，这个函数就是s3c24xx_init_irq,移植machine_desc结构中的init_irq成员就指向这个函数s3c24xx_init_irq函数在arch/arm/plat-s3c24xx/irq.c中定义，它为所有中断设置了芯片相关的数据结构（irq_desc[irq].chip），设置了处理函数入口（irq_desc[irq].handle_irq）。以外部中断EINT4-EINT23为例，用来设置它们的代码如下： 
```
void __init s3c24xx_init_irq(void)
534 {
535 unsigned long pend;
536 unsigned long last;
537 int irqno;
538 int i;
........
637 for (irqno = IRQ_EINT4; irqno <= IRQ_EINT23; irqno++) {
638 irqdbf("registering irq %d (extended s3c irq)\n", irqno);
639 set_irq_chip(irqno, &s3c_irqext_chip);
640 set_irq_handler(irqno, handle_edge_irq);
641 set_irq_flags(irqno, IRQF_VALID);
```
`...............`
```
655 for (irqno = IRQ_S3CUART_RX1; irqno <= IRQ_S3CUART_ERR1; irqno++) {
656 irqdbf("registering irq %d (s3c uart1 irq)\n", irqno);
657 set_irq_chip(irqno, &s3c_irq_uart1);
658 set_irq_handler(irqno, handle_level_irq);
659 set_irq_flags(irqno, IRQF_VALID);
660 }
..........
676 irqdbf("s3c2410: registered interrupt handlers\n");
677 }
678
```
   在639行set_irq_chip函数的作用就是“irq_desc[irno].chip = &s3c_irqext_chip”,以后就可能通过irq_desc[irqno].chip结构中的函数指针设置这些外部中断的触发方式（电平触发，边沿触发），使能中断，禁止中断。
   在640行设置这些中断的处理函数入口为handle_edge_irq，即“irq_desc[irqno].handle_irq =handle_edge_irq”.发生中断时，handle_edge_irq函数会调用用户注册的具体处理函数； 在641行设置中断标志为“IRQF_VALID”，表示可以使用它们。init_IRQ函数执行完后，irq_desc数组项的chip,handl_irq成员都被设置
2.2 用户注册中断处理函数的过程
    用户驱动程序通过request_irq函数向内核注册中断处理函数，request_irq函数根据中断号找到irq_desc数组项，然后在它的action链表添加一个表项。原先的内核中requset_irq函数在kernel/irq/manage.c中定义，而现在2.6.32版本中，进行了改变，可以看这篇文章[http://eeek.borgchat.net/lists/newbies/msg39146.html](http://eeek.borgchat.net/lists/newbies/msg39146.html) ，这里解释了，在2.6.32内核中我们可以看到找不到了request_irq函数的实现，而是用request_threaded_irq()函数给替换了。我们可以在inclue/linux/interrupt.h中找到这个函数的原型。
```
110 #ifdef CONFIG_GENERIC_HARDIRQS
111 extern int __must_check
112 request_threaded_irq(unsigned int irq, irq_handler_t handler,
113 irq_handler_t thread_fn,
114 unsigned long flags, const char *name, void *dev);
115 
116 static inline int __must_check
117 request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
118 const char *name, void *dev)
119 {
120 return request_threaded_irq(irq, handler, NULL, flags, name, dev);
121 }
123 extern void exit_irq_thread(void);
124 #else
126 extern int __must_check
127 request_irq(unsigned int irq, irq_handler_t handler, unsigned long flags,
128 const char *name, void *dev);
136 static inline int __must_check
137 request_threaded_irq(unsigned int irq, irq_handler_t handler,
138 irq_handler_t thread_fn,
139 unsigned long flags, const char *name, void *dev)
140 {
141 return request_irq(irq, handler, flags, name, dev);
142 }
143
144 static inline void exit_irq_thread(void) { }
145 #endif
```
其实具体的实现在request_threaded_irq函数中，也是在/kernel/irq/manage.c中定义，requset_threaded_irq函数首先使用这4个参数构造一个irqaction结构，然后调用setup_irq函数将它链入链表中， 
```
1003 int request_threaded_irq(unsigned int irq, irq_handler_t handler,
1004 irq_handler_t thread_fn, unsigned long irqflags,
[1005](http://lxr.free-electrons.com/source/kernel/irq/source/kernel/irq/manage.c?v=2.6.32#L1005)                          const char *[devname](http://lxr.free-electrons.com/source/kernel/irq/ident?v=2.6.32;i=devname), void *[dev_id](http://lxr.free-electrons.com/source/kernel/irq/ident?v=2.6.32;i=dev_id))
.............
1056 action->handler = handler;
1057 action->thread_fn = thread_fn;
1058 action->flags = irqflags;
1059 action->name = devname;
1060 action->dev_id = dev_id;
1061 
1062 chip_bus_lock(irq, desc);
1084 local_irq_restore(flags);
1085 enable_irq(irq);
...........
1088 return retval;
1089 }
1090 EXPORT_SYMBOL(request_threaded_irq);
```
setup_irq函数也是在kernel/irq.manage.c中定义，它完成如下3个主要功能
（1）将新建的irqaction结构链入irq_desc[irq]结构的action链表中，这有两种可能。
如果action链表为空，则直接链入,否则先判断新建的irqaction结构和链表中的irqaction结构所表示的中断类型是否一致，即是否都声明为"可共享的"（IRQF_SHARED)、是否都使用相同的触发方式，如果一致，则将新建的irqation结构链入
(2)设置irq_desc[irq]结构中chip成员的还没设置的指针，让它们指向一些默认函数
chip成员在init_IRQ函数初始化中断体系结构的时候已经设置了，这里只是设置其中还没设置的指针这通过irq_chip_set_defaults函数来完成，它在kernel/irq/chip.c中定义
```
296 void irq_chip_set_defaults(struct irq_chip *chip)
297 {
298 if (!chip->enable)
299 chip->enable = default_enable;//调用chip->unmask
300 if (!chip->disable)
301 chip->disable = default_disable;//此函数为空
302 if (!chip->startup)
303 chip->startup = default_startup;//调用chip->enable
310 if (!chip->shutdown)
311 chip->shutdown = chip->disable != default_disable ?
312 chip->disable : default_shutdown;
313 if (!chip->name)
314 chip->name = chip->typename;
315 if (!chip->end)
316 chip->end = dummy_irq_chip.end;
317 }
```
(4)启动中断
如果irq_desc[irq]结构中status成员没有被指明IRQ_NOAUTOEN（表示注册中断时不要使用中断），还要调用chip->startup或chip->enable来启动中断，所谓启动中断通常就是使用中断。一般情况下，只有那些“可以自动使能的”中断对应的irq_desc[irq].status才会被指明为IRQ_NOAUTOEN，所以，无论哪种情况，执行request_irq注册中断之后，这个中断就已经被使能了。
总结一下request_irq函数注册
(1)irq_des[irq]结构中的action链表中已经链入了用户注册的中断处理函数
(2)中断的触发方式已经被设好
(3)中断已经被使能
2.3 中断的处理过程
asm_do_IRQ是中断的C语言总入口函数，它在/arch/arm/kernel/irq.c中定义，
```
106 asmlinkage void __exception asm_do_IRQ(unsigned int irq, struct pt_regs *regs)
107 {
108 struct pt_regs *old_regs = set_irq_regs(regs);
109 
110 irq_enter();
111 
112 /*
113 * Some hardware gives randomly wrong interrupts. Rather
114 * than crashing, do something sensible.
115 */
116 if (unlikely(irq >= NR_IRQS)) {
117 if (printk_ratelimit())
118 printk(KERN_WARNING "Bad IRQ%u\n", irq);
119 ack_bad_irq(irq);
120 } else {
121 generic_handle_irq(irq);
122 }
123 
124 /* AT91 specific workaround */
125 irq_finish(irq);
126 
127 irq_exit();
128 set_irq_regs(old_regs);
129 }
```
st1\:*{behavior:url(#ieooui) }
desc_hand_irq函数直接调用desc结构中的hand_irq成员函数，它就是irq_desc[irq].handle.irq
asm_do_IRQ函数中参数irq的取值范围为IRQ_EINT0~（IRQ_EINT0 + 31）,只有32个取值。它可能是一个实际的中断号，也可能是一组中断的中断号。这里有S3C2440的芯片特性决定的：发生中断时，INTPND寄存器的某一位被置1，INTOFFSET寄存器中记录了是哪一位（0--31），中断向量调用asm_do_IRQ之前要把INTOFFSET寄存器的值确定irq参数。每一个实际的中断在irq_desc数组中都有一项与它对应，它们的数目不止32.当asm_do_IRQ函数参数irq表示的是“一组”中断时，irq_desc[irq].handle_irq成员函数还需要先分辨出是哪一个中断，然后调用irq_desc[irqno].handle_irq来进一步处理。
以外部中断EINT8—EINT23为例，它们通常是边沿触发
(1) 它们被触发里，INTOFFSET寄存器中的值都是5，asm_do_IRQ函数中参数irq的值为（IRQ_EINTO+5）,即IRQ_EINT8t23，
(2)irq_desc[IRQ_EINT8t23].handle_irq在前面init_IRQ函数初始化中断体系结构的时候被设为s3c_irq_demux_extint8.
(3)s3c_irq_demux_extint8函数的代码在arch/arm/plat-s3c24xx/irq.c中，它首先读取EINTPEND、EINTMASK寄存器，确定发生了哪些中断，重新计算它们的中断号，然后调用irq_desc数组项中的handle_irq成员函数
```
453 s3c_irq_demux_extint8(unsigned int irq,
454 struct irq_desc *desc)
455 {
456 unsigned long eintpnd = __raw_readl(S3C24XX_EINTPEND); //EINT8-EINT23 发生时，相应位被置1
457 unsigned long eintmsk = __raw_readl(S3C24XX_EINTMASK);//屏蔽寄存器
458 
459 eintpnd &= ~eintmsk; //清除被屏蔽的位
460 eintpnd &= ~0xff; /* 清除低8位（EINT8对应位8）ignore lower irqs */
461 
462 /* 循环处理所有子中断*/
463 
464 while (eintpnd) {
465 irq = __ffs(eintpnd); //确定eintpnd中为1的最高位
466 eintpnd &= ~(1<<irq); //将此们清0
467 
468 irq += (IRQ_EINT4 - 4);//重新计算中断号，前面计算出irq等于8时，中断号为
                            IRQ_EINT8
469 generic_handle_irq(irq);//调用这中断的真正的处理函数
470 }
471 
472 }
void
```
(4)IRQ_EINT8--IRQ_EINT23这几个中断的处理函数入口，在init_IRQ函数初始化中断体系结构的时候已经被设置为handle_edge_irq函数，desc_handle_irq(irq,irq_desc+irq)就是调用这个函数，它在kernel/irq/chip.c中定义，它用来处理边沿触发的中断，
中断发生的次数统计
```
531 handle_edge_irq(unsigned int irq, struct irq_desc *desc)
532 {
533 spin_lock(&desc->lock);
534 
535 desc->status &= ~(IRQ_REPLAY | IRQ_WAITING);
536 
537 /*
538 * If we're currently running this IRQ, or its disabled,
539 * we shouldn't process the IRQ. Mark it pending, handle
540 * the necessary masking and go out
541 */
542 if (unlikely((desc->status & (IRQ_INPROGRESS | IRQ_DISABLED)) ||
543 !desc->action)) {
544 desc->status |= (IRQ_PENDING | IRQ_MASKED);
545 mask_ack_irq(desc, irq);
546 goto out_unlock;
547 }
548 kstat_incr_irqs_this_cpu(irq, desc);
549 
550 /* Start handling the irq */
551 if (desc->chip->ack)
552 desc->chip->ack(irq);
553 
554 /* Mark the IRQ currently in progress.*/
555 desc->status |= IRQ_INPROGRESS;
556 
557 do {
558 struct irqaction *action = desc->action;
559 irqreturn_t action_ret;
560 
561 if (unlikely(!action)) {
562 desc->chip->mask(irq);
563 goto out_unlock;
564 }
565 
566 /*
567 * When another irq arrived while we were handling
568 * one, we could have masked the irq.
569 * Renable it, if it was not disabled in meantime.
570 */
571 if (unlikely((desc->status &
572 (IRQ_PENDING | IRQ_MASKED | IRQ_DISABLED)) ==
573 (IRQ_PENDING | IRQ_MASKED))) {
574 desc->chip->unmask(irq);
575 desc->status &= ~IRQ_MASKED;
576 }
577 
578 desc->status &= ~IRQ_PENDING;
579 spin_unlock(&desc->lock);
580 action_ret = handle_IRQ_event(irq, action);
581 if (!noirqdebug)
582 note_interrupt(irq, desc, action_ret);
583 spin_lock(&desc->lock);
584 
585 } while ((desc->status & (IRQ_PENDING | IRQ_DISABLED)) == IRQ_PENDING);
586 
587 desc->status &= ~IRQ_INPROGRESS;
588 out_unlock:
589 spin_unlock(&desc->lock);
590 }
591
```
st1\:*{behavior:url(#ieooui) }
响应中断，通常是清除当前中断使得可以接收下一个中断，对于IRQ_EINT8~IRQ_EINT23这几个中断，desc->chip在前面init_IRQ函数初始化中断体系结构的时候被设为s3c_irqext_chip.desc->chip->ack就是s3c_irqext_ack函数，（arch/armplat-s3c24xx/irq.c）它用来清除中断
handle_IRQ_event函数来逐个执行action链表中用户注册的中断处理函数，它在kernel/irq/handle.c中定义。
```
do {
379 trace_irq_handler_entry(irq, action);
380 ret = action->handler(irq, action->dev_id);//执行用户注册的中断处理函数
381 trace_irq_handler_exit(irq, action, ret);
382 
383 switch (ret) {
384 case IRQ_WAKE_THREAD:
385 /*
386 * Set result to handled so the spurious check
387 * does not trigger.
388 */
389 ret = IRQ_HANDLED;
390 
391 /*
392 * Catch drivers which return WAKE_THREAD but
393 * did not set up a thread function
394 */
395 if (unlikely(!action->thread_fn)) {
396 warn_no_thread(irq, action);
397 break;
398 }
399 
400 /*
408 if (likely(!test_bit(IRQTF_DIED,
409 &action->thread_flags))) {
410 set_bit(IRQTF_RUNTHREAD, &action->thread_flags);
411 wake_up_process(action->thread);
412 }
413 
414 /* Fall through to add to randomness */
415 case IRQ_HANDLED:
416 status |= action->flags;
417 break;
418 
419 default:
420 break;
421 }
422 
423 retval |= ret;
424 action = action->next; //下一个
425 } while (action);
```
用户注册的中断处理函数的参数为中断号irq,action->dev_id。后一个参数是通过request_irq函数注册中断时传入的dev_id参数，它由用户自己指定、自己使用，可以为空，当这个中断是“共享中断”时除外。
对于电平触发的中断，它们的irq_desc[irq].handle_irq通常是handle_level_irq函数。它也是在kernel/irq/chip.c中定义，其功能与上述handle_edge_irq函数相似，
对于handle_level_irq函数已经清除了中断，但是它只限于清除SoC内部的的信号，如果外设输入到SoC的中断信号仍然有效，这就会导致当前中断处理完成后，会误认为再次发生了中断，对于这种情况，需要用户注册的中断处理函数中清除中断，先清除外设的中断，然后再清除SoC内部的中断号。
中断的处理流程可以总结如下 
(1)中断向量调用总入口函数asm_do_IRQ，传入根据中断号irq
(2)asm_do_IRQ函数根据中断号irq调用irq_desc[irq].handle_irq，它是这个中断的处理函数入口，对于电平触发的中断，这个入口函数通常为handle_level_irq,对于边沿触发的中断，这个入口通常为handle_edge_irq
(3)入口函数首先清除中断，入口函数是handle_level_irq时还要屏蔽中断
(4)逐个调用用户在irq_desc[irq].aciton链表中注册的中断处理函数
(5) 入口函数是handle_level_irq时还要重新开启中断
  卸载中断处理函数这通过free_irq函数来实现，它与request_irq一样，也是在kernel/irq/mangage.c中定义。
它需要用到两个参数：irq和dev_id,它们与通过request_irq注册中断函数时使用的参数一样，使用中断号irq定位action链表，再使用dev_id在action链表中找到要卸载的表项。同一个中断的不同中断处理函数必须使用不同的dev_id来区分，在注册共享中断时参数dev_id必惟一。
free_irq函数的处理过程与request_irq函数相反
(1)根据中断号irq，dev_id从action链表中找到表项，将它移除
(2)如果它是惟一的表项，还要调用IRQ_DESC[IRQ].CHIP->SHUTDOWN 或IRQ_DESC[IRQ].CHIP->DISABLW来关闭中断。
在响应一个特定的中断的时候，内核会执行一个函数，该函数叫做中断处理程序（interrupt handler）或中断服务例程（interrupt service routine ,ISP）.产生中断的每个设备都有一个相应的中断处理程序，中断处理程序通常不和特定的设备关联，而是和特定的中断关联的，也就是说，如果一个设备可以产生多种不同的中断，那么该就可以对应多个中断处理程序，相应的，该设备的驱动程序也就要准备多个这样的函数。在Linux内核中处理中断是分为上半部（top half），和下半部（bottom half）之分的。上半部只做有严格时限的工作，例如对接收到的中断进行应答或复位硬件，这些工作是在所有的中断被禁止的情况下完成的，能够被允许稍后完成的工作会推迟到下半部去。要想了解上半部和下半部的机制可以阅读一下《Linux内核设计与实现》
