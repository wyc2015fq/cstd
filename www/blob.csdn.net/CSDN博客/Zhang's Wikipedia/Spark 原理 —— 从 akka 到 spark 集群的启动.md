
# Spark 原理 —— 从 akka 到 spark 集群的启动 - Zhang's Wikipedia - CSDN博客


2018年11月11日 19:14:09[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：398



## 1. actor 到 akka
Actor 是一种消息并发模型，基于事件模型的并发机制。
Scala 的 Actor 类似于 Java 中的多线程编程，不同在于：Scala 的 Actor 设计的初衷在于尽可能地避免锁和共享状态，从而避免多线程并发时出现资源征用的情况；
原因在于，Java 中多数使用的是可变状态的对象资源，对这些资源进行共享实现多线程编程的话，需要控制好资源竞争与防止对象状态被意外修改，而对象状态的不变形往往很难保证；
scala中，我们可以通过复制不可变状态的资源（即对象，scala中一切皆对象，函数方法也是）的一个副本，再基于 Actor 的消息发送和接收机制进行并行编程；
AkkaAkka 使用 Scala 语言开发，基于 actor 并发模型实现；
Spark 的 RPC 便是通过 Akka 类库实现的；Actor是Akka中最核心的概念，它是一个封装了状态和行为的对象，Actor 之间可以通过交换消息的方式进行通信；
每个 Actor 都有自己的收件箱（mailbox），通过 Actor 能够简化锁及线程管理；
ActorSystem使用ActorSystem这一重量级结构，能够创建很多 Actor
它回去分配多个线程，在实际应用中，ActorSystem 通常是一个单例对象；

## 2. actor 用法
Actor 方法的执行顺序：创建 Actor通过 ActorSystem
调用 Actor 的 start() 方法启动 Actor（类似于 Java 线程的 start() 方法）便会去执行 act() 方法（对于 Java 线程即是执行线程中定义的 run 方法）
向 Actor 发送消息；
Akka 中的 Actor，Actor 负责通信，preStart()：该方法在 Actor 对象构造方法执行后执行，整个 Actor 生命周期仅执行一次；
receive()：该方法在 Actor 的 preStart 方法执行完成后执行，用于接收消息，会被反复执行；
Spark 集群模式下的启动调用顺序：首先需要 Master 及 Worker 两个角色（模块）Master 中的 ActorSystem 创建名为 “master” 的 actor
Worker 中的 ActorSystem 创建名为 “worker” 的 actor
然后两个 actor 进行通信；
Master 及 Worker 中的 Actor 通信流程如下：worker actor 将请求的参数封装进一个 case class 中（序列化后）发送给 master actor；
master actor 收到请求，对其进行反序列化，经过模式匹配后，获取到函数调用及其参数；
master actor 将计算后的结果返回给 worker actor；


