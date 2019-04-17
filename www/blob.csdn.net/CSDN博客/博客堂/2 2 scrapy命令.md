# 2.2 scrapy命令 - 博客堂 - CSDN博客





2017年05月12日 17:34:40[最小森林](https://me.csdn.net/u012052268)阅读数：292标签：[爬虫																[scrapy																[命令](https://so.csdn.net/so/search/s.do?q=命令&t=blog)
个人分类：[Python爬虫](https://blog.csdn.net/u012052268/article/category/6889435)







- [2 scrapy命令](#22-scrapy命令)- [全局命令](#1全局命令)
- [项目命令](#2项目命令)





# 2.2 scrapy命令

Scrapy 命令 分为两种：**全局命令** 和 **项目命令**。
- 
全局命令：在哪里都能使用。

- 
项目命令：必须在爬虫项目里面才能使用。


## 1.全局命令

```
version       查看版本

startproject  创建一个新的项目（project大于spider）用法：scrapy startproject tutorial

shell   后跟某个页面，可以看到常用的全局变量如：response。
    # 可以用.re()的正则函数。

runspider   运行一个单独的爬虫文件.py

bench       基准测试，常用来测试本机scrapy有没有安装好。



另外：

fetch         Fetch a URL using the Scrapy downloader

settings      Get settings values
```

## 2.项目命令

```
genspider     （创建默认属性的spider eg：scrapy genspider dmoz_spider domz.org # 爬虫名字 和 作用域）

list    列举当前项目里的所有spider

view    用浏览器打开某个网页（方便观察）

parse   用当前项目里的特定parse函数解析某个页面（检查parse函数的正确性）

crawl         Run a spider


另外：

check         Check spider contracts
```](https://so.csdn.net/so/search/s.do?q=scrapy&t=blog)](https://so.csdn.net/so/search/s.do?q=爬虫&t=blog)




