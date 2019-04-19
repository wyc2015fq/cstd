# django1.8升级1.9的几个问题 - =朝晖= - 博客园
# [django1.8升级1.9的几个问题](https://www.cnblogs.com/dhcn/p/7124646.html)
1、URL Pattern警告，旧式的URL定义方法将在1.10版本中被废止，所以这个版本仅仅是警告，不过这个警告让我看到了升级1.9这个非LTS版本的意义。
2、Django自身所带Models变化，这个当然是用makemigrations和migrate指令就可以解决，不过不是所有的1.8子版本升级都有这个问题。
3、datetime默认显示格式发生变化，新的格式比较适合老外，所以对以国人来讲，必须以带参数的date或notzdate过滤器处理日期输出,或者在Settings里面加个DATE_FORMAT默认格式设置
Django 1.9的发布声明 new feature大概：
1、on_commit挂钩，用于事物执行成功后的事件操作
2、弱密码验证检查
3、面向Class based View的Permission Maxin
4、[测试](http://lib.csdn.net/base/softwaretest)并行执行
5、admin的地址等特性变化
6、验证模块的一些变化：比如密码加密迭代次数增加20%
7、GIS特性变化(不细说了)
8、postgres特性变化：
- Added support for the rangefield.contained_by lookup for some built in fields which correspond to the range fields.  
- Added JSONField.  
- Added PostgreSQL specific aggregation functions.  
- Added the TransactionNow database function.  
9、Session后端的构建机制发生变化
10、CSRF的变化：
- The request header’s name used for CSRF authentication can be customized with CSRF_HEADER_NAME.  
- The CSRF referer header is now validated against the CSRF_COOKIE_DOMAIN setting if set. See How it works for details.  
- The new CSRF_TRUSTED_ORIGINS setting provides a way to allow cross-origin unsafe requests (e.g. POST) over HTTPS.  
还有很多新特性，1、2句说不完，自己去看release note吧。说点不向后兼容的大东西
1、PostgresSQL9.0不在被支持
2、Removal of time zone aware global adapters and converters for datetimes--这是上面问题3的发生原因。
Removed Feature：
- django.utils.dictconfig is removed.  
- django.utils.importlib is removed.  
- django.utils.tzinfo is removed.  
- django.utils.unittest is removed.  
- The syncdb command is removed.  
