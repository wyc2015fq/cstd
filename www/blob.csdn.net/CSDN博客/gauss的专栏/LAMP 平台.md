# LAMP 平台 - gauss的专栏 - CSDN博客
2013年06月19日 22:16:43[gauss](https://me.csdn.net/mathlmx)阅读数：588
## 1.Web应用软件
[编辑本义项](http://baike.baidu.com/view/365086.htm#)[](http://baike.baidu.com/view/365086.htm#)
# lamp 
#### 百科名片
![lamp](http://c.hiphotos.baidu.com/baike/s%3D220/sign=f22b3b3fbaa1cd1101b675228913c8b0/a50f4bfbfbedab6409b892cff736afc378311ef2.jpg)[](http://baike.baidu.com/picview/365086/10014223/0/a8362712b9ec5a6ef919b81c.html)
lamp
Linux+Apache+Mysql+Perl/PHP/Python一组常用来搭建[动态网站](http://baike.baidu.com/view/863075.htm)或者服务器的[开源软件](http://baike.baidu.com/view/444964.htm)，本身都是各自独立的程序，但是因为常被放在一起使用，拥有了越来越高的兼容度，共同组成了一个强大的Web[应用程序](http://baike.baidu.com/view/330120.htm)平台。随着开源潮流的蓬勃发展，开放源[代码](http://baike.baidu.com/view/41.htm)的LAMP已经与[J2EE](http://baike.baidu.com/view/1507.htm)和[.Net](http://baike.baidu.com/view/4294.htm)[商业软件](http://baike.baidu.com/view/194971.htm)形成三足鼎立之势，并且该软件开发的项目在软件方面的投资成本较低，因此受到整个[IT](http://baike.baidu.com/view/30.htm)界的关注。从网站的流量上来说，70%以上的访问流量是LAMP来提供的，LAMP是最强大的网站解决方案．
目录
[WEB应用程序](http://baike.baidu.com/view/365086.htm#1)[LAMP网站架构方案介绍](http://baike.baidu.com/view/365086.htm#2)[LAMP安装](http://baike.baidu.com/view/365086.htm#3)展开[WEB应用程序](http://baike.baidu.com/view/365086.htm#1)[LAMP网站架构方案介绍](http://baike.baidu.com/view/365086.htm#2)[LAMP安装](http://baike.baidu.com/view/365086.htm#3)展开
## [编辑本段](http://baike.baidu.com/view/365086.htm#)WEB应用程序
### LAMP介绍
**LAMP**指的[Linux](http://baike.baidu.com/view/1634.htm)（[操作系统](http://baike.baidu.com/view/880.htm)）、[Apache](http://baike.baidu.com/view/28283.htm)HTTP
 服务器，[MySQL](http://baike.baidu.com/view/24816.htm)（[数据库软件](http://baike.baidu.com/view/1198685.htm)） 和[PHP](http://baike.baidu.com/view/99.htm)（有时也是指[Perl](http://baike.baidu.com/view/46614.htm)或[Python](http://baike.baidu.com/view/21087.htm)）
 的第一个字母，一般用来建立web 服务器。
虽然这些[开放源代码](http://baike.baidu.com/view/1708.htm)程序本身并不是专门设计成同另几个程序一起工作的，但由于它们的免费和开源，这个组合开始流行（大多数Linux发行版本捆绑了这些软件）。当一起使用的时候，它们表现的像一个具有活力的解决方案包。其他的方案包有苹果的[WebObjects](http://baike.baidu.com/view/4902264.htm)（最初是应用服务器），Java/J2EE和微软的.NET架构。
LAMP包的[脚本](http://baike.baidu.com/view/54.htm)组件中包括了CGIweb接口，它在90年代初期变得流行。这个技术允许[网页浏览器](http://baike.baidu.com/view/455968.htm)的用户在服务器上执行一个程序，并且和接受[静态](http://baike.baidu.com/view/612026.htm)的内容一样接受动态的内容。[程序员](http://baike.baidu.com/view/39175.htm)使用[脚本语言](http://baike.baidu.com/view/76320.htm)来创建这些程序因为它们能很容易有效的操作文本流，甚至当这些文本流并非源自程序自身时也是。正是由于这个原因系统设计者经常称这些[脚本语言](http://baike.baidu.com/view/76320.htm)为[胶水语言](http://baike.baidu.com/view/2993364.htm)。
### 软件组件
**Linux**
Linux 是免费[开源软件](http://baike.baidu.com/view/444964.htm)，这意味着源[代码](http://baike.baidu.com/view/41.htm)可用的[操作系统](http://baike.baidu.com/view/880.htm)。
**Apache**
Apache 是使用中最受欢迎的一个[开放源码](http://baike.baidu.com/view/394804.htm)的[WEB服务器](http://baike.baidu.com/view/460250.htm)软件。
**MySQL**
MySQL 是[多线程](http://baike.baidu.com/view/65706.htm)、多用户的SQL[数据库管理系统](http://baike.baidu.com/view/68446.htm)。
MySQL 已拥有由[Oracle](http://baike.baidu.com/view/15020.htm)公司自 2010 年 1 月 27 日通过[SUN](http://baike.baidu.com/view/24856.htm)购买。SUN 最初于 2008 年 2 月 26 日收购 MySQL。
**PHP，Perl 或 Python**
PHP 是一种[编程语言](http://baike.baidu.com/view/552871.htm)最初设计生产[动态网站](http://baike.baidu.com/view/863075.htm)。PHP 是主要用于服务器端的[应用程序](http://baike.baidu.com/view/330120.htm)软件。Perl
 和 Python 类似。
### 来源历史
Michael Kunze在一篇为德国电脑杂志c't（1998，第12期，230页）而写的文章中使用了缩略语*LAMP*。这篇文章意在展示一系列的自由软件成为了商业包的替换物。由于IT世界众所周知的对缩写的爱好，Kunze提出*LAMP*这一容易被市场接受的术语来普及[自由软件](http://baike.baidu.com/view/20965.htm)的使用。
举例来说，[Wikipedia](http://baike.baidu.com/view/1637.htm)，免费自由的百科全书，运行的一系列软件具有LAMP环境一样的特点。Wikipedia使用MediaWiki软件，主要在Linux下开发，由Apache HTTP服务器提供内容，在MySQL数据库中存储内容，PHP来实现程序逻辑。
### 经典LAMP网站架构分析
||System|Server|Storage|Script|
|----|----|----|----|----|
|Yahoo|FreeBSD + Linux|Apache|MySQL|PHP|
|Facebook|FreeBSD|Apache|MySQL + Memcached|PHP|
|Wikimedia|Linux|Apache + Lighttpd|MySQL + Memcached|PHP|
|Flickr|Redhat Linux|Apache|MySQL + Memcached|PHP+ Perl|
|Sina|FreeBSD + Solaris|Apache + Nginx|MySQL + Memcached|PHP|
|YouTube|Suse Linux|Apache + Lighttpd|MySQL|Python|
## [编辑本段](http://baike.baidu.com/view/365086.htm#)LAMP网站架构方案介绍
LAMP（Linux- Apache-MySQL-PHP）网站架构是目前国际流行的Web框架，该框架包括：Linux操作系统，Apache网络服务器，MySQL数据 库，Perl、PHP或者Python编程语言，所有组成产品均是开源软件，是国际上成熟的架构框架，很多流行的商业应用都是采取这个架构，和 Java/J2EE架构相比，LAMP具有Web资源丰富、轻量、快速开发等特点，微软的.NET架构相比，LAMP具有通用、跨平台、高性能、低价格的 优势，因此LAMP无论是性能、质量还是价格都是企业搭建网站的首选平台。
对于大流量、大并发量的网站系统架构来说，除了硬件上使用高 性能的服务器、负载均衡、CDN等之外，在软件架构上需要重点关注下面几个环节：使用高性能的操作系统（OS）、高性能的网页服务器（Web Server）、高性能的数据库（Database）、高效率的编程语言等。下面我将从这几点对其一一讨论。
**操作系统**
Linux操作系统有很多个不同的发行版，如Red Hat Enterprise Linux、SUSE Linux Enterprice、Debian、Ubuntu、CentOS等，每一个发行版都有自己的特色，比如RHEL的稳定，Ubuntu的易用，基于稳定性 和性能的考虑，操作系统选择CentOS（Community ENTerprise Operating System）是一个理想的方案。
CentOS（Community ENTerprise Operating System）是Linux发行版之一，是RHEL/Red Hat Enterprise Linux的精简免费版，和RHEL为同样的源代码，不过，RHEL和SUSE LE等企业版，提供的升级服务均是收费升级，无法免费在线升级，因此要求免费的高度稳定性的服务器可以用CentOS替代Red Hat Enterprise Linux使用。
**Web服务器、缓存和PHP加速**
Apache是LAMP架构最核心的Web Server，开源、稳定、模块丰富是Apache的优势。但Apache的缺点是有些臃肿，内存和CPU开销大，性能上有损耗，不如一些轻量级的Web 服务器（例如nginx）高效，轻量级的Web服务器对于静态文件的响应能力来说远高于Apache服务器。
Apache做为Web Server是负载PHP的最佳选择，如果流量很大的话，可以采用nginx来负载非PHP的Web请求。nginx是一个高性能的HTTP和反向代理服 务器，Nginx以它的稳定性、丰富的功能集、示例配置文件和低系统资源的消耗而闻名。Nginx不支持PHP和CGI等动态语言，但支持负载均衡和容 错，可和Apache配合使用，是轻量级的HTTP服务器的首选。
Web服务器的缓存也有多种方案，Apache提供了自己的缓存模 块，也可以使用外加的Squid模块进行缓存，这两种方式均可以有效的提高Apache的访问响应能力。Squid Cache是一个Web缓存服务器，支持高效的缓存，可以作为网页服务器的前置cache服务器缓存相关请求来提高Web服务器的速度，把Squid放在 Apache的前端来缓存Web服务器生成的动态内容，而Web应用程序只需要适当地设置页面实效时间即可。如访问量巨大则可考虑使用memcache作 为分布式缓存。
PHP的加速使用eAccelerator加速器，eAccelerator是一个自由开放源码PHP加速器，优化和动 态内容缓存，提高了性能PHP脚本的缓存性能，使得PHP脚本在编译的状态下，对服务器的开销几乎完全消除。它还有对脚本起优化作用，以加快其执行效率。 使PHP程序代码执效率能提高1-10倍。
具体的解决方案有以下几种：
1、squid + Apache + PHP + eAccelerator
使用Apache负载PHP，使用squid进行缓存，html或图片的请求可以直接由squid返回给用户。很多大型网站都采用这种架构。
2、nginx/Apache + PHP（fastcgi） + eAccelerator
使用nginx或Apache负载PHP，PHP使用fastcgi方式运行，效率较高。
3、nginx + Apache + PHP + eAccelerator
此方案综合了nginx和Apache的优点，使用Apache负载PHP，nginx负责解析其他Web请求，使用nginx的rewrite模块，Apache端口不对外开放。
**数据库**
开源的数据库中，MySQL在性能、稳定性和功能上是首选，可以达到百万级别的数据存储，网站初期可以将MySQL和Web服务器放在一起，但是当访问 量达到一定规模后，应该将MySQL数据库从Web Server上独立出来，在单独的服务器上运行，同时保持Web Server和MySQL服务器的稳定连接。
当数据库访问量达到更大的级别，可以考虑使用MySQL Cluster等数据库集群或者库表散列等解决方案。
总的来说，LAMP架构的网站性能会远远优于Windows IIS + ASP + Access（例如月光博客）这样的网站，可以负载的访问量也非常大，国内的大量个人网站如果想要支撑大访问量，采用LAMP架构是一个不错的方案。
综上所述，基于LAMP架构设计具有成本低廉、部署灵活、快速开发、安全稳定等特点，是Web网络应用和环境的优秀组合。[1]
## [编辑本段](http://baike.baidu.com/view/365086.htm#)LAMP安装
### 适用环境
系统支持：CentOS-5 (32bit/64bit)、CentOS-6 (32bit/64bit)
内存要求：≥256M
### 安装了什么
1、Apache 2.2.22或Apache 2.4.2
2、MySQL 5.5.24
3、PHP 5.2.17或PHP 5.3.13
4、[php](http://baike.baidu.com/view/99.htm)myadmin 3.5.1
5、ZendOptimizer 3.3.9(可选，只适合PHP 5.2.17)
6、xcache 1.3.2(可选)
7、pure-ftpd-1.0.36（可选）
### 如何安装
安装方法见参考资料[2]
### 安装其它
1、执行脚本pureftpd. sh安装pure-ftpd。
2、执行脚本zend. sh安装ZendOptimizer。
3、执行脚本xcache. sh安装xcache。
### 使用提示
LAMP脚本使用
lamp add(del,list)：创建（删除，列出）[虚拟主机](http://baike.baidu.com/view/7383.htm)。
lamp ftp(add|del|list)：创建（删除，列出）ftp用户。
lamp uninstall：一键[卸载](http://baike.baidu.com/view/386432.htm)lamp（卸载之前注意备份好数据！）。
程序目录
[mysql](http://baike.baidu.com/view/24816.htm)目录: /usr/local/mysql
[mysql](http://baike.baidu.com/view/24816.htm)data目录：/usr/local/mysql/data
[php](http://baike.baidu.com/view/99.htm)目录: /usr/local/php
[apache](http://baike.baidu.com/view/28283.htm)目录： /usr/local/apache
[进程管理](http://baike.baidu.com/view/364947.htm)
[apache](http://baike.baidu.com/view/28283.htm)启动（停止|重启|重载配置文件）:service httpd start(stop|restart|reload)
[mysql](http://baike.baidu.com/view/24816.htm)（停止|重启|重载配置文件）:service mysqld start(stop|restart|reload)
[vsftpd](http://baike.baidu.com/view/1806035.htm)（停止|重启|重载配置文件）:service vsftpd start(stop|restart|reload)
配置文件路径
[apache](http://baike.baidu.com/view/28283.htm):/etc/httpd/conf
[mysql](http://baike.baidu.com/view/24816.htm):/etc/my.cnf
[php](http://baike.baidu.com/view/99.htm):/etc/php.ini /etc/php.d
pure-ftpd:/etc/pure-ftpd.conf
使用注意
1、[mysql](http://baike.baidu.com/view/24816.htm)root密码存放在/root/my.cnf文件中，添加[虚拟主机](http://baike.baidu.com/view/7383.htm)的时候需要调用。如果修改了root密码，请[手动更新](http://baike.baidu.com/view/7073469.htm)my.cnf文件。
- 参考资料
- 
1．[](http://baike.baidu.com/view/365086.htm#ref_%5B1%5D_10014223)[LAMP网站架构方案介绍](http://www.ios100.net/?p=3103)．博海时空．2012-09-24[引用日期2012-09-24]．
- 
2．[](http://baike.baidu.com/view/365086.htm#ref_%5B2%5D_10014223)[lamp一键安装](http://www.centos.bz/lamp/)．centos[引用日期2012-06-12]．
