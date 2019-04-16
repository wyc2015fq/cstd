# Storm——3、 Storm Trident API 实践 - 隔壁老王的专栏 - CSDN博客





2016年12月20日 13:18:02[隔壁老王来也](https://me.csdn.net/haoyuyang)阅读数：448








## 一、概要     

### 1.1 Storm(简介)
     Storm是一个实时的可靠地分布式流计算框架。
     具体就不多说了，举个例子，它的一个典型的大数据实时计算应用场景：从Kafka消息队列读取消息（可以是logs,clicks,sensor data）、通过Storm对消息进行计算聚合等预处理、把处理结果持久化到NoSQL数据库或者HDFS做进一步深入分析。

### 1.2 Trident(简介)
     Trident是对Storm的更高一层的抽象,除了提供一套简单易用的流数据处理API之外，它以batch(一组tuples)为单位进行处理，这样一来，可以使得一些处理更简单和高效。
     我们知道把Bolt的运行状态仅仅保存在内存中是不可靠的，如果一个node挂掉，那么这个node上的任务就会被重新分配，但是之前的状态是无法恢复的。因此，比较聪明的方式就是把storm的计算状态信息持久化到database中，基于这一点，trident就变得尤为重要。因为在处理大数据时，我们在与database打交道时通常会采用批处理的方式来避免给它带来压力，而trident恰恰是以batch
 groups的形式处理数据，并提供了一些聚合功能的API。

## 二、Trident API 实践
     Trident其实就是一套API，但现阶段网上关于Trident API中各个函数的用法含义资料不多，下面我就根据一些英文资料和自己的理解，详细介绍一下Trident API各个函数的用法和含义。阅读本文需要有一定的Trident API基础。

### 2.1 each() 方法
     作用：操作batch中的每一个tuple内容，一般与Filter或者Function函数配合使用。
     下面通过一个例子来介绍each()方法，假设我们有一个FakeTweetsBatchSpout，它会模拟一个Stream，随机产生一个个消息。我们可以通过设置这个Spout类的构造参数来改变这个Spout的batch Size的大小。

#### 2.1.1 Filter类：过滤tuple
     一个通过actor字段过滤消息的Filter：



**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- publicstaticclass PerActorTweetsFilter extends BaseFilter {  
-   String actor;  
- 
- public PerActorTweetsFilter(String actor) {  
- this.actor = actor;  
-   }  
- @Override
- publicboolean isKeep(TridentTuple tuple) {  
- return tuple.getString(0).equals(actor);  
-   }  
- }  
   Topology：  



**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- topology.newStream("spout", spout)  
-   .each(new Fields("actor", "text"), new PerActorTweetsFilter("dave"))  
-   .each(new Fields("actor", "text"), new Utils.PrintFilter());  
     从上面例子看到，each()方法有一些构造参数
- 第一个构造参数：作为Field Selector，一个tuple可能有很多字段，通过设置Field，我们可以隐藏其它字段，仅仅接收指定的字段（其它字段实际还在）。
- 第二个是一个Filter：用来过滤掉除actor名叫"dave"外的其它消息。

#### 2.1.2 Function类：加工处理tuple内容
     一个能把tuple中text内容变成大写的Function：



**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- publicstaticclass UppercaseFunction extends BaseFunction {  
- @Override
- publicvoid execute(TridentTuple tuple, TridentCollector collector) {  
-     collector.emit(new Values(tuple.getString(0).toUpperCase()));  
-   }  
- }  
     Topology：



**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- topology.newStream("spout", spout)  
-   .each(new Fields("actor", "text"), new PerActorTweetsFilter("dave"))  
-   .each(new Fields("text", "actor"), new UppercaseFunction(), new Fields("uppercased_text"))  
-   .each(new Fields("actor", "text", "uppercased_text"), new Utils.PrintFilter());  
     首先，UppercaseFunction函数的输入是Fields("text", "actor")，其作用是把其中的"text"字段内容都变成大写。
     其次，它比Filter多出一个输出字段，作用是每个tuple在经过这个Function函数处理后，输出字段都会被追加到tuple后面，在本例中，执行完Function之后的tuple内容多了一个"uppercased_text"，并且这个字段排在最后面。

#### 2.1.3 Field Selector与project
   我们需要注意的是，上面每个each()方法的第一个Field字段仅仅是隐藏掉没有指定的字段内容，实际上被隐藏的字段依然还在tuple中，如果想要彻底丢掉它们，我们就需要用到project()方法。
   投影操作作用是仅保留Stream指定字段的数据，比如有一个Stream包含如下字段： [“a”, “b”, “c”, “d”]，运行如下代码：



**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- java mystream.project(new Fields("b", "d"))  
   则输出的流仅包含 [“b”, “d”]字段。

### 2.2 parallelismHint()方法和partitionBy()

#### 2.2.1 parallelismHint()
     指定Topology的并行度，即用多少线程执行这个任务。我们可以稍微改一下我们的Filter，通过打印当前任务的partitionIndex来区分当前是哪个线程。
Filter：



**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- publicstaticclass PerActorTweetsFilter extends BaseFilter {  
- 
- privateint partitionIndex;  
- private String actor;  
- 
- public PerActorTweetsFilter(String actor) {  
- this.actor = actor;  
-   }  
- @Override
- publicvoid prepare(Map conf, TridentOperationContext context) {  
- this.partitionIndex = context.getPartitionIndex();  
-   }  
- @Override
- publicboolean isKeep(TridentTuple tuple) {  
- boolean filter = tuple.getString(0).equals(actor);  
- if(filter) {  
-       System.err.println("I am partition [" + partitionIndex + "] and I have kept a tweet by: " + actor);  
-     }  
- return filter;  
-   }  
- }  
Topology：



**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- topology.newStream("spout", spout)  
-   .each(new Fields("actor", "text"), new PerActorTweetsFilter("dave"))  
-   .parallelismHint(5)  
-   .each(new Fields("actor", "text"), new Utils.PrintFilter());  
     如果我们指定执行Filter任务的线程数量为5，那么最终的执行结果会如何呢？看一下我们的测试结果：



**[plain]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- I am partition [4] and I have kept a tweet by: dave  
- I am partition [3] and I have kept a tweet by: dave  
- I am partition [0] and I have kept a tweet by: dave  
- I am partition [2] and I have kept a tweet by: dave  
- I am partition [1] and I have kept a tweet by: dave  
     我们可以很清楚的发现，一共有5个线程在执行Filter。
     如果我们想要2个Spout和5个Filter怎么办呢？如下面代码所示，实现很简单。



**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- topology.newStream("spout", spout)  
-   .parallelismHint(2)  
-   .shuffle()  
-   .each(new Fields("actor", "text"), new PerActorTweetsFilter("dave"))  
-   .parallelismHint(5)  
-   .each(new Fields("actor", "text"), new Utils.PrintFilter());  

#### 2.2.2 partitionBy()和重定向操作(repartitioning operation)  
     我们注意到上面的例子中用到了shuffle()，shuffle()是一个重定向操作。那什么是重定向操作呢？重定向定义了我们的tuple如何被route到下一处理层，当然不同的层之间可能会有不同的并行度，shuffle()的作用是把tuple随机的route下一层的线程中，而partitionBy()则根据我们的指定字段按照一致性哈希算法route到下一层的线程中，也就是说，如果我们用partitionBy()的话，同一个字段名的tuple会被route到同一个线程中。
     比如，如果我们把上面代码中的shuffle()改成partitionBy(new Fields("actor"))，猜一下结果会怎样？



**[plain]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- I am partition [2] and I have kept a tweet by: dave  
- I am partition [2] and I have kept a tweet by: dave  
- I am partition [2] and I have kept a tweet by: dave  
- I am partition [2] and I have kept a tweet by: dave  
     测试结果正如我们上面描述的那样，相同字段的tuple被route到了同一个partition中。
重定向操作有如下几种：
- shuffle：通过随机分配算法来均衡tuple到各个分区
- broadcast：每个tuple都被广播到所有的分区，这种方式在drcp时非常有用，比如在每个分区上做stateQuery
- partitionBy：根据指定的字段列表进行划分，具体做法是用指定字段列表的hash值对分区个数做取模运算，确保相同字段列表的数据被划分到同一个分区
- global：所有的tuple都被发送到一个分区，这个分区用来处理整个Stream
- batchGlobal：一个Batch中的所有tuple都被发送到同一个分区，不同的Batch会去往不同的分区
- Partition：通过一个自定义的分区函数来进行分区，这个自定义函数实现了 backtype.storm.grouping.CustomStreamGrouping

### 2.3 聚合(Aggregation)
     我们前面讲过，Trident的一个很重要的特点就是它是以batch的形式处理tuple的。我们可以很容易想到的针对一个batch的最基本操作应该就是聚合。Trident提供了聚合API来处理batches，来看一个例子：

#### 2.3.1 Aggregator:




**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- publicstaticclass LocationAggregator extends BaseAggregator<Map<String, Integer>> {  
- 
- @Override
- public Map<String, Integer> init(Object batchId, TridentCollector collector) {  
- returnnew HashMap<String, Integer>();  
-   }  
- 
- @Override
- publicvoid aggregate(Map<String, Integer> val, TridentTuple tuple, TridentCollector collector) {  
-     String location = tuple.getString(0);  
-     val.put(location, MapUtils.getInteger(val, location, 0) + 1);  
-   }  
- 
- @Override
- publicvoid complete(Map<String, Integer> val, TridentCollector collector) {  
-     collector.emit(new Values(val));  
-   }  
- }  
 Topology:



**[java]**[view plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- topology.newStream("spout", spout)  
-   .aggregate(new Fields("location"), new LocationAggregator(), new Fields("location_counts"))  
-   .each(new Fields("location_counts"), new Utils.PrintFilter());  
     这个aggregator很简单：计算每一个batch的location的数量。通过这个例子我们可以看到Aggregator接口：
- init():当刚开始接收到一个batch时执行
- aggregate():在接收到batch中的每一个tuple时执行
- complete():在一个batch的结束时执行     
     我们前面讲过aggregate()方法是一个重定向方法，因为它会随机启动一个单独的线程来进行这个聚合操作。
     下面我们来看一下测试结果：



**[plain]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- [{USA=3, Spain=1, UK=1}]  
- [{USA=3, Spain=2}]  
- [{France=1, USA=4}]  
- [{USA=4, Spain=1}]  
- [{USA=5}]  
     我们可以看到打印的结果，其中每一条的和都是5，这是因为我们的Spout的每个batch中tuple数量设置的是5，所以每个线程的计算结果也会是5。 除此之外，Trident还提供了其它两个Aggregator接口: CombinerAggregator, ReducerAggregator,具体使用方法请参考Trident API。

#### 2.3.2 partitionAggregate():


     如果我们将上面的Topology稍微改造一下，猜一下结果会是如何？





**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- topology.newStream("spout", spout)  
-   .partitionBy(new Fields("location"))  
-   .partitionAggregate(new Fields("location"), new LocationAggregator(), new Fields("location_counts"))  
-   .parallelismHint(3)  
-   .each(new Fields("location_counts"), new Utils.PrintFilter());  


     我们一起来分析一下，首先partitionBy()方法将tuples按其location字段重定向到下一处理逻辑，而且相同location字段的tuple一定会被分配到同一个线程中处理。其次，partitionAggregate()方法，注意它与Aggregate不同，它不是一个重定向方法，它仅仅是对当前partition上的各个batch执行聚合操作。因为我们根据location进行了重定向操作，测试数据一共有4个location，而当前一共有3个partition，因此可以猜测我们的最终测试结果中，有一个partition会处理两个location的batch，最终测试结果如下：



**[plain]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- [{France=10, Spain=5}]  
- [{USA=63}]  
- [{UK=22}]  
     需要注意的是，partitionAggregate虽然也是聚合操作，但与上面的Aggregate完全不同，它不是一个重定向操作。

### 2.4 groupBy
     我们可以看到上面几个例子的测试结果，其实我们通常想要的是每个location的数量是多少，那该怎么处理呢？看下面这个Topology：



**[java]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- topology.newStream("spout", spout)  
-   .groupBy(new Fields("location"))  
-   .aggregate(new Fields("location"), new Count(), new Fields("count"))  
-   .each(new Fields("location", "count"), new Utils.PrintFilter());  
     我们先看一下执行的结果：



**[plain]**[view
 plain](http://blog.csdn.net/suifeng3051/article/details/41118721#)[copy](http://blog.csdn.net/suifeng3051/article/details/41118721#)

[![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)](https://code.csdn.net/snippets/518964)[![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)](https://code.csdn.net/snippets/518964/fork)- ...  
- [France, 25]  
- [UK, 2]  
- [USA, 25]  
- [Spain, 44]  
- [France, 26]  
- [UK, 3]  
- ...  
     上面这段代码计算出了每个location的数量，即使我们的Count函数没有指定并行度。这就是groupBy()起的作用，它会根据指定的字段创建一个GroupedStream，相同字段的tuple都会被重定向到一起，汇聚成一个group。groupBy()之后是aggregate，与之前的聚合整个batch不同，此时的aggregate会单独聚合每个group。我们也可以这么认为，groupBy会把Stream按照指定字段分成一个个stream
 group，每个group就像一个batch一样被处理。


     不过需要注意的是，groupBy()本身并不是一个重定向操作，但如果它后面跟的是aggregator的话就是，跟的是partitionAggregate的话就不是。

![](https://img-blog.csdn.net/20141114145740021?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3VpZmVuZzMwNTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 三、总结 
     Storm是一个实时流计算框架，Trident是对storm的一个更高层次的抽象，Trident最大的特点以batch的形式处理stream。
     一些最基本的操作函数有Filter、Function，Filter可以过滤掉tuple，Function可以修改tuple内容，输出0或多个tuple，并能把新增的字段追加到tuple后面。
     聚合有partitionAggregate和Aggregator接口。partitionAggregate对当前partition中的tuple进行聚合，它不是重定向操作。Aggregator有三个接口：CombinerAggregator, ReducerAggregator，Aggregator，它们属于重定向操作，它们会把stream重定向到一个partition中进行聚合操作。
     重定向操作会改变数据流向，但不会改变数据内容，重定向操会产生网络传输，可能影响一部分效率。而Filter、Function、partitionAggregate则属于本地操作，不会产生网络传输。
     GroupBy会根据指定字段，把整个stream切分成一个个grouped stream，如果在grouped stream上做聚合操作，那么聚合就会发生在这些grouped stream上而不是整个batch。如果groupBy后面跟的是aggregator，则是重定向操作，如果跟的是partitionAggregate，则不是重定向操作。


     上面所以的例子都可以在github上找到： [https://github.com/pereferrera/trident-hackaton/](https://github.com/pereferrera/trident-hackaton/)
























































