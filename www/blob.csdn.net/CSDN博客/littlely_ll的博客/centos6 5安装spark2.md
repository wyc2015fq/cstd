# centos6.5安装spark2 - littlely_ll的博客 - CSDN博客





2017年06月25日 20:37:50[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：628








[centos6.5下安装hadoop2](http://blog.csdn.net/littlely_ll/article/details/73729454)

centos安装好JAVA JDK，hadoop2.7，scala后，可以直接安装spark2了。安装spark还是比较简单的。首先是下载好[spark](http://spark.apache.org/downloads.html)(**需要和Scala版本对应**)，解压到一个目录中，然后在环境变量中添加spark的路径。另外，在spark路径下的conf文件下的spark-env.sh.template修改为spark-env.sh（`mv spark-env.sh.template spark-env.sh`），并对其内容增加环境变量：

```bash
export SPARK_MASTER_IP='your ip'
export SPARK_LOCAL_IP='your ip' #对于单节点master和local都一样的ip
export JAVA_HOME=/usr/java/jdk1.7.0_80
export SCALA_HOME=/scala/scala-2.11.11
export HADOOP_HOME=/itcast/hadoop-2.7.3
export HADOOP_CONF_DIR=/itcast/hadoop-2.7.3/etc/hadoop
```

还得需要把

```bash
export JRE_HOME=$JAVA_HOME/jre
export CLASSPATH=.$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar:$JAVA_HOME/jre/lib/rt.jar
```

添加到环境变量（即`vim /etc/profile`添加）。最后`source /etc/profile`，至此，可以启动spark了，到你的spark路径下
`sbin/start-all.sh`


