# hbase split - 数据之美的博客 - CSDN博客
2018年10月31日 09:50:38[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：111
## hbase split
因为已经进行了预分区，在参数配置时，我们将hbase.hregion.max.filesize调整至了30G(32212254720),但是在测试中发现，还是会在表或者是region不到30G发生了split,日志中可见发生split信息。又碰到一个和major compaction差不多，设置了相应参数却不能唯一控制situation的参数。相关资料整理如下：
**Hbase split 点都位于region中row key的中间点。**
*HBase对表进行split有三种方式：Pre-splitting，Auto splitting，Forced Splits。* 这里我们只讨论 Auto spliting。
RegionServer split region时，先将region下线,然后拆分region，将其子region加入到META元信息中，再将他们加入到原本的RegionServer中，最后汇报给Master。 不管是否对表进行了pre-splitting，只要region的大小到达设定的阈值，Hbase都会自动将其split为两个大小相等的region。split相关的部分日志如下：
2018-10-23 14:04:40,077 INFO org.apache.hadoop.hbase.regionserver.SplitTransaction: Starting split of region test10_23,,1540274602472.47ff1b6c71722f78c58032dabee608ec.
2018-10-23 14:09:45,370 DEBUG org.apache.hadoop.hbase.regionserver.HRegion: Closing test10_23,izct,1540274680076.b497cc2f646c22e5f134022a1f120c68.: disabling compactions & flushes
2018-10-23 14:09:45,370 DEBUG org.apache.hadoop.hbase.regionserver.HRegion: waiting for 1 compactions to complete for region test10_23,izct,1540274680076.b497cc2f646c22e5f134022a1f120c68. 2018-10-23 14:09:45,507 INFO org.apache.hadoop.hbase.regionserver.HRegion: Running close preflush of test10_23,izct,1540274680076.b497cc2f646c22e5f134022a1f120c68.
2018-10-23 14:09:46,198 INFO org.apache.hadoop.hbase.regionserver.HRegion: Closed test10_23,izct,1540274680076.b497cc2f646c22e5f134022a1f120c68.
2018-10-23 14:09:46,674 DEBUG org.apache.hadoop.hbase.zookeeper.lock.ZKInterProcessLockBase: Released /hyperbase1/table-lock/test10_23/read-regionserver:600200000000002 2018-10-23 14:09:46,674 INFO org.apache.hadoop.hbase.regionserver.SplitRequest: Region split, hbase:meta updated, and report to master. Parent=test10_23,izct,1540274680076.b497cc2f646c22e5f134022a1f120c68., new regions: test10_23,izct,1540274985219.d27a97a7076c75adb21963d49d70c1af., test10_23,qzmxj,1540274985219.244795c15487ac6831e87069e0c42ce6.. Split took 1sec
2018-10-23 14:04:42,536 INFO org.apache.hadoop.hbase.regionserver.SplitRequest: Region split, hbase:meta updated, and report to master. Parent=test10_23,,1540274602472.47ff1b6c71722f78c58032dabee608ec., new regions: test10_23,,1540274680076.f234320ed3a0c73fa097c43a6db116d5., test10_23,izct,1540274680076.b497cc2f646c22e5f134022a1f120c68.. Split took 2sec
查看不少相关博客，好像暂时还没有看到博客明确说split策略有算法接口可以自行写算法接入的。总结一下HBase提到的如下几种Auto splitting的方法：
- ConstantSizeRegionSplitPolicy
- IncreasingToUpperBoundRegionSplitPolicy
- KeyPrefixRegionSplitPolicy
- DelimitedKeyPrefixRegionSplitPolicy
- DisabledRegionSplitPolicy
第一个是HBase0.94版本之前默认和唯一的策略，当region中某个Store的大小大于设置的阈值(hbase.hregion.max.filesize，默认大小是10G)，HBase就会将该region分割为两个region。*此分割方法适合已经做过pre-splitting且希望每个regionserver的region数量尽可能少的情况。*
第二个是HBase0.94及其以后版本的默认策略，该策略的阈值还跟region数量也有关系。 Store的大小的最大值取决于公式：Min (R^3 * hbase.hregion.memstore.flush.size, hbase.hregion.max.filesize)，（二者取小），其中R是同一个regionserver上同一个表的region的数量。*以hbase.hregion.memstore.flush.size=128M、hbase.hregion.max.filesize=10G为例说明： 初始时regionserver中只有一个region，那么分割大小为128M，随着region数量的增多分割大小也逐渐增大：512MB, 1152MB, 2GB, 3.2GB, 4.6GB, 6.2GB……。当有9个region时，分隔大小将会超过hbase.hregion.max.filesize，也就是将以10G作为以后的分割大小。*
=。 = 这就是设置参数后又偷偷split的幕后凶手！
第三个和第四个是两种用户自定义策略。第三个策略，保证相同的前缀的row保存在同一个region中。指定rowkey前缀位数划分region，通过读取 KeyPrefixRegionSplitPolicy.prefix_length 属性，（该属性为数字类型，表示前缀长度),在进行split时，按此长度对splitPoint进行截取。此种策略比较适合固定前缀的rowkey。
第四个是保证以分隔符前面的前缀为splitPoint，保证相同RowKey前缀的数据在一个Region中。
第五个策略是直接禁用了region的自动split。目前在博客中看支持的版本信息最低的是Hbase-1.1.3。
hbase split policy的相应设置参考[https://blog.csdn.net/jdplus/article/details/47273983，摘要如下：](https://blog.csdn.net/jdplus/article/details/47273983%EF%BC%8C%E6%91%98%E8%A6%81%E5%A6%82%E4%B8%8B%EF%BC%9A)
1.在hbase-site.xml中配置全局的分割策略
hbase.regionserver.region.split.policy org.apache.hadoop.hbase.regionserver.IncreasingToUpperBoundRegionSplitPolicy
2.利用Java API对表进行配置
HTableDescriptor tableDesc = new HTableDescriptor("test"); tableDesc.setValue(HTableDescriptor.SPLIT_POLICY, ConstantSizeRegionSplitPolicy.class.getName()); tableDesc.addFamily(new HColumnDescriptor(Bytes.toBytes("cf1")));
3.在Hbase shell中对表进行配置
hbase> create 'test', {METHOD => 'table_att', CONFIG => {'SPLIT_POLICY' => 'org.apache.hadoop.hbase.regionserver.ConstantSizeRegionSplitPolicy'}},{NAME => 'cf1'}
- 测试情况
测试时考虑到方法3是针对表进行设置的，先测shell参数控制，测试时试了有4到5次在shell中针对表进行设置但是都没有达到预期的效果，怀疑设置有问题导致参数不生效，这块还需要更多的资料和尝试。？？？待以后填坑？？？
create 'test1023', 'f', {CONFIGURATION => {'SPLIT_POLICY' => 'org.apache.hadoop.hbase.regionserver.ConstantSizeRegionSplitPolicy'}}
create 'test', {METHOD => 'table_att', CONFIGURATION => {'SPLIT_POLICY' => 'org.apache.hadoop.hbase.regionserver.ConstantSizeRegionSplitPolicy'}},{NAME => 'f'}
以及建表后 alter 't1', CONFIGURATION => {'SPLIT_POLICY' => 'org.apache.hadoop.hbase.regionserver.ConstantSizeRegionSplitPolicy'}
alter 't1', {NAME => 'f', CONFIGURATION => {'SPLIT_POLICY' => 'org.apache.hadoop.hbase.regionserver.ConstantSizeRegionSplitPolicy'}
alter 't1', CONFIGURATION => {METHOD => 'table_att','SPLIT_POLICY' => 'org.apache.hadoop.hbase.regionserver.ConstantSizeRegionSplitPolicy'}
后面改为测试方法1：方法1配置成功，测试写一个新表，约30G以后表才发生一次split。
建表方式（hbase1.3.1）：
create 'test', {METHOD => 'table_att', CONFIG => {'SPLIT_POLICY' => 'org.apache.hadoop.hbase.regionserver.ConstantSizeRegionSplitPolicy'}},{NAME => 'cf1'}
alter 'table',{METHOD => 'table_att',  CONFIGURATION => {'SPLIT_POLICY' => 'org.apache.hadoop.hbase.regionserver.ConstantSizeRegionSplitPolicy'}},{NAME => 'f'}
