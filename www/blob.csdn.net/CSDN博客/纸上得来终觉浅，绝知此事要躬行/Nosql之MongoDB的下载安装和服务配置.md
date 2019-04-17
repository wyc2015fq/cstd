# Nosql之MongoDB的下载安装和服务配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年03月07日 00:27:32[boonya](https://me.csdn.net/boonya)阅读数：2205标签：[MongoDB																[Database](https://so.csdn.net/so/search/s.do?q=Database&t=blog)](https://so.csdn.net/so/search/s.do?q=MongoDB&t=blog)
个人分类：[MongoDB](https://blog.csdn.net/boonya/article/category/1358734)








1、下载mongodb的windows版本，有32位和64位版本（当然也有linux版本的），根据系统情况下载即可。下载地址：[http://www.mongodb.org/downloads](http://www.mongodb.org/downloads)，这里我下载的是mongodb-win32-i386-2.4.0-rc1。

2、解压缩至任意硬盘分区目录：如，D:/DB/MongoDB，并在任意地方创建数据库文件的存放位置，比如D:/DB/MongoDB/db（启动mongodb服务之前需要必须创建数据库文件的存放文件夹，否则命令不会自动创建，而且不能启动成功。默认文件夹路径为c:/data/db.使用系统默认文件夹路径时，启动服务无需加--dbpath 参数说明，但文件夹还要手工创建）。

3、打开cmd命令行，进入D:/DB/MongoDB/bin目录，输入如下的命令启动mongodb服务,如图所示：![](https://img-my.csdn.net/uploads/201303/06/1362584386_6670.jpg)





























4、进入MongoDB进行简单的操作

（1）、启动mongo db，如图：

![](https://img-my.csdn.net/uploads/201303/06/1362584869_2654.jpg)





（2）、执行插入和查询，如图：

![](https://img-my.csdn.net/uploads/201303/06/1362584997_9115.jpg)

5、创建MongoDBwindows 服务，让系统自动启动服务。

(1)、创建服务




在MongoDB安装目录下创建logs目录，然后在CMD命令行输入如下命令：


**D:\DB\MongoDB\bin>mongod --logpath D:\DB\MongoDB\logs\MongoDB.log --logappend**

** --dbpath D:\DB\MongoDB\db --directoryperdb --serviceName MongoDB --install**

效果图：
![](https://img-my.csdn.net/uploads/201303/07/1362587451_1675.jpg)




(2)、启动/停止服务
**net start MongoDBnet stop MongoDB**



(3)、删除服务


**D:\DB\MongoDB\bin\mongod.exe --remove**





