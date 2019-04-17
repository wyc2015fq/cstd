# linux VPS服务器的一些配置 - 在思索中前行！ - CSDN博客





2015年09月29日 16:30:58[_Tham](https://me.csdn.net/txl16211)阅读数：489









## [SSH密钥登录让Linux VPS/服务器更安全](http://www.vpser.net/security/linux-ssh-authorized-keys-login.html)

2011年01月10日 上午 | 作者：[VPSer](http://www.vpser.net/author/admin)


随着PHP越来越流行，Linux VPS/服务器的使用也越来越多，Linux的安全问题也需要日渐加强，如果你安装过[DenyHosts](http://www.vpser.net/security/denyhosts.html)并设置过邮件提醒，你每天可能会受到数封[DenyHosts](http://www.vpser.net/security/denyhosts.html)Report将前来破解SSH密码的IP加入/etc/hosts.deny。

Linux SSH登录有两种：

**1、使用密码验证登录**

通常VPS或服务器开通后都是直接提供IP和root密码，使用这种方式就是通过密码方式登录。如果密码不够强壮，而且没有安装[DenyHosts](http://www.vpser.net/security/denyhosts.html)之类的[防止SSH密码破解的软件](http://www.vpser.net/security/denyhosts.html)，那么系统安全将存在很大的隐患。

**2、使用密钥验证登录 [查看全文 »](http://www.vpser.net/security/linux-ssh-authorized-keys-login.html#more-1857)**


分类：[VPS安全](http://www.vpser.net/category/security) | [评论(13)](http://www.vpser.net/security/linux-ssh-authorized-keys-login.html#comments) Tags: [Linux
 VPS](http://www.vpser.net/tag/linux-vps),[Putty](http://www.vpser.net/tag/putty),[SSH](http://www.vpser.net/tag/ssh),[xshell](http://www.vpser.net/tag/xshell),[安全](http://www.vpser.net/tag/%E5%AE%89%E5%85%A8),[密钥登录](http://www.vpser.net/tag/%E5%AF%86%E9%92%A5%E7%99%BB%E5%BD%95),[服务器](http://www.vpser.net/tag/%E6%9C%8D%E5%8A%A1%E5%99%A8).



## [SSH远程会话管理工具 - screen使用教程](http://www.vpser.net/manage/screen.html)

2010年10月15日 下午 | 作者：[VPSer](http://www.vpser.net/author/admin)


[VPS侦探](http://www.vpser.net/)在刚接触Linux时最怕的就是SSH远程登录Linux VPS编译安装程序时（比如安装[lnmp](http://lnmp.org/)）网络突然断开，或者其他情况导致不得不与远程SSH服务器链接断开，远程执行的命令也被迫停止，只能重新连接，重新运行。相信现在有些VPSer也遇到过这个问题，今天就给VPSer们介绍一款[远程会话管理工具](http://www.vpser.net/manage/screen.html) - [screen命令](http://www.vpser.net/manage/screen.html)。

## 一、screen命令是什么？

[Screen](http://www.vpser.net/manage/screen.html)是一个可以在多个进程之间多路复用一个物理终端的全屏窗口管理器。Screen中有会话的概念，用户可以在一个screen会话中创建多个screen窗口，在每一个screen窗口中就像操作一个真实的telnet/SSH连接窗口那样。 [查看全文 »](http://www.vpser.net/manage/screen.html#more-1584)


分类：[VPS管理维护](http://www.vpser.net/category/manage) | [评论(9)](http://www.vpser.net/manage/screen.html#comments) Tags: [screen](http://www.vpser.net/tag/screen),[SSH](http://www.vpser.net/tag/ssh),[培训](http://www.vpser.net/tag/%E5%9F%B9%E8%AE%AD),[工具](http://www.vpser.net/tag/%E5%B7%A5%E5%85%B7),[教程](http://www.vpser.net/tag/%E6%95%99%E7%A8%8B),[演示](http://www.vpser.net/tag/%E6%BC%94%E7%A4%BA),[管理](http://www.vpser.net/tag/%E7%AE%A1%E7%90%86),[远程会话](http://www.vpser.net/tag/%E8%BF%9C%E7%A8%8B%E4%BC%9A%E8%AF%9D).



## [Linux SSH远程文件/目录传输命令scp](http://www.vpser.net/manage/scp.html)

2010年08月6日 上午 | 作者：[VPSer](http://www.vpser.net/author/admin)


相信各位[VPSer](http://www.vpser.net/)在使用VPS时会经常在不同VPS间互相备份数据或者转移数据，大部分情况下VPS上都已经安装了Nginx或者类似的web server，直接将要传输的文件放到web
 server的目录，然后在目标机器上执行：wget http://www.vpser.net/testfile.zip 就行了。当VPS上没有安装web server和ftp server的时候或感觉上面的方法比较麻烦，那么用[scp命令](http://www.vpser.net/manage/scp.html)就会排上用场。

## 一、scp是什么？

scp是secure copy的简写，用于在Linux下进行远程拷贝文件的命令，和它类似的命令有cp，不过cp只是在本机进行拷贝不能跨服务器，而且scp传输是加密的。可能会稍微影响一下速度。

## 二、scp有什么用？

1、我们需要获得远程服务器上的某个文件，远程服务器既没有配置ftp服务器，没有开启web服务器，也没有做共享，无法通过常规途径获得文件时，只需要通过[scp命令](http://www.vpser.net/manage/scp.html)便可轻松的达到目的。

2、我们需要将本机上的文件上传到远程服务器上，远程服务器没有开启ftp服务器或共享，无法通过常规途径上传是，只需要通过scp命令便可以轻松的达到目的。 [查看全文 »](http://www.vpser.net/manage/scp.html#more-1432)


分类：[VPS管理维护](http://www.vpser.net/category/manage) | [评论(3)](http://www.vpser.net/manage/scp.html#comments) Tags: [Linux](http://www.vpser.net/tag/linux),[scp](http://www.vpser.net/tag/scp),[SSH](http://www.vpser.net/tag/ssh),[文件传输](http://www.vpser.net/tag/%E6%96%87%E4%BB%B6%E4%BC%A0%E8%BE%93),[目录传输](http://www.vpser.net/tag/%E7%9B%AE%E5%BD%95%E4%BC%A0%E8%BE%93).



## [RAMHOST的VPS小攻略之SSH和OpenVPN安装配置[转]](http://www.vpser.net/build/ramhost-vps-install-ssh-openvpn.html)

2010年06月4日 上午 | 作者：[VPSer](http://www.vpser.net/author/admin)


![](http://www.vpser.net/uploads/2009/12/ramhost-logo.jpg)

这两天买了台入门级的VPS，厂家是[RAMHOST](http://www.ramhost.us/)，详细的清单[这里](http://www.ramhost.us/?page=virtual-dedicated-server)有，他们家的VPS都是OpenVZ上的虚拟机，带个极其简单的web控制面板，可以预装若干种Linux系统，开通了/dev/net/tun和iptables
 SNAT/DNAT，能搭建支持转发的OpenVPN服务。各种型号唯一不同的是内存硬盘和流量，我挑了个倒数第二的micro plan，128M内存，10G空间，150G流量，每月4.99刀，季付。

购买挺简单，先注册帐号，选择机器，然后Payal付钱，等待审核一段时间，开通后他们家给我发了封邮件，整个过程相当简洁。邮件里面有web控制面板的链接，用之前的帐号密码登录，能够看到VPS的IP和VPS所在HOST的IP。预装的系统是Debian 5，我就没动他，保持原样。

然后就是SSH到HOST，用户名和密码都是vz，进去之后根据提示用账号密码登录就能获得root的环境了。这样总归是挺麻烦的，还是先把ssh装上吧，aptitude轻松加愉快。顺手adduser再加上普通权限用户若干，给一个加上sudo的权限(原始安装的deb包相当少，sudo都是要额外装的。)，其他的作为SSH Tunnel的帐号，就不给了。 [查看全文 »](http://www.vpser.net/build/ramhost-vps-install-ssh-openvpn.html#more-1249)


分类：[服务器环境搭建](http://www.vpser.net/category/build) | [评论(6)](http://www.vpser.net/build/ramhost-vps-install-ssh-openvpn.html#comments) Tags: [OpenVPN](http://www.vpser.net/tag/openvpn),[ramhost](http://www.vpser.net/tag/ramhost),[RamHost.us](http://www.vpser.net/tag/ramhost-us),[SSH](http://www.vpser.net/tag/ssh),[VPS](http://www.vpser.net/tag/vps),[安装配置](http://www.vpser.net/tag/%E5%AE%89%E8%A3%85%E9%85%8D%E7%BD%AE).



## [Linux VPS延长SSH连接时间设置](http://www.vpser.net/manage/linux-vps-ssh-clientalivecountmax.html)

2009年08月29日 下午 | 作者：[VPSer](http://www.vpser.net/author/admin)


最近公司刚刚买了一台hyper-v的VPS，但是连接SSH服务器刚刚离开一会就断开，不得不需要反复连接服务器，为了使SSH服务器可以保持足够的连接时间，大家可以按以下方法设置:

基于安全的理由，如果用户连线到 SSH Server 后闲置，SSH Server 会在超过特定时间后自动终止 SSH 连线。以下是设定终止连线时间的方法:

1、打开 /etc/ssh/sshd_config 文件，找到一个参数为 ClientAliveCountMax，它是设定用户端的 SSH 连线闲置多长时间后自动终止连线的数值，单位为分钟。

2、如果这一行最前面有#号，将那个#号删除，并修改想要的时间。

3、修改后保存并关闭文件，重新启动 sshd:

/etc/rc.d/init.d/sshd restart


分类：[VPS管理维护](http://www.vpser.net/category/manage) | [评论(3)](http://www.vpser.net/manage/linux-vps-ssh-clientalivecountmax.html#comments) Tags: [Linux](http://www.vpser.net/tag/linux),[SSH](http://www.vpser.net/tag/ssh),[VPS](http://www.vpser.net/tag/vps),[延长](http://www.vpser.net/tag/%E5%BB%B6%E9%95%BF),[连接时间](http://www.vpser.net/tag/%E8%BF%9E%E6%8E%A5%E6%97%B6%E9%97%B4).



## [SSH 登录太慢的解决方法](http://www.vpser.net/manage/ssh-login.html)

2009年07月29日 上午 | 作者：[VPSer](http://www.vpser.net/author/admin)


SSH 登录太慢可能是 DNS 解析的问题，默认配置下 sshd 初次接受 ssh 客户端连接的时候会自动反向解析客户端 IP 以得到 ssh 客户端的域名或主机名。如果这个时候 DNS 的反向解析不正确，sshd 就会等到 DNS 解析超时后才提供 ssh 连接，这样就造成连接时间过长、ssh 客户端等待的情况，一般为10-30秒左右。有个简单的解决办法就是在 sshd 的配置文件（sshd_config）里取消
 sshd 的反向 DNS 解析。

# vi /etc/ssh/sshd_config

找到UseDNS，修改为no

UseDNS no

重启SSH服务

# /etc/init.d/sshd restart





