# Posix多线程编程学习笔记（六）—共享内存（1） - ljx0305的专栏 - CSDN博客
2008年08月01日 13:49:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：795
**一、什么是共享内存区<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />**
共享内存区是最快的可用IPC形式。它允许多个不相关的进程去访问同一部分逻辑内存。如果需要在两个运行中的进程之间传输数据，共享内存将是一种效率极高的解决方案。一旦这样的内存区映射到共享它的进程的地址空间，这些进程间数据的传输就不再涉及内核。这样就可以减少系统调用时间，提高程序效率。
共享内存是由IPC为一个进程创建的一个特殊的地址范围，它将出现在进程的地址空间中。其他进程可以把同一段共享内存段“连接到”它们自己的地址空间里去。所有进程都可以访问共享内存中的地址。如果一个进程向这段共享内存写了数据，所做的改动会立刻被有访问同一段共享内存的其他进程看到。
要注意的是共享内存本身没有提供任何同步功能。也就是说，在第一个进程结束对共享内存的写操作之前，并没有什么自动功能能够预防第二个进程开始对它进行读操作。共享内存的访问同步问题必须由程序员负责。可选的同步方式有互斥锁、条件变量、读写锁、纪录锁、信号灯。
**二、mmap**
在将共享内存前我们要先来介绍下面几个函数。
mmap函数把一个文件或一个Posix共享内存区对象映射到调用进程的地址空间。使用该函数有三个目的：
1.使用普通文件以提供内存映射I/O
2.使用特殊文件以提供匿名内存映射。
3.使用shm_open以提供无亲缘关系进程间的Posix共享内存区。
1．
|名称:：|mmap|
|----|----|
|功能：|把I/O文件映射到一个存储区域中|
|头文件：|#include <sys/mman.h>|
|函数原形：|void *mmap(void *addr,size_t len,int prot,int flag,int filedes,off_t off);|
|参数：|addr指向映射存储区的起始地址len映射的字节prot对映射存储区的保护要求flagflag标志位filedes要被映射文件的描述符off要映射字节在文件中的起始偏移量|
|返回值：|若成功则返回映射区的起始地址，若出错则返回MAP_FAILED|

addr参数用于指定映射存储区的起始地址。通常将其设置为NULL，这表示由系统选择该映射区的起始地址。
filedes指要被映射文件的描述符。在映射该文件到一个地址空间之前，先要打开该文件。len是映射的字节数。
off是要映射字节在文件中的起始偏移量。通常将其设置为0。
prot参数说明对映射存储区的保护要求。可将prot参数指定为PROT_NONE,或者是PROT_READ(映射区可读),PROT_WRITE（映射区可写）,PROT_EXEC（映射区可执行）任意组合的按位或，也可以是PROT_NONE(映射区不可访问)。对指定映射存储区的保护要求不能超过文件open模式访问权限。
flag参数影响映射区的多种属性：
MAP_FIXED 返回值必须等于addr.因为这不利于可移植性，所以不鼓励使用此标志。
MAP_SHARED 这一标志说明了本进程对映射区所进行的存储操作的配置。此标志指定存储操作修改映射文件。
MAP_PRIVATE 本标志导致对映射区建立一个该映射文件的一个私有副本。所有后来对该映射区的引用都是引用该副本，而不是原始文件。
要注意的是必须指定MAP_FIXED或MAP_PRIVATE标志其中的一个，指定前者是对存储映射文件本身的一个操作，而后者是对其副本进行操作。
mmap成功返回后，fd参数可以关闭。该操作对于由mmap建立的映射关系没有影响。为从某个进程的地址空间删除一个映射关系，我们调用munmap.
2．
|名称:：|munmap|
|----|----|
|功能：|解除存储映射|
|头文件：|#include <sys/mman.h>|
|函数原形：|int munmap(caddr_t addr,size_t len);|
|参数：|addr指向映射存储区的起始地址len映射的字节|
|返回值：|若成功则返回0，若出错则返回-1|

