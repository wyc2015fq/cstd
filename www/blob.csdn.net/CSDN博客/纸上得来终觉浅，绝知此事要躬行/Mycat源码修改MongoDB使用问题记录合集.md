# Mycat源码修改MongoDB使用问题记录合集 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月31日 14:44:18[boonya](https://me.csdn.net/boonya)阅读数：682










mycat作为数据库中间件也有一定的问题，在使用过程中不免会遇到各种各样的问题，基于此我们将有关的问题都做一个记录以备作FAQ查看。以下问题是针对MongoDB修改使用而提出的，请对号入座。mycat官方问题列表：[https://github.com/MyCATApache/Mycat-Server/issues](https://github.com/MyCATApache/Mycat-Server/issues)

## 问题0：mycat 源码修改后打的jar包替换后不起作用


此问题是因为：在/usr/local/mycat/lib下不能存在多个Mycat-server的发布版本，如果需要备份请放置于其它路径下。




## 问题1：mycat 大批量查询挂起，所有连接断开后恢复

这种是大数据查询，出现无响应挂起，客户端终止也没用。详情有待DBA收集日志。



## 问题2：网关写入mycat（正常），mysql有时报错

Fatal error encountered during command execution. ---> MySql.Data.MySqlClient.MySqlException (0x80004005): Fatal error encountered attempting to read the resultset. ---> MySql.Data.MySqlClient.MySqlException (0x80004005): Reading from the stream has failed. ---> System.IO 


.EndOfStreamException: 尝试读取超出流末尾的内容。




解决方法：建议将mysql写超时时间设长一点。

![](https://img-blog.csdn.net/20170831144120586)




## 问题3：其它查询都正常为何这个查询语句就是查不出来结果




注意：我们不要在字段里面包含关键字，有些字段分割如AND、IN、OR等关键字可能导致条件解析时异常。

解决方法：排查WHERE后面的条件，字段里面最好不要夹杂关键字。




## 问题4：条件或——OR查询是一个奇葩，偶尔有结果大多数时候是空结果集

加了OR 条件的查询返回结果集为空，or是一个集合条件，来看看MongoDB Shell的操作：

db.getCollection("goods").find({ "$or" : [ { "user" : "jhon"} , { "owner" :"jhon"}]})我们习惯于 a=1 or a=20 这样的SQL语句，必须解析条件or才能实现，不妨自己做做实验。可以参考：[http://blog.csdn.net/boonya/article/details/77530263](http://blog.csdn.net/boonya/article/details/77530263)


MongoDB客户端测试:


`db.getCollection('T_SUBTABLE_DATE_20170801').find({"$or":[{"F_TIME":{"$gte":"2017-08-01 06:00:00","$lte":"2017-08-01 08:00:00"}},{"F_SEX":"female"}]});`
![](https://img-blog.csdn.net/20170831172636381)





MongoDB源码里面支持的：


![](https://img-blog.csdn.net/20170831172133408)

大多数情况：




![](https://img-blog.csdn.net/20170831172152658)

偶尔出现：

![](https://img-blog.csdn.net/20170831172415789)





因为不稳定，还是尽量避免使用OR查询。


## 问题5：大数据查询手动停止导致mycat服务瘫痪不可用【已解决】

![](https://img-blog.csdn.net/20170913175100604)




大数据表或全表查询 ：查询过程中手动停止会有问题（可以等待自动超时）， 手动停止后mycat服务虽然保持但是实际上已经不对外提供服务了 ，这就是为什么服务存活而连不上的原因  ， BUG处理方式请参考：[mycat issue 1616](https://github.com/MyCATApache/Mycat-Server/issues/1616)。







