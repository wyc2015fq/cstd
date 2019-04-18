# Elasticsearch之elasticsearch5.x 新特性 - z69183787的专栏 - CSDN博客
2017年04月11日 10:49:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3207
[http://www.cnblogs.com/zlslch/p/6619089.html](http://www.cnblogs.com/zlslch/p/6619089.html)
　其实，elasticsearch5.x 和 elasticsearch2.x 并不区别很大。
　　是因为，ELK里之前版本各种很混乱，直接升级到5.0了。
　　其实，elasticsearch5.x 按理来说是elasticsearch3.x，只是为了跟随ELK整体版本的统一。
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235135596-1837440713.png)
下面给大家介绍一下 5.0 版里面的一些新的特性和改进
　　5.0？ 天啦噜，你是不是觉得版本跳的太快了。
　　好吧，先来说说背后的原因吧。
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235159643-512355029.png)
　　相信大家都听说 ELK 吧，是 Elasticsearch 、 Logstash 、 Kibana 三个产品的首字母缩写，现在 Elastic 又新增了一个新的开源项目成员： Beats。
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235222299-1801587901.png)
 　　有人建议以后这么叫： ELKB ？
　　为了未来更好的扩展性:) ELKBS？ELKBSU？.....
　　所以我们打算将产品线命名为 ElasticStack
　　同时由于现在的版本比较混乱，每个产品的版本号都不一样， Elasticsearch和Logstash目前是2.3.4；Kibana是4.5.3；Beats是1.2.3；
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235257611-1406158587.png)
　　版本号太乱了有没有，什么版本的 ES 用什么版本的 Kibana ？有没有兼容性问题？
所以我们打算将这些的产品版本号也统一一下，即 v5.0 ，为什么是 5.0 ，因为 Kibana 都 4.x 了，下个版本就只能是 5.0 了，其他产品就跟着跳跃一把，第一个 5.0 正式版将在今年的秋季发布，目前最新的测试版本是： 5.0 Alpha 4。
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235341783-190444500.png)
　　目前各团队正在紧张的开发测试中，每天都有新的功能和改进，本次分享主要介绍一下 Elasticsearch 的主要变化。
Elasticsearch5.0新增功能
　　首先来看看 5.0 里面都引入了哪些新的功能吧。
#### 1、首先看看跟性能有关的
#### 　　1.1 第一个就是Lucene 6.x 的支持。
　　Elasticsearch5.0率先集成了Lucene6版本，其中最重要的特性就是 Dimensional Point Fields，多维浮点字段，ES里面相关的字段如date, numeric，ip 和 Geospatial 都将大大提升性能。
　　这么说吧，磁盘空间少一半；索引时间少一半；查询性能提升25%；IPV6也支持了。
　　为什么快，底层使用的是Block k-d trees，核心思想是将数字类型编码成定长的字节数组，对定长的字节数组内容进行编码排序，然后来构建二叉树，然后依次递归构建，目前底层支持8个维度和最多每个维度16个字节，基本满足大部分场景。
　　说了这么多，看图比较直接。
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235432549-997243375.png)
　　图中从 2015 /10/32  total bytes 飙升是因为 es 启用了 docvalues ，我们关注红线，最近的引入新的数据结构之后，红色的索引大小只有原来的一半大小。
　　索引小了之后， merge 的时间也响应的减少了，看下图：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235455861-885058949.png)
 　　相应的 Java 堆内存占用只原来的一半：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235509721-1185550732.png)
