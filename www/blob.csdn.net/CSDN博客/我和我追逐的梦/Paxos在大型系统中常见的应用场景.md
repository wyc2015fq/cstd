
# Paxos在大型系统中常见的应用场景 - 我和我追逐的梦~~~ - CSDN博客


2014年05月13日 19:28:27[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3537


来源: http://timyang.net/distributed/paxos-scenarios/

在分布式算法领域，有个非常重要的算法叫Paxos, 它的重要性有多高呢，Google的Chubby [1]中提到
> all working protocols for asynchronous consensus we have so far encountered have Paxos at their core.
关于Paxos算法的详述在维基百科中有更多介绍，中文版介绍的是choose value的规则[2]，英文版介绍的是Paxos 3 phase commit的流程[3]，中文版不是从英文版翻译而是独立写的，所以非常具有互补性。Paxos算法是由Leslie Lamport提出的，他在Paxos Made Simple[4]中写道
> The Paxos algorithm, when presented in plain English, is very simple.
当你研究了很长一段时间Paxos算法还是有点迷糊的时候，看到上面这句话可能会有点沮丧。但是公认的它的算法还是比较繁琐的，尤其是要用程序员严谨的思维将所有细节理清的时候，你的脑袋里更是会充满了问号。Leslie Lamport也是用了长达9年的时间来完善这个算法的理论。
实际上对于一般的开发人员，我们并不需要了解Paxos所有细节及如何实现，只需要知道Paxos是一个分布式选举算法就够了。本文主要介绍一下Paxos常用的应用场合，或许有一天当你的系统增大到一定规模，你知道有这样一个技术，可以帮助你正确及优雅的解决技术架构上一些难题。
1. database replication, log replication等， 如bdb的数据复制就是使用paxos兼容的算法。Paxos最大的用途就是保持多个节点数据的一致性。
2. naming service, 如大型系统内部通常存在多个接口服务相互调用。
1) 通常的实现是将服务的ip/hostname写死在配置中，当service发生故障时候，通过手工更改配置文件或者修改DNS指向的方法来解决。缺点是可维护性差，内部的单元越多，故障率越大。
2) LVS双机冗余的方式，缺点是所有单元需要双倍的资源投入。
通过Paxos算法来管理所有的naming服务，则可保证high available分配可用的service给client。象ZooKeeper还提供watch功能，即watch的对象发生了改变会自动发notification, 这样所有的client就可以使用一致的，高可用的接口。
3.config配置管理
1) 通常手工修改配置文件的方法，这样容易出错，也需要人工干预才能生效，所以节点的状态无法同时达到一致。
2) 大规模的应用都会实现自己的配置服务，比如用http web服务来实现配置中心化。它的缺点是更新后所有client无法立即得知，各节点加载的顺序无法保证，造成系统中的配置不是同一状态。
4.membership用户角色/access control list, 比如在权限设置中，用户一旦设置某项权限比如由管理员变成普通身份，这时应在所有的服务器上所有远程CDN立即生效，否则就会导致不能接受的后果。
5. 号码分配。通常简单的解决方法是用数据库自增ID, 这导致数据库切分困难，或程序生成GUID, 这通常导致ID过长。更优雅的做法是利用paxos算法在多台replicas之间选择一个作为master, 通过master来分配号码。当master发生故障时，再用paxos选择另外一个master。
这里列举了一些常见的Paxos应用场合，对于类似上述的场合，如果用其它解决方案，一方面不能提供自动的高可用性方案，同时也远远没有Paxos实现简单及优雅。
Yahoo!开源的ZooKeeper [5]是一个开源的类Paxos实现。它的编程接口看起来很像一个可提供强一致性保证的分布式小文件系统。对上面所有的场合都可以适用。但可惜的是，ZooKeeper并不是遵循Paxos协议，而是基于自身设计并优化的一个2 phase commit的协议，因此它的理论[6]并未经过完全证明。但由于ZooKeeper在Yahoo!内部已经成功应用在HBase, Yahoo! Message Broker, Fetch Service of Yahoo! crawler等系统上，因此完全可以放心采用。
另外选择Paxos made live [7]中一段实现体会作为结尾。
> *  There are significant gaps between the description of the Paxos algorithm and the needs of a real-world system. In order to build a real-world system, an expert needs to use numerous ideas scattered in the literature and make several relatively small
 protocol extensions. The cumulative effort will be substantial and the final system will be based on an unproven protocol.

> * 由于chubby填补了Paxos论文中未提及的一些细节，所以最终的实现系统不是一个理论上完全经过验证的系统

> * The fault-tolerance computing community has not developed the tools to make it easy to implement their algorithms.

> * 分布式容错算法领域缺乏帮助算法实现的的配套工具, 比如编译领域尽管复杂，但是yacc, ANTLR等工具已经将这个领域的难度降到最低。

> * The fault-tolerance computing community has not paid enough attention to testing, a key ingredient for building fault-tolerant systems.

