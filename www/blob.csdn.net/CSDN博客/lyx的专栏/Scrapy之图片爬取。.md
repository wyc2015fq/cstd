# Scrapy之图片爬取。 - lyx的专栏 - CSDN博客





2016年06月23日 23:00:10[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：3370








Scrapy有一个很好用的内置功能去获取图片。

首先假设我们要自己写一个获取图片的爬虫吧。那么显然，你需要的就是获取图片链接，然后写一个专门下载图片的pipline。很开心的是，scrapy其实已经给你实现好了这个pipline了，是不是很贴心呢！

好了，我们开始吧。

和一般程序员同学爬取图片动不动就是美女不同，咱们今天爬汽车。



```python
# coding=gbk
from scrapy.spiders import Spider  
from scrapy.selector import Selector  
import scrapy
#from scrapy import log  
  
from photo.items import PhotoItem  
  
  
class photoSpider(Spider):  
    name = "photoSpider"  
    start_urls = [  
        "http://car.autohome.com.cn/pic/" 
    ]  #start url at the first page
    
    def parse(self, response): #这个是spider类中默认的方法，我们做一个重写，response就是那个html文件哦。
        item = PhotoItem() 
        sel = Selector(response)#“html”文件被转化成了一个Selector（选择器）对象哦。这个对象的好处是，可以接受xpath或者css。
        sites = sel.xpath("//img/@src").extract()#提取图片的url地址
        for siteUrl in sites: 
            print siteUrl
            item['url'] = [''+siteUrl]#特别注意，不这么处理会产生错误。
            yield item
```


这就是我的爬虫了，是不是很简单啊。很容易实现，获取的就是这个网页中所有的图片。特别需要我们注意的是
```python
<span style="font-size:14px;">item['url'] = [''+siteUrl]</span>
```
这一句。按照道理，siteUrl就是一个图片的url了，但是我在这里把他变成了列表形式了。直接当做url会有这样的报错：
![](https://img-blog.csdn.net/20160623224806468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

反正这里我还不是特别了解细节，看了一下官当的文档，也没有什么干货。总之记住，url的传入是一个list，而不是一个char。个人估计的原因可能是一个item的url字段如果传入的是list的话，pipline可以处理多个图片的url，效率会更高。比如我们其实可以在上面把所有的url放到一个list里面，然后只抛出一个item。

所以，记住这个错误： raise ValueError('Missing scheme in request url: %s' % self._url)

我们写完了获取图片url的爬虫之后，就要设置pipline了。之前我们都是自己写pipline，现在这个pipline是内置的，所以我们不用自己写了，直接去setting文件里面说明要用就可以了




```python
ITEM_PIPELINES = {   'scrapy.pipelines.images.ImagesPipeline': 1}
IMAGES_URLS_FIELD = 'url'
IMAGES_STORE = r'.'
```
在setting文件里面，首先像申明我们自己的pipline一样进行申明。这个pipline的名称是固定的哦。以后有机会给大家介绍怎么继承这个pipline，然后去实现更多的功能。
接下来就是你的item里图片url的字段了。然后就是你要存储的文件地址。

赶紧去试一试吧。


工程的github地址：https://github.com/Luyixiao/photo/




