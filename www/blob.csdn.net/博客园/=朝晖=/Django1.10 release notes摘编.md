# Django1.10 release notes摘编 - =朝晖= - 博客园
# [Django1.10 release notes摘编](https://www.cnblogs.com/dhcn/p/7124879.html)
一、重点新特性：
1、面向PostgreSQL的**全文搜索支持**
2、新式风格的middleware
3、用户名Unicode编码的官方支持
二、一些可以提的改变：
1、用户名最大长度增加到150
2、不再支持PostgreSQL9.1
三、推荐不再使用的特性：
1、多对多反向集合赋值特性
2、无时区的Storage API
3、 User.is_authenticated() and User.is_anonymous()这两个方法以后将会变为属性
四、升级中碰到的主要问题：
1、PostgreSQL9.1必须得升级大版本
2、以前的Forms Fields说明的元组，如果元组里包括一个字段，那该字段之后得加个逗号，否则会报错。
3、 django.conf.urls.patterns已经彻底被删除，如果你在代码中用了这个东西，可能得改代码了。

