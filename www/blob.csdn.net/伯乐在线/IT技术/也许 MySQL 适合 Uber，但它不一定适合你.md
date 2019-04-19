# 也许 MySQL 适合 Uber，但它不一定适合你 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [LW](http://www.jobbole.com/members/LW) 翻译，[黄利民](http://www.jobbole.com/members/huanglimin) 校稿。未经许可，禁止转载！
英文出处：[Markus Winand](https://dzone.com/articles/on-ubers-choice-of-databases)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
【伯乐在线导读】：Markus Winand 是数据库专家，著有《SQL Performance Explained》一书。2013 年 Uber 从 MySQL 迁移到 PostgreSQL，而在 2016 年 8 月，Uber 工程博客发文称他们从 PostgreSQL 迁回 MySQL。当时在业内引发热议，Markus 写下了这篇文章来回应。
![](http://wx3.sinaimg.cn/large/63918611gy1ff75v6yo2dj20bz072q38.jpg)
你可能听说了 Uber 从 PostgreSQL 迁移到 MySQL 。这对于他们来说，或许是一个好的迁移，但是这未必适合你。且听我说来。
2016 年 8 月，Uber 发表了一篇名为《[为什么 Uber 工程从 PostgreSQL 迁移到了 MySQL](https://eng.uber.com/mysql-migration/)》的文章。我并没有立即阅读原文，因为我的内心深处告诉我应该做一些本地的改进。在如此做的过程中，我的邮箱塞满了问题，比如“难道 PostgreSQL 真的有这么差劲吗？”我知道  PostgreSQL 并没有如此差劲，因此这些邮件使我想知道，原文到底写了些什么鬼玩意。这是一篇企图理解 Uber 的文章。
在我看来，Uber 的文章基本上在说他们发现 MySQL 比 PostgreSQL 更适合他们的环境。然而，原文在传递这一消息时却表现的非常差劲。举例来说，原文并没有写“  PostgreSQL 在 update-heavy 的使用场景下有一些局限”，而是写到“它对于写入（write）来说是一个非常低效的架构”。当你没有 updata-heavy 的使用场景时，不必担心 Uber 的文章中描述的问题。
在本文中，我将会解释：①为什么我认为 Uber 的文章并不能作为我们在选择数据库的一个普适性建议，②为什么 MySQL 仍旧适用于 Uber，③为什么这样的成功迁移案例可能会引起更多的问题，而不是扩展了数据库。
### 在更新方面的问题
Uber 的文章所出现的第一个问题是， 在更新表中的行时，PostgreSQL 通常需要更新表上的所有索引，原文对这方面做了大篇幅的描述，但细节又不够充分。另一方面，对于具有 InnoDB 的 MySQL 来说，只需要更新那些包含有更新列的索引。在更新改变了非索引列之时（原文中的「Write Amplification」部分），PostgreSQL 的方法需要占用更多的磁盘 IO。如果这对 Uber 来说是一个大问题，那这些更新对于他们来说，或许是其整个工作量中的很大一部分。
然而，我有一些猜测，是 Uber 文章并没有提到的。原文并没有提及 PostgreSQL 的 Heap-Only-Tuples( HOT )。从 [PostgreSQL](https://github.com/postgres/postgres/blob/master/src/backend/access/heap/README.HOT) 源码来看，HOT 对于一些特殊场景非常有用，即“当一个元组进行了重复更新时，不需要对它的索引列进行更新”。在这个场景中，如果新的行版本能够和之前的版本存储在同一个页面中，PostgreSQL 能够做出一些不需要触及任何索引的更新。后一种情况可以使用  [fillfactor](https://www.postgresql.org/docs/9.2/static/sql-createtable.html#SQL-CREATETABLE-STORAGE-PARAMETERS) 来进行调谐。假设 Uber 的工程认为 HOT 不能够解决他们的问题，是因为这些频繁的更新影响了不止一个索引列。
这个假设也在文章中的下列语句得到了支持：“如果我们有一张定义了十几个索引的表，那么只覆盖了一个指标的领域的更新就必须涉及到整整12个指标来反映新的行中的 ctid ”。它明确地指出“只覆盖了一个指标，”只有一个索引这是一种极端情况，否则 PostgreSQL 的 HOT 是可以解决这一问题的。
【旁注：我比较关心他们所拥有的索引的数量能否减少— *[index redesign](http://winand.at/index-design&utm_medium=UTILt&utm_compaign=201607-uber)*是我的一个挑战。无论如何，对于那些使用的很少但在使用时又非常重要的索引来说，这是完全有可能的。】
看起来他们似乎运行着许多更新，这些更新改变了不止一个索引列，但这和表中所有索引相比来说，这算不上啥。如果这是一个主要的用例，那么原文建议用 MySQL 来取代 PostgreSQL 就能够说得通了。
### 关于 Select 的问题
还有一个关于他们用例的声明引起了我的注意：原文解释了 MySQL/InnoDB 使用了 clustered indexed ，并且承认了“这种设计意味着在做二次关键值查找时和 Postgres 相比 InnoDB 的优势并不明显”。关于这个问题（the clustered index penalty），我之前写过在 SQL Server 环境下的文章。
他们写到 clustered index penalty 的优势很小，这引发了我的兴趣。在我看来，当你运行了很多使用二次索引的查找时，这个优势是很大的。如果对于他们来说这总种优势很小，那这可能说明这些索引使用得很少。这将意味着他们在大多数情况下使用了 primary key 来进行查找（那么就不需要付出 clustered index penalty 了）。注意我写的是“查找”而不是“选择”。这其中的原因在于 clustered index penalty 会影响具有 where 字句的声明，而不仅仅是选择。这也意味着高频率的更新大多基于primary key。
最后，还有关于查询的问题：他们没有提到PostgreSQL在  [index-only scans](https://wiki.postgresql.org/wiki/What&apos;s_new_in_PostgreSQL_9.2#Index-only_scans) 方面的限制。尤其是在更新比较多的数据库，安装 PostgreSQ L的 index-only scans  没有多大用处。我甚至说过这是唯一影响我大多数客户端的问题。我在 2011 年已经在博客上写过这个问题。2012 年， PostgreSQL 9.2 在 index-only scans 获得了一些有限的支持（这些工作大多服务于静态数据）。2014 年我甚至在 PgCon 提出了我关注的一个方面。然而， Uber 并没有抱怨这个问题。选择速度对于他们而言并不是问题。我猜测查询速度有运行在  replicas 上的选择进行解决（如下所示），并且可能被大多数 primary key 所限制。
如今，他们的使用案例看起来似乎更加适合一个键/值存储。并且 Innodb 是一个很坚实和流行的键/值存储。这里甚至有一些包将 InnoDB 和一些（非常有限的）的前端 SQL 绑定：我认为 MySQL 和 MariaDB 是非常流行的两个。但是认真地说，如果你需要一个键/值存储并且恰巧想运行一个简单的 SQL 查询， MySQL （或者 MariaDB ）是合理的选择。我猜测这至少比任何随机的 NoSQL 键/值存储要好，它们往往只提供一些更加有限的 SQL-ish 查询语句作为开始。Uber ，从另一个方面来说仅仅在 InnoDB 和 MySQL 上层建立了他们自己的东西（“[Schemaless](https://eng.uber.com/schemaless-part-one/)”）。
### 关于索引再平衡（Rebalancing）
关于原文谈索引，我最后再说一个方面：它在 B-tree 索引语句中使用了“再平衡”这个词。它甚至链接到了维基百科上词条 “[Rebalancing after deletion](https://en.wikipedia.org/wiki/B-tree#Rebalancing_after_deletion)”。不幸的是，维基百科词条解释并不普遍适用于数据库索引，因为维基百科上所描述的算法维持了每个节点至少是 half-full 的要求。为了提高并发性， PostgreSQL 使用了[Lehman, Yao variation of B-trees](http://www.csd.uoc.gr/~hy460/pdf/p650-lehman.pdf)，其升高了要求并且因此允许稀疏索引。作为一个旁节点，PostgreSQL 仍旧移除索引中的空白页（参见[“ Indexing Internals” 幻灯片第 15 页](https://www.pgcon.org/2016/schedule/attachments/434_Index-internals-PGCon2016.pdf)）。然而，这仅仅只是一个次要问题。
真正使我担心的是这句话：“B-tree 有核心方面，必须周期性再索引（rebalancing）…”，看到这里，我想声明这不是一个每天运行的周期性的过程。索引平衡是和每一次单独的索引变化一同维护的（或许更严重，对吗？）。但原文继续写到“…当子树移动到新的 no-disk 位置时，这些 rabalancing 操作会彻底改变树的结构”。如果你现在认为“rebalancing”包含了大量的数据移动，那你就误解了它。
一个 B-tree 中的重要操作就是结点分裂。正如你所猜想的那样，结点分裂发生在一个结点不能主持属于这个结点的一个新条目。给你一个近似数，这可能在 100 次插入中发生一次。结点分裂分配新的结点，将一般的条目移动到新的结点并将新结点和它的前、后以及父结点连接起来。这就是 Lehman ， Yao 节省了大量锁定的地方。在某些情况下，新的结点不能直接加入到父结点中，因为父结点没有足够的空间来让新结点加入。在这种情况下，父结点进行分裂并重复所有的过程。
在最糟糕的情况下，分裂泡沫会到达根结点，它也会进行分裂，并且会在其之上产生一个新的根结点。在这种情况下，一个 B-tree 将会增加深度。请注意，根结点的分裂有效转变了整棵树，并且因此保持了平衡。然而，这并不包括许大量数据的移动。在最差情况下，它可能会在每个曾经（level）上影响到三个节点和一个新的根结点。更确切地说：根结点分裂的最差情况，可能在一个十亿的插入中发生大约 5 次。在另一种情况下，它不需要遍历整棵树。总的来说，索引的维护不是“周期性的”，甚至不会发生的很频繁，并且永远不会改变整棵树的结构。至少在物理存储单元不会这样。
### 关于物理复制
我的下一个关注点是原文有关于 PostgreSQL 的物理复制（Physical Replication）。原文谈到「索引再平衡」话题的原因是 Uber 曾经遇到了 PostgreSQL 的复制错误，这导致了下游服务器的数据毁损。（这个错误值“影响了 Postgres  9.2 的一些特定版本并且已经修复了一段很长时间”）。
因为 PostgreSQL 9.2 只在 core 提供物理复制，复制错误“只造成了树的很大一部分变得彻底无效。”具体解释为：如果一个结点分裂进行错误的重复，那么它就不能指向正确的子结点，这些子树将会变得无效。这是绝对正确的，正如其他警言“如果存在一个 Bug，那么不好的事情就会发生”。你不需要改变大量的数据，就可以打破树的结构。一个单独的坏指针就足够了。
Uber 的文章提到了其他关于物理复制的情况：大量的复制流量—一部分由于更新带来的编写增幅—并且故障时间需要更新到新的 PostgreSQL 版本。如果第一个对于我而言是由意义的，那么我是不能评论第二个的（但是这里仍有 [statements on the PostgreSQL-hackers mailing list](https://www.postgresql.org/message-id/20160727002711.GI4028%40tamriel.snowman.net) ）。
最后，原文也声明了 Postgres 并没有真正的 MVCC 支持副本。幸运的是，原文链接到了 PostgerSQL 的文档，里面这个问题得到了解释。这个问题基本上是主机并不需要知道副本做了什么，并且因此会删除数据，而这些数据仍然需要副本来完成查询。
根据 [PostgreSQL 官方文档](https://www.postgresql.org/docs/9.2/static/hot-standby.html#HOT-STANDBY-CONFLICT)，有两个办法可以处理这个问题：
1.为可配置的超时延迟复制流的应用，以便读取事务有机会完成。如果查询没有按时完成，杀死这个查询，并且继续应用这个复制流。
2.配置副本，把所有正在运行的查询，反馈送回到主机（master），这样一来主机并不需要清空行版本（row versions），恰好这些行版本在一些从属（slave）中仍然需要。Uber 的文章对第一个版本制定了规则，但根本没提第二个版本。相反，原文指责了 Uber 的开发人员。
### 关于开发者
以荣誉之名引用这段话：“举例来说，开发者有一些数据需要将发送给用户。取决于代码是如何写的，这些代码可能隐含有一个数据库事务，同时执行不相关的 I/O 阻塞，事实上是许多工程师都不是数据库方面的专家，因此不能总是理解这些问题，尤其是当使用 ORM 时，ORM 隐藏了许多底层细节，比如 open transactions 。”
不幸的是，我理解甚至同意这个评论。但是我不认同“许多工程师不是数据库，是因为每一个开发人员接触 SQL 是需要了解 transaction 的 ，不只数据库专家。”
鉴于对开发人员进行 SQL 培训是我的主要工作。我在各种规模的公司做过这件事。如果说我能确定一件事，那就是和数据库相关的知识低的离谱。比如在刚刚提到的“open transaction”，我能确定的是，很少有开发人员知道只读事务是真事。大多数的开发人员只知道 事务可以用于回写。我遇到太多这种误解了，所以我准备了[幻灯片来解释](http://www.slideshare.net/MarkusWinand/sql-transactions-what-they-are-good-for-and-how-they-work)。
### 关于成功
这是我想写的最后一个问题。一个公司雇佣的员工越多，他们的资质就越趋于平均。夸张一点说，如果你雇佣了整个星球上的人员，你将会拥有绝对的平均水平。雇佣再多的人，只是增加了公司的规模。
这里有两种方法来击败这种比率：
1、只雇佣最好的员工。这个方法最难的部分就是，当没有高于平均资质的候选者时，需要等待。
2、雇佣一般的员工，然后在工作上培训。这对于新员工来说，需要一段很长的热身时间，并且可以结合现有员工进行培训。这两个方法的共同问题就是需要时间。如果你没有时间——因为你的公司在快速增长——你就不得不接受平庸，接受一些不是很了解数据库的人（ [empirical data from 2014](http://use-the-index-luke.com/blog/2014-02/results-three-minutes-sql-performance-quiz) ）。换句话来说：**对于一个快速增长的公司来说，技术比人更容易改变。**
正如随时间改变着的需求，成功的因素也会影响技术栈。在初期阶段，创业公司需要创造性的技术，即可以立即获得并且足够灵活来用于业务中。 SQL 一个比较好的选择，因为它确实灵活（你可以用任何方法来查询你的数据）并且很容易找到理解 SQL 的人，哪怕只对 SQL 有一点点了解。好了，那么就开始吧！然而对许多—或许大多数公司来说，故事就在这里结束了。即使他们比较成功并且他们的公司成长了，他们也可能在 SQL  的局限中永远处于原地踏步。Uber 不这样。
一些幸运的创业公司最终从 SQL 蜕变。随着这种情况的发生，他们会有更多的机会访问资源并且然后…一些奇妙的事情发生了：他们意识到，如果他们开发一个只为自己使用的专用系统来取代通用数据库，他们就可以解决许多问题。这就是一个全新 NoSQL 数据库诞生的时刻。在 Uber ，他们将之称为 [Schemaless](https://eng.uber.com/schemaless-part-one/) 。
### 关于 Uber 的数据库选择
直到现在，我相信 Uber 没有像他们的文章所建议的使用 MySQL 取代 PostgreSQL 。看来他们确实在一些特定的方法上替换了 PostgreSQL ，而这些方法恰好是 MySQL / InnoDB 所支持的（在当时）。看来原文只是解释了为什么 MySQL/InnoDB 比 PostgreSQL 更好地支持 [Schemaless](https://eng.uber.com/schemaless-part-one/) 。对于在用 Schemaless 的人来说，采纳他们的建议！不幸的是，原文并没有明确这一点，因为它并没有提及，和 2013 年从 MySQL 迁移到 PostgreSQL 相比，他们的需求是如何随着 [Schemaless](https://eng.uber.com/schemaless-part-one/) 的引进而改变的。
可悲的是，Uber 那文章留在读者脑海中的唯一事情，就是 PostgreSQL 很差劲。
