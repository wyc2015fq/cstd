# Phoenix边讲架构边调优 - Spark高级玩法 - CSDN博客
2017年11月26日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：1214
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXFHZhIIT1Tia5picbOGt3mY2UzFeblPYvTssDiaEbHaU8WKXNFqNkzOAP6F9CCP1at7pMC2HPOSXvFA/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**一 基础架构详解**
**1  概念**
讲调优之前，需要大家深入了解phoenix的架构，这样才能更好的调优。
Apache Phoenix在Hadoop中实现OLTP和运营分析，实现低延迟应用是通过结合下面两个优势：
- 
具有完整ACID事务功能的标准SQL和JDBC API的强大功能
- 
通过利用HBase作为后台存储，为NoSQL世界提供了late-bound, schema-on-read灵活的功能。
Apache Phoenix与其他Hadoop产品完全集成，如Spark，Hive，Pig，Flume和Map Reduce。
**2  Sql支持**
Apache Phoenix将SQL查询编译为一系列HBase scan，并编排这些scan的运行以生成常规的JDBC结果集。直接使用HBase的API的，与协处理器和自定义过滤器一起，实现了小查询以毫秒为单位，数千万级别的查询秒级。支持所有标准的SQL查询结构，包括SELECT，FROM，WHERE，GROUP BY，HAVING，ORDER
 BY等。它还支持一整套DML命令，以及通过DDL命令进行表格创建和版本增量更改。
**以下是目前不支持的列表：**
- 
关系运算符。相交，减号。
- 
其他内置功能。这些很容易添加 – 后面会出文章介绍。
**3  事务**
要启用完整的ACID事务（4.7.0版本中提供的测试版功能），请将phoenix.transactions.enabled属性设置为true。在这种情况下，您还需要运行发行版中包含的事务管理器。一旦启用，表可以选择性地声明为事务性的（参见这里为方向）。对事务表的提交将具有全或无的行为
 - 要么所有的数据都将被提交（包括对二级索引的任何更新），要么没有（将抛出异常）。支持跨表和跨行事务。另外，查询时，事务表将会看到他们自己的未提交的数据。乐观并发模型用于检测第一次提交获取语义的行级别冲突。稍后的提交会产生一个异常，表明发现了冲突。事务是在语句中引用事务表时隐式启动的，在这种情况下，除非发生提交或回滚，否则不会看到来自其他连接的更新。
非事务表没有超出hbase担保的行级别的原子性的担保。另外，非事务表直到发生提交之后才会看到它们的更新。ApachePhoenix的DML命令，UPSERT VALUES，UPSERT SELECT和DELETE，在客户端批量挂起HBASE表的变化。当事务被提交时，更改被发送到服务器，并在事务回滚时被丢弃。如果为连接启用自动提交，则Phoenix将尽可能通过服务器端的协处理器执行整个DML命令，从而提高性能。
**4 时间戳**
通常情况下，应用程序会让HBase管理时间戳。但是，在某些情况下，应用程序需要控制时间戳本身。在这种情况下，可以在连接时指定CurrentSCN属性来控制任何DDL，DML或查询的时间戳。此功能可用于对先前行值运行快照查询，因为Phoenix使用此连接属性的值作为扫描的最大时间戳。
对于事务表时间戳是不可控的。事务管理器分配时间戳，在提交后该分配的时间戳变成hbase cell时间戳。
**5 schema**
Apache Phoenix支持通过DDL命令进行表创建和版本化增量更改。表元数据存储在HBase表中并进行版本控制，以便对先前版本的快照查询将自动使用正确的schema。
**phoenix是通过CREATE TABLE命令创建的，可以是：**
1.
从头开始构建，在这种情况下，HBase的表和列簇将自动创建。
2.
映射一张现有的hbase表，可以通过创建一个读写表或者一个只读视图来实现。需要注意的是hbase表Rowkey的数据类型及key values的数据类型必须和phoenix的数据类型一致。
- 
对于读写表，如果它们不存在，列族将自动创建。将空的键值添加到每个行的第一列族中，以最小化查询project的大小。
- 
对于只读VIEW，所有列系列必须已经存在。对HBase表进行的唯一修改是增加用于查询处理的Phoenix协处理器。VIEW的主要用例是将现有数据转移到Phoenix表中，因为在VIEW上不允许修改数据，查询性能可能会低于TABLE。
所有schema都是版本化的（最多保留1000个版本）。对旧数据进行快照查询将根据您连接的时间（基于CurrentSCN属性）提取并使用正确的模式。
**6 变更**
ALTER TABLE命令可以修改phoenix表。当运行引用表的SQL语句时，Phoenix将默认与服务器进行检查，以确保它具有最新的表元数据和统计信息。当您事先知道表格的结构可能永远不会改变时，这个RPC可能是不必要的。在Phoenix 4.7中添加了UPDATE_CACHE_FREQUENCY属性，以允许用户声明服务器检查元数据更新的频率（例如添加或删除表列或更新表统计信息）。可能的值是ALWAYS（默认），从不，和毫秒数值。ALWAYS值会导致客户端每次执行一个引用表的语句（或每次提交一个UPSERT
 VALUES语句）一次就检查服务器。
