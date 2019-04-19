# Spark分布式集群搭建（三）安装job history server +intellj idea设置 - shine302的博客 - CSDN博客
2018年04月29日 22:21:28[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：81
SPARK_HOME/conf 下:
# spark-defaults.conf 增加如下内容
spark.eventLog.enabled true 
spark.eventLog.dir hdfs://master:8020/var/log/spark 
spark.eventLog.compress true
# spark-env.sh 增加如下内容
export SPARK_HISTORY_OPTS=”-Dspark.history.ui.port=18080 -Dspark.history.retainedApplications=3 -Dspark.history.fs.logDirectory=hdfs:/master:8020/var/log/spark”
# 启动start-history-server.sh
SPARK_HOME/conf 下: 执行 ./start-history-server.sh
# spark job history web: master:18080
这样在spark任务运行完成之后,就可以通过web页面查看日志了
intellj idea设置
代码中配置
System.setProperty("spark.eventLog.enabled", "true");
System.setProperty("spark.eventLog.dir", "hdfs://172.20.161.96:9000/history/");
