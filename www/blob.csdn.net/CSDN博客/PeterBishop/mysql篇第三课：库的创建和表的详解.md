# mysql篇第三课：库的创建和表的详解 - PeterBishop - CSDN博客





2018年11月23日 13:19:04[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：12
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们讲讲怎么建库，以及对表的创建详细分析一下:



  建库特别简单:

create database dbname;



  来试试



   接下来讲建表,先把建表语句粘一下:

课件里的不通用啊。。百度一个吧


`CREATE TABLE IF NOT EXISTS 表明(``    字段名 类型 键的类型 自增 是否为空 ,``   ...``)ENGINE=引擎名 DEFAULT CHARSET=默认编码;`
上面的键的类型、自增、是否为空皆为可选项，即可写可不写

if not exist也是可写可不写

说明一下，sql大小写不敏感，所以create 和 CREATE 和 creaTE都是一样的

下面来举个例子:

Create table if not exists t_user (

  ‘id’ int(11) primary key auto_increment not null,

  ‘name’  varchar(12) not null,

  age int(2)

) engine=innodb default charset=utf8



然后，字段名和表名加不加’’都一样，加’’的话字段名和表名可以包含空格

但是，不推荐字段名和表名包含空格

加’’也可以防止字段名和mysql关键字冲突，但是建议在发生冲突的时候解决办法是给字段名加前缀或者后缀而不是加’’，所以一般情况下我们不加’’，当然加了也是对的，看个人习惯。



然后，if not exists 表示如果不存在，如果不加这个就会强制创建表，但由于表名唯一，所以如果库里已经存在同名表则会直接报错，如果加上if not exists，如果库里有同名表，则该建表sql不会被执行，但不会报错，即不会影响后续sql继续执行，所以在建表的时候建议加上 if not exists，注意exists最后一个s别忘了写



接下来，解释下刚刚那个例子:
- int(11)，括号里的11表示这个整数的最大长度
- primary key是主键， 主键的作用是该列的值必须唯一，不可重复
- not null 表示该列的值不能为空，不加就表示可以为空
- auto increment 表示自动增长，mysql会以自增的方式自动提供该列的数据
- varchar(12) varchar是可变长度的字符串，varchar(12)表名这个字符串长度最大为12
- char(12) char是不可变长度的字符串，char(12)表示这个字符串的长度 必须是12
- mysql里还有很多数据类型，常用的还有float,double，text等



  下面介绍一款mysql可视化工具 Navicat for mysql





来看看怎么使用，我先把旧有连接删了

使用步骤；
- 创建连接



连接名随便取，IP地址由于用的是本地数据库所以就localhost

Mysql默认端口号是3306

然后输入一下用户名和密码就可以了


- 双击建立好的连接



这样你mysql里的库就都能显示出来，想 打开哪个库就双击哪个库，现在我要用teach库



 可以看到表里有我们刚刚建好的t_user表



都是汉字，不用我过多解释，最后那个钥匙就是key，有钥匙就表示该列是主键



注意: 主键可以是多列，多列当主键的话只要这两列的值不完全相同即合法

  比如:

如果id和name是主键

那么两行数据(12,’小叶’,20)和(12,’徒弟’，18)是可以共存的

 当然在这个例子里主键就是id



查看表的数据的话双击表就可以了



目前还没有数据

其他功能自行探索或百度，以后我会直接在navicat里操作

最后，存储引擎默认是Innodb，不要改，字符编码一定要选uft-8，否则中文乱码



