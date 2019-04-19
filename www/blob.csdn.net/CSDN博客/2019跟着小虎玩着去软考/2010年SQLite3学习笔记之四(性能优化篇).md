# 2010年SQLite3学习笔记之四(性能优化篇) - 2019跟着小虎玩着去软考 - CSDN博客
2010年03月24日 16:31:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：6986
**SQLite3****性能优化**
**2010年SQLite3学习笔记之四(性能优化篇)**
**引言：**
**[小老虎多多](http://blog.csdn.net/littletigerat)****的SQLite3性能优化的学习笔记。**
SQLite3性能调整主要通过pragma指令来实现。
比如调整：空间释放、磁盘同步、Cache大小等。
**一.空间释放**
**1.如何查询：**
PRAGMA auto_vacuum; 
含义：查询数据库的auto-vacuum标记。
**2.标记含义：**
auto-vacuum标记的含义：
正常情况下，当提交一个从数据库中删除数据的事务时，数据库文件不改变大小。未使用的文件页被标记并在以后的添加操作中再次使用。这种情况下使用VACUUM命令释放删除得到的空间。但是Vacuum的效率非常低！
**3.如何设置：**
PRAGMA auto_vacuum = 0 | 1;
当开启auto-vacuum，也就是执行pragma auto_vacuum=1;
当提交一个从数据库中删除数据的事务时，数据库文件自动收缩， (VACUUM命令在auto-vacuum开启的数据库中不起作用)。数据库会在内部存储一些信息以便支持这一功能，这使得数据库文件比不开启该选项时稍微大一些。
**4.注意事项：**
只有在数据库中未建任何表时才能改变auto-vacuum标记。试图在已有表的情况下修改不会导致报错。
**5.建议：**
不要打开。也就是“PRAGMA auto_vacuum = 0；”执行命令。
**二.缓存大小**
**1.如何查询：**
PRAGMA cache_size; 
查询SQLite一次存储在内存中的数据库文件页数。
**2.标记含义：**
每页使用约1.5K内存，缺省的缓存大小是2000. 若需要使用改变大量多行的UPDATE或DELETE命令，并且不介意SQLite使用更多的内存的话，可以增大缓存以提高性能。
**3.如何设置**
PRAGMA cache_size = Number-of-pages;
修改SQLite一次存储在内存中的数据库文件页数。
**4.注意事项：**
当使用cache_size pragma改变缓存大小时，改变仅对当前对话有效，当数据库关闭重新打开时缓存大小恢复到缺省大小。要想永久改变缓存大小，使用default_cache_size pragma.
**5.建议：**
修改为8000，也就是执行命令“PRAGMA cache_size =8000；”即可；
**三.LIKE运算符**
PRAGMA case_sensitive_like; 
PRAGMA case_sensitive_like = 0 | 1;
**标记含义：**
LIKE运算符的缺省行为是忽略latin1字符的大小写。因此在缺省情况下'a' LIKE 'A'的值为真。可以通过打开 case_sensitive_like pragma来改变这一缺省行为。当启用case_sensitive_like，'a' LIKE 'A'为假而 'a' LIKE 'a'依然为真。
**1.注意事项：**
**SQLite3.6.22版本不支持。**
**2.建议：**
打开。也就是执行命令“PRAGMA case_sensitive_like = 1;”命令。不然搜索中文字串会出错。
**四.LIKE运算符**
PRAGMA count_changes; 
PRAGMA count_changes = 0 | 1;
查询或更改count-changes标记。
正常情况下INSERT, UPDATE和DELETE语句不返回数据。
当开启count-changes，以上语句返回一行含一个整数值的数据——该语句插入，修改或删除的行数。
**1.注意事项：**
返回的行数不包括由触发器产生的插入，修改或删除等改变的行数。
**2.建议：**
打开，便于调试。也就是执行“PRAGMA count_changes = 1；”该命令。
**五.页面大小**
PRAGMA page_size; 
PRAGMA page_size = bytes;
查询或设置page-size值。
**1.注意事项：**
只有在未创建数据库时才能设置page-size。页面大小必须是2的整数倍且大于等于512小于等于8192。
上限可以通过在编译时修改宏定义SQLITE_MAX_PAGE_SIZE的值来改变。上限的上限是32768。
**六****.****磁盘同步**
**1.如何查询：**
PRAGMA synchronous; 
**指令含义**：查询"synchronous"标记的设定，返回整数值；
**2.如何设置：**
PRAGMA synchronous = FULL; (2) 
PRAGMA synchronous = NORMAL; (1) 
PRAGMA synchronous = OFF; (0)
**3.参数含义：**
当synchronous设置为FULL (2), SQLite数据库引擎在紧急时刻会暂停以确定数据已经写入磁盘。这使系统崩溃或电源出问题时能确保数据库在重起后不会损坏。FULL synchronous很安全但很慢。
当synchronous设置为NORMAL, SQLite数据库引擎在大部分紧急时刻会暂停，但不像FULL模式下那么频繁。 NORMAL模式下有很小的几率(但不是不存在)发生电源故障导致数据库损坏的情况。但实际上，在这种情况下很可能你的硬盘已经不能使用，或者发生了其他的不可恢复的硬件错误。
设置为synchronous OFF (0)时，SQLite在传递数据给系统以后直接继续而不暂停。若运行SQLite的应用程序崩溃，数据不会损伤，但在系统崩溃或写入数据时意外断电的情况下数据库可能会损坏。另一方面，在synchronous OFF时一些操作可能会快50倍甚至更多。
在SQLite 2中，缺省值为NORMAL.而在3中修改为FULL。
**4.建议：**
如果有定期备份的机制，而且少量数据丢失可接受，用OFF。
**七****.****内存模式**
**1.如何查询：**
PRAGMA temp_store; 
**指令含义**：查询"temp_store"参数的设置；
**2.如何设置：**
PRAGMA temp_store = DEFAULT; (0) 
PRAGMA temp_store = FILE; (1) 
PRAGMA temp_store = MEMORY; (2)
**3.参数含义：**
当temp_store设置为DEFAULT (0),使用编译时的C预处理宏 TEMP_STORE来定义储存临时表和临时索引的位置。
当设置为FILE (1)，则存放于文件中。temp_store_directorypragma 可用于指定存放该文件的目录。
当设置为MEMORY (2)，临时表和索引则存放于内存中。
**4.注意事项：**
当改变temp_store设置，所有已存在的临时表，索引，触发器及视图将被立即删除。
**5.建议：**
使用2，即内存模式。
**附指令表集：**
|**序号**|**指令**|**含义**|**缺省值**|
|----|----|----|----|
|**1**|auto_vacuum|**空间释放**|**0**|
|**2**|cache_size|**缓存大小**|**2000**|
|**3**|case_sensitive_like|**LIKE****大小写敏感**|**（注意：SQLite3.6.22****不支持）**|
|**4**|count_changes|**变更行数**|**0**|
|**5**|page_size|**页面大小**|**1024**|
|**6**|synchronous|**硬盘大小**|**2**|
|**7**|temp_store;|**内存模式**|**0**|
**（End.****）**
