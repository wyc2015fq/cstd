# 【使用JSOUP实现网络爬虫】解析一个HTML字符串 - z69183787的专栏 - CSDN博客
2015年08月26日 16:02:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1325
个人分类：[爬虫-Jsoup](https://blog.csdn.net/z69183787/article/category/5766861)
## 存在问题
来自用户输入，一个文件或一个网站的HTML字符串，你可能需要对它进行解析并取其内容，或校验其格式是否完整，或想修改它。怎么办？jsonu能够帮你轻松解决这些问题
## 解决方法
使用静态`[Jsoup.parse(String html)](http://jsoup.org/apidocs/org/jsoup/Jsoup.html#parse%28java.lang.String%29)` 方法或 
```
[Jsoup.parse(String
 html, String baseUri)](http://jsoup.org/apidocs/org/jsoup/Jsoup.html#parse%28java.lang.String,%20java.lang.String%29)
```
示例代码：
``
**[html]**[view plain](http://blog.csdn.net/withiter/article/details/13510743#)[copy](http://blog.csdn.net/withiter/article/details/13510743#)[print](http://blog.csdn.net/withiter/article/details/13510743#)[?](http://blog.csdn.net/withiter/article/details/13510743#)
- String html = "<html><head><title>First parse</title></head>"
-   + "<body><p>Parsed HTML into a doc.</p></body></html>";  
- Document doc = Jsoup.parse(html);  
## 描述
```
[parse(String html, String
 baseUri)](http://jsoup.org/apidocs/org/jsoup/parser/Parser.html#parse%28java.lang.String,%20java.lang.String%29)
```
 这方法能够将输入的HTML解析为一个新的文档 (Document），参数 baseUri 是用来将相对 URL 转成绝对URL，并指定从哪个网站获取文档。如这个方法不适用，你可以使用 
```
[parse(String
 html)](http://jsoup.org/apidocs/org/jsoup/Jsoup.html#parse%28java.lang.String%29)
```
 方法来解析成HTML字符串如上面的示例。.
只要解析的不是空字符串，就能返回一个结构合理的文档，其中包含(至少) 一个head和一个body元素。
一旦拥有了一个Document，你就可以使用Document中适当的方法或它父类 `[Element](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html)`和`[Node](http://jsoup.org/apidocs/org/jsoup/nodes/Node.html)`中的方法来取得相关数据。
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
版权声明：本文为博主原创文章，未经博主允许不得转载。
