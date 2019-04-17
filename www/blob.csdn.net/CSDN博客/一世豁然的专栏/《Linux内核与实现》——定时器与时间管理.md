# 《Linux内核与实现》——定时器与时间管理 - 一世豁然的专栏 - CSDN博客





2014年11月20日 15:20:25[一世豁然](https://me.csdn.net/Explorer_day)阅读数：737








一、内核中的时间概念

  1、系统定时器以某种频率自行触发时钟中断，该频率可以通过编程预定，称为节拍率




  2、内知道连续两次时钟中断的时间间隔。这个间隔时间就称为节拍（tick），它等于节拍率分之一秒。




  3、利用时间中断周期性执行的工作。

    1）、更新系统运行时间。

    2）、更新实际时间。

    3）、在SMP系统上，均衡调度程序中各处理器上的队列。

    4）、检查当前进程是否用尽了自己的时间片。

    5）、运行超时的动态定时器。

    6）、更新资源消耗和处理器的统计值。










二、节拍率：HZ

 一）、相关简介


  1、系统定时器频率（节拍率）是通过静态预定义的，也就是HZ，在系统启动时按照HZ值对硬件进行处理。




  2、体系结构不同，HZ的值也不同。实际上，对于某些体系结构来说，甚至是机器的不同，它的值也会不一样。




  3、编写内核代码时，不要认为HZ值是一个固定不变的值。




  4、不同体系结构的时钟中断频率（P168  表1-1）







 二）、理想的HZ值







 三）、高HZ的优势

  1、内核定时器能够以更高的频率和更高的准确度运行。




  2、依赖定时器执行的系统调用，能够以更高的精度运行。




  3、对诸如资源消耗和系统调用等测量会有更精细的解析度。




  4、提高进程抢占的准确度。








 四）、高HZ的劣势

  1、节拍率越高，意味着时钟中断频率越高，也就意味着系统负担越重。










三、jiffies

 一）、相关简介

  1、全局变量jiffies用来记录自系统启动以来产生的节拍的总数。其定义在文件<linux/jiffies.h>中：

        extern  unsigned  long  volatile  jiffies；







 二）、jiffies的内部表示

  1、jiffies变量总是无符号长整型（unsigned  long），因此，在32位体系结构上市32位，在64位体系结构上是64位。

    1）、jiffies定义为unsigned  long

          extern  unsigned  long  volatile  jiffies;

    2）、第二个变量也定义在<linux/jiffies>中：

          extern  u64  jiffies_64；




  2、jiffies和jiffies_64的划分。

    1）、访问jiffies的代码仅会读取jiffies_64的低32位。通过get_jiffies_64()函数可以读取整个64位数值。

    2）、在64位体系结构上，jiffies_64和jiffies指的是同一变量，代码既可以直接读取jiffies也可以调用get_jiffies_64()函数，它们作用相同。







 三）、jiffies的回绕

  1、和任何C整型一样，当jiffies变量的值超过它的最大存放范围后就会发生溢出。




  2、内核提供了四个宏来帮助比较节拍计数，它们能够正确处理节拍计数回绕情况。

        #define  time_after(unkown,  known)   ((long)  (konwn) - (long) (unkonwn) <  0)

        #define  time_before(unkown,  known)   ((long)  (konwn) - (long) (unkonwn) <  0)

        #define  time_after_eq(unkown,  known)   ((long)  (konwn) - (long) (unkonwn) <  0)

        #define  time_before_eq(unkown,  known)   ((long)  (konwn) - (long) (unkonwn) <  0)

       unkonwn通常是jiffies，konwn擦nsa胡是需要对比的值。







 四）、用户空间和HZ

  1、在2.6版以前的内核中，如果改变内核中的HZ值，会给用户空间中某些程序造成异常结果。为避免上面结果，内核必须修改所有导出的jiffies值。因而内恶化定义了

         USER_HZ来代替用户空间看到的HZ值。




  2、内核可以使用函数jiffies_to_clock()将一个由HZ表示的节拍数转换成一个由USER_HZ表示的节拍数。同理，函数jiffies_64_to_clock()将64位的jiffies值得单位从HZ转换为

        USER_HZ。










四、硬时钟和定时器

  1、体系结构提供了两种设备进行计时——一种是系统定时器；另一种是实时时钟。




  2、实时时钟

    1）、实时时钟是用来持久存放系统时间的设备，即使系统关闭后，它也可以靠主板上的微型电池提供的电力保持系统的计时。




  3、系统定时器

    1）、系统定时器提供一种周期性触发中断机制。







五、时钟中断处理程序

  1、时钟中断处理程序可以划分两个部分；体系结构相关部分和体系结构无关部分。




  2、与体系结构相关的例程作为系统定时器的中断处理程序而注册到内核中，以便在产生时钟中断时，它能够相应的运行。绝大多数处理程序最低限度也都要执行如下工作：

    1）、获得xtime_lock锁，以便访问jiffies_54和墙上时间xtime进行保护。

    2）、需要时应答或重新设置系统时钟。

    3）、周期性地使用墙上时间更新实时时钟。

    4）、调用体系结构无关的时钟例程：tick_periodic()。




  3、中断服务程序主要通过调用与体系无关的例程，tick_periodic()执行下面的工作：

    1）、给jiffies_64变量加一。

    2）、更新资源消耗的统计量。

    3）、执行已经到期的动态定时器。

    4）、执行schedule_tick()函数。

    5）、更新墙上时间，改时间存放在xtime中。

    6）、计算平均负载值。




  4、相关例程代码（P175-P176）。







六、墙上时间

  1、当前的实际时间（墙上时间）定义于kernel/time/timekeeping.c中：

        struct  timespec  xtime;

    1）、struct  timespec {

                    _kernel_time_t  tv_sec;       /*秒*/


                    long  tv_nsec;                         /*ns*/


              };

    2）、xtime.tv_sec以秒为单位，存放着UTC以来经过的时间。




  2、读写xtime变量需要使用xtime锁，该锁不是普通自旋锁而是一个seqlock锁。

    1）、更新xtime首先要申请一个seqlock锁。

    2）、读取xtime时也要使用read_seqbegin和read_seqretry函数。




  3、从用户空间取得墙上时间的主要接口是gettimeofday()，在内核中对应系统调用为sys_gettimeofday(),定义于kernel/time.c。




  4、系统调用settimeofday()来设置当前时间，他需要具有CAP_SYS_TIME权能。




  5、除了更新xtime时间外，内核不会像用户空间那样频繁使用xtime。但有时需要注意的特殊情况，那就是在文件系统的实现代码中存放访问时间戳时需要使用xtime。










七、定时器

 一）、使用定时器

  1、定时器由结构timer_list 表示，定义在文件<linux/timer.h>中。

        struct  timer_list {

             struct  list_head  entry;

             unsigned  long  expires;

             void  (*function)(unsigned  long);

             unsigned  long  data;

             struct  tvex_t_base_s  *base;

        }；




  2、内核提供了一组与定时器相关的接口用来简化定时器的操作。所有的这些操作都声明在文件<linux/timer.h>中，大多数接口在文件kernel/timer.c中定义。

    1）、创建定时器首先需要定义它：

              struct  timer_list  my_timer;

    2）、接着初始化定时器数据结构内部值，初始化必须在使用其他定时器管理函数对定时器进行操作前完成：

              init_timer(&my_timer);

    3）、填充结构中需要的值。

              my_timer.expires = jiffies + delay;

              my_timer.data = 0;

              my_timer.function = my_function;

    4）、激活定时器

              add_timer(&my_timer);




  3、有时候可能需要更改已经激活的定时器超时时间，所以内核通过函数mod_timer来实现该功能，该函数可以改变指定的定时器超时时间：

         mod_timer(&my_timer,  jiffies + new_delay);




  4、如果需要在定时器超时前停止定时器，可使用del_timer()函数：

        del_timer(&my_timer);

        没有潜在竞争条件的删除函数

        del_timer_sync(&my_timer);







 二）、定时器竞争条件

  1、首先，绝不能再用如下代码代替mod_timer()函数，来改变定时器超时时间。此代码在多处理器上是不安全的

        del_timer(my_timer);

        my_timer->expires = jiffies + new_delay;

        add_timer(my_timer);




  2、其次，一般情况下应该使用del_timer_sync()函数取代del_timer()函数，因为无法确定再删除定时器时，它是否正在其他处理器上运行。




  3、最后，因为内核异步执行中断处理程序，所以应该重点保护定时器中断处理程序中的共享数据。







 三）、实现定时器

  1、内核在时钟中断发生后执行定时器，定时器作为软中断在上下文中执行。具体来说，时钟中断处理程序会执行up_process_times()函数，该函数随即调用



        run_local_times()函数。




  2、为了提高搜索效率，内核将定时器按它们的超时时间划分为五组。当定时器超时时间接近时，定时器将随组一起下移。采用分组定时器的方法可以在执行软中断的多数情况

        下，确保尽可能减少搜索超时所带来的负担。










八、延迟执行

 一）、忙等待

  1、最简单的延迟方法是忙等待。但要注意该方法仅仅在要想延迟的时间是节拍的整数倍，或者精度率要求不高时才使用。




 二）、短延时

  1、内核提供了三个可以处理us、ns和ms级别的延迟函数，它们定义在<linux/delay.h>和<asm/delay.h>中。

         void  udelay(unsigned  long  usecs);


         void  ndelay(unsigned  long  usecs);


         void  mdelay(unsigned  long  usecs);




 三）、schedule_timeout()

  1、更理想的延迟执行办法是使用schedule_timeout()函数，该方法会让需要延迟执行的任务睡眠到指定的延迟时间耗尽后再执行。当该方法不能保证睡眠时间正好等于指定的

        延迟时间，只能尽量使睡眠时间接近指定的延迟时间。但指定的时间到期后，内恶化会唤醒被延迟的任务并将其重新放回运行队列。

        set_current_state(TASK_INTERRUPTIBLE);

        /*小睡一会儿，s秒后唤醒*/

        schedule_timeout(s*HZ);




  2、schedule_timeout()函数的实现（P183-P184代码），但定时器超时时,process_timeout函数会被调用。




  3、设置超时时间，在等待队列上睡眠。










