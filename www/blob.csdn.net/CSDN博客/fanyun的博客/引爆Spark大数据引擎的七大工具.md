# 引爆Spark大数据引擎的七大工具 - fanyun的博客 - CSDN博客
2016年04月05日 14:05:20[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3998
所属专栏：[大数据](https://blog.csdn.net/column/details/bigdata01.html)

   原文名称：7 tools to fire up Spark's big data engine
      Spark正在数据处理领域卷起一场风暴。让我们通过本篇文章，看看为Spark的大数据平台起到推波助澜的几个重要工具。
![七大工具共筑Spark大数据引擎](http://s2.51cto.com/wyfs02/M02/7D/BC/wKioL1bvVuTiFdBhAAX5v04R8UA180.jpg-wh_651x-s_497228242.jpg)
**Spark生态系统众生相**
       Apache Spark不仅仅让大数据处理起来更快，还让大数据处理起来更简单、功能更强大、更方便。Spark并非只是一项技术，它结合了诸多部分，新的功能和性能改进不断添加进来，每个部分都在不断完善之中。
      本文介绍了Spark生态系统的每个主要部分：每个部分的功能，为什么很重要，是如何发展的，在哪方面不尽如人意，以及可能会往哪个方向发展。
**Spark Core**
![七大工具共筑Spark大数据引擎](http://s3.51cto.com/wyfs02/M00/7D/C0/wKiom1bvVoniIZf6AARnId7NBgA221.jpg-s_1526114652.jpg)
      Spark的核心是恰如其名的Spark Core。除了协调和调度作业外，Spark Core还为Spark中的数据处理提供了基本的抽象机制，名为弹性分布式数据集(RDD)。
      RDD对数据执行两个动作：转换和操作。前者转换数据，并将它们作为刚创新的RDD来提供;后者根据现有的RDD(比如对象数量)来计算结果。
      Spark的速度很快，原因是转换和操作都保存在内存中。操作慢腾腾地评估，这意味着只有需要相关的数据时，才执行操作;然而，很难搞清楚什么在缓慢运行。
      Spark的速度在不断提高。Java的内存管理往往给Spark带来问题，于是Project Tungsten计划避开JVM的内存和垃圾收集子系统，以此提高内存效率。
**Spark API**
![七大工具共筑Spark大数据引擎](http://s2.51cto.com/wyfs02/M00/7D/BC/wKioL1bvVziiXI6zAAN-DAP3xPI856.jpg-s_3707800090.jpg)
       Spark主要是用Scala编写的，所以Spark的主要API长期以来也支持Scala。不过另外三种使用广泛得多的语言同样得到支持：Java(Spark也依赖它)、Python和R.
       总的来说，你最好选择自己最擅长的那种语言，因为你需要的功能特性很可能在该语言中直接得到支持。只有一个例外：相比之下，SparkR中对机器学习的支持不大给力，目前只有一小批算法可供使用。不过将来这种情况势必会发生变化。
**Spark SQL**
![七大工具共筑Spark大数据引擎](http://s3.51cto.com/wyfs02/M01/7D/C0/wKiom1bvVsGijJDaAAO3cHfzBzk774.jpg-s_2508059683.jpg)
      千万不要低估了能够对批量数据执行SQL查询的能力或便利。Spark SQL提供了对Spark提供的数据执行SQL查询(并且请求列式DataFrame)的一种通用机制，包括通过ODBC/JDBC连接件进行管道处理的查询。你甚至不需要正规的数据源。Spark 1.6中添加了这一功能：支持以一种得到支持的格式查询扁平文件，就像Apache Drill那样。
       Spark SQL其实并不用于更新数据，因为那与Spark的整个意义相悖。可以将因而生成的数据写回成新的Spark数据源(比如新的Parquet表)，但是UPDATE查询并不得到支持。别指望诸如此类的功能特性很快就会推出;着眼于Spark SQL的改进大多数用于提升其性能，因为它也成了Spark Streaming的基础。
**Spark Streaming**
![](http://s2.51cto.com/wyfs02/M01/7D/BC/wKioL1bvV4_BKFFGAAECo9fPtDE492.jpg-s_39561298.jpg)
       Spark的设计让它得以支持许多处理方法，包括流处理――Spark Streaming因此得名。关于Spark Steaming的传统观点是，它还半生不熟，这意味着只有你不需要瞬间延迟，或者如果你还没有投入到另一种流数据处理解决方案(比如说Apache Storm)，你才会使用它。
      但是Storm在逐渐失去人气;长期使用Storm的推特此后已改用了自己的项目Heron。此外，Spark 2.0承诺会推出一种新的“结构化数据流”模式，以便对实时数据进行交互式Spark SQL查询，包括使用Spark​的机器学习库。至于其性能是否高得足以击败竞争对手仍需拭目以待，不过它值得认真考虑。
**MLlib(机器学习)**
![](http://s1.51cto.com/wyfs02/M02/7D/C0/wKiom1bvVxLSY_24AADv2QhDYsg863.jpg-s_3078108219.jpg)
      机器学习技术素有既神奇，又困难之称。Spark让你可以对Spark中的数据运行许多常见的机器学习算法，从而使这些类型的分析容易得多，也更容易被Spark用户所使用。
      MLlib中的可用算法数量众多，该框架每推出一个修订版，就会随之增多。话虽如此，一些类型的算法还是没有――比如说，涉及深度学习的任何算法。第三方正在利用Spark的人气来填补这一空白;比如说，雅虎可以借助CaffeOnSpark执行深度学习，它通过Spark充分利用了Caffe深度学习系统。
**GraphX​​(图形计算)**
![](http://s4.51cto.com/wyfs02/M02/7D/C0/wKiom1bvV02Q1p2FAADKjV_RVWA614.jpg-s_184782850.jpg)
      描绘数百万实体之间的关系通常需要图形，这种数据构件描述了那些实体之间的相互关系。Spark的GraphX​​ API让你可以使用Spark的一套方法，对数据执行图形操作，于是构建和转换这类图形的繁重任务卸载到了Spark。GraphX​​还包括用于处理数据的几种常见算法，比如PageRank或标签传播(label propagation)。
      从目前来看，GraphX的​​一个主要限制是，它最适合静态图形。处理添加了新顶点的图形会严重影响性能。此外，如果你已经在使用一种成熟的图形数据库解决方案，GraphX还​​不太可能取代它。
**SparkR(Spark上的R)**
![](http://s2.51cto.com/wyfs02/M00/7D/BC/wKioL1bvV_fAA453AADjsG4kKm0078.jpg-s_63682169.jpg)
       R语言为进行统计数值分析和机器学习工作提供了一种环境。Spark在2015年6月添加了支持R的功能，以匹配其支持Python和Scala的功能。
       除了为潜在的Spark开发人员多提供一种语言外，SparkR还让R程序员们可以做之前做不了的许多事情，比如访问超过单一机器的内存容量的数据集，或者同时轻松地使用多个进程或在多个机器上运行分析。
      SparkR还让R程序员可以充分利用Spark中的MLlib机器学习模块，创建一般的线性模型。遗憾的是，并非所有的MLlib功能在SparkR中得到支持，不过Spark每推出一个后续的修订版，都在填补R支持方面的差距。
      作者：布加迪编译来源：51CTO.com
