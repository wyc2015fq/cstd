# Hadoop2.2.0伪分布式搭建 - 一个跳popping的quant的博客 - CSDN博客





2017年08月31日 19:47:43[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：606








在hadoop中，分为单机模式，伪分布式，和完全分布式。而伪分布式在1.X中就是类似JobTracker和TaskTracker都在一台机器上运行，在2.X中，就是NameNode和DataNode在一台机器上，而并没有实现多台机器来进行真正的分布式计算。

一、Linux环境的准备 

  首先说一下VMware的三种工作模式桥接（bridge）、NAT（网络地址转换）和host-only（主机模式）。参考 [http://jingyan.baidu.com/article/3f16e003cd0a0d2591c103b4.html](http://jingyan.baidu.com/article/3f16e003cd0a0d2591c103b4.html)

   桥接模式下，VMWare虚拟出来的操作系统就像是局域网中的一台独立的主机（主机和虚拟机处于对等地位），它可以访问网内任何一台机器。在桥接模式下，我们往往需要为虚拟主机配置ＩＰ地址、子网掩码等（注意虚拟主机的ｉｐ地址要和主机ｉｐ地址在同一网段）。 

   NAT模式下，虚拟系统需要借助NAT(网络地址转换)功能，通过宿主机器所在的网络来访问公网。也就是说，使用NAT模式虚拟系统可把物理主机作为路由器访问互联网。NAT模式下的虚拟系统的TCP/IP配置信息是由VMnet8(NAT)虚拟网络的DHCP服务器提供的，无法进行手工修改，因此虚拟系统也就无法和本局域网中的其他真实主机进行通讯。采用NAT模式最大的优势是虚拟系统接入互联网非常简单，你不需要进行任何其他的配置，只需要宿主机器能访问互联网即可。 

  主机模式下，真实环境和虚拟环境是隔离开的；在这种模式下，所有的虚拟系统是可以相互通信的，但虚拟系统和真实的网络是被隔离开。（虚拟系统和宿主机器系统是可以相互通信的，相当于这两台机器通过双绞线互连。）
这里使用仅主机模式（host only）来进行实现虚拟机和主机的通信。在安装VMware的时候，在windows上同时安装了两块网卡，可以在网络和共享中心中看到，一个是VMnet1，另一个是VMnet2。在VMnet1的属性中，设置IPv4的windows的IP：192.168.1.100，子网掩码255.255.255.0确定。然后在VMware中，打开编辑虚拟网络编辑器，设置主机模式的ip网段为192.168.1.0 子网掩码255.255.255.0然后应用。 

  1.1进入Linux，修改主机名 

  vim /etc/sysconfig/network 

  NETWORKING=yes  

  HOSTNAME=itcast01    ### 

  1.2修改IP 

        两种方式： 

        第一种：通过Linux图形界面进行修改（强烈推荐） 

            进入Linux图形界面 -> 右键点击右上方的两个小电脑 -> 点击Edit connections -> 选中当前网络System eth0 -> 点击edit按钮 -> 选择IPv4 -> method选择为manual -> 点击add按钮 -> 添加IP：192.168.1.44 子网掩码：255.255.255.0 网关：192.168.1.1 -> apply 

            第二种：修改配置文件方式 

            vim /etc/sysconfig/network-scripts/ifcfg-eth0
```
DEVICE="eth0"
        BOOTPROTO="static"           ###
        HWADDR="00:0C:29:3C:BF:E7"
        IPV6INIT="yes"
        NM_CONTROLLED="yes"
        ONBOOT="yes"
        TYPE="Ethernet"
        UUID="ce22eeca-ecde-4536-8cc2-ef0dc36d4a8c"
        IPADDR="192.168.1.44"       ###
        NETMASK="255.255.255.0"      ###
        GATEWAY="192.168.1.1"        ###
```

1.3修改主机名和IP的映射关系 

        vim /etc/hosts 

        192.168.1.44    itcast01
1.4关闭防火墙 

        #查看防火墙状态 

        service iptables status 

        #关闭防火墙 

        service iptables stop 

        #查看防火墙开机启动状态 

        chkconfig iptables –list 

        #关闭防火墙开机启动 

        chkconfig iptables off
1.5重启Linux 

        reboot

二、安装JDK 

    ps:我在安装JDK时，出现了/lib/ld-linux.so.2: bad ELF interpreter: 没有那个文件或目录的问题，原因是没有考虑虚拟机的位数导致了安装了32bit的JDK在了64bit的虚拟机上。所以可以通过more /proc/version 查看位数。另外在卸载之前的JDK版本时可以：借鉴 [http://blog.csdn.net/zhongguozhichuang/article/details/52727745](http://blog.csdn.net/zhongguozhichuang/article/details/52727745)

1、卸载用 bin文件安装的JDK方法： 

     删除/usr/java目录下的所有东西

2、卸载系统自带的jdk版本方法： 

查看自带的jdk：
`#rpm -qa | grep gcj`
看到如下信息： 

libgcj-4.1.2-44.el5 

java-1.4.2-gcj-compat-1.4.2.0-40jpp.115 

使用rpm -e –nodeps 命令删除上面查找的内容：`#rpm -e –nodeps java-1.4.2-gcj-compat-1.4.2.0-40jpp.115`
3、卸载rpm安装的jdk版本 

查看安装的jdk：
`#rpm -qa|grep jdk`
看到如下信息： 

java-1.6.0-openjdk-1.6.0.0-0.25.b09.el5 

jdk-1.6.0_16-fcs 

卸载：`#rpm -e --nodeps jdk-1.6.0_16-fcs`
2.1上传 

    2.2解压jdk 

        #创建文件夹 

        mkdir /usr/java 

        #解压 

        tar -zxvf jdk-7u55-linux-i586.tar.gz -C /usr/java/ 

       2.3将java添加到环境变量中 

        vim /etc/profile 

        #在文件最后添加 

        export JAVA_HOME=/usr/java/jdk1.7.0_55 

        export PATH=$PATH:$JAVA_HOME/bin 

   #刷新配置 

        source /etc/profile 

三、安装Hadoop 

    3.1上传hadoop安装包 

    3.2解压hadoop安装包 

        mkdir /cloud 

        #解压到/cloud/目录下 

        tar -zxvf hadoop-2.2.0.tar.gz -C /cloud/ 

        3.3修改配置文件（5个） 

        第一个：hadoop-env.sh 

        把export JAVA_HOME=${JAVA_HOME} 

                改为export JAVA_HOME=/usr/java/jdk1.7.0_55 

        第二个：core-site.xml
```xml
<configuration>
            <!-- 指定HDFS老大（namenode）的通信地址 HDFS的小弟是datanode-->
            <property>
                    <name>fs.defaultFS</name>
                        <value>hdfs://itcast01:9000</value>
            </property>
            <!-- 指定hadoop运行时产生文件的存储路径 -->
            <property>
                    <name>hadoop.tmp.dir</name>
                    <value>/cloud/hadoop-2.2.0/tmp</value>
            </property>
        </configuration>
```

```
第三个：hdfs-site.xml
```

```xml
<configuration>
            <!-- 设置hdfs副本数量 备份的个数-->
            <property>
                    <name>dfs.replication</name>
                    <value>1</value>
            </property>
        </configuration>
```

```
第四个：mapred-site.xml.template 需要重命名： mv mapred-site.xml.template mapred-site.xml
```

```xml
<configuration>
            <!-- 通知框架MR使用YARN 告诉hadoopMR运行在yarn上-->
            <property>
                    <name>mapreduce.framework.name</name>
                    <value>yarn</value>
            </property>
        </configuration>
```

```
第五个：yarn-site.xml
```

```xml
<configuration>
            <!-- reducer取数据的方式是shuffle -->                        
            <property>
                <name>yarn.nodemanager.aux-services</name>
                <value>mapreduce_shuffle</value>
            </property>
<!--指定yarn的老大（ResourceManager）的地址 yarn的小弟是NodeManager-->
<property>
    <name>yarn.resourcemanager.hostname</name>
    <value>itcast01</value>
</property>
</configuration>
```

3.4将hadoop添加到环境变量 

        vim /etc/profile

```
export JAVA_HOME=/usr/java/jdk1.7.0_55
    export HADOOP_HOME=/cloud/hadoop-2.2.0
    export PATH=$PATH:$JAVA_HOME/bin:$HADOOP_HOME/bin
        这里也可以把sbin添加到PATH中
    source /etc/profile
```

3.5格式化HDFS（namenode）第一次使用时要格式化 

        hadoop namenode -format（过时了，但是也可以使用） 

        hdfs namenode -format（建议使用） 

3.6启动hadoop
```
使用sbin/start-all.sh（过时了）

          或者先启动HDFS
      sbin/start-dfs.sh
      再启动YARN
      sbin/start-yarn.sh
```

3.7验证是否启动成功 

        使用jps（Java Virtual Machine Process Status Tool查看java进程的所有pid）命令验证 

        27408 NameNode 

        28218 Jps 

        27643 SecondaryNameNode 

        28066 NodeManager 

        27803 ResourceManager 

        27512 DataNode
可以在本地查看[http://192.168.1.44:50070](http://192.168.1.44:50070)  (HDFS管理界面) [http://192.168.1.44:8088](http://192.168.1.44:8088) （MR管理界面） 

   在本地这个文件中添加linux主机名和IP的映射关系 

  C:\Windows\System32\drivers\etc\hosts 

        192.168.1.119   itcast  

   之后ip地址可以用itcast主机名来代替
四、配置ssh免登陆 

    生成ssh免登陆密钥 

    cd ~，进入到我的home目录 

    cd .ssh/ 

       ssh-keygen -t rsa （四个回车） 

    执行完这个命令后，会生成两个文件id_rsa（私钥）、id_rsa.pub（公钥） 

    将公钥拷贝到要免登陆的机器上 

    cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys 

    或 

    ssh-copy-id -i localhost 













