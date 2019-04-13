
# HDFS基本概念与重新梳理（hdfs文件信息介绍） - leofionn的博客 - CSDN博客


2018年10月03日 18:45:11[leofionn](https://me.csdn.net/qq_36142114)阅读数：50个人分类：[hadoop搭建																](https://blog.csdn.net/qq_36142114/article/category/7383244)


HDFS（Hadoop Distributed File System ）Hadoop分布式文件系统。是根据google发表的论文翻版的。论文为GFS（Google File System）Google 文件系统（中文，英文）。
HDFS有很多特点：
1.保存多个副本，且提供容错机制，副本丢失或宕机自动恢复。默认存3份。
2.运行在廉价的机器上。
3.适合大数据的处理。HDFS默认会将文件分割成block，64M为1个block。然后将block按键值对存储在HDFS上，并将键值对的映射存到内存中。如果小文件太多，那内存的负担会很重。
![在这里插入图片描述](https://img-blog.csdn.net/20181003183509934?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
一、数据块(block)
1.HDFS(Hadoop Distributed File System)默认的最基本的存储单位是64M的数据块。
2. 和普通文件系统相同的是，HDFS中的文件是被分成64M一块的数据块存储的。
3.不同于普通文件系统的是，HDFS中，如果一个文件小于一个数据块的大小，并不占用整个数据块存储空间。
4.块大小设置为64m的原因：为了最小化寻址开销。如设置过大，会导致map数过少，集群利用率过低。如设置过小，要维护的元数据信息过多，由于元数据信息是存储在内存中的，所以可能会造成内存溢出。
二、HDFS数据节点
1.元数据节点(Namenode)
1.其将所有的文件和文件夹的元数据保存在一个文件系统树中
2.元数据中存放着文件包括哪些数据块以及这些数据分布到哪些DataNode数据节点上
3.VERSION——java properties文件，保存了HDFS的版本号。
4.fsimage——元数据的镜像文件，可以理解为元数据保存在磁盘上的一个副本
5.edits——修改日志文件，记录引发元数据改变的操作。
6.fstime——合并fimage和edits的时间
![在这里插入图片描述](https://img-blog.csdn.net/20181003183745136?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
2.数据节点（DataNode)
1.文件系统中真正存储数据的地方
2.客户端(client)或者元数据信息(namenode)可以向数据节点请求写入或者读出数据块。
3.其周期性的向元数据节点回报其存储的数据块信息。
![在这里插入图片描述](https://img-blog.csdn.net/20181003183833592?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
数据节点的VERSION文件格式如下：
![在这里插入图片描述](https://img-blog.csdn.net/20181003183851303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**blk_保存的是HDFS的数据块，其中保存了具体的二进制数据。**
**blk_.meta保存的是数据块的属性信息：版本信息，类型信息，和checksum**
**当一个目录中的数据块到达一定数量的时候，则创建子文件夹来保存数据块及数据块属性信息。**
**3.从元数据节点(secondary namenode)**
**1.从元数据节点并不是元数据节点出现问题时候的备用节点，它和元数据节点负责不同的事情。**
**2.其主要功能就是周期性将元数据节点的fsimage和edits合并，以防edits修改日志文件过大。**
**3.合并过后的命名空间镜像文件也在从元数据节点保存了一份，以防元数据节点失败的时候，可以恢复。**
**image**
**三、文件系统命名空间映像文件及修改日志**
**1.当文件系统客户端(client)进行写操作时，首先把它记录在修改日志中(edit log)**
**2.元数据节点在内存中保存了文件系统的元数据信息。在记录了修改日志后，元数据节点则修改内存中的数据结构。**
**3.每次的写操作成功之前，修改日志都会同步(sync)到文件系统。**
**4.fsimage文件，也即命名空间映像文件，是内存中的元数据在硬盘上的checkpoint，它是一种序列化的格式，并不能够在硬盘上直接修改。**
**5.同数据的机制相似，当元数据节点失败时，则最新checkpoint的元数据信息从fsimage加载到内存中，然后逐一重新执行修改日志中的操作。**
**6.从元数据节点就是用来帮助元数据节点将内存中的元数据信息checkpoint到硬盘上的**
**checkpoint的过程如下：**
**从元数据节点通知元数据节点生成新的日志文件，以后的日志都写到新的日志文件中。**
**从元数据节点用http get从元数据节点获得fsimage文件及旧的日志文件。**
**从元数据节点将fsimage文件加载到内存中，并执行日志文件中的操作，然后生成新的		fsimage文件。**
**从元数据节点奖新的fsimage文件用http post传回元数据节点**
**元数据节点可以将旧的fsimage文件及旧的日志文件，换为新的fsimage文件和新的日志	文件(第一步生成的)，然后更新fstime文件，写入此次checkpoint的时间。**
**这样元数据节点中的fsimage文件保存了最新的checkpoint的元数据信息，日志文件也重新开始，不会变的很大了。**
![在这里插入图片描述](https://img-blog.csdn.net/20181003184256785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM2MTQyMTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

