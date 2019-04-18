# 三十二、scrapy爬虫抓取网页写入mysql数据库 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:53:30[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：684












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_59_default_big.jpeg)



scrapy抓取的网页默认存成了python的dict结构，scrapy提供了pipeline接口来存储数据，为了方便以后使用，我们把抓取的结构化内容存入mysql

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 安装MySQL-python
`[root@centos7vm ~]# pip install MySQL-python`
执行如下不报错说明安装成功：

```
[root@centos7vm ~]# python
Python 2.7.5 (default, Nov 20 2015, 02:00:19)
[GCC 4.8.5 20150623 (Red Hat 4.8.5-4)] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import MySQLdb
>>>
```

## 创建page表

为了保存网页，在mysql数据库中创建page表，sql语句如下：

```
CREATE TABLE `page` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) COLLATE utf8_unicode_ci NOT NULL,
  `post_date` timestamp NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `post_user` varchar(255) COLLATE utf8_unicode_ci DEFAULT '',
  `body` longtext COLLATE utf8_unicode_ci,
  `content` longtext COLLATE utf8_unicode_ci,
  PRIMARY KEY (`id`),
  UNIQUE KEY `title` (`title`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COLLATE=utf8_unicode_ci;
```

其中title是文章标题，post_date是文章发布时间，post_user是发布者（也就是公众号），body是网页原始内容，content是提取出的纯文本格式的正文



## 创建item结构



在我们的scrapy项目里修改item.py文件，用来保存提取出来的结构化数据，内容如下：

```python
import scrapy

class WeixinItem(scrapy.Item):
    # define the fields for your item here like:
    title = scrapy.Field()
    post_date = scrapy.Field()
    post_user = scrapy.Field()
    body = scrapy.Field()
    content = scrapy.Field()
```



## 生成item结构

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

修改爬虫脚本，在parse函数中增加如下几句：

```python
def parse_profile(self, response):
        title = response.xpath('//title/text()').extract()[0]
        post_date = response.xpath('//em[@id="post-date"]/text()').extract()[0]
        post_user = response.xpath('//a[@id="post-user"]/text()').extract()[0]
        body = response.body
        tag_content = response.xpath('//div[@id="js_content"]').extract()[0]
        content = remove_tags(tag_content).strip()
        item = WeixinItem()
        item['title'] = title
        item['post_date'] = post_date
        item['post_user'] = post_user
        item['body'] = body
        item['content'] = content
        return item
```



注：如果不知道爬虫脚本怎么写，请看上一篇文章《[教你成为全栈工程师(Full Stack Developer) 三十一-利用微信搜索抓取公众号文章](http://www.shareditor.com/blogshow/44)》

另外：这里的content是去标签后的纯文本，使用了remove_tags，这需要加载库：

```python
from w3lib.html import remove_tags
```



## 创建pipelines



scrapy持久化数据的方式是通过pipeline来实现。开源的各种爬虫软件均会对持久化方式提供各种方法，比如pyspider提供了写入mysql、mongodb、文件等的持久化方法，scrapy作为爬虫老将把接口留给了我们，我们可以自定义各种pipeline，同时可以通过配置灵活选择

pipeline机制通过pipelines.py文件和settings.py文件结合实现

修改scrapy项目里的pipelines.py内容如下：

```python
# -*- coding: utf-8 -*-
import sys
reload(sys)
sys.setdefaultencoding('utf8')
import MySQLdb

class WeixinPipeline(object):
    def __init__(self):
        self.conn = MySQLdb.connect(host="127.0.0.1",user="myname",passwd="mypasswd",db="mydbname",charset="utf8")
        self.cursor = self.conn.cursor()

    def process_item(self, item, spider):
        sql = "insert ignore into page(title, post_user, body, content) values(%s, %s, %s, %s)"
        param = (item['title'], item['post_user'], item['body'], item['content'])
        self.cursor.execute(sql,param)
        self.conn.commit()
```



里面的数据库配置按照自己的修改好，这里的process_item会在抓取时自动调用，并把爬虫脚本返回的item通过参数传递进来，这里通过insert将item结构化数据插入了mysql数据库



下面再看settings.py文件，如下：

```python
ITEM_PIPELINES = {
    'weixin.pipelines.WeixinPipeline': 300,
}
```



运行爬虫后看到数据库如下：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/4928a890e421bd28b05dfc7caad26df07060606c.png)



相当完美，准备用这些数据来作为训练样本来做机器学习之用，预知后事如何，且听下回分解




