# lxml xpath 爬取并正常显示中文内容 - Grandyang - 博客园







# [lxml xpath 爬取并正常显示中文内容](https://www.cnblogs.com/grandyang/p/7990505.html)







在使用python爬虫提取中文网页的内容，为了能正确显示中文的内容，在转为字符串时一定要声明编码为utf-8，否则无法正常显示中文，而是显示原编码的字符，并没有正确转换。比如下面这个简单的爬取百度页面的title的示例：



```
import os
import lxml
from urllib2 import urlopen # Mac
# from urllib.request import Request, urlopen # Win
from lxml import etree

hfile = urlopen('http://www.baidu.com').read()
tree = etree.HTML(hfile)
strs = tree.xpath( "//title")
strs = strs[0]
# strs = (etree.tostring(strs)) # 不能正常显示中文
strs = (etree.tostring(strs, encoding = "utf-8", pretty_print = True, method = "html")) # 可以正常显示中文
print (strs)
```



如果不在tostring函数中正确配置的话，会打印出：

```
<title>&#30334;度一下，你就知道</title>
```



而正确的应该是：

```
<title>百度一下，你就知道</title>
```














