# Spring MVC “404 Not Found”错误的解决 - =朝晖= - 博客园
# [Spring MVC “404 Not Found”错误的解决](https://www.cnblogs.com/dhcn/p/7105423.html)
     一般这个问题发生的原因不会出在Web.xml配置文件的Servlet URL映射部分，因为这个URL映射做JavaWeb的人经常配置不会发生错误，而且此处的这个映射自由度很大，可以有目录字符串，也可以没有，只有最后的文件名匹配方式。所以问题一般出在控制器Controller的映射配置定义部分，因为如果DispatchServlet分发器在其配置文件中找不到请求URL所对应的控制器Controller，也会返回404 Not Found错误。而控制器此处的URL定义无论是BeanNameUrlHandlerMapping配置中的name属性还是SimpleUrlHandlerMapping配置中key属性，这些定义URL的位置，一定要出现URL的服务器项目绝对路径，也就是说不能省略成像某些不严谨或过期的书籍或者文档中描述的那样只要出现URL的最后部分就行了，实际上不是这样，在此处的定义必须写服务器项目绝对路径，也就是说是URL中项目部署名称后面的那部分，有的Web项目直接部署在域名根目录下的，则是域名后面的那部分，说一个简单的规则，说白了就是在Web.xml中定义Servlet映射的URL中的那种模式
参考资料：[http://blog.csdn.net/wangyaoheng/article/details/2465834](http://blog.csdn.net/wangyaoheng/article/details/2465834)

