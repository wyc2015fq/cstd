# 【py/pyspider抓取】进行简单的静态网页抓取 - CD's Coding - CSDN博客





2015年08月25日 13:16:52[糖果天王](https://me.csdn.net/okcd00)阅读数：3807








## 前言

python脚本写了一段时间之后，大致理解了python抓取的一些简单用法，也尝试着对一些大站点进行了抓取

于是觉得，咱来试着用用看既有的平台吧……




## 话说回来，python的话是这样的



```python
#-*- coding: gbk -*-

from bs4 import BeautifulSoup
import urllib2
import sys
import logging 
import json

Log = "./log/Log.txt"
Data =  "./data/text.txt"

logger=logging.getLogger()
handler=logging.FileHandler(Log)
logger.addHandler(handler)
logger.setLevel(logging.NOTSET)

reload(sys)
sys.setdefaultencoding('utf-8')

UrlHead = "http://blog.csdn.net"
MainUrl = "http://blog.csdn.net/okcd00/article/list/pagesites"


def GetDesc(pagesite) :
    value = "NULL"
    headers = {'User-Agent':'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6'} 
    req = urllib2.Request(url = pagesite, headers = headers)
    #print "%s\t" % (req)
    try :
        html_doc = urllib2.urlopen(req)
    except :
        logger.info("[ERROR]:" + pagesite)
        return value
    soup = BeautifulSoup(html_doc)
    #print "I'm going to get Maintext\n"
    for link in soup.find_all('div') :
        if link.get('class') and link.get('class')[0] == 'article_content' :
            value = link.get_text().replace('\t','').replace('\r','').replace('\n','').replace('\b','').encode('utf-8')
    return value

    
def GetNext(html_doc) :
    value = []
    soup = BeautifulSoup(html_doc)
    #print "I'm going to find hrefs\n"
    for link in soup.find_all('h1') :
        if link.find('a') :
            value.append(link.a['href'])
            #print "%s\n%s\n" % (link.a.string,link.a['href'])
    return value  

	
def MergeUrl(link) :
    try:
        mid = link
        pagesite = UrlHead + mid
        #print "now go to the url: %s" % pagesite
    except:
        logger.info("[ERROR]:" + link)
        return "NULL"
    #print "Now I'm going to request %s \n" % pagesite
    return pagesite


def GetContent(soup) :
    for link in soup :
        #print link
        pagesite, value = "", ""
        if MergeUrl(link) != "NULL":
            pagesite = MergeUrl(link)
            #print "Now getting Desc\n"
            value = GetDesc(pagesite)
            #f.write("%s\t%s\t%s\n" % (link['title'], pagesite, value))
            f = open(Data,'a')
            f.write("%s\t%s\n" % (pagesite, value))
            f.close()
        else:
            print "%s\t Nothing here\n" % pagesite 

		
def GetMainUrl(pagesite) :
    headers = {'User-Agent':'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6'} 
    try:
        req = urllib2.Request(url = pagesite, headers = headers)
        #print "%s\t" % (req)
        html_doc = urllib2.urlopen(req)
        #print "%s\t" % (html_doc)
        #the_page = html_doc.read()
        #print "%s\t" % (the_page)
    except:
        logger.info("[ERROR]:" + pagesite)
        return -1
    url_list = GetNext(html_doc)
    GetContent(url_list)
    return 0

	
if __name__ == '__main__' :
    for i in xrange(1,10) :
        ret = 0
        url = MainUrl.replace("pagesites", str(i))
        ret = GetMainUrl(url)
        if ret == -1 :
            print "no return at %s : %s" % (i,url)
            continue
```
![](https://img-blog.csdn.net/20150825125456259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


python的话由于可读性强，所以多看看别人的程序和代码是提高的关键。

这个代码是我最早版的抓取~~（毕竟是在实习，第二版起的代码抓大站的和增量轮询什么的就不放啦）~~，写的是抓取视图下文章标题的超链接，并抓取链之后的文章正文，以‘\t’分割开。

关于实现，并没有使用过多的麻烦的东西，基础操作的话可以在[上一篇文章](http://blog.csdn.net/okcd00/article/details/47004995)中理解，不同的点在于，这里使用了bs4库，这是一个可以将拖回来的html代码通过bs4自带的函数来筛选所需要行的库。



```python
soup = BeautifulSoup(html_doc)
    #print "I'm going to get Maintext\n"
    for link in soup.find_all('div') :
        if link.get('class') and link.get('class')[0] == 'article_content' :
            value = link.get_text().replace('\t','').replace('\r','').replace('\n','').replace('\b','').encode('utf-8')
    return value
```
比如这一段就是新建bs4对象，然后通过soup.findall('<这里是想要找的关键字>')，来获得所需的行，当然，为了防止传回空值，请记住做try/except哦~



自然不仅仅如此，还可以实现非常多的功能，~~我已经可以抓取十一列相关数据而且增量轮询抓取了哈哈哈，~~在这里就不一一展示了。




## 于是，试试看Pyspider吧



```python
#!/usr/bin/env python
# -*- encoding: utf-8 -*-
# Created on 2015-08-25 11:24:45
# Project: Try_csdnBlo

from pyspider.libs.base_handler import *


class Handler(BaseHandler):
    crawl_config = {
    }

    @every(minutes=24 * 60)
    def on_start(self):
        self.crawl('http://blog.csdn.net/okcd00',callback=self.index_page)

    @config(age=10 * 24 * 60 * 60)
    def index_page(self, response):
        for each in response.doc('h1 a').items():
            self.crawl(each.attr.href, callback=self.detail_page)
        for nxt in response.doc('.pagelist > a').items():
            if nxt.text() == "下一页" :
                self.crawl(nxt.attr.href, callback=self.index_page)
            
    
    @config(priority=2)
    def detail_page(self, response):
        return {
            "url": response.url,
            "title": response.doc('h1 a').text(),
            "text": response.doc('.article_content').text(),
        }
```
![](https://img-blog.csdn.net/20150825125831201?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20150825125914243?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






关于pyspider，使用的是pyQuery语言，可读性也非常强，有个[QuickGuide](http://pyspider.readthedocs.org/en/master/Quickstart/)非常的棒，这是它的文档，可以通过这个入门。

什么你跟我说你看不懂英文？T^T，唉，为什么我就是个这么好的人呢……呐，[中文版的文档](http://blog.binux.me/2015/01/pyspider-tutorial-level-1-html-and-css-selector/)拿去。




## 那么pyspider该怎么用呢

简单的说起来就是——

先把想要抓取的主网页放在on_start(self)的url参数区


```python
def on_start(self):
    # self.crawl(url, callback=self.index_page)* 
    # is the most important API here. It will add a new task to be crawled.
        self.crawl('http://cdisagoodperson.com/', callback=self.index_page)
```






然后在左边点击run，跑起来获得主页面地址，然后点击web就可以和浏览器里一样看到这个页面
![](https://img-blog.csdn.net/20150825131017969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




点击enable css selector helper，右上角就会出现上图右上角的那个东西，左按钮是复制，右按钮是把你选中的那个元素直接贴到你光标所在的位置

![](https://img-blog.csdn.net/20150825131204362?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





模仿给出的函数写出你自己的函数，你中间想叠加几层就写几个，注意传参的时候传你自己的函数名。



```python
@config(age=10 * 24 * 60 * 60)
    # def index_page(self, response) get a Response* object. 
    # response.doc* is a pyquery object which has jQuery-like API 
    # to select elements to be extracted.
    def index_page(self, response):
        for each in response.doc('a[href^="http"]').items():
            self.crawl(each.attr.href, callback=self.detail_page)

    @config(priority=2)
    # def detail_page(self, response) return a dict object as result. 
    # The result will be captured into resultdb by default. 
    # You can override on_result(self, result) method to manage the result yourself.
    def detail_page(self, response):
        return {
            "url": response.url,
            "title": response.doc('title').text(),
        }
```





代码写好没错之后就去Dashboard，把这个任务的属性改成RUNNING，按右边的RUN即可

![](https://img-blog.csdn.net/20150825131421709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





可以通过Result查看结果，点按Action Tasks可以查看日志：

![](https://img-blog.csdn.net/20150825131557424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




