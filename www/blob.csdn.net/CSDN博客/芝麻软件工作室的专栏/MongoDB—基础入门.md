
# MongoDB—基础入门 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月30日 21:44:03[seven-soft](https://me.csdn.net/softn)阅读数：200


关于mongodb的好处，优点之类的这里就不说了，唯一要讲的一点就是mongodb中有三元素：数据库，集合，文档，其中“集合”
就是对应关系数据库中的“表”，“文档”对应“行”。
一： 下载
上[MongoDB官网](http://www.mongodb.org/downloads)，我们发现有32bit和64bit，这个就要看你系统了，不过这里有两点注意：
①：根据业界规则，偶数为“稳定版”（如：1.6.X，1.8.X），奇数为“开发版”（如：1.7.X，1.9.X)，这两个版本的区别相信大家都知道吧。
②：32bit的mongodb最大只能存放2G的数据，64bit就没有限制。
我这里就下载"2.0.2版本，32bit“，ok，下载之后我就放到”E盘“，改下文件夹名字为”mongodb“。
二：启动
①：启动之前，我们要给mongodb指定一个文件夹，这里取名为”db",用来存放mongodb的数据。
![](http://pic002.cnblogs.com/images/2012/214741/2012021722595889.png)
②：微软徽标+R，输入cmd，首先找到“mongodb”的路径，然后运行mongod开启命令，同时用--dbpath指定数据存放地点为“db”文件夹。
![](http://pic002.cnblogs.com/images/2012/214741/2012021723060683.png)
③：最后要看下是否开启成功，从图中的信息中获知，mongodb采用27017端口，那么我们就在浏览器里面键入“[http://localhost:27017/](http://localhost:27017/)”，
打开后，mongodb告诉我们在27017上Add 1000可以用http模式查看mongodb的管理信息。
![](http://pic002.cnblogs.com/images/2012/214741/2012021723201250.png)
三：基本操作
由于是开篇，就大概的说下基本的“增删查改“，我们再开一个cmd，输入mongo命令打开shell，其实这个shell就是mongodb的客户端，
同时也是一个js的编译器，默认连接的是“test”数据库。
![](http://pic002.cnblogs.com/images/2012/214741/2012021723343781.png)
<1>  insert 操作
好，数据库有了，下一步就是集合，这里就取集合名为“person”，要注意的就是文档是一个json的扩展（Bson)形式。
![](http://pic002.cnblogs.com/images/2012/214741/2012021800015368.png)
<2> find 操作
我们将数据插入后，肯定是要find出来，不然插了也白插，这里要注意两点：
① “_id"： 这个字段是数据库默认给我们加的GUID，目的就是保证数据的唯一性。
② 严格的按照Bson的形式书写文档，不过也没关系，错误提示还是很强大的。
![](http://pic002.cnblogs.com/images/2012/214741/2012021723563119.png)
<3> update操作
update方法的第一个参数为“查找的条件”，第二个参数为“更新的值”，学过C\#，相信还是很好理解的。
![](http://pic002.cnblogs.com/images/2012/214741/2012021800091093.png)
<4> remove操作
remove中如果不带参数将删除所有数据，呵呵，很危险的操作，在mongodb中是一个不可撤回的操作，三思而后行。
![](http://pic002.cnblogs.com/images/2012/214741/2012021800132992.png)