例如，以下DDL命令将创建表FOO并声明客户端应每15分钟检查表或其统计信息的更新：
CREATE TABLE FOO（k BIGINT PRIMARY KEY，v VARCHAR）UPDATE_CACHE_FREQUENCY = 900000;
**7 视图**
Phoenix支持表上的可更新视图，这种独特的功能可以利用HBase的无模式功能来添加列。所有视图都共享相同的底层物理HBase表，甚至可以独立索引。后面可以详细介绍。
**8 多租户**
phoenix建立在视图支持之上，也支持多租户。与视图一样，多租户视图可以添加专门为该用户定义的列。
**9 Salting (散列)**
一个表可以被声明为咸防止Region热点。您只需声明您的表有多少个salted桶，phoenix将透明地管理您的Salting。这个优化后面详细介绍。
**9 映射hbase表**
可以通过创建视图或者表的方式映射一张已经存在的hbase表到phoenix。详细操作请参考：[实战phoenix](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484253&idx=1&sn=2d1a9100dae4abae0efc2a4fea6fded5&chksm=9f38e075a84f69639b03aaa5491c9d9016a3e9057a37f3bf3dc892159fcde39dbb361350d7ac&scene=21#wechat_redirect)
**二 调优指引**
调优phoenix是非常复杂的，但是假如知道一点它的工作原理，就可以在读写性能方面做出巨大改进。性能最大的影响因素就是你schema的设计，尤其他会影响hbase的Rowkey。
**1 主键**
底层的Rowkey是Phoenix性能中最重要的一个因素，在设计阶段正确设置是非常重要的，因为在不重写数据和索引表的情况下，以后无法进行更改。
Phoenix主键连接在Apache
 HBase中创建底层Rowkey。主键约束的列应该按照与常用查询模式对齐的方式进行选择和排序
 - 选择最频繁查询的列作为主键。放在前位置的key是非常重要的。例如，如果您使用包含组织标识值的列(ID)来引导，则可以轻松选择与特定组织有关的所有行。您可以将HBase行时间戳添加到主键，以通过跳过查询时间范围外的行来提高扫描效率。
每个主键都会产生一定的成本，因为整个行键被添加到内存中和磁盘上的每一条数据上。行键越大，存储开销就越大。例如，找到方法来将信息紧凑地存储在您计划用于主键的列中 - 存储变量而不是完整的时间戳。
总而言之，最佳做法是设计主键来添加行键，以便扫描最小量的数据。
*** 提示： *选择主键时，首先在最重要的优化查询中过滤最频繁的列。如果您在查询中使用ORDER BY，请确保您的PK列匹配您的ORDER BY子句中的表达式。**
**总而言之，最佳做法是设计主键来添加行键，以便扫描最小量的数据。**
**单调递增主键**
如果主键单调递增，则使用salting来帮助在整个集群中分散写入并提高并行性。例：
CREATE TABLE ...（...）SALT_BUCKETS = N
为了获得最佳性能，盐桶的数量应该近似等于Region Server的数量。不要自动salting。只有在遇到热点时才使用salting。腌制的缺点是它增加了读的成本，因为当你想查询数据时，你必须运行多个查询来进行范围扫描。
**2 通用技巧**
**2.1 数据随机访问**
与任何随机读取工作负载一样，SSD可以提高性能，因为它们的随机查找时间更快。
**2.2 数据是重读还是重写**
对于重读数据：
创建全局索引。这将影响写入速度，具体取决于包含在索引中的列数，因为每个索引写入到自己的单独表中。
使用多个索引来提供对常见查询的快速访问。
在为HBase指定机器时，不要吝啬cpu; HBase需要它们。
对于重写数据：
预分割表。将表拆分成预定义的区域，或者如果键单调递增，可以使用salting来避免在少量节点上创建写入热点。使用真正的数据类型而不是原始字节数据。
创建本地索引。从本地索引读取有性能损失，所以做性能测试是很重要的。
**2.3 哪些列将经常访问**
选择通常查询的列作为主键。创建额外的索引来支持常见的查询模式，包括大量访问不在主键中的字段。
**2.4 数据可以追加（不可变）吗？**
如果数据是不可变的或仅附加的，则在创建时使用IMMUTABLE_ROWS 选项将表及其索引声明为不可变，以减少写入时间成本。如果您需要使现有的表不可变，那么可以在创建后使用ALTER TABLE trans.event SET IMMUTABLE_ROWS = true。
如果速度比数据完整性更重要，则可以使用DISABLE_WAL 选项。注意：如果区域服务器发生故障，可能会丢失DISABLE_WAL的数据。
如果元数据不经常更改，请将UPDATE_CACHE_FREQUENCY 选项设置为15分钟左右。此属性确定RPC的执行频率，以确保您看到最新的schema。
如果数据不稀疏（超过50％的单元格有值），请使用Phoenix 4.10中引入的SINGLE_CELL_ARRAY_WITH_OFFSETS数据编码方案，通过减小数据大小来获得更快的性能。有关更多信息，请参阅Apache Phoenix博客上的“ 列映射和不可变数据编码 ”。
**2.5  表很大？**
在您的CREATE INDEX调用中使用ASYNC关键字，通过MapReduce作业异步创建索引。你需要手动启动job;
如果数据太大而无法完全扫描表，则使用主键创建底层组合行键，以便返回数据的一个子集或便于跳过扫描。当查询包括时，Phoenix可以直接跳转到匹配键谓词中的键集。 
**2.6 事务需要否？**
事务是一个原子的数据操作 - 即保证完全成功或根本不成功。例如，如果您需要对数据表进行跨行更新，那么您应该将数据视为事务性的。事务后面会详细介绍。
如果需要事务，请使用TRANSACTIONAL 选项。
**2.7 块编码**
使用压缩或编码是必须的。SNAPPY和FAST_DIFF都是很好的选择。
FAST_DIFF编码默认在所有Phoenix表上自动启用，并且通过允许更多的数据适合块缓存，几乎总是可以提高整体读取延迟和吞吐量。注意：FAST_DIFF编码会增加请求处理过程中产生的垃圾。
在表创建时设置编码。示例：CREATE TABLE ...（...）DATA_BLOCK_ENCODING ='FAST_DIFF'
**3 schema设计**
由于schema会影响数据写入底层HBase层的方式，因此Phoenix性能依赖于表，索引和主键的设计。
**3.1 Phoenix和HBase数据模型**
Hbase的数据存储在表中，并且按照列簇将列进行分组。HBase表中的一行由与一个或多个列关联的版本化单元组成。HBase行是许多键值对的集合，其中键的rowkey属性相同。HBase表中的数据按rowkey排序，所有访问都通过rowkey进行。Phoenix在HBase的基础上创建了一个关系数据模型，强制执行一个PRIMARY KEY约束，这些约束的列被连接起来形成底层HBase表的Rowkey。出于这个原因，认识到PK约束中包含的列的大小和数量非常重要，因为HBase表中的每个cell都包含Rowkey的副本。
**3.2 列簇**
如果某些列的访问频率比其他列高，则可以创建多个列族，将经常访问的列与很少访问的列分开。这可以提高性能，因为HBase只读取查询中指定的列族。
**3.3 列**
一般来说，以下是适用于列的一些技巧，不管它们是否已编入索引：
- 
由于I / O成本，请将VARCHAR列保留在1MB左右。在处理查询时，HBase在将它们发送给客户端之前将它们全部实现，客户端将它们全部接收，然后将它们交给应用程序代码。
- 
对于结构化对象，不要使用JSON，它不是非常紧凑的。使用protobuf，Avro，msgpack或BSON等格式。
- 
考虑在使用快速LZ变体存储之前压缩数据以削减延迟和I / O成本。
- 
使用列映射功能（添加在Phoenix 4.10中），该功能对非PK列使用数字HBase列限定符，而不是直接使用列名。当在HBase返回的已排序单元列表中查找单元格时，这会提高性能，通过减少表使用的磁盘大小进一步提高了性能，并加快了DDL操作（如列重命名和元数据级别的列丢弃）。
**4 索引**
Phoenix表是一个物理表，他存储了主表的部分或者全部数据的副本，以便为特定种类的查询提供服务。发出查询时，phoenix会自动为查询选择最佳索引。主所以没是根据选择的主键自动创建的。你可以直接创建二级索引，根据索引将支持的预期查询来指定包含哪些列。
**4.1 二级索引**
二级索引可以将通常将全表扫描转换为点查找（以存储空间和写入速度为代价），从而提高读取性能。可以在创建表之后添加或删除次要索引，而不需要对现有查询进行更改 - 查询运行速度更快。少量二级指标通常就足够了。根据您的需要，可以考虑创建覆盖索引或功能索引，或两者兼而有之。
如果您的表很大，则使用CREATE INDEX的ASYNC关键字异步创建索引。在这种情况下，索引将通过MapReduce建立，这意味着客户端上下不会影响索引创建，并且必要时会自动重试作业。您将需要手动启动作业，然后您可以像监视其他MapReduce作业一样监视作业。
示例：如果不存在，则创建索引event.eobject_id_idx_b on trans.event（object_id）ASYNC UPDATE_CACHE_FREQUENCY = 60000;
如果由于某种原因不能异步创建索引，则增加查询超时（phoenix.query.timeoutMs），使其大于生成索引所花费的时间。如果CREATE INDEX调用超时或客户端在完成之前关闭，则索引构建将停止并且必须再次运行。您可以在创建索引表时监视索引表，您将看到在发生分割时创建的新regions。您可以查询SYSTEM.STATS表，split和压缩发生将会用新的插入。您也可以直接对索引表运行一个count(*)查询，尽管这会增加系统负载，因为需要全表扫描。
**4.2 技巧：**
- 
为写入繁重的表创建本地索引。
- 
为读取大量用例创建全局索引。为节省读取时间开销，请考虑创建覆盖索引。
- 
如果主键单调递增，则创建salt buckets。salt
 buckets以后不能改变，所以设计它们来处理未来的增长。salt buckets有助于避免写入热点，但由于读取所需的额外扫描，可能会降低整体吞吐量。
- 
设置一个cron作业来建立索引。对CREATE INDEX使用ASYNC来避免阻塞。
- 
只创建你需要的索引。
- 
限制频繁更新表上的索引数量。
- 
使用覆盖索引将表扫描转换为索引表（而不是主表）上的高效点查找或范围查询：CREATE INDEX索引ON表（...）INCLUDE（...）
**5 查询**
重要的是要知道哪些查询在服务器端与客户端的执行，因为这会影响性能，由于网络I/O和其他瓶颈。如果您正在查询一个十亿行的表，您希望在服务器端执行尽可能多的计算，而不是将十亿行传输到客户端进行处理。另一方面，一些查询必须在客户端上执行。例如，对驻留在多个区域服务器上的数据进行排序，要求您在客户端上进行聚合和重新排序。
**5.1 读**
- 
除非一方小，尤其是频繁的查询，否则应避免join。
- 
在WHERE子句中，过滤主键约束中的靠前的列。
- 
WHERE子句中的相等或比较（<或>）使范围扫描优化成为可能。
- 
让Phoenix使用统计来优化查询并行性。如果在生产中使用Phoenix 4.2或更高版本，这将带来自动收益。
**5.2 范围查询**
如果您经常从旋转磁盘扫描大型数据集，则最好使用GZIP（但要注意速度）。使用大量内核进行扫描以利用可用内存带宽。Apache Phoenix可以轻松利用多个cores来提高扫描性能。
对于范围查询，HBase块缓存没有提供太多优势。
**5.3 大范围查询**
对于大范围查询，即使整个扫描可能适合块缓存，也应考虑设置Scan.setCacheBlocks（false）。
如果你主要执行大范围的查询，你甚至可能会考虑使用一个更小的堆来运行HBase，并将块缓存大小设置为仅依靠OS Cache。这将缓解一些垃圾收集相关的问题。
**5.4 点查找**
对于点查找，缓存数据集非常重要，您应该使用HBase块缓存。
**5.5 提示：**
- 
提示使您可以覆盖默认的查询处理行为，并指定使用哪个索引，要执行的扫描类型以及要使用的连接类型等因素。
- 
在查询期间，如果您想在查询包含不在索引中的列时强制提示全局索引。
- 
如果有必要，可以使用/ * + USE_SORT_MERGE_JOIN * /提示进行更大的连接，但是大连接对于大数据量来说将是一个昂贵的操作。
- 
如果所有右侧表的总大小超过内存大小限制，请使用/ * + NO_STAR_JOIN * /提示。
**6 写入**
**6.1 批量处理大量记录**
使用UPSERT写入大量记录时，请关闭自动提交和批处理记录。注意： Phoenix使用commit（）而不是executeBatch（）来控制批量更新。
从批量大小1000开始，根据需要进行调整。下面是一些伪代码，显示了批量提交记录的一种方式：
尝试（连接conn = DriverManager.getConnection（url））{
 conn.setAutoCommit（false）; 
 int batchSize = 0; 
 int commitSize = 1000; //每批次提交的行数。  
 尝试（语句stmt = conn.prepareStatement（upsert））{
   stmt.set ... while（有记录上插）{
     stmt.executeUpdate（）; 
     BATCHSIZE ++; 
     if（batchSize％commitSize == 0）{
       conn.commit（）; 
     } 
  } 
conn.commit（）; //提交最后一批记录
}
注意：由于Phoenix客户端在内存中保留未提交的行，请小心不要将commitSize设置得太高。
**6.2 减少RPC**
为了减少RPC流量，在创建表或索引时设置UPDATE_CACHE_FREQUENCY（4.7或更高版本）
**6.3 使用本地索引**
如果使用4.8，请考虑使用本地索引来最小化写入时间。在这种情况下，二级索引的写入将与基表相同的区域服务器。不过，这种方法确实会影响读取性能，所以请确保对写入速度的提高和读取速度的降低进行量化。
**7 删除**
删除大型数据集时，请在发出DELETE查询之前启用autoCommit，以便客户端在删除所有键时不必记住所有键的行键。这可以防止客户端缓冲受DELETE影响的行，以便Phoenix可以直接在区域服务器上删除它们，而无需将其返回给客户端。
**8 解释计划**
一个EXPLAIN计划告诉你很多关于如何运行一个查询：
- 
所有将要执行的HBase范围查询
- 
将被扫描的字节数
- 
将要遍历的行数
- 
哪个HBase表将用于每个扫描
- 
在客户端和服务器端执行哪些操作（排序，合并，扫描，限制）
- 
使用EXPLAIN计划检查查询的运行方式，并考虑重写查询以达到以下目标：
- 
强调服务器上的操作，而不是客户端上的操作。服务器操作分布在集群中，并行操作，而客户端操作在单个客户端JDBC驱动程序内执行。
- 
尽可能使用RANGE SCAN或SKIP SCAN，而不是TABLE SCAN。
- 
根据主键约束中的前导列进行过滤。假设您已经设计了主键，以便像上面的“主键”中所描述的那样使用频繁访问或频繁过滤的列。
- 
如有必要，引入涵盖查询的本地索引或全局索引。
- 
如果你有一个索引覆盖你的查询，但优化器没有检测到它，请尝试暗示查询：SELECT / * + INDEX（）* / ...
**9 解释计划的解剖**
解释计划包括描述Phoenix在查询期间执行的操作的文本行，使用以下术语：
- 
AGGREGATE IN ORDER DISTINCT ROWS -使用诸如添加之类的操作来聚集返回的行。使用ORDERED时，GROUP BY操作应用于主键约束的前导部分，这样可以在恰当的位置完成聚合，而不是将所有去重的组保留在服务器端的内存中。
- 
AGGREGATE INTO SINGLE ROW -使用不带GROUP BY子句的聚合函数将结果聚集成单行。例如，count（）语句返回一行，其中包含匹配查询的总行数。
- 
client - 操作将在客户端执行。在服务器端执行大多数操作会更快，因此您应该考虑是否有重写查询的方法，以便在服务端运行更多的工作。
- 
FILTER BY表达式 - 只返回匹配表达式的结果。
- 
FULL SCAN OVER tableName - 操作将扫描指定表中的每一行。
- 
INNER-JOIN - 该操作将在join条件满足的行上join多个表。
- 
MERGE SORT -对结果执行合并排序。
- 
RANGE SCAN OVER tableName [ ... ] - 方括号中的信息表示查询中使用的每个主键的开始和结束。
- 
ROUND ROBIN - 如果查询不包含ORDER BY，因此可以按任意顺序返回行，则ROUND ROBIN命令可以最大化客户端的并行处理。
- 
x -CHUNK - 描述将有多少线程用于操作。最大并行度受限于线程池中的线程数。最小平行化对应于表格在扫描的开始行和停止行之间的region数量。块的数量将随着路标宽度的增加而增加，因为每个region有多个块。
- 
PARALLEL x- WAY - 描述在操作过程中合并排序的并行扫描数量。
- 
SERIAL - 一些查询串行运行。例如，单行查询或在主键的前导部分进行筛选并将结果限制在可配置阈值以下的查询。
例
+ ------------------------------------------ +
| PLAN |
+ ------------------------------------------ +
| 客户端36-CHUNK并行36-way全面扫描exDocStoreb |
| 并行内部连接表0（跳过合并）|
| 客户端36-CHUNK并行36-way范围扫描indx_exdocb [0，'42ecf4abd4bd7e7606025dc8eee3de 6a3cc04418cbc2619ddc01f54d88d7 c3bf']  -  [0，'42ecf4abd4bd7e7606025dc8eee3de 6a3cc04418cbc2619ddc01f54d88d7
 c3bg'| 
| 服务器仅通过第一键筛选|
| 服务器通过[“ID”] |进入排序的DISTINCT行
| 客户端合并排序|
| 动态服务器过滤器（A.CURRENT_TIMESTAMP，[A.ID]（http://a.id/））IN（（TMP.MCT，TMP.TID））|
+ ------------------------------------------- +
**10 提高并行性**
您可以使用UPDATE STATISTICS命令提高并行性。这个命令通过确定被称为路标的关键字来划分每个区域，这些关键字彼此等距，然后使用这些路标将查询分解成多个并行扫描。统计信息默认打开。使用Phoenix 4.9，用户可以为每张表设置路标宽度。最佳路标宽度取决于许多因素，如群集大小，群集使用情况，每个节点的内核数量，表大小和磁盘I / O。
在Phoenix 4.12中，我们添加了一个新的配置phoenix.use.stats.parallelization，控制是否应该使用统计来驱动并行化。请注意，仍然可以运行统计信息收集。收集到的信息用于估计查询在为其生成EXPLAIN时将扫描的字节数和行数。
**推荐阅读：**
1， [实战phoenix](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484253&idx=1&sn=2d1a9100dae4abae0efc2a4fea6fded5&chksm=9f38e075a84f69639b03aaa5491c9d9016a3e9057a37f3bf3dc892159fcde39dbb361350d7ac&scene=21#wechat_redirect)
2，[大数据查询——HBase读写设计与实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484239&idx=1&sn=93c55d3a91f6ff9646389755844cabe7&chksm=9f38e067a84f697182d978ba8c3d89a4b3b7349beccbf070a9dc958b5738a83e6cd9bf24b49d&scene=21#wechat_redirect)
3，[Spark高级操作之json复杂和嵌套数据结构的操作一](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483842&idx=1&sn=8db9a7f4e141d2d79ab46d0c8ee35650&chksm=9f38e2eaa84f6bfc96cab257a88a131d3d5fc53bff195c9d7d0c022790703965afe1eea6b114&scene=21#wechat_redirect)
4，[大数据基础系列之spark的监控体系介绍](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483792&idx=1&sn=2fe2364e08fec3fd9d57d2f5d1b99e2b&chksm=9f38e2b8a84f6baee19e8d430e522389cf8df33820d95dbfa4f0e4ef63f7661ca3bf41e73d7f&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**
