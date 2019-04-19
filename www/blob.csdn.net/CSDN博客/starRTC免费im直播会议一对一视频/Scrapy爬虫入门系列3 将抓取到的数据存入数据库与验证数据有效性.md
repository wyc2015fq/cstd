# Scrapy爬虫入门系列3 将抓取到的数据存入数据库与验证数据有效性 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月09日 16:03:42[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：367
所属专栏：[前端技术](https://blog.csdn.net/column/details/31053.html)

抓取到的item 会被发送到Item Pipeline进行处理
Item Pipeline常用于
- cleansing HTML data
- validating scraped data (checking that the items contain certain fields)
- checking for duplicates (and dropping them)
- storing the scraped item in a database
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973:%E5%B0%86%E6%8A%93%E5%8F%96%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93#)] 
- [1 写一个自己的item
 pipeline](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973:%E5%B0%86%E6%8A%93%E5%8F%96%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93#.E5.86.99.E4.B8.80.E4.B8.AA.E8.87.AA.E5.B7.B1.E7.9A.84item_pipeline)- [1.1 Price
 validation and dropping items with no prices](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973:%E5%B0%86%E6%8A%93%E5%8F%96%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93#Price_validation_and_dropping_items_with_no_prices)
- [2 写到JSON文件中](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973:%E5%B0%86%E6%8A%93%E5%8F%96%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93#.E5.86.99.E5.88.B0JSON.E6.96.87.E4.BB.B6.E4.B8.AD)
- [3 Duplicates
 filter](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973:%E5%B0%86%E6%8A%93%E5%8F%96%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93#Duplicates_filter)
- [4 Activating
 an Item Pipeline component](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973:%E5%B0%86%E6%8A%93%E5%8F%96%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93#Activating_an_Item_Pipeline_component)
- [5 存入数据库](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973:%E5%B0%86%E6%8A%93%E5%8F%96%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93#.E5.AD.98.E5.85.A5.E6.95.B0.E6.8D.AE.E5.BA.93)
- [6 艺搜参考](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973:%E5%B0%86%E6%8A%93%E5%8F%96%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93#.E8.89.BA.E6.90.9C.E5.8F.82.E8.80.83)
## 写一个自己的item pipeline
就是写一个Python类，并且实现process_item(item, spider)方法
musteither return a Item (or any descendant**子孙**class)
 objectorraise a DropItem exception.
### Price validation and dropping items with no prices
adjusts the price attribute for those items that do not include VAT (price_excludes_vat attribute), and drops those items which don’t contain a price:
如果没有price则丢掉，如果没有price_excludes_vat，调整价格值。
**from** scrapy.exceptions **import** DropItem
 
**class** PricePipeline(object):
 
    vat_factor =1.15**def** process_item(self, item, spider):
        **if** item['price']:
            **if** item['price_excludes_vat']:
                item['price']= item['price'] * self.vat_factor**                    return** item
        **else**:
            **raise** DropItem("Missing price in %s" % item)
## 写到JSON文件中
**import** json
 
**class** JsonWriterPipeline(object):
 
    **def**__init__(self):
        self.file=open('items.jl','wb')**def** process_item(self, item, spider):
        line = json.dumps(dict(item)) + "**\n**"        self.file.write(line)**        return** item
## Duplicates filter
A filter that looks for duplicate items, and drops those items that were already processed. Let say that our items have an unique id, but our spider returns multiples items with the same id:
**from** scrapy.exceptions**import** DropItem
 
**class** DuplicatesPipeline(object):
 
    **def**__init__(self):
        self.ids_seen=set()**def** process_item(self, item, spider):
        **if** item['id']**in **self.ids_seen:
            **raise** DropItem("Duplicate item found: %s" % item)**                   else**:
            self.ids_seen.add(item['id'])**  return** item
## Activating**激活**an Item Pipeline component
在settings.py中加入如下代码：
ITEM_PIPELINES ={'myproject.pipelines.PricePipeline': 300,'myproject.pipelines.JsonWriterPipeline': 800,}
我们在[Scrapy爬虫入门系列2:示例教程](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B&action=edit&redlink=1)的基础上，支持json输出
- 1，先写好pipeline
**import** json
 
**class** TutorialPipeline(object):
 
    **def**__init__(self):
        self.file=open('output.json','wb')**def** process_item(self, item, spider):
        line = json.dumps(dict(item)) + "**\n**"self.file.write(line)**return** item
- 2，然后在settings.py中加入
ITEM_PIPELINES={'tutorial.pipelines.TutorialPipeline':400,}
最后运行scrapy crawl dmoz会生成output.json。
## 存入数据库
打开pipelines.py输入如下：
*# -*- coding: utf-8 -*-**# Define your item pipelines here**#**# Don't forget to add your pipeline to the ITEM_PIPELINES setting**# See: http://doc.scrapy.org/en/latest/topics/item-pipeline.html**# -*- coding: utf-8 -*-***from** scrapy **import** log
**from** twisted.enterprise **import** adbapi
**from** scrapy.http**import** Request
**from** scrapy.selector**import** HtmlXPathSelector
**import **urllib**import** MySQLdb
**import** MySQLdb.cursors**class** TutorialPipeline(object):
	**def**__init__(self):
	self.dbpool= adbapi.ConnectionPool('MySQLdb',db ='scrapy',user='root',passwd ='pwd',
		cursorclass = MySQLdb.cursors.DictCursor,
		charset ='utf8',
		use_unicode =False)**         def** process_item(self, item, spider):
		query =self.dbpool.runInteraction(self._conditional_insert, item)
		query.addErrback(self.handle_error)**               return** item
	**def** _conditional_insert(self,tx,item):
		tx.execute("select * from item where title = %s",(item['title']))
		result=tx.fetchone()*         #       log.msg(result,level=log.DEBUG)**#print result***             if** result:
			log.msg("Item already stored in db:%s" % item,level=log.DEBUG)**             else**:
			tx.execute("insert into item (title) values (%s)",(item['title']))**def** handle_error(self, e):
		log.err(e)
请注意python的缩进，不然会报错。
然后在settings.py里加上：
ITEM_PIPELINES={'tutorial.pipelines.TutorialPipeline':400,}
运行scrapy crawl dmoz，会发现数据成功插入到数据库中：
如果报错：
No module named MySQLdb
解决：
yum install MySQL-python
pip install mysql-python
源码下载：[艺搜下载](http://dl.vmall.com/c09bkfivnj)
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%973:%E5%B0%86%E6%8A%93%E5%8F%96%E5%88%B0%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%85%A5%E6%95%B0%E6%8D%AE%E5%BA%93&action=edit&section=7)]艺搜参考
[http://doc.scrapy.org/en/latest/topics/item-pipeline.html](http://doc.scrapy.org/en/latest/topics/item-pipeline.html)
[http://stackoverflow.com/questions/10845839/writing-items-to-a-mysql-database-in-scrapy](http://stackoverflow.com/questions/10845839/writing-items-to-a-mysql-database-in-scrapy)
[http://www.cnblogs.com/lchd/p/3820968.html](http://www.cnblogs.com/lchd/p/3820968.html)
