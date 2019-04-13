
# window平台安装 MongoDB -  芝麻软件工作室的专栏 - CSDN博客


2016年07月30日 22:14:11[seven-soft](https://me.csdn.net/softn)阅读数：183个人分类：[MongoDB																](https://blog.csdn.net/softn/article/category/6339531)



## MongoDB 下载
MongoDB提供了可用于32位和64位系统的预编译二进制包，你可以从MongoDB官网下载安装，MongoDB预编译二进制包下载地址：[http://www.mongodb.org/downloads](http://www.mongodb.org/downloads)
> 注意：在 MongoDB2.2 版本后已经不再支持 Windows XP 系统。
![](http://www.runoob.com/wp-content/uploads/2013/10/mongodb-download.jpg)
MongoDB for Windows 64-bit适合 64 位的 Windows Server 2008 R2, Windows 7 , 及最新版本的 Window 系统。
MongoDB for Windows 32-bit适合 32 位的 Window 系统及最新的 Windows Vista。 32 位系统上 MongoDB 的数据库最大为 2GB。
MongoDB for Windows 64-bit Legacy适合 64 位的 Windows Vista, Windows Server 2003, 及 Windows Server 2008 。
根据你的系统下载 32 位或 64 位的 .msi 文件，下载后双击该文件，按操作提示安装即可。
安装过程中，你可以通过点击 "Custom(自定义)" 按钮来设置你的安装目录。
![](http://www.runoob.com/wp-content/uploads/2013/10/win-install1.jpg)
![](http://www.runoob.com/wp-content/uploads/2013/10/win-install2.jpg)
创建数据目录
MongoDB将数据目录存储在 db 目录下。但是这个数据目录不会主动创建，我们在安装完成后需要创建它。请注意，数据目录应该放在根目录下（(如： C:\ 或者 D:\ 等 )。
在本教程中，我们已经在C：盘 安装了 mongodb，现在让我们创建一个data的目录然后在data目录里创建db目录。
c:\>cd c:\
c:\>mkdir data
c:\>cd data
c:\data>mkdir db
c:\data>cd db
c:\data\db>你也可以通过window的资源管理器中创建这些目录，而不一定通过命令行。

---

## 命令行下运行 MongoDB 服务器
为了从命令提示符下运行MongoDB服务器，你必须从MongoDB目录的bin目录中执行mongod.exe文件。
mongod.exe--dbpath c:\data\db如果执行成功，会输出如下信息：
2015-09-25T15:54:09.212+0800I CONTROLHotfixKB2731284orlater updateisnotinstalled,will zero-outdata files2015-09-25T15:54:09.229+0800I JOURNAL[initandlisten]journal dir=c:\data\db\j
ournal2015-09-25T15:54:09.237+0800I JOURNAL[initandlisten]recover:nojournal fil
es present,norecovery needed2015-09-25T15:54:09.290+0800I JOURNAL[durability]Durabilitythread started2015-09-25T15:54:09.294+0800I CONTROL[initandlisten]MongoDBstarting:pid=2488port=27017dbpath=c:\data\db64-bit host=WIN-1VONBJOCE882015-09-25T15:54:09.296+0800I CONTROL[initandlisten]targetMinOS:Windows7/W
indowsServer2008R22015-09-25T15:54:09.298+0800I CONTROL[initandlisten]db version v3.0.6……
---

## 将MongoDB服务器作为Windows服务运行
请注意，你必须有管理权限才能运行下面的命令。执行以下命令将MongoDB服务器作为Windows服务运行：
mongod.exe --bind_ip yourIPadress --logpath "C:\data\dbConf\mongodb.log" --logappend --dbpath "C:\data\db" --port yourPortNumber --serviceName "YourServiceName" --serviceDisplayName "YourServiceName" --install
下表为mongodb启动的参数说明：
|参数|描述|
|--bind_ip|绑定服务IP，若绑定127.0.0.1，则只能本机访问，不指定默认本地所有IP|
|--logpath|定MongoDB日志文件，注意是指定文件不是目录|
|--logappend|使用追加的方式写日志|
|--dbpath|指定数据库路径|
|--port|指定服务端口号，默认端口27017|
|--serviceName|指定服务名称|
|--serviceDisplayName|指定服务名称，有多个mongodb服务时执行。|
|--install|指定作为一个Windows服务安装。|

---

## MongoDB后台管理 Shell
如果你需要进入MongoDB后台管理，你需要先打开mongodb装目录的下的bin目录，然后执行mongo.exe文件，MongoDB Shell是MongoDB自带的交互式Javascript shell,用来对MongoDB进行操作和管理的交互式环境。
当你进入mongoDB后台后，它默认会链接到 test 文档（数据库）：
>mongoMongoDBshell version:3.0.6connecting to:test……由于它是一个JavaScript shell，您可以运行一些简单的算术运算:
>2+24>db命令用于查看当前操作的文档（数据库）：
>db
test>插入一些简单的记录并查找它：
>db.runoob.insert({x:10})WriteResult({"nInserted":1})>db.runoob.find(){"_id":ObjectId("5604ff74a274a611b0c990aa"),"x":10}>第一个命令将数字 10 插入到 runoob 集合的 x 字段中。

