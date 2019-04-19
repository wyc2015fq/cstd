# windows 7架设OpenSSH服务器实践 - xqhrs232的专栏 - CSDN博客
2017年04月28日 11:27:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：972
原文地址::[http://www.cnblogs.com/ericsun/archive/2012/06/10/2544413.html](http://www.cnblogs.com/ericsun/archive/2012/06/10/2544413.html)
相关文章
1、Windows7安装OpenSSH----[http://blog.sina.com.cn/s/blog_4a0a8b5d01015b0n.html](http://blog.sina.com.cn/s/blog_4a0a8b5d01015b0n.html)
2、如何在Windows系统上安装OPENSSH----[https://zhidao.baidu.com/question/1497984396179752899.html](https://zhidao.baidu.com/question/1497984396179752899.html)
参考链接
http://www.worldgoneweb.com/2011/installing-openssh-on-windows-7/
最近有个想法就是在自己的电脑开个ssh服务，然后在远程如果需要在自己电脑上工作的时候就只需要ssh连接到自己的电脑然后用mstsc remote desktop连接到自己的电脑IP上就可以了。
1.下载OpenSSH：http://sourceforge.net/projects/sshwindows/files/OpenSSH%20for%20Windows%20-%20Release/3.8p1-1%2020040709%20Build/setupssh381-20040709.zip/download
2.安装，反正我是全都默认一路走下来，期间跳出个窗口说要编辑passwd文件的警告，OK继续搞下去。
3.切换到SSH安装目录下的bin
4.
```
# create the default groups
mkgroup -l >> ..\etc\group
#create the default users
mkpasswd -l >> ..\etc\passwd
# Note: for neither of the above commands did I bother with the domain version documented in the quickstart
# Now, if you look in ..\etc\passwd, you will see entries (one per line) for all your Windows users.
# Start the server in debug mode
cd ..\usr\sbin
sshd -d -d -d
```
5.出现如下提示
```
debug1: sshd version OpenSSH_3.8.1p1
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@         WARNING: UNPROTECTED PRIVATE KEY FILE!          @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Permissions 0644 for '/etc/ssh_host_rsa_key' are too open.
It is recommended that your private key files are NOT accessible by others.
This private key will be ignored.
bad permissions: ignore key: /etc/ssh_host_rsa_key
Could not load host key: /etc/ssh_host_rsa_key
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@         WARNING: UNPROTECTED PRIVATE KEY FILE!          @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
Permissions 0644 for '/etc/ssh_host_dsa_key' are too open.
It is recommended that your private key files are NOT accessible by others.
This private key will be ignored.
bad permissions: ignore key: /etc/ssh_host_dsa_key
Could not load host key: /etc/ssh_host_dsa_key
Disabling protocol version 2. Could not load host key
sshd: no hostkeys available -- exiting.
```
6.可以看出是配置文件的权限没有配置好，于是chown chmod之
```
cd ..\..\etc
..\bin\chown <username> *
..\bin\chmod 600 *
```
7.发现貌似cygwin的dll文件出了问题，参考贴里面提供了更新的dll文件的链接cygintl-2和cygwin1.dll，地址是http://samanthahalfon.net/resources/cygwin_includes.zip，将其中的两个dll解压覆盖bin目录下的老文件，如果提示权限不足，可能是因为cygrunsrv进程还是活着的，使用了这个dll文件，杀掉cygrunsrc进程完成替换
8.重启opensshd服务
```
net start opensshd
```
9.系统提示服务启动成功，我们可以用ssh命令或者putty来试验一下
```
ssh <username>@<ip_windows_machine>
# You will be prompted to accept the authenticity of host, type yes
# You will be prompted for a password
# Enter your windows password
```
10.试验成功，连接到自己的机器了
