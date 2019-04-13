
# Python爬虫的两套解析方法和四种爬虫实现 - 喜欢打酱油的老鸟 - CSDN博客


2018年07月20日 12:51:26[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：107


[http://blog.itpub.net/31077337/viewspace-2157261/](http://blog.itpub.net/31077337/viewspace-2157261/)
对于大多数朋友而言，爬虫绝对是学习python的最好的起手和入门方式。因为爬虫思维模式固定，编程模式也相对简单，一般在细节处理上积累一些经验都可以成功入门。本文想针对某一网页对python基础爬虫的两大解析库（BeautifulSoup和lxml）和几种信息提取实现方法进行分析，以开python爬虫之初见。
基础爬虫的固定模式
笔者这里所谈的基础爬虫，指的是不需要处理像异步加载、验证码、代理等高阶爬虫技术的爬虫方法。一般而言，基础爬虫的两大请求库urllib和requests中requests通常为大多数人所钟爱，当然urllib也功能齐全。两大解析库BeautifulSoup因其强大的HTML文档解析功能而备受青睐，另一款解析库lxml在搭配xpath表达式的基础上也效率提高。就基础爬虫来说，两大请求库和两大解析库的组合方式可以依个人偏好来选择。
笔者喜欢用的爬虫组合工具是：
requests+BeautifulSoup
requests+lxml

**同一网页爬虫的四种实现方式**
笔者以腾讯新闻首页的新闻信息抓取为例。
首页外观如下：
![](http://img.blog.itpub.net/blog/attachment/201807/3/31077337_1530608988IITt.jpg?x-oss-process=style/bb)
比如说我们想抓取每个新闻的标题和链接，并将其组合为一个字典的结构打印出来。首先查看HTML源码确定新闻标题信息组织形式。
![](http://img.blog.itpub.net/blog/attachment/201807/3/31077337_1530608999qJd2.jpg?x-oss-process=style/bb)
可以目标信息存在于em标签下a标签内的文本和href属性中。可直接利用requests库构造请求，并用BeautifulSoup或者lxml进行解析。
方式一：requests+BeautifulSoup+select css选择器
\# select methodimportrequestsfrombs4importBeautifulSoup
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.119 Safari/537.36'}
url ='http://news.qq.com/'Soup = BeautifulSoup(requests.get(url=url, headers=headers).text.encode("utf-8"),'lxml')
em = Soup.select('em[class="f14 l24"] a')foriinem:
    title = i.get_text()
    link = i['href']
    print({'标题': title,'链接': link
    })很常规的处理方式，抓取效果如下：
![](http://img.blog.itpub.net/blog/attachment/201807/3/31077337_1530609020XPfE.jpg?x-oss-process=style/bb)
方式二：requests+BeautifulSoup+find_all进行信息提取
\# find_all methodimportrequestsfrombs4importBeautifulSoup
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.119 Safari/537.36'}
url ='http://news.qq.com/'Soup = BeautifulSoup(requests.get(url=url, headers=headers).text.encode("utf-8"),'lxml')
em = Soup.find_all('em', attrs={'class':'f14 l24'})foriinem:
    title = i.a.get_text()
    link = i.a['href']
    print({'标题': title,'链接': link
    })同样是requests+BeautifulSoup的爬虫组合，但在信息提取上采用了find_all的方式。效果如下：
![](http://img.blog.itpub.net/blog/attachment/201807/3/31077337_1530609089ta9l.jpg?x-oss-process=style/bb)
方式三：requests+lxml/etree+xpath表达式
\# lxml/etree methodimportrequestsfromlxmlimportetree
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.119 Safari/537.36'}
url ='http://news.qq.com/'html = requests.get(url = url, headers = headers)
con = etree.HTML(html.text)
title = con.xpath('//em[@class="f14 l24"]/a/text()')
link = con.xpath('//em[@class="f14 l24"]/a/@href')foriinzip(title, link):
    print({'标题': i[0],'链接': i[1]
    })使用lxml库下的etree模块进行解析，然后使用xpath表达式进行信息提取，效率要略高于BeautifulSoup+select方法。这里对两个列表的组合采用了zip方法。效果如下：
![](http://img.blog.itpub.net/blog/attachment/201807/3/31077337_1530609108D7CG.jpg?x-oss-process=style/bb)
方式四：requests+lxml/html/fromstring+xpath表达式
\# lxml/html/fromstring methodimportrequestsimportlxml.htmlasHTML
headers = {'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.119 Safari/537.36'}
url ='http://news.qq.com/'con = HTML.fromstring(requests.get(url = url, headers = headers).text)
title = con.xpath('//em[@class="f14 l24"]/a/text()')
link = con.xpath('//em[@class="f14 l24"]/a/@href')foriinzip(title, link):
    print({'标题': i[0],'链接': i[1]
    })跟方法三类似，只是在解析上使用了lxml库下的html.fromstring模块。抓取效果如下：
![](http://img.blog.itpub.net/blog/attachment/201807/3/31077337_15306091256Q0W.jpg?x-oss-process=style/bb)
很多人觉得爬虫有点难以掌握，因为知识点太多，需要懂前端、需要python熟练、还需要懂数据库，更不用说正则表达式、XPath表达式这些。其实对于一个简单网页的数据抓取，不妨多尝试几种抓取方案，举一反三，也更能对python爬虫有较深的理解。长此以往，对于各类网页结构都有所涉猎，自然经验丰富，水到渠成。

