# ssh 连接缓慢解决方法 - xqhrs232的专栏 - CSDN博客
2017年06月29日 14:57:02[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：194
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.chinaunix.net/uid-16728139-id-3435980.html](http://blog.chinaunix.net/uid-16728139-id-3435980.html)
相关文章
1、[SSH连接慢与反向解析（转）](http://blog.csdn.net/tylz04/article/details/9041717)----[http://blog.csdn.net/tylz04/article/details/9041717](http://blog.csdn.net/tylz04/article/details/9041717)
2、SSHD服务取消DNS反向解析 SSH连接慢----[http://www.tuicool.com/articles/zemIZj](http://www.tuicool.com/articles/zemIZj)
3、
# [SSH 连接慢与反向解析](http://blog.csdn.net/tianlesoftware/article/details/6103857)----[http://blog.csdn.net/tianlesoftware/article/details/6103857](http://blog.csdn.net/tianlesoftware/article/details/6103857)
4、SSH登录过慢怎么办？取消ssh的DNS反解----[http://www.zxsdw.com/index.php/archives/1078](http://www.zxsdw.com/index.php/archives/1078/)
# [/](http://www.zxsdw.com/index.php/archives/1078/)
# 5、
SSHD服务取消DNS反向解析 SSH连接慢----[http://www.tuicool.com/articles/zemIZj](http://www.tuicool.com/articles/zemIZj)
重新安装机器的情况下我们经常会遇到的一个情况是telnet到server速度很快，但是ssh连接的时候却很慢，大概
要等半分钟甚至更久。ping的速度也非常好，让人误以为是ssh连接不上。
　　下面说下如何解决这样的问题，最为常见的原因是因为server的sshd会去DNS查找访问
client IP的hostname，如果DNS不可用或者没有相关记录，就会耗费大量时间。
　　1、在server上/etc/hosts文件中把你本机的ip和hostname加入
　　2、在server上/etc/ssh/sshd_config文件中修改或加入UseDNS=no，另外在authentication gssapi-with-mic也有可能出现问题，在server上/etc/ssh/sshd_config文件中修改GSSAPIAuthentication no.
3、修改server上/etc/nsswitch.conf中hosts为hosts： files
　 4、reboot server使配置生效
　/etc/init.d/sshd restart重启sshd进程使配置生效。
项目组的服务器，通过SSH服务远程访问Linux服务器，总是等待很久才能登陆。
试过下面的方法，再通过SSH服务远程访问Linux服务器，瞬间登陆。。。甚是开心。。。
vi /etc/ssh/sshd_config
关闭 SSH 的 DNS 反解析,添加下面一行:
UseDNS no
※虽然配置文件中[UseDNS yes]被注释点，
但默认开关就是yes...(SSH服务默认启用了DNS反向解析的功能)
#############################################################
上网查了相关的资料：↓
在目标服务器上有一个文件/etc/nsswitch.conf ，里面有如下一行
hosts： files dns
这行的含义是对于访问的主机进行域名解析的顺序，是先访问file，也就是/etc/hosts文件，如果hosts中没有记录域名，则访问dns，进行域名解析，如果dns也无法访问，就会等待访问超时后返回，因此等待时间比较长。那如果将这一行屏蔽掉是不是也可以达到同样的效果呢？应该是可以的，但是如果本机要通过域名访问其他服务器，则肯定无法访问，因此这行应该需要保留。这个问题也提示我们，dns如果不可用，会带来的一些副作用的。
相信很多朋友在使用Linux系统的时候因为安全性的原因摒弃了telnet rlogin 或者X-window，而把openssh作为自己默认的远程登录方式。然而经常会遇到的一个情况是telnet到server速度很快，但是ssh连接的时候却很慢，大概要等半分钟甚至更久。ping的速度也非常好，让人误以为是ssh连接不上。下面说下如何解决这样的问题，最为常见的原因是因为server的sshd会去DNS查找访问clientIP的hostname，如果DNS不可用或者没有相关记录，就会耗费大量时间。
1, 在server上/etc/hosts文件中把你本机的ip和hostname加入
2, 在server上/etc/ssh/sshd_config文件中修改或加入UseDNS=no(这个方法试过了，好用)
3, 注释掉server上/etc/resolv.conf中所有行（关于这个方法：另一台服务器虽然没有按照方法2修改配置文件sshd_config，但是在这台服务器resolv.conf为空，也就是说这个方法也好用） 
4, 修改server上/etc/nsswitch.conf中hosts为hosts: files
5, reboot server使配置生效
#############################################################

