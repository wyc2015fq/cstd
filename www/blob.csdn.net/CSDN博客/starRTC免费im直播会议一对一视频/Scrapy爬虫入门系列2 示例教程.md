# Scrapy爬虫入门系列2 示例教程 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月05日 12:44:23[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1251
本来想爬下[http://www.alexa.com/topsites/countries/CN](http://www.alexa.com/topsites/countries/CN) 总排名的，但是收费了
只爬了50条数据:
response.xpath('//div[@class="td DescriptionCell"]/p/a/text()').extract()
[u'Baidu.com', u'Qq.com', u'Taobao.com', u'Sohu.com', u'Tmall.com', u'Sina.com.cn', u'360.cn', u'Jd.com', u'Weibo.com', u'Google.com', u'List.tmall.com', u'Youtube.com', u'Hao123.com', u'Google.com.hk', u'Alipay.com', u'Tianya.cn', u'Csdn.net', u'Detail.tmall.com', u'Soso.com', u'China.com', u'So.com', u'Gmw.cn', u'Huaban.com', u'Youku.com', u'Bing.com', u'Wikipedia.org', u'Xinhuanet.com', u'Cctv.com', u'Zhihu.com', u'Google.cn', u'Google.co.jp', u'Sogou.com', u'Babytree.com', u'China.com.cn', u'Mama.cn', u'Douyu.com', u'Cnblogs.com', u'Chinadaily.com.cn', u'Cnzz.com', u'1688.com', u'39.net', u'Bilibili.com', u'163.com', u'Yesky.com', u'Iqiyi.com', u'Yahoo.com', u'Huanqiu.com', u'Eastday.com', u'K618.cn', u'Caijing.com.cn']
我们使用[dmoz](http://www.dmoz.org/)分类目录作为抓取例子。 本教程将引导你完成如下任务：
- 创建一个新的Scrapy项目
- 定义你想提取的Item
- 写一个Spider用来爬行站点，并提取Items
- 写一个Item Pipeline用来存储提取的Items
## 目录
 [[隐藏](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#)] 
- [1 新建工程](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E6.96.B0.E5.BB.BA.E5.B7.A5.E7.A8.8B)
- [2 定义Item](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E5.AE.9A.E4.B9.89Item)
- [3 我们的第一个爬虫(Spider)](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E6.88.91.E4.BB.AC.E7.9A.84.E7.AC.AC.E4.B8.80.E4.B8.AA.E7.88.AC.E8.99.AB.28Spider.29)
- [4 爬爬爬](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E7.88.AC.E7.88.AC.E7.88.AC)- [4.1 发生了什么](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E5.8F.91.E7.94.9F.E4.BA.86.E4.BB.80.E4.B9.88)
- [5 提取Item](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E6.8F.90.E5.8F.96Item)- [5.1 选择器介绍](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E9.80.89.E6.8B.A9.E5.99.A8.E4.BB.8B.E7.BB.8D)
- [5.2 尝试在shell中使用Selectors](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E5.B0.9D.E8.AF.95.E5.9C.A8shell.E4.B8.AD.E4.BD.BF.E7.94.A8Selectors)
- [6 提取数据](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E6.8F.90.E5.8F.96.E6.95.B0.E6.8D.AE)
- [7 使用Item](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E4.BD.BF.E7.94.A8Item)
- [8 保存抓取的数据](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E4.BF.9D.E5.AD.98.E6.8A.93.E5.8F.96.E7.9A.84.E6.95.B0.E6.8D.AE)
- [9 结束语](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E7.BB.93.E6.9D.9F.E8.AF.AD)
- [10 艺搜参考](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B#.E8.89.BA.E6.90.9C.E5.8F.82.E8.80.83)
## 新建工程
进入你想保存代码的目录，然后执行：
scrapy startproject tutorial
其中，tutorial为项目名称。
这个命令会在当前目录下创建一个新目录tutorial，内容如下：
- scrapy.cfg: 项目配置文件
- tutorial/: 项目python模块, 待会代码将从这里导入
- tutorial/items.py: 项目items文件
- tutorial/pipelines.py: 项目管道文件
- tutorial/settings.py: 项目设置文件
- tutorial/spiders: 放置spider的目录
## 定义Item
Items是装载抓取数据的容器。
通过创建一个scrapy.item的类来声明，用scrpy.Field对象定义它的属性.
比如我们要获得站点的名字，url和网站描述，我们定义这三种属性的域。
编辑items.py文件：
**import** scrapy
 
**class** DmozItem(scrapy.Item):
    *# define the fields for your item here like:**# name = scrapy.Field()*
    title = scrapy.Field()
    link   = scrapy.Field()
    desc  = scrapy.Field()
定义这些item能让你用其他Scrapy组件的时候知道你的 items到底是什么。
## 我们的第一个爬虫(Spider)
首先要获取整个网页的所有内容，然后再取出其中有用的部分。
定义用于下载的URL初步列表，如何跟踪链接，以及如何解析这些网页的内容来提取items。
要建立一个Spider，你必须为scrapy.spider创建一个子类，并确定三个主要的属性：
- name：爬虫的名字，它必须是唯一的，不同的爬虫必须定义不同的名字.
- start_urls：爬虫开始爬的一个URL列表。爬虫从这里开始抓取数据，所以，第一次下载的数据将会从这些URLS开始。其他子URL将会从这些起始URL中生成。
- parse()：爬虫的方法，调用时，传入每一个URL返回的Response对象作为参数，response是parse方法的唯一的一个参数。
这个方法负责解析返回的数据、匹配抓取的数据(解析为item)并跟踪更多的URL。
下面是我们的第一只爬虫代码，将其命名为dmoz_spider.py并保存在tutorial\spiders目录下。
**import** scrapy
 
**class** DmozSpider(scrapy.Spider):
    name ="dmoz"allowed_domains=["dmoz.org"]
    start_urls =["http://www.dmoz.org/Computers/Programming/Languages/Python/Books/","http://www.dmoz.org/Computers/Programming/Languages/Python/Resources/"]**def** parse(self, response):
        filename = response.url.split("/")[-2]**      with **open(filename,'wb')**as** f:
            f.write(response.body)
allow_domains规定爬虫只爬取这个域名下的网页。
从parse函数可以看出，两个文件被创建：分别是 Books 和 Resources(因为split分割后最后2个元素是'Books', ''，L[-2]表示反向读取；读取倒数第二个元素)，保存的是网页的源代码
## 爬爬爬
为了让我们的爬虫工作，我们返回项目根目录执行以下命令：
scrapy crawl dmoz
exceptions.ImportError: No module named _sqlite3
解决：
yum -y install sqlite-devel  
find / -name _sqlite3.so
/usr/lib64/python2.6/lib-dynload/_sqlite3.so
whereis python
 cp /usr/lib64/python2.6/lib-dynload/_sqlite3.so /usr/local/lib/python2.7/lib-dynload/
输入python命令后，
crawl dmoz 命令从dmoz.org域启动爬虫。 你将会获得如下类似输出
2014-06-2318:13:07-0400 **[**scrapy**]** INFO: Scrapy started **(**bot: tutorial**)**2014-06-2318:13:07-0400 **[**scrapy**]** INFO: Optional features available: ...2014-06-2318:13:07-0400 **[**scrapy**]** INFO: Overridden settings: **{****}**2014-06-2318:13:07-0400 **[**scrapy**]** INFO: Enabled extensions: ...2014-06-2318:13:07-0400 **[**scrapy**]** INFO: Enabled downloader middlewares: ...2014-06-2318:13:07-0400 **[**scrapy**]** INFO: Enabled spider middlewares: ...2014-06-2318:13:07-0400 **[**scrapy**]** INFO: Enabled item pipelines: ...2014-06-2318:13:07-0400 **[**dmoz**]** INFO: Spider opened2014-06-2318:13:08-0400 **[**dmoz**]** DEBUG: Crawled **(**200**)****<**GET http:**//**www.dmoz.org**/**Computers**/**Programming**/**Languages**/**Python**/**Resources**/>****(**referer: None**)**2014-06-2318:13:09-0400 **[**dmoz**]** DEBUG: Crawled **(**200**)****<**GET http:**//**www.dmoz.org**/**Computers**/**Programming**/**Languages**/**Python**/**Books**/>****(**referer: None**)**2014-06-2318:13:09-0400 **[**dmoz**]** INFO: Closing spider **(**finished**)**
最后一句INFO: Closing spider (finished)表明爬虫已经成功运行并且自行关闭了。
注意包含 [dmoz]的行 ，那对应着我们的爬虫运行的结果。你可以看到start_urls中定义的每个URL都有日志行。因为这些URL是起始页面，所以他们没有引用(referrers)，所以在每行的末尾你会看到
 (referer: <None>).
在parse方法的作用下，两个文件被创建：分别是 Books 和 Resources。
### 发生了什么
那么在刚才的电闪雷鸣之中到底发生了什么呢？
Scrapy为爬虫start_urls属性中的每个URL创建了一个 scrapy.Request
 对象， 并将爬虫的parse 方法指定为回调函数。
然后这些 Request被调度并执行，之后通过parse()方法返回scrapy.http.Response 对象，并反馈给爬虫。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B&action=edit&section=6)]提取Item
### [[编辑](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B&action=edit&section=7)]选择器介绍
有多种方法从网页中提取数据。
Scrapy 使用基于 XPath的机制，或称为Scrapy Selectors的CSS表达式。
如果你想了解更多selectors和其他机制，请参见：[http://doc.scrapy.org/en/latest/topics/selectors.html#topics-selectors](http://doc.scrapy.org/en/latest/topics/selectors.html#topics-selectors)
下面是一些XPath表达式的例子及他们的含义
- /html/head/title: 选择HTML文档<head>元素下面的<title> 标签。
- /html/head/title/text(): 选择前面提到的<title> 元素下面的文本内容
- //td: 选择所有 <td> 元素
- //div[@class="mine"]: 选择所有包含 class="mine" 属性的div 标签元素
这只是几个使用XPath的简单例子，实际上XPath非常强大。
如果你想了解更多XPATH的内容，参见：[http://www.w3schools.com/XPath/default.asp](http://www.w3schools.com/XPath/default.asp) 或[http://www.w3school.com.cn/xpath/](http://www.w3school.com.cn/xpath/)
为了方便使用XPaths，Scrapy提供了一个 Selector class and convenient shortcuts to avoid instantiating selectors yourself everytime you need to select something from a response.
You can see selectors as objects that represent nodes in the document structure. So, the first instantiated selectors are associated with the root node, or the entire document.
Selectors拥有4个 basic methods：
- xpath():返回一系列的selectors，xpath参数表达式选择的节点
- css():返回一系列的selectors，css参数表达式选择的节点
- extract(): returns a unicode string with the selected data. 返回一个unicode字符串，为选中的数据
- re():返回一串unicode字符串，为使用正则表达式抓取出来的内容

### [[编辑](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B&action=edit&section=8)]尝试在shell中使用Selectors
为了演示Selectors的用法，我们将用到 内建的Scrapy shell，这需要系统已经安装IPython (一个扩展的python交互环境) 。
进入项目根目录，然后输入：
scrapy shell "[http://www.dmoz.org/Computers/Programming/Languages/Python/Books/](http://www.dmoz.org/Computers/Programming/Languages/Python/Books/)"
注意url要用双引号括起来。单引号也可以
**[** ... Scrapy log here ... **]**2014-01-2317:11:42-0400 **[**default**]** DEBUG: Crawled **(**200**)****<**GET http:**//**www.dmoz.org**/**Computers**/**Programming**/**Languages**/**Python**/**Books**/>****(**referer: None**)****[**s**]** Available Scrapy objects:
**[**s**]**   crawler    **<**scrapy.crawler.Crawler object at 0x3636b50**>****[**s**]**   item       **{****}****[**s**]**   request    **<**GET http:**//**www.dmoz.org**/**Computers**/**Programming**/**Languages**/**Python**/**Books**/>****[**s**]**   response   **<**200 http:**//**www.dmoz.org**/**Computers**/**Programming**/**Languages**/**Python**/**Books**/>****[**s**]**   settings   **<**scrapy.settings.Settings object at 0x3fadc50**>****[**s**]**   spider     **<**Spider 'default' at 0x3cebf50**>****[**s**]** Useful shortcuts:
**[**s**]**   shelp**(****)**           Shell **help****(**print this **help****)****[**s**]**   fetch**(**req_or_url**)** Fetch request **(**or URL**)** and update **local** objects
**[**s**]**   view**(**response**)**    View response **in** a browser
 
In **[**1**]**:
Shell载入后，你将获得response回应，存储在本地变量 response 中，所以如果你输入response.body 你将会看到response的body部分，也就是抓取到的页面内容源代码，或者输入response.headers
 来查看它的 header部分。
response.headers
 
**{**'Cteonnt-Length': **[**'33758'**]**, 'Content-Language': **[**'en'**]**, 'Set-Cookie': **[**'JSESSIONID=D3A77F68542FFABB68E306C30D69FD4B; Path=/'**]**, 
'Server': **[**'Apache'**]**, 'Date': **[**'Sun, 13 Jul 2014 04:21:20 GMT'**]**, 'Content-Type': **[**'text/html;charset=UTF-8'**]****}**
现在就像是一大堆沙子握在手里，里面藏着我们想要的金子，所以下一步，就是用筛子摇两下，把杂质筛出去，选出关键的内容。
selector就是这样一个筛子。
现在的Shell为我们准备好的selector对象是sel，可以根据返回的数据类型自动选择最佳的解析方案(XML or HTML)。
More important, if you type response.selector you will access a selector object you can use to query the response, and convenient shortcuts like response.xpath() and response.css() mapping to response.selector.xpath()
 and response.selector.css()
In **[**1**]**: response.xpath**(**'//title'**)**
Out**[**1**]**: **[****<**Selector xpath='//title'data=u'<title>Open Directory - Computers: Progr'**>****]**
 
In **[**2**]**: response.xpath**(**'//title'**)**.extract**(****)**
Out**[**2**]**: **[**u'<title>Open Directory - Computers: Programming: Languages: Python: Books</title>'**]**
 
In **[**3**]**: response.xpath**(**'//title/text()'**)**
Out**[**3**]**: **[****<**Selector xpath='//title/text()'data=u'Open Directory - Computers: Programming:'**>****]**
 
In **[**4**]**: response.xpath**(**'//title/text()'**)**.extract**(****)**
Out**[**4**]**: **[**u'Open Directory - Computers: Programming: Languages: Python: Books'**]**
 
In **[**5**]**: response.xpath**(**'//title/text()'**)**.re**(**'(\w+):'**)  //匹配单词后面是冒号的**
Out**[**5**]**: **[**u'Computers', u'Programming', u'Languages', u'Python'**]****从上面可以看出，****加了text()后再**extract**(****)  ，会没有html的标签：**<title>
使用response.xpath('//title')就能把这个标签取出来，用extract()和text()还可以进一步做处理。
下面列出了最有用的路径表达式：
- nodename 选取此节点的所有子节点。
- / 从根节点选取。
- // 从匹配选择的当前节点选择文档中的节点，而不考虑它们的位置。
- . 选取当前节点。
- .. 选取当前节点的父节点。
- @ 选取属性。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B&action=edit&section=9)]提取数据
现在我们尝试从网页中提取一些数据。
你可以在控制台输入 response.body， 检查源代码中的 XPaths 是否与预期相同。然而，检查HTML源代码是件很枯燥的事情。
为了使事情变得简单，我们使用Firefox的扩展插件Firebug。更多信息请查看[http://doc.scrapy.org/en/latest/topics/firebug.html#topics-firebug](http://doc.scrapy.org/en/latest/topics/firebug.html#topics-firebug) 和[http://doc.scrapy.org/en/latest/topics/firefox.html#topics-firefox](http://doc.scrapy.org/en/latest/topics/firefox.html#topics-firefox)
检查源代码后，你会发现我们需要的数据在一个
<**ul**>
元素中，
我们可以通过如下命令来抓取这个
<**li**>
标签：sel.xpath('//ul/li')  //选取所有ul,而不管他们的位置
然后是网站描述:
sel.xpath('//ul/li/text()').extract()
网站标题:
sel.xpath('//ul/li/a/text()').extract()
网站链接:
sel.xpath('//ul/li/a/@href').extract()
如：
>>>response.xpath('//ul/li[1]/text()').extract()
>>>response.xpath('//ul/li[1]/a/text()').extract()
输出如下：
[u'Top', u'Computers: Programming: Languages: Python: Resources', u'German', u'Core Python Programming']
为所有ul下第一个li的a标签文字，一个文档源文件中可能有多个ul，
输入Ctrl+D退出
response.xpath('//ul/li[1]/a/@href').extract()
[u'/', u'/Computers/Programming/Languages/Python/Resources/', u'/World/Deutsch/Computer/Programmieren/Sprachen/Python/B%C3%BCcher/',                                     u'[http://www.pearsonhighered.com/educator/academic/product/0,,0130260363,00%2Ben-USS_01DBC.html'](http://www.pearsonhighered.com/educator/academic/product/0,,0130260363,00%2Ben-USS_01DBC.html' rel=)]
如前所述，每个path()调用返回一个selectors列表，所以我们可以结合path()去挖掘更深的节点：
**for** sel **in** response.xpath('//ul/li')
    title = sel.xpath('a/text()').extract()
    link = sel.xpath('a/@href').extract() desc = sel.xpath('text()').extract()**print** title, link, desc
更多关于嵌套选择器的内容，请阅读 [Nesting selectors](http://doc.scrapy.org/en/latest/topics/selectors.html#topics-selectors-nesting-selectors) 和[Working
 with relative XPaths](http://doc.scrapy.org/en/latest/topics/selectors.html#topics-selectors-relative-xpaths)
我们用shell做了这么久的实战，最后我们可以把前面学习到的内容应用到dmoz_spider这个爬虫中。
将代码添加到爬虫中：
**import** scrapy
 
**class** DmozSpider(scrapy.Spider):
    name ="dmoz"
    allowed_domains =["dmoz.org"]
    start_urls =["http://www.dmoz.org/Computers/Programming/Languages/Python/Books/","http://www.dmoz.org/Computers/Programming/Languages/Python/Resources/"]**def** parse(self, response):
        **for** sel **in** response.xpath('//ul/li'):
            title = sel.xpath('a/text()').extract()
            link = sel.xpath('a/@href').extract()
            desc = sel.xpath('text()').extract()**print** title, link, desc
再次抓取dmoz.org，你将看到输出，运行命令:
scrapy crawl dmoz 
为了查看方便，可以只print title
成功的抓到了所有的标题。但是好像不太对啊，怎么Top，Python这种导航栏也抓取出来了呢？

我们只需要红圈中的内容：
看来是我们的xpath语句有点问题，没有把我们需要的项目名称抓取出来。   因为有多个ul
审查网页源代码元素发现我们需要的
<**ul**>
具有class='directory-url'的属性，那么只要把xpath语句改成sel.xpath('//ul[@class="directory-url"]/li')即可
**import** scrapy
 
**class** DmozSpider(scrapy.Spider):
    name ="dmoz"
    allowed_domains =["dmoz.org"]
    start_urls =["http://www.dmoz.org/Computers/Programming/Languages/Python/Books/","http://www.dmoz.org/Computers/Programming/Languages/Python/Resources/"]**def** parse(self, response):
        **for** sel **in** response.xpath('//ul[@class="directory-url"]/li'):
            title = sel.xpath('a/text()').extract()
            link = sel.xpath('a/@href').extract()
            desc = sel.xpath('text()').extract()**print** title
成功抓出了所有的标题：
## 使用Item
Item对象是自定义的python字典，可以使用标准字典语法获取某个属性的值：
**>>>** item = DmozItem**(****)****>>>** item**[**'title'**]** = 'Example title'**>>>** item**[**'title'**]**'Example title'
作为一只爬虫，Spiders希望能将其抓取的数据存放到Item对象中。为了返回我们抓取到的数据，spider的最终代码应当是这样:
**import** scrapy
 
**from** tutorial.items **import** DmozItem
 
**class** DmozSpider(scrapy.Spider):
    name ="dmoz"
    allowed_domains =["dmoz.org"]
    start_urls =["http://www.dmoz.org/Computers/Programming/Languages/Python/Books/","http://www.dmoz.org/Computers/Programming/Languages/Python/Resources/"]**def** parse(self, response):
        **for** sel **in** response.xpath('//ul[@class="directory-url"]/li'):
            item = DmozItem()
            item['title']= sel.xpath('a/text()').extract()
            item['link']= sel.xpath('a/@href').extract()
            item['desc']= sel.xpath('text()').extract()**yield** item
重新运行scrapy crawl dmoz即可看到和上面一样的效果。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B&action=edit&section=11)]保存抓取的数据
保存信息的最简单的方法是通过[Feed exports](http://doc.scrapy.org/en/latest/topics/feed-exports.html#topics-feed-exports)，命令如下：
scrapy crawl dmoz -o items.json -t json
-o 后面是导出文件名，-t 后面是导出类型。
所有抓取的items将以JSON格式被保存在新生成的items.json 文件中。（为了方便显示，在item中删去了除了title之外的属性）
像本教程这样的小型项目，这些已经足够。然而，如果你想用抓取的items做更复杂的事情，你可以写一个 Item Pipeline(条目管道)。
因为在项目创建的时候，一个专门用于条目管道的占位符文件已经随着items一起被建立，目录在tutorial/pipelines.py。
如果你只需要存取这些抓取后的items的话，就不需要去实现任何的条目管道。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B&action=edit&section=12)]结束语
本教程简要介绍了Scrapy的使用，但是许多其他特性并没有提及。
我们将在后面的系列中一一举例说明。
## [[编辑](http://192.168.1.100/elesos_com/index.php?title=Scrapy%E7%88%AC%E8%99%AB%E5%85%A5%E9%97%A8%E7%B3%BB%E5%88%972:%E7%A4%BA%E4%BE%8B%E6%95%99%E7%A8%8B&action=edit&section=13)]艺搜参考
[http://doc.scrapy.org/en/latest/intro/tutorial.html#intro-tutorial](http://doc.scrapy.org/en/latest/intro/tutorial.html#intro-tutorial)
[http://www.cnblogs.com/txw1958/archive/2012/07/16/scrapy-tutorial.html](http://www.cnblogs.com/txw1958/archive/2012/07/16/scrapy-tutorial.html)
[http://blog.csdn.net/pleasecallmewhy/article/details/19642329](http://blog.csdn.net/pleasecallmewhy/article/details/19642329)
