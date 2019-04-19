# Spring MVC入门（一） - 左直拳的马桶_日用桶 - CSDN博客
2017年09月13日 19:32:00[左直拳](https://me.csdn.net/leftfist)阅读数：300标签：[spring																[spring mvc](https://so.csdn.net/so/search/s.do?q=spring mvc&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
昨天学习了Spring，知道里面有两个重要的内容，IoC和AOP。但更重要的是，Spring是一个MVC框架，如何应用在Web上呢？
所以今天又看了下Spring MVC。按照网上教程一步一步来，搞得很崩溃。记录心得如下： 
**一、创建WEB项目**
eclipse下，new -> Project… -> Web -> Dynamic Web Project。 
记得在最后一步，勾上”Generate web.xml deployment descriptor”，以自动生成web.xml。
**二、添加servlet、监听**
刚生成的Web项目，目录大致是： 
![这里写图片描述](https://img-blog.csdn.net/20170913191051273?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中WebContent目录即为网站内容。在里面添加一个test.html,Run as Server 到tomcat，通过`http://localhost:8080/test/test.html` 即可访问到这个页面。
但加上相关servlet之后，情况就发生了变化了。我按照教程的步骤，添加的servlet，会将*.html的请求地址，引导到jsp里面去。里面的原理现在还无暇顾及，留待日后再学习。
**三、运行**
在Eclipse中运行web，当然是Run as Server。要求本机安装Web服务器，我装的是Tomcat 7.0。
结果出来的显示，404错误，访问不到什么内容。
折腾了好久，不知所谓。后来看了tomcat的控制台输出，提示找不到 “org.springframework.web.servlet.DispatcherServlet”，我靠我明明引用了spring mvc的各种jar包的。
又折腾了一阵，估计是这些包在运行时没有发布到tomcat，即那个“Web INF/lib”文件夹里面去。怎么办呢，这样： 
点Web项目，右键属性，Deployment Assemply 
![这里写图片描述](https://img-blog.csdn.net/20170913192824098?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170913192839885?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
将我们在开发阶段引入的jar包，统统加进去。
运行，正常了啊！
得到一个教训是tomcat的控制台输出内容很重要，如果情况不对，则应该看看里面输出了啥。
参考教程 
[SpringMVC 基础教程 简单入门实例](http://blog.csdn.net/swingpyzf/article/details/8904205)
