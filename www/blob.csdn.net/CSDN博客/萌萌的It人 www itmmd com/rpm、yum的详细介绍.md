
# rpm、yum的详细介绍 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月29日 23:42:08[Jlins](https://me.csdn.net/dyllove98)阅读数：1144


大家好，我是"屌爆了"(RPM)!，今天我还带来了我的朋友"屌翻了"(YUM)!

1、RPM的使用：
概念：
RPM：RedHat package Manager, RPM is Package Manager
源码包的命名格式：
testapp-VERSION.FileType
VERSION：x.y.z
x：主版本号(major)
y：次版本号(minor)
z；发行号(release)
FileType:
一切皆文件, 可以为任意文件
例如：
bash-4.3.2.tar.xz, nginx-1.6.1.tar.gz
rpm包命名格式：
name-major.minor.release-release.arch.rpm
rpm包自己的发行号与源代码的发行号无关, 仅用于标识对rpm自身的修订, 有时候release还包含使用的os类型
httpd-tools-2.2.15-39.el6.centos.x86_64.rpm
rpm分包机制：
核心包：包含程序最常用的功能
httpd-2.2.15-39.el6.centos.x86_64.rpm
开发包：包含开发所需要的库模块
httpd-devel-2.2.15-39.el6.centos.x86_64.rpm
帮助文档：
httpd-manual-2.2.15-39.el6.centos.noarch.rpm
工具包：
httpd-tools-2.2.15-39.el6.centos.x86_64.rpm
获取包的途径：
1、系统发行光盘或发行版官方站点或官方镜像站点(以稳定为主, 因此软件版本相对较老)
2、程序项目官方站点(版本相对较高)
3、第三方组织
Fedora-EPEL: epel
http://rpmfind.net, http://rpm.pbone.net, http://pkgs.org
4、自己制作
注意：在安装包之前做合法性验证
1、来源合法性
2、包的完整性
rpm的管理：
安装：
-i: 安装
-v, -vv , -vvv：显示详细信息, vvv比vv显示的信息更为详细, vv又比v详细
--replacepkg：重新安装
--test：测试安装, 不会真正执行安装操作
升级：
-U：如果有较旧版本程序包, 则升级安装；否则执行安装操作
-F：如果有较旧版本程序包, 则升级安装；否则终止操作
-v, -vv , -vvv：显示详细信息
--oldpackage：降级安装, 用旧版本软件包替换当前新版本的软件包
注意：如果原程序包的配置文件有被修改, 升级时新版本的配置文件不会直接覆盖源文件, 新版本的文件会以.rpmnew保存
卸载：
-e | --erase：卸载软件
注意：如果包的配置文件安装后被修改过, 卸载时此文件不会被删除, 会在源文件名加.rpmsave保留
查询：
-qa：查询所有已经安装的包
-qi：显示包简要信息(名称, 版本, 大小, 许可证等)
-ql：显示包里面的文件列表
-qc：只显示包里面的包含配置文件
-qd：显示说明文档
-qf：查看某个文件是哪个包提供的
-qs：查看已安装包文件的状态信息
-R：查看包的依赖关系
-qp[a|i|l|c]：查询未安装的包(rpm)的信息
--scripts：显示包里面包含的脚本内容
preinstall：安装前脚本
postinstall：安装后脚本
preuninstall：卸载前脚本
postuninstall：卸载后脚本
--changelog：显示包的changelog信息
校验：
-V：
S.5....T.  c /etc/skel/.bash_profile
c %config configuration file.   \# 文件类型为配置文件
d %doc documentation file.      \# 文件类型为说明文档
l %license license file.        \# 文件类型为license文件
r %readme readme file.          \# 文件类型为readme文件
S file Size differs             \# 文件大小发生了改变
M Mode differs (includes permissions and file type)   \# 文件权限发生了改变(包含文件类型变化)
5 digest (formerly MD5 sum) differs    \# MD5发生变化, 即文件已经改变
D Device major/minor number mismatch   \# 设备号改变
U User ownership differs               \# 所属主发生改变
G Group ownership differs              \# 所属组发生改变
T mTime differs                        \# mtime发生改变

