# JDK自带jvisualvm监控tomcat - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月17日 17:32:10[boonya](https://me.csdn.net/boonya)阅读数：1729








**原文地址：[http://my.oschina.net/kone/blog/157239](http://my.oschina.net/kone/blog/157239)**

**前提条件：本地的Tomcat 在jvisualvm启动后自动会监控起来，如果是要监控远程的Linux服务器的Tomcat需要在Linux中配置JMX。**


jdk自带有个jvisualvm工具、该工具是用来监控java运行程序的cpu、内存、线程等的使用情况。并且使用图表的方式监控java程序、还具有远程监控能力。不失为一个用来监控tomcat的好工具。


在jdk目录下的bin目录中可以找到jvisualvm.exe文件、直接启动可以看到如下界面： 

![](http://static.oschina.net/uploads/space/2013/0827/154728_CGjA_146430.png)

以上是已经连接远程的界面。直观的监控界面、更有助于我们分析tomcat的运行情况。 

下面主要介绍下怎么样使用本地jvisualvm监控远程tomcat。

tomcat自带的监控介绍可以参考：

[http://tomcat.apache.org/tomcat-6.0-doc/monitoring.html](http://tomcat.apache.org/tomcat-6.0-doc/monitoring.html)

下面是我的配置方法：

 在tomcat/bin目录下打开startup.sh或者startup.bat,在最后一行前面加上，也就是：





[?](http://my.oschina.net/kone/blog/157239#)

```
```java
exec
```

```java
"$PRGDIR"
```

```java
/
```

```java
"$EXECUTABLE"
```

```java
start
```

```java
"$@"
```
```






前面加上





[?](http://my.oschina.net/kone/blog/157239#)

```
```java
export CATALINA_OPTS="$CATALINA_OPTS
```
```





[?](http://my.oschina.net/kone/blog/157239#)

```
```java
-Dcom.sun.management.jmxremote -Djava.rmi.server.hostname=xxx.xxx.xxx.xxx  -Dcom.sun.management.jmxremote.port=xxx -Dcom.sun.management.jmxremote.ssl=
```

```java
false
```

```java
-Dcom.sun.management.jmxremote.authenticate=
```

```java
true
```

```java
-Dcom.sun.management.jmxremote.password.file=../conf/jmxremote.password  -Dcom.sun.management.jmxremote.access.file=../conf/jmxremote.access"
```
```



参数说明： 






[?](http://my.oschina.net/kone/blog/157239#)

```
```java
-Djava.rmi.server.hostname=xxx.xxx.xxx.xxx 主机地址，我配置的是外网地址
```
```





[?](http://my.oschina.net/kone/blog/157239#)

```
```java
-Dcom.sun.management.jmxremote.port=xxx  端口号、配置完端口号之后需要将此端口开放出来  可以使用命令：iptables -A INPUT -p tcp --d port  端口号 -j ACCEPT
```
```





[?](http://my.oschina.net/kone/blog/157239#)

```
```java
-Dcom.sun.management.jmxremote.authenticate=
```

```java
true
```

```java
是否开启认证、
```

```java
false
```

```java
的时候可以不使用密码访问
```
```





[?](http://my.oschina.net/kone/blog/157239#)

```
```java
-Dcom.sun.management.jmxremote.password.file=../conf/jmxremote.password -Dcom.sun.management.jmxremote.access.file=../conf/jmxremote.access  以上配置是开启认证时使用的access文件和password文件
```
```



其中jmxremote.access文件的内容可以配置为：



[?](http://my.oschina.net/kone/blog/157239#)

```
```java
username1 readonly
```



```java
username2 readwrite
```
```



jmxremote.password文件可以配置为： 



[?](http://my.oschina.net/kone/blog/157239#)

```
```java
username1 password1
```



```java
username2 password2
```
```



将以上俩文件放在对应配置目录tomcat/conf/下

并且将以上俩个文件访问权限配置成： 



[?](http://my.oschina.net/kone/blog/157239#)

```
```java
chmod
```

```java
600
```

```java
jmx*
```
```



如果没有这一步、使用startup启动不了tomcat。

按照上述配置、再使用startup.sh启动tomcat。 

接下来就是使用jvisualvm来远程监控tomcat了

右键点击远程-添加远程主机-输入主机ip地址 

![](http://static.oschina.net/uploads/space/2013/0827/161014_dnoT_146430.png)


![](http://static.oschina.net/uploads/space/2013/0827/161026_bidh_146430.png)


添加远程主机之后，点击远程主机右键-添加jmx连接：

![](http://static.oschina.net/uploads/space/2013/0827/161216_lIXy_146430.png)


弹出登录框 

![](http://static.oschina.net/uploads/space/2013/0827/161301_b53F_146430.png)

输入主机名:端口号，如果配置了用户名和密码，请输入用户名和密码。然后点击确定。 

点击确定之后、在远程主机下会有一个jmx连接，打开连接，点击监视tab，可以看到如下界面了。 

![](http://static.oschina.net/uploads/space/2013/0827/161509_miAJ_146430.jpg)

在上图中可以看到cpu利用率和垃圾回收活动（这个在分析tomcat性能时也很重要）。然后是堆栈使用情况。下面是类的使用情况，最后一个是线程活动情况。

点击线程tab可以看到： 

![](http://static.oschina.net/uploads/space/2013/0827/161831_SIY1_146430.png)

上图可以非常清晰的看到线程活动情况，那些线程正在执行，哪些线程正在等待中，以及执行完毕的线程等。 

这里可以看到每个线程的状态，点击某个线程右键可以查看该线程的详细情况： 

![](http://static.oschina.net/uploads/space/2013/0827/162045_ZkaY_146430.png)

使用左上角的线程dump按钮，还可以看到线程的堆栈情况，这样就可以具体分析线程是在什么地方进入等待，什么地方进入休眠，以及什么地方一直处于执行状态。这也是一个性能分析利器哦！


![](http://static.oschina.net/uploads/space/2013/0827/174110_s01F_146430.png)

以上就是使用jvisualvm的jmx远程监控tomcat的简单配置。 



