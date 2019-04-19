# Windows下搭建Redis服务器 - xqhrs232的专栏 - CSDN博客
2018年09月30日 16:53:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：316
原文地址::[https://blog.csdn.net/e62ces0iem/article/details/73477182](https://blog.csdn.net/e62ces0iem/article/details/73477182)
Redis服务器是当下比较流行的缓存服务器，Redis通常被人拿来和Memcached进行对比。在我看来，应当是各具优势吧，虽然应用场景基本类似，但总会根据项目的不同来进行不通的选用。
我们今天主要讲Redis在windows平台下的安装和部署。进入正题->
一、单机部署使用Redis
1、下载Redis，我选用的是当下的最新版本3.2.0 地址在这里[https://github.com/MSOpenTech/redis/releases](https://github.com/MSOpenTech/redis/releases)
下载好之后，直接解压到任意位置，我存放在了 D:\Program Files\Redis-x64-3.2.100
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605150536543-398834377.png)
2、运行cmd命令启动Redis服务。
redis-server.exe redis.windows.conf
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605150855903-1185573081.png)
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605150623309-740612801.png)
这里需要注意的是一定要带后面的启动参数，如果对配置文件修改完没有带启动参数并不会生效的，所以一定记得带参数。
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605150731090-1700936934.png)
这样服务就算启动成功了，这个窗口是服务终端，如果窗口被关闭，对应的服务也会停止。
3、继续cmd命令启动客户端。
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605150905122-2076849240.png)
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605151036325-739604165.png)
客户端连接服务成功。
4、设置和获取数据操作。
使用Set命令存入一个数据。
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605151128340-1029980902.png)
用Get命令获取数据。
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605151143934-1690972305.png)
至此，我们简单地redis安装部署并存入数据命令操作已经概括完毕。
二、分布式部署Redis
windows下我们如果想让其他ip地址的用户也能访问到我们的Redis服务，那么就需要对Redis文件夹下的redis.windows.conf进行配置。
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605164208012-764916485.png)
![](http://images2015.cnblogs.com/blog/998608/201706/998608-20170605164217043-1605568927.png)
 View Code
将绑定设置成 bind 0.0.0.0 然后将保护模式关闭，重新启动服务器。
远程用客户端尝试登录redis客户端， 正常情况是可以访问的。当然了，我们这样配置的redis存在很大的安全漏洞，留作Redis的后续研究。
至此，我们的windows平台下的Redis安装部署及分布式配置已经进行完毕。
--------------------- 本文来自 e62ces0iem 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/e62ces0iem/article/details/73477182?utm_source=copy