1.2  再看看 索引的性能 ，也是飙升：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235522721-1949146733.png)
　　当然 Lucene6 里面还有很多优化和改进，这里没有一一列举。
　1.3  我们再看看索引性能方面的其他优化。
　　ES5.0在Internal engine级别移除了用于避免同一文档并发更新的竞争锁，带来15%-20%的性能提升 #18060 。
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235606986-1682862552.png)
　　　　以上截图来自 ES 的每日持续性能监控： https://benchmarks.elastic.co/index.html
#### 　　1.4 另一个 和 aggregation 的改进也是非常大， Instant Aggregations。
　　Elasticsearch已经在Shard层面提供了Aggregation缓存，如果你的数据没有变化，ES能够直接返回上次的缓存结果，但是有一个场景比较特殊，就是 date histogram，大家kibana上面的条件是不是经常设置的相对时间，如：from:now-30d to:now，好吧，now是一个变量，每时每刻都在变，所以query条件一直在变，缓存也就是没有利用起来。
　　经过一年时间大量的重构，现在可以做到对查询做到灵活的重写：
　　　　首先，`now`关键字最终会被重写成具体的值；
　　　　其次 ， 每个shard会根据自己的数据的范围来重写查询为 `match_all`或者是`match_none`查询，所以现在的查询能够被有效的缓存，并且只有个别数据有变化的Shard才需要重新计算，大大提升查询速度。
1.5   另外再看看和Scroll相关的吧。
　　现在新增了一个：Sliced Scroll类型
　　用过Scroll接口吧，很慢？如果你数据量很大，用Scroll遍历数据那确实是接受不了，现在Scroll接口可以并发来进行数据遍历了。
　　每个Scroll请求，可以分成多个Slice请求，可以理解为切片，各Slice独立并行，利用Scroll重建或者遍历要快很多倍。
　　看看这个demo
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235721799-48686168.png)
　　可以看到两个 scroll 请求， id 分别是 0 和 1 ， max 是最大可支持的并行任务，可以各自独立进行数据的遍历获取。
2、我们再看看es在查询优化这块做的工作
#### 　　2.1 新增了一个Profile API。
#https://www.elastic.co/guide/en/elasticsearch/reference/master/search-profile.html#_usage_3
　　都说要致富先修路，要调优当然需要先监控啦，elasticsearch在很多层面都提供了stats方便你来监控调优，但是还不够，其实很多情况下查询速度慢很大一部分原因是糟糕的查询引起的，玩过SQL的人都知道，数据库服务的执行计划（execution plan）非常有用，可以看到那些查询走没走索引和执行时间，用来调优，elasticsearch现在提供了Profile API来进行查询的优化，只需要在查询的时候开启profile：true就可以了，一个查询执行过程中的每个组件的性能消耗都能收集到。
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235753908-1505539590.png)
　　那个子查询耗时多少，占比多少，一目了然。
　　同时支持search和aggregation的profile。
　　还有一个和翻页相关的问题，就是深度分页 ，是个老大难的问题，因为需要全局排序（ number_of_shards * (from + size) ），所以需要消耗大量内存，以前的 es 没有限制，有些同学翻到几千页发现 es 直接内存溢出挂了，后面 elasticsearch 加上了限制， from+size 不能超过 1w 条，并且如果需要深度翻页，建议使用 scroll 来做。
　　但是 scroll 有几个问题，第一个是没有顺序，直接从底层 segment 进行遍历读取，第二个实时性没法保证， scroll 操作有状态， es 会维持 scroll 请求的上下文一段时间，超时后才释放，另外你在 scroll 过程中对索引数据进行了修改了，这个时候 scroll接口是拿不到的，灵活性较差， 现在有一个新的 Search After 机制，其实和 scroll 类似，也是游标的机制，它的原理是对文档按照多个字段进行排序，然后利用上一个结果的最后一个文档作为起始值，拿 size 个文档，一般我们建议使用 _uid 这个字段，它的值是唯一的 id 。
#（Search After
https://github.com/elastic/elasticsearch/blob/148f9af5857f287666aead37f249f204a870ab39/docs/reference/search/request/search-after.asciidoc ）
　　来看一个Search After 的demo 吧，比较直观的理解一下：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235818002-1726839844.png)
　　上面的 demo ， search_after 后面带的两个参数，就是 sort 的两个结果。
　　根据你的排序条件来的，三个排序条件，就传三个参数。
3、再看看跟索引与分片管理相关的新功能吧。
#### 　　3.1 新增了一个 Shrink  API
#https://www.elastic.co/guide/en/elasticsearch/reference/master/indices-shrink-index.html#_shrinking_an_index
　　相信大家都知道elasticsearch索引的shard数是固定的，设置好了之后不能修改，如果发现shard太多或者太少的问题，之前如果要设置Elasticsearch的分片数，只能在创建索引的时候设置好，并且数据进来了之后就不能进行修改，如果要修改，只能重建索引。
　　现在有了Shrink接口，它可将分片数进行收缩成它的因数，如之前你是15个分片，你可以收缩成5个或者3个又或者1个，那么我们就可以想象成这样一种场景，在写入压力非常大的收集阶段，设置足够多的索引，充分利用shard的并行写能力，索引写完之后收缩成更少的shard，提高查询性能。
　　这里是一个API调用的例子
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235846877-1512182075.png)
　　上面的例子对 my_source_index 伸缩成一个分片的 my_targe_index, 使用了最佳压缩。
　　有人肯定会问慢不慢？非常快！ Shrink的过程会借助操作系统的Hardlink进行索引文件的链接，这个操作是非常快的，毫秒级Shrink就可收缩完成，当然windows不支持hard link，需要拷贝文件，可能就会很慢了。
　　再来看另外一个比较有意思的新特性，除了有意思，当然还很强大。
#### 　　3.2 新增了一个Rollover API。
https://www.elastic.co/guide/en/elasticsearch/reference/master/indices-rollover-index.html#indices-rollover-index
　　前面说的这种场景对于日志类的数据非常有用，一般我们按天来对索引进行分割（数据量更大还能进一步拆分），我们以前是在程序里设置一个自动生成索引的模板，大家用过logstash应该就记得有这么一个模板logstash-[YYYY-MM-DD]这样的模板，现在es5.0里面提供了一个更加简单的方式：Rollover API
　　API调用方式如下：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235912330-417331800.png)
　　从上面可以看到，首先创建一个 logs-0001 的索引，它有一个别名是 logs_write, 然后我们给这个 logs_write 创建了一个 rollover 规则，即这个索引文档不超过 1000 个或者最多保存 7 天的数据，超过会自动切换别名到 logs-0002, 你也可以设置索引的 setting 、 mapping 等参数 , 剩下的 es 会自动帮你处理。这个特性对于存放日志数据的场景是极为友好的。
#### 　　3.3  新增：Reindex。
　　另外关于索引数据，大家之前经常重建，数据源在各种场景，重建起来很是头痛，那就不得不说说现在新加的Reindex接口了，Reindex可以直接在Elasticsearch集群里面对数据进行重建，如果你的mapping因为修改而需要重建，又或者索引设置修改需要重建的时候，借助Reindex可以很方便的异步进行重建，并且支持跨集群间的数据迁移。
　　比如按天创建的索引可以定期重建合并到以月为单位的索引里面去。
　　当然索引里面要启用_source。
　　来看看这个demo吧，重建过程中，还能对数据就行加工。
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170325235938127-1989503956.png)
　　3.4  再看看跟Java开发者最相关的吧，就是 RestClient了
　　5.0里面提供了第一个Java原生的REST客户端SDK，相比之前的TransportClient，版本依赖绑定，集群升级麻烦，不支持跨Java版本的调用等问题，新的基于HTTP协议的客户端对Elasticsearch的依赖解耦，没有jar包冲突，提供了集群节点自动发现、日志处理、节点请求失败自动进行请求轮询，充分发挥Elasticsearch的高可用能力，并且性能不相上下。 #19055 。
4、然后我们再看看其他的特性吧：
#### 　　4.1 新增了一个 Wait for refresh 功能。
　　简单来说相当于是提供了文档级别的Refresh： https://www.elastic.co/guide/en/elasticsearch/reference/master/docs-refresh.html。
　　索引操作新增refresh参数，大家知道elasticsearch可以设置refresh时间来保证数据的实时性，refresh时间过于频繁会造成很大的开销，太小会造成数据的延时，之前提供了索引层面的_refresh接口，但是这个接口工作在索引层面，我们不建议频繁去调用，如果你有需要修改了某个文档，需要客户端实时可见怎么办？
　　在 5.0中，Index、Bulk、Delete、Update这些数据新增和修改的接口能够在单个文档层面进行refresh控制了，有两种方案可选，一种是创建一个很小的段，然后进行刷新保证可见和消耗一定的开销，另外一种是请求等待es的定期refresh之后再返回。
　　调用例子：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170326000005315-1224242494.png)
#### 　　4.2 # 新增： Ingest Node #
　　#https://www.elastic.co/guide/en/elasticsearch/reference/master/ingest.html#
　　再一个比较重要的特性就是IngestNode了，大家之前如果需要对数据进行加工，都是在索引之前进行处理，比如logstash可以对日志进行结构化和转换，现在直接在es就可以处理了，目前es提供了一些常用的诸如convert、grok之类的处理器，在使用的时候，先定义一个pipeline管道，里面设置文档的加工逻辑，在建索引的时候指定pipeline名称，那么这个索引就会按照预先定义好的pipeline来处理了；
　　Demo again：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170326000021783-1467409645.png)
　　上图首先创建了一个名为my-pipeline-id的处理管道，然后接下来的索引操作就可以直接使用这个管道来对foo字段进行操作了，上面的例子是设置foo字段为bar值。
　　上面的还不太酷，我们再来看另外一个例子，现在有这么一条原始的日志，内容如下：
{
"message": "55.3.244.1 GET /index.html 15824 0.043”
}
　　google之后得知其Grok的pattern如下：）
%{IP:client} %{WORD:method} %{URIPATHPARAM:request} %{NUMBER:bytes} %{NUMBER:duration}
　　那么我们使用Ingest就可以这么定义一个pipeline：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170326000038596-600212832.png)
　　那么通过我们的 pipeline 处理之后的文档长什么样呢，我们获取这个文档的内容看看：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170326000107611-2004280656.png)
　　很明显，原始字段 message 被拆分成了更加结构化的对象了。
5、再看看脚本方面的改变
#### 　　5.1  #新增Painless Scripting#
　　还记得Groove脚本的漏洞吧，Groove脚本开启之后，如果被人误用可能带来的漏洞，为什么呢，主要是这些外部的脚本引擎太过于强大，什么都能做，用不好或者设置不当就会引起安全风险，基于安全和性能方面，我们自己开发了一个新的脚本引擎，名字就叫Painless，顾名思义，简单安全，无痛使用，和Groove的沙盒机制不一样，Painless使用白名单来限制函数与字段的访问，针对es的场景来进行优化，只做es数据的操作，更加轻量级，速度要快好几倍，并且支持Java静态类型，语法保持Groove类似，还支持Java的lambda表达式。
　　我们对比一下性能，看下图
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170326000134190-1658627867.png)
　　Groovy 是弱弱的绿色的那根。
　　再看看如何使用：
def first = input.doc.first_name.0;
def last  = input.doc.last_name.0;
return first + " " + last;
　　是不是和之前的写法差不多
　　或者还可以是强类型（10倍速度于上面的动态类型）
String first = (String)((List)((Map)input.get("doc")).get("first_name")).get(0);
String last  = (String)((List)((Map)input.get("doc")).get("last_name")).get(0);
return first + " " + last;
　　脚本可以在很多地方使用，比如搜索自定义评分；更新时对字段进行加工等
　　如：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170326000157393-1176168364.png)
 6、再来看看基础架构方面的变化
