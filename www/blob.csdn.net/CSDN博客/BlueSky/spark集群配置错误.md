# spark集群配置错误 - BlueSky - CSDN博客
2015年11月17日 15:17:35[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：365
1.当用hdfs传文件时，出现如下错误： 
Call to Master:9000 failed on connection exception 
解决办法：把hadoop配置文件中的hdfs-site.xml中 
                dfs.data.dir 
                /data/hdfs/data  
中的文件夹中的所有文件删掉 
然后在hadoop namenode -format //格式化namedone节点 
再重启即可 
2.启动spark集群的时候出现： 
Call to master/192.168.137.101:9001 failed on connection exception: java.net.ConnectException: Connection refused 
解决办法:同上！
