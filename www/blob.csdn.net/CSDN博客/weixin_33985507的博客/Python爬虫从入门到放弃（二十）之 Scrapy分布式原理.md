# Python爬虫从入门到放弃（二十）之 Scrapy分布式原理 - weixin_33985507的博客 - CSDN博客
2017年08月08日 13:31:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
## 关于Scrapy工作流程回顾
**Scrapy单机架构**
**![](https://images2017.cnblogs.com/blog/997599/201708/997599-20170808132857870-734224242.png)**
上图的架构其实就是一种单机架构，只在本机维护一个爬取队列，Scheduler进行调度，而要实现多态服务器共同爬取数据关键就是共享爬取队列。
![](https://images2017.cnblogs.com/blog/997599/201708/997599-20170808132914120-1848384667.png)
**分布式架构**
**![](https://images2017.cnblogs.com/blog/997599/201708/997599-20170808132946917-419124623.png)**
我将上图进行再次更改
![](https://images2017.cnblogs.com/blog/997599/201708/997599-20170808133001245-553904990.png)
这里重要的就是我的队列通过什么维护？
这里一般我们通过Redis为维护，Redis，非关系型数据库，Key-Value形式存储，结构灵活。
并且redis是内存中的数据结构存储系统，处理速度快，提供队列集合等多种存储结构，方便队列维护
如何去重？
这里借助redis的集合，redis提供集合数据结构，在redis集合中存储每个request的指纹
在向request队列中加入Request前先验证这个Request的指纹是否已经加入集合中。如果已经存在则不添加到request队列中，如果不存在，则将request加入到队列并将指纹加入集合
如何防止中断？如果某个slave因为特殊原因宕机，如何解决？
这里是做了启动判断，在每台slave的Scrapy启动的时候都会判断当前redis request队列是否为空
如果不为空，则从队列中获取下一个request执行爬取。如果为空则重新开始爬取，第一台丛集执行爬取向队列中添加request
如何实现上述这种架构？
这里有一个scrapy-redis的库，为我们提供了上述的这些功能
scrapy-redis改写了Scrapy的调度器，队列等组件，利用他可以方便的实现Scrapy分布式架构
关于scrapy-redis的地址：[https://github.com/rmax/scrapy-redis](https://github.com/rmax/scrapy-redis)
## 搭建分布式爬虫
参考官网地址：[https://scrapy-redis.readthedocs.io/en/stable/](https://scrapy-redis.readthedocs.io/en/stable/)
前提是要安装scrapy_redis模块：pip install scrapy_redis
这里的爬虫代码是用的之前写过的爬取知乎用户信息的爬虫
### 修改该settings中的配置信息：
**替换scrapy调度器**
SCHEDULER = "scrapy_redis.scheduler.Scheduler"
**添加去重的class**
DUPEFILTER_CLASS = "scrapy_redis.dupefilter.RFPDupeFilter"
**添加pipeline**
如果添加这行配置，每次爬取的数据也都会入到redis数据库中，所以一般这里不做这个配置
ITEM_PIPELINES = {
'scrapy_redis.pipelines.RedisPipeline': 300
}
**共享的爬取队列，这里用需要redis的连接信息**
这里的user:pass表示用户名和密码，如果没有则为空就可以
REDIS_URL = 'redis://user:pass@hostname:9001'
**设置为为True则不会清空redis里的dupefilter和requests队列**
这样设置后指纹和请求队列则会一直保存在redis数据库中，默认为False，一般不进行设置
SCHEDULER_PERSIST = True
**设置重启爬虫时是否清空爬取队列**
这样每次重启爬虫都会清空指纹和请求队列,一般设置为False
SCHEDULER_FLUSH_ON_START=True
### 分布式
将上述更改后的代码拷贝的各个服务器，当然关于数据库这里可以在每个服务器上都安装数据，也可以共用一个数据，我这里方面是连接的同一个mongodb数据库，当然各个服务器上也不能忘记：
所有的服务器都要安装scrapy,scrapy_redis,pymongo
这样运行各个爬虫程序启动后，在redis数据库就可以看到如下内容，dupefilter是指纹队列，requests是请求队列
![](https://images2017.cnblogs.com/blog/997599/201708/997599-20170808133028714-204052116.png)