其中addr参数是由mmap返回的地址，len是映射区的大小。再次访问这些地址导致向调用进程产生一个SIGSEGV信号。
如果被映射区是使用MAP_PRIVATE标志映射的，那么调用进程对它所作的变动都被丢弃掉。
内核的虚存算法保持内存映射文件（一般在硬盘上）与内存映射区（在内存中）的同步（前提它是MAP_SHARED内存区）。这就是说，如果我们修改了内存映射到某个文件的内存区中某个位置的内容，那么内核将在稍后某个时刻相应地更新文件。然而有时候我们希望确信硬盘上的文件内容与内存映射区中的文件内容一致，于是调用msync来执行这种同步。
3．
|名称:：|msync|
|----|----|
|功能：|同步文件到存储器|
|头文件：|#include <sys/mman.h>|
|函数原形：|int msync(void *addr,size_t len,int flags);|
|参数：|addr指向映射存储区的起始地址len映射的字节protflags|
|返回值：|若成功则返回0，若出错则返回-1|

其中addr和len参数通常指代内存中的整个内存映射区，不过也可以指定该内存区的一个子集。flags参数为MS_ASYNC(执行异步写)，MS_SYNC（执行同步写），MS_INVALIDATE（使高速缓存的数据实效）。其中MS_ASYNC和MS_SYNC这两个常值中必须指定一个，但不能都指定。它们的差别是，一旦写操作已由内核排入队列，MS_ASYNC即返回，而MS_SYNC则要等到写操作完成后才返回。如果还指定了MS_INVALIDATE，那么与其最终拷贝不一致的文件数据的所有内存中拷贝都失效。后续的引用将从文件取得数据。
4．
|名称:：|memcpy|
|----|----|
|功能：|复制映射存储区|
|头文件：|#include <string.h>|
|函数原形：|void *memcpy(void *dest,const void *src,size_t n);|
|参数：|dest待复制的映射存储区src复制后的映射存储区n待复制的映射存储区的大小|
|返回值：|返回dest的首地址|

memcpy拷贝n个字节从dest到src。
下面就是利用mmap函数影射I/O实现的cp命令。
/*mycp.c*/
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
int main(int argc,char *argv[])
{
int fdin,fdout;
void *arc,dst;
struct stat statbuf;
if(argc!=3)
{
printf(“please input two file!/n”);
exit(1);
}
if((fdin=open(argv[1],O_RDONLY))<0) /*打开原文件*/
perror(argv[1]);
if((fdout=open(argv[2],O_RDWR|O_CREAT|O_TRUNC))<0)/*创建并打开目标文件*/
perror(argv[2]);
if(fstat(fdin,&statbuf)<0) /*获得文件大小信息*/
printf(“fstat error”);
if(lseek(fdout,statbuf.st_size-1,SEEK_SET)==-1)/*初始化输出映射存储区*/
printf(“lseek error”);
if(write(fdout,”<?xml:namespace prefix = st1 ns = "urn:schemas-microsoft-com:office:smarttags" />1”)!=1)
printf(“write error”);
if((src=mmap(0,statbuf.st_size,PROT_READ,MAP_SHARED,fdin,0))==MAP_FAILED)
/*映射原文件到输入的映射存储区*/
printf(“mmap error); 
if((dst=mmap(0,statbuf.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fdout,0)) ==MAP_FAILED) /*映射目标文件到输出的映射存储区*/
printf(“mmap error);
memcpy(dst,src,statbuf.st_size);/*复制映射存储区*/
munmap(src,statbuf.st_size); /*解除输入映射*/
munmap(dst,statbuf.st_size); /*解除输出映射*/
close(fdin);
close(fdout);
}
下面是运行结果：
#cc –o mycp mycp.c
#./mycp test1 test2
引用：[http://blog.chinaunix.net/u/22935/showart_341892.html](http://blog.chinaunix.net/u/22935/showart_341892.html)
