# Windows下实现端口映射 - xqhrs232的专栏 - CSDN博客
2017年05月03日 11:07:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3957
原文地址::[http://www.linuxidc.com/Linux/2016-09/135489.htm?t=1474790697677](http://www.linuxidc.com/Linux/2016-09/135489.htm?t=1474790697677)
相关文章
1、[Windows xp做nat地址端口映射的几种方法](http://blog.chinaunix.net/uid-683300-id-24921.html)----[http://blog.chinaunix.net/uid-683300-id-24921.html](http://blog.chinaunix.net/uid-683300-id-24921.html)
2、Windows下主机名和IP映射设置----[http://blog.163.com/lively_caicai/blog/static/2092171222013316115029533/](http://blog.163.com/lively_caicai/blog/static/2092171222013316115029533/)
3、Windows端口映射，怎样才能实现外网IP映射成内网IP----[http://hsk.oray.com/news/4422.html](http://hsk.oray.com/news/4422.html)
4、如何做端口映射----[http://jingyan.baidu.com/article/8cdccae947c73e315413cd3e.html](http://jingyan.baidu.com/article/8cdccae947c73e315413cd3e.html)
5、
# [windows下主机名与IP地址的映射](http://blog.csdn.net/caiandyong/article/details/48958303)----[http://blog.csdn.net/caiandyong/article/details/48958303](http://blog.csdn.net/caiandyong/article/details/48958303)

想必大家在Windows下最常用的虚拟机大多是VMware了，那么是否有想过在连接虚拟机的时候出现的端口映射问题，自己使用的情况下还好，有没有想象过在一个局域网中接受虚拟机的远程连接呢？或者你会说，使用桥接模式不就可以了么！是的，桥接模式是可以解决这个问题的，但是在公司额环境下，这样做会造成ip地址的极大浪费，我们需要一个更好的办法，下面我们来看两种方法：
方法一：
1）使用VMware自带的NAT模式进行映射：
打开虚拟机的虚拟网络编辑器
![Windows下实现端口映射](http://www.linuxidc.com/upload/2016_09/16092513139037.png)
选择VMnet8 NAT模式的网卡，点击NAT设置进行编辑：
![Windows下实现端口映射](http://www.linuxidc.com/upload/2016_09/16092513148133.png)
选择添加，进行配置：
![Windows下实现端口映射](http://www.linuxidc.com/upload/2016_09/16092513156927.png)
配置 主机端口(物理机：192.168.1.192：13800)、虚拟机IP地址（192.168.56.137）、虚拟机端口（22） 等的对应关系：
![Windows下实现端口映射](http://www.linuxidc.com/upload/2016_09/16092513165229.png)
配置完成后点击确定进行保存即可。
测试连接，这里使用putty：
![Windows下实现端口映射](http://www.linuxidc.com/upload/2016_09/16092513161959.png)
可以看见，连接到了56.137的虚拟机上：
![Windows下实现端口映射](http://www.linuxidc.com/upload/2016_09/16092513178572.png)
方法二：
其实在Windows系统下，包括xp  2003,2008等，都自带了一个叫做netsh的工具，它自带portproxy的功能。但是只支持tcp协议的端口转发，另外，值得注意的是，作为portproxy的主机需要安装IPV6（必需的）,安装后可以不启用IPV6。
1.配置方法
同上面的例子一样，假设我需要通过192.168.1.192的13800号端口连接192.168.56.137的22号端口，则需要在192.168.1.192主机的命令行键入如下命令：
netsh interface ipv6 install      //如果已经安装IPV6，则可以不执行
如果windows开启了防火墙，需要设置放行TCP 13800的入站规则。
netsh interface portproxy add v4tov4 listenaddress=192.168.1.192 listenport=13800 connectaddress=192.168.56.137 connectport=22
netsh interface portproxy add v4tov4 listenport=13800 connectaddress=192.168.56.137 connectport=22
如果要取消上面配置的端口转发，可以键入下面命令：
netsh interface portproxy delete v4tov4 listenaddress=192.168.1.192 listenport=13800
如果想查看已经配置了哪些端口转发，可以键入下面命令：
netsh interface portproxy show v4tov4  // all
今天就写到这里啦，希望对大家有帮助。
**本文永久更新链接地址**：[http://www.linuxidc.com/Linux/2016-09/135489.htm](http://www.linuxidc.com/Linux/2016-09/135489.htm)

