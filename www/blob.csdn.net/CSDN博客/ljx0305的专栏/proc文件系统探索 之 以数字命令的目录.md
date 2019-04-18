# proc文件系统探索 之 以数字命令的目录 - ljx0305的专栏 - CSDN博客
2012年11月07日 17:21:09[ljx0305](https://me.csdn.net/ljx0305)阅读数：2215
以数字命名的目录表示当前一个运行的进程，目录名即为进程的pid。怒录内的目录和文件给出了一些关于该进程的一些信息。
```
niutao@niutao-desktop:/proc/6584$ ls
attr        coredump_filter  fd        maps        oom_score  statm
auxv        cpuset           fdinfo    mem         root       status
cgroup      cwd              io        mounts      sched      task
clear_refs  environ          limits    mountstats  smaps      wchan
cmdline     exe              loginuid  oom_adj     stat
```
我们可以看到该目录下有这么些文件。其中attr、fd、fdinfo、task为目录，cwd、root为指向目录的链接，exe为指向文件的链接，其余为一般的文件。对于一些文件或目录操纵的权限(查看或者修改的权限)是该进程的创建者才有，例如auxv、envion、fd、fdinfo、limits、mem、mountstats等文件或目录只有创建该进程的用户才具有查看或者进入的权限，而其他一些文件则对所有用户具有可读权限。关于这些文件或目录的权限，我们可以在内核中找到(fs/proc/base.c tid_base_stuff数组)。下我们来详细探讨每一个文件或目录的作用.
  1．cmdline文件：
该文件中包含的是该进程的命令行参数，包括进程的启动路径(argv[0])。也就是说例如你在命令行上运行一个hello程序:
```
niutao@niutao-desktop:~$
 cat hello.c
#include<stdio.h>
#include<wait.h>
int main()
{
    int i;
    for(i=0;i<100;i++)
{
        printf("Hello world\n");
        sleep(2);
    }
    return 0;
}
niutao@niutao-desktop:~$ gcc
-o hello hello.c
niutao@niutao-desktop:~$
./hello one two
niutao@niutao-desktop:~$ ps
-A |grep hello
 7282 pts/4 00:00:00 hello
niutao@niutao-desktop:~$ cd
/proc/7282/
niutao@niutao-desktop:/proc/7282$
 cat cmdline
./helloonetwoniutao@niutao-desktop:/proc/7282$
```
可以看到cmdline里的内容为"./helloonetwo"，正是命令行的参数。可能你会疑问为什幺参数没有分开？呵呵，那是因为cat欺骗了你。我们可以做一个实验，将该cmdline文件复制到你的用户目录下，如果使用vim查看就会发现是这样:
 ./hello^@one^@two^@ 
也就是说实际每个参数之间是有东西隔开的，之不过cat将其忽略了而已，而vim可以给你标识出有东西，但vim本身不可显示罢了。我们可以通过编程读取该文件。下面给出我写的一个读取该文件的小程序。
我们一个字符一个字符的读取文件内容直到文件结束，在读取没一个字符的时候，打印其字符和对应的数值：
```
niutao@niutao-desktop:~/c$
 cat readcmd.c
#include<stdio.h>
#include<string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int args,char
*argv[])
{
    FILE *fp;
    char path[80];
    unsigned char ch;
    snprintf(path,80,"/home/niutao/cmdline");
    if((fp=fopen(path,"r"))==NULL)
    {
        perror("fopen");
        return 0;
    }
    while(!feof(fp))
{
        ch=fgetc(fp);
        printf("%c %d\n",ch,ch);
    }
    fclose(fp);
    return 0;
}
niutao@niutao-desktop:~/c$
 gcc -o readcmd readcmd.c
niutao@niutao-desktop:~/c$
./readcmd
. 46
/ 47
h 104
e 101
l 108
l 108
o 111
 0
o 111
n 110
e 101
 0
t 116
w 119
o 111
 0
```
3．cmd目录链接:
该目录链接指向该进程运行的当前路径。该符号链接虽然使用ls命令查看其权限是对所有用户都有权限，但实际中是只有启动该进程的用户才具有读写的权限，其他用户不具有一切权限。该链接指向该进程运行的当前路径，例如我们在用户目录下启动该进程，那么cwd就指向用户目录。
4．environ文件:
    该进程运行的环境变量。我们常用的一些环境变量都包含在该文件中，例如PATH,HOME,PWD等。所以如果我们想在一个进程中获取这些环境变量而又不想使用getenv(),getpwd函数的时候，我们就可以通过直接读取该进程的该文件以直接获得环境变量。下面是一个程序实现这个过程：
```
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include <fcntl.h>
int main(int args,char
*argv[])
{
    FILE *fp;
    int pid;
    char path[80];
    int i=0;
    unsigned char buff[1024];
    unsigned char *p=NULL;
    unsigned char ch;
    if(args<2)
{
        printf("no argument\n");
        return 0;
    }
    pid=getpid();
    snprintf(path,80,"/proc/%d/environ",pid);
    if((fp=fopen(path,"r"))==NULL)
{
        perror("fopen");
        return 0;
    }
    while(!feof(fp))
{
        if((ch=fgetc(fp))!='\0')
{
            buff[i]=ch;
            i++;
            continue;
        }
        buff[i]='\0';
        if((p=strstr(buff,argv[1]))!=NULL)
{
            printf("%s\n",p+strlen(argv[1])+1);
            return 0;
        }
        i=0;
        memset(buff,'\0',1024);
    }
    fclose(fp);
    return 0;
}
niutao@niutao-desktop:~/c$
 gcc -o getenv getenv.c
niutao@niutao-desktop:~/c$
./getenv HOME
/home/niutao
```
5．exe链接文件:
    指向该进程相应的可执行文件。从这里我们可以看到该进程的可执行文件所在路径，例如：
niutao@niutao-desktop:/proc/8070$ ls -l exe
lrwxrwxrwx 1 niutao niutao 0 2008-10-21 22:01 exe -> /bin/bash
niutao@niutao-desktop:/proc/8070$
我们可以看到pid等于8070的是一个bash程序，其所在路径为/bin。
6．maps文件和smaps文件
maps 文件是可执行文件或者库文件对应的内存映像，而smaps文件显示的是该进程这些可执行我呢件或者库我呢件随应内存映像在内存中的大小等信息。在 GNOME桌面下的gnome-system-monitor中，我们选中一个进程，右键Memory Maps，其中显示的内容就来自这两个文件的信息。
  首先我们来看maps文件:
```
niutao@niutao-desktop:/proc/6740$
 cat maps
08048000-080ef000 r-xp 00000000 08:0d 636485
/bin/bash
080ef000-080f5000 rw-p 000a6000 08:0d 636485
/bin/bash
080f5000-08337000 rw-p 080f5000 00:00 0
[heap]
b7c91000-b7cd0000 r--p 00000000 08:0d 1183449
/usr/lib/locale/en_US.utf8/LC_CTYPE
b7cd0000-b7cd9000 r-xp 00000000 08:0d 1111788
/lib/tls/i686/cmov/libnss_files-2.7.so
b7cd9000-b7cdb000 rw-p 00008000 08:0d 1111788
/lib/tls/i686/cmov/libnss_files-2.7.so
b7cdb000-b7ce3000 r-xp 00000000 08:0d 1111790
/lib/tls/i686/cmov/libnss_nis-2.7.so
b7ce3000-b7ce5000 rw-p 00007000 08:0d 1111790
/lib/tls/i686/cmov/libnss_nis-2.7.so
。。。。
```
我们可以看到pid等于6740的进程的内存映像。其中从0x08048000到0x080ef000是/bin/bash文件的内存映像，其在虚拟内存中的偏移为0x00000000，该文件的inode为636485，其访问标志为r-xp，即可读，可执行，私有的。由此我们可以猜出此为/bin/bash可执行文件的程序段。而紧接着一个从0x80ef000到0x080f5000也是/bin/bash文件的内存映像，从其访问标志rw-p可以看出其可读，可写，私有。我们推测其为数据段。08:0d表示文件所在的设备的主设备号和次设备号，比如这个08:0d，其中08"为主设备号，表示该文件所在的设备是一个SCSI类型的硬盘(更详细的设备类型标号参见/linux/Documentation/devices.txt文件)，"0d"表示次设备号为13，所以我们可以知道该文件/bin/bash的设备驱动是/dev/sda13。对于共享库，各共享库的数据段，存放着程序执行所需的全局变量，是由kernel把ELF文件的数据段map到虚存空间；用户代码段，存放着二进制形式的可执行的机器指令，是由kernel把ELF文件的代码段map到虚存空间；用户数据段之上是代码段，存放着程序执行所需的全局变量，是由kernel把ELF文件的数据段map到虚存空间；用户数据段之下是堆(heap)，当且仅当malloc调用时存在，是由kernel把匿名内存map到虚存空间，堆则在程序中没有调用malloc的情况下不存在；用户数据段之下是栈(stack)，作为进程的临时数据区，是由kernel把匿名内存map到虚存空间，栈空间的增长方向是从高地址到低地址。maps文件列出的内容基本都是这样的形式，即对一个被当前进程使用的文件都分别列出其程序段和数据段。
  现在我们再来看一些smaps文件:
```
niutao@niutao-desktop:/proc/6740$
 cat smaps
08048000-080ef000 r-xp 00000000 08:0d 636485
/bin/bash
Size: 668 kB
Rss: 584 kB
Shared_Clean: 584 kB
Shared_Dirty: 0 kB
Private_Clean: 0 kB
Private_Dirty: 0 kB
Referenced: 584 kB
080ef000-080f5000 rw-p 000a6000 08:0d 636485
/bin/bash
Size: 24 kB
Rss: 24 kB
Shared_Clean: 0 kB
Shared_Dirty: 0 kB
Private_Clean: 0 kB
Private_Dirty: 24 kB
Referenced: 24 kB
。。。。
```
我们可以看到其显示的信息比maps文件更详细。对于内存映像的每一个段它都列出该段的详细信息，如大小等。
    所以我们可以结合这两个文件获取该进程的完整的内存映像信息。
7．fd目录fdinfo目录
fd目录包含了所有该进程使用的文件描述符,而fdinfo目录包含的是对应的fd目录中进程打开文件的操作权限。
```
niutao@niutao-desktop:/proc/6772/fd$
 ls -l
lrwx------ 1 niutao niutao 64 2008-10-22
 21:32 0 ->
/dev/pts/5
lrwx------ 1 niutao niutao 64 2008-10-22
 21:32 1 ->
/dev/pts/5
lrwx------ 1 niutao niutao 64 2008-10-22
 21:32 2 ->
/dev/pts/5
niutao@niutao-desktop:/proc/6772/fd$
 cd ../fdinfo/
niutao@niutao-desktop:/proc/6772/fdinfo$
 cat 0
pos:    0
flags:    02
```
我们可以看出fd目录中包含的是进程打开文件的链接，这里我们就可以看到我们经常提到的标准输入(0)，标准输出(1)，标准错误输出(2)。那么fdinfo中包含的文件的含义，我们可以从这两个方面探索。一个是内核中的 proc_fd_info函数(/fs/proc/base.c)，fdinfo目录中的文件中的内容正是由这个函数写的，而flags正对应的是文件结构体(struct file)的f_flags域(知识我呢件的访问权限)。另一个是我们可以通过查看fd目录中包含的符号链接文件指向的文件的权限得知其(fdinfo目录中文件内容)含义：
```
niutao@niutao-desktop:/proc/6772/fd$
 ls -l /dev/pts/5
crw--w---- 1 niutao tty 136,
 5 2008-10-22 21:32
/dev/pts/5
```
"flags:02"表示文件访问权限是O_RDWR（可读可写），而fd中符号链接文件指向的文件/dev/pts/5对用户的权限也是可读可写。
8．root符号链接文件
该文件指向的是根目录(/)。
9．stat文件
该文件的内容反应的是该进程的PCB(task_struct结构)的一些数据域的信息。下面我们来具体看一下它的含义。首先我们在终端上启动gedit程序，然后使用系统监视器(gnome-system-monitor)查看gedit进程的pid为11942，然后我们读取它的stat文件
```
niutao@niutao-desktop:/proc/11942$
 cat stat
11942 (gedit) S 7293 11942 7293 34820 11942 4202496 5017 0 0 0 80 10 0 0 20 0 1 0 1292037 61636608 4481 4294967295 134512640 135099420 3216990608 3216990068 3085931536 0 0 4096 0 0 0 0 17 0 0
 0 0 0 0
```
在内核中，该文件的内容由do_task_stat函数(fs/proc/array.c)写。主要操作是：
```
sprintf(buffer,
"%d (%s) %c %d %d %d %d %d %u %lu \
%lu %lu %lu %lu %lu %ld %ld %ld %ld %d 0 %llu %lu %ld %lu %lu %lu %lu %lu \
%lu %lu %lu %lu %lu %lu %lu %lu %d %d %u %u %llu %lu %ld\n",
  task_pid_nr_ns(task, ns),
/*进程(包括轻量级进程，即线程)号(task->pid)*/
  tcomm, /*应用程序或命令的名字(task->comm)*/
  state,/*进程的状态信息(task->state),具体参见http://blog.chinaunix.net/u2/73528/showart_1106510.html*/
  ppid,/*父进程ID*/
  pgid,/*线程组ID*/
  sid,/*会话组ID*/
  tty_nr,/*该进程的tty终端的设备号，INT（34817/256）=主设备号，（34817-主设备号）=次设备号*/
  tty_pgrp,/*终端的进程组号，当前运行在该进程所在终端的前台进程(包括shell 应用程序)的PID*/
  task->flags,/*进程标志位，查看该进程的特性(定义在/include/kernel/sched.h中)*/
  min_flt,/*累计进程的次缺页数（Copy on　Write页和匿名页）*/
  cmin_flt,/*该进程所有的子进程发生的次缺页的次数*/
  maj_flt,/*主缺页数（从映射文件或交换设备读入的页面数）*/
  cmaj_flt,/*该进程所有的子进程发生的主缺页的次数*/
  cputime_to_clock_t(utime),/*该进程在用户态运行的时间，单位为jiffies*/
  cputime_to_clock_t(stime),/*该进程在核心态运行的时间，单位为jiffies*/
  cputime_to_clock_t(cutime),/*该进程所有的子进程在用户态运行的时间总和，单位为jiffies*/
  cputime_to_clock_t(cstime),/*该进程所有的子进程在内核态运行的时间的总和，单位为jiffies*/
  priority,/*进程的动态优先级*/
  nice,/*进程的静态优先级*/.
  num_threads,/*该进程所在的线程组里线程的个数*/.
  start_time,/*该进程创建的时间*/.
  vsize,/*该进程的虚拟地址空间大小*/.
  mm ? get_mm_rss(mm)
: 0,/*该进程当前驻留物理地址空间的大小*/.
  rsslim,/*该进程能驻留物理地址空间的最大值*/.
  mm ? mm->start_code
: 0,/*该进程在虚拟地址空间的代码段的起始地址*/.
  mm ? mm->end_code
: 0,/*该进程在虚拟地址空间的代码段的结束地址*/.
  mm ? mm->start_stack
: 0,/*该进程在虚拟地址空间的栈的结束地址*/.
  esp,/*esp(32 位堆栈指针) 的当前值, 与在进程的内核堆栈页得到的一致*/.
  eip,/*指向将要执行的指令的指针, EIP(32 位指令指针)的当前值*/.
  /* The signal information here is obsolete.
   * It must be decimal 
for Linux 2.0 compatibility.
   * Use 
/proc/#/status for real-time signals.
   */
  task->pending.signal.sig[0]
& 0x7fffffffUL,/*待处理信号的位图，记录发送给进程的普通信号*/.
  task->blocked.sig[0]
& 0x7fffffffUL,/*阻塞信号的位图*/.
  sigign .sig[0]
& 0x7fffffffUL,/*忽略的信号的位图*/.
  sigcatch .sig[0]
& 0x7fffffffUL,/*被俘获的信号的位图*/.
  wchan,/*如果该进程是睡眠状态，该值给出调度的调用点*/.
  0UL,/*被swapped的页数,当前没用*/.
  0UL,/*所有子进程被swapped的页数的和，当前没用*/.
  task->exit_signal,/*该进程结束时，向父进程所发送的信号*/.
  task_cpu(task),/*运行在哪个CPU上*/.
  task->rt_priority,/*实时进程的相对优先级别*/.
  task->policy,/*进程的调度策略，0=非实时进程，1=FIFO实时进程；2=RR实时进程*/.
  (unsigned long long)delayacct_blkio_ticks(task),/**/.
  cputime_to_clock_t(gtime),/**/.
  cputime_to_clock_t(cgtime));/**/.
```
由以上解释我们可以知道该进程的pid为11942,可执行程序名为gedit，当前正处于睡眠状态，其父进程pid为7293，按理说应该是一个终端(因为我们是在终端上启动gedit的)，那么我们来验证一下：
```
bashniutao@niutao-desktop:/proc/7293$
 cat stat
7293 (bash) S 7095 7293 7293 34820 7293 4194304 2902 40892 1 166 16 2 600 64 20 0 1 0 509398 6619136 918 4294967295 134512640 135194160 3214795824 3214792344 3086427152 0 0 3686404 1266761467
 0 0 0 17 0 0 0 0 0 0
```
可以看到pid等于7293的进程其可执行程序名的确是bash，所以它就是一个终端，并且是11942(gedit)的父进程。我们接着看11942号进程。其所在线程组id为11942，会话组id都为7293，所以如果我们关闭pid为7293的终端，则11942号进程也会被关闭。它的tty终端下启动的，所以终端设备号是一个有效值(如
果是双击启动的，那么该项0，也就是说该程序不是在终端下启动的)。终端的进程组号为7293，该进程的标志为4202496，对应十六进制为0x402000。对于进程的标志，内核定义在/include/linux/sched.h中，都已PF_开头（process flags），0x402000=PF_RANDOMIZE | PF_USED_MATH，表示没有设置fpu的话，这个进程在使用任何变量之前都需初始化(PF_USED_MATH)，并且该进程的虚拟地址空间是不固定的(PF_RANDOMIZE )。接下来的4个(5017
 0 0 0)是该进程的缺页管理的统计，说明该进程发生了5017次次缺页（次缺页：Copy on　Write页和匿名页）(min_flt)，并且其所有子进程没有发生次缺页(cmin_flt)，没有发生主缺页（主缺页：从映射文件或交换设备读入的页面数）（maj_flt），并且其所有子进程没有发生住缺页（cmaj_flt）。该进程在用户态下运行的时间是80个jiffies（在我的系统中jiffies等于250，所以80个jiffies为20秒）(cputime_to_clock_t(utime))，该进程在内核态下运行的时间是10个jiffies(2.5秒)(cputime_to_clock_t(stime))，该进程的所有子进程在用户态下运行的时间为0(cputime_to_clock_t(cutime))，所有子进程在内核态下运行的时间为0(cputime_to_clock_t(cstime))。该进程的动态优先级为20（priority），静态优先级为0（nice）。该进程所在的线程组里的线程个数为1（num_threads）。接下来的一个"0"是直接输出的，没有含义。下面的1292037是该进程的的创建时间，说明该进程的创建时间是开机后大约3.6小时时创建的（start_time=task->real_start_time,start_time/100/3600=小时）。该进程的虚拟地址空间的大小是61636608B()，该进程当前主流物理内存空间的大小是4481B，能驻留物理地址空间的最大值为4294967295B(4GB)，在虚拟地址空间的代码段的起始地址是134512640(0x8048000，一般的应用程序虚拟地址空间的代码段的起始地址都是0x80xxxxx，可以使用objdump
 -d查看)，虚拟地址空间的代码段的结束地址是135099420（0x80D741C），虚拟地址空间的栈的起始地址是3216990608（0xBFBF6190），堆栈指针的当前值为3216990068（0xBFBF5F74），可见我的系统的堆栈的扩展方向是向下扩展（每压栈一个数，esp向下递减）。
下一条要执行的指令的地址是3085931536（0xB7EF9410）。接下来的7个（0 0 4096 0 0 0 0 ）是与信号有关的，内核注释说在这里已经没有作用，这里就不做解释。下面一个是进程退出时向父进程发送的信号，该出发送的信号为SIGCHLD(17)。下来一个0表示该进程运行在第0个cpu上。该进程的实时程的相对优先级别为0，该进程是一个非实时进程。
10．statm文件
描述进程的内存状态。
```
niutao@niutao-desktop:/proc/6950$
 cat statm
12992 4432 3213 144 0 1028 0
niutao@niutao-desktop:/proc/6950$
```
下面我们来详细解释该文件中内容的含义。首先我们可以在内核中搜索到该文件的内容是由函数proc_pid_statm()函数写入的：（/fs/proc/array.c）
```
int proc_pid_statm(struct task_struct
*task, 
char *buffer)
{
        int size = 0, resident
= 0, shared 
= 0, text = 0, lib
= 0, data 
= 0;
        struct mm_struct 
*mm = get_task_mm(task);
        if (mm)
{
                size = task_statm(mm,
&shared, 
&text, &data,
&resident);
                mmput(mm);
        } 
        return sprintf(buffer,
"%d %d %d %d %d %d %d\n",
                       size, resident, shared, text, lib, data,
 0); 
}
/*fs/proc/task_mmu.c*/
int task_statm(struct mm_struct
*mm, 
int *shared,
int *text,
               int *data,
int *resident)
{ 
        *shared = get_mm_counter(mm, file_rss);
        *text =
(PAGE_ALIGN(mm->end_code)
- (mm->start_code
& PAGE_MASK))
                                                                >> PAGE_SHIFT;
        *data = mm->total_vm
- mm->shared_vm;
        *resident =
*shared + get_mm_counter(mm, anon_rss);
        return mm->total_vm;
}
```
size表示进程虚拟地址空间的大小（单位为页），resident表示文件映射内存大小和分配给匿名内存映射的大小（单位为页），shared表示共享文件内存映射大小（单位为页），text表示可执行代码区域的内存空间的大小（单位为页），data表示。。。所以该进程的内存信息可描述为其虚拟地址空间的大小为12992页(将近60MB)，文件映射内存大小和分配给匿名内存映射的大小为4432页（将近18MB）， 3213 144 0 1028 0
11．status文件：
用可读的方式描述进程的状态
```
niutao@niutao-desktop:/proc/9744$
 cat status
Name:    gedit /*进程的程序名*/
State:    S (sleeping)
/*进程的状态信息,具体参见http://blog.chinaunix.net/u2/73528/showart_1106510.html*/
Tgid:    9744 /*线程组号*/
Pid:    9744 /*进程pid*/
PPid:    7672 /*父进程的pid*/
TracerPid:    0 /*跟踪进程的pid*/
Uid:    1000    1000    1000    1000 
/*uid euid suid fsuid*/
Gid:    1000    1000    1000    1000 
/*gid egid sgid fsgid*/
FDSize:    256 /*文件描述符的最大个数，file->fds*/
Groups:    0 4 20 24 25 29 30 44 46 107 109 115 124 1000
/*启动该进程的用户所属的组的id*/
VmPeak:     60184 kB /*进程地址空间的大小*/
VmSize:     60180 kB /*进程虚拟地址空间的大小reserved_vm：进程在预留或特殊的内存间的物理页*/
VmLck: 0 kB /*进程已经锁住的物理内存的大小.锁住的物理内存不能交换到硬盘*/
VmHWM:     18020 kB /*文件内存映射和匿名内存映射的大小*/
VmRSS:     18020 kB /*应用程序正在使用的物理内存的大小，就是用ps命令的参数rss的值
(rss)*/
VmData:     12240 kB /*程序数据段的大小（所占虚拟内存的大小），存放初始化了的数据*/
VmStk:     84 kB /*进程在用户态的栈的大小*/
VmExe:     576 kB /*程序所拥有的可执行虚拟内存的大小,代码段,不包括任务使用的库
*/
VmLib:     21072 kB /*被映像到任务的虚拟内存空间的库的大小*/
VmPTE:     56 kB /*该进程的所有页表的大小*/
Threads:    1 /*共享使用该信号描述符的任务的个数*/
SigQ:    0/8183
/*待处理信号的个数/目前最大可以处理的信号的个数*/
SigPnd:    0000000000000000 
/*屏蔽位，存储了该线程的待处理信号*/
ShdPnd:    0000000000000000 
/*屏蔽位，存储了该线程组的待处理信号*/
SigBlk:    0000000000000000 
/*存放被阻塞的信号*/
SigIgn:    0000000000001000 
/*存放被忽略的信号*/
SigCgt:    0000000180000000 
/*存放被俘获到的信号*/
CapInh:    0000000000000000 
/*能被当前进程执行的程序的继承的能力*/
CapPrm:    0000000000000000 
/*进程能够使用的能力，可以包含CapEff中没有的能力，这些能力是被进程自己临时放弃的*/
CapEff:    0000000000000000 
/*是CapPrm的一个子集，进程放弃没有必要的能力有利于提高安全性*/
Cpus_allowed:    01 /*可以执行该进程的CPU掩码集*/
Mems_allowed:    1 /**/
voluntary_ctxt_switches:    1241 
/*进程主动切换的次数*/
nonvoluntary_ctxt_switches:    717 
/*进程被动切换的次数*/
```
该文件的内容在内核中由proc_pid_status函数写入：
```
int proc_pid_status(struct task_struct
*task, 
char *buffer)
{
        char *orig
= buffer;
        struct mm_struct 
*mm = get_task_mm(task);
        buffer = task_name(task, buffer);
        buffer = task_state(task, buffer);
        if (mm)
{
                buffer = task_mem(mm, buffer);
                mmput(mm);
        }
        buffer = task_sig(task, buffer);
        buffer = task_cap(task, buffer);
        buffer = cpuset_task_status_allowed(task, buffer);
#if defined(CONFIG_S390)
        buffer = task_show_regs(task, buffer);
#endif
        buffer = task_context_switch_counts(task, buffer);
        return buffer 
- orig;
}
```
经过以上分析，我们知道该进程的程序名为gedit，目前处于睡眠状态，该进程的线程组号为9744，进程的pid为9744，父进程的pid为7672，没有跟踪进程。该进程所属用户的id为1000，用户组id为1000，限制该进程最大可以同时打开256个文件。进程的地址空间的大小是60184 kB，进程的虚拟地址空间大小是60180 kB，常驻物理内存的大小为0KB，文件内存映射和匿名内存映射的大小为18020 kB，程序正在使用的物理内存的大小18020 kB，程序数据段的大小12240 kB，*进程在用户态的栈的大小84KB，程序所拥有的可执行虚拟内存的大小576KB，被映像到任务的虚拟内存空间的库的大小21072KB，该进程的所有页表的大小56KB，只有一个进程共享使用该进程的信号描述符，没有带处理的信号，进程主动切换了1241次，被动切换了717次。
12．mounts文件
该文件包含该系统挂在的文件系统的信息。该文件在/proc下和每个进程文件夹下都有，并且内容一样。
```
niutao@niutao-desktop:/proc/1$
 cat mounts
rootfs / rootfs rw 0 0
none /sys sysfs rw,nosuid,nodev,noexec 0 0
none /proc proc rw,nosuid,nodev,noexec 0 0
udev /dev tmpfs rw,relatime 0 0
fusectl /sys/fs/fuse/connections fusectl rw,relatime 0 0
/dev/disk/by-uuid/f9f21592-a8a3-4e61-ac3d-0c7b7aa2cd42
/ ext3 rw,relatime,errors=remount-ro,data=ordered
 0 0
/dev/disk/by-uuid/f9f21592-a8a3-4e61-ac3d-0c7b7aa2cd42
/dev/.static/dev ext3 rw,relatime,errors=remount-ro,data=ordered
 0 0
....
```
该文件的输出结果和/etc/mtab文件的内容类似，但比/etc/mtab文件多一些内容。第一列指出被挂载的设备，第二列表示挂载点，第三列指出该文件系统的类型。第四列对该挂载的文件系统的读写权限，一般有ro(read-only )和rw(read-write)。第五列和第六列是虚拟数据，用在/etc/mtab中。
13．io文件
```
niutao@niutao-desktop:/proc/1$
 cat io
rchar: 14699 /*task_struct->rchar*/
wchar: 20553017 /*task_struct->wchar*/
syscr: 350 /*task_struct->syscr*/
syscw: 1128 /*task_struct->syscw,*/
read_bytes: 1605632 /*task_struct->ioac.read_bytes*/
write_bytes: 0 /*task_struct->ioac.write_bytes*/
cancelled_write_bytes: 0 
/*task_struct->ioac.cancelled_write_bytes*/
```
引用：[http://blog.chinaunix.net/uid-20729605-id-1884340.html](http://blog.chinaunix.net/uid-20729605-id-1884340.html)
