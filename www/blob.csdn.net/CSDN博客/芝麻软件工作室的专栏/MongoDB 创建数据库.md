
# MongoDB 创建数据库 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月30日 22:26:40[seven-soft](https://me.csdn.net/softn)阅读数：158个人分类：[MongoDB																](https://blog.csdn.net/softn/article/category/6339531)



### 语法
MongoDB 创建数据库的语法格式如下：
use DATABASE_NAME如果数据库不存在，则创建数据库，否则切换到指定数据库。
### 实例
以下实例我们创建了数据库 runoob:
> use runoob
switched to db runoob
> db
runoob
>如果你想查看所有数据库，可以使用show dbs命令：
> show dbs
local  0.078GB
test   0.078GB
>可以看到，我们刚创建的数据库 runoob 并不在数据库的列表中， 要显示它，我们需要向 runoob 数据库插入一些数据。
> db.runoob.insert({"name":"菜鸟教程"})
WriteResult({ "nInserted" : 1 })
> show dbs
local   0.078GB
runoob  0.078GB
test    0.078GB
>MongoDB 中默认的数据库为 test，如果你没有创建新的数据库，集合将存放在 test 数据库中。

