# centos配置yum本地源 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年07月07日 12:57:11[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2001








有时生产环境限制centos主机外联，这个时候要部署一些软件就比较麻烦，为利用yum机制，需要建立yum本地源，最好是建立ftp服务器作为源，可以在局域网内共享。






yum 主要功能是更方便的添加/删除/更新RPM 包，自动解决包的倚赖性问题，便于管理大量系统的更新问题。

#rpm-qa|grep yum //系统默认安装的yum

yum 的配置文件分为两部分：main 和repository

main 部分定义了全局配置选项，整个yum 配置文件应该只有一个main，一般位于/etc/yum.conf 中。repository 部分定义了每个源/服务器的具体配置，可以有一到多个，一般位于/etc/yum.repos.d目录下的各文件中。

#cat /etc/yum.conf

//可以设置yum源代理服务器，如proxy=http://IP:Port

# ls -l /etc/yum.repos.d

//默认文件：CentOS-Base.repo是yum 网络源的配置文件

//默认文件：CentOS-Media.repo是yum 本地源的配置文件

下面配置本地yum源：

第一步：yum源机禁用默认的网络源和本地源文件，将其改名

#mv /etc/yum.repos.d/CentOS-Base.repo/etc/yum.repos.d/CentOS-Base.repo.bak

# mv/etc/yum.repos.d/CentOS-Media.repo /etc/yum.repos.d/CentOS-Media.repo.bak

第二步：yum源机安装包挂载

把安装包文件mount挂载在/mnt，也可以建ftp服务器作为局域网内的yum源。

这里把jdk-8u73-linux-x64.rpm安装包挂载在/mnt下。

第三步：yum源机创建依赖关系：

#rpm-qa|grep createrepo //未安装createrepo

# rpm-ivh /mnt/createrepo-0.4.11-3.el5.noarch.rpm //下载并安装

下载地址：找到对应操作系统版本

http://rpmfind.net/linux/rpm2html/search.php?query=createrepo

//创建repodata/repomd.xml

#createrepo-v /mnt 

看到在/mnt目录下有repodata/repomd.xml文件

第四步：客户端配置yum源文件，使用yum源机的安装包

#vi /etc/yum.repos.d/local.repo

[local]

name=local

baseurl=file:///mnt/

gpgcheck=0

enable=1

如果是ftp的yum源，则将格式如：baseurl=ftp://username:password@ip/filedirectory/，用ftp服务器建立的yum源可以在局域网内使用。

第五步：安装

#yum clean//清除缓存

#yum repolist  //列出所配置的local信息

#yumsearch  //搜索安装包

#yum –y install  //安装




