# 跨Hadoop平台Hive表export和import迁移 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年03月16日 09:01:28[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6930
所属专栏：[Hadoop专栏](https://blog.csdn.net/column/details/16284.html)









                
场景：两个Hadoop平台集群之间Hive表迁移。

思路：Hive表元数据和文件数据export到HDFS文件，通过Distcp将HDFS迁移到另一个集群的HDFS文件，再通过import在新Hive中建表；



1、原平台：Hive表元数据和文件数据到export到平台HDFS文件

   1）hive下执行如下命令：


`#export table conf_adsl_app to '/tmp/hive-export/conf_adsl_app';`conf_adsl_app表的外部存储路径：hdfs://nameservice-ha/user/hive/warehouse/conf_adsl_app

   2）可以看到hdfs输出目录下有元数据和文件数据；



```
hadoop fs -ls /tmp/hive-export/conf_adsl_app/
     Found 2 items
     -rwxr-xr-x   3 hdfs supergroup       1309 2017-03-16 08:03 /tmp/hive-export/conf_adsl_app/_metadata
     drwxr-xr-x   - hdfs supergroup          0 2017-03-16 08:03 /tmp/hive-export/conf_adsl_app/data
```
  3）3）查看hadoop fs -cat /tmp/hive-export/conf_adsl_app/_metadata元文件

      可看到文件路径是：hdfs://nameservice-ha/user/hive/warehouse/conf_adsl_app

      在新平台下不存在/user/hive/warehouse/conf_adsl_app目录，import时会自动生成相应目录。



2、用distcp从原平台将/tmp/hive-export/conf_adsl_app/下数据迁移到新平台。

   注意下面用的distcp语法涉及到从非安全集群迁移到安全集群(kerberos)的操作。

   在新平台执行命令：


`hadoop distcp -D ipc.client.fallback-to-simple-auth-allowed=true -D dfs.checksum.type=CRC32 webhdfs://namenode ip:50070/tmp/hive-export/conf_adsl_app/ /flume/his/`

可以看到新平台/flume/his目录下有文件：




```
hadoop fs -ls /flume/his/conf_adsl_app/
Found 2 items
-rw-r--r--   3 etl etl       1309 2017-03-16 08:49 /flume/his/conf_adsl_app/_metadata
drwxr-xr-x   - etl etl          0 2017-03-16 08:49 /flume/his/conf_adsl_app/data
```


3、新平台：从HDFS文件import到Hive生成新表

   1)hive下执行命令：


`#import table new_conf_adsl_app from '/flume/his/conf_adsl_app';`2)可以看到hive上新生成了new_conf_adsl_app表

     LOCATION

     'hdfs://c9/apps/hive/warehouse/cwh.db/new_conf_adsl_app'

   3)查看location目录下hive表对应的文件



```
hadoop fs -ls /apps/hive/warehouse/cwh.db/new_conf_adsl_app
Found 1 items
-rwxrwxrwx   3 etl hdfs      18227 2017-03-16 08:51 /apps/hive/warehouse/cwh.db/new_conf_adsl_app/adsl_apps_20130829.txt
```


自此跨平台Hive表迁移完成。




