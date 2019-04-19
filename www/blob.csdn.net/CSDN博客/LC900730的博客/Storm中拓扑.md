# Storm中拓扑 - LC900730的博客 - CSDN博客
2017年12月08日 10:13:37[lc900730](https://me.csdn.net/LC900730)阅读数：165
TopologyBuilder是构建拓扑的类，用于指定执行的拓扑。
### 创建提交拓扑
- new关键字创建一个TopologyBuilder对象，然后调用setSpout方法设置Spout，接着调用setBolt方法设置Bolt，最后调用createTopology方法返回StormTopology对象给submitTopology方法作为输入参数。
```
//创建TopologyBuilder对象
TopologyBuilder builder=new TopologyBuilder();
//添加一个id为‘1’，并行度为5的TestWordSpout对象
builder.setSpout('1',new TestWordSpout(treu),5);
//添加一个id为‘2’，并行度为3的TestWordSpout对象
builder.setSpout('2',new TestWordSpout(treu),3);
//添加一个id为‘3’，并行度为3的TestWordSpout对象
//对id为1的组件word字段进行分组
//对id为2的组件word字段进行分组
builder.setBolt('3',new TestWordCounter(),3)
    .fieldsGrouping("1",new Fields('word'))
    .fieldsGrouping("2",new Fields('word'))
//添加一个id为4，并行度为1的TestGlobalCount对象
//对id为1的组件按全局分组
builder.setBolt('4',new TestGlobalCount(),1).
    globalGrouping('1');
//创建HashMap对象
Map conf=new HashMap();
//创建worker数量为4
conf.put(Config.TOPOLOGY_WORKERS,4);
//提交拓扑
StromSubmitter.submitTopology('mytopology',conf,builder.createTopology());
```
#### 消息源Spout->继承BaseRichSpout类，实现IRichSpout接口：
open()：初始化动作； 
nextTuple()：消息接入，执行数据发射； 
ack()：tuple成功处理后调用 
fail()：tuple处理失败时候调用 
declareOutputFields()：通常声明输出字段
#### 处理单元Bolt->继承BaseBasicBolt类/实现IRichBolt接口
prepare()：worker启动时候初始化； 
execute()：接受一个tuple并执行逻辑处理，发射出去 
cleanup()：关闭前调用 
declareOutputFields()：字段声明
//提交topology 
storm jar all-mycode.jar backtype.storm.MyTopology arg1 arg2 
该命令使用参数arg1和arg2来运行all-my-code.jar包里面的类backtype.storm.MyTopology。类的主要功能是定义了拓扑，并将其提交到Nimbus。Storm jar命令部分负责连接nimbus和上传jar包。
