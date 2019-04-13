
# centos 安装与操作 - 阳光岛主 - CSDN博客

2013年09月30日 23:57:08[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：4024个人分类：[Linux/Shell																](https://blog.csdn.net/sunboy_2050/article/category/655460)



**CentOS 简介**
CentOS（Community Enterprise Operating System）是一个基于Red Hat Linux 提供的可自由使用源代码的企业级 Linux发行版本，它是来自于Red Hat Enterprise Linux依照开放源代码规定释出的源代码所编译而成。由于出自同样的源代码，因此有些要求高度稳定性的服务器以CentOS替代商业版的Red Hat Enterprise Linux使用，相对于其他Linux发行版，其稳定性值得信赖。两者的不同，在于CentOS并不包含封闭源代码软件，详见我先前的博客 Centos和Redhat的区别和联系

**CentOS 下载安装**
官方下载页面：[http://wiki.centos.org/Download](http://wiki.centos.org/Download)
i386镜像：[http://isoredirect.centos.org/centos/6/isos/i386/](http://isoredirect.centos.org/centos/6/isos/i386/)
x86-64镜像：[http://isoredirect.centos.org/centos/6/isos/x86_64/](http://isoredirect.centos.org/centos/6/isos/x86_64/)
CentOS 6.4下载：[http://mirrors.163.com/centos/6.4/isos/](http://mirrors.163.com/centos/6.4/isos/)（网易镜像）
CentOS帮助文档：[http://mirrors.163.com/.help/centos.html](http://mirrors.163.com/.help/centos.html)

**CentOS 查看版本**
\#lsb_release -a
LSB Version:    :core-3.1-ia32:core-3.1-noarch:graphics-3.1-ia32:graphics-3.1-noarch
Distributor ID: CentOS
Description:    CentOS release 5.4 (Final)
Release:        5.4
Codename:       Final
uname命令适用于所有的linux，包括Redhat、SuSE、Debian等发行版。
root@MyMail ~ \#uname
Linux
root@MyMail ~ \#uname -r
2.6.32-279.el6.x86_64
以下二种方法适用于RedHat,CentOS
root@MyMail ~ \#cat /etc/redhat-release
CentOS release 6.3 (Final)
登录到linux执行 rpm -q redhat-release 或rpm -q centos-release
root@MyMail ~ \#rpm -q centos-release
centos-release-6-4.el6.centos.10.x86_64


**参考推荐：**
[centos 查看版本](http://www.cnblogs.com/hitwtx/archive/2012/02/13/2349742.html)



