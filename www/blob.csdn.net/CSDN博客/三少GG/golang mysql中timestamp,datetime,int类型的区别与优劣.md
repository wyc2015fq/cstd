# golang mysql中timestamp,datetime,int类型的区别与优劣 - 三少GG - CSDN博客
2016年01月02日 22:51:18[三少GG](https://me.csdn.net/scut1135)阅读数：3651
golang time:  [https://golang.org/pkg/time/](https://golang.org/pkg/time/)
[golang中time包用法](http://blog.csdn.net/chenbaoke/article/details/41519193) http://blog.csdn.net/chenbaoke/article/details/41519193
首先 DATETIM和TIMESTAMP类型所占的存储空间不同，前者8个字节，后者4个字节，这样造成的后果是两者能表示的时间范围不同。前者范围为1000-01-01 00:00:00 ~ 9999-12-31 23:59:59，后者范围为1970-01-01 08:00:01到2038-01-19 11:14:07。所以可以看到TIMESTAMP支持的范围比DATATIME要小,容易出现超出的情况.
其次，TIMESTAMP类型在默认情况下，insert、update 数据时，TIMESTAMP列会自动以当前时间（CURRENT_TIMESTAMP）填充/更新。
第三，TIMESTAMP比较受时区timezone的影响以及MYSQL版本和服务器的SQL MODE的影响
所以一般来说，我比较倾向选择DATETIME，至于你说到索引的问题，选择DATETIME作为索引，如果碰到大量数据查询慢的情况，也可以分区表解决。
其实速度上差别不是很大，你可以自己做做测试就知道了。内部存储都是整数，只不过datetime和timestamp会仅仅在显示的时候，显示为人能读的日期（当然存储空间有点区别，整形和timestamp都是4字节，datetime是8字节），做索引也应该没什么区别，这个不敢确定，如果有请指正我……
另外datetime和timestamp相对于int来说也有一个小小的好处，就是对于时间类型来说，可以有一系列的时间函数可以用
Q:Mysql的时间字段貌似有各种选择，像一般的情况（我也是）下就是用`INT`型来表示，直接存储时间戳。但是我知道在Mysql里至少还有`TIMESTAMP`和`DATETIME`型可以用来存储时间，我不知道这三者在使用上各有什么区别，在使用场景上需要怎么考虑，特别是它们在索引或者查询速度上有区别吗？
这几个类型的选择还是看你的需求。
我用timestamp比较多，对于记录日志什么的需求，timestamp绝对够用了，除非你保证说你的程序能一直用到2038年，就算如此也可以用迁移程序处理……
如果需求是允许用户保存一些超过timestamp能保存的时间（@QingchaoWu 已经给出了timestamp的范围）比如说todo list什么的，允许用户计划38年以后的事情，那就用datetime好了。
##################################
转载请注明来自[souldak](http://blog.csdn.net/souldak)，微博:[@evagle](http://weibo.com/souldak)
以下内容**整合筛选**自互联网：
int
1. 占用4个字节
2. 建立索引之后，查询速度快
3. 条件范围搜索可以使用使用between
4. 不能使用mysql提供的时间函数
结论：适合需要进行大量时间范围查询的数据表
datetime
1. 占用8个字节
2. 允许为空值，可以自定义值，系统不会自动修改其值。
3. 实际格式储存（Just stores what you have stored and retrieves the same thing which you have stored.）
4. 与时区无关（It has nothing to deal with the TIMEZONE and Conversion.）
5. 不可以设定默认值，所以在不允许为空值的情况下，必须手动指定datetime字段的值才可以成功插入数据。
6. 可以在指定datetime字段的值的时候使用now()变量来自动插入系统的当前时间。
结论：datetime类型适合用来记录数据的原始的创建时间，因为无论你怎么更改记录中其他字段的值，datetime字段的值都不会改变，除非你手动更改它。
timestamp
1. 占用4个字节
2. 允许为空值，但是不可以自定义值，所以为空值时没有任何意义。
3. TIMESTAMP值不能早于1970或晚于2037。这说明一个日期，例如'1968-01-01'，虽然对于DATETIME或DATE值是有效的，但对于TIMESTAMP值却无效，如果分配给这样一个对象将被转换为0。
4.值以UTC格式保存（ it stores the number of milliseconds）
5.时区转化 ，存储时对当前的时区进行转换，检索时再转换回当前的时区。
6. 默认值为CURRENT_TIMESTAMP()，其实也就是当前的系统时间。
7. 数据库会自动修改其值，所以在插入记录时不需要指定timestamp字段的名称和timestamp字段的值，你只需要在设计表的时候添加一个timestamp字段即可，插入后该字段的值会自动变为当前系统时间。
8. 以后任何时间修改表中的记录时，对应记录的timestamp值会自动被更新为当前的系统时间。
结论：timestamp类型适合用来记录数据的最后修改时间，因为只要你更改了记录中其他字段的值，timestamp字段的值都会被自动更新。
