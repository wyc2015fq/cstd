# ARM Linux系统的时钟机制 - DoubleLi - 博客园






1. Linux下有两类时钟：

1.1 实时钟RTC

    它由板上电池驱动的“Real Time Clock”也叫做RTC或者叫CMOS时钟，硬件时钟。当操作系统关机的时候，用这个来记录时间，但是对于运行的系统是不用这个时间的。

1.2 系统时钟

     “System clock”也叫内核时钟或者软件时钟，是由软件根据时间中断来进行计数的，内核时钟在系统关机的情况下是不存在的，所以，当操作系统启动的时候，内核时钟是要读取RTC时间来进行时间同步.



2. 标准计时器

2.1 时钟滴答计时（jiffies）的几个基本参数

2.1.1 时钟周期（clock cycle）的频率-晶振频率
       计时器Timer晶体振荡器在1秒内产生的时钟脉冲个数就是时钟周期的频率, 要注意把这个Timer的时钟周期频率与时钟中断的频率区别开来,  Linux用宏CLOCK_TICK_RATE来表示计时器的输入时钟脉冲的频率（比如我的为#define CLOCK_TICK_RATE  1193180），该宏定义在arm/mach-xxx/include/mach/timex.h头文件中。

2.1.2 时钟中断（clock tick）
      我们知道当计数器减到0值时，它就在IRQ0上产生一次时钟中断，也即一次时钟中断, 计数器的初始值决定了要过多少时钟周期才产生一次时钟中断，因此也就决定了一次时钟滴答的时间间隔长度.

2.1.3 时钟中断的频率（HZ）
      即1秒时间内Timer所产生的时钟中断次数。确定了时钟中断的频率值后也就可以确定Timer的计数器初值。Linux内核用宏HZ来表示时钟中断的频率，而且在不同的平台上HZ有不同的定义值。对于SPARC、MIPS、ARM和i386等平台HZ的值都是100。该宏在ARM平台上的定义如下（/arch/arm/include/asm/param.h）

2.1.4 计数器的初始值

计数器的初始值由宏LATCH定义在文件：include/linux/jiffies.h

#define LATCH  ((CLOCK_TICK_RATE + HZ/2) / HZ) /* For divider */

2.1.5 jiffies

       在 Linux 内核中，时间由一个名为 jiffies 的全局变量衡量，该变量标识系统启动以来经过的滴答数。在最低的级别上，计算滴答数的方式取决于正在运行的特定硬件平台；但是，滴答计数通常在一次中断期间仍然继续进行。

       标准计时器 API 作为 Linux 内核的一部分已经有很长一段时间了（自从 Linux 内核的早期版本开始）。尽管它提供的精确性比高精确度计时器要低，但它对于在处理物理设备时提供错误覆盖的传统驱动程序超时来说比较理想。在很多情况下，这些超时实际上从不触发，而是被启动，然后被删除。

       简单内核计时器使用计时器轮（timer wheel） 实现。这个主意是由 Finn Arne Gangstad 在 1997 年首次引入的。它不理睬管理大量计时器的问题，而是很好地管理数量合理的计时器 — 典型情况。（原始计时器实现只是按照过期顺序将计时器实现双重链接。尽管在概念上比较简单，但这种方法是不可伸缩的。）时间轮是一个 buckets 集合，其中每个 bucker 表示将来计时器过期的一个时间块。这些 buckets 使用基于 5 个 bucket 的对数时间定义。使用 jiffies 作为时间粒度，定义了几个组，它们表示将来的过期时段（其中每个组通过一列计时器表示）。计时器插入使用具有 O(1) 复杂度的列表操作发生，过期发生在 O(N) 时间内。计时器过期以串联的形式出现，其中计时器被从高粒度 buckets 删除，然后随着它们的过期时间的下降被插入到低粒度 buckets 中。现在我们查看一下针对这个计时器实现的 API。



2.2 计时器 API

       Linux 提供了一个简单的 API 来构造和管理计时器。它包含一些函数（和助手函数），用于创建、取消和管理计时器。

       计时器通过 timer_list 结构定义，该结构包括实现一个计时器所需的所有数据（其中包括列表指针和在编译时配置的可选计时器统计数据）。从用户角度看，timer_list 包含一个过期时间，一个回调函数（当/如果计时器过期），以及一个用户提供的上下文。用户必须初始化计时器，可以采取几种方法，最简单的方法是调用 setup_timer，该函数初始化计时器并设置用户提供的回调函数和上下文。或者，用户可以设置计时器中的这些值（函数和数据）并简单地调用 init_timer。注意，init_timer 由 setup_timer 内部调用。



