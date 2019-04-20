# Linux内部的时钟处理机制全面剖析 - h13 - 博客园
**Linux****内部的时钟处理机制全面剖析**
在 Linux 操作系统中，很多活动都和时间有关，例如：进程调度和网络处理等等。所以说，了解 Linux 操作系统中的时钟处理机制有助于更好地了解 Linux 操作系统的运作方式。本文分析了 Linux 2.6.25 内核的时钟处理机制，首先介绍了在计算机系统中的一些硬件计时器，然后重点介绍了 Linux 操作系统中的硬件时钟和软件时钟的处理过程以及软件时钟的应用。最后对全文进行了总结。
**◆1、计算机系统中的计时器**
在计算机系统中存在着许多硬件计时器，例如 Real Timer Clock （ RTC ）、Time Stamp Counter （ TSC ） 和 Programmable Interval Timer （ PIT ） 等等。
这部分内容不是本文的重点，这里仅仅简单介绍几种，更多内容参见参考文献：
·Real Timer Clock （ RTC ）：
o独立于整个计算机系统（例如：
CPU 和其他 chip ）
o内核利用其获取系统当前时间和日期
·Time Stamp Counter
（ TSC ）：
o从 Pentium 起，提供一个寄存器 TSC，用来累计每一次外部振荡器产生的时钟信号
o通过指令 rdtsc 访问这个寄存器
o比起 PIT，TSC 可以提供更精确的时间测量
·Programmable
Interval Timer （ PIT ）：
o时间测量设备
o内核使用的产生时钟中断的设备，产生的时钟中断依赖于硬件的体系结构，慢的为 10 ms 一次，快的为 1 ms 一次
·High Precision
Event Timer （ HPET ）：
oPIT 和 RTC 的替代者，和之前的计时器相比，HPET 提供了更高的时钟频率（至少10 MHz ）以及更宽的计数器宽度（64位）
o一个 HPET 包括了一个固定频率的数值增加的计数器以及3到32个独立的计时器，这每一个计时器有包涵了一个比较器和一个寄存器（保存一个数值，表示触发中断的时机）。每一个比较器都比较计数器中的数值和寄存器中的数值，当这两个数值相等时，将产生一个中断
**◆2、硬件时钟处理**
这里所说的硬件时钟处理特指的是硬件计时器时钟中断的处理过程。
**2.1****数据结构**
和硬件计时器（本文又称作硬件时钟，区别于软件时钟）相关的数据结构主要有两个：
·struct clocksource
：对硬件设备的抽象，描述时钟源信息
·struct
clock_event_device ：时钟的事件信息，包括当硬件时钟中断发生时要执行那些操作（实际上保存了相应函数的指针）。本文将该结构称作为“时钟事件设备”。
上述两个结构内核源代码中有较详细的注解，分别位于文件 clocksource.h
和 clockchips.h 中。需要特别注意的是结构
clock_event_device 的成员 event_handler ，它指定了当硬件时钟中断发生时，内核应该执行那些操作，也就是真正的时钟中断处理函数。
在2.3节“时钟初始化”部分会介绍它真正指向哪个函数。
Linux 内核维护了两个链表，分别存储了系统中所有时钟源的信息和时钟事件设备的信息。这两个链表的表头在内核中分别是 clocksource_list 和 clockevent_devices 。图2-1显示了这两个链表。
![](https://pic002.cnblogs.com/images/2012/121648/2012022117274240.jpg)
图2-1 时钟源链表和时钟事件链表
**2.2****通知链技术（ notification chain ）**
在时钟处理这部分中，内核用到了所谓的“通知链（ notification
chain ）”技术。所以在介绍时钟处理过程之前先来了解下“通知链”技术。
在 Linux 内核中，各个子系统之间有很强的相互关系，一些被一个子系统生成或者被探测到的事件，很可能是另一个或者多个子系统感兴趣的，也就是说这个事件的获取者必须能够通知所有对该事件感兴趣的子系统，并且还需要这种通知机制具有一定的通用性。基于这些， Linux 内核引入了“通知链”技术。
**2.2.1****数据结构：**
通知链有四种类型，
1.原子通知链（ Atomic notifier chains ）：通知链元素的回调函数（当事件发生时要执行的函数）只能在中断上下文中运行，不允许阻塞
2.可阻塞通知链（ Blocking notifier chains ）：通知链元素的回调函数在进程上下文中运行，允许阻塞
3.原始通知链（ Raw notifier chains ）：对通知链元素的回调函数没有任何限制，所有锁和保护机制都由调用者维护
4.SRCU 通知链（ SRCU notifier chains ）：可阻塞通知链的一种变体
所以对应了四种通知链头结构：
·struct
atomic_notifier_head ：原子通知链的链头
·struct
blocking_notifier_head ：可阻塞通知链的链头
·struct raw_notifier_head
：原始通知链的链头
·struct
srcu_notifier_head ： SRCU 通知链的链头
通知链元素的类型：
·struct
notifier_block ：通知链中的元素，记录了当发出通知时，应该执行的操作（即回调函数）
链头中保存着指向元素链表的指针。通知链元素结构则保存着回调函数的类型以及优先级，参见 notifier.h 文件。
**2.2.2****运作机制**
通知链的运作机制包括两个角色：
1.被通知者：对某一事件感兴趣一方。定义了当事件发生时，相应的处理函数，即回调函数。但需要事先将其注册到通知链中（被通知者注册的动作就是在通知链中增加一项）。
2.通知者：事件的通知者。当检测到某事件，或者本身产生事件时，通知所有对该事件感兴趣的一方事件发生。他定义了一个通知链，其中保存了每一个被通知者对事件的处理函数（回调函数）。通知这个过程实际上就是遍历通知链中的每一项，然后调用相应的事件处理函数。
包括以下过程：
1.通知者定义通知链
2.被通知者向通知链中注册回调函数
3.当事件发生时，通知者发出通知（执行通知链中所有元素的回调函数）
被通知者调用 notifier_chain_register 函数注册回调函数，该函数按照优先级将回调函数加入到通知链中
。注销回调函数则使用 notifier_chain_unregister 函数，即将回调函数从通知链中删除。2.2.1节讲述的4种通知链各有相应的注册和注销函数，但是他们最终都是调用上述两个函数完成注册和注销功能的。有兴趣的读者可以自行查阅内核代码。
通知者调用 notifier_call_chain 函数通知事件的到达，这个函数会遍历通知链中所有的元素，然后依次调用每一个的回调函数（即完成通知动作）。2.2.1节讲述的4种通知链也都有其对应的通知函数，这些函数也都是最终调用 notifier_call_chain 函数完成事件的通知。
由以上的叙述，“通知链”技术可以概括为：事件的被通知者将事件发生时应该执行的操作通过函数指针方式保存在链表（通知链）中，然后当事件发生时通知者依次执行链表中每一个元素的回调函数完成通知。
**2.3 ****时钟初始化**
内核初始化部分（ start_kernel 函数）和时钟相关的过程主要有以下几个：
1.tick_init()
2.init_timers()
3.hrtimers_init()
4.time_init()
其中函数 hrtimers_init() 和高精度时钟相关（本文暂不介绍这部分内容）。下面将详细介绍剩下三个函数。
**2.3.1 tick_init ****函数**
函数 tick_init() 很简单，调用
clockevents_register_notifier 函数向 clockevents_chain 通知链注册元素： tick_notifier。这个元素的回调函数指明了当时钟事件设备信息发生变化（例如新加入一个时钟事件设备等等）时，应该执行的操作，该回调函数为 tick_notify （参见2.4节）。
**2.3.2 init_timers ****函数**
函数 init_timers() 的实现如清单2-1（省略了部分和主要功能无关的内容，以后代码同样方式处理）
注：本文中所有代码均来自于Linux2.6.25 源代码
单2-1
  init_timers 函数
  void __init init_timers(void)
  {
  int err = timer_cpu_notify(&timers_nb, (unsigned long)CPU_UP_PREPARE,
  (void *)(long)smp_processor_id());
……
  register_cpu_notifier(&timers_nb);
  open_softirq(TIMER_SOFTIRQ,run_timer_softirq, NULL);
  }
代码解释：
·初始化本 CPU 上的软件时钟相关的数据结构，参见3.2节
·向 cpu_chain 通知链注册元素 timers_nb ，该元素的回调函数用于初始化指定 CPU 上的软件时钟相关的数据结构
·初始化时钟的软中断处理函数
**2.3.3 time_init ****函数**
函数 time_init 的实现如清单2-2
清单2-2
  time_init 函数
  void __init time_init(void)
  {
……
  init_tsc_clocksource();
  late_time_init = choose_time_init();
  }
函数 init_tsc_clocksource 初始化 tsc 时钟源。choose_time_init 实际是函数 hpet_time_init ，其代码清单2-3
清单2-3 hpet_time_init 函数
清单2-3 hpet_time_init 函数
  void __init hpet_time_init(void)
  {
  if (!hpet_enable())
  setup_pit_timer();
setup_irq(0, &irq0);
  }
函数 hpet_enable 检测系统是否可以使用 hpet 时钟，如果可以则初始化 hpet 时钟。否则初始化 pit 时钟。最后设置硬件时钟发生时的处理函数（参见2.4节）。
初始化硬件时钟这个过程主要包括以下两个过程（参见 hpet_enable 的实现）：
1.初始化时钟源信息（ struct
clocksource 类型的变量），并将其添加到时钟源链表中，即 clocksource_list 链表（参见图2-1）。
2.初始化时钟事件设备信息（
struct clock_event_device 类型的变量），并向通知链
clockevents_chain 发布通知：一个时钟事件设备要被添加到系统中。在通知（执行回调函数）结束后，该时钟事件设备被添加到时钟事件设备链表中，即 clockevent_devices 链表（参见图2-1）。有关通知链的内容参见2.2节。
需要注意的是在初始化时钟事件设备时，全局变量 global_clock_event 被赋予了相应的值。该变量保存着系统中当前正在使用的时钟事件设备（保存了系统当前使用的硬件时钟中断发生时，要执行的中断处理函数的指针）。
**2.4 ****硬件时钟处理过程**
由2.3.3可知硬件时钟中断的处理函数保存在静态变量 irq0 中，其定义如清单2-4
单2-4 变量irq0定义
static struct irqaction irq0 = {
  .handler = timer_event_interrupt,
  .flags  = IRQF_DISABLED | IRQF_IRQPOLL | IRQF_NOBALANCING,
  .mask  = CPU_MASK_NONE,
  .name  = "timer"
  };
由定义可知：函数 timer_event_interrupt 为时钟中断处理函数，其定义如清单2-5
清单2-5
  timer_event_interrupt 函数
  static irqreturn_t timer_event_interrupt(int irq, void *dev_id)
  {
  add_pda(irq0_irqs, 1);
  global_clock_event->event_handler(global_clock_event);
  return IRQ_HANDLED;
  }
为了说明这个问题，不妨假设系统中使用的是 hpet 时钟。由2.3.3节可知 global_clock_event 指向 hpet 时钟事件设备（ hpet_clockevent ）。查看 hpet_enable 函数的代码并没有发现有对 event_handler 成员的赋值。所以继续查看时钟事件设备加入事件的处理函数
tick_notify ，该函数记录了当时钟事件设备发生变化（例如，新时钟事件设备的加入）时，执行那些操作（参见2.3.1节），代码如清单2-6
清单2-6 tick_notify 函数
static int tick_notify(struct notifier_block *nb, unsigned long reason,
  void *dev)
  {
  switch (reason) {
  case CLOCK_EVT_NOTIFY_ADD:
  return tick_check_new_device(dev);
……
  return NOTIFY_OK;
  }
由代码可知：对于新加入时钟事件设备这个事件，将会调用函数 tick_check_new_device 。顺着该函数的调用序列向下查找。tick_set_periodic_handler 函数将时钟事件设备的
event_handler 成员赋值为 tick_handle_periodic 函数的地址。由此可知，函数 tick_handle_periodic 为硬件时钟中断发生时，真正的运行函数。
函数 tick_handle_periodic 的处理过程分成了以下两个部分：
1.全局处理：整个系统中的信息处理
2.局部处理：局部于本地 CPU 的处理
总结一下，一次时钟中断发生后， OS 主要执行的操作（
tick_handle_periodic ）：
·全局处理（仅在一个 CPU 上运行）：
1.更新 jiffies_64
2.更新 xtimer 和当前时钟源信息等
3.根据 tick 计算 avenrun 负载
·局部处理（每个 CPU 都要运行）：
1.根据当前在用户态还是核心态，统计当前进程的时间：用户态时间还是核心态时间
2.唤醒 TIMER_SOFTIRQ
软中断
3.唤醒 RCU 软中断
4.调用
scheduler_tick （更新进程时间片等等操作，更多内容参见参考文献）
5.profile_tick 函数调用
以上就介绍完了硬件时钟的处理过程，下面来看软件时钟。
**◆3、软件时钟处理**
这里所说“软件时钟”指的是软件定时器（Software Timers），是一个软件上的概念，是建立在硬件时钟基础之上的。它记录了未来某一时刻要执行的操作（函数），并使得当这一时刻真正到来时，这些操作（函数）能够被按时执行。举个例子说明：它就像生活中的闹铃，给闹铃设定振铃时间（未来的某一时间）后，当时间（相当于硬件时钟）更新到这个振铃时间后，闹铃就会振铃。这个振铃时间好比软件时钟的到期时间，振铃这个动作好比软件时钟到期后要执行的函数，而闹铃时间更新好比硬件时钟的更新。
实现软件时钟原理也比较简单：每一次硬件时钟中断到达时，内核更新的 jiffies ，然后将其和软件时钟的到期时间进行比较。如果 jiffies 等于或者大于软件时钟的到期时间，内核就执行软件时钟指定的函数。
接下来的几节会详细介绍 Linux2.6.25 是怎么实现软件时钟的。
**3.1 ****相关数据结构**
·struct timer_list ：软件时钟，记录了软件时钟的到期时间以及到期后要执行的操作。具体的成员以及含义见表3-1。
·struct tvec_base ：用于组织、管理软件时钟的结构。在 SMP 系统中，每个 CPU 有一个。具体的成员以及含义参见表3-2。
表3-1 struct timer_list 主要成员
|**域名**|**类型**|**描述**|
|----|----|----|
|entry|struct list_head|所在的链表|
|expires|unsigned long|到期时间，以 tick 为单位|
|function|void  (*)(unsigned long)|回调函数，到期后执行的操作|
|data|unsigned long|回调函数的参数|
|base|struct tvec_base  *|记录该软件时钟所在的  struct tvec_base 变量|
表3-2 struct tvec_base 类型的成员
|**域名**|**类型**|**描述**|
|----|----|----|
|lock|spinlock_t|用于同步操作|
|running_timer|struct  timer_list *|正在处理的软件时钟|
|timer_jiffies|unsigned long|当前正在处理的软件时钟到期时间|
|tv1|struct tvec_root|保存了到期时间从  timer_jiffies 到 timer_jiffies + 2^8-1之间（包括边缘值）的所有软件时钟|
|tv2|struct tvec|保存了到期时间从  timer_jiffies +2^8到 timer_jiffies +2^14-1之间（包括边缘值）的  所有软件时钟|
|tv3|struct tvec|保存了到期时间从  timer_jiffies +2^14到 timer_jiffies +2^20-1之间（包括边缘值）的所有软件时钟|
|tv4|struct tvec|保存了到期时间从  timer_jiffies +2^20到 timer_jiffies +2^26-1之间（包括边缘值）的所有软件时钟|
|tv5|struct tvec|保存了到期时间从  timer_jiffies +2^26到 timer_jiffies +2^32-1之间（包括边缘值）的所有软件时钟|
**注：**一个 tick 表示的时间长度为两次硬件时钟中断发生时的时间间隔
其中 tv1 的类型为 struct tvec_root ，tv 2~ tv 5的类型为 struct tvec ，清单3-1显示它们的定义清单3-1 struct tvec_root 和 struct tvec 的定义
struct tvec {
  struct list_head vec[TVN_SIZE];
  };
  struct tvec_root {
  struct list_head vec[TVR_SIZE];
  }
可见它们实际上就是类型为 struct list_head 的数组，其中 TVN_SIZE 和 TVR_SIZE 在系统没有配置宏 CONFIG_BASE_SMALL 时分别被定义为64和256。
**3.2 ****数据结构之间的关系**
![](https://pic002.cnblogs.com/images/2012/121648/2012022117283153.jpg)
图3-1显示了以上数据结构之间的关系
从图中可以清楚地看出：软件时钟（ struct timer_list ，在图中由 timer 表示）以双向链表（ struct list_head ）的形式，按照它们的到期时间保存相应的桶（ tv1~tv5 ）中。tv1 中保存了相对于 timer_jiffies 下256个
tick 时间内到期的所有软件时钟； tv2 中保存了相对于
timer_jiffies 下256*64个 tick 时间内到期的所有软件时钟； tv3 中保存了相对于 timer_jiffies 下256*64*64个 tick 时间内到期的所有软件时钟； tv4 中保存了相对于 timer_jiffies 下256*64*64*64个 tick 时间内到期的所有软件时钟； tv5 中保存了相对于 timer_jiffies 下256*64*64*64*64个 tick 时间内到期的所有软件时钟。具体的说，从静态的角度看，假设 timer_jiffies 为0，那么
tv1[0] 保存着当前到期（到期时间等于 timer_jiffies ）的软件时钟（需要马上被处理）， tv1[1] 保存着下一个 tick 到达时，到期的所有软件时钟， tv1[n] （0<= n <=255）保存着下 n 个 tick 到达时，到期的所有软件时钟。而 tv2[0] 则保存着下256到511个 tick 之间到期所有软件时钟， tv2[1] 保存着下512到767个 tick 之间到期的所有软件时钟，
tv2[n] （0<= n <=63）保存着下256*(n+1)到256*(n+2)-1个 tick 之间到达的所有软件时钟。 tv3~tv5 依次类推。
从上面的说明中可以看出：软件时钟是按照其到期时间相对于当前正在处理的软件时钟的到期时间（
timer_jiffies 的数值）保存在 struct tvec_base 变量中的。而且这个到期时间的最大相对值（到期时间 - timer_jiffies ）为 0xffffffffUL （ tv5 最后一个元素能够表示的相对到期时间的最大值）。
注：一个tick的长度指的是两次硬件时钟中断发生之间的时间间隔
还需要注意的是软件时钟的处理是局部于 CPU 的，所以在 SMP 系统中每一个 CPU 都保存一个类型为 struct tvec_base 的变量，用来组织、管理本 CPU 的软件时钟。从图中也可以看出 struct tvec_base 变量是 per-CPU 的（关于 per-CPU 的变量原理和使用参见参考资料）。
由于以后的讲解经常要提到每个 CPU 相关的 struct
tvec_base 变量，所以为了方便，称保存软件时钟的 struct tvec_base 变量为该软件时钟的 base ，或称 CPU 的 base
。
**3.3 ****添加或删除软件时钟**
在了解了软件时钟的数据组织关系之后，现在来看一下如何添加以及删除一个软件时钟。
**3.3.1 ****添加软件时钟**
在 Linux 内核中要添加一个软件时钟，首先必须分配
struct timer_list 类型的变量，然后调用函数 add_timer() 将该软件时钟添加到相应调用 add_timer 函数的 CPU 的
base 中。 Add_timer 是对函数
__mod_timer() 的一层包装。函数 __mod_timer() 的代码如清单3-2：
清单3-2 __mod_timer 函数
int __mod_timer(struct timer_list *timer, unsigned long expires)
  {
  struct tvec_base *base, *new_base;
  unsigned long flags;
  int ret = 0;
……
  base = lock_timer_base(timer, &flags);
  if (timer_pending(timer)) {
  detach_timer(timer, 0);
  ret = 1;
  }
  new_base = __get_cpu_var(tvec_bases);
if (base != new_base) {
  if (likely(base->running_timer != timer)) {
  /* See the comment in lock_timer_base() */
  timer_set_base(timer, NULL);
  spin_unlock(&base->lock);
  base = new_base;
  spin_lock(&base->lock);
  timer_set_base(timer, base);
  }
  }
  timer->expires = expires;
  internal_add_timer(base, timer);
  spin_unlock_irqrestore(&base->lock, flags);
  return ret;
  }
清单3-2 __mod_timer 函数
代码解释：
1.取得软件时钟所在 base 上的同步锁（ struct tvec_base 变量中的自旋锁），并返回该软件时钟的 base ，保存在 base 变量中
2.如果该软件时钟处在
pending 状态（在 base 中，准备执行），则卸载该软件时钟
3.取得本 CPU 上的 base 指针（类型为 struct tvec_base* ），保存在 new_base 中
4.如果 base 和 new_base 不一样，也就是说软件时钟发生了迁移（从一个 CPU 中移到了另一个 CPU 上），那么如果该软件时钟的处理函数当前没有在迁移之前的那个 CPU 上运行，则先将软件时钟的 base 设置为 NULL ，然后再将该软件时钟的 base 设置为 new_base 。否则，跳到5。
5.设置软件时钟的到期时间
6.调用
internal_add_timer 函数将软件时钟添加到软件时钟的 base 中（本 CPU 的 base ）
7.释放锁
注：卸载软件时钟的意思是指将软件时钟从软件时钟所在 base 中删除，以后所说的卸载软件时钟也都是这个意思
这里有必要详细说明一下软件时钟如何被添加到软件时钟的 base 中的（添加到本 CPU base 的 tv1~tv5 里面），因为这是软件时钟处理的基础。来看函数 internal_add_timer 函数的实现，如清单3-3
清单3-3 internal_add_timer 函数
static void internal_add_timer(struct tvec_base *base, struct timer_list
  *timer)
  {
  unsigned long expires = timer->expires;
  unsigned long idx = expires - base->timer_jiffies;
  struct list_head *vec;
  if (idx < TVR_SIZE) {
  int i = expires & TVR_MASK;
  vec = base->tv1.vec + i;
  } else if (idx < 1 << (TVR_BITS + TVN_BITS)) {
  int i = (expires >> TVR_BITS) & TVN_MASK;
  vec = base->tv2.vec + i;
  } else if (idx < 1 << (TVR_BITS + 2 * TVN_BITS)) {
  int i = (expires >> (TVR_BITS + TVN_BITS)) & TVN_MASK;
  vec = base->tv3.vec + i;
  } else if (idx < 1 << (TVR_BITS + 3 * TVN_BITS)) {
  int i = (expires >> (TVR_BITS + 2 * TVN_BITS)) & TVN_MASK;
  vec = base->tv4.vec + i;
  } else if ((signed long) idx < 0) {
  vec = base->tv1.vec + (base->timer_jiffies & TVR_MASK);
  } else {
  int i;
  if (idx > 0xffffffffUL) {
  idx = 0xffffffffUL;
  expires = idx + base->timer_jiffies;
  }
  i = (expires >> (TVR_BITS + 3 * TVN_BITS)) & TVN_MASK;
  vec = base->tv5.vec + i;
  }
  list_add_tail(&timer->entry, vec);
  }
代码解释：
·计算该软件时钟的到期时间和
timer_jiffies （当前正在处理的软件时钟的到期时间）的差值，作为索引保存到 idx 变量中。
·判断 idx 所在的区间，在
o[0,2^8-1 ]或者( -无穷,
0)（该软件时钟已经到期），则将要添加到 tv1 中
o[
2^8 ,2^14-1 ]，则将要添加到 tv2 中
o[
2^14,2^20-1 ]，则将要添加到 tv3 中
o[
2^20 ,2^26-1 ]，则将要添加到 tv4 中
o[
2^26 ,2^32-1 )，则将要添加到 tv5 中，但实际上最大值为
0xffffffffUL
·计算所要加入的具体位置（哪个链表中，即 tv1~tv5 的哪个子链表，参考图3-1）
·最后将其添加到相应的链表中
从这个函数可以得知，内核中是按照软件时钟到期时间的相对值（相对于 timer_jiffies 的值）将软件时钟添加到软件时钟所在的 base 中的。
**3.3.2 ****删除软件时钟**
内核可调用 del_timer 函数删除软件时钟，
del_timer 的代码如清单3-4
清单3-4 del_timer 函数
int
  del_timer(struct timer_list *timer)
  {
  struct tvec_base *base;
  unsigned long flags;
  int ret = 0;
……
  if (timer_pending(timer)) {
  base = lock_timer_base(timer, &flags);
  if (timer_pending(timer)) {
  detach_timer(timer, 1);
  ret = 1;
  }
  spin_unlock_irqrestore(&base->lock, flags);
  }
  return ret;
  }
代码解释：
1.检测该软件时钟是否处在
pending 状态（在 base 中，准备运行），如果不是则直接函数返回
2.如果处于 pending 状态，则获得锁
3.再次检测软件时钟是否处于
pending 状态（该软件时钟可能被卸载了），不是则释放锁然后函数返回
4.如果还是 pending 状态，则将其卸载，之后释放锁，函数返回
如果在 SMP 系统中，则需使用 del_timer_sync 函数来删除软件时钟。在讲解 del_timer_sync 函数之前，先来看下
try_to_del_timer_sync 函数的实现（该函数被 del_timer_sync 函数使用），其代码如清单3-5
清单3-5 try_to_del_timer_sync 函数
int
  try_to_del_timer_sync(struct timer_list *timer)
  {
  struct tvec_base *base;
  unsigned long flags;
  int ret = -1;
  base = lock_timer_base(timer, &flags);
  if (base->running_timer == timer)
  goto out;
  ret = 0;
  if (timer_pending(timer)) {
  detach_timer(timer, 1);
  ret = 1;
  }
  out:
  spin_unlock_irqrestore(&base->lock, flags);
  return ret;
  }
该函数检测当前运行的软件时钟是不是该软件时钟，如果是，则函数返回-1，表明目前不能删除该软件时钟；如果不是检测该软件时钟是否处于 pending 状态，如果不是，则函数返回0，表明软件时钟已经被卸载，如果处于 pending 状态再把软件时钟卸载，函数返回1，表明成功卸载该软件时钟。
接下来，再来看看函数 del_timer_sync 定义，如清单3-6
清单3-6 del_timer_sync 函数
int
  del_timer_sync(struct timer_list *timer)
  {
  for (;;) {
  int ret = try_to_del_timer_sync(timer);
  if (ret >= 0)
  return ret;
  cpu_relax();
  }
  }
del_timer_sync 函数无限循环试图卸载该软件时钟，直到该软件时钟能够被成功卸载。从其实现中可以看出：如果一个软件时钟的处理函数正在执行时，对其的卸载操作将会失败。一直等到软件时钟的处理函数运行结束后，卸载操作才会成功。这样避免了在 SMP 系统中一个 CPU 正在执行软件时钟的处理函数，而另一个 CPU 则要将该软件时钟卸载所引发的问题。
**3.3 ****时钟的软中断处理**
软件时钟的处理是在时钟的软中断中进行的。
**3.3.1 ****软中断初始化**
软中断的一个重要的处理时机是在每个硬件中断处理完成后（参见 irq_exit 函数），且由2.4节的内容可知：在硬件时钟中断处理中，会唤醒时钟的软中断，所以每次硬件时钟中断处理函数执行完成后都要进行时钟的软中断处理。和时钟相关的软中断是 TIMER_SOFTIRQ ，其处理函数为 run_timer_softirq ，该函数用来处理所有的软件时钟。这部分初始化代码在函数 init_timers 中进行，如清单3-7
清单3-7 init_timers 函数
void __init
  init_timers(void)
  {
……
  open_softirq(TIMER_SOFTIRQ, run_timer_softirq, NULL);
  }
**3.3.2 ****处理过程**
函数 run_timer_softirq 所作的工作就是找出所有到期的软件时钟，然后依次执行其处理函数。其代码如清单3-8
清单3-8 run_timer_softirq函数
static void
  run_timer_softirq(struct softirq_action *h)
  {
  struct tvec_base *base = __get_cpu_var(tvec_bases);
hrtimer_run_pending();
  if (time_after_eq(jiffies, base->timer_jiffies))
  __run_timers(base);
  }
函数首先获得到本地 CPU 的 base 。然后检测如果 jiffies大于等于 timer_jiffies ，说明可能已经有软件时钟到期了，此时就要进行软件时钟的处理，调用函数 __run_timers 进行处理。如果 jiffies 小于 timer_jiffies ，表明没有软件时钟到期，则不用对软件时钟进行处理。函数返回。
注： hrtimer_run_pending() 函数是高精度时钟的处理。本文暂没有涉及高精度时钟相关的内容。
接下来看一下函数 __run_timers 都作了些什么，如清单3-9
清单3-9 __run_timers函数
static inline void __run_timers(struct tvec_base *base)
  {
……
  spin_lock_irq(&base->lock);
  while (time_after_eq(jiffies, base->timer_jiffies)) {
……
  int index = base->timer_jiffies & TVR_MASK;
  if (!index &&
  (!cascade(base, &base->tv2, INDEX(0))) &&
  (!cascade(base, &base->tv3, INDEX(1))) &&
  !cascade(base, &base->tv4, INDEX(2)))
  cascade(base, &base->tv5, INDEX(3));
  ++base->timer_jiffies;
  list_replace_init(base->tv1.vec + index, &work_list);
  while (!list_empty(head)) {
……
  timer = list_first_entry(head, struct timer_list,entry);
  fn = timer->function;
  data = timer->data;
……
  set_running_timer(base, timer);
  detach_timer(timer, 1);
  spin_unlock_irq(&base->lock);
  {
  int preempt_count = preempt_count();
  fn(data);
……
  }
  spin_lock_irq(&base->lock);
  }
  }
  set_running_timer(base, NULL);
  spin_unlock_irq(&base->lock);
  }
代码解释：
1.获得 base 的同步锁
2.如果 jiffies 大于等于 timer_jiffies （当前正要处理的软件时钟的到期时间，说明可能有软件时钟到期了），就一直运行3~7，否则跳转至8
3.计算得到 tv1 的索引，该索引指明当前到期的软件时钟所在 tv1 中的链表（结构参见3.2节），代码：
int index =
  base->timer_jiffies & TVR_MASK;
1.调用 cascade 函数对软件时钟进行必要的调整（稍后会介绍调整的过程）
2.使得 timer_jiffies
的数值增加1
3.取出相应的软件时钟链表
4.遍历该链表，对每个元素进行如下操作
·设置当前软件时钟为 base 中正在运行的软件时钟（即保存当前软件时钟到 base-> running_timer 成员中）
·将当前软件时钟从链表中删除，即卸载该软件时钟
·释放锁，执行软件时钟处理程序
·再次获得锁
1.设置当前 base 中不存在正在运行的软件时钟
2.释放锁
**3.3.3 ****软件时钟调整过程**
函数 cascade 用于调整软件时钟（这个调整过程是指：将马上就要到期的软件时钟从其所在的链表中删除，重新计算到期时间的相对值（到期时间 - timer_jiffies ），然后根据该值重新插入到 base 中）。注意到在软件时钟处理过程中，每次都是从 tv1 中取出一个链表进行处理，而不是从 tv2~tv5 中取，所以对软件时钟就要进行必要的调整。
在讲解 cascade 函数之前，再从直观上理解下为什么需要进行调整。所有软件时钟都是按照其到期时间的相对值（相对于 timer_jiffies ）被调加到 base 中的。但是 timer_jiffies 的数值都会在处理中增加1（如3.3.2节所示），也就是说这个相对值会随着处理发生变化，当这个相对值小于等于256时，就要将软件时钟从 tv2~tv5 中转移到 tv1 中（
tv1 中保存着下256个 tick 内到期的所有软件时钟）。
函数 cascade 的实现如清单3-10
清单3-10 cascade 函数
static int cascade(struct tvec_base *base, struct tvec *tv, int index)
  {
  struct timer_list *timer, *tmp;
  struct list_head tv_list;
  list_replace_init(tv->vec + index, &tv_list);
  list_for_each_entry_safe(timer, tmp, &tv_list, entry) {
……
  internal_add_timer(base, timer);
  }
  return index;
  }
该函数根据索引，取出相应的 tv （ tv2~tv5 ）中的链表，然后遍历链表每一个元素。按照其到期时间重新将软件时钟加入到软件时钟的 base 中。该函数返回 tv 中被调整的链表索引值（参见图3-1）。
清单3-9中调整软件时钟的代码如下：
int index = base->timer_jiffies & TVR_MASK;
  if (!index &&
  (!cascade(base, &base->tv2, INDEX(0))) &&
  (!cascade(base, &base->tv3, INDEX(1))) &&
  !cascade(base, &base->tv4, INDEX(2)))
  cascade(base, &base->tv5, INDEX(3));
这部分代码表明：如果 index 有0再到0时（ index 是对
timer_jiffies 取模），说明时间已经过了256个
tick ，这时要把 tv2 中软件时钟转移到 tv1 中。如果 index 和第一个 cascade 函数的返回值都从0再到到0时，说明时间已经过了256*64个 tick ，这时要把 tv3 中软件时钟转移到 tv1 或者 tv2 中。之后的调整过程依次类推。
**3.4 ****自我激活**
软件时钟可分为两种类型：
·仅仅激活一次
·激活多次或者周期性激活
多次激活的实现机制就是要在软件时钟处理函数中重新设置软件时钟的到期时间为将来的一个时间，这个过程通过调用
mod_timer 函数来实现。该函数的实现如清单3-11
清单3-11 mod_timer 函数
int mod_timer(struct timer_list *timer, unsigned long expires)
  {
……
  if (timer->expires == expires && timer_pending(timer))
  return 1;
return __mod_timer(timer, expires);
  }
从代码中可以看出，该函数实际上调用 __mod_timer 函数（参见3.3.1节）来调整软件时钟的到期时间。
**3.5 ****软件时钟的应用**
软件时钟的处理是在处理软中断时触发的，而软中断的处理又会紧接着硬件中断处理结束而进行，并且系统会周期地产生时钟中断（硬件中断），这样，软件时钟的处理至少会在系统每一次时钟中断处理完成后触发（如果软件时钟的到期时间大于系统当前的 jiffies ，表明时间未到期，则不会调用保存在软件时钟中的函数，但此时的确提供了处理软件时钟的时机）。从这点上看，软件时钟会有较快的相应——一旦时间到期，保存在软件时钟中的函数会将快地被调用（在时钟软中断中被调用，参见3.3.2节）。所以内核中凡是需要隔一段时间间隔后作指定操作的过程都通过软件时钟完成。例如大部分设备驱动程序使用软件时钟探测异常条件、软盘驱动程序利用软件时钟关闭有一段时间没有被访问软盘的设备马达、进程的定时睡眠（ schedule_timeout 函数）和网络超时重传等等。
本节主要通过介绍进程的定时睡眠（ schedule_timeout 函数）和网络超时重传来说明软件时钟的应用。
**3.5.1 ****进程的定时睡眠**
函数 schedule_timeout 的代码如清单3-12
清单3-12 函数 schedule_timeout
signed long __sched schedule_timeout(signed long timeout)
  {
  struct timer_list timer;
  unsigned long expire;
……
  expire = timeout + jiffies;
setup_timer(&timer, process_timeout, (unsigned long)current);
  __mod_timer(&timer, expire);
  schedule();
  del_singleshot_timer_sync(&timer);
timeout = expire - jiffies;
out:
  return timeout < 0 ? 0 : timeout;
  }
函数 schedule_timeout 定义了一个软件时钟变量
timer ，在计算到期时间后初始化这个软件时钟：设置软件时钟当时间到期时的处理函数为
process_timeout ，参数为当前进程描述符，设置软件时钟的到期时间为 expire 。之后调用 schedule() 函数。此时当前进程睡眠，交出执行权，内核调用其它进程运行。但内核在每一个时钟中断处理结束后都要检测这个软件时钟是否到期。如果到期，将调用 process_timeout 函数，参数为睡眠的那个进程描述符。
process_timeout 函数的代码如清单3-13。
清单3-13 函数 process_timeout
static void process_timeout(unsigned long __data)
  {
  wake_up_process((struct task_struct *)__data);
  }
函数 process_timeout 直接调用
wake_up_process 将进程唤醒。当内核重新调用该进程执行时，该进程继续执行
schedule_timeout 函数，执行流则从 schedule 函数中返回，之后调用 del_singleshot_timer_sync 函数将软件时钟卸载，然后函数
schedule_timeout 结束。函数 del_singleshot_timer_sync 是实际上就是函数 del_timer_sync （参见3.3.2节），如清单3-14
清单3-14 函数del_singleshot_timer_sync
  #define del_singleshot_timer_sync(t) del_timer_sync(t)
以上就是进程定时睡眠的实现过程。接下来介绍的是软件时钟在网络超时重传上的应用。
**3.5.2 ****网路超时重传**
对于 TCP 协议而言，如果某次发送完数据包后，并超过一定的时间间隔还没有收到这次发送数据包的 ACK 时， TCP 协议规定要重新发送这个数据包。
在 Linux2.6.25 的内核中，这种数据的重新发送使用软件时钟来完成。这个软件时钟保存在面向连接的套接字（对应内核中 inet_connection_sock 结构）中。对这个域的初始在函数
tcp_init_xmit_timers 中，如清单3-15
清单3-15 函数 tcp_init_xmit_timers 、函数 inet_csk_init_xmit_timers 和函数 setup_timer
void
  tcp_init_xmit_timers(struct sock *sk)
  {
  inet_csk_init_xmit_timers(sk,&tcp_write_timer,
  &tcp_delack_timer,&tcp_keepalive_timer);
  }
void
  inet_csk_init_xmit_timers(struct sock *sk,
  void (*retransmit_handler)(unsigned long),
  void (*delack_handler)(unsigned long),
  void (*keepalive_handler)(unsigned long))
  {
  struct inet_connection_sock *icsk = inet_csk(sk);
setup_timer(&icsk->icsk_retransmit_timer, retransmit_handler,
  (unsigned long)sk);
……
  }
static inline
  void setup_timer(struct timer_list * timer,
  void (*function)(unsigned long),
  unsigned long data)
  {
  timer->function = function;
  timer->data = data;
  init_timer(timer);
  }
在函数 inet_csk_init_xmit_timers 中，变量 icsk 就是前面提到的面向连接的套接字，其成员
icsk_retransmit_timer 则为实现超时重传的软件时钟。该函数调用 setup_timer 函数将函数 tcp_write_timer （参考函数 tcp_init_xmit_timers ）设置为软件时钟 icsk->icsk_retransmit_timer 当时间到期后的处理函数。初始化的时候并没有设置该软件时钟的到期时间。
在 TCP 协议具体的一次数据包发送中，函数
tcp_write_xmit 用来将数据包从 TCP 层发送到网络层，如清单3-16。
清单3-16 tcp_write_xmit 函数
static int
  tcp_write_xmit(struct sock *sk, unsigned int mss_now, int nonagle)
  {
  struct tcp_sock *tp = tcp_sk(sk);
  struct sk_buff *skb;
……
**if (unlikely(tcp_transmit_skb(sk, skb, 1, GFP_ATOMIC)))**
  break;
  tcp_event_new_data_sent(sk, skb);
……
  return !tp->packets_out && tcp_send_head(sk);
  }
注意该函数中加粗的函数，其中 tcp_transmit_skb 函数是真正将数据包由 TCP 层发送到网络层中的函数。数据发送后，将调用函数
tcp_event_new_data_sent ，而后者又会调用函数 inet_csk_reset_xmit_timer
来设置超时软件时钟的到期时间。
当函数 tcp_event_new_data_sent 结束之后，处理超时的软件时钟已经设置好了。内核会在每一次时钟中断处理完成后检测该软件时钟是否到期。如果网络真的超时，没有 ACK 返回，那么当该软件时钟到期后内核就会执行函数 tcp_write_timer 。函数 tcp_write_timer 将进行数据包的重新发送，并重新设置超时重传软件时钟的到期时间。
**◆4 总结**
本文介绍了 Linux 内核的时钟处理机制。首先简单介绍了系统的硬件计时器，然后重点介绍了硬件时钟的处理过程和软件时钟的处理过程以及软件时钟的应用。
文章转自：[http://os.51cto.com/art/200709/89522.htm](http://os.51cto.com/art/200709/89522.htm)
