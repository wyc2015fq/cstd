# free命令、buffer与cache的区别 - DoubleLi - 博客园






**free**
free 命令相对于top 提供了更简洁的查看系统内存使用情况：



|# free             total       used       free     shared    buffers     cachedMem:        255988     231704      24284          0       6432     139292-/+ buffers/cache:      85980     170008Swap:       746980          0     746980|
|----|



Mem：表示物理内存统计 
-/+ buffers/cached：表示物理内存的缓存统计 
Swap：表示硬盘上交换分区的使用情况，这里我们不去关心。
系统的总物理内存：255988Kb（256M），但系统当前真正可用的内存并不是第一行free 标记的 24284Kb，它仅代表未被分配的内存。

我们使用total1、used1、free1、used2、free2 等名称来代表上面统计数据的各值，1、2 分别代表第一行和第二行的数据。

total1：表示物理内存总量。 
used1：表示总计分配给缓存（包含buffers 与cache ）使用的数量，但其中可能部分缓存并未实际使用。 
free1：未被分配的内存。 
shared1：共享内存，一般系统不会用到，这里也不讨论。 
buffers1：系统分配但未被使用的buffers 数量。 
cached1：系统分配但未被使用的cache 数量。buffer 与cache 的区别见后面。 
used2：实际使用的buffers 与cache 总量，也是实际使用的内存总量。 
free2：未被使用的buffers 与cache 和未被分配的内存之和，这就是系统当前实际可用内存。


可以整理出如下等式：

total1 = used1 + free1

total1 = used2 + free2

used1 = buffers1 + cached1 + used2

free2 = buffers1 + cached1 + free1


**buffer 与cache 的区别**
A buffer is something that has yet to be "written" to disk. A cache is something that has been "read" from the disk and stored for later use.



两者都是RAM中的数据。简单来说，buffer是即将要被写入磁盘的，cache是被从磁盘中读出来的。
buffer是由各种进程分配的，被用在如输入队列等方面，一个简单的例子如某个进程要求有多个字段读入，在所有字段被读入完整之前，进程把先前读入的字段放在buffer中保存。
cache经常被用在磁盘的I/O请求上，如果有多个进程都要访问某个文件，于是该文件便被做成cache以方便下次被访问，这样可提供系统性能。









