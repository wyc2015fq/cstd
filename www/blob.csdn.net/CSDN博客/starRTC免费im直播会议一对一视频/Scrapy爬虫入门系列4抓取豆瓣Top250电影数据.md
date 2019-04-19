# Scrapy爬虫入门系列4抓取豆瓣Top250电影数据 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月10日 12:24:38[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：411
所属专栏：[前端技术](https://blog.csdn.net/column/details/31053.html)
                
豆瓣有些电影页面需要登录才能查看。
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974:%E6%8A%93%E5%8F%96%E8%B1%86%E7%93%A3Top250%E7%94%B5%E5%BD%B1%E6%95%B0%E6%8D%AE#)] 
- [1 创建工程](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974:%E6%8A%93%E5%8F%96%E8%B1%86%E7%93%A3Top250%E7%94%B5%E5%BD%B1%E6%95%B0%E6%8D%AE#.E5.88.9B.E5.BB.BA.E5.B7.A5.E7.A8.8B)
- [2 定义Item](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974:%E6%8A%93%E5%8F%96%E8%B1%86%E7%93%A3Top250%E7%94%B5%E5%BD%B1%E6%95%B0%E6%8D%AE#.E5.AE.9A.E4.B9.89Item)
- [3 编写爬虫(Spider)](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974:%E6%8A%93%E5%8F%96%E8%B1%86%E7%93%A3Top250%E7%94%B5%E5%BD%B1%E6%95%B0%E6%8D%AE#.E7.BC.96.E5.86.99.E7.88.AC.E8.99.AB.28Spider.29)
- [4 存储数据](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974:%E6%8A%93%E5%8F%96%E8%B1%86%E7%93%A3Top250%E7%94%B5%E5%BD%B1%E6%95%B0%E6%8D%AE#.E5.AD.98.E5.82.A8.E6.95.B0.E6.8D.AE)
- [5 配置文件](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974:%E6%8A%93%E5%8F%96%E8%B1%86%E7%93%A3Top250%E7%94%B5%E5%BD%B1%E6%95%B0%E6%8D%AE#.E9.85.8D.E7.BD.AE.E6.96.87.E4.BB.B6)
- [6 艺搜参考](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%974:%E6%8A%93%E5%8F%96%E8%B1%86%E7%93%A3Top250%E7%94%B5%E5%BD%B1%E6%95%B0%E6%8D%AE#.E8.89.BA.E6.90.9C.E5.8F.82.E8.80.83)
## 创建工程
scrapy startproject doubanmoive
## 定义Item
*# -*- coding: utf-8 -*-**# Define here the models for your scraped items**# See documentation in:**# http://doc.scrapy.org/en/latest/topics/items.html***from** scrapy.item**import** Item, Field
 
 
**class** DoubanmoiveItem(Item):
    *# define the fields for your item here like:**# name = scrapy.Field()*
    url =Field()*#豆瓣链接*
    name = Field()*#电影名*
    year = Field()*#上映年份*
    score = Field()*#分数*
    vote = Field()*#评价人数*
## 编写爬虫(Spider)
在doubanmoive/spiders目录下新建moive_spider.py文件
*# -*- coding: utf-8 -*-***from** scrapy.selector**import** Selector
**from** scrapy.contrib.spiders**import** CrawlSpider,Rule
**from** scrapy.contrib.linkextractors.sgml**import** SgmlLinkExtractor
**from** doubanmoive.items**import** DoubanmoiveItem
 
**class** MoiveSpider(CrawlSpider):
	name="doubanmoive"
	allowed_domains=["movie.douban.com"]
	start_urls=["http://movie.douban.com/top250"]
	rules=[
		Rule(SgmlLinkExtractor(allow=(r'http://movie.douban.com/top250**\?**start=\d+.*'))),     //d+表示数字，至少一个数字， .* 任意数量的不包含换行的字符
		Rule(SgmlLinkExtractor(allow=(r'http://movie.douban.com/subject/\d+')),callback="parse_item"),]   **def** parse_item(self,response):
		sel=Selector(response)
		item=DoubanmoiveItem()
		item['url']= response.url
		item['name']=sel.xpath('//*[@id="content"]/h1/span[1]/text()').extract()        //   //*选取所有元素
		item['year']=sel.xpath('//*[@id="content"]/h1/span[2]/text()').re(r'\((**\d**+)\)')              //re里的r是什么意思？  
		item['score']=sel.xpath('//*[@id="interest_sectl"]/div/p[1]/strong/text()').extract()
		item['vote']=sel.xpath('//*[@id="interest_sectl"]/div/p[2]/a/span/text()').re(r'\d+')**                    return** item
代码说明：MoiveSpider继承Scrapy中的CrawlSpider，其中rules稍微复杂一些，定义了URL的抓取规则，符合allow正则表达式的链接都会加入到Scheduler（调度程序）。通过分析豆瓣电影Top250的分页URL
http://movie.douban.com/top250?start=25&filter=&type=可以得到以下规则：
Rule**(**SgmlLinkExtractor**(**allow=**(**r'http://movie.douban.com/top250\?start=\d+.*'**)****)****)**,
而我们真正要抓取的页面是每一部电影的详细介绍，如《肖申克的救赎》的链接为[http://movie.douban.com/subject/1292052/，只有subject后面的数字是变化的，根据正则表达式得到如下代码。](http://movie.douban.com/subject/1292052/%EF%BC%8C%E5%8F%AA%E6%9C%89subject%E5%90%8E%E9%9D%A2%E7%9A%84%E6%95%B0%E5%AD%97%E6%98%AF%E5%8F%98%E5%8C%96%E7%9A%84%EF%BC%8C%E6%A0%B9%E6%8D%AE%E6%AD%A3%E5%88%99%E8%A1%A8%E8%BE%BE%E5%BC%8F%E5%BE%97%E5%88%B0%E5%A6%82%E4%B8%8B%E4%BB%A3%E7%A0%81%E3%80%82)
Rule**(**SgmlLinkExtractor**(**allow=**(**r'http://movie.douban.com/subject/\d+'**)****)**,callback="parse_item"**)**,
我们需要抓取这种类型链接中的内容，于是加入callback属性，将Response交给parse_item函数来处理。
在parse_item函数中的处理逻辑非常简单，根据一定的规则抓取内容赋给item并返回Item Pipeline。获取大部分标签的内容不需要编写复杂的正则表达式，我们可以使用XPath。
我们可以通过Chrome开发者工具(F12)来获取某内容的XPath表达式，具体操作为在需要抓取的内容上点击审查元素，下方就会出现开发者工具，并定位到该元素，在内容上点击右键，选择复制XPath。注意最好多测试几个页面看看表达式是否是一样的
## 存储数据
爬虫获取到数据以后我们需要将其存储到数据库中，之前我们提到该操作需要靠项目管道（pipeline）来处理，其通常执行的操作为：
- 清洗HTML数据
- 验证解析到的数据（检查项目是否包含必要的字段）
- 检查是否是重复数据（如果重复就删除）
- 将解析到的数据存储到数据库中
由于我们获取的数据格式多种多样，有一些存储在关系型数据库中并不方便，可以考虑使用MongoDB。
MySQL不太方便的一点就是需要将数组类型的数据通过分隔符转换。而MongoDB支持存入List、Dict等多种类型的数据。
pipelines.py代码如下：
*# -*- coding: utf-8 -*-**# Define your item pipelines here**#**# Don't forget to add your pipeline to the ITEM_PIPELINES setting**# See: http://doc.scrapy.org/en/latest/topics/item-pipeline.html***from** scrapy **import** log
**from** twisted.enterprise**import** adbapi
**from** scrapy.http**import** Request
 
**import** MySQLdb
**import** MySQLdb.cursors**class** DoubanmoivePipeline(object):
    **def**__init__(self):
		self.dbpool= adbapi.ConnectionPool('MySQLdb',
		db ='scrapy',user='root',
		passwd ='pwd',
		cursorclass = MySQLdb.cursors.DictCursor,
		charset ='utf8',
		use_unicode =False)**def** process_item(self, item, spider):
	    query =self.dbpool.runInteraction(self._conditional_insert, item)
	    query.addErrback(self.handle_error)**return** item
 
    **def** _conditional_insert(self,tx,item):
	    tx.execute("select * from doubanmoive where m_name= %s",(item['name'][0],))    //可以通过网页的subject数字进行排重。	    result=tx.fetchone()*#log.msg(result,level=log.DEBUG)**#print result***if** result:
			log.msg("Item already stored in db:%s" % item,level=log.DEBUG)**else**:
			tx.execute(\
			"insert into doubanmoive (m_name,m_year,m_score, url, vote) values (%s,%s,%s,%s,%s)",\
			(item['name'][0],item['year'][0],item['score'][0],item['url'],item['vote']))
			log.msg("Item stored in db: %s" % item, level=log.DEBUG)**def** handle_error(self, e):
			log.err(e)
## 配置文件
在运行爬虫之前还需要将在settings.py中增加一些配置信息。
*# -*- coding: utf-8 -*-**# Scrapy settings for doubanmoive project**#**# For simplicity, this file contains only the most important settings by**# default. All the other settings are documented here:**#**#     http://doc.scrapy.org/en/latest/topics/settings.html**#*
 
BOT_NAME ='doubanmoive'
 
SPIDER_MODULES =['doubanmoive.spiders']
NEWSPIDER_MODULE ='doubanmoive.spiders'
 
ITEM_PIPELINES={'doubanmoive.pipelines.DoubanmoivePipeline':400,}
LOG_LEVEL='DEBUG'
DOWNLOAD_DELAY =3
RANDOMIZE_DOWNLOAD_DELAY =True
USER_AGENT ='Mozilla/5.0 (Macintosh; Intel Mac OS X 10_8_3) AppleWebKit/536.5 (KHTML,like Gecko) Chrome/19.0.1084.54 Safari/536.5'
 
COOKIES_ENABLED =True*# Crawl responsibly by identifying yourself (and your website) on the user-agent**#USER_AGENT = 'doubanmoive (+http://www.yourdomain.com)'*
ITEM_PIPELINES中定义了MySQL的 Pipeline文件，后面的数字代表执行的优先级顺序，范围为0~1000。而中间的DOWNLOAD_DELAY等信息是为了防止爬虫被豆瓣禁掉，增加了一些随机延迟，浏览器代理等。
至此，抓取豆瓣电影的爬虫就已经完成了。在命令行中执行scrapy crawl doubanmoive让蜘蛛开始爬行吧！
报错：403
Crawled (403)
HTTP Status Code 403 definitely means Forbidden / Access Denied.
未解决！！
[https://stackoverflow.com/questions/37283564/difference-between-linkextractor-and-sgmllinkextractor](https://stackoverflow.com/questions/37283564/difference-between-linkextractor-and-sgmllinkextractor)
Link extractors are used in the[`CrawlSpider`](https://doc.scrapy.org/en/latest/topics/spiders.html#scrapy.spiders.CrawlSpider)class
SgmlLinkExtractor  is now deprecated 

Rule:
If`callback`is
 None`follow`defaults
 to`True`
下面这一行报错：
/usr/lib64/python2.7/site-packages/MySQLdb/cursors.py:187:execute
 query = query % tuple([db.literal(item) for item in args]) 
 <type 'exceptions.TypeError'>: not all arguments converted during string formatting
传参数需要
(item['name'][0],)
Adding the comma at the end of the tuple
'exceptions.IndexError'>: list index out of range
注意xpath存储到item中的类型是
`.xpath()`and`.css()`methods
 return a[`SelectorList`](https://doc.scrapy.org/en/latest/topics/selectors.html#scrapy.selector.SelectorList)instance,
 which is alistof
 new selectors.
列表用[ ]标识。访问时需要用index
print list[0] #输出列表的第一个元素
## 艺搜参考
[http://www.ituring.com.cn/article/114408](http://www.ituring.com.cn/article/114408)
[http://www.cnblogs.com/lchd/p/3820968.html](http://www.cnblogs.com/lchd/p/3820968.html)
[http://www.jianshu.com/p/078ad2067419](http://www.jianshu.com/p/078ad2067419)
