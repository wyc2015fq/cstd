# LDD3源码分析之时间与延迟操作 - 一世豁然的专栏 - CSDN博客





2014年12月15日 21:37:35[一世豁然](https://me.csdn.net/Explorer_day)阅读数：633








本博客转载于：http://blog.csdn.net/liuhaoyutz/article/details/7412931[作者：**刘昊昱**]







一、jit.c文件分析

jit.c程序是一个综合性的演示程序，涉及操作时间和延迟的各种技术。为了使程序代码最少，jit使用动态的/proc文件方式。

按照惯例，我们从模块初始化函数jit_init开始看：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 263int __init jit_init(void)  
- 264{  
- 265    create_proc_read_entry("currentime", 0, NULL, jit_currentime, NULL);  
- 266    create_proc_read_entry("jitbusy", 0, NULL, jit_fn, (void *)JIT_BUSY);  
- 267    create_proc_read_entry("jitsched",0, NULL, jit_fn, (void *)JIT_SCHED);  
- 268    create_proc_read_entry("jitqueue",0, NULL, jit_fn, (void *)JIT_QUEUE);  
- 269    create_proc_read_entry("jitschedto", 0, NULL, jit_fn, (void *)JIT_SCHEDTO);  
- 270  
- 271    create_proc_read_entry("jitimer", 0, NULL, jit_timer, NULL);  
- 272    create_proc_read_entry("jitasklet", 0, NULL, jit_tasklet, NULL);  
- 273    create_proc_read_entry("jitasklethi", 0, NULL, jit_tasklet, (void *)1);  
- 274  
- 275    return 0; /* success */
- 276}  

jit_init共调用了8次create_proc_read_entry函数，我们在《LDD3源码分析之调试技术》一文中分析过这个函数，该函数用来创建一个只读的/proc入口项，其函数原型如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- struct proc_dir_entry *create_proc_read_entry(constchar *name,   
-                                               mode_t mode,  
- struct proc_dir_entry *base,  
-                                               read_proc_t *read_proc,  
- void *data)  


name是要创建的文件名称；

mode是该文件的保护掩码(传入0表示使用系统默认值)；

base指定该文件所在的目录(如果base为NULL，则该文件将创建在/proc根目录下)；

read_proc是实现该文件的读操作的函数；

data是传递给read_proc的参数。

这里我们重点看read_proc函数。为了创建一个只读的/proc文件，驱动程序必须实现一个函数，用于在读取文件时生成数据，这个函数称为read_proc。当某个进程读取这个/proc文件时(使用read系统调用)，就会调用相应驱动程序的read_proc函数。

read_proc函数的原型如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- int (*read_proc)(char *page, char **start, off_t off, int count, int *eof, void *data)  


这个函数的参数比较难理解，这里我偷下懒，直接把文档Linux Kernel Procfs Guide上对这个函数的解释复制过来，大家自己看。另外，LDD3中文版，翻译上有许多不对的地方，建议大家看英文版的描述。



The read function is a call back function that allows userland processes to read data

from the kernel. 

The read function should write its information into the *page*. For proper use, the function should start writing at an offset of*off*in*page*and write at most*count*bytes, but because most read functions are quite
 simple and only return a small amount of information, these two parameters are usually ignored (it breaks pagers like more and less, but cat still works).

If the *off *and *count *parameters are properly used,*eof*should be used to signal that the end of the file has been reached by writing 1 to the memory location*eof*points to.

The parameter *start *doesn’t seem to be used anywhere in the kernel. The*data*

parameter can be used to create a single call back function for several files.

The read_func function must return the number of bytes written into the*page*.

理解了create_proc_read_entry和read_proc函数，我们就可以来看jit_init函数都完成了哪些工作了。8次调用create_proc_read_entry函数，也就是说，jit_init创建了8个/proc入口项，文件名、read_proc函数以及传递给read_proc函数的参数的如下：

/proc/currentime      jit_currentime        NULL

/proc/jitbusy             jit_fn                      JIT_BUSY

/proc/jitsched           jit_fn                      JIT_SCHED

/proc/jitqueue          jit_fn                       JIT_QUEUE

/proc/jitschedto       jit_fn                        JIT_SCHEDTO

/proc/jitimer             jit_timer                   NULL

/proc/jitasklet          jit_tasklet                  NULL

/proc/jitasklethi       jit_tasklet                  1

创建了这些模块入口项之后，jit模块的初始化工作就完成了。下面我们按照LDD3讲的顺序，依次看每个/proc入口项完成什么工作。

首先来看/proc/currentime，它对应的read_proc函数是jit_currentime，其代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

-  91/*
-  92 * This file, on the other hand, returns the current time forever
-  93 */
-  94int jit_currentime(char *buf, char **start, off_t offset,  
-  95                   int len, int *eof, void *data)  
-  96{  
-  97    struct timeval tv1;  
-  98    struct timespec tv2;  
-  99    unsigned long j1;  
- 100    u64 j2;  
- 101  
- 102    /* get them four */
- 103    j1 = jiffies;  
- 104    j2 = get_jiffies_64();  
- 105    do_gettimeofday(&tv1);  
- 106    tv2 = current_kernel_time();  
- 107  
- 108    /* print */
- 109    len=0;  
- 110    len += sprintf(buf,"0x%08lx 0x%016Lx %10i.%06i\n"
- 111               "%40i.%09i\n",  
- 112               j1, j2,  
- 113               (int) tv1.tv_sec, (int) tv1.tv_usec,  
- 114               (int) tv2.tv_sec, (int) tv2.tv_nsec);  
- 115    *start = buf;  
- 116    return len;  
- 117}  


jit_currentime函数以多种形式返回当前时间。

97 - 98行，定义了一个timeval类型的变量tv1和一个timespec类型的变量tv2，这两个结构体都定义在linux/time.h中：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- struct timeval {  
- time_t      tv_sec;     /* seconds */
-     suseconds_t tv_usec;    /* microseconds */
- };  
- 
- struct timespec {  
- time_t  tv_sec;     /* seconds */
- long    tv_nsec;    /* nanoseconds */
- };  


用户空间表述时间就是使用这两个结构体，timeval使用秒和毫秒，timespec使用秒和纳秒，timeval出现的更早，也更常用。

103行，将当前jiffies值保存在j1中，注意，j1是unsigned long类型变量。

104行，调用get_jiffies_64函数，取得64位的jiffies值。

105行，调用do_gettimeofday函数，把当前jiffies值转换为timeval类型保存在tv1中。

106行，调用current_kernel_time函数取得当前jiffies值对应的timespec类型，保存在tv2中。

110 - 114行，将4种形式的时间值保存在buf指向的内存页中。

由上面的分析可见，当读/proc/currentime文件时，会打印4种形式的时间值。在我的机器上测试现象如下图所示：

![](https://img-my.csdn.net/uploads/201203/30/1333111243_1806.png)

接下来我们看jit_fn函数，/proc/jitbusy、/proc/jitsched、/proc/jitqueue、/proc/jitschedto四个文件的read_proc函数都是jit_fn，所不同的是，读这四个文件时，会分别传递给jit_fn四个不同的参数JIT_BUSY、JIT_SCHED、JIT_QUEUE、JIT_SCHEDTO。函数代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 44/* use these as data pointers, to implement four files in one function */
- 45enum jit_files {  
- 46    JIT_BUSY,  
- 47    JIT_SCHED,  
- 48    JIT_QUEUE,  
- 49    JIT_SCHEDTO  
- 50};  
- 51  
- 52/*
- 53 * This function prints one line of data, after sleeping one second.
- 54 * It can sleep in different ways, according to the data pointer
- 55 */
- 56int jit_fn(char *buf, char **start, off_t offset,  
- 57          int len, int *eof, void *data)  
- 58{  
- 59    unsigned long j0, j1; /* jiffies */
- 60    wait_queue_head_t wait;  
- 61  
- 62    init_waitqueue_head (&wait);  
- 63    j0 = jiffies;  
- 64    j1 = j0 + delay;  
- 65  
- 66    switch((long)data) {  
- 67        case JIT_BUSY:  
- 68            while (time_before(jiffies, j1))  
- 69                cpu_relax();  
- 70            break;  
- 71        case JIT_SCHED:  
- 72            while (time_before(jiffies, j1)) {  
- 73                schedule();  
- 74            }  
- 75            break;  
- 76        case JIT_QUEUE:  
- 77            wait_event_interruptible_timeout(wait, 0, delay);  
- 78            break;  
- 79        case JIT_SCHEDTO:  
- 80            set_current_state(TASK_INTERRUPTIBLE);  
- 81            schedule_timeout (delay);  
- 82            break;  
- 83    }  
- 84    j1 = jiffies; /* actual value after we delayed */
- 85  
- 86    len = sprintf(buf, "%9li %9li\n", j0, j1);  
- 87    *start = buf;  
- 88    return len;  
- 89}  


44 - 50行，定义了JIT_BUSY、JIT_SCHED、JIT_QUEUE、JIT_SCHEDTO四个枚举值，分别对应0，1，2，3。

59行，定义了两个unsigned long类型变量j0, j1，用来保存jiffies值。

60 - 62行，定义并初始化了等待队列头wait。

63行，将j0设置为当前jiffies值。

64行，将j1设置为j0 + delay。delay默认值在37行定义为HZ，也可以在命令行指定。

67 - 70行，如果传递给jit_fn的参数是JIT_BUSY，也就是说用户在读/proc/jitbusy文件，此时采用忙等待的方式延迟delay长度的时钟滴答。time_before(jiffies,
 j1)宏在jiffies小于j1的情况下为真。cpu_relax函数不做任何事情。

因为内核头文件中将jiffies声明为volatile型变量，所以每次C代码访问它时都会重新读取它，因此该循环可以直到延迟的作用。尽管也是正确的实现，但这个忙等待会严重降低系统性能。如果我们没有将内核配置为抢占型的，那么这个循环将在延迟期间独占处理器，在j1所代表的时间到达之前，系统看起来就像死掉了一样。而如果我们将内核配置为可抢占型的，则问题不会那么严重，这是因为除非代码拥有一个锁，否则处理器的时间还可以用作他用。但是在抢占式系统中，忙等待仍然有些浪费。

更糟糕的是，如果在进入while循环之前，禁止了中断，jiffies值就得不到更新，那么while循环的判定条件就永远为真了。

下图是测试/proc/jitbusy的过程:

![](https://img-my.csdn.net/uploads/201203/30/1333111414_2212.png)

上图中,第一次执行”dd bs=20 count=5 < /proc/jitbusy”时,是在系统负载比较小的情况下。第二次执行”dd
 bs=20 count=5 < /proc/jitbusy”时，在另外一个终端下执行了LDD3提供的load50程序，该程序用来增加系统负载。

从上图中的输出内容可以看出，当系统负载较小时，每次读操作延迟恰好是1秒(1000个jiffies)，而下一次读操作会在前一个结束后立即执行。但是，当系统负载较大时，每个读操作的延迟恰好是1秒，但是下一次读操作可能会5秒后才会执行。

我们回到jit_fn函数继续往下看。

71 - 75行，如果传递给jit_fn的参数是JIT_SCHED，也就是说用户在读/proc/jitsched文件，此时如果time_before(jiffies,
 j1)宏为真，则调用schedule函数让出处理器。

虽然这种延迟技术在等待时放弃了CPU，但是性能仍然不理想。当前进程虽然放弃了CPU，但是它仍然在运行队列中，如果系统中只有一个可运行进程，则该进程又会立即运行，而空闲任务(进程号为0，称为swapper)从来不会运行。在系统空闲时运行swapper可以减轻处理器负载，延长处理器寿命，降低电量消耗。

下图是测试/proc/jitsched的过程：

![](https://img-my.csdn.net/uploads/201203/30/1333111472_5783.png)

上图中,第一次执行”dd bs=20 count=5 < /proc/jitsched”时,是在系统负载比较小的情况下。第二次执行”dd
 bs=20 count=5 < /proc/jitsched”时，在另外一个终端下执行了load50程序增加系统负载。可以看出，在系统负载较大时，读操作的延迟比所请求的延迟时间长几个时钟滴答，系统越忙，这个问题越突出。当一个进程使用schedule释放处理器后，不能保证在进程需要时很快就能得到处理器。

因此，除了影响系统整体性能外，使用schedule延迟的方法对驱动程序需求来讲并不安全。

我们再回到jit_fn函数，76 - 78行，如果传递给jit_fn的参数是JIT_QUEUE，也就是说用户在读/proc/jitqueue文件，则调用wait_event_interruptible_timeout(wait,
 0, delay)函数在等待队列wait上休眠，但是会在指定的等待时间到期时返回。在到期之前，如果进程收到信号也会退出休眠返回。

下图是测试/proc/jitqueue的过程：

![](https://img-my.csdn.net/uploads/201203/30/1333111560_8825.png)

从上图可以看出，即使是在高负载的系统上，对/proc/jitqueue的读取也将得到接近优化的结果。

回到jit_fn函数，现在看79 - 82行，如果传递给jit_fn的参数是JIT_SCHEDTO，也就是说用户在读/proc/schedto文件，则在80行调用set_current_state将进程状态设置为TASK_INTERRUPTIBLE，然后在81行调用schedule_timeout
 (delay)，schedule_timeout函数让出处理器，并等待delay指定的时间。实际上wait_event_interruptible_timeout函数在内部就是利用了schedule_timeout函数。

使用schedule_timeout方式延迟，性能与使用wait_event_interruptible_timeout相似，但是可以避免声明和使用多余的等待队列。

下图是测试/proc/schedto文件的过程：

![](https://img-my.csdn.net/uploads/201203/30/1333111601_2926.png)

从上图打印的信息可以看出，即使是在高负载的系统上，对/proc/schedto的读取也将得到接近优化的结果。

到此，jit_fn函数我们就分析完了。下面我们来看jit_timer函数，它对应/proc/jitimer其代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 155/* the /proc function: allocate everything to allow concurrency */
- 156int jit_timer(char *buf, char **start, off_t offset,  
- 157          int len, int *eof, void *unused_data)  
- 158{  
- 159    struct jit_data *data;  
- 160    char *buf2 = buf;  
- 161    unsigned long j = jiffies;  
- 162  
- 163    data = kmalloc(sizeof(*data), GFP_KERNEL);  
- 164    if (!data)  
- 165        return -ENOMEM;  
- 166  
- 167    init_timer(&data->timer);  
- 168    init_waitqueue_head (&data->wait);  
- 169  
- 170    /* write the first lines in the buffer */
- 171    buf2 += sprintf(buf2, "   time   delta  inirq    pid   cpu command\n");  
- 172    buf2 += sprintf(buf2, "%9li  %3li     %i    %6i   %i   %s\n",  
- 173            j, 0L, in_interrupt() ? 1 : 0,  
- 174            current->pid, smp_processor_id(), current->comm);  
- 175  
- 176    /* fill the data for our timer function */
- 177    data->prevjiffies = j;  
- 178    data->buf = buf2;  
- 179    data->loops = JIT_ASYNC_LOOPS;  
- 180  
- 181    /* register the timer */
- 182    data->timer.data = (unsigned long)data;  
- 183    data->timer.function = jit_timer_fn;  
- 184    data->timer.expires = j + tdelay; /* parameter */
- 185    add_timer(&data->timer);  
- 186  
- 187    /* wait for the buffer to fill */
- 188    wait_event_interruptible(data->wait, !data->loops);  
- 189    if (signal_pending(current))  
- 190        return -ERESTARTSYS;  
- 191    buf2 = data->buf;  
- 192    kfree(data);  
- 193    *eof = 1;  
- 194    return buf2 - buf;  
- 195}  


159行，定义了jit_data结构体指针变量data，jit_data结构体定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 126/* This data structure used as "data" for the timer and tasklet functions */
- 127struct jit_data {  
- 128    struct timer_list timer;  
- 129    struct tasklet_struct tlet;  
- 130    int hi; /* tasklet or tasklet_hi */
- 131    wait_queue_head_t wait;  
- 132    unsigned long prevjiffies;  
- 133    unsigned char *buf;  
- 134    int loops;  
- 135};  


167行，调用init_timer初始化定时器data->timer。

168行，调用init_waitqueue_head初始化等待队列头data->wait。

171行，向buf2中写入标题头，同时更新buf2的值。

172行，向buf2中写入相关数据，同时更新buf2的值。

177 - 179行，初始化data->prevjiffies为当前jiffies值，data->buf指向buf2，data->loops设置为
 JIT_ASYNC_LOOPS，JIT_ASYNC_LOOPS在136行定义为5。

181 - 185行，设置和注册定时器dev->timer。定时器函数为jit_timer_fn，传递给该函数的参数为data变量，定时器等待时间为j+tdelay，最后调用add_timer将定时器注册到系统中，此时，即启动了定时器。

188行，在data->wait上休眠等待，注意唤醒条件是data->loops变为0。

193行，将*eof设置为1，表示读/proc/jitimer文件结束。

下面该分析定时器函数jit_timer_fn了，其代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 138void jit_timer_fn(unsigned long arg)  
- 139{  
- 140    struct jit_data *data = (struct jit_data *)arg;  
- 141    unsigned long j = jiffies;  
- 142    data->buf += sprintf(data->buf, "%9li  %3li     %i    %6i   %i   %s\n",  
- 143                 j, j - data->prevjiffies, in_interrupt() ? 1 : 0,  
- 144                 current->pid, smp_processor_id(), current->comm);  
- 145  
- 146    if (--data->loops) {  
- 147        data->timer.expires += tdelay;  
- 148        data->prevjiffies = j;  
- 149        add_timer(&data->timer);  
- 150    } else {  
- 151        wake_up_interruptible(&data->wait);  
- 152    }  
- 153}  


在定时器函数jit_timer_fn中，首先向data->buf中写入相关数据，然后判断data->loops的值是否为0，如果不为0，重新设置相关参数并再次启动定时器；如果data->loops的值变为0，则唤醒等待队列data->wait上的进程。

测试/proc/jitimer的过程如下图所示：

![](https://img-my.csdn.net/uploads/201203/30/1333111983_9797.png)

下面我们来看jit_tasklet函数，该函数对应的文件是/proc/ jitasklet和/proc/ jitasklethi，代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 216/* the /proc function: allocate everything to allow concurrency */
- 217int jit_tasklet(char *buf, char **start, off_t offset,  
- 218          int len, int *eof, void *arg)  
- 219{  
- 220    struct jit_data *data;  
- 221    char *buf2 = buf;  
- 222    unsigned long j = jiffies;  
- 223    long hi = (long)arg;  
- 224  
- 225    data = kmalloc(sizeof(*data), GFP_KERNEL);  
- 226    if (!data)  
- 227        return -ENOMEM;  
- 228  
- 229    init_waitqueue_head (&data->wait);  
- 230  
- 231    /* write the first lines in the buffer */
- 232    buf2 += sprintf(buf2, "   time   delta  inirq    pid   cpu command\n");  
- 233    buf2 += sprintf(buf2, "%9li  %3li     %i    %6i   %i   %s\n",  
- 234            j, 0L, in_interrupt() ? 1 : 0,  
- 235            current->pid, smp_processor_id(), current->comm);  
- 236  
- 237    /* fill the data for our tasklet function */
- 238    data->prevjiffies = j;  
- 239    data->buf = buf2;  
- 240    data->loops = JIT_ASYNC_LOOPS;  
- 241  
- 242    /* register the tasklet */
- 243    tasklet_init(&data->tlet, jit_tasklet_fn, (unsigned long)data);  
- 244    data->hi = hi;  
- 245    if (hi)  
- 246        tasklet_hi_schedule(&data->tlet);  
- 247    else
- 248        tasklet_schedule(&data->tlet);  
- 249  
- 250    /* wait for the buffer to fill */
- 251    wait_event_interruptible(data->wait, !data->loops);  
- 252  
- 253    if (signal_pending(current))  
- 254        return -ERESTARTSYS;  
- 255    buf2 = data->buf;  
- 256    kfree(data);  
- 257    *eof = 1;  
- 258    return buf2 - buf;  
- 259}  


jit_tasklet的实现与前面分析的jit_timer很类似，相同的地方这里我们不仔细分析了，这里重点看一下两个函数不同的地方。

243行，调用tasklet_init初始化data->tlet，指定tasklet函数为jit_tasklet_fn，交将data作为参数传递给jit_tasklet_fn。

245 - 246行，如果hi为1，则使用tasklet_hi_schedule函数以高优先级调度data->tlet执行。当软件中断处理程序运行时，它会在处理其他软件中断(包括普通的tasklet)之前，处理高优先级的tasklet。

247 - 248行，如果hi为0，则使用tasklet_schedule调度data->tlet执行。

251行，在data->wait上休眠等待，注意唤醒条件是data->loops变为0。

257行，将*eof设置为1，表示读/proc/ jitasklet或/proc/
 jitasklethi文件结束。

下面看tasklet函数jit_tasklet_fn：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 197void jit_tasklet_fn(unsigned long arg)  
- 198{  
- 199    struct jit_data *data = (struct jit_data *)arg;  
- 200    unsigned long j = jiffies;  
- 201    data->buf += sprintf(data->buf, "%9li  %3li     %i    %6i   %i   %s\n",  
- 202                 j, j - data->prevjiffies, in_interrupt() ? 1 : 0,  
- 203                 current->pid, smp_processor_id(), current->comm);  
- 204  
- 205    if (--data->loops) {  
- 206        data->prevjiffies = j;  
- 207        if (data->hi)  
- 208            tasklet_hi_schedule(&data->tlet);  
- 209        else
- 210            tasklet_schedule(&data->tlet);  
- 211    } else {  
- 212        wake_up_interruptible(&data->wait);  
- 213    }  
- 214}  


该函数的功能与前面介绍的定时器函数jit_timer_fn非常像，只不过把调度定时器的操作换成了调度tasklet的函数。

下图是测试/proc/ jitasklet或/proc/ jitasklethi的过程：

![](https://img-my.csdn.net/uploads/201203/30/1333112121_5974.png)

至此，jit.c文件我们就全部分析完了。



二、jiq.c文件分析

LDD3第7章涉及的代码还有一个jiq.c文件，这个文件主要演示了等待队列的用法。下面是jiq模块初始化函数jiq_init的代码：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 240static int jiq_init(void)  
- 241{  
- 242  
- 243    /* this line is in jiq_init() */
- 244    INIT_WORK(&jiq_work, jiq_print_wq, &jiq_data);  
- 245  
- 246    create_proc_read_entry("jiqwq", 0, NULL, jiq_read_wq, NULL);  
- 247    create_proc_read_entry("jiqwqdelay", 0, NULL, jiq_read_wq_delayed, NULL);  
- 248    create_proc_read_entry("jitimer", 0, NULL, jiq_read_run_timer, NULL);  
- 249    create_proc_read_entry("jiqtasklet", 0, NULL, jiq_read_tasklet, NULL);  
- 250  
- 251    return 0; /* succeed */
- 252}  


244行，初始化一个工作jiq_work，其工作函数是jiq_print_wq，jiq_data是传递给该函数的参数。jiq_work是work_struct类型的变量，struct
 work_struct在linux/workqueue.h文件中定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- struct work_struct {  
-     atomic_long_t data;  
- #define WORK_STRUCT_PENDING 0       /* T if work item pending execution */
- #define WORK_STRUCT_FLAG_MASK (3UL)
- #define WORK_STRUCT_WQ_DATA_MASK (~WORK_STRUCT_FLAG_MASK)
- struct list_head entry;  
-     work_func_t func;  
- #ifdef CONFIG_LOCKDEP
- struct lockdep_map lockdep_map;  
- #endif
- };  


jiq_data是一个全局变量，其定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 63static struct clientdata {  
-  64    int len;  
-  65    char *buf;  
-  66    unsigned long jiffies;  
-  67    long delay;  
-  68} jiq_data;  


246 - 249行，创建4个/proc只读接口，它们分别是/proc/ jiqwq、/proc/
 jiqwqdelay、/proc/ jitimer、/proc/ jiqtasklet，这4个文件对应的read_proc函数分别是jiq_read_wq、jiq_read_wq_delayed、jiq_read_run_timer、jiq_read_tasklet。

首先来看jiq_read_wq函数的实现：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 129static int jiq_read_wq(char *buf, char **start, off_t offset,  
- 130                   int len, int *eof, void *data)  
- 131{  
- 132    DEFINE_WAIT(wait);  
- 133  
- 134    jiq_data.len = 0;                /* nothing printed, yet */
- 135    jiq_data.buf = buf;              /* print in this place */
- 136    jiq_data.jiffies = jiffies;      /* initial time */
- 137    jiq_data.delay = 0;  
- 138  
- 139    prepare_to_wait(&jiq_wait, &wait, TASK_INTERRUPTIBLE);  
- 140    schedule_work(&jiq_work);  
- 141    schedule();  
- 142    finish_wait(&jiq_wait, &wait);  
- 143  
- 144    *eof = 1;  
- 145    return jiq_data.len;  
- 146}  


jiq_read_wq函数中使用的是共享工作队列，而不是进程专用的工作队列，共享工作队列更节约系统资源。该函数的休眠采用的是手工休眠。

132行，使用DEFINE_WAIT宏定义一个等待队列入口wait。

134 - 137行，对jiq_data进行初始化。

139行，调用prepare_to_wait函数将wait加入到等待队列jiq_wait中，并把进程状态设置为TASK_INTERRUPTIBLE。

140行，调用schedule_work将jiq_work提交到系统提供的共享工作队列中。

141行，调用schedule函数，让出处理器，此后，进程就在等待队列jiq_wait中休眠。

142行，休眠被唤醒后，调用finish_wait做一些清理工作。

144行，将*eof设置为1，表明已经读到/proc/jiqwq的结束处。

把工作jiq_work提交给共享工作队列后，在此后的某一时间，工作函数jiq_print_wq就会执行，该函数代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 111/*
- 112 * Call jiq_print from a work queue
- 113 */
- 114static void jiq_print_wq(void *ptr)  
- 115{  
- 116    struct clientdata *data = (struct clientdata *) ptr;  
- 117  
- 118    if (! jiq_print (ptr))  
- 119        return;  
- 120  
- 121    if (data->delay)  
- 122        schedule_delayed_work(&jiq_work, data->delay);  
- 123    else
- 124        schedule_work(&jiq_work);  
- 125}  


工作函数jiq_print_wq的核心代码在jiq_print函数中，如果该函数返回0，则退出。如果返回非0值，说明还需要再次调用该工作。如果jiq_print函数返回非0值，根据data->delay的值是否为0，分别采用schedule_delayed_work和schedule_work，以延迟和非延迟的方式重新调度工作。

jiq_print函数定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 78/*
-  79 * Do the printing; return non-zero if the task should be rescheduled.
-  80 */
-  81static int jiq_print(void *ptr)  
-  82{  
-  83    struct clientdata *data = ptr;  
-  84    int len = data->len;  
-  85    char *buf = data->buf;  
-  86    unsigned long j = jiffies;  
-  87  
-  88    if (len > LIMIT) {  
-  89        wake_up_interruptible(&jiq_wait);  
-  90        return 0;  
-  91    }  
-  92  
-  93    if (len == 0)  
-  94        len = sprintf(buf,"    time  delta preempt   pid cpu command\n");  
-  95    else
-  96        len =0;  
-  97  
-  98    /* intr_count is only exported since 1.3.5, but 1.99.4 is needed anyways */
-  99    len += sprintf(buf+len, "%9li  %4li     %3i %5i %3i %s\n",  
- 100            j, j - data->jiffies,  
- 101            preempt_count(), current->pid, smp_processor_id(),  
- 102            current->comm);  
- 103  
- 104    data->len += len;  
- 105    data->buf += len;  
- 106    data->jiffies = j;  
- 107    return 1;  
- 108}  


88 - 91行，如果打印的信息已经超过了LIMIT()的限制，则唤醒等待队列jiq_wait上的进程，并返回0，表示不需要再次调度该工作。

93 - 102行，打印相关信息。

104 - 107行，更新jiq_data的信息。

108行，返回1，表明还需要再次调度该工作。

下图是对/proc/jiqwq的测试过程：

![](https://img-my.csdn.net/uploads/201203/30/1333112356_2005.png)

下面我们来看jiq_read_wq_delayed函数，其代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 149static int jiq_read_wq_delayed(char *buf, char **start, off_t offset,  
- 150                   int len, int *eof, void *data)  
- 151{  
- 152    DEFINE_WAIT(wait);  
- 153  
- 154    jiq_data.len = 0;                /* nothing printed, yet */
- 155    jiq_data.buf = buf;              /* print in this place */
- 156    jiq_data.jiffies = jiffies;      /* initial time */
- 157    jiq_data.delay = delay;  
- 158  
- 159    prepare_to_wait(&jiq_wait, &wait, TASK_INTERRUPTIBLE);  
- 160    schedule_delayed_work(&jiq_work, delay);  
- 161    schedule();  
- 162    finish_wait(&jiq_wait, &wait);  
- 163  
- 164    *eof = 1;  
- 165    return jiq_data.len;  
- 166}  


这个函数与前面分析的jiq_read_wq非常相似，区别是这里采用延迟方式调度工作。这里我只看两个函数不同的地方，如果有不明白的，参考jiq_read_wq理解。

157行，因为要采用延迟方式，设置jiq_data.delay的值为delay，其默认值为1，可以通过命令行传递参数修改。

160行，schedule_delayed_work以延迟模式将工作提交到工作队列。

把工作jiq_work提交给共享工作队列后，在此后的某一时间，工作函数jiq_print_wq就会执行，该函数我们前面已经分析过，/proc/jiqwqdelay与/proc/jiqwq的区别是因为指定了jiq_data.delay的值为非0值，所以121行，如果需要再次调度工作，还是用延迟模式调度。

下图是测试/proc/jiqwqdelay的过程：

![](https://img-my.csdn.net/uploads/201203/30/1333112426_4662.png)

下面我们来看jiq_read_run_timer函数，这个函数使用的是定时器，其代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 212static int jiq_read_run_timer(char *buf, char **start, off_t offset,  
- 213                   int len, int *eof, void *data)  
- 214{  
- 215  
- 216    jiq_data.len = 0;           /* prepare the argument for jiq_print() */
- 217    jiq_data.buf = buf;  
- 218    jiq_data.jiffies = jiffies;  
- 219  
- 220    init_timer(&jiq_timer);              /* init the timer structure */
- 221    jiq_timer.function = jiq_timedout;  
- 222    jiq_timer.data = (unsigned long)&jiq_data;  
- 223    jiq_timer.expires = jiffies + HZ; /* one second */
- 224  
- 225    jiq_print(&jiq_data);   /* print and go to sleep */
- 226    add_timer(&jiq_timer);  
- 227    interruptible_sleep_on(&jiq_wait);  /* RACE */
- 228    del_timer_sync(&jiq_timer);  /* in case a signal woke us up */
- 229  
- 230    *eof = 1;  
- 231    return jiq_data.len;  
- 232}  


有了前面分析的基础，我们看这个函数应该比较轻松，因为大部分代码和前面分析过的代码是一样的。

216 - 218行，初始化jiq_data。

220 - 223行，初始化定时器jiq_timer，定时器函数是jiq_timedout。

225行，调用jiq_print打印标题栏和一行信息。

226行，启动定时器。

227行，进程在jiq_wait上休眠等待。

定时器函数是jiq_timedout的代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 205static void jiq_timedout(unsigned long ptr)  
- 206{  
- 207    jiq_print((void *)ptr);            /* print a line */
- 208    wake_up_interruptible(&jiq_wait);  /* awake the process */
- 209}  


该定时器函数只执行一次，207行打印一行信息，208行唤醒等待进程。

下图是测试/proc/jitimer的过程：

![](https://img-my.csdn.net/uploads/201203/30/1333112566_2621.png)

下面我们看jiq_read_tasklet函数，这个函数使用的是tasklet，其代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 182static int jiq_read_tasklet(char *buf, char **start, off_t offset, int len,  
- 183                int *eof, void *data)  
- 184{  
- 185    jiq_data.len = 0;                /* nothing printed, yet */
- 186    jiq_data.buf = buf;              /* print in this place */
- 187    jiq_data.jiffies = jiffies;      /* initial time */
- 188  
- 189    tasklet_schedule(&jiq_tasklet);  
- 190    interruptible_sleep_on(&jiq_wait);    /* sleep till completion */
- 191  
- 192    *eof = 1;  
- 193    return jiq_data.len;  
- 194}  


189行，调度jiq_tasklet。

190行，进程在jiq_wait上休眠。

jiq_tasklet在第75行定义：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 75static DECLARE_TASKLET(jiq_tasklet, jiq_print_tasklet, (unsigned long)&jiq_data);  


tasklet函数是jiq_print_tasklet，其定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 171/*
- 172 * Call jiq_print from a tasklet
- 173 */
- 174static void jiq_print_tasklet(unsigned long ptr)  
- 175{  
- 176    if (jiq_print ((void *) ptr))  
- 177        tasklet_schedule (&jiq_tasklet);  
- 178}  


调用jiq_print打印信息，如果jiq_print返回非0值，重新调度task_let。

测试/proc/jiqtasklet的过程如下图所示：

![](https://img-my.csdn.net/uploads/201203/30/1333112694_2642.png)



三、编译jiq模块时出现的问题

在编译jiq模块过程中，出现如下图所示错误：

![](https://img-my.csdn.net/uploads/201203/30/1333112809_3354.png)

这是因为linux/config.h在2.6.32-38-generic-pae内核中不存在了，解决方法是直接把jiq的第18行删除即可。再次编译，出现如下错误：

![](https://img-my.csdn.net/uploads/201203/30/1333112938_4192.png)

这是因为在2.6.32-38-generic-pae内核中，INIT_WORK宏的定义发生了变化，只能接受两个参数，但是LDD3传递了3个参数。解决办法是把243行



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 243    INIT_WORK(&jiq_work, jiq_print_wq, &jiq_data);  


替换为



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 243    INIT_WORK(&jiq_work, jiq_print_wq);  


这样替换完成后，还有几个连锁反应需要处理。因为LDD3中的本意是把jiq_data作为参数传递给jiq_print_wq函数。现在新的INIT_WORK中无法传递参数了，所以我们需要修改jiq_print_wq函数，手动把jiq_data传递过去。修改后的jiq_print_wq代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 113static void jiq_print_wq(struct work_struct *work)  
- 114{  
- 115    struct clientdata *data = (struct clientdata *) &jiq_data;  
- 116  
- 117    if (! jiq_print (&jiq_data))  
- 118        return;  
- 119  
- 120    if (data->delay)  
- 121        schedule_delayed_work(&jiq_work, data->delay);  
- 122    else
- 123        schedule_work(&jiq_work);  
- 124}  


与原来相比，共修改了3个地方。

113行，参数类型改为work_struck结构指针，这是新内核定义的参数类型。

115行，直接使用全局变量jiq_data，原来是通过参数传递的。

117行，直接使用全局变量jiq_data，原来是通过参数传递的。

修改后，再编译，能编译通过，但仍有问题，如下图所示：

![](https://img-my.csdn.net/uploads/201203/30/1333113121_5658.png)

这是因为，在2.6.32-38-generic-pae内核中，schedule_delayed_work函数的定义发生了变化，现在其函数原型是



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- int schedule_delayed_work(struct delayed_work *dwork, unsigned long delay)  


而在LDD3使用的2.6.10版本的内核中，其函数原型是：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- int fastcall schedule_delayed_work(struct work_struct *work, unsigned long delay)  


所以要在新内核上执行schedule_delayed_work，原来的work_struct必须改为delayed_work。

delayed_work结构体定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- struct delayed_work {  
- struct work_struct work;  
- struct timer_list timer;  
- };  


为了使修改的代码最少，我做了如下修改：

1、在第56行定义了一个全局delayed_work结构体变量 jiq_delayed_work;



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 56static struct delayed_work jiq_delayed_work;  


2、重新定义jiq_read_wq_delayed函数如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 148static int jiq_read_wq_delayed(char *buf, char **start, off_t offset,  
- 149                   int len, int *eof, void *data)  
- 150{  
- 151    DEFINE_WAIT(wait);  
- 152    INIT_DELAYED_WORK(&jiq_delayed_work, jiq_print_wq);  
- 153  
- 154    jiq_data.len = 0;                /* nothing printed, yet */
- 155    jiq_data.buf = buf;              /* print in this place */
- 156    jiq_data.jiffies = jiffies;      /* initial time */
- 157    jiq_data.delay = delay;  
- 158  
- 159    prepare_to_wait(&jiq_wait, &wait, TASK_INTERRUPTIBLE);  
- 160    schedule_delayed_work(&jiq_delayed_work, delay);  
- 161    schedule();  
- 162    finish_wait(&jiq_wait, &wait);  
- 163  
- 164    *eof = 1;  
- 165    return jiq_data.len;  
- 166}  


与原来的相比，增加了152行初始化jiq_delayed_work。修改了160行，调度jiq_delayed_work。

3、修改工作函数jiq_print_wq。



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7412931#)

- 113static void jiq_print_wq(struct work_struct *work)  
- 114{  
- 115    struct clientdata *data = (struct clientdata *) &jiq_data;  
- 116  
- 117    if (! jiq_print (&jiq_data))  
- 118        return;  
- 119  
- 120    if (data->delay)  
- 121        schedule_delayed_work(&jiq_delayed_work, data->delay);  
- 122    else
- 123        schedule_work(&jiq_work);  
- 124}  


与原来相比，修改了121行，调度jiq_delayed_work。

修改完成后，编译通过，如下图所示：

![](https://img-my.csdn.net/uploads/201203/30/1333113302_6639.png)




