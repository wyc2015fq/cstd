# linux系统安装yum环境 - 菜鸟路上的小白 - CSDN博客





2016年09月05日 16:43:48[somnus_小凯](https://me.csdn.net/u012486840)阅读数：502
个人分类：[linux](https://blog.csdn.net/u012486840/article/category/6382672)









1、卸载rhel的默认安装的yum包

查看yum包

rpm -qa|grep yum

卸载之

rpm -qa|grep yum|xargs rpm -e --nodeps




2、下载新的yum包

64位linux系统下载安装包

wget http://centos.ustc.edu.cn/centos/5/os/x86_64/CentOS/yum-3.2.22-40.el5.centos.noarch.rpm

wget http://centos.ustc.edu.cn/centos/5/os/x86_64/CentOS/yum-fastestmirror-1.1.16-21.el5.centos.noarch.rpm

wget http://centos.ustc.edu.cn/centos/5/os/x86_64/CentOS/yum-metadata-parser-1.1.2-4.el5.x86_64.rpm

32位linux系统下载安装包

wget http://centos.ustc.edu.cn/centos/5/os/i386/CentOS/yum-3.2.22-40.el5.centos.noarch.rpm

wget http://centos.ustc.edu.cn/centos/5/os/i386/CentOS/yum-fastestmirror-1.1.16-21.el5.centos.noarch.rpm

wget http://centos.ustc.edu.cn/centos/5/os/i386/CentOS/yum-metadata-parser-1.1.2-4.el5.i386.rpm




注意：安装包可能会升级，最后的文件名可以通过最新路径下查看得到，例如：

http://centos.ustc.edu.cn/centos/5/os/x86_64/CentOS/

http://centos.ustc.edu.cn/centos/5/os/i386/CentOS/




然后进行安装

rpm -ivh yum-*

注意：yum和yum-fastestmirror相互依赖，所以同时安装即可。

3、下载yum的配置源

wget http://docs.linuxtone.org/soft/lemp/CentOS-Base.repo 下载到 /etc/yum.repos.d/ 目录下面




4、运行yum makecache生成缓存



