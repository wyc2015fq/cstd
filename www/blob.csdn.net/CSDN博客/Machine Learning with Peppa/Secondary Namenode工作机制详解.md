# Secondary Namenode工作机制详解 - Machine Learning with Peppa - CSDN博客





2018年06月17日 23:19:53[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：403








SecondaryNameNode有两个作用，一是镜像备份，二是日志与镜像的定期合并。两个过程同时进行，称为checkpoint（检查点）。 从它的名字上看，它给人的感觉就像是NameNode的备份,但它实际上却不是。很多Hadoop的初学者都很疑惑，Secondary NameNode究竟是做什么的，以及它为什么会出现在HDFS中。




### NameNode

NameNode主要是用来保存HDFS的元数据信息，比如命名空间信息，块信息等。当它运行的时候，这些信息是存在内存中的。但是这些信息也可以持久化到磁盘上。

![](https://img-blog.csdn.net/20180617231606278?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面的这张图片展示了NameNode怎么把元数据保存到磁盘上的。这里有两个不同的文件：

- fsimage - 它是在NameNode启动时对整个文件系统的快照
- edit logs - 它是在NameNode启动后，对文件系统的改动序列

只有在NameNode重启时，edit logs才会合并到fsimage文件中，从而得到一个文件系统的最新快照。但是在产品集群中NameNode是很少重启的，这也意味着当NameNode运行了很长时间后，edit logs文件会变得很大。

在这种情况下就会出现下面一些问题：
- edit logs文件会变的很大，怎么去管理这个文件是一个挑战。
- NameNode的重启会花费很长时间，因为有很多改动要合并到fsimage文件上。

如果NameNode挂掉了，那我们就丢失了很多改动因为此时的fsimage文件非常旧。因此为了克服这个问题，我们需要一个易于管理的机制来帮助我们减小edit logs文件的大小和得到一个最新的fsimage文件，这样也会减小在NameNode上的压力。这跟Windows的恢复点是非常像的，Windows的恢复点机制允许我们对OS进行快照，这样当系统发生问题时，我们能够回滚到最新的一次恢复点上。

现在我们明白了NameNode的功能和所面临的挑战 - 保持文件系统最新的元数据。那么，这些跟Secondary NameNode又有什么关系呢？

![](https://img-blog.csdn.net/20180617231659235?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


Secondary NameNode所做的不过是在文件系统中设置一个检查点来帮助NameNode更好的工作。它不是要取代掉NameNode也不是NameNode的备份， SecondaryNameNode有两个作用，一是镜像备份，二是日志与镜像的定期合并。两个过程同时进行，称为checkpoint（检查点）。 
镜像备份的作用：备份fsimage(fsimage是元数据发送检查点时写入文件)； 日志与镜像的定期合并的作用：将Namenode中edits日志和fsimage合并,防止如果Namenode节点故障，namenode下次启动的时候，会把fsimage加载到内存中，应用edits log,edits log往往很大，导致操作往往很耗时。（这也是namenode容错的一套机制）

## Secondarynamenode工作过程 


SecondaryNameNode备份由三个参数控制fs.checkpoint.period控制周期（以秒为单位，默认3600秒），fs.checkpoint.size控制日志文件超过多少大小时合并（以字节为单位，默认64M）， dfs.http.address表示http地址，这个参数在SecondaryNameNode为单独节点时需要设置。
- SecondaryNameNode通知NameNode准备提交edits文件，此时主节点将新的写操作数据记录到一个新的文件edits.new中。 
- SecondaryNameNode通过HTTP GET方式获取NameNode的fsimage与edits文件（在SecondaryNameNode的current同级目录下可见到 temp.check-point或者previous-checkpoint目录，这些目录中存储着从namenode拷贝来的镜像文件）。 
- SecondaryNameNode开始合并获取的上述两个文件，产生一个新的fsimage文件fsimage.ckpt。 
- SecondaryNameNode用HTTP POST方式发送fsimage.ckpt至NameNode。 
- NameNode将fsimage.ckpt与edits.new文件分别重命名为fsimage与edits，然后更新fstime，整个checkpoint过程到此结束。 

![](https://img-blog.csdn.net/20180617232211889?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


从工作过程可以看出，SecondaryNameNode的重要作用是定期通过编辑日志文件合并命名空间镜像，以防止编辑日志文件过大。SecondaryNameNode一般要在另一台机器上运行，因为它需要占用大量的CPU时间与namenode相同容量的内存才可以进行合并操作。它会保存合并后的命名空间镜像的副本，并在namenode发生故障时启用。






