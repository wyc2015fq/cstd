# Scrapy的几个命令行命令 - lyx的专栏 - CSDN博客





2016年06月20日 21:34:41[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：1068








      Scrapy在命令行有许多直接可以使用的命令。命令可以分为两类，一类是依赖于工程的，一类是不依赖的。对于不依赖的，就当做是一个工具就行了，有点java里面静态方法的感觉，就是一个tool。

      依赖于工程的方法有这么几个。

1.list，进入工程文件夹之后，输入scrapy list就可以获取该工程下所有的爬虫名字。

2.genspider。顾名思义，就是用来产生爬虫的。也是一个依赖于工程的。前面记得加scrapy哦。在后面输入爬虫名字和随便输一个网站就可以了。你会发现在你的爬虫文件夹里面多了好多爬虫。

![](https://img-blog.csdn.net/20160620212650229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




      下面介绍一个不依赖与工程的命令，shell，这是一个交互式调试工具，这么理解就可以了，具体怎么使用，后期有机会给大家详细说说，官方文档有说明呦。


![](https://img-blog.csdn.net/20160620212655186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

      其他还有runspider、bench。前者可以独立运行一个能够自足的爬虫，也就是没有item和pipline类的爬虫。后者，就是一个测试用的东西，安装好scrapy之后，运行scrapy bench，就能知道你的scrapy有没有成功安装好了。







