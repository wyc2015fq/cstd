# Scrapy之探讨3个细节 - lyx的专栏 - CSDN博客





2016年06月22日 21:55:56[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1165











## 1.多个pipline的处理

在Scrapy的框架中，其实可以有好多pipline。大家在编写pipline的时候有没有这样的一个疑问，为什么pipline函数最后要有一个语句是



```python
<span style="font-size:18px;">return item</span>
```
    明明item是传入pipline的，怎么又传出去了呢。


    大家还记得在使用pipline的时候，在setting文件中要写一个数字么，

    比如：XXX.XXX.Mypipline : 300

    这里的300就是这个pipline的运行次序。也就是说，你可以写很多pipline去处理一个item。有点像流水线加工哦。很标准的模块化思想呢！

    所以你可以写一个pipline用来清洗数据，一个pipline用来存储。当然，不要忘了后面的序号不要弄错，因为那代表着顺序。

## 2.Request参数解析

    request我们之前使用的时候，一般都只用到了两个参数，一个是url一个是callball调用的回调函数。

其实这个函数还有很多参数。



```python
class scrapy.http.Request(url[, callback, method=’GET’, headers, body, cookies, meta,
encoding=’utf-8’, priority=0, dont_filter=False, errback])
```
  函数的原型是这个样子的，看到，除了url外，其他的参数都是被[]给包含起来的，所以后面的参数都是可选的。
   
 method就是该请求的方式了，比如post，get这种。meat则是一个数据传递的通道。学过android的同学还记得intent方法切换界面吗？好多类的封装过程中都会有这样的一种机制，目的就是为了保证数据的安全传递。




```python
request_with_cookies = Request(url="http://www.example.com",
cookies={'currency': 'USD', 'country': 'UY'},
meta={'dont_merge_cookies': True})
```
    举了例子吧，就是上面这样的。



## 3.发送邮件

    
 Scrapy还可以让你去发送邮件。



```python
from scrapy.mail import MailSender
mailer = MailSender()
mailer = MailSender.from_settings(settings)
mailer.send(to=["someone@example.com"], subject="Some subject", body="Some body", cc=["another@examp])
```
    上面的代码，首先生成了一个MailSender类，用于发送邮件，如果事先在Setting文件中设置好了自己的邮箱那么直接读取设置就可以了。

详细的MailSender类的一些参数和Setting文件的设置，大家可以参考手册的说明，不过，这个功能似乎很少有机会用到吧。