**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- void init_timer( struct timer_list *timer );  
- void setup_timer( struct timer_list *timer,   
- void (*function)(unsigned long), unsigned long data );  



        拥有一个经过初始化的计时器之后，用户现在需要设置过期时间，这通过调用 mod_timer 来完成。由于用户通常提供一个未来的过期时间，他们通常在这里添加 jiffies 来从当前时间偏移。用户也可以通过调用 del_timer 来删除一个计时器（如果它还没有过期）：



**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- int mod_timer( struct timer_list *timer, unsigned long expires );  
- void del_timer( struct timer_list *timer );  



最后，用户可以通过调用 timer_pending（如果正在等待，将返回 1）来发现计时器是否正在等待（还没有发出）：



**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- int timer_pending( const struct timer_list *timer );  



2.3 计时器示例

我们来检查一下这些 API 函数的实际运行情况。下面的代码提供了一个简单的内核模块，用于展示简单计时器 API 的核心特点。在 init_module 中，您使用 setup_timer 初始化了一个计时器，然后调用 mod_timer 来启动它。当计时器过期时，将调用回调函数 my_timer_callback。最后，当您删除模块时，计时器删除（通过 del_timer）发生。（注意来自 del_timer 的返回检查，它确定计时器是否还在使用。）



**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- #include <linux/kernel.h>  
- #include <linux/module.h>  
- #include <linux/timer.h>  
- 
- MODULE_LICENSE("GPL");  
- 
- static struct timer_list my_timer;  
- 
- void my_timer_callback( unsigned long data )  
- {  
-   printk( "my_timer_callback called (%ld).\n", jiffies );  
- }  
- 
- int init_module( void )  
- {  
- int ret;  
- 
-   printk("Timer module installing\n");  
- 
- // my_timer.function, my_timer.data  
-   setup_timer( &my_timer, my_timer_callback, 0 );  
- 
-   printk( "Starting timer to fire in 200ms (%ld)\n", jiffies );  
-   ret = mod_timer( &my_timer, jiffies + msecs_to_jiffies(200) );  
- if (ret) printk("Error in mod_timer\n");  
- 
- return 0;  
- }  
- 
- void cleanup_module( void )  
- {  
- int ret;  
- 
-   ret = del_timer( &my_timer );  
- if (ret) printk("The timer is still in use...\n");  
- 
-   printk("Timer module uninstalling\n");  
- 
- return;  
- }  




 3. 高精确度计时器(hrtimer)

         高精确度计时器（简称 hrtimers）提供一个高精确度的计时器管理框架，这个框架独立于此前讨论过的标准计时器框架，原因是合并这两个框架太复杂。尽管计时器在 jiffies 粒度上运行，hrtimers 在纳秒粒度上运行。

        hrtimer 框架的实现方式与标准计时器 API 不同。hrtimer 不使用 buckets 和串联操作，而是维护一个按时间排序的计时器数据结构（按时间顺序插入计时器，以最小化激活时的处理）。这个数据结构是一个 “红-黑” 树，对于注重性能的应用程序很理想（且恰好作为内核中的一个库普遍可用）。

        hrtimer 框架作为内核中的一个 API 可用，用户空间应用程序也可以通过 nanosleep、itimers 和 Portable Operating System Interface (POSIX)-timers interface 使用它。hrtimer 框架被主线化（mainlined）到 2.6.21 内核中。



3.1 高精确度计时器 API

       hrtimer API 与传统 API 有些相似，但它们之间的一些根本差别是它能够进行额外的时间控制。应该注意的第一点是：时间不是用 jiffies 表示的，而是以一种名为 ktime 的特殊数据类型表示。这种表示方法隐藏了在这个粒度上有效管理时间的一些细节。hrtimer API 正式确认（formalize）了绝对时间和相对时间之间的区别，要求调用者指定类型。 



