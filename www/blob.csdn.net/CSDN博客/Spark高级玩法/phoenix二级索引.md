# phoenix二级索引 - Spark高级玩法 - CSDN博客
2017年11月27日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：601
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXR9gDaCNpJmopRlb7jViaYLxufcb0KCytxRWKJSFjA3dxN3AUWiaZibJ9MZhGrgQ62FnJbEcnetDicTg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**二级索引**
二级索引是从主键访问数据的正交方式。Hbase中有一个按照字典排序的主键Rowkey作为单一的索引。不按照Rowkey去读取记录都要遍历整张表，然后按照你指定的过滤条件过滤。通过二级索引，索引的列或表达式形成一个备用行键，以允许沿着这个新轴进行点查找和范围扫描。
**1   覆盖索引（Covered Indexes）**
Phoenix特别强大，因为它提供了覆盖索引。一旦找到索引的条目，不需要返回主表。相反，把我么关心的数据绑定到索引行，节省了读取的时间开销。
例如，以下内容将在v1和v2列上创建一个索引，并在索引中包含v3列，以防止从原始数据表中获取该列：
CREATE INDEX my_index ON my_table（v1，v2）INCLUDE（v3）
**2  功能索引（Functional Indexes）**
功能索引（在4.3和更高版本中可用）允许您不仅在列上而且在任意表达式上创建索引。然后，当一个查询使用该表达式时，索引可以用来检索结果而不是数据表。例如，您可以在UPPER（FIRST_NAME ||''|| LAST_NAME）上创建一个索引，以便您可以对组合的名字和姓氏进行不区分大小写的搜索。
例如，下面将创建这个功能索引：
在EMP上创建索引UPPER_NAME_IDX（UPPER（FIRST_NAME
 ||''|| LAST_NAME））
有了这个索引，发出下面的查询时，将使用索引而不是数据表来检索结果：
SELECT EMP_ID FROM EMP WHERE UPPER（FIRST_NAME
 ||''|| LAST_NAME）='JOHN DOE'
Phoenix支持两种索引技术：全局索引和本地索引。每个在不同的情况下都很有用，并且有自己的故障概况和性能特点。
**3  全局索引**
全局索引适合读操作任务重的用例。使用全局索引，索引的所有性能损失都是在写入时发生的。我们拦截数据表更新写（DELETE，UPSERT VALUES和UPSERT SELECT），建立索引更新，然后发送任何必要的更新到所有感兴趣的索引表。在读的时候，phoenix会选择索引表，然后使用它，这使得查询加快并且直接可想其它表一样scan索引表。默认情况下，除非暗示，否则索引不会用于引用不属于索引的列的查询。
**4  本地索引**
本地索引适合写任务繁重，且空间有限的用例。就像全局索引一样，Phoenix会在查询时自动选择是否使用本地索引。使用本地索引，索引数据和表数据共同驻留在同一台服务器上，防止写入期间的任何网络开销。即使查询没有被完全覆盖，也可以使用本地索引（即Phoenix自动检索不在索引中的列，通过与数据表相对应的索引）。与全局索引不同，4.8.0版本之前所有的本地索引都存储在一个单独独立的共享表中。从4.8.0版本开始，所有的恩地索引数据都存储于相同数据表的独立列簇里。。在读取本地索引时，由于不能确定索引数据的确切区域位置，所以必须检查每个区域的数据。因此在读取时会发生一些开销。
**5  索引填写**
默认情况下，创建索引时，会在CREATE INDEX调用期间同步填充该索引。根据数据表的当前大小，这可能是不可行的。从4.5开始，可以通过在索引创建DDL语句中包含ASYNC关键字来异步完成索引的填充：
CREATE INDEX async_index ON my_schema.my_table（v）ASYNC
必须通过HBase命令行单独启动填充索引表的map reduce作业，如下所示：
$ {HBASE_HOME} / bin / hbase org.apache.phoenix.mapreduce.index.IndexTool
 --schema MY_SCHEMA --data-table MY_TABLE --index-table ASYNC_IDX
 -
