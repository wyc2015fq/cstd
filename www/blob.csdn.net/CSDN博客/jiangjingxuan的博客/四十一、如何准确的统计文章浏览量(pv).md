# 四十一、如何准确的统计文章浏览量(pv) - jiangjingxuan的博客 - CSDN博客





2017年01月25日 11:57:56[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2787












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_174_default_big.jpeg)



自己写的原创文章很想知道都有多少网友浏览过，单纯的显示次数并不能准确地反应真实浏览量，还需要解决一些意想不到的问题，本节我把刚刚完成的浏览量统计方案分享出来

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 数据库表建立

pv（Page View，网页浏览量）是针对blog文章的，因此数据库表直接在BlogPost表中扩充，修改BlogPost.php，增加如下内容：

```php
/**
 * @var int
 *
 * @ORM\Column(name="pv", type="integer", options={"default": "0"})
 */
private $pv;

/**
 * @return mixed
 */
public function getPv()
{
    return $this->pv;
}

/**
 * @param mixed $pv
 */
public function setPv($pv)
{
    $this->pv = $pv;
}
```



并执行：
`php app/console doctrine:schema:update --force`
生效数据库，我们可以看到数据库多了一列pv，默认值为0：

![](http://www.shareditor.com/uploads/media/my-context/0001/01/8037c65fceff1e9fed32af48370b46370c754ee7.png)

## 方案

浏览量统计的关键点在于“排除干扰因素”，所谓的干扰因素主要是非真实用户浏览：爬虫、恶意刷网页等

因此我们不采取实时浏览写数据的方案，我们采取基于浏览日志做清洗和统计



首先，分析了一下我们的日志，通过不断的grep一些关键词（spider、bot）总结出如下爬虫关键，后面我们把这部分过滤掉：
`YunGuanCe|Baiduspider|Sogou web spider|spiderman|pyspider|bingbot|Googlebot|MJ12bot|Slurp|Uptimebot|SafeDNSBot|AhrefsBot|YandexBot|GimmeUSAbot|Alibaba|YisouSpider|360Spider|semanticbot|TeeRaidBot|Cliqzbot|pycurl|SurveyBot|yandex.com/bots|linkdexbot|xml-sitemaps|Slackbot-LinkExpanding|Applebot|rogerbot|org_bot|TweetmemeBot|Twitterbot|Scrapy`


然后，继续通过统计来源ip的方式发现存在一类无http_referer、无http_user_agent、无http_x_forwarded_for的三无日志，类似这样：

```bash
180.153.163.191 - - [11/Jun/2016:18:30:31 +0800] "GET /blogshow/?blogId=30 HTTP/1.1" 200 18399 "-" "-" "-"
```

这种一定不是正常的浏览器，因此我们也要过滤掉



## 日志清洗

按照上述方案，我们的日志清洗脚本新鲜出炉：

```bash
#!/bin/bash

date

LOG_PATH="/data/httpdir/logs/shareditor2.0.log"

SPIDER_UA="YunGuanCe|Baiduspider|Sogou web spider|spiderman|pyspider|bingbot|Googlebot|MJ12bot|Slurp|Uptimebot|SafeDNSBot|AhrefsBot|YandexBot|GimmeUSAbot|Alibaba|YisouSpider|360Spider|semanticbot|TeeRaidBot|Cliqzbot|pycurl|SurveyBot|yandex.com/bots|linkdexbot|xml-sitemaps|Slackbot-LinkExpanding|Applebot|rogerbot|org_bot|TweetmemeBot|Twitterbot|Scrapy"

egrep -v "$SPIDER_UA" $LOG_PATH | grep 'GET /blogshow' | grep -v '"-" "-" "-"' > ./clean.log
```

生成的clean.log就是清洗后的日志

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## pv统计

下面就是基于清洗后的日志做pv统计了，创建gen_pv.py如下：

```python
#!/usr/bin/python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

import MySQLdb
from conn import Conn
import re

pattern1 = re.compile('/blogshow/\?blogId=([0-9][0-9]*)$')
pattern2 = re.compile('/blogshow/([0-9][0-9]*)$')

pv={}

f = open('./clean.log')
try:
    while True:
        line = f.readline()
        if line:
            fields = line.split(' ')
            url_path = fields[6] # /blogshow/3 or /blogshow/?blogId=3
            match1 = pattern1.match(url_path)
            match2 = pattern2.match(url_path)
            blog_id = -1
            if match1:
                blog_id = int(match1.group(1))
            if match2:
                blog_id = int(match2.group(1))

            if blog_id != -1:
                if pv.has_key(blog_id):
                    pv[blog_id] = pv[blog_id] + 1
                else:
                    pv[blog_id] = 1

        else:
            break
finally:
    f.close( )

conn = Conn().getConnection()
cursor = conn.cursor()

for id in pv:
    sql = "update BlogPost set pv=%d where id=%d" % (pv[id], id)
    cursor.execute(sql)
    conn.commit()

conn.close()
```

讲解一下：

因为网站做过改版，blogshow网页的url存在过两种形式，因此要把这两部分都考虑进来，所以做了两部分正则匹配

获取到全部blog的pv数据最后刷入数据库



## crontab配置

为了能比较及时又不损失服务器性能，我们配置5分钟执行一次统计，配置crontab如下：
`*/5 * * * * cd /data/gen_pv && sh run.sh &>> run.sh.log`





