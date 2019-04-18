# Redis在windows下安装过程 - z69183787的专栏 - CSDN博客
2017年10月19日 17:29:44[OkidoGreen](https://me.csdn.net/z69183787)阅读数：260
个人分类：[Redis-基础](https://blog.csdn.net/z69183787/article/category/2923783)
[http://www.cnblogs.com/M-LittleBird/p/5902850.html](http://www.cnblogs.com/M-LittleBird/p/5902850.html)
一、下载windows版本的Redis
去官网找了很久，发现原来在官网上可以下载的windows版本的，现在官网以及没有下载地址，只能在github上下载，官网只提供linux版本的下载
官网下载地址：[http://redis.io/download](http://redis.io/download)
github下载地址：[https://github.com/MSOpenTech/redis/tags](https://github.com/MSOpenTech/redis/tags)
二、安装Redis
1.这里下载的是Redis-x64-3.2.100版本，我的电脑是win7 64位，所以下载64位版本的，在运行中输入cmd，然后把目录指向解压的Redis目录。
![image](http://images2015.cnblogs.com/blog/596198/201609/596198-20160924113325981-1281380353.png)
2、启动命令
redis-server redis.windows.conf，出现下图显示表示启动成功了。
![image](http://images2015.cnblogs.com/blog/596198/201609/596198-20160924113330574-845670085.png)
#### 三、设置Redis服务
1、由于上面虽然启动了redis，但是只要一关闭cmd窗口，redis就会消失。所以要把redis设置成windows下的服务。
也就是设置到这里，首先发现是没用这个Redis服务的。
![image](http://images2015.cnblogs.com/blog/596198/201609/596198-20160924113335199-1382315421.png)
2、设置服务命令
redis-server --service-install redis.windows-service.conf --loglevel verbose
#### ![image](http://images2015.cnblogs.com/blog/596198/201609/596198-20160924113338965-1626955828.png)
输入命令之后没有报错，表示成功了，刷新服务，会看到多了一个redis服务。
![image](http://images2015.cnblogs.com/blog/596198/201609/596198-20160924113343184-1672706333.png)
3、常用的redis服务命令。
卸载服务：redis-server --service-uninstall
开启服务：redis-server --service-start
停止服务：redis-server --service-stop
4、启动服务
![image](http://images2015.cnblogs.com/blog/596198/201609/596198-20160924113346559-1291084867.png)
5、测试Redis
![image](http://images2015.cnblogs.com/blog/596198/201609/596198-20160924113349668-1075805574.png)
安装测试成功。
   这里只是做简单的安装，部署服务使用，更深入的使用可以去redis中文网看看 [http://www.redis.net.cn/](http://www.redis.net.cn/) 博客园也有很多深入使用方法，由于我的博客系统需要用到，做个抛砖引玉吧。
   在下载的解压包里面有一份文档，有详细的说明，
![image](http://images2015.cnblogs.com/blog/596198/201609/596198-20160924113353715-1398104768.png)
