# RedHat 6.0如何使用CentOS YUM源更新的方法 - weixin_33985507的博客 - CSDN博客
2012年02月24日 09:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
- 6.0
- 5.4
原文地址：[http://www.linuxde.net/2011/10/1578.html](http://www.linuxde.net/2011/10/1578.html)
2011 年 10 月 31 日 by 天明 | 2 条评论 | 1,267阅
由于RedHat的yum在线更新是收费的，如果没有注册的话是不能使用的，即不能在线安装软件。在这种情况下，想使用Redhat系统，还想用yum源来在线安装软件，有没有办法？
答案是有办法，请往下看！
1、删除redhat原有的yum源
# rpm -aq | grep yum|xargs rpm -e --nodeps
2、下载新的yum安装包 
这里我们使用CentOS的yum源
#wget http://mirror.centos.org/centos-6/6/os/i386/Packages/python-iniparse-0.3.1-2.1.el6.noarch.rpm
# wget http://mirror.centos.org/centos-6/6/os/i386/Packages/yum-metadata-parser-1.1.2-16.el6.i686.rpm
# wget http://mirror.centos.org/centos-6/6/os/i386/Packages/yum-3.2.29-22.el6.centos.noarch.rpm
# wget http://mirror.centos.org/centos-6/6/os/i386/Packages/yum-plugin-fastestmirror-1.1.30-10.el6.noarch.rpm
3、安装yum软件包
# rpm -ivh  python-iniparse-0.3.1-2.1.el6.noarch.rpm
# rpm -ivh  yum-metadata-parser-1.1.2-14.1.el6.i686.rpm
# rpm -ivh  yum-3.2.27-14.el6.centos.noarch.rpm  yum-plugin-fastestmirror-1.1.26-11.el6.noarch.rpm
注意：最后两个安装包要放在一起同时安装，否则会提示相互依赖，安装失败。
4、更改yum源  #我们使用网易的CentOS镜像源
# cd /etc/yum.repos.d/
# wget  http://mirrors.163.com/.help/CentOS6-Base-163.repo
# vi CentOS6-Base-163.repo
编辑文件，把文件里面的$releasever全部替换为版本号，即6 最后保存！或者直接把下面的内存拷贝到CentOS6-Base-163.repo文件中即可（已经修改好）
#########################################################################
# CentOS-Base.repo
#
# The mirror system uses the connecting IP address of the client and the
# update status of each mirror to pick mirrors that are updated to and
# geographically close to the client. You should use this for CentOS updates
# unless you are manually picking other mirrors.
#
# If the mirrorlist= does not work for you, as a fall back you can try the
# remarked out baseurl= line instead.
#
#
[base]
name=CentOS-6 - Base - 163.com
baseurl=http://mirrors.163.com/centos/6/os/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=6&arch=$basearch&repo=os
gpgcheck=1
gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-CentOS-6
#released updates
[updates]
name=CentOS-6 - Updates - 163.com
baseurl=http://mirrors.163.com/centos/6/updates/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=6&arch=$basearch&repo=updates
gpgcheck=1
gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-CentOS-6
#additional packages that may be useful
[extras]
name=CentOS-6 - Extras - 163.com
baseurl=http://mirrors.163.com/centos/6/extras/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=6&arch=$basearch&repo=extras
gpgcheck=1
gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-CentOS-6
#additional packages that extend functionality of existing packages
[centosplus]
name=CentOS-6 - Plus - 163.com
baseurl=http://mirrors.163.com/centos/6/centosplus/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=6&arch=$basearch&repo=centosplus
gpgcheck=1
enabled=0
gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-CentOS-6
#contrib - packages by Centos Users
[contrib]
name=CentOS-6 - Contrib - 163.com
baseurl=http://mirrors.163.com/centos/6/contrib/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=6&arch=$basearch&repo=contrib
gpgcheck=1
enabled=0
gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-CentOS-6
#########################################################################
5、清理yum缓存
# yum clean all
# yum makecache     #将服务器上的软件包信息缓存到本地,以提高搜索安装软件的速度
# yum install vim*    #测试域名是否可用
至此，Redhat6可以使用CentOS的yum源在线安装软件了！
[原文链接](http://www.cnblogs.com/xiaochaohuashengmi/archive/2011/10/10/2205310.html)
今天碰上要给一台装有Linux的机器做维护，于是就以为按照以往经验就能解决问题，但是神奇的发现机器上一堆的软件包都没有安装，
最要命的是连软件包管理器也都没有安装，本以为是FreeBSD，查了一下才发现是RedHat Linux Server 5.4，
于是开始着手安装YUM，在Google搜索了一下，发现貌似类似的情况不多，但是这确实不是我第一次遇到RedHat没有yum了，
因为上次遇到的时候是别人让帮忙看看，解决不了也就没留心，但这次维护是必要的，于是尝试了许久，
这大概是我最不舒服的一次经历了，而且RedHat很多资源都不多，于是基本rpm包都是借助了CentOS 5，于是对RedHat充满了怨念。
1.首先需要确定系统架构
确认是x86_64之后，在网易的mirrors下载需要的rpm包：（如果是i386的话则需要到http://mirrors.163.com/centos/5/os/i386/CentOS/ 找到对应的软件包）
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
wget http://mirrors.163.com/centos/5/os/x86_64/CentOS/m2crypto-0.16-8.el5.x86_64.rpm
wget http://mirrors.163.com/centos/5/os/x86_64/CentOS/python-elementtree-1.2.6-5.x86_64.rpm
wget http://mirrors.163.com/centos/5/os/x86_64/CentOS/python-iniparse-0.2.3-4.el5.noarch.rpm
wget http://mirrors.163.com/centos/5/os/x86_64/CentOS/python-sqlite-1.1.7-1.2.1.x86_64.rpm
wget http://mirrors.163.com/centos/5/os/x86_64/CentOS/python-urlgrabber-3.1.0-6.el5.noarch.rpm
wget http://mirrors.163.com/centos/5/os/x86_64/CentOS/yum-3.2.22-37.el5.centos.noarch.rpm
wget http://mirrors.163.com/centos/5/os/x86_64/CentOS/yum-fastestmirror-1.1.16-16.el5.centos.noarch.rpm
wget http://mirrors.163.com/centos/5/os/x86_64/CentOS/yum-metadata-parser-1.1.2-3.el5.centos.x86_64.rpm 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
2.然后 安装(升级)软件包，并导入证书
```
#rpm -Uvh *
#rpm -import http://mirrors.163.com/centos/RPM-GPG-KEY-CentOS-5
```
3.让yum使用网易的源：
```
#cd /etc/yum.repos.d/
#wget http://mirrors.163.com/.help/CentOS-Base-163.repo
```
4.使用
```
#vi CentOS-Base-163.repo
```
在所有mirrorlist前面加上#，把所有$releasever替换成5，保存
5.清理并重新生成yum缓存
```
#yum clean metadata
#yum makecache
```
6.于是yum就正常工作了，然后随手就更新一下，就OK了
```
#yum -y upgrade
```
完
