# hadoop2.4.1完全分布式搭建 - 蓁蓁尔的博客 - CSDN博客





2016年03月26日 08:38:58[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：443







**1.准备Linux环境**
1.0点击VMware快捷方式，右键打开文件所在位置 -> 双击vmnetcfg.exe -> VMnet1 host-only ->修改subnet ip 设置网段：192.168.1.0 子网掩码：255.255.255.0 -> apply -> ok
回到windows --> 打开网络和共享中心 -> 更改适配器设置 -> 右键VMnet1 -> 属性 -> 双击IPv4 -> 设置windows的IP：192.168.1.100 子网掩码：255.255.255.0 -> 点击确定
在虚拟软件上 --My Computer -> 选中虚拟机 -> 右键 -> settings -> network adapter -> host only -> ok

--------------------------------------------------------------------------------
补充1：让普通用户具备sudo执行权限
切换到root
然后 vi /etc/sudoers  加入一行
root     ALL=(ALL)     ALL
hadoop    ALL=(ALL)     ALL
最后  ：wq！ 
退出强制保存。

----------------------------------------------------------------------------------
vi可以分为三种状态，分别是命令模式（command mode）、插入模式（Insert mode）和底行模式（last line mode），各模式的功能区分如下：

1) 命令行模式command mode）

控制屏幕光标的移动，字符、字或行的删除，移动复制某区段及进入Insert mode下，或者到 last line mode。

2) 插入模式（Insert mode）

只有在Insert mode下，才可以做文字输入，按「ESC」键可回到命令行模式。

3) 底行模式（last line mode）

将文件保存或退出vi，也可以设置编辑环境，如寻找字符串、列出行号……等。

不过一般我们在使用时把vi简化成两个模式，就是将底行模式（last line mode）也算入命令行模式command mode）。


#######   i 表示切换到插入模式  etc键表示切换到命令模式

我一般不用vi命令，用gedit

=-----------------------------------------------------------------------------------
补充2：让linux服务器在启动时不启动图形界面
sudo vi /etc/inittab 
将启动级别改为3
id:3:initdefault:

---------------------------------------------------------------------------

配好网络以后开始搭建啦---------------------------------------------------------------------------

**集群分布**
192.168.65.60   master
192.168.65.61   slave1
192.168.65.61   slave2
**1.1修改主机名**
vim /etc/sysconfig/network

NETWORKING=yes
HOSTNAME=itcast    ###

**1.2修改IP**
两种方式：
第一种：通过Linux图形界面进行修改（强烈推荐）
进入Linux图形界面 -> 右键点击右上方的两个小电脑 -> 点击Edit connections -> 选中当前网络System eth0 -> 点击edit按钮 -> 选择IPv4 -> method选择为manual -> 点击add按钮 -> 添加IP：192.168.1.101 子网掩码：255.255.255.0 网关：192.168.1.1 -> apply

第二种：修改配置文件方式（屌丝程序猿专用）
vim /etc/sysconfig/network-scripts/ifcfg-eth0

DEVICE="eth0"
BOOTPROTO="static"               ###
HWADDR="00:0C:29:3C:BF:E7"
IPV6INIT="yes"
NM_CONTROLLED="yes"
ONBOOT="yes"
TYPE="Ethernet"
UUID="ce22eeca-ecde-4536-8cc2-ef0dc36d4a8c"
IPADDR="192.168.1.101"           ###
NETMASK="255.255.255.0"          ###
GATEWAY="192.168.1.1"            ###

改完配置文件后不会立即生效，必须重启linux服务器（reboot）或者是重启network服务（sudo service  network restart）。

**1.3修改主机名和IP的映射关系**
                 vim /etc/hosts

                192.168.65.60 master

                192.168.65.61 slave1
                192.168.65.62 slave2

**1.4关闭防火墙（系统相关操作，下面所有的操作前面加上 sudo ）**
#查看防火墙状态
service iptables status
#关闭防火墙
service iptables stop
#查看防火墙开机启动状态
chkconfig iptables --list
#关闭防火墙开机启动

chkconfig iptables off

               #再查看防火墙开机启动状态，所有的都是off才可以


**1.5重启Linux**
reboot

**2.安装JDK**
2.1上传alt+p 后出现sftp窗口，然后put d:\xxx\yy\ll\jdk-7u_65-i585.tar.gz

注意，此处为32位的jdk，对应着32位的linux虚拟机。

