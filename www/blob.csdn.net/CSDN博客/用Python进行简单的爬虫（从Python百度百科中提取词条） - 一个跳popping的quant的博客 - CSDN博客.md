# 用Python进行简单的爬虫（从Python百度百科中提取词条） - 一个跳popping的quant的博客 - CSDN博客





2017年09月09日 14:25:01[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：924标签：[python																[爬虫](https://so.csdn.net/so/search/s.do?q=爬虫&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[python](https://blog.csdn.net/FrankieHello/article/category/7159892)

所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)








一、前提准备 
**首先用到的Python版本是3.5.2**

   目的是从Python的百度百科提取Python相关的词条 

   入口网页是[https://baike.baidu.com/item/Python/407313?fr=aladdin](https://baike.baidu.com/item/Python/407313?fr=aladdin)

   编码格式utf-8 

   词条的url格式/item/* 
![这里写图片描述](https://img-blog.csdn.net/20170909134312560?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 二、爬虫的架构 

   这个爬虫的架构如下： 
![这里写图片描述](https://img-blog.csdn.net/20170909140005660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

   url管理器的作用就是存放待爬取和已经爬取的URL。 

   网页下载器的作用就是下载从url管理器中取得的url中的内容，并以字符串的形式下载下来。 

   网页解析器的作用就是解析从网页下载器中下载下来的网页字符串。 

 三、代码编写 

   1、spider_main
```python
# coding:utf8
from pa_test import url_manager, html_downloader, html_outputer, html_parser


class SpiderMain(object):
    def __init__(self):
        self.urls = url_manager.UrlManager()
        self.downloader = html_downloader.HtmlDownloader()
        self.parser = html_parser.HtmlParser()
        self.outputer = html_outputer.HtmlOutputer()

    def craw(self, root_url):
        count = 1
        self.urls.add_new_url(root_url)
        #爬取100条数据
        while self.urls.has_new_url():
            try:
                new_url = self.urls.get_new_url()
                print("craw %d : %s"%(count,new_url))
                #下载该url中的内容
                html_content = self.downloader.download(new_url)
                #解析下载内容中的新的url和该url对应的数据
                new_urls, new_data = self.parser.parse(new_url,html_content)
                self.urls.add_new_urls(new_urls)
                self.outputer.collect_data(new_data)

                count = count+1
                if count == 100:
                    break
            except:
                print("error")

        self.outputer.output_html()

if __name__ == "__main__":
    root_url = "https://baike.baidu.com/item/Python/407313?fr=aladdin"
    #alt+Enter快速修正
    obj_spider = SpiderMain()
    obj_spider.craw(root_url)
```

2、url_manager

```python
class UrlManager(object):
    def __init__(self):
        self.new_urls = set()
        self.old_urls = set()

    def add_new_url(self, url):
        if url is None:
             return
        if url not in self.new_urls and url not in self.old_urls:
            self.new_urls.add(url)

    def add_new_urls(self, urls):
        if urls is None or len(urls) == 0:
            return
        for url in urls:
            self.add_new_url(url)

    def has_new_url(self):
        return len(self.new_urls) != 0

    def get_new_url(self):
        #取出并删除url
        new_url = self.new_urls.pop()
        self.old_urls.add(new_url)
        return new_url
```

3、htm_downloader

```python
import urllib.request as request


class HtmlDownloader(object):

    def download(self, url):
        if url is None:
            return None

        response = request.urlopen(url)

        if response.getcode() != 200:
            return None
        return response.read()
```

4、html_parser

```python
import re
from urllib import parse
from bs4 import BeautifulSoup


class HtmlParser(object):

    def _get_new_urls(self, page_url, soup):
        new_urls = set()
        #找到所有指定格式的a标签
        links = soup.find_all('a', href=re.compile(r"/item/*"))
        for link in links:
            new_url = link['href']
            #将主url和a标签中的url拼接成为新的url
            new_full_url = parse.urljoin(page_url, new_url)
            new_urls.add(new_full_url)
        return new_urls

    def _get_new_data(self, page_url, soup):
        res_data = {}

        res_data['url'] = page_url

        title_node = soup.find('dd', class_="lemmaWgt-lemmaTitle-title").find('h1')
        res_data['title'] = title_node.get_text()

        summary_node = soup.find('div',class_="lemma-summary")
        res_data['summary'] = summary_node.get_text()

        return res_data

    def parse(self, page_url, html_content):
        if page_url is None or html_content is None:
            return
        #ps：我在输出结果的时候输出的都是乱码，解决办法如下：
        #1. 打开文件时直接指定编码
        #fout = open('output.html', 'w', encoding='utf-8')
        #2.写如内容时取消编码
        #fout.write('<td>%s</td>' % data['title'])
        #fout.write('<td>%s</td>' % data['summary'])
        soup = BeautifulSoup(html_content, 'html.parser', from_encoding='utf-8')
        new_urls = self._get_new_urls(page_url, soup)
        new_data = self._get_new_data(page_url,soup)
        return new_urls, new_data
```

这样在项目中有可以找到output.html用浏览器打开即可： 
![这里写图片描述](https://img-blog.csdn.net/20170909142412031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRnJhbmtpZUhlbGxv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





