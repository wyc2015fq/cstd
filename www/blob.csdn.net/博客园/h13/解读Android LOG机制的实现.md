# 解读Android LOG机制的实现 - h13 - 博客园
[http://armboard.taobao.com/](http://armboard.taobao.com/)
Android提供了用户级轻量的LOG机制，它的实现贯穿了Java，JNI，本地c/c++实现以及LINUX内核驱动等Android的各个层次，而且足够简单清晰，是一个相当不错的解读案例。本系列文章针对LOG机制的内部实现机理进行解读，本文是系列的第一篇，解读LOG机制的实现架构。
（1）LOG的实现架构
LOG的运行环境
    下图是Android官方网站上给出的Android的Debug环境。
![](https://pic002.cnblogs.com/images/2011/121648/2011093010345079.gif)
    Android的LOG机制当然也在这个环境中运行。我们重点关注Emulator和Device上运行的部分，App VMs产生LOG信息，并与ADB Device Daemon交互输出这些信息，而ADB Device Daemon又通过相应的协议通过USB（Device）或本地连接（Emulator），与PC上运行的ADB Host Daemon交互，通过PC上的调试工具呈现给用户。JDWP Debugger、DDMS、ADB Host Daemon以及ADB Device Daemon之间的交互与其使用的协议，不在本文讨论范围之内。本文讨论的内容运行在Emulator/Device上，产生LOG信息，并通过程序LogCat输出。
LOG的实现架构
    Android中LOG的实现架构如下图所示，这基本上也是Android的某个模块实现各个层次的经典架构。
![](https://pic002.cnblogs.com/images/2011/121648/2011093010352546.gif)
    Android应用程序通过Framework提供的机制操作；Java领域需要本地c/c++提供服务的地方，通过JNI实现；JNI调用底层库；库函数通过操作映射的设备文件操作设备，LINUX kernel中的Driver完成相应的操作。另外，抛开Java和JNI，LINUX上用户域的c/c++程序，也可以通过操作设备文件来完成。 
    Android的LOG也是这样实现的，并将在本系列文章中分别讲述。应用程序通过android.util.Log里的各种静态方法，输出LOG信息[系列之二中具体讲述]；Log通过JNI接口调用c/c++的实现，而本地实现的写LOG，也基本就是写信息到设备文件[系列之三中具体讲述]；设备文件是Android为了LOG机制而写的LINUX的一个轻量级的驱动logger[系列之四中具体讲述]；LOG信息的显示可以是Emulator/Device上运行的LogCat程序[系列之五中具体讲述]；另外，Android的本地实现库也可利用现有机制，在c/c++的空间 直接输出LOG[系列之六中具体讲述]。
（2）JAVA域输出LOG
LOG输出帮助类
Android的Java程序通过android.util.Log类来输出Log，下图列出了我们常用的Log的静态方法。
![加载中...](http://www.2cto.com/statics/images/s_nopic.gif)
一般，要输出Log信息，可直接调用Log.v()/Log.d()/Log.i()/Log.w()/Log.e()等类方法。这里之所以有这么多有区分的方法，这也是Log的分类。Log的分类就如同Log的静态常量成员定义的那样，而Log的优先级按照数字大小排列，数字大的优先级高。而Log.wtf()记录的则是非常致命的FAULT信息（What  a Terrible Failure），报这个错误，不光是在Log里记录，还要在界面上有提示，并可能杀死当前的进程。
有了这些分类，如果要输出的LOG优先级低于当前设置的优先级，则该Log信息不会显示。一般的，在Java程序中用Log的方法打印Log之前，应先用isLoggable()判断一下，该级别是否能被记录。
另外，用Log.println()能达到与Log.v()/Log.d()/…等方法同样的输出效果，只是在用它时，要指定对应的优先级。
类Log的实现
类android.util.Log的实现是比较简单的。
![](https://pic002.cnblogs.com/images/2011/121648/2011093010355484.gif)
类android.util.Log的构造函数是私有的，并不会被实例化，只是提供了静态的属性和方法。
而android.util.Log的各种Log记录方法的实现都依赖于native的实现println_native()，Log.v()/Log.d()/Log.i()/Log.w()/Log.e()最终都是调用了println_native()。如Log.d()的实现：
    public static int d(String tag, String msg) {
        return println_native(LOG_ID_MAIN, DEBUG, tag, msg);
    }
Native方法println_native()是通过JNI在c/c++中实现的，详情参阅本系列之三：JNI及c/c++ 域写设备文件。
（3）JNI及c/c++域写设备文件
类Log的JNI实现
由前文知道，类android.util.Log有两个Native方法，需要通过JNI在c/c++中实现。
<pre class="java" name="code">public static native boolean isLoggable(String tag, int level);
public static native int println_native(int bufID,
            int priority, String tag, String msg);
这两个方法是在frameworks/base/core/jni/android_util_log.cpp中实现的。如何实现JNI的，在这里不做表述。不过最终这两个方法分别转入了下列两个c/c++函数的调用。
static jboolean android_util_Log_isLoggable(JNIEnv* env, jobject clazz, jstring tag, jint level)
static jint android_util_Log_println_native(JNIEnv* env, jobject clazz,
        jint bufID, jint priority, jstring tagObj, jstring msgObj)
isLoggable()的实现
isLoggable的实现是比较<level>（来自参数）与当前property里设定的“log.tag.<tag>”（<tag>来自参数）的值，大于或等于都是可记录的。程序实现片断如下：
    // LOG_NAMESPACE : “log.tag.”
    // chars: convert from param<tag>
    strncpy(key, LOG_NAMESPACE, sizeof(LOG_NAMESPACE)-1);
    strcpy(key + sizeof(LOG_NAMESPACE) - 1, chars);
    len = property_get(key, buf, "");
    int logLevel = toLevel(buf);
    return (logLevel >= 0 && level >= logLevel) ? true : false;
println_native()的实现
函数android_util_Log_println_native() [文件android_util.Log.cpp中]调用了__android_log_buf_write()[文件system/core/liblog/logd_write.c中]。__android_log_buf_write()组织了参数，又调用了write_to_log这个函数指针。
write_to_log这个函数指针是实现的关键。
看write_to_log的定义：
static int __write_to_log_init(log_id_t, struct iovec *vec, size_t nr);
static int (*write_to_log)(log_id_t, struct iovec *vec, size_t nr) = __write_to_log_init;
write_to_log初始是指向__write_to_log_init()这个函数的。所以第一次执行write_to_log的时候是执行了__write_to_log_init()。而如果write_to_log不是第一次被执行，它已经在__write_to_log_init()里被修改指向了__write_to_log_kernel()。
先看__write_to_log_init()的实现：
static int __write_to_log_init(log_id_t log_id, struct iovec *vec, size_t nr)
{
#ifdef HAVE_PTHREADS
    pthread_mutex_lock(&log_init_lock);
#endif
    if (write_to_log == __write_to_log_init) {
        log_fds[LOG_ID_MAIN] = log_open("/dev/"LOGGER_LOG_MAIN, O_WRONLY);
        log_fds[LOG_ID_RADIO] = log_open("/dev/"LOGGER_LOG_RADIO, O_WRONLY);
        log_fds[LOG_ID_EVENTS] = log_open("/dev/"LOGGER_LOG_EVENTS, O_WRONLY);
        log_fds[LOG_ID_SYSTEM] = log_open("/dev/"LOGGER_LOG_SYSTEM, O_WRONLY);
        write_to_log = __write_to_log_kernel;
        if (log_fds[LOG_ID_MAIN] < 0 || log_fds[LOG_ID_RADIO] < 0 ||
                log_fds[LOG_ID_EVENTS] < 0) {
            log_close(log_fds[LOG_ID_MAIN]);
            log_close(log_fds[LOG_ID_RADIO]);
            log_close(log_fds[LOG_ID_EVENTS]);
            log_fds[LOG_ID_MAIN] = -1;
            log_fds[LOG_ID_RADIO] = -1;
            log_fds[LOG_ID_EVENTS] = -1;
            write_to_log = __write_to_log_null;
        }
        if (log_fds[LOG_ID_SYSTEM] < 0) {
            log_fds[LOG_ID_SYSTEM] = log_fds[LOG_ID_MAIN];
        }
    }
#ifdef HAVE_PTHREADS
    pthread_mutex_unlock(&log_init_lock);
#endif
    return write_to_log(log_id, vec, nr);
}
基本上就是做互斥访问的保护，然后如果是第一次调用（write_to_log还指向__write_to_log_init()），就打开相应的设备文件，获取描述符，并把write_to_log指向__write_to_log_kernel()。再在__write_to_log_kernel()中具体执行写入文件操作。
看__write_to_kernel()的实现，基本就是写操作：
static int __write_to_log_kernel(log_id_t log_id, struct iovec *vec, size_t nr)
{
    ssize_t ret;
    int log_fd;
    if (/*(int)log_id >= 0 &&*/ (int)log_id < (int)LOG_ID_MAX) {
        log_fd = log_fds[(int)log_id];
    } else {
        return EBADF;
    }
    do {
        ret = log_writev(log_fd, vec, nr);
    } while (ret < 0 && errno == EINTR);
    return ret;
}
总结一下，println_native()的操作，就是打开设备文件（如果还没打开），然后写入数据。而具体怎么写入的，要看Log的设备驱动Logger的实现。
（4）LOG设备驱动Logger
Log的驱动是在kernel/drivers/staging/android/Logger.c中实现的。
一、初始化
看一个LINUX驱动，先看它如何初始化的。
static int __init init_log(struct logger_log *log)
{
        int ret;
        ret = misc_register(&log->misc);
        if (unlikely(ret)) {
                printk(KERN_ERR "logger: failed to register misc "
                                "device for log '%s'!\n", log->misc.name);
                return ret;
        }
        printk(KERN_INFO "logger: created %luK log '%s'\n",
                       (unsigned long) log->size >> 10, log->misc.name);
        return 0;
}
static int __init logger_init(void)
{
        int ret;
        ret = init_log(&log_main);
        if (unlikely(ret))
                goto out;
        ret = init_log(&log_events);
        if (unlikely(ret))
                goto out;
        ret = init_log(&log_radio);
        if (unlikely(ret))
                goto out;
        ret = init_log(&log_system);
        if (unlikely(ret))
                goto out;
out:
        return ret;
}
device_initcall(logger_init);
整个Logger驱动的入口点就是Logger_init()，它用init_log(struct logger_log *log)初始化了log_main, log_events, log_radio和log_system四个logger_log类型的结构，而这四个结构变量分别记录着log的四个存储体。Logger从这四个变量实现了同种设备的四个驱动，而log的驱动是MISC类型的驱动，通过misc_register()向系统注册。四次注册之后，它们对应的MINOR ID将是不同的，Looger也是通过minor来区分是哪一个驱动的。
static struct logger_log *get_log_from_minor(int minor)
{
        if (log_main.misc.minor == minor)
                return &log_main;
        if (log_events.misc.minor == minor)
                return &log_events;
        if (log_radio.misc.minor == minor)
                return &log_radio;
        if (log_system.misc.minor == minor)
                return &log_system;
        return NULL;
}
本文将以log_main来讲解Logger驱动的实现。
二、关键数据结构
上节中，提到了log_main这个结构体变量，现在来看它的定义。
![](https://pic002.cnblogs.com/images/2011/121648/2011093010362585.gif)
Log_main里保存了Logger操作必须的变量。buffer指向的真是一个静态数组，用来存放用来读写的数据，Logger用它组成了一个逻辑上的循环队列，写者可以往w_off指向的地方写东西，而一旦有内容，会通知等待队列wq里的读者们来读取内容。因为buffer实现的是循环队列，所以buffer的大小size经常用来做除高位的运算，一定要是一个2次幂的数字。mutex用来保护log_main这个关键资源的。Logger是MISC类型的驱动，它保留着一个miscdevice类型的变量misc。misc里面也有最为关键的file_operations结构，这正是应用程序通过文件操作，与驱动打交道的入口。
三、Logger实现的功能
从上面log_main的类型定义就能看出，Logger实现了什么。一句话概括Logger就是实现了读写者，并实现同步操作。不过，Logger的读写者有些特殊，写者写操作不会被阻塞，也不会写满溢出，也就是写时只要有内容可以不停的写，超出Buffer就覆盖旧的[与应用程序具体的写操作结合来看]；读者因为要读的内容为空就会被阻塞挂起，而一旦有内容，所有被挂起的读者都会被唤醒[与应用程序具体的读操作结合来看]。
下面看具体实现的时候，就分别从读者和写者的角度去看。
3.1. 写者的实现
看二小节图中的关键结构logger_fops: file_operations，写者的关键实现就看open、release和write这几个函数的实现了，它们被分别赋值给了logger_open() / logger_release() / logger_aio_write()。
![](https://pic002.cnblogs.com/images/2011/121648/2011093010364357.gif)
logger_open()为写者做的工作就是，通过minor id获得logger_log的实例，然后赋值给函数参数中传递进来的file的private_data中。
logger_release()不需要为写者做的什么工作。
logger_poll()因为写不需要被阻塞。所以这里检测到是因为非因为读而打开的文件（!(file->f_mode &FMODE_READ)）时，就直接返回POLLOUT | POLLWRNORM。无论怎样都可写。
logger_aio_write()是写数据（也就是log信息）的关键。这里是通过异步IO的方法，应用程序通过write()/writev()和aio_write()时都能调用到这个方法。
记录log信息时，写log用的接口是writev()，写的是vec形式的数据，这边写的过程中来的当然也是vec数据了，另外，写具体之间，还写入了类型为logger_entry的数据，来记录时间等信息。写数据到具体buffer时因为存储的位置可能不是连续的，而写在buffer的结尾和开头位置，所以要做判断，并可能要有两次写的buffer的动作。参数里的数据来自用户空间，不能在内核空间直接使用，要用copy_from_user()。写完之后，用wake_up_interruptible(&log->wq)唤醒所有在挂起等待的读者。
3.2. 读者的实现
看二小节图中的关键结构logger_fops: file_operations，写者的关键实现就看open、release和read这几个函数的实现了，它们被分别赋值给了logger_open() / logger_release() / logger_read()。
![](https://pic002.cnblogs.com/images/2011/121648/2011093010370234.gif)
logger_open() 为读者做的工作就是，通过minor id获得logger_log的实例，然后动态申请一个logger_reader类型的读者，并把它加入到logger_log的读者列表readers的结尾，再赋值给函数参数中传递进来的file的private_data中。
logger_release() 与logger_open()对应，将这个读者从读者列表logger_log.readers中移除，并释放掉这个动态申请的实例。
logger_poll()因为应用读之前会调用poll()/select()查看是否可以写。所以这里会用poll_wait()把参数中的poll_table加入到logger_log.wq中，并且如果有内容可读，才设置可读标志|= POLLIN |POLLRDNORM。
logger_read() 是读数据（也就是log信息）的关键。
读数据之前，要先保证有数据，否则该读者就要被挂起在logger_log的等待队列wq上。从具体buffer读数据到时因为存储的位置可能不是连续的，存储在buffer的结尾和开头位置，所以要做判断，并可能要有两次读去buffer的动作。数据来自内核空间，要通过用户空间的参数里传递出去，需要copy_to_user()。
3.3 循环队列的实现
这个是数据结构里最经典的案例了，这里不再具体解释如何实现，只是列出重要结构，只是希望读者还记得数据结构里逻辑结构和物理结构的说法。
队列大小：log_main.size
写头：log_main.w_off
读头：logger_reader.r_off
队列为空判断：log_main.w_off == logger_reader.r_off
队列为满判断：不需要
3.4 ioctl的实现
Logger提供给应用程序通过ioctl()来获取信息或控制LOGbuffer的功能。Logger是把logger_ioctl通过file_operations注册到文件系统中来实现这一功能的。Logger_ioctl()提供了下列ioctl控制命令：LOGGER_GET_LOG_BUF_SIZE / LOGGER_GET_LOG_LEN/ LOGGER_GET_NEXT_ENTRY_LEN / LOGGER_FLUSH_LOG。实现很简单：
LOGGER_GET_LOG_BUF_SIZE获取Buffer的大小，直接返回logger_log.size即可；
LOGGER_GET_LOG_LEN只对读有效，获取当前LOG的大小，存储连续的话就是log->w_off -reader->r_off，否则就是(log->size -reader->r_off) + log->w_off；
LOGGER_GET_NEXT_ENTRY_LEN获取Entry的长度，只对读有效。
LOGGER_FLUSH_LOG只对写打开有效。所谓FLUSH LOG，直接重置每个reader的r_off，并设置新reader要访问用的head即可。
从前文知道，LOG被写入到了驱动的节点，那如何获取这些LOG信息并呈现出来的呢？ANDROID里是有个叫LogCat的应用程序被用来获取LOG信息。LogCat不仅从设备节点处获取LOG，并且还提供了很多选项供用户来过滤、控制输出格式等。本文只讲解如何获取LOG部分，相关的LogCat的使用方式，可参考Android的Logcat命令详解。
LogCat是在文件system/core/logcat/logcat.cpp中实现的。
（5）获取LOG的应用程序LogCat
从Logger设备驱动的实现知道，Log的读取是阻塞的操作，亦即，有数据可用，读出数据；否则，读操作会被BLOCK，相应的读进程也会被挂起等待。下面看应用程序LogCat中如何实现读的，这可能需要不断回头与写操作和驱动实现结合来看。
看具体实现之前，先看一个logcat中定义的重要的结构体log_device_t。其中的重要的成员在后面用到的时候再具体解释。
![](https://pic002.cnblogs.com/images/2011/121648/2011093010372543.gif)
一、打开设备节点
Android的Logcat命令详解的命令参数-b <buffer>知道，logcat是可以通过参数来指定对哪个buffer(main/radio/event)进行操作的。Logcat的b参数解析的地方，是通过传递进来的参数（main/radio/event）来创建了一个上面的结构变量，而这些结构通过log_device_t.next链接起来。
                if (devices) {
                    dev = devices;
                    while (dev->next) {
                        dev = dev->next;
                    }
                    dev->next = new log_device_t(buf, binary, optarg[0]);
                } else {
                    devices = new log_device_t(buf, binary, optarg[0]);
                }
而创建实例的时候的参数被保留了下来，用于后续操作。
<buf>是由LOG_FILE_DIR和optarg（-b参数）组合在一起的（为：“/dev/log/main”，“/dev/log/event”或“/dev/log/radio”），保留在device: char*；
<binary>保留在binary: bool；
<optarg[0]>是-b参数的第一个字符，保存在label: char中。
好了，下面就有了打开设备节点时的参数：
dev->fd = open(dev->device, mode);
dev->device根据-b的参数可能为“/dev/log/main”，“/dev/log/event”或“/dev/log/radio”；
mode缺省时为O_RDONLY，读取。只要在运行logcat时，用了-c参数清除log时才以O_WRONLY打开。
而打开文件的文件操作符保存在log_device_t的fd域中，用于后续的操作。
获取Log的操作都是在readLogLines(log_device_t* devices)中实现的。
因为logcat可能会同时操作多个Buffer，而read()会阻塞读取进程，对其他Buffer的读取就不能进行，所以这里用select()来判断可读取的Buffer。
二、select选取可读取的Buffer
Logcat把log_device_t中的所有的buffer的文件操作符dev->fd，都放在readset中[line#7]，做为select()的里的<readfds: fd_set*>读参数，来获取可读取的Buffer。这样当任何一个Buffer上有LOG数据时，select()都会返回。当然等待过程中也忽略掉其他signal的影响。相应的代码如下：
       fd_set readset;
       do {
            timeval timeout = { 0, 5000 /* 5ms */ }; // If we oversleep it's ok, i.e. ignore EINTR.
            FD_ZERO(&readset);
            for (dev=devices; dev; dev = dev->next) {
                FD_SET(dev->fd, &readset);
            }
            result = select(max + 1, &readset, NULL, NULL, sleep ? NULL : &timeout);
        } while (result == -1 && errno == EINTR);
三、读LOG操作
select()返回之后，通过循环判定dev->fd是否在readset里被设置（FD_ISSET）[line#3]，知道哪个log buffer里已经有数据了。
        if (result >= 0) {
            for (dev=devices; dev; dev = dev->next) {
                if (FD_ISSET(dev->fd, &readset)) {
                    queued_entry_t* entry = new queued_entry_t();
                    /* NOTE: driver guarantees we read exactly one full entry */
                    ret = read(dev->fd, entry->buf, LOGGER_ENTRY_MAX_LEN);
        //…
通过read()读取[line#6]已经有数据的LOG Buffer的文件操作符dev->fd就可得到新到来的log了。
应用程序logcat中已经获取了LOG信息，接下来对数据的处理就都可以在这里进行了，可以过滤，写文件，格式化输入等操作。详细的logcat的命令参数可参见Android的Logcat命令详解.
（6）c/c++域使用LOG
c/c++本地库中实现LOG输出
通过前面的文章知道Android的Java中通过android.util.Log输出Log信息，那Android的本地c/c++程序能不能也通过这样的机制来记录Log呢？再回头看Log现有的c/c++的本地实现，答案当然是肯定的，而且是相当简单。Android直接在头文件（system/core/include/cutils/log.h）里定义了一些宏就可以很好的实现了。
因为，LOG分了VERBOSE/DEBUG/INFO/WARN/ERROR/ASSERT等类别，简单起见，以DEBUG为例的实现来说明。
#ifndef LOGD
#define LOGD(...) LOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif
#ifndef LOGD_IF
#define LOGD_IF(cond, ...) \
    ( (CONDITION(cond)) \
    ? LOG(LOG_DEBUG, LOG_TAG, __VA_ARGS__) \
    : (void)0 )
#endif
#ifndef LOG
#define LOG(priority, tag, ...) \
    LOG_PRI(ANDROID_##priority, tag, __VA_ARGS__)
#endif
#ifndef LOG_PRI
#define LOG_PRI(priority, tag, ...)                                     \
    ({                                                                  \
       if (((priority == ANDROID_LOG_VERBOSE) && (LOG_NDEBUG == 0)) ||  \
           ((priority == ANDROID_LOG_DEBUG) && (LOG_NDDEBUG == 0))  ||  \
           ((priority == ANDROID_LOG_INFO) && (LOG_NIDEBUG == 0))   ||  \
            (priority == ANDROID_LOG_WARN)                          ||  \
            (priority == ANDROID_LOG_ERROR)                         ||  \
            (priority == ANDROID_LOG_FATAL))                            \
                (void)android_printLog(priority, tag, __VA_ARGS__);     \
    })
#endif
#define android_printLog(prio, tag, fmt...) \
__android_log_print(prio, tag, fmt)
而这一系列宏，最后还是用到了函数__android_log_print()
int __android_log_print(int prio, const char *tag, const char *fmt, ...)
{
    va_list ap;
    char buf[LOG_BUF_SIZE];
    va_start(ap, fmt);
    vsnprintf(buf, LOG_BUF_SIZE, fmt, ap);
    va_end(ap);
    return __android_log_write(prio, tag, buf);
}
这里还是调到了函数__android_log_write()。这个函数应该很熟悉吧，正是前文叙及的c/c++本地函数实现写设备文件的地方。
c/c++程序中记录Log的做法
要在c/c++中记录Log通常的做法是：
定义自己的TAG_LOG宏；包含头文件log.h；然后在需要记录Log的地方直接用LOGV/LOGD/LOGI/LOGW/LOGE即可。
比如，文件lights.c中就在开头这样写，
#define LOG_TAG "lights"
#include <cutils/log.h>
然后在该文件的后续部分，大量的用了LOGV/LOGE, etc来记录LOG。
