# （useAffectedRows）Mysql / Mybatis update时，字段不变返回值的问题 - z69183787的专栏 - CSDN博客
2016年11月12日 23:40:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5244
（
mysql
当执行一句update语句，但修改内容与原数据一致时，mysql本神返回的  受影响行数为0 ，在 控制台或者 客户端如navicat中都可看到
但jdbc/mybatis 返回的却是1，返回的是 sql语句 的匹配行数。
oracle直接返回1条记录被更新。
所以很明显，不同数据update的机制是不一样的。
但是针对标准SQL，不涉及具体数据库，update t_order set order_mark = 2 where
 order_id=1555肯定是会操作一条记录，所以你可以理解通过jdbc执行update返回的是操作记录数，而不是影响记录数
使用spring，mbatis更新数据时如果数据前后相同，会返回1
在使用客户端时，如果数据相同，返回值为0
返回不一样，不知道为什么会是这样？
找到原因了
Found MySQL specific workaround. If someone is curious - useAffectedRows option for jdbc url, e.g. 
jdbc:mysql://${jdbc.host}/${jdbc.db}?useAffectedRows=true
useAffectedRows的含义 ：是否用受影响的行数替代查找到的行数来返回数据，也就是查找到了 但却不一定真正修改了
JDBC默认返回的是符合的行数Rows matched， 如果想返回修改过（ Changed）的行数  ，需要使用useAffectedRows参数
附上该参数修改方式：
遇到MySQL中on duplicate key update语句返回值不正确：
在server5.1.*的返回分别为insert=1,update=3,nochange=2
在server5.5.*的返回分别为insert=1,update=2,nochange=1
应返回：insert=1,update=2,nochange=0
使用mysql-connector-java-*.jar不当导致。修正为：jdbc:mysql://ip:port/db?**useAffectedRows**=true&…
useAffectedRows的含义 ：是否用受影响的行数替代查找到的行数来返回数据，也就是查找到了 但却不一定真正修改了
其他常见常用参数：
UsePerformanceMonitor，userperfmon, perfmon：是否启用性能监视，默认 false
IgnorePrepare：    是否忽略 Prepare() 调用，默认 true
UseProcedureBodies,procedure bodies：是否检查存储过程体、参数的有效性，默认 true
AutoEnlist：    是否自动使用活动的连接，默认 true
TreatTinyAsBoolean：是否将 TINYINT(1) 列视为布尔型，默认 true
AllowUserVariables：是否允许 SQL 中出现用户变量，默认 false
FunctionsReturnString：所有服务器函数是否按返回字符串处理，默认 false
UseAffectedRows：是否用受影响的行数替代查找到的行数来返回数据，默认 false
Keepalive：    保持 TCP 连接的秒数，默认0，不保持。
ConnectionLifeTime：连接被销毁前在连接池中保持的最少时间（秒）。默认 0
Pooling：    是否使用线程池，默认 true
MinimumPoolSize, min pool size：线程池中允许的最少线程数，默认 0
MaximumPoolSize，max pool size：线程池中允许的最多线程数，默认 100
ConnectionReset：连接过期后是否自动复位，默认 false
CharacterSet, charset：向服务器请求连接所使用的字符集，默认：无
当执行一句update语句，但修改内容与原数据一致时，mysql本神返回的  受影响行数为0 ，在 控制台或者 客户端如navicat中都可看到
但jdbc/mybatis 返回的却是1，返回的是 sql语句 的匹配行数。
