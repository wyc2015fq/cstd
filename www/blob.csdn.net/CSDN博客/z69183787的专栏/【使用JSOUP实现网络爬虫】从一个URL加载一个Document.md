# 【使用JSOUP实现网络爬虫】从一个URL加载一个Document - z69183787的专栏 - CSDN博客
2015年08月26日 17:18:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：809
## 存在问题
你需要从一个网站获取和解析一个HTML文档，并查找其中的相关数据。你可以使用下面解决方法：
## 解决方法
使用 `[Jsoup.connect(String url)](http://jsoup.org/apidocs/org/jsoup/Jsoup.html#connect%28java.lang.String%29)`方法:
``
**[java]**[view plain](http://blog.csdn.net/withiter/article/details/13627477#)[copy](http://blog.csdn.net/withiter/article/details/13627477#)[print](http://blog.csdn.net/withiter/article/details/13627477#)[?](http://blog.csdn.net/withiter/article/details/13627477#)
- Document doc = Jsoup.connect("http://example.com/").get();  
- String title = doc.title();  
## 说明
`[connect(String url)](http://jsoup.org/apidocs/org/jsoup/Jsoup.html#connect%28java.lang.String%29)` 方法创建一个新的 `[Connection](http://jsoup.org/apidocs/org/jsoup/Connection.html)`,
 和 `[get()](http://jsoup.org/apidocs/org/jsoup/helper/HttpConnection.html#get%28%29)` 取得和解析一个HTML文件。如果从该URL获取HTML时发生错误，便会抛出 IOException，应适当处理。
`[Connection](http://jsoup.org/apidocs/org/jsoup/Connection.html)` 接口还提供一个方法链来解决特殊请求，具体如下：
``
**[java]**[view plain](http://blog.csdn.net/withiter/article/details/13627477#)[copy](http://blog.csdn.net/withiter/article/details/13627477#)[print](http://blog.csdn.net/withiter/article/details/13627477#)[?](http://blog.csdn.net/withiter/article/details/13627477#)
- Document doc = Jsoup.connect("http://example.com")  
-   .data("query", "Java")  
-   .userAgent("Mozilla")  
-   .cookie("auth", "token")  
-   .timeout(3000)  
-   .post();  
这个方法只支持Web URLs (`http`和`https` 协议); 假如你需要从一个文件加载，可以使用 
```
[parse(File
 in, String charsetName)](http://jsoup.org/apidocs/org/jsoup/Jsoup.html#parse%28java.io.File,%20java.lang.String%29)
```
 代替。
阅读更多JSOUP相关文章，请看专栏：[《使用JSOUP实现网络爬虫》](http://blog.csdn.net/column/details/jsoup.html)
