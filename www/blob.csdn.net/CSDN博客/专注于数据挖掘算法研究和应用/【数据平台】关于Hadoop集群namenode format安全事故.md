# 【数据平台】关于Hadoop集群namenode format安全事故 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月25日 14:13:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：273








1、问题：重启namenode后提示连接失败，居然执行了format命令，出现了很严重的安全事故。

2、教训：对于生产集群，一定要建立运维安全体系，对于高危命令要做权限控制。

3、对于备份的理解：

      1）由于所操作的集群namenode和secondary namenode是同一台机子，secondary只发挥在namenode当机下充当namenode的作用，而没有独立保存元数据结构和日志，导致无法从secondary恢复数据。

      2）对于dfs.namenode.name.dir路径下namenode保存的fsimage_和edits_要预先做备份，最好是设置双路径，以便从fsimage_和edits_中恢复。

hdfs-site.xml



```
<property>
<name>dfs.namenode.name.dir</name>
    <value>/data/hadoop-2.7.2/current/dfs/name</value>
  </property>

   <property>
       <name>dfs.datanode.data.dir</name>
       <value>/data/hadoop-2.7.2/current/data</value>
   </property>
```
core-site.xml

```
<property>
        <name>hadoop.tmp.dir</name>
        <value>/data/hadoop-2.7.2/current/tmp</value>
    </property>
```


      上面三个路径的数据在format前要注意备份。




4、对于format前的工作，要先前上面三个路径备份后再执行。




5、hadoop的集群有不同的情况，生产的集群和研发的集群在安全级别上有不同的管理。




6、checkpoint参考：http://www.cnblogs.com/hanyuanbo/archive/2012/07/25/2608698.html





