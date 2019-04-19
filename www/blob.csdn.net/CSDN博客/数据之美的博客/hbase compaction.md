# hbase compaction - 数据之美的博客 - CSDN博客
2018年10月30日 10:25:10[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：179
## hbase compaction
*最近发现在habse中，hbase.hregion.majorcompaction已经设置为0，日志中发现还存在major compaction的发生。将相关资料整理如下。*
compaction 分为minor compaction和major compaction。major compaction 把所有hfile文件compact成一个单独的文件。在执行compac检查时,系统自动决定运行哪种合并。
*触发compaction检查的情况：*
- 
memstore被刷写到磁盘后会触发检查。*compaction操作的最根本原因就是来自于flush，memstore flush会产生越来越多的hfile文件，因此需要compaction*
- 
shell 命令compact之后触发检查
- 
shell命令major compaction之后触发
- 
相应API在被调用后触发检查
- 
一个异步的后台进程触发(CompactionChecker类实现) --> 这个周期由hbase.server.thread.wakefrequency参数控制
*发生major compaction的情况：*
- 
shell命令major_compoact将强制运行major compaction
- 
majorCompact()API将强制运行major合并
- 
hbase.hregion.majorcomapction达到周期
- 
当minor合并包括所有的存储文件，且所有的文件均未达到设置的每次compaction的最大文件数时，minor 合并被提升为major compaction
除非用户使用shell命令major_compoact或者使用majorCompact()API，将强制运行major caompaction，否则服务器会首先检查上次运行到现在是否达到 hbase.hregion.majorcomapction（默认24小时）指定的时限。如果还没有达到此合并的执行周期，系统会选择minor合并执行。
针对测试中情况来看：可排除发生major compaction的1或2情况，hbase.hregion.majorcomapction设置为0也可配除周期性检查引起的major compact。 所以很有可能是minor合并包含了所有的hfile文件，提升成为了major compact。
日志也证明了这一点：
Major compaction相关日志：
2018-10-17 16:33:35,222 DEBUG org.apache.hadoop.hbase.regionserver.CompactSplitThread: Small Compaction requested: system; Because: MemStoreFlusher.0; compaction_queue=(0:1), split_queue=0, merge_queue=0
2018-10-17 16:33:35,222 DEBUG org.apache.hadoop.hbase.regionserver.compactions.RatioBasedCompactionPolicy: Selecting compaction from 7 store files, 0 compacting, 7 eligible, 300 blocking
2018-10-17 16:33:35,222 DEBUG org.apache.hadoop.hbase.regionserver.compactions.ExploringCompactionPolicy: Exploring compaction algorithm has selected 7 files of size 1834141505 starting at candidate #0 after considering 6 permutations with 5 in ratio
2018-10-17 16:33:35,222 DEBUG org.apache.hadoop.hbase.regionserver.HStore: 98c13fa94035e7a58f959e3ce7ad2fb5 - f: Initiating major compaction
2018-10-17 16:33:35,222 INFO org.apache.hadoop.hbase.regionserver.HRegion: Starting compaction on f in region .......98c13fa94035e7a58f959e3ce7ad2fb5.
2018-10-17 16:33:35,223 INFO org.apache.hadoop.hbase.regionserver.HStore: Starting compaction of 7 file(s) in f of .......98c13fa94035e7a58f959e3ce7ad2fb5. into tmpdir=hdfs://....../98c13fa94035e7a58f959e3ce7ad2fb5/.tmp, totalSize=1.7 G
minor compaction相关日志：
2018-10-17 16:36:57,977 DEBUG org.apache.hadoop.hbase.regionserver.CompactSplitThread: Small Compaction requested: system; Because: MemStoreFlusher.1; compaction_queue=(0:0), split_queue=0, merge_queue=0
2018-10-17 16:36:57,977 DEBUG org.apache.hadoop.hbase.regionserver.compactions.RatioBasedCompactionPolicy: Selecting compaction from 7 store files, 0 compacting, 7 eligible, 300 blocking
2018-10-17 16:36:57,977 DEBUG org.apache.hadoop.hbase.regionserver.compactions.ExploringCompactionPolicy: Exploring compaction algorithm has selected 5 files of size 219948132 starting at candidate #2 after considering 6 permutations with 1 in ratio
2018-10-17 16:36:57,977 DEBUG org.apache.hadoop.hbase.regionserver.HStore: 98c13fa94035e7a58f959e3ce7ad2fb5 - f: Initiating minor compaction
2018-10-17 16:36:57,978 INFO org.apache.hadoop.hbase.regionserver.HRegion: Starting compaction on f in region .......98c13fa94035e7a58f959e3ce7ad2fb5.
2018-10-17 16:36:57,978 INFO org.apache.hadoop.hbase.regionserver.HStore: Starting compaction of 5 file(s) in f of .......98c13fa94035e7a58f959e3ce7ad2fb5. into tmpdir=hdfs://....../98c13fa94035e7a58f959e3ce7ad2fb5/.tmp, totalSize=209.8 M
有一个点很有意思：测试中可能因为写入压力较大，compaction_queue过长，似乎major compaction也会降级为monir:
2018-10-18 11:01:08,676 DEBUG org.apache.hadoop.hbase.regionserver.CompactSplitThread: CompactSplitThread Status: compaction_queue=(368:0), split_queue=0, merge_queue=0
2018-10-18 11:01:08,677 DEBUG org.apache.hadoop.hbase.regionserver.compactions.RatioBasedCompactionPolicy: Selecting compaction from 9 store files, 0 compacting, 9 eligible, 300 blocking
2018-10-18 11:01:08,677 DEBUG org.apache.hadoop.hbase.regionserver.compactions.RatioBasedCompactionPolicy: Too many admissible files. Excluding 4 files from compaction candidates *#从日志可以观察到这里是主动排除掉几个#*
2018-10-18 11:01:08,677 DEBUG org.apache.hadoop.hbase.regionserver.HStore: bb4c87d7684b09b7d5fbc3898cbf41c7 - f: Initiating minor compaction
2018-10-18 11:01:08,677 INFO org.apache.hadoop.hbase.regionserver.HRegion: Starting compaction on f in region .......bb4c87d7684b09b7d5fbc3898cbf41c7.
2018-10-18 11:01:08,677 INFO org.apache.hadoop.hbase.regionserver.HStore: Starting compaction of 5 file(s) in f of .......bb4c87d7684b09b7d5fbc3898cbf41c7. into tmpdir=hdfs://....../bb4c87d7684b09b7d5fbc3898cbf41c7/.tmp, totalSize=3.4 G
- 关于compaction 策略
选择合适的文件进行合并是整个compaction的核心，因为合并文件的大小以及其当前承载的IO数直接决定了compaction的效果。[http://www.nosqlnotes.com/technotes/hbase/flush-compaction/](http://www.nosqlnotes.com/technotes/hbase/flush-compaction/)[https://yq.aliyun.com/articles/209062](https://yq.aliyun.com/articles/209062)[http://hbasefly.com/2016/07/13/hbase-compaction-1/](http://hbasefly.com/2016/07/13/hbase-compaction-1/)[http://hbasefly.com/2016/07/25/hbase-compaction-2/](http://hbasefly.com/2016/07/25/hbase-compaction-2/)
HBase主要有两种minor策略：RatioBasedCompactionPolicy和ExploringCompactionPolicy：
1.RatioBasedCompactionPolicy
Ratio策略是0.94版本的默认策略。从老到新逐一扫描所有候选文件，满足其中条件之一便停止扫描： （1）当前文件大小 < 比它更新的所有文件大小总和 * ratio，其中ratio是一个可变的比例，在高峰期时ratio为1.2，非高峰期为5，也就是非高峰期允许compact更大的文件。那什么时候是高峰期，什么时候是非高峰期呢？用户可以配置参数hbase.offpeak.start.hour和hbase.offpeak.end.hour来设置高峰期 （2）当前所剩候选文件数 <= hbase.store.compaction.min（默认为3） 停止扫描后，待合并文件就选择出来了，即为当前扫描文件+比它更新的所有文件
2.ExploringCompactionPolicy
在0.98版本中，默认的compaction算法换成了ExploringCompactionPolicy，之前是RatioBasedCompactionPolicy。ExploringCompactionPolicy继承RatioBasedCompactionPolicy，重写了applyCompactionPolicy方法，applyCompactionPolicy是对minor compaction的选择文件的策略算法。具体请看[https://my.oschina.net/u/220934/blog/363270](https://my.oschina.net/u/220934/blog/363270)
3.Date Tiered Compaction
这是一个基于日期感知的存储文件compaction策略，有利于对时间序列数据的时间范围扫描。当希望读取有限的时间范围，特别是最近数据的扫描时可以考虑该算法。博客[http://hbasefly.com/2016/07/25/hbase-compaction-2/中提到HBase计划在2.0.0版本发布该策略，所以该策略支持版本应该较高。http://hbase.apache.org/book.html#ops.date.tiered](http://hbasefly.com/2016/07/25/hbase-compaction-2/%E4%B8%AD%E6%8F%90%E5%88%B0HBase%E8%AE%A1%E5%88%92%E5%9C%A82.0.0%E7%89%88%E6%9C%AC%E5%8F%91%E5%B8%83%E8%AF%A5%E7%AD%96%E7%95%A5%EF%BC%8C%E6%89%80%E4%BB%A5%E8%AF%A5%E7%AD%96%E7%95%A5%E6%94%AF%E6%8C%81%E7%89%88%E6%9C%AC%E5%BA%94%E8%AF%A5%E8%BE%83%E9%AB%98%E3%80%82http://hbase.apache.org/book.html#ops.date.tiered)
alter 'orders_table', CONFIGURATION => {'hbase.hstore.engine.class' => 'org.apache.hadoop.hbase.regionserver.DateTieredStoreEngine', 'hbase.hstore.blockingStoreFiles' => '60', 'hbase.hstore.compaction.min'=>'2', 'hbase.hstore.compaction.max'=>'60'}
4.Stripe Compaction （HBASE-7667）
stripe压缩是HBase 0.98中添加的一个实验特性，其目的是改善大区域或不均匀分布的行键的compaction。hbase官方指南建议大region或者Non-uniform keys使用。具体请看[http://hbase.apache.org/book.html#ops.stripe](http://hbase.apache.org/book.html#ops.stripe)
示例：
alter 'orders_table', CONFIGURATION => {'hbase.hstore.engine.class' => 'org.apache.hadoop.hbase.regionserver.StripeStoreEngine', 'hbase.hstore.blockingStoreFiles' => '100'}
其他或是后续还有很多提出的算法：
5.FIFO Compaction（HBASE-14468）
FIFO Compaction策略主要参考了rocksdb的实现，它会选择那些过期的数据文件，即该文件内所有数据都已经过期。因此，对应业务的列族必须设置TTL，否则肯定不适合该策略。需要注意的是，该策略只做这么一件事情：收集所有已经过期的文件并删除。修复版本为：1.2.0, 1.3.0, 0.98.17, 2.0.0。具体可以看[https://issues.apache.org/jira/browse/HBASE-14468](https://issues.apache.org/jira/browse/HBASE-14468)
create 'x',{NAME=>'y', TTL=>'30'}, {CONFIGURATION => {'hbase.hstore.defaultengine.compactionpolicy.class' => 'org.apache.hadoop.hbase.regionserver.compactions.FIFOCompactionPolicy', 'hbase.hstore.blockingStoreFiles' => 1000}}
6.Tier-Based Compaction（HBASE-7055）（HBASE-14477）
该方案会根据候选文件的新老程度将其分为多个不同的等级，每个等级都有对应等级的参数，比如参数Compation Ratio，表示该等级文件选择时的选择几率，Ratio越大，该等级的文件越有可能被选中参与Compaction。而等级数、每个等级参数都可以通过CF属性在线更新。这是Facebook HBase分支的解决方案，Date Tiered Compaction策略应该是基于该策略的。
7.Limit Compaction Speed
8.Compaction BandWidth Limit
10.In memory compaction (hbase 2.0)
- 测试情况
- Date Tiered Compaction
hbase(main):031:0> alter 'testdatecompact', CONFIGURATION => {'hbase.hstore.engine.class' => 'org.apache.hadoop.hbase.regionserver.DateTieredStoreEngine'} Updating all regions with the new schema... 0/1 regions updated. 0/1 regions updated. 0/1 regions updated. 0/1 regions updated. 0/1 regions updated. 0/1 regions updated. ......
查看master界面,出现RIT:
980e53b851bb2b0f892bc72164d4f95e testdatecompact,,1540449545990.980e53b851bb2b0f892bc72164d4f95e. state=FAILED_OPEN, ts=Thu Oct 25 14:39:18 CST 2018 (412s ago), server=tdh07,60020,1540448882040 412668 Total number of Regions in Transition for more than 60000 milliseconds 1 Total number of Regions in Transition 1
Emmmmm，退出shell再进shell并尝试使用describe，嘿嘿，有输出：
hbase(main):008:0> describe 'testdatecompact' DESCRIPTION ENABLED
'testdatecompact', {CONFIGURATION => {'hbase.hstore.engine.class' => 'org.apache.hadoop.hbase.regionserver.DateTieredSto true
reEngine'}}, {NAME => 'f', DATA_BLOCK_ENCODING => 'NONE', BLOOMFILTER => 'ROW', REPLICATION_SCOPE => '0', VERSIONS => '1
', COMPRESSION => 'NONE', MIN_VERSIONS => '0', TTL => 'FOREVER', KEEP_DELETED_CELLS => 'FALSE', BLOCKSIZE => '65536', IN
_MEMORY => 'false', BLOCKCACHE => 'true'}
1 row(s) in 3.0620 seconds
Emmmmmm,接着试一下在shell中使用以及scan,报错ERROR: org.apache.hadoop.hbase.NotServingRegionException: Region testdatecompact,,1540449545990.980e53b851bb2b0f892bc72164d4f95e. is not online on tdh05,60020,1540448881040；试了试delete_table命令，这就不好使了，一直执行不下去.
hbck检查: ERROR: Region { meta => testdatecompact,,1540449545990.980e53b851bb2b0f892bc72164d4f95e., hdfs => hdfs://nameservice1/hyperbase1/data/default/testdatecompact/980e53b851bb2b0f892bc72164d4f95e, deployed => } not deployed on any region server. 修复也修复不成功，删。。。也删不掉了 = =
推测原因可能数是版本不支持该策略咔咔咔咔。。。。
ps:最后怎么删掉的>>>>>
(1)尝试alter 'testdatecompact', CONFIGURATION => {'hbase.hstore.engine.class' => 'org.apache.hadoop.hbase.regionserver.DefaultStoreEngine'} Updating all regions with the new schema... 0/1 regions updated. 0/1 regions updated. 0/1 regions updated. K.O (2)尝试hbase 重启大法，重启大法好。hahahahaha重启以后RIT信息没啦！！！ 重新describe一下这张表： DESCRIPTION ENABLED
'testdatecompact', {CONFIGURATION => {'hbase.hstore.engine.class' => 'org.apache.hadoop.hbase.regionserver.DefaultStoreE false
ngine'}}, {NAME => 'f', DATA_BLOCK_ENCODING => 'NONE', BLOOMFILTER => 'ROW', REPLICATION_SCOPE => '0', VERSIONS => '1',
COMPRESSION => 'NONE', MIN_VERSIONS => '0', TTL => 'FOREVER', KEEP_DELETED_CELLS => 'FALSE', BLOCKSIZE => '65536', IN_ME
MORY => 'false', BLOCKCACHE => 'true'}
1 row(s) in 1.6570 seconds
果然。。。。
- 新建表并测试Stripe Compaction策略：
hbase(main):004:0> create 'test','f' 0 row(s) in 0.4080 seconds
=> Hbase::Table - test hbase(main):005:0> alter 'test', CONFIGURATION => {'hbase.hstore.engine.class' => 'org.apache.hadoop.hbase.regionserver.StripeStoreEngine', 'hbase.hstore.blockingStoreFiles' => '300'} Updating all regions with the new schema... 0/1 regions updated. 1/1 regions updated. Done. 0 row(s) in 2.2540 seconds
hbase(main):006:0> describe 'test' DESCRIPTION ENABLED
'test', {CONFIGURATION => {'hbase.hstore.blockingStoreFiles' => '300', 'hbase.hstore.engine.class' => 'org.apache.hadoop true
.hbase.regionserver.StripeStoreEngine'}}, {NAME => 'f', DATA_BLOCK_ENCODING => 'NONE', BLOOMFILTER => 'ROW', REPLICATION
_SCOPE => '0', VERSIONS => '1', COMPRESSION => 'NONE', MIN_VERSIONS => '0', TTL => 'FOREVER', KEEP_DELETED_CELLS => 'FAL
SE', BLOCKSIZE => '65536', IN_MEMORY => 'false', BLOCKCACHE => 'true'}
1 row(s) in 0.0470 seconds
hbase(main):007:0> scan 'test' ROW COLUMN+CELL
0 row(s) in 0.0620 seconds
写入数据后开始追踪日志，发现有如下信息：
2018-10-25 10:22:09,545 DEBUG org.apache.hadoop.hbase.regionserver.StripeStoreFileManager: Added new files; current stripe state is as such: level 0 with 0 files: 0; stripe ending in (end) with 9 files: 2.8 G; 1 stripes total. 9 files total.
^_^ 这就证明设置的策略生效了。接下来跑一段时间数据，然后用major compaction关键字过滤日志查看major cpmaction情况：
关键字过滤后暂时没有发现major compaction信息，但是写入很不稳定。所以新建表并重启hbase再测过一个然后发现：
2018-10-25 16:02:52,231 INFO org.apache.hadoop.hbase.regionserver.HStore: Completed major compaction of 11 file(s) in f of teststripe,,1540453184643.e9cdd38bcc3a8d92efc134f07755d248. into 3044d5862388478f8c12230bb250e6bc(size=1.8 G), 980a9f07ff67453aa28a42ae5bc2d1cc(size=1.8 G), c58914f0aae94810881a0758a9fb7da0(size=1.8 G), total size for store is 7.0 G. This selection was in queue for 0sec, and took 3mins, 2sec to execute. 并且写入还是不太稳定，白萝卜蹲白萝卜蹲白萝卜蹲完红萝卜蹲。。。。。
