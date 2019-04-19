# python sqlite 数据库_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
SQLite是一种嵌入式数据库，它的数据库就是一个文件。由于SQLite本身是C写的，而且体积很小，所以，经常被集成到各种应用程序中，甚至在iOS和Android的App中都可以集成。
Python就内置了SQLite3，所以，在Python中使用SQLite，不需要安装任何东西，直接使用。
在使用SQLite前，我们先要搞清楚几个概念：
表是数据库中存放关系数据的集合，一个数据库里面通常都包含多个表，比如学生的表，班级的表，学校的表，等等。表和表之间通过外键关联。
要操作关系数据库，首先需要连接到数据库，一个数据库连接称为Connection；
连接到数据库后，需要打开游标，称之为Cursor，通过Cursor执行SQL语句，然后，获得执行结果。
Python定义了一套操作数据库的API接口，任何数据库要连接到Python，只需要提供符合Python标准的数据库驱动即可。
由于SQLite的驱动内置在Python标准库中，所以我们可以直接来操作SQLite数据库。
使用Python的DB-API时，只要搞清楚Connection和Cursor对象，打开后一定记得关闭，就可以放心地使用。
使用Cursor对象执行`insert`，`update`，`delete`语句时，执行结果由`rowcount`返回影响的行数，就可以拿到执行结果。
使用Cursor对象执行`select`语句时，通过`featchall()`可以拿到结果集。结果集是一个list，每个元素都是一个tuple，对应一行记录。
如果SQL语句带有参数，那么需要把参数按照位置传递给`execute()`方法，有几个`?`占位符就必须对应几个参数，
