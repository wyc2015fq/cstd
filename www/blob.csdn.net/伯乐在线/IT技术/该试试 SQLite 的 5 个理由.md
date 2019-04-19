# 该试试 SQLite 的 5 个理由 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [郑芸](http://www.jobbole.com/members/paradise_FD) 翻译，[sunshinebuel](http://www.jobbole.com/members/sunshinebuel) 校稿。未经许可，禁止转载！
英文出处：[charles leifer](http://charlesleifer.com/blog/five-reasons-you-should-use-sqlite-in-2016/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
如果你还没听说，SQLite是一款能够实际运作在真实产品环境中的惊艳的数据库。在这篇文章中，我会列出5大理由来解释为什么我认为你应该在2016年试试 SQLite。
### 简便管理
你管理过一个Postgres数据库吗？为了确保数据库服务器得到恰当设置（共享数据缓冲区，有效缓存大小，工作记忆，维持工作记忆，缓冲机制…）你需要明了相当多的事项。升级也会是一个可怕的过程，你也许需要离线你的数据库，运行一个程序来阻止它升级，并希望等你重启时它仍可以运行。更进一步说，你知道你的 postgres 数据库存储在哪吗？你可以指着一个地方说，「这就是我的数据库吗」？
> 
（现在，我们都知道在很多情况下只有 Postgres （或MySQL, Oracle, SQL Server等）可以满足应用上的要求。这里我的目的并不是说SQLite另有规定。而是我只想强调相比较于传统的数据库服务器，管理一个SQLite db数据库的不同点。
SQLite管理起来非常简便——它是一个单一的文件（或者有时是一个文件 + 事务日志）。文件格式在主要的版本中非常稳定，所以如果我在SQLite3.0.0版本上（2004年推出的版本）有数据库文件，我可以使用 SQLite 3.10.0版本读出它。如果我想把数据文件装在U盘中随身携带，我可以拷贝文件，或者目前更好的是我可保存在我的dropbox文件夹。如果我每天晚上都要备份这些文件，我只需把数据库文件同步到S3。如果我想共享我正与我同事合作的数据分析，我只需发送给他们这个数据库文件的一个我已准备好的副本。数据库在一个单一的文件中，且保持稳定的格式是SQLite的一大特性。
而且，SQLite设置起来也很方便。 SQLite的功能可以用两种方式来管理：编译标志 和 编译指示语句（运行时配置）。这里没有指明的配置文件，你只需把你想要的功能构建成库，然后在创建数据库连接时配置这些运行时选项。
### 不断改善目前已坚若磐石的稳定性
SQLite是由几个真正的软件工程师大神主动开发。并以惊人的速度不断增加高质量的新功能。就在前不久SQLite通过[json1拓展](https://www.sqlite.org/json1.html)来支持JSON数据（获取更多关于在 Python上应用它的文章，请点击[这篇文章](http://charlesleifer.com/blog/using-the-sqlite-json1-and-fts5-extensions-with-python/)）。SQLite还发布了一个改进的全文检索的扩展版本，其中包括使用BM25算法的结果排序。
除了增加新的功能，SQLite开发者也正在努力使库的使用更高效。在3.8.11版本，发行说明包含这条宣传语：
> 
SQLite如今的速度是 3.8.0版本速度的两倍，是 3.3.9版本速度的3倍。
尽管有了这些变化和改进，SQLite鲜少出现 bug。SQLite测试套件被广泛认为是行业内最佳之一，在开发者发现这个令人印象深刻的文件后，[SQLite如何测试](https://www.sqlite.org/testing.html)的页面经常出现在HackerNews上。
### 可拓展性和可控性
我个人最喜欢的特性是它的可扩展性。因为SQLite是由你的应用程序插入的，它运行在相同的地址空间，可以代表你执行应用程序代码。无论是Python标准库SQLite驱动，[pysqlite](https://github.com/ghaering/pysqlite)，和替代驱动[apsw](https://github.com/rogerbinns/apsw)提供的APIs来定义自定义的SQL函数，聚合函数和排序规则。apsw更进了一步，提供APIs定义虚拟表和虚拟文件系统！
举一个实际的例子，假设在你的数据表中有一列存储 URLs，你希望确定哪个是最常见的主机名——如果你正使用不同的数据库，你会被迫去写一个复杂的正则表达式，一个字符串操作函数的组合，或把数据放入你的应用程序并在代码中进行运算。而有了SQLite，你可以在Python中定义一个主机名的功能，然后使用它来创建一个简单的计数查询：

```
from urlparse import urlparse
def hostname(url):
    return urlparse(url).netloc
conn = sqlite3.connect('my-database.db')
conn.create_function('hostname', 1, hostname)  # name, num_params, func
```


```
SELECT hostname(mytable.url), COUNT(mytable.id) AS ct
FROM mytable
GROUP BY hostname(mytable.url)
ORDER BY ct DESC;
```
您还可以创建聚合函数，它接受 `0..n` 值，并生成一个单个输出值。例子可能是计算标准偏差、通过用一些方式处理值来产生字符串、或者做一些类型的分类等等。
虚拟表，目前只受apsw支持，允许你在代码中定义一个表，然后把它当作一个正常的SQL表来查询，即使后台的数据完全是动态的。举例来说，我写了一个简单的虚拟表，可以允许你[像查询SQL表一样来查询Redis](https://github.com/coleifer/peewee/blob/master/examples/redis_vtable/redis_vtab.py).[](https://github.com/coleifer/peewee/blob/master/examples/redis_vtable/redis_vtab.py)
你也可以写[同名的函数](http://sqlite.org/vtab.html#tabfunc2)，其中函数返回0..n行结果。正则表达式搜索函数也许是个例子，它处理输入值和产生匹配令牌的行。我写了一个库，[sqlite vtfunc](https://github.com/coleifer/sqlite-vtfunc)，它使得写这些类型的函数变得极其容易。
SQLite的每个方面都可以被你的应用程序操作。
### Lightning fast
SQLite运行起来很快。它在同一个机器上运行，所以在执行查询和读出结果时没有网络管理费用。它在同一个地址空间运行，所以没有有线协议，串行化或需要通过UNIX套接字通信 。SQLite可以在资源匮乏、要求高效率的无线设备上运行。SQLite也支持大量编制标志，允许你移除不打算用的功能。
SQLite的速度弥补了他的最大的缺陷，就是数据库文件锁定为写。正因为写数据的速度快到不可思议，只有当存在大量的并发写进程时，数据库的锁定才会成为一个问题。
### WAL ( Write-Ahead Logging  预写式日志记录 ) 模式
3.7.0版本的SQLite添加了一个新的日志记录方法，使用预写日志记录。这本身不是什么真正值得兴奋的新闻，但这意味着对于 web应用（或任何并发机制）开发者来说，读进程和写进程不再相互限制。或者换个方式来说，读和写可以发生同时。如果没有有 WAL 模式，为了写这个数据库，写进程将要求独占访问权，在撰写完成前，这个数据库无法被访问。
下面是一个例子来图解两者的不同，假设我们有两种进程状态，写进程和读进程。写进程开始一个专有事务（表示有撰写的意图）。接下来，读进程会开启一个事务，然后读进程尝试去发布一个筛选的状态：
Journal mode = “delete” (the default):
- Writer: `BEGIN EXCLUSIVE`
- Reader: `BEGIN`
- Reader: `SELECT * FROM foo;` Error: database is locked
Journal mode = “wal”:
- Writer: `BEGIN EXCLUSIVE`
- Reader: `BEGIN`
- Reader: `SELECT * FROM foo;` Returns table contents
然而，值得注意的是，即使你不具有WAL模式，写通常发生在毫秒间。这是一个如此短的时间来让你注意到问题，如果你有非常高的并发或有很长的事务要写。
### 额外的原因：BerkeleyDB
BerkeleyDB的SQLite的整合甚至可以给需要访问并发数据库的应用程序开发者们提供更好的性能，因为BerkeleyDB不是锁定整个数据库，而是只需要锁定单个页面。这使得BerkeleyDB在加载并发数据库时有更高的吞吐量，只要事务没有争夺同一页数据。BerkeleyDB还支持多版本并发控制（MVCC），它允许读的操作继续发生在正在运作写的事务的数据页。
BerkeleyDB另一个好处是效率的提高。换句话说，BerkeleyDB可以使用更少的系统资源和执行更少的系统调用。你可以在[这份白皮书](http://media.charlesleifer.com/playground/notes/files/bdb-sqlite-comparison-wp-176431.pdf)和[简要技术概述中](http://media.charlesleifer.com/playground/notes/files/bdbvssqlite-wp-186779.pdf)找到更多的细节。
BerkeleyDB的SQL接口是SQLite的一个插入式替换方式，并支持相同的APIs和功能。BerkeleyDB提供了一些附加功能，比如复制（SQLite有备份实用工具，但我的理解是，它不如BDB强大）、加密，当然还有包括BerkeleyDB自身所有的功能。
BerkeleyDB使用起来的一大缺点是对配置值非常敏感，并且获取正确的页面大小、缓存大小等其他设置需要深厚的知识。另一个缺点是授权许可——阅读更多关于BerkeleyDB的授权许可，可查看[Oracle的授权页面](http://www.oracle.com/technetwork/database/berkeleydb/downloads/licensing-098979.html)。
关于编译使用BerkeleyDB编译Python SQLite驱动的操作指南，可以[看看这个帖子](http://charlesleifer.com/blog/updated-instructions-for-compiling-berkeleydb-with-sqlite-for-use-with-python/)。
### 结束语
我希望你可以试一试SQLite 。不要相信FUD（Fear,Uncertainty,Doubt，一种别有用心的打击手段）对它的描述：没有生产价值，或者不适合在web应用上使用。
如果你想了解更多，SQLite本身有一个好的描述： [when to use SQLite](https://www.sqlite.org/whentouse.html), 它总结了一个单子，什么情况下RDBMS可以更好地工作。我也曾写过一篇同样的短文叫做：[SQLite: Small. Fast. Reliable. Choose any three](http://charlesleifer.com/blog/sqlite-small-fast-reliable-choose-any-three-/)  ，你可能会喜欢。最后，你可查看我带「[sqlite](http://charlesleifer.com/blog/tags/sqlite/) 」文章来看更多热文。
感谢你抽出宝贵的时间来阅读，如果你有任何问题、评论或者建议欢迎在评论区留言~
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://jbcdn2.b0.upaiyun.com/2018/04/10c821c591861f1d3069bcc2d1bb6649.png)![](http://jbcdn2.b0.upaiyun.com/2016/06/7bd3dbe2e00ad5c5d39167e2fbbc6464.jpg)
