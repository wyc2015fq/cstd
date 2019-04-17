# 关于Hadoop多用户管理支持客户端远程操作的理论总结 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年04月27日 16:52:55[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4943








**1、问题**

Hadoop客户端如何配置可远程操作Hadoop；Hadoop多用户情况下，是如何管理权限并分配存储空间和计算能力，保证集群的稳定。

**2、Hadoop平台**

要理解客户端如何通过指定用户远程操作hadoop集群，这里先理解下hadoop平台的部署架构。Hadoop是大数据分布式处理的软件框架，集成MapReduce分布式编程模型和HDFS分布式文件存储系统。MapReduce将执作业job分割成任务task，运行于各计算节点的。HDFS负责各个节点的数据存储，实现高吞吐率的数据读写。

Hadoop的主/从（Master/Slave）集群架构，主要由NameNode、DataNode、Secondary NameNode、JobTracker、TaskTracker组成。其中NameNode、Secondary  NameNode、JobTracker运行在Master节点上，而在每个Slave节点上，部署一个DataNode和TaskTracker。集群datanode之间并行地复制大文件可通过distcp命令。

![](https://img-blog.csdn.net/20160427165239105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




从这个体系上看，主要是Client和Master之间通信来实现远程和多用户管理。

2、Hadoop客户端安装和配置

1）Centos操作系统下，客户端Client用户名为A，服务器Master用户名为[M，复制服务器的Hadoop安装包到客户端。](http://lib.csdn.net/base/20)

2）系统/etc/profile配置hadoop的home路径；

3）Hadoop目录下hadoop-env.sh配置jdk路径；

4）Hadoop目录下core-site.xml配置集群namenode路径；

   <property>

<name>fs.default.name</name>

<value>hdfs://Master:8020</value>

</property>

5）Hadoop目录下mapred-site.xml配置集群jobtracker路径；

<property>

<name>mapred.job.tracker</name>

<value>[Master:8021](http://master:8021/)</value>

</property>

Hadoop客户端已完成配置，如Master服务器已设置好用户A，则可以运行Hadoop命令，如向hdfs系统上传文件。

现在的关键是Master服务器如何用M管理多用户权限，使客户端Client的用户A具有执行Master命令的权限。有必要先了解Linux的ACL机制。

**3、ACL机制**

ACL(AccessControl List) 访问控制列表，可置特定用户或用户组对于一个文件/目录的操作权限，包包括owner、group、others的read、write、execute权限。主要两个命令是：getfacl查看文件/目录的ACL设定内容和setfacl设置文件/目录的ACL内容。

1）$getfaclmy.cnf  //查看my.cnf的acl内容

*# file: my.cnf*

*# owner: root*

*# group: root*

*user::rw-*

*group::r--*

*other::r—*

通过执行$ls-l my.cnf比照更清晰理解：

*-rw-r--r-- 1 root root 943 3月 13 2015 my.cnf*

2）$setfacl-m o:rwx my.cnf //设置my.cnf文件other

*# file: my.cnf*

*# owner: root*

*# group: root*

*user::rw-*

*group::r--*

*other::rwx*

3）语法参考：setfacl[-bkRd] [{-m|-x} acl参数] 文件名

-m ：设置后续的acl参数

-x ：删除后续的acl参数

-b ：删除所有的acl设定参数

-R ：递归设置acl参数

-d ：设置预设的acl参数（只对目录有效，在该目录新建的文件也会使用此ACL默认值）

-k ：删除预设的acl参数

4）开启和关闭：

ACL默认是开启的，开启和关闭可通过mount -o acl实现。

查看当前文件系统的挂载选项：

# tune2fs -l/dev/sda1 | grep “mount option”

Defaultmount options: user_xattr acl

使用tune2fs开启和关闭acl的命令：

tune2fs -oacl /dev/sda1

tune2fs -o^acl /dev/sda1

**4、Hadoop访问控制**

Hadoop访问控制分为两级，其中Service Level Authorization为系统级，用于控制是否可以访问指定的服务，例如用户/组是否可以向集群提交Job，它是最基础的访问控制，优先于文件权限和mapred队列权限验证。Access Control on Job Queues在job调度策略层之上，控制mapred队列的权限。DFS Permmision用户控制文件权限。

![](https://img-blog.csdn.net/20160427165053493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




 Hadoop文件和目录权限管理集成ACL，基于POSIX（Portable Operating System Interface，可移植操作系统接口）模型，支持按用户、用户组、其他用户的读写执行控制权限。

Hadoop修改文件的权限、所有者及其所属组的基本命令如下：

hadoop fs –chmod //修改文件所有者、所属组、其他用户的读、写、执行权限； 

haddop fs –chown  //修改文件所有者

hadoop fs –chgrp   //修改文件所属组

用户组和用户名都是Linux本身管理，Hadoop只是管理特定Linux用户对hdfs文件系统的权限，实现不同客户端通过不同linux帐户远程Hadoop。客户端调用hadoop 的文件操作命令时，hadoop 识别出客户端执行命令所在进程的用户名和用户组（通过linux命令whoami识别用户名、通过groups 识别用户组，也有Kerberos、LDAP等身份识别方式），然后Master服务器检查赋予的该用户名和组的文件权限。

客户端Client启动hadoop进程远程Namenode时识别用户为A；Master服务器系统已有分配好hdfs文件权限的用户A，收到客户端请求后，检查A所执行命令的文件权限。

那么Hadoop具体如何分配用户权限及其所拥有的存储空间和计算能力。

**5、Hadoop多用户管理**

Master服务器的hadoop超级用户M（不是Linux操作系统的root），具有启动集群和执行所有命令的权限。那么M超级用户是如何管理hadoop普通用户的权限。

1）新建Master服务器Linux系统用户A、用户组A；

2）Hadoop创建用户A的目录

$hadoop fs –mkdir/user/A 

$hadoop fs –ls  /user/A  //查看该文件夹权限

3）赋予用户A拥有目录的权限

$hadoop fs –chownA:A /user/A

4）设定用户A目录的空间容量

$hadoopdfsadmin –setSpaceQuota 1t /user/A  //如设置为1T

此时客户端A用户启动hadoop远程Master即可执行相关hadoop命令，提交任务数据到HDFS的/user/A目录。实际上，简单的多用户权限管理并不需要ACL功能，只需要hadoop本身文件操作命令即可，将hadoop多用户权限配置转移成单纯的hdfs文件的用户权限配置。

那如何进一步管理多用户的文件权限和资源能力呢？

对于复杂的文件权限访问控制，如要用户组里的所有用户都具备用户组相应的权限，需要使用ACL权限管理机制。

1）Hadoop启用ACL

hdfs-site.xml设置：

<property>

<name>dfs.permissions.enabled</name>

<value>true</value>

</property>

<property>

<name>dfs.namenode.acls.enabled</name>

<value>true</value>

</property>

core-site.xml设置用户组默认权限：

<property>

<name>fs.permissions.umask-mode</name>

<value>022</value>

</property>

2）HadoopACL命令

$hdfs dfs -getfacl [-R] path

显示文件和目录的访问控制列表。如果一个目录有默认的ACL，getfacl也可以显示默认的ACL设置。

$hdfs dfs -setfacl [-R] [-b|-k -m|-x acl_spec path]|[--set acl_spec path]

设置文件和目录的ACL

-R: Use thisoption to recursively list ACLs for all files and directories.

-b: Revokeall permissions except the base ACLs for user, groups and others.

-k: Removethe default ACL.

-m: Add newpermissions to the ACL with this option. Does not affect existing permissions.

-x: Removeonly the ACL specified.

参考：

http://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-hdfs/HdfsPermissionsGuide.html

对于多用户的资源分配，Hadoop通过Fair Scheduler或者 Capacity Scheduler支持，而作业管理则通过ACL来满足。

1）资源管理：以用户组和用户为主体配置可用资源。Fair Scheduler将整个Yarn的可用资源划分成多个资源池，支持按照用户名配置所属的资源池。每个资源池中可以配置最小和最大的可用资源（内存和CPU）、最大可同时运行Application数量、权重、以及可以提交和管理Application的用户等。

2）作业管理：主要是作业提交权限控制、作业运行状态查看权限控制等。如限定可提交作业的用户、限定可查看作业运行状态的用户、限定普通用户只能修改自己作业的优先级、kill自己的作业、高级用户可以控制所有作业等。

**6、总结**

对于Hadoop多用户管理的机制，从用户创建、到文件管理、资源和作业管理，通过两天时间的学习，大致梳理了下。对于进一步深入掌握Hadoop相关机制，还有很漫长的路，需要通过实践不断总结。


Hadoop多用户管理最主要是基于linux的用户分组和用户基础，并应用ACL管理文件和作业访问权限。在具体作业和资源管理上，还未进一步试验练习。



