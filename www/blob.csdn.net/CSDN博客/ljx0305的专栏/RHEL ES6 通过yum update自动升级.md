# RHEL ES6 通过yum update自动升级 - ljx0305的专栏 - CSDN博客
2011年12月24日 16:01:44[ljx0305](https://me.csdn.net/ljx0305)阅读数：1412标签：[mozilla																[centos																[list																[search																[import																[服务器](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=import&t=blog)](https://so.csdn.net/so/search/s.do?q=search&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=centos&t=blog)](https://so.csdn.net/so/search/s.do?q=mozilla&t=blog)
个人分类：[Linux程序及使用																[linux下软件安装](https://blog.csdn.net/ljx0305/article/category/440323)](https://blog.csdn.net/ljx0305/article/category/394700)
 通过 [http://mirror-status.centos.org](http://mirror-status.centos.org) 网站查找离自己最近的镜像站点
以下我以mirrors.163.com做为我们的镜象站点
1.要使用CentOS的源，就得将RHEL的更新源移除，或是禁用掉，并且需要将RHEL6自带的YUM工具删除。
#cd /etc/yum.repos.d
#mv packagekit-media.repo packagekit-media.repo.back
此处将其备份起来，供意外恢复时使用，如有其他的相关的源设置，都要删除，或是采用以上方法备份。
2.下载源配置文件：
#cd /etc/yum.repos.d
wget [http://mirrors.163.com/.help/CentOS6-Base-163.repo](http://mirrors.163.com/.help/CentOS6-Base-163.repo)
在这个配置文件里，有些地方要改一下，否则无法连接到服务器进行更新，其中有一个变量－－$releasever，它所代表的是系统的版本号，本文是基于6版本的，所以需要将配置文件中所有的变量名换成6即可，这里将用到一个sed的语法
#vim /etc/yum.repos.d/CentOS6-Base-163.repo
先按Esc键然后输入
：1,%s/$releasever/6/g
保存退出即可。这时RHEL6使用CentOS源的问题，算是解决了。但有一个地方需要说一下，网上好多高手们写的解决方案最后都有一个说使用设置好的源进行升级，如#yum update。
我想说的是，如果你喜欢RHEL的界面和外观的话，就不要轻意尝试这么做，否则你会后悔的，因为这样会把你的系统变得和CentOS一样，只是保留了原先的内核，但并不做为默认启动项…也就是相当于你在centos6里装了一个RHEL6的内核，虽然两个内核一样，但由于名字不同而让它们分立开来，这岂不是有违本意？
所以，如果你介意这样，请千万不要偷懒，需要升级哪个，或是需要安装哪个，就针对哪个软件进行操作，这也是我们设置这个源的目的。
3.下载并导入KEY
cd /etc/pki/rpm-gpg
wget [http://mirrors.163.com/centos/6/os/i386/](http://mirrors.163.com/centos/6/os/i386/)[RPM-GPG-KEY-CentOS-6](http://mirrors.163.com/centos/6/os/i386/RPM-GPG-KEY-CentOS-6)
rpm --import /etc/pki/rpm-gpg/RPM-GPG-KEY*
完成以上操作就可以通过yum进行更新操作
4. yum指令 
注:当第一次使用yum或yum资源库有更新时,yum会自动下载所有所需的headers放置于/var/cache/yum目录下,所需时间可能较长. 
rpm包的更新 
* 检查可更新的rpm包 
#yum check-update 
* 更新所有的rpm包 
#yum update 
* 更新指定的rpm包,如更新kernel和kernel source 
#yum update kernel kernel-source 
* 大规模的版本升级,与yum update不同的是,连旧的淘汰的包也升级 
#yum upgrade 
rpm包的安装和删除 
* 安装rpm包,如xmms-mp3 
#yum install xmms-mp3 
* 删除rpm包,包括与该包有倚赖性的包 
#yum remove licq 
* 注:同时会提示删除licq-gnome,licq-qt,licq-text 
yum暂存(/var/cache/yum/)的相关参数 
* 清除暂存中rpm包文件 
#yum clean packages 
* 清除暂存中rpm头文件 
#yum clearn headers 
* 清除暂存中旧的rpm头文件 
#yum clean oldheaders 
* 清除暂存中旧的rpm头文件和包文件 
#yum clearn 或#yum clearn all 
* 注:相当于yum clean packages + yum clean oldheaders 
包列表 
* 列出资源库中所有可以安装或更新的rpm包 
#yum list 
* 列出资源库中特定的可以安装或更新以及已经安装的rpm包 
#yum list mozilla
#yum list mozilla* 
* 注:可以在rpm包名中使用匹配符,如列出所有以mozilla开头的rpm包 
* 列出资源库中所有可以更新的rpm包 
#yum list updates 
* 列出已经安装的所有的rpm包 
#yum list installed 
* 列出已经安装的但是不包含在资源库中的rpm包 
#yum list extras 
* 注:通过其它网站下载安装的rpm包 
rpm包信息显示(info参数同list) 
* 列出资源库中所有可以安装或更新的rpm包的信息 
#yum info 
* 列出资源库中特定的可以安装或更新以及已经安装的rpm包的信息 
#yum info mozilla
#yum info mozilla* 
* 注:可以在rpm包名中使用匹配符,如列出所有以mozilla开头的rpm包的信息 
* 列出资源库中所有可以更新的rpm包的信息 
#yum info updates 
* 列出已经安装的所有的rpm包的信息 
#yum info installed 
* 列出已经安装的但是不包含在资源库中的rpm包的信息 
#yum info extras 
* 注:通过其它网站下载安装的rpm包的信息 
搜索rpm包 
* 搜索匹配特定字符的rpm包 
#yum search mozilla 
* 注:在rpm包名,包描述等中搜索 
* 搜索有包含特定文件名的rpm包 
#yum provides realplay 
5. 增加资源库 
* 例如:增加rpm.livna.org作为资源库 
* 安装Livna.org rpms GPG key 
#rpm –import 
[http://rpm.livna.org/RPM-LIVNA-GPG-KEY](http://rpm.livna.org/RPM-LIVNA-GPG-KEY)
* 检查GPG Key 
# rpm -qa gpg-pubkey* 
* 显示Key信息 
#rpm -qi gpg-pubkey-a109b1ec-3f6e28d5 
* (注:如果要删除Key,使用#rpm -e gpg-pubkey-a109b1ec-3f6e28d5)
