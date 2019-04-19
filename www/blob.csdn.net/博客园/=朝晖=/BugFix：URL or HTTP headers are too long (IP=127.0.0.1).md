# BugFix：URL or HTTP headers are too long (IP=127.0.0.1) - =朝晖= - 博客园
# [BugFix：URL or HTTP headers are too long (IP=127.0.0.1)](https://www.cnblogs.com/dhcn/p/7116145.html)
错误提示：
```
URL or HTTP headers are too long (IP=127.0.0.1)  
  
com.caucho.server.dispatch.BadRequestException: URL or HTTP headers are  
too long (IP=127.0.0.1)  
Resin/4.0.27 Server: 'app-0'
```
官方解释：http://bugs.caucho.com/view.php?id=4999
第三方解释：http://blog.sina.com.cn/s/blog_8a8fd38501014v39.html
总结：这个问题的直接原因是这次访问的URL过长，实际修正可能和浏览器环境，服务器版本，应用服务器版本等因素有关。这个错误在我本地[测试](http://lib.csdn.net/base/softwaretest)环境下出现，出现原因是官方解释的第二次请求，在生产服务器上没有出现。
比较可能的原因是Cookie太长，除了本站Cookie以外，用到的某些第三方的东西会毫无节制地加Cookie做用户行为跟踪，这个问题一般出现在JEE容器上，所以建议打开浏览器当前域Cookie，删掉一些就Ok了。

