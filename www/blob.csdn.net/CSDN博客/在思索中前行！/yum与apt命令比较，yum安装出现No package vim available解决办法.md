# yum与apt命令比较，yum安装出现No package vim available解决办法 - 在思索中前行！ - CSDN博客





2014年08月17日 13:26:52[_Tham](https://me.csdn.net/txl16211)阅读数：5310标签：[yum																[apt																[yum安装错误](https://so.csdn.net/so/search/s.do?q=yum安装错误&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)





[yum](http://bolg.malu.me/html/tag/yum) (Yellowdog
 Updater Modified)是一个集与查找，安装，更新和删除程序的Linux软件。它运行在RPM包兼容的[Linux](http://bolg.malu.me/html/tag/linux)发行版本上，如：[RedHat](http://bolg.malu.me/html/tag/redhat),
 Fedora, SUSE, [CentOS](http://bolg.malu.me/html/tag/centos),
 Mandriva。



今天我在用CentOS 5.5的yum命令安装的时候出现如下错误：




```
[root@malu2 ~]# yum install vim

Loaded plugins: fastestmirror

Loading mirror speeds from cached hostfile

* base: mirror.hosting90.cz

* extras: mirror.hosting90.cz

* updates: mirror.hosting90.cz

Setting up Install Process

No package vim available.

Nothing to do
```




对于此错误，我一般习惯性先去更新一下yum仓库：


```
#yum -y update
```


再次运行安装，发现错误依旧，看来这种猜包名的方法不适用；所以接下来可以通过yum的查找参数来列出[vim](http://bolg.malu.me/html/tag/vim)相关的包名:



```
[root@malu2 yum.repos.d]# yum search vim
Loaded plugins: fastestmirror

Loading mirror speeds from cached hostfile

* base: mirror.hosting90.cz

* extras: mirror.hosting90.cz

* updates: mirror.hosting90.cz

===================== Matched: vim ===================

vim-X11.x86_64 : The VIM version of the vi editor for the X Window System.

vim-common.x86_64 : The common files needed by any version of the VIM editor.

vim-enhanced.x86_64 : A version of the VIM editor which includes recent

: enhancements.

vim-minimal.x86_64 : A minimal version of the VIM editor.
```


``
``
通过查看，我们可以看到，yum列出了详细的vim相关的包名，这样就可以顺利的安装vim了：


```
#yum -y install vim-enhanced.x86_64
```


以上命令就能执行成功了。当然不仅仅vim安装，其他程序比如mysql我们也同样只要先进行yum查找，再通过匹配详细包名来进行安装，就不会出现上述错误了



## Yum的更新及安装操作
更新及安装：
yum install package 安装指定的安装包package
yum localinstall package 本地安装软件包
yum update 全部更新 
yum update package 更新指定程序包package 
yum check-update 检查可更新的程序

查找和显示
yum info <package> 显示安装包信息 
yum list 显示所有已经安装和可以安装的程序包 
yum list <package> 显示指定程序包安装情况 
yum search <keyword>查找软件包



## Yum和Apt-get对比


|**Task**|**Red Hat/Fedora**|**Ubuntu/Debian**|
|----|----|----|
|**Adding, Removing and Upgrading Packages**| | |
|Refresh list of available packages|Yum refreshes each time it’s used|apt-get update|
|Install a package from a repository|yum install*package_name*|apt-get install*package_name*|
|Install a package file|yum install*package.rpm*rpm -i*package.rpm*|dpkg –install *package.deb*|
|Remove a package|rpm -e *package_name*|apt-get remove*package_name*|
|Check for package upgrades|yum check-update|apt-get -s upgradeapt-get -s dist-upgrade|
|Upgrade packages|yum updaterpm -Uvh [args]|apt-get dist-upgrade|
|Upgrade the entire system|yum upgrade|apt-get dist-upgrade|
|**Package Information**| | |
|Get information about an available package|yum search*package_name*|apt-cache search*package_name*|
|Show available packages|yum list available|apt-cache dumpavail|
|List all installed packages|yum list installedrpm -qa|dpkg –list|
|Get information about a package|yum info *package_name*|apt-cache show*package_name*|
|Get information about an installed package|rpm -qi *package_name*|dpkg –status*package_name*|
|List files in an installed package|rpm -ql *package_name*|dpkg –listfiles*package_name*|
|List documentation files in an installed package|rpm -qd *package_name*|-|
|List configuration files in an installed package|rpm -qc *package_name*|-|
|Show the packages a given package depends on|rpm -qR *package_name*|apt-cache depends|
|Show other packages that depend on agiven package (reverse dependency)|rpm -q -whatrequires [args]|apt-cache rdepends|
|**Package File Information**| | |
|Get information about a package file|rpm -qpi *package.rpm*|dpkg –info *package.deb*|
|List files in a package file|rpm -qpl *package.rpm*|dpkg –contents*package.deb*|
|List documentation files in a package file|rpm -qpd *package.rpm*|-|
|List configuration files in a package file|rpm -qpc *package.rpm*|-|
|Extract files in a package|rpm2cpio *package.rpm* | cpio -vid|dpkg-deb –extract*package.deb* dir-to-extract-to|
|Find package that installed a file|rpm -qf *filename*|dpkg –search *filename*|
|Find package that provides a particular file|yum provides *filename*|apt-file search *filename*|
|**Misc. Packaging System Tools**| | |
|Show stats about the package cache|-|apt-cache stats|
|Verify all installed packages|rpm -Va|debsums|
|Remove packages from the local cache directory|yum clean packages|apt-get clean|
|Remove only obsolete packages from the local cache directory|-|apt-get autoclean|
|Remove header files from the local cache directory(forcing a new download of same on next use)|yum clean headers|apt-file purge|
|**General Packaging System Information**| | |
|Package file extension|*.rpm*|.deb|
|Repository location configuration|/etc/yum.conf|/etc/apt/sources.list|](https://so.csdn.net/so/search/s.do?q=apt&t=blog)](https://so.csdn.net/so/search/s.do?q=yum&t=blog)




