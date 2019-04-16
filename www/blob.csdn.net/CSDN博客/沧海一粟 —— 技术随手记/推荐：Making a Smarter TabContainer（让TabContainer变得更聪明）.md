# 推荐：Making a Smarter TabContainer（让TabContainer变得更聪明） - 沧海一粟 —— 技术随手记 - CSDN博客





2011年07月29日 17:29:06[慢游](https://me.csdn.net/eengel)阅读数：758标签：[dojo																[测试																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)
个人分类：[Dojo](https://blog.csdn.net/eengel/article/category/773224)





这儿推荐一篇帖子：[http://kennethfranqueiro.com/2010/06/making-a-smarter-tabcontainer/#comment-692](http://kennethfranqueiro.com/2010/06/making-a-smarter-tabcontainer/#comment-692)




dijit.layout.TabContainer 很好用，就是有一个致命缺点：当关闭一个tab后，无论如何，都会跳转到显示第一个tab。有时候让人很抓狂。这篇帖子提供了一个继承自dijit.layout.TabContainer的自定义对象，并且提供了一个比较能让人接受的tab跳转方案：
- 当关闭的tab不是当前打开的tab时，直接关掉它，并且还维持当前tab的打开状态;
- 当关闭的tab正是当前打开的tab时，关掉它后，跳转到显示之前打开的那个tab。


里面针对Dojo 1.4的例子在1.5下也能测试通过。




其实这个问题已经被提交到了dojotoolkit组（http://bugs.dojotoolkit.org/ticket/9776）。但目前Dojo 1.5下貌似还没有被修正。](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=dojo&t=blog)




