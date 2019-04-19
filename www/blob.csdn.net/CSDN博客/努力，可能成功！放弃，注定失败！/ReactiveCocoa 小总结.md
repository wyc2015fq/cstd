# ReactiveCocoa 小总结 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年11月06日 09:45:14[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：206
本文作者：[伯乐在线](http://ios.jobbole.com/)-[Jerry4me](http://www.jobbole.com/members/Jerry4me)。未经作者许可，禁止转载！
欢迎加入伯乐在线[专栏作者](http://blog.jobbole.com/99322)。
我的Github地址 :[Jerry4me](https://github.com/Jerry4me), 本文章的demo链接 :[JRReactiveCocoa](https://github.com/Jerry4me/JRReactiveCocoa)
RAC与MVVM如今已经不是一个新鲜的玩意了, 对于介绍他们两的精品文章更是大把, 这篇文章主要是用来记录自己学习RAC的过程以及RAC的一些用法, 以防以后要用到的时候却记不起来了.
具体RAC的用法以及本文出现的代码均能在我的[Github](https://github.com/Jerry4me/JRReactiveCocoa)上, 另外附有2个MVVM的小demo. 欢迎大家查看, 赏脸的给个star~
# RAC编程思想
编程学的是思想, 学一样东西最主要是学会它的思想, 那才是它的灵魂, 而不是学习调用方法而已.
RAC又被称为FRP, 函数响应式编程.
何为函数式? 把操作写成一系列嵌套的函数或者方法调用

```
```
|1|[[[[Person getup]eat]run]goHome];|
何为响应式? 不需要考虑调用顺序, 只考虑结果. 一个属性, 一个请求改变马上引发一系列改变.

``
|12|datastream->(filter,combine,map,...)->anotherstreamstream是基于时间上的事件流|
所以RAC即糅合了函数式和响应式编程的优点, 使用RAC编程不需要考虑代码调用顺序, 只需要考虑结果. 把每一个操作都写成一系列的嵌套的方法, 使代码变得高内聚, 低耦合.
# RAC使用场景
数据随着时间而产生, 例如以下三点 :
- UI操作, 连续的动作和动画部分, 例如某些控件跟随滚动
- 网络库, 因为数据是在一定时间后才返回回来, 不是立刻返回的
- 刷新的业务逻辑, 当触发点是多种的时候, 业务往往会变得很复杂, 用delegate, notification, observe混用, 难以统一. 这时用RAC能保证上层的高度一致性, 从而简化逻辑上分层.
# RAC类关系图
RAC类的关系图如下, 下面会抽出一部分类进行讲解, 另外有部分类与用法会在github上的demo上看得到, 还有部分类将不在本文中出现, 本文(demo)只说明了一些常用的类与方法.
![111862021-24909c97608f0849](http://jbcdn2.b0.upaiyun.com/2017/03/976cbc54bc4d0cde6ee49a1cabb3257d.png)
ReactiveCocoa类图.png
# 信号源
![121862021-d93ae8886277bf50](http://jbcdn2.b0.upaiyun.com/2017/03/4a963f7104aef32226beeb08831c0568.png)
RACStream.ng
### RACSignal
RACSignal只会向订阅者发送三种事件 :`next`,`error`和`completed`.
RACSignal的一系列功能是通过类簇来实现的. 如 :


```
```
|12345|RACEmptySignal：空信号，用来实现RACSignal的+empty方法；RACReturnSignal：一元信号，用来实现RACSignal的+return:方法；RACDynamicSignal：动态信号，使用一个block来实现订阅行为，我们在使用RACSignal的+createSignal:方法时创建的就是该类的实例；RACErrorSignal：错误信号，用来实现RACSignal的+error:方法；RACChannelTerminal：通道终端，代表RACChannel的一个终端，用来实现双向绑定。|
核心方法 :`-subscribe:`.
### RACSubject
继承自RACSignal, 是可以手动控制的信号, 相当于RACSignal的可变版本.
能作为信号源被订阅者订阅, 又能作为订阅者订阅其他信号源(实现了RACSubscriber协议).
RACSubject有三个用来实现不同功能的子类 :

``
|123|RACGroupedSignal：分组信号，用来实现RACSignal的分组功能；RACBehaviorSubject：重演最后值的信号，当被订阅时，会向订阅者发送它最后接收到的值；RACReplaySubject：重演信号，保存发送过的值，当被订阅时，会向订阅者重新发送这些值。|
### RACSequence
代表的是一个不可变的值的序列. 不能被订阅者订阅, 但是能与RACSignal之间非常方便地进行转换.
RACSequence由两部分组成 :`head`和`tail`,
 head是序列中的第一个对象, tail则是其余的全部对象.
RACSequence存在的最大意义就是简化OC中的集合操作. 并且RACSequence所包含的值默认是懒计算的, 所以不知不觉中提高了我们应用的性能.
### push-driven与pull-driven
- RACSignal : push-driven, 生产一个吃一个, 类似于工厂的主动生产模式, 生产出产品就push给供销商.
- RACSequence : pull-driven, 吃一个生产一个, 类似于工厂的被动生产模式, 供销商过来pull的时候才现做产品.
对于RACSignal的push-driven模式来说, 没有供销商(`subscriber`)签合同要产品, 当然就不生产了. 只有一个以上准备收货的供销商时, 工厂才开始生产. 这就是RACSignal的休眠(cold)和激活(hot)状态, 也就是`冷信号`和`热信号`.
 一般情况下RACSignal创建以后都处于cold状态, 当有人去`subscribe`才变成hot状态.
## 冷信号与热信号
热信号 : 主动, 即使你没有订阅事件, 仍然会时刻推送. 热信号可以有多个订阅者, 是一对多的关系, 信号可以与订阅者共享信息.
冷信号 : 被动, 只有当你订阅的时候, 它才会发布消息. 冷信号只能一对一, 当有不同的订阅者, 消息是重新完整发送的.
ps : 任何的信号转换即是对原有信号进行订阅从而产生新的信号. (例如 :`Map`,`FlattenMap`等等)
#### 如何区分热信号和冷信号
Subject类似于直播, 错过了就不再处理, 而Signal类似于点播, 每次订阅都从头开始重新发送.
我们能得出 :

```
```
|12|1.RACSubject及其子类是热信号2.RACSignal排除RACSubject类以外的都是冷信号|
#### 将冷信号转化成热信号
RAC帮我们封装了一套可以轻松将冷信号转换成热信号的API :

``
|12345|-(RACMulticastConnection *)publish;-(RACMulticastConnection *)multicast:(RACSubject *)subject;-(RACSignal *)replay;-(RACSignal *)replayLast;-(RACSignal *)replayLazily;// 跟replay的区别是replayLazily会在第一次订阅的时候才订阅sourceSignal|
其中最重要的就是`- (RACMulticastConnection *)multicast:(RACSubject *)subject;`, 其他几个方法都是间接调用它的.
本质 : 使用一个Subject来订阅原始信号, 并让其他订阅者订阅这个Subject, 由于RACSubject本身为热信号, 所以源信号此时就像由冷信号变成了热信号.
# 订阅者
## RACSubscriber
其中`-sendNext:`,`-sendError:`和`-sendCompleted`分别用来从RACSignal接收`next`,`error`和`completed`事件,
 而`-didSubscribeWithDisposable:`则用来接收代表某次订阅的disposable对象.
一个RACDisposable对象就代表这一次订阅, 并且我们可以用它来取消这次订阅.
RACSubscriber就是真正的订阅者, 而RACPassthroughSubscriber可以使得一个订阅者可以订阅多个信号源, 即拥有多个RACDisposable对象, 并能随时取消其中的任何一次订阅. 为了实现这个功能, RAC就引入了RACPassthroughSubscriber类, 它是RACSubscriber类的一个装饰器, 封装了一个真正的订阅者 RACSubscriber 对象, 它负责转发所有事件给这个真正的订阅者, 而当此次订阅被取消时, 它就会停止转发
## RACMulticastConnection
![131862021-8a006ce730feb460](http://jbcdn2.b0.upaiyun.com/2017/03/c14518f6b04972cfa403e405f3213044.png)
RACMulticastConnection.png
使得不管外面有多少个订阅者, 对源信号的订阅只会有一次. 为了防止副作用的产生, 使用的便是multicast机制
### multicast的机制
机制一 : 能防止某信号被多次订阅时调用多次didSubscribe block产生副作用.
机制二 : 实现replay, 即每当有订阅者订阅时, 会将之前缓存中的sendNext重新发送给该订阅者.
### 副作用
- 函数的处理过程中, 修改了外部的变量(例如 : 全局变量, 成员变量等)
- 函数的处理过程中, 出发了一些额外的动作(例如 : 发送了一个全局的Notification, 在console打印了一行信息, 保存了文件, 触发了网络, 更新了屏幕等)
- 函数的处理过程中, 受到外部变量的影响(例如 : 全局变量, 成员变量等, block中捕获到的外部变量也算)
- 函数的处理过程中, 受到线程锁的影响
以上都算副作用. 然而冷信号有可能因为有多个订阅者订阅而产生极大的副作用, 例如发送了同一个网络请求若干次, 同一个计算做了若干次等等, 这些问题都可以通过把这个冷信号转化成热信号得以解决.


```
```
|12345678910111213141516171819202122232425262728293031|RACSignal *signal=[RACSignal createSignal:^RACDisposable *(id subscriber){NSLog(@"创建");/* 发送网络请求 */[subscriber sendNext:@"data"];[subscriber sendCompleted];return[RACDisposable disposableWithBlock:^{NSLog(@"销毁");}];}];[signal subscribeNext:^(idx){// 第一个订阅者NSLog(@"id = %@",x);}];[signal subscribeNext:^(idx){// 第二个订阅者NSLog(@"id2 = %@",x);}];/*    控制台输出为 :     2017-03-13 15:48:09.632 使用cocoapods[41347:10397774] 创建     2017-03-13 15:48:09.634 使用cocoapods[41347:10397774] id = data     2017-03-13 15:48:09.636 使用cocoapods[41347:10397774] 销毁     2017-03-13 15:48:09.637 使用cocoapods[41347:10397774] 创建     2017-03-13 15:48:09.638 使用cocoapods[41347:10397774] id2 = data     2017-03-13 15:48:09.639 使用cocoapods[41347:10397774] 销毁     由此可见有多个订阅者订阅了该信号源的话, 就会多次调用信号源block中的方法, 产生副作用*/|
# 调度器
## RACScheduler
RAC中对GCD的简单封装. 子类如下 :

``
|1234|RACImmediateScheduler：立即执行调度的任务，这是唯一一个支持同步执行的调度器；RACQueueScheduler：一个抽象的队列调度器，在一个GCD串行列队中异步调度所有任务；RACTargetQueueScheduler：继承自RACQueueScheduler，在一个以一个任意的GCD队列为target的串行队列中异步调度所有任务；RACSubscriptionScheduler：一个只用来调度订阅的调度器。|
# 清洁工
## RACDisposable
在订阅者订阅信号源的过程中, 可能会产生副作用或者消耗一定的资源, 所以在取消订阅或完成订阅的时候我们就需要做一些资源回收和辣鸡清理的工作. 核心方法为`-dispose`


```
```
|1234|RACSerialDisposable：作为disposable的容器使用，可以包含一个disposable对象，并且允许将这个disposable对象通过原子操作交换出来；RACKVOTrampoline：代表一次KVO观察，并且可以用来停止观察；RACCompoundDisposable：跟RACSerialDisposable一样，RACCompoundDisposable也是作为disposable的容器使用。不同的是，它可以包含多个disposable对象，并且支持手动添加和移除disposable对象，有点类似于可变数组NSMutableArray。而当一个RACCompoundDisposable对象被disposed时，它会调用其所包含的所有disposable对象的-dispose方法，有点类似于autoreleasepool的作用;RACScopedDisposable：当它被dealloc的时候调用本身的-dispose方法。|
总的来说就是在适当的时机调用disposable对象的`-dispose`方法而已.
# RAC常见宏
用法在demo中

``
|123456789|1.RAC(TARGET,[KEYPATH,[NIL_VALUE]])->总是出现在等号左边,等号右边是一个RACSignal2.RACObserve(TARGET,KEYPATH)->产生一个RACSignal3.@weakify(self)和@strongify(self)4.RACTuplePack和RACTupleUnpack->压包与解包5.@keypath(self.property)->产生一个字符串@"property"|
# RAC中潜在的内存泄漏及解决方法
### RACObserve
如果在block中使用到了RACObserve, 则必须加上`@weakify`和`@strongify`,
 尽管没有显示使用到了`self`. 文档事例如下 :


```
```
|123456|@weakify(self);RACSignal *signal3=[anotherSignal flattenMap:^(NSArrayController *arrayController){// Avoids a retain cycle because of RACObserve implicitly referencing self@strongify(self);returnRACObserve(arrayController,items);}];|
### RACSubject
RACSubject实例进行`map`操作之后, 发送完毕一定要调用`-sendCompleted`, 否则会出现内存泄漏; 而RACSignal实例不管是否进行`map`操作, 不管是否调用`-sendCompleted`, 都不会出现内存泄漏.
原因 : 因为RACSubject是热信号, 为了保证未来有事件发生的时候, 订阅者可以收到信息, 所以需要对持有订阅者!
ps : 几乎所有操作底层都会调用`bind`这样一个方法, 包括但不限于以下方法 :`map`,`filter`,`merge`,`combineLatest`,`flattenMap`…

``
|1234|map:map->flattenMap->bindfilter:filter->flattenMap->bind|
所以 : 对信号操作完成记得发送`-sendCompleted`. (或者`-sendError`).
### 线程安全
Signal events是线性的, 不会出现并发的情况, 除非显示地指定Scheduler. 所以`-subscribeNext:`里的block不需要加锁, 其他的events会依次排队, 直到block处理完成.
为了方便调试, 最好给信号指定Name :`-setNameWithFormat:`
参考文章 :
[ReactiveCocoa 和 MVVM 入门](http://yulingtianxia.com/blog/2015/05/21/ReactiveCocoa-and-MVVM-an-Introduction/)
