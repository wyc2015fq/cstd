# Scrapy: 10分钟写一个爬虫抓取美女图 - zhusongziye的博客 - CSDN博客





2018年04月25日 20:23:37[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：225








简介


scrapy 是一个 python 下面功能丰富、使用快捷方便的爬虫框架。用 scrapy 可以快速的开发一个简单的爬虫，官方给出的一个简单例子足以证明其强大：

![](https://img-blog.csdn.net/2018042520212925?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

快速开发

当然，开始前，可以先看看之前我们写过的 scrapy 入门文章 《Scrapy 从这里开始：Hello Scrapy》。

![](https://img-blog.csdn.net/20180425202156920?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面开始10分钟倒计时：

1. 初始化项目

> 
scrapy startproject mzt


cd mzt

scrapy genspider meizitu meizitu.com


2. 添加 spider 代码：

定义 scrapy.Item ，添加 image_urls 和 images ，为下载图片做准备。

修改 start_urls 为初始页面, 添加 parse 用于处理列表页， 添加 parse_item 处理项目页面。

![](https://img-blog.csdn.net/20180425202215638?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

3. 修改配置文件：

DOWNLOAD_DELAY = 1 # 添加下载延迟配置

ITEM_PIPELINES = {'scrapy.pipelines.images.ImagesPipeline': 1} # 添加图片下载 pipeline

IMAGES_STORE = '.' # 设置图片保存目录

![](https://img-blog.csdn.net/20180425202226493?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

4. 运行项目：

scrapy crawl meizitu

看，项目运行效果图

![](https://img-blog.csdn.net/20180425202243925?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

等待一会儿，就是收获的时候了

![](https://img-blog.csdn.net/20180425202257827?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/2018042520231157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)








