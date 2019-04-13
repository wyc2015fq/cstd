
# spark history server配置 - leofionn的博客 - CSDN博客


2018年03月22日 14:04:17[leofionn](https://me.csdn.net/qq_36142114)阅读数：81


spark history server的相关属性配置通过修改$SPARK_HOME/conf下的spark-default.conf文件来设定的
官方文档地址：[http://spark.apache.org/docs/latest/monitoring.html](http://spark.apache.org/docs/latest/monitoring.html)
相关 spark-default.conf 调优参数：

```python
属性名称	                           默认值	含义
spark.history.updateInterval	    10	        以秒为单位，更新日志相关的时间间隔
spark.history.retainedApplications  250	        保存Application历史记录的个数字，如果超过这个是值，旧的应用程序信息被删除
spark.history.ui.port	           18080	spark history server的web端口
spark.history.kerberos.principal	        用于histroyserver的kerberos主体名字
spark.history.kerberos.keytab		        用于histroyserver的kerberos keytab文件位置
spark.history.ui.acls.enable	   FALSE	授权用户查看应用程序信息时是否检查acl。如果启用，只有应用程序所有者和spark.ui.view.acls指定的用户可以查看应用程序信息，否则不做任何检查
spark.eventLog.enabled	           FLASE	是否记录spark事件
spark.eventLog.dir		                保存日志相关信息的路径，可以是hdfs，也可以说file，都需要提前创建
spark.yarn.historyServer.address		server端的URL：ip：port或者host：port
```
通常将日志文集相关的文件放置在hdfs上
进入spark/sbin目录，用如下命令启动historyserver：
./start-history-server.sh hdfs://usr/local/hadoop/log


【来自@若泽大数据】


