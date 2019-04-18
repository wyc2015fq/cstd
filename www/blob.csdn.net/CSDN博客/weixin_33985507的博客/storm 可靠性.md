# storm 可靠性 - weixin_33985507的博客 - CSDN博客
2017年01月04日 11:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
**去掉storm可靠性有三种方式：**
1、Config.TOPOLOGY_ACKERS 设置为0；
2、在发送数据时不带上mesage id；
3、将tuple不做anchor发送到下一个节点，因为没有anchor到任何spout的tuple，就算没有成功处理被ack，也不会造成tuple fail
> 
如果设置storm的信息处理不需要可靠性保证，spout的等待队列一直为空，因为无需缓存数据。
保证storm可靠性，需要构建tuple树，如果整个树在规定超时时间内不能没有处理完成，则认为是失败。为保证tuple树的构造：
1.在Spout发送数据时带上messageId（collector.emit(new Values(“test),msgId)），这样，当storm认为该tuple被成功处理，将调用spout的ack方法，并将msgId传给spout.
2.在每个bolt处理tuple的时候，因tuple树需要生长，所以在bolt处理完tuple并发送数据的时候，需要通知storm在tuple树添加新的链路，通过anchor即可，anchor链接一个节点时，emit(tuple, new Value(“test”));如果需要同时链接两个节点，通过emit(List<Tupple>, new Value(“test”))
因为storm通过内存追踪元组的状态，所以需要保证可靠性时，一定要ack或者fail，不然会耗尽内存, messageId在发射数据时，根据anchors去生成新的messageId.
实现Bolt有两种方式，一种是实现IRichBolt，一种是继承实现IBasicBolt的BasicBolt，在TopologyBuilder中，可以看到在调用setBolt的方法时，对比两种类型的实现方法：
![1968475-376109500060f827.png](https://upload-images.jianshu.io/upload_images/1968475-376109500060f827.png)
Paste_Image.png
在该方法中，直接将IRichBolt作为参数传入topologyBuilder中初始化。
IBasicBolt:
![1968475-2323c73be78cec80.png](https://upload-images.jianshu.io/upload_images/1968475-2323c73be78cec80.png)
Paste_Image.png
将IBasicBolt包装成BasicBoltExecutor,这个类中的execute方法将会自动ack tuple，这也就是为什么继承BasicBolt会自动ack tuple而无需手动ack：
![1968475-1283599e7ec107e2.png](https://upload-images.jianshu.io/upload_images/1968475-1283599e7ec107e2.png)
Paste_Image.png
