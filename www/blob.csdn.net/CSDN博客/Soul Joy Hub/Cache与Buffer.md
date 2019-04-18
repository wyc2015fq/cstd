# Cache与Buffer - Soul Joy Hub - CSDN博客

2017年03月08日 22:33:13[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：321标签：[buffer																[内存																[缓存																[cache																[linux](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=cache&t=blog)](https://so.csdn.net/so/search/s.do?q=缓存&t=blog)](https://so.csdn.net/so/search/s.do?q=内存&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)
个人分类：[Linux](https://blog.csdn.net/u011239443/article/category/6304554)


[http://blog.csdn.net/caoshuming_500/article/details/7332561](http://blog.csdn.net/caoshuming_500/article/details/7332561)
[http://blog.csdn.net/dianhuiren/article/details/7543886](http://blog.csdn.net/dianhuiren/article/details/7543886)

# Cache

缓存区，是高速缓存，是位于CPU和主内存之间的容量较小但速度很快的存储器，因为CPU的速度远远高于主内存的速度，CPU从内存中读取数据需等待很长的时间，而  Cache保存着CPU刚用过的数据或循环使用的部分数据，这时从Cache中读取数据会更快，减少了CPU等待的时间，提高了系统的性能。

## Page cache

实际上是针对文件系统的，是文件的缓存，在文件层面上的数据会缓存到page cache。文件的逻辑层需要映射到实际的物理磁盘，这种映射关系由文件系统来完成。当page cache的数据

需要刷新时，page cache中的数据交给buffer cache，但是这种处理在2.6版本的内核之后就变的很简单了，没有真正意义上的cache操作。

## Buffer cache

是针对磁盘块的缓存，也就是在没有文件系统的情况下，直接对磁盘进行操作的数据会缓存到buffer cache中，例如，文件系统的元数据都会缓存到buffer cache中。

# Buffer

缓冲区，用于存储速度不同步的设备或优先级不同的设备之间传输数据；通过buffer可以减少进程间通信需要等待的时间，当存储速度快的设备与存储速度慢的设备进行通信时，存储慢的数据先把数据存放到buffer，达到一定程度存储快的设备再读取buffer的数据，在此期间存储快的设备CPU可以干其他的事情。

Buffer：一般是用在写入磁盘的，例如：某个进程要求多个字段被读入，当所有要求的字段被读入之前已经读入的字段会先放到buffer中。


