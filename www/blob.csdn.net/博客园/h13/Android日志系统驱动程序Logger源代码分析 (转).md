# Android日志系统驱动程序Logger源代码分析 (转) - h13 - 博客园
我们知道，在[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)系统中，提供了一个轻量级的日志系统，这个日志系统是以驱动程序的形式实现在内核空间的，而在用户空间分别提供了Java接口和C/C++接口来使用这个日志系统，取决于你编写的是Android应用程序还是系统组件。在前面的文章[浅谈Android系统开发中LOG的使用](http://www.linuxidc.com/Linux/2011-07/38984.htm)中，已经简要地介绍了在Android应用程序开发中Log的使用方法，在这一篇文章中，我们将更进一步地分析Logger驱动程序的源代码，使得我们对Android日志系统有一个深刻的认识。
既然[Android](http://www.linuxidc.com/topicnews.aspx?tid=11) 日志系统是以驱动程序的形式实现在内核空间的，我们就需要获取Android内核源代码来分析了，请参照前面[在Ubuntu上下载、编译和安装Android最新源代码](http://www.linuxidc.com/Linux/2011-06/37781.htm)和[在Ubuntu上下载、编译和安装Android最新内核源代码（Linux Kernel）](http://www.linuxidc.com/Linux/2011-06/37782.htm)两篇文章，下载好Android源代码工程。Logger驱动程序主要由两个文件构成，分别是：
       kernel/common/drivers/staging/[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)/logger.h
       kernel/common/drivers/staging/[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)/logger.c
       接下来，我们将分别介绍Logger驱动程序的相关数据结构，然后对Logger驱动程序源代码进行情景分析，分别日志系统初始化情景、日志读取情景和日志写入情景。
       一. Logger驱动程序的相关数据结构。
      我们首先来看logger.h头文件的内容：
- #ifndef _LINUX_LOGGER_H 
- #define _LINUX_LOGGER_H 
- 
- #include <linux/types.h> 
- #include <linux/ioctl.h> 
- 
- **struct** logger_entry {  
-     __u16       len;    /* length of the payload */
-     __u16       __pad;  /* no matter what, we get 2 bytes of padding */
-     __s32       pid;    /* generating process's pid */
-     __s32       tid;    /* generating process's tid */
-     __s32       sec;    /* seconds since Epoch */
-     __s32       nsec;   /* nanoseconds */
- **char**        msg[0]; /* the entry's payload */
- };  
- 
- #define LOGGER_LOG_RADIO    "log_radio" /* radio-related messages */ 
- #define LOGGER_LOG_EVENTS   "log_events"    /* system/hardware events */ 
- #define LOGGER_LOG_MAIN     "log_main"  /* everything else */ 
- 
- #define LOGGER_ENTRY_MAX_LEN        (4*1024) 
- #define LOGGER_ENTRY_MAX_PAYLOAD    \ 
-     (LOGGER_ENTRY_MAX_LEN - **sizeof**(**struct** logger_entry))  
- 
- #define __LOGGERIO  0xAE 
- 
- #define LOGGER_GET_LOG_BUF_SIZE     _IO(__LOGGERIO, 1) /* size of log */ 
- #define LOGGER_GET_LOG_LEN      _IO(__LOGGERIO, 2) /* used log len */ 
- #define LOGGER_GET_NEXT_ENTRY_LEN   _IO(__LOGGERIO, 3) /* next entry len */ 
- #define LOGGER_FLUSH_LOG        _IO(__LOGGERIO, 4) /* flush log */ 
- 
- #endif /* _LINUX_LOGGER_H */
        struct logger_entry是一个用于描述一条Log记录的结构体。len成员变量记录了这条记录的有效负载的长度，有效负载指定的日志记录本身的长度，但是不包括用于描述这个记录的struct logger_entry结构体。回忆一下我们调用[Android](http://www.linuxidc.com/topicnews.aspx?tid=11).util.Log接口来使用日志系统时，会指定日志的优先级别Priority、Tag字符串以及Msg字符串，Priority + Tag + Msg三者内容的长度加起来就是记录的有效负载长度了。__pad成员变量是用来对齐结构体的。pid和tid成员变量分别用来记录是哪条进程写入了这条记录。sec和nsec成员变量记录日志写的时间。msg成员变量记录的就有效负载的内容了，它的大小由len成员变量来确定。
       接着定义两个宏：
       #define LOGGER_ENTRY_MAX_LEN             (4*1024)
       #define LOGGER_ENTRY_MAX_PAYLOAD   \
                         (LOGGER_ENTRY_MAX_LEN - sizeof(struct logger_entry))
      从这两个宏可以看出，每条日志记录的有效负载长度加上结构体logger_entry的长度不能超过4K个字节。
      logger.h文件中还定义了其它宏，读者可以自己分析，在下面的分析中，碰到时，我们也会详细解释。
      再来看logger.c文件中，其它相关数据结构的定义：
- /*
-  * struct logger_log - represents a specific log, such as 'main' or 'radio'
-  *
-  * This structure lives from module insertion until module removal, so it does
-  * not need additional reference counting. The structure is protected by the
-  * mutex 'mutex'.
-  */
- **struct** logger_log {  
-     unsigned **char** *     buffer; /* the ring buffer itself */
- **struct** miscdevice   misc;   /* misc device representing the log */
-     wait_queue_head_t   wq; /* wait queue for readers */
- **struct** list_head    readers; /* this log's readers */
- **struct** mutex        mutex;  /* mutex protecting buffer */
- **size_t**          w_off;  /* current write head offset */
- **size_t**          head;   /* new readers start here */
- **size_t**          size;   /* size of the log */
- };  
- 
- /*
-  * struct logger_reader - a logging device open for reading
-  *
-  * This object lives from open to release, so we don't need additional
-  * reference counting. The structure is protected by log->mutex.
-  */
- **struct** logger_reader {  
- **struct** logger_log * log;    /* associated log */
- **struct** list_head    list;   /* entry in logger_log's list */
- **size_t**          r_off;  /* current read head offset */
- };  
- 
- /* logger_offset - returns index 'n' into the log via (optimized) modulus */
- #define logger_offset(n)    ((n) & (log->size - 1))
        结构体struct logger_log就是真正用来保存日志的地方了。buffer成员变量变是用保存日志信息的内存缓冲区，它的大小由size成员变量确定。从misc成员变量可以看出，logger驱动程序使用的设备属于misc类型的设备，通过在[Android](http://www.linuxidc.com/topicnews.aspx?tid=11)模拟器上执行cat /proc/devices命令（可参考 [在Ubuntu上下载、编译和安装Android最新内核源代码（Linux Kernel）](http://www.linuxidc.com/Linux/2011-06/37782.htm) 一文），可以看出，misc类型设备的主设备号是10。关于主设备号的相关知识，可以参考[Android学习启动篇](http://www.linuxidc.com/Linux/2011-06/37775.htm)一文中提到的Linux Driver Development一书。wq成员变量是一个等待队列，用于保存正在等待读取日志的进程。readers成员变量用来保存当前正在读取日志的进程，正在读取日志的进程由结构体logger_reader来描述。mutex成员变量是一个互斥量，用来保护log的并发访问。可以看出，这里的日志系统的读写问题，其实是一个生产者-消费者的问题，因此，需要互斥量来保护log的并发访问。 w_off成员变量用来记录下一条日志应该从哪里开始写。head成员变量用来表示打开日志文件中，应该从哪一个位置开始读取日志。
       结构体struct logger_reader用来表示一个读取日志的进程，log成员变量指向要读取的日志缓冲区。list成员变量用来连接其它读者进程。r_off成员变量表示当前要读取的日志在缓冲区中的位置。
       struct logger_log结构体中用于保存日志信息的内存缓冲区buffer是一个循环使用的环形缓冲区，缓冲区中保存的内容是以struct logger_entry为单位的，每个单位的组成为：
       struct logger_entry | priority | tag | msg
       由于是内存缓冲区buffer是一个循环使用的环形缓冲区，给定一个偏移值，它在buffer中的位置由下logger_offset来确定：
       #define logger_offset(n)          ((n) & (log->size - 1))
二. Logger驱动程序模块的初始化过程分析。
       继续看logger.c文件，定义了三个日志设备：
- /*
-  * Defines a log structure with name 'NAME' and a size of 'SIZE' bytes, which
-  * must be a power of two, greater than LOGGER_ENTRY_MAX_LEN, and less than
-  * LONG_MAX minus LOGGER_ENTRY_MAX_LEN.
-  */
- #define DEFINE_LOGGER_DEVICE(VAR, NAME, SIZE) \ 
- **static** unsigned **char** _buf_ ## VAR[SIZE]; \  
- **static****struct** logger_log VAR = { \  
-     .buffer = _buf_ ## VAR, \  
-     .misc = { \  
-         .minor = MISC_DYNAMIC_MINOR, \  
-         .name = NAME, \  
-         .fops = &logger_fops, \  
-         .parent = NULL, \  
-     }, \  
-     .wq = __WAIT_QUEUE_HEAD_INITIALIZER(VAR .wq), \  
-     .readers = LIST_HEAD_INIT(VAR .readers), \  
-     .mutex = __MUTEX_INITIALIZER(VAR .mutex), \  
-     .w_off = 0, \  
-     .head = 0, \  
-     .size = SIZE, \  
- };  
- 
- DEFINE_LOGGER_DEVICE(log_main, LOGGER_LOG_MAIN, 64*1024)  
- DEFINE_LOGGER_DEVICE(log_events, LOGGER_LOG_EVENTS, 256*1024)  
- DEFINE_LOGGER_DEVICE(log_radio, LOGGER_LOG_RADIO, 64*1024)  
       分别是log_main、log_events和log_radio，名称分别LOGGER_LOG_MAIN、LOGGER_LOG_EVENTS和LOGGER_LOG_RADIO，它们的次设备号为MISC_DYNAMIC_MINOR，即为在注册时动态分配。在logger.h文件中，有这三个宏的定义：
       #define LOGGER_LOG_RADIO"log_radio"/* radio-related messages */
       #define LOGGER_LOG_EVENTS"log_events"/* system/hardware events */
       #define LOGGER_LOG_MAIN"log_main"/* everything else */
       注释说明了这三个日志设备的用途。注册的日志设备文件操作方法为logger_fops：
- **static****struct** file_operations logger_fops = {  
-     .owner = THIS_MODULE,  
-     .read = logger_read,  
-     .aio_write = logger_aio_write,  
-     .poll = logger_poll,  
-     .unlocked_ioctl = logger_ioctl,  
-     .compat_ioctl = logger_ioctl,  
-     .open = logger_open,  
-     .release = logger_release,  
- };  
       日志驱动程序模块的初始化函数为logger_init：
- **static****int** __init logger_init(**void**)  
- {  
- **int** ret;  
- 
-     ret = init_log(&log_main);  
- **if** (unlikely(ret))  
- **goto** out;  
- 
-     ret = init_log(&log_events);  
- **if** (unlikely(ret))  
- **goto** out;  
- 
-     ret = init_log(&log_radio);  
- **if** (unlikely(ret))  
- **goto** out;  
- 
- out:  
- **return** ret;  
- }  
- device_initcall(logger_init);  
        logger_init函数通过调用init_log函数来初始化了上述提到的三个日志设备：
- **static****int** __init init_log(**struct** logger_log *log) 
- {  
- **int** ret;  
- 
-     ret = misc_register(&log->misc);  
- **if** (unlikely(ret)) {  
-         printk(KERN_ERR "logger: failed to register misc "
- "device for log '%s'!\n", log->misc.name);  
- **return** ret;  
-     }  
- 
-     printk(KERN_INFO "logger: created %luK log '%s'\n",  
-            (unsigned **long**) log->size >> 10, log->misc.name);  
- 
- **return** 0;  
- }  
        init_log函数主要调用了misc_register函数来注册misc设备，misc_register函数定义在kernel/common/drivers/char/misc.c文件中：
- /**
-  *      misc_register   -       register a miscellaneous device
-  *      @misc: device structure
-  *
-  *      Register a miscellaneous device with the kernel. If the minor
-  *      number is set to %MISC_DYNAMIC_MINOR a minor number is assigned
-  *      and placed in the minor field of the structure. For other cases
-  *      the minor number requested is used.
-  *
-  *      The structure passed is linked into the kernel and may not be
-  *      destroyed until it has been unregistered.
-  *
-  *      A zero is returned on success and a negative errno code for
-  *      failure.
-  */
- 
- **int** misc_register(**struct** miscdevice * misc)  
- {  
- **struct** miscdevice *c;  
-         dev_t dev;  
- **int** err = 0;  
- 
-         INIT_LIST_HEAD(&misc->list);  
- 
-         mutex_lock(&misc_mtx);  
-         list_for_each_entry(c, &misc_list, list) {  
- **if** (c->minor == misc->minor) {  
-                         mutex_unlock(&misc_mtx);  
- **return** -EBUSY;  
-                 }  
-         }  
- 
- **if** (misc->minor == MISC_DYNAMIC_MINOR) {  
- **int** i = DYNAMIC_MINORS;  
- **while** (--i >= 0)  
- **if** ( (misc_minors[i>>3] & (1 << (i&7))) == 0)  
- **break**;  
- **if** (i<0) {  
-                         mutex_unlock(&misc_mtx);  
- **return** -EBUSY;  
-                 }  
-                 misc->minor = i;  
-         }  
- 
- **if** (misc->minor < DYNAMIC_MINORS)  
-                 misc_minors[misc->minor >> 3] |= 1 << (misc->minor & 7);  
-         dev = MKDEV(MISC_MAJOR, misc->minor);  
- 
-         misc->this_device = device_create(misc_class, misc->parent, dev, NULL,  
- "%s", misc->name);  
- **if** (IS_ERR(misc->this_device)) {  
-                 err = PTR_ERR(misc->this_device);  
- **goto** out;  
-         }  
- 
- /*
-          * Add it to the front, so that later devices can "override"
-          * earlier defaults
-          */
-         list_add(&misc->list, &misc_list);  
-  out:  
-         mutex_unlock(&misc_mtx);  
- **return** err;  
- }  
        注册完成后，通过device_create创建设备文件节点。这里，将创建/dev/log/main、/dev/log/events和/dev/log/radio三个设备文件，这样，用户空间就可以通过读写这三个文件和驱动程序进行交互。
三. Logger驱动程序的日志记录读取过程分析。
继续看logger.c 文件，注册的读取日志设备文件的方法为logger_read： 
- /*
-  * logger_read - our log's read() method
-  *
-  * Behavior:
-  *
-  *  - O_NONBLOCK works
-  *  - If there are no log entries to read, blocks until log is written to
-  *  - Atomically reads exactly one log entry
-  *
-  * Optimal read size is LOGGER_ENTRY_MAX_LEN. Will set errno to EINVAL if read
-  * buffer is insufficient to hold next entry.
-  */
- **static** ssize_t logger_read(**struct** file *file, **char** __user *buf,  
- **size_t** count, loff_t *pos)  
- {  
- **struct** logger_reader *reader = file->private_data;  
- **struct** logger_log *log = reader->log;  
-     ssize_t ret;  
-     DEFINE_WAIT(wait);  
- 
- start:  
- **while** (1) {  
-         prepare_to_wait(&log->wq, &wait, TASK_INTERRUPTIBLE);  
- 
-         mutex_lock(&log->mutex);  
-         ret = (log->w_off == reader->r_off);  
-         mutex_unlock(&log->mutex);  
- **if** (!ret)  
- **break**;  
- 
- **if** (file->f_flags & O_NONBLOCK) {  
-             ret = -EAGAIN;  
- **break**;  
-         }  
- 
- **if** (signal_pending(current)) {  
-             ret = -EINTR;  
- **break**;  
-         }  
- 
-         schedule();  
-     }  
- 
-     finish_wait(&log->wq, &wait);  
- **if** (ret)  
- **return** ret;  
- 
-     mutex_lock(&log->mutex);  
- 
- /* is there still something to read or did we race? */
- **if** (unlikely(log->w_off == reader->r_off)) {  
-         mutex_unlock(&log->mutex);  
- **goto** start;  
-     }  
- 
- /* get the size of the next entry */
-     ret = get_entry_len(log, reader->r_off);  
- **if** (count < ret) {  
-         ret = -EINVAL;  
- **goto** out;  
-     }  
- 
- /* get exactly one entry from the log */
-     ret = do_read_log_to_user(log, reader, buf, ret);  
- 
- out:  
-     mutex_unlock(&log->mutex);  
- 
- **return** ret;  
- }  
       注意，在函数开始的地方，表示读取日志上下文的struct logger_reader是保存在文件指针的private_data成员变量里面的，这是在打开设备文件时设置的，设备文件打开方法为logger_open：
- /*
-  * logger_open - the log's open() file operation
-  *
-  * Note how near a no-op this is in the write-only case. Keep it that way!
-  */
- **static****int** logger_open(**struct** inode *inode, **struct** file *file)  
- {  
- **struct** logger_log *log;  
- **int** ret;  
- 
-     ret = nonseekable_open(inode, file);  
- **if** (ret)  
- **return** ret;  
- 
-     log = get_log_from_minor(MINOR(inode->i_rdev));  
- **if** (!log)  
- **return** -ENODEV;  
- 
- **if** (file->f_mode & FMODE_READ) {  
- **struct** logger_reader *reader;  
- 
-         reader = kmalloc(**sizeof**(**struct** logger_reader), GFP_KERNEL);  
- **if** (!reader)  
- **return** -ENOMEM;  
- 
-         reader->log = log;  
-         INIT_LIST_HEAD(&reader->list);  
- 
-         mutex_lock(&log->mutex);  
-         reader->r_off = log->head;  
-         list_add_tail(&reader->list, &log->readers);  
-         mutex_unlock(&log->mutex);  
- 
-         file->private_data = reader;  
-     } **else**
-         file->private_data = log;  
- 
- **return** 0;  
- }  
       新打开日志设备文件时，是从log->head位置开始读取日志的，保存在struct logger_reader的成员变量r_off中。
start标号处的while循环是在等待日志可读，如果已经没有新的日志可读了，那么就要读进程就要进入休眠状态，等待新的日志写入后再唤醒，这是通过prepare_wait和schedule两个调用来实现的。如果没有新的日志可读，并且设备文件不是以非阻塞O_NONBLOCK的方式打开或者这时有信号要处理（signal_pending(current)），那么就直接返回，不再等待新的日志写入。判断当前是否有新的日志可读的方法是：
       ret = (log->w_off == reader->r_off);
       即判断当前缓冲区的写入位置和当前读进程的读取位置是否相等，如果不相等，则说明有新的日志可读。
       继续向下看，如果有新的日志可读，那么就，首先通过get_entry_len来获取下一条可读的日志记录的长度，从这里可以看出，日志读取进程是以日志记录为单位进行读取的，一次只读取一条记录。get_entry_len的函数实现如下：
- /*
-  * get_entry_len - Grabs the length of the payload of the next entry starting
-  * from 'off'.
-  *
-  * Caller needs to hold log->mutex.
-  */
- **static** __u32 get_entry_len(**struct** logger_log *log, **size_t** off)  
- {  
-     __u16 val;  
- 
- **switch** (log->size - off) {  
- **case** 1:  
-         memcpy(&val, log->buffer + off, 1);  
-         memcpy(((**char** *) &val) + 1, log->buffer, 1);  
- **break**;  
- **default**:  
-         memcpy(&val, log->buffer + off, 2);  
-     }  
- 
- **return****sizeof**(**struct** logger_entry) + val;  
- }  
        上面我们提到，每一条日志记录是由两大部分组成的，一个用于描述这条日志记录的结构体struct logger_entry，另一个是记录体本身，即有效负载。结构体struct logger_entry的长度是固定的，只要知道有效负载的长度，就可以知道整条日志记录的长度了。而有效负载的长度是记录在结构体struct logger_entry的成员变量len中，而len成员变量的地址与struct logger_entry的地址相同，因此，只需要读取记录的开始位置的两个字节就可以了。又由于日志记录缓冲区是循环使用的，这两个节字有可能是第一个字节存放在缓冲区最后一个字节，而第二个字节存放在缓冲区的第一个节，除此之外，这两个字节都是连在一起的。因此，分两种情况来考虑，对于前者，分别通过读取缓冲区最后一个字节和第一个字节来得到日志记录的有效负载长度到本地变量val中，对于后者，直接读取连续两个字节的值到本地变量val中。这两种情况是通过判断日志缓冲区的大小和要读取的日志记录在缓冲区中的位置的差值来区别的，如果相差1，就说明是前一种情况了。最后，把有效负载的长度val加上struct logger_entry的长度就得到了要读取的日志记录的总长度了。
接着往下看，得到了要读取的记录的长度，就调用do_read_log_to_user函数来执行真正的读取动作：
[](http://blog.csdn.net/luoshengyang/article/details/6595744)
- **static** ssize_t do_read_log_to_user(**struct** logger_log *log,  
- **struct** logger_reader *reader,  
- **char** __user *buf,  
- **size_t** count)  
- {  
- **size_t** len;  
- 
- /*
-      * We read from the log in two disjoint operations. First, we read from
-      * the current read head offset up to 'count' bytes or to the end of
-      * the log, whichever comes first.
-      */
-     len = min(count, log->size - reader->r_off);  
- **if** (copy_to_user(buf, log->buffer + reader->r_off, len))  
- **return** -EFAULT;  
- 
- /*
-      * Second, we read any remaining bytes, starting back at the head of
-      * the log.
-      */
- **if** (count != len)  
- **if** (copy_to_user(buf + len, log->buffer, count - len))  
- **return** -EFAULT;  
- 
-     reader->r_off = logger_offset(reader->r_off + count);  
- 
- **return** count;  
- }  
        这个函数简单地调用copy_to_user函数来把位于内核空间的日志缓冲区指定的内容拷贝到用户空间的内存缓冲区就可以了，同时，把当前读取日志进程的上下文信息中的读偏移r_off前进到下一条日志记录的开始的位置上。
        四.  Logger驱动程序的日志记录写入过程分析。
        继续看logger.c 文件，注册的写入日志设备文件的方法为logger_aio_write：
- /*
-  * logger_aio_write - our write method, implementing support for write(),
-  * writev(), and aio_write(). Writes are our fast path, and we try to optimize
-  * them above all else.
-  */
- ssize_t logger_aio_write(**struct** kiocb *iocb, **const****struct** iovec *iov,  
-              unsigned **long** nr_segs, loff_t ppos)  
- {  
- **struct** logger_log *log = file_get_log(iocb->ki_filp);  
- **size_t** orig = log->w_off;  
- **struct** logger_entry header;  
- **struct** timespec now;  
-     ssize_t ret = 0;  
- 
-     now = current_kernel_time();  
- 
-     header.pid = current->tgid;  
-     header.tid = current->pid;  
-     header.sec = now.tv_sec;  
-     header.nsec = now.tv_nsec;  
-     header.len = min_t(**size_t**, iocb->ki_left, LOGGER_ENTRY_MAX_PAYLOAD);  
- 
- /* null writes succeed, return zero */
- **if** (unlikely(!header.len))  
- **return** 0;  
- 
-     mutex_lock(&log->mutex);  
- 
- /*
-      * Fix up any readers, pulling them forward to the first readable
-      * entry after (what will be) the new write offset. We do this now
-      * because if we partially fail, we can end up with clobbered log
-      * entries that encroach on readable buffer.
-      */
-     fix_up_readers(log, **sizeof**(**struct** logger_entry) + header.len);  
- 
-     do_write_log(log, &header, **sizeof**(**struct** logger_entry));  
- 
- **while** (nr_segs-- > 0) {  
- **size_t** len;  
-         ssize_t nr;  
- 
- /* figure out how much of this vector we can keep */
-         len = min_t(**size_t**, iov->iov_len, header.len - ret);  
- 
- /* write out this segment's payload */
-         nr = do_write_log_from_user(log, iov->iov_base, len);  
- **if** (unlikely(nr < 0)) {  
-             log->w_off = orig;  
-             mutex_unlock(&log->mutex);  
- **return** nr;  
-         }  
- 
-         iov++;  
-         ret += nr;  
-     }  
- 
-     mutex_unlock(&log->mutex);  
- 
- /* wake up any blocked readers */
-     wake_up_interruptible(&log->wq);  
- 
- **return** ret;  
- }  
输入的参数iocb表示io上下文，iov表示要写入的内容，长度为nr_segs，表示有nr_segs个段的内容要写入。我们知道，每个要写入的日志的结构形式为：
        struct logger_entry | priority | tag | msg
        其中， priority、tag和msg这三个段的内容是由iov参数从用户空间传递下来的，分别对应iov里面的三个元素。而logger_entry是由内核空间来构造的：
        struct logger_entry header;
struct timespec now;
now = current_kernel_time();
header.pid = current->tgid;
header.tid = current->pid;
header.sec = now.tv_sec;
header.nsec = now.tv_nsec;
header.len = min_t(size_t, iocb->ki_left, LOGGER_ENTRY_MAX_PAYLOAD);
然后调用do_write_log首先把logger_entry结构体写入到日志缓冲区中：
- /*
-  * do_write_log - writes 'len' bytes from 'buf' to 'log'
-  *
-  * The caller needs to hold log->mutex.
-  */
- **static****void** do_write_log(**struct** logger_log *log, **const****void** *buf, **size_t** count)  
- {  
- **size_t** len;  
- 
-     len = min(count, log->size - log->w_off);  
-     memcpy(log->buffer + log->w_off, buf, len);  
- 
- **if** (count != len)  
-         memcpy(log->buffer, buf + len, count - len);  
- 
-     log->w_off = logger_offset(log->w_off + count);  
- 
- }  
       由于logger_entry是内核堆栈空间分配的，直接用memcpy拷贝就可以了。
       接着，通过一个while循环把iov的内容写入到日志缓冲区中，也就是日志的优先级别priority、日志Tag和日志主体Msg：
- **while** (nr_segs-- > 0) {  
- **size_t** len;  
-         ssize_t nr;  
- 
- /* figure out how much of this vector we can keep */
-         len = min_t(**size_t**, iov->iov_len, header.len - ret);  
- 
- /* write out this segment's payload */
-         nr = do_write_log_from_user(log, iov->iov_base, len);  
- **if** (unlikely(nr < 0)) {  
-             log->w_off = orig;  
-             mutex_unlock(&log->mutex);  
- **return** nr;  
-         }  
- 
-         iov++;  
-         ret += nr;  
- }  
由于iov的内容是由用户空间传下来的，需要调用do_write_log_from_user来写入：
[](http://blog.csdn.net/luoshengyang/article/details/6595744)
- **static** ssize_t do_write_log_from_user(**struct** logger_log *log,  
- **const****void** __user *buf, **size_t** count)  
- {  
- **size_t** len;  
- 
-     len = min(count, log->size - log->w_off);  
- **if** (len && copy_from_user(log->buffer + log->w_off, buf, len))  
- **return** -EFAULT;  
- 
- **if** (count != len)  
- **if** (copy_from_user(log->buffer, buf + len, count - len))  
- **return** -EFAULT;  
- 
-     log->w_off = logger_offset(log->w_off + count);  
- 
- **return** count;  
- }  
        这里，我们还漏了一个重要的步骤：
- /*
-   * Fix up any readers, pulling them forward to the first readable
-   * entry after (what will be) the new write offset. We do this now
-   * because if we partially fail, we can end up with clobbered log
-   * entries that encroach on readable buffer.
-   */
- fix_up_readers(log, **sizeof**(**struct** logger_entry) + header.len);  
为什么要调用fix_up_reader这个函数呢？这个函数又是作什么用的呢？是这样的，由于日志缓冲区是循环使用的，即旧的日志记录如果没有及时读取，而缓冲区的内容又已经用完时，就需要覆盖旧的记录来容纳新的记录。而这部分将要被覆盖的内容，有可能是某些reader的下一次要读取的日志所在的位置，以及为新的reader准备的日志开始读取位置head所在的位置。因此，需要调整这些位置，使它们能够指向一个新的有效的位置。我们来看一下fix_up_reader函数的实现：
- /*
-  * fix_up_readers - walk the list of all readers and "fix up" any who were
-  * lapped by the writer; also do the same for the default "start head".
-  * We do this by "pulling forward" the readers and start head to the first
-  * entry after the new write head.
-  *
-  * The caller needs to hold log->mutex.
-  */
- **static****void** fix_up_readers(**struct** logger_log *log, **size_t** len)  
- {  
- **size_t** old = log->w_off;  
- **size_t****new** = logger_offset(old + len);  
- **struct** logger_reader *reader;  
- 
- **if** (clock_interval(old, **new**, log->head))  
-         log->head = get_next_entry(log, log->head, len);  
- 
-     list_for_each_entry(reader, &log->readers, list)  
- **if** (clock_interval(old, **new**, reader->r_off))  
-             reader->r_off = get_next_entry(log, reader->r_off, len);  
- }  
        判断log->head和所有读者reader的当前读偏移reader->r_off是否在被覆盖的区域内，如果是，就需要调用get_next_entry来取得下一个有效的记录的起始位置来调整当前位置：
- /*
-  * get_next_entry - return the offset of the first valid entry at least 'len'
-  * bytes after 'off'.
-  *
-  * Caller must hold log->mutex.
-  */
- **static****size_t** get_next_entry(**struct** logger_log *log, **size_t** off, **size_t** len)  
- {  
- **size_t** count = 0;  
- 
- **do** {  
- **size_t** nr = get_entry_len(log, off);  
-         off = logger_offset(off + nr);  
-         count += nr;  
-     } **while** (count < len);  
- 
- **return** off;  
- }  
        而判断log->head和所有读者reader的当前读偏移reader->r_off是否在被覆盖的区域内，是通过clock_interval函数来实现的：
- /*
-  * clock_interval - is a < c < b in mod-space? Put another way, does the line
-  * from a to b cross c?
-  */
- **static****inline****int** clock_interval(**size_t** a, **size_t** b, **size_t** c)  
- {  
- **if** (b < a) {  
- **if** (a < c || b >= c)  
- **return** 1;  
-     } **else** {  
- **if** (a < c && b >= c)  
- **return** 1;  
-     }  
- 
- **return** 0;  
- }  
        最后，日志写入完毕，还需要唤醒正在等待新日志的reader进程:
        /* wake up any blocked readers */
wake_up_interruptible(&log->wq);
        至此， Logger驱动程序的主要逻辑就分析完成了，还有其它的一些接口，如logger_poll、 logger_ioctl和logger_release函数，比较简单，读取可以自行分析。这里还需要提到的一点是，由于Logger驱动程序模块在退出系统时，是不会卸载的，所以这个模块没有module_exit函数，而对于模块里面定义的对象，也没有用对引用计数技术。
转自：
[http://www.linuxidc.com/Linux/2011-07/38988.htm](http://www.linuxidc.com/Linux/2011-07/38988.htm)