输出路径ASYNC_IDX_HFILES
只有mapreduce作业完成后，索引才会被激活并开始在查询中使用。这项工作对于退出的客户端是有弹性的。输出路径选项用于指定用于写入HFile的HDFS目录。
**6  索引用法**
Phoenix会在查询的时候自动选择高效的索引。但是，除非查询中引用的所有列都包含在索引中，否则不会使用全局索引。
例如，以下查询不会使用索引，因为在查询中引用了v2，但未包含在索引中：
SELECT v2 FROM my_table WHERE v1 ='foo'
在这种情况下，有三种获取索引的方法：
**1， 通过在索引中包含v2来创建一个覆盖索引：**
CREATE INDEX my_index ON my_table（v1）INCLUDE（v2）
这将导致v2列值被复制到索引中，并随着更改而保持同步。这显然会增加索引的大小。
2**，提示查询强制它使用索引：**
SELECT / * + INDEX（my_table
 my_index）* / v2 FROM my_table WHERE v1 ='foo'
这将导致在遍历索引时找到每个数据行以找到缺少的v2列值。这个提示只有在你知道索引有很好的选择性的时候才可以使用（例如，在这个例子中有少数量行的值是'foo'），否则你可以通过默认的行为来获得更好的性能全表扫描。
**3， 创建一个本地索引：**
CREATE LOCAL INDEX my_index ON my_table（v1）
与全局索引不同，即使查询中引用的所有列都不包含在索引中，本地索引也将使用索引。这是默认为本地索引完成的，因为我们知道在同一个区域服务器上的表和索引数据coreside确保查找是本地的。
**7  索引删除**
要删除索引，使用以下语句：
DROP INDEX my_index ON my_table
如果索引列在数据表上被删除，索引会被自动删除。另外，如果一个覆盖的列在数据表中被删除，它也会被从索引表中本删除。
**8  索引属性**
就像使用CREATE TABLE语句一样，CREATE INDEX语句可以通过属性应用到底层的HBase表，包括对其进行限制的能力：
CREATE INDEX my_index ON my_table（v2
 DESC，v1）INCLUDE（v3）
   SALT_BUCKETS = 10，DATA_BLOCK_ENCODING
 ='NONE'
请注意，如果主表是salted，则对于全局索引，索引将以相同的方式自动被salted。另外，相对于主索引表与索引表的大小，索引的MAX_FILESIZE向下调整。另一方面，使用本地索引时，不允许指定SALT_BUCKETS。
**一致性保证**
在提交后成功返回给客户端，所有数据保证写入所有感兴趣的索引和主表。换句话说，索引更新与HBase提供的相同强一致性保证是同步的。
然而，由于索引存储在与数据表不同的表中，取决于表的属性和索引类型，当服务器崩溃时提交失败时，表和索引之间的一致性会有所不同。这是一个由您的需求和用例驱动的重要设计考虑。
**1  事务表**
通过将您的表声明为事务性的，您可以实现表和索引之间最高级别的一致性保证。在这种情况下，您的表突变和相关索引更新的提交是具有强ACID保证的原子。如果提交失败，那么您的数据（表或索引）都不会更新，从而确保您的表和索引始终保持同步。
为什么不总是把你的表声明为事务性的？这可能很好，特别是如果你的表被声明为不可变的，因为在这种情况下事务开销非常小。但是，如果您的数据是可变的，请确保与事务性表发生冲突检测相关的开销和运行事务管理器的运行开销是可以接受的。此外，具有二级索引的事务表可能会降低写入数据表的可用性，因为数据表及其辅助索引表必须可用，否则写入将失败。
**2  不变的表**
对于其中数据只写入一次而从不更新的表格，可以进行某些优化以减少增量维护的写入时间开销。这是常见的时间序列数据，如日志或事件数据，一旦写入行，它将永远不会被更新。要利用这些优化，通过将IMMUTABLE_ROWS = true属性添加到您的DDL语句中，将您的表声明为不可变：
CREATE TABLE my_table（k
 VARCHAR PRIMARY KEY，v VARCHAR）IMMUTABLE_ROWS
 = true
