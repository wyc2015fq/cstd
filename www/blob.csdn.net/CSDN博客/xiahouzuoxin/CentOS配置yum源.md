# CentOS配置yum源 - xiahouzuoxin - CSDN博客





2014年02月20日 09:04:23[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：1182
个人分类：[Linux](https://blog.csdn.net/xiahouzuoxin/article/category/1286747)










网易（163）yum源是国内最好的yum源之一 ，无论是速度还是软件版本，都非常的不错，将yum源设置为163yum，可以提升软件包安装和更新的速度，同时避免一些常见软件版本无法找到。V9zz的云主机就将yum源设置为163的yum，具体设置方法如下：





1，进入yum源配置目录

cd /etc/yum.repos.d





2，备份系统自带的yum源

mv CentOS-Base.repo CentOS-Base.repo.bk

下载163网易的yum源：

wget [http://mirrors.163.com/.help/CentOS-Base-163.repo](http://mirrors.163.com/.help/CentOS-Base-163.repo)





3，更新玩yum源后，执行下边命令更新yum配置，使操作立即生效

yum makecache





4，除了网易之外，国内还有其他不错的yum源，比如中科大和搜狐的，大家可以根据自己需求下载

中科大的yum源：

wget [http://centos.ustc.edu.cn/CentOS-Base.repo](http://centos.ustc.edu.cn/CentOS-Base.repo)

sohu的yum源

wget [http://mirrors.sohu.com/help/CentOS-Base-sohu.repo](http://mirrors.sohu.com/help/CentOS-Base-sohu.repo)






