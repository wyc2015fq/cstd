# scala 简要： Actor - 我相信...... - CSDN博客





2015年01月03日 07:25:29[半吊子全栈工匠](https://me.csdn.net/wireless_com)阅读数：1450








                 actor提供了并发程序中与传统的基于锁的结构不同的另一种选择，通过尽可能避免锁和共享状态，actor更容易地设计出正确、没有死锁或争用状况的程序。Scala提供了actor的简单实现，akka（http：//akka.io）提供了高级actor类库。

每个actor都要扩展Actor类并重写Act方法，actor是处理异步消息的对象，消息可以是任何对象，通过！操作符发送消息，例如：
actorX ！“happy new year”
一个好的方式是使用样例类作为消息，这样，actor可以使用模式匹配了。发送的消息存放在mailbox，receive方法从mailbox中取下一条消息并处理，如果在receive方法被调用时并没消息，则该调用会阻塞，直到有消息抵达。actor可以安全地修改它自己的数据。

向其他actor发送消息的方法：
1）使用全局的actor
2）actor可以构造成带有指向一个或更多actor的引用
3）actor可接收带有指向另一个actor的引用的消息
4）actor可以返回消息给发送方
actor可以发送一个消息并等待回复，用！？操作符即可，尽量避免同步消息。

actor的act方法在actor的start方法被调用时开始执行。接下来进入某个循环，终止条件如下：
1）act方法返回
2）act方法由于异常被终止
3）actor调用exit方法

通过link方法可以将不同的actor链接在一起。

actor的设计原则如下：
1）避免使用共享状态
2）不要调用actor的方法
3）保持每个actor简单
4）上下文数据包含在消息中
5）最小化给发送方回复
6）最少阻塞调用
7）使用react
8）建立失败区            


