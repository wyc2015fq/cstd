# LDD3源码分析之poll分析 - 一世豁然的专栏 - CSDN博客





2014年12月17日 17:28:25[一世豁然](https://me.csdn.net/Explorer_day)阅读数：666








本博客转载于：[http://blog.csdn.net/liuhaoyutz/article/details/7400037](http://blog.csdn.net/liuhaoyutz/article/details/7400037)  [作者：刘昊昱]






本文分析LDD3第六章介绍的scullpipe设备是如何实现阻塞I/O的。另外，我发现scullpipe的实现代码有一个问题，在文章的最后，对这个问题进行了说明，并给出了修正代码。



一、scullpipe设备实现阻塞I/O分析

scuoopipe设备被实现为具有类似管道特性，用以演示如何实现阻塞型和非阻塞型I/O。实际的设备驱动中，这通常通过硬件中断的方式实现：当等待事件发生时，硬件发出一个中断，然后在中断处理程序中，驱动程序会唤醒等待进程。作为一个演示程序，scullpipe没有利用中断，而是选择使用另一个进程来产生数据并唤醒读取进程，类似的，利用读取进程来唤醒等待缓冲区可用的写入进程。

scullpipe的主体实现在examples/scull/pipe.c中，但是也利用了examples/scull/main.c中的一些代码。我们分析scullpipe的起点在main.c中，首先看main.c中的如下代码：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- 648        /* Initialize each device. */
- 649    for (i = 0; i < scull_nr_devs; i++) {  
- 650        scull_devices[i].quantum = scull_quantum;  
- 651        scull_devices[i].qset = scull_qset;  
- 652        init_MUTEX(&scull_devices[i].sem);  
- 653        scull_setup_cdev(&scull_devices[i], i);  
- 654    }  
- 655  
- 656        /* At this point call the init function for any friend device */
- 657    dev = MKDEV(scull_major, scull_minor + scull_nr_devs);  
- 658    dev += scull_p_init(dev);  
- 659    dev += scull_access_init(dev);  


649 - 654行，我们在分析第三章中的scull设备时已经分析过了，用for循环初始化scull0 - scull3，但当时657
 - 659行我们跳过没有分析。

现在来看657行，scull_major和scull_minor的默认值都是0，scull_nr_devs的默认值是4。dev变量在618行定义，它是dev_t类型，注意dev_t类型用来保存设备编号，包括主设备号和次设备号。所以，657行，我们通过MKDEV宏得到一个主设备号为0，次设备号为4的设备编号保存在dev中。这里之所以让次设备号为4，是因为前面已经注册了scull0
 - scull3，它们的主设备号均为系统分配值，次设备号分别是0，1，2，3。虽然我们还没有看后面的代码，但我们由此可以推测出作者的意图，要创建scullpipe0
 - scullpipe3，其主设备号均为系统分配值，次设备号分别为4，5，6，7。

658行是我们要分析的重点。而659行以后才会涉及，在本文中不分析659行。注意，658行，把657行生成的设备编号dev做为参数传递给scull_p_init函数，现在看这个函数的实现，在examples/scull/pipe.c文件中：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- 343/*
- 344 * Initialize the pipe devs; return how many we did.
- 345 */
- 346int scull_p_init(dev_t firstdev)  
- 347{  
- 348    int i, result;  
- 349  
- 350    result = register_chrdev_region(firstdev, scull_p_nr_devs, "scullp");  
- 351    if (result < 0) {  
- 352        printk(KERN_NOTICE "Unable to get scullp region, error %d\n", result);  
- 353        return 0;  
- 354    }  
- 355    scull_p_devno = firstdev;  
- 356    scull_p_devices = kmalloc(scull_p_nr_devs * sizeof(struct scull_pipe), GFP_KERNEL);  
- 357    if (scull_p_devices == NULL) {  
- 358        unregister_chrdev_region(firstdev, scull_p_nr_devs);  
- 359        return 0;  
- 360    }  
- 361    memset(scull_p_devices, 0, scull_p_nr_devs * sizeof(struct scull_pipe));  
- 362    for (i = 0; i < scull_p_nr_devs; i++) {  
- 363        init_waitqueue_head(&(scull_p_devices[i].inq));  
- 364        init_waitqueue_head(&(scull_p_devices[i].outq));  
- 365        init_MUTEX(&scull_p_devices[i].sem);  
- 366        scull_p_setup_cdev(scull_p_devices + i, i);  
- 367    }  
- 368#ifdef SCULL_DEBUG  
- 369    create_proc_read_entry("scullpipe", 0, NULL, scull_read_p_mem, NULL);  
- 370#endif  
- 371    return scull_p_nr_devs;  
- 372}  


350行，调用register_chrdev_region函数申请从firstdev开始的scull_p_nr_devs个设备编号，firstdev是scull_p_init函数的形参，所以firstdev是前面main.c中658行传的设备编号。而scull_p_nr_devs的默认值为4.

355行，scull_p_devno = firstdev; scull_p_devno用于记录第一个scullpipe设备的设备编号。

356行，为scullpipe0 - scullpipe3的设备结构体分配内存空间。scullpipe设备由scull_pipe结构体表示，其定义如下：



**[html]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- 33struct scull_pipe {  
- 34        wait_queue_head_t inq, outq;        /* read and write queues */  
- 35        char *buffer, *end;                 /* begin of buf, end of buf */  
- 36        int buffersize;                      /* used in pointer arithmetic */  
- 37        char *rp, *wp;                     /* where to read, where to write */  
- 38        int nreaders, nwriters;               /* number of openings for r/w */  
- 39        struct fasync_struct *async_queue;    /* asynchronous readers */  
- 40        struct semaphore sem;              /* mutual exclusion semaphore */  
- 41        struct cdev cdev;                   /* Char device structure */  
- 42};  


361行，将scull_pipe结构体数组的内容清0。

362 - 367行，初始化scullpipe设备对应的scull_pipe结构体。每次for循环初始化一个scullpipe设备。

363 - 364行，初始化两个等待队列头scull_p_devices[i].inq和scull_p_devices[i].outq，这两个等待队列头分别代表休眠在scullpipe设备上的等待读取和等待写入的进程。

365行初始化信号量scull_p_devices[i].sem。

366行，调用scull_p_setup_cdev函数完成进一步的初始化。下面看这个函数的实现：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- 326/*
- 327 * Set up a cdev entry.
- 328 */
- 329static void scull_p_setup_cdev(struct scull_pipe *dev, int index)  
- 330{  
- 331    int err, devno = scull_p_devno + index;  
- 332  
- 333    cdev_init(&dev->cdev, &scull_pipe_fops);  
- 334    dev->cdev.owner = THIS_MODULE;  
- 335    err = cdev_add (&dev->cdev, devno, 1);  
- 336    /* Fail gracefully if need be */
- 337    if (err)  
- 338        printk(KERN_NOTICE "Error %d adding scullpipe%d", err, index);  
- 339}  


333行，调用cdev_init函数初始化dev->cdev，指定设备操作函数集是scull_pipe_fops。

335行，调用cdev_add函数将dev->cdev注册到系统中, devno是对应的设备编号。

至此，scullpipe设备就初始化完毕并且注册到系统中。

再回到scull_p_init函数，368 - 370行，注册了scullpipe的/proc接口。因为本文主要分析阻塞型I/O,所以不讨论这个接口。但需要注意的是，scullpipe的/proc接口实现与scull的/proc接口实现不一样，主要是start参数相关的问题，有兴趣可以仔细研究一下。

371行，返回注册的scullpipe设备个数。

下面我们来看scullpipe的read函数，即scull_p_read函数，其代码如下所示：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- 118static ssize_t scull_p_read (struct file *filp, char __user *buf, size_t count,  
- 119                loff_t *f_pos)  
- 120{  
- 121    struct scull_pipe *dev = filp->private_data;  
- 122  
- 123    if (down_interruptible(&dev->sem))  
- 124        return -ERESTARTSYS;  
- 125  
- 126    while (dev->rp == dev->wp) { /* nothing to read */
- 127        up(&dev->sem); /* release the lock */
- 128        if (filp->f_flags & O_NONBLOCK)  
- 129            return -EAGAIN;  
- 130        PDEBUG("\"%s\" reading: going to sleep\n", current->comm);  
- 131        if (wait_event_interruptible(dev->inq, (dev->rp != dev->wp)))  
- 132            return -ERESTARTSYS; /* signal: tell the fs layer to handle it */
- 133        /* otherwise loop, but first reacquire the lock */
- 134        if (down_interruptible(&dev->sem))  
- 135            return -ERESTARTSYS;  
- 136    }  
- 137    /* ok, data is there, return something */
- 138    if (dev->wp > dev->rp)  
- 139        count = min(count, (size_t)(dev->wp - dev->rp));  
- 140    else/* the write pointer has wrapped, return data up to dev->end */
- 141        count = min(count, (size_t)(dev->end - dev->rp));  
- 142    if (copy_to_user(buf, dev->rp, count)) {  
- 143        up (&dev->sem);  
- 144        return -EFAULT;  
- 145    }  
- 146    dev->rp += count;  
- 147    if (dev->rp == dev->end)  
- 148        dev->rp = dev->buffer; /* wrapped */
- 149    up (&dev->sem);  
- 150  
- 151    /* finally, awake any writers and return */
- 152    wake_up_interruptible(&dev->outq);  
- 153    PDEBUG("\"%s\" did read %li bytes\n",current->comm, (long)count);  
- 154    return count;  
- 155}  


126行,当读指针dev->rp与写指针 dev->wp相等时，说明缓冲区中没有数据可读。这种情况下，要根据用户指定的标志位决定是进入休眠等待还是直接返回。

127行，进入休眠之前，要将已经获得的锁释放掉。

128 - 129行，如果用户空间的read函数指定了O_NONBLOCK标志，则不阻塞进程，直接退出。

131 - 132行，调用wait_event_interruptible函数休眠在dev->inq等待队列上，注意被唤醒的条件是(dev->rp != dev->wp)，即缓冲区中有数据可读。由这一句也可以看出，用户空间的read进程对应的等待队列是dev->inq。另外，wait_event_interruptible的返回值有两种，返回0表示缓冲区中有数据可读，被唤醒。返回非0值表示休眠被某个信号中断，这时，132行直接返回-ERESTARTSYS。

134 - 135行，休眠被唤醒后，首先重新获得互斥锁，然后返回到while循环开始处判断是否有数据可读。

138 - 141行，确认有数据可读后，退出while循环，根据读写指针的位置位及count值，决定要读多少数据。

142行，读数据到用户空间。

146 - 148行，更新读指针的位置。

149行，释放互斥锁。

152行，读取结束后，就有缓冲区空间可以进行写操作了。所以唤醒所有休眠在dev->outq上的写进程，唤醒函数是wake_up_interruptible。

154行，返回读取的字节数。

注意，scull_p_read在142行调用copy_to_user将数据拷贝到用户空间过程中，可能会休眠。此时，虽然进程拥有互斥锁，但这种情况下拥有互斥锁休眠是可以接受的，因为我们知道内核会把数据复制到用户空间并唤醒我们，同时不会试图锁上同一个信号量。

下面我们来看scullpipe的write函数，即scull_p_write函数，其代码如下所示：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- 188static ssize_t scull_p_write(struct file *filp, constchar __user *buf, size_t count,  
- 189                loff_t *f_pos)  
- 190{  
- 191    struct scull_pipe *dev = filp->private_data;  
- 192    int result;  
- 193  
- 194    if (down_interruptible(&dev->sem))  
- 195        return -ERESTARTSYS;  
- 196  
- 197    /* Make sure there's space to write */
- 198    result = scull_getwritespace(dev, filp);  
- 199    if (result)  
- 200        return result; /* scull_getwritespace called up(&dev->sem) */
- 201  
- 202    /* ok, space is there, accept something */
- 203    count = min(count, (size_t)spacefree(dev));  
- 204    if (dev->wp >= dev->rp)  
- 205        count = min(count, (size_t)(dev->end - dev->wp)); /* to end-of-buf */
- 206    else/* the write pointer has wrapped, fill up to rp-1 */
- 207        count = min(count, (size_t)(dev->rp - dev->wp - 1));  
- 208    PDEBUG("Going to accept %li bytes to %p from %p\n", (long)count, dev->wp, buf);  
- 209    if (copy_from_user(dev->wp, buf, count)) {  
- 210        up (&dev->sem);  
- 211        return -EFAULT;  
- 212    }  
- 213    dev->wp += count;  
- 214    if (dev->wp == dev->end)  
- 215        dev->wp = dev->buffer; /* wrapped */
- 216    up(&dev->sem);  
- 217  
- 218    /* finally, awake any reader */
- 219    wake_up_interruptible(&dev->inq);  /* blocked in read() and select() */
- 220  
- 221    /* and signal asynchronous readers, explained late in chapter 5 */
- 222    if (dev->async_queue)  
- 223        kill_fasync(&dev->async_queue, SIGIO, POLL_IN);  
- 224    PDEBUG("\"%s\" did write %li bytes\n",current->comm, (long)count);  
- 225    return count;  
- 226}  


与scull_p_read采用简单休眠不同，scull_p_write采用了手工休眠的方式。

194行，获得互斥锁。

198行，调用scull_getwritespace函数。下面看这个函数的定义：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- 157/* Wait for space for writing; caller must hold device semaphore.  On
- 158 * error the semaphore will be released before returning. */
- 159static int scull_getwritespace(struct scull_pipe *dev, struct file *filp)  
- 160{  
- 161    while (spacefree(dev) == 0) { /* full */
- 162        DEFINE_WAIT(wait);  
- 163  
- 164        up(&dev->sem);  
- 165        if (filp->f_flags & O_NONBLOCK)  
- 166            return -EAGAIN;  
- 167        PDEBUG("\"%s\" writing: going to sleep\n",current->comm);  
- 168        prepare_to_wait(&dev->outq, &wait, TASK_INTERRUPTIBLE);  
- 169        if (spacefree(dev) == 0)  
- 170            schedule();  
- 171        finish_wait(&dev->outq, &wait);  
- 172        if (signal_pending(current))  
- 173            return -ERESTARTSYS; /* signal: tell the fs layer to handle it */
- 174        if (down_interruptible(&dev->sem))  
- 175            return -ERESTARTSYS;  
- 176    }  
- 177    return 0;  
- 178}  


161行，使用spacefree函数判断是否有空间可写，该函数定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- 180/* How much space is free? */
- 181static int spacefree(struct scull_pipe *dev)  
- 182{  
- 183    if (dev->rp == dev->wp)  
- 184        return dev->buffersize - 1;  
- 185    return ((dev->rp + dev->buffersize - dev->wp) % dev->buffersize) - 1;  
- 186}  


如果spacefree函数返回值为0，说明现在scullpipe设备缓冲区已满，没有空间可以写入。写进程需要根据调用者设置的标志位决定是否休眠等待。

162行，使用DEFINE_WAIT函数创建并初始化一个等待队列入口。

164行，在进入休眠前，释放互斥锁。

165 - 166行，如果调用者设置了O_NONBLOCK标志位，则直接退出。

168行，调用prepare_to_wait(&dev->outq, &wait, TASK_INTERRUPTIBLE)函数，将等待队列入口wait插入到dev->outq等待队列中，并设置进程状态为TASK_INTERRUPTIBLE。

169 - 170行，再次调用spacefree(dev)判断是否有空间可写，如果还是确认没有空间，则调用170行的schedule()进行进程调度。这里需要注意的是，如果在169行的if判断和170行的调度之间，有了可写入空间，那么会发生什么情况？这种情况下没有任何问题，只要进程已经把自己放到了等待队列中并修改了进程状态，就不会有任何问题，schedule会把进程状态重新设置为TASK_RUNNING，并返回。

171行，调用finish_wait执行清理操作。

172 - 173行，判断是否是因为收到信号而醒来的，如果是返回-ERESTARTSYS。

174 - 176行，重新申请互斥锁，再次回到while开始处确认是否有空间写入。

177行，如果有空间可写入，返回0。

下面执行流程回到scull_p_write函数的198行，如果有可写入的空间，scull_getwritespace返回值为0，如果是因为收到信号而醒来的，scull_getwritespace返回非0值。

202 - 207行，计算可写入数据的大小。

209 - 212行，将数据从用户空间拷贝到内核空间。

213 - 215行，调整写入指针的位置。

216行，释放互斥锁。

219行，唤醒所有在读取等待队列上休眠的进程。

222 - 223行，是异步通知相关的操作，这里不讨论，后面在相关文章中再分析。

225行，返回写入的字节数。

scullpipe的测试需要打开两个终端窗口，安装好scullpipe后，在一个终端窗口上运行cat /dev/scullpipe0，因为缓冲区中没有可读内容，所以cat操作会休眠等待，如下图所示：

![](https://img-my.csdn.net/uploads/201203/26/1332745327_2182.png)

然后在另外一个终端窗口上执行echo “hello” > /dev/scullpipe0，将hello写入scullpipe0，可以看到cat进程会被唤醒，读出并打印”hello”。再次执行ls
 work/ >/dev/scullpipe0，会把work目录下的内容写入/dev/scullpipe0，同时，cat会被再次唤醒并打印输出work目录下的内容。

![](https://img-my.csdn.net/uploads/201203/26/1332745361_6993.png)



二、scullpipe源码中的一个问题

LDD3提供的scullpipe的代码有一个问题，不知道大家发现了没有，列出大家讨论一下。

首先看下图所示的执行步骤出现的问题：

![](https://img-my.csdn.net/uploads/201203/28/1332924401_7366.png)

上图中，先向/dev/scullpipe写入字符串”hello”，然后用cat命令读/dev/scullpipe，正常情况下应该能读出”hello”才对，但是从上图我们可以看出，cat被阻塞住了。原因是什么呢？我们先看scull_p_open函数的实现：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- 61static int scull_p_open(struct inode *inode, struct file *filp)  
- 62{  
- 63    struct scull_pipe *dev;  
- 64  
- 65    dev = container_of(inode->i_cdev, struct scull_pipe, cdev);  
- 66    filp->private_data = dev;  
- 67  
- 68    if (down_interruptible(&dev->sem))  
- 69        return -ERESTARTSYS;  
- 70    if (!dev->buffer) {  
- 71        /* allocate the buffer */
- 72        dev->buffer = kmalloc(scull_p_buffer, GFP_KERNEL);  
- 73        if (!dev->buffer) {  
- 74            up(&dev->sem);  
- 75            return -ENOMEM;  
- 76        }  
- 77    }  
- 78    dev->buffersize = scull_p_buffer;  
- 79    dev->end = dev->buffer + dev->buffersize;  
- 80    dev->rp = dev->wp = dev->buffer; /* rd and wr from the beginning */
- 81  
- 82    /* use f_mode,not  f_flags: it's cleaner (fs/open.c tells why) */
- 83    if (filp->f_mode & FMODE_READ)  
- 84        dev->nreaders++;  
- 85    if (filp->f_mode & FMODE_WRITE)  
- 86        dev->nwriters++;  
- 87    up(&dev->sem);  
- 88  
- 89    return nonseekable_open(inode, filp);  
- 90}  


70 - 80行，如果dev->buffer为NULL，则给dev->buffer分配内存空间，然后设置缓冲区大小和结束位置，以及读写指针位置。

关键在第80行，这行将读写指针位置dev->rp和dev->wp设置为同时指向缓冲区的开始处，而在scullpipe设备中，dev->rp等于dev->wp表明缓冲区中没有数据。

所以，问题的原因就找到了，我们向scullpipe中写入了字符串，但是执行cat /dev/scullpipe时，cat命令会调用open打开/dev/scullpipe文件，这时，就会将dev->rp和dev->wp设置为指向缓冲区的开始处，然后，cat读/dev/scullpipe的时候，就会认为该文件中没有数据可读，就会阻塞住。直到下次有数据写入时，cat才能被唤醒并读出数据。

另外，这个scull_p_open函数的逻辑有问题。在open时给dev->buffer分配内存，就是说，每次打开scullpipe设备，都会为它分配内存，这本来就不合理，尽管70行加了一个判断，只有当dev->buffer为空时，才分配内存。合理的位置应该是在初始化函数中完成这些操作。所以，我修改了scull_p_open函数和scull_p_setup_cdev函数，把对缓冲区的初始化工作放在了scull_p_setup_cdev函数中，该函数由初始化函数scull_p_init调用。列出这两个函数如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- staticint scull_p_open(struct inode *inode, struct file *filp)  
- {  
- struct scull_pipe *dev;  
- 
-         dev = container_of(inode->i_cdev, struct scull_pipe, cdev);  
-         filp->private_data = dev;  
- 
- if (down_interruptible(&dev->sem))  
- return -ERESTARTSYS;  
- 
- /* use f_mode,not  f_flags: it's cleaner (fs/open.c tells why) */
- if (filp->f_mode & FMODE_READ)  
-                 dev->nreaders++;  
- if (filp->f_mode & FMODE_WRITE)  
-                 dev->nwriters++;  
-         up(&dev->sem);  
- 
- return nonseekable_open(inode, filp);  
- }  







**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- /*
-  * Set up a cdev entry.
-  */
- staticvoid scull_p_setup_cdev(struct scull_pipe *dev, int index)  
- {  
- int err, devno;  
- 
- /* allocate the buffer */
-         dev->buffer = kmalloc(scull_p_buffer, GFP_KERNEL);  
- if (!dev->buffer)  
- return -ENOMEM;  
- 
-         dev->buffersize = scull_p_buffer;  
-         dev->end = dev->buffer + dev->buffersize;  
-         dev->rp = dev->wp = dev->buffer; /* rd and wr from the beginning */
- 
-         devno = scull_p_devno + index;  
-         cdev_init(&dev->cdev, &scull_pipe_fops);  
-         dev->cdev.owner = THIS_MODULE;  
-         err = cdev_add (&dev->cdev, devno, 1);  
- /* Fail gracefully if need be */
- if (err)  
-                 printk(KERN_NOTICE "Error %d adding scullpipe%d", err, index);  
- }  


由于把分配缓冲区的工作从scull_p_open函数移到了模块初始化函数中，连锁反应，还有一个函数需要修改，那就是scull_p_release，修改后的代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7395057#)

- staticint scull_p_release(struct inode *inode, struct file *filp)  
- {  
- struct scull_pipe *dev = filp->private_data;  
- 
- /* remove this filp from the asynchronously notified filp's */
-         scull_p_fasync(-1, filp, 0);  
-         down(&dev->sem);  
- if (filp->f_mode & FMODE_READ)  
-                 dev->nreaders--;  
- if (filp->f_mode & FMODE_WRITE)  
-                 dev->nwriters--;  
- //      if (dev->nreaders + dev->nwriters == 0) {
- //              kfree(dev->buffer);
- //              dev->buffer = NULL; /* the other fields are not checked on open */
- //      }
-         up(&dev->sem);  
- return 0;  
- }  


因为原来在scull_p_open中分配缓冲区，所以在scull_p_release中释放缓冲区，现在scull_p_open中不再分配缓冲区了，所以scull_p_release也就不再释放缓冲区了。所以必须屏蔽掉scull_p_release函数中释放缓冲区的4条语句。

用上面的列出的scull_p_open、scull_p_setup_cdev、scull_p_release函数替换原来的函数，上面的问题就解决了，执行结果如下图所示：

![](https://img-my.csdn.net/uploads/201203/28/1332933471_2343.png)




