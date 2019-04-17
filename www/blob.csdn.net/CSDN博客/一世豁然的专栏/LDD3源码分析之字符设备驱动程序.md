# LDD3源码分析之字符设备驱动程序 - 一世豁然的专栏 - CSDN博客





2014年12月16日 20:43:36[一世豁然](https://me.csdn.net/Explorer_day)阅读数：725








本博客转载于：[http://blog.csdn.net/liuhaoyutz/article/details/7383313](http://blog.csdn.net/liuhaoyutz/article/details/7383313)  [作者：刘昊昱]






本文是对LDD3第三章驱动代码的分析记录。LDD3的scull源码是一个综合性的驱动程序，内容涵盖了第三章到第六章的内容，初学者在学习过程中可能会有一定困难，这里仅分析LDD3第三章涉及的相关代码。在本文的最后，列出了在2.6.32内核上编译scull可能遇到的问题及解决方法。

本文要分析代码位于examples/scull/main.c中。

前面文章中我们曾经介绍过，分析Linux设备驱动程序，起点是找module_init指定的模块初始化函数。所以，我们打开examples/scull/main.c文件，从module_init开始分析。在该文件的672行即是对module_init宏的调用：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 672 module_init(scull_init_module);  
- 673 module_exit(scull_cleanup_module);  


可见，模块初始化函数是scull_init_module函数。我们找到这个函数的定义：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 615int scull_init_module(void)  
- 616{  
- 617    int result, i;  
- 618    dev_t dev = 0;  
- 619  
- 620/*
- 621 * Get a range of minor numbers to work with, asking for a dynamic
- 622 * major unless directed otherwise at load time.
- 623 */
- 624    if (scull_major) {  
- 625        dev = MKDEV(scull_major, scull_minor);  
- 626        result = register_chrdev_region(dev, scull_nr_devs, "scull");  
- 627    } else {  
- 628        result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs,  
- 629                "scull");  
- 630        scull_major = MAJOR(dev);  
- 631    }  
- 632    if (result < 0) {  
- 633        printk(KERN_WARNING "scull: can't get major %d\n", scull_major);  
- 634        return result;  
- 635    }  
- 636  
- 637        /*
- 638     * allocate the devices -- we can't have them static, as the number
- 639     * can be specified at load time
- 640     */
- 641    scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);  
- 642    if (!scull_devices) {  
- 643        result = -ENOMEM;  
- 644        goto fail;  /* Make this more graceful */
- 645    }  
- 646    memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev));  
- 647  
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
- 660  
- 661#ifdef SCULL_DEBUG /* only when debugging */
- 662    scull_create_proc();  
- 663#endif  
- 664  
- 665    return 0; /* succeed */
- 666  
- 667  fail:  
- 668    scull_cleanup_module();  
- 669    return result;  
- 670}  


第624到631行，申请分配设备编号，根据scull_major的值是否为0，分别采用静态分配设备编号(register_chrdev_region)或动态分配设备编号(alloc_chrdev_region)的方法。scull_major代表主设备号，它的值是怎么确定的呢？在main.c的41行有如下定义：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 41int scull_major =   SCULL_MAJOR;  
- 42int scull_minor =   0;  
- 43int scull_nr_devs = SCULL_NR_DEVS;  /* number of bare scull devices */
- 44int scull_quantum = SCULL_QUANTUM;  
- 45int scull_qset =    SCULL_QSET;  
- 46  
- 47module_param(scull_major, int, S_IRUGO);  
- 48module_param(scull_minor, int, S_IRUGO);  
- 49module_param(scull_nr_devs, int, S_IRUGO);  
- 50module_param(scull_quantum, int, S_IRUGO);  
- 51module_param(scull_qset, int, S_IRUGO);  


SCULL_MAJOR、SCULL_NR_DEVS、SCULL_QUANTUM、SCULL_QSET这四个宏在scull.h中有定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 44#define SCULL_MAJOR 0   /* dynamic major by default */
- 48#define SCULL_NR_DEVS 4    /* scull0 through scull3 */
- 65#define SCULL_QUANTUM 4000  
- 69#define SCULL_QSET    1000  


