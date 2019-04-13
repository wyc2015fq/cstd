
# hadoop在ubuntu下的伪分布式安装配置 - 我和我追逐的梦~~~ - CSDN博客


2013年11月28日 19:07:05[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1056标签：[hadoop																](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)个人分类：[Hadoop																](https://blog.csdn.net/heyongluoyao8/article/category/1773173)


**本文转自：http://www.cnblogs.com/tippoint/archive/2012/10/23/2735532.html**
**一.安装ubuntu**
我是下载的ubuntu12.04,在64位的虚拟机上使用virtualbox安装的，没错，是在虚拟机上在安装虚拟机，然后安装ubuntu
具体安装过程就不描述了，不过安装过程确实慢，除非你的网络很快。12.04的新主题使用真是别扭，在登陆界面切换到gonme classic，这里可以使用windows共享，可以很方便的从硬盘或者其他机器拉东西回来。
**二. 在Ubuntu下创建hadoop用户组和用户;**
**1.**创建hadoop用户组;
|1
|sudo|addgroup hadoop
|

如图：
![](http://m1.img.libdd.com/farm4/247/A4F7CC07CA789BE5AC25E7952B8411F7_411_125.PNG)
**2.**创建hadoop用户;
|1
|sudo|adduser -ingroup hadoop hadoop
|

如图：
![](http://m2.img.libdd.com/farm4/242/C09A95E53257EBCCB22E5B5C4D649DF2_500_276.jpg)
**3.**给hadoop用户添加权限，打开/etc/sudoers文件;
|1
|sudo|gedit|/etc/sudoers
|

按回车键后就会打开/etc/sudoers文件了，给hadoop用户赋予root用户同样的权限。
在root   ALL=(ALL:ALL)   ALL下添加hadoop   ALL=(ALL:ALL)  ALL，
|1
|hadoop  ALL=(ALL:ALL) ALL
|

如图：
![](http://m3.img.libdd.com/farm4/90/7A4ACE8A99AE1F3DCA45BFEDE5CB985A_347_163.PNG)
**三. 在Ubuntu下安装JDK**
使用如下命令执行即可：
|1
|sudo|apt-get|install|openjdk-6-jre
|

如图：
![](http://m1.img.libdd.com/farm4/37/106AEB25E755F6FD72553E88A39EC625_500_272.jpg)
这里需要说明：我就是在这里没有太注意，后面费了点事情。
linux下的java有两种一个是openjdk一个sun的。因为一些原因，Sun Java 无法从 Ubuntu 的软件库里面取得
取而代之的是 OpenJDK，不过 OpenJDK 跑某些程式会出现问题所以还是需要 Sun Java
下面说说在 Ubuntu 12.04 安装 Sun Java 的方法：
（1）首先删除掉已经安装的openjdk
sudo apt-get purge openjdk*
(2)$ sudo apt-add-repository ppa:flexiondotorg/java
(3)$ sudo apt-get update 一定要执行这个步骤啊，如果更新比较慢，在unbuntu软件中心更新源，操作方面是编辑--源，使用自动测试寻找最快的源，我开始是使用的这个，给我找的一个源后来不稳定，我手工切换到科技大学的源（前面也是这个源稳定啊）
（4）$ sudo apt-get install sun-java6-jre sun-java6-jdk sun-java6-plugin
这一步骤大概花费一点时间，下载几十M的软件，我是在这步就执行成功的，如果执行不成功，还可以使用下面的办法：
$ sudo add-apt-repository "deb[
http://archive.canonical.com/ubuntu](http://archive.canonical.com/ubuntu)maverick partner"
$ sudo apt-get install python-software-properties
$ sudo apt-get update
$ sudo apt-get install sun-java*
$ sudo apt-get install openjdk-6-jdk openjdk-6-jdk gcj-4.6-jdk ecj default-jdk

更改：
apt-get purge openjdk*
apt-add-repository ppa:flexiondotorg/java
apt-get update
apt-get install sun-java6-jre sun-java6-jdk sun-java6-plugin
（5）配置JAVA环境变量:
sudo gedit /etc/environment
在其中添加如下两行：
JAVA_HOME=/usr/lib/jvm/java-6-sun
CLASSPATH=.:/usr/lib/jvm/java-6-sun/lib
（6）java -version查看不出错说明，成功。
我之前执行的是openjdk后来才切换的sun-java6,当然现在有7了。
**四. 修改机器名 （这步骤可以省略）**
每当ubuntu安装成功时，我们的机器名都默认为：ubuntu ，但为了以后集群中能够容易分辨各台服务器，需要给每台机器取个不同的名字。机器名由 /etc/hostname文件决定。
**1.**打开/etc/hostname文件;
|1
|sudo|gedit|/etc/hostname
|

**2.**将/etc/hostname文件中的ubuntu改为你想取的机器名。这里我取"dubin-ubuntu"。**重启系统后才会生效。**
**五. 安装ssh服务**
这里的ssh和三大框架:spring,struts,hibernate没有什么关系，ssh可以实现远程登录和管理，具体可以参考其他相关资料。
安装openssh-server;
|1
|sudo|apt-get|install|ssh|openssh-server
|

这时假设您已经安装好了ssh，您就可以进行第六步了哦~
**六、 建立ssh无密码登录本机**
**首先**要转换成hadoop用户，执行以下命令：
|1
|su|- hadoop
|

如图：
![](http://m3.img.libdd.com/farm4/32/CF23A8FFAF35FF4D2C241118CAC23920_438_90.PNG)
ssh生成密钥有rsa和dsa两种生成方式，默认情况下采用rsa方式。
**1.**创建ssh-key，，这里我们采用rsa方式;
|1
|ssh|-keygen -t rsa -P|""
|

如图：
![](http://m3.img.libdd.com/farm4/17/80D479F1992B9BE98BA476ECA5D0B611_500_333.jpg)
（注：回车后会在~/.ssh/下生成两个文件：id_rsa和id_rsa.pub这两个文件是成对出现的）
**2.**进入~/.ssh/目录下，将id_rsa.pub追加到authorized_keys授权文件中，开始是没有authorized_keys文件的;
|1
|2
|cd|~/.|ssh
|cat|id_rsa.pub >> authorized_keys
|

如图：
![](http://m3.img.libdd.com/farm4/172/442846248D642A775F7970A956DB4CAC_500_43.jpg)
（完成后就可以无密码登录本机了。）
**3.**登录localhost;
|1
|ssh|localhost
|

如图：
( 注：当ssh远程登录到其它机器后，现在你控制的是远程的机器，需要执行退出命令才能重新控制本地主机。)
**4.**执行退出命令;
|1
|exit
|

**七. 安装hadoop**
我们采用的hadoop版本是：hadoop-0.20.203（[http://www.apache.org/dyn/closer.cgi/hadoop/common/](http://www.apache.org/dyn/closer.cgi/hadoop/common/)），因为该版本比较稳定。最新的alpha版本已经很高，但是还是建议采用稳定的版本。我是在apache站点直接下载的。
**1.**假设hadoop-0.20.203.tar.gz在桌面，将它复制到安装目录 /usr/local/下；
|1
|sudo|cp|hadoop-0.20.203.0rc1.|tar|.gz|/usr/local/
|

**2.**解压hadoop-0.20.203.tar.gz；
|1
|2
|cd|/usr/local
|sudo|tar|-zxf hadoop-0.20.203.0rc1.|tar|.gz
|

**3.**将解压出的文件夹改名为hadoop;
|1
|sudo|mv|hadoop-0.20.203.0 hadoop
|

**4.**将该hadoop文件夹的属主用户设为hadoop，
|1
|sudo|chown|-R hadoop:hadoop hadoop
|

**5.**打开hadoop/conf/hadoop-env.sh文件;
|1
|sudo|gedit hadoop|/conf/hadoop-env|.sh
|

**6.**配置conf/hadoop-env.sh（找到\#export JAVA_HOME=...,去掉\#，然后加上本机jdk的路径）;
|1
|export JAVA_HOME=/usr/lib/jvm/java-6-openjdk
|

**7.**打开conf/core-site.xml文件;
|1
|sudo|gedit hadoop|/conf/core-site|.xml
|

编辑如下：property后面需要手工敲
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|<?|xml|version|=|"1.0"|?>
|<?|xml-stylesheet|type|=|"text/xsl"|href|=|"configuration.xsl"|?>
|<!-- Put site-specific property overrides in this file. -->
|<|configuration|>
|<|property|>
|<|name|>fs.default.name</|name|>
|<|value|>|[hdfs://localhost:9000](http://www.cnblogs.com/tippoint/admin/hdfs://localhost:9000/)|</|value|>
|</|property|>
|</|configuration|>
|

**8.**打开conf/mapred-site.xml文件;
|1
|sudo|gedit hadoop|/conf/mapred-site|.xml
|

编辑如下property后面需要手工敲：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|<?|xml|version|=|"1.0"|?>
|<?|xml-stylesheet|type|=|"text/xsl"|href|=|"configuration.xsl"|?>
|<!-- Put site-specific property overrides in this file. -->
|<|configuration|>
|<|property|>
|<|name|>mapred.job.tracker</|name|>
|<|value|>localhost:9001</|value|>
|</|property|>
|</|configuration|>
|

**9.**打开conf/hdfs-site.xml文件;
|1
|sudo|gedit hadoop|/conf/hdfs-site|.xml
|

编辑如下：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|<|configuration|>
|<|property|>
|<|name|>dfs.name.dir</|name|>
|<|value|>/usr/local/hadoop/datalog1,/usr/local/hadoop/datalog2</|value|>
|</|property|>
|<|property|>
|<|name|>dfs.data.dir</|name|>
|<|value|>/usr/local/hadoop/data1,/usr/local/hadoop/data2</|value|>
|</|property|>
|<|property|>
|<|name|>dfs.replication</|name|>
|<|value|>2</|value|>
|</|property|>
|</|configuration|>
|

**10.**打开conf/masters文件，添加作为secondarynamenode的主机名，作为单机版环境，这里只需填写**localhost**就Ok了。
|1
|sudo|gedit hadoop|/conf/masters
|

**11.**打开conf/slaves文件，添加作为slave的主机名，一行一个。作为单机版，这里也只需填写**localhost**就Ok了。
|1
|sudo|gedit hadoop|/conf/slaves
|

10.11我自己没敲，默认就是localhost，千万别修改为127.0.0.1 localhost虽然正式的多台机器是这样的写法，但是单台机器不需要前面的ip地址，否则会报错

**八. 在单机上运行hadoop**
**1.**进入hadoop目录下，格式化hdfs文件系统，初次运行hadoop时一定要有该操作，
|1
|2
|cd|/usr/local/hadoop/
|bin|/hadoop|namenode -|format
|

**2.**当你看到下图时，就说明你的hdfs文件系统格式化成功了。
![](http://m3.img.libdd.com/farm5/136/32EA7B6BD6013BAA9B160AEE5C084088_500_103.jpg)
**3.**启动bin/start-all.sh
|1
|bin|/start-all|.sh
|

**4.**检测hadoop是否启动成功
|1
|jps
|
**千万要注意这里，我使用这个命令只能看到jps自己的进程，纠结了很长时间，忽然发现得用 sudo jps果然能出现6个进程，坑爹啊，大家千万要注意。**
如果有Namenode，SecondaryNameNode，TaskTracker，DataNode，JobTracker五个进程，就说明你的hadoop单机版环境配置好了！
如下图：
![](http://m3.img.libdd.com/farm5/121/1F30C66942D136BA64589626F7FAC579_406_202.PNG)
然后可以通过firefox浏览器查看，使用[http://localhost:50030/](http://localhost:50030/)
对，你没有看错不是我们配置的core-site的9000的端口，也不是9001的端口，是50030，我也不知道为什么，有谁知道一下。如果看到下面激动人心的画面，就说明你成功了。


