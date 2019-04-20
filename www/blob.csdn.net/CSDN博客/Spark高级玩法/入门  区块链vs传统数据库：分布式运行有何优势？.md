# 入门 | 区块链vs传统数据库：分布式运行有何优势？ - Spark高级玩法 - CSDN博客
2018年02月28日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：464
选自TowardsDataScience
**作者：Shaan Ray**
**机器之心编译**
**参与：路雪、李泽南**
> 
区块链（blockchain）这一概念正因比特币等虚拟货币的兴盛而变得火热起来，实际上，这种技术因为特殊的设计思路也可以应用于很多其他领域中。作为一种容错率很高的分布式数据存储模式，区块链与传统数据库有哪些不同之处？想要回答这个问题，我们需要看看它们的运行机制。
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibhWklL7plpXzAacM1kx5Q6Vv2c7gXEHNXGS8fqsXlI6mLZtHjRJPqrnps12LUnYpCZ3XanO3icCuA/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
*区块链的分布式节点。*
**传统数据库**
传统数据库使用客户端-服务器网络架构。在这种结构中，用户（或称为客户端）可以修改存储在中央服务器中的数据。数据库的控制权保留在获得指定授权的机构处，他们会在用户试图接入数据库前对其身份进行验证。由于授权机构对于数据库的管理负责，如果授权机构的安全性受到损害，则数据面临被修改、甚至被删除的风险。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibhWklL7plpXzAacM1kx5Q6s8Uk7DLkq6NqdjrmSl86SH0FjBCHJ1qUj3QtAJV7gVS5KDGZk4GJ8w/640?wx_fmt=png)
*传统数据库*
**区块链数据库**
区块链数据库由数个分散的节点组成。每一个节点都会参与数据管理：所有节点都会验证新加入区块链的内容，并将新数据写入数据库。对于加入区块链的新内容，大多数节点必须达成一致才能成功写入。这种共识机制保证了网络安全，让篡改内容变得非常困难。
区块链最引人关注的实例就是比特币（Bitcoin）了，在比特币中，共识通过「挖矿」达成（使用计算机计算复杂 hashing 问题的解）；而在另一个著名的案例以太坊中，共识机制是由股权证明（POS）来完成的。想要了解这两种机制的详细内容，可以参阅本文作者此前的文章：https://hackernoon.com/what-is-proof-of-stake-8e0433018256。
**完整性与透明度**
区块链技术区别于传统数据库技术的一大特点就是其具备公开可验证性，这是通过完整性与透明度来实现的。
- 
完整性：每名用户都可以得到这样的保证——他们所检索的数据自被记录的那一刻起不会遭到损坏或改写。
- 
透明度：每名用户都可以获知并验证区块链内容是如何随着时间推移而变化的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibhWklL7plpXzAacM1kx5Q6Bm89hgzD4U66nA7BB2YM2ibYa4YbTxg6VPtj58VZhB8Z9YhDTfX2Wpw/640?wx_fmt=png)
*Dashcoin 主节点的全球分布图。*
**CRUD vs 读取 & 写入操作**
传统数据库中，客户可以对数据执行四种操作：创建、读取、更新和删除（通称为 CRUD 命令）。
而区块链只能增加。用户只能以附加块的形式添加数据，所有先前的数据被永久存储，无法更改。因此，区块链仅能执行以下操作：
- 
读取：用区块链查询和获取数据；
- 
写入：向区块链添加更多数据。
**验证和写入**
区块链具备这两个功能：交易验证和新交易写入。交易是一种改变区块链上数据状态的操作。区块链上之前的 entries 永远保持不变，而新的 entry 可以改变之前 entries 中数据的状态。例如，如果区块链记录我的比特币钱包中有一百万比特币，该数字永久存储在区块链中。当我花费 20 万比特币时，该交易也被记录在区块链上，我的余额为 80 万比特币。但是，由于区块链只能不断加长，因此这次交易之前的余额 100 万比特币仍然永久保存在区块链上。这就是为什么区块链通常指不可更改的分布式账本。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWibhWklL7plpXzAacM1kx5Q6icd4U0Ouja1hKiaEnUxNEgVM9BJqX0CWnLEgKT63SdicB3c6XXtQJfBBg/640?wx_fmt=png)
*中心化 vs. 点对点*
**总之，区别在于去中心化控制**
去中心化控制消除了中心化控制的风险。任何能够充分访问中心化数据库的人都可以摧毁或破坏其中的数据，因此用户依赖于数据库管理员的安全基础架构。
区块链技术使用去中心化数据存储来避开这一问题，从而在自己的结构中建立安全性。
区块链技术很适合记录某些种类的信息，传统数据库更适合记录另外一些种类的信息。对于每个组织而言，理解它想从数据库中获得什么非常关键，我们需要在选择数据库之前，判断每种数据库的优缺点。![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gWicKczlUNeEAmUicw7MXcFnte6PhPpD5AHeEVvqImFdQz3ziaXsDmtjAp2icXupgXc6j3OpGl8dvHEicvQ/640?wx_fmt=png)
*原文链接：https://towardsdatascience.com/blockchains-versus-traditional-databases-e496d8584dc*
![640.png?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png?)
**关于Spark学习技巧**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg?)
**更多文章，敬请期待**
