# 绕过JS写爬虫 - lyx的专栏 - CSDN博客





2018年02月03日 20:38:06[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：2463








      最近要把很多数据抓下来先存起来，现有历史数据再说。其中，东方财富网有许多数据，其中有一个是机构调研的数据。

      http://data.eastmoney.com/jgdy/tj.html

      我们希望抓取的是js生成的表格。

      这种带有js的网站抓取其实不是那么简单的，基本分为那么几种方法，一种是观察页面，有的会有json数据，有的有js代码可以解析目标的url；一种是使用渲染工具；还有一种就是用工具来点击相关button，来抓取url，进而进一步分析。

      今天我们使用的是第三种。

![](https://img-blog.csdn.net/20180203202702137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


      我们希望爬取的是表格中的数据，但是如果我们仔细看一下html代码，会发现，这其实是js生成的，下面这张图是源代码的截图。

![](https://img-blog.csdn.net/20180203202903933?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

      这就很尴尬了，怎么办呢？我们自己自己解析url数据访问你的api好了。

      我们用浏览器自带的解析功能来试一下。首先，我们打开network，chrome内核的好像都这么叫。

![](https://img-blog.csdn.net/20180203203145741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)    然后我们就点击第二页、第三页不断的来观察究竟js代码访问了什么后台的url。事实是这样的

http://data.eastmoney.com/DataCenter_V3/jgdy/gsjsdy.ashx?pagesize=50&page=1&js=var YnQNqDYj&param=&sortRule=-1&sortType=0&rt=50585869
http://data.eastmoney.com/DataCenter_V3/jgdy/gsjsdy.ashx?pagesize=50&page=2&js=var strLvpoP&param=&sortRule=-1&sortType=0&rt=50585871
http://data.eastmoney.com/DataCenter_V3/jgdy/gsjsdy.ashx?pagesize=50&page=3&js=var ggPyhcCU&param=&sortRule=-1&sortType=0&rt=50585871
http://data.eastmoney.com/DataCenter_V3/jgdy/gsjsdy.ashx?pagesize=50&page=4&js=var SjIkKWnA&param=&sortRule=-1&sortType=0&rt=50585872

      我们就可以找到规律了，本质上就是改变paee后面的数字了。至于最后面一个数字，目前猜测是一个计数标记，所有的api中设置一致就可以了。

      接下来我们就可以用urllib来获得api背后的json内容了，比如是这样的：

![](https://img-blog.csdn.net/20180203203710039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXRseXg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