包来源合法性及完整性验证：
rpm --import PUBKEY ...
rpm {-K|--checksig} [--nosignature] [--nodigest] PACKAGE_FILE ...
rpm公共数据库：
/var/lib/rpm：
重建数据库：
rpm {--initdb|--rebuilddb}
--initdb：初始化, 如果数据库存在, 则不执行任何操作
--rebuilddb：重新构建, 无论当前是否已经存在数据库, 都会直接重建并覆盖原始数据库
2、YUM工具的使用：
概念：
Yum（全称为 Yellow dog Updater, Modified）是一个在Fedora和RedHat以及SUSE中的Shell前端软件包管理器。
基於RPM包管理，能够从指定的服务器自动下载RPM包并且安装，可以自动处理依赖性关系，
并且一次安装所有依赖的软体包，无须繁琐地一次次下载、安装。
YUM仓库：
存放了众多rpm包，以及包的相关元数据的文件(repodata)服务器
文件服务器：
http://, ftp://, nfs://, file:///
YUM客户端：
配置文件：
指向仓库的位置以及各种配置信息,每个yum客户端可以有多个可用的yum仓库
主配置文件：
/etc/yum.conf
子配置文件：
/etc/yum.repos.d/*.repo
仓库定义：
[repo_id]   \# 仓库id应该确保唯一
name=仓库名称
baseurl=仓库路径 可以是http://, ftp://, nfs://, file:///
enabled={1|0}  \# 设置是否启用此仓库
gpgcheck={1|0} \# 设置是否启用gpg检查
gpgkey=file:///etc/pki/rpm-gpg/RPM-GPG-KEY-CentOS-6  \# 配置gpgcheck路径
cost=num   \# 定义仓库的开销，默认是1000，num越小优先级越高
内置变量：
$releasever   \# 当前操作系统的发行主版本号  5 6 7等
$arch         \# 平台 i486, i586, i686
$basearch     \# 基础平台i386，x86_64
镜像列表：
mirrorlist=http://mirrorlist.centos.org/?release=$releasever&arch=$basearch&repo=os
yum会使用fastestmirror插件找出最快的服务器，并以之作为本次访问的baseurl
YUM命令的使用：
列出所有可用仓库：
repolist
列出所有程序包：
list {all | installed | available}   \# 可使用glob匹配
列出所有可用包组：
grouplist
缓存管理：
clean {expire-cache|packages|headers|metadata|dbcache|rpmdb|plugins|all} \# 清除缓存
makecache  \# 生成缓存
选项：优先级高于配置文件
--enablerepo=repoidglob         \# 启用此仓库
--disablerepo=repoidglob        \# 禁用此仓库
--nogpgcheck   \# 不做gpg检查
-y： \# 自动回答为yes
安装：
install
reinstall  \# 重新安装包
升级：
checkupdate  \# 检查可用升级
update package_name
如果有多个版本的升级包可用，可以指定具体版本来升级
降级安装：
downgrade package_name
卸载：
remove | erase package_name
依赖于指定程序包的其他包也会被删除
查询：
info
list
search  \# 根据关键字模糊查询包名
provides filepath  \# 查询文件由哪个软件包提供
包组管理：
\# 包组有些有空格，因此包组名需要加引号
yum install @"包组"  \# 安装一个包组
yum remove @"包组"   \# 移除一个包组
groupinfo     \# 查看包组信息
groupinstall  \# 安装一个包组
安装本地rpm文件：
localinstall *.rpm
命令历史：
yum history
YUM仓库的构建：
createrepo [options] [directory]

小试牛刀：

1、看了这么多东西，来安装个东西试试看@@
![wKioL1R5e6ngtkgaAADCOS5rnR4008.jpg](http://s3.51cto.com/wyfs02/M00/54/24/wKioL1R5e6ngtkgaAADCOS5rnR4008.jpg)
2、步步高打火机，哪里不会点哪里，so easy！麻麻再也不用担心我的学习！在安装个东西试试@@
![wKioL1R5e9fCq9WgAADa0Y82yBo814.jpg](http://s3.51cto.com/wyfs02/M01/54/24/wKioL1R5e9fCq9WgAADa0Y82yBo814.jpg)
3、完了，问题来了@_@，请问解决依赖关系哪家强？
yum install php-mysql
![wKiom1R5e2fzzZBNAAERbBnV__Q629.jpg](http://s3.51cto.com/wyfs02/M01/54/26/wKiom1R5e2fzzZBNAAERbBnV__Q629.jpg)
4、我能不能"偷窥"下这个包里面有哪些东西捏？(还未安装的rpm)
![wKioL1R5fA7BlWx5AAClDAYas0k443.jpg](http://s3.51cto.com/wyfs02/M02/54/24/wKioL1R5fA7BlWx5AAClDAYas0k443.jpg)
5、我能不能看下安装的包有没有被别人捣乱过？
![wKiom1R5e6Owy19NAABasHK0TC0505.jpg](http://s3.51cto.com/wyfs02/M02/54/26/wKiom1R5e6Owy19NAABasHK0TC0505.jpg)
6、有时候我想知道某个命令到底是哪个包提供的，该咋整？
![wKioL1R5fD6isajOAABB8zrbduw760.jpg](http://s3.51cto.com/wyfs02/M00/54/24/wKioL1R5fD6isajOAABB8zrbduw760.jpg)
7、楼上的，万一这个文件本身就没有安装，这时候该怎么办？
![wKioL1R5fFPjg8aVAAEPuDyVUvE868.jpg](http://s3.51cto.com/wyfs02/M00/54/24/wKioL1R5fFPjg8aVAAEPuDyVUvE868.jpg)
8、你一个一个的装软件包有意思吗？俺们都是一组打包安装！！
![wKiom1R5e-DSUdvxAADAMQ1uojQ675.jpg](http://s3.51cto.com/wyfs02/M00/54/26/wKiom1R5e-DSUdvxAADAMQ1uojQ675.jpg)
师父说的安装包组还可以用更高端大气上档次的方法：
yum install @"包组"

9、当初是你要分开，分开就分开@@
![wKiom1R5fBbjuSJnAAK88VVTB-U053.jpg](http://s3.51cto.com/wyfs02/M02/54/26/wKiom1R5fBbjuSJnAAK88VVTB-U053.jpg)
![wKioL1R5gXahLnRhAADYv9WSUXU481.jpg](http://s3.51cto.com/wyfs02/M01/54/25/wKioL1R5gXahLnRhAADYv9WSUXU481.jpg)

10、一直都用别人提供的源，如果自己能建立一个属于自己的源该多好啊!!!
1) yum install httpd -y       \# 还记得baseurl吗？ 这里我们使用http://
2) yum install createrepo -y  \# 创建仓库元数据需要createrepo
3) mount /dev/cdrom /mnt/     \# 使用CentOS6.6光盘ISO
4) mkdir /var/www/html/CentOS6.6_X86_64/
5) cp -va /mnt/Packages/*.rpm /var/www/html/CentOS6.6_X86_64/ && service httpd start
6) cd /var/www/html/CentOS6.6_X86_64/
7) createrepo .    \#  Create repomd (xml-rpm-metadata) repository  创建仓库元数据
8) mkdir /etc/yum.repos.d/old
9) mv /etc/yum.repos.d/*.repo /etc/yum.repos.d/old/
10) 在/etc/yum.repos.d/中新建一个diyrepo.repo的文件内容如：
[diyrepo]
name=Diy Repo
baseurl=http://yourip/CentOS6.6_X86_64/
gpgcheck=0
11) yum clean all && yum repolist  \# 看看你自己建立的源有没有出现
12)赶快把你的这个地址告诉你的小伙伴把！！！

![wKioL1R5gZeTMH-IAADi7UfUQ3k094.jpg](http://s3.51cto.com/wyfs02/M02/54/25/wKioL1R5gZeTMH-IAADi7UfUQ3k094.jpg)

提示：
如果你自己能用，其它小伙伴不能用，麻烦你执行下面的命令：
\# service iptables save  \# 先备份一次
\# iptables -F   \# 清空
\# setenforce 0  \# 先临时关闭selinux
师父说的，还没有到那一步，先都关闭！！！

![wKiom1R5gSKAMHecAAJ7WL2u95s150.jpg](http://s3.51cto.com/wyfs02/M02/54/26/wKiom1R5gSKAMHecAAJ7WL2u95s150.jpg)
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)

