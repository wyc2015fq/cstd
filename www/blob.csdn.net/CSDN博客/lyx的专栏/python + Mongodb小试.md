# python + Mongodb小试 - lyx的专栏 - CSDN博客





2017年05月21日 22:14:29[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：542








很早就想接触一下NoSql了，苦于一直备考。考完FRM二级之后，开始有时间接触新的东西了。Mongdb的名字一听就比Redis有趣。

# 1.启动Mongodb

如何安装就不说了，基本没什么门槛。安装完之后，在安装目录的根目录下创建data\db文件夹，然后进入安装目录下的bin文件夹中，打开命令行，运行mongod，就可以启动一个mongodb的服务了。就像启动一个mysql服务一样。启动之后，后面会显示有prot号，也就是端口号。mongodb的默认端口号27017.

# 2.安装python的mongodb包

这个最简单，pip install pymongo就可以了。

# 3.试一下吧



```python
# coding=utf-8
from pymongo import MongoClient
# 1.1创建mongodb的连接，端口为默认端口27017，端口号也可以省略
mc = MongoClient('localhost', 27017)
# 1.2使用mydb数据库，不存在则新建
db = mc.mydb
# 1.3同样的，使用名字为mycol的collection，相当于mysql中的tabe，如果不存在则新建
mycolection = db.mycol
# 2.1往table中写入两个记录
mycolection.save({"name":'luyixiao', "sexy":"yes"})
mycolection.save({"name":'haha', "sexy":"no", "age":90})
# 2.2查询这两个记录，并显示出来
rst = mycolection.find()
for ele in rst:
    print ele
# 3.关闭连接
mc.close()
```
很简单，很sexy！






