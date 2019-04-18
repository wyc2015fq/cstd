# 【使用JSOUP实现网络爬虫】入门：解析和遍历一个HTML文档 - z69183787的专栏 - CSDN博客
2015年08月26日 15:53:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：912
jsoup 是一款Java 的HTML解析器，可直接解析某个URL地址、HTML文本内容。它提供了一套非常省力的API，可通过DOM，CSS以及类似于jQuery的操作方法来取出和操作数据。
### jsoup的主要功能如下：
1. 从一个URL，文件或字符串中解析HTML；
2. 使用DOM或CSS选择器来查找、取出数据；
3. 可操作HTML元素、属性、[文本](http://baike.baidu.com/view/300107.htm)；
如何解析一个HTML文档：
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/13510357#)[copy](http://blog.csdn.net/withiter/article/details/13510357#)[print](http://blog.csdn.net/withiter/article/details/13510357#)[?](http://blog.csdn.net/withiter/article/details/13510357#)
- String html = "<html><head><title>First parse</title></head>"
-   + "<body><p>Parsed HTML into a doc.</p></body></html>";  
- Document doc = Jsoup.parse(html);  
其解析器能够尽最大可能从你提供的HTML文档来创见一个干净的解析结果，无论HTML的格式是否完整。比如它可以处理：
- 没有关闭的标签 (比如： `<p>Lorem <p>Ipsum` parses to `<p>Lorem</p> <p>Ipsum</p>`)
- 隐式标签 (比如. 它可以自动将 `<td>Table data</td>`包装成`<table><tr><td>?`)
- 创建可靠的文档结构（html标签包含head 和 body，在head只出现恰当的元素）
## 一个文档的对象模型
- 文档由多个Elements和TextNodes组成 (以及其它辅助nodes：详细可查看：nodes package tree).
- 其继承结构如下：Document继承Element继承Node. TextNode继承 Node.
- 一个Element包含一个子节点集合，并拥有一个父Element。他们还提供了一个唯一的子元素过滤列表。
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
