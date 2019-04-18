# Linux下的C编程实战之四线程 - nosmatch的专栏 - CSDN博客
2011年07月26日 23:25:04[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：599
**1.Linux“线程”**
　　Linux内核只提供了轻量进程的支持，未实现线程模型，但Linux尽最大努力优化了进程的调度开销，这在一定程度上弥补无线程的缺陷。Linux用一个核心进程（轻量进程）对应一个线程，将线程调度等同于进程调度，交给核心完成。
　　笔者曾经在《基于嵌入式操作系统VxWorks的多任务并发程序设计》（《软件报》2006年第5~12期）中详细叙述了进程和线程的区别，并曾经说明Linux是一种“多进程单线程”的操作系统。Linux本身只有进程的概念，而其所谓的“线程”本质上在内核里仍然是进程。大家知道，进程是资源分配的单位，同一进程中的多个线程共享该进程的资源（如作为共享内存的全局变量）。Linux中所谓的“线程”只是在被创建的时候“克隆”(clone)了父进程的资源，因此，clone出来的进程表现为“线程”，这一点一定要弄清楚。因此，Linux“线程”这个概念只有在打冒号的情况下才是最准确的，可惜的是几乎没有书籍留心去强调这一点。
　　目前Linux中最流行的线程机制为LinuxThreads，所采用的就是线程－进程“一对一”模型，调度交给核心，而在用户级实现一个包括信号处理在内的线程管理机制。LinuxThreads由Xavier Leroy (Xavier.Leroy@inria.fr)负责开发完成，并已绑定在GLIBC中发行，它实现了一种BiCapitalized面向Linux的Posix 1003.1c “pthread”标准接口。Linuxthread可以支持Intel、Alpha、MIPS等平台上的多处理器系统。
　　按照POSIX 1003.1c 标准编写的程序与Linuxthread 库相链接即可支持Linux平台上的多线程，在程序中需包含头文件pthread. h，在编译链接时使用命令：
gcc -D -REENTRANT -lpthread xxx. c
　　其中-REENTRANT宏使得相关库函数(如stdio.h、errno.h中函数) 是可重入的、线程安全的(thread-safe)，-lpthread则意味着链接库目录下的libpthread.a或libpthread.so文件。使用Linuxthread库需要2.0以上版本的Linux内核及相应版本的C库(libc 5.2.18、libc 5.4.12、libc 6)。
**2.“线程”控制**
　　线程创建
　　进程被创建时，系统会为其创建一个主线程，而要在进程中创建新的线程，则可以调用pthread_create：
pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *
(start_routine)(void*), void *arg);
　　start_routine为新线程的入口函数，arg为传递给start_routine的参数。
　　每个线程都有自己的线程ID，以便在进程内区分。线程ID在pthread_create调用时回返给创建线程的调用者；一个线程也可以在创建后使用pthread_self()调用获取自己的线程ID：
pthread_self (void) ;
　　线程退出
　　线程的退出方式有三：
　　（1）执行完成后隐式退出；
　　（2）由线程本身显示调用pthread_exit 函数退出；
pthread_exit (void * retval) ;
　　（3）被其他线程用pthread_cance函数终止：
pthread_cance (pthread_t thread) ;
　　在某线程中调用此函数，可以终止由参数thread 指定的线程。
　　如果一个线程要等待另一个线程的终止，可以使用pthread_join函数，该函数的作用是调用pthread_join的线程将被挂起直到线程ID为参数thread的线程终止：
pthread_join (pthread_t thread, void** threadreturn);
