# 建立SSH隧道（SSH端口转发） - xqhrs232的专栏 - CSDN博客
2017年04月28日 11:25:27[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：298
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.xushulong.com/ssh-tunnel/](http://www.xushulong.com/ssh-tunnel/)
ssh隧道功能，也就是端口转发功能非常好用，可以实现一些代理功能或者是穿透内网功能。
ssh的端口转发(或者叫做隧道)命令分为三种：
> 
本地：ssh -C -f -N -g -L listen_port:DST_Host:DST_port user@Tunnel_Host
远程：ssh -C -f -N -g -R listen_port:DST_Host:DST_port user@Tunnel_Host
动态：ssh -C -f -N -g -D listen_port user@Tunnel_Host
乍一看有点乱，仔细一分析就更蒙了-_-!
但是再深入的分析了搜索出来的相关的说明和别人的一些分享之后，终于搞清楚了使用方法。下面详细的看各个参数的用法。
-L port:host:hostport
将本地机(客户机)的某个端口转发到远端指定机器的指定端口.
```
ssh -C -f -N -g -L listen_port:DST_Host:DST_port user@Tunnel_Host
```
以这个为例，在本地监听一个端口listen_port，把listen_port的所有数据通过Tunnel_Host全部转发到DST_Host:DST_port上去。
-R port:host:hostport
将远程主机(服务器)的某个端口转发到本地端指定机器的指定端口.
```
ssh -C -f -N -g -R listen_port:DST_Host:DST_port user@Tunnel_Host
```
远程端口转发，在Tunnel_Host上面监听一个端口 listen_port，把listen_port的所有数据通过Tunnel_Host全部转发到DST_Host:DST_port上去。
DST_Host:DST_port是对于本机来说的，但却是通过Tunnel_Host来访问的，所以Tunnel_Host必须能访问到DST_Host:DST_port，才能进行转发。
举个例子：我在家里和公司各有一台电脑，均没有公网ip，不能互通，但是我有一个有公网ip的VPS，我的两台电脑都可以访问vps，但是vps不能访问我的两台电脑，然后通过公司电脑ssh到vps建立一条隧道，我在家里就可以访问公司里的电脑了。说起来罗嗦的不得了，但是做起来很容易：
在公司的电脑上：ssh -C -f -N -g -R 8080:127.0.0.1:80 root@vps的ip
然后在家里直接访问vps的8080端口，就相当与访问了公司电脑的80端口了，over。
当然80端口只是web端口，我们可以转发22或者3389端口，就可以远程控制公司的电脑了。
-D port
指定一个本地机器 “动态的’’ 应用程序端口转发. 这个动态的就简单了，
```
ssh -N -f -D 8000 登录名@我的vps的ip
```
就相当与在本机上开了一个sock代理，端口是8000，给浏览器挂上127.0.0.1:8000的代理，查一下本机ip，发现已经变成vps的ip了。
我的上面用的vps是ubuntu系统，有的时候做了远程端口转发之后发现不能从外面访问，需要修改一下vps上面ssh的一个配置，/etc/ssh/sshd_config，把这个改成yes，如果没这句就加上：
```
GatewayPorts yes
```
然后重启sshd。这个作用是把转发的端口绑定在0.0.0.0的接口上，让外部地址也可以访问。
还要注意一点就是listen_port必须是没有被使用的，不然会监听失败。
下面是几个辅助参数，看着乱的话可以忽略，用的时候按需加上就好了。
-C Enable compression.
压缩数据传输。
-f Fork into background after authentication.
后台认证用户/密码，通常和-N连用，不用登录到远程主机。
-N Do not execute a shell or command.
不执行脚本或命令，通常与-f连用。
-g Allow remote hosts to connect to forwarded ports.
在-L/-R/-D参数中，允许远程主机连接到建立的转发的端口，如果不加这个参数，只允许本地主机建立连接。
-p port Connect to this port. Server must be on the same port.
被登录的ssd服务器的sshd服务端口，省略就是默认的22端口。
