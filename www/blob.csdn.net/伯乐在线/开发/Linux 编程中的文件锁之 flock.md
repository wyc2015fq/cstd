# Linux 编程中的文件锁之 flock - 文章 - 伯乐在线
原文出处： [cywosp](http://blog.csdn.net/cywosp/article/details/30083015)
**1. 场景概述**
在多线程开发中，互斥锁可以用于对临界资源的保护，防止数据的不一致，这是最为普遍的使用方法。那在多进程中如何处理文件之间的同步呢？我们看看下面的图：
![](http://img.blog.csdn.net/20140611180504125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3l3b3Nw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图中所示的是两个进程在无同步的情况下同时更新同一个文件的过程，其主要的操作是：
- 1. 从文件中读取序号。
- 2. 使用这个序号完成应用程序定义的任务。
- 3. 递增这个序号并将其写回文件中。
从图中可得知两个进程读取分别增加了所读取到的序号，并写回到了文件中，但是如果有相互互斥的话，最后的值应该是1002，而不是所示的1001。为了防止出现这种情况，Linux提供了flock(对整个文件加锁)、fcntl(对整个文件区域加锁)两个函数来做进程间的文件同步。同时也可以使用信号量来完成所需的同步，但通常使用文件锁会更好一些，因为内核能够自动将锁与文件关联起来。
**2. flock()**
flock的声明如下


```
#include <sys/file.h>
// Returns 0 on success, or -1 on error
int flock (intfd, int operation);
```
fcntl()函数提供了比该函数更为强大的功能，并且所拥有的功能也覆盖了flock()所拥有的功能，但是在某些应用中任然使用着flock()函数，并且在继承和锁释放方面的一些语义 中flock()与fcntl()还是有所不同的。
flock()系统调用是在整个文件中加锁，通过对传入的fd所指向的文件进行操作，然后在通过operation参数所设置的值来确定做什么样的操作。operation可以赋如下值:
![](http://img.blog.csdn.net/20140611180814390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3l3b3Nw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在默认情况下，如果另一个进程已经持有了文件上的一个不兼容的锁，那么flock()会阻塞。如果需要防止这种情况的出现，可以在operation参数中对这些值取OR（|）。在这种情况下，如果一个进程已经持有了文件上的一个不兼容锁，那么flock()就会阻塞，相反，它会返回-1，并将errno设置成EWOULDBLOCK。
任意数量的进程可同时持有一个文件上的共享锁，但子任意时刻只能有一个进程能够持有一个文件上的互斥锁，（这有点类似读写锁）。下图是进程A先设置了锁，进程B后设置锁的支持情况：
![](http://img.blog.csdn.net/20140611180740109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY3l3b3Nw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
无论程序以什么模式打开了文件（读、写或者读写），该文件上都可以放置一把共享锁或互斥锁。在实际操作过程中，参数operation可以指定对应的值将共享锁转换成互斥锁（反之亦然）。将一个共享锁转换成互斥锁，如果另一个进程要获取该文件的共享锁则会阻塞，除非operation参数指定了LOCK_NB标记，即：(LOCK_SH | LOCK_NB)。锁的转换过程不是一个原子操作，在转换的过程中首先会删除既有的锁，然后创建新锁。
**3. 锁继承与释放的语义**
flock()根据调用时operation参数传入LOCK_UN的值来释放一个文件锁。此外，锁会在相应的文件描述符被关闭之后自动释放。同时，当一个文件描述符被复制时（dup()、dup2()、或一个fcntl() F_DUPFD操作），新的文件描述符会引用同一个文件锁。


```
flock(fd, LOCK_EX);
new_fd = dup(fd);
flock(new_fd, LOCK_UN);
```
这段代码先在fd上设置一个互斥锁，然后通过fd创建一个指向相同文件的新文件描述符new_fd，最后通过new_fd来解锁。从而我们可以得知新的文件描述符指向了同一个锁。所以，如果通过一个特定的文件描述符获取了一个锁并且创建了该描述符的一个或多个副本，那么，如果不显示的调用一个解锁操作，只有当文件描述符副本都被关闭了之后锁才会被释放。
由上我们可以推出，如果使用fork()创建一个子进程，子进程会复制父进程中的所有描述符，从而使得它们也会指向同一个文件锁。例如下面的代码会导致一个子进程删除一个父进程的锁：


```
flock (fd, LOCK_EX);
if (0 == fork ()) {
    flock (fd, LOCK_UN);
}
```
所以，有时候可以利用这些语义来将一个文件锁从父进程传输到子进程：在fork()之后，父进程关闭其文件描述符，然后锁就只在子进程的控制之下了。通过fork()创建的锁在exec()中会得以保留（除非在文件描述符上设置了close-on-exec标记并且该文件描述符是最后一个引用底层的打开文件描述的描述符）。
如果程序中使用open()来获取第二个引用同一个文件的描述符，那么，flock()会将其视为不同的文件描述符。如下代码会在第二个flock()上阻塞。


```
fd1 = open ("test.txt", O_RDWD);
fd2 = open ("test.txt", O_RDWD);
flock (fd1, LOCK_EX);
flock (fd2, LOCK_EX);
```
**4. flock()的限制**
flock()放置的锁有如下限制
- 只能对整个文件进行加锁。这种粗粒度的加锁会限制协作进程间的并发。假如存在多个进程，其中各个进程都想同时访问同一个文件的不同部分。
- 通过flock()只能放置劝告式锁。
- 很多NFS实现不识别flock()放置的锁。
注释：在默认情况下，文件锁是劝告式的，这表示一个进程可以简单地忽略另一个进程在文件上放置的锁。要使得劝告式加锁模型能够正常工作，所有访问文件的进程都必须要配合，即在执行文件IO之前先放置一把锁。
