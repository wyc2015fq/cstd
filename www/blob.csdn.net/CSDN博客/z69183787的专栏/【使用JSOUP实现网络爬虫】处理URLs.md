# 【使用JSOUP实现网络爬虫】处理URLs - z69183787的专栏 - CSDN博客
2015年08月26日 17:39:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：822
个人分类：[爬虫-Jsoup](https://blog.csdn.net/z69183787/article/category/5766861)
## 问题
你有一个包含相对URLs路径的HTML文档，需要将这些相对路径转换成绝对路径的URLs。
## 方法
- 在你解析文档时确保有指定`base URI`，然后
- 使用 `abs:` 属性前缀来取得包含`base URI`的绝对路径。代码如下： 
``
**[java]**[view plain](http://blog.csdn.net/withiter/article/details/14521391#)[copy](http://blog.csdn.net/withiter/article/details/14521391#)[print](http://blog.csdn.net/withiter/article/details/14521391#)[?](http://blog.csdn.net/withiter/article/details/14521391#)
- Document doc = Jsoup.connect("http://www.open-open.com").get();  
- 
- Element link = doc.select("a").first();  
- String relHref = link.attr("href"); // == "/"
- String absHref = link.attr("abs:href"); // "http://www.open-open.com/"
## 说明
在HTML元素中，URLs经常写成相对于文档位置的相对路径： `<a href="/download">...</a>`. 当你使用 
```
[Node.attr(String
 key)](http://jsoup.org/apidocs/org/jsoup/nodes/Node.html#attr%28java.lang.String%29)
```
 方法来取得a元素的href属性时，它将直接返回在HTML源码中指定定的值。
假如你需要取得一个绝对路径，需要在属性名前加 `abs:` 前缀。这样就可以返回包含根路径的URL地址`attr("abs:href")`
因此，在解析HTML文档时，定义base URI非常重要。
如果你不想使用`abs:` 前缀，还有一个方法能够实现同样的功能 
```
[Node.absUrl(String
 key)](http://jsoup.org/apidocs/org/jsoup/nodes/Node.html#absUrl%28java.lang.String%29)
```
。
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
