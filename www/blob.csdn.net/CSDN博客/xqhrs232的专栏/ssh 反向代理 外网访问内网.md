# ssh 反向代理 外网访问内网 - xqhrs232的专栏 - CSDN博客
2017年04月27日 14:31:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：833
原文地址::[http://www.open-open.com/lib/view/open1434007270286.html](http://www.open-open.com/lib/view/open1434007270286.html)
相关文章
1、通过SSH反向代理，远程登录局域网主机----[https://segmentfault.com/a/1190000007512451](https://segmentfault.com/a/1190000007512451)
2、[年三十折腾极路由之SSH反向代理](http://spaces.ac.cn/archives/3604/)----[http://spaces.ac.cn/archives/3604/](http://spaces.ac.cn/archives/3604/)
通过SSH反向代理，远程登录局域网主机
先明确一下概念
  外网是有固定ip的机器，ssh可以之间连接上；
  内网是类似公司局域网之类，有独立ip但是不能ssh，需要管理员做端口转发等等权限性操作才可以。
文中用内网表示需要被访问的局域网内电脑，用外网表示局域网外电脑。
# 一、首先，查看外网现有的ssh端口开放情况。
[zhipeng@zhangzhipeng2023.cn ~]$sudo netstat -anopl | grep ssh
[sudo] password for zhipeng:
tcp     0    0 0.0.0.0:22        0.0.0.0:*        LISTEN    22645/sshd      off (0.00/0/0)
-------------------------------------------------------------------
内网ssh连接外网：
ssh -C -R （vpsPort ）:（vpsHostname ）:
 （mySSHPort ）-p（vpsSSHPort ）（vpsUser ）@（vpsIP ）
参数说明： 
-C 数据压缩；
-R vpsPort 要绑定的外网端口；
vpsHostname 给外网设置主机名；
mySSHPort 内网ssh服务的端口。
-p vpsSSHPort 外网ssh服务端口
vpsUser 外网用户名
vpsIP 外网用户名
neiwang@ZHIPENG-PC ~ $ ssh -C -R 3345:localhost:22 -p 22 zhipeng@VPSIP
Last login: Tue Jun  9 13:12:51 2015 from NeiWangIP
Welcome to WaiWang Compute Service!
# OK，成功登陆到外网服务器上
# 二、现在再查看一下外网的SSH端口状态
可以看到，外网已经在监听3344端口，并且和内网电脑建立了ssh连接
[zhipeng@zhangzhipeng2023.cn ~]$sudo netstat -anopl | grep ssh
[sudo] password for zhipeng:
tcp     0    0 127.0.0.1:3345      0.0.0.0:*        LISTEN    22799/sshd: zhipeng  off (0.00/0/0)
tcp     0    0 0.0.0.0:22        0.0.0.0:*        LISTEN    22645/sshd      off (0.00/0/0)
tcp     0   36 VPSIP:22    NeiWangIP:11694    ESTABLISHED 22796/sshd: zhipeng  on (0.38/0/0)
# 三、 接着，试一下从外网ssh内网
$ssh -C  -N -D（vpsPort ） -p（mySSHPort ）
 （myHostname ）@（vpsHostname ）
参数我就不解释了，看名字和第一步对比就知道了 myName是内网用户名
我这里加-f -N 是会报错的，如下：
[zhipeng@zhangzhipeng2023.cn ~]$ssh -C  -f -N -D 3345 -p 3345 neiwang@localhost
The authenticity of host '[localhost]:3345 ([127.0.0.1]:3345)' can't be established.
ECDSA key fingerprint is 93:e8:a5:fa:1a:3e:0e:19:6e:c3:28:af:f7:9d:15:3c.
Are you sure you want to continue connecting (yes/no)? yes
Warning: Permanently added '[localhost]:3345' (ECDSA) to the list of known hosts.
neiwang@localhost's password:
bind: Cannot assign requested address
channel_setup_fwd_listener: cannot listen to port: 3345
Could not request local forwarding.
[zhipeng@zhangzhipeng2023.cn ~]$sudo ssh -C -D 3345 -p 3345 neiwang@localhost
neiwang@localhost's password: #输入内网密码
bind: Cannot assign requested address
channel_setup_fwd_listener: cannot listen to port: 3345
Could not request local forwarding.
Last login: Tue Jun  9 13:18:53 2015 from 127.0.0.1
[2015-06-09 13:21.51]  ~
[neiwang.neiwang-PC] 0bB logout
Connection to localhost closed.
ssh反向代理大功告成。
题外话，也不能全算题外话，很重要的
0、安全问题，慎用。
1、别忘了打开内网的sshd服务
2、外网iptables记得打开端口（防火墙）
3、如果内网电脑是windows，记得关闭防火墙，或者设置好端口访问，不然ssh 22端口会访问异常无法连接。
来自：[http://blog.sina.com.cn/s/blog_83dc494d0102vph3.html](http://blog.sina.com.cn/s/blog_83dc494d0102vph3.html)
通过SSH反向代理，远程登录局域网主机
