# 记录一次让我吐血的spring3 MVC HTTP406 Json转换错误 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2014年03月17日 22:40:12[gloomyfish](https://me.csdn.net/jia20003)阅读数：34245








朋友给了我点东西，我就想把它用Maven重新搞一下，不要ant啦。

看到它的spring dispatch servlet url mapping 是根据*.do来做的

我不习惯就为了*.html然后其它都搞了过来，tomcat一跑发现一个

第一个错误：

![](https://img-blog.csdn.net/20140317220852843)


于是我在对应的spring servlet XML文件中加了<mvc:default-servlet-handler/>

问题解决啦，然后前台JS调用返回json数据时出现了第二个错误

![](https://img-blog.csdn.net/20140317221209015)


HTTP-406错误[Spring
 MVC + JSON = 406 Not Acceptable](http://stackoverflow.com/questions/16335591/spring-mvc-json-406-not-acceptable)

然后我就一直检查配置文件，网上一google发现很多人跟我一样问题

原因基本上classpath下是没有加入jackson的maven依赖或者是

没有启用注解标签在配置文件中<mvc:annotation-driven />

或者是controller中方法返回时候没有使用@ResponseBody

而我自己检查了一下，我三个都有啊，这也太没天理啦，就在翻来覆去

找原因的时候，无意中又点到web.xml，突然我知道原因是什么啦，看下图

![](https://img-blog.csdn.net/20140317221850656)


**于是我一个晚上的时间就没有啦，这算谁的错误啊!**

**最后想起了一句英文：**

**[A clever person solves a problem.A wise person avoids it.](http://refspace.com/quotes/Albert_Einstein/Q697)**

- Albert Einstein

大师的话总是这么有道理！！






