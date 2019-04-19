# hadoop2.5.2分布式环境配置(无HA和联邦) - 零度的博客专栏 - CSDN博客
2016年10月09日 14:55:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：377标签：[hadoop																[mapreduce](https://so.csdn.net/so/search/s.do?q=mapreduce&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)
个人分类：[Hadoop](https://blog.csdn.net/zmx729618/article/category/6452458)
**一、基础环境准备**
系统：(VirtualBox) ubuntu-12.04.2-desktop-i386.iso
hadoop版本：hadoop-2.5.2
jdk版本：jdk-6u26-linux-i586.bin
1.三台测试集群，一个master(ubuntu-V01),两个slave(ubuntu-V02,ubuntu-V03)
/etc/hosts
192.168.1.112 ubuntu-V01
192.168.1.113 ubuntu-V02
192.168.1.114 ubuntu-V03
注意不要保留127.0.0.1 localhost
配置同步到其他两台机器
scp /etc/hosts root@192.168.1.113:/etc/hosts
scp /etc/hosts root@192.168.1.114:/etc/hosts
2. 设置linux上ssh是用户可以自动登录 
$ ssh-keygen -t dsa -P '' -f ~/.ssh/id_dsa 
$ cat ~/.ssh/id_dsa.pub >> ~/.ssh/authorized_keys
3.java环境配置
略，*当前已配好，JAVA_HOME为/usr/lib/jvm/jdk1.6.0_26*
**二、下载解压hadoop-2.5.2.tar.gz**
hadoop@ubuntu-V01:~/data$ pwd
/home/hadoop/data
hadoop@ubuntu-V01:~/data$ wget http://mirror.bit.edu.cn/apache/hadoop/common/hadoop-2.5.2/hadoop-2.5.2.tar.gz
hadoop@ubuntu-V01:~/data$tar zxvf hadoop-2.5.2.tar.gz
**三、配置环境变量**
hadoop@ubuntu-V01:~/data$gedit /etc/profile 
追加内容如下：
#HADOOP VARIABLES START
export HADOOP_INSTALL=/home/hadoop/data/hadoop-2.5.2
export PATH=$PATH:$HADOOP_INSTALL/bin
export PATH=$PATH:$HADOOP_INSTALL/sbin
export HADOOP_MAPRED_HOME=$HADOOP_INSTALL
export HADOOP_COMMON_HOME=$HADOOP_INSTALL
export HADOOP_HDFS_HOME=$HADOOP_INSTALL
export YARN_HOME=$HADOOP_INSTALL
export HADOOP_COMMON_LIB_NATIVE_DIR=$HADOOP_INSTALL/lib/native
export HADOOP_OPTS="-Djava.library.path=$HADOOP_INSTALL/lib"
#HADOOP VARIABLES END
使配置生效
hadoop@ubuntu-V01:~/data$source /etc/profile
同时需要修改**$HADOOP_HOME/etc/hadoop/hadoop-env.sh**
export JAVA_HOME=/usr/lib/jvm/jdk1.6.0_26
**四、修改$HADOOP_HOME/etc/hadoop/core-site.xml**
添加如下内容：
<property> 
<name>fs.default.name</name> 
<value>hdfs://ubuntu-V01:9000</value> 
</property> 
<property> 
<name>hadoop.tmp.dir</name> 
<value>/home/hadoop/data/hadoop-2.5.2/hadoop-${user.name}</value> 
</property> 
**五、修改$HADOOP_HOME/etc/hadoop/yarn-site.xml**
添加如下内容：
<property>
<name>yarn.nodemanager.aux-services</name>
<value>mapreduce_shuffle</value>
</property>
<property>
<name>yarn.nodemanager.aux-services.mapreduce_shuffle.class</name>
<value>org.apache.hadoop.mapred.ShuffleHandler</value>
</property>
<property>
<name>yarn.resourcemanager.hostname</name>
<value>ubuntu-V01</value>
</property>
更多yarn-site.xml参数配置可参考：
http://hadoop.apache.org/docs/r2.5.2/hadoop-yarn/hadoop-yarn-common/yarn-default.xml
**六、修改$HADOOP_HOME/etc/hadoop/mapred-site.xml**
默认没有mapred-site.xml文件，copy  mapred-site.xml.template 一份为 mapred-site.xml即可
#cp etc/hadoop/mapred-site.xml.template ./etc/hadoop/mapred-site.xml
添加如下内容：
<property>
<name>mapreduce.framework.name</name>
<value>yarn</value>
<final>true</final>
</property>
**七、配置hdfs-site.xml (这里可以不配，采用默认参数)**
    /usr/local/hadoop/etc/hadoop/hdfs-site.xml
用来配置集群中每台主机都可用，指定主机上作为namenode和datanode的目录。
<property>
<name>dfs.name.dir</name>
<value>/home/hadoop/data/hadoop-2.5.2/name1,/home/hadoop/data/hadoop-2.5.2/name2</value>
</property>
<property>
<name>dfs.data.dir</name>
<value>/home/hadoop/data/hadoop-2.5.2/data1,/home/hadoop/data/hadoop-2.5.2/data2</value>
</property>
<property> 
<name>dfs.replication</name> 
<value>2</value> 
</property>
**八、配置salves**
告诉hadoop 其他从节点，这样，只要主节点启动，他会自动启动其他机器上的nameNode dataNode 等等
编辑  $HADOOP_HOME/etc/hadoop/slaves
内容如下：
ubuntu-V02
ubuntu-V03
**九、同步同步该文件夹 到其他各个从主机上即可**
因为我们使用ssh免登陆 不需要使用密码
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$scp -r /home/hadoop/data/hadoop-2.5.2 hadoop@192.168.1.113:/home/hadoop/data/hadoop-2.5.2
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$scp -r /home/hadoop/data/hadoop-2.5.2 hadoop@192.168.1.114:/home/hadoop/data/hadoop-2.5.2
**十、格式化hdfs**
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$./bin/hdfs namenode -format
**十一、启动hadoop集群**
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$./sbin/start-dfs.sh
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$./sbin/start-yarn.sh   
**十二、浏览器查看**
浏览器打开 http://ubuntu-V01:50070/，会看到hdfs管理页面
浏览器打开 http://ubuntu-V01:8088/，会看到hadoop进程管理页面
浏览器打开 http://ubuntu-v01:8088/cluster 查看cluster情况
**十三、验证（WordCount验证）**
1.dfs上创建input目录
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$bin/hadoop fs -mkdir -p input
2.把hadoop目录下的README.txt拷贝到dfs新建的input里
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$bin/hadoop fs -copyFromLocal README.txt input
3.运行WordCount
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$bin/hadoop jar share/hadoop/mapreduce/sources/hadoop-mapreduce-examples-2.5.2-sources.jar org.apache.hadoop.examples.WordCount input output
4.运行完毕后，查看单词统计结果
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$bin/hadoop fs -cat output/*
假如程序的输出路径为output，如果该文件夹已经存在,先删除
hadoop@ubuntu-V01:~/data/hadoop-2.5.2$bin/hadoop dfs -rmr output
