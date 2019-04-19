# 【一键Hadoop】 开源项目EasyHadoop - 三少GG - CSDN博客
2013年04月21日 21:52:58[三少GG](https://me.csdn.net/scut1135)阅读数：4903
类似【一键OpenStack】，介绍个好的快速部署Hadoop工具：
和phpHiveAdmin一样，**EasyHadoop也是个为懒惰而生的项目。**
**简而言之，就是在一个网页上对Hadoop集群进行安装，重启，修改配置文件，查看log等功能。**
后续还会加入hive，hbase，pig之类的。
当然，**Cloudera Manager 是个不错的选择，不过一旦集群数量超过50，当然仍然可以使用hadoop，但是那个好用的界面会提示你购买许可协议。**所以，基于这一点，为了打破商业公司的垄断，为开源做一点微薄的贡献。更是为自己部署集群的时候可以更快捷方便，业余时间在家花了3周左右时间开发了这么一个东西。软件本身完全开源，遵循GPLv3协议。
EasyHadoop 是一个[Hadoop](http://www.oschina.net/p/hadoop)一键安装系统,方便大家更容易安装部署Hadoop软件。
EasyHadoop 源自暴风影音Hadoop数据平台项目而来,由前暴风影音数据团队[修湘](http://blog.csdn.net/azhao_dn)调研,[向磊](http://slaytanic.blog.51cto.com/)编写。
![](http://static.oschina.net/uploads/space/2012/0830/090100_nRYu_12.jpg)
### 本着从来源中来,到开源中去,并回馈开源社区的回馈思想。
开放此管理部署系统方便大家安装使用,以此和行业共同学习进步。
## 支持特性如下:
1.Haoop 稳定版本 hadoop-1.1.1-1
2.集成 JDK 6
3.集成LZO,hadoop-gpl-packaging-0.5.4-1
4.界面化一键安装部署管理hadoop集群。
5.支持64位 linux 操作系统. 特别是 redhat centos 5 6
6.五种秒级监控图表，实时查看节点服务器内存，硬盘，CPU，MR状态，负载状态，及时发现问题。
7.支持配置推送，hosts文件推送，自动生成机架感知脚本
## 使用方法:
http://github.com/xianglei/easyhadoop
点zip图标下载，然后运行EasyHadoopCentral中的install_centos(x).sh进行安装
**项目托管在 [github](http://github.com/xianglei/easyhadoop) 上面。**
------------------------------------------------------------------------------EH Agent-----------------------------------------------------------------------------------------
注意事项：
1. 务必关闭防火墙，否则连不上！
首先切换至root :  sudo -s
**/etc/rc.d/init.d/iptables stop  **
查看iptables状态：**service iptables status**
iptables开机自动启动：
开启： chkconfig iptables on 
关闭： chkconfig iptables off
iptables关闭服务：
开启： service iptables start 
关闭：sudo  **service iptables stop2.  **
将EasyHadoopAgent中的python脚本分别上传至每台待安装的
节点服务器的任意路径下，并依次执行下列命令：
------------
chmod +x NodeAgent.py
**sudo ./NodeAgent.py -s start -b node_ip**
------------
3.  查看hostname:
#hostname
状态：
1. iptables已经关闭；
2. 确认30050端口已经打开
#netstat -lnp|grep 30050
tcp    0   0 192.168.153.129:30050       0.0.0.0:*        LISTEN   2378/python
setsebool命令查
是selinux的相关设置项
这是SELinux的设置命令.
其实 man 一下setsebool也知道了:
NAME
setsebool – set SELinux boolean value
在不熟悉SELnux前，把SELinux关掉也可以的。有时间研究下SELinux。
Redhat从FC3开始，就使用了SELinux来增强安全，但是使用起来有时候太繁琐，就想关闭它，但是如果安装服务器的时候开始没有图形界面，还真很难找到地方可以关闭。
正确方法如下：修改/etc/selinux/config文件中的SELINUX=" " 为 disabled ，然后重启。
#### 配置selinux
selinux是一套linux的安全系统，它指定了应用程序可以访问的磁盘文件、网络端口等等。
如果装有selinux，那么默认的selinux会阻止memcache程序访问11211端口，所以必须对selinux进行配置才行。
方法1： 临时降低selinux运行级别，以便我们进行测试
命令：setenforce [Enforcing | Permissive]
Enforcing表示不允许违反策略的操作
Permissive表示允许违反策略的操作，但会记录下来
我们使用 setenforce Permissive
方法2： 修改selinux配置文件，关闭selinux
编辑 /etc/selinux/config 文件，将 SELINUX=enforcing 改为 SELINUX=disabled
方法3： 修改selinux的http策略，使得httpd进程可以访问网络，这样也就可以使用memcache了
命令：setsebool -P httpd_can_network_connect true
参数P的意思是保持设置的有效性，这样在重启之后这个设置依然有效，不会改变
#### 查看selinux状态
sestatus -bv
1. iptables已经关闭；
2. 确认30050端口已经打开
#netstat -lnp|grep 30050
tcp    0   0 192.168.153.129:30050       0.0.0.0:*        LISTEN   2378/python
**1.临时修改主机名**
显示主机名：
zhouhh@zzhh64:~$ hostname
zhh64
修改主机名：
zhouhh@zzhh64:~$ sudo hostname zzofs
zhouhh@zzhh64:~$ hostname
zzofs
看一下$PS1
zhouhh@zzhh64:~$ echo $PS1
/[/e]0;/u@/h: /w/a/]${debian_chroot:+($debian_chroot)}/u@/h:/w/$
@符号后面跟/h即主机名。
命令行前的提示符主机名怎么没有更新呢？
重新打开一个终端，就看到更新了。
zhouhh@zzofs:~$
**2.永久修改主机名**
以上的修改只是临时修改，重启后就恢复原样了。
redhat/centos上永久修改
[root@localhost ~]# cat /etc/sysconfig/network
NETWORKING=yes
HOSTNAME=localhost.localdomain
GATEWAY=192.168.10.1
修改network的HOSTNAME项。点前面是主机名，点后面是域名。没有点就是主机名。
[root@localhost ~]# vi /etc/sysconfig/network
NETWORKING=yes
NETWORKING_IPV6=no
HOSTNAME=gdbk
这个是永久修改，重启后生效。目前不知道怎么立即生效。
想立即生效，可以同时采用第一种方法。
----------------------------------------------------------------------------------EH C----------------------------------------------------------------------------------------
4. 文件会自动放置在web server上。即目录
 /var/www/html/
但需要修改
chmod +x NodeAgent.py
浏览器输入localhost/index.php即可
**5. 提示“Caught exception: TSocket: Could not connect to XXXX:30050 (Permission denied [13])”**
setsebool命令查
是selinux的相关设置项
这是SELinux的设置命令.
其实 man 一下setsebool也知道了:
NAME
setsebool – set SELinux boolean value
在不熟悉SELnux前，把SELinux关掉也可以的。有时间研究下SELinux。
Redhat从FC3开始，就使用了SELinux来增强安全，但是使用起来有时候太繁琐，就想关闭它，但是如果安装服务器的时候开始没有图形界面，还真很难找到地方可以关闭。
正确方法如下：修改/etc/selinux/config文件中的SELINUX=" " 为 disabled ，然后重启。
#### 配置selinux
selinux是一套linux的安全系统，它指定了应用程序可以访问的磁盘文件、网络端口等等。
如果装有selinux，那么默认的selinux会阻止memcache程序访问11211端口，所以必须对selinux进行配置才行。
方法1： 临时降低selinux运行级别，以便我们进行测试
命令：setenforce [Enforcing | Permissive]
Enforcing表示不允许违反策略的操作
Permissive表示允许违反策略的操作，但会记录下来
我们使用 setenforce Permissive
方法2： 修改selinux配置文件，关闭selinux
编辑 /etc/selinux/config 文件，将 SELINUX=enforcing 改为 SELINUX=disabled
方法3： 修改selinux的http策略，使得httpd进程可以访问网络，这样也就可以使用memcache了
命令：**setsebool -P httpd_can_network_connect true （尝试有效）**
参数P的意思是保持设置的有效性，这样在重启之后这个设置依然有效，不会改变
#### 查看selinux状态
sestatus -bv
# (13)Permission denied: proxy: HTTP: attempt to connect to 127.0.0.1:8080 (localhost) failed
This error is not really about file permissions or anything like that. What it actually means is that httpd has been denied permission to connect to that
 IP address and port.
The most common cause of this is SELinux not permitting httpd to make network connections.
To resolve it, you need to change an SELinux boolean value (which will automatically persist across reboots). You may also want to restart httpd to reset
 the proxy worker, although this isn't strictly required.
# setsebool -P httpd_can_network_connect 1
For more information on how SELinux can affect httpd, read the httpd_selinux man page.
6.  case reboot
**sudo service httpd start**
**sudo service mysqld start**
必备步骤：
**1. 将hostname信息推送至各节点 EHC settings--->/etc/hosts/  --->push**
**2.  hostname不可使用下划线"_" 而要用连接线"-" !否则会报错UnknownHostException....**
**修改/etc/sysconfig/network中HOSTNAME的值为自己指定的主机名，保证localhost在/etc/hosts文件中映射为正确的IP地址**
**-----------------------知识点-----------------------------------------------------------------------------------------------------------------------------------**
##  hosts文件与主机名修改无关
**一些网络文章中提出修改主机名还需修改Hosts文件，其实****hosts文件和主机名修改无关。**
cat /etc/hosts
127.0.0.1       localhost
192.168.11.116  zhh64
192.168.12.14 centdev
# The following lines are desirable for IPv6 capable hosts
::1     localhost ip6-localhost ip6-loopback
fe00::0 ip6-localnet
ff00::0 ip6-mcastprefix
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters
ff02::3 ip6-allhosts
**hosts文件是配本地主机名/域名解析的。**
如我本机ip是192.168.11.116名字是zhh64.就可以直接访问主机名。
zhouhh@zhh64:~$ ping zhh64
PING zhh64 (192.168.11.116) 56(84) bytes of data.
64 bytes from zhh64 (192.168.11.116): icmp_seq=1 ttl=64 time=0.077 ms
zhouhh@zhh64:~$ ping centdev
PING centdev (192.168.12.14) 56(84) bytes of data.
64 bytes from centdev (192.168.12.14): icmp_seq=1 ttl=63 time=0.726 ms
如果是小型局域网，就可以将hosts文件机器配全了，拷贝到每个机器，然后在ssh访问时用主机名直接访问。
zhouhh@zhh64:~$ ssh centdev
zhouhh@centdev's password: 
Last login: Wed Feb  3 10:03:09 2010 from 192.168.11.116
[zhouhh@centdev ~]$ 
[zhouhh@centdev ~]$ ifconfig eth0 | grep inet
          inet addr:192.168.12.14  Bcast:192.168.12.255  Mask:255.255.255.0
          inet6 addr: fe80::21e:c9ff:fe57:2575/64 Scope:Link
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------
# [EasyHadoop作者向磊谈Hadoop平民化](http://www.infoq.com/cn/news/2012/11/easyhadoop)
作者 [水羽哲](http://www.infoq.com/cn/author/%E6%B0%B4%E7%BE%BD%E5%93%B2)发布于 2012年11月14日
- 领域 
[运维 & 基础架构](http://www.infoq.com/cn/operations-infrastructure), [语言 & 开发](http://www.infoq.com/cn/development)- 主题 
[Hadoop](http://www.infoq.com/cn/hadoop) , [大数据](http://www.infoq.com/cn/bigdata) , [阿里巴巴](http://www.infoq.com/cn/alibaba) , [云计算](http://www.infoq.com/cn/cloud-computing) , [数据库](http://www.infoq.com/cn/Database) , [阿里云](http://www.infoq.com/cn/aliyun) , [架构](http://www.infoq.com/cn/architecture)
[新浪微博](http://www.infoq.com/cn/news/2012/11/easyhadoop#)[腾讯微博](http://www.infoq.com/cn/news/2012/11/easyhadoop#)[豆瓣网](http://www.infoq.com/cn/news/2012/11/easyhadoop#)[Twitter](http://www.infoq.com/cn/news/2012/11/easyhadoop#)[Facebook](http://www.infoq.com/cn/news/2012/11/easyhadoop#)[linkedin](http://www.infoq.com/cn/news/2012/11/easyhadoop#)[邮件分享](http://www.infoq.com/cn/news/2012/11/easyhadoop#)更多[4](http://www.infoq.com/cn/news/2012/11/easyhadoop#)
![](http://www.infoq.com/styles/cn/i/bookmarkthis.png)
随着[Hadoop](http://hadoop.apache.org/)的推出，大数据处理实现了技术上的落地。但是对于一般的公司和开发者而言，Hadoop依旧是一个陌生或者难以使用的技术。这次的阿里云开发者大会上，[向磊](http://weibo.com/slaytanic)凭借EasyHadoop将Hadoop平民化而获得了最佳创意奖，InfoQ对他进行了采访。
**InfoQ： 首先请您做下自我介绍并介绍本次的参赛作品EasyHadoop?**
> 
向磊：大家好，我是向磊，本次阿里云开发者大赛最佳创意奖作品[EasyHadoop](http://www.easyhadoop.com/)和[phpHiveAdmin](http://www.phphiveadmin.net/)的作者。目前供职于一家互联网视频公司的数据部门，负责整个Hadoop集群的维护和一些数据分析产品的研发工作。
本次参赛的EasyHadoop和phpHiveAdmin是我在业余时间编写的开源软件，其中EasyHadoop是界面化安装部署和管理Hadoop集群用的，phpHiveAdmin是用来管理和查询Hive数据仓库用的。如果打个类比的话，EasyHadoop类似于[Cloudera Manager](http://www.cloudera.com/)，phpHiveAdmin类似于HUE里面的[Beeswax](https://ccp.cloudera.com/display/CDHDOC/Beeswax)。主要是希望能够帮助到一些需要用到Hadoop做数据分析，又苦于Hadoop技术门槛太高的公司能够快速应用Hadoop。
**InfoQ： 目前国内公司对Hadoop接受程度是怎样的？**
> 
向磊：个人看法吧，国内有一些大的互联网公司比较早就已经开始用Hadoop分析业务数据，但是很多中小企业或者国企央企还是处于观望状态。我觉得主要还是由于Hadoop的技术门槛比较高，推广起来具有一定难度，对中小企业来说，Hadoop的实施成本比较高。然后国企央企可能认为假如自己上了Hadoop，维护和开发将是比较困难的事情。当前Hadoop的人才稀有，所以，国内很多人听说过Hadoop了，但是能够真正下决心在自己企业实施的还是少数。不过总的趋势还是好的，愿意尝试Hadoop的公司和个人越来越多了。主要我觉得是业务压力逼的，传统的关系型数据库做统计分析现在确实有些力不从心了。数据量越来越大，一个group
 by就内存溢出了。在量变已经无法满足需求的时候，就需要质变了。
**InfoQ：Hadoop虽然很火，但是能够操作起来的公司和技术人员却不是很多，那么你认为是什么阻碍了Hadoop的“平民化”？EasyHadoop又是如何帮助开发者来降低门槛的？**
> 
向磊：这个就需要说到我在大赛演示的时候说的Hadoop的三高问题(不是血糖高血脂高血压高，哈哈，这三高由我来替大家承担)。
第一是硬件投入高，Hadoop是集群存储和计算，这就意味着无法由一台服务器完成，搭建集群的话，一台高配服务器大概需要2-4万左右。对于中小企业来说，哪怕搭建3-4台集群都是很大的成本压力。不过很高兴这次看到阿里云提供的云主机，可以大幅度降低硬件的成本投入，适合Hadoop用的主机用一年才几千块钱，10台下来一年才几万块钱，这可以说个人都是可以承受的了。
第二是技术门槛高，能够安装部署运维Hadoop集群的人极其稀少，因为分布式计算毕竟需要数台到数十台服务器的协同运作，这时故障就变成常态了。给我很深刻印象的是阿里云的唐洪博士做的报告，单台服务器的硬盘故障率是4%，但是当你是一个集群，有5000台服务器的时候，硬盘故障率就是99.6%了。而且服务器之间网络通信，任务调度都会出问题，这些问题对维护人员来说都提出了相当高的要求。不过，其实最早做EasyHadoop和phpHiveAdmin是为了降低自己工作中的重复劳动，因为Hadoop的集群部署和安装配置乃至使用都是非常繁琐且容易出错的。所以为了省事，写了这样一个东西，从一个安装脚本开始，后来慢慢越写越大。最后整理一下功能就开源出来了。里面把安装脚本都封装起来了，用户不需要关心Hadoop安装的繁琐步骤。只要泡杯茶，点根烟，点点鼠标就好了，运维中的查看日志和重启节点也只需要动动手指就完成了。
第三是开发成本高，每个公司对自己的数据需求都是不同的，而能够开发Map/Reduce程序的人又少又贵，这就造成了大数据分析的人力成本很高。以前淘宝的报告提到，淘宝的数据90%分析是通过Hive仓库完成的，在我目前的公司，可能比例更高，95%是Hive完成的。但是[Hive](http://hive.apache.org/)本身是完全命令行操作的，除了技术人员可能都用不了。于是我就写了phpHiveAdmin用来做Hive数据仓库的管理和查询界面。这样，之前的mysql/oracle管理员可以不用开掉，留下来管理Hive仓库就好了。只要会写SQL就可以做大数据的分析了。并且我想提一个当初开发时候完全意料之外的改变，是管理流程上的变化。之前没有界面化，业务人员需要提交需求给技术人员。由技术人员编写HQL或者Map/Reduce，然后生成报表返回给业务人员，周期慢，中间对技术和业务人员对业务理解有偏差还要重新返工。写了phpHiveAdmin之后，业务人员只要简单培训一下HQL语言，很多数据他们可以自己去数据仓库里查询，而不需要通过技术人员了，节省了管理流程上的时间和成本。数据结果也更加直观。技术人员也能有更多的时间和精力去研发更多更重要的数据内容。这算是phpHiveAdmin为公司节省管理成本和数据研发成本做的一点贡献吧。
所以总结下来，云计算解决硬件成本问题，EasyHadoop解决技术门槛问题，phpHiveAdmin解决开发和管理成本高的问题，开源出来，算是对大家作出一点微不足道的帮助吧。
**InfoQ：目前已经进入到了“大数据的时代”，比如王坚博士说阿里云是一家以“数据为中心的云计算服务公司”已经印证了这一点，那么您认为“大数据”带来的机遇和挑战分别是什么？开发者如何应该如何应对？**
> 
向磊：王坚博士演讲的时候我在场外展台，没有听到，但是我听说他的演讲中也提到了EasyHadoop，令我感到十分的荣幸。我十分同意王坚博士的观点，阿里系一直以来都是数据推动业务，才能获得今天雄霸整个中国电子商务市场和云计算市场的成就。关于大数据，我个人的看法是，大数据带来的也是大机遇和大挑战，不仅仅是技术层面上的，更是管理者眼界的问题。你打算靠什么来指导你自身的业务发展，是开所谓Brain Storm的拍脑袋会，还是用数据来证明，这是管理者需要改变思路的地方。
而作为开发者，也需要改变一些思维方式，从传统的单机思维，扩展到集群思维，大数据时代需要的是能够提出更快更好更创新的概念并实现。Hadoop和大数据我认为给我的一个启示是，个人英雄主义的单枪匹马作战方式将不再适用。集体和合作的力量将远大于个人的力量，用中国俗话来说，就是“三个臭皮匠，顶个诸葛亮”。对于技术人员来说也是这样，一个人再强也不是强，团队强才是真的强。更多分享和更多交流，是开发者需要加入到自己的时间表里的，而不仅仅是提高自己的技术和算法水平。团队协作不会降低你的社会地位和收入。
**InfoQ： 在开发的过程中，你用到了阿里云的哪些服务？请列举这些服务的优点和需要改进的地方？**
> 
向磊：主要是用到了阿里云的ECS服务器，对Hadoop来说这就够了。优点嘛，就是执行速度快，内网间通信也非常快。需要改进的么，我了解了一下，阿里云为了保证数据安全，每个云主机里的数据都是备份3份的。然后如果用上Hadoop，Hadoop本身又是备份呢3份，这样数据就变成9份了。不知道阿里云能否有一种服务，用户可以自己定义主机数据的备份份数的。不过我觉得这会比较困难，因为这是云计算的底层服务，可能会很难改。算是个希望吧。
**InfoQ： 你提到EasyHadoop是根据GPL协议进行开发的，主要贡献者是几个核心团队的人员，您认为这种开源方式有什么利弊？**
> 
向磊：GPL简单的说就是开源传染协议。我的想法是，我既然愿意开源出来，就意味这我愿意为需要的人作出一点贡献。而被提供帮助的人们不需要向我支付费用，只要把这种帮助一直延续下去就好了。“Pay it forward, not pay me”，目前代码贡献者主要是我个人，主要原因是语言和程序架构比较复杂，语言方面用到了php，shell，python三种。然后涉及操作系统的底层内容比较多，像异步调用，线程池，socket通信等等。然后趣游的程序员史东杰提交过phpHiveAdmin的HQL自动提示代码和其他修改意见。窝窝团的用户提交过phpHiveAdmin的HQL正则表达式bug问题，目前已经修复了。
利弊方面，有利的方面是能够让软件可持续发展下去，每个人的代码都会被公开出来。弊端就是不会得到商业的支持，也就是没有任何来自商业公司的赞助。我目前是这样看的。
**InfoQ：请你分享一下获奖的感受？**
> 
向磊：首先得感谢阿里云提供了这样一个比赛，能够和很多优秀的开发者面对面交流开发技巧。获奖感受一个成语总结就是受宠若惊，确实没有想到能拿到奖，因为毕竟参赛人数很多，每个人的作品又都独具匠心，非常有想法和特色，有些技术水平也非常高。比如三等奖的一个作者，北航的研三学生蔺波写的僵尸网络仿真就很厉害，网络仿真本身就是一个比较偏技术和前沿的东西，很难想象是一个学生的作品。
**InfoQ： EasyHadoop下一步的发展计划是什么？**
> 
向磊：首先来说还是会继续完善整个产品，继续简化用户操作，增加功能，增强监控，修正bug，提高代码的安全性和强壮性。我还有很多想法，慢慢会逐步实现出来。然后也会对其他Hadoop生态圈的周边软件进行界面化封装，比如Pig，Mahout等等。大家在大会上都看到了phpHiveAdmin的Logo，当时因为时间关系没有讲。在这里我也想借这个logo的诞生贫两句，这个Logo是我一个发小帮我设计的。当时我告诉了他三个关键词：蜜蜂，大象，和简单。大家都知道，Hive的标志是蜜蜂，Hadoop的标志是大象，phpHiveAdmin的标志就是简单。所以就诞生了这个又像蜜蜂，又像大象，又很抽象的类似于任天堂红白机感觉的Logo，我觉得他很好的诠释了我对这整个系列产品的概念，就是要让大家像打红白机一样简单的进入大数据的殿堂。只要你会用Mysql，你就可以用Hive，只要你会点鼠标，你就可以用Hadoop。极简是EasyHadoop和整个系列产品的发展目标和方向，这也是我为什么当初决定用php来开发的理由。
感谢阿里云，感谢InfoQ，感谢谷歌，感谢Doug Cutting，以及感谢为大数据作出贡献的每一个人。
