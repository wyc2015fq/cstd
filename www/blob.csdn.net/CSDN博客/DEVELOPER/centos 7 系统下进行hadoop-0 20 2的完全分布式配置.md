# centos 7 系统下进行hadoop-0.20.2的完全分布式配置 - DEVELOPER - CSDN博客





2017年11月27日 15:32:28[学术袁](https://me.csdn.net/u012827205)阅读数：171
所属专栏：[Hadoop-学习](https://blog.csdn.net/column/details/20399.html)









记录一下我含泪史![哭](http://static.blog.csdn.net/xheditor/xheditor_emot/default/cry.gif),为什么叫含泪史呢？因为自学的过程中会遇到各种各样的问题，遇到问题-解决问题。利用了一个双休+今天，才真正的配好了hadoop。

其过程，可谓柔肠百结、心如吞石。配置成功后，看到自己的成果，挺-感-动！

首先说明的是，这一篇博客是依据上一篇*["centos 7 系统下进行多服务器的SSH免密码登录配置"](http://blog.csdn.net/junhuahouse/article/details/78623505)*继续的。

##### hadoop的完全分布式配置

###### 配置环境信息





|系统|hadoop版本号|jdk版本号|主机名|别名|用户|IP地址|功能|
|----|----|----|----|----|----|----|----|
|centos7|hadoop-0.20.2.tar.gz|java-1.8.0|h147|master|hadoop|192.168.127.147|namenode|
|centos7|hadoop-0.20.2.tar.gz|java-1.8.0|h148|slave1|hadoop|192.168.127.148|datanode|
|centos7|hadoop-0.20.2.tar.gz|java-1.8.0|h149|slave2|hadoop|192.168.127.149|datanode|

###### JDK配置

关于JDK的配置是根据这一篇*[“Hadoop基于Linux-CentOS7安装-安装Hadoop”](http://blog.csdn.net/junhuahouse/article/details/78177527)*来进行配置的。

展示一下此时已配置成功的hadoop集群下，jdk配置成功配置信息：



```java
# set jdk env
export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el7_4.x86_64
export CLASSPATH=.:$CLASSPATH:$JAVA_HOME/lib:$JAVA_HOME/jre/lib  
export PATH=$PATH:$JAVA_HOME/bin:$JAVA_HOME/jre/bin
```




![](https://img-blog.csdn.net/20171127155859806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**需要指出的是，在master、slave1、slave2 这三天机器上，这个jdk以及环境配置文件都是要写的。**

###### SSH配置

关于SSH的配置是根据这一篇“centos 7 系统下进行多服务器的SSH免密码登录配置”来进行配置的。


**这里需要提出的是，如果使用"ssh localhost"命令进行访问本地服务器的时候需要输入密码，这时候之需要修改一下文件权限为600：authorized_keys **




###### hadoop配置

好的，基于上面的配置信息我们看一下：

![](https://img-blog.csdn.net/20171127155109192?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**需要提出的是，图片中可以看到，我把“192.168.127.147 localhost”给注释了！**

**![](https://img-blog.csdn.net/20171127160331675?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)**

然后我也做了对主机名的修改，但是这种修改是否有助于hadoop的安装配置，我还没有进行测试。"修改主机名之后，重启网络使用命令：/etc/init.d/network restart"

三台服务器：master、slave1、slave2.他们的hosts文件都是相同的。


配置hadoop，首先使用winSCP把windows上已经下载的hadoop-0.20.2压缩包复制到centos7上。

![](https://img-blog.csdn.net/20171127161009032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





然后在centos7 中找到这个压缩包，进行解压-并修改文件夹的名称为hado：



```java
#解压命令 
tar zxvf hadoop-0.20.2.tar.gz /home/hadoop/
#修改文件名命令
mv -r hadoop-0.20.2 /home/hadoop/hado
```
然后配置环境文件-/etc/profile<这个文件，三台服务器：master、slave1、slave2.他们的都是相同的。>





```java
#set hadoop
export HADOOP_HOME=/home/hadoop/hadoop
export PATH=$HADOOP_HOME/bin:$PATH
```
![](https://img-blog.csdn.net/20171127155859806?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后就是比较重要的文件配置：/home/hadoop/hado/conf/*



![](https://img-blog.csdn.net/20171127161857502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**core-site.xml**



```java
<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>

<!-- Put site-specific property overrides in this file. -->

<configuration>
<property>
        <name>fs.default.name</name>
        <value>hdfs://192.168.127.147:9000</value>
    </property>
</configuration>
```
![](https://img-blog.csdn.net/20171127162324144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




**hadoop-env.xml**



```java
# The java implementation to use.  Required.
 export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-1.8.0.151-1.b12.el7_4.x86_64
```



![](https://img-blog.csdn.net/20171127162334768?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





**hdfs-site.xml**



```java
<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>

<!-- Put site-specific property overrides in this file. -->

<configuration>
<property>
        <name>dfs.data.dir</name>
        <value>/home/hadoop/dir/data</value>
    </property>
    <property>
        <name>dfs.name.dir</name>
        <value>/home/hadoop/dir/name</value>
    </property>
    <property>
        <name>dfs.replication</name>
        <value>2</value>
    </property>
</configuration>
```
![](https://img-blog.csdn.net/20171127162512922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**mapred-site.xml**

```java
<?xml version="1.0"?>
<?xml-stylesheet type="text/xsl" href="configuration.xsl"?>

<!-- Put site-specific property overrides in this file. -->

<configuration>
<property>
        <name>mapred.job.tracker</name>
        <value>192.168.127.147:9001</value>
    </property>
</configuration>
```
![](https://img-blog.csdn.net/20171127162633784?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**配置 masters和slaves**

![](https://img-blog.csdn.net/20171127162825113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





然后，把已经配置好的hadoop，传送给slave1和slave2。使用命令：



```java
#传给slave1
scp -r /home/hadoop/hado hadoop@slave1:/home/hadoop/

#传给slave2
scp -r /home/hadoop/hado hadoop@slave2:/home/hadoop/
```






好了到这里，对于配置上就全部完结了！接下来就是初始化，测试阶段了。



```java
#对 namenode 进行格式化
bin/hadoop namenode -format

#格式化成功之后，启动
bin/start-all.sh
```






如果你的所有配置都没有问题的话，分别执行这两句命令可以看到：

![](https://img-blog.csdn.net/20171127163556908?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


然后，还可以这样

在 /home/hadoop/hado/ 下建立一个目录 input。并在目录中input建立两个文件 test1.txt、test2.txt。



```java
#创建目录 input
mkdir /home/hadoop/hado/input

#在input 中建立两个文件
echo "hello mufafa , the world love you ." > test1.txt
echo "hello hadoop , the females love you ." > test1.txt
```
然后，把目录input中的文件上传到hdfs上<在目录 /home/hadoop/hado 下执行>：





```java
#上传文件
bin/hadoop dfs -put /home/hadoop/input in
```






进行 wordcount  处理：



```java
bin/hadoop jar /home/hadoop/hado/hadoop-0.20.2-examples.jar wordcount in out
```
最后，查看处理结果：





```java
bin/hadoop dfs -cat ./out/*
```
![](https://img-blog.csdn.net/20171127165112879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20171127165239900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




并在这里总结一下经常会使用到的命令：



```java
#创建群组
groupadd supergroup
#添加用户到群组
usermod -a -G supergroup hadoop

#启动防火墙
systemctl start  firewalld
#关闭防火墙
systemctl stop firewalld
#停止防火墙
systemctl disable firewalld
#查看防火墙的状态
systemctl status firewalld 
#或者 firewall-cmd --state

#查看namenode主机的9000的监听情况
netstat -nap | grep 9000

#查看磁盘空间
df –ah      

#格式化namenode
bin/hadoop namenode -format

#修改了host后，重启网络 
/etc/init.d/network restart
#查看主机名 
hostname 

#查看$之后的路径
echo $JAVA_HOME

bin/hadoop dfsadmin -report

#单独启动 namenode 或 datanode
bin/hadoop-daemon.sh start namenode 
bin/hadoop-daemon.sh start datanode
```





**然后简单总结一下，我在配置过程遇到的问题：**

1，遇到了 namenode 的安全模式异常，并附解决方案

![](https://img-blog.csdn.net/20171127171425520?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


2，需要关闭防火墙-所有机器都关掉（不关，要开放出一个端口），这个问题我觉得新手基本上逃不掉；这个问题的出现，比如http://192.168.127.147:50030/jobtracker.jsp，无法打开网页；

3，切记，namenode(master) 还有 所有的 datanode(slave)，在配置hadoop之前，对于 host 、 jdk、ssh等，配置一定要保持一致！

4，在进行配置 ./hadoop/conf/* 的 hdfs-site.xml 时，会让我们写 <name>dfs.data.dir</name> 和 <name>dfs.name.dir</name>
 。这时，我们不用自己创建他们所对应的目录，因为他们会自己创建。并且如果你自己创建的目录，如果权限不对，那么数据无法写入，又要报错了。还有就是我在配置时候并没有做任何的目录"tmp";

5，很多博客上写了，创建群组什么的，然后把这些用户都添加到同一个群组中，便于管理。其实，这个不是必须的！

6，还有就是执行了 start-all.sh 后，jps可以看到 Jps、SecondaryNameNode、JobTracker、NameNode。但是，经过几分钟亦或是几秒钟之后，再次执行jps发现NameNode没了！！然后又分别在其他节点执行jps发现，都没有了！

7，当执行命令比如bin/hadoop
 dfsadmin -report，发现一直在重试连接master，但就是链接不上；

8，当执行命令比如bin/hadoop
 dfsadmin -report，发现直接出现了异常，没有namenode或是datanode;

9，对于配置hadoop的conf/*下的文件时候，最好使用IP（如192.168.127.147）；masters和slaves也使用IP；






