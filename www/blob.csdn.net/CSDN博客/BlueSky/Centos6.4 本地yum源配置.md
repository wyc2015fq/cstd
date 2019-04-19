# Centos6.4 本地yum源配置 - BlueSky - CSDN博客
2015年11月26日 00:02:48[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：506
个人分类：[Linux技巧](https://blog.csdn.net/ASIA_kobe/article/category/5981797)
转自：http://www.cnblogs.com/shenliang123/p/3829044.html
由于单位的服务器均使用的是内网，而安装一些软件如Git，需要很多的依赖包，使用yum安装相对简单，由于不能联网故配置本地yum源配置。
1.首先将需要rpm库添加到系统中：
1).虚拟机中安装的linux操作系统:则只需将安装包ISO文件挂载到media下面（如无法挂载请参考http://www.cnblogs.com/shenliang123/p/3203278.html）
2).单独的服务器系统，则先在/media下新建centos（名字可以自定义，但与下面4中添加的目录相关），然后将ISO包里面的内容全部复制到该文件夹中
2.进入到 /etc/yum.repos.d/  ,该目录下包含了四个文件：
```
[root@sl media]# ls -l /etc/yum.repos.d/
total 16
-rw-r--r--. 1 root root 1926 Feb 25  2013 CentOS-Base.repo
-rw-r--r--. 1 root root  638 Feb 25  2013 CentOS-Debuginfo.repo
-rw-r--r--. 1 root root  630 Feb 25  2013 CentOS-Media.repo
-rw-r--r--. 1 root root 3664 Feb 25  2013 CentOS-Vault.repo
```
3.将 CentOS-Base.repo和CentOS-Debuginfo.repo两个文件改名，绕过网络安装：
```
[root@sl yum.repos.d]# mv CentOS-Base.repo CentOS-Base.repose_bak
[root@sl yum.repos.d]# mv CentOS-Debuginfo.repo CentOS-Debuginfo.repo_bak
```
4.编辑文件CentOS-Media.repo:（也可以自己新增一个repo文件，more用于查看，使用vi进行编辑）
```
[root@sl yum.repos.d]# more CentOS-Media.repo　　#repo后缀
# CentOS-Media.repo
#
#  This repo can be used with mounted DVD media, verify the mount point for
#  CentOS-6.  You can use this repo and yum to install items directly off the
#  DVD ISO that we release.
#
# To use this repo, put in your DVD and use it with the other repos too:
#  yum --enablerepo=c6-media [command]
#
# or for ONLY the media repo, do this:
#
#  yum --disablerepo=\* --enablerepo=c6-media [command]
[c6-media]　　　　　　　　　　　　　　 #库名称
name=CentOS-$releasever - Media　　#名称描述
baseurl=file:///media/centos/     #yum源目录，源地址
gpgcheck=1　　　　　　　　　　　　    #检查GPG-KEY，0为不检查，1为检查
enabled=1　　　　　　　　　　　　　　 #是否用该yum源，0为禁用，1为使用
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-6　　#gpgcheck=0时无需配置
```
5.测试yum是否配置成功：
```
[root@sl yum.repos.d]# yum list
Loaded plugins: fastestmirror, security
Loading mirror speeds from cached hostfile
Installed Packages
ConsoleKit.x86_64                        0.4.1-3.el6                          @anaconda-CentOS-201303020151.x86_64/6.4
ConsoleKit-libs.x86_64                   0.4.1-3.el6                          @anaconda-CentOS-201303020151.x86_64/6.4
MAKEDEV.x86_64                           3.24-6.el6                           @anaconda-CentOS-201303020151.x86_64/6.4
MySQL-python.x86_64                      1.2.3-0.3.c1.1.el6                   @anaconda-CentOS-201303020151.x86_64/6.4
PyGreSQL.x86_64                          3.8.1-2.el6                          @anaconda-CentOS-201303020151.x86_64/6.4
SDL.x86_64                               1.2.14-3.el6                         @anaconda-CentOS-201303020151.x86_64/6.4
TurboGears2.noarch                       2.0.3-4.el6                          @anaconda-CentOS-201303020151.x86_64/6.4
abrt.x86_64                              2.0.8-15.el6.centos                  @anaconda-CentOS-201303020151.x86_
more.........
```
6.清除yum缓存：
```
[root@sl yum.repos.d]# yum clean all
Loaded plugins: fastestmirror, security
Cleaning repos:
Cleaning up Everything
Cleaning up list of fastest mirrors
```
**可能由于没有使用光驱的缘故，以上设置后安装一直报nothing to do,所以选了另外一种方法重新安装：**
1.将yum源放到/opt目录下,并复制系统安装包下的packages文件夹中所有的repo文件
```
[root@sl opt]# mkdir yumserver
[root@sl opt]# cp -rv /media/centos/Packages/* /opt/yumserver/
```
2.安装createrepo命令
```
[root@sl opt]# cd localserver/
[root@sl localserver]# rpm -ivh createrepo-0.9.9-17.el6.noarch.rpm
warning: createrepo-0.9.9-17.el6.noarch.rpm: Header V3 RSA/SHA1 Signature, key ID c105b9de: NOKEY
error: Failed dependencies:
        python-deltarpm is needed by createrepo-0.9.9-17.el6.noarch
```
以上提示安装失败，需要安装依赖包：python-deltarpm
```
[root@sl localserver]# rpm -ivh createrepo-0.9.9-17.el6.noarch.rpm python-deltarpm-3.5-0.5.20090913git.el6.x86_64.rpm deltarpm-3.5-0.5.20090913git.el6.x86_64.rpm
warning: createrepo-0.9.9-17.el6.noarch.rpm: Header V3 RSA/SHA1 Signature, key ID c105b9de: NOKEY
Preparing...                ########################################### [100%]
   1:deltarpm               ########################################### [ 33%]
   2:python-deltarpm        ########################################### [ 67%]
   3:createrepo             ########################################### [100%]
```
createrepo安装成功
3.利用createrepo创建yum仓库，创建索引信息(需要等待一段时间)
```
[root@sl localserver]# createrepo -v ./
```
4.配置文件，将/etc/yum.repos.d/下现存文件都删除或重命名.bak,然后新建一个local.repo文件，并添加相应内容：
```
[root@sl yum.repos.d]# mkdir local.repo
[root@sl yum.repos.d]# vi local.repo　　#repo后缀
#
#  This repo can be used with mounted DVD media, verify the mount point for
#  CentOS-6.  You can use this repo and yum to install items directly off the
#  DVD ISO that we release.
#
# To use this repo, put in your DVD and use it with the other repos too:
#  yum --enablerepo=c6-media [command]
#
# or for ONLY the media repo, do this:
#
#  yum --disablerepo=\* --enablerepo=c6-media [command]
[local]　　　　　　　　　　　　　　 #库名称
name=CentOS-local　　               #名称描述
baseurl=file:///opt/localserver/    #yum源目录，源地址
gpgcheck=1　　　　　　　　　　   #检查GPG-KEY，0为不检查，1为检查
enabled=1　　　　　　　　　　　  #是否用该yum源，0为禁用，1为使用
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-6　　#gpgcheck=0时无需配置
```
5.安装成功后，测试：
```
[root@sl yum.repos.d]# yum clean all          #清除缓存
Loaded plugins: fastestmirror, security
Cleaning repos:
Cleaning up Everything
Cleaning up list of fastest mirrors
[root@sl yum.repos.d]# yum makecache        #重新建立缓存
Loaded plugins: fastestmirror, security
Loading mirror speeds from cached hostfile
local                                                                                           | 2.9 kB     00:00 ...
local/filelists_db                                                                              | 3.9 MB     00:00 ...
local/other_db                                                                                  | 1.7 MB     00:00 ...
Metadata Cache Created
```
