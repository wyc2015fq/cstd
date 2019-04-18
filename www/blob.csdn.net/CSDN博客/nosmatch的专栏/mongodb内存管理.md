# mongodb内存管理 - nosmatch的专栏 - CSDN博客
2012年07月18日 10:40:48[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：623
> 
但凡初次接触MongoDB的人，无不惊讶于它对内存的贪得无厌，至于个中缘由，我先讲讲Linux是如何管理内存的，再说说MongoDB是如何使用内存的，答案自然就清楚了。
据说带着问题学习更有效，那就先看一个MongoDB服务器的top命令结果：
shell> top -p $(pidof mongod)
Mem:  32872124k total, 30065320k used,  2806804k free,   245020k buffers
Swap:  2097144k total,      100k used,  2097044k free, 26482048k cached
 VIRT  RES  SHR %MEM
1892g  21g  21g 69.6
这台MongoDB服务器有没有性能问题？大家可以一边思考一边继续阅读。
### 先讲讲Linux是如何管理内存的
在Linux里（别的系统也差不多），内存有物理内存和[虚拟内存](http://en.wikipedia.org/wiki/Virtual_memory)之说，物理内存是什么自然无需解释，虚拟内存实际是物理内存的抽象，多数情况下，出于方便性的考虑，程序访问的都是虚拟内存地址，然后操作系统会把它翻译成物理内存地址。
很多人会把虚拟内存和Swap混为一谈，实际上Swap只是虚拟内存引申出的一种技术而已：操作系统一旦物理内存不足，为了腾出内存空间存放新内容，就会把当前物理内存中的内容放到交换分区里，稍后用到的时候再取回来，需要注意的是，Swap的使用可能会带来性能问题，偶尔为之无需紧张，糟糕的是物理内存和交换分区频繁的发生数据交换，这被称之为Swap颠簸，一旦发生这种情况，先要明确是什么原因造成的，如果是内存不足就好办了，加内存就可以解决，不过有的时候即使内存充足也可能会出现这种问题，比如MySQL就有可能出现这样的情况，解决方法是限制使用Swap：
shell> sysctl -w vm.swappiness=0
查看内存情况最常用的是free命令：
shell> free -m
             total       used       free     shared    buffers     cached
Mem:         32101      29377       2723          0        239      25880
-/+ buffers/cache:       3258      28842
Swap:         2047          0       2047
新手看到used一栏数值偏大，free一栏数值偏小，往往会认为内存要用光了。其实并非如此，之所以这样是因为每当我们操作文件的时候，Linux都会尽可能的把文件缓存到内存里，这样下次访问的时候，就可以直接从内存中取结果，所以cached一栏的数值非常的大，不过不用担心，这部分内存是可回收的，操作系统会按照[LRU](http://en.wikipedia.org/wiki/Cache_algorithms)算法淘汰冷数据。除了cached，还有一个buffers，它和cached类似，也是可回收的，不过它的侧重点在于缓解不同设备的操作速度不一致造成的阻塞，这里就不多做解释了。
知道了原理，我们就可以推算出系统可用的内存是free + buffers + cached：
shell> echo "2723 + 239 + 25880" | bc -l
28842
至于系统实际使用的内存是used – buffers – cached：
shell> echo "29377 - 239 - 25880" | bc -l
3258
除了free命令，还可以使用sar命令：
shell> sar -r
kbmemfree kbmemused  %memused kbbuffers  kbcached
  3224392  29647732     90.19    246116  26070160
  3116324  29755800     90.52    245992  26157372
  2959520  29912604     91.00    245556  26316396
  2792248  30079876     91.51    245680  26485672
  2718260  30153864     91.73    245684  26563540
shell> sar -W
pswpin/s pswpout/s
    0.00      0.00
    0.00      0.00
    0.00      0.00
    0.00      0.00
    0.00      0.00
希望你没有被%memused吓到，如果不幸言中，请参考free命令的解释。
### 再说说MongoDB是如何使用内存的
目前，MongoDB使用的是[内存映射存储引擎](http://www.mongodb.org/display/DOCS/Caching)，它会把磁盘IO操作转换成内存操作，如果是读操作，内存中的数据起到缓存的作用，如果是写操作，内存还可以把随机的写操作转换成顺序的写操作，总之可以大幅度提升性能。MongoDB并不干涉内存管理工作，而是把这些工作留给操作系统的虚拟缓存管理器去处理，这样的好处是简化了MongoDB的工作，但坏处是你没有方法很方便的控制MongoDB占多大内存，事实上MongoDB会占用所有能用的内存，所以最好不要把别的服务和MongoDB放一起。
有时候，即便MongoDB使用的是64位操作系统，也可能会遭遇臭名昭著的[OOM](http://en.wikipedia.org/wiki/Out_of_memory)问题，出现这种情况，多半是因为限制了虚拟内存的大小所致，可以这样查看当前值：
shell> ulimit -a | grep 'virtual'
多数操作系统缺省都是把它设置成unlimited的，如果你的操作系统不是，可以这样修改：
shell> ulimit -v unlimited
不过要注意的是，ulimit的使用是有上下文的，最好放在MongoDB的启动脚本里。
有时候，出于某些原因，你可能想释放掉MongoDB占用的内存，不过前面说了，内存管理工作是由虚拟内存管理器控制的，所以通常你只能通过重启服务来释放内存，你一定不齿于这样的方法，幸好可以使用MongoDB内置的[closeAllDatabases](http://www.mongodb.org/display/DOCS/List+of+Database+Commands)命令达到目的：
mongo> use admin
mongo> db.runCommand({closeAllDatabases:1})
另外，通过调整内核参数[drop_caches](http://www.kernel.org/doc/Documentation/sysctl/vm.txt)也可以释放缓存：
shell> sysctl -w vm.drop_caches=1
平时可以通过mongo命令行来监控MongoDB的内存使用情况，如下所示：
mongo> db.serverStatus().mem:
{
    "resident" : 22346,
    "virtual" : 1938524,
    "mapped" : 962283
}
还可以通过[mongostat](http://www.mongodb.org/display/DOCS/mongostat)命令来监控MongoDB的内存使用情况，如下所示：
shell> mongostat
mapped  vsize    res faults
  940g  1893g  21.9g      0
  940g  1893g  21.9g      0
  940g  1893g  21.9g      0
  940g  1893g  21.9g      0
  940g  1893g  21.9g      0
其中内存相关字段的含义是：
- mapped：映射到内存的数据大小
- visze：占用的虚拟内存大小
- res：实际使用的内存大小
注：如果操作不能再内存中完成，结果faults列的数值不会是0，视大小可能有性能问题。
在上面的结果中，vsize是mapped的两倍，而mapped等于数据文件的大小，所以说vsize是数据文件的两倍，之所以会这样，是因为本例中，MongoDB开启了[journal](http://www.mongodb.org/display/DOCS/Journaling)，需要在内存里多映射一次数据文件，如果关闭journal，则vsize和mapped大致相当。
如果想验证这一点，可以在开启或关闭journal后，通过pmap命令来观察文件映射情况：
shell> pmap $(pidof mongod)
到底MongoDB配备多大内存合适？宽泛点来说，多多益善，如果要确切点来说，这实际取决于你的数据及索引的大小，内存如果能够装下全部数据加索引是最佳情况，不过很多时候，数据都会比内存大，比如本文说涉及的MongoDB实例：
mongo> db.stats()
{
        "dataSize" : 1004862191980,
        "indexSize" : 1335929664
}
本例中索引只有1G多，内存完全能装下，而数据文件则达到了1T，估计很难找到这么大内存，此时保证内存能装下热数据即可，至于热数据有多少，这就是个比例问题了，取决于具体的应用。如此一来内存大小就明确了：内存 > 索引 + 热数据。
关于MongoDB与内存的话题，大家还可以参考[官方文档](http://www.mongodb.org/display/DOCS/Checking+Server+Memory+Usage)中的相关介绍。
