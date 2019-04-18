# 【使用JSOUP实现网络爬虫】解析一个body片断 - z69183787的专栏 - CSDN博客
2015年08月26日 16:24:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1118
## 问题
假如你有一个HTML片断 (比如. 一个 `div` 包含一对 `p` 标签; 一个不完整的HTML文档) 想对它进行解析。这个HTML片断可以是用户提交的一条评论或在一个CMS页面中编辑body部分。
## 办法
使用
```
[Jsoup.parseBodyFragment(String
 html)](http://jsoup.org/apidocs/org/jsoup/Jsoup.html#parseBodyFragment%28java.lang.String%29)
```
方法.
``
**[java]**[view plain](http://blog.csdn.net/withiter/article/details/13510803#)[copy](http://blog.csdn.net/withiter/article/details/13510803#)[print](http://blog.csdn.net/withiter/article/details/13510803#)[?](http://blog.csdn.net/withiter/article/details/13510803#)
- String html = "<div><p>Lorem ipsum.</p>";  
- Document doc = Jsoup.parseBodyFragment(html);  
- Element body = doc.body();  
## 说明
`parseBodyFragment` 方法创建一个空壳的文档，并插入解析过的HTML到`body`元素中。假如你使用正常的 
```
[Jsoup.parse(String
 html)](http://jsoup.org/apidocs/org/jsoup/Jsoup.html#parse%28java.lang.String%29)
```
 方法，通常你也可以得到相同的结果，但是明确将用户输入作为 body片段处理，以确保用户所提供的任何糟糕的HTML都将被解析成body元素。
`[Document.body()](http://jsoup.org/apidocs/org/jsoup/nodes/Document.html#body%28%29)` 方法能够取得文档body元素的所有子元素，与 `doc.getElementsByTag("body")`相同。
## 保证安全Stay safe
假如你可以让用户输入HTML内容，那么要小心避免跨站脚本攻击。利用基于 `[Whitelist](http://jsoup.org/apidocs/org/jsoup/safety/Whitelist.html)` 的清除器和 
```
[clean(String
 bodyHtml, Whitelist whitelist)](http://jsoup.org/apidocs/org/jsoup/Jsoup.html#clean%28java.lang.String,%20org.jsoup.safety.Whitelist%29)
```
方法来清除用户输入的恶意内容。
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
版权声明：本文为博主原创文章，未经博主允许不得转载。
