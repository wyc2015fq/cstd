# centos6.2部署jdk+tomcat+mysql总结 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年03月18日 11:24:15[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2498
个人分类：[Linux](https://blog.csdn.net/fjssharpsword/article/category/6480442)









### 1.1 主机环境

1）操作系统：

VirtualBox+Centos-6.2-x86_64-minimal

下载地址：http://mirror.symnds.com/distributions/CentOS-vault/

2）网络设置：

——VB上设置虚拟机网络为NAT模式

——#ifconfig eth0 up //启用网卡    

#ifconfigeth0 down  //关闭网卡

——#vi /etc/sysconfig/network-scripts/ifcfg-eth0

配置：ONBOOT="yes"

增加：BOOTPROTO="dhcp"

——#reboot //重启

——#ping [www.163.com](http://www.163.com/) //可以访问

3）组件安装包获取方法：

方法一：#yum –y install 或$wget    

//安装wget，可执行wget下载安装包，适合联网情况下，但存在无法下载情况，如jdk需cookie验证。

//连外网可设置全局代理：未验证

#vi /etc/profile

export http_proxy= yourproxy:port

export ftp_proxy= yourproxy:port

//单独yum的代理设置修改/etc/yum.conf

//单独wget的代理设置修改/etc/wgetrc

方法二：FTP服务器架设，通过ftp上传和下载；

方法三：通过mount挂载，共享windows文件夹，适用内网部署。

Windows（IP：192.168.1.100）下共享文件夹tmp

Centos下执行：

#mount-t cifs //192.168.1.100/tmp /mnt -o username=xxx,password=xxx

  #df –h  //可以看到//192.168.1.100/tmp

 #cd /mnt/ //进入目录

### 1.2 JDK部署

1）首先查看系统是否已自带openjdk

#yum list installed |grep java

如有则卸载，#yum–y remove。

#yum –y list java*

查看yum库中的Java安装包

2）通过共享文件夹获取JDK安装包jdk-8u73-linux-x64.rpm

#mkdirsw  //在root目录下新建文件夹sw

#cp/mnt/jdk-8u73-linux-x64.rpm /root/sw/  //从挂载点复制到sw目录下

#rpm –ivh  jdk-8u73-linux-x64.rpm  //进入/root/sw目录安装

#ls/usr/java   

//显示defaultjdk1.8.0_73  latest，说明JAVA_HOME在/usr/java/ jdk1.8.0_73

3）#vi/etc/profile  //配置Java环境变量，i切换到编辑模式，esc切换命令模式:wq退出保存

exportJAVA_HOME=/usr/java/jdk1.8.0_73

exportPATH=$JAVA_HOME/bin:$PATH

exportCLASSPATH=.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib

#source/etc/profile   //文件修改生效

4）#java–version  //显示安装成功

### 1.3 tomcat部署

1）下载并解压

通过共享文件夹获取Tomcat安装包apache-tomcat-8.0.32.tar.gz

挂载到centos上

#mount –tcifs //192.168.1.100/tmp /mnt –o username=xxx,password=xxx

    移动到/mnt/sw目录下：

#cp/mnt/apache-tomcat-8.0.32.tar.gz /root/sw/

进入/root/sw目录下解压

#tar –xzvfapache-tomcat-8.0.32.tar.gz

移动到/usr/tomcat目录下

#cp-R apache-tomcat-8.0.32 /usr/tomcat

2）启动和关闭：

    #cd /usr/tomcat

#./bin/startup.sh  //启动 

#wget [http://localhost:8080](http://localhost:8080/)  //测试成功

#./bin/shutdown.sh//关闭

在linux下让tomcat在开机时自启动，可以将启动代码写到/etc/rc.local里面（参考：http://blog.csdn.net/fjssharpsword/article/details/7436898）。但是tomcat以root权限运行，是不安全的。下面步骤让tomcat以非特权身份作为daemon运行。

3）新增tomcat用户

新增tomcat用户，避免root用户安全风险

# useradd -s/sbin/nologin tomcat   //免登陆的用户

# chown -Rtomcat:tomcat /usr/tomcat

4）JSVC编译安装

    将tomcat作为linux的daemon运行，需要commons-daemon工程的jsvc工具，tomcat的bin目录里自带daemon源码。

#cd /usr/tomcat/bin

#tar –xzvf commons-daemon-native.tar.gz

#cdcommons-daemon-1.0.15-native-src/unix

#./configure --with-java=/usr/java/jdk1.8.0_73   

//如已配置JAVA_HOME则不需要with-java参数

//需先安装gcc，在线安装：$yum install gcc 

//可从http://mirror.hust.edu.cn/gnu/gcc/下载gcc-5.3.0.tar.gz编译安装，未验证

参考：http://www.bubuko.com/infodetail-1413659.html

#make 

//make得到jsvc文件，复制到tomcat的bin目录中。Jsvc是tomcat的守护进程管理工具。

#cp jsvc ../..  

#cd ../..   //回到上两层bin目录下

5）配置daemon.sh文件

daemon.sh脚本配置启动jsvc工具控制tomcat。脚本里面指定了TOMCAT_USER，jsvc将先以特权身份启动tomcat，在切换到指定的用户，使tomcat以非特权身份监听需要特权的端口。

#cd /usr/tomcat/bin

#vi daemon.sh

编辑daemon.sh，在开始增加一行，支持chkconfig，增加后如下：

#!/bin/sh

# chkconfig:- 80 20        

# Licensed to theApache Software Foundation (ASF) under one or more

在脚本注释后，执行命令前面增加下面：

JAVA_HOME=/usr/java/jdk1.8.0_73

CATALINA_HOME=/usr/tomcat

TOMCAT_USER=tomcat

修改ARG0的值为如下：

ARG0="/usr/tomcat"

6）增加到service自启动

# cp daemon.sh/etc/init.d/tomcatd

#cd /etc/init.d/

#chkconfig --addtomcatd

#chkconfig --level 2345tomcatd on   //tomcat在linux2、3、4、5启动级别自启动

#service tomcatd start  //启动

#wget [http://localhost:8080](http://localhost:8080/)   //reboot重启机子，成功

### 1.4 mysql部署

1）下载

通过共享文件夹获取mysq server安装包MySQL-server-5.6.29-1.el7.x86_64.rpm

挂载到centos上

#mount –tcifs //192.168.1.100/tmp /mnt –o username=xxx,password=xxx

    移动到/mnt/sw目录下：

#cp /mnt/ MySQL-server-5.6.29-1.el7.x86_64.rpm/root/sw/

2）安装依赖项

//缺失perl

#yun install perl



//缺失libstdc++.so.6查看安装libstdc++-4.4.7-16.el6.i686

#yum whatprovides libstdc++.so.6  

#yum install libstdc++-4.4.7-16.el6.x86_64



//缺失libc.so.6查看安装glibc-2.12-1.166.el6_7.7.i686

#yum whatprovides libc.so.6

# yum install glibc-2.12-1.166.el6_7.7.x86_64



//缺失libnuma.so.1查看安装numactl-2.0.9-2.el6.i686

#yumwhatprovides libnuma.so.1

# yuminstall numactl-2.0.9-2.el6.x86_64



总结：centos minimal版缺失太多组件，依赖项安装太费时间，ibstdc++.so.6和libc.so.6都已安装，但仍持续提示缺失，无解，后续再研究。另外对于mysql下载版本不要过高，如果rpm包无法安装，也可以考虑下载源码包便已安装，但应该也会受到依赖项制约。

3）安装mysql-server

//进入/root/sw目录安装

#rpm –ivh  MySQL-server-5.6.29-1.el7.x86_64.rpm 

//前面依赖项缺失libstdc++.so.6和libc.so.6，各种安装，一直无法通过，只能通过yum联网安装。

#yum installmysql-server

4）启动和执行

#service mysqldstart  //启动

# service mysqldstop  //关闭

#chkconfig mysqld on  //设置mysqld开机启动

#chkconfig –list  //查看

//开启3306端口并保存

# /sbin/iptables-I INPUT -p tcp --dport 3306 -j ACCEPT

#/etc/rc.d/init.d/iptablessave

//修改密码并设置远程访问，#mysql

mysql>use mysql;

mysql>update userset password=password('123456') where user='root';

mysql>flushprivileges;

//设置Mysql远程访问

mysql>grant allprivileges on *.* to 'root'@'%' identified by '123456' with grant option;

//解决Mysql乱码问题

#vi /etc/my.cnf

最后添加：default-character-set=utf8

//vi操作：按esc退出编辑模式（按i进入编辑模式），输入:wq保存退出，输入:q不保存退出。

#mysql –u root -p   //reboot后验证是否开机启动



