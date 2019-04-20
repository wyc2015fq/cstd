# 架构设计之「 CAP 定理 」 - 纯洁的微笑 - CSDN博客
2019年03月22日 09:09:00[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：64
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/jgOJKOvxkeM1xYqU1DZUTkjVNnpbcKianJgsFES8Nianyd2MJyAuecYp5liaCico72ojvWSfT6IdxQl26xT3h4UzQA/640?wx_fmt=jpeg)
在计算机领域，如果是初入行就算了，如果是多年的老码农还不懂 CAP 定理，那就真的说不过去了。CAP可是每一名技术架构师都必须掌握的基础原则啊。
现在只要是稍微大一点的互联网项目都是采用 分布式 结构了，一个系统可能有多个节点组成，每个节点都可能需要维护一份数据。那么如何维护各个节点之间的状态，如何保障各个节点之间数据的同步问题就是大家急需关注的事情了。
CAP定理是分布式系统中最基础的原则。所以理解和掌握了CAP，对系统架构的设计至关重要。
#### 一、什么是 CAP？
「 CAP定理 」又被称为 布鲁尔定理，它提出对于一个分布式系统而言，不能同时满足以下三点：
- 
**Consisteny（一致性）**
- 
**Availability（可用性）**
- 
**Partition tolerance（分区容错性）**
也就是说CAP定理指明了，任何分布式系统只能同时满足这三项中的两项。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/jgOJKOvxkeM1xYqU1DZUTkjVNnpbcKianejyktH6KEicy1icd1ib1ouU2qRKRfSSrJLG0vFEGwfUAF2T7aYe8y2JJQ/640?wx_fmt=png)
如上图，如果是最多同时满足两项，那我们可以有三个组合：CA、CP、AP。在聊这三个组合之前，我们先分别看一下 Consisteny（一致性）、Availability（可用性）、Partition tolerance（分区容错性）的含义。
假设某个系统当前有两个节点A和B，两个节点分别可以由Actor进行读写，两个节点之间的数据会自动完成同步。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/jgOJKOvxkeM1xYqU1DZUTkjVNnpbcKianwWIicOyJSwhZTg5je9lCNRv4yFIH9BI85XcnFQycK57hOgjN1yeG03Q/640?wx_fmt=png)
- 
**Consisteny（一致性）**
一致性的要求是指，对于任何客户端（上图Actor）来说，每次的读操作，都能获得最新的数据。即，当有客户端向A节点写入了新数据之后，其它客户端从B节点中进行读操作所获得的数据必须也是最新的，是与A节点数据保持一致的。
- 
**Availability（可用性）**
可用性的要求是指，每个请求都能在合理的时间内获得符合预期的响应（不保证获取的结果是最新的数据）。
按照上图来看就是，客户端只要向A节点或B节点发起请求后，只要这两个节点收到了请求，就必须响应给客户端，但不需要保证响应的值是否正确。
- 
**Partition tolerance（分区容错性）**
分区容错性是指，当节点之间的网络出现问题之后，系统依然能正常提供服务。
讲完了C、A、P的含义和要求，我们继续来看看它们之间如何组合使用。
#### 二、CAP 怎么应用？
先把视野回到这张图上：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/jgOJKOvxkeM1xYqU1DZUTkjVNnpbcKianejyktH6KEicy1icd1ib1ouU2qRKRfSSrJLG0vFEGwfUAF2T7aYe8y2JJQ/640?wx_fmt=png)
虽然我们知道有 CA、CP、AP 三种组合方式，但是在分布式系统的结构下，网络是不可能做到100%可靠的。既然网络不能保证绝对可靠，那 P（分区容错性）就是一个必选项了。原因如下：
如果选择 CA组合，放弃 P（分区容错性）。还是以最上面的图中A和B节点来举例，当发生节点间网络故障时，为了保证 C（一致性），那么就必须将系统锁住，不允许任何写入操作，否者就会出现节点之间数据不一致了。但是锁住了系统，就意味着当有写请求进来的时候，系统是不可用的，这一点又违背了 A（可用性）原则。
因此分布式系统理论上是不可能有CA组合的，所以我们只能选择 CP 和 AP组合架构。
下面我们来详细看一下  CP架构 和 AP架构的特点：
- 
**CP 架构**
CP架构即 Consisteny（一致性）与 Partition tolerance（分区容错性）的组合。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/jgOJKOvxkeM1xYqU1DZUTkjVNnpbcKianvic2GOY09llGRsyoJShKEKvOC4MuCKIUKCUbvkMO6gmCxiaLNiadQCTag/640?wx_fmt=png)
如上图，由于网络问题，节点A和节点B之前不能互相通讯。当有客户端（上图Actor）向节点A进行写入请求时（准备写入Message 2），节点A会不接收写入操作，导致写入失败，这样就保证了节点A和节点B的数据一致性，即保证了Consisteny（一致性）。
然后，如果有另一个客户端（上图另一个Actor）向B节点进行读请求的时候，B请求返回的是网络故障之前所保存的信息（Message 1），并且这个信息是与节点A一致的，是整个系统最后一次成功写入的信息，是能正常提供服务的，即保证了Partition tolerance（分区容错性）。
上述情况就是保障了CP架构，但放弃了Availability（可用性）的方案。
- 
**AP 架构**
AP架构即 Availability（可用性）与 Partition tolerance（分区容错性）的组合架构。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/jgOJKOvxkeM1xYqU1DZUTkjVNnpbcKianwWIicOyJSwhZTg5je9lCNRv4yFIH9BI85XcnFQycK57hOgjN1yeG03Q/640?wx_fmt=png)
如上图，由于网络问题，节点A和节点B之前不能互相通讯。当有客户端（上图Actor）向节点A进行写入请求时（准备写入Message 2），节点A允许写入，请求操作成功。但此时，由于A和B节点之前无法通讯，所以B节点的数据还是旧的（Message 1）。当有客户端向B节点发起读请求时候，读到的数据是旧数据，与在A节点读到的数据不一致。但由于系统能照常提供服务，所以满足了Availability（可用性）要求。
因此，这种情况下，就是保障了AP架构，但其放弃了 Consisteny（一致性）。
#### 三、CAP 注意事项？
了解了CAP定理后，对于开发者而言，当我们构建服务的时候，就需要根据业务特性作出权衡考虑，哪些点是当前系统可以取舍的，哪些是应该重点保障的。
即使是在同一个系统中，不同模块的数据可能应用的CAP架构都是不同的。举个例子，在某个电商系统中，属于用户模块的数据（账密、钱包余额等）对一致性的要求很高，就可以采用CP架构。而对于一些商品信息方面的数据对一致性要求没那么高，但为了照顾用户体验，所以对可用性要求更高一些，那么这个模块的数据就可以采用AP架构。
另外，虽然上面第二节讲到过我们只能选择CP和AP，无法选择CA。但这句话成立的前提条件是在系统发生了网络故障的情况下。然而，网络故障的概率在系统的整个生命周期中占比是很小的，因此我们在设计的时候，虽然要考虑网络问题下的方案，但也要考虑网络正常情况下的方案，即在网络正常情况下，CA是可以实现的，我们也需要去保证在绝大多数时间下的CA架构。
再者，即使我们按照CAP定理，三个中只能取其二，但不代表我们只需要保障其中的两点，而完全的放弃第三点，我们应该为不能保障的第三点也做一些防备措施或者冗余方案，来使系统更加的完善健全。
以上，就是对CAP定理的一些思考。
-END-
**纯洁的微笑**
**一个有故事的程序员**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/PgqYrEEtEnqjV7GOKB2htgfZjgMjqxftxfmmdrLiaMKpyicTmLLX5fUjb6YxA6Z5Bhcozb3p0uMV7wqdKED89HZA/640?wx_fmt=jpeg)
