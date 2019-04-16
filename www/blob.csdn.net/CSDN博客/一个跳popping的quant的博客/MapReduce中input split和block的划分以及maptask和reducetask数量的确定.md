# MapReduce中input split和block的划分以及maptask和reducetask数量的确定 - 一个跳popping的quant的博客 - CSDN博客





2017年08月17日 21:52:18[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：782








学了mapreudce框架没多久，虽然会写很多的一些常见的案例，但是这个框架中一些本质地方的实现还是有很多疑惑的。所以就想整理一下最近所学到的东西。 

  首先先理清楚block和split的区别。 

  1、block块 

  当把文件上传到hdfs中时，第一步要进行的操作就是划分数据，数据在hdfs中被划分为一块一块的，默认的大小是64MB，这个块的大小可以在haddop-default.xml中配置。另外，如果一个文件的大小占据了二点几个block，那么那零点几个的大小虽然占据了一个block，但是它在文件系统实际占据的大小，就是它本身的大小。可以参考[http://blog.csdn.net/samhacker/article/details/23089157](http://blog.csdn.net/samhacker/article/details/23089157) block的数据划分是在**物理层次**上的划分。 

  2、split块 

  mapreduce为了更好的获取和处理数据，就需要对文件进一步进行**逻辑上的划分**，这种划分的结果就是Input split（输入分片）。而每个输入分片都针对一个map任务，输入分片存储的并不是数据本身，儿时一个分片的长度和一个记录数据位置的数组。而一个分片的大小的确定则是通过源码和配置文件决定的。具体可以查看FileInputFormat的源码对splits的操作。
从配置文件中获取split的最大和最小的值： 

   其中minsize默认为1，和block块的大小一致，maxsize默认的大小为Long.MaxValue（巨大无比的一个数字）

```
public static void setMinInputSplitSize(Job job,long size) {
    job.getConfiguration().setLong("mapred.min.split.size",size);
  }

  public static void setMaxInputSplitSize(Job job,long size) {
    job.getConfiguration().setLong("mapred.max.split.size",size);
  }
```

在FileInputFormat类中还有一个computeSplitSize()的方法来计算一个split的大小：

所以这里如果blockSize是3，那么Math.min先返回最小的是3，然后3与minSize（默认是1）进行比较，然后返回最大的是3。 

如果blockSize不足1，那么同理返回的就是minSize，也就是1。

```
protected long computeSplitSize(long blockSize, long minSize,long maxSize) {
    return Math.max(minSize, Math.min(maxSize, blockSize));
  }
```

最后，两者的关系就是 

  1、一个split要大于或者等于一个整数的Block。 

  2、一个split包含的是一个文件中的Block，不会跨域File的边界。
maptask和reducetask的个数的确定： 

1、关于maptask的个数 

在hadoop的配置文件中，还有一个配置的参数是mapred.map.tasks，来控制map任务的个数，但是它的数量的最终确定并不是每次都是通过这个确定的，还要取决于其他的因素。具体可以查看 [http://www.cnblogs.com/junneyang/p/5850440.html](http://www.cnblogs.com/junneyang/p/5850440.html) 从这篇文章和以上的分析还可以得到几个改变maptask数量的结论： 

（1）如果想增加map个数，则设置mapred.map.tasks 为一个较大的值。 

（2）如果想减小map个数，则设置mapred.min.split.size 为一个较大的值。 

另外一个split对应一个maptask，改变maptask的个数就可以通过修改这几个参数来实现。
2、reducetask的个数的确定 

可以通过代码job.setNumReduceTasks(10);来设定。