由此可见，scull驱动程序对于scull_major、scull_minor、scull_nr_devs、scull_quantum、scull_qset这几个变量的值采用了赋默认值，但同时也保留用户在命令行通过模块参数进行设置的可能。以scull_major为例，其默认值是0，在scull_init_module函数中，如果用户没有通过命令行参数给scull_major赋任意大于0的值，则会采用alloc_chrdev_region动态分配设备编号。如果用户给scull_major赋了一个大于0值，则采用register_chrdev_region静态申请设备编号。

我们继续看scull_init_module函数。下面看641行：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 641    scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev), GFP_KERNEL);  
- 642    if (!scull_devices) {  
- 643        result = -ENOMEM;  
- 644        goto fail;  /* Make this more graceful */
- 645    }  
- 646    memset(scull_devices, 0, scull_nr_devs * sizeof(struct scull_dev));  


给scull_nr_devs个scull设备分配内存空间，并将分配得到的内存清0。scull_nr_devs默认值为4，即默认创建4个scull设备(scull0
 - scull3)，每个scull设备由scull_dev结构体表示，其定义在scull.h文件中：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 87struct scull_dev {  
- 88    struct scull_qset *data;     /* Pointer to first quantum set */
- 89    int quantum;             /* the current quantum size */
- 90    int qset;                 /* the current array size */
- 91    unsigned long size;        /* amount of data stored here */
- 92    unsigned int access_key;   /* used by sculluid and scullpriv */
- 93    struct semaphore sem;    /* mutual exclusion semaphore */
- 94    struct cdev cdev;         /* Char device structure */
- 95};  


scull_dev结构体的每个成员，我们用到时再解释其含义。继续看scull_init_module函数。下面看649行：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 649    for (i = 0; i < scull_nr_devs; i++) {  
- 650        scull_devices[i].quantum = scull_quantum;  
- 651        scull_devices[i].qset = scull_qset;  
- 652        init_MUTEX(&scull_devices[i].sem);  
- 653        scull_setup_cdev(&scull_devices[i], i);  
- 654    }  


因为我们分配了scull_nr_devs个scull_dev结构体，即创建了scull_nr_devs个scull设备，所以这里用for循环对前面创建scull设备进行初始化，共循环scull_nr_devs次，每次循环完成对一个scull设备的初始化。

scull_devices[i].quantum代表scull设备当前“量子”大小，这里赋值为scull_quantum，其默认值为4000。scull_devices[i].qset代表当前数组大小，这里赋值为scull_qset，其默认值为1000。调用init_MUTEX对每个scull设备的sem成员进行初始化，这是一个互斥体，用于保证对scull设备的互斥访问，这是第5章的内容，这里不详细介绍。

在每次循环的最后，调用了scull_setup_cdev函数对相应scull设备进行设置。我们来看这个函数的完成了什么工作：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 598/*
- 599 * Set up the char_dev structure for this device.
- 600 */
- 601static void scull_setup_cdev(struct scull_dev *dev, int index)  
- 602{  
- 603    int err, devno = MKDEV(scull_major, scull_minor + index);  
- 604  
- 605    cdev_init(&dev->cdev, &scull_fops);  
- 606    dev->cdev.owner = THIS_MODULE;  
- 607    dev->cdev.ops = &scull_fops;  
- 608    err = cdev_add (&dev->cdev, devno, 1);  
- 609    /* Fail gracefully if need be */
- 610    if (err)  
- 611        printk(KERN_NOTICE "Error %d adding scull%d", err, index);  
- 612}  


scull_setup_cdev函数完成对scull设备的cdev成员变量(struct cdev类型)的初始化和注册。cdev结构体在内核中代表一个字符设备。

603行，调用MKDEV宏得到设备编号，注意，4个scull设备的主设备号都是一样的，但次设备号分别是0
 - 3。

605行，调用cdev_init函数对cdev结构体进行初始化，指定对应的文件操作函数集是scull_fops，这个scull_fops必须是前面已经定义实现好的。这个是我们后面介绍的重点。

606行指定所有者是THIS_MODULE。

607行是多余的，因为在605行已经指定了文件操作函数集是scull_fops。

