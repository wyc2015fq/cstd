# centos6.5安装hadoop2 - littlely_ll的博客 - CSDN博客





2017年06月25日 20:53:21[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：308标签：[centos																[hadoop](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=centos&t=blog)
个人分类：[hadoop数据分析](https://blog.csdn.net/littlely_ll/article/category/6943993)








# 准备Linux环境

点击VMware快捷方式，右键打开文件所在位置 -> 双击vmnetcfg.exe -> VMnet1 host-only ->修改subnet ip 设置网段：192.168.1.90 （这儿需要对应自己的地址）子网掩码：255.255.255.0 -> apply -> ok 

        回到windows –> 打开网络和共享中心 -> 更改适配器设置 -> 右键VMnet1 -> 属性 -> 双击IPv4 -> 设置windows的IP：192.168.1.110 子网掩码：255.255.255.0 -> 点击确定 

        在虚拟软件上 –My Computer -> 选中虚拟机 -> 右键 -> settings -> network adapter -> host only -> ok 
**带有###的为要修改的**
## 修改主机名

```
vim /etc/sysconfig/network

        NETWORKING=yes 
        HOSTNAME=centos    ###
```

## 修改IP

两种方式：

### 第一种：通过Linux图形界面进行修改（强烈推荐）

进入Linux图形界面 -> 右键点击右上方的两个小电脑 -> 点击Edit connections -> 选中当前网络System eth0 -> 点击edit按钮 -> 选择IPv4 -> method选择为manual -> 点击add按钮 -> 添加IP：192.168.1.119 子网掩码：255.255.255.0 网关：192.168.1.1 -> apply

### 第二种：修改配

置文件方式（屌丝程序猿专用）

```
vim /etc/sysconfig/network-scripts/ifcfg-eth0

            DEVICE="eth0"
            BOOTPROTO="static"           ###
            HWADDR="00:0C:29:3C:BF:E7"
            IPV6INIT="yes"
            NM_CONTROLLED="yes"
            ONBOOT="yes"
            TYPE="Ethernet"
            UUID="ce22eeca-ecde-4536-8cc2-ef0dc36d4a8c"
            IPADDR="192.168.1.90"       ###
            NETMASK="255.255.255.0"      ###
            GATEWAY="192.168.1.1"        ###
            DNS1=8.8.8.8 ###
            DNS2=8.8.4.4 ###
```

## 修改主机名和IP的映射关系

```
vim /etc/hosts

    192.168.1.90    centos
```

## 关闭防火墙

```
#查看防火墙状态
    service iptables statushostname
    #关闭防火墙
    service iptables stop
    #查看防火墙开机启动状态
    chkconfig iptables --list
    #关闭防火墙开机启动
    chkconfig iptables off
```

## 重启Linux

```
reboot
```

# 安装JDK

## 上传JDK压缩包

## 解压jdk

```
#创建文件夹
        mkdir /usr/java
        #解压
        tar -zxvf jdk-7u80-linux-i586.tar.gz -C /usr/java/
```

## 将java添加到环境变量中

```bash
vim /etc/profile
        #在文件最后添加
        export JAVA_HOME=/usr/java/jdk1.7.0_80 ##
        export PATH=$PATH:$JAVA_HOME/bin
```

#刷新配置 

source /etc/profile

# 安装Hadoop

## 上传hadoop安装包

## 解压hadoop安装包

```
mkdir /itcast
        #解压到/itcast/目录下
        tar -zxvf hadoop-2.2.0.tar.gz -C /itcast/
```

## 修改配置文件（5个）

(/itcast/hadoop-2.2.0/etc/hadoop路径下)

```
第一个：hadoop-env.sh
           vim /hadoop-env.sh
    #在27行修改
    export JAVA_HOME=/usr/java/jdk1.7.0_80

    第二个：core-site.xml
    <configuration>
        <!-- 指定HDFS老大（namenode）的通信地址 -->
        <property>
                <name>fs.defaultFS</name>
                <value>hdfs://centos:9000</value>
        </property>
        <!-- 指定hadoop运行时产生文件的存储路径 -->
        <property>
                <name>hadoop.tmp.dir</name>
                <value>/itcast/hadoop-2.2.0/tmp</value>
        </property>
    </configuration>

    第三个：hdfs-site.xml
    <configuration>
        <!-- 设置hdfs副本数量 -->
        <property>
                <name>dfs.replication</name>
                <value>1</value>
        </property> 
    </configuration>

    第四个：mapred-site.xml.template 需要重命名： mv mapred-site.xml.template mapred-site.xml
    <configuration>
        <!-- 通知框架MR使用YARN -->
        <property>
                <name>mapreduce.framework.name</name>
                <value>yarn</value>
        </property>
    </configuration>

    第五个：yarn-site.xml
    <configuration>
        <!-- reducer取数据的方式是mapreduce_shuffle -->
        <property>
            <name>yarn.nodemanager.aux-services</name>
            <value>mapreduce_shuffle</value>
        </property>
                    <property>
                           <name>yarn.resourcemanager.hostname</name>
                           <value>centos</value>
                    </property>
    </configuration>
```

## 将hadoop添加到环境变量

```bash
vim /etc/profile

        export JAVA_HOME=/usr/java/jdk1.7.0_80
        export HADOOP_HOME=/itcast/hadoop-2.2.0
        export PATH=$PATH:$JAVA_HOME/bin:$HADOOP_HOME/bin
```

source /etc/profile #刷新配置

## 格式化HDFS（namenode）第一次使用时要格式化

(/itcast/ahdoop-2.2.0/路径下)

```
hadoop namenode -format（过时了，用hdfs namenode -format）
```

## 启动hadoop

```
先启动HDFS
    sbin/start-dfs.sh

    再启动YARN
    sbin/start-yarn.sh   ##(sbin下)./start.all.sh（过时了）
```

## 验证是否启动成功

(/itcast/ahdoop-2.2.0/sbin下)

```
使用jps命令验证
    27408 NameNode
    28218 Jps
    27643 SecondaryNameNode
    28066 NodeManager
    27803 ResourceManager
    27512 DataNode
which jps
    ##显示/usr/java/jdk1.7.0_80/bin.jps
    http://192.168.1.90:50070  (HDFS管理界面)
    在这个文件中添加linux主机名和IP的映射关系
    C:\Windows\System32\drivers\etc\hosts
    192.168.1.119   itcast

    http://192.168.1.90:8088 （MR管理界面）
```

# 配置ssh免登陆

```
生成ssh免登陆密钥
cd ~，进入到我的home目录
cd .ssh/

ssh-keygen -t rsa （四个回车）
执行完这个命令后，会生成两个文件id_rsa（私钥）、id_rsa.pub（公钥）
将公钥拷贝到要免登陆的机器上
cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys
或
ssh-copy-id -i localhost 
(cp id_rsa.pub authorized_keys)
```





