# Hadoop文件压缩格式分析和比较 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年07月10日 08:29:22[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3131
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









Hadoop支持压缩格式：
|压缩格式|可分割|算法|扩展名|Linux工具|
|----|----|----|----|----|
|gzip|否|DEFLATE|.gz|gzip|
|lzo|是(加索引)|LZO|.lzo|lzop|
|snappy|否|Snappy|.snappy|无|
|Bzip2|是|Bzip2|.bz2|bzip2|
|deflate|否|DEFLATE|.deflate|无|
|zip|是|ZIP|.zip|zip|



**1）Gzip压缩**

优点：压缩率比较高，压缩/解压速度也比较快，hadoop本身支持。

缺点：不支持分片。

应用场景：当每个文件压缩之后在1个block块大小内，可以考虑用gzip压缩格式。



**2）lzo压缩**

优点：压缩/解压速度也比较快，合理的压缩率，支持分片，是Hadoop中最流行的压缩格式，支持Hadoop native库。

缺点：压缩率比gzip要低一些，Hadoop本身不支持，需要安装，如果支持分片需要建立索引，还需要指定inputformat改为lzo格式。

应用场景：一个很大的文本文件，压缩之后还大于200M以上的可以考虑，而且单个文件越大，lzo优点越明显。



**3）snappy压缩**

优点：支持Hadoop native库，高速压缩速度和合理的压缩率。

缺点：不支持分片，压缩率比gzip要低，Hadoop本身不支持，需要安装。

应用场景：当MapReduce作业的map输出的数据比较大的时候，作为map到reduce的中间数据的压缩格式。



**4）bzip2压缩**

优点：支持分片，具有很高的压缩率，比gzip压缩率都高，Hadoop本身支持，但不支持native。

缺点：压缩/解压速度慢，不支持Hadoop native库。

应用场景：适合对速度要求不高，但需要较高的压缩率的时候，可以作为mapreduce作业的输出格式，输出之后的数据比较大，处理之后的数据需要压缩存档减少磁盘空间并且以后数据用得比较少的情况。



总结：压缩比：bzip2 > gzip > lzo > snappy ，压缩速度：snappy > lzo> gzip > bzip2



