# Django代码注意 - =朝晖= - 博客园
# [Django代码注意](https://www.cnblogs.com/dhcn/p/7115512.html)
1、模板标签里面 extend和include是冲突的，有了extend，include无法生效，原因：是底层渲染独立机制设计导致。 
2、#coding:utf-8 这句只有放在代码文件第一行才能生效，放在注释字符串后面可能会失效。 
3、由于前端发展而导致的Post请求Rest化和Django原生的技术设施层简化还有事务封装前移，由此产生的结果是业务层完全可以放在views里面。同事Restful化的好处就是可以把跨业务模块调用放在前端，保证了后端模块之间的正切
4、有用户自生成富文本内容的页面上最好不要放置带XSRF的POST表单，前者可能会窃取后者的Token信息。
5、在template里面的==这一类比较逻辑运算符号两边必须有空格，否则影响模板解析 
6、form.is_valid内部逻辑中的Clean_data处理中抛出的异常不会向外传递，只会变成form.is_valid()返回false.
7、Django的业务层和View层怎么切分这个问题，一个简单的方法就是给业务层传递什么层级的参数，个人觉得传递验证过的form比较合适。
8、多级if else的两个简化技巧：1是直接用except处理；2是该半路return的直接return掉,这样做虽然不符合过程编程函数设计原则，但是代码相对简洁了很多。
9、Ubuntu生产环境下不能Print Unicode中文，否则会导致error.
10、因为DJango的500机制和事务机制，所以Django的View层对异常处理代码的依赖比较弱。
11、model form定义：没有在前端页面出现的字段，一定要exclude掉或者Null了，不过Null会影响默认值，所以最好的方法是Exclude掉，否则即便blank掉，也会导致form存储时出错。因为表单中字段不出现会把默认值覆盖成Null。 比exclude更方便的定义方式是定义fields元信息，这样model添加不用的字段不用跑来重新更新form定义
12、[数据库](http://lib.csdn.net/base/mysql)存时区性数据的格式化显示一定要放在template里面用date之类的过滤器操作，如果用datetime的striftime直接格式化，会导致时区性数据丢失，出来的时间成了格林威治时间值了，如果在代码中strifttime处理,可以先用django.utils.timezone.localtime方法处理一下，这样出来的时间才是正常的
13、Django调试中的一个问题：众所周知，runserver启动，改动代码，服务会重启，但是改动自定义标签代码，服务是不会重启的。
14、form验证的errors在比较旧的版本里面是没有文本信息，前一段时间看文档，发现新版本有对errors有所加强，比较好用的比如as_json()和as_text()，两个方法，我在比较旧的版本中是自己写个函数对errors对象做解析生成反馈文本信息。
15、ManyToMany字段的through不能add or remove，为了扩展性的考虑，建议默认都加上through，可以为中间关系表加个date_added字段，顺便都加上unique_together约束，不过用through是有缺陷的：写操作略麻烦。那么如果你没加through，准备改成加through的，应该怎样最小改动的操作哪，应该是先把这个ManyToMany字段删除掉，并且migrate生效，然后再加一个有through的ManyToMany字段，当然了后台的数据还的备份重生效一次。这应该算是目前Django Migration特性的一个缺陷。
16、Django 邮件发送函数的默认backend不支持465端口的SSL SMTP发送，解决办法，一是把465改成587，或者pip安装django_smtp_ssl，然后在settings.py里面把email backend改一下:
```
EMAIL_BACKEND = 'django_smtp_ssl.SSLEmailBackend'
```

