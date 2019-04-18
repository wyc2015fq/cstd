# Spring Security教程（大纲）----学习过程分享 - z69183787的专栏 - CSDN博客
2014年03月13日 21:18:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3302
今天给大家分享一下我学习SpringSecurity的过程，及我随后要讲的Spring Security的学习大纲。一为指导想学习SpringSecurity的同学，二为留着自己备用，以便忘了的时候翻出来看看。第一次写博客，写的不好的地方还请大家勿喷。
Spring Security是Spring的一个安全框架，它的前身是Acegi Security.这个框架主要分为两个部分，认证、验证。
- 认证是为用户建立一个他所声明的主体，主体一般是指用户。
- 认证是判断用户访问的资源是否有足够的权限。
因为项目的需求，需要用到细粒度的权限控制，所以决定用SpringSecurity来做。也因为我自己一直想研究SpringSecurity，所以项目中权限的部分就由我来做了。我学习SpringSecurity大概用了两个星期，虽然研究的不够深，但在项目中用也够了。下面我分享下我学习SpringSecurity的过程：
- SpringSecurity环境搭建（我用的是SpringSecurity  3.2）
- SpringSecurity简单测试
- 自定义登录页面
- 使用数据库进行用户认证、密码加密
- 使用数据库进行URL资源认证
- 使用数据库进行Method资源认证
- remember-me功能、验证码功能的实现
教程大纲安排如下
- [SpringSecurity环境搭建（与SpringMVC整合）](http://blog.csdn.net/jaune161/article/details/17640071)
- [SpringSecurity简单测试](http://blog.csdn.net/jaune161/article/details/18350183)
- [自定义登录页面及](http://blog.csdn.net/jaune161/article/details/18351247)
- [数据库表结构创建](http://blog.csdn.net/jaune161/article/details/18353397)
- [国际化配置及UserCach](http://blog.csdn.net/jaune161/article/details/18356061)
- [使用数据库管理用户及权限](http://blog.csdn.net/jaune161/article/details/18354599)
- [解决UsernameNotFoundException无法被捕获的问题](http://blog.csdn.net/jaune161/article/details/18359321)
- 使用数据库管理URL资源
- 使用注解保护Method
- 使用数据库管理Method
- remember-me功能、验证码功能的实现
在实际教程编写过程中可能会有变动、并且会把相关链接加上。
SpringSecurity参考资料 
[http://www.mossle.com/docs/auth/html/index.html](http://www.mossle.com/docs/auth/html/index.html)
[http://www.blogjava.net/SpartaYew/archive/2011/06/15/350630.html](http://www.blogjava.net/SpartaYew/archive/2011/06/15/350630.html)