608行，调用cdev_add函数将cdev结构体注册到内核，注册成功后，相应的scull设备就“活”了，其它程序就可以访问scull设备的资源。所以在注册之前，必须确保所有必要的初始化工作都完成了。

经过前面的步骤，scull设备初始化工作就完成了，并将相应的cdev结构体注册给内核。下面我们再次回到scull_init_module函数，现在代码执行到657行：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 657    dev = MKDEV(scull_major, scull_minor + scull_nr_devs);  
- 658    dev += scull_p_init(dev);  
- 659    dev += scull_access_init(dev);  
- 660  
- 661#ifdef SCULL_DEBUG /* only when debugging */
- 662    scull_create_proc();  
- 663#endif  
- 664  
- 665    return 0; /* succeed */
- 666  
- 667  fail:  
- 668    scull_cleanup_module();  
- 669    return result;  


657 - 664行是第4 - 6章相关的代码，这里不做讨论。所以，如果成功，scull_init_module函数就在665行返回0.

667 - 669行，如果前面的执行出现问题，清理已经分配的资源并返回错误信息。

现在scull设备的模块初始化函数就全部结束了。scull驱动程序已经被注册在内核中，该驱动程序不会主动做任何事情，而是等待响应用户程序的访问。我们应该知道，在用户空间，在用户程序看来，设备和其它普通文件一样，都是文件，而操作这些文件的接口就是文件操作函数集，比如open，read，write等等。

如果用户空间程序通过open，read，write等函数操作设备文件，驱动程序应该做什么样的响应呢？我们在注册scull设备的cdev时，指定了设备文件操作函数集



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 605    cdev_init(&dev->cdev, &scull_fops);  


而scull_fops就指定了如果用户空间程序执行open，read，write操作，应该调用什么函数进行响应。scull_fops定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 551struct file_operations scull_fops = {  
- 552    .owner =    THIS_MODULE,  
- 553    .llseek =   scull_llseek,  
- 554    .read =     scull_read,  
- 555    .write =    scull_write,  
- 556    .ioctl =    scull_ioctl,  
- 557    .open =     scull_open,  
- 558    .release =  scull_release,  
- 559};  


通过scull_fops，内核就知道了，如果用户空间程序调用 open操作打开scull相应设备，内核就会执行scull驱动程序的scull_open函数进行响应。其它函数依次类推。如果驱动程序没有定义对应某个用户空间操作的函数，内核就会执行默认动作响应。

在scull_fops结构体中，与第3章相关的是scull_open，scull_read，scull_write，scull_release的实现。下面我们依次来看这几个函数如何响应用户空间的对应操作。

在继续分析之前，我们先来看scull设备是怎么利用内存的，如下图所示

