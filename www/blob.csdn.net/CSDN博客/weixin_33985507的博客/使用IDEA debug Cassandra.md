# 使用IDEA debug Cassandra - weixin_33985507的博客 - CSDN博客
2018年10月18日 17:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
- 下载源代码
- 进入cassandra源码目录，运行ant generate-eclipse-files(前提是安装了ant，如果是mac用户brew install ant很快就好了)
- 
点击+添加新的configuration,选择Application
![4055686-8fee8c694fded373.png](https://upload-images.jianshu.io/upload_images/4055686-8fee8c694fded373.png)
Configuration
其中Main class为org.apache.cassandra.service.CassandraDaemon
VM option为：
```
-Dcassandra-foreground=yes -Dcassandra.config=file://$PROJECT_DIR$/conf/cassandra.yaml -Dcassandra.storagedir=$PROJECT_DIR$/data -Dlogback.configurationFile=file://$PROJECT_DIR$/conf/logback.xml -Dcassandra.logdir=$PROJECT_DIR$/data/logs -ea -Xmx1G
```
- 点击debug按钮
- 在cassandra/bin目录下运行cqlsh即可
