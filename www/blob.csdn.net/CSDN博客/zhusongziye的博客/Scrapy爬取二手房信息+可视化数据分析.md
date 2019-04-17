# Scrapy爬取二手房信息+可视化数据分析 - zhusongziye的博客 - CSDN博客





2018年04月23日 22:36:11[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：575








本篇介绍一个`scrapy`的实战爬虫项目，并对爬取信息进行简单的数据分析。目标是`北京二手房信息`，下面开始分析。

## 网页结构分析

采用安居客网页信息作为二手房的信息来源，直接点击进入二手房信息的页面。

![](https://img-blog.csdn.net/20180423223300253?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




每页的住房信息：

![](https://img-blog.csdn.net/20180423223312277?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




点开链接后的详细信息：

![](https://img-blog.csdn.net/20180423223340632?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




博主并没有采用分区域进行爬取，博主是直接进行全部爬取，然后循环下一页完成的。步骤很简单，如下：



- 
先把每一页的所有二手住房详细链接爬取到

- 
请求每一个爬取到的详细链接，解析住房信息

- 
完成所有解析后，请求下一页的链接

- 
返回步骤一循环，直到返回内容为空


## Scrapy代码实现




## 数据结构定义




`Scrapy`中的元数据`field`其实是继承了`Python`中的`字典`数据类型，使用起来很方便，博主直接定义了几个住房的信息，如下代码所示。当然还有高级的用法，配合`itemloader`加入`processor`，这里只使用简单的定义即可。




```
class AnjukeItem(scrapy.Item):
    # define the fields for your item here like:
    # name = scrapy.Field()
    price = scrapy.Field()
    mode = scrapy.Field()
    area = scrapy.Field()
    floor = scrapy.Field()
    age = scrapy.Field()
    location = scrapy.Field()
    district = scrapy.Field()
    pass
```




爬虫解析
- 
定义了一个继承`Scrapy`自带的爬虫类`Spider`。

- 
然后一个必不可少的东西是`name`，它贯穿了整个`Scrapy`的始终，后面会看到它的作用。

- 
`start_urls`是初始请求的url的列表，也可以有多个初始url，这里只有一个。

- 
由于`Scrapy`的`Spider`类中默认使用了`Request`请求，因此这里选择不覆盖`Request`，使用默认请求，且请求中调用`parse`回调函数。

- 
解析部分用`Scrapy`的高级`selector`选择器的`xpath`进行解析。






`parse`函数请求中有两个`yield`，代表生成器。
- 
第一个`yield`返回每一页的下一页链接`next_page`url。

- 
第二个`yield`返回每一页所有的住房详细链接，并再次`Request`请求跟进，然后调用下一个回调函数`parse_detail`。





请求的过程中如果速度过快，会要求输入验证码，这里放慢了请求速度，暂不处理验证部分（后续慢慢介绍）。




```
class AnjukeSpider(scrapy.Spider):
    name = 'anjuke'
    # custom_settings = {
    #     'REDIRECT_ENABLED': False
    # }
    start_urls = ['https://beijing.anjuke.com/sale/']
    def parse(self, response):
        # 验证码处理部分
        pass
        # next page link
        next_url = response.xpath(
            '//*[@id="content"]/div[4]/div[7]/a[7]/@href').extract()[0]
        print('*********' + str(next_url) + '**********')
        if next_url:
            yield scrapy.Request(url=next_url,
                                 callback=self.parse)
        # 爬取每一页的所有房屋链接
        num = len(response.xpath(
            '//*[@id="houselist-mod-new"]/li').extract())
        for i in range(1, num + 1):
            url = response.xpath(
                '//*[@id="houselist-mod-new"]/li[{}]/div[2]/div[1]/a/@href'
                    .format(i)).extract()[0]
            yield scrapy.Request(url, callback=self.parse_detail)
```

`parse_detail`回调函数中使用`itemloader`解析`items`住房信息，并返回载有信息的`item`。

```
def parse_detail(self, response):
        houseinfo = response.xpath('//*[@class="houseInfo-wrap"]')
        if houseinfo:
            l = ItemLoader(AnjukeItem(), houseinfo)
            l.add_xpath('mode', '//div/div[2]/dl[1]/dd/text()')
            l.add_xpath('area', '//div/div[2]/dl[2]/dd/text()')
            l.add_xpath('floor', '//div/div[2]/dl[4]/dd/text()')
            l.add_xpath('age', '//div/div[1]/dl[3]/dd/text()')
            l.add_xpath('price', '//div/div[3]/dl[2]/dd/text()')
            l.add_xpath('location', '//div/div[1]/dl[1]/dd/a/text()')
            l.add_xpath('district', '//div/div[1]/dl[2]/dd/p/a[1]/text()')
            yield l.load_item()
```




数据清洗

由于爬取后的`items`数据很乱，有各种`\n,\t`等符号，因此在`pipelines`中进行简单的清理工作，使用`正则表达式`实现，代码如下：




```
import re
def list2str(value):
    new = ''.join(value).strip()
    return new
class AnjukePipeline(object):
    def process_item(self, item, spider):
        area = item['area']
        price = item['price']
        loc = item['location']
        district = item['district']
        mode = item['mode']
        age = item['age']
        floor = item['floor']
        modes = list2str(mode)
        item['area'] = int(re.findall(r'\d+', list2str(area))[0])
        item['age'] = int(re.findall(r'\d+', list2str(age))[0])
        item['floor'] = list2str(floor)
        item['location'] = list2str(loc)
        item['district'] = list2str(district)
        item['price'] = int(re.findall(r'\d+', list2str(price))[0])
        item['mode'] = modes.replace('\t', '').replace('\n', '')
        return item
```

别忘记在`setting`里面设置`pipeline`参数。


```
ITEM_PIPELINES = {
   'anjuke.pipelines.AnjukePipeline': 300,
}
```




命令行运行


我们想要将爬取的数据输出到一个文件中，`csv`或者`json`，我们这里输出为`csv`格式的文件。

在`Scrapy`中只需要一个command指令即可完成，在项目文件下的命令行输入：




```
scrapy crawl anjuke -o items.csv
```

命令行中的`anjuke`就是最开始我们定义的`name`。

开始进行爬取：

![](https://img-blog.csdn.net/20180423223407225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




## 数据可视化分析

爬取数据后，我们得到了一个`csv`文件，打开显示如下：

![](https://img-blog.csdn.net/20180423223424266?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




然后，我们将使用`jupyter notebook`进行数据分析，代码如下：

![](https://img-blog.csdn.net/20180423223441491?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





简单分析一下各大区的每平米二手房单价和各大区二手房数量，数据仅为部分，博主没等数据全部爬取完，仅供参考。当然也可根据实际情况进行更复杂的数据分析和机器学习进行房价预测。




效果图如下：






![](https://img-blog.csdn.net/20180423223511194?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180423223523959?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 总结

本篇只是一个简单的例子，一个完整的高效的爬虫还有很多需要完善。
- 
加入代理ip池

- 
scrapd的部署分布式爬虫

- 
增量式的爬虫考虑

- 
....



这些将在后续会慢慢进行介绍，完毕。



