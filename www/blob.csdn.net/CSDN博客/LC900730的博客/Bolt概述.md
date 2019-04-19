# Bolt概述 - LC900730的博客 - CSDN博客
2017年12月08日 16:31:47[lc900730](https://me.csdn.net/LC900730)阅读数：178
Bolt是一个组件，以元组作为输入，以生成元组作为输出。当编写Bolt的时候，通常会实现IRichBolt接口。在客户端主机中创建Bolt，序列化到拓扑，并提交到集群中的主控节点。集群启动Worker,反序列化Bolt，准备调用它，然后开始处理元组。
prepare():仅在bolt开始处理Tuple之前调用。 
execute():处理一个Tuple的输入。
继承BaseRichBolt的Bolt类，还需要重写declareOutputFields()、prepare()、execute(); 
* 在declareOutputfields()方法中声明字段； 
* 在prepare()方法中初始化参数和变量 
* 在execute()方法中接收元组，处理逻辑
另外还可以通过实现IRichBolt接口或者IBolt接口，定义逻辑更复杂的Bolt。