![](http://hi.csdn.net/attachment/201203/22/0_1332400842knuy.gif)

每个scull设备是一个指针链表，其中每个指针都指向一个scull_qset结构，scull_qset结构在scull.h中定义如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 82struct scull_qset {  
- 83    void **data;  
- 84    struct scull_qset *next;  
- 85};  


每个scull_qset通过一个中间指针数组最多可引用4000000个字节。因为scull.c中，默认情况下，每个指针数组有SCULL_QSET(1000)个指针，每个指针指向一个SCULL_QUANTUM(4000)字节大小的内存区域。每个内存区域称为一个量子(大小为4000字节)，而这个指针数组，称为量子集。

下面我们先来看scull_open函数：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 238int scull_open(struct inode *inode, struct file *filp)  
- 239{  
- 240    struct scull_dev *dev; /* device information */
- 241  
- 242    dev = container_of(inode->i_cdev, struct scull_dev, cdev);  
- 243    filp->private_data = dev; /* for other methods */
- 244  
- 245    /* now trim to 0 the length of the device if open was write-only */
- 246    if ( (filp->f_flags & O_ACCMODE) == O_WRONLY) {  
- 247        if (down_interruptible(&dev->sem))  
- 248            return -ERESTARTSYS;  
- 249        scull_trim(dev); /* ignore errors */
- 250        up(&dev->sem);  
- 251    }  
- 252    return 0;          /* success */
- 253}  


242行，调用container_of宏，通过cdev成员得到包含该cdev的scull_dev结构。

243行，将得到的scull_dev结构保存在filp->private_data中，因为open结束后，后面的read，write等操作使用同一个filp变量，它们即可以从filp->private_data中直接取出scull_dev结构体来使用。

246 - 251行，如果scull设备文件是以只写的方式打开，则要调用scull_trim将scull设备清空。247和250行是进行加锁解锁操作，进行互斥。这里不讨论。

下面看scull_trim函数是怎样清空scull设备的：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 59/*
- 60 * Empty out the scull device; must be called with the device
- 61 * semaphore held.
- 62 */
- 63int scull_trim(struct scull_dev *dev)  
- 64{  
- 65    struct scull_qset *next, *dptr;  
- 66    int qset = dev->qset;   /* "dev" is not-null */
- 67    int i;  
- 68  
- 69    for (dptr = dev->data; dptr; dptr = next) { /* all the list items */
- 70        if (dptr->data) {  
- 71            for (i = 0; i < qset; i++)  
- 72                kfree(dptr->data[i]);  
- 73            kfree(dptr->data);  
- 74            dptr->data = NULL;  
- 75        }  
- 76        next = dptr->next;  
- 77        kfree(dptr);  
- 78    }  
- 79    dev->size = 0;  
- 80    dev->quantum = scull_quantum;  
- 81    dev->qset = scull_qset;  
- 82    dev->data = NULL;  
- 83    return 0;  
- 84}  


66行，qset = dev->qset;  dev->qset的值为1000，即量子集的大小。

69行，dptr = dev->data;  dev->data指向第一个量子集scull_qset。所以这个for循环每次循环处理一个scull_qset。

71行，这个for循环循环1000次，因为每个量子集有1000个量子。

72行，每次kfree释放一个量子的内存空间。

73行，释放量子集数组占用的内存空间。

74行，将指针重新初始化为NULL。防止野指针。

76行，next指向下一个量子集。

77行，释放scull_qset占用的内存空间。

79 - 82行，恢复初始状态。

下面我们看scull_release函数的实现：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 255int scull_release(struct inode *inode, struct file *filp)  
- 256{  
- 257    return 0;  
- 258}  


这个函数直接返回0。因为scull设备是内存设备，关闭设备时也没有什么需要特别清理的。所以这个函数比较简单。

下面我们看scull_read函数的实现：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 292ssize_t scull_read(struct file *filp, char __user *buf, size_t count,  
- 293                loff_t *f_pos)  
- 294{  
- 295    struct scull_dev *dev = filp->private_data;  
- 296    struct scull_qset *dptr;    /* the first listitem */
- 297    int quantum = dev->quantum, qset = dev->qset;  
- 298    int itemsize = quantum * qset; /* how many bytes in the listitem */
- 299    int item, s_pos, q_pos, rest;  
- 300    ssize_t retval = 0;  
- 301  
- 302    if (down_interruptible(&dev->sem))  
- 303        return -ERESTARTSYS;  
- 304    if (*f_pos >= dev->size)  
- 305        goto out;  
- 306    if (*f_pos + count > dev->size)  
- 307        count = dev->size - *f_pos;  
- 308  
- 309    /* find listitem, qset index, and offset in the quantum */
- 310    item = (long)*f_pos / itemsize;  
- 311    rest = (long)*f_pos % itemsize;  
- 312    s_pos = rest / quantum; q_pos = rest % quantum;  
- 313  
- 314    /* follow the list up to the right position (defined elsewhere) */
- 315    dptr = scull_follow(dev, item);  
- 316  
- 317    if (dptr == NULL || !dptr->data || ! dptr->data[s_pos])  
- 318        goto out; /* don't fill holes */
- 319  
- 320    /* read only up to the end of this quantum */
- 321    if (count > quantum - q_pos)  
- 322        count = quantum - q_pos;  
- 323  
- 324    if (copy_to_user(buf, dptr->data[s_pos] + q_pos, count)) {  
- 325        retval = -EFAULT;  
- 326        goto out;  
- 327    }  
- 328    *f_pos += count;  
- 329    retval = count;  
- 330  
- 331  out:  
- 332    up(&dev->sem);  
- 333    return retval;  
- 334}  


295 - 300行，进行必要的初始化。

302行，获得互斥锁。

310行，item = (long)*f_pos / itemsize; item代表要读的数据起始点在哪个scull_qset中。

311行，rest = (long)*f_pos % itemsize; rest是一个中间值。在312行使用。

312行，s_pos = rest / quantum; q_pos = rest % quantum; s_pos代表要读的数据起始点在哪个量子中。q_pos代表要读的数据的起始点在量子的具体哪个位置。

315行，dptr = scull_follow(dev, item);调用scull_follow函数，这个函数的第二个参数代表要读的数据在哪个scull_qset中，该函数的作用是返回item指定的scull_qset。如果scull_qset不存在，还要分配内存空间，创建指定的scull_qset。下面看scull_follow函数的实现：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 262struct scull_qset *scull_follow(struct scull_dev *dev, int n)  
- 263{  
- 264    struct scull_qset *qs = dev->data;  
- 265  
- 266        /* Allocate first qset explicitly if need be */
- 267    if (! qs) {  
- 268        qs = dev->data = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);  
- 269        if (qs == NULL)  
- 270            return NULL;  /* Never mind */
- 271        memset(qs, 0, sizeof(struct scull_qset));  
- 272    }  
- 273  
- 274    /* Then follow the list */
- 275    while (n--) {  
- 276        if (!qs->next) {  
- 277            qs->next = kmalloc(sizeof(struct scull_qset), GFP_KERNEL);  
- 278            if (qs->next == NULL)  
- 279                return NULL;  /* Never mind */
- 280            memset(qs->next, 0, sizeof(struct scull_qset));  
- 281        }  
- 282        qs = qs->next;  
- 283        continue;  
- 284    }  
- 285    return qs;  
- 286}  


