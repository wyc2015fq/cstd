# 【使用JSOUP实现网络爬虫】修改数据-设置元素的文本内容 - z69183787的专栏 - CSDN博客
2015年08月26日 18:25:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1014
## 问题
你需要修改一个HTML文档中的文本内容
## 方法
可以使用`[Element](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html)`的设置方法：:
``
**[java]**[view plain](http://blog.csdn.net/withiter/article/details/16802117#)[copy](http://blog.csdn.net/withiter/article/details/16802117#)[print](http://blog.csdn.net/withiter/article/details/16802117#)[?](http://blog.csdn.net/withiter/article/details/16802117#)
- Element div = doc.select("div").first(); // <div></div>
- div.text("five > four"); // <div>five > four</div>
- div.prepend("First ");  
- div.append(" Last");  
- // now: <div>First five > four Last</div>
## 说明
文本设置方法与 [HTML setter](http://jsoup.org/cookbook/modifying-data/set-html) 方法一样：
- `[Element.text(String text)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#text%28java.lang.String%29)` 将清除一个元素中的内部HTML内容，然后提供的文本进行代替
- `[Element.prepend(String first)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#prepend%28java.lang.String%29)` 和 
```
[Element.append(String
 last)](http://jsoup.org/apidocs/org/jsoup/nodes/Element.html#append%28java.lang.String%29)
```
 将分别在元素的内部html前后添加文本节点。
对于传入的文本如果含有像 `<`, `>` 等这样的字符，将以文本处理，而非HTML。
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
版权声明：本文为博主原创文章，未经博主允许不得转载。
