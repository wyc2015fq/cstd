# 如何在高并发分布式系统中生成全局唯一Id - z69183787的专栏 - CSDN博客
2017年03月13日 15:28:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11460
[http://www.cnblogs.com/lsx1993/p/4663125.html](http://www.cnblogs.com/lsx1993/p/4663125.html)
# 《分布式环境下数据库主键方案》
[ http://www.2cto.com/database/201309/243195.html ]
在只使用单数据库时，使用自增主键ID无疑是最适合的。但在集群、主从架构上时就会有一些问题，比如：主键的全局唯一。
集群环境下除了自增ID外的其它创建主键的方案
1、通过应用程序生成一个GUID，然后和数据一起插入切分后的集群。
优点是维护简单，实现也容易。缺点是应用的计算成本较大，且GUID的长度比较长，占用数据库存储空间较大，涉及到应用的开发。
说明：主要优势是简单，缺点是浪费存储空间。
2、通过独立的应用程序事先在数据库中生成一系列唯一的 ID，各应用程序通过接口或者自己去读取再和数据一起插入到切分后的集群中。
优点是全局唯一主键简单，维护相对容易。
缺点是实现复杂，需要应用开发。
说明：ID表要频繁查和频繁更新，插入数据时，影响性能。
3、通过【中心数据库服务器】利用数据库自身的自增类型（如 MySQL的 auto_increment 字段），或者自增对象（如 Oracle 的 Sequence）等先生成一个唯一 ID 再和数据一起插入切分后的集群。优点是？好像没有特别明显的优点。缺点是实现较为复杂，且整体可用性维系在这个中心数据库服务器上，一旦这里crash 了，所有的集群都无法进行插入操作，涉及到应用开发。
说明：不推荐。
4、通过集群编号加集群内的自增（auto_increment类型）【两个字段】共同组成唯一主键。优点是实现简单，维护也比较简单，对应用透明。
缺点是引用关联操作相对比较复杂，需要两个字段，主键占用空间较大，在使用 InnoDB 的时候这一点的副作用很明显。
说明：虽然是两个字段，但是这方式存储空间最小，仅仅多了一个smallint两个字节。
5、通过设置每个集群中自增 ID 起始点（auto_increment_offset），将各个集群的ID进行绝对的分段来实现全局唯一。当遇到某个集群数据增长过快后，通过命令调整下一个 ID 起始位置跳过可能存在的冲突。优点是实现简单，且比较容易根据 ID 大小直接判断出数据处在哪个集群，对应用透明。缺点是维护相对较复杂，需要高度关注各个集群 ID 增长状况。
说明：段满了，调整太麻烦。
6、通过设置每个集群中自增 ID 起始点（auto_increment_offset）以及 ID 自增步长（auto_increment_increment），让目前每个集群的起始点错开 1，步长选择大于将来基本不可能达到的切分集群数，达到将 ID 相对分段的效果来满足全局唯一的效果。优点是实现简单，后期维护简单，对应用透明。缺点是第一次设置相对较为复杂。
说明：避免重合需要多种方案结合
【使用UUID作为主键带来的问题】
对于InnoDB这种聚集主键类型的引擎来说，数据会按照主键进行排序，由于UUID的无序性，InnoDB会产生巨大的IO压力，此时不适合使用UUID做物理主键，可以把它作为逻辑主键，物理主键依然使用自增ID。
首先，innodb会对主键进行物理排序，这对auto_increment_int是个好消息，因为后一次插入的主键位置总是在最后。但是对uuid来 说，这却是个坏消息，因为uuid是杂乱无章的，每次插入的主键位置是不确定的，可能在开头，也可能在中间，在进行主键物理排序的时候，势必会造成大量的 IO操作影响效率。
这个问题解决起来办法不多，比较常见的方式是主键仍然用auto_increment_int来做，而另加一个uuid做唯一索引，表外键关联什么的，还 用uuid来做，也就是说auto_increment_int只是一个形式上的主键，而uuid才是事实上的主键，这样，一方面int主键不会浪费太多空间，另一方面，还可以继续使用uuid。
1、最简单的方法
4台数据库，第一台mysql主键从1开始每次加4，第二台从2开始每次加4，以此类推。。
2、搭建sequence server
2.1、选用N台mysql作为sequence server，防止单点故障。
2.2、每个server上的每张表都代表一个序列，每张表也只有一条记录（表级锁，选用myisam引擎）。
2.3、第一台server的序列从1开始每次加N，第二台从2开始每次加N。
2.4、获取时先从第一台server上获取nextVal并修改nextVal加N，如果第一台Server获取失败，则从第二台Server上获取。。
修改MySQL【默认自动增长的步长】
set global auto_increment_increment=1; — 设置序列的增长值
show global variables; — 显示所有的global变量
show global variables like ‘%test%’ — 查询包含test字符串的global变量
# 《如何在高并发分布式系统中生成全局唯一Id》
使用数据库自增Id
优势：编码简单，无需考虑记录唯一标识的问题。
缺陷：
1) 在大表做水平分表时，就不能使用自增Id，因为Insert的记录插入到哪个分表依分表规则判定决定，若是自增Id，各个分表中Id就会重复，在做查询、删除时就会有异常。
2) 在对表进行高并发单记录插入时需要加入事物机制，否则会出现Id重复的问题。
3) 在业务上操作父、子表（即关联表）插入时，需要在插入数据库之前获取max(id)用于标识父表和子表关系，若存在并发获取max(id)的情况，max(id)会同时被别的线程获取到。
4) 等等。
结论：适合小应用，无需分表，没有高并发性能要求。
单独开一个数据库，获取全局唯一的自增序列号或各表的MaxId
1) 使用自增序列号表
专门一个数据库，生成序列号。开启事物，每次操作插入时，先将数据插入到序列表并返回自增序列号用于做为唯一Id进行业务数据插入。
注意：需要定期清理序列表的数据以保证获取序列号的效率；插入序列表记录时要开启事物。
使用此方案的问题是：每次的查询序列号是一个性能损耗；如果这个序列号列暴了，那就杯具了，你不知道哪个表使用了哪个序列，所以就必须换另一种唯一Id方式如GUID。
2) 使用MaxId表存储各表的MaxId值
专门一个数据库，记录各个表的MaxId值，建一个存储过程来取Id，逻辑大致为：开启事务，对于在表中不存在记录，直接返回一个默认值为1的键值，同时插入该条记录到table_key表中。而对于已存在的记录，key值直接在原来的key基础上加1更新到MaxId表中并返回key。
使用此方案的问题是：每次的查询MaxId是一个性能损耗；不过不会像自增序列表那么容易列暴掉，因为是摆表进行划分的。
详细可参考：[《使用MaxId表存储各表的MaxId值，以获取全局唯一Id》](http://www.cnblogs.com/repository/archive/2011/01/17/1937265.html)
我截取此文中的sql语法如下：
`第一步：创建表`
` `
```
```
create
 table table_key
```
`(`
`table_name   ``varchar`
```
(50)
 notnullprimarykey,
```
```
key_value   
 intnotnull
```
`)`
```
`第二步：创建存储过程来取自增ID`
` `
```
`createprocedureup_get_table_key`
`(`
`@table_name     ``varchar``(50),`
```
@key_value     
 intoutput
```
`)`
`as`
`begin`
`begintran`
`declare``@keyint`
```
--initialize
 the key with 1
```
`set``@``key``=1`
```
--whether
 the specified table is exist
```
```
if
 notexists(selecttable_name fromtable_key wheretable_name=@table_name)
```
`begin`
`insertintotable_key ``values``(@table_name,@``key``)        `
```
--default
 key vlaue:1
```
`end`
```
--
 step increase
```
`else`
`begin`
`select``@``key`
```
=key_value
 fromtable_key
```
`with`
```
(nolock)
 wheretable_name=@table_name
```
`set``@``key``=@``key``+1`
```
--update
 the key value by table name
```
```
updatetable_key
 setkey_value=@keywheretable_name=@table_name
```
`end`
```
--set
 ouput value
```
`set``@key_value=@``key`
```
--commit
 tran
```
`committran`
```
if
 @@error>0
```
`rollbacktran`
`end`
```
感谢园友的好建议：
1. （[@辉_辉](http://home.cnblogs.com/u/hui-hui/)）建议给table_key中为每个表初始化一条key为1的记录，这样就不用每次if来判断了。
2. （[@乐活的CodeMonkey](http://home.cnblogs.com/cwc-lehuo/)）建议给存储过程中[数据库事物隔离级别](http://www.blogjava.net/hnicypb/archive/2008/10/11/233731.html)提高一下，因为出现在CS代码层上使用如下事物代码会导致并发重复问题.
` `
```
```
TransactionOptions
 option = newTransactionOptions();
```
```
option.IsolationLevel
 = IsolationLevel.ReadUncommitted;
```
```
option.Timeout
 = newTimeSpan(0, 10, 0);
```
`using`
```
(TransactionScope
 transaction = newTransactionScope(TransactionScopeOption.RequiresNew, option))
```
`{`
`//调用存储过程`
`}`
```
　　在咨询过DBA后，这个存储过程提高数据库隔离级别会加大数据库访问压力，导致响应超时问题。所以这个建议我们只能在代码编写宣导上做。
3. （[@土豆烤肉](http://home.cnblogs.com/u/bagegejin/)）存储过程中不使用事物，一旦使用到事物性能就急剧下滑。直接使用UPDATE获取到的更新锁，即SQL
 SERVER会保证UPDATE的顺序执行。（已在用户过千万的并发系统中使用）
` `
```
`createprocedure[dbo].[up_get_table_key]`
`(`
`@table_name     ``varchar``(50),`
```
@key_value     
 intoutput
```
`)`
`as`
`begin`
`SETNOCOUNT ``ON``;`
`DECLARE``@maxId ``INT`
`UPDATEtable_key`
`SET`
```
@maxId
 = key_value,key_value = key_value + 1
```
`WHEREtable_name=@table_name`
`SELECT``@maxId`
`end`
```
结论：适用中型应用，此方案解决了分表，关联表插入记录的问题。但是无法满足高并发性能要求。同时也存在单点问题，如果这个数据库cash掉的话……
　　我们目前正头痛这个问题，因为我们的高并发常常出现数据库访问超时，瓶颈就在这个MaxId表。我们也有考虑使用分布式缓存（eg：memcached）缓存第一次访问MaxId表数据，以提高再次访问速度，并定时用缓存数据更新一次MaxId表，但我们担心的问题是：
　　a) 倘若缓存失效或暴掉了，那缓存的MaxId没有更新到数据库导致数据丢失，必须停掉站点来执行Select max(id)各个表来同步MaxId表。
　　b) 分布式缓存不是一保存下去，其他服务器上就立马可以获取到的，即数据存在不确定性。（其实也是缓存的一个误用，缓存应该用来存的是频繁访问并且很少改动的内容）
改进方案：
　　整体思想：建立两台以上的数据库ID生成服务器，每个服务器都有一张记录各表当前ID的MaxId表，但是MaxId表中Id的增长步长是服务器的数量，起始值依次错开，这样相当于把ID的生成散列到每个服务器节点上。例如：如果我们设置两台数据库ID生成服务器，那么就让一台的MaxId表的Id起始值为1（或当前最大Id+1），每次增长步长为2，另一台的MaxId表的ID起始值为2（或当前最大Id+2），每次步长也为2。这样就将产生ID的压力均匀分散到两台服务器上，同时配合应用程序控制，当一个服务器失效后，系统能自动切换到另一个服务器上获取ID，从而解决的单点问题保证了系统的容错。（Flickr思想）
　　但是要注意：1、多服务器就必须面临负载均衡的问题；2、倘若添加新节点，需要对原有数据重新根据步长计算迁移数据。
结论：适合大型应用，生成Id较短，友好性比较好。（强烈推荐）
3、 Sequence特性
　　这个特性在SQL Server 2012、Oracle中可用。这个特性是数据库级别的，允许在多个表之间共享序列号。它可以解决分表在同一个数据库的情况，但倘若分表放在不同数据库，那将共享不到此序列号。（eg：Sequence使用场景：你需要在多个表之间公用一个流水号。以往的做法是额外建立一个表，然后存储流水号）
相关Sequence特性资料：
[SQL Server2012中的SequenceNumber尝试](http://www.cnblogs.com/CareySon/archive/2012/03/12/2391581.html)
[SQL Server 2012 开发新功能——序列对象（Sequence）](http://www.cnblogs.com/zhangyoushugz/archive/2012/11/09/2762720.html)
[identity和sequence的区别](http://www.cnblogs.com/langtianya/archive/2013/05/08/3068020.html)
[Difference between Identity and Sequence in SQL Server 2012](http://raresql.com/2012/05/01/difference-between-identity-and-sequence/)
结论：适用中型应用，此方案不能完全解决分表问题，而且无法满足高并发性能要求。同时也存在单点问题，如果这个数据库cash掉的话……
4、 通过数据库集群编号+集群内的自增类型两个字段共同组成唯一主键
优点：实现简单，维护也比较简单。
缺点：关联表操作相对比较复杂，需要两个字段。并且业务逻辑必须是一开始就设计为处理复合主键的逻辑，倘若是到了后期，由单主键转为复合主键那改动成本就太大了。
结论：适合大型应用，但需要业务逻辑配合处理复合主键。
5、 通过设置每个集群中自增 ID 起始点（auto_increment_offset），将各个集群的ID进行绝对的分段来实现全局唯一。当遇到某个集群数据增长过快后，通过命令调整下一个 ID 起始位置跳过可能存在的冲突。
优点：实现简单，且比较容易根据 ID 大小直接判断出数据处在哪个集群，对应用透明。缺点：维护相对较复杂，需要高度关注各个集群 ID 增长状况。
结论：适合大型应用，但需要高度关注各个集群 ID 增长状况。
6、 [GUID（Globally Unique Identifier，全局唯一标识符）](http://zh.wikipedia.org/zh/%E5%85%A8%E5%B1%80%E5%94%AF%E4%B8%80%E6%A0%87%E8%AF%86%E7%AC%A6)
GUID通常表示成32个16进制数字（0－9，A－F）组成的字符串，如：{21EC2020-3AEA-1069-A2DD-08002B30309D}，它实质上是一个128位长的二进制整数。
GUID制定的算法中使用到用户的网卡MAC地址，以保证在计算机集群中生成唯一GUID；在相同计算机上随机生成两个相同GUID的可能性是非常小的，但并不为0。所以，用于生成GUID的算法通常都加入了非随机的参数（如时间），以保证这种重复的情况不会发生。
优点：GUID是最简单的方案，跨平台，跨语言，跨业务逻辑，全局唯一的Id，数据间同步、迁移都能简单实现。
缺点：
1) 存储占了32位，且无可读性，返回GUID给客户显得很不专业；
2) 占用了珍贵的聚集索引，一般我们不会根据GUID去查单据，并且插入时因为GUID是无需的，在聚集索引的排序规则下可能移动大量的记录。
有两位园友主推GUID，无须顺序GUID方案原因如下：
[@徐少侠](http://home.cnblogs.com/u/Chinese-xu/) GUID无序在并发下效率高，并且一个数据页内添加新行，是在B树内增加，本质没有什么数据被移动，唯一可能的，是页填充因子满了，需要拆页。而GUID方案导致的拆页比顺序ID要低太多了（数据库不是很懂，暂时无法断定，大家自己认识）
[@无色](http://home.cnblogs.com/u/wusee/) 我们要明白id是什么，是身份标识，标识身份是id最大的业务逻辑，不要引入什么时间，什么用户业务逻辑，那是另外一个字段干的事，使用base64(guid,uuid)，是通盘考虑，完全可以更好的兼容nosql,key-value存储。
（推荐），但是倘若你系统一开始没有规划一个业务Id，那么将导致大量的改动，所以这个方案的最佳状态是一开始就设计业务Id，当然业务Id的唯一性也是我们要考虑的。
结论：适合大型应用；生成的Id不够友好；占据了32位；索引效率较低。
改进：
1) （[@dudu](http://home.cnblogs.com/u/dudu/)提点）在SQL Server 2005中新增了NEWSEQUENTIALID函数。
详细请看：[《理解newid()和newsequentialid()》](http://blog.csdn.net/xushichang/article/details/4390957)
在指定计算机上创建大于先前通过该函数生成的任何 GUID 的 GUID。 newsequentialid 产生的新的值是有规律的，则索引B+树的变化是有规律的，就不会导致索引列插入时移动大量记录的问题。
但一旦服务器重新启动，其再次生成的GUID可能反而变小（但仍然保持唯一）。这在很大程度上提高了索引的性能，但并不能保证所生成的GUID一直增大。SQL的这个函数产生的GUID很简单就可以预测，因此不适合用于安全目的。
a) 只能做为数据库列的DEFAULT VALUE，不能执行类似SELECT NEWSEQUENTIALID()的语句.
b) 如何获得生成的GUID.
如果生成的GUID所在字段做为外键要被其他表使用，我们就需要得到这个生成的值。通常，PK是一个IDENTITY字段，我们可以在INSERT之后执行 SELECT SCOPE_IDENTITY()来获得新生成的ID，但是由于NEWSEQUENTIALID()不是一个INDETITY类型，这个办法是做不到了，而他本身又只能在默认值中使用，不可以事先SELECT好再插入，那么我们如何得到呢？有以下两种方法：
` `
```
```
--1.
 定义临时表变量
```
`DECLARE``@outputTable ``TABLE`
```
(ID
 uniqueidentifier)
```
```
INSERTINTOTABLE1(col1,
 col2)
```
`OUTPUTINSERTED.ID ``INTO``@outputTable`
`VALUES``(``'value1'``, ``'value2'``)`
`SELECTID ``FROM``@outputTable`
```
--2.
 标记ID字段为ROWGUID（一个表只能有一个ROWGUID）
```
```
INSERTINTOTABLE1(col1,
 col2)
```
`VALUES``(``'value1'``, ``'value2'``)`
`--在这里，ROWGUIDCOL其实相当于一个别名`
```
SELECTROWGUIDCOL
 FROMTABLE1
```
```
结论：适合大型应用，解决了GUID无序特性导致索引列插入移动大量记录的问题。但是在关联表插入时需要返回数据库中生成的GUID；生成的Id不够友好；占据了32位。
2) “COMB”（combined guid/timestamp，意思是：组合GUID/时间截）
（感谢：[@ ethan-luo](http://home.cnblogs.com/u/ethan-luo/) ，[@lcs-帅](http://home.cnblogs.com/u/luchaoshuai/) ）
COMB数据类型的基本设计思路是这样的：既然GUID数据因毫无规律可言造成索引效率低下，影响了系统的性能，那么能不能通过组合的方式，保留GUID的10个字节，用另6个字节表示GUID生成的时间（DateTime），这样我们将时间信息与GUID组合起来，在保留GUID的唯一性的同时增加了有序性，以此来提高索引效率。
在NHibernate中，COMB型主键的生成代码如下所示：
` `
```
```
///
 <summary> /// Generate a new <see cref="Guid"/> using the comb algorithm.
```
```
///
 </summary>
```
```
privateGuid
 GenerateComb()
```
`{`
`byte`
```
[]
 guidArray = Guid.NewGuid().ToByteArray();
```
```
DateTime
 baseDate = newDateTime(1900, 1, 1);
```
```
DateTime
 now = DateTime.Now;
```
```
//
 Get the days and milliseconds which will be used to build
```
```
//the
 byte string
```
```
TimeSpan
 days = newTimeSpan(now.Ticks - baseDate.Ticks);
```
```
TimeSpan
 msecs = now.TimeOfDay;
```
```
//
 Convert to a byte array
```
```
//
 Note that SQL Server is accurate to 1/300th of a
```
```
//
 millisecond so we divide by 3.333333
```
`byte`
```
[]
 daysArray = BitConverter.GetBytes(days.Days);
```
`byte`
```
[]
 msecsArray = BitConverter.GetBytes((
```
`long``)`
```
(msecs.TotalMilliseconds
 / 3.333333));
```
```
//
 Reverse the bytes to match SQL Servers ordering
```
`Array.Reverse(daysArray);`
`Array.Reverse(msecsArray);`
```
//
 Copy the bytes into the guid
```
```
Array.Copy(daysArray,
 daysArray.Length - 2, guidArray,
```
```
guidArray.Length
 - 6, 2);
```
```
Array.Copy(msecsArray,
 msecsArray.Length - 4, guidArray,
```
```
guidArray.Length
 - 4, 4);
```
`returnnewGuid(guidArray);`
`}`
```
结论：适合大型应用。即保留GUID的唯一性的同时增加了GUID有序性，提高了索引效率；解决了关联表业务问题；生成的Id不够友好；占据了32位。（强烈推荐）
3) 长度问题，使用Base64或Ascii85编码解决。（要注意的是上述有序性方案在进行编码后也会变得无序）
如：
GUID：{3F2504E0-4F89-11D3-9A0C-0305E82C3301}
当需要使用更少的字符表示GUID时，可能会使用Base64或Ascii85编码。Base64编码的GUID有22－24个字符，如：
7QDBkvCA1+B9K/U0vrQx1A
7QDBkvCA1+B9K/U0vrQx1A==
Ascii85编码后是20个字符，如：
5:$Hj:Pf\4RLB9%kU\Lj
代码如：
Guid guid = Guid.NewGuid();
byte[] buffer = guid.ToByteArray();
var shortGuid = Convert.ToBase64String(buffer);
结论：适合大型应用，缩短GUID的长度。生成的Id不够友好；索引效率较低。
7、 GUID TO Int64
对于GUID的可读性，有园友给出如下方案：（感谢：[@黑色的羽翼](http://home.cnblogs.com/u/335908/)）
` `
```
```
///
 <summary>
```
```
///
 根据GUID获取19位的唯一数字序列
```
```
///
 </summary>
```
`publicstaticlongGuidToLongID()`
`{`
`byte`
```
[]
 buffer = Guid.NewGuid().ToByteArray();
```
```
returnBitConverter.ToInt64(buffer,
 0);
```
`}`
```
即将GUID转为了19位数字，数字反馈给客户可以一定程度上缓解友好性问题。EG:
GUID: cfdab168-211d-41e6-8634-ef5ba6502a22 （不友好）
Int64: 5717212979449746068 （友好性还行）
不过我的小伙伴说ToInt64后就不唯一了。因此我专门写了个并发测试程序，后文将给出测试结果截图及代码简单说明。
（唯一性、业务适合性是可以权衡的，这个唯一性肯定比不过GUID的，一般程序上都会安排错误处理机制，比如异常后执行一次重插的方案……）
结论：适合大型应用，生成相对友好的Id（纯数字）------因简单和业务友好性而推荐。
8、 自己写编码规则
优点：全局唯一Id，符合业务后续长远的发展（可能具体业务需要自己的编码规则等等）。
缺陷：根据具体编码规则实现而不同；还要考虑倘若主键在业务上允许改变的，会带来外键同步的麻烦。
我这边写两个编码规则方案：（可能不唯一，只是个人方案，也请大家提出自己的编码规则）
1) 12位年月日时分秒+3位服务器编码+3位表编码+5位随机码 （这样就完全单机完成生成全局唯一编码）---共23位
缺陷：因为附带随机码，所以编码缺少一定的顺序感。（生成高唯一性随机码的方案稍后给给出程序）
2) 12位年月日时分秒+3位服务器编码+3位表编码+5位流水码 （这样流水码就需要结合数据库和缓存）---共23位
缺陷：因为使用到流水码，流水码的生成必然会遇到和MaxId、序列表、Sequence方案中类似的问题
（为什么没有毫秒？毫秒也不具备业务可读性，我改用5位随机码、流水码代替，推测1秒内应该不会下99999[五位]条语法）
结论：适合大型应用，从业务上来说，有一个规则的编码能体现产品的专业成度。（强烈推荐）
GUID生成Int64值后是否还具有唯一性测试
测试环境
![clip_image002](http://images.cnitblog.com/blog/106337/201308/16080802-a0e5e5f2abf449cd91c6560e6e8188e8.jpg)
主要测试思路：
1. 根据内核数使用多线程并发生成Guid后再转为Int64位值，放入集合A、B、…N，多少个线程就有多少个集合。
2. 再使用Dictionary字典高效查key的特性，将步骤1中生成的多个集合全部加到Dictionary中，看是否有重复值。
示例注解：测了 Dictionary<long,bool> 最大容量就在5999470左右，所以每次并发生成的唯一值总数控制在此范围内，让测试达到最有效话。
主要代码：
` `
```
`for`
```
(inti
 = 0; i <= Environment.ProcessorCount - 1; i++)
```
`{`
`ThreadPool.QueueUserWorkItem(`
```
(list)
 =>
```
`{`
`List<``long`
```
>
 tempList = list asList<
```
`long``>;`
`for`
```
(intj
 = 1; j < listLength; j++)
```
`{`
`byte`
```
[]
 buffer = Guid.NewGuid().ToByteArray();
```
```
tempList.Add(BitConverter.ToInt64(buffer,
 0));
```
`}`
`barrier.SignalAndWait();`
```
},
 totalList[i]);
```
`}`
```
测试数据截图：
![clip_image004](http://images.cnitblog.com/blog/106337/201308/16080802-b7d385423a064ea093ab946c2a211a0d.jpg)
数据一（循环1000次，测试数：1000*5999470）
![image](http://images.cnitblog.com/blog/106337/201308/16080803-df53e12f2ceb40a0b1c04db43270fe10.png)
数据二（循环5000次,测试数：5000*5999470）--跑了一个晚上……
![image](http://images.cnitblog.com/blog/106337/201308/16080804-94893f591c044072af5db2ed27e5f498.png)
感谢[@Justany_WhiteSnow](http://home.cnblogs.com/u/justany/)的专业回答：(大家分析下，我数学比较差，稍后再说自己的理解)
GUID桶数量：(2 ^ 4) ^ 32 = 2 ^ 128
Int64桶数量： 2 ^ 64
倘若每个桶的机会是均等的，则每个桶的GUID数量为：
(2 ^ 128) / (2 ^ 64) = 2 ^ 64 = 18446744073709551616
也就是说，其实重复的机会是有的，只是概率问题。
楼主测试数是29997350000，发生重复的概率是：
1 - ((1 - (1 / (2 ^ 64))) ^ 29997350000) ≈ 1 - ((1 - 1 / （2 ^ 64)) ^ (2 ^ 32)) < 1 - 1 + 1 / (2 ^ 32) = 1 / (2 ^ 32) ≈ 2.3283064e-10
（唯一性、业务适合性是可以权衡的，这个唯一性肯定比不过GUID的，一般程序上都会安排错误处理机制，比如异常后执行一次重插的方案……）
（唯一性、业务适合性是可以权衡的，这个唯一性肯定比不过GUID的，一般程序上都会安排错误处理机制，比如异常后执行一次重插的方案……）
结论：GUID转为Int64值后，也具有高唯一性，可以使用与项目中。
Random生成高唯一性随机码
我使用了五种Random生成方案，要Random生成唯一主要因素就是种子参数要唯一。（这是比较久以前写的测试案例了，一直找不到合适的博文放，今天终于找到合适的地方了）
不过该测试是在单线程下的，多线程应使用不同的Random实例，所以对结果影响不会太大。
1. 使用Environment.TickCount做为Random参数（即Random的默认参数），重复性最大。
2. 使用DateTime.Now.Ticks做为Random参数，存在重复。
3. 使用unchecked((int)DateTime.Now.Ticks)做为Random参数，存在重复。
4. 使用Guid.NewGuid().GetHashCode()做为random参数，测试不存在重复（或存在性极小）。
5. 使用RNGCryptoServiceProvider做为random参数，测试不存在重复（或存在性极小）。
即：
```
`static``int``GetRandomSeed()`
`{`
`byte`
```
[]
 bytes =
```
`new``byte``[4];`
```
System.Security.Cryptography.RNGCryptoServiceProvider
 rng
```
`= ``new``System.Security.Cryptography.RNGCryptoServiceProvider();`
`rng.GetBytes(bytes);`
`return`
```
BitConverter.ToInt32(bytes,
 0);
```
`}`
```
测试结果：
![clip_image007](http://images.cnitblog.com/blog/106337/201308/16080804-c3130297cc3a4adbbb3cae404182ca60.jpg)
结论：随机码使用RNGCryptoServiceProvider或Guid.NewGuid().GetHashCode()生成的唯一性较高。
一些精彩评论（部分更新到原博文对应的地方）
一、
数据库文件体积只是一个参考值，可水平扩展系统性能（如nosql，缓存系统）并不和文件体积有高指数的线性相关。
如taobao/qq的系统比拼byte系统慢，关键在于索引的命中率，缓存，系统的水平扩展。
如果数据库很少，你搞这么多byte能提高性能？
如果数据库很大，你搞这么多byte不兼容索引不兼容缓存，不是害自已吗？
如果数据库要求伸缩性，你搞这么多byte，需要不断改程序，不是自找苦吗？
如果数据库要求移植性，你搞这么多byte，移植起来不如重新设计，这是不是很多公司不断加班的原因？
不依赖于数据存储系统是分层设计思想的精华，实现战略性能最大化，而不是追求战术单机性能最大化。
不要迷信数据库性能，不要迷信三范式，不要使用外键，不要使用byte，不要使用自增id，不要使用存储过程，不要使用内部函数，不要使用非标准sql，存储系统只做存储系统的事。当出现系统性能时，如此设计的数据库可以更好的实现迁移数据库（如mysql->oracle)，实现nosql改造（(mongodb/hadoop），实现key-value缓存(redis,memcache)。
二、
很多程序员有对性能认识有误区，如使用存储过程代替正常程序，其实使用存储过程只是追求单服务器的高性能，当需要服务器水平扩展时，存储过程中的业务逻辑就是你的噩运。
三、
除数字日期，能用字符串存储的字段尽量使用字符串存储，不要为节省那不值钱的1个g的硬盘而使用类似字节之类的字段，进而大幅牺牲系统可伸缩性和可扩展性。
不要为了追求所谓的性能，引入byte，使用byte注定是短命和难于移植，想想为什么html,email一直流行，因为它们使用的是字符串表示法，只要有人类永远都能解析，如email把二进制转成base64存储。除了实时系统，视频外，建议使用字符串来存储数据，系统性能的关键在于分布式，在于水平扩展。
# 【数据库水平拆分时的方案】
【自增字段分布式方案】
方案一：自增列都有设定步长的特性，假如我们打算把一张表只拆分为两个物理表，那么我们可以在其中一张表里把主键的自增列的步长设计为2，起始值为1，那么它的自增规律就是1,3,5,7依次类推，另外一张物理表的步长我们也可以设置为2，如果起始值为2，那么自增规律就是2,4,6,8以此类推，这样两张表的主键就绝对不会重复了，而且我们也不用另外做两张物理表相应的逻辑关联了。这种方案还有个潜在的好处，那就是步长的大小和水平数据拆分的粒度关联，也是我们为水平拆分的扩容留有余量，例如我们把步长设计为9，那么理论上水平拆分的物理表可以扩容到9个。
方案二：拆分出的物理表我们允许它最多存储多少数据，我们其实事先通过一定业务技术规则大致估算出来，假如我们估算一张表我们最多让它存储2亿条，那么我们可以这么设定自增列的规律，第一张物理表自增列从1开始，步长就设为1，第二种物理表的自增列则从2亿开始，步长也设为1，自增列都做最大值的限制，其他的依次类推。
如果表的主键不是使用自增列，而是【业务设计的唯一字段】，那么我们又如何处理主键分布问题了？这种场景很典型，例如交易网站里一定会有订单表，流水表这样的设计，订单表里有订单号，流水表里有流水号，这些编号都是按一定业务规则定义并且保证它的唯一性，那么前面的自增列的解决方案就没法完成它们做水平拆分的主键问题，那么碰到这个情况我们又该如何解决了？我们仔细回味下数据库的水平拆分，它其实和分布式缓存何其的类似，数据库的主键就相当于分布式缓存里的键值，那么我们可以按照分布式缓存的方案来设计主键的模型，方案如下：
　　方案一：使用整数哈希求余的算法，字符串如果进行哈希运算会得出一个值，这个值是该字符串的唯一标志，如果我们稍微改变下字符串的内容，计算的哈希值肯定是不同，两个不同的哈希值对应两个不同字符串，一个哈希值有且只对应唯一一个字符串，加密算法里的MD5，SHA都是使用哈希算法的原理计算出一个唯一标示的哈希值，通过哈希值的匹配可以判断数据是否被篡改过。不过大多数哈希算法最后得出的值都是一个字符加数字的组合，这里我使用【整数哈希算法】，这样计算出的哈希值就是一个整数。接下来我们就要统计下我们用于做水平拆分的服务器的数量，假如服务器的数量是3个，那么接着我们将计算的整数哈希值除以服务器的数量即取模计算，通过得到的余数来选择服务器，该算法的原理图如下所示：
　　方案二：就是方案一的升级版一致性哈希，【一致性哈希】最大的作用是保证当我们要扩展物理数据表的数量时候以及物理表集群中某台服务器失效时候才会体现，这个问题我后续文章会详细讨论物理数据库扩容的问题，因此这里先不展开讨论了。
　　由上所述，我们发现在数据库进行水平拆分时候，我们设定的算法都是通过主键唯一性进行的，根据主键唯一性设计的特点，最终数据落地于哪个物理数据库也是由主键的设计原则所决定的，回到上文里我提到的如果原库的数据表使用联合字段设计主键，那么我们就必须首先合并联合主键字段，然后通过上面的算法来确定数据的落地规则，虽然不合并一个字段看起来也不是太麻烦，但是在我多年开发里，把唯一性的字段分割成多个字段，就等于给主键增加了维度，字段越多，维度也就越大，到了具体的业务计算了我们不得不时刻留心这些维度，结果就很容易出错，我个人认为如果数据库已经到了水平拆分阶段了，那么就说明数据库的存储的重要性大大增强，为了让数据库的存储特性变得纯粹干净，我们就得尽力避免增加数据库设计的复杂性，例如去掉外键，还有这里的合并联合字段为一个字段，其实为了降低难度，哪怕做点必要的冗余也是值得。
　　解决数据库表的水平拆分后的主键唯一性问题有一个更加直接的方案，这也是很多人碰到此类问题很自然想到的方法，那就是把主键生成规则做成一个【主键生成系统】，放置在单独一台服务器上统一生成，每次新增数据主键都从这个服务器里获取，主键生成的算法其实很简单，很多语言都有计算UUID的功能，UUID是根据所在服务器的相关的硬件信息计算出的全球唯一的标示，但是这里我并没有首先拿出这个方案，因为它相比如我前面的方案缺点太多了，下面我要细数下它的缺点，具体如下：
　　缺点一：把主键生成放到外部服务器进行，这样我们就不得不通过网络通信完成主键值的传递，而网络是计算机体系里效率最低效的方式，因此它会影响数据新增的效率，特别是数据量很大时候，新增操作很频繁时候，该缺点会被放大很多；
　　缺点二：如果我们使用UUID算法做主键生成的算法，因为UUID是依赖单台服务器进行，那么整个水平拆分的物理数据库集群，主键生成器就变成整个体系的短板，而且是关键短板，主键生成服务器如果失效，整个系统都会无法使用，而一张表需要被水平拆分，而且拆分的表是业务表的时候，那么这张表在整个系统里的重要度自然很高，它如果做了水平拆分后出现单点故障，这对于整个系统都是致命的。当然有人肯定说，既然有单点故障，那么我们就做个集群系统，问题不是解决了吗？这个想法的确可以解决我上面阐述的问题，但是我前文讲到过，现实的软件系统开发里我们要坚守一个原则那就是有简单方案尽量选择简单的方案解决问题，引入集群就是引入了分布式系统，这样就为系统开发增加了开发难度和运维风险，如果我们上文的方案就能解决我们的问题，我们何必自讨苦吃做这么复杂的方案呢？
　　缺点三：使用外部系统生成主键使得我们的水平拆分数据库的方案增加了状态性，而我上面提到的方案都是无状态的，有状态的系统会相互影响，例如使用外部系统生成主键，那么当数据操作增大时候，必然会造成在主键系统上资源竞争的事情发生，如果我们对主键系统上的竞争状态处理不好，很有可能造成主键系统被死锁，这也就会产生我前文里说到的503错误，而无状态的系统是不存在资源竞争和死锁的问题，这洋就提升了系统的健壮性，无状态系统另一个优势就是水平扩展很方便。
　　这里我列出单独主键生成系统的缺点不是想说明我觉得这种解决方案完全不可取，这个要看具体的业务场景，根据作者我的经验还没有找到一个很合适使用单独主键生成器的场景。
　　上文里我提出的方案还有个特点就是能保证数据在不同的物理表里均匀的分布，均匀分布能保证不同物理表的负载均衡，这样就不会产生系统热点，也不会让某台服务器比其他服务器做的事情少而闲置资源，均匀分配资源可以有效的利用资源，降低生产的成本提高生产的效率，但是均匀分布式数据往往会给我们业务运算带来很多麻烦。
　　水平拆分数据库后我们还要考虑水平扩展问题，例如如果我们事先使用了3台服务器完成了水平拆分，如果系统运行到一定阶段，该表又遇到存储瓶颈了，我们就得水平扩容数据库，那么如果我们的水平拆分方案开始设计的不好，那么扩容时候就会碰到很多的麻烦。
