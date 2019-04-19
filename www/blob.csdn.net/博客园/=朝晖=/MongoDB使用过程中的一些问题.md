# MongoDB使用过程中的一些问题 - =朝晖= - 博客园
# [MongoDB使用过程中的一些问题](https://www.cnblogs.com/dhcn/p/7120900.html)
        1、[MongoDB](http://lib.csdn.net/base/mongodb)配置修改不生效的问题：今天因为某个原因，需要修改[mongodb](http://lib.csdn.net/base/mongodb)的配置文件。 改完以后，在init.d里面restart命令重启server，后来stop又start重启server。修改都不能生效，后来stop以后看了一下进程，发现mongodb依然活着，原来相关命令根本不生效。不过它提供了force动作命令，我懒得用命令了，直接shutdown -r，配置修改生效。环境：ubuntu14.04
        2、MongoEngine的连接参数问题，正确的写法应该是:
```
connect(username="username", password="password", host="mongodb://username:password@123.57.59.76:27017/resultdb")
```
如果你只在函数参数中写用户名密码，在host字符串中不写，可能导致查询因权限问题失败的提示：not authorized for query on  db.collecitonname 的错误。
       3、Django中使用MongoDB，刚开始用mongodb-engine,后来发现它有个缺憾，使用它的话，Django本身都得使用该工具官方改造过的版本。后来改用mongoengine这个ORM解决方案，不过这个ORM当前状态，还不能完全作为Django生产环境的Backend使用。
      4、在pyspider中用这个做后台[数据库](http://lib.csdn.net/base/mysql)使用，本来是想发挥其JSON威力，想不到pyspider在后台存储结果直接就是JSON字符串，说白了成了数据库的string字段，没有起到预想的作用。
      5、第三条的mongoengine也有个纠结的问题，如果数据库JSON对象里面有个某个键值字段，而ORM Document里面没有定义这个字段，读数据的时候会报错，这一点和RDB的ORM区别很大。
      6、使用mongodb这种nosql数据库就像使用SQL关系数据库一样，使用关系数据库，使用什么API，自己都得懂SQL，使用Mongodb也一样，使用什么库操作它，自己都得对其操作指令比较熟悉。

