# 爬虫、网页分析解析辅助工具 Xpath-helper - z69183787的专栏 - CSDN博客
2016年11月17日 19:36:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：879
搬运自本人[博客](http://www.07net01.com/tags-%E5%8D%9A%E5%AE%A2-0.html)：http://www.xgezhang.com/xpath_helper.html
每一个写爬虫、或者是做[网页](http://www.07net01.com/tags-%E7%BD%91%E9%A1%B5-0.html)分析的人，相信都会因为在定位、获取xpath路径上花费大量的时间，甚至有时候当爬虫框架成熟之后，基本上主要的时间都花费在了页面的解析上。在没有这些辅助工具的日子里，我们只能通过搜索html[源代码](http://www.07net01.com/tags-%E6%BA%90%E4%BB%A3%E7%A0%81-0.html)，定位一些id去找到对应的位置，非常的麻烦，而且经常出错。这里介绍一个[chrome](http://www.07net01.com/tags-chrome-0.html)的xpath辅助插件给大家。
先给到下载链接：xpath-helper
安装好之后，我们重新打开[浏览器](http://www.07net01.com/tags-%E6%B5%8F%E8%A7%88%E5%99%A8-0.html)，按ctrl+shift+x就能调出xpath-helper框了。界面如下：
![爬虫、网页分析解析辅助工具 Xpath-helper-xge技术博客](http://img4.07net01.com/upload/images/2015/08/16/1660944161803151.png)
如果我们要查找某一个、或者某一块[元素](http://www.07net01.com/tags-%E5%85%83%E7%B4%A0-0.html)的xpath路径，可以按住shift，并移动到这一块中，上面的框就会显示这个元素的xpath路径，右边则会显示解析出的文本内容，并且我们可以自己改动xpath路径，[程序](http://www.07net01.com/tags-%E7%A8%8B%E5%BA%8F-0.html)也会自动的显示对应的位置，可以很方便的帮助我们判断我们的xpath语句是否书写正确。
虽然这个小插件使用非常方便，但它也不是万能的，有两个问题：
  1.XPath Helper 自动提取的 XPath 都是从根路径开始的，这几乎必然导致 XPath 过长，不利于维护；
  2.当提取循环的列表数据时，XPath Helper 是使用的下标来分别提取的列表中的每一条数据，这样并不适合程序批量处理，还是需要人为修改一些类似于*标记等。
不过，合理的使用Xpath，还是能帮我们省下很多时间的！
版权声明：本文为博主原创[文章](http://www.07net01.com/2015/07/860262.html)，未经博主允许不得转载。
