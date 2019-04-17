# CentOS下重新安装yum的方法 - 在思索中前行！ - CSDN博客





2014年09月24日 14:05:03[_Tham](https://me.csdn.net/txl16211)阅读数：8492








不小心误删除了VPS下面的yum，大家都知道yum在linux中是很重要的一个功能，软件的下载，系统的更新都要靠他。没有yum，系统基本处于半残废状态。

## yum的安装操作：




在SSH里面依次输入下面的命令：


1，下载最新的yum-3.2.28.tar.gz并解压

> 
wget http://yum.baseurl.org/download/3.2/yum-3.2.28.tar.gz

tar xvf yum-3.2.28.tar.gz




2，进入目录，运行安装

> 
cd yum-3.2.28

yummain.py install yum



结果提示错误： CRITICAL:yum.cli:Config Error: Error accessing file for config file:///etc/


后来经猫哥提醒，原来是缺少配置文件。在etc目录下面新建yum.conf文件，然后再次运行 yummain.py install yum，顺利完成安装。


3，最后更新系统，全部搞定，收工。^_^

> 
yum check-update

yum update

yum clean all


## 1．yum的安装与配置

（1）yum的安装

检查yum是否已经安装：

[root@localhost~]# rpm -qa|grep yum

如果没有任何显示，表示系统中还没有安装yum工具，yum安装包在centos系统光盘中可以找到，执行如下指令进行安装：

[root@localhost~]# rpm -ivh yum-*.noarch.rpm

安装yum需要python-elementtree、python-sqlite、urlgrabber、yumconf等软件包的支持，这些软件包在Centos Linux系统安装光盘均可找到，如果在安装yum过程中出现软件包之间的依赖性，只需按照依赖提示寻找相应软件包安装即可，直到yum包安装成功。

下面是某个环境的安装示例：

[root@localhost~]# rpm -ivh yum-2.4.3-1.c4.noarch.rpm

warning:yum-2.4.3-1.c4.noarch.rpm: V3 DSA signature:NOKEY, key ID 443e1821

error:Failed dependencies:

       python-elementtree is needed by yum-2.4.3-1.c4.noarch

       python-sqlite is needed by yum-2.4.3-1.c4.noarch

       urlgrabber is needed by yum-2.4.3-1.c4.noarch

       yumconf is needed by yum-2.4.3-1.c4.noarch

[root@localhost~]# rpm -ivh python-elementtree-1.2.6-4.2.1.i386.rpm

warning:python-elementtree-1.2.6-4.2.1.i386.rpm: V3 DSAsignature: NOKEY, key ID 443e1821

Preparing...               ########################################### [100%]

  1:python-elementtree     ###########################################[100%]

[http://www.uplook.cn/?a=index&m=index&c=iframe&url=mailto%3Aroot%40localhost](http://www.uplook.cn/?a=index&m=index&c=iframe&url=mailto%3Aroot%40localhost) ~]#rpm
 -ivh sqlite-3.3.3-1.2.i386.rpm       

warning:sqlite-3.3.3-1.2.i386.rpm: V3 DSA signature:NOKEY, key ID 443e1821

Preparing...               ########################################### [100%]

  1:sqlite                ########################################### [100%]

[root@localhost~]# rpm -ivh python-sqlite-1.1.7-1.2.i386.rpm

warning:python-sqlite-1.1.7-1.2.i386.rpm: V3 DSAsignature: NOKEY, key ID 443e1821

Preparing...               ########################################### [100%]

  1:python-sqlite         ########################################### [100%]

[root@localhost~]# rpm -ivh python-urlgrabber-2.9.8-2.noarch.rpm

warning:python-urlgrabber-2.9.8-2.noarch.rpm: V3 DSAsignature: NOKEY, key ID 443e1821

Preparing...               ########################################### [100%]

  1:python-urlgrabber     ########################################### [100%]

[http://www.uplook.cn/?a=index&m=index&c=iframe&url=mailto%3Aroot%40localhost](http://www.uplook.cn/?a=index&m=index&c=iframe&url=mailto%3Aroot%40localhost) ~]#rpm
 -ivh centos-yumconf-4-4.5.noarch.rpm

warning:centos-yumconf-4-4.5.noarch.rpm: V3 DSA signature: NOKEY, key ID 443e1821

Preparing...               ########################################### [100%]

  1:centos-yumconf        ########################################### [100%]

[root@localhost~]# rpm -ivh yum-2.4.3-1.c4.noarch.rpm

warning:yum-2.4.3-1.c4.noarch.rpm: V3 DSA signature:NOKEY, key ID 443e1821

Preparing...               ########################################### [100%]

  1:yum                   ########################################### [100%]

（2）yum的配置

yum工具安装完毕，接下来的工作是进行yum的配置，yum的配置文件有主配置文件/etc/yum.conf、资源库配置目录/etc/yum.repos.d，yum安装后，默认的一些资源库配置可能无法使用，因此需要进行修改，下面是/etc/yum.repos.d/CentOS-Base.repo资源库配置文件各项的详细含义：

[root@localhost~]#/etc/yum.repos.d/CentOS-Base.repo

[base]

name=CentOS-4- Base

#name就是发行版的名称，其格式表示“操作系统名和释出版本”，“base”是表明此段寻找的是base包信息。

baseurl=http://mirror.centos.org/centos/4/os/$basearch/ 

#baseurl”表示yum在互联网上查找升级文件的URL地址。其中“$basearch”代表了系统的硬件构架如“i386”、“x86-64等，同时，yum在资源更新时，会检查baseurl/repodata/repomd.xml文件。 “repomd.xml”是一个索引文件，它的作用是提供了更新rpm包文件的下载信息和SHA校验值。“repomd.xml”包括了3个文件，分别为“other.xml.gz”、”filelists.xml.gz”和”primary.xml.gz”，表示的含义依次是“其它更新包列表”、“更新文件集中列表”和“主要更新包列表”。

gpgcheck=1 

#gpgcheck表示是否启用gpg检查,1表示启用，0表示不启用校验，如果启用，就是需要在配置文件里注明GPG-RPM-KEY的位置，可以看到下面gpgkey字段，指定了GPG-RPM-KEY验证文件的位置。

#releasedupdates

[update]

#本段是updates更新模块要用到的部分配置。

name=CentOS-4- Updates

baseurl=http://mirror.centos.org/centos/4/updates/$basearch/

gpgcheck=1

gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-centos4

#gpgkey指定GPG密钥的地址



#packagesused/produced in the build but not released

#本段指定的是尚未发布的软件包部分（addons）配置

[addons]

name=CentOS-4- Addons

baseurl=http://mirror.centos.org/centos/4/addons/$basearch/

gpgcheck=1

gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-centos4

#additionalpackages that may be useful

#本段指定的是有用的额外软件包部分（extras）配置

[extras]

name=CentOS-4- Extras

baseurl=http://mirror.centos.org/centos/4/extras/$basearch/

gpgcheck=1

gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-centos4

#additionalpackages that extend functionality of existing packages

#本段指定的是扩展的额外软件包部分（centosplus）配置

[centosplus]

name=CentOS-4- Plus

baseurl=http://mirror.centos.org/centos/4/centosplus/$basearch/

gpgcheck=1

enabled=0

gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-centos4

#contrib- packages by Centos Users

#这里是contrib部分

[contrib]

name=CentOS-4- Contrib

baseurl=http://mirror.centos.org/centos/4/contrib/$basearch/

gpgcheck=1

enabled=0

gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-centos4