scull_follow函数的作用是返回第二个参数指定的scull_qset。如果该scull_qset不存在，分配内存空间创建该scull_qset。

我们再返回到scull_read函数中，代码现在运行到了317行：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 317    if (dptr == NULL || !dptr->data || ! dptr->data[s_pos])  
- 318        goto out; /* don't fill holes */
- <span style="font-family:Calibri;font-size:16px;"></span>  


如果指定的scull_qset不存在，或者量子指针数组不存在，或者量子不存在，都退出。

321 - 322行，设置scull_read一次最多只能读一个量子。



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 320    /* read only up to the end of this quantum */
- 321    if (count > quantum - q_pos)  
- 322        count = quantum - q_pos;  
- <span style="font-family:Calibri;font-size:16px;"></span>  


324行，调用copy_to_user(buf, dptr->data[s_pos] + q_pos, count)函数，将数据拷贝到用户空间。

328行，*f_pos += count;读取完成后，新的文件指针位置向前移动count个字节。

329 行，retval = count;返回读取到的字节数，即count。

最后，我们来分析scull_write函数：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7383313#)

- 336ssize_t scull_write(struct file *filp, constchar __user *buf, size_t count,  
- 337                loff_t *f_pos)  
- 338{  
- 339    struct scull_dev *dev = filp->private_data;  
- 340    struct scull_qset *dptr;  
- 341    int quantum = dev->quantum, qset = dev->qset;  
- 342    int itemsize = quantum * qset;  
- 343    int item, s_pos, q_pos, rest;  
- 344    ssize_t retval = -ENOMEM; /* value used in "goto out" statements */
- 345  
- 346    if (down_interruptible(&dev->sem))  
- 347        return -ERESTARTSYS;  
- 348  
- 349    /* find listitem, qset index and offset in the quantum */
- 350    item = (long)*f_pos / itemsize;  
- 351    rest = (long)*f_pos % itemsize;  
- 352    s_pos = rest / quantum; q_pos = rest % quantum;  
- 353  
- 354    /* follow the list up to the right position */
- 355    dptr = scull_follow(dev, item);  
- 356    if (dptr == NULL)  
- 357        goto out;  
- 358    if (!dptr->data) {  
- 359        dptr->data = kmalloc(qset * sizeof(char *), GFP_KERNEL);  
- 360        if (!dptr->data)  
- 361            goto out;  
- 362        memset(dptr->data, 0, qset * sizeof(char *));  
- 363    }  
- 364    if (!dptr->data[s_pos]) {  
- 365        dptr->data[s_pos] = kmalloc(quantum, GFP_KERNEL);  
- 366        if (!dptr->data[s_pos])  
- 367            goto out;  
- 368    }  
- 369    /* write only up to the end of this quantum */
- 370    if (count > quantum - q_pos)  
- 371        count = quantum - q_pos;  
- 372  
- 373    if (copy_from_user(dptr->data[s_pos]+q_pos, buf, count)) {  
- 374        retval = -EFAULT;  
- 375        goto out;  
- 376    }  
- 377    *f_pos += count;  
- 378    retval = count;  
- 379  
- 380        /* update the size */
- 381    if (dev->size < *f_pos)  
- 382        dev->size = *f_pos;  
- 383  
- 384  out:  
- 385    up(&dev->sem);  
- 386    return retval;  
- 387}  


