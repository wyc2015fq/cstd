# Windows向Ubuntu server传递文件 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月10日 17:52:45[boonya](https://me.csdn.net/boonya)阅读数：2482








### 1.开启SSH服务



1.1判断是否开启ssh



先使用netstat -tl或service ssh status查看ssh服务是否开启，如果没有开启，用service ssh restart开启，如果没有安装，使用sudo apt-get install openssh-server安装ssh

1.2检查防火墙

注：可以使用iptables，但我在ubuntu中没找到，会用的同学可以忽略下面的东西。Ubuntu在它的发行版中，附带了一个相对iptables简单很多的防火墙配置工具：ufw

使用如下命令：
$sudo ufw default deny
$sudo ufw enable

通过第一命令，我们设置默认的规则为deny， 这样除非指明打开的端口， 否则所有端口默认都是关闭的。第二个命令则启动了ufw。如果下次重新启动机器， ufw也会自动启动。

1.3开启22端口



$sudo ufw allow 22

由于在/etc/services中， 22端口对应的服务名是ssh。所以下面的命令是一样的：

$sudo ufw allow ssh

现在可以通过下面命令来查看防火墙的状态了：

$sudo ufw status

– —— —-

22:tcp ALLOW Anywhere

22:udp ALLOW Anywhere

可以看到，22端口的tcp和udp协议都打开了。

### 2.putty和pscp

2.1下载putty和pscp

下载地址：[http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html](http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html)

![](https://img-blog.csdn.net/20150610172050808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2.2putty登录测试ssh

![](https://img-blog.csdn.net/20150610174304905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

测试登录：

![](https://img-blog.csdn.net/20150610175105929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注：有个疑问，root用户不能在此登录，总是提示Access defined!

2.3pscp命令上传文件

将pscp.exe放置于系统磁盘Window/System32路径下，否则会出现pscp命令不能使用的情况：

![](https://img-blog.csdn.net/20150610173601776?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注：上面的命令不正确，且pscp.exe未放置在System32目录下。

2.4pscp命令规范

![](https://img-blog.csdn.net/20150610174134948?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2.5上传文件成功示例

![](https://img-blog.csdn.net/20150610174329506?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注：服务器上应该是用户对应于/home/username的用户文件夹，而不是root用户对应的/home，要直接往/home下写东西需要超级用户权限.

更多可以参考：[两台Linux系统之间传输文件的几种方法](https://blog.csdn.net/gatieme/article/details/51673229)



