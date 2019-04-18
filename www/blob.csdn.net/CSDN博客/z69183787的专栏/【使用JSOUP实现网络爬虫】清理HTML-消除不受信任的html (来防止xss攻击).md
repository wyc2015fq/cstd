# 【使用JSOUP实现网络爬虫】清理HTML-消除不受信任的html (来防止xss攻击) - z69183787的专栏 - CSDN博客
2015年08月26日 18:26:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1442
## 问题
在做网站的时候，经常会提供用户评论的功能。有些不怀好意的用户，会搞一些脚本到评论内容中，而这些脚本可能会破坏整个页面的行为，更严重的是获取一些机要信息，此时需要清理该HTML，以避免跨站脚本[cross-site scripting](http://en.wikipedia.org/wiki/Cross-site_scripting)攻击（XSS）。
## 方法
使用jsoup HTML `[Cleaner](http://jsoup.org/apidocs/org/jsoup/safety/Cleaner.html)` 方法进行清除，但需要指定一个可配置的 `[Whitelist](http://jsoup.org/apidocs/org/jsoup/safety/Whitelist.html)`。
``
**[java]**[view plain](http://blog.csdn.net/withiter/article/details/16802241#)[copy](http://blog.csdn.net/withiter/article/details/16802241#)[print](http://blog.csdn.net/withiter/article/details/16802241#)[?](http://blog.csdn.net/withiter/article/details/16802241#)
- String unsafe =   
- "<p><a href='http://example.com/' onclick='stealCookies()'>Link</a></p>";  
- String safe = Jsoup.clean(unsafe, Whitelist.basic());  
- // now: <p><a href="http://example.com/" rel="nofollow">Link</a></p>
## 说明
XSS又叫CSS (Cross Site Script) ，跨站脚本攻击。它指的是恶意攻击者往Web页面里插入恶意html代码，当用户浏览该页之时，嵌入其中Web里面的html代码会被执行，从而达到恶意攻击用户的特殊目的。XSS属于被动式的攻击，因为其被动且不好利用，所以许多人常忽略其危害性。所以我们经常只让用户输入纯文本的内容，但这样用户体验就比较差了。
一个更好的解决方法就是使用一个富文本编辑器WYSIWYG如[CKEditor](http://ckeditor.com/) 和 [TinyMCE](http://tinymce.moxiecode.com/)。这些可以输出HTML并能够让用户可视化编辑。虽然他们可以在客户端进行校验，但是这样还不够安全，需要在服务器端进行校验并清除有害的HTML代码，这样才能确保输入到你网站的HTML是安全的。否则，攻击者能够绕过客户端的Javascript验证，并注入不安全的HMTL直接进入您的网站。
jsoup的whitelist清理器能够在服务器端对用户输入的HTML进行过滤，只输出一些安全的标签和属性。
jsoup提供了一系列的`[Whitelist](http://jsoup.org/apidocs/org/jsoup/safety/Whitelist.html)`基本配置，能够满足大多数要求；但如有必要，也可以进行修改，不过要小心。
这个cleaner非常好用不仅可以避免XSS攻击，还可以限制用户可以输入的标签范围。
## 参见
- 参阅[XSS cheat sheet](http://ha.ckers.org/xss.html) ，有一个例子可以了解为什么不能使用正则表达式，而采用安全的whitelist parser-based清理器才是正确的选择。
- 参阅`[Cleaner](http://jsoup.org/apidocs/org/jsoup/safety/Cleaner.html)` ，了解如何返回一个 `[Document](http://jsoup.org/apidocs/org/jsoup/nodes/Document.html)` 对象，而不是字符串
- 参阅`[Whitelist](http://jsoup.org/apidocs/org/jsoup/safety/Whitelist.html)`，了解如何创建一个自定义的whitelist
- [nofollow](http://en.wikipedia.org/wiki/Nofollow) 链接属性了解
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
版权声明：本文为博主原创文章，未经博主允许不得转载。
