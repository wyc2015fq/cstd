# 细述hbase协处理器 - Spark高级玩法 - CSDN博客
2018年10月29日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：99
1.起因(Why HBase  Coprocessor)
HBase作为列族数据库最经常被人诟病的特性包括：无法轻易建立“二级索引”，难以执行求和、计数、排序等操作。比如，在旧版本的(<0.92)Hbase中，统计数据表的总行数，需要使用Counter方法，执行一次MapReduce Job才能得到。虽然HBase在数据存储层中集成了MapReduce，能够有效用于数据表的分布式计算。然而在很多情况下，做一些简单的相加或者聚合计算的时候，如果直接将计算过程放置在server端，能够减少通讯开销，从而获得很好的性能提升。于是，HBase在0.92之后引入了协处理器(coprocessors)，实现一些激动人心的新特性：能够轻易建立二次索引、复杂过滤器(谓词下推)以及访问控制等。
2.灵感来源( Source of Inspration)
HBase协处理器的灵感来自于Jeff Dean 09年的演讲( P66-67)。它根据该演讲实现了类似于bigtable的协处理器，包括以下特性:
每个表服务器的任意子表都可以运行代码 
客户端的高层调用接口(客户端能够直接访问数据表的行地址，多行读写会自动分片成多个并行的RPC调用) 
提供一个非常灵活的、可用于建立分布式服务的数据模型 
能够自动化扩展、负载均衡、应用请求路由 
HBase的协处理器灵感来自bigtable，但是实现细节不尽相同。HBase建立了一个框架，它为用户提供类库和运行时环境，使得他们的代码能够在HBase region server和master上处理。
3.细节剖析（Implementation)
协处理器分两种类型，系统协处理器可以全局导入region server上的所有数据表，表协处理器即是用户可以指定一张表使用协处理器。协处理器框架为了更好支持其行为的灵活性，提供了两个不同方面的插件。一个是观察者（observer），类似于关系数据库的触发器。另一个是终端(endpoint)，动态的终端有点像存储过程。3.1观察者(Observer)
观察者的设计意图是允许用户通过插入代码来重载协处理器框架的upcall方法，而具体的事件触发的callback方法由HBase的核心代码来执行。协处理器框架处理所有的callback调用细节，协处理器自身只需要插入添加或者改变的功能。
以HBase0.92版本为例，它提供了三种观察者接口：
RegionObserver：提供客户端的数据操纵事件钩子：Get、Put、Delete、Scan等。 
WALObserver：提供WAL相关操作钩子。 
MasterObserver：提供DDL-类型的操作钩子。如创建、删除、修改数据表等。 
这些接口可以同时使用在同一个地方，按照不同优先级顺序执行.用户可以任意基于协处理器实现复杂的HBase功能层。HBase有很多种事件可以触发观察者方法，这些事件与方法从HBase0.92版本起，都会集成在HBase API中。不过这些API可能会由于各种原因有所改动，不同版本的接口改动比较大，具体参考Java Doc。
RegionObserver工作原理，如图1所示。更多关于Observer细节请参见HBaseBook的第 9.6.3章节。 
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUpySMSATw0hMkZw0njRnd30CoHrB9AwDw566U3AKx6UTaHianAwJNf4yejzvaslX3r4MCf1t931wg/640)
###   3.2终端(Endpoint)
终端是动态RPC插件的接口，它的实现代码被安装在服务器端，从而能够通过HBase RPC唤醒。客户端类库提供了非常方便的方法来调用这些动态接口，它们可以在任意时候调用一个终端，它们的实现代码会被目标region远程执行，结果会返回到终端。用户可以结合使用这些强大的插件接口，为HBase添加全新的特性。终端的使用，如下面流程所示：
- 
定义一个新的protocol接口，必须继承CoprocessorProtocol.
- 
实现终端接口，该实现会被导入region环境执行。
- 
继承抽象类BaseEndpointCoprocessor.
- 
在客户端，终端可以被两个新的HBase Client API调用 。
```
单个region：HTableInterface.coprocessorProxy(Class protocol, byte[] 
row) 。
rigons区域：HTableInterface.coprocessorExec(Class protocol, 
byte[] startKey, byte[] endKey, Batch.Call callable)
```
整体的终端调用过程范例，所示： 
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUpySMSATw0hMkZw0njRnd3Nn9gUfibgvEuDh6HD7nK5ibmvTlaibHBuy6ibadfrOeI88MIHk60aOciciag/640)
## 4.编程实践(Code Example)
在该实例中，我们通过计算HBase表中行数的一个实例，来真实感受协处理器 的方便和强大。在旧版的HBase我们需要编写MapReduce代码来汇总数据表中的行数，在0.92以上的版本HBase中，只需要编写客户端的代码即可实现，非常适合用在WebService的封装上。
4.1启用协处理器 Aggregation(Enable Coprocessor Aggregation)
我们有两个方法： 
1.启动全局aggregation，能过操纵所有的表上的数据。通过修改hbase-site.xml这个文件来实现，只需要添加如下代码：
```
<property>
  <name>hbase.coprocessor.user.region.classes</name>
  <value>org.apache.hadoop.hbase.coprocessor.AggregateImplementation</value>
</property>
```
2.启用表aggregation，只对特定的表生效。通过HBase Shell 来实现。
- 
disable指定表。hbase> disable 'mytable'
- 
添加aggregation hbase> alter 'mytable', METHOD => 
'table_att','coprocessor'=>'|org.apache.hadoop.hbase.coprocessor.AggregateImplementation||'
- 
重启指定表 hbase> enable 'mytable'
```
disable 'xyz'
alter 'xyz','coprocessor'=>'|org.apache.hadoop.hbase.coprocessor.example.RowCountEndpoint||'
enable 'xyz'
```
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUpySMSATw0hMkZw0njRnd3Qs54AkUp7TtLy8LNCs73dnKloatWzxPPtLAnNaXwomoYyaMDx8EiaDw/640)
```
package hbase.learn.com;
import java.util.Map;
import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.hbase.HBaseConfiguration;
import org.apache.hadoop.hbase.client.HTable;
public class RowCount {
   public static void main(String[] args) {
       String tableName="xyz";
       // Get instance of Configuration
       Configuration configuration = HBaseConfiguration.create();
       // Get table instance
       HTable table = new HTable(configuration, tableName);
        final ExampleProtos.CountRequest request = ExampleProtos.CountRequest.getDefaultInstance();
           Map<byte[],Long> results = table.coprocessorService(ExampleProtos.RowCountService.class,
               null, null,
               new Batch.Call<ExampleProtos.RowCountService,Long>() {
                 public Long call(ExampleProtos.RowCountService counter) throws IOException {
                   ServerRpcController controller = new ServerRpcController();
                   BlockingRpcCallback<ExampleProtos.CountResponse> rpcCallback =
                       new BlockingRpcCallback<ExampleProtos.CountResponse>();
                   counter.getRowCount(controller, request, rpcCallback);
                   ExampleProtos.CountResponse response = rpcCallback.get();
                   if (controller.failedOnException()) {
                     throw controller.getFailedOn();
                   }
                   return (response != null && response.hasCount()) ? response.getCount() : 0;
                 }
               });
           long sum=0;
           int count=0;
           for(Long l:results.values()){
               sum+=l;
               count++;
           }
           System.out.println("row count = "+sum);
           System.out.println("region count = "+count);
   }
}
```
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFUpySMSATw0hMkZw0njRnd3oG67UmNyibCHwKhEsDibWWdEO0HaRE6hX2NnOBZy5ic7caUfIeJpPLF4w/640)
推荐阅读
[HBase的region管理](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484456&idx=1&sn=691da41a4d8cf3bae2981cc8dc8263b4&chksm=9f38e700a84f6e16386d2e509e48fa6ad2e93be0d868d89638647bbc32f71750263bbaa2708c&scene=21#wechat_redirect)
[HBase最佳实践－读性能优化策略](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485168&idx=1&sn=a3ad3ef87ac23f6ef7a84f8d133b887a&chksm=9f38e5d8a84f6cce60b2b3c670ab653b48dcb6c04c52de480295f0d160e78b054622d066de86&scene=21#wechat_redirect)
[Spark 下操作 HBase（1.0.0 新 API）](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485050&idx=1&sn=56a578f0f69f992041e1ce566bc6952f&chksm=9f38e552a84f6c44891a8bb397b952a13fb9ff555f30178a27f3c330b30af1e0da4c1aee7fba&scene=21#wechat_redirect)
[HBase从入门到精通系列：误删数据如何抢救？](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485005&idx=1&sn=371cdc7f893ff836aefcdd1202bff3f3&chksm=9f38e565a84f6c73bd76a1c5284925003e59ac129841685de94c5cbe0acc935b58ea26dc070e&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
