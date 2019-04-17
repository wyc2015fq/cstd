# Hadoop基于Linux-CentOS7安装-安装Hadoop - DEVELOPER - CSDN博客





2017年10月08日 21:09:31[学术袁](https://me.csdn.net/u012827205)阅读数：847标签：[Hadoop](https://so.csdn.net/so/search/s.do?q=Hadoop&t=blog)
个人分类：[Hadoop](https://blog.csdn.net/u012827205/article/category/7211245)

所属专栏：[Hadoop-学习](https://blog.csdn.net/column/details/20399.html)









在此之前首先提供一个 xshell 的免费地址：[http://www.netsarang.com/download/free_license.html](http://www.netsarang.com/download/free_license.html)

安装Hadoop首先要安装jdk和配置jdk环境

![](https://img-blog.csdn.net/20171008212058121?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171008212132226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们使用yum命令进行在线安装，yum命令进行安装是不能指定软件的安装目录的。

![](https://img-blog.csdn.net/20171008213905895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171008213914990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


我们要记住，jdk的目录地址就是：/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.144-0.b01.el7_4.x86_64

接下来，我们配置JDK的环境

![](https://img-blog.csdn.net/20171008215420106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171008215430373?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```java
export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.144-0.b01.el7_4.x86_64
export CLASSPATH=.:$CLASSPATH:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
export PATH=$PATH:$JAVA_HOME/bin:$JAVA_HOME/jre/bin
```
只是更改第一行代码，换做你自己的代码；其他不变~



至此，JDK环境配置完毕！！

接下来再安装配置Hadoop环境

这里我们需要使用一个软件（winSCP），她可以在windows和linux系统之间进行信息传递交换。我们现在windows上下载一个Hadoop-0.20.2.tar.gz的Liunux压缩包。使用winSCP从windows上传递到Linux中。这个是Hadoop-0.20.2.tar.gz的地址，[点击下载](http://download.csdn.net/download/u012827205/10010736)

![](https://img-blog.csdn.net/20171008220813264?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171008220839544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171008221654265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


当下，我们就要配置Hadoop的环境了；这个是麻烦的，要注意-谨慎操作~

还是要进入之前配置JDK的环境配置文件中，然后再末尾再加一点Hadoop 的内容。

![](https://img-blog.csdn.net/20171008223636301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171008223739777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


接下来就是配置目录 /home/123456/hadoop-0.20.2/conf 下的文件-四个

![](https://img-blog.csdn.net/20171008224242827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


配置：hadoop-env.sh

![](https://img-blog.csdn.net/20171008232932979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


配置：core-site.xml<端口号是固定的>



```java
<configuration>
<property>
 <name>hadoop.tmp.dir</name>
  <value>/hadoop</value>
</property>
<property>
  <name>hadoop.name.dir</name>
 <value>/hadoop/name</value>
</property>
<property>
<name>fs.default.name</name>
<value>hdfs://localhost.localdomain:9000</value>
</property>
</configuration>
```
![](https://img-blog.csdn.net/20171008225115676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




配置：hdfs-site.xml



```java
<configuration>
<property>
<name>dfs.data.dir</name>
<value>/hadoop/data</value>
</property>
</configuration>
```
![](https://img-blog.csdn.net/20171008225420821?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




配置：mapred-site.xml<端口号是固定的>



```java
<configuration>
<property>
  <name>mapred.job.tracker</name>
  <value>localhost.localdomain:9001</value>
</property>
</configuration>
```
![](https://img-blog.csdn.net/20171008231256358?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




中间遇到一个问题，解决如下

![](https://img-blog.csdn.net/20171008231338044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后就是更新一下，格式化文件系统操作了

![](https://img-blog.csdn.net/20171008233035638?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后在这个地址下执行Hadoop 的启动-（"/home/123456/hadoop-0.20.2/bin/"）启动命令：start-all.sh

![](https://img-blog.csdn.net/20171008233433510?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








![](https://img-blog.csdn.net/20171008233736681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


最后了，随便提一下；如果你的操作中出现了这种错误提示“没有那个文件或目录”。说明你的Hadoop 没有配置正确，所以你要耐心的嘻嘻查找问题的所在位置，改正过来就好了！

![](https://img-blog.csdn.net/20171008234116623?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





关闭防火墙和禁止开机启动



```java
systemctl stop firewalld.service #停止firewall

systemctl disable firewalld.service #禁止firewall开机启动
```









