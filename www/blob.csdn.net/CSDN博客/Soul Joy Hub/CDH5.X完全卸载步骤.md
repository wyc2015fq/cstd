# CDH5.X完全卸载步骤 - Soul Joy Hub - CSDN博客

2016年08月30日 19:25:56[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：924标签：[集群																[cloudera																[cdh																[hadoop																[大数据](https://so.csdn.net/so/search/s.do?q=大数据&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=cdh&t=blog)](https://so.csdn.net/so/search/s.do?q=cloudera&t=blog)](https://so.csdn.net/so/search/s.do?q=集群&t=blog)
个人分类：[大数据平台](https://blog.csdn.net/u011239443/article/category/6296513)


							http://blog.csdn.net/wulantian/article/details/42706777

//CDH5.X完全卸载步骤

# by coco

# 2015-01-14

1. 关闭集群中的所有服务。

 这个可以通过clouder manger 主页关闭集群。

2. 卸载

[root@master ~]# /usr/share/cmf/uninstall-cloudera-manager.sh 

[root@slave1 ~]# service cloudera-scm-agent stop

[root@slave1 ~]# service cloudera-scm-agent stop

一下都是所有要卸载的集群均要执行清除工作：

[root@master ~]# umount /var/run/cloudera-scm-agent/process

[root@slave1 ~]# umount /var/run/cloudera-scm-agent/process

[root@slave2 ~]# umount /var/run/cloudera-scm-agent/process


[root@master ~]# rm -rf /usr/share/cmf /var/lib/cloudera* /var/cache/yum/x86_64/6/cloudera* /var/log/cloudera* /var/run/cloudera*  /etc/cloudera* 

3. 卸载安装包：

[root@slave1 ~]# rpm -qa | grep cloudera

[root@slave2 ~]# for f in `rpm -qa | grep cloudera `  ; do rpm -e ${f} ; done     （如果有保存，在执行一遍）

4. 清除安装文件

 rm -rf /var/lib/hadoop-* /var/lib/impala /var/lib/solr /var/lib/zookeeper /var/lib/hue /var/lib/oozie  /var/lib/pgsql  /var/lib/sqoop2  /data/dfs/  /data/impala/ /data/yarn/  /dfs/ /impala/ /yarn/
  /var/run/hadoop-*/ /var/run/hdfs-*/ /usr/bin/hadoop* /usr/bin/zookeeper* /usr/bin/hbase* /usr/bin/hive* /usr/bin/hdfs /usr/bin/mapred /usr/bin/yarn /usr/bin/sqoop* /usr/bin/oozie /etc/hadoop* /etc/zookeeper* /etc/hive* /etc/hue /etc/impala /etc/sqoop* /etc/oozie
 /etc/hbase* /etc/hcatalog 

//只删除hadoop系列的，不要删除其他软件的，否则其他软件的版本控制会被破坏

[root@master alternatives]# rm -rf ` find /var/lib/alternatives/* ! -name "mta" ! -name "print" ! -name "zlibrary-ui"  -mtime -3` 

[root@master alternatives]# rm -rf /etc/alternatives/* 

5. 杀死相关进程：

for u in hdfs mapred cloudera-scm hbase hue zookeeper oozie hive impala flume; do sudo kill $(ps -u $u -o pid=); done

6. 删除parcel包分发文件和解压文件

  rm -rf /opt/cloudera/parcel-cache /opt/cloudera/parcels

到此卸载完毕。