339 - 344行，完成必要的初始化工作。

346行，取得互斥锁。

350行，item = (long)*f_pos / itemsize; item代表要写入的位置在哪个scull_qset中。

351行，rest = (long)*f_pos % itemsize; rest中一个中间值，在352行用到它。

352行，s_pos = rest / quantum; q_pos = rest % quantum; s_pos代表要写入的位置在哪个量子中，q_pos代表要写入的位置在量子的具体位置。

355行，dptr = scull_follow(dev, item);调用scull_follow函数，返回item指定的scull_qset。scull_follow函数在前面分析scull_read函数时已经分析过了。

358 - 363行，如果指定的量子指针数组不存在，则分配内存空间，创建量子指针数组。

364 - 368行，如果指定量子不存在，则分配内存空间，创建量子。

370 - 371行，限定一次最多只能写满一个量子。

373行，调用copy_from_user，将用户数据写到量子中。

377行，将文件指针后移count字节。

378行，设置返回值为count，即写入字节数。

382行，更新文件大小。

至此，LDD3第三章相关的源码我们就都分析完了。

在我的系统上(2.6.32-38-generic-pae),编译scull代码时,遇到如下图所示错误:

![](http://hi.csdn.net/attachment/201203/22/0_1332401333Iwwi.gif)

解决办法是将Makefile的第12行,13行,38行的CFLAGS替换为EXTRA_CFLAGS即可。但是再执行make,又出现另一个错误如下图所示：

![](http://hi.csdn.net/attachment/201203/22/0_13324014858ys2.gif)

只要把main.c第17行的#include <linux/config.h>屏蔽掉即可解决这个问题。但是再执行make，又出现如下图所示问题：

![](http://hi.csdn.net/attachment/201203/22/0_1332401535w30q.gif)

在pipe.c文件中加入如下语句：#include <linux/sched.h>即可解决，再次make，又出现如下图所示错误：

![](http://hi.csdn.net/attachment/201203/22/0_1332401576Q332.gif)

在access.c文件中加入如下语句：#include <linux/sched.h>，再次make，又出现如下图所示错误：

![](http://hi.csdn.net/attachment/201203/22/0_13324016276414.gif)

task_struct结构体在iclude/linux/sched.h中定义，这个错误因为task_struct结构体有改动，将uid和euid等变量挪到了cred中，cred结构体定义在include/linux/cred.h中，这个头文件已经在include/linux/sched.h中包含。所以，需要将报错的代码做如下修改：

current->uid改为current->cred->uid

current->euid改为current->cred->euid

再次make,编译通过，如下图所示：

![](http://hi.csdn.net/attachment/201203/22/0_1332401779jLyc.gif)

安装/卸载scull模块使用LDD3源码中自带的scull_load和scull_unload脚本。如下图所示：(注意，在我的系统上，我修改了scull_load文件的第5行，将mode设置为”666”，否则当往/dev/scull0写入数据时会提示没有操作权限。)

![](http://hi.csdn.net/attachment/201203/22/0_1332401861Qkaf.gif)

测试scull测试如下图所示：

![](http://hi.csdn.net/attachment/201203/22/0_13324019023TBY.gif)




