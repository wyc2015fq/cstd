# 谈谈如何抓取ajax动态网站 - zhusongziye的博客 - CSDN博客





2018年06月10日 17:13:26[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：2246








什么是ajax呢，简单来说，就是加载一个网页完毕之后，有些信息你你还是看不到，需要你点击某个按钮才能看到数据，或者有些网页是有很多页数据的，而你在点击下一页的时候，网页的url地址没有变化，但是内容变了，这些都可以说是ajax。如果还听不懂，我给你看看百度百科的解释吧，下面就是。




> 
Ajax 即“Asynchronous Javascript And XML”（异步 JavaScript 和 XML），是指一种创建交互式网页应用的网页开发技术。

Ajax = 异步 JavaScript 和 XML（标准通用标记语言的子集）。

Ajax 是一种用于创建快速动态网页的技术。

Ajax 是一种在无需重新加载整个网页的情况下，能够更新部分网页的技术。 [

通过在后台与服务器进行少量数据交换，Ajax 可以使网页实现异步更新。这意味着可以在不重新加载整个网页的情况下，对网页的某部分进行更新。

传统的网页（不使用 Ajax）如果需要更新内容，必须重载整个网页页面。





下面说下例子，我抓取过的ajax网页最难的就是网易云音乐的评论，感兴趣的可以看看[利用python爬取网易云音乐，并把数据存入mysql](http://mp.weixin.qq.com/s?__biz=MzU0NzY0NzQyNw==&mid=2247483673&idx=1&sn=6c89eb9474d2076afb6405a9fd972e4f&chksm=fb4a7bb5cc3df2a3ca3235ebe8bddf635839e5fd5a1b998b08dae521d449ee4388ae6ed7bbda&scene=21#wechat_redirect)

这里的评论就是ajax加载的，其他的那个抓今日头条妹子图片的也算是ajax加载的，只不过我把它简单化了。还有很多，就不说了，说下我今天要说的ajax网站吧！




http://www.kfc.com.cn/kfccda/storelist/index.aspx

这个是肯德基的门面信息

![](https://img-blog.csdn.net/20180610171257714?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




这里有很多页数据，每一页的数据都是ajax加载的。如果你直接用python请求上面那个url的话，估计什么数据都拿不到，不信的话可以试试哈。这时候，我们照常打开开发者工具。先把所有请求清楚，把持续日志打上勾，然后点击下一页，你会看到

![](https://img-blog.csdn.net/20180610171309467?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面那个请求就是ajax请求的网页，里面就会有我们需要的数据，我们看看是什么样的请求

![](https://img-blog.csdn.net/20180610171319433?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




是个post请求，请求成功状态码为200，请求url上面也有了，下面的from data就是我们需要post的数据，很容易就可以猜到pageIndex就是页数，所以我们可以改变这个值来进行翻页。




这个网页就分析完了，这样就是解决ajax动态网页了，是不是觉得很简单，其实不是的，只是这个网页比较简单的，因为表单(from data)的数据并没有进行加密，如果进行加密的话估计你的找js文件看看参数是怎样加密的了，这就是我之前写的网易云音乐评论的爬取。看这些混淆的js寻找加密方法的话有时会让你很头痛，所以经常有人会选择用selenium这些来进行爬取，但是用这些会使爬虫的性能降低，所以这个方法在工作里是不允许的。所以必须学会怎样应对这些ajax。




贴下代码

```
import requests
page = 1
while True:
   url = 'http://www.kfc.com.cn/kfccda/ashx/GetStoreList.ashx?op=cname'
   data = {
           'cname': '广州',
           'pid': '',
           'pageIndex': page,
           'pageSize': '10'
   }
   response = requests.post(url, data=data)
   print(response.json())
   if response.json().get('Table1', ''):
       page += 1
   else:
       break
```




可以看到去掉from data，不用十行代码就可以把数据都爬下来了，所以说这个网站适合练手，大家可以去试试。




写在最后

下篇文章我会写下复杂点的ajax请求，这个网站

http://drugs.dxy.cn/



