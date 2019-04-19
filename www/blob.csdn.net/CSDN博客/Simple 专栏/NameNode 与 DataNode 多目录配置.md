# NameNode 与 DataNode 多目录配置 - Simple 专栏 - CSDN博客
2015年04月08日 00:35:06[Simple_Zz](https://me.csdn.net/love284969214)阅读数：282标签：[大数据																[hadoop																[HDFS](https://so.csdn.net/so/search/s.do?q=HDFS&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)
个人分类：[hadoop生态圈](https://blog.csdn.net/love284969214/article/category/7975457)
所属专栏：[Hadoop 浅谈](https://blog.csdn.net/column/details/28029.html)
Namenode多目录配置
namenode的本地目录可以配置成多个，且每个目录存放内容相同，增加了可靠性。
```
hdfs-site.xml
<property>
    <name>dfs.namenode.name.dir</name>
    <value>file:///${hadoop.tmp.dir}/dfs/name1,file:///${hadoop.tmp.dir}/dfs/name2</value>
</property>
```
Datanode多目录配置
datanode也可以配置成多个目录，每个目录存储的数据不一样。即：数据不是副本。
```
hdfs-site.xml
<property>
    <name>dfs.datanode.data.dir</name>
    <value>file:///${hadoop.tmp.dir}/dfs/data1,file:///${hadoop.tmp.dir}/dfs/data2</value>
</property>
```
