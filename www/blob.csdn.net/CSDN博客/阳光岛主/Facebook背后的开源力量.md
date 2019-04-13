
# Facebook背后的开源力量 - 阳光岛主 - CSDN博客

2013年09月13日 23:18:46[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5126


尽管Google极其推崇开源，并推出大量的开源项目，但是从根本上来看，Google的开源力度、开放程度远远比不上Facebook。这也是为什么Facebook能够在短短的几年间，一跃成为全球最大的社交网站的原因之一。

**一、Facebook vs. Google**
Facebook从2004年创始以来，已经从一个哈佛大学校内网站一跃发展成为了全球最大的社交网站。目前，Facebook的用户数量已经突破10亿，全球平均每天有7亿用户在使用其服务，远远将Google+（活跃用户3.43亿）甩在了身后。
相比Google+，Facebook已经完胜。Facebook大量的用户也造就其巨额的广告收入，在互联网广告业务方面，Facebook已经成为Google最具威胁的竞争对手。
Facebook发展如此迅速，与其内部大量的人才分不开。而近几年，Facebook与Google的竞争也体现在人才抢夺战中。Facebook疯狂从Google、微软、苹果、雅虎等公司挖掘人才，以微软为例，2012年每1名Facebook员工跳槽至微软，就有30.5名微软员工跳槽至Facebook。而Google对Facebook的该比例为15.5 : 1。
如今Facebook的员工总数已达5000人，尽管如此，Facebook公司的规模还只相当于Google的十分之一（Google员工已经达到5万多人）。而在资金方面，Facebook比Google也差了一大截。
在人力、财力都不及对手的情况下，Facebook凭什么得以与互联网的霸主Google抗衡？其中一个很大的因素就是开源，这样可以吸引全球更多的开发者为你工作，以此来弥补某些不足。

**二、Facebook的开源大手笔**
尽管Google一直声称拥抱开源，但是Google对于其某些技术、服务器方面的信息讳莫如深。而在此方面，Facebook要显得大度得多。
1.  开源Facebook平台代码
Facebook在2008年选择将该平台上的重要部分的代码和应用工具开源。Facebook称，平台已经基本发展成熟，此举可以让开发者更全面地理解整个Facebook平台，更容易地为Facebook开发应用软件，并可以回报社区。
该项目代号为“[FBOpen](https://github.com/facebook/platform/tree/master/fbopen)”，其中包含了实现Facebook平台的一些基础设施、功能等，如API架构、FQL分析器、FBML分析器、FBJS，以及许多常用方法和标签的实现，代码基于PHP。这意味着其他开发者可以很方便地利用Facebook所采用的基础设施和技术。
开源的代码中，绝大部分使用CPAL（Common Public Attribution License）许可，而FBML分析器使用的是MPL（Mozilla Public License）许可证。
2.  开源数据中心和网络系统
为了在数据中心上与Google对抗，2011年Facebook宣布了[开源计算项目](http://www.opencompute.org/)（Open Compute Project，简称OCP）计划。该项目旨在创建“开源的”数据中心硬件，Facebook数据中心设计图、服务器配置、数据中心监控界面等悉数开源。Facebook此举的目的是为大数据中心建立更快、更便宜、用材更少的硬件。更重要的是，通过OCP免费提供的设计，任何企业都可以使用它，并对其进行调整。
今年5月份，Facebook又[宣布](http://www.opencompute.org/2013/05/08/up-next-for-the-open-compute-project-the-network/)将其包括交换机等网络设备在内的网络系统开源。
采用开源数据中心，使得Facebook可以与Google这一规模、经济实力都在它之上的对手在数据中心领域展开竞争。

**三、Facebook开源的基础设施**
除了大的系统平台、数据中心外，Facebook也开源了其所使用的基础设施和服务。介绍如下。
1.[Apache Cassandra](http://cassandra.apache.org/)
这是一个分布式的存储系统，用来管理结构化数据，并可以扩展非常大型的、跨多个商品服务器（commodity servers）的大数据集，而不用担心出现单点故障。
2.[Apache Hive](http://hadoop.apache.org/hive/)
这是一个数据仓库基础设施，构建于Hadoop之上，其提供的工具可以轻松对大数据集进行数据汇总、即席查询（adhoc querying）和分析等。
3.[FlashCache](http://github.com/facebook/flashcache/)
这是一个针对Linux的通用的回写块缓存。它可以作为一个可加载的Linux内核模块，在文件系统之下使用。
4.[HipHop for PHP](http://wiki.github.com/facebook/hiphop-php/)
这是一个PHP虚拟机、运行时、JIT（Just In Time，实时生产系统）。它的前身是HPHPc（可将PHP代码编译为C++的项目）。HipHop虚拟机为PHP代码带来了一个大大的性能提升。
5.[folly](https://github.com/facebook/folly)
folly（Facebook Open-source LibrarY）是一个提供类似std和boost功能的底层库，基于C++11特性。folly的开发者包括了Andrei Alexandrescu之类的C++编程领域的大师级人物。folly与std和boost的最大不同就是在大规模的场景中效率更高。
6.[Scribe](http://github.com/facebook/scribe)
这是一个可扩展的服务，用于从大量的服务器中实时聚合日志数据流。
7.[Thrift](http://incubator.apache.org/thrift/)
该项目提供了一个框架，用于可扩展的跨语言服务开发。Thrift结合了一个软件堆栈和一个代码生成引擎，来构建可高效工作的服务，可无缝跨C++、Java、Python、PHP、Ruby、Erlang、Perl、Haskell、C\#、Cocoa、JavaScript、Node.js、Smalltalk、OCaml、Delphi和其他语言。
8.[Tornado](http://www.tornadoweb.org/)
一个相对简单、无阻塞的Web服务器框架，使用Python编写。它被设计用来处理成千上万的并发连接，这使得它非常适合实时Web服务。
9.[React](http://facebook.github.io/react/)
这是一个用于构建用户界面的JavaScript库，用来呈现[instagram.com](http://instagram.com/)和Facebook的部分站点。
10.[FBMock](https://github.com/facebook/FBMock)
FBMock是一个PHP mocking框架，简单且易于使用。

**四、Facebook开源的内部开发工具**
Facebook也选择将其内部开发人员所使用的工具开源。
1.[codemod](http://github.com/facebook/codemod)
可以协助开发者对大型代码库进行部分自动化重构工作，但仍然需要人的监督和不定期的干预。
2.[Facebook Animation](https://developers.facebook.com/docs/fbjs)
这是一个JavaScript库，可以帮助开发者使用DOM和CSS来创建自定义动画。
3.[Online Schema Change for MySQL](http://www.facebook.com/notes/mysql-at-facebook/online-schema-change-for-mysql/430801045932)
允许你在线更改集群中的大型数据库表，而不需将集群离线。
4.[Phabricator](http://phabricator.org/)
这是一个Web应用程序集合，可以帮助开发者轻松编写、审查和共享源代码。目前数百名Facebook工程师每天都在使用它。
5.[PHPEmbed](http://github.com/facebook/phpembed)
该工具使得嵌入PHP对于开发者来说变得真正简单，这是一个更方便和更简化的、构建于PHP SAPI之上的API。
6.[phpsh](http://www.phpsh.org/)
该工具为PHP提供了一个交互式的shell，功能包括readline历史、标签自动完成以及快速访问文档等。有趣的是这个工具是用Python写的。
7.[Three20](http://github.com/facebook/three20)
这是一个针对iPhone开发者的Objective-C库，提供了很多UI元素和数据辅助工具，不过该项目现在已经不维护了，不过你可以用于iOS5/6系统的应用开发中。
8.[XHP](http://github.com/facebook/xhp)
XHP是一个PHP扩展，可以增强PHP语言的语法，比如将XML文档片段变为有效的表达式。
9.[XHProf](http://pecl.php.net/package/xhprof)
这是一个针对PHP的函数级的分层分析器，拥有一个简单的基于HTML的导航界面。

**五、开发平台**
Facebook的平台工程团队也发布和维护着一些开源的平台SDK，这些SDK可以帮助开发者很方便地将Facebook整合进第三方应用中。
[Facebook Android SDK](https://github.com/facebook/facebook-android-sdk)：针对Android平台的SDK。
[Facebook iOS SDK](https://github.com/facebook/facebook-ios-sdk)：针对iOS平台的SDK。
[Facebook JavaScript SDK](https://github.com/facebook/facebook-js-sdk)：针对JavaScript应用的SDK。
[Facebook PHP SDK](https://github.com/facebook/facebook-php-sdk)：针对PHP应用的SDK。

**六、Facebook工程师贡献的开源项目**
Facebook的工程师也参与了很多开源项目的贡献。这些开源项目在Facebook中都有应用，并在生产环境中得到了优化。
1.[Apache Hadoop](http://hadoop.apache.org/)
Apache Hadoop提供了可靠的、可扩展的、分布式的计算基础设施，在Facebook中用于数据分析。
2.[Apache HBase](http://hbase.apache.org/)
这是一个分布式的、面向列的数据存储，构建于Hadoop分布式文件系统（HDFS）之上。
3.[Cfengine](http://www.cfengine.org/)
这是一个基于规则的配置系统，用于服务器的自动配置和维护。Facebook使用Cfengine来维护主机配置，并在产品层面上自动化许多管理操作。
4.[jemalloc](http://www.canonware.com/jemalloc/)
这是一个快速、一致、支持堆分析的内存分配器。Facebook的工程师为其添加了堆分析特性，并做出了很多优化。
5.[memcached](http://memcached.org/)
这是一个分布式的内存对象缓存系统。Memcached最初不是由Facebook开发的，但Facebook已经成为了该技术的最大用户。
6.[MySQL](http://www.mysql.com/)
MySQL是Facebook数据库基础设施的骨干。你可以在[Launchpad](https://launchpad.net/mysqlatfacebook)中找到很多Facebook贡献的补丁，还可以通过[MySQL@Facebook页面](http://www.facebook.com/MySQLatFacebook)来了解Facebook如何使用它。
7.[PHP](http://www.php.net/)
Facebook的大部分代码都是使用PHP开发的。它的语法很简单，让Facebook在产品上能够更快地迁移和迭代。
8.[Varnish](http://varnish-cache.org/)
在Facebook，该项目每天处理来自全世界用户的数十亿请求。当你加载朋友的图片和和头像时，Varnish就有参与。

**总结**
由上可知，Facebook在开源方面的力度非常大。在与规模、财力都在其之上的Google的竞争中，开源确实是一个非常好的途径——采用开源项目，使得Facebook可以使用很少的成本即可快速构建稳定、高效的系统；而通过将自己的产品开源，Facebook吸引了全球大量的优秀的开发者为其工作。
Facebook的所有开源项目都可以在Github中找到：[https://github.com/facebook](https://github.com/facebook)
利用开源，使得Facebook这个后起之秀能够快速崛起，并逐渐缩小与互联网霸主Google的差距。


**参考推荐：**
[Facebook背后的开源力量](http://os.51cto.com/art/201308/407789.htm)



