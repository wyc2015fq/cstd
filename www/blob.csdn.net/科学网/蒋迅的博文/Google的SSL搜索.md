# 科学网—Google的SSL搜索 - 蒋迅的博文




# Google的SSL搜索                           

已有 4248 次阅读2010-5-27 08:44|个人分类:[科技](http://blog.sciencenet.cn/home.php?mod=space&uid=420554&do=blog&classid=136794&view=me)|系统分类:[海外观察](http://blog.sciencenet.cn/home.php?mod=space&do=blog&view=all&uid=420554&catid=2)|关键词:Google,谷歌,SSL,科学网,博客|[google](misc.php?mod=tag&id=236), [博客](misc.php?mod=tag&id=417), [谷歌](misc.php?mod=tag&id=1495), [科学网](misc.php?mod=tag&id=1788), [SSL](misc.php?mod=tag&id=15769)



作者：蒋迅

一般地说，一个网页前的“www.”是可有可无的。比如 Google 的主页可以是 [http://www.google.com/](http://www.google.com/)，也可以是 [http://google.com/](http://google.com/)，其实都是一个网址。最初人们喜欢用“www.”，但后来发现它是多余的，於是越来越多的人就不用它了。

![](http://image.sciencenet.cn/album//photo/upload/bigimg/20105278322563.gif)
[https://www.google.com/](https://www.google.com/)
最近 Google [推出](http://tech.slashdot.org/story/10/05/25/1552256/Google-Rolls-Out-Encrypted-Web-Search-Option)了一个加密的搜索服务[https://www.google.com/](https://www.google.com/)，我想这对中国的网民们应该是一个[好消息](http://search.slashdot.org/story/10/05/22/1218242/Google-Offers-Encrypted-Web-Search-Option)。但是奇怪的是，你必须加上“www.”，否则网页会自动跳转到普通的 Google [搜索网页](http://www.google.com/)上去。Google 就不能多买一个 Certificate 吗？目前这项新服务还是Beta试用，注意左上角没有图像、视屏、地图、新闻等搜索。如果你想用这项服务，建议你先阅读阿莫在[Google SSL搜索在FireFox和IE8中的解决方案](http://www.williamlong.info/%3E%E6%9C%88%E5%85%89%E5%8D%9A%E5%AE%A2%3C/a%3E%E5%8F%91%E8%A1%A8%E7%9A%84%E2%80%9C%3Ca%20target=)”和后面的评论。

顺便提一句，在科学网博客上有没有“www.”也有区别。如果你能的是火狐并且你是从[http://www.sciencenet.cn/](http://www.sciencenet.cn/)登陆的，那么当你打开[http://www.sciencenet.cn/](http://www.sciencenet.cn/)时，你会发现你并没有登陆。下面是我的博客主页在两种情况下的显示截图：

![](http://image.sciencenet.cn/album//photo/upload/bigimg/201052783050563.gif)
[http://www.sciencenet.cn/u/jiangxun/](http://www.sciencenet.cn/u/jiangxun/)
![](http://image.sciencenet.cn/album//photo/upload/bigimg/20105278305163.gif)
[http://sciencenet.cn/u/jiangxun/](http://sciencenet.cn/u/jiangxun/)
很显然，造成这个不同的原因是科学网原程序在读取 Cookie 时的错误。


转载本文请联系原作者获取授权，同时请注明本文来自蒋迅科学网博客。
链接地址：[http://blog.sciencenet.cn/blog-420554-329132.html](http://blog.sciencenet.cn/blog-420554-329132.html)

上一篇：[【数学都知道】2010年5月25日](blog-420554-328399.html)
下一篇：[美国大学入学考试 (SAT) 数学部分有多难](blog-420554-330287.html)





