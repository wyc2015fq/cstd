# 在VMware下安装CentOS系列1：配置VMware - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月07日 10:37:15[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：647
## 安装环境
[VMware Workstation](http://192.168.1.100/elesos_com/index.php?title=VMware_Workstation&action=edit&redlink=1) v9.0.0 build-812388
CentOS-6.3-x86_64-minimal.iso 
## minimal，bin-DVD，netinstall,LiveCD，LiveDVD区别
- CentOS-6.5-x86_64-LiveCD.iso 649M
- CentOS-6.5-x86_64-LiveDVD.iso 2G
- CentOS-6.5-x86_64-bin-DVD1.iso 4G
- CentOS-6.5-x86_64-bin-DVD2.iso 1G
- CentOS-6.5-x86_64-minimal.iso 398M
- CentOS-6.5-x86_64-netinstall.iso 243M
liveDVD版本：它就是一个体验版，无需安装到硬盘，插入光盘就可以体验Centos的各种功能。
liveCD版本：和liveDVD一样，也是一个体验版，和DVD体验版唯一的区别就是，CD的存储空间要小一些，文件也会小一些，里面的软件包少一点。
bin-DVD版本：这个版本就是普通的安装版本，如果需要给服务器安装一个centos系统到硬盘，就需要选择这个版本。这个版本的文件很大，里面包含了大量的常用软件，这样安装系统的时候就可以直接安装而无需从网络上再去下载了。
minimal版本：和bin-DVD一样也是一个安装版镜像文件，只是minimal这个文件中只包含了系统和系统必须的几个基本软件包。
netinstall版本：和bin-DVD一样也是一个安装版镜像文件，但是netinstall的软件包都需要通过网络下载进行安装而bin-DVD镜像自身包含了离线的软件包无需下载。
一般常用的有bin-DVD版本和minimal版本。
## 配置VMware
点击“创建新的虚拟机”并且选择“自定义（高级）”
“硬件兼容性”选择当前最高的版本
继续，选择“我以后再安装操作系统”
后面大多默认：
内存的选择：我机器本身有4G内存，所以就选了推荐的1G
网络类型：默认NAT，选择这个之后不用怎么设置就能上网了。如果设置为桥接则需要配独立IP。
参见：[简单区分VMware的三种网络连接模式(bridged、NAT、host-only)](http://192.168.1.100/elesos_com/index.php?title=%E7%AE%80%E5%8D%95%E5%8C%BA%E5%88%86VMware%E7%9A%84%E4%B8%89%E7%A7%8D%E7%BD%91%E7%BB%9C%E8%BF%9E%E6%8E%A5%E6%A8%A1%E5%BC%8F(bridged%E3%80%81NAT%E3%80%81host-only))

选择“定制硬件”，删除不常用的软驱和打印机。
