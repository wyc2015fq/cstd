# 安装完最小化 RHEL/CentOS 7 后需要做的 30 件事情7. 安装 PHP PHP 是用于 web 基础服务的服务器端脚本语言。它也经常被用作通用编程语言。在最小化安装的 CentOS 中安 - 在思索中前行！ - CSDN博客





2015年10月16日 15:36:45[_Tham](https://me.csdn.net/txl16211)阅读数：1101









CentOS 是一个工业标准的 Linux 发行版，是红帽企业版 Linux 的衍生版本。你安装完后马上就可以使用，但是为了更好地使用你的系统，你需要进行一些升级、安装新的软件包、配置特定服务和应用程序等操作。

centos 安装完后，首先检查centos的更新： yum list updates  ，更新系统是 yum update 命令，将会在 yum 上将centos 更新到当前发行号的最新版本

一键安装安装lnmp


这篇文章介绍了 “安装完 RHEL/CentOS 7 后需要做的 30 件事情”。阅读帖子的时候请先完成 RHEL/CentOS 最小化安装，这是首选的企业和生产环境。如果还没有，你可以按照下面的指南，它会告诉你两者的最小化安装方法。
- [最小化安装 CentOS 7](http://www.tecmint.com/centos-7-installation/)
- [最小化安装 RHEL 7](http://www.tecmint.com/redhat-enterprise-linux-7-installation/)


我们会基于工业标准的需求来介绍以下列出的这些重要工作。我们希望这些东西在你配置服务器的时候能有所帮助。
- 
注册并启用红帽订阅
- 
使用静态 IP 地址配置网络
- 
设置服务器的主机名称
- 
更新或升级最小化安装的 CentOS
- 
安装命令行 Web 浏览器
- 
安装 Apache HTTP 服务器
- 
安装 PHP
- 
安装 MariaDB 数据库
- 
安装并配置 SSH 服务器
- 
安装 GCC (GNU 编译器集)
- 
安装 Java
- 
安装 Apache Tomcat
- 
安装 Nmap 检查开放端口
- 
配置防火墙
- 
安装 Wget
- 
安装 Telnet
- 
安装 Webmin
- 
启用第三方库
- 
安装 7-zip 工具
- 
安装 NTFS-3G 驱动
- 
安装 Vsftpd FTP 服务器
- 
安装和配置 sudo
- 
安装并启用 SELinux
- 
安装 Rootkit Hunter
- 
安装 Linux Malware Detect (LMD)
- 
用 Speedtest-cli 测试服务器带宽
- 
配置 Cron 作业
- 
安装 Owncloud
- 
启用 VirtualBox 虚拟化
- 
用密码保护 GRUB


（LCTT 译注：关于 CentOS/RHEL 7的更多信息，请参考专题：[http://www.linux.cn/topic-centos7.html](http://www.linux.cn/topic-centos7.html) ）

### 1. 注册并启用红帽订阅


RHEL 7 最小化安装完成后，就应该注册并启用系统红帽订阅库， 并执行一个完整的系统更新。这只当你有一个可用的红帽订阅时才能有用。你要注册才能启用官方红帽系统库，并时不时进行操作系统更新。（LCTT 译注：订阅服务是收费的）


在下面的指南中我们已经包括了一个如何注册并激活红帽订阅的详细说明。
- [在 RHEL 7 中注册并启用红帽订阅](http://www.tecmint.com/enable-redhat-subscription-reposiories-and-updates-for-rhel-7/)

注意: 这一步仅适用于有一个有效订阅的红帽企业版 Linux。如果你用的是 CentOS 服务器，请查看后面的章节。

## 2. 使用静态 IP 地址配置网络


你第一件要做的事情就是为你的 CentOS 服务器配置静态 IP 地址、路由以及 DNS。我们会使用 ip 命令代替 ifconfig 命令。当然，ifconfig 命令对于大部分 Linux 发行版来说还是可用的，还能从默认库安装。
- `# yum install net-tools             [它提供 ifconfig 工具，如果你不习惯 ip 命令，还可以使用它]`

![在 Linux 上安装 ifconfig](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151951oeeewafswse9rt9f.jpg)

在 Linux 上安装 ifconfig


（LCTT 译注：关于 ip 命令的使用，请参照：[http://www.linux.cn/article-3631-1.html](http://www.linux.cn/article-3631-1.html) ）


但正如我之前说，我们会使用 ip 命令来配置静态 IP 地址。所以，确认你首先检查了当前的 IP 地址。
- `# ip addr show`

![在 CentOS 查看 IP 地址](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151951l7a3ba7m5fm421mt.jpg)

在 CentOS 查看 IP 地址


现在用你的编辑器打开并编辑文件 /etc/sysconfig/network-scripts/ifcfg-enp0s3 （LCTT 译注：你的网卡名称可能不同，如果希望修改为老式网卡名称，参考：[http://www.linux.cn/article-4045-1.html](http://www.linux.cn/article-4045-1.html) ）。这里，我使用
 vi 编辑器，另外你要确保你是 root 用户才能保存更改。
- `# vi /etc/sysconfig/network-scripts/ifcfg-enp0s3`


我们会编辑文件中的四个地方。注意下面的四个地方并保证不碰任何其它的东西。也保留双引号，在它们中间输入你的数据。
- `IPADDR = "[在这里输入你的静态 IP]"`
- `GATEWAY = "[输入你的默认网关]"`
- `DNS1 = "[你的DNS 1]"`
- `DNS2 = "[你的DNS 2]"`


更改了 ‘ifcfg-enp0s3’ 之后，它看起来像下面的图片。注意你的 IP，网关和 DNS 可能会变化，请和你的 ISP(译者注：互联网服务提供商，即给你提供接入的服务的电信或 IDC) 确认。保存并退出。

![网络详情](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151952rjc15ub2ojifg9zn.jpg)

网络详情


重启网络服务并检查 IP 是否和分配的一样。如果一切都顺利，用 Ping 查看网络状态。
- `# service network restart`

![重启网络服务](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151952i6v6rxxezvxq16z6.jpg)

重启网络服务


重启网络后，确认检查了 IP 地址和网络状态。
- `# ip addr show`
- `# ping -c4 google.com`

![验证 IP 地址](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151953ml62hlhry2v0dlg0.jpg)

验证 IP 地址

![检查网络状态](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151953t9kunzlkypyrly5d.jpg)

检查网络状态


（LCTT 译注：关于设置静态 IP 地址的更多信息，请参照：[http://www.linux.cn/article-3977-1.html](http://www.linux.cn/article-3977-1.html) ）

## 3. 设置服务器的主机名称


下一步是更改 CentOS 服务器的主机名称。查看当前分配的主机名称。
- `# echo $HOSTNAME`

![查看系统主机名称](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151953iyz7224vn46t362r.jpg)

查看系统主机名称


要设置新的主机名称，我们需要编辑 ‘/etc/hostsname’ 文件并用想要的名称替换旧的主机名称。
- `# vi /etc/hostname`

![在 CentOS 中设置主机名称](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151954ec4q5i5zphsil445.jpg)

在 CentOS 中设置主机名称


设置完了主机名称之后，务必注销后重新登录确认主机名称。登录后检查新的主机名称。
- `$ echo $HOSTNAME`

![确认主机名称](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151954lb4pd12v448zm4bs.jpg)

确认主机名称


你也可以用 ‘hostname’ 命令查看你当前的主机名。
- `$ hostname`


（LCTT 译注：关于设置静态、瞬态和灵活主机名的更多信息，请参考：[http://www.linux.cn/article-3937-1.html](http://www.linux.cn/article-3937-1.html) ）

### 4. 更新或升级最小化安装的 CentOS


这样做除了更新安装已有的软件最新版本以及安全升级，不会安装任何新的软件。总的来说更新（update）和升级（upgrade）是相同的，除了事实上 升级 = 更新 + 更新时进行废弃处理。
- `# yum update && yum upgrade`

![更新最小化安装的 CentOS 服务器](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151955xcnq8vais7vvuys8.jpg)

更新最小化安装的 CentOS 服务器

重要: 你也可以运行下面的命令，这不会弹出软件更新的提示，你也就不需要输入 ‘y’ 接受更改。


然而，查看服务器上会发生的变化总是一个好主意，尤其是在生产中。因此使用下面的命令虽然可以为你自动更新和升级，但并不推荐。
- `# yum -y update && yum -y upgrade`

### 5. 安装命令行 Web 浏览器


大部分情况下，尤其是在生产环境中，我们通常用没有 GUI 的命令行安装 CentOS，在这种情况下我们必须有一个能通过终端查看网站的命令行浏览工具。为了实现这个目的，我们打算安装名为 ‘links’ 的著名工具。
- `# yum install links`

![安装命令行浏览器](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151955rffl5ihb5s5cjknr.jpg)

Links: 命令行 Web 浏览器


请查看我们的文章 [用 links 工具命令行浏览 Web](http://www.tecmint.com/command-line-web-browsers/) 了解用 links 工具浏览 web 的方法和例子。

### 6. 安装 Apache HTTP 服务器


不管你因为什么原因使用服务器，大部分情况下你都需要一个 HTTP 服务器运行网站、多媒体、用户端脚本和很多其它的东西。
- `# yum install httpd`

![在 CentOS 上安装 Apache](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151956k9sc9h6quq5jdsb6.jpg)

安装 Apache 服务器


如果你想更改 Apache HTTP 服务器的默认端口号(80)为其它端口，你需要编辑配置文件 ‘/etc/httpd/conf/httpd.conf’ 并查找以下面开始的行：
- `LISTEN 80 `


把端口号 ‘80’ 改为其它任何端口(例如 3221)，保存并退出。

![在 CentOS 上更改 Apache 端口](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151956inck0k662l66kc88.jpg)

更改 Apache 端口


增加刚才分配给 Apache 的端口通过防火墙，然后重新加载防火墙。


允许 http 服务通过防火墙(永久)。
- `# firewall-cmd –add-service=http`


允许 3221 号端口通过防火墙(永久)。
- `# firewall-cmd –permanent –add-port=3221/tcp`


重新加载防火墙。
- `# firewall-cmd –reload`


（LCTT 译注：关于 firewall 的进一步使用，请参照：[http://www.linux.cn/article-4425-1.html](http://www.linux.cn/article-4425-1.html) ）


完成上面的所有事情之后，是时候重启 Apache HTTP 服务器了，然后新的端口号才能生效。
- `# systemctl restart httpd.service`


现在添加 Apache 服务到系统层使其随系统自动启动。
- `# systemctl start httpd.service`
- `# systemctl enable httpd.service`


（LCTT 译注：关于 systemctl 的进一步使用，请参照：[http://www.linux.cn/article-3719-1.html](http://www.linux.cn/article-3719-1.html) ）


如下图所示，用 links 命令行工具 验证 Apache HTTP 服务器。
- `# links 127.0.0.1`

![验证 Apache 状态](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/151957kcjayjmqrr0czwmw.jpg)



### 7. 安装 PHP




PHP 是用于 web 基础服务的服务器端脚本语言。它也经常被用作通用编程语言。在最小化安装的 CentOS 中安装 PHP：
- `# yum install php`


安装完 php 之后，确认重启 Apache 服务以便在 Web 浏览器中渲染 PHP。
- `# systemctl restart httpd.service`


下一步，通过在 Apache 文档根目录下创建下面的 php 脚本验证 PHP。
- `# echo -e "<?php\nphpinfo();\n?>"  > /var/www/html/phpinfo.php`


现在在 Linux 命令行中查看我们刚才创建的 PHP 文件(phpinfo.php)。
- `# php /var/www/html/phpinfo.php`
- `或者`
- `# links http://127.0.0.1/phpinfo.php`

![验证 PHP](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160953cmpmppl2bloms2ay.jpg)

验证 PHP

### 8. 安装 MariaDB 数据库


MariaDB 是 MySQL 的一个分支。RHEL 以及它的衍生版已经从 MySQL 迁移到 MariaDB。这是一个主流的数据库管理系统，也是一个你必须拥有的工具。不管你在配置怎样的服务器，或迟或早你都会需要它。在最小化安装的 CentOS 上安装 MariaDB，如下所示：
- `# yum install mariadb-server mariadb`

![安装 MariaDB 数据库](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160954pkzo6xji6rocxwj7.jpg)

安装 MariaDB 数据库


启动 MariaDB 并配置它开机时自动启动。
- `# systemctl start mariadb.service`
- `# systemctl enable mariadb.service`


允许 mysql(mariadb) 服务通过防火墙（LCTT 译注：如果你的 MariaDB 只用在本机，则务必不要设置防火墙允许通过，使用 UNIX Socket 连接你的数据库；如果需要在别的服务器上连接数据库，则尽量使用内部网络，而不要将数据库服务暴露在公开的互联网上。）
- `# firewall-cmd –add-service=mysql`


现在是时候确保 MariaDB 服务器安全了（LCTT 译注：这个步骤主要是设置 mysql 管理密码）。
- `# /usr/bin/mysql_secure_installation`

![保护 MariaDB 数据库](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160955abqcbbp0qb9dbf3s.jpg)

保护 MariaDB 数据库


请阅读:
- [在 CentOS 7.0 上安装 LAMP (Linux, Apache, MariaDB, PHP/PhpMyAdmin)](http://www.tecmint.com/install-lamp-in-centos-7/)
- [在 CentOS 7.0 上创建 Apache 虚拟主机](http://www.tecmint.com/apache-virtual-hosting-in-centos/)

### 9. 安装和配置 SSH 服务器


SSH 即 Secure Shell，是 Linux 远程管理的默认协议。 SSH 是随最小化 CentOS 服务器中安装运行的最重要的软件之一。


检查当前已安装的 SSH 版本。
- `# SSH -V`

![检查 SSH 版本](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160955aaxr9rszr9ryr7ax.jpg)

检查 SSH 版本


使用更安全的 SSH 协议，而不是默认的协议，并更改端口号进一步加强安全。编辑 SSH 的配置文件 ‘/etc/ssh/ssh_config’。


去掉下面行的注释或者从协议行中删除 1，然后行看起来像这样（LCTT 译注： SSH v1 是过期废弃的不安全协议）：
- `# Protocol 2,1 (原来)`
- `Protocol 2 (现在)`


这个改变强制 SSH 使用 协议 2，它被认为比协议 1 更安全，同时也确保在配置中更改端口号 22 为其它。

![保护 SSH 登录](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160955na9cp0f161al1ju6.jpg)

保护 SSH 登录


取消 SSH 中的‘root login’， 只允许通过普通用户账号登录后才能使用 su 切换到 root，以进一步加强安全。请打开并编辑配置文件 ‘/etc/ssh/sshd_config’ 并更改 PermitRootLogin yes 为 PermitRootLogin no。
- `# PermitRootLogin yes (原来) `
- `PermitRootLogin no (现在)`

![取消 SSH Root 登录](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160956m9o99z1zqy79qr5r.jpg)

取消 SSH Root 直接登录


最后，重启 SSH 服务启用更改。
- `# systemctl restart sshd.service`


请查看:
- [加密和保护 SSH 服务器的 5 个最佳实践](http://www.tecmint.com/5-best-practices-to-secure-and-protect-ssh-server/)
- [5 个简单步骤实现使用 SSH Keygen 无密码登录 SSH](http://www.tecmint.com/ssh-passwordless-login-using-ssh-keygen-in-5-easy-steps/)
- [在 PuTTY 中实现 “无密码 SSH 密钥验证”](http://www.tecmint.com/ssh-passwordless-login-with-putty/)

### 10. 安装 GCC (GNU 编译器集)


GCC 即 GNU 编译器集，是一个 GNU 项目开发的支持多种编程语言的编译系统（LCTT 译注：在你需要自己编译构建软件时需要它）。在最小化安装的 CentOS 没有默认安装。运行下面的命令安装 gcc 编译器。
- `# yum install gcc`

![在 CentOS 上安装 GCC](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160956uxzecrzlsrhxtyos.jpg)

在 CentOS 上安装 GCC


检查安装的 gcc 版本。
- `# gcc --version`

![检查 GCC 版本](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160957sykk4j8xq1ougfi4.jpg)

检查 GCC 版本

### 11. 安装 Java


Java是一种通用的基于类的，面向对象的编程语言。在最小化 CentOS 服务器中没有默认安装（LCTT 译注：如果你没有任何 Java 应用，可以不用装它）。按照下面命令从库中安装 Java。
- `# yum install java`

![在 CentOS 上安装 Java](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160957ecefff8tskzucsb8.jpg)

安装 Java


检查安装的 Java 版本。
- `# java -version`

![检查 Java 版本](https://dn-linuxcn.qbox.me/data/attachment/album/201504/27/160957vhc33wkhpr3t9vwg.jpg)










