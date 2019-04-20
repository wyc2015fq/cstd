# 重要 : 优化flink的四种方式 - Spark高级玩法 - CSDN博客
2018年09月06日 00:01:45[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：141
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
flink这个框架在逐步变为流处理的主流。本文，我们将针对flink性能调优讲四种不同的方法。加浪尖微信 **158570986**，拉入大数据微信交流群。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)使用flink tuples
当使用groupby，join，后者key这类算子的时候，FLink提供了很多种访问你key的方法。
1. 使用key selector
```
// Join movies and ratings datasets
movies.join(ratings)
        // Use movie id as a key in both cases
        .where(new KeySelector<Movie, String>() {
            @Override
            public String getKey(Movie m) throws Exception {
                return m.getId();
            }
        })
        .equalTo(new KeySelector<Rating, String>() {
            @Override
            public String getKey(Rating r) throws Exception {
                return r.getMovieId();
            }
        })
```
2. pojo类型使用字段
```
movies.join(ratings) 
  // Use same fields as in the previous example 
  .where("id") 
  .equalTo("movieId")
```
3. tuble类型使用offset
```
DataSet<Tuple2<String, String>> movies = ... 
DataSet<Tuple3<String, String, Double>> ratings = ... 
movies.join(ratings) 
// Specify fields positions in tuples 
.where(0) 
.equalTo(1)
```
4继承TupleX
第三种方法虽然给了最好的性能，但是可读性很差。比如，我们的代码有可能写成下面这样：
```
DataSet<Tuple3<Integer, String, Double>> result = movies.join(ratings) 
  .where(0) 
  .equalTo(0) 
  .with(new JoinFunction<Tuple2<Integer,String>, Tuple2<Integer,Double>, Tuple3<Integer, String, Double>>() { 
     // What is happening here? 
     @Override 
     public Tuple3<Integer, String, Double> join(Tuple2<Integer, String> first, Tuple2<Integer, Double> second) throws Exception { 
        // Some tuples are joined with some other tuples and some fields are returned??? 
        return new Tuple3<>(first.f0, first.f1, second.f1); 
     } 
  });
```
为了提升可读性，那么我们可以继承TupleX方法，然后重写其getter和setter方法。在flink的Gelly依赖内部有个Edge类，它就继承了Tuple3，如下：
```
public class Edge<K, V> extends Tuple3<K, K, V> {
  private static final long serialVersionUID = 1L;
  public Edge(){}
  public Edge(K source, K target, V value) {
    this.f0 = source;
    this.f1 = target;
    this.f2 = value;
  }
  /**
   * Reverses the direction of this Edge.
   * @return a new Edge, where the source is the original Edge's target
   * and the target is the original Edge's source.
   */
  public Edge<K, V> reverse() {
      return new Edge<>(this.f1, this.f0, this.f2);
  }
  public void setSource(K source) {
    this.f0 = source;
  }
  public K getSource() {
    return this.f0;
  }
  public void setTarget(K target) {
    this.f1 = target;
  }
  public K getTarget() {
    return f1;
  }
  public void setValue(V value) {
    this.f2 = value;
  }
  public V getValue() {
    return f2;
  }
}
```
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)重利用flink对象
### 可用于提高Flink应用程序性能的另一个选项是在从用户定义的函数返回数据时使用可变对象。看看这个例子：
```
stream
  .apply(new WindowFunction<WikipediaEditEvent, Tuple2<String, Long>, String, TimeWindow>() {
@Override
public void apply(String userName, TimeWindow timeWindow, Iterable<WikipediaEditEvent> iterable, Collector<Tuple2<String, Long>> collector) throws Exception {
  long changesCount = ...
  // A new Tuple instance is created on every execution
  collector.collect(new Tuple2<>(userName, changesCount));
  }
  }
```
正如上面例子所看到的在每次执行apply函数时，都创建了一个Tuple2类的新实例，这会增加垃圾收集器的压力。解决此问题的一种方法是重用相同的实例：
```
stream
  .apply(new WindowFunction<WikipediaEditEvent, Tuple2<String, Long>, String, TimeWindow>() {
// Create an instance that we will reuse on every call
private Tuple2<String, Long> result = new Tuple<>();
@Override
public void apply(String userName, TimeWindow timeWindow, Iterable<WikipediaEditEvent> iterable, Collector<Tuple2<String, Long>> collector) throws Exception {
  long changesCount = ...
  // Set fields on an existing object instead of creating a new one
  result.f0 = userName;
  // Auto-boxing!! A new Long value may be created
  result.f1 = changesCount;
  // Reuse the same Tuple2 object
  collector.collect(result);
  }
  }
```
上面代码有些提升。但是，我们也会在每次调用的时候非直接创建 long类型的实例。为了解决这个问题，flink有许多所谓的值类：IntValue，LongValue，StringValue，FloatValue等。这些类的主要特点是提供内置类可修改的版本，我们可以在自定义函数中使用它们。例如：
```
stream
  .apply(new WindowFunction<WikipediaEditEvent, Tuple2<String, Long>, String, TimeWindow>() {
// Create a mutable count instance
private LongValue count = new IntValue();
// Assign mutable count to the tuple
private Tuple2<String, LongValue> result = new Tuple<>("", count);
@Override
// Notice that now we have a different return type
public void apply(String userName, TimeWindow timeWindow, Iterable<WikipediaEditEvent> iterable, Collector<Tuple2<String, LongValue>> collector) throws Exception {
  long changesCount = ...
  // Set fields on an existing object instead of creating a new one
  result.f0 = userName;
  // Update mutable count value
  count.setValue(changesCount);
  // Reuse the same tuple and the same LongValue instance
  collector.collect(result);
  }
  }
```
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)使用函数注解
另外一个优化Flink任务的方法是提供一些关于你UDF如何处理输入数据的一些信息。由于Flink无法解析和理解代码，因此您可以提供有助于构建更高效执行计划的关键信息。 我们可以使用三种注解：
- 
`**@ForwardedFields**`: 指定保留不动的输入字段，同时该字段要作为输出值的一部分。
- 
`**@NotForwardedFields**`: 指定不再输出中相同位置保留的字段。
- 
`**@ReadFields**`: 指定用于计算结果的字段。应该只指定计算中使用的字段，而不仅仅是复制到输出。
举个例子，ForwardedFields注解使用如下：
```
// Specify that the first element is copied without any changes
@ForwardedFields("0")
class MyFunction implements MapFunction<Tuple2<Long, Double>, Tuple2<Long, Double>> {
  @Override
  public Tuple2<Long, Double> map(Tuple2<Long, Double> value) {
     // Copy first field without change
     return new Tuple2<>(value.f0, value.f1 + 123);
  }
}
```
上面的意思是tuple中的第一个元素不会被改变，并且会在相同的位置返回。
如果你不想修改字段的值，但是想修改字段在输出结果中的位置，可以通过ForwardedFields来指定。下面的例子就是交换字段在tuple中的位置，然后提醒flink。
```
//第一个元素换到第二个位置，第二个元素换到第一个位置
@ForwardedFields("0->1; 1->0")
class SwapArguments implements MapFunction<Tuple2<Long, Double>, Tuple2<Double, Long>> {
  @Override
  public Tuple2<Double, Long> map(Tuple2<Long, Double> value) {
     // Swap elements in a tuple
     return new Tuple2<>(value.f1, value.f0);
  }
}
```
上面的注解仅能用于仅有一个输入的参数，比如map或者flatmap。如果有两个输入参数，你可以使用ForwardedFieldsFirst和ForwardedFieldsSecond注解。
下面用JoinFunction接口：
```
// Two fields from the input tuple are copied to the first and second positions of the output tuple
@ForwardedFieldsFirst("0; 1")
// The third field from the input tuple is copied to the third position of the output tuple
@ForwardedFieldsSecond("2")
class MyJoin implements JoinFunction<Tuple2<Integer,String>, Tuple2<Integer,Double>, Tuple3<Integer, String, Double>>() {
@Override
public Tuple3<Integer, String, Double> join(Tuple2<Integer, String> first, Tuple2<Integer, Double> second) throws Exception {
  return new Tuple3<>(first.f0, first.f1, second.f1);
  }
  })
```
flink还有NotForwardedFieldsFirst, NotForwardedFieldsSecond, ReadFieldsFirst, 和 ReadFirldsSecond等注解。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz/cZV2hRpuAPgQ1OsgaP0iaM2iaUqyHW5WSDNHVkkrpriboBDrogCqa4GIR4nsNRibTTEue7PRJoL2QPbgWXdSCfuzMQ/640?wx_fmt=png)优化join
### 通过给Flink一些提示，可以使得你的join更快，但是首先我们要简单了解一下Flink如何执行join的。
当Flink处理批量数据的时候，每台机器只是存储了集群的部分数据。为了执行join，Flink需要找到两个数据集的所有满足join条件的数据。为了实现这个目标，Flink需要将两个数据集有相同key的数据发送到同一台机器上。有两种策略：
1. repartition-repartition strategy
在该情况下，两个数据集都会使用key进行重分区并使用通过网络传输。这就意味着假如数据集太大的话，网络传输数据集将耗费大量的时间。
2. broadcast-forward strategy
在该情况下，一个数据集不动，另一个数据集会copy到有第一个数据集部分数据的所有机器上。
如果使用小数据集与大数据集进行join，可以选择broadcast-forward策略，将小数据集广播，避免代价高的重分区。
`ds1.join(ds2, JoinHint.BROADCAST_HASH_FIRST)`
第二个参数就是提示，第一个数据集比第二个小。
也可以使用下面几个提示：
- 
`**BROADCAST_HASH_SECOND**`: 第二个数据集是较小的数据集
- 
`**REPARTITION_HASH_FIRST**`: 第一个书记集是较小的数据集
- 
`**REPARTITION_HASH_SECOND**`: 第二个数据集是较小的数据集。
- 
`**REPARTITION_SORT_MERGE**`: 对数据集进行重分区，同时使用sort和merge策略。
- 
`**OPTIMIZER_CHOOSES**`: Flink的优化器决定两个数据集如何join。
[完 译自https://dzone.com/articles/four-ways-to-optimize-your-flink-applications]
**推荐阅读：**
[从滴滴flinkCEP说起](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485303&idx=1&sn=921d1eb15da490719ca84326e2941111&chksm=9f38e45fa84f6d49f850a8051cc0f2b9c55500466e809b053cf0ebb18ce60c924d7dcaa8f6e2&scene=21#wechat_redirect)
[必读|spark的重分区及排序](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485223&idx=1&sn=3b5d1d501866366ea71be28d1bcbe81a&chksm=9f38e40fa84f6d19ce57fe923d644723143d438e8afd6f888f2a329dff8eda70e4c3fe6105e8&scene=21#wechat_redirect)
[基石 | Flink Checkpoint-轻量级分布式快照](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485192&idx=1&sn=598ad9eb1a28130fefd70b32c9b1b741&chksm=9f38e420a84f6d36b7fdc96205547d709348420ab45386beade49bfdf048c0c6bc34ddced669&scene=21#wechat_redirect)
更多大数据视频，技术文档，调优，源码解读欢迎扫码加入知识星球
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWEbA0t2WgVoUbCSPAic4uAuwkHOg8iczLJQ4gpogt2Gy2FelL1iaM9jW6IPAtjuB4c3ZmfmNb1vRy2w/640?wx_fmt=jpeg)
