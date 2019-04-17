# MySQL [Err] 1055 - Expression #1 of ORDER BY clause is not in GROUP BY clause - DoubleLi - 博客园






#### MySQL[Err]1055
- 上次MySQL5.7.19主从建立完成之后，所有的测试都是在MySQL命令行下进行的，最近用Navicat Premium进行MySQL的连接，然后在插入数据的时候MySQL突然报错了，可是我的数据插入成功了，感觉很奇怪！！
报错内容如下:

```
[SQL]INSERT INTO biao(ziduan2) VALUES(8888888);
受影响的行: 1
时间: 0.012s

[Err] 1055 - Expression #1 of ORDER BY clause is not in GROUP BY clause and contains nonaggregated column 'information_schema.PROFILING.SEQ' which is not functionally dependent on columns in GROUP BY clause; this is incompatible with sql_mode=only_full_group_by
```
- 在知乎上找到了一个答案，貌似可以解决问题

```
在my.cnf 里面设置sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION'在sql_mode 中去掉only_full_group_by 解决问题！

PS:可是依然不知道sql_mode是什么？
```

#### 这里我们先看一下MySQL5.6和MySQL5.7默认的sql模式

```
# MySQL 5.6 Default SQL_MODE;
mysql> select @@sql_mode;
+--------------------------------------------+
| @@sql_mode                                 |
+--------------------------------------------+
| STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION |
+--------------------------------------------+
1 row in set (0.00 sec)
 
# MySQL 5.7 Default SQL_MODE;
mysql> select @@sql_mode;
+-------------------------------------------------------------------------------------------------------------------------------------------+
| @@sql_mode                                                                                                                                |
+-------------------------------------------------------------------------------------------------------------------------------------------+
| ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION |
+-------------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0.00 sec)
```

#### mysql的sql_mode合理设置
- mysql的sql_mode合理设置

sql_mode是个很容易被忽视的变量，默认值是空值，在这种设置下是可以允许一些非法操作的，比如允许一些非法数据的插入。在生产环境必须将这个值设置为严格模式，所以开发、测试环境的数据库也必须要设置，这样在开发测试阶段就可以发现问题

#### sql_mode常用值如下：
- ONLY_FULL_GROUP_BY

对于GROUP BY聚合操作，如果在SELECT中的列，没有在GROUP BY中出现，那么这个SQL是不合法的，因为列不在GROUP BY从句中。
在严格模式下，不要让GROUP BY部分中的查询指向未选择的列，否则报错。
- NO_AUTO_VALUE_ON_ZERO

该值影响自增长列的插入。默认设置下，插入0或NULL代表生成下一个自增长值。如果用户 希望插入的值为0，而该列又是自增长的，那么这个选项就有用了。
- STRICT_TRANS_TABLES

在该模式下，如果一个值不能插入到一个事务表中，则中断当前的操作，对非事务表不做限制
- NO_ZERO_IN_DATE

在严格模式下，不允许日期和月份为零
- NO_ZERO_DATE

设置该值，mysql数据库不允许插入零日期，插入零日期会抛出错误而不是警告。
- ERROR_FOR_DIVISION_BY_ZERO

在INSERT或UPDATE过程中，如果数据被零除，则产生错误而非警告。如 果未给出该模式，那么数据被零除时MySQL返回NULL
- NO_AUTO_CREATE_USER

禁止GRANT创建密码为空的用户
- NO_ENGINE_SUBSTITUTION

如果需要的存储引擎被禁用或未编译，那么抛出错误。不设置此值时，用默认的存储引擎替代，并抛出一个异常
- PIPES_AS_CONCAT

将"||"视为字符串的连接操作符而非或运算符，这和Oracle数据库是一样的，也和字符串的拼接函数Concat相类似
- ANSI_QUOTES

启用ANSI_QUOTES后，不能用双引号来引用字符串，因为它被解释为识别符

> 
ORACLE的sql_mode设置等同：PIPES_AS_CONCAT, ANSI_QUOTES, IGNORE_SPACE, NO_KEY_OPTIONS, NO_TABLE_OPTIONS, NO_FIELD_OPTIONS, NO_AUTO_CREATE_USER.


#### 如果使用mysql，为了继续保留大家使用oracle的习惯，可以对mysql的sql_mode设置如下：

```
在my.cnf添加如下配置
[mysqld]
sql_mode='ONLY_FULL_GROUP_BY,NO_AUTO_VALUE_ON_ZERO,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,

ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION,PIPES_AS_CONCAT,ANSI_QUOTES'
```

PS:参考自：http://www.cnblogs.com/grimm/p/5752259.html，http://www.ywnds.com/?p=8865









