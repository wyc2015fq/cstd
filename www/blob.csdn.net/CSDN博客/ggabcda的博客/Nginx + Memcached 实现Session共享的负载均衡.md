# Nginx + Memcached 实现Session共享的负载均衡 - ggabcda的博客 - CSDN博客





2017年09月09日 22:05:56[ggabcda](https://me.csdn.net/ggabcda)阅读数：128








## Session共享

　　我们在做站点的时候，通常需要保存用户的一些基本信息，比如登录就会用到Session；当使用Nginx做负载均衡的时候，用户浏览站点的时候会被分配到不同的服务器上，此时如果登录后Session就在不同发服务器上，A记住了用户的登录状态，可是下一次用户请求被分配到B去了怎么办？不可能让用户再登陆一次，所以要让session在不同的服务器上都能获取到相同的内容。

## 方法思路
-  ip_hash,这是Nginx的轮询方法，用户登录站点时，就会一直在这台服务器上就不存在Session的问题；

优点：不需要采用任何处理；

缺点：当某台服务器访问的量大时，负载均衡就相当于没用了

- 用Memcached或者redis缓存的方式； 
- 使用数据库直接存储；
- 　　缺点：对数据库要求高，当出现高访问量时，需要不停的读写，数据库压力大， 

- 用Memcached或者redis + DB异步存储； 



　　这四点是实现Session共享的一种方案和思路，每种方法都有自己的优缺点，全部的优缺点我也无法描述，毕竟没有具体的去实现和使用；使用哪种方式还需要看项目的情况和具体的解决方案；

　　在自己的学习过程选择第二种，Nginx + Memcached 实现Session共享的负载均衡；

　　自己搞一台服务器，Nginx和Memcached 在一台服务器；

　　在服务器我部署了相同的两个站点，代码相同，一个端口为8080；另一个为8090；

```
1    private static readonly IMemcachedManager _memcachedManager = new MemcachedManager();
 2 
 3         public ActionResult Index()
 4         {
 5             Random rd = new Random();
 6             Session["Session"] = rd.Next(1, 100000);
 7 
 8             if (!_memcachedManager.IsExit("Session"))
 9             {
10                 _memcachedManager.SetValue("Session", Session["Session"]);               
11             }
12 
13             ViewBag.Message = "当前站点为：8080；Session为：" + _memcachedManager.GetValue<string>("Session") + "";
14 
15             return View();
16         }
```



#### 运行结果：

![](http://images2017.cnblogs.com/blog/259735/201709/259735-20170907180400569-2102492884.jpg)

![](http://images2017.cnblogs.com/blog/259735/201709/259735-20170907180413147-724681485.jpg)

　　实现了我的目的，达到了均衡负载同时session共享的目的。

　　Nginx除了Session共享的问题，还有一个共享资源的问题，比如上传文件，这需要负载均衡需要解决的一个问题；

## 总结

　　不管是安装Nginx还是Memcached,在这个过程中，并没有遇到比较深的坑，当然Memcached在网上也有很多的例子，一些基本的安装都能够很快的实现，部署起来相对于简单毕竟只是一个Demo，不知道在实际的项目中将会是如何的，也会会有很大的不同；

　　这只是自己学习的一个地方，当然这里面还有更多东西需要去研究；

　　共勉







