# python selenium查找网页内容 - zhusongziye的博客 - CSDN博客





2017年09月06日 21:58:40[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：310标签：[python																[selenium查找网页内容](https://so.csdn.net/so/search/s.do?q=selenium查找网页内容&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Selenium自动化测试](https://blog.csdn.net/zhusongziye/article/category/7131148)













```python
# coding=utf-8
```

```python
from selenium import webdriver
import time
import requests
from bs4 import BeautifulSoup as bs

driver = webdriver.Firefox()
url = "http://china.huanqiu.com/article/2016-07/9132061.html?from=bdwz"
driver.get(url)

response = bs(requests.get(url).content, 'html.parser')

# 获取页面内，h1标签的文章标题

print(u"这篇文章的标题是：", response.h1.string)

# 使用find方法，寻找页面内name=source的content的内容

print (u"这篇文章的类型是：", response.find(attrs={'name': 'source'})['content'])

# 从find_all返回的列表中，查找content字段

for content in response.find_all('meta', {'name': 'source'}):
    print(u"这篇文章的类型是：", content['content'])
```






