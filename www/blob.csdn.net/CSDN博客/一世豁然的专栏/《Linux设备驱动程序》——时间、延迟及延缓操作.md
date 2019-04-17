# 《Linux设备驱动程序》——时间、延迟及延缓操作 - 一世豁然的专栏 - CSDN博客





2014年11月12日 15:16:12[一世豁然](https://me.csdn.net/Explorer_day)阅读数：736








一、度量时间差

 一）、相关介绍


  1、时钟中断由系统定时硬件以周期性的间隔产生，这个间隔有内核根据HZ的值设定，HZ是一个与体系结构有关的常数。

    1）、作为一般性规则，即使知道对应平台上的确切HZ值，也不应在编程依赖该HZ值。

    2）、如果想改变系统时钟中断发生的频率，可以通过修改HZ值来进行。但是，如果修改了头文件中的HZ，则必须使用新的值重新编译内核以及所有模块。




  2、每当时钟中断发生时，内核内部计数的值就增加一。这个技术器的值在系统引导时被初始化为0，因此，它的值就是自上次操作系统引导以来的时钟滴答数。

    1）、驱动程序开发者访问的是jiffies变量，它是unsigned  long 型变量，要么和jiffies_64相同，要么仅仅是jiffies_64的低32位。通常首选jiffies，因此对它的访问很快，从而对

              64位jiffies_64值访问并不需要在所有架构上都是原子的。

    2）、除了由内核管理的低分辨率jiffy机制，某些CPU平台还包含有一个软件可读取的高分辨率计数器。







 二）、使用jiffies计数器

  1、jiffies计数器和额度去计数器的工具包含在<linux/jiffies.h>中，但是通常只需包括<linux/sched.h>文件，后者会自动放入jiffies.h。

    1）、jiffies和jiffies_64均应被看成只读变量。

    2）、在代码需要记录jiffies的当前值时。可简单访问unsigned  long变量。该变量被声明为volatile，可避免编译器对该变量的语句的优化。




   2、比较缓存值和当前值，应该使用下面的宏：

        #include<linux/jiffies.h>

        int  time_after(unsigned  long  a,  unisgned  long  b);

        int  time_before(unsigned  long  a,  unisgned  long  b);

        int  time_after_eq(unsigned  long  a,  unisgned  long  b);

        int  time_before_eq(unsigned  long  a,  unisgned  long  b);

        I、如果a所代表的时间比b靠后，则第一个宏返回真。

        II、如果a比b靠前，则第二个宏返回真。

        III、后面两个宏分别用来比较“靠前或者相等”及“靠后或者相等”。

        IV、这些宏会将计算器值转换成signed  long，相减，然后比较结果。




  3、有时需要来自用户空间的时间表述方法（使用struct  timeval和struct  timespec）和内核表述方法进行转换。这两个结构使用两个数来表示精确时间。

    1）、在老的、流行的struct  timeval中使用秒和毫秒值，而较新的struct  timespce中使用秒和纳秒，前者比后者出现得早，但更常用。

    2）、为了完成jiffies值和这些结构间的转换，内核提供了下面四个辅助函数：

          #include<linux/time.h>

          unsigned  long  timespec_to_jiffies(struct  timespec  *value);

          void  jiffies_to_timespec(unisgned  long  jiffies,  struct  timespec  *value);

          unsigend  long  timeval_to_jiffies(struct  timespec  *value);

          void  jiffies_to_timeval(unisgned  long  jiffies,  struct  timespec  *value);




  4、对64位jiffies_64的访问不像对jiffies的访问那样直接。在64位计算机结构上，这俩个变量其实是同一个；但在32位处理器上，对64位的访问不是原子的。

    1）、如果必须读取64位计数器，则应该使用内核导出的一个特殊辅助函数，该函数为我们完成适当的锁定：

         #include<linux/jiffies.h>

         u64  get_jiffies_64(void);




  5、需要注意，实际的时钟频率对用户控件来讲几乎是完全不可见的。对用户来讲，如果想知道定时器中断的准确HZ值，只能通过/proc/interrupts获得。







 三）、处理器特定的寄存器

  1、指令时序本质上是不可预测的，这样依赖于指令周期的经验型性能描述方法就不再适用。




  2、为了解决这一问题，CPU制造商引入了一种通过计算时钟周期来度量时间差的渐变而可靠的方法。绝大多数现代处理器都包含一个随时钟周期不断递增的寄存器计数器。

    1）、基于不同的平台，在用户空间，这个寄存器可能是可读的，也可能是不可读的。可能是可写的，也可能是不可写的。可能是可32位的，也可能是不可64位的。

    2）、在某些平台上，该寄存器可能根本不存在，或者由硬件设计者通过外部设备来实现。

    3）、无论该寄存器是否可以值0，都强烈建议不要重置它，即使硬件允许这么做。




  3、时间戳计数器（TSC）

    1）、包含头文件<asm/msr.h>之后，就可以使用如下的宏：

        rdtsc(low32,hige32);

        rdtscl(low32);

        rdtscll(var64);

    2）、在大多数TSC应用中，读取计数器底半部分就够了。

    3）、在内核头文件中还有一个于体系结构无关的函数可以代替rdtsc：

        #include(linux/times.h)

       cycles_t  get_cycles(void);




  4、对TSC，在SMP系统中，它们不会在多个处理器间保持同步，为了确保获得一致的值，我们需要为查询该计数器的代码禁止抢占。










二、获取当前时间

  1、内核一般通过jiffies值来获取当前时间。利用jiffies值来测量时间间隔在大多数情况下已经足够，如果还需要测量更短的时间差，就只能使用处理器特定的寄存器了。




  2、内核提供了将墙上时间转换为jiffies值得函数：

     #include<linux/time.h>

     unsigned  long  mktime(unsigned  int  yer,  unsigned  int  mon,

                                                  unsigend  int  day,  unsigend  int  hour,

                                                  unsigend  int  min,  unisgned  int  sec);




  3、处理绝对时间戳用do_gettimeofday函数，该函数用秒或微妙值来填充一个指向struct  timeval的指针变量：

     #incldue<linux/time.h>

     void  do_gettimeofday(struct  tmeval  *tv);




  4、当前时间值也可以通过xtime变量类型来获得，但精度差些。但并不鼓励使用该变量。内核提供了一个辅助函数：

     #include<linux/time.h>

     struct  timespec  current_kernel_time(void);







三、延迟执行

 一）、长延时

  1、忙等待

    1）、如果想把执行延迟若干个时钟滴答，或者对延迟的精度要求不高，最简单的实现方法就是监视一个jiffies计数器循环。这种忙等待实现方式通常具有下main的形式，其

              中j1是延时终止时的jiffies值：

              while(time_before(jiffies,  j1))

                   cpu_relax();

         I、对cpu_relax的调用将以架构相关的方式执行，其中不执行大量的处理器代码。




  2、让出处理器

    1）、在不需要CPU时主动释放CPU，这可以通过调用schedule函数实现：

         while(time_before(jiffies,  j1)) {

                  schedule();

         }；




  3、超时

    1）、如果驱动程序使用等待队列来等待其他一些事件，而我们同时希望在特定时间段中运行，可使用两个函数：

          #include<linux/wait.h>

          long  wait_event_timeout(wait_queue_head_t  q,  condition,   long  timeout);

          long  wait_event_interruptible(wait_queue_head_t  q,  condition,   long  timeout);

    2）、进程始终会在超时到其时被唤醒。为了适应这种特殊情况，内核为我们提供了schedule_timeout函数，这样，就可以避免声明和使用多余的等待队列头：

          #include<linux/sched.h>


          signed  long  schedule_timeout(signed  long  timeout);







 二）、短延时

  1、ndelay、udelay和mdelay这几个函数可以很好的完后曾短延时任务：

      #include<linux/delay.h>

      void  ndelay(unsigned  long  nsecs);

      void  udelay(unsigned  long  nsecs);

      void  mdelay(unsigned  long  nsecs);

    1）、为了避免循环计算中的整数溢出，udelay和ndelay为传递给他们的值强加了上限。

    2）、这三个延迟函数是忙等待函数，因而在延迟过程中无法运行其他任务。




  2、实现毫秒级延迟还有另外一个办法，这种办法不涉及忙等待。

    void  msleep(unsigned  int  millisecs);

    unsigned  long  mseep_interruptible(unsigned  int  millisecs);

    void  ssleep(unsigned  int seconds);










四、内核定时器

 一）、相关简介

  1、如果我们希望在将来的某个时间点调度执行某个工作，同时在该时间点到达之前不会堵塞当前进程，则可以使用内核定时器。

    1）、内恶化定时器可用来在未来的某个特定时间点调度执行某个函数，从而可用于完成许多任务。

    2）、一个内核定时器是一个数据结构，它告诉内核在用户定义的时间点使用用户定义的参数来执行一个用户定义的函数。被调度运行的函数几乎肯定不会注册这些函数的进

              程正在执行时运行。相反，这些函数会异步的运行。

    3）、定时器函数必须以原子方式运行，但是这种非进程上下文还带来其他一些问题。




  2、许多动作需要在进程上下文中运行。如果处于进程上下文之外，则必须遵守如下规则：

    1）、不允许访问用户空间。

    2）、current指针在原子模式下是没有任何意义的，也是不可用的。

    3）、不能执行休眠或调度。




  3、内核代码可以通过调用函数in_interrupt()来判断自己是否正处于中断上下文，该函数无需参数，如果处于中断上下文就返回非零值。和in_interrupt()相关的函数是

        in_atomic()。




  4、内核定时器的特性：

    1）、任务可以将自己注册以在稍后的时间重新运行。

    2）、在SMP系统中，定时器会由注册它的同一CPU执行。

    3）关于定时器：即使在单处理器上，定时器也会是竞态的潜在来源。







 二）、定时器API

  1、内核为驱动程序提供了一组用来声明、注册和删除内核定时器的函数：

      #include<linux/timer.h>

      struct  timer_list {

         /*  .  .  .  */

         unsigned  long  expires;

         void  (*function)(unsigned  long);

         unisgned  long  data;

       };

       void  init_timer(struct  timer_list  *timer);

       struct  timer_list  TIMER_INITIALIZER(_funciotn,  _expires,  _data);

       void  add_timer(struct  timer_list  *timer);

       int  del_timer(struct  timer_list  *timer);




  2、其他内核定时器函数

      int  mod_timer(struct  timer_list  *timer,  unsigned  long  expires);

      int  del_timer_sync(struyct  timer_list  *timer);

      int  timer_pending(const  struct  timer_list  *timer);







 三）、内核定时器的实现

  1、内核定时器的实现需要满足以下需求及假定：

    1）、定时器的管理必须尽可能做到轻量级。

    2）、其设计必须在活动定时器大量增加时具有良好的伸缩性。

    3）、大部分定时器会在最多几秒或者几分钟内到期，而很少存在很长期延时的定时器。

    4）、定时器应该在注册它们的同一CPU上运行。




  2、内核开发者使用的解决办法是利用per-CPU数据结构。timer_list结构的base字段包含了指向该结构的指针。




  3、不管何时内核代码注册了一个定时器，其操作最终会由internal_add_timer执行，该函数又会将新的定时器添加到和当前CPU关联的“级联表”中。




  4、但需要谨记的是，内核定时器离完美还有很大距离。







 四）、tasklet

  1、tasklet在很多方面类似内核定时器：它们始终在中断期间运行，始终会在调度它们的同一CPU上运行，而且都接收一个unsigned  long参数。和内核定时器一样，tasklet也

        会在“软件中断”上下文以原子模式执行。

        和内核定时器不同的是：我们不能要求tasklet在某个给定时间执行。调度一个tasklet，表明我们只希望内核选择某个其后的时间来执行给定的函数。




  2、tasklet以数据结构的形式存在，并在使用前必须初始化。调用特定的函数或者使用特定的宏来声明该结构，即可完成tasklet初始化：

      #include<linux/interrupt.h>

      struct  tasklet_struct {

                void  (*func)(unsigned  long);

                unsigned  long  data;

       };

       void  tasklet_init(struct  tasklet_struct  *t,  void  (*func)(unsigned  long),  unsigned  long  data);

       DECLARE_TASKLET(name,  func,  data);

       DECLARE_TASKLET_DISABLED(name,  func,  data);




  3、tasklet的特性




  4、tasklet相关内核接口

     void  tasklet_disable(struct  tasklet_struct  *t);

     void  tasklet_disable_nosync(struct  tasklet_struct  *t);

     void  tasklet_enable(struct  tasklet_struct  *t);

     void  tasklet_schedule(struct  tasklet_struct  *t);

     void  tasklet_hi_schedule(struct  tasklet_struct  *t);

     void  tasklet_kill(struct  tasklet_struct  *t);







五、工作队列

 一）、内核接口函数


  1、tasklet和工作队列的区别

    1）、两者的关键区别在于：tasklet会在很短的时间段内很快执行，并且以原子模式执行。而工作队列函数可具有更长的延时并且不必原子化。




  2、工作队列有struct  workqueue_struct的类型。在使用之前，必须显式地创建一个工作队列，可使用下面两个函数之一：

     struct  workqueue_struct  *create_workqueue(const  char  *name);

     struct  workqueue_struct  *create_singlethread_workqueue(const  char  *name);




  3、要向一个工作队列提交一个任务，需要填充一个work_struct结构，可通过以下宏完成：

      DECLARE_WORK（name,  void  (*function)(void  *),  void  *data);




  4、如果要在运行时构造work_struct结构，可使用下面两个宏：

      INIT_WORK（struct  work_struct  *work,  void  (*function)(void  *),  void  *data);

      PREPARE_WORK(struct  work_struct  *work,  void  (*function)(void  *),  void  *data);




  5、如果要将工作提交到工作队列，可使用下列函数：

     int  queue_work(struct  work_struct  *queue, struct  work_struct  *work);

     int  queue_delayed_work(struct  work_struct  *queue, struct  work_struct  *work,  unsigned  long  delay);




  6、取消某个挂起的工作队列入口项：

     int  cancel_delayed_work(struct  work_struct  *work);

    1）、为了绝对确保在cancel_delayed_work返回0之后，工作函数不会再系统中的任何地方运行，则应该随后调用下面的函数：

      void  flush_workqueue(struct  workqueue_struct  &queue);




  7、在结束对工作队列的使用之后，可调用下面的函数释放相关资源：

    void  destroy_workqueue(struct  workqueue_struct  *queue);







 二）、共享队列

  1、在许多情况下，设备驱动程序不需要有自己的工作队列。如果我们只是偶尔需要向队列中提交任务，则一种更简单，更有效的办法是使用内核提供的共享的默认工作队列。




  2、如果我们使用共享队列，则意味着我们不应该长期独占该队列，既不能长时间休眠，而且我们的任务可能需要更长的时间才能获得处理器时间。




