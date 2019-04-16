# 转载和积累系列 - Page cache和Buffer cache - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年04月25日 10:27:53[initphp](https://me.csdn.net/initphp)阅读数：2429








Page cache实际上是针对文件系统的，是文件的缓存，在文件层面上的数据会缓存到page cache。文件的逻辑层需要映射到实际的物理磁盘，这种映射关系由文件系统来完成。当page
 cache的数据需要刷新时，page cache中的数据交给buffer cache，但是这种处理在2.6版本的内核之后就变的很简单了，没有真正意义上的cache操作。

在Linux 2.6的内核中Page cache和Buffer
 cache进一步结合，Buffer pages其实也是Page cache里面的页。从Linux算法实现的角度，Page
 cache和Buffer cache目前是一样的,只是多了一层抽象，通过buffer_head来进行一些访问管理。可以理解为只有Page
 cache概念亦可。在Linux 2.6的内核中Page cache和Buffer
 cache进一步结合，Buffer pages其实也是Page cache里面的页。从Linux算法实现的角度，Page
 cache和Buffer cache目前是一样的,只是多了一层抽象，通过buffer_head来进行一些访问管理。可以理解为只有Page
 cache概念亦可。

![](https://img-blog.csdn.net/20140425102712468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




标准IO：
在 Linux中，这种访问文件的方式是通过两个系统调用实现的：read()和
write()。当应用程序调用read()
系统调用读取一块数据的时候，如果该块数据已经在内存中了，那么就直接从内存中读出该数据并返回给应用程序；如果该块数据不在内存中，那么数据会被从磁盘上读到页高缓存中去，然后再从页缓存中拷贝到用户地址空间中去。如果一个进程读取某个文件，那么其他进程就都不可以读取或者更改该文件；对于写数据操作来说，当一个进程调用了write()
系统调用往某个文件中写数据的时候，数据会先从用户地址空间拷贝到操作系统内核地址空间的页缓存中去，然后才被写到磁盘上。但是对于这种标准的访问文件的方式来说，在数据被写到页缓存中的时候，write()系统调用就算执行完成，并不会等数据完全写入到磁盘上。Linux在这里采用的是我们前边提到的延迟写机制（
deferred writes）。如果用户采用的是延迟写机制（
deferred writes），那么应用程序就完全不需要等到数据全部被写回到磁盘，数据只要被写到页缓存中去就可以了。在延迟写机制的情况下，操作系统会定期地将放在页缓存中的数据刷到磁盘上。
![](https://img-blog.csdn.net/20140425102732062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



直接IO：

凡是通过直接 I/O方式进行数据传输，数据均直接在用户地址空间的缓冲区和磁盘之间直接进行传输，完全不需要页缓存的支持。操作系统层提供的缓存往往会使应用程序在读写数据的时候获得更好的性能，但是对于某些特殊的应用程序，比如说数据库管理系统这类应用，他们更倾向于选择他们自己的缓存机制，因为数据库管理系统往往比操作系统更了解数据库中存放的数据，数据库管理系统可以提供一种更加有效的缓存机制来提高数据库中数据的存取性能。

简单说来，page cache用来缓存文件数据，buffer cache用来缓存磁盘数据。在有文件系统的情况下，对文件操作，那么数据会缓存到page
 cache，如果直接采用dd等工具对磁盘进行读写，那么数据会缓存到buffer cache。

补充一点，在文件系统层每个设备都会分配一个def_blk_ops的文件操作方法，这是设备的操作方法，在每个设备的inode下面会存在一个radix
 tree，这个radix tree下面将会放置缓存数据的page页。这个page的数量将会在top程序的buffer一栏中显示。如果设备做了文件系统，那么会生成一个inode，这个inode会分配ext3_ops之类的操作方法，这些方法是文件系统的方法，在这个inode下面同样存在一个radix
 tree，这里会缓存文件的page页，缓存页的数量在top程序的cache一栏进行统计。从上面的分析可以看出，2.6内核中的buffer
 cache和page cache在处理上是保持一致的，但是存在概念上的差别，page cache针对文件的cache，buffer是针对磁盘块数据的cache，仅此而已。

"如果一个进程读取某个文件，那么其他进程就都不可以读取或者更改该文件；"对这一句话存在质疑。

原文

http://blog.chinaunix.net/uid-1829236-id-3152172.html