#### 　　6.1 新增：Task Manager
　　这个是5.0 引入任务调度管理机制，用来做 离线任务的管理，比如长时间运行的reindex和update_by_query等都是运行在TaskManager机制之上的，并且任务是可管理的，你可以随时cancel掉，并且任务状态持久化，支持故障恢复；
#### 　　6.2 还新增一个： Depreated logging
　　大家在用ES的时候，其实有些接口可能以及打上了Depreated标签，即废弃了，在将来的某个版本中就会移除，你当前能用是因为一般废弃的接口都不会立即移除，给足够的时间迁移，但是也是需要知道哪些不能用了，要改应用代码了，所以现在有了Depreated日志，当打开这个日志之后，你调用的接口如果已经是废弃的接口，就会记录下日志，那么接下来的事情你就知道你应该怎么做了。
#### 　　6.3 新增 : Cluster allocation explain API
　　『谁能给我一个shard不能分配的理由』，现在有了，大家如果之前遇到过分片不能正常分配的问题，但是不知道是什么原因，只能尝试手动路由或者重启节点，但是不一定能解决，其实里面有很多原因，现在提供的这个explain接口就是告诉你目前为什么不能正常分配的原因，方便你去解决。
#### 　　6.4 另外在数据结构这块，新增 : half_float 类型
https://www.elastic.co/guide/en/elasticsearch/reference/master/number.html
　　只使用 16 位 足够满足大部分存储监控数值类型的场景，支持范围：2负24次方 到 65504，但是只占用float一半的存储空间。
#### 　　6.5 Aggregation 新增 : Matrix Stats Aggregation # 18300
金融领域非常有用的，可计算多个向量元素协方差矩阵、相关系数矩阵等等
#### 　　6.6 另外一个重要的特性：为索引写操作添加顺序号 # 10708
　　大家知道es是在primary上写完然后同步写副本，这些请求都是并发的，虽然可以通过version来控制冲突，
　　但是没法保证其他副本的操作顺序，通过写的时候产生顺序号，并且在本地也写入checkpoint来记录操作点，
　　这样在副本恢复的时候也可以知道当前副本的数据位置，而只需要从指定的数据开始恢复就行了，而不是像以前的粗暴的做完整的文件同步 ，另外这些顺序号也是持久化的，重启后也可以快速恢复副本信息，想想以前的大量无用拷贝吧和来回倒腾数据吧。
7、Elasticsearch5.0其他方面的改进
7.1 我们再看看 mapping 这块的改进 吧。
#### 　　　　引入新的字段类型 Text/Keyword 来替换  String
　　以前的string类型被分成Text和Keyword两种类型，keyword类型的数据只能完全匹配，适合那些不需要分词的数据，
　　对过滤、聚合非常友好，text当然就是全文检索需要分词的字段类型了。将类型分开的好处就是使用起来更加简单清晰，以前需要设置analyzer和index，并且有很多都是自定义的分词器，从名称根本看不出来到底分词没有，用起来很麻烦。
　　另外string类型暂时还在的，6.0会移除。
#### 　　7.2 还有关于 Index Settings 的改进
　　Elasticsearch的配置实在太多，在以前的版本间，还移除过很多无用的配置，经常弄错有没有？
　　现在，配置验证更加严格和保证原子性，如果其中一项失败，那个整个都会更新请求都会失败，不会一半成功一半失败。下面主要说两点：
　　　　1.设置可以重设会默认值，只需要设置为 `null`即可
　　　　2.获取设置接口新增参数`?include_defaults`,可以直接返回所有设置和默认值
#### 　　7.3 集群处理的改进 : Deleted Index Tombstones
　　在以前的es版本中，如果你的旧节点包含了部分索引数据，但是这个索引可能后面都已经删掉了，你启动这个节点之后，会把索引重新加到集群中，是不是觉得有点阴魂不散，现在es5.0会在集群状态信息里面保留500个删除的索引信息，所以如果发现这个索引是已经删除过的就会自动清理，不会再重复加进来了。
　　文档对象的改进 : 字段名重新支持英文句号，再 2.0 的时候移除过 dot 在字段名中的支持，现在问题解决了，又重新支持了。
　　es会认为下面两个文档的内容一样：
![](http://images2015.cnblogs.com/blog/855959/201703/855959-20170326000251393-166746379.png)
 　　7.4 还有其他的一些改进
　　Cluster state 的修改现在会和所有节点进行 ack 确认。
　　Shard 的一个副本如果失败了， Primary 标记失败的时候会和 Master 节点确认完毕再返回。
　　使用 UUID 来作为索引的物理的路径名，有很多好处，避免命名的冲突。
　　_timestamp 和 _ttl 已经移除，需要在 Ingest 或者程序端处理。
　　ES 可直接用 HDFS 来进行备份还原（ Snapshot/Restore ）了 #15191 。
　　Delete-by-query 和 Update-by-query 重新回到 core ，以前是插件，现在可以直接使用了，也是构建在 Reindex 机制之上。(es1.x版本是直接支持，在es2.x中提取为插件，5.x继续回归直接支持)
　　HTTP 请求默认支持压缩，当然 http 调用端需要在 header 信息里面传对应的支持信息。
　　创建索引不会再让集群变红了，不会因为这个卡死集群了。
　　默认使用 BM25 评分算法，效果更佳，之前是 TF/IDF。
　　快照 Snapshots 添加 UUID 解决冲突 #18156 。
　　限制索引请求大小，避免大量并发请求压垮 ES #16011。
　　限制单个请求的 shards 数量，默认 1000 个 #17396。
　　移除 site plugins ，就是说 head 、 bigdesk 都不能直接装 es 里面了，不过可以部署独立站点（反正都是静态文件）或开发 kibana 插件 #16038 。
　　允许现有 parent 类型新增 child 类型 #17956。
　　这个功能对于使用parent-child特性的人应该非常有用。
　　支持分号（；）来分割 url 参数，与符号（ & ）一样 #18175 。
　　比如下面这个例子：
curl http://localhost:9200/_cluster/health?level=indices;pretty=true
　　好吧，貌似很多，其实上面说的还只是众多特性和改进的一部分， es5.0 做了非常非常多工作，本来还打算讲讲 bug 修复的，但是太多了，时间有限， 一些重要的 bug在 2.x 都已经第一时间解决了。
8、大家可以查看下面的链接了解更多更详细的更新日志
https://www.elastic.co/guide/en/elasticsearch/reference/master/release-notes-5.0.0-alpha1-2x.html
https://www.elastic.co/guide/en/elasticsearch/reference/master/release-notes-5.0.0-alpha1.html
https://www.elastic.co/guide/en/elasticsearch/reference/master/release-notes-5.0.0-alpha2.html
https://www.elastic.co/guide/en/elasticsearch/reference/master/release-notes-5.0.0-alpha3.html
https://www.elastic.co/guide/en/elasticsearch/reference/master/release-notes-5.0.0-alpha4.html
下载体验最新的版本 ： https://www.elastic.co/v5
升级向导：https://github.com/elastic/elasticsearch-migration/blob/2.x/README.asciidoc
