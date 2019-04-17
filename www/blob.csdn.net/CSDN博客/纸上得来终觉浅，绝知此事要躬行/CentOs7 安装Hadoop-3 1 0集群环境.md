# CentOs7 安装Hadoop-3.1.0集群环境 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月17日 16:25:57[boonya](https://me.csdn.net/boonya)阅读数：1435
所属专栏：[Hadoop学习实践](https://blog.csdn.net/column/details/14484.html)









虚拟机准备



```
master  172.16.20.11

slave1  172.16.20.12

slave2  172.16.20.13
```

安装JDK


version 1.8.0_121，注意需要配置环境变量。


安装：参考[https://blog.csdn.net/boonya/article/details/55506386](https://blog.csdn.net/boonya/article/details/55506386)



SSH免密登录

1、修改hostname

```
[root@localhost local]# hostname master
[root@localhost local]# hostname
master
[root@localhost local]#


[root@localhost local]# hostname slave1
[root@localhost local]# hostname
slave1
[root@localhost local]#


[root@localhost local]# hostname slave2
[root@localhost local]# hostname
slave2
[root@localhost local]#
```

2、修改/etc/hosts文件


 修改这3台机器的/etc/hosts文件，在文件中添加以下内容：


```
172.16.20.11   master
172.16.20.12   slave1
172.16.20.13   slave2
```



3、ping验证







```
[root@localhost local]# ping -c 3 slave1
PING slave1 (172.16.20.11) 56(84) bytes of data.
64 bytes from master (172.16.20.11): icmp_seq=1 ttl=64 time=0.058 ms
64 bytes from master (172.16.20.11): icmp_seq=2 ttl=64 time=0.047 ms
64 bytes from master (172.16.20.11): icmp_seq=3 ttl=64 time=0.058 ms

--- slave1 ping statistics ---
3 packets transmitted, 3 received, 0% packet loss, time 2000ms
rtt min/avg/max/mdev = 0.047/0.054/0.058/0.008 ms
[root@localhost local]#
```



4、每个机器生成秘钥文件


秘钥生成命令：ssh-keygen  -t   rsa   -P  ''

root秘钥文件保存目录：/root/.ssh/

查看秘钥：cd  /root/.ssh/   里面包含如下文件：id_rsa  id_rsa.pub  known_hosts



5、创建authorized_keys


 在3台机器的/root/.ssh/目录下都存入一个内容相同的文件，文件名称叫authorized_keys

 合并每个节点生成的id_rsa.pub到authorized_keys


authorized_keys内容:









```
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQDwyWE1fa07nuDV0lcpczMktjX76B2TkjRyirTf4jUB9nQKneWWdCu4tUoTmlMkuG3TRt4gHLxNFbdGUK0nO7rbczUm2AdQwUNmVJKGiBk9A9AYkqbjgB8k3J0cv7PAutVHF+R0jsPArYH0FmDugAS2T24GIPNfOIu+HBsU3qlGwo8/SKiAUifO9xd7znDNBdjEptCp/9ZFB9MJN0XKLx8LG1c2nfduQBjG0UqtWjryB3GnGgaKVIGIdHW4pcZ5YYkrLqhRmjscxUmCHdHWe40fpt0J2Bt9j3LCGmuNkK3+9p3zlChPKhM8zTZW6NDLuBToZxBJFBGbWMQUag2YHVVT root@master
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCs6RYt1z/JZrXaAXfinffEH8pYzZc11vmNQ7QvSXnBFGeGvHVX/vQhwXgoIONdYyz4pCDUnyWa1d2sbEmq+gN+Dg1I1CPb6kBP5ZZuah5A1IuIFzN+OUU0UqaKnjv+WGJdf4HmxiVy6VUQsMKxoLifQFxIqfn30+KGPwN9yKS5NPG/eJrUPoUtrxumLiQ9tS3H5NWKwE86Uock1VIxOoZx6SjuoqynoIZb5i24wiN0k+DeyrYQJaPj3+vyTv9M1XFGgjMUxhBYXTNhBjMTUEY03K3ytWxBozH0Ffto9ITHFUQ0ax/92YtMPOE87e7Sp4DRILjmPnu9WahS6SISfRp9 root@slave1
ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQCyjeMfF92+Wvus9JdatBCGR6jKN+ZZvehDQ08fxIhcbWBymKNzz6HkS6puHFFj3aM9lFW6cE2+/pR6CLi/tQ8tGZTOm1eAjv+RwKGZvckjyE080ZfhRgjM4zy4NcF3udM1Dd4y4nPEb2h8E+ZmTL2KALuYYeDgliQQ/1TDDxtJFPxUV/R5Ul8tZZtIuyFyZJtv60NmYDkzvjcmG80OQmJkFH7jlPrErdYQORTYraDC8aLF2QJ8FnbxkYcNStKjkcuSEyBv26fE/dXO3wZD04IDS8RE35Zd0aome/8OKkQfe19LORRJne3AyM9V1xxl7cXUQxLEikvpIV33KnH8Dy0X root@slave2
```



6、测试SSH无密访问









```
[root@localhost .ssh]# ssh slave1
The authenticity of host 'slave1 (172.16.20.11)' can't be established.
ECDSA key fingerprint is ab:fb:8a:69:f0:ab:2f:3b:c4:1e:74:85:93:19:49:7c.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'slave1' (ECDSA) to the list of known hosts.
Last login: Fri Jun 15 02:33:51 2018 from master
[root@master ~]#
```



注意：每次ssh完成后，都要执行exit，否则你的后续命令是在另外一台机器上执行的。









```
[root@localhost .ssh]# ssh slave2
The authenticity of host 'slave2 (172.16.20.11)' can't be established.
ECDSA key fingerprint is ab:fb:8a:69:f0:ab:2f:3b:c4:1e:74:85:93:19:49:7c.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added 'slave2,172.16.20.11' (ECDSA) to the list of known hosts.
Last login: Fri Jun 15 02:05:25 2018 from 172.16.22.31
[root@master ~]#
```



注意：每次ssh完成后，都要执行exit，否则你的后续命令是在另外一台机器上执行的。





Hadoop安装配置



version 3.1.0，下载地址：[http://hadoop.apache.org/releases.html#Download](http://hadoop.apache.org/releases.html#Download)


1、上传：


分别创建/opt/hadoop目录:mkdir /opt/hadoop,cd /opt/hadoop进入目录上传Hadoop安装包。


2、解压：


tar -zxvf hadoop-3.1.0.tar.gz 



3、分别在/root目录下新建几个目录，复制粘贴执行下面的命令：







```
mkdir  /root/hadoop  
mkdir  /root/hadoop/tmp  
mkdir  /root/hadoop/var  
mkdir  /root/hadoop/dfs  
mkdir  /root/hadoop/dfs/name  
mkdir  /root/hadoop/dfs/data
```









4、分别配置Hadoop环境变量



在/etc/profile中添加如下代码

```
export HADOOP_HOME=/opt/hadoop/hadoop-3.1.0
export PATH=$PATH:$HADOOP_HOME/bin
```

使修改生效
`source /etc/profile`


Hadoop修改配置和测试

下面开始修改master主机cd /opt/hadoop/hadoop-3.1.0/etc/hadoop目录内的一系列文件，然后将此目录拷贝到其他两个机器。

1、修改vi /opt/hadoop/hadoop-3.1.0/etc/hadoop/core-site.xml文件



 在<configuration>节点内加入配置:







```
<configuration>
       <property>
        <name>hadoop.tmp.dir</name>
        <value>/root/hadoop/tmp</value>
        <description>Abase for other temporary directories.</description>
       </property>
       <property>
        <name>fs.default.name</name>
        <value>hdfs://master:9000</value>
       </property>

</configuration>
```



2、 修改/opt/hadoop/hadoop-3.1.0/etc/hadoop/hadoop-env.sh文件





添加如下JDK配置：


` export   JAVA_HOME=${JAVA_HOME}`
配置JDK安装目录。

3、  修改vi /opt/hadoop/hadoop-3.1.0/etc/hadoop/hdfs-site.xml文件


在<configuration>节点内加入配置:


```
<!-- 设置namenode的http通讯地址 -->
     <property>
        <name>dfs.namenode.http-address</name>
        <value>master:50070</value>
     </property>
    <property>
       <name>dfs.name.dir</name>
       <value>/root/hadoop/dfs/name</value>
       <description>Path on the local filesystem where theNameNode stores the namespace and transactions logs persistently.</description>
    </property>
    <property>
       <name>dfs.data.dir</name>
       <value>/root/hadoop/dfs/data</value>
       <description>Comma separated list of paths on the localfilesystem of a DataNode where it should store its blocks.</description>
    </property>
    <property>
       <name>dfs.replication</name>
       <value>2</value>
    </property>
    <property>
          <name>dfs.permissions</name>
          <value>true</value>
          <description>need not permissions</description>
    </property>
```

 说明：dfs.permissions配置为false后，可以允许不要检查权限就生成dfs上的文件，方便倒是方便了，但是你需要防止误删除，请将它设置为true，或者直接将该property节点删除，因为默认就是true。

4、修改mapred-site.xml文件，在<configuration>节点内加入配置:







```
<property>
        <name>mapred.job.tracker</name>
        <value>master:49001</value>
    </property>
    <property>
          <name>mapred.local.dir</name>
           <value>/root/hadoop/var</value>
    </property>
    <property>
           <name>mapreduce.framework.name</name>
           <value>yarn</value>
    </property>
```



5、 修改vi /opt/hadoop/hadoop-3.1.0/etc/hadoop/workers文件，将里面的localhost删除，添加如下内容：









```
slave1  
slave2
```

6、  修改/opt/hadoop/hadoop-3.1.0/etc/hadoop/yarn-site.xml文件

在<configuration>节点内加入配置(注意了，内存根据机器配置越大越好，我这里只配2个G是因为机器不行):



```
<property>
        <name>yarn.resourcemanager.hostname</name>
        <value>master</value>
   </property>
   <property>
        <description>The address of the applications manager interface in the RM.</description>
        <name>yarn.resourcemanager.address</name>
        <value>${yarn.resourcemanager.hostname}:8032</value>
   </property>
   <property>
        <description>The address of the scheduler interface.</description>
        <name>yarn.resourcemanager.scheduler.address</name>
        <value>${yarn.resourcemanager.hostname}:8030</value>
   </property>
   <property>
        <description>The http address of the RM web application.</description>
        <name>yarn.resourcemanager.webapp.address</name>
        <value>${yarn.resourcemanager.hostname}:8088</value>
   </property>
   <property>
        <description>The https adddress of the RM web application.</description>
        <name>yarn.resourcemanager.webapp.https.address</name>
        <value>${yarn.resourcemanager.hostname}:8090</value>
   </property>
   <property>
        <name>yarn.resourcemanager.resource-tracker.address</name>
        <value>${yarn.resourcemanager.hostname}:8031</value>
   </property>
   <property>
        <description>The address of the RM admin interface.</description>
        <name>yarn.resourcemanager.admin.address</name>
        <value>${yarn.resourcemanager.hostname}:8033</value>
   </property>
   <property>
        <name>yarn.nodemanager.aux-services</name>
        <value>mapreduce_shuffle</value>
   </property>
   <property>
        <name>yarn.scheduler.maximum-allocation-mb</name>
        <value>2048</value>
        <discription>每个节点可用内存,单位MB,默认8182MB</discription>
   </property>
   <property>
        <name>yarn.nodemanager.vmem-pmem-ratio</name>
        <value>2.1</value>
   </property>
   <property>
        <name>yarn.nodemanager.resource.memory-mb</name>
        <value>2048</value>
   </property>
   <property>
        <name>yarn.nodemanager.vmem-check-enabled</name>
        <value>false</value>
   </property>
```

说明：yarn.nodemanager.vmem-check-enabled这个的意思是忽略虚拟内存的检查，如果你是安装在虚拟机上，这个配置很有用，配上去之后后续操作不容易出问题。如果是实体机上，并且内存够多，可以将这个配置去掉。


7、初始化hadoop-master-只需要操作主节点(master)即可







```
master namenode

    slave1 datanode

    slave2 datanode
```



    进入Hadoop可执行目录：cd /opt/hadoop/hadoop-3.1.0/bin

**    初始化命令：./hadoop  namenode  -format**

======输出：================





```
[root@localhost bin]#  ./hadoop  namenode  -format
WARNING: Use of this script to execute namenode is deprecated.
WARNING: Attempting to execute replacement "hdfs namenode" instead.

WARNING: /opt/hadoop/hadoop-3.1.0/logs does not exist. Creating.
2018-06-15 03:52:43,865 INFO namenode.NameNode: STARTUP_MSG:
/************************************************************
STARTUP_MSG: Starting NameNode
STARTUP_MSG:   host = master/172.16.20.11
STARTUP_MSG:   args = [-format]
STARTUP_MSG:   version = 3.1.0
STARTUP_MSG:   classpath = /opt/hadoop/hadoop-3.1.0/etc/hadoop:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/httpcore-4.4.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jetty-util-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jackson-core-2.7.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/mockito-all-1.8.5.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerb-core-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerb-crypto-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/accessors-smart-1.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/stax2-api-3.1.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerb-simplekdc-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerby-asn1-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jersey-server-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/guava-11.0.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jetty-servlet-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-lang-2.6.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/httpclient-4.5.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jaxb-impl-2.2.3-1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jsch-0.1.54.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jackson-mapper-asl-1.9.13.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-cli-1.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-io-2.5.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerb-identity-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/xz-1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/slf4j-log4j12-1.7.25.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerb-server-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerby-config-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/avro-1.7.7.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-math3-3.1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/javax.servlet-api-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/log4j-1.2.17.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jackson-annotations-2.7.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jetty-io-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/paranamer-2.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/junit-4.11.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jetty-xml-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jetty-http-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jettison-1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/hadoop-auth-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-beanutils-1.9.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jsr305-3.0.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jetty-server-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/token-provider-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerb-common-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-compress-1.4.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-configuration2-2.1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/curator-recipes-2.12.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jackson-jaxrs-1.9.13.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jetty-webapp-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jackson-xc-1.9.13.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jackson-core-asl-1.9.13.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/curator-client-2.12.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/curator-framework-2.12.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jetty-security-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/zookeeper-3.4.9.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-net-3.6.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/woodstox-core-5.0.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jersey-json-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/hadoop-annotations-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/gson-2.2.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/htrace-core4-4.1.0-incubating.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/snappy-java-1.0.5.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/slf4j-api-1.7.25.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/json-smart-2.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/netty-3.10.5.Final.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/asm-5.0.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-collections-3.2.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jersey-core-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jersey-servlet-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerb-util-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerby-pkix-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jsr311-api-1.1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/nimbus-jose-jwt-4.41.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerb-client-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerb-admin-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-codec-1.11.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/re2j-1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-logging-1.1.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerby-xdr-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/metrics-core-3.2.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jcip-annotations-1.0-1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jackson-databind-2.7.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/hamcrest-core-1.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/kerby-util-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jsp-api-2.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jul-to-slf4j-1.7.25.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/protobuf-java-2.5.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/jaxb-api-2.2.11.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/lib/commons-lang3-3.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/hadoop-common-3.1.0-tests.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/hadoop-kms-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/hadoop-common-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/common/hadoop-nfs-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/httpcore-4.4.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jetty-util-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jackson-core-2.7.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerb-core-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/json-simple-1.1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerb-crypto-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/accessors-smart-1.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/stax2-api-3.1.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerb-simplekdc-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerby-asn1-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jersey-server-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/guava-11.0.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jetty-servlet-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-lang-2.6.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/httpclient-4.5.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jaxb-impl-2.2.3-1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jsch-0.1.54.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jackson-mapper-asl-1.9.13.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-cli-1.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-io-2.5.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerb-identity-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/xz-1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-daemon-1.0.13.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerb-server-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerby-config-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/avro-1.7.7.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-math3-3.1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/javax.servlet-api-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/log4j-1.2.17.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jackson-annotations-2.7.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jetty-io-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/paranamer-2.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jetty-xml-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jetty-http-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jettison-1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/hadoop-auth-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-beanutils-1.9.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jsr305-3.0.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/leveldbjni-all-1.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jetty-server-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/token-provider-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerb-common-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-compress-1.4.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-configuration2-2.1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/curator-recipes-2.12.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jackson-jaxrs-1.9.13.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jetty-webapp-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jackson-xc-1.9.13.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jackson-core-asl-1.9.13.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/curator-client-2.12.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/okio-1.6.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/curator-framework-2.12.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jetty-security-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/zookeeper-3.4.9.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-net-3.6.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/woodstox-core-5.0.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jersey-json-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/hadoop-annotations-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/gson-2.2.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/htrace-core4-4.1.0-incubating.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/snappy-java-1.0.5.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/json-smart-2.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/netty-3.10.5.Final.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/asm-5.0.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-collections-3.2.2.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jersey-core-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/okhttp-2.7.5.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jersey-servlet-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerb-util-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/netty-all-4.0.52.Final.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerby-pkix-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jsr311-api-1.1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/nimbus-jose-jwt-4.41.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerb-client-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jetty-util-ajax-9.3.19.v20170502.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerb-admin-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-codec-1.11.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/re2j-1.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-logging-1.1.3.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerby-xdr-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jcip-annotations-1.0-1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jackson-databind-2.7.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/kerby-util-1.0.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/protobuf-java-2.5.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/jaxb-api-2.2.11.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/lib/commons-lang3-3.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-httpfs-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-rbf-3.1.0-tests.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-3.1.0-tests.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-native-client-3.1.0-tests.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-rbf-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-client-3.1.0-tests.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-native-client-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-client-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-nfs-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/hdfs/hadoop-hdfs-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-common-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-shuffle-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-3.1.0-tests.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-app-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-hs-plugins-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-nativetask-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-core-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-examples-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-jobclient-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/mapreduce/hadoop-mapreduce-client-uploader-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/javax.inject-1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/snakeyaml-1.16.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/jackson-jaxrs-json-provider-2.7.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/jackson-jaxrs-base-2.7.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/guice-servlet-4.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/swagger-annotations-1.5.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/json-io-2.5.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/jersey-guice-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/aopalliance-1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/jersey-client-1.19.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/mssql-jdbc-6.2.1.jre7.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/geronimo-jcache_1.0_spec-1.0-alpha-1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/fst-2.50.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/java-util-1.9.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/guice-4.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/jackson-module-jaxb-annotations-2.7.8.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/HikariCP-java7-2.4.12.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/metrics-core-3.2.4.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/ehcache-3.3.1.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/lib/dnsjava-2.1.7.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-server-timeline-pluginstorage-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-common-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-server-common-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-services-core-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-applications-distributedshell-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-client-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-applications-unmanaged-am-launcher-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-registry-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-server-applicationhistoryservice-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-server-router-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-server-nodemanager-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-server-tests-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-server-sharedcachemanager-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-api-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-server-web-proxy-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-services-api-3.1.0.jar:/opt/hadoop/hadoop-3.1.0/share/hadoop/yarn/hadoop-yarn-server-resourcemanager-3.1.0.jar
STARTUP_MSG:   build = https://github.com/apache/hadoop -r 16b70619a24cdcf5d3b0fcf4b58ca77238ccbe6d; compiled by 'centos' on 2018-03-30T00:00Z
STARTUP_MSG:   java = 1.8.0_121
************************************************************/
2018-06-15 03:52:43,993 INFO namenode.NameNode: registered UNIX signal handlers for [TERM, HUP, INT]
2018-06-15 03:52:44,142 INFO namenode.NameNode: createNameNode [-format]
2018-06-15 03:52:48,834 WARN common.Util: Path /root/hadoop/dfs/name should be specified as a URI in configuration files. Please update hdfs configuration.
2018-06-15 03:52:48,836 WARN common.Util: Path /root/hadoop/dfs/name should be specified as a URI in configuration files. Please update hdfs configuration.
Formatting using clusterid: CID-b68f9bc2-8813-4fef-8e98-903e82769d41
2018-06-15 03:52:48,944 INFO namenode.FSEditLog: Edit logging is async:true
2018-06-15 03:52:49,207 INFO namenode.FSNamesystem: KeyProvider: null
2018-06-15 03:52:49,208 INFO namenode.FSNamesystem: fsLock is fair: true
2018-06-15 03:52:49,210 INFO namenode.FSNamesystem: Detailed lock hold time metrics enabled: false
2018-06-15 03:52:49,254 INFO namenode.FSNamesystem: fsOwner             = root (auth:SIMPLE)
2018-06-15 03:52:49,254 INFO namenode.FSNamesystem: supergroup          = supergroup
2018-06-15 03:52:49,267 INFO namenode.FSNamesystem: isPermissionEnabled = true
2018-06-15 03:52:49,268 INFO namenode.FSNamesystem: HA Enabled: false
2018-06-15 03:52:49,359 INFO common.Util: dfs.datanode.fileio.profiling.sampling.percentage set to 0. Disabling file IO profiling
2018-06-15 03:52:49,393 INFO blockmanagement.DatanodeManager: dfs.block.invalidate.limit: configured=1000, counted=60, effected=1000
2018-06-15 03:52:49,393 INFO blockmanagement.DatanodeManager: dfs.namenode.datanode.registration.ip-hostname-check=true
2018-06-15 03:52:49,410 INFO blockmanagement.BlockManager: dfs.namenode.startup.delay.block.deletion.sec is set to 000:00:00:00.000
2018-06-15 03:52:49,410 INFO blockmanagement.BlockManager: The block deletion will start around 2018 Jun 15 03:52:49
2018-06-15 03:52:49,413 INFO util.GSet: Computing capacity for map BlocksMap
2018-06-15 03:52:49,414 INFO util.GSet: VM type       = 64-bit
2018-06-15 03:52:49,430 INFO util.GSet: 2.0% max memory 407.5 MB = 8.1 MB
2018-06-15 03:52:49,430 INFO util.GSet: capacity      = 2^20 = 1048576 entries
2018-06-15 03:52:49,470 INFO blockmanagement.BlockManager: dfs.block.access.token.enable = false
2018-06-15 03:52:49,531 INFO Configuration.deprecation: No unit for dfs.namenode.safemode.extension(30000) assuming MILLISECONDS
2018-06-15 03:52:49,532 INFO blockmanagement.BlockManagerSafeMode: dfs.namenode.safemode.threshold-pct = 0.9990000128746033
2018-06-15 03:52:49,532 INFO blockmanagement.BlockManagerSafeMode: dfs.namenode.safemode.min.datanodes = 0
2018-06-15 03:52:49,532 INFO blockmanagement.BlockManagerSafeMode: dfs.namenode.safemode.extension = 30000
2018-06-15 03:52:49,532 INFO blockmanagement.BlockManager: defaultReplication         = 2
2018-06-15 03:52:49,532 INFO blockmanagement.BlockManager: maxReplication             = 512
2018-06-15 03:52:49,532 INFO blockmanagement.BlockManager: minReplication             = 1
2018-06-15 03:52:49,533 INFO blockmanagement.BlockManager: maxReplicationStreams      = 2
2018-06-15 03:52:49,533 INFO blockmanagement.BlockManager: redundancyRecheckInterval  = 3000ms
2018-06-15 03:52:49,533 INFO blockmanagement.BlockManager: encryptDataTransfer        = false
2018-06-15 03:52:49,533 INFO blockmanagement.BlockManager: maxNumBlocksToLog          = 1000
2018-06-15 03:52:49,806 INFO util.GSet: Computing capacity for map INodeMap
2018-06-15 03:52:49,807 INFO util.GSet: VM type       = 64-bit
2018-06-15 03:52:49,807 INFO util.GSet: 1.0% max memory 407.5 MB = 4.1 MB
2018-06-15 03:52:49,807 INFO util.GSet: capacity      = 2^19 = 524288 entries
2018-06-15 03:52:50,059 INFO namenode.FSDirectory: ACLs enabled? false
2018-06-15 03:52:50,059 INFO namenode.FSDirectory: POSIX ACL inheritance enabled? true
2018-06-15 03:52:50,059 INFO namenode.FSDirectory: XAttrs enabled? true
2018-06-15 03:52:50,060 INFO namenode.NameNode: Caching file names occurring more than 10 times
2018-06-15 03:52:50,069 INFO snapshot.SnapshotManager: Loaded config captureOpenFiles: false, skipCaptureAccessTimeOnlyChange: false, snapshotDiffAllowSnapRootDescendant: true, maxSnapshotLimit: 65536
2018-06-15 03:52:50,077 INFO snapshot.SnapshotManager: SkipList is disabled
2018-06-15 03:52:50,095 INFO util.GSet: Computing capacity for map cachedBlocks
2018-06-15 03:52:50,095 INFO util.GSet: VM type       = 64-bit
2018-06-15 03:52:50,095 INFO util.GSet: 0.25% max memory 407.5 MB = 1.0 MB
2018-06-15 03:52:50,096 INFO util.GSet: capacity      = 2^17 = 131072 entries
2018-06-15 03:52:50,119 INFO metrics.TopMetrics: NNTop conf: dfs.namenode.top.window.num.buckets = 10
2018-06-15 03:52:50,119 INFO metrics.TopMetrics: NNTop conf: dfs.namenode.top.num.users = 10
2018-06-15 03:52:50,119 INFO metrics.TopMetrics: NNTop conf: dfs.namenode.top.windows.minutes = 1,5,25
2018-06-15 03:52:50,138 INFO namenode.FSNamesystem: Retry cache on namenode is enabled
2018-06-15 03:52:50,138 INFO namenode.FSNamesystem: Retry cache will use 0.03 of total heap and retry cache entry expiry time is 600000 millis
2018-06-15 03:52:50,150 INFO util.GSet: Computing capacity for map NameNodeRetryCache
2018-06-15 03:52:50,151 INFO util.GSet: VM type       = 64-bit
2018-06-15 03:52:50,151 INFO util.GSet: 0.029999999329447746% max memory 407.5 MB = 125.2 KB
2018-06-15 03:52:50,151 INFO util.GSet: capacity      = 2^14 = 16384 entries
2018-06-15 03:52:50,214 INFO namenode.FSImage: Allocated new BlockPoolId: BP-1573278845-172.16.20.11-1529049170199
2018-06-15 03:52:50,242 INFO common.Storage: Storage directory /root/hadoop/dfs/name has been successfully formatted.
2018-06-15 03:52:50,273 INFO namenode.FSImageFormatProtobuf: Saving image file /root/hadoop/dfs/name/current/fsimage.ckpt_0000000000000000000 using no compression
2018-06-15 03:52:50,459 INFO namenode.FSImageFormatProtobuf: Image file /root/hadoop/dfs/name/current/fsimage.ckpt_0000000000000000000 of size 389 bytes saved in 0 seconds .
2018-06-15 03:52:50,482 INFO namenode.NNStorageRetentionManager: Going to retain 1 images with txid >= 0
2018-06-15 03:52:50,546 INFO namenode.NameNode: SHUTDOWN_MSG:
/************************************************************
SHUTDOWN_MSG: Shutting down NameNode at master/172.16.20.11
************************************************************/
[root@localhost bin]#
```



 格式化成功后，可以在看到在/root/hadoop/dfs/name/目录多了一个current


8、启动namenode：hadoop-master


 进入指令操作：cd /opt/hadoop/hadoop-3.1.0/sbin


 执行指令： ./start-all.sh


 启动失败：



```
[root@localhost sbin]# ./start-all.sh
Starting namenodes on [master]
ERROR: Attempting to operate on hdfs namenode as root
ERROR: but there is no HDFS_NAMENODE_USER defined. Aborting operation.
Starting datanodes
ERROR: Attempting to operate on hdfs datanode as root
ERROR: but there is no HDFS_DATANODE_USER defined. Aborting operation.
Starting secondary namenodes [master]
ERROR: Attempting to operate on hdfs secondarynamenode as root
ERROR: but there is no HDFS_SECONDARYNAMENODE_USER defined. Aborting operation.
Starting resourcemanager
ERROR: Attempting to operate on yarn resourcemanager as root
ERROR: but there is no YARN_RESOURCEMANAGER_USER defined. Aborting operation.
Starting nodemanagers
ERROR: Attempting to operate on yarn nodemanager as root
ERROR: but there is no YARN_NODEMANAGER_USER defined. Aborting operation.
[root@localhost sbin]#
```



解决失败方案（参考：[https://blog.csdn.net/lglglgl/article/details/80553828](https://blog.csdn.net/lglglgl/article/details/80553828)）：

master，slave都需要修改start-dfs.sh，stop-dfs.sh，start-yarn.sh，stop-yarn.sh四个文件

将start-dfs.sh，stop-dfs.sh两个文件顶部添加以下参数



```
#!/usr/bin/env bash
HDFS_DATANODE_USER=root
HADOOP_SECURE_DN_USER=hdfs
HDFS_NAMENODE_USER=root
HDFS_SECONDARYNAMENODE_USER=root
```

start-yarn.sh，stop-yarn.sh顶部也需添加以下：







```
#!/usr/bin/env bash
YARN_RESOURCEMANAGER_USER=root
HADOOP_SECURE_DN_USER=yarn
YARN_NODEMANAGER_USER=root
```



WARNING: HADOOP_SECURE_DN_USER has been replaced by HDFS_DATANODE_SECURE_USER. Using value of HADOOP_SECURE_DN_USER.


将start-dfs.sh，stop-dfs.sh两个文件顶部添加以下参数









```
#!/usr/bin/env bash
HDFS_DATANODE_USER=root
HDFS_DATANODE_SECURE_USER=hdfs
HDFS_NAMENODE_USER=root
HDFS_SECONDARYNAMENODE_USER=root
```



start-yarn.sh，stop-yarn.sh顶部也需添加以下：





```
#!/usr/bin/env bash
YARN_RESOURCEMANAGER_USER=root
HDFS_DATANODE_SECURE_USER=yarn
YARN_NODEMANAGER_USER=root
```

9、复制/opt/hadoop到salve1和slave2节点



```
scp -r /opt/hadoop slave1:/opt/

 scp -r /opt/hadoop slave2:/opt/
```

10、浏览器访问（访问不到时请关闭master的防火墙systemctl   stop   firewalld.service）：

节点监控：http://172.16.20.11:50070/  

![](https://img-blog.csdn.net/20180619150612285?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

查看资源：http://172.16.20.11:8088/

![](https://img-blog.csdn.net/20180619150331224?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

配置不出来可以参考以下文章：

[Linux上安装Hadoop集群(CentOS7+hadoop-2.8.0)](https://blog.csdn.net/pucao_cug/article/details/71698903)

[centos7下hadoop-3.1.0集群搭建](https://my.oschina.net/orrin/blog/1816023)

[Hadoop3.1.0完全分布式集群部署超详细记录](https://blog.csdn.net/dream_an/article/details/80258283)

此处提供一组集群下载:[https://pan.baidu.com/s/1yvC7eQx_UFYuzgLbTNTw6Q](https://pan.baidu.com/s/1yvC7eQx_UFYuzgLbTNTw6Q)

![](https://img-blog.csdn.net/20180618003838855?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如有问题请参考：

[Hadoop 集群多次格式化导致容量分配为0不能导入数据](https://blog.csdn.net/boonya/article/details/85697815)

[Hadoop运行默认提供的字数统计示例](https://blog.csdn.net/boonya/article/details/85700599)



