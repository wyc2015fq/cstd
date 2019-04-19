# void *mmap(void *start,size_t length,int prot,int flags,int fd,off_t offsize); - Koma Hub - CSDN博客
2019年01月24日 22:04:37[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：159
个人分类：[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)
mmap()函数的主要用途有三个：
1、将一个普通文件映射到内存中，通常在需要对文件进行频繁读写时使用，这样用内存读写取代I/O读写，以获得较高的性能；
2、将特殊文件进行匿名内存映射，可以为关联进程提供共享内存空间；
3、为无关联的进程提供共享内存空间，一般也是将一个普通文件映射到内存中。
头文件：
 #include <sys/mman.h>
原型：
void *mmap(void *start,size_t length,int prot,int flags,int fd,off_t offsize);
参数说明：
参数start：指向欲映射的内存起始地址，通常设为 NULL，代表让系统自动选定地址，映射成功后返回该地址。
参数length：代表将文件中多大的部分映射到内存。
参数prot：映射区域的保护方式。可以为以下几种方式的组合：
PROT_EXEC       映射区域可被执行
PROT_READ       映射区域可被读取
PROT_WRITE     映射区域可被写入
PROT_NONE      映射区域不能存取
参数flags：影响映射区域的各种特性。在调用mmap()时必须要指定MAP_SHARED 或MAP_PRIVATE。
MAP_FIXED                如果参数start所指的地址无法成功建立映射时，则放弃映射，不对地址做修正。通常不鼓励用此旗标。
MAP_SHARED          对映射区域的写入数据会复制回文件内，而且允许其他映射该文件的进程共享。
MAP_PRIVATE            对映射区域的写入操作会产生一个映射文件的复制，即私人的“写入时复制”（copy on write）对此区域作的任何修改都不会写回原来的文件内容。
MAP_ANONYMOUS  建立匿名映射。此时会忽略参数fd，不涉及文件，而且映射区域无法和其他进程共享。
MAP_DENYWRITE   只允许对映射区域的写入操作，其他对文件直接写入的操作将会被拒绝。
MAP_LOCKED          将映射区域锁定住，这表示该区域不会被置换（swap）。
参数fd：要映射到内存中的文件描述符。如果使用匿名内存映射时，即flags中设置了MAP_ANONYMOUS，fd设为-1。有些系统不支持匿名内存映射，则可以使用fopen打开/dev/zero文件，然后对该文件进行映射，可以同样达到匿名内存映射的效果。
参数offset：文件映射的偏移量，通常设置为0，代表从文件最前方开始对应，offset必须是分页大小的整数倍。
返回值：
若映射成功则返回映射区的内存起始地址，否则返回MAP_FAILED(－1)，错误原因存于errno 中。
错误代码：
EBADF          参数fd 不是有效的文件描述词
EACCES       存取权限有误。如果是MAP_PRIVATE 情况下文件必须可读，使用MAP_SHARED则要有PROT_WRITE以及该文件要能写入。
EINVAL          参数start、length 或offset有一个不合法。
EAGAIN         文件被锁住，或是有太多内存被锁住。
ENOMEM      内存不足。
--------------------- 
作者：为幸福写歌 
来源：CSDN 
原文：https://blog.csdn.net/yzy1103203312/article/details/78286360 
版权声明：本文为博主原创文章，转载请附上博文链接！
