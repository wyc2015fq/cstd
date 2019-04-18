# 【使用JSOUP实现网络爬虫】从元素抽取属性，文本和HTML - z69183787的专栏 - CSDN博客
2015年08月26日 17:31:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1555
## 问题
在解析获得一个Document实例对象，并查找到一些元素之后，你希望取得在这些元素中的数据。
## 方法
- 要取得一个属性的值，可以使用`[Node.attr(String key)](http://jsoup.org/apidocs/org/jsoup/nodes/Node.html#attr%28java.lang.String%29)` 方法
- 对于一个元素中的文本，可以使用`[Element.text()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#text%28%29)`方法
- 对于要取得元素或属性中的HTML内容，可以使用`[Element.html()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#html%28%29)`,
 或 `[Node.outerHtml()](http://jsoup.org/apidocs/org/jsoup/nodes/Node.html#outerHtml%28%29)`方法
示例：
``
**[java]**[view plain](http://blog.csdn.net/withiter/article/details/14450003#)[copy](http://blog.csdn.net/withiter/article/details/14450003#)[print](http://blog.csdn.net/withiter/article/details/14450003#)[?](http://blog.csdn.net/withiter/article/details/14450003#)
- String html = "<p>An <a href='http://example.com/'><b>example</b></a> link.</p>";  
- Document doc = Jsoup.parse(html);//解析HTML字符串返回一个Document实现
- Element link = doc.select("a").first();//查找第一个a元素
- 
- String text = doc.body().text(); // "An example link"//取得字符串中的文本
- String linkHref = link.attr("href"); // "http://example.com/"//取得链接地址
- String linkText = link.text(); // "example""//取得链接地址中的文本
- 
- String linkOuterH = link.outerHtml();   
- // "<a href="http://example.com"><b>example</b></a>"
- String linkInnerH = link.html(); // "<b>example</b>"//取得链接内的html内容
## 说明
上述方法是元素数据访问的核心办法。此外还其它一些方法可以使用：
- `[Element.id()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#id%28%29)`
- `[Element.tagName()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#tagName%28%29)`
- `[Element.className()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#className%28%29)` and 
```
[Element.hasClass(String
 className)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#hasClass%28java.lang.String%29)
```
这些访问器方法都有相应的setter方法来更改数据.
## 参见
- `[Element](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html)`和`[Elements](http://jsoup.org/apidocs/org/jsoup/select/Elements.html)`集合类的参考文档
- [URLs处理](http://www.open-open.com/jsoup/working-with-urls.htm)
- [使用CSS选择器语法来查找元素](http://www.open-open.com/jsoup/selector-syntax.htm)
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
