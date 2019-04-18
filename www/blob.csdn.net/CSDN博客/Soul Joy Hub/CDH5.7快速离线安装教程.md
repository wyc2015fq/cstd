# CDH5.7快速离线安装教程 - Soul Joy Hub - CSDN博客

2016年08月30日 19:45:18[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1357标签：[hadoop																[集群																[cdh																[cloudera](https://so.csdn.net/so/search/s.do?q=cloudera&t=blog)](https://so.csdn.net/so/search/s.do?q=cdh&t=blog)](https://so.csdn.net/so/search/s.do?q=集群&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)
个人分类：[大数据平台](https://blog.csdn.net/u011239443/article/category/6296513)


[http://www.6gdown.com/softedupage/60099.html](http://www.6gdown.com/softedupage/60099.html)

一、简介

CDH是cloudera公司开发的一个快速部署、高效管理Hadoop和其各种组件的一个商业化产品。主要分为两部分，分别为Cloudera Manager和CDH软件包。其中Cloudera Manager负责集群的部署与管理。CDH软件包囊括了hdaoop各类的组件的安装包，例如hive、hdfs、spark等等。

由于实验室服务器集群实现了硬件虚拟化，要在虚拟资源重新搭建CDH集群。cloudera的搭建官方提供了三种安装方式。分别是在线安装、yum安装和离线安装，我首先是采用离线安装的方式，这种方式也是目前大部分博客教程所采用的方式。但是照着这种方式我每次到最后安装服务的时候总是在部署配置文件的时候出错，提示错误如图所示：

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/212351JG-0.jpg)

猜测原因是权限问题，但是在网上寻找办法也一直没有解决。这里吐槽一下cloudera官方社区，没有几个人。因此舍弃到这种安装方式采用在线安装，但是在线安装需要耗费大量时间在软件包的下载上，不过我们可以手动下载安装，这样可以大大提高安装速度。

二、基础环境

软件环境

```
1.操作系统：Centos6.5
2.CDH软件包版本5.6、Cloudra Manager版本5.7
3.JDK版本oracle jdk1.7.0_67
```

硬件环境

```
9台虚拟机节点，硬件配置如下：
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/21235151O-1.jpg)

三、基础配置

以下所有操作均在root下进行

1.host配置

```
1)修改主机名，vim /etc/sysconfig/network,各台主机honstname改名为对应的名称，service network restart重启网卡生效。

2)添加hostname与ip的对应关系如下图所示：
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123514329-2.jpg)

```
3)将host从主节点master分发到各个从节点。：
scp /etc/hosts root@slave1:/etc
```

2.关闭防火墙和selinux

```
1）关闭防火墙(每个节点)
service iptables stop
chkconfig iptables off

2）关闭selinux(重启生效)
vim /etc/selinux/config
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123513926-3.jpg)

3.ssh无密码登录

```
1）各个节点安装ssh
ssh-keygen -t rsa 一路回车结束

2）将公钥加入到authorized_keys(只需master操作)
cat id_rsa.pub >authorized_keys

3) 修改权限
chmod 600 authorized_keys

4）将authorized_keys从master分发到各个slave
scp authorize_keys root@slave1:~/.ssh/
```

4.jdk安装

```
1）卸载自带java
rpm -qa |grep java
yum remove java*(删除自带的java)

2）安装jdk（每个节点rpm安装）
rpm -ivh jdk1.7.0_67.rpm

3）配置java环境（每个节点配置，当然可以一个节点配置完了使用scp分发）
在/etc/profile中加入：
export JAVA_HOME=/usr/java/jdk1.7.0_67
export CLASSPATH=.:$CLASSPTAH:$JAVA_HOME/lib
export PATH=$PATH:$JAVA_HOME/bin

4）使配置生效（每个节点）
source /etc/profile
```

5.ntp时间同步

```
1）安装NTP（每个节点）
yum install ntp

2）配置NTP
vim /etc/ntp.conf
master配置：(选用复旦大学ntp服务器)
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123512059-4.jpg)

```
slave配置：（同步master）
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123515459-5.jpg)

```
3）开启NTP服务
service ntpd start
chkconfig ntpd off

4）查看同步效果
命令：ntpstat
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123513291-6.jpg)

四、Cloudera Manager安装

1.下载rpm安装包

```
rpm安装包下载地址：其中jdk如果自己安装好了可以不用下载。
```

