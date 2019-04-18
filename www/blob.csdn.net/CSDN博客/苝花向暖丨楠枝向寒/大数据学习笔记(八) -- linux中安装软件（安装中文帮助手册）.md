# 大数据学习笔记(八) -- linux中安装软件（安装中文帮助手册） - 苝花向暖丨楠枝向寒 - CSDN博客

2018年09月25日 19:46:35[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：54



**Table of Contents**

[编译安装(make)](#%E7%BC%96%E8%AF%91%E5%AE%89%E8%A3%85(make))

[rpm](#rpm%C2%A0)

[yum](#yum)

[中文帮助手册](#%E4%B8%AD%E6%96%87%E5%B8%AE%E5%8A%A9%E6%89%8B%E5%86%8C)

# 编译安装(make)

有些我们下载下来的压缩包，比如tengine-2.1.0.tar.gz(淘宝对nginx源码的改进版)，我们将其传入到linux以后需要先解压缩.

一般压缩包的名称就会提示我们用什么工具解压，如上面的tar

x 代表 解压、f代表文件，有gz后缀的不加f也可以
`tar xf 文件名`
解压完毕以后进入文件夹，会看到README这个文件，我们查看文件可以看到安装文档。然后按照命令提示安装即可，安装过程中会遇到错误，一般都是缺少某些程序，缺什么用 yum 命令安装什么就ok。

README中提示的操作命令，我们可以使用    命令   --help  来查看该命令的解释

注意

REAME中会提示一条 名为make的命令，这个命令必须在包含Makefile的目录下去执行。这个Makefile可能解压完就存在，也有可能不存在，不存在就必须去按照README中的提示去执行。make的意思是编译

# rpm 

由于编译安装非常麻烦，rpm安装对其进一步简化

![](https://img-blog.csdn.net/20180924201650924?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180924201550209?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 使用rpm 安装jdk

```bash
[root@node001 java]# rpm -ivh jdk-7u67-linux-x64.rpm
```

 查看jdk是否已经被安装

```bash
[root@node001 java]# rpm -qa | grep jdk
```

 查看jdk安装目录

```bash
rpm -ql jdk-1.7.0_67-fcs.x86_64 | more
```

 rpm安装的是包，一个包下又包含很多命令。我们可以查看一个命令是属于哪个包的通过以下命令

```bash
[root@node001 sbin]# rpm -qf ifconfig
net-tools-1.60-110.el6_2.x86_64
```

 rpm有个问题就是在安装的过程中可能会不那么顺利，这是由于依赖文件缺失导致的。

下面我们模拟以下上述情景

首先 将原来的minimal镜像，换位 DVD1版

![](https://img-blog.csdn.net/20180924212539393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180924212520388?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 然后将原来挂载在mnt目录下的光盘，撤销，挂载新光盘

```bash
[root@node001 sbin]# df -h
Filesystem      Size  Used Avail Use% Mounted on
/dev/sda3        97G  1.4G   91G   2% /
tmpfs           491M     0  491M   0% /dev/shm
/dev/sda1       194M   28M  157M  15% /boot
/dev/sr0        398M  398M     0 100% /mnt
[root@node001 sbin]# umount /mnt
[root@node001 sbin]# mount /dev/cdrom  /mnt
mount: block device /dev/sr0 is write-protected, mounting read-only
```

 进入 光盘目录下cp一个文件到当前用户目录

```bash
[root@node001 ~]# cd /mnt/Packages/
[root@node001 Packages]# cp httpd-2.2.15-59.el6.centos.x86_64.rpm ~
```

 回到用户目录进行安装

```bash
[root@node001 Packages]# rpm -ivh httpd-2.2.15-59.el6.centos.x86_64.rpm 
error: Failed dependencies:
	/etc/mime.types is needed by httpd-2.2.15-59.el6.centos.x86_64
	apr-util-ldap is needed by httpd-2.2.15-59.el6.centos.x86_64
	httpd-tools = 2.2.15-59.el6.centos is needed by httpd-2.2.15-59.el6.centos.x86_64
	libapr-1.so.0()(64bit) is needed by httpd-2.2.15-59.el6.centos.x86_64
	libaprutil-1.so.0()(64bit) is needed by httpd-2.2.15-59.el6.centos.x86_64
```

发现出现 依赖错误，所以就引出了yum

# yum

yum安装能够自动安装所需依赖，yum与仓库建立连接，当我们有了仓库的时候，第一件事情就是先从仓库中将所有的源数据和rpm包下载到本地，然后在本地去推算这个包所需要的相关依赖，然后全部下载下来。mnt/repodata这个存放的rpm包的信息，mnt/Packages存放的rpm包

![](https://img-blog.csdn.net/20180924225713452?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180924230121588?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

/etc/yum.repos.d  这个文件中可以配置仓库的位置，这个仓库默认位置是在国外，不过我们可以给它配置个镜像来增加其下载速度  ， [阿里镜像](https://opsx.alibaba.com/mirror)

进入 阿里镜像网页 点击 帮助可以教我们如何配置镜像。

![](https://img-blog.csdn.net/20180924220808926?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

按照帮助文档中下载完以后再次查看   vi  /etc/yum.repos.d/CentOS-Base.repo 会发现仓库位置已改变

![](https://img-blog.csdn.net/20180924221329988?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后执行

```bash
yum clean all
```

将 原来的仓库下载到本地的rpm包以及源数据删除

再执行

```bash
yum makecache
```

将 新配置的仓库 的源数据和rpm包下载到本地

使用

```bash
[root@node001 yum.repos.d]# yum repolist
```

查看每个仓库包含多少个包

**如果公司不支持访问外网，我们可以使用mnt作为一个仓库**

首先把 etc/yum.repos.d 目录下 除了CentOS-Base.repo 的其他内容移动到backup（新建的文件夹）下

**[root@node001 yum.repos.d]# lltotal 8drwxr-xr-x. 2 root root 4096 Sep 22 10:49 backup-rw-r--r--. 1 root root   58 Sep 22 10:53 CentOS-Base.repo**

然后更改 CentOS-Base.repo 中的内容为如下所示

![](https://img-blog.csdn.net/20180924224150330?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 中文帮助手册

下面我们安装一个中文的帮助手册

首先进入[阿里镜像](https://opsx.alibaba.com/mirror)找到epel进入帮助文档。按步骤进行安装

然后执行下面的命令

```bash
yum clean all
yum makecache
yum groupinstall "Chinese Support"
echo $LANG
LANG=zh_CN.utf-8
yum install man man-pages man-pages-zh-CN
```

输入 man bash 进行测试

