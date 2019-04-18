# ubuntu16.04安装KDE - wishchinYang的专栏 - CSDN博客
2016年05月16日 14:41:43[wishchin](https://me.csdn.net/wishchin)阅读数：7397
由于对KDE界面情有独钟，升级到ubuntu之后，第一件事就是安装kde桌面
命令：
add-apt-repository ppa:kubuntu-ppa/backports
apt-get update
apt-get upgrade
Or............
sudo apt-get install kubuntu-dektop
静等........................
然而，出现了这种问题，
kde-config-telepathy-accounts  软件包冲突
导致 软件安装程序，直接不能使用，出现：
```
xxxxx@xxxxx-xxxxxx:~$ sudo apt-get install -f
正在读取软件包列表... 完成
正在分析软件包的依赖关系树       
正在读取状态信息... 完成       
正在修复依赖关系... 完成
将会同时安装下列软件：
  kde-config-telepathy-accounts
下列【新】软件包将被安装：
  kde-config-telepathy-accounts
升级了 0 个软件包，新安装了 1 个软件包，要卸载 0 个软件包，有 0 个软件包未被升级。
有 726 个软件包没有被完全安装或卸载。
需要下载 0 B/138 kB 的归档。
解压缩后会消耗 828 kB 的额外空间。
您希望继续执行吗？ [Y/n] y
(正在读取数据库 ... 系统当前共安装有 239409 个文件和目录。)
正准备解包 .../kde-config-telepathy-accounts_4%3a15.08.2-0ubuntu1_amd64.deb  ...
正在解包 kde-config-telepathy-accounts (4:15.08.2-0ubuntu1) ...
dpkg: 处理归档 /var/cache/apt/archives/kde-config-telepathy-accounts_4%3a15.08.2-0ubuntu1_amd64.deb (--unpack)时出错：
 正试图覆盖 /usr/share/accounts/services/google-im.service，它同时被包含于软件包 account-plugin-google 0.12+15.10.20150914-0ubuntu1
dpkg-deb：错误：子进程 粘贴 被信号(断开的管道) 终止了
在处理时有错误发生：
 /var/cache/apt/archives/kde-config-telepathy-accounts_4%3a15.08.2-0ubuntu1_amd64.deb
E: Sub-process /usr/bin/dpkg returned an error code (1)
```
所以只有：
`apt-get -o Dpkg::Options::="--force-overwrite" -f install`
强制安装
参考链接：[http://forum.ubuntu.org.cn/viewtopic.php?p=3156522](http://forum.ubuntu.org.cn/viewtopic.php?p=3156522)

