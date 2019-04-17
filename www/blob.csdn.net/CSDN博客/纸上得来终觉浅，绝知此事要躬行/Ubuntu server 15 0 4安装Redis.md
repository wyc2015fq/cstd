# Ubuntu server 15.0.4安装Redis - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年08月20日 16:04:00[boonya](https://me.csdn.net/boonya)阅读数：1091标签：[linux																[ubuntu																[redis](https://so.csdn.net/so/search/s.do?q=redis&t=blog)
个人分类：[Memcache/Redis																[Linux](https://blog.csdn.net/boonya/article/category/1740759)](https://blog.csdn.net/boonya/article/category/5904837)





这里整理一下我在Ubuntu server 15.0.4安装Redis 遇到的一些问题和处理方法，以便后续使用做参考。

#### 1、下载Ubuntu server 15.0.4

[http://www.ubuntu.com/download/](http://www.ubuntu.com/download/)

#### 2、安装虚拟机和Ubuntu系统

（1）、虚拟机安装：没有可赘言的；

（2）、Ubuntu安装：只需要记住，安装过程中设置的账号和密码，其他的一步一步安装即可。

#### 3、在Ubuntu上安装GCC工具



```
$sudo apt-get clean
 $sudo apt-get update 
 $sudo apt-get build-dep gcc
```
按照上面处理就可以使用make命令了。







#### 4.启动系统下载并安装Redis过程

##### 4.1下载


`$ wget http://download.redis.io/releases/redis-3.0.3.tar.gz`







##### 4.2解压


` $ tar xzf redis-3.0.3.tar.gz`







##### 4.3安装

先执行$make 再执行 $make install

![](https://img-blog.csdn.net/20150820154539878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```
$ cd redis-3.0.3
$ make
$ make install
```







##### 4.4连接打开Redis服务

使用$sudo su进入用户路径下找到redis下的src路径；

![](https://img-blog.csdn.net/20150820160552767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![](https://img-blog.csdn.net/20150820155216010?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上面拒绝连接因为此时还没启动服务，不能这么操作，执行：$redis-server将数据库启动起来；

![](https://img-blog.csdn.net/20150820155111593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 5、安装过程中遇到的问题

##### 5.1 $make 命令不能执行 

![图片说明](http://img.ask.csdn.net/upload/201508/19/1439976271_745329.png)

  原因：没有安装gcc 

 处理：执行$sudo apt-get build-dep gcc，而不是直接执行$sudo apt-get gcc

##### 5.2 $sudo apt-get update 不能执行

![](https://img-blog.csdn.net/20150820155506956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

  原因：之前执行下载GCC失败，有痕迹。

  处理：执行$sudo apt-get clean

#### 6.将Redis设置为系统启动

参考Redis的三种启动方式：[http://www.tuicool.com/articles/aQbQ3u](http://www.tuicool.com/articles/aQbQ3u)

修改启动文件内容：[http://blog.csdn.net/littlefishzhang/article/details/8203183](http://blog.csdn.net/littlefishzhang/article/details/8203183)

注：暂时没搞出来，文件编辑不是很熟，有时间再研究这个。

#### 7.查看Redis服务器IP对外提供访问

![](https://img-blog.csdn.net/20150820154719432?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这里在客户端就可以访问192.168.28.255:6379;

#### 8.Java客户端代码测试

网上很多示例，就不在此粘贴了；

上述所有问题描述很简略，实际问题可以在我的提问中看（Ubuntu server 15.0.4安装Redis make命令不能执行），地址：[http://ask.csdn.net/questions/201208](http://ask.csdn.net/questions/201208)



﻿﻿](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)




