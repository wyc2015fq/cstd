
# DPOS共识算法 - Wesley's Blog - CSDN博客


2018年05月26日 13:06:34[WesleyWang97](https://me.csdn.net/yinanmo5569)阅读数：642


# DPOS算法摘要
DPOS全称是委托股权证明 (Delegated proof os Stake). DPOS算法分为两个部分:**区块生产者选举，生产调度**．
区块生产者选举(election a group of block producers)过程确保利益相关者最终得到控制，因为当网络不顺利运作时，利益相关者损失最大。人们如何当选对于如何逐分实现共识几乎没有影响．因此，本文将重点讨论如何在选定区块生产商后达成共识．
为了帮助解释这个算法，我想假设3个块生产者A，B和C.因为共识需要2/3 + 1来解决所有情况，所以这个简化模型将假设生产者C被认为是攻击者。 在现实世界中，将有21个或更多的块生产者。就像工作证明一样，一般规则是最长的连锁胜出。任何时候，一个诚实的同行看到一个有效的严格的更长的链条，它会从当前的分叉切换到更长的分叉。
我将通过示例展示DPOS如何在最可想象的网络条件下运行。这些例子应该可以帮助你理解为什么DPOS很健壮并且很难打破。
# 正常情况
在正常操作下，块生产者轮流每3秒产生一个块。假设没有人错过他们，那么这将产生最长的链条。 块生产者在任何其他时间段生成块都是无效的。
![这里写图片描述](https://img-blog.csdn.net/20180526130412503?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 少数派未连接分支
高达1/3的节点可能是恶意或故障，并创建一个少数叉。在这种情况下，少数叉将只产生一个块每9秒，而大多数叉将产生2块每9秒。再次，诚实的2/3多数将比少数人长。
![这里写图片描述](https://img-blog.csdn.net/20180526130422898?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 少数派未连接多分支
少数人可以尝试生产无限数量的分支，但是他们的所有分支都将比主要连锁店短，因为少数人的生产环节比大多数人慢。
![这里写图片描述](https://img-blog.csdn.net/20180526130434180?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 网络破碎
在某种情况下，网络完全有可能被分割很多个碎片，而没有大部分区块生产者。 在这种情况下，最长的链将会成为最大的少数。 当网络连接恢复后，较小的少数民族将自然地切换到最长的连锁店，并且明确的共识将被恢复。
![这里写图片描述](https://img-blog.csdn.net/20180526130446186?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
有可能有3个分支其中2个最长的分支是相同的长度。在这种情况下，在第三条最短链上的生产者会放弃原有链，重新加入两条最长链之一的网络。生产商的数量是奇数的，所以不可能长期保持相同。之后，我们将覆盖生产者洗牌，这将随机化生产顺序，以确保即使两个分支具有相同数量的生产者，分支将以不同速度增长．
# 连接少数派双生产
在这种情况下，少数B在他们的时隙产生了两个或更多个备选块。下一个计划生产者（C），可以选择从B产生的任何一个区块中建立。当这发生时，它将成为最长链，并且选择B1的所有节点将切换叉。不多的少数不良生产商试图传播的替代品并没关系，它们将永远不会成为超过一轮的最长链的一部分。
![这里写图片描述](https://img-blog.csdn.net/20180526130503785?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 最后不可逆转的块
在网络碎片的情况下，多个分支可以在长时间内继续增长。从长远来看，最长的链将获胜，但观察者需要一种手段来确定何时块绝对是增长最快的链的一部分。这可以通过看到2/3 + 1的块生产商的确认来确定。
在下面的图中，B块已经被C和A所证实，A代表2/3＋1确认，因此我们可以推断，如果我们2/3的生产者诚实的话，没有其他的链可能更长
![这里写图片描述](https://img-blog.csdn.net/20180526130530832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[ ](https://img-blog.csdn.net/20180526130530832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
请注意，这个“规则”类似于Bitcoin的6个区块确认“规则”。一些聪明的个体可以设计一系列事件，其中两个节点可以在不同的最后不可逆块上结束。这种边缘情况需要攻击者对通信延迟进行完全控制，并利用该控制而不是一次，而是分开两分钟。如果发生这种情况，那么最长链的长期法则仍然适用。我们估计这种袭击的几率接近0，经济后果如此微不足道，因此不值得担心。
[

](https://img-blog.csdn.net/20180526130530832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 缺少具体数量的生产者
[
](https://img-blog.csdn.net/20180526130530832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)如果不存在明确的生产者法定人数，则少数人有可能继续生产区块。在这些块中，利益相关者可以包括改变他们投票的交易。然后，这些投票可以选择一组新的生产者，并恢复块生产参与到100%。一旦这种情况发生，少数派链最终将超过所有其他链而不需要100%的参与。
[
](https://img-blog.csdn.net/20180526130530832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)在这个过程中，所有观察者都会知道网络状态在不断变化，直到一个链出现67%的参与。那些选择在这些条件下交易的人承担的风险与那些选择接受少于6个确认的人相似。他们这样做的知识，有一个小概率，共识可能最终定居在不同的分支。在实践中，这种情况比接受少于3比特币确认的块要安全得多。
[

](https://img-blog.csdn.net/20180526130530832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 大多数生产者出错
[
](https://img-blog.csdn.net/20180526130530832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)如果大多数生产者变得腐败，那么他们可以生产无限数量的分支，每个分支似乎都会以2/3的多数确认推进。在这种情况下，最后的不可逆块算法会恢复到最长链算法。 最长的链将是最大多数人批准的链，这将由少数剩余的诚实节点决定。 这种行为不会持续很长时间，因为利益相关者最终会投票替换这些生产者。
![这里写图片描述](https://img-blog.csdn.net/20180526130544669?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
# 交易作为证明凭证（TaPoS）
当用户签署交易时，他们会根据对区块链状态的特定假设进行交易。 这个假设是基于他们对近期区块的看法。如果最长链上的共识发生变化，那么它可能会使签署者同意交易时的假设无效。
对于TaPoS，所有事务都包含最近一个块的散列，并且如果该块不存在于链历史中，则被视为无效。任何在孤立分叉处签署交易的人都会发现交易无效并且无法迁移到主分叉。
这一过程的副作用是针对试图产生替代链的远程攻击的安全性。 每个利益相关方在每次交易时都会直接确认区块链。 随着时间的流逝，所有的障碍都得到了所有利益相关者的确认，而这是无法在伪造链中复制的。
# 确定生产者洗牌
在所有的例子中，我们展示了块生产商的循环调度。实际上，块生产商在每一个n个块中洗牌，其中n是生产者的数量。这种随机化确保块生产商B不总是忽略块生产者A，并且在任何时候有多个分支相同生产者数（联盟）的关系最终被打破。
# 总结
在每一个可想象的自然网络出错下，DPOS是可靠的，即使面对少数生产商的腐败，也能保证安全。与一些竞争算法不同，当大多数生产者失败时，DPOS可以继续发挥作用。在这个过程中，社区可以投票来取代失败的生产商，直到它能恢复100%的参与。我知道没有其他一致性算法在这样高的和不同的故障条件下是鲁棒的。
最终，DPOS从选择块生产商的算法中获得显著的安全性，并验证节点是高质量和独特的个体。使用批准投票的过程确保即使有50%的活跃投票权的人也不能单独选择一个单独的制作人。DPOS的设计，以优化性能的名义条件100%参与诚实节点与强大的网络连接。这使得DPOS有能力在99.9%秒的平均时间内确认交易，同时以优雅、可检测的方式降级，这是微不足道的。
对于网络条件较差的不诚实节点的标称条件的其他一致性算法设计。替代设计的最终结果是性能较慢、延迟更高、通信开销高的网络，并且在节点失败的事件33%中完全停止。
经过3年的BIT和StEm年的成功运营，我们经历了各种各样的网络条件和软件错误。DPOS成功地导航了这个环境，并展示了它在处理更多事务时保持一致性的能力。
英文原文：[https://steemit.com/dpos/@dantheman/dpos-consensus-algorithm-this-missing-white-paper](https://steemit.com/dpos/@dantheman/dpos-consensus-algorithm-this-missing-white-paper)

[
](https://img-blog.csdn.net/20180526130530832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3lpbmFubW81NTY5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
