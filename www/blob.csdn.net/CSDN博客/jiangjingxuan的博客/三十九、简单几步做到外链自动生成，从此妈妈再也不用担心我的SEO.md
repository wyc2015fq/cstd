# 三十九、简单几步做到外链自动生成，从此妈妈再也不用担心我的SEO - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:56:50[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：723












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_142_default_big.jpeg)



搜索引擎排名最关键的数据是高质量外链，无法被被人收录，我们就自己做一个收录自己的网站，同时用最佳的相关性生成高质量外链，百度也无法辨别真伪

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 具体方案

用抓取回来的所有网页做一个简单的好文站，用来做外链，引流量。然后写一个complete_link.py，遍历所有CrawlPage，找到content没有标记过<a>的，拿它的title去查elasticSearch，得到的第一篇文章，提出这篇文章的标题做成anchor添加到content尾部



## 为什么做高质量外链？

搜索引擎判断一个网站质量的好坏，主要会看有没有优秀的网页指向了它，就好比“你自己说你好，你不一定真的好，很多人说你好，你才是真的好”。强调高质量，就是说不是什么外链都起到加分的作用，如果在新浪头条引用你的网站，那会加分，一个垃圾站提到你，没有什么用。



## 怎么样的外链是高质量的？

首先引用你的网站比较优秀，本身打分比较高，那么你就会被抬高。其次，需要有优秀描述的锚文本，优秀体现在：简短的话，准确表述。百度对网页的标题和锚文本是最高优先级做相关性匹配的，所以标题和锚文本非常重要。然后，重复的引用不要太多，以免被判为垃圾。



## 为什么用elasticSearch？

这不是重点，重点是要找到一个最相关的链接，这样用户转化才够精准，找到这样的链接不求多，一个就够了。



## 为什么用标题作为锚文本

因为标题是对这篇文章最好的描述。锚文本内容是用来做检索召回的，用户搜了这个词，才会返回这个结果。



## 详细步骤



安装elasticsearch的python扩展
`pip install elasticsearch`


创建complete_link.py，内容如下：

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

import MySQLdb
import re
from conn import Conn
from elasticsearch import Elasticsearch

es = Elasticsearch()

conn = Conn().getConnection()
cursor = conn.cursor()
upcursor = conn.cursor()
sql = "select id, title, substring_index(content,'相关原创文章,敬请关注',1) from CrawlPage where content not like '%</a>'"
cursor.execute(sql)
for row in cursor.fetchall():
    id = row[0]
    title = row[1]
    content = row[2]
    title = re.sub('\[|\]|\/|\'|\"|\(|\)|\!|\?|\~','',title)

    try:
        res = es.search(index="app", body={"fields":["title"],"size":1,"query": {"query_string": {"query":title}}})
        for hit in res['hits']['hits']:
            print "process:", title
            update_sql = "update CrawlPage set content=CONCAT(content,\"%s%s%s%s%s\") where id=%d" % (
                    " 相关原创文章,敬请关注：<a href='http://www.shareditor.com/blogshow/?blogId=",
                    hit['_id'],
                    "'>",
                    hit['fields']['title'][0],
                    "</a>",
                    id)
            upcursor.execute(update_sql)
            conn.commit()

    except Exception,e:
        print "Error:"
        print title
        print e
        sys.exit(-1)
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 讲解一下

```python
update_sql = "update CrawlPage set content=CONCAT(content,\"%s%s%s%s%s\") where id=%d" % (
  " 相关原创文章,敬请关注：<a href='http://www.shareditor.com/blogshow/?blogId=",
  hit['_id'],
  "'>",
  hit['fields']['title'][0],
  "</a>",
  id)
```

这是生成链接的样子，锚文本是文章标题，链接指向文章页面


`res = es.search(index="app", body={"fields":["title"],"size":1,"query": {"query_string": {"query":title}}})`
是查询elasticSearch搜博客，取第一条，因为第一条是相关性最高的一篇

有关elasticSearch的python接口详细可以参考：[http://elasticsearch-py.readthedocs.io/en/latest/](http://elasticsearch-py.readthedocs.io/en/latest/)



## 最终效果

![](http://www.shareditor.com/uploads/media/my-context/0001/01/03d1852d47f6c033f8f3fab913f6c3c19b9c4e0a.png)




