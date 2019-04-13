
# ubuntu16.04LTS下Hadoop2.7.3完全分布式搭建 - lpty的博客 - CSDN博客

2017年01月14日 15:53:31[lpty](https://me.csdn.net/sinat_33741547)阅读数：2031


最近在看Hadoop相关的资料，在这里做一些整理，下面是Hadoop环境搭建的详细步骤。
一、工具准备
1、ubuntu16.04LTS（[安装方式](http://blog.csdn.net/sinat_33741547/article/details/54412206)）
2、jdk1.8._111([下载地址](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html))
3、Hadoop2.7.3（[下载地址](http://hadoop.apache.org/releases.html)）
二、环境搭建
注：为了搭建方便，我都是使用root账户，如何获取root账户可看我的另一篇博客
1、首先更新一下apt，防止有些软件更新失败
apt-get update
2、安装java环境
(1)解压jdk文件到/usr/local/java目录下
(2)配置环境变量
gedit .bashrc
添加以下内容并保存
export JAVA_HOME=/usr/local/java/jdk1.8.0_111
export PATH=$JAVA_HOME/bin:$PATH
(3)使环境变量生效
source .bashrc
(4)可以用以下命令查看是否配置成功
echo $PATH
(5)查看java版本
java -version
3、安装Hadoop
(1)解压Hadoop文件到/usr/local/hadoop目录下
(2)配置环境变量
gedit .bashrc
添加以下内容并保存
export HADOOP_HOME=/usr/local/hadoop/hadoop-2.7.3
export PATH=$PATH:HADOOP_HOME/bin
(3)使环境变量生效
source .bashrc
(4)可以用以下命令查看是否配置成功
echo $PATH
(5)新建以下目录
/usr/local/hadoop/hdfs
/usr/local/hadoop/hdfs/data
/usr/local/hadoop/hdfs/name
/usr/local/hadoop/temp
(6)配置Hadoop配置文件（/usr/local/hadoop/hadoop-2.7.3/etc/hadoop）
这里有以下文件需要修改，这里贴出我的配置，详细可看官方文档
core-site.xml

```python
<configuration>
 <property>
    <name>fs.default.name</name>
    <value>hdfs://master:9000</value>
    <description>HDFS的URI，文件系统://namenode标识:端口号</description>
</property>
<property>
    <name>hadoop.tmp.dir</name>
    <value>file:/usr/local/soft/hadoop/tmp</value>
    <description>namenode上本地的hadoop临时文件夹</description>
</property>
</configuration>
```
hdfs-site.xml
```python
<configuration>
<property>
    <name>dfs.name.dir</name>
    <value>file:/usr/local/soft/hadoop/hdfs/name</value>
    <description>namenode上存储hdfs名字空间元数据 </description> 
</property>
<property>
    <name>dfs.data.dir</name>
    <value>file:/usr/local/soft/hadoop/hdfs/data</value>
    <description>datanode上数据块的物理存储位置</description>
</property>
<property>
    <name>dfs.replication</name>
    <value>1</value>
    <description>副本个数，配置默认是3,应小于datanode机器数量</description>
</property>
<property>
        <name>dfs.namenode.secondary.http-address</name>  
        <value>master:9001</value> 
</property>
</configuration>
```
mappred-site.xml

```python
<configuration>
    <property>
        <name>mapreduce.framework.name</name>
        <value>yarn</value>
    </property>
    <property>  
       <name>mapreduce.jobhistory.address</name>  
        <value>master:10020</value>  
    </property>  
    <property>  
       <name>mapreduce.jobhistory.webapp.address</name>  
        <value>master:19888</value>  
    </property> 
</configuration>
```
yarn-site.xml

```python
<configuration>
    <property>  
       <name>yarn.nodemanager.aux-services</name>  
       <value>mapreduce_shuffle</value>  
    </property>  
    <property>                                                                 
       <name>yarn.nodemanager.aux-services.mapreduce.shuffle.class</name>  
       <value>org.apache.hadoop.mapred.ShuffleHandler</value>  
    </property>  
    <property>  
       <name>yarn.resourcemanager.address</name>  
        <value>master:8032</value>  
    </property>  
    <property>  
        <name>yarn.resourcemanager.scheduler.address</name>  
        <value>master:8030</value>  
    </property>  
    <property>  
       <name>yarn.resourcemanager.resource-tracker.address</name>  
        <value>master:8031</value>  
    </property>  
    <property>  
       <name>yarn.resourcemanager.admin.address</name>  
        <value>master:8033</value>  
    </property>  
    <property>  
       <name>yarn.resourcemanager.webapp.address</name>  
        <value>master:8088</value>  
    </property>  
</configuration>
```
之后把hadoop-env.sh，mapred-env.sh，yarn-env.sh中的JAVA_HOME都修改为我们配置好的java路径
slaves文件
这个文件是配置slave节点信息的，这里添加slave1进去即可，如有多台计算机，序号递增。
因为我只有两台计算机所以我也将master节点增加到slave中，不清楚的可看完后边文档再回头修改。
4、配置Hadoop节点信息
我的集群中只有两个节点，多节点配置方式相同，后面会说明
master192.168.1.102
slave1192.168.1.107
(1)修改本机hostname
gedit /etc/hostname
分别修改主节点为master，从节点为slave1，如有多台计算机则序号递增即可，重启
(2)修改本机DNS服务信息
gedit /etc/hosts
添加以下内容，如有多台计算机则依样添加即可
192.168.1.102master
192.168.1.107slave1
5、配置ssh无密码登录
(1)ubuntu自带ssh-client，我们还需要ssh-server
apt-get install openssh-server
(2)启动ssh服务
/etc/init.d/ssh start
(3)查看sshd是否启动
ps -e | grep ssh
(4)root账户默认不允许登录ssh，修改权限
gedit /etc/ssh/sshd_config
找到Authentication，修改PermitRootLogin yes，保存
/etc/init.d/ssh restart
(5)ssh登录本地
ssh localhost
SSH首次登录会有提示，直接输入yes即可，这时是需要密码的
(6)生成秘钥
ssh-keygen -t rsa
之后一直按Enter键，默认将秘钥保存在.shh/id_rsa文件中
(7)RSA公钥加入授权文件
cd .ssh
cp id_rsa.pub authorized_keys
(8)重新登录，实现免密码登录localhost
6、配置master免密码登录slave1
这里使用scp命令，可自行搜索相关信息，配置确保slave1节点已安装ssh-server
(1)将master根目录下密码复制到slave1的根目录下
root@master:~\# scp ~/.ssh/id_rsa.pub root@slave1:~/.ssh
(2)在slave1节点将RSA公钥加入授权文件
root@slave:~\# cp .ssh/id_rsa.pub authorized_keys
如有多台计算机，重复以上操作即可
(3)在master节点上ssh登录slave1,实现免密码登录
ssh slave1
7、配置slave1节点的hadoop，同master节点，复制过去即可
8、Hadoop运行
(1)格式化分布式文件系统，在master节点下
cd /usr/local/hadoop/hadoop-2.7.3
bin/hadoop namenode -format
(2)启动hadoop守护进程
sbin/start-all.sh
(3)检测启动情况
jps
这时在master节点可以看到NameNode，SecondaryNameNode，ResourceManager
在slave1节点可以看到DataNode，NodeManager，因为我也将master节地点添加为slaves，所以也能看到类似信息
(4)停止hadoop进程
sbin/stop-all.sh
到这里我们已经配置好了hadoop完全分布式环境，有兴趣可查看我的另一篇博客，用[python实现MapReduce实例](http://blog.csdn.net/sinat_33741547/article/details/54428025)。




