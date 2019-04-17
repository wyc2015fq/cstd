# Hadoop三种安装模式：单机模式，伪分布式，真正分布式 - 战斗蜗牛的专栏 - CSDN博客





2015年07月04日 08:53:41[vbskj](https://me.csdn.net/vbskj)阅读数：8603







Hadoop三种安装模式：单机模式，伪分布式，真正分布式


一 单机模式standalone
单机模式是Hadoop的默认模式。当首次解压Hadoop的源码包时，Hadoop无法了解硬件安装环境，便保守地选择了最小配置。在这种默认模式下所有3个XML文件均为空。当配置文件为空时，Hadoop会完全运行在本地。因为不需要与其他节点交互，单机模式就不使用HDFS，也不加载任何Hadoop的守护进程。该模式主要用于开发调试MapReduce程序的应用逻辑。


二 伪分布模式安装
tar xzvf hadoop-0.20.2.tar.gz
Hadoop的配置文件：
conf/hadoop-env.sh  配置JAVA_HOME
core-site.xml  配置HDFS节点名称和地址
hdfs-site.xml  配置HDFS存储目录，复制数量
mapred-site.xml    配置mapreduce的jobtracker地址


配置ssh，生成密匙，使到ssh可以免密码连接
(RSA算法，基于因数不对称加密：公钥加密私钥才能解密，私钥加密公钥才能解密)
cd /root
ssh -keygen -t rsa
cd .ssh
cp id_rsa.pub authorized_keys覆盖公钥，就能免密码连接


启动Hadoop bin/start-all.sh
停止Hadoop bin/stop-all.sh




二 完全分布式模式
1. 配置etc/hosts文件,使主机名解析为IP  或者使用DNS服务解析主机名
2. 建立hadoop运行用户: useradd grid>passwd grid
3. 配置ssh密码连入: 每个节点用grid登录，进入主工作目录，ssh -keygen -t rsa生产公钥，然后将每个节点的公钥复制到同一个文件中，再将这个包含所有节点公钥的文件复制到每个节点authorized_keys目录，这个每个节点之间彼此可以免密码连接
4. 下载并解压hadoop安装包
5. 配置namenode，修改site文件
6. 配置hadoop-env.sh
7. 配置masters和slaves文件
8. 向各个节点复制hadoop
9. 格式化namenode
10.启动hadoop
11.用jps检验各后台进程是否成功启动






vim /etc/hosts
vim /etc/sysconfig/network
vim /etc/sysconfig/network-scripts/ifcfg-eth0 
service network restart 


service NetworkManager stop
chkconfig NetworkManager off
service iptables stop
chkconfig iptables off
service ip6tables stop 
chkconfig ip6tables off
vim /etc/sysconfig/selinuxenforce-->disabled
setenforce 0
getenforce 


useradd hadoop
passwd hadoop   


secureCRT生产公钥，将公钥拷贝到/home/hadoop/.ssh下：
chmod 700 .ssh
ssh-keygen -i -f PubKey_Master_Hadoop.pub >> authorized_key
chmod 600 authorized_keys
vim /etc/ssh/sshd-config如下：
RSAAuthentication yes # 启用
 RSA 认证
PubkeyAuthentication yes #
 启用公钥私钥配对认证方式
AuthorizedKeysFile .ssh/authorized_keys
 # 公钥文件路径（和上面生成的文件同）
PasswordAuthentication no #
 禁止密码验证登陆（根据需要，不过一般启用了密钥，就不用密码了）
service sshd restart
secureCRT使用PublicKey（PubKey_Master_Hadoop.pub）连接测试


Master 使用ssh公钥连接Slave：
挂载光盘，创建yum源：
vim /etc/fstab
vim /etc/yum.repos.d/rhel-source.repo
yum -y install ssh
yum -y install rsync


Master host:
mkdir .ssh
ssh-keygen –t rsa –P ''
cat id_rsa.pub >> authorized_keys
chmod 700 .ssh
chmod 600 authorized_keys
ssh localhost
scp id_rsa.pub hadoop@192.168.175.12:~/,ssh
ssh 192.168.175.12


Slave host:
mkdir .ssh
cat id_rsa.pub >> authorized_keys
chmod 700 .ssh
chmod 600 authorized_keys
vim /etc/ssh/sshd-config如上
service sshd restart


安装java：拷贝到/usr/hava
chmod +x jdk-6u37-linux-x64.bin
vim /etc/profile 添加JAVA_HOME
source /etc profile
=================================
安装hadoop-1.2.0：拷贝到/usr/hadoop-1.2.0，所以者和组修改为hadoop 
vim /etc/profile: 
export JAVA_HOME=/usr/java/jdk1.6.0_37
export HADOOP_HOME=/usr/hadoop-1.2.0
export PATH=$PATH:$HADOOP_HOME/bin:$HADOOP_HOME/sbin
source /etc/profile


配置hadoop-env.sh:目录：/usr/hadoop/conf
#export HADOOP_ROOT_LOGGER=DEBUG,console
export JAVA_HOME=/usr/java/jdk1.6.0_37
export HADOOP_HOME_WARN_SUPPRESS=1

配置core-site.xml文件：
         <property>
                <name>hadoop.tmp.dir</name>
                <value>/usr/hadoop-1.2.0/tmp</value>
                <description>A base for other temporary directories.</description>
        </property>
<!-- file system properties -->
        <property>
                <name>fs.default.name</name>
                <value>hdfs://192.168.175.11:9000</value>
        </property>


配置hdfs-site.xml文件：
         <property>
                <name>dfs.replication</name>
                <value>1</value>
        </property>
        <property>                 
                <name>dfs.datanode.max.xcievers</name>   //同时处理文件上限，hbase并发比较大，至少要4096
                <value>4096</value>
        </property>
        <property>
                <name>dfs.support.append</name>    //如不指定，在使用HDFS存储的时候，HBase可能会遗失数据
                <value>true</value>
        </property>


配置mapred-site.xml文件：
        <property>
                <name>mapred.job.tracker</name>
                <value>http://192.168.175.11:9001</value>
        </property>


配置masters文件：
master.hadoop或者192.168.175.11
配置slave文件：
slave1.hadoop或者192.168.175.12
slave2.hadoop或者192.168.175.13
slave3.hadoop或者192.168.175.14


=================================
安装hadoop-2.0.5：拷贝到/usr
tar -zxvf hadoop-2.0.5-alpha.tar.gz
mv hadoop-2.0.5-alpha /usr/hadoop
chown -R hadoop:hadoop hadoop
vim /etc/profile:
# set hadoop path
export HADOOP_HOME=/usr/hadoop
export PATH=$PATH:$HADOOP_HOME:$HADOOP_HOME/bin
source /etc/profile


配置hadoop-env.sh:目录：/usr/hadoop/etc/hadoop
在末尾添加：export JAVA_HOME=/usr/java/jdk1.6.0_37
配置yarn-env.sh和 .bash_profile：
export HADOOP_PREFIX=/usr/hadoop
export PATH=$PATH:$HADOOP_PREFIX/bin
export PATH=$PATH:$HADOOP_PREFIX/sbin
export HADOOP_MAPRED_HOME=${HADOOP_PREFIX}
export HADOOP_COMMON_HOME=${HADOOP_PREFIX}
export HADOOP_HDFS_HOME=${HADOOP_PREFIX}
export YARN_HOME=${HADOOP_PREFIX}
export HADOOP_CONF_DIR=${HADOOP_FREFIX}/etc/hadoop
export YARN_CONF_DIR=${HADOOP_FREFIX}/etc/hadoop


配置core-site.xml文件：
<configuration>
<property>
<name>hadoop.tmp.dir</name>
<value>/usr/hadoop/tmp</value>
        （备注：请先在 /usr/hadoop 目录下建立 tmp 文件夹）
<description>A base for other
 temporary directories.</description>
</property>
<!-- file system properties
 -->
<property>
<name>fs.default.name</name>
<value>hdfs://192.168.175.11:9000</value>
</property>
</configuration>
配置hdfs-site.xml文件：修改Hadoop中HDFS的配置，配置的备份方式默认为3。
<configuration>
<property>
<name>dfs.replication</name>
<value>1</value>        (备注：replication
 是数据副本数量，默认为3，salve少于3台就会报错)
</property>
        <property>
                <name>dfs.namenode.name.dir</name>
                <value>file:/home/hadoop/dfs/name</value> 
                <final>true</final>
        </property> 
        <property>
                <name>dfs.datanode.data.dir</name>
                <value>file:/home/hadoop/dfs/data</value> 
                <final>true</final>
        </property>
<configuration>
配置mapred-site.xml文件：修改Hadoop中MapReduce的配置文件，配置的是JobTracker的地址和端口。
<configuration>
<property>
<name>mapred.job.tracker</name>
<value>hdfs://192.168.175.11:9001</value>
</property>
<property>
                <name>mapreduce.framework.name</name>
                <value>yarn</value>
        </property>
        <property>
                <name>mapred.system.dir</name>
                <value>file:/home/hadoop/mapred/system</value>
                <final>true</final>
        </property>
        <property>
                <name>mapred.local.dir</name>
                <value>file:/home/hadoop/mapred/local</value>
                <final>true</final>
        </property>
        <property>
                <name>mapred.job.tracker</name>
                <value>hdfs://192.168.175.11:9001</value>
        </property>
</configuration>
配置yarn-site.xml：
<configuration> 
        <property>
                <name>yarn.resourcemanager.address</name>
                <value>192.168.175.11:8080</value>
        </property>
        <property>
                <name>yarn.resourcemanager.scheduler.address</name>
                <value>192.168.175.11:8081</value>
        </property>
        <property>
                <name>yarn.resourcemanager.resource-tracker.address</name>
                <value>192.168.175.11:8082</value>
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


配置masters文件：
master.hadoop或者192.168.175.11
配置slave文件：
slave1.hadoop或者192.168.175.12
slave2.hadoop或者192.168.175.13
slave3.hadoop或者192.168.175.14


mkdir -p /usr/hadoop/tmp
mkdir -p /home/hadoop/dfs/data
mkdir -p /home/hadoop/dfs/name
mkdir -p /home/hadoop/mapred/system
mkdir -p /home/hadoop/mapred/local


格式化HDFS文件系统：使用hadoop用户，只需格式一次：hadoop namenode -format
开始守护进程
# hadoop-daemon.sh start namenode
# hadoop-daemon.sh start datanode
可以同时启动：start-dfs.sh
开始 Yarn 守护进程
# yarn-daemon.sh start resourcemanager
# yarn-daemon.sh start nodemanager
或同时启动：start-yarn.sh
检查守护进程是否启动
# jps




将hadoop日志级别改为debug，并且在控制台输出：export HADOOP_ROOT_LOGGER=DEBUG,console



