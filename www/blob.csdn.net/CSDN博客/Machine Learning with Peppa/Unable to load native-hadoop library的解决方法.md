# Unable to load native-hadoop library的解决方法 - Machine Learning with Peppa - CSDN博客





2018年05月27日 20:35:16[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：2764








今天跑Hadoop的文件命令，报了一个这样的错误（其实是warning）：

![](https://img-blog.csdn.net/20180527203137134?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





虽说不影响任务进程，不过看起来确实不好看（强迫症受不了）。自己捣鼓了一下发现是编译版本不对齐导致的问题，重新编译过于浪费时间，所以直接把警告消息mute掉就好：

具体操作是直接在log4j日志中去除告警信息。在/$HADOOP_HOME/etc/hadoop/log4j.properties

```java
cd /usr/local/hadoop-2.9.1/etc/hadoop/
vim log4j.properties
```

在文件最后一行加入：

```java
log4j.logger.org.apache.hadoop.util.NativeCodeLoader=ERROR
```

再运行命令，显示一切正常