用IMMUTABLE_ROWS = true声明的表上的所有索引都被认为是不可变的（请注意，默认情况下，表被认为是可变的）。对于全局不可变索引，索引完全在客户端维护，索引表是在数据表发生更改时生成的。另一方面，本地不可变索引在服务器端保持不变。请注意，没有任何保护措施可以强制执行，声明为不可变的表格实际上不会改变数据（因为这会否定所达到的性能增益）。如果发生这种情况，指数将不再与表格同步。
如果您有一个现有的表，您想从不可变索引切换到可变索引，请使用ALTER TABLE命令，如下所示：
ALTER TABLE my_table SET IMMUTABLE_ROWS = false
非事务性，不可变表的索引没有自动处理提交失败的机制。保持表和索引之间的一致性留给客户端处理。因为更新是幂等的，所以最简单的解决方案是客户端继续重试一批修改，直到它们成功。
**3  可变表**
对于非事务性可变表，我们通过将索引更新添加到主表行的预写日志（WAL）条目来维护索引更新持久性。只有在WAL条目成功同步到磁盘后，我们才会尝试更新索引/主表。phoenix默认并行编写索引更新，从而导致非常高的吞吐量。如果服务器在我们写索引更新的时候崩溃了，我们会重做所有索引更新到WAL恢复过程中的索引表，并依赖更新的幂等性来确保正确性。因此，非事务性可变表上的索引只是主表的一批编辑。
**重要注意几点：**
- 
对于非事务性表，可能看到索引表与主表不同步。
- 
如上所述，由于我们只是有一小部分落后并且仅仅一小段时间不同步所以这是ok的。
- 
每个数据行及其索引行保证被写入或丢失 - 从来没有看到部分更新，因为这是HBase原子性保证的一部分。
- 
首先将数据写入表中，然后写入索引表（如果禁用WAL，则相反）。
**3.1  单个写入路径**
有一个保证失败属性的写入路径。所有写入HRegion的内容都被我们的协处理器拦截。然后，我们根据挂起更新（或更新，如果是批处理）构建索引更新。然后这些更新被附加到原始更新的WAL条目。
在此之前如果有任何失败，将会返回失败给客户端，并且没有数据会被持久化，客户端也看不到任何数据。
一旦WAL被写入，我们确保即使在失败的情况下，索引和主表数据也将变得可见。
- 
如果服务崩溃，phoenix会使用WAL重复机制去重新构建索引更新。
- 
如果服务器没有崩溃，我们只是将索引更新插入到它们各自的表中。
- 
如果索引更新失败，下面概述了保持一致性的各种方法。
- 
如果Phoenix系统目录表在发生故障时无法到达，phoenix强制服务器立即中止并失败，在JVM上调用System.exit，强制服务器死机。通过杀死服务器，我们确保WAL将在恢复时重新使用，将索引更新重新生成到相应的表中。这确保了二级索引在知道无效状态时不会继续使用。
**3.2 禁止表写入，直到可变的索引是一致的**
在非事务性表和索引之间保持一致性的最高级别是声明在更新索引失败的情况下应暂时禁止写入数据表。在此一致性模式下，表和索引将保留在发生故障之前的时间戳，写入数据表将被禁止，直到索引重新联机并与数据表同步。该索引将保持活动状态，并像往常一样继续使用查询。
**以下服务器端配置控制此行为：**
- 
**phoenix.index.failure.block.write**必须为true，以便在发生提交失败时写入数据表失败，直到索引可以追上数据表。
- 
**phoenix.index.failure.handling.rebuild**必须为true（缺省值），以便在发生提交失败的情况下在后台重建可变索引。
**3.3  写入失败时禁用可变索引，直到一致性恢复**
在写入的时候提交失败，可变索引的默认行为是将index标记为禁止，并且在后台部分构建它们，然后当写入一致性被重新保证的时候将索引标记为可用状态。在这种一致性模式下，在重建二级索引时，写入数据表不会被阻塞。但是，在重建过程中，二级索引不会被查询使用。
**以下服务器端配置控制此行为：**
- 
**phoenix.index.failure.handling.rebuild**必须为true（缺省值），以便在发生提交失败的情况下在后台重建可变索引。
- 
**phoenix.index.failure.handling.rebuild.interval**控制服务器检查是否需要部分重建可变索引以赶上数据表更新的毫秒频率。默认值是10000或10秒。
- 
**phoenix.index.failure.handling.rebuild.overlap.time**控制执行部分重建时从发生故障的时间戳开始返回的毫秒数。默认值是1。
**3.4  写入失败时禁用可变索引，手动重建**
这是可变二级索引的最低一致性水平。在这种情况下，当写入二级索引失败时，索引将被标记为禁用，并且手动重建所需的索引以使其再次被查询使用。
**以下服务器端配置控制此行为：**
如果提交失败，**phoenix.index.failure.handling.rebuild**必须设置为false，以禁止在后台重建可变索引。
**配置**
非事务，可变索引需要在regionserver和master上运行特殊的配置=phoenix保证在你使能可变索引的时候这些配置正确设置。如果未设置正确的属性，则将无法使用二级索引。将这些设置添加到您的hbase-site.xml后，您需要执行集群的滚动重新启动。
您将需要将以下参数添加到每个regionserver上的hbase-site.xml：
<property>
 <name>hbase.regionserver.wal.codec</name>
 <value>org.apache.hadoop.hbase.regionserver.wal.IndexedWALEditCodec</value>