> * 分布式容错算法领域缺乏测试手段
这里要补充一个背景，就是要证明分布式容错算法的正确性通常比实现算法还困难，Google没法证明Chubby是可靠的，Yahoo!也不敢保证它的ZooKeeper理论正确性。大部分系统都是靠在实践中运行很长一段时间才能谨慎的表示，目前系统已经基本没有发现大的问题了。
**Resources**
[1][The Chubby lock service for loosely-coupled distributed systems](http://labs.google.com/papers/chubby-osdi06.pdf)(PDF)
[2][
http://zh.wikipedia.org/wiki/Paxos算法](http://zh.wikipedia.org/wiki/Paxos%E7%AE%97%E6%B3%95)
[3][http://en.wikipedia.org/wiki/Paxos_algorithm](http://en.wikipedia.org/wiki/Paxos_algorithm)
[4][
Paxos Made Simple](http://research.microsoft.com/en-us/um/people/lamport/pubs/paxos-simple.pdf)(PDF)
[5][ZooKeeper](http://hadoop.apache.org/zookeeper/)
[6][
The life and times of a zookeeper](http://portal.acm.org/citation.cfm?id=1583991.1584007)
[7][Paxos Made Live – An Engineering Perspective](http://labs.google.com/papers/paxos_made_live.pdf)(PDF)

#### 6 Comments
#### [»](http://timyang.net/distributed/paxos-scenarios/#postcomment)
![](http://www.gravatar.com/avatar/284fd13ba99a256cea6204078cf5a5a8?s=32&d=%3Cpath_to_url%3E&r=G)fishyuzesays:
[Jan 23rd 2010 at 19:56](http://timyang.net/distributed/paxos-scenarios/comment-page-1/#comment-2687)
“Leslie Lamport也是用了长达9年的时间来完善这个算法的理论”–
这个故事其实是这样的:
Lamport大牛在他Paxos的第一个版本早在1990年就提交给ACM TOCS Jnl.的评审委员会了 但是当时没有人理解他的算法 主编回执他的稿子建议他用数学而不是神话描述他的算法 他们才会考虑接受这篇paper
Lamport大牛很生气 他有次就在一个会议上说:”为什么搞理论的这群人一点幽默感也没有呢?” 他拒绝修改 而且withdraw了这篇文章
1996年微软的Butler Lampson在WDAG96上提出了重新审视这篇文章 因为他读懂了
1997年MIT的Nancy Lynch在WDAG97上 根据原文重新改写了这篇文章 叫做 在本文中她们”代替”Lamport用数学形式化的定义并证明了Paxos
于是在1998年的ACM TOCS上 这篇迟到了9年的paper终于被接受了
后来2001年 Lamport大牛也作出了让步 他用简单的语言而不是神话故事 重述了原文 这就是 但是通篇还是没有数学符号 L大牛甚为固执 据他自己说 他检查过了自己的语言并没有歧义 不需要数学来描述
Paxos可以说是Theory of CS领域被流传最广泛的一则趣事~

![](http://www.gravatar.com/avatar/284fd13ba99a256cea6204078cf5a5a8?s=32&d=%3Cpath_to_url%3E&r=G)fishyuzesays:
[Jan 23rd 2010 at 19:59](http://timyang.net/distributed/paxos-scenarios/comment-page-1/#comment-2688)
(上条回复”中的内容看不见)
“Leslie Lamport也是用了长达9年的时间来完善这个算法的理论”–
这个故事其实是这样的:
Lamport大牛在他Paxos的第一个版本”The Part-Time Parliament”早在1990年就提交给ACM TOCS Jnl.的评审委员会了 但是当时没有人理解他的算法 主编回执他的稿子建议他用数学而不是神话描述他的算法 他们才会考虑接受这篇paper
Lamport大牛很生气 他有次就在一个会议上说:”为什么搞理论的这群人一点幽默感也没有呢?” 他拒绝修改 而且withdraw了这篇文章
1996年微软的Butler Lampson在WDAG96上提出了重新审视这篇文章 因为他读懂了
1997年MIT的Nancy Lynch在WDAG97上 根据原文重新改写了这篇文章 叫做”Revisiting the Paxos Algorithm” 在本文中她们”代替”Lamport用数学形式化的定义并证明了Paxos
于是在1998年的ACM TOCS上 这篇迟到了9年的paper终于被接受了
后来2001年 Lamport大牛也作出了让步 他用简单的语言而不是神话故事 重述了原文 这就是”Paxos made simple” 但是通篇还是没有数学符号 L大牛甚为固执 据他自己说 他检查过了自己的语言并没有歧义 不需要数学来描述
Paxos可以说是Theory of CS领域被流传最广泛的一则趣事~

![](http://www.gravatar.com/avatar/88d137b73ef16659885db71401b1a192?s=32&d=%3Cpath_to_url%3E&r=G)heleisays:
[Nov 9th 2011 at 15:58](http://timyang.net/distributed/paxos-scenarios/comment-page-1/#comment-77224)
我并不觉得这些领域是paxos本身所适合的，paxos只是一个协议，用来处理一致性的问题。上述各个领域实际上是需要一套能提供一致性保证的服务，至于这些服务底下用什么协议，用什么语言，who cares。仔细阅读chubby论文就会发现，在这些领域中直接应用paxos(意为使用libpaxos这样的东东来保证一致性)甚至可能是不太合适的，提供一个zookeeper这样的东东可能更加靠谱



