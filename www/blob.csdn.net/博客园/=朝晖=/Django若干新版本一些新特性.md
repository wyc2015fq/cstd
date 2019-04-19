# Django若干新版本一些新特性 - =朝晖= - 博客园
# [Django若干新版本一些新特性](https://www.cnblogs.com/dhcn/p/7124649.html)
前面那篇文章简单总结了1.9版本的新特性，其实这是我一直想做的一件事情，不过因为对于工作不是那么紧急需要，所以一直被搁浅着，今天既然做了就多做一点吧
Django1.8的新特性大概(2015年4月发布):
1、官方开始内建支持Model._meta API
2、多模板引擎
3、安全加强：
- Several features of the django-secure third-party library have been integrated into Django.   
- django.middleware.security.SecurityMiddleware provides several security enhancements to the request/response cycle.   
- The new check --deploy option allows you to check your production settings file for ways to increase the security of your site.  
4、新的PostgreSQL特定功能
5、查询表达式增强：比如[Conditional Expressions](https://docs.djangoproject.com/en/1.9/ref/models/conditional-expressions/) allow you to use [`if`](https://docs.python.org/3/reference/compound_stmts.html#if) ... [`elif`](https://docs.python.org/3/reference/compound_stmts.html#elif) ... [`else`](https://docs.python.org/3/reference/compound_stmts.html#else) logic within queries.
6、TestCase data setup
Django1.7新特性大概(2014年9月发布)
1、开始支持数据模式升级(Migrations)特性
2、app loading 重构
3、字段的新方法
4、新的Manager定制方式
5、查询反向关系时使用定制Manager
6、新的系统检查框架
7、新的Prefetch对象定义预提取操作
Django1.6新特性大概(2013年11月发布)：
1、项目及App默认模板简化
2、增强的事务管理
3、[数据库](http://lib.csdn.net/base/mysql)长连接
4、[测试](http://lib.csdn.net/base/softwaretest)代码发现机制强化
5、时区聚合
6、二进制字段
7、check指令
8、save()方法[算法](http://lib.csdn.net/base/datastructure)改变
Django1.5新特性大概(2013年11月发布):
1、引入Python3的支持
2、可配置的UserModel
3、支持存储字段子集
4、相关数据实例的缓存
5、流式Response的显式支持
Django1.4新特性大概(2012年3月发布)
1、开始支持时区
2、浏览器内测试框架支持
3、项目默认布局发生变化
4、可定制的项目和App模板
5、增强的WSGI支持
6、SELECT FOR UPDATE support
在以前的就不说了，我好像是从1.4开始在生产项目中使用Django。以上版本修改信息只为简要大概，出现版本升级问题没招时，去看官方Release Note。

