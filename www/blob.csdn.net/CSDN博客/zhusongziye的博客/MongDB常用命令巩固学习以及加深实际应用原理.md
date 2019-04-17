# MongDB常用命令巩固学习以及加深实际应用原理 - zhusongziye的博客 - CSDN博客





2018年11月10日 21:48:18[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：60标签：[MongDB常用命令巩固学习以及加深实际应用原理](https://so.csdn.net/so/search/s.do?q=MongDB常用命令巩固学习以及加深实际应用原理&t=blog)
个人分类：[数据库](https://blog.csdn.net/zhusongziye/article/category/7131156)









**MongoDB常用命令基础巩固**

1 .进入数据库

use admin



2 .增加或修改密码

db.addUser('xingoo','123')

db.addUser("xingoo","123",true) 参数分别为 用户名、密码、是否只读



3 .查看用户列表

db.system.users.find()



4 .用户认证

db.auth('xingoo','123')



5 .删除用户

db.removeUser('xingoo')



6 .查看所有用户

show users



7 .查看所有数据库

show dbs



8 .查看所有的collection集合

show collections



9 .查看各个collection的状态

db.printCollectionStats()



10 .查看主从复制状态

db.printReplicationInfo()



11 .修复数据库

db.repairDatabase()



12 .设置profiling,0:off 1:slow 2 all

db.setProfilingLevel(1)



13 .查看profiling

show profiling



14 .拷贝数据库

db.copyDatabase('xingootest','xingootest1')

db.copyDatabase("xingootest","temp","127.0.0.1")



15 .删除集合collection

db.xingootest.drop()



16 .删除当前数据库

db.dropDatabase()



**第二节MongoDB增删改命令**

1 .存储嵌套的对象

db.foo.save({'name':xingoo,'age':25,'address':{'city':'changchun','Province':'Jilin'}})



2 .存储数组对象

db.foo.save({'name':xingoo,'age':25,'address':['Jilin Province','Liaoning Province']})



3 .根据query条件修改，如果不存在则插入，允许修改多条记录

db.foo.update({'age':'25'},{'$set':{'name':'xingoo'}},upsert=true,multi=true)



4 .删除yy=5的记录

db.foo.remove({'name':'xingoo'})



5 .删除所有的记录

db.foo.remove()



**第三节MongoDB索引相关系列**

1 .增加索引:1 asc -1 desc

db.foo.ensureIndex({firstname:1,lastname:-1},{unieap:true})



2 .索引子对象(不懂)

db.foo.ensureIndex({'Al.Em':!})



3 .查看索引信息

db.foo.getIndexes()

db.foo.getIndexKeys()



4 .根据索引名删除索引(不懂)

db.foo.dropIndex('Al.Em_1')



**第四节MongoDB应用原理**

1 .MongoDB支持存储过程吗？如果支持的话，怎么用？

MongoDB支持存储过程，它是javascript写的，保存在db.system.js表中。



2 .如何理解MongoDB中的GridFS机制，MongoDB为何使用GridFS来存储文件？



3 .GridFS是一种将大型文件存储在MongoDB中的文件规范。使用GridFS可以将大文件分隔成多个小文档存放，这样我们能够有效的保存大文档，而且解决了BSON对象有限制的问题。



4 .为什么MongoDB的数据文件很大？

MongoDB采用的预分配空间的方式来防止文件碎片。



5 .如果一个分片（Shard）停止或很慢的时候，发起一个查询会怎样？



6 .如果一个分片停止了，除非查询设置了“Partial”选项，否则查询会返回一个错误。如果一个分片响应很慢，MongoDB会等待它的响应。



7 .我怎么查看 Mongo 正在使用的链接?

db._adminCommand("connPoolStats");



8 .MongoDB要注意的问题

1.因为MongoDB是全索引的，所以它直接把索引放在内存中，因此最多支持2.5G的数据。如果是64位的会更多。

2.因为没有恢复机制，因此要做好数据备份

3.因为默认监听地址是127.0.0.1，因此要进行身份验证，否则不够安全；如果是自己使用，建议配置成localhost主机名

4.通过GetLastError确保变更。



