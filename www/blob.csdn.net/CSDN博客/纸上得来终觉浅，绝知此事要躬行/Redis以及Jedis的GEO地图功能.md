# Redis以及Jedis的GEO地图功能 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客
2019年04月08日 20:07:20[boonya](https://me.csdn.net/boonya)阅读数：27
原文地址：[https://blog.csdn.net/xiangnan10/article/details/80225929](https://blog.csdn.net/xiangnan10/article/details/80225929)
引言：redis是一个高性能的非关系型数据库，作为一个单线程的应用程序，速度非常快。并且不存在多线程情况下的共同资源访问锁的问题。
## Redis介绍
Redis 是完全开源免费的，遵守BSD协议，是一个高性能的key-value数据库。
在Java开发中，Redis是一款经常使用的Nosql数据库，无论是做计数、缓存还是分布式共享Session都非常方便
Redis 与其他 key - value 缓存产品有以下三个特点：
Redis支持数据的持久化，可以将内存中的数据保存在磁盘中，重启的时候可以再次加载进行使用。
Redis不仅仅支持简单的key-value类型的数据，同时还提供list，set，zset，hash等数据结构的存储。
Redis支持数据的备份，即master-slave模式的数据备份。
Redis支持事务，但是事务功能不是很完善，不推荐使用
## Redis优势
性能极高 – Redis能读的速度是110000次/s,写的速度是81000次/s 。
丰富的数据类型 – Redis支持二进制案例的 Strings, Lists, Hashes, Sets 及 Ordered Sets 数据类型操作。
原子 – Redis的所有操作都是原子性的，意思就是要么成功执行要么失败完全不执行。单个操作是原子性的。多个操作也支持事务，即原子性，通过MULTI和EXEC指令包起来。
丰富的特性 – Redis还支持 publish/subscribe, 通知, key 过期等等特性。
## Redis坐标功能语法
今天主要来说说计数功能
Redis3.2版本提供了GEO(地理位置定位)功能(注意：只有3.2以上的Redis版本才能使用)，支持存储地理位置信息来实现诸如附近位置、摇一摇这类依赖于地理位置信息的功能，对于需要实现这些功能的开发者来说是一大福音。GEO功能是Redis的另一做着Matt Stancliff借鉴NoSQL数据库Ardb实现的，Ardb的作者是一名中国人，它提供了优秀的GEO功能。
先来说说Redis的原生语法
![](https://img-blog.csdnimg.cn/20190408200150454.png)
参数介绍:
geoadd: redis 语法，固定关键字
key: 一系列点的集合Key
longitude: 经度
latitude: 纬度
member: 具体的经纬度的点的名字
例子：geoadd points 104.074977 30.560872 世纪城地铁站
![](https://img-blog.csdnimg.cn/20190408200204747.png)
根据某个点的名字来获取经纬度信息
![](https://img-blog.csdnimg.cn/20190408200212342.png)
![](https://img-blog.csdnimg.cn/20190408200229190.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
georadius : 根据某个点，查出其周围半径为radiusm的点
georadiusbymember : 根据某个点的名字找到那个点，然后找其周围半径为radiusm的点
![](https://img-blog.csdnimg.cn/20190408200245999.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
例子:georadiusbymember points beijing 150km
例子2: georadius points 104.074977 30.560872 2km
![](https://img-blog.csdnimg.cn/20190408200258932.png)
## Redis的Geo与java的结合
当然，上面那么多东西并不是我们Java程序员实际关心的语法，我们关心的更多还是上面的那些语法与Java的一个结合。
下面我们就根据一个demo程序来具体来看geo的用法
demo中使用的是springboot程序
首先是pom.xml配置文件
![](https://img-blog.csdnimg.cn/2019040820032617.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
注：spring-boot-starter-data-redis中包含了Jedis的依赖包
然后是Jedis相关Bean的配置
![](https://img-blog.csdnimg.cn/20190408200336380.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
接下来就是Geo的相关的Jedis工具类
![](https://img-blog.csdnimg.cn/20190408200344293.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/2019040820041022.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190408200421900.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
最后一个controller来控制
![](https://img-blog.csdnimg.cn/20190408200432524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
注:该controller只是用于测试geo的功能，写得比较简陋。。。
一切搞定后，我们就开始测试该程序是否起作用。
先是向redis中添加坐标
![](https://img-blog.csdnimg.cn/20190408200454124.png)
![](https://img-blog.csdnimg.cn/20190408200501970.png)
然后看是否能根据某个点，查找周围的点
当距离为三公里事，可以找到先前添加的两个点
![](https://img-blog.csdnimg.cn/20190408200515267.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
当距离为两公里时，就不能找到"世纪城地铁口"
![](https://img-blog.csdnimg.cn/201904082005252.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
以下地图中红色圈的位置，是我文章中用到的三个点坐标
![](https://img-blog.csdnimg.cn/20190408200541350.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)
到这里，已经可以证明，Redis的周围坐标功能起了作用。
## 总结
这个功能在做摇一摇或者周边餐饮、车辆时非常有用。当然，使用百度JSPAPI也可以实现这个功能，不过要引入第三方的依赖。
比较简单的功能，完全可以使用Redis的GEO功能来实现。以上，是Redis的GEO功能，感谢阅读。