http://archive.cloudera.com/cm5/redhat/6/x86_64/cm/5.7/RPMS/x86_64/

```
包含软件：
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/212351Mb-7.jpg)

2.master节点安装

```
将下载好的rpm包放到一个文件夹中，任意命名，进入到这个文件夹手动安装：
yum localinstall --nogpgcheck *.rpm
使用yum安装会同时安装相关的依赖，非常方便
如果要卸载使用
yum --setopt=tsflags=noscripts remove xxxx
```

3.slave节点安装

slave中不需要安装server的包，只需要安装cloudera-manager-agent.rpm和cloudera-manager-daemons.rpm。先将两个rpm包拷贝到slave节点上，剩下安装方法和master一样。

4.安装cloduera manager二进制安装包

```
1)wget http://archive.cloudera.com/cm5/installer/latest/cloudera-manager-installer.bin

2)chmod u+x cloudera-manager-installer.bin

3)./cloudera-manager-installer.bin

4)根据安装向导一路next。注意，如果之前master上没有手动安装rpm包此时就会联网下载，下载速度一般都较慢，太费时间。

5)安装结束以后会提示登录7180端口。
```

五、CDH服务安装

1.制作本地parcel

```
1）下载CDH软件包，下载地址：
```

http://archive.cloudera.com/cdh5/parcels/5.6/

```
下载对应版本的CDH:下载图中标红的三个资源（el6代表centos6）
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/212351E94-8.jpg)

```
之前完成CM安装之后master节点会在/opt目录下生成cloudera文件夹，将刚才下载的三个文件移动到parcel-repo文件夹中并将
CDH-5.6.0-1.cdh5.6.0.p0.45-el6.parcel.sha1更名为
CDH-5.6.0-1.cdh5.6.0.p0.45-el6.parcel.sha如不更名会在线重新下载。
```

2.配置软件

```
1）登录7180端口:http://master:7180
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123513536-9.jpg)

```
初始用户名与密码均为admin

2）同意协议一路continue
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123512W6-10.jpg)

```
输入集群中各个主机名或者ip，可以用空格分隔，点击search，然后continue
```

3)选择parcel版本，由于我们下载的是CDH5.6，所以选择CDH5.6。这里可能没有CDH5.6的选项，这是由于我们是在cloudera-server开启之后才将CDH5.6的三个文件放入parcel-repo文件夹中，重启一下cloudera-scm-server就行了：

/etc/init.d/cloudera-scm-server restart

```
4）安装jdk，我们之前已经在每个节点都安装了jdk，所以这步可以跳过。

5）设置ssh登录，选择全部主机使用统一ssh密码，输入密码点击continue。
```

6)安装cloudera-manager-agent相关软件。由于我们之前在每个节点手动安装了相应的rpm包，所以这里很快就会完成。如果之前各个节点没有手动安装，这一步会在线下载rpm包，速度非常慢，并且如果错误还会中断。强烈建议不要使用在线下载。

大概十分钟左右安装完成(这里只有7个节点，第一次安装的时候我只安装了7个节点，剩下两个节点是后来添加到集群中的)，然后继续Continue。

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/21235160W-11.jpg)

```
7）主机检测：
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123514191-12.jpg)

```
会提示错误，解决办法
echo 0 >/proc/sys/vm/swappiness(临时生效)
echo never >/sys/kernel/mm/redhat_transparent_hugepage/defrag（临时生效）

重启永久生效：
编辑vim /etc/sysctl.conf
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123512W2-13.jpg)

```
编辑vim /etc/rc.local
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/21235130F-14.jpg)

```
8）安装parcel包
    接下来CM安装parcel包，图中提示host is in bad health，这个可以忽略，多等一会就会恢复正常。
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123512922-15.jpg)

```
9）安装服务
如没有特殊需求可以默认
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/2123515547-16.jpg)

```
10）一路continue完成安装
```

![CDH5.7快速离线安装教程](http://www.6gdown.com/uploads/allimg/1604/212351NC-17.jpg)

至此CDH集群就搭建完成了，前前后后花费了两个周末的时间才完成，过程中遇到了数不完的坑。感觉目前大部分博客教程都是较早之前的，特别是对于离线安装的方法我重复安装了好几次都出现了同样的问题，最后还是在不断尝试中总结出了这个即方便又快速的安装方法，希望对大家有所帮助。


