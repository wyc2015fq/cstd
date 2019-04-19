# Tuple接口 - LC900730的博客 - CSDN博客
2017年12月09日 10:10:08[lc900730](https://me.csdn.net/LC900730)阅读数：172
### Tuple是Storm中主要数据结构，在Storm发送接收消息过程中，每一条消息都是一个Tuple对象。
Storm提供了Tuple的默认实现类TupleImpl。它除了实现Tuple接口外，还实现了Clojure定义的几个接口Seqable、Indexed和IMeta，实现这些接口的目的是为了在Clojure代码中能更好操纵Tuple对象。
ack-fail机制： 
* 1.需要ackfail时候，请为每个tuple生成一个messageID，这个messageID是用来标识你关系的tuple。当这个tuple被完全处理时候，storm框架会调用spoutd ack方法，否则调用fail，至于你的消息是否重发，完全自己处理。 
* 2.在Spout有并发度情况下，storm会根据tuple最开始属于的spout taskid，通知相应的spoutTask 
* 3.在流式计算中topology的bolt组件是可以陪着多个的。在每个环节中，都需要显式告诉storm框架。
```
<spoutTaskId,<RootID,ackValue=0>>
spout1--->tuple1(msgId,rootId)
MySpout{
    private Map buffer=new HashMap();
    spout.open()
    spout.nextTuple(){
        collector.emit();
        buffer.put(msgId,messValue);
    }
    spout.outputFields()
    //发送成功后移除
    spout.ack(msgId){
        buffer.remove(msgId);
    }
    spout.fail(msgId){
        //消息重发
        String messvalue=buffer.get(msgId);
        collector.emit();
    }
}
```
- 
ack机制里面，发送2种类型的tuple。一种是原始消息(DataTuple),另外一种是`ackTuple<RootID,tupleID>`,DataTuple中会包含一个MessageId的对象。
spout.emit(DataTuple(MessageId(ackTuple)))—>bolt()
## 任务分配
```
task总数
可用worker数量
```
## 通信机制
```
去zk获取每个组件任务
启动不同服务
    nimbus
    supervisor
    worker supervisor启动----java -server XXX.java main-class(main(Worker.mkwork(new Worker())))
    Task worker启动Task
```
## 心跳机制
