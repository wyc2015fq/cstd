# 【使用JSOUP实现网络爬虫】使用DOM方法来遍历一个文档 - z69183787的专栏 - CSDN博客
2015年08月26日 17:19:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2704
## 问题
你有一个HTML文档要从中提取数据，并了解这个HTML文档的结构。
## 方法
将HTML解析成一个`[Document](http://jsoup.org/apidocs/org/jsoup/nodes/Document.html)`之后，就可以使用类似于DOM的方法进行操作。示例代码：
``
**[java]**[view plain](http://blog.csdn.net/withiter/article/details/14166613#)[copy](http://blog.csdn.net/withiter/article/details/14166613#)[print](http://blog.csdn.net/withiter/article/details/14166613#)[?](http://blog.csdn.net/withiter/article/details/14166613#)
- File input = new File("/tmp/input.html");  
- Document doc = Jsoup.parse(input, "UTF-8", "http://example.com/");  
- 
- Element content = doc.getElementById("content");  
- Elements links = content.getElementsByTag("a");  
- for (Element link : links) {  
-   String linkHref = link.attr("href");  
-   String linkText = link.text();  
- }  
## 说明
Elements这个对象提供了一系列类似于DOM的方法来查找元素，抽取并处理其中的数据。具体如下：
### 查找元素
- 
```
[getElementById(String
 id)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#getElementById%28java.lang.String%29)
```
- 
```
[getElementsByTag(String
 tag)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#getElementsByTag%28java.lang.String%29)
```
- 
```
[getElementsByClass(String
 className)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#getElementsByClass%28java.lang.String%29)
```
- 
```
[getElementsByAttribute(String
 key)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#getElementsByAttribute%28java.lang.String%29)
```
 (and related methods)
- Element siblings: `[siblingElements()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#siblingElements%28%29)`, `[firstElementSibling()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#firstElementSibling%28%29)`, `[lastElementSibling()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#lastElementSibling%28%29)`; `[nextElementSibling()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#nextElementSibling%28%29)`, `[previousElementSibling()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#previousElementSibling%28%29)`
- Graph: `[parent()](http://jsoup.org/apidocs/org/jsoup/nodes/Node.html#parent%28%29)`, `[children()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#children%28%29)`, 
```
[child(int
 index)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#child%28int%29)
```
### 元素数据
- 
```
[attr(String
 key)](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#attr%28java.lang.String%29)
```
获取属性
```
[attr(String
 key, String value)](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#attr%28java.lang.String,%20java.lang.String%29)
```
设置属性
- `[attributes()](http://jsoup.org/apidocs/org/jsoup/nodes/TextNode.html#attributes%28%29)`获取所有属性
- `[id()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#id%28%29)`, `[className()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#className%28%29)` and `[classNames()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#classNames%28%29)`
- `[text()](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#text%28%29)`获取文本内容
```
[text(String
 value)](http://jsoup.org/apidocs/org/jsoup/nodes/TextNode.html#text%28java.lang.String%29)
```
 设置文本内容
- `[html()](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#html%28%29)`获取元素内HTML
```
[html(String
 value)](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#html%28java.lang.String%29)
```
设置元素内的HTML内容
- `[outerHtml()](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#outerHtml%28%29)`获取元素外HTML内容
- `[data()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#data%28%29)`获取数据内容（例如：script和style标签)
- `[tag()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#tag%28%29)` and `[tagName()](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#tagName%28%29)`
### 操作HTML和文本
- 
```
[append(String
 html)](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#append%28java.lang.String%29)
```
, 
```
[prepend(String
 html)](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#prepend%28java.lang.String%29)
```
- 
```
[appendText(String
 text)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#appendText%28java.lang.String%29)
```
, 
```
[prependText(String
 text)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#prependText%28java.lang.String%29)
```
- 
```
[appendElement(String
 tagName)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#appendElement%28java.lang.String%29)
```
, 
```
[prependElement(String
 tagName)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#prependElement%28java.lang.String%29)
```
- `[html(String value)](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#html%28java.lang.String%29)`
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
