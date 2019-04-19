# HDFS  文件快大小 - Simple 专栏 - CSDN博客
2015年03月03日 17:52:12[Simple_Zz](https://me.csdn.net/love284969214)阅读数：487标签：[大数据																[Hadoop																[block																[HDFS](https://so.csdn.net/so/search/s.do?q=HDFS&t=blog)](https://so.csdn.net/so/search/s.do?q=block&t=blog)](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)
个人分类：[hadoop生态圈](https://blog.csdn.net/love284969214/article/category/7975457)
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
![](https://img-blog.csdn.net/20180928202507986?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xvdmUyODQ5NjkyMTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)HDFS  文件块大小
- HDFS中的文件在物理上是分块存储（block），块的大小可以通过配置参数( dfs.blocksize)来规定，默认大小在hadoop2.x版本中是128M，老版本中是64M。
- HDFS的块比磁盘的块大，其目的是为了最小化寻址开销。如果块设置得足够大，从磁盘传输数据的时间会明显大于定位这个块开始位置所需的时间。因而，传输一个由多个块组成的文件的时间取决于磁盘传输速率。
- 如果寻址时间约为10ms，而传输速率为100MB/s，为了使寻址时间仅占传输时间的1%，我们要将块大小设置约为100MB。默认的块大小128MB。
- 块的大小：10ms*100*100M/s = 100M
### 设置块大小
设置 ** hdfs-site.xml  **下面的属性值
```
<property>
    <name>dfs.block.size</name>
    <value>134217728</value>    <-- 设置为128M -->
    <description>Block size</description>
</property>
```
修改说明
hdfs-site.xml  是HDFS的配置文件，修改  hdfs-site.xml  配置文件的  dfs.block.size  属性值就会改变上传到HDFS所有文件的默认块大小。修改这个值并不会对HDFS上现存的任何文件的块大小产生影响，只会改变新上传文件的块大小。
