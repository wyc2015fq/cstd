
# Linux平台安装MongoDB -  芝麻软件工作室的专栏 - CSDN博客


2016年07月30日 22:16:57[seven-soft](https://me.csdn.net/softn)阅读数：242个人分类：[MongoDB																](https://blog.csdn.net/softn/article/category/6339531)



MongoDB提供了linux平台上32位和64位的安装包，你可以在官网下载安装包。
下载地址：[http://www.mongodb.org/downloads](http://www.mongodb.org/downloads)
![](http://www.runoob.com/wp-content/uploads/2013/10/MongoDB-Linux.jpg)下载完安装包，并解压tgz（以下演示的是 64 位 Linux上的安装） 。
curl-O https://fastdl.mongodb.org/linux/mongodb-linux-x86_64-3.0.6.tgz    \# 下载tar-zxvf mongodb-linux-x86_64-3.0.6.tgz\# 解压mv  mongodb-linux-x86_64-3.0.6/ /usr/local/mongodb\# 将解压包拷贝到指定目录MongoDB 的可执行文件位于 bin 目录下，所以可以将其添加到PATH路径中：
exportPATH=<mongodb-install-directory>/bin:$PATH<mongodb-install-directory>为你 MongoDB 的安装路径。如本文的/usr/local/mongodb。

---

## 创建数据库目录
MongoDB的数据存储在data目录的db目录下，但是这个目录在安装过程不会自动创建，所以你需要手动创建data目录，并在data目录中创建db目录。
以下实例中我们将data目录创建于根目录下(/)。
注意：/data/db 是 MongoDB 默认的启动的数据库路径(--dbpath)。
mkdir-p/data/db
---

## 命令行中运行 MongoDB 服务
你可以再命令行中执行mongo安装目录中的bin目录执行mongod命令来启动mongdb服务。
> 注意：如果你的数据库目录不是/data/db，可以通过 --dbpath 来指定。
$./mongod2015-09-25T16:39:50.549+0800I JOURNAL[initandlisten]journal dir=/data/db/journal2015-09-25T16:39:50.550+0800I JOURNAL[initandlisten]recover:nojournal files present,norecovery needed2015-09-25T16:39:50.869+0800I JOURNAL[initandlisten]preallocateIsFaster=true3.162015-09-25T16:39:51.206+0800I JOURNAL[initandlisten]preallocateIsFaster=true3.522015-09-25T16:39:52.775+0800I JOURNAL[initandlisten]preallocateIsFaster=true7.7
---

## MongoDB后台管理 Shell
如果你需要进入MongoDB后台管理，你需要先打开mongodb装目录的下的bin目录，然后执行mongo命令文件。
MongoDB Shell是MongoDB自带的交互式Javascript shell,用来对MongoDB进行操作和管理的交互式环境。
当你进入mongoDB后台后，它默认会链接到 test 文档（数据库）：
$ cd/usr/local/mongodb/bin
$./mongoMongoDBshell version:3.0.6connecting to:testWelcometo theMongoDBshell.……由于它是一个JavaScript shell，您可以运行一些简单的算术运算:
>2+24>3+69现在让我们插入一些简单的数据，并对插入的数据进行检索：
>db.runoob.insert({x:10})WriteResult({"nInserted":1})>db.runoob.find(){"_id":ObjectId("5604ff74a274a611b0c990aa"),"x":10}>第一个命令将数字 10 插入到 runoob 集合的 x 字段中。

---

## MongoDb web 用户界面
MongoDB 提供了简单的 HTTP 用户界面。 如果你想启用该功能，需要在启动的时候指定参数 --rest 。
$./mongod--dbpath=/data/db--restMongoDB 的 Web 界面访问端口比服务的端口多1000。
如果你的MongoDB运行端口使用默认的27017，你可以在端口号为28017访问web用户界面，即地址为：http://localhost:28017。
![](http://www.runoob.com/wp-content/uploads/2013/10/mongodb-web.jpg)

