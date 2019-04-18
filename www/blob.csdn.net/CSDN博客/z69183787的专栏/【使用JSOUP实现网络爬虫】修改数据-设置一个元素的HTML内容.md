# 【使用JSOUP实现网络爬虫】修改数据-设置一个元素的HTML内容 - z69183787的专栏 - CSDN博客
2015年08月26日 18:00:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1496

## 问题
你需要一个元素中的HTML内容
## 方法
可以使用`[Element](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html)`中的HTML设置方法具体如下：
``
**[java]**[view plain](http://blog.csdn.net/withiter/article/details/16336939#)[copy](http://blog.csdn.net/withiter/article/details/16336939#)[print](http://blog.csdn.net/withiter/article/details/16336939#)[?](http://blog.csdn.net/withiter/article/details/16336939#)
- Element div = doc.select("div").first(); // <div></div>
- div.html("<p>lorem ipsum</p>"); // <div><p>lorem ipsum</p></div>
- div.prepend("<p>First</p>");//在div前添加html内容
- div.append("<p>Last</p>");//在div之后添加html内容
- // 添完后的结果: <div><p>First</p><p>lorem ipsum</p><p>Last</p></div>
- 
- Element span = doc.select("span").first(); // <span>One</span>
- span.wrap("<li><a href='http://example.com/'></a></li>");  
- // 添完后的结果: <li><a href="http://example.com"><span>One</span></a></li>
## 说明
- `[Element.html(String html)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#html%28java.lang.String%29)` 这个方法将先清除元素中的HTML内容，然后用传入的HTML代替。
- `[Element.prepend(String first)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#prepend%28java.lang.String%29)` 和 
```
[Element.append(String
 last)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#append%28java.lang.String%29)
```
 方法用于在分别在元素内部HTML的前面和后面添加HTML内容
- `[Element.wrap(String around)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#wrap%28java.lang.String%29)` 对元素包裹一个外部HTML内容。
## 参见
可以查看API参考文档中 
```
[Element.prependElement(String
 tag)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#prependElement%28java.lang.String%29)
```
和
```
[Element.appendElement(String
 tag)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#appendElement%28java.lang.String%29)
```
 方法来创建新的元素并作为文档的子元素插入其中。
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
版权声明：本文为博主原创文章，未经博主允许不得转载。
