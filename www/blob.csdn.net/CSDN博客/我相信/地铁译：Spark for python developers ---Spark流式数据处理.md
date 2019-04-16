# 地铁译：Spark for python developers ---Spark流式数据处理 - 我相信...... - CSDN博客





2016年06月15日 09:22:28[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：21406
所属专栏：[Python 开发者的Spark](https://blog.csdn.net/column/details/12815.html)









先研究一下不断改变的动态环境带来的挑战，在列出流处理应用的先决条件（如，Twitter的TCP Sockets连接）之后， 结合Spark,  Kafka  和 Flume 把数据放入一个低延迟，高吞吐量，可缩放的处理流水线。 

要点如下: 

 • 分析流式应用架构的挑战，约束和需求  

 • 利用Spark  Streaming  从 TCP  socket 中处理实时数据 

  • 连接 Twitter  服务，准实时解析 tweets  

  • 使用 Spark,  Kafka,  和 Flume 建立一个可靠，容错，可缩放，高吞吐量，低延迟的集成应用 

   • 以  Lambda 和  Kappa  的架构范式结尾  
# Spark Streaming在数据密集型应用中的位置



按照惯例,  先看一下最初的数据密集型应用架构，指明我们所感兴趣的 Spark  Streaming  模块的所处位置. 

下图着重指明了整体架构中的Spark  Streaming模块，Spark  SQL和 Spark MLlib： 
![5-1 Streaming 在数据密集型应用架构的位置](https://img-blog.csdn.net/20160512090327442)
数据流可以来自股票市场的时序分析，企业交易，各种交互，事件，web流量，点击流，和传感器数据等，都是及时且带有时间戳的数据。用例有欺诈检测和防伪，移动的交叉销售和销售提升，或者交通预警。这些数据流需要及时处理以便于监测，例如 异常检测，异常奇点，垃圾邮件，欺诈和入侵;  也可提供基础的统计，见解，趋势，和推荐。 某些情形，总结性的汇总信息需要存储以备将来使用。从架构范式的角度看，我们从面向服务的架构转为事件驱动的架构。  

 数据的流处理有两个模型:  

 • 在数据到达时及时处理每条记录，在处理前不需要在容器中缓存记录，比如 Twitter’s  Storm,  Yahoo’s  S4,  和  Google’s  MillWheel. 

  • 微型批处理 或者小间隔执行批处理计算，例如 Spark  Streaming  and  Storm  Trident.   根据微型批处理设置的时间窗口，将流入的数据记录缓冲到一个容器中。 Spark  Streaming  经常用来与 Storm 比较，这是两种不同的流数据模型。 Spark  Streaming 基于微型批处理，而  Storm 基于及时处理流入的数据。 Storm  也提供了微型批处理选项 即 Storm  Triden。 
流处理应用的主要驱动因素的时间延迟。时延从  RPC  (short  for  Remote  Procedure  Call) 的毫秒 到几秒 到微信批处理方案的几分钟不等。  RPC  允许请求程序与远程服务器之间的同步操作。线程允许多个   RPC  调用的并发。   

分布式RPC 模型实现的一个实例就是  Apache  Storm. Storm  实现了无边界元组的无状态毫秒级延迟处理，结合数据流作为喷发源使用了拓扑或定向环图的及时，提供了过滤，  join,  聚合和转换.   Storm  也实现了一个高层抽象叫做 Trident ,  与Spark类似,  以微型批处理进行流式数据处理。  

考虑时延的限制, 从毫秒到秒级,  Storm  是个很好的候选.  对于秒到分钟级,  Spark  Streaming  and  Storm  Trident 是最佳选择。  对于若干分钟以上，  Spark  和  NoSQL  数据库如 Cassandra  和HBase 是合适的方案.  对于多小时运算海量数据，Hadoop 是理想的竞争者。 尽管吞吐量逾时延相关，但不是一个简单的反线性关系。 如果处理一条消息需要2  ms,  只取决于延迟的情况, 可以假设吞吐量限制在500条消息每秒。如果缓存了8ms的数据，那么批处理允许更高的吞吐量。 10  ms 时延, 系统可以缓冲10,000  条消息.  为了一个可忍受的时延增长，我们要显著的增加吞吐量。这是Spark  Streaming 微型批处理的一个魔法。

# Spark Streaming 内部工作方式

  Spark  Streaming  充分利用了 Spark  的核心架构，作为流处理功能的入口点，它构建在 SparkContext 的 StreamingContext  之上. 集群管理器将至少单独分片一个工作节点作为接收器，这是一个长时间运行的任务执行器来处理进入的流数据。 执行器创建 Discretized  Streams 或者从输入数据流中得来的 DStreams， DStream 默认为另一个worker 的缓存。 接收器服务于输入数据流，多个接收器提升了并行性，产生多个DStreams ， Spark  用它unite  或  join RDD. 

下图给出了 Spark  Streaming 的那边工作概要。客户端通过集群管理器与  Spark 集群交互 ，Spark Streaming  有一个专属的worker  运行着长任务来接待输入的数据流，转化为 discretized  streams 或DStreams.  接收器采集，缓存，复制数据，然后放入一个 RDDs的流. 
![5-2 Spark Streaming 内部工作方式](https://img-blog.csdn.net/20160512091224496)
Spark  接收器能够从多种数据源接收数据。核心的输入源包括从TCP  socket  和 HDFS/Amazon  S3  到 Akka  Actors.  其它的数据源包括 Apache  Kafka,  Apache  Flume,  Amazon  Kinesis,  ZeroMQ,  Twitter,  以及其它用户定义的接收器。 

我们可以区分资源是否可靠，可靠的数据源可以确认数据的接收凭证以及副本能否重发,非可靠数据源指接收器不能确认消息的接收凭证。 Spark 可以对workers，分区和接收器的数量进行缩放。  

下图给出了Spark  Streaming  可能的数据源和持久化选项: 
![5-3 Spark Streaming 概览](https://img-blog.csdn.net/20160512091627212)
# Spark Streaming 的底层基础

Spark  Streaming 由接收器，  Discretized  Streams 和 用于持久化的Spark  Connectors  组成. 作为 Spark  Core,  基本的数据结构是 RDD, 基本编程抽是  Discretized  Stream  或  DStream.

下图解释了看做 RDDs连续序列的  Discretized  Streams.   DStream 的批处理间隔是可配置的。 

![5-4 RDD-Descretized Stream](https://img-blog.csdn.net/20160512092006968)

DStreams  是在批处理间隔是对流入数据的快照。时间步长可以从500ms 到几秒，DStream 的底层结构是一个  RDD.  一个DStream 基本上是 RDDs的一个连续序列。这使得Spark Streaming 非常强大，可以充分利用Spark Core 中已有的函数，数据转换和动作，  可以与Spark  SQL 对话,  在流入的数据流上执行SQL 查询和 Spark  MLlib. 通用的及基于键值对RDDs的数据转换同样适用。DStreams 得益于 RDDs 内部体系和容错机制。  discretized  stream  有额外的转换和输出操作。DStream 的大多数通用操作是  transform  和 foreachRDD.  

 下图给出了 DStreams的生命周期，将消息通过微型批处理实现创建成  RDDs ，RDDs上的数据转换函数和动作 触发了 Spark  jobs.  从上而下分布解释：  

 1. 输入流中, 流入的消息缓存在容器中，时间窗口有微型批处理分配.  

 2. 在 discretized  stream  步骤中,  缓存的微型批处理转化成 DStream  RDDs. 

3. Mapped  DStream  是通过使用转换函数从原始的 DStream中获得的.  这三个步骤构成了在预定义时间窗口的所接收数据的转换。 底层的数据结构是RDD,  我们保留了转换中的数据世系。 

4.  最后一步是 RDD上的一个动作，触发了Spark  job. 
![5-5 Dstream ](https://img-blog.csdn.net/20160512092511657)
数据转换可以无状态的或者有状态的。无状态意味着没有状态需要程序维护，而有状态意味着程序保持状态，前面所记住的操作可能影响当前的操作。一个有状态操作需要或修改一些系统的状态，无状态操作则不是。 无状态数据转换在一个时间点处理每个批处理中的一个 DStream.  有状态数据转换处理多个批处理来获得结果，需要可配置的目录检查点。检查点是容错体系的主要机制， Spark  Streaming  周期性存储一个应用的数据和元数据。Spark  Streaming 中的有状态数据转换有两种类型：updateStateByKey  和  windowed  transformations. 

updateStateByKey 维护了在  Pair  RDD 中一个流中的每个键值的状态。当DStream中的键值涨停被更新时返回一个新的状态。  一个例子是tweets的stream 中运行了given  hashtags  的个数统计。Windowed  transformations  在多个批处理的滑动窗口中执行。一个窗口有一个定义好的长度或时间单元的区间，是多个单一  DStream 处理的间隔，定义了一个窗口转换中有多少个批处理或窗口转换中计算的频率。  下面的 schema 在  DStreams上的窗口操作，在给定长度和滑动间隔时生成  window  DStreams  : 
![5-6 Windowing on Dstreams](https://img-blog.csdn.net/20160512092833088)
函数示例是 countByWindow  (windowLength,  slideInterval). 在 DStream上的一个滑动窗口中，计算每个单元素RDD的元素个数，返回一个新的DStream.  通过每60秒计算一次tweets流中指定的hashtags的数据作为示例的解释。 窗口的时间帧是指定的。分钟级的窗口长度是有道理的。 由于是计算和内存密集型的，所以小时级的窗口长度是不推荐的。在Cassandra  或 HBase 这样的数据中聚合数据更为方便。窗口数据转换计算出的结果是基于窗口长度和窗口滑动间隔的。 

Spark 的性能受窗口长度，窗口滑动间隔，和持久化的影响。

# 构建容错系统

实时流处理系统必须是 24/7可用的，这需要在各种错误发生时系统是可靠的。   Spark  和 RDD  抽象 被设计成可以无缝处理集群中任何节点的故障. 

Spark  Streaming 容错处理的主要机制是 核对检验点，自动重启驱动， 和自动故障切换。检验点中存储了应用的状态，使它 Spark能够从驱动故障中国年恢复。   

Spark  Version  1.2 通过写前日志, 可靠的接收器,  和文件流保证了数据的零丢失。  写前日志代表了接收数据的容错存储。出现故障要求重新计算结果。  DStream  操着有着准确的语意。数据转换可能计算多次，但产生相同的结果。 DStream  输出操作至少有一个语意。输出操作也可能执行多次。 

# 以TCP sockets处理实时数据

理解了流操作的基础，先在TCP  socket 上作实验。 TCP  socket  建立在客户端和服务器之间 的双工通信，通过已建的连接交互数据。 WebSocket  与Http 连接不一样，是长连接。 HTTP 不会在server侧保持一个连接并连续推送数据到web浏览器。很多  web应用因而通过AJAX和XML请求采用了长轮询 .  WebSockets 是HTML5 中实现的标准，支持现代的  web浏览器并称为了实时通讯的跨平台标准。

## 创建TCP sockets

运行netcat 创建一个  TCP  Socket  Server,  这是Linux系统中的一个小工具可以作为数据服务器，命令如下:  

```
#
#  Socket  Server
#
an@an-VB:~$  nc  -lk  9999
hello  world
how  are  you
hello   world
cool  it  works
```

一旦 netcat运行起来，  以Spark  Streaming 的客户端打开第二个控制台来接收和处理数据。  一旦 Spark  Streaming  客户端控制台监听我们的键入单词并处理, 就是反馈,  hello  world.  

## 处理实时数据

我们将使用 Spark 提供的Spark Streaming 示例程序  network_wordcount.py. 可以从GitHub 获得 [https://github.com/apache/spark/blob/master/examples/src/main/python/streaming/network_wordcount.py](https://github.com/apache/spark/blob/master/examples/src/main/python/streaming/network_wordcount.py). 

代码如下: 

```python
""" Counts  words  in  UTF8  encoded,  '\n'  delimited  text  received  from  the network  every  second. Usage:  network_wordcount.py  <hostname>  <port> <hostname>  and  <port>  describe  the  TCP  server  that  Spark  Streaming would  connect  to  receive  data. To  run  this  on  your  local  machine,  you  need  to  first  run  a  Netcat server `$  nc  -lk  9999` and  then  run  the  example `$  bin/spark-submit  examples/src/main/python/streaming/network_ wordcount.py  localhost  9999` """ 
from  __future__  import  print_function
  import  sys
from  pyspark.streaming  import  StreamingContext  
if  __name__  ==  "__main__": 
    if  len(sys.argv)  !=  3: 
        print("Usage:  network_wordcount.py  <hostname>  <port>", file=sys.stderr) 
        exit(-1) 
    sc  = SparkContext(appName="PythonStreamingNetworkWordCount")
    ssc = StreamingContext(sc,  1)  
    lines  =  ssc.socketTextStream(sys.argv[1], int(sys.argv[2])) 
    counts  =  lines.flatMap(lambda  line:  line.split("  "))\ .map(lambda  word:  (word,  1))\ 
    .reduceByKey(lambda  a,  b:  a+b) 
  ssc.start()  
 ssc.awaitTermination()
```

解释一下程序的过程:  

1. 第一行代码初始化 : `ssc  =  StreamingContext(sc,  1)`

2.接着, 建立流式计算.  

3. 连接localhost  或  127.0.0.1  的9999端口从接收数据中得到一个或多个 DStream 对象 : `stream  =  ssc.socketTextStream("127.0.0.1",  9999)`

4.  定义DStream 的计算:  数据转换和输出操作: `stream.map(x:  lambda  (x,1)) .reduce(a+b) .print()`

5.  开始计算: `ssc.start()`

6. 手工刮起活错误处理完成时中断程序： `ssc.awaitTermination()`

7.  当到达完成条件时，可以手工完成处理: `ssc.stop()` 通过浏览Spark 的监测主页localhost：4040 可以监测Spark  Streaming  流应用 .  

这里是程序运行的结果以及netcat记录的字符: 
```
# #  Socket  Client #  
an@an-VB:~/spark/spark-1.5.0-bin-hadoop2.6$  ./bin/spark-submit
examples/src/main/python/streaming/network_wordcount.py  localhost  9999
```

连接localhost  的9999端口 运行Spark  Streaming  network_count 程序:

```python
an@an-VB:~/spark/spark-1.5.0-bin-hadoop2.6$  ./bin/spark-submit  examples/
src/main/python/streaming/network_wordcount.py  localhost  9999
-------------------------------------------
Time:  2015-10-18  20:06:06
-------------------------------------------
(u'world',  1)
(u'hello',  1)

-------------------------------------------
Time:  2015-10-18  20:06:07
-------------------------------------------
.  .  .
-------------------------------------------
Time:  2015-10-18  20:06:17
-------------------------------------------
(u'you',  1)
(u'how',  1)
(u'are',  1)

-------------------------------------------
Time:  2015-10-18  20:06:18
-------------------------------------------

.  .  .

-------------------------------------------
Time:  2015-10-18  20:06:26
-------------------------------------------
(u'',  1)
(u'world',  1)
(u'hello',  1)
```

因此,  已经建立了本地9999端口的socket连接,流式处理  netcat 发送的数据 ,  执行发送消息的单纯统计。  

# 实时控制Twitter数据



Twitter  提供了两个 APIs. 搜索 API允许我们根据搜索条目获得过去的  tweets.  这就是我们前面章节如何从Twitter采集数据的方式。我们的目的是通过,Twitter 提供的实时streaming  API ，从博客世界接收用户刚发送的   tweets  .  

# 实时处理Tweets

 下面的程序连接了 Twitter服务，处理流入的 tweets单不包括已删除或无效的 tweets，实时解析相关tweet提取   screen 名称,   或  tweet  text,  retweet  count,  geo-location  信息.  处理过的  tweets  通过Spark  Streaming 收集到一个RDD  Queue，然后每隔一秒显示在控制台上 : 


```python
""" 
 Twitter  Streaming  API  Spark  Streaming  into  an  RDD-Queue  to  process tweets  live Create  a  queue  of  RDDs  that  will  be  mapped/reduced  one  at  a  time  in 1  second  intervals.

To  run  this  example  use
'$  bin/spark-submit  examples/AN_Spark/AN_Spark_Code/s07_
twitterstreaming.py'
"""

#
import  time
from  pyspark  import  SparkContext
from  pyspark.streaming  import  StreamingContext
import  twitter
import  dateutil.parser
import  json
 #  Connecting  Streaming  Twitter  with  Streaming  Spark  via  Queue 

 class  Tweet(dict):
    def  __init__(self,  tweet_in):
        super(Tweet,  self).__init__(self)
        if  tweet_in  and  'delete'  not  in  tweet_in:
            self['timestamp']  =  dateutil.parser.parse(tweet_
            in[u'created_at']
            ).replace(tzinfo=None).isoformat()
            self['text']  =  tweet_in['text'].encode('utf-8')
            #self['text']  =  tweet_in['text']
            self['hashtags']  =  [x['text'].encode('utf-8')  for  x  in
            tweet_in['entities']['hashtags']]
            #self['hashtags']  =  [x['text']  for  x  in  tweet_
            in['entities']['hashtags']]
            self['geo']  =  tweet_in['geo']['coordinates']  if  tweet_
            in['geo']  else  None
            self['id']  =  tweet_in['id']
            self['screen_name']  =  tweet_in['user']['screen_name'].
            encode('utf-8')
            #self['screen_name']  =  tweet_in['user']['screen_name']
            self['user_id']  =  tweet_in['user']['id'`

    def  connect_twitter():
        twitter_stream  =  twitter.TwitterStream(auth=twitter.OAuth(
        token  =  "get_your_own_credentials",
        token_secret  =  "get_your_own_credentials",
        consumer_key  =  "get_your_own_credentials",
        consumer_secret  =  "get_your_own_credentials"))
        return  twitter_stream

    def  get_next_tweet(twitter_stream):
        stream  =  twitter_stream.statuses.sample(block=True)
        tweet_in  =  None
        while  not  tweet_in  or  'delete'  in  tweet_in:
            tweet_in  =  stream.next()
            tweet_parsed  =  Tweet(tweet_in)
        return  json.dumps(tweet_parsed)

    def  process_rdd_queue(twitter_stream):
        #  Create  the  queue  through  which  RDDs  can  be  pushed  to
        #  a  QueueInputDStream
        rddQueue  =  []
        for  i  in  range(3):
            rddQueue  +=  [ssc.sparkContext.parallelize([get_next_
            tweet(twitter_stream)],  5)]

        lines  =  ssc.queueStream(rddQueue)
        lines.pprint()

    if  __name__  ==  "__main__":
        sc  =  SparkContext(appName="PythonStreamingQueueStream")
        ssc  =  StreamingContext(sc,  1)

        #  Instantiate  the  twitter_stream
        twitter_stream  =  connect_twitter()
        #  Get  RDD  queue  of  the  streams  json  or  parsed
        process_rdd_queue(twitter_stream)

        ssc.start()
        time.sleep(2)
        ssc.stop(stopSparkContext=True,  stopGraceFully=True)
```

运行这个程序，有如下输出:

```
an@an-VB:~/spark/spark-1.5.0-bin-hadoop2.6$  bin/spark-submit  examples/
AN_Spark/AN_Spark_Code/s07_twitterstreaming.py
-------------------------------------------
Time:  2015-11-03  21:53:14
------------------------------------------- 
{"user_id":  3242732207,  "screen_name":  "cypuqygoducu",  "timestamp":
"2015-11-03T20:53:04",  "hashtags":  [],  "text":  "RT  @VIralBuzzNewss:
Our  Distinctive  Edition  Holiday  break  Challenge  Is  In  this  article!
Hooray!...  -   https://t.co/9d8wumrd5v  https://t.co/\u2026",  "geo":  null,
"id":  661647303678259200}

-------------------------------------------
Time:  2015-11-03  21:53:15
-------------------------------------------
{"user_id":  352673159,  "screen_name":  "melly_boo_orig",  "timestamp":
"2015-11-03T20:53:05",  "hashtags":  ["eminem"],  "text":  "#eminem
https://t.co/GlEjPJnwxy",  "geo":  null,  "id":  661647307847409668}

-------------------------------------------
Time:  2015-11-03  21:53:16
-------------------------------------------
{"user_id":  500620889,  "screen_name":  "NBAtheist",  "timestamp":  "2015-11-
03T20:53:06",  "hashtags":  ["tehInterwebbies",  "Nutters"],  "text":  "See?
That  didn't  take  long  or  any  actual  effort.  This  is  #tehInterwebbies
...     #Nutters  Abound!  https://t.co/QS8gLStYFO",  "geo":  null,  "id":
661647312062709761}
```

以, 我们有了一个用Spark流式接收并飞速地处理它们.  

## 构建一个稳定缩放的流式应用

摄取数据是从各种源中获取数据并马上处理或存储起来稍后处理的过程。数据消费系统是分散的，可以从架构或物理上与数据源分开。数据摄取经常通过手工脚本实现或基本的自动化。实际上，可以调用象 Flume 和Kafka这样的高层框架。数据摄取的挑战来自原数据的物理分散和暂态导致的集成集成脆弱性。对于天气，交通，社交媒体，网络行为，传感器，安全，监控来说，数据产品是连续的。 

不断增长的数据容量和流量以及变化的数据结构和语意是数据摄取混乱并有错误倾向。   

我们的目标是更加的敏捷，可靠和伸缩性。数据摄取的敏捷性，可靠性和伸缩性决定了流水线的健康程度。敏捷性意味着可以集成新的数据源，并按需改变现存的数据源。  为了保证安全可靠， 需要从基础设施上防止数据丢失，防止下游应用在入口处得到的数据遭到破坏。伸缩性避免了摄取瓶颈，能够保持数据的快速处理。

![5-7 数据摄取模式](https://img-blog.csdn.net/20160512100920583)

使所有数据得到快速的响应，核心驱动是统一日志。统一日志是实时订阅的集中化企业结构化日志。所有的组织数据放到一个中心化的日志中。记录编号从零开始，被看作是一个提交日志或者 journal.   Unified  Log  的概念是Kappa 架构的核心宗旨。 Unified  Log 的特性如下:  

  • Unified:  针对所有组织的单一部署。 

 • Append  only: 事件是可追加且不可变的 

  • Ordered: 在一个切片内每个事件有位惟一的偏移量 

   • Distributed:  考虑容错, Unified  Log 冗余分布在集群的各台主机上  

   • Fast:  系统每秒摄取成千上万的消息  
# 搭建 Kafka

为了独立数据上行和数据消费，需要对数据提供者和消费者解耦合。 由于双方有不同的周期和约束,  Kafka 可以解耦合数据处理的流水线.Apache  Kafka  是一个分布式的发布订阅消息系统，也可以理解成一个分布式的提交日志。消息按照话题存储. 

 Apache  Kafka  有如下特性. 它支持: 

 • 高容量事件的高吞吐量 


 • 新的和派生的 feeds 的实时处理   
• 低时延的企业级消息系统 

•容错归功于分区内的分布式消息存储，每个消息都有一个惟一的序列  ID 叫做  offset. Consumers  通过 元组(offset,  partition,  topic)检查它们的指针.  

  深入解剖一下 Kafka.Kafka 有三个基本组件:  producers, consumers 和brokers.  Producers 推送并写数据到brokers.  Consumers 从broker 拉取并读到数据.  Brokers不推送消息给consumers. Consumers 从brokers拉取数据. 分布式协作由 Apache  Zookeeper完成. brokers以话题形式存储和管理数据. 话题分割到复制分区内。数据持久化broker,  在存留期间并不移除.  如果一个consumer 失败了,  它总是回到broker  再次获取数据. 


   Kafka  需要 Apache  ZooKeeper支持.  ZooKeeper 是分布式应用的一个高性能协调服务。它集中管理配置，注册或命名服务，组的成员关系，锁， 服务器间的同步协调，提供了层次化命名空间，有元数据，监测统计和集群状态。  ZooKeeper  可以快速地引入 brokers和  consumers，然后在集群内重新均衡。  


   Kafka  producers 不需要 ZooKeeper.  Kafka  brokers  使用  ZooKeeper提供通用的状态信息在故障的时候选举leader.  Kafka  consumers  使用ZooKeeper  跟踪消息的偏移量.  新版本的 Kafka 将通过 ZooKeeper  保存consumers，并提取Kafka 中特定的话题信息 .Kafka 为producers提供了自动的负载均衡. 下图描述了Kafka  的建立过程: 
![5-8 Kafka 架构](https://img-blog.csdn.net/20160512110934303)
## 安装测试Kafka

下载 Apache  Kafka  执行文件  [http://kafka.apache.org/downloads.html](http://kafka.apache.org/downloads.html)  按下列步骤安装软件:  

1. 下载代码 .  

2.  下载  0.8.2.0  版本并解压:
> 
tar  -xzf  kafka_2.10-0.8.2.0.tgz 

   cd  kafka_2.10-0.8.2.0  


3.启动zooeeper. 使用一个方便的脚步让Kafka 得到 ZooKeeper 的一个单节点实例   

```
. >  bin/zookeeper-server-start.sh  config/zookeeper.properties 
an@an-VB:~/kafka/kafka_2.10-0.8.2.0$  bin/zookeeper-server-start.sh config/zookeeper.properties  

[2015-10-31  22:49:14,808]  INFO  Reading  configuration  from: config/zookeeper.properties  (org.apache.zookeeper.server.quorum. QuorumPeerConfig) 
[2015-10-31  22:49:14,816]  INFO  autopurge.snapRetainCount  set  to  3 (org.apache.zookeeper.server.DatadirCleanupManager)..
```

4.启动 Kafka  server: 

```
>  bin/kafka-server-start.sh  config/server.properties  
an@an-VB:~/kafka/kafka_2.10-0.8.2.0$  bin/kafka-server-start.sh config/server.properties 
[2015-10-31  22:52:04,643]  INFO  Verifying  properties  (kafka.utils. VerifiableProperties) 
[2015-10-31  22:52:04,714]  INFO  Property  broker.id  is  overridden  to 0  (kafka.utils.VerifiableProperties) 
[2015-10-31  22:52:04,715]  INFO  Property  log.cleaner.enable  is overridden  to  false  (kafka.utils.VerifiableProperties) 
[2015-10-31  22:52:04,715]  INFO  Property  log.dirs  is  overridden  to /tmp/kafka-logs  (kafka.utils.VerifiableProperties)  
[2013-04-22 15:01:47,051]  INFO  Property  socket.send.buffer.bytes  is  overridden to  1048576  (kafka.utils.VerifiableProperties)
```

5.创建主题.  这里创建一个名为 test 的主题，只有一个分区和一个副本  : 
`>  bin/kafka-topics.sh  --create  --zookeeper  localhost:2181 --replication-factor  1  --partitions  1  --topic  test `
6.如果运行 list  topic  命令，可以看到这一主题: 

```
>  bin/kafka-topics.sh  --list  --zookeeper  localhost:2181 Test 
an@an-VB:~/kafka/kafka_2.10-0.8.2.0$  bin/kafka-topics.sh  --create --zookeeper  localhost:2181  --replication-factor  1  --partitions  1 --topic  test 
Created  topic  "test". 
an@an-VB:~/kafka/kafka_2.10-0.8.2.0$  bin/kafka-topics.sh  --list --zookeeper  localhost:2181 test
```

7.通过创建一个  producer  and  consumer来检查Kafka  的安装。 先创建一个  producer ，从控制台键入消息:  

```
an@an-VB:~/kafka/kafka_2.10-0.8.2.0$  bin/kafka-console-producer.sh --broker-list  localhost:9092  --topic  test 
  [2015-10-31  22:54:43,698]  WARN  Property  topic  is  not  valid  (kafka. utils.VerifiableProperties) 
  This  is  a  message 
  This  is  another  message
```



  8. 启动一个 consumer  检查接收到的消息: an@an-VB:~$  cd  kafka/ an@an-

```
VB:~/kafka$  cd  kafka_2.10-0.8.2.0/ 
 an@an-VB:~/kafka/kafka_2.10-0.8.2.0$  bin/kafka-console-consumer.sh --zookeeper  localhost:2181  --topic  test  --from-beginning  
 This  is  a  message
This  is  another  message
```

consumer 正确接收消息的过程:  

1.检查Kafka  和  Spark  Streaming  consumer.  使用 Spark Streaming  Kafka  单词统计示例. 要注意的是:  当发送Spark job的时候，必须绑定 Kafka  包,  –packages  org.apache. spark:spark-streaming-kafka_2.10:1.5.0.  命令如下:
` ./bin/spark-submit  --packages  org.apache.spark:spark-streaming- kafka_2.10:1.5.0  \  examples/src/main/python/streaming/kafka_ wordcount.py  \ localhost:2181  test `
2.当用Kafka 启动Spark  Streaming  单词计算程序的时候,  得到下面的输出:  

```python
an@an-VB:~/spark/spark-1.5.0-bin-hadoop2.6$  ./bin/spark-submit --packages  org.apache.spark:spark-streaming-kafka_2.10:1.5.0 examples/src/main/python/streaming/kafka_wordcount.py localhost:2181  test 
-------------------------------------------
Time:  2015-10-31  23:46:33  
-------------------------------------------
(u'',  1)
(u'from',  2)
(u'Hello',  2)
(u'Kafka',  2)

-------------------------------------------
Time:  2015-10-31  23:46:34
-------------------------------------------

-------------------------------------------
Time:  2015-10-31  23:46:35
-------------------------------------------
```

3.安装Kafka  Python  driver 以便对 Producers  and  Consumers  编程，并使用python 与Kafka  and  Spark 交互.  我门使用David  Arthur开发的库,  aka,  Mumrah on  GitHub 网址 ([https://github.com/mumrah](https://github.com/mumrah)).  安装明亮如下:  

```
>  pip  install  kafka-python an@an-VB:~$  pip  install  kafka-python 
 Collecting  kafka-python 
 Downloading  kafka-python-0.9.4.tar.gz  (63kB) ... 
 Successfully  installed  kafka-python-0.9.4
```

## 开发 producers

下面的程序创建了一个简单的Kafka  Producer ,来发送一个消息 *this  is  a  message  sent  from  the  Kafka  producer*:  5次，带时间戳: 

```
# #  kafka  producer # # 
import  time 
from  kafka.common  import  LeaderNotAvailableError
 from  kafka.client  import  KafkaClient
  from  kafka.producer  import  SimpleProducer  
  from  datetime  import  datetime

def  print_response(response=None):
    if  response:
        print('Error:  {0}'.format(response[0].error))
        print('Offset:  {0}'.format(response[0].offset))

def  main():
    kafka  =  KafkaClient("localhost:9092")
    producer  =  SimpleProducer(kafka)
    try:
        time.sleep(5)
        topic  =  'test'
        for  i  in  range(5):
            time.sleep(1)
            msg  =  'This  is  a  message  sent  from  the  kafka  producer:  '  \
            +  str(datetime.now().time())  +  '  --  '\
            +  str(datetime.now().strftime("%A,  %d  %B  %Y
            %I:%M%p"))
            print_response(producer.send_messages(topic,  msg))
    except  LeaderNotAvailableError:
    #  https://github.com/mumrah/kafka-python/issues/249
    time.sleep(1)
    print_response(producer.send_messages(topic,  msg))

    kafka.close()

if  __name__  ==  "__main__":
    main()
```

运行程序，产生如下输出:

```
an@an-VB:~/spark/spark-1.5.0-bin-hadoop2.6/examples/AN_Spark/AN_Spark_
Code$  python  s08_kafka_producer_01.py
Error:  0
Offset:  13
Error:  0
Offset:  14
Error:  0
Offset:  15
Error:  0
Offset:  16
Error:  0
Offset:  17
an@an-VB:~/spark/spark-1.5.0-bin-hadoop2.6/examples/AN_Spark/AN_Spark_Code$
```

没有错误，并且Kafka  broker 给出了消息的偏移量。

## 开发 consumers

为了从 Kafka  brokers获取消息,  开发一个Kafka  consumer: 

```python
#  kafka  consumer
#  consumes  messages  from  "test"  topic  and  writes  them  to  console.
#
from  kafka.client  import  KafkaClient
from  kafka.consumer  import  SimpleConsumer

def  main():
    kafka  =  KafkaClient("localhost:9092")
    print("Consumer  established  connection  to  kafka")
    consumer  =  SimpleConsumer(kafka,  "my-group",  "test")
    for  message  in  consumer:
    #  This  will  wait  and  print  messages  as  they  become  available
        print(message)

if  __name__  ==  "__main__":
    main()
```

运行程序, 可以确认consumer  收到了所有消息:

```
an@an-VB:~$  cd  ~/spark/spark-1.5.0-bin- hadoop2.6/examples/AN_Spark/AN_Spark_Code/
an@an-VB:~/spark/spark-1.5.0-bin－hadoop2.6/examples/AN_Spark/AN_Spark_Code$  python  s08_kafka_consumer_01.py

Consumer  established  connection  to  kafka
OffsetAndMessage(offset=13,  message=Message(magic=0,  attributes=0,
key=None,  value='This  is  a  message  sent  from  the  kafka  producer:
11:50:17.867309Sunday,  01  November  2015  11:50AM'))
...
OffsetAndMessage(offset=17,  message=Message(magic=0,  attributes=0,
key=None,  value='This  is  a  message  sent  from  the  kafka  producer:
11:50:22.051423Sunday,  01  November  2015  11:50AM'))
```

## 在Kafka 上开发Spark   Streaming   consumer

基于Spark  Streaming  中提供的示例代码,  创建 一个Kafka的  Spark  Streaming  consumer，并对Brokers中存储的消息执行 单词统计: 

```
#
#  Kafka  Spark  Streaming  Consumer
#
from  __future__  import  print_function

import  sys

from  pyspark  import  SparkContext
from  pyspark.streaming  import  StreamingContext
from  pyspark.streaming.kafka  import  KafkaUtils

if  __name__  ==  "__main__":
    if  len(sys.argv)  !=  3:
        print("Usage:  kafka_spark_consumer_01.py  <zk>  <topic>",
        file=sys.stderr)
        exit(-1)

    sc  =  SparkContext(appName="PythonStreamingKafkaWordCount")
    ssc  =  StreamingContext(sc,  1)

    zkQuorum,  topic  =  sys.argv[1:]
    kvs  =  KafkaUtils.createStream(ssc,  zkQuorum,  "spark-streaming-
    consumer",  {topic:  1})
    lines  =  kvs.map(lambda  x:  x[1])
    counts  =  lines.flatMap(lambda  line:  line.split("  "))  \
    .map(lambda  word:  (word,  1))  \
    .reduceByKey(lambda  a,  b:  a+b)
    counts.pprint()

    ssc.start()
    ssc.awaitTermination()
```

执行程序，并运行  Spark  submit  命令:

```
./bin/spark-submit  --packages  org.apache.spark:spark-streaming-
kafka_2.10:1.5.0  examples/AN_Spark/AN_Spark_Code/s08_kafka_spark_
consumer_01.py  localhost:2181  test
```

得到如下输出：

```python
an@an-VB:~$  cd  spark/spark-1.5.0-bin-hadoop2.6/
an@an-VB:~/spark/spark-1.5.0-bin-hadoop2.6$  ./bin/spark-submit \
--packages  org.apache.spark:spark-streaming-kafka_2.10:1.5.0  \
examples/AN_Spark/AN_Spark_Code/s08_kafka_spark_consumer_01.py
localhost:2181  test
...
::  retrieving  ::  org.apache.spark#spark-submit-parent
confs:  [default]
0  artifacts  copied,  10  already  retrieved  (0kB/18ms)
-------------------------------------------
Time:  2015-11-01  12:13:16
-------------------------------------------

-------------------------------------------
Time:  2015-11-01  12:13:17
-------------------------------------------

-------------------------------------------
Time:  2015-11-01  12:13:18
-------------------------------------------

-------------------------------------------
Time:  2015-11-01  12:13:19
-------------------------------------------
(u'a',  5)
(u'the',  5)
(u'11:50AM',  5)
(u'from',  5)
(u'This',  5)
(u'11:50:21.044374Sunday,',  1)
(u'message',  5)
(u'11:50:20.036422Sunday,',  1)
(u'11:50:22.051423Sunday,',  1)
(u'11:50:17.867309Sunday,',  1)
...

-------------------------------------------
Time:  2015-11-01  12:13:20
-------------------------------------------

-------------------------------------------
Time:  2015-11-01  12:13:21
-------------------------------------------
```

# 探索flume

Flume 是一个连续摄取数据的系统，最初被设计成一个日志聚会系统，但演变为可以处理任何类型的流事件数据。 Flume 是一个分布式，可靠的，具有伸缩性的流水线系统，可以有效地采集，聚会，传输大容量数据，内置支持上下文路由，过滤复制，和多路复用。 容错和健壮性好，拥有可调的可靠性机制，多种故障切换和恢复机制.  它使用简单可扩展的数据模型应用于实时分析。

 Flume  提供了:  

 • 保证传输的语意  

 • 低时延可靠的数据传输  

 • 不需要编码的声明式配置  

 • 可扩展和定制的设置 

  • 集成了大多数通用的端点数据源 
  Flume  包括以下元素: 

• Event:  一个事件是Flume从源到目的地址传输数据的基本单元.  象一条消息那样，对 Flume  而言是不透明的字节数组，通过可选的头信息来做上下文路由。  

 • Client:  一个 client  生产和传输事件.  客户端把 Flume和数据消费者解耦合，是一个生成事件并把它们发送到一个或多个代理 的实体，可以是定制的客户端或者  Flume  log4J  程序 或  潜入到应用中的代理.  

 • Agent:  一个代理是一个容器，承载了源，信道， sinks, 和其它元素使数据能在不同地址间传输。它对托管的组件提供了配置，生命周期管理和监控。代理在物理上是一个运行 Flume的Java 虚拟机。 

  • Source:  源师Flume 接收事件的实体，至少使用一个信道，通过该信道轮询数据或者等待数据发送给它，有各种采集数据的源，例如  log4j  logs 和 syslogs.  

  • Sink:  Sink 是一个实体，将信道中的数据排空并发送到下一个目的地址。 sinks 允许数据被流式发送到一定范围的目的地址。 Sinks  支持序列化到用户的格式.  例如 HDFS  sink 将事件写到 HDFS. 

• Channel:  信道是源和sink之间的管道，缓存流入的事件之道sink 将信道中的数据排空。源给信道提供事件，Sink 排空信道。 信道解耦合了上下行系统，抑制了上行数据的突发性， 客服了下行故障。为了实现这一目的，关键是调整处理事件的信道容量。 信道提供了两种层次的持久化:  内存型信道和文件型信道。当JVM崩溃时，内存型信道是不稳定的, 而文件型信道通过写前日志 把信息存储到硬盘，所以可以回复。信道是全事务型的。 
下图解释了这些概念:  
![5-8 Flume](https://img-blog.csdn.net/20160512171855286)
# 基于Flume, Kafka和Spark开发数据流水线

利用我们已学的知识，可以构建一个弹性的数据处理流水线。通过Flume 将数据摄取和传输放到一起，把Kafka 这样的可靠的发布订阅消息系统作为数据代理，最后使用Spark Streaming 完成高速的计算处理。 

下图解释了流式数据流水线的组成 connect,  collect,  conduct,  compose,  consume,  consign,  和 control.  这些活动根据用例来配置:  

 + Connect 建立了streaming  API的绑定  

  + Collect  创建了采集线程.  

+ Compose 专注于处理数据 

+ Consume  为消费系统提供处理后的数据 

+   Consign  负责数据持久化 

+  Control  负责系统、数据和营养的统筹和监控。
![5-9 Streaming 数据流水线](https://img-blog.csdn.net/20160512190819471)

下图介绍了流式数据流水线的概念及关键组件:  Spark  Streaming,  Kafka,  Flume和低时延数据库.  在消费或控制型应用中，需要实时监控系统，或者在超出一定阈值时实时发送告警。

![5-10 flume kafka streaming](https://img-blog.csdn.net/20160512191149349)

下图介绍了Spark 在处理单平台运动数据和闲置数据的独特能力，即根据不同的用例需求与多个持久化数据存储无缝对接。

![5-11 运动数据处理](https://img-blog.csdn.net/20160512191411055)

这张图是到目前为止所谈论的所有概念。顶部描述了流式处理流水线。底部描述了批处理流水线 ，两者共享位于中间的通用持久化层，包含各种模式的持久化和序列化。 




















