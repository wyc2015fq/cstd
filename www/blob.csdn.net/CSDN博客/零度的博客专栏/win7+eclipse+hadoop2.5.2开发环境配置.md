# win7+eclipse+hadoop2.5.2开发环境配置 - 零度的博客专栏 - CSDN博客
2016年10月09日 15:02:00[零度anngle](https://me.csdn.net/zmx729618)阅读数：407标签：[hadoop																[eclipse																[win7](https://so.csdn.net/so/search/s.do?q=win7&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)
个人分类：[Hadoop](https://blog.csdn.net/zmx729618/article/category/6452458)
**一.hadoop集群环境配置 **
参考前一篇文章（hadoop2.5.2分布式环境配置 [http://www.cnblogs.com/huligong1234/p/4136331.html](http://www.cnblogs.com/huligong1234/p/4136331.html)）
但本人配置时还修改了如下内容(由于你的环境和我的可能不一致，可以在后面出现相关问题后再进行修改)： 
a.在master节点上(ubuntu-V01)修改hdfs-site.xml加上以下内容 
<property> 
<name>dfs.permissions</name> 
<value>false</value> 
</property> 
旨在取消权限检查，原因是为了解决我在windows机器上配置eclipse连接hadoop服务器时，配置map/reduce连接后报以下错误，org.apache.hadoop.security.AccessControlException: Permission denied: 
b.同样在master节点上(ubuntu-V01)修改hdfs-site.xml加上以下内容 
<property> 
<name>dfs.web.ugi</name> 
<value>jack,supergroup</value> 
</property> 
原因是运行时，报如下错误 WARN org.apache.hadoop.security.ShellBasedUnixGroupsMapping: got exception trying to get groups for user jack 
应该是我的windows的用户名为jack，无访问权限 
更多权限配置可参看官方说明文档： 
HDFS权限管理用户指南[http://hadoop.apache.org/docs/r1.0.4/cn/hdfs_permissions_guide.html](http://hadoop.apache.org/docs/r1.0.4/cn/hdfs_permissions_guide.html)
配置修改完后重启hadoop集群： 
[hadoop@ubuntu-V01](mailto:hadoop@ubuntu-V01):~/data$./sbin/stop-dfs.sh 
[hadoop@ubuntu-V01](mailto:hadoop@ubuntu-V01):~/data$./sbin/stop-yarn.sh 
[hadoop@ubuntu-V01](mailto:hadoop@ubuntu-V01):~/data$./sbin/start-dfs.sh 
[hadoop@ubuntu-V01](mailto:hadoop@ubuntu-V01):~/data$./sbin/start-yarn.sh 
**二.windows基础环境准备**
windows7(x64),jdk,ant,eclipse,hadoop 
1.jdk环境配置 
jdk-6u26-windows-i586.exe安装后好后配置相关JAVA_HOME环境变量,并将bin目录配置到path 
2.eclipse环境配置 
eclipse-standard-luna-SR1-win32.zip解压到D:\eclipse\目录下并命名eclipse-hadoop 
下载地址：[http://developer.eclipsesource.com/technology/epp/luna/eclipse-standard-luna-SR1-win32.zip](http://developer.eclipsesource.com/technology/epp/luna/eclipse-standard-luna-SR1-win32.zip)
3.ant环境配置 
apache-ant-1.9.4-bin.zip解压到D:\apache\目录下，配置环境变量ANT_HOME,并将bin目录配置到path 
下载地址：[http://mirror.bit.edu.cn/apache//ant/binaries/apache-ant-1.9.4-bin.zip](http://mirror.bit.edu.cn/apache//ant/binaries/apache-ant-1.9.4-bin.zip)
4.下载hadoop-2.5.2.tar.gz 
[http://mirror.bit.edu.cn/apache/hadoop/common/hadoop-2.5.2/hadoop-2.5.2.tar.gz](http://mirror.bit.edu.cn/apache/hadoop/common/hadoop-2.5.2/hadoop-2.5.2.tar.gz)
5.下载hadoop-2.5.2-src.tar.gz 
[http://mirror.bit.edu.cn/apache/hadoop/common/hadoop-2.5.2/hadoop-2.5.2-src.tar.gz](http://mirror.bit.edu.cn/apache/hadoop/common/hadoop-2.5.2/hadoop-2.5.2-src.tar.gz)
6.下载hadoop2x-eclipse-plugin 
[https://github.com/winghc/hadoop2x-eclipse-plugin](https://github.com/winghc/hadoop2x-eclipse-plugin)
7.下载hadoop-common-2.2.0-bin 
[https://github.com/srccodes/hadoop-common-2.2.0-bin](https://github.com/srccodes/hadoop-common-2.2.0-bin)
分别将hadoop-2.5.2.tar.gz、hadoop-2.5.2-src.tar.gz、hadoop2x-eclipse-plugin、hadoop-common-2.2.0-bin下载解压到F:\hadoop\目录下 
8.修改本地hosts文件，加入如下内容：
192.168.1.112 ubuntu-V01
**三、编译hadoop-eclipse-plugin-2.5.2.jar配置 **
1.添加环境变量HADOOP_HOME=F:\hadoop\hadoop-2.5.2\ 
追加环境变量path内容：%HADOOP_HOME%/bin 
2.修改编译包及依赖包版本信息 
修改F:\hadoop\hadoop2x-eclipse-plugin-master\ivy\libraries.properties 
hadoop.version=2.5.2 
jackson.version=1.9.13 
3.ant编译 
F:\hadoop\hadoop2x-eclipse-plugin-master\src\contrib\eclipse-plugin> 
ant jar -Dversion=2.5.2 -Declipse.home=D:\eclipse\eclipse-hadoop\eclipse -Dhadoop.home=F:\hadoop\hadoop-2.5.2 
编译好后hadoop-eclipse-plugin-2.5.2.jar会在F:\hadoop\hadoop2x-eclipse-plugin-master\build\contrib\eclipse-plugin目录下 
**四、eclipse环境配置**
1.将编译好的hadoop-eclipse-plugin-2.5.2.jar拷贝至eclipse的plugins目录下，然后重启eclipse 
2.打开菜单Window--Preference--Hadoop Map/Reduce进行配置，如下图所示： 
![](http://images.cnitblog.com/blog/311379/201412/021235230611994.png)
3.显示Hadoop连接配置窗口：Window--Show View--Other-MapReduce Tools,如下图所示： 
![](http://images.cnitblog.com/blog/311379/201412/021235418894008.png)
4.配置连接Hadoop，如下图所示： 
![](http://images.cnitblog.com/blog/311379/201412/021236022335993.png)
查看是否连接成功，能看到如下信息，则表示连接成功： 
![](http://images.cnitblog.com/blog/311379/201412/021236150301284.png)
**五、hadoop集群环境添加测试文件**
(如果已有则无需配置) 
a.dfs上创建input目录 
[hadoop@ubuntu-V01](mailto:hadoop@ubuntu-V01):~/data/hadoop-2.5.2$bin/hadoop fs -mkdir -p input 
b.把hadoop目录下的README.txt拷贝到dfs新建的input里 
[hadoop@ubuntu-V01](mailto:hadoop@ubuntu-V01):~/data/hadoop-2.5.2$bin/hadoop fs -copyFromLocal README.txt input 
**六、创建一个Map/Reduce Project **
1.新建项目 File--New--Other--Map/Reduce Project 命名为MR1, 
然后创建类org.apache.hadoop.examples.WordCount，从hadoop-2.5.2-src中拷贝覆盖 
（F:\hadoop\hadoop-2.5.2-src\hadoop-mapreduce-project\hadoop-mapreduce-examples\src\main\java\org\apache\hadoop\examples\WordCount.java） 
2.创建log4j.properties文件 
在src目录下创建log4j.properties文件，内容如下： 
log4j.rootLogger=debug,stdout,R 
log4j.appender.stdout=org.apache.log4j.ConsoleAppender 
log4j.appender.stdout.layout=org.apache.log4j.PatternLayout 
log4j.appender.stdout.layout.ConversionPattern=%5p - %m%n 
log4j.appender.R=org.apache.log4j.RollingFileAppender 
log4j.appender.R.File=mapreduce_test.log 
log4j.appender.R.MaxFileSize=1MB 
log4j.appender.R.MaxBackupIndex=1 
log4j.appender.R.layout=org.apache.log4j.PatternLayout 
log4j.appender.R.layout.ConversionPattern=%p %t %c - %m%n 
log4j.logger.com.codefutures=DEBUG 
3.解决java.lang.UnsatisfiedLinkError: org.apache.hadoop.io.nativeio.NativeIO$Windows.access0(Ljava/lang/String;I)异常问题 
(由于你的环境和我的可能不一致，可以在后面出现相关问题后再进行修改) 
拷贝源码文件org.apache.hadoop.io.nativeio.NativeIO到项目中 
然后定位到570行，直接修改为return true; 
如下图所示： 
![](http://images.cnitblog.com/blog/311379/201412/021236424985792.png)
**七、windows下运行环境配置**
(如果不生效，则需要重启机器) 
需要hadoop.dll,winutils.exe 
我是直接拷贝F:\hadoop\hadoop-common-2.2.0-bin-master\bin目录下内容覆盖F:\hadoop\hadoop-2.5.2\bin 
**八、运行project**
在eclipse中点击WordCount.java，右键，点击Run As—>Run Configurations，配置运行参数，即输入和输出文件夹 
hdfs://ubuntu-V01:9000/user/hadoop/input hdfs://ubuntu-V01:9000/user/hadoop/output 
如下图所示： 
![](http://images.cnitblog.com/blog/311379/201412/021236562013939.png)
注意：如果output目录已经存在，则删掉或换个名字，如output01，output02 。。。 
另外，出现问题可以多看日志([http://ubuntu-v01](http://ubuntu-v01/):8088/logs/) 
