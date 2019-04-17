# 2.5 scrapy的内置服务介绍 - 博客堂 - CSDN博客





2017年05月19日 16:51:38[最小森林](https://me.csdn.net/u012052268)阅读数：808










- [5 scrapy的内置服务介绍](#25-scrapy的内置服务介绍)- [logging日志服务](#logging日志服务)- [基本使用方法](#基本使用方法)
- [在scrapy中使用](#在scrapy中使用)
- [在settingspy中配置](#在settingspy中配置)

- [stats collections](#stats-collections)- [基本操作](#基本操作)
- [内置可用收集器](#内置可用收集器)

- [sending-email](#sending-email)- [基本使用方法](#基本使用方法-1)
- [mailsender类](#mailsender类)
- [mail settings](#mail-settings)






# 2.5 scrapy的内置服务介绍

## logging日志服务

logging的等级：
- logging.CRITICAL - for critical errors (highest severity)
- logging.ERROR - for regular errors
- logging.WARNING - for warning messages
- logging.INFO - for informational messages
- logging.DEBUG - for debugging messages (lowest severity)

### 基本使用方法

三种基本使用方法：

```
# 1.简单使用
import logging
logging.warning("this is a warning")


# 2.通用的记录日志的方法，可加入日志的级别
import logging
logging.log(logging.WARNING,"this is a warning")


# 3.通过logger记录日志
import logging
logger = logging.getLogger(__name__)
logger.warning("this is a warning")
```

### 在scrapy中使用
- 在spider中直接使用

```python
import scrapy

class MySpider(scrapy.Spider): # 因为MySpider继承了scrapy.Spider，所以自带了logger

    name = 'myspider'
    start_urls = ['http://scrapinghub.com']

    def parse(self, response):
        self.logger.info('Parse function called on %s', response.url)
```
- 可以自己改名字

```python
import logging
import scrapy

logger = logging.getLogger('mycustomlogger') # 给自己的logger取名字为mycustomlogger

class MySpider(scrapy.Spider):

    name = 'myspider'
    start_urls = ['http://scrapinghub.com']

    def parse(self, response):
        logger.info('Parse function called on %s', response.url)
```

### 在settings.py中配置

These settings can be used to configure the logging:
- LOG_FILE
- LOG_ENABLED
- LOG_ENCODING
- LOG_LEVEL
- LOG_FORMAT
- LOG_DATEFORMAT
- LOG_STDOUT

## stats collections

Scrapy提供了方便的收集数据的机制。数据以key/value方式存储，值大多是计数值。该机制叫做数据收集器(Stats Collector)。

通过 stats 属性来使用数据收集器。 下面是在扩展中使用状态的例子:

### 基本操作

```python
class ExtensionThatAccessStats(object):

    def __init__(self, stats):
        self.stats = stats

    @classmethod
    def from_crawler(cls, crawler):
        return cls(crawler.stats)
```

设置数据:
`stats.set_value('hostname', socket.gethostname())`
增加数据值:

```bash
stats.inc_value('pages_crawled')
```

当新的值比原来的值大时设置数据:
`stats.max_value('max_items_scraped', value)`
当新的值比原来的值小时设置数据:
`stats.min_value('min_free_memory_percent', value)`
获取数据:

```python
>>> stats.get_value('pages_crawled')
8
```

获取所有数据:

```python
>>> stats.get_stats()
{'pages_crawled': 1238, 'start_time': datetime.datetime(2009, 7, 14, 21, 47, 28, 977139)}
```

### 内置可用收集器

```
# 记录内存的只能在linux中使用
class scrapy.statscollectors.MemoryStatsCollector

# 信息清道收集器
class scrapy.statscollectors.DummyStatsCollector
```

## sending-email

虽然Python通过 smtplib 库使得发送email变得很简单，Scrapy仍然提供了自己的实现。 该功能十分易用，同时由于采用了 Twisted非阻塞式(non-blocking)IO ，其避免了对爬虫的非阻塞式IO的影响。 另外，其也提供了简单的API来发送附件。 通过一些 settings 设置，您可以很简单的进行配置。

### 基本使用方法

有两种方法可以创建邮件发送器(mail sender)。 您可以通过标准构造器(constructor)创建:

```python
from scrapy.mail import MailSender
mailer = MailSender()
```

或者您可以传递一个Scrapy设置对象，其会参考 settings:
`mailer = MailSender.from_settings(settings)`
这是如何来发送邮件了(不包括附件):
`mailer.send(to=["someone@example.com"], subject="Some subject", body="Some body", cc=["another@example.com"])`
### mailsender类

初始化参数：
`class scrapy.mail.MailSender(smtphost=None, mailfrom=None, smtpuser=None, smtppass=None, smtpport=None)`
参数:

```
smtphost (str) – 发送email的SMTP主机(host)。如果忽略，则使用 MAIL_HOST 。
mailfrom (str) – 用于发送email的地址(address)(填入 From:) 。 如果忽略，则使用 MAIL_FROM 。
smtpuser – SMTP用户。如果忽略,则使用 MAIL_USER 。 如果未给定，则将不会进行SMTP认证(authentication)。
smtppass (str) – SMTP认证的密码
smtpport (int) – SMTP连接的短裤
smtptls – 强制使用STARTTLS
smtpssl (boolean) – 强制使用SSL连接
```

```
classmethod from_settings(settings)
使用Scrapy设置对象来初始化对象。其会参考 这些Scrapy设置.


send(to, subject, body, cc=None, attachs=(), mimetype='text/plain')
发送email到给定的接收者。
```

### mail settings

这些设置定义了 MailSender 构造器的默认值。其使得在您不编写任何一行代码的情况下，为您的项目配置实现email通知的功能。

```
MAIL_FROM
默认值: 'scrapy@localhost'
用于发送email的地址(address)(填入 From:) 。

MAIL_HOST
默认值: 'localhost'
发送email的SMTP主机(host)。

MAIL_PORT
默认值: 25
发用邮件的SMTP端口。

MAIL_USER
默认值: None
SMTP用户。如果未给定，则将不会进行SMTP认证(authentication)。

MAIL_PASS
默认值: None
用于SMTP认证，与 MAIL_USER 配套的密码。

MAIL_TLS
默认值: False
强制使用STARTTLS。STARTTLS能使得在已经存在的不安全连接上，通过使用SSL/TLS来实现安全连接。

MAIL_SSL
默认值: False
强制使用SSL加密连接。
```