**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- union ktime {  
-     s64 tv64;  
- #if BITS_PER_LONG != 64 && !defined(CONFIG_KTIME_SCALAR)  
- struct {  
- # ifdef __BIG_ENDIAN  
-     s32 sec, nsec;  
- # else  
-     s32 nsec, sec;  
- # endif  
-     } tv;  
- #endif  
- };  




       与传统的计时器 API 类似，高精确度计时器通过一个结构表示 — 这里是 hrtimer。这个结构从用户角度定义定时器（回调函数、过期时间等）并包含了管理信息（其中计时器存在于 “红-黑” 树、可选统计数据等中）。  

       定义过程首先通过 hrtimer_init 初始化一个计时器。这个调用包含计时器、时钟定义和计时器模式（绝对或相对）。使用的时钟在 ./include/linux/time.h 中定义，表示系统支持的各种时钟（比如实时时钟或者单一时钟，后者只表示从一个起点[比如系统启动]开始的时间）。计时器被初始化之后，就可以通过 hrtimer_start 启动。这个调用包含过期时间（在 ktime_t 中）和时间值的模式（绝对或相对值）。





**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- struct hrtimer {  
- struct rb_node          node;  
-     ktime_t             _expires;  
-     ktime_t             _softexpires;  
- enum hrtimer_restart        (*function)(struct hrtimer *);  
- struct hrtimer_clock_base   *base;  
-     unsigned long           state;  
- #ifdef CONFIG_TIMER_STATS  
- int             start_pid;  
- void                *start_site;  
- char                start_comm[16];  
- #endif  
- };  






**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- void hrtimer_init( struct hrtimer *time, clockid_t which_clock,   
- enum hrtimer_mode mode );  
- int hrtimer_start(struct hrtimer *timer, ktime_t time, const   
- enum hrtimer_mode mode);  


hrtimer 启动后，可以通过调用 hrtimer_cancel 或 hrtimer_try_to_cancel 来取消。每个函数都包含将被停止的计时器的 hrtimer 引用。这两个函数的区别在于：hrtimer_cancel 函数试图取消计时器，但如果计时器已经发出，那么它将等待回调函数结束；hrtimer_try_to_cancel 函数也试图取消计时器，但如果计时器已经发出，它将返回失败。



**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- int hrtimer_cancel(struct hrtimer *timer);  
- int hrtimer_try_to_cancel(struct hrtimer *timer);  



可以通过调用 hrtimer_callback_running 来检查 hrtimer 是否已经激活它的回调函数。注意，这个函数由 hrtimer_try_to_cancel 内部调用，以便在计时器的回调函数被调用时返回一个错误。



**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- int hrtimer_callback_running(struct hrtimer *timer);  


3.2 一个 hrtimer 示例



**[cpp]**[view plain](http://blog.csdn.net/myarrow/article/details/7059827)[copy](http://blog.csdn.net/myarrow/article/details/7059827)



- #include <linux/kernel.h>  
- #include <linux/module.h>  
- #include <linux/hrtimer.h>  
- #include <linux/ktime.h>  
- 
- MODULE_LICENSE("GPL");  
- 
- #define MS_TO_NS(x) (x * 1E6L)  
- 
- static struct hrtimer hr_timer;  
- 
- enum hrtimer_restart my_hrtimer_callback( struct hrtimer *timer )  
- {  
-   printk( "my_hrtimer_callback called (%ld).\n", jiffies );  
- 
- return HRTIMER_NORESTART;  
- }  
- 
- int init_module( void )  
- {  
-   ktime_t ktime;  
-   unsigned long delay_in_ms = 200L;  
- 
-   printk("HR Timer module installing\n");  
- 
-   ktime = ktime_set( 0, MS_TO_NS(delay_in_ms) );  
- 
-   hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );  
- 
-   hr_timer.function = &my_hrtimer_callback;  
- 
-   printk( "Starting timer to fire in %ldms (%ld)\n", delay_in_ms, jiffies );  
- 
-   hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL );  
- 
- return 0;  
- }  
- 
- void cleanup_module( void )  
- {  
- int ret;  
- 
-   ret = hrtimer_cancel( &hr_timer );  
- if (ret) printk("The timer was still in use...\n");  
- 
-   printk("HR Timer module uninstalling\n");  
- 
- return;  
- }  



关于 hrtimer API，还有许多内容这里没有涉及到。一个有趣的方面是它能够定义回调函数的执行上下文（比如在 softirq 或 hardiirq 上下文中）。您可以在 ./include/linux/hrtimer.h 文件中进一步了解 hrtimer API。









