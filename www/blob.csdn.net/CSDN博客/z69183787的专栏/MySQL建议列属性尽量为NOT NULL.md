# MySQL建议列属性尽量为NOT NULL - z69183787的专栏 - CSDN博客
2016年11月24日 11:17:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3198
除非你有一个很特别的原因去使用 NULL 值，你应该总是让你的字段保持 NOT NULL。这看起来好像有点争议，请往下看。
首先，我们要搞清楚“空值” 和 “NULL” 的概念：
1、空值是不占用空间的
2、[MySQL](http://lib.csdn.net/base/mysql)中的NULL其实是占用空间的
所谓的NULL就是什么都没有，连\0都没有，\0在字符串中是结束符，但是在物理内存是占空间的，等于一个字节，而NULL就是连这一个字节都没有。在[数据库](http://lib.csdn.net/base/mysql)里是严格区分的，任何数跟NULL进行运算都是NULL,
 判断值是否等于NULL，不能简单用=，而要用IS NULL关键字。
数据库的字段ID设为NOT NULL, 仅仅说明该字段不能为NULL, 也就是说只有在
INSERT INTO table(ID) VALUES(NULL);
这种情况下数据库会报错，而
INSERT INTO table(ID) VALUES( ‘ ‘);
这不能说明是NULL, 数据库系统会根据ID设的缺省值填充，或者如果是自增字段就自动加一等缺省操作。
尽量避免NULL：应该指定列为NOT NULL，除非你想存储NULL。在MySQL中，含有空值的列很难进行查询优化，而且对表索引时不会存储NULL值的，所以如果索引的字段可以为NULL，索引的效率会下降很多。因为它们使得索引、索引的统计信息以及比较运算更加复杂。你应该用0、一个特殊的值或者一个空串代替空值。
从网上学习的时候一直看到，[MySQL](http://lib.csdn.net/base/mysql)列建议列是非null的。说是如果是允许null的列，对索引会有影响（索引不会包括有NULL值）。
原文如下：
尽量避免NULL：应该指定列为NOT NULL，除非你想存储NULL。在MySQL中，含有空值的列很难进行查询优化。
因为它们使得索引、索引的统计信息以及比较运算更加复杂。你应该用0、一个特殊的值或者一个空串代替空值。
--------------------------------------------------
null会影响索引的统计，一般会默认null为同一个值，这样这个索引的筛选价值就降低了，影响优化器的判断。当然也可以调整参数，使得null被认为是不同的值。
-----------------------------------------------
怎么理解呢？
---------------------------------------------
先看空值跟null的区别。mysql官方：
“NULL columns require additional space in the row to record whether their values are NULL. For MyISAM
 tables, each NULL column takes one bit extra, rounded up to the nearest byte.”
Mysql难以优化引用可空列查询，它会使索引、索引统计和值更加复杂。可空列需要更多的存储空间（见上面的e文），还需要mysql内部进行特殊处理。可空列被索引后，每条记录都需要一个额外的字节，还能导致MYisam 中固定大小的索引变成可变大小的索引。这也是《高性能mysql第二版》介绍的。
