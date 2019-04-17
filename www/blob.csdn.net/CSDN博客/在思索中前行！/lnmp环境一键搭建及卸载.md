# lnmp环境一键搭建及卸载 - 在思索中前行！ - CSDN博客





2014年09月19日 16:26:46[_Tham](https://me.csdn.net/txl16211)阅读数：15910标签：[lnmp搭建](https://so.csdn.net/so/search/s.do?q=lnmp搭建&t=blog)
个人分类：[linux学习](https://blog.csdn.net/txl16211/article/category/2231071)









系统需求:

- CentOS/Debian/Ubuntu Linux系统
- 需要2GB以上硬盘剩余空间
- 128M以上内存,OpenVZ的建议192MB以上(小内存请勿使用64位系统)
- VPS或服务器必须已经联网！
- Linux下区分大小写，输入命令时请注意！

说明：LNMP一键安装包
 V1.1 已经在[Linode](http://www.vpser.net/go/linode)、[DiaHosting](http://www.vpser.net/go/diahosting)、[PhotonVPS](http://www.vpser.net/go/photonvps)、[DigitalOcean](http://www.vpser.net/go/digitalocean)、[VR香港VPS](http://www.vpser.net/go/vr)、[遨游主机](http://www.vpser.net/go/aoyohost)、[RamNode](http://www.vpser.net/go/ramnode)、[BudgetVM](http://www.vpser.net/go/budgetvm)、[瑞豪开源](http://www.vpser.net/go/rashost)、[DirectSpace](http://www.vpser.net/go/directspace)、[KVMLA](http://www.vpser.net/go/kvmla)、[OneAsiahost新加坡VPS](http://www.vpser.net/go/oneasiahost)、[景文互联](http://www.vpser.net/go/jwdns)、[VPSYOU](http://www.vpser.net/go/vpsyou)、[Hostigation](http://www.vpser.net/go/hostigation)、[BuyVM](http://www.vpser.net/go/buyvm)、[123systems](http://www.vpser.net/go/123systems)、[bluevm](http://www.vpser.net/go/bluevm)、[80VPS](http://www.vpser.net/go/80vps)、[XSVPS](http://www.vpser.net/go/xsvps)等众多VPS的CentOS
 5-6.5、Debian 5-7、Ubuntu 10.04-14.04的32位和64位系统上测试通过。


安装步骤:


1、使用[putty](http://www.vpser.net/other/putty-ssh-linux-vps.html)或类似的SSH工具登陆VPS或服务器；

   登陆后运行：screen -S lnmp

   (screen是linux下的一种多重视窗管理程序。在使用telnet或SSH远程登录linux时，如果连接非正常中断，重新连接时，系统将开一个新的session，无法恢复原来的session.screen命令可以解决这个问题。)

   如果提示screen: command not found 命令不存在可以执行：yum install screen 或 apt-get install screen安装，详细的[screen教程](http://www.vpser.net/manage/run-screen-lnmp.html)。（也可在主机本地直接进行安装lnmp）

2、下载并安装LNMP一键安装包：

   您可以选择使用下载版(推荐国外或者美国VPS使用)或者完整版(推荐国内VPS使用)，两者没什么区别。

执行安装程序前需要您确认您的Linux发行版，可以执行：cat /etc/issue 查看是CentOS、Debian还是Ubuntu，也可以通过VPS服务商提供的控制面板上查看。确定好之后，选择下面对应系统的安装命令： 

CentOS系统下执行：wget -c http://soft.vpser.net/lnmp/lnmp1.1-full.tar.gz && tar zxf lnmp1.1-full.tar.gz && cd lnmp1.1-full
 && ./centos.sh

(命令解释，wget是直接向地址下载，&& bash命令中连续相关的一组命令的使用方法。当前目录tar解压缩gz到lnmp1.1-full，并且cd到该目录，执行当前目录下的安装命令centos.sh，自动执行任务了)

***该目录下还有一个uninstall.sh文件，为一键安装包的卸载命令程序，该目录下执行就能卸载lnmp了。**

Debian系统下执行：wget -c http://soft.vpser.net/lnmp/lnmp1.1-full.tar.gz && tar zxf lnmp1.1-full.tar.gz && cd lnmp1.1-full
 && ./debian.sh

Ubuntu系统下执行：wget -c http://soft.vpser.net/lnmp/lnmp1.1-full.tar.gz && tar zxf lnmp1.1-full.tar.gz && cd lnmp1.1-full
 && ./ubuntu.sh

另外有2个国外镜像下载点可以使用soft2.vpser.net 和soft3.vpser.net 具体替换方法：[http://lnmp.org/faq/lnmp-download-source.html](http://lnmp.org/faq/lnmp-download-source.html)。

如需安装0.9版，请访问[http://old.lnmp.org/install.html](http://old.lnmp.org/install.html)

按上述命令执行后，会出现如下提示：

(注：如果是Debian系统，会提示Where are your servers located? asia,america,europe,oceania or africa，这里是选择服务器/VPS在哪个大洲，一般都是美国VPS，直接回车就行，如果是中国的，输入asia，回车。如果输入有错误，可以按住Ctrl再按Backspace键删除，以下均可采用此方法删除。)
![](http://lnmp.org/images/1.1/lnmp-1.1-install-1.png)

这一步需要设置MySQL的root密码，输入后回车进入下一步，如下图所示：
![](http://lnmp.org/images/1.1/lnmp-1.1-install-2.png)

这里需要确认是否启用MySQL InnoDB，如果不确定是否启用可以输入 y ，这个可以单独在MySQL文件里关闭，输入 y 表示启用，输入 n 表示不启用。输入 y 或 n 后回车进入下一步，显示如下信息：
![](http://lnmp.org/images/1.1/lnmp-1.1-install-3.png)

这一步是选择php版本，可以选择 PHP 5.3.28 或 PHP 5.2.17，如果需要安装PHP 5.3.28的话输入 y ，如果需要安装PHP 5.2.17 输入 n，输入 y 或 n 后回车，进入下一步MySQL版本的选择，如下图：
![](http://lnmp.org/images/1.1/lnmp-1.1-install-4.png)

可以选择MySQL 版本 5.1.73、5.5.37或MariaDB 5.5.37，如果需要安装MySQL 5.5.37的话输入 y ，如果需要安装MySQL 5.1.73 输入n，如果需要安装MariaDB 5.5.37的话输入 md，输入完成后回车，完成选择。

提示"Press any key to start..."，按回车键确认开始安装。

LNMP脚本就会自动安装编译Nginx、MySQL、PHP、phpMyAdmin、Zend Optimizer这几个软件。

安装时间可能会几十分钟到几个小时不等，主要是机器的配置网速等原因会造成影响。

3、安装完成

如果显示如下界面：
![](http://lnmp.org/images/1.1/lnmp-1.1-install-5.png)

Nginx、MySQL、PHP都是running，80和3306端口都存在，说明已经安装成功。

接下来按[添加虚拟主机教程](http://lnmp.org/faq/lnmp-vhost-add-howto.html)，添加虚拟主机，通过[sftp](http://lnmp.org/faq/sftp.html)或[ftp服务器](http://lnmp.org/faq/ftpserver.html)上传网站，将域名解析到VPS或服务器的IP上，解析生效即可使用。

4、安装失败
![](http://lnmp.org/images/lnmp-install-6.png)

如果出现类似上图的提示，则表明安装失败，说明没有安装成功！！需要用[winscp](http://www.vpser.net/manage/winscp.html)或其他类似工具，将/root目录下面的lnmp-install.log下载下来，到[LNMP支持论坛](http://bbs.vpser.net/forum-25-1.html)发帖注明你的发行版名称版本号(可通过
 cat /etc/issue获得)、32位还是64位，并将lnmp-install.log压缩以附件形式上传到论坛，我们会通过日志查找错误，并给予相应的解决方法。



5、添加、删除虚拟主机及伪静态管理
[http://lnmp.org/faq/lnmp-vhost-add-howto.html](http://lnmp.org/faq/lnmp-vhost-add-howto.html)

6、eAccelerator、xcache、memcached、imageMagick、ionCube、redis、opcache的安装
[http://lnmp.org/faq/Add-ons.html](http://lnmp.org/faq/Add-ons.html)

7、LNMP相关软件目录及文件位置
[http://lnmp.org/faq/lnmp-software-list.html](http://lnmp.org/faq/lnmp-software-list.html)

8、LNMP状态管理命令
[http://lnmp.org/faq/lnmp-status-manager.html](http://lnmp.org/faq/lnmp-status-manager.html)






LNMP状态管理命令：

LNMP 1.2状态管理: lnmp {start|stop|reload|restart|kill|status}

LNMP 1.2各个程序状态管理: lnmp {nginx|mysql|mariadb|php-fpm|pureftpd} {start|stop|reload|restart|kill|status}

LNMP 1.1状态管理： /root/lnmp {start|stop|reload|restart|kill|status}

Nginx状态管理：/etc/init.d/nginx {start|stop|reload|restart}

MySQL状态管理：/etc/init.d/mysql {start|stop|restart|reload|force-reload|status}

Memcached状态管理：/etc/init.d/memcached {start|stop|restart}

PHP-FPM状态管理：/etc/init.d/php-fpm {start|stop|quit|restart|reload|logrotate}

PureFTPd状态管理： /etc/init.d/pureftpd {start|stop|restart|kill|status}

ProFTPd状态管理： /etc/init.d/proftpd {start|stop|restart|reload}

如重启LNMP，输入命令：/root/lnmp restart 即可，单独重启mysql：/etc/init.d/mysql restart

LNMPA状态管理命令：

LNMPA 1.2状态管理: lnmp {start|stop|reload|restart|kill|status}

LNMPA 1.2各个程序状态管理: lnmp {httpd|mysql|mariadb|pureftpd} {start|stop|reload|restart|kill|status}

LNMPA1.1状态管理： /root/lnmpa {start|stop|reload|restart|kill|status}

Nginx状态管理：/etc/init.d/nginx {start|stop|reload|restart}

MySQL状态管理：/etc/init.d/mysql {start|stop|restart|reload|force-reload|status}

Memcached状态管理：/etc/init.d/memcached {start|stop|restart}

PureFTPd状态管理： /etc/init.d/pureftpd {start|stop|restart|kill|status}

ProFTPd状态管理： /etc/init.d/proftpd {start|stop|restart|reload}

Apache状态管理：/etc/init.d/httpd {start|stop|restart|graceful|graceful-stop|configtest|status}

LAMP状态管理命令：

LAMP 1.2状态管理: lnmp {start|stop|reload|restart|kill|status}

LAMP 1.2各个程序状态管理: lnmp {httpd|mysql|mariadb|pureftpd} {start|stop|reload|restart|kill|status}