</property>
上面的配置使能自定义WAL预写日志被写入，确保index的更新正确的写入或者重建。
<property>
 <name>hbase.region.server.rpc.scheduler.factory.class</name>
 <value>org.apache.hadoop.hbase.ipc.PhoenixRpcSchedulerFactory</value>
 <description>Factory to create the Phoenix RPC Scheduler that uses separate queues for index and metadata updates</description>
</property>
<property>
 <name>hbase.rpc.controllerfactory.class</name>
 <value>org.apache.hadoop.hbase.ipc.controller.ServerRpcControllerFactory</value>
 <description>Factory to create the Phoenix RPC Scheduler that uses separate queues for index and metadata updates</description>
</property>
通过确保索引更新的优先级高于数据更新，上述属性可防止在全局索引（HBase 0.98.4+和Phoenix 4.3.1+）的索引维护过程中发生死锁。它还通过确保元数据rpc调用比数据rpc调用具有更高的优先级来防止死锁。
从Phoenix 4.8.0开始，不需要更改配置就可以使用本地索引。在Phoenix 4.7及更低版本中，主服务器节点和区域服务器节点上的服务器端hbase-site.xml需要进行以下配置更改：
<property>
 <name>hbase.master.loadbalancer.class</name>
 <value>org.apache.phoenix.hbase.index.balancer.IndexLoadBalancer</value>
</property>
<property>
 <name>hbase.coprocessor.master.classes</name>
 <value>org.apache.phoenix.hbase.index.master.IndexMasterObserver</value>
</property>
<property>
 <name>hbase.coprocessor.regionserver.classes</name>
 <value>org.apache.hadoop.hbase.regionserver.LocalIndexMerger</value>