2.2解压jdk
#创建文件夹
mkdir /home/hadoop/app
#解压到新建的文件夹中
tar -zxvf jdk-7u65-linux-i586.tar.gz -C /home/hadoop/app

2.3将java添加到环境变量中
vim /etc/profile
#在文件最后添加
export JAVA_HOME=/home/hadoop/app/jdk1.7.0_65
export PATH=$PATH:$JAVA_HOME/bin

#刷新配置///因为上面的操作还没有生效，下面命令使之生效。
source /etc/profile

**3.安装hadoop2.4.1**
先上传hadoop的安装包到服务器上去/home/hadoop/
将其解压到/home/hadoop/app  文件夹中
注意：hadoop2.x的配置文件$HADOOP_HOME/etc/hadoop
伪分布式需要修改5个配置文件（在/home/hadoop/app/hadoop-2.4.1/etc/hadoop中）

1.3.2，，修改slves文件中的配置
  slave1
  slave2

3.1配置hadoop
第一个：hadoop-env.sh
vim hadoop-env.sh
#第27行xport JAVA_HOME=${JAVA_HOME}改为：
export JAVA_HOME=/home/hadoop/app/jdk1.7.0_65

第二个：core-site.xml

<!-- 指定HADOOP所使用的文件系统schema（URI），HDFS的老大（NameNode）的地址 -->
<property>
<name>fs.defaultFS</name>
<value>hdfs://本主机名:9000</value>
</property>
<!-- 指定hadoop运行时产生文件的存储目录 -->
<property>
<name>hadoop.tmp.dir</name>
<value>/home/hadoop/app/hadoop-2.4.1/data/</value>

    </property>

第三个：hdfs-site.xml   hdfs-default.xml  (3)
<!-- 指定HDFS副本的数量，一般是3个， -->
<!-- 还可以在此处指定hdfs块的大小，hadoop2默认是128兆，hadoop1中默认是64兆-->
<property>
<name>dfs.replication</name>
<value>2</value>

    </property>

第四个：mapred-site.xml (在进行前面操作之前，先改名字 mv mapred-site.xml.template mapred-site.xml)
mv mapred-site.xml.template mapred-site.xml
vim mapred-site.xml
<!-- 指定mr运行在yarn上（指定mr程序在哪个资源调度集群上跑，如果不指定，就在本地跑，无意义。） -->
<property>
<name>mapreduce.framework.name</name>
<value>yarn</value>

    </property>

第五个：yarn-site.xml
<!-- 指定YARN的老大（ResourceManager）的地址 -->
<property>
<name>yarn.resourcemanager.hostname</name>
<value>本主机名称</value>

    </property>
<!-- reducer获取数据的方式 -->

    <property>
<name>yarn.nodemanager.aux-services</name>
<value>mapreduce_shuffle</value>

     </property>

3.2将hadoop添加到环境变量

vim /etc/profile

export HADOOP_HOME=/home/hadoop/app/hadoop-2.4.1
export PATH=$PATH:$JAVA_HOME/bin:$HADOOP_HOME/bin:$HADOOP_HOME/sbin

source /etc/profile

3.3格式化namenode（是对namenode进行初始化）
hdfs namenode -format (hadoop namenode -format)
你好，格式化的时候，找不到hadoop，所以你在hadoop的bin下，用 ./hadoop namenode -format,应该就好了。

3.4启动hadoop（在sbin目录下）
先启动HDFS
./start-dfs.sh

再启动YARN（在sbin目录下）
./start-yarn.sh

3.5验证是否启动成功
使用jps命令验证
27408 NameNode
28218 Jps
27643 SecondaryNameNode
28066 NodeManager
27803 ResourceManager
27512 DataNode

http://192.168.65.104:50070 （HDFS管理界面）
http://192.168.65.104:8088 （MR管理界面）

**4.配置ssh免密码登陆**
#生成ssh免登陆密钥
#进入到我的home目录
cd ~/.ssh

ssh-keygen -t rsa 产生密钥对（四个回车）
执行完这个命令后，会生成两个文件id_rsa（私钥）、id_rsa.pub（公钥）
将公钥拷贝到要免登陆的机器上
scp id_rsa.pub 被请求的主机名：/home/hadoop   
将公玥加到authorized_keys列表中
touch authorized_keys
chmod 600 authorized_keys //修改权限

  cat /home/hadoopid_rsa.pub >> authorized_keys

**ssh免密码登陆原理见：[点击打开链接](http://blog.csdn.net/u013527419/article/details/50985237)**



