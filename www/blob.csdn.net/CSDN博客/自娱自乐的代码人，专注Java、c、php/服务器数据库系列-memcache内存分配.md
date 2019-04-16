# 服务器数据库系列 - memcache内存分配 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月27日 15:24:17[initphp](https://me.csdn.net/initphp)阅读数：815标签：[memcached																[byte																[存储](https://so.csdn.net/so/search/s.do?q=存储&t=blog)
个人分类：[服务器数据库系列](https://blog.csdn.net/initphp/article/category/2609107)





**一、Memcache内存分配机制**

        关于这个机制网上有很多解释的，我个人的总结如下。
- **Page为内存分配的最小单位。**
Memcached的内存分配以page为单位，默认情况下一个page是1M，可以通过-I参数在启动时指定。如果需要申请内存时，memcached会划分出一个新的page并分配给需要的slab区域。page一旦被分配在重启前不会被回收或者重新分配（page ressign已经从1.2.8版移除了）
![](https://img-blog.csdn.net/20150322121340971?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


- **Slabs划分数据空间。**
Memcached并不是将所有大小的数据都放在一起的，而是预先将数据空间划分为一系列slabs，每个slab只负责一定范围内的数据存储。如下图，每个slab只存储大于其上一个slab的size并小于或者等于自己最大size的数据。例如：slab 3只存储大小介于137 到 224 bytes的数据。如果一个数据大小为230byte将被分配到slab 4中。从下图可以看出，每个slab负责的空间其实是不等的，memcached默认情况下下一个slab的最大值为前一个的1.25倍，这个可以通过修改-f参数来修改增长比例。
![](https://img-blog.csdn.net/20150322121320335?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


- **Chunk才是存放缓存数据的单位。**
Chunk是一系列固定的内存空间，这个大小就是管理它的slab的最大存放大小。例如：slab 1的所有chunk都是104byte，而slab 4的所有chunk都是280byte。chunk是memcached实际存放缓存数据的地方，因为chunk的大小固定为slab能够存放的最大值，所以所有分配给当前slab的数据都可以被chunk存下。如果时间的数据大小小于chunk的大小，空余的空间将会被闲置，这个是为了防止内存碎片而设计的。例如下图，chunk size是224byte，而存储的数据只有200byte，剩下的24byte将被闲置。
![](https://img-blog.csdn.net/20150322121405557?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


- **Slab的内存分配。**
Memcached在启动时通过-m指定最大使用内存，但是这个不会一启动就占用，是随着需要逐步分配给各slab的。

         如果一个新的缓存数据要被存放，memcached首先选择一个合适的slab，然后查看该slab是否还有空闲的chunk，如果有则直接存放进去；如果没有则要进行申请。slab申请内存时以page为单位，所以在放入第一个数据，无论大小为多少，都会有1M大小的page被分配给该slab。申请到page后，slab会将这个page的内存按chunk的大小进行切分，这样就变成了一个chunk的数组，在从这个chunk数组中选择一个用于存储数据。如下图，slab 1和slab 2都分配了一个page，并按各自的大小切分成chunk数组。
![](https://img-blog.csdn.net/20150322121414496?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

- **Memcached内存分配策略。**
综合上面的介绍，memcached的内存分配策略就是：按slab需求分配page，各slab按需使用chunk存储。

这里有几个特点要注意，
- Memcached分配出去的page不会被回收或者重新分配 
- Memcached申请的内存不会被释放 
- slab空闲的chunk不会借给任何其他slab使用 

![](https://img-blog.csdn.net/20150322121511701?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





      知道了这些以后，就可以理解为什么总内存没有被全部占用的情况下，memcached却出现了丢失缓存数据的问题了。](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=memcached&t=blog)




