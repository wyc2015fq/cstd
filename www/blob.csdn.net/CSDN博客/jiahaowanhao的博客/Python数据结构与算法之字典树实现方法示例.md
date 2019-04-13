
# Python数据结构与算法之字典树实现方法示例 - jiahaowanhao的博客 - CSDN博客


2018年04月22日 21:16:42[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：94


[Python数据结构与算法之字典树实现方法示例](http://cda.pinggu.org/view/25340.html)
有的时候，我们本来写得好好的爬虫代码，之前还运行得Ok, 一下子突然报错了。
报错信息如下：
Http 800 Internal internet error
这是因为你的对象网站设置了反爬虫程序，如果用现有的爬虫代码，会被拒绝。
之前正常的爬虫代码如下：
from urllib.request import urlopen
...
html = urlopen(scrapeUrl)
bsObj = BeautifulSoup(html.read(), "html.parser")
这个时候，需要我们给我们的爬虫代码做下伪装，
给它添加表头伪装成是来自浏览器的请求
修改后的代码如下：
import urllib.parse
import urllib.request
from bs4 import BeautifulSoup
...
req = urllib.request.Request(scrapeUrl)
req.add_header('User-Agent', 'Mozilla/4.0 (compatible; MSIE 5.5; Windows NT)')
response = urllib.request.urlopen(req)
html = response.read()
bsObj = BeautifulSoup(html, "html.parser")
Ok,一切搞定，又可以继续爬了。
以上就是本文的全部内容，希望对大家的学习有所帮助

