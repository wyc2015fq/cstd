# CentOS搭建本地yum源（http方式） - BlueSky - CSDN博客
2015年11月25日 23:59:00[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：361
**YUM介绍**
YUM主要用于自动升级、安装\移除rpm软件包，它能自动查找并解决rpm包之间的依赖关系，要成功的使用YUM工具更新系统和软件，需要有一个包含各种rpm软件包的repository（软件仓库），提供软件仓库的服务器习惯上成为“源”服务器。网络上有大量的源服务器，但是，由于受到网络连接速度、带宽的限制，导致软件安装耗时过长甚至失败。特别是当有大量服务器大量软件包需要升级时，更新的缓慢程序令人难以忍受。
相比较而言，本地YUM源服务器最大优点在局域网的快速网络连接和稳定性。有了局域网中的YUM源服务器，即便在Internet连接中断的情况下，也不会影响其他YUM客户端的软件升级和安装。
下面就介绍下 本地yum源的搭建。
**一、****启动 httpd 服务（一般httpd服务已经安装）**
取一台 CentOS操作系统的机器作为源服务器。启动服务器的httpd 服务：service httpd start
![](http://static.oschina.net/uploads/space/2015/0127/104057_1Y5n_1461927.png)
可查看配置文件: /etc/httpd/conf/httpd.conf 
了解httpd相关配置信息
这里我们配置的文档根目录为：/var/www/html/
端口: Listen 80
vi /etc/httpd/conf/httpd.conf
![](http://static.oschina.net/uploads/space/2015/0127/104057_m1Pg_1461927.png)
确认服务启动，浏览器访问： [http://localhost:80](http://localhost/)
 出现如下apache页面（如果从其他机器访问，请先关闭防火墙：service iptables stop ）
![](http://static.oschina.net/uploads/space/2015/0127/104057_iOF7_1461927.png)
**二、****安装 createrepo 工具 （若使用 linux iso 镜像 则为非必须）**
createrepo是linux下的创建仓库的软件包。
1、在机器联网的情况下可直接安装： yum install createrepo   
2、可使用rpm -ivh createrepo-xxx.noarch.rpm 安装由网上下载的rpm包，一般会有两个依赖
3、linux常用工具rpm包，一般在镜像文件中都存在
**三、****安装 yum 源**
**1、****使用 Linux ISO 镜像中的RPM包安装**
（1）、将 ISO 文件 copy 到服务器上，并在文件夹: /var/www/html/ 下创建文件夹 CentOS
![](http://static.oschina.net/uploads/space/2015/0127/104058_1nAF_1461927.png)
（2）、将 ISO文件挂载至文件夹 /var/www/html/CentOS 
执行：mount -o loop /var/www/html/CentOS-6.4-x86_64-bin-DVD1.iso /var/www/html/CentOS
(取消挂载 umount /var/www/html/CentOS)
![](http://static.oschina.net/uploads/space/2015/0127/104058_toud_1461927.png)
（3）、查看文件夹 /var/www/html/CentOS 
![](http://static.oschina.net/uploads/space/2015/0127/104058_56t8_1461927.png)
可以看到 repodata 文件夹的存在，repodata作为软件的仓库信息文件。 
ISO镜像 yum源搭建OK
浏览器访问：[http://localhost:80/CentOS/](http://localhost/CentOS/)
![](http://static.oschina.net/uploads/space/2015/0127/104058_ZTJn_1461927.png)
**2、****使用 自己准备的 RPM 包 （这里以 apache ambari为例）**
（1）、在目录 /var/www/html/ 下建立一个文件夹，把准备的 RPM包放到里面
![](http://static.oschina.net/uploads/space/2015/0127/104058_R9vZ_1461927.png)
（2）、创建仓库信息文件，在目录 /var/www/html/ 下执行：createrepo ambari
![](http://static.oschina.net/uploads/space/2015/0127/104059_FyjF_1461927.png)
查看ambari目录，可以看到 repodate 文件已经生成
ambari yum源搭建OK
![](http://static.oschina.net/uploads/space/2015/0127/104059_bUVE_1461927.png)
**四、使用yum源**
（1）、编辑 repo 文件
在本机或其他机器上，进入文件夹：/etc/yum.repos.d/   将其中的文件移除掉
![](http://static.oschina.net/uploads/space/2015/0127/104059_tcFu_1461927.png)
新建文件：CentOS.repo
（可由 CentOS-Base.repo copy）
![](http://static.oschina.net/uploads/space/2015/0127/104059_CHxs_1461927.png)
（2）、更新 yum源
yum clean  清除yum源缓存
yum repolist
列出可用yum源
![](http://static.oschina.net/uploads/space/2015/0127/104059_oUT9_1461927.png)
（3）、使用 （尝试安装postgresql）
![](http://static.oschina.net/uploads/space/2015/0127/104059_Db5d_1461927.png)
同理可配置使用 ambari 源
![](http://static.oschina.net/uploads/space/2015/0127/104059_2765_1461927.png)
可将 repo 文件放置在源服务器 /var/www/html/ 目录下，以供其他机器方便下载使用
**五、可能会出现的问题**
1、问题
you don't have permission to access / on this server(Apache Server权限访问问题）
Apache Http Server已经在linux上安装完成，可是通过浏览器访问，却出现了“you don't have permission to access / on this server”.
解决：
（1）、在httpd.conf文件中，有这么一段
<Directory/> # 这里的目录为自己的目录
Options FollowSymLinks
AllowOverride None
Order deny,allow
Deny from all
</Directory>
可以尝试把Deny改成Allow，然后重启apache服务，访问页面，看问题是否解决。
（2）、查看相应的目录是否拥有权限，如果你不知道怎么加权限之类，运行如下两个命令：
chown apache.root 目录名 －R    # 此是更改目录所属用户和用户组
chmod 755  目录名 －R    # 此是更改目录读写权限
然后重启apache服务，访问页面，看问题是否解决。
（3）、终极绝杀，以上2条都还通不过的话（报相同的错误信息），运行如下两个命令
setenforce 0               # 暂时禁用SELinux服务
service iptables stop  #暂时禁用防火墙
访问页面，看问题是否解决；
2、问题：
Loaded plugins: fastestmirror, refresh-packagekit, security
Determining fastest mirrors
http://10.1.33.21/ambari/repodata/repomd.xml: [Errno 14] PYCURL ERROR 22 - "The requested URL returned error: 403 Forbidden"
Trying other mirror.
Error: Cannot retrieve repository metadata (repomd.xml) for repository: ambari-1.x. Please verify its path and try again
解决：
yum源服务器运行如下命令
setenforce 0               # 暂时禁用SELinux服务
3、问题：
yum install xxx.rpm
warning：
Package xxx.rpm is not signed
解决：
只要在在后面加上--nogpgcheck就可以了，格式如下
yum install *rpm --nogpgcheck
