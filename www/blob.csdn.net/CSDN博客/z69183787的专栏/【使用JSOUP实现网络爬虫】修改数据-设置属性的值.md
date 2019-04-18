# 【使用JSOUP实现网络爬虫】修改数据-设置属性的值 - z69183787的专栏 - CSDN博客
2015年08月26日 17:44:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1423
## 问题
在你解析一个Document之后可能想修改其中的某些属性值，然后再保存到磁盘或都输出到前台页面。
## 方法
可以使用属性设置方法 
```
[Element.attr(String
 key, String value)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#attr%28java.lang.String,%20java.lang.String%29)
```
, 和 
```
[Elements.attr(String
 key, String value)](http://jsoup.org/apidocs/org/jsoup/select/Elements.html#attr%28java.lang.String,%20java.lang.String%29)
```
.
假如你需要修改一个元素的 `class` 属性，可以使用 
```
[Element.addClass(String
 className)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#addClass%28java.lang.String%29)
```
 和 
```
[Element.removeClass(String
 className)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#removeClass%28java.lang.String%29)
```
 方法。
`[Elements](http://jsoup.org/apidocs/org/jsoup/select/Elements.html)` 提供了批量操作元素属性和class的方法，比如：要为div中的每一个a元素都添加一个 `rel="nofollow"` 可以使用如下方法：
``
**[javascript]**[view plain](http://blog.csdn.net/withiter/article/details/16117727#)[copy](http://blog.csdn.net/withiter/article/details/16117727#)[print](http://blog.csdn.net/withiter/article/details/16117727#)[?](http://blog.csdn.net/withiter/article/details/16117727#)
- doc.select("div.comments a").attr("rel", "nofollow");  
## 说明
与`[Element](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html)`中的其它方法一样，`attr` 方法也是返回当 `[Element](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html)` (或在使用选择器是返回 `[Elements](http://jsoup.org/apidocs/org/jsoup/select/Elements.html)` 集合)。这样能够很方便使用方法连用的书写方式。比如：
``
**[javascript]**[view plain](http://blog.csdn.net/withiter/article/details/16117727#)[copy](http://blog.csdn.net/withiter/article/details/16117727#)[print](http://blog.csdn.net/withiter/article/details/16117727#)[?](http://blog.csdn.net/withiter/article/details/16117727#)
- doc.select("div.masthead").attr("title", "jsoup").addClass("round-box");  
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