</property>
**升级4.8.0之前创建的本地索引**
在服务器上将Phoenix升级到4.8.0以上版本时，如果存在，请从hbase-site.xml中除去以上三个与本地索引相关的配置。从客户端，我们支持在线（在初始化来自4.8.0+版本的phoenix客户端的连接时）和离线（使用psql工具）在4.8.0之前创建的本地索引的升级。作为升级的一部分，我们在ASYNC模式下重新创建本地索引。升级后用户需要使用IndexTool建立索引。
在升级之后使用客户端配置。
- 
phoenix.client.localIndexUpgrade
- 
它的值是true，意味着在线升级，false意味着离线升级。
- 
默认值：true
- 
命令使用psql工具$ psql [zookeeper] -l运行离线升级
**调优**
索引是相当快的。不过，为了优化您的特定环境和工作负载，您可以调整几个属性。以下所有参数必须在hbase-site.xml中设置- 对于整个集群和所有索引表，以及在同一台服务器上的所有区域上都是如此（例如，一台服务器也不会一次写入许多不同的索引表）。
- 
**index.builder.threads.max**
- 
用于从主表更新构建索引更新的线程数
- 
增加此值克服了从底层HRegion读取当前行状态的瓶颈。调整这个值太高，只会导致HRegion瓶颈，因为它将无法处理太多的并发扫描请求，以及引入线程切换的问题。
- 
默认：10
**2. index.builder.threads.keepalivetime**
- 
在构建器线程池中的线程过期后的的时间（以秒为单位）。
- 
在这段时间之后，未使用的线程立即被释放，而不是保留核心线程（尽管这是最后一个小问题，因为表预计将保持相当恒定的写负载），但同时允许我们在没有看到预期负载的情况下删除线程。
- 
默认：60
**3.index.writer.threads.max**
- 
写入目标索引表时使用的线程数。
- 
并行化的第一级，基于每个表 - 它应该大致对应于索引表的数量
- 
默认：10
**4.index.writer.threads.keepalivetime**
- 
写入程序线程池中线程过期后的时间（以秒为单位）。
- 
无用的线程会在这段时间后立即释放，而不会保留核心线程（尽管这最后一个小问题是因为表预计会承受相当恒定的写入负载），但同时允许我们在没有看到预期负载的情况下删除线程。
- 
默认：60
**5. hbase.htable.threads.max**
- 
HTable可用于写入的每个索引的线程数。
- 
增加这个允许更多的并发索引更新（例如跨批次），从而使得整体吞吐量较高。
- 
默认：2,147,483,647
**6.hbase.htable.threads.keepalivetime**
- 
在HTable的线程池中使线程过期之后的时间（以秒为单位）。
- 
使用“直接切换”方法，只有必要时才会创建新线程，并且将会无限增长。这可能是坏的，但HTables只能创建与区域服务器一样多的Runnables。因此，在添加新的regionserver时也会进行缩放。
- 
默认：60
**7.index.tablefactory.cache.size**
- 
我们应该保留在缓存中的索引HTable的数量。
- 
增加这个数字可以确保我们不需要为每次尝试写入索引表而重新创建一个HTable。相反，如果此值设置得太高，则可能会看到内存压力。
- 
默认：10
**8.org.apache.phoenix.regionserver.index.priority.min**
- 
指定索引优先级的范围的最小（包含）值。
- 
默认值：1000
**9.org.apache.phoenix.regionserver.index.priority.max**
- 
用于指定索引优先级可能位于的范围的最大（不包括）值。
- 
索引最小/最大范围内的更高优先级不意味着更新被更早地处理。
- 
默认：1050
**10.org.apache.phoenix.regionserver.index.handler.count**
- 
为全局索引维护提供索引写请求时要使用的线程数。
- 
尽管线程的实际数量是由Max（调用队列数，处理器数）决定的，其中调用队列数由标准HBase配置决定。为了进一步调整队列，你可以调整标准的rpc队列长度参数（目前，没有特别的索引队列的旋钮），具体是ipc.server.max.callqueue.length和ipc.server.callqueue.handler.factor。有关更多详细信息，请参阅HBase参考指南。
- 
默认：30
**性能**
Phoenix性能测试结果：https://phoenix-bin.github.io/client/performance/latest.htm。这是基于默认值的一般性能测试 - 结果将根据硬件规格和个人配置而变化。
索引审查工具
使用Phoenix 4.12，现在有一个工具可以运行MapReduce作业来验证索引表是否对数据表有效。在表中查找孤行的唯一方法是扫描表中的所有行，并在另一个表中查找相应的行。因此，该工具可以使用数据表或索引表作为“源”表，而另一个作为“目标”表运行。该工具将所有无效行写入文件或输出表PHOENIX_INDEX_SCRUTINY。无效行是在目标表中没有相应行或在目标表中具有不正确值的源行（即覆盖的列值）。无效行是在目标表中没有相应行或在目标表中具有不正确值的源行（即覆盖的列值）。
该工具具有跟踪其状态的工作计数器。VALID_ROW_COUNT，INVALID_ROW_COUNT，BAD_COVERED_COL_VAL_COUNT。请注意，无效的行 - 坏的行数=孤行的数量。这些计数器连同其他作业元数据一起被写入表PHOENIX_INDEX_SCRUTINY_METADATA。这些计数器连同其他作业元数据一起被写入表PHOENIX_INDEX_SCRUTINY_METADATA。
索引审查工具可以通过hbase命令（以hbase / bin）启动，如下所示：
hbase org.apache.phoenix.mapreduce.index.IndexScrutinyTool -dt my_table -it my_index -o
也可以使用phoenix-core或phoenix-server jar从Hadoop运行，如下所示：
HADOOP_CLASSPATH = $（hbase
 mapredcp）hadoop jar phoenix- <version> -server.jar org.apache.phoenix.mapreduce.index.IndexScrutinyTool
 -dt my_table -it my_index -o
