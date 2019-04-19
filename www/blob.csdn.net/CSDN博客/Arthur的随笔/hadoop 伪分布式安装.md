# hadoop 伪分布式安装 - Arthur的随笔 - CSDN博客
2013年03月24日 16:09:41[largetalk](https://me.csdn.net/largetalk)阅读数：1581
这个也是个简单的步骤，备忘而已。
1. java的安装
[http://www.devsniper.com/ubuntu-12-04-install-sun-jdk-6-7/](http://www.devsniper.com/ubuntu-12-04-install-sun-jdk-6-7/)
Installing Sun JDK 6 on Ubuntu 12.04 / 12.10:
- Download the sun jdk 6 bin [from
 here](http://www.oracle.com/technetwork/java/javase/downloads/jdk6u38-downloads-1877406.html).
- Make the bin file executeable:
chmod +x jdk-6u32-linux-x64.bin- Extract the bin file:
./jdk-6u32-linux-x64.bin- Move extracted folder to this location:
sudo mv jdk1.6.0_38 /usr/lib/jvm/jdk1.6.0_38- Install new java source in system:
sudo update-alternatives --install /usr/bin/javac javac /usr/lib/jvm/jdk1.6.0_38/bin/javac 1
sudo update-alternatives --install /usr/bin/java java /usr/lib/jvm/jdk1.6.0_38/bin/java 1
sudo update-alternatives --install /usr/bin/javaws javaws /usr/lib/jvm/jdk1.6.0_38/bin/javaws 1- Choose default java:
sudo update-alternatives --config javac
sudo update-alternatives --config java
sudo update-alternatives --config javaws- java version test:
java -version- Verify the symlinks all point to the new java location:
ls -la /etc/alternatives/java*- Enable Java plugin for Mozilla Firefox (even for Chrome)
#for 64-Bit jdk
sudo ln -s /usr/lib/jvm/jdk1.6.0_38/jre/lib/amd64/libnpjp2.so /usr/lib/mozilla/plugins
#for 32-Bit jdk
sudo ln -s /usr/lib/jvm/jdk1.6.0_38/jre/lib/i386/libnpjp2.so /usr/lib/mozilla/plugins
2. 下载hadoop
[http://www.apache.org/dyn/closer.cgi/hadoop/common/](http://www.apache.org/dyn/closer.cgi/hadoop/common/)
现在hadoop版本都2.0.3了，不过是alpha版本，我无所谓，就用最新版本了
tar xvf hadoop-2.0.3-alpha.tar.gz
3. 无密码登录本机
ssh localhost
如果可以则行，不可以则执行
$ ssh-keygen -t dsa -P '' -f ~/.ssh/id_dsa
$ cat ~/.ssh/id_dsa.pub >> ~/.ssh/authorized_keys
4. 配置JAVA_HOME
vim /etc/profile
在最后添加这几行
export JAVA_HOME=/usr/lib/jvm/jdk1.6.0_38
export CLASSPATH=.:$CLASSPATH:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
export PATH=$PATH:$JAVA_HOME/bin:$JAVA_HOME/jre/bin
然后source /etc/profile
5. 配置hadoop环境变量
sudo mkdir -p /usr/local/hadoop
sudo mv hadoop-2.0.3-alpha /usr/local/hadoop
sudo vim /etc/profile 并且添加：
export HADOOP_HOME=/usr/local/hadoop/hadoop-2.0.3-alpha
export PATH=$HADOOP_HOME/bin:$PATH
sudo vim /usr/local/hadoop/hadoop-2.0.3-alpha/etc/hadoop/hadoop-env.sh 并添加：
export JAVA_HOME=/usr/lib/jvm/jdk1.6.0_38
6. 配置master和slave
hadoop-2.0.3-alpha/etc/hadoop/masters 和 hadoop-2.0.3-alpha/etc/hadoop/slaves
 都加入一行
localhost
7.
 配置xml文件
etc/hadoop/core-site.xml:
<configuration>
     <property>
         <name>fs.default.name</name>
         <value>hdfs://localhost:9000</value>
     </property>
</configuration>
etc/hadoop/hdfs-site.xml:
<configuration>
     <property>
         <name>dfs.replication</name>
         <value>1</value>
     </property>
</configuration>
etc/hadoop/mapred-site.xml:
<configuration>
     <property>
         <name>mapred.job.tracker</name>
         <value>localhost:9001</value>
     </property>
</configuration>
8.
 启动
Format a new distributed-filesystem:
$ bin/hadoop namenode -format
Start the hadoop daemons:
$ sbin/start-all.sh
不过看输出信息， start-all.sh方式不被推荐了，要用start-dfs.sh 和start-yarn.sh, 这些还没搞懂，很久没用了，以前用的时候hadoop版本还是0.20呢
The hadoop daemon log output is written to the ${HADOOP_LOG_DIR} directory (defaults to ${HADOOP_HOME}/logs).
Browse the web interface for the NameNode and the JobTracker; by default they are available at:
- NameNode - [http://localhost:50070/](http://localhost:50070/)
- JobTracker - [http://localhost:50030/](http://localhost:50030/)
执行jps可查看java进程
9. 试用
Copy the input files into the distributed filesystem:
$ bin/hadoop fs -put conf input
Run some of the examples provided:
$ bin/hadoop jar hadoop-examples-*.jar grep input output 'dfs[a-z.]+'
Examine the output files:
Copy the output files from the distributed filesystem to the local filesytem and examine them:
$ bin/hadoop fs -get output output
$ cat output/*
or
View the output files on the distributed filesystem:
$ bin/hadoop fs -cat output/*
When you're done, stop the daemons with:
$ sbin/stop-all.sh
今天先到这，以后再写点
#############################################
ps. 安装完之后发现 JobTracker - [http://localhost:50030/](http://localhost:50030/) 没有起起来，没有map/reduce了。yarn是第二代MR，据说速度提升不少。
 不过使用上应该差距不大，可以看看这个：[http://slaytanic.blog.51cto.com/2057708/885198](http://slaytanic.blog.51cto.com/2057708/885198) 这个：[http://blog.sina.com.cn/s/blog_4a1f59bf010116rh.html](http://blog.sina.com.cn/s/blog_4a1f59bf010116rh.html)
节选：
```
./hadoop namenode -format
#先格式化
cd ../sbin/
#进入sbin目录，这里放的都是server启动脚本
./hadoop-daemon.sh start namenode
./hadoop-daemon.sh start datanode
./hadoop-daemon.sh start secondarynamenode
#备份服起不起都无所谓，不影响使用，不过可以用来试试HA功能
#下面较重要，2.0取消了jobtracker和tasktracker，以YARN来代替，所以如果运行start jobtracker一类的，会报错。
#且hadoop,hdfs,map/reduce功能都分离出了单独脚本，所以不能用hadoop-daemon.sh启动所有了。
./yarn-daemon.sh start resourcemanager
#这个就相当于原来的jobtracker，用作运算资源分配的进程，跟namenode可放在一起。
./yarn-daemon.sh start nodemanager
#这个相当于原来的tasktracker，每台datanode或者叫slave的服务器上都要启动。
```
```
./hadoop jar /opt/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.0.0-alpha.jar -mapper /opt/hadoop/mrs/map.py -reducer /opt/hadoop/mrs/red.py -input /tmp/asf -output /asf
或者
./yarn jar /opt/hadoop/share/hadoop/tools/lib/hadoop-streaming-2.0.0-alpha.jar -mapper /opt/hadoop/mrs/map.py -reducer /opt/hadoop/mrs/red.py -input /tmp/asf -output /asf
```
**appendix 1**: 配置yarn
vim etc/hadoop/yarn-site.xml
```
<configuration>
  <property>
    <description>The address of the applications manager interface in the RM.</description>
    <name>yarn.resourcemanager.address</name>
      <value>localhost:18040</value>
  </property>
  <property>
    <description>The address of the scheduler interface.</description>
    <name>yarn.resourcemanager.scheduler.address</name>
      <value>localhost:18030</value>
  </property>
  <property>
    <description>The address of the RM web application.</description>
    <name>yarn.resourcemanager.webapp.address</name>
      <value>0.0.0.0:18088</value>
  </property>
  <property>
    <name>yarn.resourcemanager.resource-tracker.address</name>
    <value>localhost:18025</value>
  </property>
  <property>
    <description>The address of the RM admin interface.</description>
    <name>yarn.resourcemanager.admin.address</name>
       <value>localhost:18141</value>
  </property>
 <property>
    <name>yarn.nodemanager.aux-services</name>
    <value>mapreduce.shuffle</value>
</property> 
  <property>
    <name>yarn.nodemanager.aux-services.mapreduce.shuffle.class</name>
    <value>org.apache.hadoop.mapred.ShuffleHandler</value>
  </property>
</configuration>
```
mapred-site.xml改为：
```
<configuration>
  <property>
    <name>mapreduce.framework.name</name>
    <value>yarn</value>
  </property>
</configuration>
```
然后
 sbin/stop-yarn.sh
sbin/start-yarn.sh
现在浏览 [http://localhost:18088/](http://localhost:50030/)
上面的配置全都用localhost，yarn的配置可能有点不正确，不过我们现在是单机伪分布式，也就没关系了
附yarn的安装： [http://dongxicheng.org/mapreduce-nextgen/hadoop-yarn-install/](http://dongxicheng.org/mapreduce-nextgen/hadoop-yarn-install/)
