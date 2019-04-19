# yum 下载RPM包 - sxf_123456的博客 - CSDN博客
2018年02月27日 11:20:40[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：984
                yum 下载RPM包
yum install --downloadonly <package-name>
下载的rpm包默认在
/var/cache/yum/x86_64/[centos/fedora-version]/[repository]/packages
下载到指定目录
yum install --downloadonly --downloaddir=/tmp <package-name>
运行时出现Command line error:no such option:--downloadonly
执行下边安装命令
yum install yum-plugin-downloadonly
linux批量安装某个目录下的rpm包
rpm -ivh *.rpm --force --nodeps            
