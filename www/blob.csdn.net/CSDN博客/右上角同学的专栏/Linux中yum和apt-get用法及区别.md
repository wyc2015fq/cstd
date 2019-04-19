# Linux中yum和apt-get用法及区别 - 右上角同学的专栏 - CSDN博客
2016年07月14日 16:36:43[右上角徐](https://me.csdn.net/u011032983)阅读数：150
**                    Linux中yum和apt-get用法及区别**
                                                                                                                                                                                                         本文转自：http://www.2cto.com/os/201306/221649.html
Linux中yum和apt-get用法及区别
一般来说著名的linux系统基本上分两大类：
1.RedHat系列：Redhat、Centos、Fedora等
2.Debian系列：Debian、Ubuntu等
RedHat 系列 
1 常见的安装包格式 rpm包,安装rpm包的命令是“rpm -参数” 
2 包管理工具  yum 
3 支持tar包 
Debian系列
1 常见的安装包格式 deb包,安装deb包的命令是“dpkg -参数”
2 包管理工具 apt-get
3 支持tar包
tar 只是一种压缩文件格式，所以，它只是把文件压缩打包而已。
rpm 相当于windows中的安装文件，它会自动处理软件包之间的依赖关系。
优缺点来说，rpm一般都是预先编译好的文件，它可能已经绑定到某种CPU或者发行版上面了。
tar一般包括编译脚本，你可以在你的环境下编译，所以具有通用性。
如果你的包不想开放源代码，你可以制作成rpm，如果开源，用tar更方便了。
tar一般都是源码打包的软件，需要自己解包，然后进行安装三部曲，./configure, make, make install.　来安装软件。
rpm是redhat公司的一种软件包管理机制，直接通过rpm命令进行安装删除等操作，最大的优点是自己内部自动处理了各种软件包可能的依赖关系。
 -------------------------------- *.rpm形式的二进制软件包[centos]
　　安装：rpm -ivh *.rpm
　　卸载：rpm -e packgename
      rpm -q nginx  查看是否已经安装
   　升级：rpm -Uvh xxx
查询：
查询所有安装的包： rpm -qa
查询某个包：rpm -qa | grep xxx
rpm -qi xxx
查询软件的安装路径：rpm -ql xxx
rpm -qc xxx
查询某个文件是那个rpm包产生：rpm -qf /etc/yum.conf
rpm -qpi xxx
rpm -qa|grep php  查看已安装的RMP包
安装：rpm -ivh xxx
移除：rpm -e xxx
升级：rpm -Uvh xxx
-------------------------------- src.rpm 源代码分发软件包的安装与卸载
　　Linux软件的源代码分发是指提供了该软件所有程序源代码的发布形式，需要用户自己编译成可执行的二进制代码并进行安装，其优点是配置灵活，可以随意去掉或保留某些功能/模块，适应多种硬件/操作系统平台及编译环境，缺点是难度较大，一般不适合初学者使用。
　　1、*.src.rpm形式的源代码软件包
　　安装：rpm -rebuild *.src.rpm
　　cd /usr/src/dist/RPMS
　　rpm -ivh *.rpm
　　卸载：rpm -e packgename
　　说明：rpm –rebuild *.src.rpm命令将源代码编译并在/usr/src/dist/RPMS下生成二进制的rpm包，然后再安装该二进制包即可。packgename如前所述。
--------------------------------dpkg【ubuntu】
dpkg -l | grep 'php'  使用dpkg -l 来查看已经安装了的软件
dpkg 是Debian[待宾] Package 的简写。为 Debian 专门开发的套件管理系统，方便软件的安装、更新及移除。所有源自Debian的Linux 发行版都使用 dpkg，例如 Ubuntu、Knoppix 等。
　　以下是一些 Dpkg 的普通用法：
　　1、dpkg -i <package.deb>
　　安装一个 Debian 软件包，如你手动[下载](http://www.2cto.com/soft)的文件。
　　2、dpkg -c <package.deb>
　　列出 <package.deb> 的内容。
　　3、dpkg -I <package.deb>
　　从 <package.deb> 中提取包裹信息。
　　4、dpkg -r <package>
　　移除一个已安装的包裹。
　　5、dpkg -P <package>
　　完全清除一个已安装的包裹。和 remove 不同的是，remove 只是删掉数据和可执行文件，purge 另外还删除所有的配制文件。
　　6、dpkg -L <package>
　　列出 <package> 安装的所有文件清单。同时请看 dpkg -c 来检查一个 .deb 文件的内容。
　　7、dpkg -s <package>
　　显示已安装包裹的信息。同时请看 apt-cache 显示 Debian 存档中的包裹信息，以及 dpkg -I 来显示从一个 .deb 文件中提取的包裹信息。
　　8、dpkg-reconfigure <package>
　　重新配制一个已经安装的包裹，如果它使用的是 debconf (debconf 为包裹安装提供了一个统一的配制界面)。
-------------------------------- 使用yum和apt-get。软件管理方法的升级.
yum的配置文件是/etc/yum.conf
1. 我们来先讲Redhat的yum 这种高级的包管理.
yum install gcc  [centos]
更新：yum update
安装：yum install xxx
移除：yum remove xxx
清除已经安装过的档案（/var/cache/yum/）：yum clean all
搜寻：yum search xxx
列出所有档案：yum list
查询档案讯息：yum info xxx
#sudo -s
#LANG=C
#yum -y install gcc gcc-c autoconf libjpeg libjpeg-devel libpng libpng-devel freetype freetype-devel libpng libpng-devel libxml2 libxml2-devel zlib zlib-devel glibc glibc-devel glib2 glib2-devel bzip2 bzip2-devel ncurses ncurses-devel curl curl-devel
用YUM安装软件包
yum -y <package_name>
命令：yum install <package_name>
用YUM删除软件包
命令：yum remove <package_name>
yum -y remove httpd*   
命令：yum search <keyword>
列出所有可安装的软件包
命令：yum list
yum list php*
列出所有可更新的软件包
命令：yum list updates
列出所有已安装的软件包
命令：yum list installed
列出所有已安装但不在 Yum Repository 內的软件包
命令：yum list extras
列出所指定的软件包
命令：yum list <package_name>
yum = Yellow dog Updater, Modified
主要功能是更方便的添加/删除/更新RPM包.
它能自动解决包的倚赖性问题.
它能便于管理大量系统的更新问题
yum特点
可以同时配置多个资源库(Repository)
简洁的配置文件(/etc/yum.conf
自动解决增加或删除rpm包时遇到的倚赖性问题
使用方便
保持与RPM[数据库](http://www.2cto.com/database/)的一致性
yum安装
CentOS 自带(yum-*.noarch.rpm)
#rpm -ivh yum-*.noarch.rpm
在第一次启用yum之前首先需要导入[系统](http://www.2cto.com/os/)的RPM-GPG-KEY：
#rpm --import /usr/share/doc/centos-release-3(4)/RPM-GPG-KEY-CentOS-3(4)
yum指令
注:当第一次使用yum或yum资源库有更新时,yum会自动下载 所有所需的headers放置于/var/cache/yum目录下,所需时间可能较长.
rpm包的更新
检查可更新的rpm包
#yum check-update
更新所有的rpm包
#yum update
更新指定的rpm包,如更新kernel和kernel source
#yum update kernel kernel-source
大规模的版本升级,与yum update不同的是,连旧的淘汰的包也升级
#yum upgrade
rpm包的安装和删除
安装rpm包,如xmms-mp3
#yum install xmms-mp3
删除rpm包,包括与该包有倚赖性的包
#yum remove licq
注:同时会提示删除licq-gnome,licq-qt,licq-text
yum暂存(/var/cache/yum/)的相关参数
清除暂存中rpm包文件
#yum clean packages
清除暂存中rpm头文件
#yum clearn headers
清除暂存中旧的rpm头文件
#yum clean oldheaders
清除暂存中旧的rpm头文件和包文件
#yum clearn 或#yum clearn all
注:相当于yum clean packages + yum clean oldheaders
包列表
列出资源库中所有可以安装或更新的rpm包
#yum list
列出资源库中特定的可以安装或更新以及已经安装的rpm包
#yum list mozilla#yum list mozilla*
注:可以在rpm包名中使用匹配符,如列出所有以mozilla开头的rpm包
列出资源库中所有可以更新的rpm包
#yum list updates
列出已经安装的所有的rpm包
#yum list installed
列出已经安装的但是不包含在资源库中的rpm包
#yum list extras
注:通过其它网站下载安装的rpm包
rpm包信息显示(info参数同list)
列出资源库中所有可以安装或更新的rpm包的信息
#yum info
列出资源库中特定的可以安装或更新以及已经安装的rpm包的信息
#yum info mozilla#yum info mozilla*
注:可以在rpm包名中使用匹配符,如列出所有以mozilla开头的rpm包的信息
列出资源库中所有可以更新的rpm包的信息
#yum info updates
列出已经安装的所有的rpm包的信息
#yum info installed
列出已经安装的但是不包含在资源库中的rpm包的信息
#yum info extras
注:通过其它网站下载安装的rpm包的信息
搜索rpm包
搜索匹配特定字符的rpm包
#yum search mozilla
注:在rpm包名,包描述等中搜索
搜索有包含特定文件名的rpm包
#yum provides realplay
增加资源库
例如:增加rpm.livna.org作为资源库
安装Livna.org rpms GPG key
#rpm --import http://rpm.livna.org/RPM-LIVNA-GPG-KEY
检查GPG Key
# rpm -qa gpg-pubkey*
显示Key信息
#rpm -qi gpg-pubkey-a109b1ec-3f6e28d5
(注:如果要删除Key,使用#rpm -e gpg-pubkey-a109b1ec-3f6e28d5)
yum常用的命令
# yum install xxx            安装xxx软件
# yum info xxx                查看xxx软件的信息
# yum remove xxx        删除软件包
# yum list                        列出软件包
# yum clean                    清除缓冲和就的包
# yum provides xxx        以xxx为关键字搜索包（提供的信息为关键字）
# yum search xxx           搜索软件包（以名字为关键字）
# yum groupupdate xxx
# yum grouplist xxx
# yum groupremove xxx
这三个都是一组为单位进行升级 列表和删除的操作。比如 "Mysql Database"就是一个组会同时操作相关的所有软件包；
# yum update                系统升级
# yum list available        列出所有升级源上的包；
# yum list updates         列出所有升级源上的可以更新包；
# yum list installed         列出已经安装的包；
# yun update kernel       升级内核；
yum常用的源
(1) 自动选择最快的源
由于yum中有的mirror速度是非常慢的，如果yum选择了这个mirror，这个时候yum就会非常慢，对此，可以下载fastestmirror插件，它会自动选择最快的mirror：
#yum install yum-fastestmirror
配置文件：（一般不用动）/etc/yum/pluginconf.d/fastestmirror.conf
你的yum镜像的速度测试记录文件：/var/cache/yum/timedhosts.txt
(2)使用图形界面的yum
如果觉得命令行的yum不方便，那么可以使用图形化的yumex，这个看起来更方便，因为可以自由地选择软件仓库：
#yum install yumex
然后在系统工具中就可以看到yum extender了。实际上系统自带的“添加/删除程序“也可以实现图形化的软件安装，但有些yumex的功能它没有。
下面介绍Ubuntu中的高级包管理方法apt-get
配置文件/etc/apt/sources.list
对于Server版， 推荐使用aptitude来查看，安装、删除deb包
sudo apt-get install aptitude
然后执行 sudo aptitude 进入管 理
也可以使用命令：
aptitude update 更新可用的包列表
aptitude upgrade 升级可用的包
aptitude dist-upgrade 将系统升级到新的发行版
aptitude install pkgname 安装包
aptitude remove pkgname 删除包
aptitude purge pkgname 删除包及其配置文件
aptitude search string 搜索包
aptitude show pkgname 显示包的详细信息
aptitude clean 删除下载的包文件
aptitude autoclean 仅删除过期的包文件
考虑到系统的兼容性,并且上面的东东比较都大,不找最新版本了，直接用apt-get install XXX 来安装.因为我们的Ubuntu是 dailyBulid的,所以光盘的内容基本上都是最新的了,无需重新下载.一定要最新版本的话，不妨先apt-get update 来更新一下软件的仓库,然后再 apt-get install.
常用的APT命令参数：
apt-cache search package 搜索包
apt-cache show package 获取包的相关信息，如说明、大小、版本等
sudo apt-get install package 安装包
sudo apt-get install package - - reinstall 重新安装包
sudo apt-get -f install 修复安装"-f = ——fix-missing"
sudo apt-get remove package 删除包
sudo apt-get remove package - - purge 删除包，包括删除配置文件等
sudo apt-get update 更新源
sudo apt-get upgrade 更新已安装的包
sudo apt-get dist-upgrade 升级系统
sudo apt-get dselect-upgrade 使用 dselect 升级
apt-cache depends package 了解使用依赖
apt-cache rdepends package 是查看该包被哪些包依赖
sudo apt-get build-dep package 安装相关的编译环境
apt-get source package 下载该包的源代码
sudo apt-get clean && sudo apt-get autoclean 清理无用的包
sudo apt-get check 检查是否有损坏的依赖
