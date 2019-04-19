# mysql中insert into select from的使用 - 数据之美的博客 - CSDN博客
2017年06月06日 15:14:09[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：313
个人分类：[大数据](https://blog.csdn.net/oppo62258801/article/category/6768103)
如何在[MySQL](http://lib.csdn.net/base/mysql)从多个表中组合字段然后插入到一个新表中，通过一条sql语句实现。具体情形是：有三张表a、b、c，现在需要从表b和表c中分别查几个字段的值插入到表a中对应的字段。对于这种情况，我们可以使用如下的语句来实现：
1.**INSERT****INTO** db1_name(field1,field2)**SELECT** field1,field2 **FROM** db2_name
当然，上面的语句比较适合两个表的数据互插，如果多个表就不适应了。对于多个表，我们可以先将需要查询的字段join起来，然后组成一个视图后再select
 from就可以了：
1.**INSERT****INTO** a(field1,field2)**SELECT*****FROM**(**SELECT** f1,f2 **FROM** b **JOIN** c)**AS** tb
其中f1是表b的字段，f2是表c的字段，通过join查询就将分别来自表b和表c的字段进行了组合，然后再通过select嵌套查询插入到表a中，这样就满足了我们这个场景了，如果需要不止2个表，那么可以多个join的形式来组合字段。需要注意的是嵌套查询部分最后一定要有设置表别名，如下：
1.**SELECT*****FROM**(**SELECT** f1,f2 **FROM** b **JOIN** c)**AS** tb
即最后的**as tb**是必须的（当然tb这个名称可以随意取），即指定一个别名，否则在mysql中会报如下错误：
1.ERROR 1248(42000): Every derived **TABLE** must have its own alias
即每个派生出来的新表都必须指定别名才可以的。
