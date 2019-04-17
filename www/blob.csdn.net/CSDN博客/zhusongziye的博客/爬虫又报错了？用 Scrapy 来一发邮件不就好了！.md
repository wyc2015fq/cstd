# 爬虫又报错了？用 Scrapy 来一发邮件不就好了！ - zhusongziye的博客 - CSDN博客





2018年11月19日 21:15:46[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：255








# 一、需求

有时候我们写了很多爬虫在运行，但是我们可能不会一直有时间来监控它的变化，比如报错了，或者爬完了，如果我们能有个通知来通知我们，那该是多好啊！比如发个邮件，发个短信那是再好不过了，那么本篇文章就介绍一下使用 Scrapy 怎样来发送一封通知邮件。

我们的目的是可以根据实际需求，在不同的时机发送不同的提醒邮件，以对爬虫状态进行监控。

这里我以监控爬虫的停止信息来作为示例。

# 二、Scrapy文档

Scrapy 官网文档 有提供邮件发送的资料：

### 发送 Email

虽然 Python 通过 Smtplib 库使得发送 Email 变得很简单，Scrapy 仍然提供了自己的实现。 该功能十分易用，同时由于采用了 Twisted 非阻塞式(non-blocking) IO ，其避免了对爬虫的非阻塞式 IO 的影响。 另外，其也提供了简单的 API 来发送附件。 通过一些 Settings 设置，您可以很简单的进行配置。

### 简单例子

有两种方法可以创建邮件发送器(mail sender)。 您可以通过标准构造器(constructor)创建:

```
from scrapy.mail import MailSender
mailer = MailSender()
```

或者您可以传递一个 Scrapy 设置对象，其会参考 Settings:

```
mailer = MailSender.from_settings(settings)
```

这是如何来发送邮件了(不包括附件):

```
mailer.send(to=["someone@example.com"], subject="Some subject", body="Some body", cc=["another@example.com"])
```

### MailSender 类参考手册

在 Scrapy 中发送 Email 推荐使用 MailSender。其同框架中其他的部分一样，使用了 Twisted 非阻塞式(non-blocking)IO 。

```
class scrapy.mail.MailSender(smtphost=None, mailfrom=None, smtpuser=None, smtppass=None, smtpport=None)
```

参数由以下组成:

```
smtphost (str) – 发送email的SMTP主机(host)。如果忽略，则使用 MAIL_HOST 。
        mailfrom (str) – 用于发送email的地址(address)(填入 From:) 。 如果忽略，则使用 MAIL_FROM 。
        smtpuser – SMTP用户。如果忽略,则使用 MAIL_USER 。 如果未给定，则将不会进行SMTP认证(authentication)。
        smtppass (str) – SMTP认证的密码
        smtpport (int) – SMTP连接的端口
        smtptls – 强制使用STARTTLS
        smtpssl (boolean) – 强制使用SSL连接
```

使用Scrapy设置对象来初始化对象。其会参考 这些Scrapy设置.
参数: settings (scrapy.settings.Settings object) – the e-mail recipients


`send(to, subject, body, cc=None, attachs=(), mimetype='text/plain')`
```
发送email到给定的接收者。
参数:
```


```
to (list) – email接收者

 subject (str) – email内容

 cc (list) – 抄送的人

 body (str) – email的内容

 attachs (iterable) – 可迭代的元组 (attach_name, mimetype, file_object)。 attach_name 是一个在email的附件中显示的名字的字符串， mimetype 是附件的mime类型， file_object 是包含附件内容的可读的文件对象。

 mimetype (str) – email的mime类型
```


# 三、实际写法

根据上面官网文档的一些介绍和语法（更多语法请上官网翻阅）

这里编写一段示例代码（结合爬虫关闭信号）

### 3.1 设置邮箱pop

登录 QQ 邮箱  找到设置-账户

然后生成授权码（以前是生成密码，现在用授权码）

### 3.2 编码

在具体的爬虫文件中编写：

```
from scrapy.mail import MailSender
from scrapy.xlib.pydispatch import dispatcher
from scrapy import signals
```

接着在 Class 的上方编写 Emial 的链接配置信息：

```
mailers = MailSender(
    smtphost="smtp.qq.com",  # 发送邮件的服务器
    mailfrom="1499450062@qq.com",   # 邮件发送者
    smtpuser="1499450062@qq.com",   # 用户名
    smtppass="qtpzvxxyyxxyyxxyyxde",  # 发送邮箱的密码不是你注册时的密码，而是授权码！！！切记！
    smtpport=25  # 端口号
    )  # 初始化邮件模块
```

然后再到 Class 中编写信号量监听和具体的邮件发送代码：

```
def __init__(self):
        """ 监听信号量 """
        super(YoutubeapiSpider, self).__init__()
        # 当收到spider_closed信号的时候，调用下面的close方法来发送通知邮件
        dispatcher.connect(self.close, signals.spider_closed)

    def close(spider, reason):
        """ 执行邮件发送操作 """
        body = "爬虫[%s]已经关闭，原因是: %s" % (spider.name, reason)
        subject = "[%s]爬虫关闭提醒" % spider.name
        mailers.send(to={"admin@qq.com", "quinns@aliyun.com"}, subject=subject,
                     body=body)
```

这样就会在收到爬虫关闭信号的时候，通过 1499450062@qq.com 给指定的 admin@qq.com 和 quinns@aliyun.com 发送邮件，邮件内容是 body,邮件标题是 subject。

### 3.3 另一种写法

当然了，写法很多，除了def cloase 还可以：

```
def __init__(self):
        """ 监听信号量 """
        super(YoutubeapiSpider, self).__init__()
        # 当收到spider_closed信号的时候，调用下面的close方法来发送通知邮件
        dispatcher.connect(self.spider_closed, signals.spider_closed)

    def spider_closed(self, spider):
        # 当爬虫推出的时候发送邮件
        body = "爬虫已经关闭，原因是"
        subject = "爬虫关闭提醒"
        mailers.send(to={"admin@qq.com", "quinns@aliyun.com"}, subject=subject,
                    body=body)
```

只要满足触发条件，就可以发送指定内容的邮件。

这里虽然只是以爬虫关闭来举例，实际上可以监控更多的行为操作，比如百度翻译的接口超量、捕获一些特殊的异常等。



