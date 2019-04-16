# sjtuLib爬虫-Scrapy - lyx的专栏 - CSDN博客





2016年05月13日 22:01:16[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：773标签：[python																[爬虫																[图书](https://so.csdn.net/so/search/s.do?q=图书&t=blog)
个人分类：[网页爬虫](https://blog.csdn.net/qtlyx/article/category/6124371)





交大的图书馆网站做的真的不好，不好。但是还是要爬。没有做防墙机制，在爬取了15万条记录之后，IP又被图书馆墙了，而且貌似整个实验室都被wall了。。。。

**上代码！**

****

```python
from scrapy.spiders import Spider  
from scrapy.selector import Selector  
import scrapy
#from scrapy import log  
  
from appstore.items import appstoreItem  
  
  
class appstoreSpider(Spider):  
   
    
    name = "appstore" #不要在意爬虫的名字，这只是一个代号
    #allowed_domains = ["www.apple.com"]#这个部分其实只是最为一个控制，是的爬取的过程中所有的url都在这个域名下。也可以没有这个限制。这里就是把其注释掉了
    start_urls = [  
        "http://ourex.lib.sjtu.edu.cn/primo_library/libweb/action/search.do?dscnt=0&dstmp=1462803572893&searchField=callnumber&fn=BrowseRedirect&vid=chinese&searchTxt="  
    ]  #我们开始的网页地址，交大图书馆思源浏览下的按索书号浏览。
    
    #我们的爬虫有点点复制，首先涉及到下一页跳转的问题，然后，为了获取每个数目的信息，还要点进去连接，反正道理都一样。给每个函数定义好功能就很清楚了呢。
  	#讲述之前，我们先来说一下蜘蛛的逻辑吧。
  	#每个蜘蛛都有一个（当然可以不止一个start_url），我们一运行蜘蛛，蜘蛛就会把这个当做入口，然后获得一个网页
  	#我们把这个网页想成是一个html文件就可以了。这个文件呢，会被默认传入prase方法，也就是下面这个方法。
    def parse(self, response): #这个是spider类中默认的方法，我们做一个重写，response就是那个html文件哦。
  
        sel = Selector(response)#“html”文件被转化成了一个Selector（选择器）对象哦。这个对象的好处是，可以接受xpath或者css。
        #sel是一个选择器哦，我们就很方便使用xpath去获取一些数据。有时候呢，我们获取的会是一个数据list，因为你的路径下面也许会有很多并列，同一个level的项目啊。       
        sites = sel.xpath('//*[@id="exlidBrowseResultsEnteries"]/tbody/tr/td/a/@href').extract()  
        #所以呢，我们把这个List做一个遍历，这个遍历呢，就获取了某一页25本书的连接哦。可以用火狐的xpath插件检查哦。
        #不会xpath就去我之前的一篇文章看一下吧http://blog.csdn.net/qtlyx/article/details/51036437
        for siteUrl in sites:  
            siteUrl = "http://ourex.lib.sjtu.edu.cn/primo_library/libweb/action/"+siteUrl
            #获取了书本的怎么办呢？当然是点进去啊！所以你就yield了。yield是什么呢？暂时把他理解成return就可以了，以后写个文章说明一下他和return的区别。
            #siteUrl是每本书对应的链接哦，所以本获取一本书链接就会执行一下下面这个语句，语句里面有一个callback
            #整个的意思呢，就是，我Request一下siteUrl这个链接，把Request的结果当做参数传给 parse_dir_contents 这个函数
            #这种当某件事情发生，传个参数给他的函数，我们叫他回调函数，常常用在监听机制里面。
            #那Request了什么呢?还是理解成点了一下链接获得的html文件吧。然后去看一下parse_dir_contents函数吧。
            yield scrapy.Request(siteUrl, callback=self.parse_dir_contents)
        
        #处理完每一本书的目录，把链接交给回调函数之后，我们的目的就是自动获取下一页的链接了。
        #这部分可比上面简单，同学们自己打开网页，xpath看一下吧。
        #next page  
        urls = sel.xpath('id("resultsNavNoIddown")/a/@href').extract()
        for url in urls:
            #print url
            url = "http://ourex.lib.sjtu.edu.cn/primo_library/libweb/action/" + url
            #print url
            yield scrapy.Request(url, callback=self.parse)
            
    #这是一个回调函数多余的不讲了，在这个回调函数里面，又回调了一个。有没有这样的一种感觉，没点进去一个页面，
    #就多一个回调函数。其实就是这样的。有几个层次们就会有多少个回调函数。        
    def parse_dir_contents(self, response):
        sel = Selector(response)
        inUrls = sel.xpath('//*[@id="exlidResult0-LocationsTab"]/a/@href').extract()
        for url in inUrls:
            url = "http://ourex.lib.sjtu.edu.cn/primo_library/libweb/action/" + url
            print "test url"+url
            yield scrapy.Request(url, callback=self.parse_req)
    
    def parse_req(self, response):   
        sel = Selector(response)
    		#sites = sel.xpath('id('locationsTable0')/x:tbody/x:tr[3]/x:td/x:ul/x:li[2]')
        #for site in sel:
        item = appstoreItem() 
        status = sel.xpath('//*[@id="locationsTable0"]//tr[3]/td/ul/li[2]/text()').extract()
        #status = sel.xpath('//*[@id="exlidResult-1-TabContent"]/div[2]/div/span[1]/span/text()').extract()
        print "lyx status is" + str(status)
        name = sel.xpath('//*[@id="resultsListNoId"]/div[1]/div[1]/div/h1/text()').extract() 
            
        item['status'] = [t.encode('utf-8') for t in status]  
        item['name'] = [l.encode('utf-8') for l in name]  
            
        yield item
   
  
        #log.msg("Append done.",level='INFO')  
        #return items
```
最后说明一下要获取的数据。item中，我们要存的就是书的名字，和他是否被预约的状态。

还有，这些回调函数都是可以并行运行的，比如网页很卡，那么，某一个时刻，可能同时有好多好多多个****

```python
parse_dir_contents（）
```
****
```python
arse_req（）
```
方法在同时运行，想想那场景。。。感觉你拥有了整个互联网。](https://so.csdn.net/so/search/s.do?q=爬虫&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)




