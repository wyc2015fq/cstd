# Eclipse远程调试Spark - Soul Joy Hub - CSDN博客

2017年04月11日 13:52:49[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1973


# 修改配置文件

修改${spark_home}/bin/spark-class文件：
`"$RUNNER" -Xmx128m -cp "$LAUNCH_CLASSPATH" org.apache.spark.launcher.Main -Xdebug -Xrunjdwp:transport=dt_socket,server=y,suspend=y,address=8888 "$@"`
![这里写图片描述](https://img-blog.csdn.net/20170411134118037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

参数说明：
- 
-Xdebug 启用调试特性

- 
-Xrunjdwp 启用JDWP实现，包含若干子选项：

- 
transport=dt_socket JPDA front-end和back-end之间的传输方法。dt_socket表示使用套接字传输。

- 
address=8888 JVM在8888端口上监听请求，这个设定为一个不冲突的端口即可。

- 
server=y 

y表示启动的JVM是被调试者。如果为n，则表示启动的JVM是调试器。

- 
suspend=y y表示启动的JVM会暂停等待，直到调试器连接上才继续执行。suspend=n，则JVM不会暂停等待。

# 启动Spark Application

用`eclipse`将项目`export`出`jar`包，传到集群上使用`spark-submit`提交`Application`，可以看到在等待：

![这里写图片描述](https://img-blog.csdn.net/20170411134419082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**注意，传到集群上的代码要和本地一样！**

# 在Eclipse设置远程调试的ip和port
- Debug as
- Debug Configurations 
- Remote Java Application
- 配置ip和port 
![这里写图片描述](https://img-blog.csdn.net/20170612224831276?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

