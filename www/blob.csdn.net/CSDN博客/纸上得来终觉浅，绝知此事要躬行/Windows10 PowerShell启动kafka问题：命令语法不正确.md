# Windows10 PowerShell启动kafka问题：命令语法不正确 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月28日 10:47:52[boonya](https://me.csdn.net/boonya)阅读数：1004标签：[kafka																[start](https://so.csdn.net/so/search/s.do?q=start&t=blog)](https://so.csdn.net/so/search/s.do?q=kafka&t=blog)
个人分类：[ApacheKafka](https://blog.csdn.net/boonya/article/category/6722001)








Kafka依赖环境：JDK、Zookeeper，首先确保Java环境JDK已安装，其次Zookeeper必须在kafaka启动之前启动。

报错问题：

```
PS C:\DEVELOPERS\ApacheKafka\kafka_2.11-2.1.0> .\bin\windows\kafka-server-start.bat .\config\server.properties
命令语法不正确。
错误: 找不到或无法加载主类 Files\Java\jdk1.8.0_121\lib;C:\Program
PS C:\DEVELOPERS\ApacheKafka\kafka_2.11-2.1.0> .\bin\windows\kafka-server-start.bat .\config\server.properties
命令语法不正确。
错误: 找不到或无法加载主类 Files\Java\jdk1.8.0_121\lib;C:\Program
PS C:\DEVELOPERS\ApacheKafka\kafka_2.11-2.1.0> .\bin\windows\kafka-server-start.bat .\config\server.properties
错误: 找不到或无法加载主类 Files\Java\jdk1.8.0_121\lib;C:\Program
```

解决方法：

 在kafka安装目录中找到bin\windows目录中的kafka-run-class.bat为%CLASSPATH%加上双引号

```
#修改前
set COMMAND=%JAVA%%KAFKA_HEAP_OPTS% %KAFKA_JVM_PERFORMANCE_OPTS% %KAFKA_JMX_OPTS%%KAFKA_LOG4J_OPTS% -cp%CLASSPATH% %KAFKA_OPTS% %*   

#修改后
set COMMAND=%JAVA%%KAFKA_HEAP_OPTS% %KAFKA_JVM_PERFORMANCE_OPTS% %KAFKA_JMX_OPTS%%KAFKA_LOG4J_OPTS% -cp"%CLASSPATH%"%KAFKA_OPTS% %*
```

运行命令：

定位到kafka目录，shift+鼠标右键，打开PowerShell命令窗口执行如下语句：
`.\bin\windows\kafka-server-start.bat.\config\server.properties`
![](https://img-blog.csdnimg.cn/20181228104513584.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

这样就可以正常启动了。 

Zookeeper&Kafka安装配置参考：[https://www.cnblogs.com/mrblue/p/6425309.html](https://www.cnblogs.com/mrblue/p/6425309.html)

windows10启动错误参考：[https://blog.csdn.net/tianmanchn/article/details/78943147](https://blog.csdn.net/tianmanchn/article/details/78943147)






