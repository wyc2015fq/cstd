# 修改zookeeper日志 - weixin_33985507的博客 - CSDN博客
2017年07月29日 20:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：11
# 1.修改/opt/zookeeper-3.4.6/bin/zkEnv.sh
```
if [ "x${ZOO_LOG_DIR}" = "x" ]
then
    ZOO_LOG_DIR="/apps/zookeeper/logs/"
fi
if [ "x${ZOO_LOG4J_PROP}" = "x" ]
then
    ZOO_LOG4J_PROP="INFO,ROLLINGFILE"
fi
```
![4618795-6281a33af773580a.png](https://upload-images.jianshu.io/upload_images/4618795-6281a33af773580a.png)
图片.png
# 2.修改 /opt/zookeeper-3.4.6/conf/log4j.properties
```
zookeeper.root.logger=INFO,ROLLINGFILE
log4j.appender.ROLLINGFILE=org.apache.log4j.DailyRollingFileAppender
#log4j.appender.ROLLINGFILE.MaxFileSize=10MB
```
![4618795-0ada8443678dbc21.png](https://upload-images.jianshu.io/upload_images/4618795-0ada8443678dbc21.png)
图片.png
# 3.修改 /opt/zookeeper-3.4.6/bin/zkServer.sh
`可以不修改该文件，只是zk的启动脚本默认用nohup 启动会生成一个zookeeper.out的空文件`
```
#_ZOO_DAEMON_OUT="$ZOO_LOG_DIR/zookeeper.out"
 "$JAVA" "-Dzookeeper.log.dir=${ZOO_LOG_DIR}" "-Dzookeeper.root.logger=${ZOO_LOG4J_PROP}" -cp "$CLASSPATH" $JVMFLAGS $ZOOMAIN "$ZOOCFG" &
```
![4618795-78bc544a95c8c8de.png](https://upload-images.jianshu.io/upload_images/4618795-78bc544a95c8c8de.png)
图片.png
