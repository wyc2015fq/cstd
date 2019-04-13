
# 4.4 Hadoop集群搭建 - 尹成的技术博客 - CSDN博客

2018年11月15日 14:00:34[尹成](https://me.csdn.net/yincheng01)阅读数：31个人分类：[区块链](https://blog.csdn.net/yincheng01/article/category/7618299)



CentOS7下搭建hadoop2.7.3完全分布式
这里搭建的是3个节点的完全分布式，即1个nameNode，2个dataNode，分别如下：
CentOS-master   nameNode   192.168.11.128
CentOS-node1   dataNode    192.168.11.131
CentOS-node2   dataNode    192.168…11.132
1.首先创建好一个CentOS虚拟机，将它作为主节点我这里起名为CentOS-master，起什么都行，不固定要求
2.VMware中打开虚拟机，输入java -version，检查是否有JDK环境，不要用系统自带的openJDK版本，要自己安装的版本
3.输入 systemctl status firewalld.service ，若如图，防火墙处于running状态，则执行第4和第5步，否则直接进入第6步
4.输入 systemctl stop firewalld.service ，关闭防火墙
5.输入 systemctl disable firewalld.service ，禁用防火墙
关闭SELinux
修改/etc/selinux/config文件，SELINUX=enforcing改为SELINUX=disabled
6.输入 mkdir /usr/local/hadoop 创建一个hadoop的文件夹
7.将hadoop的tar包放到刚创建好的目录
8.进入hadoop目录，输入 tar -zxvf hadoop-2.7.3.tar.gz 解压tar包
9.输入 vi /etc/profile ，配置环境变量
10.加入如下内容，保存并退出
HADOOP_HOME=/usr/local/hadoop/hadoop-2.7.3/
PATH=$PATH:$JAVA_HOME/bin:$HADOOP_HOME/bin
11.输入 . /etc/profile ，使环境变量生效
12.任意目录输入 hado ，然后按Tab，如果自动补全为hadoop，则说明环境变量配的没问题，否则检查环境变量哪出错了
13.创建3个之后要用到的文件夹，分别如下：
mkdir /usr/local/hadoop/tmp
`mkdir -p /usr/local/hadoop/hdfs/name
mkdir /usr/local/hadoop/hdfs/data`14.进入hadoop解压后的 /etc/hadoop 目录，里面存放的是hadoop的配置文件，接下来要修改这里面一些配置文件
15.有2个.sh文件，需要指定一下JAVA的目录，首先输入 vi[hadoop-env.sh](http://hadoop-env.sh)修改配置文件
16.将原有的JAVA_HOME注释掉，根据自己的JDK安装位置，精确配置JAVA_HOME如下，保存并退出
export JAVA_HOME=/usr/local/java/jdk1.8.0_102/
17.输入 vi[yarn-env.sh](http://yarn-env.sh)修改配置文件
18.加入如下内容，指定JAVA_HOME，保存并退出
export JAVA_HOME=/usr/local/java/jdk1.8.0_102
19.输入 vi core-site.xml 修改配置文件
20.在configuration标签中，添加如下内容，保存并退出，注意这里配置的hdfs:master:9000是不能在浏览器访问的
[fs.default.name](http://fs.default.name)
hdfs://master:9000
指定HDFS的默认名称

fs.defaultFS
hdfs://master:9000
HDFS的URI

hadoop.tmp.dir
/usr/local/hadoop/tmp
节点上本地的hadoop临时文件夹

hadoop.proxyuser.root.hosts

hadoop.proxyuser.root.groups

21.输入 vi hdfs-site.xml 修改配置文件
22.在configuration标签中，添加如下内容，保存并退出
dfs.namenode.name.dir
file:/usr/local/hadoop/hdfs/name
namenode上存储hdfs名字空间元数据

dfs.datanode.data.dir
file:/usr/local/hadoop/hdfs/data
datanode上数据块的物理存储位置

dfs.replication
1
副本个数，默认是3,应小于datanode机器数量
23.输入 cp mapred-site.xml.template mapred-site.xml 将mapred-site.xml.template文件复制到当前目录，并重命名为mapred-site.xml
24.输入 vi mapred-site.xml 修改配置文件
25.在configuration标签中，添加如下内容，保存并退出
mapreduce.framework.name
yarn
指定mapreduce使用yarn框架
26.输入 vi yarn-site.xml 修改配置文件
27.在configuration标签中，添加如下内容，保存并退出
yarn.resourcemanager.hostname
master
指定resourcemanager所在的hostname

yarn.nodemanager.aux-services
mapreduce_shuffle
NodeManager上运行的附属服务。
需配置成mapreduce_shuffle，才可运行MapReduce程序

28.输入 vi slaves 修改配置文件
29.将localhost删掉，加入如下内容，即dataNode节点的主机名
node1
node2
30.将虚拟机关闭，再复制两份虚拟机，重命名为如下，注意这里一定要关闭虚拟机，再复制
31.将3台虚拟机都打开，后两台复制的虚拟机打开时，都选择“我已复制该虚拟机”
32.在master机器上，输入 vi /etc/hostname，将localhost改为master，保存并退出
33.在node1机器上，输入 vi /etc/hostname，将localhost改为node1，保存并退出
34.在node2机器上，输入 vi /etc/hostname，将localhost改为node2，保存并退出
35.在三台机器分别输入 vi /etc/hosts 修改文件，其作用是将一些常用的网址域名与其对应的IP地址建立一个关联，当用户在访问网址时，系统会首先自动从Hosts文件中寻找对应的IP地址
36.三个文件中都加入如下内容，保存并退出，注意这里要根据自己实际IP和节点主机名进行更改，IP和主机名中间要有一个空格
192.168.11.128 master
192.168.11.131 node1
192.168.11.132 node2
37.SSH的配置
在主节点，ssh-keygen -t rsa     生成密钥
在主节点，ssh-copy-id 主机名    复制密钥到每个子节点
45.在master节点上分别输入 ssh node1 和 ssh node2 测试是否配置ssh成功
46.如果node节点还没有hadoop，则master机器上分别输入如下命令将hadoop复制
scp -r /usr/local/hadoop/ root@node1:/usr/local/
scp -r /usr/local/hadoop/ root@node2:/usr/local/
47.在master机器上，任意目录输入 hdfs namenode -format 格式化namenode，第一次使用需格式化一次，之后就不用再格式化，如果改一些配置文件了，可能还需要再次格式化
48.格式化完成
49.在master机器上，进入hadoop的sbin目录，输入 ./start-all.sh 启动hadoop
50.输入yes，回车
51.输入 jps 查看当前java的进程，该命令是JDK1.5开始有的，作用是列出当前java进程的PID和Java主类名，nameNode节点除了JPS，还有3个进程，启动成功
52.在node1机器和node2机器上分别输入 jps 查看进程如下，说明配置成功
53.在浏览器访问nameNode节点的8088端口和50070端口可以查看hadoop的运行状况
学院Go语言视频主页
[https://edu.csdn.net/lecturer/1928](https://edu.csdn.net/lecturer/1928)
[清华团队带你实战区块链开发](https://ke.qq.com/course/344443?tuin=3d17195d)
扫码获取海量视频及源码   QQ群：721929980
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114143613461.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmNoZW5nMDE=,size_16,color_FFFFFF,t_70)

