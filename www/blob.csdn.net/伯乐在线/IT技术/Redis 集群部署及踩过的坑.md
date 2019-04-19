# Redis 集群部署及踩过的坑 - 文章 - 伯乐在线
原文出处： [码上青天](https://my.oschina.net/gaoenwei/blog/1623214)
## **本文目标**
> 
要在单台机器上搭建Redis集群，方式是通过不同的TCP端口启动多个实例，然后组成集群，同时记录在搭建过程中踩过的坑。
## 安装准备
> 
centos版本：6.7
redis版本：3.2.3
安装方式：源码安装
服务器：1台
## 操作步骤
此处默认已安装好单台redis，如果不会可以参照《[centos上安装redis](https://my.oschina.net/gaoenwei/blog/1622588)》
### 1、启动Redis多个实例
我们在Redis安装目录下创建目录cluster，并编写7000.conf~7005.conf 6个配置文件，这6个配置文件用来启动6个实例，后面将使用这6个实例组成集群。
以7000.conf为例，配置文件需要填写如下几项：


```
port  7000                                        //端口7000,7001,7002,7003,7004,7005        
bind 192.168.186.91                                     //默认ip为127.0.0.1 需要改为其他节点机器可访问的ip 否则创建集群时无法访问对应的端口，无法创建集群
daemonize    yes                               //redis后台运行
pidfile  ./redis_7000.pid          //pidfile文件对应7000,7001,7002,7003,7004,7005 
cluster-enabled  yes                           //开启集群  把注释#去掉
cluster-config-file  nodes_7000.conf   //集群的配置  配置文件首次启动自动生成 7000,7001,7002,7003,7004,7005 
cluster-node-timeout  15000                //请求超时  默认15秒，可自行设置
appendonly  yes                           //aof日志开启  有需要就开启，它会每次写操作都记录一条日志
```
分别启动6个实例


```
redis-server redis_cluster/7000/redis.conf
redis-server redis_cluster/7001/redis.conf 
redis-server redis_cluster/7002/redis.conf 
redis-server redis_cluster/7003/redis.conf 
redis-server redis_cluster/7004/redis.conf 
redis-server redis_cluster/7005/redis.conf
```
启动成功后，看一下进程


```
# ps -ef | grep redis | grep cluster
idata    15711 22329  0 18:40 pts/10   00:00:00 ./bin/redis-server 192.168.186.91:7000 [cluster]
idata    15740 22329  0 18:40 pts/10   00:00:00 ./bin/redis-server 192.168.186.91:7001 [cluster]
idata    15810 22329  0 18:40 pts/10   00:00:00 ./bin/redis-server 192.168.186.91:7002 [cluster]
idata    17023 22329  0 18:42 pts/10   00:00:00 ./bin/redis-server 192.168.186.91:7003 [cluster]
idata    17030 22329  0 18:42 pts/10   00:00:00 ./bin/redis-server 192.168.186.91:7004 [cluster]
idata    17035 22329  0 18:42 pts/10   00:00:00 ./bin/redis-server 192.168.186.91:7005 [cluster]
```
至此，ip=192.168.186.91机器上创建了6个实例，端口号为port=7000~7005。
### **2、安装ruby**
**1）yum安装ruby和依赖的包。**


```
[root@itfirst redis_cluster]# yum -y install ruby ruby-devel rubygems rpm-build
已加载插件：fastestmirror
设置安装进程
Loading mirror speeds from cached hostfile
 * base: mirrors.cn99.com
 * extras: mirrors.cn99.com
 * updates: mirrors.cn99.com
base                                                                                                                                                                                                                                                   | 3.7 kB     00:00     
extras                                                                                                                                                                                                                                                 | 3.4 kB     00:00     
mysql-connectors-community                                                                                                                                                                                                                             | 2.5 kB     00:00     
mysql-tools-community                                                                                                                                                                                                                                  | 2.5 kB     00:00     
mysql56-community                                                                                                                                                                                                                                      | 2.5 kB     00:00     
updates                                                                                                                                                                                                                                                | 3.4 kB     00:00     
解决依赖关系
--> 执行事务检查
---> Package rpm-build.x86_64 0:4.8.0-55.el6 will be 安装
--> 处理依赖关系 patch >= 2.5，它被软件包 rpm-build-4.8.0-55.el6.x86_64 需要
--> 处理依赖关系 elfutils >= 0.128，它被软件包 rpm-build-4.8.0-55.el6.x86_64 需要
--> 处理依赖关系 xz，它被软件包 rpm-build-4.8.0-55.el6.x86_64 需要
--> 处理依赖关系 system-rpm-config，它被软件包 rpm-build-4.8.0-55.el6.x86_64 需要
--> 处理依赖关系 lzma，它被软件包 rpm-build-4.8.0-55.el6.x86_64 需要
--> 处理依赖关系 /usr/bin/gdb-add-index，它被软件包 rpm-build-4.8.0-55.el6.x86_64 需要
---> Package ruby.x86_64 0:1.8.7.374-5.el6 will be 安装
--> 处理依赖关系 ruby-libs = 1.8.7.374-5.el6，它被软件包 ruby-1.8.7.374-5.el6.x86_64 需要
--> 处理依赖关系 libruby.so.1.8()(64bit)，它被软件包 ruby-1.8.7.374-5.el6.x86_64 需要
---> Package ruby-devel.x86_64 0:1.8.7.374-5.el6 will be 安装
---> Package rubygems.noarch 0:1.3.7-5.el6 will be 安装
--> 处理依赖关系 ruby-rdoc，它被软件包 rubygems-1.3.7-5.el6.noarch 需要
--> 执行事务检查
---> Package elfutils.x86_64 0:0.164-2.el6 will be 安装
--> 处理依赖关系 elfutils-libs(x86-64) = 0.164-2.el6，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libdw.so.1(ELFUTILS_0.158)(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libdw.so.1(ELFUTILS_0.149)(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libdw.so.1(ELFUTILS_0.148)(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libdw.so.1(ELFUTILS_0.138)(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libdw.so.1(ELFUTILS_0.130)(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libdw.so.1(ELFUTILS_0.127)(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libdw.so.1(ELFUTILS_0.126)(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libdw.so.1(ELFUTILS_0.122)(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libasm.so.1(ELFUTILS_1.0)(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libdw.so.1()(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
--> 处理依赖关系 libasm.so.1()(64bit)，它被软件包 elfutils-0.164-2.el6.x86_64 需要
---> Package gdb.x86_64 0:7.2-92.el6 will be 安装
---> Package patch.x86_64 0:2.6-6.el6 will be 安装
---> Package redhat-rpm-config.noarch 0:9.0.3-51.el6.centos will be 安装
---> Package ruby-libs.x86_64 0:1.8.7.374-5.el6 will be 安装
--> 处理依赖关系 libreadline.so.5()(64bit)，它被软件包 ruby-libs-1.8.7.374-5.el6.x86_64 需要
---> Package ruby-rdoc.x86_64 0:1.8.7.374-5.el6 will be 安装
--> 处理依赖关系 ruby-irb = 1.8.7.374-5.el6，它被软件包 ruby-rdoc-1.8.7.374-5.el6.x86_64 需要
---> Package xz.x86_64 0:4.999.9-0.5.beta.20091007git.el6 will be 安装
---> Package xz-lzma-compat.x86_64 0:4.999.9-0.5.beta.20091007git.el6 will be 安装
--> 执行事务检查
---> Package compat-readline5.x86_64 0:5.2-17.1.el6 will be 安装
---> Package elfutils-libs.x86_64 0:0.164-2.el6 will be 安装
---> Package ruby-irb.x86_64 0:1.8.7.374-5.el6 will be 安装
--> 完成依赖关系计算
依赖关系解决
==============================================================================================================================================================================================================================================================================
 软件包                                                              架构                                                     版本                                                                               仓库                                                    大小
==============================================================================================================================================================================================================================================================================
正在安装:
 rpm-build                                                           x86_64                                                   4.8.0-55.el6                                                                       base                                                   131 k
 ruby                                                                x86_64                                                   1.8.7.374-5.el6                                                                    base                                                   538 k
 ruby-devel                                                          x86_64                                                   1.8.7.374-5.el6                                                                    base                                                   318 k
 rubygems                                                            noarch                                                   1.3.7-5.el6                                                                        base                                                   207 k
为依赖而安装:
 compat-readline5                                                    x86_64                                                   5.2-17.1.el6                                                                       base                                                   130 k
 elfutils                                                            x86_64                                                   0.164-2.el6                                                                        base                                                   240 k
 elfutils-libs                                                       x86_64                                                   0.164-2.el6                                                                        base                                                   230 k
 gdb                                                                 x86_64                                                   7.2-92.el6                                                                         base                                                   2.3 M
 patch                                                               x86_64                                                   2.6-6.el6                                                                          base                                                    90 k
 redhat-rpm-config                                                   noarch                                                   9.0.3-51.el6.centos                                                                base                                                    60 k
 ruby-irb                                                            x86_64                                                   1.8.7.374-5.el6                                                                    base                                                   318 k
 ruby-libs                                                           x86_64                                                   1.8.7.374-5.el6                                                                    base                                                   1.7 M
 ruby-rdoc                                                           x86_64                                                   1.8.7.374-5.el6                                                                    base                                                   381 k
 xz                                                                  x86_64                                                   4.999.9-0.5.beta.20091007git.el6                                                   base                                                   137 k
 xz-lzma-compat                                                      x86_64                                                   4.999.9-0.5.beta.20091007git.el6                                                   base                                                    16 k
事务概要
==============================================================================================================================================================================================================================================================================
Install      15 Package(s)
总下载量：6.7 M
Installed size: 20 M
下载软件包：
(1/15): compat-readline5-5.2-17.1.el6.x86_64.rpm                                                                                                                                                                                                       | 130 kB     00:00     
(2/15): elfutils-0.164-2.el6.x86_64.rpm                                                                                                                                                                                                                | 240 kB     00:00     
(3/15): elfutils-libs-0.164-2.el6.x86_64.rpm                                                                                                                                                                                                           | 230 kB     00:00     
(4/15): gdb-7.2-92.el6.x86_64.rpm                                                                                                                                                                                                                      | 2.3 MB     00:00     
(5/15): patch-2.6-6.el6.x86_64.rpm                                                                                                                                                                                                                     |  90 kB     00:00     
(6/15): redhat-rpm-config-9.0.3-51.el6.centos.noarch.rpm                                                                                                                                                                                               |  60 kB     00:00     
(7/15): rpm-build-4.8.0-55.el6.x86_64.rpm                                                                                                                                                                                                              | 131 kB     00:00     
(8/15): ruby-1.8.7.374-5.el6.x86_64.rpm                                                                                                                                                                                                                | 538 kB     00:00     
(9/15): ruby-devel-1.8.7.374-5.el6.x86_64.rpm                                                                                                                                                                                                          | 318 kB     00:00     
(10/15): ruby-irb-1.8.7.374-5.el6.x86_64.rpm                                                                                                                                                                                                           | 318 kB     00:00     
(11/15): ruby-libs-1.8.7.374-5.el6.x86_64.rpm                                                                                                                                                                                                          | 1.7 MB     00:00     
(12/15): ruby-rdoc-1.8.7.374-5.el6.x86_64.rpm                                                                                                                                                                                                          | 381 kB     00:00     
(13/15): rubygems-1.3.7-5.el6.noarch.rpm                                                                                                                                                                                                               | 207 kB     00:00     
(14/15): xz-4.999.9-0.5.beta.20091007git.el6.x86_64.rpm                                                                                                                                                                                                | 137 kB     00:00     
(15/15): xz-lzma-compat-4.999.9-0.5.beta.20091007git.el6.x86_64.rpm                                                                                                                                                                                    |  16 kB     00:00     
------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
总计                                                                                                                                                                                                                                          4.6 MB/s | 6.7 MB     00:01     
运行 rpm_check_debug 
执行事务测试
事务测试成功
执行事务
  正在安装   : xz-4.999.9-0.5.beta.20091007git.el6.x86_64                                                                                                                                                                                                                1/15 
  正在安装   : xz-lzma-compat-4.999.9-0.5.beta.20091007git.el6.x86_64                                                                                                                                                                                                    2/15 
  正在安装   : elfutils-libs-0.164-2.el6.x86_64                                                                                                                                                                                                                          3/15 
  正在安装   : elfutils-0.164-2.el6.x86_64                                                                                                                                                                                                                               4/15 
  正在安装   : redhat-rpm-config-9.0.3-51.el6.centos.noarch                                                                                                                                                                                                              5/15 
  正在安装   : patch-2.6-6.el6.x86_64                                                                                                                                                                                                                                    6/15 
  正在安装   : compat-readline5-5.2-17.1.el6.x86_64                                                                                                                                                                                                                      7/15 
  正在安装   : ruby-libs-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                          8/15 
  正在安装   : ruby-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                               9/15 
  正在安装   : ruby-irb-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                          10/15 
  正在安装   : ruby-rdoc-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                         11/15 
  正在安装   : gdb-7.2-92.el6.x86_64                                                                                                                                                                                                                                    12/15 
  正在安装   : rpm-build-4.8.0-55.el6.x86_64                                                                                                                                                                                                                            13/15 
  正在安装   : rubygems-1.3.7-5.el6.noarch                                                                                                                                                                                                                              14/15 
  正在安装   : ruby-devel-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                        15/15 
  Verifying  : gdb-7.2-92.el6.x86_64                                                                                                                                                                                                                                     1/15 
  Verifying  : compat-readline5-5.2-17.1.el6.x86_64                                                                                                                                                                                                                      2/15 
  Verifying  : ruby-rdoc-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                          3/15 
  Verifying  : xz-lzma-compat-4.999.9-0.5.beta.20091007git.el6.x86_64                                                                                                                                                                                                    4/15 
  Verifying  : patch-2.6-6.el6.x86_64                                                                                                                                                                                                                                    5/15 
  Verifying  : redhat-rpm-config-9.0.3-51.el6.centos.noarch                                                                                                                                                                                                              6/15 
  Verifying  : ruby-irb-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                           7/15 
  Verifying  : ruby-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                               8/15 
  Verifying  : rpm-build-4.8.0-55.el6.x86_64                                                                                                                                                                                                                             9/15 
  Verifying  : elfutils-libs-0.164-2.el6.x86_64                                                                                                                                                                                                                         10/15 
  Verifying  : rubygems-1.3.7-5.el6.noarch                                                                                                                                                                                                                              11/15 
  Verifying  : xz-4.999.9-0.5.beta.20091007git.el6.x86_64                                                                                                                                                                                                               12/15 
  Verifying  : elfutils-0.164-2.el6.x86_64                                                                                                                                                                                                                              13/15 
  Verifying  : ruby-devel-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                        14/15 
  Verifying  : ruby-libs-1.8.7.374-5.el6.x86_64                                                                                                                                                                                                                         15/15 
已安装:
  rpm-build.x86_64 0:4.8.0-55.el6                                    ruby.x86_64 0:1.8.7.374-5.el6                                    ruby-devel.x86_64 0:1.8.7.374-5.el6                                    rubygems.noarch 0:1.3.7-5.el6                                   
作为依赖被安装:
  compat-readline5.x86_64 0:5.2-17.1.el6  elfutils.x86_64 0:0.164-2.el6       elfutils-libs.x86_64 0:0.164-2.el6  gdb.x86_64 0:7.2-92.el6                       patch.x86_64 0:2.6-6.el6                                  redhat-rpm-config.noarch 0:9.0.3-51.el6.centos 
  ruby-irb.x86_64 0:1.8.7.374-5.el6       ruby-libs.x86_64 0:1.8.7.374-5.el6  ruby-rdoc.x86_64 0:1.8.7.374-5.el6  xz.x86_64 0:4.999.9-0.5.beta.20091007git.el6  xz-lzma-compat.x86_64 0:4.999.9-0.5.beta.20091007git.el6 
完毕！
```
**2）使用gem这个命令来安装redis接口**


```
[root@itfirst redis_cluster]# gem install redis
ERROR:  Error installing redis:
	redis requires Ruby version >= 2.2.2.
```
这一步骤中出现了bug,度娘告诉我是Ruby版本太低，需要升级版本。
**3）升级Ruby的版本**
安装rvm，我不知道这是个什么东西，但是感觉像是Ruby的一个包管理器。


```
[root@itfirst redis_cluster]# curl -L get.rvm.io | bash -s stable
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100 24090  100 24090    0     0  10919      0  0:00:02  0:00:02 --:--:-- 91242
Downloading https://github.com/rvm/rvm/archive/1.29.3.tar.gz
Downloading https://github.com/rvm/rvm/releases/download/1.29.3/1.29.3.tar.gz.asc
gpg: 于 2017年09月11日 星期一 04时59分21秒 CST 创建的签名，使用 RSA，钥匙号 BF04FF17
gpg: 无法检查签名：No public key
Warning, RVM 1.26.0 introduces signed releases and automated check of signatures when GPG software found. Assuming you trust Michal Papis import the mpapis public key (downloading the signatures).
GPG signature verification failed for '/usr/local/rvm/archives/rvm-1.29.3.tgz' - 'https://github.com/rvm/rvm/releases/download/1.29.3/1.29.3.tar.gz.asc'! Try to install GPG v2 and then fetch the public key:
    gpg2 --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
or if it fails:
    command curl -sSL https://rvm.io/mpapis.asc | gpg2 --import -
the key can be compared with:
    https://rvm.io/mpapis.asc
    https://keybase.io/mpapis
NOTE: GPG version 2.1.17 have a bug which cause failures during fetching keys from remote server. Please downgrade or upgrade to newer version (if available) or use the second method described above.
```
这一操作得到了：
gpg2 –recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
然后利用该密钥下载Ruby并升级。


```
[root@itfirst redis_cluster]# gpg --keyserver hkp://keys.gnupg.net --recv-keys 409B6B1796C275462A1703113804BB82D39DC0E3
gpg: 下载密钥‘D39DC0E3’，从 hkp 服务器 keys.gnupg.net
gpg: /root/.gnupg/trustdb.gpg：建立了信任度数据库
gpg: 密钥 D39DC0E3：公钥“Michal Papis (RVM signing) <mpapis@gmail.com>”已导入
gpg: 没有找到任何绝对信任的密钥
gpg: 合计被处理的数量：1
gpg:           已导入：1  (RSA: 1)
[root@itfirst redis_cluster]# curl -sSL https://get.rvm.io | bash -s stable
Downloading https://github.com/rvm/rvm/archive/1.29.3.tar.gz
Downloading https://github.com/rvm/rvm/releases/download/1.29.3/1.29.3.tar.gz.asc
gpg: 于 2017年09月11日 星期一 04时59分21秒 CST 创建的签名，使用 RSA，钥匙号 BF04FF17
gpg: 完好的签名，来自于“Michal Papis (RVM signing) <mpapis@gmail.com>”
gpg:               亦即“Michal Papis <mpapis@gmail.com>”
gpg:               亦即“[jpeg image of size 5015]”
gpg: 警告：这把密钥未经受信任的签名认证！
gpg:       没有证据表明这个签名属于它所声称的持有者。
主钥指纹： 409B 6B17 96C2 7546 2A17  0311 3804 BB82 D39D C0E3
子钥指纹： 62C9 E5F4 DA30 0D94 AC36  166B E206 C29F BF04 FF17
GPG verified '/usr/local/rvm/archives/rvm-1.29.3.tgz'
Creating group 'rvm'
Installing RVM to /usr/local/rvm/
Installation of RVM in /usr/local/rvm/ is almost complete:
  * First you need to add all users that will be using rvm to 'rvm' group,
    and logout - login again, anyone using rvm will be operating with `umask u=rwx,g=rwx,o=rx`.
  * To start using RVM you need to run `source /etc/profile.d/rvm.sh`
    in all your open shell windows, in rare cases you need to reopen all shell windows.
```
接着，source环境，让rvm可用。


```
[root@itfirst ~]# source /usr/local/rvm/scripts/rvm
```
查看Ruby可用版本


```
[root@itfirst ~]# rvm list known
# MRI Rubies
[ruby-]1.8.6[-p420]
[ruby-]1.8.7[-head] # security released on head
[ruby-]1.9.1[-p431]
[ruby-]1.9.2[-p330]
[ruby-]1.9.3[-p551]
[ruby-]2.0.0[-p648]
[ruby-]2.1[.10]
[ruby-]2.2[.7]
[ruby-]2.3[.4]
[ruby-]2.4[.1]
ruby-head
# for forks use: rvm install ruby-head-<name> --url https://github.com/github/ruby.git --branch 2.2
# JRuby
jruby-1.6[.8]
jruby-1.7[.27]
jruby[-9.1.13.0]
jruby-head
# Rubinius
rbx-1[.4.3]
rbx-2.3[.0]
rbx-2.4[.1]
rbx-2[.5.8]
rbx-3[.84]
rbx-head
# Opal
opal
# Minimalistic ruby implementation - ISO 30170:2012
mruby-1.0.0
mruby-1.1.0
mruby-1.2.0
mruby-1[.3.0]
mruby[-head]
# Ruby Enterprise Edition
ree-1.8.6
ree[-1.8.7][-2012.02]
# Topaz
topaz
# MagLev
maglev[-head]
maglev-1.0.0
# Mac OS X Snow Leopard Or Newer
macruby-0.10
macruby-0.11
macruby[-0.12]
macruby-nightly
macruby-head
# IronRuby
ironruby[-1.1.3]
ironruby-head
```
可以看到最新的版本是2.4.1，本文安装2.3.0
至此，我们升级了Ruby的版本。


```
[root@itfirst ~]# rvm install 2.3.0
Searching for binary rubies, this might take some time.
Found remote file https://rvm_io.global.ssl.fastly.net/binaries/centos/6/x86_64/ruby-2.3.0.tar.bz2
Checking requirements for centos.
Installing requirements for centos.
Installing required packages: autoconf, automake, bison, libffi-devel, libtool, readline-devel, sqlite-devel, libyaml-devel..........
Requirements installation successful.
ruby-2.3.0 - #configure
ruby-2.3.0 - #download
  % Total    % Received % Xferd  Average Speed   Time    Time     Time  Current
                                 Dload  Upload   Total   Spent    Left  Speed
100 21.9M  100 21.9M    0     0   266k      0  0:01:24  0:01:24 --:--:--  278k
No checksum for downloaded archive, recording checksum in user configuration.
ruby-2.3.0 - #validate archive
ruby-2.3.0 - #extract
ruby-2.3.0 - #validate binary
ruby-2.3.0 - #setup
ruby-2.3.0 - #gemset created /usr/local/rvm/gems/ruby-2.3.0@global
ruby-2.3.0 - #importing gemset /usr/local/rvm/gemsets/global.gems..............................
ruby-2.3.0 - #generating global wrappers........
ruby-2.3.0 - #gemset created /usr/local/rvm/gems/ruby-2.3.0
ruby-2.3.0 - #importing gemsetfile /usr/local/rvm/gemsets/default.gems evaluated to empty gem list
ruby-2.3.0 - #generating default wrappers........
```
**4）安装gem redis接口**


```
[root@itfirst ~]# rvm use 2.3.0
Using /usr/local/rvm/gems/ruby-2.3.0
[root@itfirst ~]# rvm remove 1.8.7
ruby-1.8.7-head - #already gone
Using /usr/local/rvm/gems/ruby-2.3.0
[root@itfirst ~]#  ruby --version
ruby 2.3.0p0 (2015-12-25 revision 53290) [x86_64-linux]
[root@itfirst ~]# gem install redis
Fetching: redis-4.0.1.gem (100%)
Successfully installed redis-4.0.1
Parsing documentation for redis-4.0.1
Installing ri documentation for redis-4.0.1
Done installing documentation for redis after 0 seconds
1 gem installed
```
**5）安装rubygems**


```
# yum install -y rubygems
Loaded plugins: fastestmirror, security
Setting up Install Process
Loading mirror speeds from cached hostfile
base                                                                                                                                                   | 3.7 kB     00:00     
didi_jenkins_enable                                                                                                                                    | 1.5 kB     00:00     
didi_op_toa_enable                                                                                                                                     | 1.5 kB     00:00     
didi_txjenkins_enable                                                                                                                                  | 1.5 kB     00:00     
didi_update                                                                                                                                            | 1.5 kB     00:00     
epel                                                                                                                                                   | 4.3 kB     00:00     
extras                                                                                                                                                 | 3.4 kB     00:00     
tmprepo                                                                                                                                                | 1.5 kB     00:00     
updates                                                                                                                                                | 3.4 kB     00:00     
Package rubygems-1.3.7-5.el6.noarch already installed and latest version
Nothing to do
```
到目前为止，我们的Ruby和运行redis-trib.rb需要的环境安装完成了。
### 3、Redis集群搭建
有了Ruby执行环境，可以开始将之前的6个实例组建成集群了。


```
redis-trib.rb  create  --replicas  1 192.168.186.91:7000 192.168.186.91:7001 192.168.186.91:7002 192.168.186.91:7003 192.168.186.91:7004 192.168.186.91:7005
```
有三个master，有三个是slave。 后面跟上6个实例就好了，形式就是ip:port
【此处有坑】
第一坑


```
[root@itfirst src]# redis-trib.rb  create  --replicas  1 192.168.186.91:7000 192.168.186.91:7001 192.168.186.91:7002 192.168.186.91:7003 192.168.186.91:7004 192.168.186.91:7005 
-bash: redis-trib.rb: command not found
[root@itfirst src]# cp redis-trib.rb /usr/local/bin
```
需要将redis-trib.rb复制到/usr/local/bin目录下。
第二坑


```
[root@itfirst bin]# redis-trib.rb  create  --replicas  1 192.168.186.91:7000 192.168.186.91:7001 192.168.186.91:7002 192.168.186.91:7003 192.168.186.91:7004 192.168.186.91:7005 
>>> Creating cluster
[ERR] Node 192.168.186.91:7000 is not empty. Either the node already knows other nodes (check with CLUSTER NODES) or contains some key in database 0.
```
踩完第一坑后，继续执行，发现了第二坑，还好有度娘，但是网上各种说法都有（主要参照了《[极客on之路](http://blog.csdn.net/u014756827/article/details/52061987)》的博客），发现错误的原因是redis数据库没有清除。


```
[root@itfirst src]# redis-cli -h 192.168.186.91 -p 7001
192.168.186.91:7001> flushdb
OK
192.168.186.91:7001> quit
[root@itfirst src]# redis-cli -h 192.168.186.91 -p 7002
192.168.186.91:7002> flushdb
OK
192.168.186.91:7002> quit
[root@itfirst src]# redis-cli -h 192.168.186.91 -p 7003
192.168.186.91:7003> flushdb
OK
192.168.186.91:7003> quit
[root@itfirst src]# redis-cli -h 192.168.186.91 -p 7004
192.168.186.91:7004> flushdb
OK
192.168.186.91:7004> quit
[root@itfirst src]# redis-cli -h 192.168.186.91 -p 7005
192.168.186.91:7005> flushdb
OK
192.168.186.91:7005> quit
[root@itfirst src]# redis-trib.rb  create  --replicas  1 192.168.186.91:7000 192.168.186.91:7001 192.168.186.91:7002 192.168.186.91:7003 192.168.186.91:7004 192.168.186.91:7005 
>>> Creating cluster
>>> Performing hash slots allocation on 6 nodes...
Using 3 masters:
192.168.186.91:7000
192.168.186.91:7001
192.168.186.91:7002
Adding replica 192.168.186.91:7003 to 192.168.186.91:7000
Adding replica 192.168.186.91:7004 to 192.168.186.91:7001
Adding replica 192.168.186.91:7005 to 192.168.186.91:7002
M: 61b2b608177374fd0382c872f996a2c25f62daac 192.168.186.91:7000
   slots:0-5460,9189 (5462 slots) master
M: 50e678c98c31baa4ece1cba096cc34b4545456f3 192.168.186.91:7001
   slots:5461-10922 (5462 slots) master
M: b8dc855a92d1c9a6e358380286a757011c40601d 192.168.186.91:7002
   slots:9189,10923-16383 (5462 slots) master
S: 42392d8b4665500b3229b5c5b9dcebed311c9cdf 192.168.186.91:7003
   replicates 61b2b608177374fd0382c872f996a2c25f62daac
S: 4e8cd9bae1dc0ffa63a3b8315e3f92b0490e65f8 192.168.186.91:7004
   replicates 50e678c98c31baa4ece1cba096cc34b4545456f3
S: 3344981c3290c39b0d9f427842398c17de835293 192.168.186.91:7005
   replicates b8dc855a92d1c9a6e358380286a757011c40601d
Can I set the above configuration? (type 'yes' to accept): yes
/usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis/client.rb:119:in `call': ERR Slot 9189 is already busy (Redis::CommandError)
	from /usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis.rb:2764:in `block in method_missing'
	from /usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis.rb:45:in `block in synchronize'
	from /usr/local/rvm/rubies/ruby-2.3.0/lib/ruby/2.3.0/monitor.rb:214:in `mon_synchronize'
	from /usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis.rb:45:in `synchronize'
	from /usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis.rb:2763:in `method_missing'
	from /usr/local/bin/redis-trib.rb:212:in `flush_node_config'
	from /usr/local/bin/redis-trib.rb:776:in `block in flush_nodes_config'
	from /usr/local/bin/redis-trib.rb:775:in `each'
	from /usr/local/bin/redis-trib.rb:775:in `flush_nodes_config'
	from /usr/local/bin/redis-trib.rb:1296:in `create_cluster_cmd'
	from /usr/local/bin/redis-trib.rb:1696:in `<main>'
```
第三坑


```
[root@itfirst src]# redis-trib.rb  create  --replicas  1 192.168.186.91:7000 192.168.186.91:7001 192.168.186.91:7002 192.168.186.91:7003 192.168.186.91:7004 192.168.186.91:7005 
>>> Creating cluster
>>> Performing hash slots allocation on 6 nodes...
Using 3 masters:
192.168.186.91:7000
192.168.186.91:7001
192.168.186.91:7002
Adding replica 192.168.186.91:7003 to 192.168.186.91:7000
Adding replica 192.168.186.91:7004 to 192.168.186.91:7001
Adding replica 192.168.186.91:7005 to 192.168.186.91:7002
M: 61b2b608177374fd0382c872f996a2c25f62daac 192.168.186.91:7000
   slots:0-5460,9189 (5462 slots) master
M: 50e678c98c31baa4ece1cba096cc34b4545456f3 192.168.186.91:7001
   slots:5461-10922 (5462 slots) master
M: b8dc855a92d1c9a6e358380286a757011c40601d 192.168.186.91:7002
   slots:9189,10923-16383 (5462 slots) master
S: 42392d8b4665500b3229b5c5b9dcebed311c9cdf 192.168.186.91:7003
   replicates 61b2b608177374fd0382c872f996a2c25f62daac
S: 4e8cd9bae1dc0ffa63a3b8315e3f92b0490e65f8 192.168.186.91:7004
   replicates 50e678c98c31baa4ece1cba096cc34b4545456f3
S: 3344981c3290c39b0d9f427842398c17de835293 192.168.186.91:7005
   replicates b8dc855a92d1c9a6e358380286a757011c40601d
Can I set the above configuration? (type 'yes' to accept): yes
/usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis/client.rb:119:in `call': ERR Slot 9189 is already busy (Redis::CommandError)
	from /usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis.rb:2764:in `block in method_missing'
	from /usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis.rb:45:in `block in synchronize'
	from /usr/local/rvm/rubies/ruby-2.3.0/lib/ruby/2.3.0/monitor.rb:214:in `mon_synchronize'
	from /usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis.rb:45:in `synchronize'
	from /usr/local/rvm/gems/ruby-2.3.0/gems/redis-4.0.1/lib/redis.rb:2763:in `method_missing'
	from /usr/local/bin/redis-trib.rb:212:in `flush_node_config'
	from /usr/local/bin/redis-trib.rb:776:in `block in flush_nodes_config'
	from /usr/local/bin/redis-trib.rb:775:in `each'
	from /usr/local/bin/redis-trib.rb:775:in `flush_nodes_config'
	from /usr/local/bin/redis-trib.rb:1296:in `create_cluster_cmd'
	from /usr/local/bin/redis-trib.rb:1696:in `<main>'
```
还是度娘靠谱，在《[redis 跨机器集群启动出错](http://blog.csdn.net/tengdazhang770960436/article/details/49962979)》博客中找到了答案。
这是由于之间创建集群没有成功,需要将nodes.conf和dir里面的文件全部删除。


```
[root@itfirst 7000]# find / -name "nodes-7000.conf"
/usr/local/redis-3.2.3/src/nodes-7000.conf
[root@itfirst 7000]# cd ../../
[root@itfirst src]# ls
adlist.c     bitops.o      dict.h         lzf_c.o          notify.c         redis-benchmark.c  rio.c           slowlog.h       t_zset.o
adlist.h     blocked.c     dict.o         lzf_d.c          notify.o         redis-benchmark.o  rio.h           slowlog.o       util.c
adlist.o     blocked.o     dump.rdb       lzf_d.o          object.c         redis-check-aof    rio.o           solarisfixes.h  util.h
ae.c         cluster.c     endianconv.c   lzf.h            object.o         redis-check-aof.c  scripting.c     sort.c          util.o
ae_epoll.c   cluster.h     endianconv.h   lzfP.h           pqsort.c         redis-check-aof.o  scripting.o     sort.o          valgrind.sup
ae_evport.c  cluster.o     endianconv.o   Makefile         pqsort.h         redis-check-rdb    sdsalloc.h      sparkline.c     version.h
ae.h         config.c      fmacros.h      Makefile.dep     pqsort.o         redis-check-rdb.c  sds.c           sparkline.h     ziplist.c
ae_kqueue.c  config.h      geo.c          memtest.c        pubsub.c         redis-check-rdb.o  sds.h           sparkline.o     ziplist.h
ae.o         config.o      geo.h          memtest.o        pubsub.o         redis-cli          sds.o           syncio.c        ziplist.o
ae_select.c  crc16.c       geo.o          mkreleasehdr.sh  quicklist.c      redis-cli.c        sentinel.c      syncio.o        zipmap.c
anet.c       crc16.o       help.h         multi.c          quicklist.h      redis-cli.o        sentinel.o      testhelp.h      zipmap.h
anet.h       crc64.c       hyperloglog.c  multi.o          quicklist.o      redis_cluster      server.c        t_hash.c        zipmap.o
anet.o       crc64.h       hyperloglog.o  networking.c     rand.c           redis-sentinel     server.h        t_hash.o        zmalloc.c
aof.c        crc64.o       intset.c       networking.o     rand.h           redis-server       server.o        t_list.c        zmalloc.h
aof.o        db.c          intset.h       nodes-7000.conf  rand.o           redis-trib.rb      setproctitle.c  t_list.o        zmalloc.o
asciilogo.h  db.o          intset.o       nodes-7001.conf  rdb.c            release.c          setproctitle.o  t_set.c
bio.c        debug.c       latency.c      nodes-7002.conf  rdb.h            release.h          sha1.c          t_set.o
bio.h        debugmacro.h  latency.h      nodes-7003.conf  rdb.o            release.o          sha1.h          t_string.c
bio.o        debug.o       latency.o      nodes-7004.conf  redisassert.h    replication.c      sha1.o          t_string.o
bitops.c     dict.c        lzf_c.c        nodes-7005.conf  redis-benchmark  replication.o      slowlog.c       t_zset.c
[root@itfirst src]# rm -rf nodes-700*
```
然后重启redis服务。


```
[root@itfirst src]# redis-trib.rb  create  --replicas  1 192.168.186.91:7000 192.168.186.91:7001 192.168.186.91:7002 192.168.186.91:7003 192.168.186.91:7004 192.168.186.91:7005 
>>> Creating cluster
>>> Performing hash slots allocation on 6 nodes...
Using 3 masters:
192.168.186.91:7000
192.168.186.91:7001
192.168.186.91:7002
Adding replica 192.168.186.91:7003 to 192.168.186.91:7000
Adding replica 192.168.186.91:7004 to 192.168.186.91:7001
Adding replica 192.168.186.91:7005 to 192.168.186.91:7002
M: 319da27d8668a15d2d2d02afe433247694343459 192.168.186.91:7000
   slots:0-5460 (5461 slots) master
M: 3da756265e301ac0210760f13e990473f87a3017 192.168.186.91:7001
   slots:5461-10922 (5462 slots) master
M: 6f336da48c892d8e0c541a864765978ebfbca6d5 192.168.186.91:7002
   slots:10923-16383 (5461 slots) master
S: ff4cf9d8a141d85c478b9af0358c93bca342c236 192.168.186.91:7003
   replicates 319da27d8668a15d2d2d02afe433247694343459
S: 43c2e0d7799e84b449803a68d557c3431e9e047e 192.168.186.91:7004
   replicates 3da756265e301ac0210760f13e990473f87a3017
S: 3f174fae106cb6cf7e7f21ed844895ed7c18f793 192.168.186.91:7005
   replicates 6f336da48c892d8e0c541a864765978ebfbca6d5
Can I set the above configuration? (type 'yes' to accept): yes
>>> Nodes configuration updated
>>> Assign a different config epoch to each node
>>> Sending CLUSTER MEET messages to join the cluster
Waiting for the cluster to join....
>>> Performing Cluster Check (using node 192.168.186.91:7000)
M: 319da27d8668a15d2d2d02afe433247694343459 192.168.186.91:7000
   slots:0-5460 (5461 slots) master
M: 3da756265e301ac0210760f13e990473f87a3017 192.168.186.91:7001
   slots:5461-10922 (5462 slots) master
M: 6f336da48c892d8e0c541a864765978ebfbca6d5 192.168.186.91:7002
   slots:10923-16383 (5461 slots) master
M: ff4cf9d8a141d85c478b9af0358c93bca342c236 192.168.186.91:7003
   slots: (0 slots) master
   replicates 319da27d8668a15d2d2d02afe433247694343459
M: 43c2e0d7799e84b449803a68d557c3431e9e047e 192.168.186.91:7004
   slots: (0 slots) master
   replicates 3da756265e301ac0210760f13e990473f87a3017
M: 3f174fae106cb6cf7e7f21ed844895ed7c18f793 192.168.186.91:7005
   slots: (0 slots) master
   replicates 6f336da48c892d8e0c541a864765978ebfbca6d5
[OK] All nodes agree about slots configuration.
>>> Check for open slots...
>>> Check slots coverage...
[OK] All 16384 slots covered.
```
### 4、验证集群状态
登录集群客户端，-c标识以集群方式登录


```
[root@itfirst src]# redis-cli -h 192.168.186.91 -c -p 7002
```
查看集群状态


```
192.168.186.91:7002> cluster info
cluster_state:ok
cluster_slots_assigned:16384
cluster_slots_ok:16384
cluster_slots_pfail:0
cluster_slots_fail:0
cluster_known_nodes:6
cluster_size:3
cluster_current_epoch:6
cluster_my_epoch:3
cluster_stats_messages_sent:124174
cluster_stats_messages_received:124174
192.168.186.91:7002> cluster nodes
319da27d8668a15d2d2d02afe433247694343459 192.168.186.91:7000 master - 0 1519465974307 1 connected 0-5460
3f174fae106cb6cf7e7f21ed844895ed7c18f793 192.168.186.91:7005 slave 6f336da48c892d8e0c541a864765978ebfbca6d5 0 1519465971278 6 connected
6f336da48c892d8e0c541a864765978ebfbca6d5 192.168.186.91:7002 myself,master - 0 0 3 connected 10923-16383
3da756265e301ac0210760f13e990473f87a3017 192.168.186.91:7001 master - 0 1519465972288 2 connected 5461-10922
43c2e0d7799e84b449803a68d557c3431e9e047e 192.168.186.91:7004 slave 3da756265e301ac0210760f13e990473f87a3017 0 1519465973298 5 connected
ff4cf9d8a141d85c478b9af0358c93bca342c236 192.168.186.91:7003 slave 319da27d8668a15d2d2d02afe433247694343459 0 1519465969258 4 connected
```
