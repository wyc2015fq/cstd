# [Web开发] 微软的RSS协议扩展 - FeedSync 介绍 (1) - weixin_33985507的博客 - CSDN博客
2008年02月15日 06:56:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
FeedSync 是微软对 RSS 协议的扩展，前不久刚刚发布v1.0版本。以前的v0.9版本名字叫做 SSE(Simple Sharing Extensions)。   官方网站是  [http://FeedSync.org](http://FeedSync.org)， 协议标准可以在 [http://dev.live.com/feedsync/spec/v1.htm](http://dev.live.com/feedsync/spec/v1.htm)  看到。 
该协议由目前微软首席软件架构师Ray Ozzie 设计，目的在于利用RSS/Atom 协议在互联网上共享同步数据。 现在有的RSS/Atom 协议都是只读的， 也就是说，用户只能通过RSS/Atom下载数据，不能上传数据。 FeedSync 的出现就是为了解决这个问题的。 从下面的图解释了这个框架。
![](http://blog.51cto.com/attachment/201002/201002011265030903538.png)
可以看到， FeedSync 是个双向的同步协议。 FeedSync除了具有用户上传数据功能之外，更重要是解决数据差异冲突问题。 比如拿在线文档做例子，文档存在服务器上， 我在家里电脑，公司电脑，笔记本上都可能随时对文档进行修改， 这个时候可能存在3个完全不同版本的文档， FeedSync可以解决这个冲突问题。 
相信随着Web 应用程序的丰富， 会有越来越多地方都需要解决数据同步问题， FeedSync将是个很好的解决方案。 FeedSync 以Creative Commons license版权发布，鼓励大家开发协议相关工具。
[](http://blog.csdn.net/WinGeek/archive/2008/02/15/2096346.aspx)
