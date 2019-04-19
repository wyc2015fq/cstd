# CentOS下yum安装PostgreSQL - starRTC免费im直播会议一对一视频 - CSDN博客
2018年04月12日 10:36:36[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：245
关键词：centos install PostgreSQL
## Configure YUM repository
vim /etc/yum.repos.d/CentOS-Base.repo
[base] and [updates] sections添加：
exclude=postgresql*
## InstallPGDGRPM file
go[http://yum.postgresql.org](http://yum.postgresql.org/)and find your correct RPM.
For example, to install PostgreSQL 9.3 on CentOS 6 64-bit:
打开[http://yum.postgresql.org/repopackages.php#pg93](http://yum.postgresql.org/repopackages.php#pg93)后找到CentOS 6 - x86_64
then：
yum localinstall [http://yum.postgresql.org/9.3/redhat/rhel-6-x86_64/pgdg-centos93-9.3-1.noarch.rpm](http://yum.postgresql.org/9.3/redhat/rhel-6-x86_64/pgdg-centos93-9.3-1.noarch.rpm)
[https://download.postgresql.org/pub/repos/yum/9.6/redhat/rhel-7-ppc64le/pgdg-centos96-9.6-3.noarch.rpm](https://download.postgresql.org/pub/repos/yum/9.6/redhat/rhel-7-ppc64le/pgdg-centos96-9.6-3.noarch.rpm)
## Install PostgreSQL
list available packages:
yum list postgres*
For example, to install a basic PostgreSQL 9.3 server:
yum install postgresql93-server
yum install postgresql96-server
Other packages can be installed according to your needs.
## 配置
After installing the packages, a database needs to be initialized and configured.
PostgreSQL data directory（/var/lib/pgsql/9.3/data） contains all of the data files for the database.
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=CentOS%E4%B8%8Byum%E5%AE%89%E8%A3%85PostgreSQL&action=edit&section=5)]Initialize
The first command (only needed once) is to initialize the database：
service postgresql-9.3 initdb正在初始化数据库：                                         [确定]
或/usr/pgsql-9.6/bin/postgresql96-setup initdb报错：
postgresql96-setup initdb
Failed to get D-Bus connection: Operation not permitted
you're running a non-privileged container.
还是别在docker里面玩了！
docker run -d -e "container=docker" --privileged=true -v /sys/fs/cgroup:/sys/fs/cgroup --name pgsql centos /usr/sbin/init
docker exec -it pgsql /bin/bash
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=CentOS%E4%B8%8Byum%E5%AE%89%E8%A3%85PostgreSQL&action=edit&section=6)]Startup
开机启动：
chkconfig postgresql-9.3 onservice postgresql-9.3 start
centos7: systemctl enable postgresql-9.6.service systemctl start postgresql-9.6.service
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=CentOS%E4%B8%8Byum%E5%AE%89%E8%A3%85PostgreSQL&action=edit&section=7)]艺搜参考
[https://wiki.postgresql.org/wiki/YUM_Installation](https://wiki.postgresql.org/wiki/YUM_Installation)
