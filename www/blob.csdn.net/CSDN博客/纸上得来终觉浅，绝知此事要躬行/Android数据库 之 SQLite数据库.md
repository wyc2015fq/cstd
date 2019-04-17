# Android数据库 之 SQLite数据库 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年08月21日 14:56:24[boonya](https://me.csdn.net/boonya)阅读数：1045标签：[android																[sqlite																[sql																[java																[database](https://so.csdn.net/so/search/s.do?q=database&t=blog)
个人分类：[Android](https://blog.csdn.net/boonya/article/category/2496125)









原文地址：[http://www.cnblogs.com/kgb250/archive/2012/08/28/sqlitedatabase.html](http://www.cnblogs.com/kgb250/archive/2012/08/28/sqlitedatabase.html)

一、关系型数据库SQLIte



![SQLite商标](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%871.png)


　　每个应用程序都要使用数据，Android应用程序也不例外，Android使用开源的、与操作系统无关的SQL数据库—SQLite。SQLite第一个Alpha版本诞生于2000年5月，它是一款轻量级数据库，它的设计目标是嵌入式的，占用资源非常的低，只需要几百K的内存就够了。SQLite已经被多种软件和产品使用，Mozilla FireFox就是使用SQLite来存储配置数据的，Android和iPhone都是使用SQLite来存储数据的。


SQLite体系结构图如下：

![框架图](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%872.jpg)


　　编译器包括Tokenizer（词法分析器）、 Parser（语法分析器）、Code Generator（代码产生器）。他们协同处理文本形式的结构化查询语句。


　　后端由B-tree，Pager，OS Interface组成。B-tree的职责是负责排序，维护多个数据库页之间错综复杂的关系，将页面组织成树状结构，页面就是树的叶子。Pager负责传输，根据B-tree的请求从磁盘读取页面或者写入页面。


　　公共服务中有各种实用的功能比如：内存分配、字符串比较，Unicode转换等。

> 
 SQLite数据库是D.Richard Hipp用C语言编写的开源嵌入式数据库，支持的数据库大小为2TB。它具有如下特征：

1、轻量级

SQLite和C\S模式的数据库软件不同，它是进程内的数据库引擎，因此不存在数据库的客户端和服务器。使用SQLite一般只需要带上它的一个动态库，就可以享受它的全部功能。而且那个动态库的尺寸也相当小。

2、独立性

SQLite数据库的核心引擎本身不依赖第三方软件，使用它也不需要“安装”，所以在使用的时候能够省去不少麻烦。

3、隔离性

SQLite数据库中的所有信息（比如表、视图、触发器）都包含在一个文件内，方便管理和维护。

4、跨平台

SQLite数据库支持大部分操作系统，除了我们在电脑上使用的操作系统之外，很多手机操作系统同样可以运行，比如Android、Windows Mobile、Symbian、Palm等。

5、多语言接口

SQLite数据库支持很多语言编程接口，比如C\C++、Java、Python、dotNet、Ruby、Perl等，得到更多开发者的喜爱。

6、安全性

SQLite数据库通过数据库级上的独占性和共享锁来实现独立事务处理。这意味着多个进程可以在同一时间从同一数据库读取数据，但只有一个可以写入数据。在某个进程或线程向数据库执行写操作之前，必须获得独占锁定。在发出独占锁定后，其他的读或写操作将不会再发生。



SQLite官方网站（[http://www.sqlite.org](http://www.sqlite.org/)）,了解更多内容请前往。


**二、导出查看数据库文件**


　　在android中，为某个应用程序创建的数据库，只有它可以访问，其它应用程序是不能访问的，数据库位于Android设备/data/data/*package_name*/databases文件夹中。


　　想要将数据库文件导出可以使用eclipse，如图所示：

![导出数据库文件](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%873.png)


　　查看数据库，使用SQlite Database Browser，如图所示：

![查看数据库](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%874.png)



**三、扩展类**


3.1扩展SQLiteOpenHelper


　　Android 不自动提供数据库。在 Android 应用程序中使用 SQLite，必须自己创建数据库，然后创建表、索引，填充数据。Android 提供了 SQLiteOpenHelper 帮助你创建一个数据库，你只要继承 SQLiteOpenHelper 类，就可以轻松的创建数据库。SQLiteOpenHelper 类根据开发应用程序的需要，封装了创建和更新数据库使用的逻辑。SQLiteOpenHelper 的子类，至少需要实现三个方法：


　　构造函数，调用父类 SQLiteOpenHelper 的构造函数


　　onCreate（）方法；// TODO 创建数据库后，对数据库的操作


　　onUpgrage()方法。// TODO 更改数据库版本的操作


　　当你完成了对数据库的操作（例如你的 Activity 已经关闭），需要调用 SQLiteDatabase 的 Close() 方法来释放掉数据库连接。


　　操作数据库的最佳实践是创建一个辅助类，例如联系人模块


　　class ContactsDatabaseHelper extends SQLiteOpenHelper


3.2 Cursor类


　　Android使用Cursor类返回一个需要的值，Cursor作为一个指针从数据库查询返回结果集，使用Cursor允许Android更有效地管理它们需要的行和列，你使用ContentValues对象存储键/值对，它的put()方法允许你插入不同数据类型的键值。


3.3 数据类型



　　SQLite 和其他数据库最大的不同就是对数据类型的支持，创建一个表时，可以在 CREATE TABLE 语句中指定某列的数据类型，但是你可以把任何数据类型放入任何列中。当某个值插入数据库时，SQLite 将检查它的类型。如果该类型与关联的列不匹配，则 SQLite 会尝试将该值转换成该列的类型。如果不能转换，则该值将作为其本身具有的类型存储。比如可以把一个字符串（String）放入 INTEGER 列。SQLite 称这为“弱类型”（manifest typing.）。





**四、数据库操作**


4.1创建和打开数据库


　　在Android中创建和打开一个数据库都可以使用openOrCreateDatabase方法来实现，因为它会自动去检测是否存在这个数据库，如果存在则打开，如果不存在则创建一个数据库：创建成功则返回一个SQLiteDatebase对象，否则抛出异常FileNotFoundException。


下面我们来创建一个名为Test的数据库，并返回一个SQLiteDatabase对象mSQLiteDatabase。

```
mSQLiteDatabase=this.openOrCreateDatabase("Test",MODE_PRIVATE,null);
```


4.2创建表


　　通过execSQL方法来执行一条SQL语句。

```
String CREATE_TABLE="create table 表名（列名，列名，……）";
mSQLiteDatabase.execSQL(CREATE_TABLE);
```


　　创建表的时候总要确定一个主键，这个字段是64位整型，别名_rowid。其特点就是自增长功能。当到达最大值时，会搜索该字段未使用的值（某些记录被删除_rowid会被回收），所以要唯一严格增长的自动主键必须加入关键字autoincrement。


4.3删除表

```
mSQLiteDatabase("drop table 表名");
```


4.4修改数据


4.4.1 插入记录


可以使用insert方法来添加数据，但是insert方法要求把数据都打包到ContentValues中，ContentValues其实就是一个Map，Key值是字段名称，Value值是字段的值。通过ContentValues的put方法就可以把数据放到ContentValues对象中，然后插入到表中去。具体实现如下：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

ContentValues cv=new ContentValues();
cv.put(TABLE_NUM,**1**);
cv.put(TABLE_DATA,"测试数据库数据");
mSQLiteDatabase.insert(Test,null,cv);

//同样可以使用execSQL方法来执行一条“插入“的SQL语句

String INSERT_DATA="insert into 表名（列名，……） values （值，……）";
mSQLiteDatabase.execSQL(INSERT_DATA);

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


4.4.2 更新记录

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

ContentValues cv=new ContentValues();
cv.put(TABLE_NUM,3);
cv.put(TABLE_DATA,"修改后数据");
mSQLiteDatabase.update(Test,cv,"num"+"="+rowId,null);

//同样可以使用execSQL方法来执行一条“更新”的SQL语句

String UPDATE_DATA="update 表名 set 列名=xxx where xxx;
mSQLiteDatabase.execSQL(UPDATE_DATA);

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


update 表名 set 列名=xxx [where条件]


4.4.3 删除记录



```
//要删除数据可以使用delete方法
mSQLiteDatabase.delete("Test","WHERE _id="+**0**,null);

//也可以通过execSQL方法执行SQL语句删除数据
mSQLiteDatabase.execSQL("delete from 表名 where 条件");
```





4.5查询

**SELECT 列名 FROM 表名 WHERE 条件**


例如在联系人中保存3个联系人，data表如下：

![查询1](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%875.png)


查询data1值为10086的项

![查询2](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%876.png)


对查询条件和返回值稍做修改

![查询3](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%877.png)


LIKE的使用


Like经常和 %或者 _ 搭配使用。


%可与任意0个或者多个字符匹配，_可与任意单个字符匹配。

![查询4](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%878.png)



![查询5](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%879.png)


GLOB的使用


Glob与like非常相似，常与 * 搭配。

![查询6](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%8710.png)


限定和排序


关键字：order、limit、offset。


Order分为asc（默认升序），desc（降序）。

![查询7](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%8711.png)

![查询8](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%8712.png)


限定个数

![查询9](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%8713.png)


指定偏移

![查询10](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%8714.png)




内连接：通过表中两个字段进行连接，找出两个集合的交集。


  select  xxx from 表名1 inner join 表名2 on 表名1.列名 = 表名2.列名


左外连接：左表的所有项和内连接项。


  select  xxx from 表名1 left outer join 表名2 on 表名1.列名 = 表名2.列名

![查询11](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%8715.png)



![查询12](http://images.cnblogs.com/cnblogs_com/kgb250/408935/r_%E5%9B%BE%E7%89%8716.png)



**五、高级特性**


5.1 视图


　　视图是虚拟表，它的内容都派生自其它表的查询结果。虽然它看起来像基本表，但是它不是，因为基本表的内容是持久的，而视图的内容是使用时动态产生的。


　　create view 表名 as 定义


5.2 索引


　　索引是一种用来在某种条件下加速查询的结构。


　　create index 索引名 on 表名（列名）


5.3 触发器


　　触发器的作用是当具体的表发生特定的数据事件时，执行对应的操作。


　　create trigger 触发器名 [before|after] [insert|delete|update|update of columns] on 表名 action



> 
首先还谢谢是提供底稿的同事，这里只是介绍了基础的东西，感兴趣的同学，请SQLite官方网站（[http://www.sqlite.org](http://www.sqlite.org/)）了解。






> 
这里提供一个博客园的博客

[在Android中查看和管理sqlite数据库](http://www.cnblogs.com/walkingp/archive/2011/03/28/1997437.html)

具体地址：[http://www.cnblogs.com/walkingp/archive/2011/03/28/1997437.html](http://www.cnblogs.com/walkingp/archive/2011/03/28/1997437.html)



其中的评论说道：

有两点需要注明一下：
1. data/data这个目录是需要root权限的；
2. adb shell进入手机后，不是每个手机都能找到sqlite3这个命令的。至少说我们拿到的工程样机里面就没有。](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




