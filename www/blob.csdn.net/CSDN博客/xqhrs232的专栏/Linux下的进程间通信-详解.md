# Linux下的进程间通信-详解 - xqhrs232的专栏 - CSDN博客
2017年11月16日 15:04:12[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2979
原文地址::[https://www.cnblogs.com/skyofbitbit/p/3651750.html](https://www.cnblogs.com/skyofbitbit/p/3651750.html)
相关文章
1、[Linux下进程通信的八种方法](http://www.cnblogs.com/sooner/archive/2013/05/16/3082758.html)----[https://www.cnblogs.com/sooner/archive/2013/05/16/3082758.html](https://www.cnblogs.com/sooner/archive/2013/05/16/3082758.html)
2、Linux进程间通信——使用信号----[http://blog.csdn.net/ljianhui/article/details/10128731](http://blog.csdn.net/ljianhui/article/details/10128731)
3、Linux进程间通信——使用消息队列----[http://blog.csdn.net/ljianhui/article/details/10287879](http://blog.csdn.net/ljianhui/article/details/10287879)
 详细的讲述进程间通信在这里绝对是不可能的事情，而且笔者很难有信心说自己对这一部分内容的认识达到了什么样的地步，所以在这一节的开头首先向大家推荐著 名作者Richard Stevens的著名作品：《Advanced Programming in the UNIX Environment》，它的中文译本《UNIX环境高级编程》已有机械工业出版社出版，原文精彩，译文同样地道，如果你的确对在Linux下编程有浓 厚的兴趣，那么赶紧将这本书摆到你的书桌上或计算机旁边来。说这么多实在是难抑心中的景仰之情，言归正传，在这一节里，我们将介绍进程间通信最最初步和最
 最简单的一些知识和概念。 
　　 首先，进程间通信至少可以通过传送打开文件来实现，不同的进程通过一个或多个文件来传递信息，事实上，在很多应用系统里，都使用了这种方法。但一般说来， 进程间通信（IPC：InterProcess Communication）不包括这种似乎比较低级的通信方法。Unix系统中实现进程间通信的方法很多，而且不幸的是，极少方法能在所有的Unix系 统中进行移植（唯一一种是半双工的管道，这也是最原始的一种通信方式）。而Linux作为一种新兴的操作系统，几乎支持所有的Unix下常用的进程间通信
 方法：管道、消息队列、共享内存、信号量、套接口等等。下面我们将逐一介绍。 
　　 2.3.1 管道 
　　 管道是进程间通信中最古老的方式，它包括无名管道和有名管道两种，前者用于父进程和子进程间的通信，后者用于运行于同一台机器上的任意两个进程间的通信。 
　　 无名管道由pipe（）函数创建： 
　　 #include <unistd.h> 
　　 int pipe(int filedis[2])； 
　　 参数filedis返回两个文件描述符：filedes[0]为读而打开，filedes[1]为写而打开。filedes[1]的输出是filedes[0]的输入。下面的例子示范了如何在父进程和子进程间实现通信。 
#define INPUT 0 
#define OUTPUT 1 
void main() { 
int file_descriptors[2]; 
/*定义子进程号 */ 
pid_t pid; 
char buf[256]; 
int returned_count; 
/*创建无名管道*/ 
pipe(file_descriptors); 
/*创建子进程*/ 
if((pid = fork()) == -1) { 
printf("Error in fork/n"); 
exit(1); 
} 
/*执行子进程*/ 
if(pid == 0) { 
printf("in the spawned (child) process.../n"); 
/*子进程向父进程写数据，关闭管道的读端*/ 
close(file_descriptors[INPUT]); 
write(file_descriptors[OUTPUT], "test data", strlen("test data")); 
exit(0); 
} else { 
/*执行父进程*/ 
printf("in the spawning (parent) process.../n"); 
/*父进程从管道读取子进程写的数据，关闭管道的写端*/ 
close(file_descriptors[OUTPUT]); 
returned_count = read(file_descriptors[INPUT], buf, sizeof(buf)); 
printf("%d bytes of data received from spawned process: %s/n", 
returned_count, buf); 
} 
} 
　　 在Linux系统下，有名管道可由两种方式创建：命令行方式mknod系统调用和函数mkfifo。下面的两种途径都在当前目录下生成了一个名为myfifo的有名管道： 
　　　　 方式一：mkfifo("myfifo","rw"); 
　　　　 方式二：mknod myfifo p 
生成了有名管道后，就可以使用一般的文件I/O函数如open、close、read、write等来对它进行操作。下面即是一个简单的例子，假设我们已经创建了一个名为myfifo的有名管道。 
　 /* 进程一：读有名管道*/ 
#include <stdio.h> 
#include <unistd.h> 
void main() { 
FILE * in_file; 
int count = 1; 
char buf[80]; 
in_file = fopen("mypipe", "r"); 
if (in_file == NULL) { 
printf("Error in fdopen./n"); 
exit(1); 
} 
while ((count = fread(buf, 1, 80, in_file)) > 0) 
printf("received from pipe: %s/n", buf); 
fclose(in_file); 
} 
　 /* 进程二：写有名管道*/ 
#include <stdio.h> 
#include <unistd.h> 
void main() { 
FILE * out_file; 
int count = 1; 
char buf[80]; 
out_file = fopen("mypipe", "w"); 
if (out_file == NULL) { 
printf("Error opening pipe."); 
exit(1); 
} 
sprintf(buf,"this is test data for the named pipe example/n"); 
fwrite(buf, 1, 80, out_file); 
fclose(out_file); 
} 
　　 2.3.2 消息队列 
　　 消息队列用于运行于同一台机器上的进程间通信，它和管道很相似，是一个在系统内核中用来保存消息的队列，它在系统内核中是以消息链表的形式出现。消息链表中节点的结构用msg声明。
事实上，它是一种正逐渐被淘汰的通信方式，我们可以用流管道或者套接口的方式来取代它，所以，我们对此方式也不再解释，也建议读者忽略这种方式。 
　　 2.3.3 共享内存 
　 　 共享内存是运行在同一台机器上的进程间通信最快的方式，因为数据不需要在不同的进程间复制。通常由一个进程创建一块共享内存区，其余进程对这块内存区进行 读写。得到共享内存有两种方式：映射/dev/mem设备和内存映像文件。前一种方式不给系统带来额外的开销，但在现实中并不常用，因为它控制存取的将是
 实际的物理内存，在Linux系统下，这只有通过限制Linux系统存取的内存才可以做到，这当然不太实际。常用的方式是通过shmXXX函数族来实现利 用共享内存进行存储的。 
　　 首先要用的函数是shmget，它获得一个共享存储标识符。 
　　　　 #include <sys/types.h> 
#include <sys/ipc.h> 
　　　　 #include <sys/shm.h> 
　　　　　 int shmget(key_t key, int size, int flag); 
 这个函数有点类似大家熟悉的malloc函数，系统按照请求分配size大小的内存用作共享内存。Linux系统内核中每个IPC结构都有的一个非负整数 的标识符，这样对一个消息队列发送消息时只要引用标识符就可以了。这个标识符是内核由IPC结构的关键字得到的，这个关键字，就是上面第一个函数的 key。数据类型key_t是在头文件sys/types.h中定义的，它是一个长整形的数据。在我们后面的章节中，还会碰到这个关键字。 
当共享内存创建后，其余进程可以调用shmat（）将其连接到自身的地址空间中。 
　　 void *shmat(int shmid, void *addr, int flag); 
　　 shmid为shmget函数返回的共享存储标识符，addr和flag参数决定了以什么方式来确定连接的地址，函数的返回值即是该进程数据段所连接的实际地址，进程可以对此进程进行读写操作。 
　 　 使用共享存储来实现进程间通信的注意点是对数据存取的同步，必须确保当一个进程去读取数据时，它所想要的数据已经写好了。通常，信号量被要来实现对共享存 储数据存取的同步，另外，可以通过使用shmctl函数设置共享存储内存的某些标志位如**SHM_LOCK、SHM_UNLOCK**等来实现。 
　　 2.3.4 信号量 
　　 信号量又称为信号灯，它是用来协调不同进程间的数据对象的，而最主要的应用是前一节的共享内存方式的进程间通信。本质上，信号量是一个计数器，它用来记录对某个资源（如共享内存）的存取状况。一般说来，为了获得共享资源，进程需要执行下列操作： 
　　 （1） 测试控制该资源的信号量。 
　　 （2） 若此信号量的值为正，则允许进行使用该资源。进程将信号量减1。 
　　 （3） 若此信号量为0，则该资源目前不可用，进程进入睡眠状态，直至信号量值大于0，进程被唤醒，转入步骤（1）。 
　　 （4） 当进程不再使用一个信号量控制的资源时，信号量值加1。如果此时有进程正在睡眠等待此信号量，则唤醒此进程。 
 维护信号量状态的是Linux内核操作系统而不是用户进程。我们可以从头文件/usr/src/linux/include　/linux　/sem.h 中看到内核用来维护信号量状态的各个结构的定义。信号量是一个数据集合，用户可以单独使用这一集合的每个元素。要调用的第一个函数是semget，用以获
 得一个信号量ID。 
struct sem {
  short sempid;/* pid of last operaton */
  ushort semval;/* current value */
  ushort semncnt;/* num procs awaiting increase in semval */
  ushort semzcnt;/* num procs awaiting semval = 0 */
}
　　 #include <sys/types.h> 
　　 #include <sys/ipc.h> 
　　 #include <sys/sem.h> 
　　 int semget(key_t key, int nsems, int flag); 
key是前面讲过的IPC结构的关键字，flag将来决定是创建新的信号量集合，还是引用一个现有的信号量集合。nsems是该集合中的信号量数。如果是创建新 集合（一般在服务器中），则必须指定nsems；如果是引用一个现有的信号量集合（一般在客户机中）则将nsems指定为0。 
　　 semctl函数用来对信号量进行操作。 
　　 int semctl(int semid, int semnum, int cmd, union semun arg); 
　　 不同的操作是通过cmd参数来实现的，在头文件sem.h中定义了7种不同的操作，实际编程时可以参照使用。 
     semop函数自动执行信号量集合上的操作数组。 
　　 int semop(int semid, struct sembuf semoparray[], size_t nops); 
　　 semoparray是一个指针，它指向一个信号量操作数组。nops规定该数组中操作的数量。 
　　 下面，我们看一个具体的例子，它创建一个特定的IPC结构的关键字和一个信号量，建立此信号量的索引，修改索引指向的信号量的值，最后我们清除信号量。在下面的代码中，函数ftok生成我们上文所说的唯一的IPC关键字。 
#include <stdio.h> 
#include <sys/types.h> 
#include <sys/sem.h> 
#include <sys/ipc.h> 
void main() { 
key_t unique_key; /* 定义一个IPC关键字*/ 
int id; 
struct sembuf lock_it; 
union semun options; 
int i; 
unique_key = ftok(".", 'a'); /* 生成关键字，字符'a'是一个随机种子*/ 
/* 创建一个新的信号量集合*/ 
id = semget(unique_key, 1, IPC_CREAT | IPC_EXCL | 0666); 
printf("semaphore id=%d/n", id); 
options.val = 1; /*设置变量值*/ 
semctl(id, 0, SETVAL, options); /*设置索引0的信号量*/ 
/*打印出信号量的值*/ 
i = semctl(id, 0, GETVAL, 0); 
printf("value of semaphore at index 0 is %d/n", i); 
/*下面重新设置信号量*/ 
lock_it.sem_num = 0; /*设置哪个信号量*/ 
lock_it.sem_op = -1; /*定义操作*/ 
lock_it.sem_flg = IPC_NOWAIT; /*操作方式*/ 
if (semop(id, &lock_it, 1) == -1) { 
printf("can not lock semaphore./n"); 
exit(1); 
} 
i = semctl(id, 0, GETVAL, 0); 
printf("value of semaphore at index 0 is %d/n", i); 
/*清除信号量*/ 
semctl(id, 0, IPC_RMID, 0); 
} 
**semget()**
     可以使用系统调用semget()创建一个新的信号量集，或者存取一个已经存在的信号量集：
系统调用：semget();
原型：intsemget(key_t key,int nsems,int semflg);
返回值：如果成功，则返回信号量集的IPC标识符。如果失败，则返回-1：errno=EACCESS(没有权限)
EEXIST(信号量集已经存在，无法创建)
EIDRM(信号量集已经删除)
ENOENT(信号量集不存在，同时没有使用IPC_CREAT)
ENOMEM(没有足够的内存创建新的信号量集)
ENOSPC(超出限制)
    系统调用semget()的第一个参数是关键字值（一般是由系统调用ftok()返回的）。系统内核将此值和系统中存在的其他的信号量集的关键字值进行比较。打开和存取操作与参数semflg中的内容相关。IPC_CREAT如果信号量集在系统内核中不存在，则创建信号量集。IPC_EXCL当和 IPC_CREAT一同使用时，如果信号量集已经存在，则调用失败。如果单独使用IPC_CREAT，则semget()要么返回新创建的信号量集的标识符，要么返回系统中已经存在的同样的关键字值的信号量的标识符。如果IPC_EXCL和IPC_CREAT一同使用，则要么返回新创建的信号量集的标识符，要么返回-1。IPC_EXCL单独使用没有意义。参数nsems指出了一个新的信号量集中应该创建的信号量的个数。信号量集中最多的信号量的个数是在linux/sem.h中定义的：
#defineSEMMSL32/*<=512maxnumofsemaphoresperid*/
下面是一个打开和创建信号量集的程序：
intopen_semaphore_set(key_t keyval,int numsems)
{
intsid;
if(!numsems)
return(-1);
if((sid=semget(mykey,numsems,IPC_CREAT|0660))==-1)
{
return(-1);
}
return(sid);
}
};
==============================================================
**semop()**
系统调用：semop();
调用原型：int semop(int semid,struct sembuf*sops,unsign ednsops);
返回值：0，如果成功。-1，如果失败：errno=E2BIG(nsops大于最大的ops数目)
EACCESS(权限不够)
EAGAIN(使用了IPC_NOWAIT，但操作不能继续进行)
EFAULT(sops指向的地址无效)
EIDRM(信号量集已经删除)
EINTR(当睡眠时接收到其他信号)
EINVAL(信号量集不存在,或者semid无效)
ENOMEM(使用了SEM_UNDO,但无足够的内存创建所需的数据结构)
ERANGE(信号量值超出范围)
    第一个参数是关键字值。第二个参数是指向将要操作的数组的指针。第三个参数是数组中的操作的个数。参数sops指向由sembuf组成的数组。此数组是在linux/sem.h中定义的：
/*semop systemcall takes an array of these*/
structsembuf{
ushortsem_num;/*semaphore index in array*/
shortsem_op;/*semaphore operation*/
shortsem_flg;/*operation flags*/
sem_num将要处理的信号量的个数。
sem_op要执行的操作。
sem_flg操作标志。
    如果sem_op是负数，那么信号量将减去它的值。这和信号量控制的资源有关。如果没有使用IPC_NOWAIT，那么调用进程将进入睡眠状态，直到信号量控制的资源可以使用为止。如果sem_op是正数，则信号量加上它的值。这也就是进程释放信号量控制的资源。最后，如果sem_op是0，那么调用进程将调用sleep()，直到信号量的值为0。这在一个进程等待完全空闲的资源时使用。
===============================================================
**semctl()**
系统调用：semctl();
原型：int semctl(int semid,int semnum,int cmd,union semunarg);
返回值：如果成功，则为一个正数。
如果失败，则为-1：errno=EACCESS(权限不够)
EFAULT(arg指向的地址无效)
EIDRM(信号量集已经删除)
EINVAL(信号量集不存在，或者semid无效)
EPERM(EUID没有cmd的权利)
ERANGE(信号量值超出范围)
    系统调用semctl用来执行在信号量集上的控制操作。这和在消息队列中的系统调用msgctl是十分相似的。但这两个系统调用的参数略有不同。因为信号量一般是作为一个信号量集使用的，而不是一个单独的信号量。所以在信号量集的操作中，不但要知道IPC关键字值，也要知道信号量集中的具体的信号量。这两个系统调用都使用了参数cmd，它用来指出要操作的具体命令。两个系统调用中的最后一个参数也不一样。在系统调用msgctl中，最后一个参数是指向内核中使用的数据结构的指针。我们使用此数据结构来取得有关消息队列的一些信息，以及设置或者改变队列的存取权限和使用者。但在信号量中支持额外的可选的命令，这样就要求有一个更为复杂的数据结构。
系统调用semctl()的第一个参数是关键字值。第二个参数是信号量数目。
    参数cmd中可以使用的命令如下：
    ·IPC_STAT读取一个信号量集的数据结构semid_ds，并将其存储在semun中的buf参数中。
    ·IPC_SET设置信号量集的数据结构semid_ds中的元素ipc_perm，其值取自semun中的buf参数。
    ·IPC_RMID将信号量集从内存中删除。
    ·GETALL用于读取信号量集中的所有信号量的值。
    ·GETNCNT返回正在等待资源的进程数目。
    ·GETPID返回最后一个执行semop操作的进程的PID。
    ·GETVAL返回信号量集中的一个单个的信号量的值。
    ·GETZCNT返回这在等待完全空闲的资源的进程数目。
    ·SETALL设置信号量集中的所有的信号量的值。
    ·SETVAL设置信号量集中的一个单独的信号量的值。
    参数arg代表一个semun的实例。semun是在linux/sem.h中定义的：
/*arg for semctl systemcalls.*/
unionsemun{
intval;/*value for SETVAL*/
structsemid_ds*buf;/*buffer for IPC_STAT&IPC_SET*/
ushort*array;/*array for GETALL&SETALL*/
structseminfo*__buf;/*buffer for IPC_INFO*/
void*__pad;
    val当执行SETVAL命令时使用。buf在IPC_STAT/IPC_SET命令中使用。代表了内核中使用的信号量的数据结构。array在使用GETALL/SETALL命令时使用的指针。
    下面的程序返回信号量的值。当使用GETVAL命令时，调用中的最后一个参数被忽略：
intget_sem_val(intsid,intsemnum)
{
return(semctl(sid,semnum,GETVAL,0));
}
    下面是一个实际应用的例子：
#defineMAX_PRINTERS5
printer_usage()
{
int x;
for(x=0;x<MAX_PRINTERS;x++)
printf("Printer%d:%d/n/r",x,get_sem_val(sid,x));
}
    下面的程序可以用来初始化一个新的信号量值：
void init_semaphore(int sid,int semnum,int initval)
{
union semunsemopts;
semopts.val=initval;
semctl(sid,semnum,SETVAL,semopts);
}
    注意系统调用semctl中的最后一个参数是一个联合类型的副本，而不是一个指向联合类型的指针。
　　 2.3.5 套接口 
　 　 套接口（socket）编程是实现Linux系统和其他大多数操作系统中进程间通信的主要方式之一。我们熟知的WWW服务、FTP服务、TELNET服务 等都是基于套接口编程来实现的。除了在异地的计算机进程间以外，套接口同样适用于本地同一台计算机内部的进程间通信。关于套接口的经典教材同样是 Richard Stevens编著的《Unix网络编程：联网的API和套接字》，清华大学出版社出版了该书的影印版。它同样是Linux程序员的必备书籍之一。 
　 　 关于这一部分的内容，可以参照本文作者的另一篇文章《设计自己的网络蚂蚁》，那里由常用的几个套接口函数的介绍和示例程序。这一部分或许是Linux进程 间通信编程中最须关注和最吸引人的一部分，毕竟，Internet 正在我们身边以不可思议的速度发展着，如果一个程序员在设计编写他下一个程序的时候，根本没有考虑到网络，考虑到Internet，那么，可以说，他的设 计很难成功。 
3 Linux的进程和Win32的进程/线程比较 
　　 熟悉WIN32编程的人一定知道，WIN32的进程管理方式与Linux上有着很大区别，在UNIX里，只有进程的概念，但在WIN32里却还有一个"线程"的概念，那么Linux和WIN32在这里究竟有着什么区别呢？ 
　 　 WIN32里的进程/线程是继承自OS/2的。在WIN32里，"进程"是指一个程序，而"线程"是一个"进程"里的一个执行"线索"。从核心上讲， WIN32的多进程与Linux并无多大的区别，在WIN32里的线程才相当于Linux的进程，是一个实际正在执行的代码。但是，WIN32里同一个进 程里各个线程之间是共享数据段的。这才是与Linux的进程最大的不同。 
　　 下面这段程序显示了WIN32下一个进程如何启动一个线程。 
int g; 
DWORD WINAPI ChildProcess( LPVOID lpParameter ){ 
int i; 
for ( i = 1; i <1000; i ++) { 
g ++; 
printf( "This is Child Thread: %d/n", g ); 
} 
ExitThread( 0 ); 
}; 
void main() 
{ 
int threadID; 
int i; 
g = 0; 
CreateThread( NULL, 0, ChildProcess, NULL, 0, &threadID ); 
for ( i = 1; i <1000; i ++) { 
g ++; 
printf( "This is Parent Thread: %d/n", g ); 
} 
} 
　 　 在WIN32下，使用CreateThread函数创建线程，与Linux下创建进程不同，WIN32线程不是从创建处开始运行的，而是由 CreateThread指定一个函数，线程就从那个函数处开始运行。此程序同前面的UNIX程序一样，由两个线程各打印1000条信息。 threadID是子线程的线程号，另外，全局变量g是子线程与父线程共享的，这就是与Linux最大的不同之处。大家可以看出，WIN32的进程/线程 要比Linux复杂，在Linux要实现类似WIN32的线程并不难，只要fork以后，让子进程调用ThreadProc函数，并且为全局变量开设共享
 数据区就行了，但在WIN32下就无法实现类似fork的功能了。所以现在WIN32下的C语言编译器所提供的库函数虽然已经能兼容大多数 Linux/UNIX的库函数，但却仍无法实现fork。 
　　 对于多任务系统，共享数据区是必要的，但也是一个容易引起混乱的问题，在WIN32下，一个程序员很容易忘记线程之间的数据是共享的这一情况，一个线程修 改过一个变量后，另一个线程却又修改了它，结果引起程序出问题。但在Linux下，由于变量本来并不共享，而由程序员来显式地指定要共享的数据，使程序变 得更清晰与安全。 
至于WIN32的"进程"概念，其含义则是"应用程序"，也就是相当于UNIX下的exec了。 
　　 Linux也有自己的多线程函数pthread，它既不同于Linux的进程，也不同于WIN32下的进程，关于pthread的介绍和如何在Linux环境下编写多线程程序我们将在另一篇文章《Linux下的多线程编程》中讲述。  
分类: [Linux多线程多进程，同步异步，进程间通信](http://www.cnblogs.com/skyofbitbit/category/567755.html)
