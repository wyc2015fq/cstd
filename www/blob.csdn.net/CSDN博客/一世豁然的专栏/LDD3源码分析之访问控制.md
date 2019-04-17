# LDD3源码分析之访问控制 - 一世豁然的专栏 - CSDN博客





2014年12月18日 16:26:43[一世豁然](https://me.csdn.net/Explorer_day)阅读数：662








本博客转载于：[http://blog.csdn.net/liuhaoyutz/article/details/7407700](http://blog.csdn.net/liuhaoyutz/article/details/7407700) [作者：刘昊昱]





一、访问控制设备的注册本文分析LDD3第6章介绍的设备文件访问控制的实现，涉及的代码主要在access.c文件中，但是作为分析的起点，我们还是要看一下main.c文件中的scull_init_module函数，在该函数中，有如下语句：


**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- 657    dev = MKDEV(scull_major, scull_minor + scull_nr_devs);  
- 658    dev += scull_p_init(dev);  
- 659    dev += scull_access_init(dev);  

657行，scull_major和scull_minor的默认值都是0，scull_nr_devs的默认值是4。dev变量在618行定义，它是dev_t类型，注意dev_t类型用来保存设备编号，包括主设备号和次设备号。所以，657行，我们通过MKDEV宏得到一个主设备号为0，次设备号为4的设备编号保存在dev中。这里之所以让次设备号为4，是因为前面已经注册了scull0 - scull3，它们的主设备号均为系统动态分配值，次设备号分别是0，1，2，3。



658行，调用了pipe.c文件中的scull_p_init函数，我们在前面的《LDD3源码分析之阻塞型I/O》一文中对这个函数进行了分析，它创建了scullpipe0
 - scullpipe3四个设备，对应的主设备号是系统动态分配值，次设备号为4，5，6，7。而scull_p_init函数的返回值是4，所以658行把dev的值再加4，然后传递给659行的scull_access_init函数。

659行，调用scull_access_init函数，这个函数就是我们今天要分析的起点，在access.c文件中定义，下面看这个函数的代码：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- 366int scull_access_init(dev_t firstdev)  
- 367{  
- 368    int result, i;  
- 369  
- 370    /* Get our number space */
- 371    result = register_chrdev_region (firstdev, SCULL_N_ADEVS, "sculla");  
- 372    if (result < 0) {  
- 373        printk(KERN_WARNING "sculla: device number registration failed\n");  
- 374        return 0;  
- 375    }  
- 376    scull_a_firstdev = firstdev;  
- 377  
- 378    /* Set up each device. */
- 379    for (i = 0; i < SCULL_N_ADEVS; i++)  
- 380        scull_access_setup (firstdev + i, scull_access_devs + i);  
- 381    return SCULL_N_ADEVS;  
- 382}  


371行注册访问控制相关设备的设备号，起始设备号是由参数传递进来的，注册的设备编号的个数是SCULL_N_ADEVS，它的值是4。

376行，保存第一个设备的设备编号。

379 - 380行，调用scull_access_setup函数，循环初始化4个访问控制相关设备。注意传递给scull_access_setup函数的第二个参数是scull_access_devs
 + i，先看一下scull_access_devs的定义：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- 327static struct scull_adev_info {  
- 328    char *name;  
- 329    struct scull_dev *sculldev;  
- 330    struct file_operations *fops;  
- 331} scull_access_devs[] = {  
- 332    { "scullsingle", &scull_s_device, &scull_sngl_fops },  
- 333    { "sculluid", &scull_u_device, &scull_user_fops },  
- 334    { "scullwuid", &scull_w_device, &scull_wusr_fops },  
- 335    { "sullpriv", &scull_c_device, &scull_priv_fops }  
- 336};  


可见，scull_access_devs是一个scull_adev_info结构体数组，该结构体代表一个访问控制设备，scull_adev_info有3个成员，第一个代表设备名，第二个是第3章中介绍的scull设备，第三个是对于这个访问控制设备的操作函数集。

scull_access_devs数组定义了4个访问控制设备，这4个设备使用不同的访问控制策略。第一个设备叫scullsingle，对应的”bare
 scull device”是scull_s_device,定义在49行，对应的操作函数集是scull_sngl_fops，定义在78行：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- 49static struct scull_dev scull_s_device;  






**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- 78struct file_operations scull_sngl_fops = {  
- 79    .owner =    THIS_MODULE,  
- 80    .llseek =       scull_llseek,  
- 81    .read =         scull_read,  
- 82    .write =        scull_write,  
- 83    .ioctl =        scull_ioctl,  
- 84    .open =         scull_s_open,  
- 85    .release =      scull_s_release,  
- 86};  


其它三个设备分别是sculluid、scullwuid、sullpriv，它们对应的”bare scull device”和操作函数集也都是在access.c中定义，这里不一一列出了，后面分析相应设备时再详细介绍。由上面的内容可以看出，访问控制设备的实现是建立在”bare
 scull device”的基础上的，很多代码都是与”bare scull device”复用的。

下面看scull_access_setup函数的定义：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- 339/*
- 340 * Set up a single device.
- 341 */
- 342static void scull_access_setup (dev_t devno, struct scull_adev_info *devinfo)  
- 343{  
- 344    struct scull_dev *dev = devinfo->sculldev;  
- 345    int err;  
- 346  
- 347    /* Initialize the device structure */
- 348    dev->quantum = scull_quantum;  
- 349    dev->qset = scull_qset;  
- 350    init_MUTEX(&dev->sem);  
- 351  
- 352    /* Do the cdev stuff. */
- 353    cdev_init(&dev->cdev, devinfo->fops);  
- 354    kobject_set_name(&dev->cdev.kobj, devinfo->name);  
- 355    dev->cdev.owner = THIS_MODULE;  
- 356    err = cdev_add (&dev->cdev, devno, 1);  
- 357        /* Fail gracefully if need be */
- 358    if (err) {  
- 359        printk(KERN_NOTICE "Error %d adding %s\n", err, devinfo->name);  
- 360        kobject_put(&dev->cdev.kobj);  
- 361    } else
- 362        printk(KERN_NOTICE "%s registered at %x\n", devinfo->name, devno);  
- 363}  


348 - 353行，和第三章中初始化scull设备一样，分别初始化了量子数，量子集数，信号量和cdev成员。353行还将字符设备关联了相应的文件操作函数集。

354行，注册了sys系统中的名字。

356行，将字符设备注册到系统中，完成注册。

这样，就完成了对字符设备的初始化和注册，现在我们有了4个采用不同访问控制策略的设备，分别是scullsingle、sculluid、scullwuid和scullpriv。

为了对这个4设备的访问控制策略进行测试，我编写了一个简单的测试程序access_control.c，其代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- #include <stdio.h>
- #include <unistd.h>
- #include <fcntl.h>
- #include <string.h>
- #include <sys/types.h>
- #include <sys/stat.h>
- 
- #define BUF_SIZE 50
- 
- int main(int argc, char *argv[])  
- {  
- int fd;  
- int num, n;  
- char buf[BUF_SIZE];  
- 
-     fd = open(argv[1], O_RDWR);  
- if(fd < 0)  
-     {  
-         printf("open scull error!\n");  
- return -1;  
-     }  
- 
-     n = 0;  
- while(n < 10)  
-     {  
-         lseek(fd, 0, SEEK_SET);  
-         memset(buf, 0, BUF_SIZE);  
-         num = read(fd, buf, BUF_SIZE);  
- if( num > 0)  
-         {  
-             buf[num] = 0;  
-             printf("%s\n", buf);  
-         }  
-         sleep(2);  
-         n++;  
-     }  
- 
- return 0;  
- }  


后面将使用这个测试程序对不同的设备进行测试。



二、独享设备

这种访问控制一次只允许一个进程访问设备，最好避免使用这种技术，因为它限制了用户的灵活性。scullsingle设备实现了独享设备的策略，其主要代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- 49static struct scull_dev scull_s_device;  
- 50static atomic_t scull_s_available = ATOMIC_INIT(1);  
- 51  
- 52static int scull_s_open(struct inode *inode, struct file *filp)  
- 53{  
- 54    struct scull_dev *dev = &scull_s_device; /* device information */
- 55  
- 56    if (! atomic_dec_and_test (&scull_s_available)) {  
- 57        atomic_inc(&scull_s_available);  
- 58        return -EBUSY; /* already open */
- 59    }  
- 60  
- 61    /* then, everything else is copied from the bare scull device */
- 62    if ( (filp->f_flags & O_ACCMODE) == O_WRONLY)  
- 63        scull_trim(dev);  
- 64    filp->private_data = dev;  
- 65    return 0;          /* success */
- 66}  
- 67  
- 68static int scull_s_release(struct inode *inode, struct file *filp)  
- 69{  
- 70    atomic_inc(&scull_s_available); /* release the device */
- 71    return 0;  
- 2}  
- 73  
- 74  
- 75/*
- 76 * The other operations for the single-open device come from the bare device
- 77 */
- 78struct file_operations scull_sngl_fops = {  
- 79    .owner =    THIS_MODULE,  
- 80    .llseek =       scull_llseek,  
- 81    .read =         scull_read,  
- 82    .write =        scull_write,  
- 83    .ioctl =        scull_ioctl,  
- 84    .open =         scull_s_open,  
- 85    .release =      scull_s_release,  
- 86};  


49行定义了scullsingle设备对应的”bare scull device” scull_s_device。

50行定义了一个原子变量(atomic_t)scull_s_available，其初始值为1，表明设备可用。如果其值为0，表明设备不可用。

56 - 59行，对原子变量scull_s_available执行atomic_dec_and_test操作，该函数将原子变量减1并测试其值是否为0，如果为0，返回TRUE，说明没有进程在使用设备，可以独享使用了。如果测试返回FALSE，说明有进程正在使用设备，将原子变量加1后，返回-EBUSY退出。

68 - 72行，定义了scull_s_release函数，该函数在进程关闭设备文件时调用，其作用是将原子变量scull_s_available的值加1，表示释放设备。

78 - 86行，定义了scullsingle设备的操作函数集，可以看到，除了open和release函数外，其他函数都是复用的scull设备的操作函数。

这样，通过加入一个原子变量，并在open函数中对其值进行判断，就能达到独享设备的目的了。

注意：通常应该把scull_s_available变量放在设备结构中(这里是scull_dev结构)，因为从概念上讲它本身属于设备。但是scullsingle设备的实现是把scull_s_available定义为一个全局变量，这样做是为了与scull复用代码。

使用测试程序access_control.c测试scullsingle设备的过程如下图所示：

![](https://img-my.csdn.net/uploads/201203/29/1333009349_5791.png)

因为access_control进程会占用scullsingle设备20秒，从上图可以看出，在access_control执行的这20秒内，另外一个进程即echo试图操作(这里是写)scullsingle设备会返回设备正忙的错误信息，这说明，scullsingle设备同时只能被一个进程访问。如下图所示，当access_control退出后，echo进程就可以操作scullsingle设备了：

![](https://img-my.csdn.net/uploads/201203/29/1333009432_4419.png)



三、限制每次只由一个用户访问

这种访问策略允许一个用户的多个进程同时访问设备，但是不允许多个用户同时访问设备。与独享设备的策略相比，这种方法更加灵活。此时需要增加两个数据项，一个打开计数器和一个设备属主UID。同样，这两个数据项最好保存在设备结构体内部，但是为了与scull复用代码，在实现时我们把这两个变量定义为全局变量。

使用这种策略实现的设备叫sculluid，其主要代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

-  95static struct scull_dev scull_u_device;  
-  96static int scull_u_count;   /* initialized to 0 by default */
-  97static uid_t scull_u_owner; /* initialized to 0 by default */
-  98static spinlock_t scull_u_lock = SPIN_LOCK_UNLOCKED;  
-  99  
- 100static int scull_u_open(struct inode *inode, struct file *filp)  
- 101{  
- 102    struct scull_dev *dev = &scull_u_device; /* device information */
- 103  
- 104    spin_lock(&scull_u_lock);  
- 105    if (scull_u_count &&  
- 106            (scull_u_owner != current->uid) &&  /* allow user */
- 107            (scull_u_owner != current->euid) && /* allow whoever did su */
- 108            !capable(CAP_DAC_OVERRIDE)) { /* still allow root */
- 109        spin_unlock(&scull_u_lock);  
- 110        return -EBUSY;   /* -EPERM would confuse the user */
- 111    }  
- 112  
- 113    if (scull_u_count == 0)  
- 114        scull_u_owner = current->uid; /* grab it */
- 115  
- 116    scull_u_count++;  
- 117    spin_unlock(&scull_u_lock);  
- 118  
- 119/* then, everything else is copied from the bare scull device */
- 120  
- 121    if ((filp->f_flags & O_ACCMODE) == O_WRONLY)  
- 122        scull_trim(dev);  
- 123    filp->private_data = dev;  
- 124    return 0;          /* success */
- 125}  
- 126  
- 127static int scull_u_release(struct inode *inode, struct file *filp)  
- 128{  
- 129    spin_lock(&scull_u_lock);  
- 130    scull_u_count--; /* nothing else */
- 131    spin_unlock(&scull_u_lock);  
- 132    return 0;  
- 133}  
- 134  
- 135  
- 136  
- 137/*
- 138 * The other operations for the device come from the bare device
- 139 */
- 140struct file_operations scull_user_fops = {  
- 141    .owner =      THIS_MODULE,  
- 142    .llseek =     scull_llseek,  
- 143    .read =       scull_read,  
- 144    .write =      scull_write,  
- 145    .ioctl =      scull_ioctl,  
- 146    .open =       scull_u_open,  
- 147    .release =    scull_u_release,  
- 148};  


95行定义了设备结构体scull_u_device。

96行定义了访问计数器变量scull_u_count，该变量用来保存正在访问设备的进程数。

97行定义了uid_t变量scull_u_owner，用来保存正在访问设备的用户UID。

105 - 111行，如果不是当前进程不是第一个访问设备的进程，并且当前进程的uid或euid不等于scull_u_owner变量的值，并且不是root权限用户，则返回-EBUSY退出。表明有另外一个用户正在访问设备。

113 - 114行，如果是第一个访问设备的进程，则将进程的UID保存在scull_u_owner。

116行，将访问计数器值加1。

127 - 133行，scull_u_release函数在关闭设备文件时调用，其作用是将访问计数器值减1。

140 - 148行，定义了sculluid的设备操作函数集，可以看到，除了open和release函数，其它函数都是与scull复用的。

使用accell_control测试sculluid设备的过程如下图所示：

![](https://img-my.csdn.net/uploads/201203/29/1333009548_7177.png)

由上图操作过程可以看出，同一用户在两个终端下可以同时对sculluid设备进行操作。不同用户对sculluid同时进行操作是否可以呢？如下图所示：

![](https://img-my.csdn.net/uploads/201203/29/1333009593_9082.png)

由上图可以看出，不同用户，不能同时对sculluid进行操作。

另外，一个用普通用户，一个用root用户，能不能同时操作sculluid呢？大家可以自己试验一下。



四、阻塞型open

上面两种访问控制方法当设备不能访问时，都是返回-EBUSY退出，但是有些情况下，可能需要让进程阻塞等待，这时就需要实现阻塞型open。

scullwuid设备实现了阻塞型open，其主要代码如下：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- 156static struct scull_dev scull_w_device;  
- 157static int scull_w_count;   /* initialized to 0 by default */
- 158static uid_t scull_w_owner; /* initialized to 0 by default */
- 159static DECLARE_WAIT_QUEUE_HEAD(scull_w_wait);  
- 160static spinlock_t scull_w_lock = SPIN_LOCK_UNLOCKED;  
- 161  
- 162static inlineint scull_w_available(void)  
- 163{  
- 164    return scull_w_count == 0 ||  
- 165        scull_w_owner == current->uid ||  
- 166        scull_w_owner == current->euid ||  
- 167        capable(CAP_DAC_OVERRIDE);  
- 168}  
- 169  
- 170  
- 171static int scull_w_open(struct inode *inode, struct file *filp)  
- 172{  
- 173    struct scull_dev *dev = &scull_w_device; /* device information */
- 174  
- 175    spin_lock(&scull_w_lock);  
- 176    while (! scull_w_available()) {  
- 177        spin_unlock(&scull_w_lock);  
- 178        if (filp->f_flags & O_NONBLOCK) return -EAGAIN;  
- 179        if (wait_event_interruptible (scull_w_wait, scull_w_available()))  
- 180            return -ERESTARTSYS; /* tell the fs layer to handle it */
- 181        spin_lock(&scull_w_lock);  
- 182    }  
- 183    if (scull_w_count == 0)  
- 184        scull_w_owner = current->uid; /* grab it */
- 185    scull_w_count++;  
- 186    spin_unlock(&scull_w_lock);  
- 187  
- 188    /* then, everything else is copied from the bare scull device */
- 189    if ((filp->f_flags & O_ACCMODE) == O_WRONLY)  
- 190        scull_trim(dev);  
- 191    filp->private_data = dev;  
- 192    return 0;          /* success */
- 193}  
- 194  
- 195static int scull_w_release(struct inode *inode, struct file *filp)  
- 196{  
- 197    int temp;  
- 198  
- 199    spin_lock(&scull_w_lock);  
- 200    scull_w_count--;  
- 201    temp = scull_w_count;  
- 202    spin_unlock(&scull_w_lock);  
- 203  
- 204    if (temp == 0)  
- 205        wake_up_interruptible_sync(&scull_w_wait); /* awake other uid's */
- 206    return 0;  
- 207}  
- 208  
- 209  
- 210/*
- 211 * The other operations for the device come from the bare device
- 212 */
- 213struct file_operations scull_wusr_fops = {  
- 214    .owner =      THIS_MODULE,  
- 215    .llseek =     scull_llseek,  
- 216    .read =       scull_read,  
- 217    .write =      scull_write,  
- 218    .ioctl =      scull_ioctl,  
- 219    .open =       scull_w_open,  
- 220    .release =    scull_w_release,  
- 221};  



159行定义了一个等待队列scull_w_wait。

176 - 182行，判断能否访问设备的方法与sculluid相同，但是，如果不能访问设备，阻塞在scull_w_wait上等待而不是返回-EBUSY退出。

195 - 207行，scull_w_release函数在关闭设备文件时调用，它将使用计数器值减1，如果使用计数为0，则唤醒在等待队列scull_w_wait中阻塞等待的进程。

213 - 221行，定义scullwuid的文件操作函数集，除了open和release函数外，其它的函数都是与scull复用代码。

使用access_control测试scullwuid设备的过程如下图所示。注意，测试必须使用两个不同的普通用户进行。

![](https://img-my.csdn.net/uploads/201203/29/1333009708_6360.png)

由上图可以看出，当一个用户在操作scullwuid时，另一个用户的如果要打开scullwuid设备会被阻塞住。而当前一个用户操作完成了，被阻塞用户会解除阻塞，继续执行，如下图所示：

![](https://img-my.csdn.net/uploads/201203/29/1333009754_3012.png)



五、打开时clone设备

另一个实现访问控制的方法是，在进程打开设备时clone一个设备给进程使用。使用这种控制策略实现的设备是scullpriv，它使用当前进程控制终端的设备号作为访问虚拟设备的键值，也可以使用任意整数值做为键值，但是不同的键值将导致不同的访问策略。例如，使用uid作为键值，则会给每个用户clone一个设备。使用pid作为键值，则会给每个进程clone一个设备。所以，对于scullpriv，不同终端上的进程会有不同的clone设备。

下面是scullpriv的主要实现代码：



**[cpp]**[view plain](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)[copy](http://blog.csdn.net/liuhaoyutz/article/details/7407700#)

- 229/* The clone-specific data structure includes a key field */
- 230  
- 231struct scull_listitem {  
- 232    struct scull_dev device;  
- 233    dev_t key;  
- 234    struct list_head list;  
- 235  
- 236};  
- 237  
- 238/* The list of devices, and a lock to protect it */
- 239static LIST_HEAD(scull_c_list);  
- 240static spinlock_t scull_c_lock = SPIN_LOCK_UNLOCKED;  
- 241  
- 242/* A placeholder scull_dev which really just holds the cdev stuff. */
- 243static struct scull_dev scull_c_device;  
- 244  
- 245/* Look for a device or create one if missing */
- 246static struct scull_dev *scull_c_lookfor_device(dev_t key)  
- 247{  
- 248    struct scull_listitem *lptr;  
- 249  
- 250    list_for_each_entry(lptr, &scull_c_list, list) {  
- 251        if (lptr->key == key)  
- 252            return &(lptr->device);  
- 253    }  
- 254  
- 255    /* not found */
- 256    lptr = kmalloc(sizeof(struct scull_listitem), GFP_KERNEL);  
- 257    if (!lptr)  
- 258        return NULL;  
- 259  
- 260    /* initialize the device */
- 261    memset(lptr, 0, sizeof(struct scull_listitem));  
- 262    lptr->key = key;  
- 263    scull_trim(&(lptr->device)); /* initialize it */
- 264    init_MUTEX(&(lptr->device.sem));  
- 265  
- 266    /* place it in the list */
- 267    list_add(&lptr->list, &scull_c_list);  
- 268  
- 269    return &(lptr->device);  
- 270}  
- 271  
- 272static int scull_c_open(struct inode *inode, struct file *filp)  
- 273{  
- 274    struct scull_dev *dev;  
- 275    dev_t key;  
- 276  
- 277    if (!current->signal->tty) {  
- 278        PDEBUG("Process \"%s\" has no ctl tty\n", current->comm);  
- 279        return -EINVAL;  
- 280    }  
- 281    key = tty_devnum(current->signal->tty);  
- 282  
- 283    /* look for a scullc device in the list */
- 284    spin_lock(&scull_c_lock);  
- 285    dev = scull_c_lookfor_device(key);  
- 286    spin_unlock(&scull_c_lock);  
- 287  
- 288    if (!dev)  
- 289        return -ENOMEM;  
- 290  
- 291    /* then, everything else is copied from the bare scull device */
- 292    if ( (filp->f_flags & O_ACCMODE) == O_WRONLY)  
- 293        scull_trim(dev);  
- 294    filp->private_data = dev;  
- 295    return 0;          /* success */
- 296}  
- 297  
- 298static int scull_c_release(struct inode *inode, struct file *filp)  
- 299{  
- 300    /*
- 301     * Nothing to do, because the device is persistent.
- 302     * A `real' cloned device should be freed on last close
- 303     */
- 304    return 0;  
- 305}  
- 306  
- 307  
- 308  
- 309/*
- 310 * The other operations for the device come from the bare device
- 311 */
- 312struct file_operations scull_priv_fops = {  
- 313    .owner =    THIS_MODULE,  
- 314    .llseek =   scull_llseek,  
- 315    .read =     scull_read,  
- 316    .write =    scull_write,  
- 317    .ioctl =    scull_ioctl,  
- 318    .open =     scull_c_open,  
- 319    .release =  scull_c_release,  
- 320};  


我们从272scull_c_open函数开始分析。

277行，current->signal->tty代表当前进程的控制终端，如果当前进程没有控制终端，则退出。

281行，通过tty_devnum函数，取得当前进程控制终端的设备号，赋值给key。

285行，调用scull_c_lookfor_device(key)查找设备，如果没有，在scull_c_lookfor_device函数中会创建一个。注意，传递给scull_c_lookfor_device的参数是key。

下面看scull_c_lookfor_device函数的实现：

250 - 253行，遍历链表scull_c_list，如果有链表项的key值等于参数传递进来的key值，则说明已经为该控制终端clone过设备，则直接返回对应的设备结构。

256行，如果在scull_c_list链表中没有查找到对应key的节点，说明是第一次在该控制终端上打开设备，则为链表节点scull_listitem分配内存空间。

261 - 264行，初始化链表节点结构体。

267行，将链表节点加入到scull_c_list链表中。

269行，返回找到或新创建的scull_dev结构体。

298 - 305行，release函数不做任何事，因为scullpriv是永久存在的，如果是一个真正的clone设备，应该在最后一次关闭后释放空间。

312 - 320行，定义了设备文件操作函数集，除了open和release函数外，其它函数都是利用的scull的代码。

使用access_control测试scullpriv设备的过程如下图所示：

![](https://img-my.csdn.net/uploads/201203/29/1333009849_8391.png)

由上图可以看出，两个终端下的进程可以同时操作scullpriv，并且相互没有影响。因为两个终端操作的是两个不同的scullpriv的clone版本。




