# Windows下的安装MongoDB - 零度的博客专栏 - CSDN博客
2016年07月19日 17:41:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：385标签：[mongodb](https://so.csdn.net/so/search/s.do?q=mongodb&t=blog)
个人分类：[Mongodb](https://blog.csdn.net/zmx729618/article/category/6319580)
1.下载，直接从[http://www.mongodb.org/downloads](http://www.mongodb.org/downloads) 下载需要的版本即可
2.解压，下载好MongoDB数据后，将此解压至C:\MongoDB下面，直接将mongodb-win32-x86_64-2008plus-2.4.8目录中文件的解压至了MongoDB，这样的目录看起来还简洁不少。
3.创建数据库文件的存放位置，比如c:/mongodb/dbData。启动mongodb服务之前需要必须创建数据库文件的存放文件夹，否则命令不会自动创建，而且不能启动成功。默认文件夹路径为c:/data/db.使用系统默认文件夹路径时，启动服务无需加--dbpath 参数说明，但文件夹还要手工创建
4.运行,打开cmd命令行，进入C:/MongoDB/bin目录，输入如下的命令启动mongodb服务：
![](http://images.cnitblog.com/blog/142184/201311/22162138-7cee4f673f2f4eb695da4a670dea6f03.png)
此时MongoDB数据库已经成功运行，最下面显示的一条1 connection now open表示已经有链接连接上来了。此时使用的是默认的端口号，如下所示：
![](http://images.cnitblog.com/blog/142184/201311/22162514-faab806e335c4f68b05e235d3b7fd259.png)
那么要指定端口号，则需要使用下面的命令：
C:/mongodb/bin/mongod.exe  --port 8888 --dbpath C:/mongodb/dbData 即可使用指定的端口
5.连接测试，再打开一个cmd输入：c:/mongodb/bin>mongo，或者双击mongo.exe，即可进行mongodb的客户端命令操作了,测试下
c:\Mongodb\bin>mongo.exe
MongoDB shell version: 2.4.8
connecting to: test
> 1+1
2
> db
test
> db.foo.insert({test:helloWorld})
Fri Nov 22 16:31:15.350 ReferenceError: helloWorld is not defined
> db.foo.insert({b:100})
> db.foo.find()
{ "_id" : ObjectId("528f165c87d7a726554b7a57"), "b" : 100 }
>
此时表明数据库已经能正常使用.
6.安装成服务，每次启动太不方便，因此需要将MongoDB像MySQl一样做成服务
要做成服务，需要切换到bin目录下
使用以下命令将MongoDB安装成为Windows服务。笔者的MongoDB目录为D:\Program Files\mongodb
切换到D:\Program Files\mongodb\bin>
```
mongod --logpath "C:\mongodb\logs.txt" --logappend --dbpath "c:\MongoDB\dbData\" --directoryperdb --serviceName "MongoDB" --serviceDisplayName "MongoDB" --install
```
输入以上命令。
Creating service MongoDB.
Service creation successful.
Service can be started from the command line via 'net start "MongoDB"'.
该命令行指定了日志文件：C:\mongodb\logs.txt，日志是以追加的方式输出的；
数据文件目录：C:/mongodb/dbData ，并且参数--directoryperdb说明每个DB都会新建一个目录；
Windows服务的名称：MongoDB；
最后是安装参数：--install，与之相对的是--remove
启动MongoDB：net start MongoDB
停止MongoDB：net stop MongoDB
注意：遇到问题请查看日志文件
mongodb exception in initAndListen: 12596 old lock file, terminating解决方法
错误信息如下：
exception in initAndListen: 12596 old lock file, terminating
解决方法
1.删除data目录中的.lock文件
2.mongod.exe --repair
3.启动mongod就可以了
