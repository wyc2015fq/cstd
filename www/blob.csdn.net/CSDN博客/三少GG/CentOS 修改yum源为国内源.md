# CentOS 修改yum源为国内源 - 三少GG - CSDN博客
2013年04月22日 22:11:01[三少GG](https://me.csdn.net/scut1135)阅读数：8153

## CentOS 修改yum源为国内源
修改yum源的方法有两种，很多朋友喜欢修改yum.repos.d这个文件进行更改，deepvps感觉这个方法不是太好，以下这个方法就方便多了。
在CentOS系统中，可以直接通过yum来安装组件，但系统默认的yum源速度往往不尽人意，都连接海外的，为了在国内达到快速安装的目的，就要需要修改yum源，可以使用
http://mirrors.163.com/   http://mirrors.sohu.com 这2个国内源。
执行以下命令就可以了
> 
cd /etc/yum.repos.d
mv CentOS-Base.repo CentOS-Base.repo.bak
wget http://mirrors.163.com/.help/CentOS-Base-163.repo
或
wget http://mirrors.sohu.com/help/CentOS-Base-sohu.repo
运行yum makecache生成缓存
搞定。
# 更改centos yum 成中国镜像加快yum速度
163的开源镜像地址 [http://mirrors.163.com/.help/CentOS-Base-163.repo](http://mirrors.163.com/.help/CentOS-Base-163.repo)不同版本见[http://mirrors.163.com/.help/centos.html](http://mirrors.163.com/.help/CentOS-Base-163.repo)
sohu的开源镜像地址 [http://mirrors.sohu.com/help/CentOS-Base-sohu.repo](http://mirrors.sohu.com/help/CentOS-Base-sohu.repo)不同版本见[http://mirrors.sohu.com/help/centos.html](http://mirrors.sohu.com/help/centos.html)
中国科技大学 [http://centos.ustc.edu.cn/CentOS-Base.repo](http://centos.ustc.edu.cn/CentOS-Base.repo)**不同版本见 [http://lug.ustc.edu.cn/wiki/mirrors/help/centos](http://lug.ustc.edu.cn/wiki/mirrors/help/centos) (亲试ok)**
> 
如果使用上面YUM源的话,最好把里面的 mirrorlist 注释掉,否则系统会启动 fastesmirror 插件自动检查的,并不一定会使用这个yum源的.
**我用的是中国科技大学的速度不错。**
法一：直接下载源文件
> 
CentOS USTC mirror 这个镜像不错，大家更新可用这个
#yum -y install wget
#cd /etc/yum.repos.d
#mv CentOS-Base.repo  CentOS-Base.repo.save
#wget http://centos.ustc.edu.cn/CentOS-Base.repo
注意：如果为第一次安装的新系统的话，则需要先安装wget这个下载软件，不然没有办法下载ＣentOS-Base.repo这个文件的。
法二：手动创建源文件
更改yum镜像站点为中国站点地址,推荐[http://centos.ustc.edu.cn/centos/](http://centos.ustc.edu.cn/centos/)。(中国科技大学)
> 
#cd /etc/yum.repos.d/
#mv CentOS-Base.repo CentOS-Base.repo.bak
#vi CentOS-Base.repo
修改 /etc/yum.repos.d/CentOS-Base.repo 如下：
# Centos镜像使用帮助
## 收录架构
- 
i386
- 
x86_64
## 收录版本
- 
5
- 
6
## 使用说明
首先备份CentOS-Base.repo
mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup
下载对应版本的CentOS-Base.repo, 放入/etc/yum.repos.d/
这是CentOS 5的:
- [CentOS-Base.repo](http://lug.ustc.edu.cn/wiki/_export/code/mirrors/help/centos?codeblock=1)
# CentOS-Base.repo
#
# The mirror system uses the connecting IP address of the client and the
# update status of each mirror to pick mirrors that are updated to and
# geographically close to the client.  You should use this for CentOS updates
# unless you are manually picking other mirrors.
#
# If the mirrorlist= does not work for you, as a fall back you can try the 
# remarked out baseurl= line instead.
#
#
 
[base]
name=CentOS-$releasever - Base - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/os/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=os
gpgcheck=1
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-5
 
#released updates 
[updates]
name=CentOS-$releasever - Updates - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/updates/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=updates
gpgcheck=1
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-5
 
#additional packages that may be useful
[extras]
name=CentOS-$releasever - Extras - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/extras/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=extras
gpgcheck=1
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-5
 
#packages used/produced in the build but not released
[addons]
name=CentOS-$releasever - Addons - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/addons/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=addons
gpgcheck=1
gpgkey=http://mirror.centos.org/centos/RPM-GPG-KEY-CentOS-5
 
#additional packages that extend functionality of existing packages
[centosplus]
name=CentOS-$releasever - Plus - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/centosplus/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=centosplus
gpgcheck=1
enabled=0
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-5
 
#contrib - packages by Centos Users
[contrib]
name=CentOS-$releasever - Contrib - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/contrib/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=contrib
gpgcheck=1
enabled=0
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-5
这是CentOS 6的:
- [CentOS-Base.repo](http://lug.ustc.edu.cn/wiki/_export/code/mirrors/help/centos?codeblock=2)
# CentOS-Base.repo
#
# The mirror system uses the connecting IP address of the client and the
# update status of each mirror to pick mirrors that are updated to and
# geographically close to the client.  You should use this for CentOS updates
# unless you are manually picking other mirrors.
#
# If the mirrorlist= does not work for you, as a fall back you can try the 
# remarked out baseurl= line instead.
#
#
 
[base]
name=CentOS-$releasever - Base - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/os/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=os
gpgcheck=1
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-6
 
#released updates 
[updates]
name=CentOS-$releasever - Updates - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/updates/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=updates
gpgcheck=1
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-6
 
#additional packages that may be useful
[extras]
name=CentOS-$releasever - Extras - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/extras/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=extras
gpgcheck=1
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-6
 
#additional packages that extend functionality of existing packages
[centosplus]
name=CentOS-$releasever - Plus - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/centosplus/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=centosplus
gpgcheck=1
enabled=0
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-6
 
#contrib - packages by Centos Users
[contrib]
name=CentOS-$releasever - Contrib - mirrors.ustc.edu.cn
baseurl=http://mirrors.ustc.edu.cn/centos/$releasever/contrib/$basearch/
#mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=contrib
gpgcheck=1
enabled=0
gpgkey=http://mirrors.ustc.edu.cn/centos/RPM-GPG-KEY-CentOS-6
运行yum makecache生成缓存
