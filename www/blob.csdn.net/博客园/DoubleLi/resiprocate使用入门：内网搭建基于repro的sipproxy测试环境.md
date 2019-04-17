# resiprocate使用入门：内网搭建基于repro的sipproxy测试环境 - DoubleLi - 博客园






# 测试环境

sipproxy：repro + centos 
客户端：windows电脑客户端使用X-Lite，手机andriod客户端使用linphone

# repro配置和启动

## log的配置

如果使用默认的log配置，则log级别是INFO，且所有log都打印到标准输出。如果要输出到文件，则做如下类似配置：

```
########################################################
# Log settings
########################################################
LoggingType = file
LogLevel = STACK
#指定log文件路径
LogFilename = ../log/repro.log
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7

## web地址设置

如果要通过网页操作repro的相关配置，则需要将HTTP的地址改成repro所在机器的ip。端口默认配置是5080。我的repro机器ip是172.16.12.29，则配置如下：

```
HttpBindAddress = 172.16.12.29, ::1
```
- 1
- 2

## 启动repro

./repro

# 创建domain、user以及route

浏览器登陆repro的控制页面：
`172.16.12.9:5080`- 1

提示输入用户名和密码，默认都是admin，登陆成功后点击login，界面如下: 
![这里写图片描述](http://img.blog.csdn.net/20170122112101237?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzM1OTcxOTQzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 创建domain

任何user都必须要属于某个domain，repro会根据domain去路由下一跳。创建domain界面如下，端口可以不填，域名随便输入。 创建domain后，会显示在下面的列表中。 
![这里写图片描述](http://img.blog.csdn.net/20170122111927361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzM1OTcxOTQzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

特别注意的是： 
（1）创建domain后，需要重启repro。 
（2）域名可以是字符串，也可以直接使用repro 的ip地址。如果是字符串，就需要配置DNS规则。比如我创建 test.sipproxy.cn，则需要将这个域名DNS到proxy上。最简单的就是配置到/etc/hosts上。否则会出现480 NO DNS错误。

## 创建user

输入用户名，选择domain,输入密码，add即可。 
![这里写图片描述](http://img.blog.csdn.net/20170122112126556?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzM1OTcxOTQzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 创建route

这一步不是必要的，但是我们拨打的时候，习惯输入数字号码，而不是用户名。所以通过创建route来实现数字号码到用户名的映射。按照提示正确输入URI和Destination。 
![这里写图片描述](http://img.blog.csdn.net/20170122112139884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzM1OTcxOTQzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 客户端登陆

经过以上步骤，就有用户了： 
![这里写图片描述](http://img.blog.csdn.net/20170122112157385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzM1OTcxOTQzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

比如我有两个用户：test3@test.sipproxy.cn和test4@test.sipproxy.cn 
对应的号码分别是33333和44444

> 
电脑上X-lite登陆：


![这里写图片描述](http://img.blog.csdn.net/20170122112215151?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzM1OTcxOTQzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 
手机上linphone登陆:


![这里写图片描述](http://img.blog.csdn.net/20170122112229932?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzM1OTcxOTQzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 
登陆成功后，repro控制页面上可以看到相关登陆注册的信息： 
![这里写图片描述](http://img.blog.csdn.net/20170122112243683?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzM1OTcxOTQzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# 测试结果

在test3上拨打号码44444，拨打成功。抓包，flow图如下： 
![这里写图片描述](http://img.blog.csdn.net/20170122112256988?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYzM1OTcxOTQzNQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 遇到的问题

> 
（1）域名一定要配置DNS

（2）如果域名用的是repro的ip，则不要改动repro的默认端口5060。我的测试中一开始由于5060被其他应用占用，将repro的端口改成5066，配置ip域名打不通。因为repro代码中从域名中解析不到端口，就会使用默认的端口5060，这个是硬编码写死的。也就是说会将主叫的invite路由到5060端口去，导致通话失败。非要用ip当域名且改变端口，可以在域名上加上端口，如172.16.12.29:5066。

（3）在repro端口上抓包，通话结束时，竟然没有抓到bye包。 
这个的原因是ack和bye包，主叫和被叫直接发送了，没有经过repro。因为bye是不同的事务，bye的路径依赖于RecordRoute，所以这个是repro没有将自己插入到RecordRoute导致的。这个在配置中将ForceRecordRouting配成true，且RecordRouteUri配上就可以了。










