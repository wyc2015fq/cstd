# Python分布式爬虫详解（一） - zhusongziye的博客 - CSDN博客





2018年10月20日 16:03:39[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：234








当项目上升到一定境界时候，需要同时抓取几百个甚至上千个网站，这个时候，单个的爬虫已经满足不了需求。比如我们日常用的百度，它每天都会爬取大量的网站，一台服务器肯定是不够用的。所以需要各个地方的服务器一起协同工作。

**本章知识点：**

a.scrapy-redis简介

b.开始项目前的准备

**一、Scrapy-Redis 简介**

scrapy-redis是一个基于redis数据库的scrapy组件，它提供了四种组件，通过它，可以快速实现简单分布式爬虫程序。

![](https://img-blog.csdn.net/2018102016021519?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

scrapy-redis组件 ：

1、Scheduler（调度）：Scrapy改造了python本来的collection.deque(双向队列)形成了自己Scrapy queue，而scrapy-redis 的解决是把这个Scrapy queue换成redis数据库，从同一个redis-server存放要爬取的request，便能让多个spider去同一个数据库里读取。Scheduler负责对新的request进行入列操作（加入Scrapy queue），取出下一个要爬取的request（从Scrapy queue中取出）等操作。

2、Duplication Filter（去重）：Scrapy中用集合实现这个request去重功能，Scrapy中把已经发送的request指纹放入到一个集合中，把下一个request的指纹拿到集合中比对，如果该指纹存在于集合中，说明这个request发送过了，如果没有则继续操作。

3、Item Pipline（管道）：引擎将(Spider返回的)爬取到的Item给Item Pipeline，scrapy-redis 的Item Pipeline将爬取到的 Item 存⼊redis的 items queue。

4、Base Spider（爬虫）：不再使用scrapy原有的Spider类，重写的RedisSpider继承了Spider和RedisMixin这两个类，RedisMixin是用来从redis读取url的类。

项目地址：

```
https://github.com/rmax/scrapy-redis
```

**二、Scrapy-Redis 工作机制**

![](https://img-blog.csdn.net/20181020160225593?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

1、首先Slaver端从Master端拿任务（Request、url）进行数据抓取，Slaver抓取数据的同时，产生新任务的Request便提交给 Master 处理；

2、Master端只有一个Redis数据库，负责将未处理的Request去重和任务分配，将处理后的Request加入待爬队列，并且存储爬取的数据。

**三、开始项目前的准备**

1、Redis配置安装：

工欲善其事必先利其器，既然是基于redis的服务，当然首先要安装redis了。

安装Redis服务器端

```
sudo apt-get install redis-server
```

修改配置文件 redis.conf

```
sudo nano /etc/redis/redis.conf
```

将`bind 127.0.0.1`注释掉。这样Slave端才能远程连接到Master端的Redis数据库。

将Ubuntu作为Master端，Windows10和Windows7作为Slaver端，在Master中开启redis-service服务。Slaver端也需要有redis。

```
redis-server
```

Slaver连接测试：

```
redis-cli -h MasterIP地址
```

![](https://img-blog.csdn.net/2018102016024738?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

至此，redis已经安装完成。

**Redis可视化管理工具-Redis Desktop Manager**

下载地址：

```
https://redisdesktop.com/download
```

配置方法：

![](https://img-blog.csdn.net/20181020160258225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、获取自己的ip代理池

很多网站都有反爬虫机制，只用一个ip去频繁访问网站的话，很容易引起网站管理员的注意，如果管理员将这个ip加入黑名单，那么这个爬虫就废掉了。所以，想要做大型的爬虫的话，基本上是必须要面对ip的问题。

那么问题来了，我们去哪里搞代理ip呢？？第一种方法就是买买买！！没有什么事情是用钱解决不了的，如果有，那就加倍。

当然，网上也有一堆免费的ip代理，但是，免费的质量参差不齐，所以就需要进行筛选。以西刺代理为例：用爬虫爬取国内的高匿代理IP，并进行验证。（只爬取前五页，后面的失效太多，没有必要去验证了。）

爬虫：

```
class XiciSpider(scrapy.Spider):
    name = 'xici'
    allowed_domains = ['xicidaili.com']
    start_urls = []
    for i in range(1, 6):
        start_urls.append('http://www.xicidaili.com/nn/' + str(i))

    def parse(self, response):
        ip = response.xpath('//tr[@class]/td[2]/text()').extract()
        port = response.xpath('//tr[@class]/td[3]/text()').extract()
        agreement_type = response.xpath('//tr[@class]/td[6]/text()').extract()
        proxies = zip(ip, port, agreement_type)
        # print(proxies)

        # 验证代理是否可用
        for ip, port, agreement_type in proxies:
            proxy = {'http': agreement_type.lower() + '://' + ip + ':' + port,
                     'https': agreement_type.lower() + '://' + ip + ':' + port}
            try:
                # 设置代理链接  如果状态码为200 则表示该代理可以使用
                print(proxy)
                resp = requests.get('http://icanhazip.com', proxies=proxy, timeout=2)
                print(resp.status_code)
                if resp.status_code == 200:
                    print(resp.text)
                    # print('success %s' % ip)
                    item = DailiItem()
                    item['proxy'] = proxy
                    yield item
            except:
                print('fail %s' % ip)
```

Pipeline：

```
class DailiPipeline(object):

    def __init__(self):
        self.file = open('proxy.txt', 'w')

    def process_item(self, item, spider):
        self.file.write(str(item['proxy']) + '\n')
        return item

    def close_spider(self, spider):
        self.file.close()
```

运行结果：

![](https://img-blog.csdn.net/20181020160316243?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

爬了500条数据，只有四条可以用………

本章项目地址:
`https://github.com/ZhiqiKou/Scrapy_notes`


**本文作者**

♚

Zhiqi Kou，一个向往成为真正程序员的码奴。

地址：zhihu.com/people/zhiqi-kou



