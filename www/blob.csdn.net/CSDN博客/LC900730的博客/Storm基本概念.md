# Storm基本概念 - LC900730的博客 - CSDN博客
2017年12月06日 18:14:48[lc900730](https://me.csdn.net/LC900730)阅读数：154标签：[storm](https://so.csdn.net/so/search/s.do?q=storm&t=blog)
个人分类：[storm](https://blog.csdn.net/LC900730/article/category/7291649)
Spout是计算流的来源。通常在系统中spout从Kestrel、RabbitMQ和kafka等消息队列读取，但是spout还可以生成自己的流或者从某个API读取。 
Bolt处理任意数量的输入流，并且产生任意数量的新的输出流。大多数的逻辑计算进入bolt,如功能、过滤、流连接与数据库交互等。 
Topology是一个由很多Spout和Bolt构成的网络，网络上每条边代表了一个Bolt订阅的数据流，这些数据流包括从Spout或从Bolt输出的数据流。一个Topology实际上就是一个任意复杂的多级流计算过程。当Topology在服务器上部署完以后，它就会一直运行下去，直到用户禁止相应进程。
## 元组(Tuple)
是消息传递的基本单元，是一个命名的值列表，元组中的字段可以是任意类型的对象。 
元组支持所有基本类型、字符串和字节数组作为字段值，只要实现类型的序列化接口就可以使用该类型的对象。元组本来该是一个key-value的map，但是由于各个组件间传递的元组字段名称已经事先定义好，所以只要按序把元组填入各个value即可，所以元组是一个value的List。
## 流(Stream)
流是Storm的核心抽象，是一个无界的元组序列。源源不断传递的元组就组成了流，在分布式环境中并行进行创建和处理。 
流由元组组成，使用OutputFieldsDeclarer声明流及其模式。Serialization是Storm的动态元组类型的信息，声明自定义序列化。自定义序列化必须实现ISerialization接口，自定义序列化可以注册使用CONFIGTOPOLOGY_SERIALIZATIONS这个配置。
Storm提供可靠的方式把原语转化成一个新的分布式流，执行流转换的基本元素是Spout和Bolt。 
Spout是流的源头，通常从外部数据源读取元组，并emit到拓扑中。如Spout从Kestrel队列中读取元组，并作为一个流提交到拓扑中。 
Bolt可以接收任何数量的输入流，执行处理后，可能提交新的流。复杂流的转换，如从tweets流中计算一个热门话题，需要多个步骤，因此需要多个Bolt。Bolt可以执行运行函数、过滤元组、连接流和连接数据库等任何事情。
### 喷口(Spout)
喷口是拓扑的流的来源，是一个拓扑中产生源数据流的组件。
##### 作用：Spout会从外部数据源中读取数据，然后转换为拓扑内部源数据。
Spout可以发出超出一个流，为此使用OutputFieldsDeclarer类的declareStream方法来声明多个流，使用SpoutOutputCollector类的emit执行流的提交。
主要方式：
- nextTuple()。
nextTuple()会发出一个新的Tuple到拓扑，如果没新的元组则简单返回。nextTuple()不阻止任何Spout的实现，因为Storm在同一个线程调用所有Spout方法。
- ack()
- fail()
当Storm检测到一个元组从Spout发出时，ack()和fail()会被调用，要么成功完成拓扑，要么未能完成。ack()和fail()仅被可靠的Spout调用。
IRichSpout是Spout必须实现的接口。
当Storm检测到有向无环图的每个元组中已经被成功处理时，它将会发送一个ack消息到Spout。
```java
public interface ISpout extends Serilizable{
    //在该组件的一个任务在集群的工作进程内被初始化时候被调用，提供了Spout执行需要的环境。
    /**
     * conf:这个Spout的Storm配置，提供给拓扑与这台主机上集群配置一起合并
     * context:获取关于任务在拓扑中位置信息，包括任务id、任务组件id、输入和输出信息。
     * collector 参数是收集器，用于从这个Spout发射元组。收集器是线程安全的，应该作为这个Spout对象实例变量进行保存。
    */
    void open(Map conf,TopologyContext context,SpoutOutputcollector collector)；
    void close();
    /**
    当Spout已经从失效模式中激活时被调用。该spout
     的 nextTuple() 很快会被调用。
    */
    void activate();
    /*
    在Spout失效期间，nextTuple()不会被调用。
    */
    void deactiveate();
    /*
    当调用nextTuple时，Storm要求spout发射元组到输出收集器(OutputCollector)。nextTuple()方法应该是非堵塞的。所有如果Spout没有元组可以发射，该方法应该返回。nextTuple()、ack()和fail()方法都在Spout任务的单一线程内紧密循环被调用。当没有元组可以发射时，可以让nextTuple()去sleep很短的时间，如1ms。这样就不会浪费太多CPU资源。
    */
    void nextTuple();
    /**
    该Spout发射的标识符为msgId的元组被完全处理时，会调用ack方法。通常情况下，ack()会将该消息移除队列防止被重发。
    */
    void ack(Object msgId);
    void fail(Object msgId);
}
```
## Bolt
- 拓扑中所有处理都是在Bolt中完成，Bolt是流的处理节点，从一个拓扑中接收数据然后执行进行处理的组件。Bolt可以完成过滤、业务处理、连接运算、连接与访问数据库等任何操作。是一个被动角色，接口中有一个execute()方法，在接收到消息后会调用此方法，用户可以在其中执行自己希望的操作。
- Bolt可以完成简单流转换，而完成复杂的流转换需要多个步骤，因此需要多个Bolt。
- Bolt可以发出超过一个的流。为此使用OutputFieldsDeclarer类的declareStream()方法声明多个流，并使用OutputCollector类的emit()方法指定发射的流。
- prepare()为Bolt提供一个OutputCollector对象，用于从这个Bolt发射Tuple。Tuple可以随时从Bolt发射，从Bolt的prepare、execute或者cleanup，甚至在另一个线程的异步方法发射。这个prepare可以实现简单地把OutputCollector对象作为一个实例变量保存，使得稍后可以在execute方法中使用。
- execute()从一个Bolt的输入接收一个Tuple。如下ExclamationBlolt从元组中取出第一个字段，把字符串‘!!!’追加到它后面，然后发射出一个新的元组。如果要实现一个订阅多个输入来源的Bolt，通过使用Tuple.getSourcecomponent方法，可以找出Tuple来着哪个组件。
```java
public static class ExclamationBolt implements IRichBolt{
    OutputCollector _collector;
    public void prepare(Map conf,TopologyContext context,Outputcollector collector){
        _collector=collector;
    }
    public void executor(Tuple tuple){
        _collector.emit(tuple,new Values(tuple.getString(0)+"!!!"));
        _collector.ack(tuple);
    }
    public void cleanup(){
    }
    public void declarerOutputFields(OutputFieldsDeclarer declarer){
        declarer.declare(new Fields("word"));
    }
    public Map getComponentConfiguration(){
        return null;
    }
}
```
## 拓扑
类似hadoop中作业job，但是job会结束，拓扑不会结束，除非显式杀死。 
在Java中使用TopologyBuilder类来构造拓扑。
## Config类
- Config类，即backtype.storm.Config类，是一个所有配置的清单，一个创建拓扑特定配置的helper类。
- Config类提供了方便的方法来创建一个拓扑配置Map，为所有可以设置项提供了setter方法，这也使得它很容易实现序列化。
- Config类还提供了Storm集群和Storm拓扑上所有可能的配置的常量，可以在default.yaml文件中找到默认值。
## Spout
ISpout接口的定义如下：
```java
public interface ISpout extends Serializable{
    void open(Map conf, TopologyContext context,SpoutOutputCollector collector);
    void close();
    void nextTuple();
    void ack(Object msgId);
    void fail(Object msgId);
}
```
- Storm调用Spout的nextTuple()方法从Spout请求一个元组，Spout使用open()方法提供的SpoutOutputCollector对象发射一个元组到它的输出流。
- 发射元组时候，Spout会提供一个消息id，以便于用来识别元组。
- 如KestrelSpout从Kestrel消息队列中读取一个消息时，会发射Kestrel提供的消息id，下面发射一个消息到SpoutOutputCollector对象。
`_collector.emit(new Values("field1","field2",3),msgId);`- 接下来，元组被发送到Bolt，同时Strom负责跟踪创建的消息树。如果Storm检测到一个元组是完全处理的，Storm将调用原来Spout任务的ack()方法，把Spout提供给Storm的消息id作为输入参数。如果超时，将调用Spout的fail()方法。
## 拓扑并行度
并行度专门用于表示一个组件的执行器(线程)的初始数量。
#### 1.工作进程的数量
- 描述集群中不同节点的拓扑可以创建多少个工作进程
- Config # setNumWorkers
#### 2.执行器/线程的数量
- 描述每个组件产生多少个执行器
- TopologyBuilder#setSpout()
- TopologyBuilder#setBolt()
#### 3.任务数量
- 描述每个组件创建多少个任务
- ComponentConfigurationDeclarer#setNumTasks()
- T setNumTasks（java.lang.Number val）
### 流分组
当Bolt A的任务发送到Bolt B 时，它应该发送到Bolt B中的哪一个任务呢？‘流分组’告诉storm如何在任务集之间发送元组。
### 设计流时候，重点解决：流如何被Bolt消耗。一个流分组指定每个Bolt消耗哪个流，流如何被消耗。
### 一个节点可以发出多个数据流，流分组允许我们有选择接收流。
