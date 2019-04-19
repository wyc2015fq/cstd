# hadoop 集群开启之后datanode没有启动 - BlueSky - CSDN博客
2016年03月16日 10:29:25[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：13190
## datanode没有启动
将hadoop配置文件修改后，重新格式化了hadoop集群，即
`hadoop name -format`但是此时发现slave节点没有启动datanode
上网查找解决方法，最后终于解决了，解决方法如下：
1. 先执行stop-all.sh暂停所有服务
2. 将所有Salve节点上的tmp(即 hdfs-site.xml 中指定的 dfs.data.dir 文件夹，DataNode存放数据块的位置)、 logs 文件夹删除 ， 然后重新建立tmp , logs 文件夹
3. 将所有Salve节点上的/usr/hadoop/conf下的core-site.xml删除，将master节点的core-site.xml文件拷贝过来，到各个Salve节点
- 
scp /usr/hadoop/conf/core-site.xml   hadoop@slave1:/usr/hadoop/conf/
复制代码
4. 重新格式化: hadoop namenode -format
5. 启动：start-all.sh
**hadoop中启动namenode等出现的一些问题**
1、先运行stop-all.sh
2、格式化namdenode，不过在这之前要先删除原目录，即core-site.xml下配置的<name>hadoop.tmp.dir</name>所指向的目录，删除后切记要重新建立配置的空目录，然后运行hadoop namenode -format
3、运行start-all.sh
以上顺序很重要！！！
