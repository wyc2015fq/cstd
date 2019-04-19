# jtds驱动更新对一个老问题的解决 - =朝晖= - 博客园
# [jtds驱动更新对一个老问题的解决](https://www.cnblogs.com/dhcn/p/7116174.html)
07年年末的一篇blog：
    以前网站做初期开发时，有一个问题:[hibernate](http://lib.csdn.net/base/javaee)下text大字符串读取时出这个异常:JDBCExceptionReporter - The amount of data read from the stream is not = length.resin和tomcat下都是这个问题，最后只好把网站部署到重量级的Weblogic上，才避免了这个问题，但是Weblogic毕竟属于企业级收费软件，感觉比较笨重，对于我们这个网站这种轻量级的开发，确实不好，最近这几天想起来想找个办法解决这个问题，上网看了看，好像已经搜不出来了，那天看一个解决hibernate+[Oracle](http://lib.csdn.net/base/oracle) clob的问题，对方更新驱动，就好了，我试着把jtds驱动从1.02更新到1.22，这个老问题居然也解决了，看来造轮子的人果然很灵敏。