默认情况下，启动两个mapreduce作业，一个以数据表作为源表，另一个以索引表作为源表。
索引检查工具可以使用以下参数：
|参数|描述|
|----|----|
|-dt,–data-table|数据表名(必填)|
|-it,–index-table|索引表名称(必填)|
|-s,–schema|Phoenix表schema名(可选)|
|-src,–source|DATA_TABLE_SOURCE，INDEX_TABLE_SOURCE或BOTH。默认为BOTH|
|-o,–output|是否输出无效的行。默认关闭|
|-of,–output-format|TABLE或FILE输出格式。默认为TABLE|
|-om,–output-max|每个mapper输出的最大无效行数。默认为1M|
|-op,–output-path|对于FILE输出格式，写入文件的HDFS目录|
|-t,–time|以millis为单位的时间戳进行审查。这一点很重要，这样一来正在写入的数据不会被审查了。默认为当前时间减去60秒|
|-b,–batch-size|一次比较的行数|
**限制**
如果在执行审查时正在更新或删除行，则该工具可能会给出不一致的误报（PHOENIX-4277）。
审查工具（PHOENIX-4270）不支持快照读取。
**推荐阅读：**
1，[实战phoenix](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484253&idx=1&sn=2d1a9100dae4abae0efc2a4fea6fded5&chksm=9f38e075a84f69639b03aaa5491c9d9016a3e9057a37f3bf3dc892159fcde39dbb361350d7ac&scene=21#wechat_redirect)
2，[Phoenix边讲架构边调优](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484257&idx=1&sn=256c22a50424342c74cd2fe417ad2b13&chksm=9f38e049a84f695fcf35d445d8b3ed2331b6b72fc7d4b93edc0bad8edce849f66bafa2e09429&scene=21#wechat_redirect)
3，[论Spark
 Streaming的数据可靠性和一致性](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484247&idx=1&sn=f9a65c44ea7186012d72dee25526ec77&chksm=9f38e07fa84f69699c3599dbf06753b7b4b99148fc076b1ea6896aef466731c012a4831a4607&scene=21#wechat_redirect)
4，[Hbase源码系列之regionserver应答数据请求服务设计](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483723&idx=1&sn=177f7efd2d9155614f40bc46c14c3104&chksm=9f38e263a84f6b756436e068ebb00afc4a81a476d61dbf03711dae062969751ce471aa2219f1&scene=21#wechat_redirect)
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

