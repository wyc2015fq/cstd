# 深入解析SQL Server并行执行原理及实践(上) - 文章 - 伯乐在线
原文出处： [ShanksGao（@ShanksGao）](http://www.cnblogs.com/shanksgao/p/5497106.html)
在成熟领先的企业级数据库系统中,并行查询可以说是一大利器,在某些场景下他可以显著的提升查询的相应时间,提升用户体验.如SQL Server, Oracle等, Mysql目前还未实现,而PostgreSQL在2015实现了并行扫描,相信他们也在朝着更健壮的企业级数据库迈进.RDBMS中并行执行的实现方式大抵相同,本文将通过SQL Server为大家详细解析SQL Server并行执行的原理及一些实践.
准备知识
硬件环境-在深入并行原理前,我们需要一些准备知识,用以后面理解并行.首先是当下的硬件环境,社会信息化建设,互联网的普及,硬件工艺的大发展…我们现在的硬件设备的性能虽然已经不复摩尔定律的神奇,但已经相当丰富了,存储上15K RPM的硬盘,SSD,PCI-E SSD使得磁盘作为数据库系统的”终极”瓶颈得到了一定的缓解,而内存上百G内存也早已不是小机的”特性”了,PC Server上已经很普遍.
在处理能力上,由于现今物理工艺上的极限,单颗CPU处理能力提升困难,系统架构已经朝着多颗多核的架构上迅猛发展,如8路CPU的服务器也已经有一定的使用案例了.总之我们的硬件资源越来越丰富,而本文所讲的并行查询查询主要是针对上面叙述的多个CPU协同工作,用以提升SQL查询中的CPU-BOUND响应时间.(虽然在并行扫描(SCAN)的时候实际中同样可以提升查询的响应时间,但这我们就不细述了)
SQL Server关于”任务”的相关知–Schedulers,Tasks,Workers. Schedulers是指SQL Server中SQLOS识别到的硬件环境中的逻辑CPU,它是管理SQL Server计算工作的基本单位,它提供”线程”(workers)用于分配相应的”CPU”资源用于计算.
Tasks实际就是SQL Server中的工作单元,本质上就是一个函数指针(C++),用于指向需要执行的代码.比如LazyWriter实际上就是去调用相应的Function(sqlmin!Lazywriter()).Workers 如果说Tasks是指向工作的地方,那么Workers是处理相应的工作,它绑定到Windows线程的方式用于计算.OK,现在说一条SQL请求SQL Request=Task+Worker应该能理解了吧.为了加深理解,我们可以看图1-1
![1_1](http://jbcdn2.b0.upaiyun.com/2016/08/f79a7029d39c1df8672d9de1a5996e1c.png)
图1-1
最终回到我们的并行查询,实际就是多个tasks在多个schedulers上同时运行,提升响应速度！
关于SQLOS,微软在SQL Server 2005年引入,可以说是个重大的突破,就像Michael Stonebraker(2014图灵奖得主)老爷子的早年Paper” Operating System Support for Database Management”中叙述的那样,SQLOS,比OS更懂数据库,感兴趣的朋友自行搜索下.限于篇幅这里就不细深入SQLOS了,给出一张简单架构图大家感受下J 如图1-2
![1_2](http://jbcdn2.b0.upaiyun.com/2016/08/e099ed2232c236a4123a97c60dafc5fb.png)
图1-2
OK准备知识完毕,我们进入并行查询执行吧
并行相关概念
串行执行计划–在了解并行执行之前,我们先了解下串行执行计划的相关知识.其实很简单,关于执行计划本身这里就不介绍了,只是介绍下与本文相关重要元素.
串行执行计划实际就是由单个线程(thread)执行完成,单个执行上下文(execution context)
执行上下文就是指执行计划中用于执行的一些信息,如对象ID,如执行计划中使用到的临时表等等.在执行中实际上就是单个(核)CPU在工作,如图2-1
![2_1](http://jbcdn2.b0.upaiyun.com/2016/08/5ebcadfd5cf5136e2d482250f051a67e.png)
图2-1
而并行执行计划,顾名思义,就是多个(核)CPU在同时工作,用于提升*CPU-Bound*的响应时间,对应串行,它有多个线程,多个执行上下文,但也会消耗更多的资源.但对于磁盘IO,SQL Server认为是木有帮助的,我们通过一个简单的实例看下:
注:文章中使用的AdventureWorks,网上可以自行搜索下载
—串行执行
select COUNT(*)
from dbo.bigTransactionHistory option(maxdop 1)
–并行执行
select COUNT(*)
from dbo.bigTransactionHistory option(maxdop 2)
通过观察上面两条简单语句的执行计划可以发现,在预估Subtree cost中 (资源消耗量)实际上单CPU与双CPU相比只是CPU预估减半,IO预估不变.如图2-1-a
![2_1_a](http://jbcdn2.b0.upaiyun.com/2016/08/c21ca5a6d0c41000b5c77d3140f556fa.png)
图2-1-a
让我们真正的进入并行执行计划,每个并行执行计划都有一个主的交换操作(root exchange),即图形执行计划中最左面的Gather stream运算符(后面会讲到)(SQL Server有图形执行计划,不小小伙伴很羡慕吧J)而所有的并行执行计划都会有一个固定的串行部分—即最左边Gather stream运算符中所有靠左的部分(包含Gather中消费者,后面会讲到生产-消费模型)的所有操作,他是由主线程Thread Zero控制,同时它的执行上下文也是context zero,如图2-2所示
![2_2](http://jbcdn2.b0.upaiyun.com/2016/08/3479e585158ae9f6c63be2819c4c9dff.png)
图2-2
而并行区域,顾名思义就是root exchange所有靠右的部分,而并行部分又有可能有多个分支
(Branches),每个Branch都可以同时执行(分支有自己的tasks),分支自身可以是并行,也可以是串行.但分支不会使用主线程thread zero.关于分支大家可以用如下语句自己看下相关执行计划属性(SQL Server 2012版本及之后版本可以显现) 如图2-3所示
如图2-3-2,我的最大并行度设置为4,有三个branches,而这里我使用的线程数就是
4*3=12,再加上一个主线程 thread zero 这个并行查询我所使用的线程总数为13个.

MySQL
```
select a.productid,count_big(*) as rows
from dbo.bigproduct as a
inner  join dbo.bigtransactionhistory as b
on a.productid=b.productid
where a.ProductID between  1000 and 5000
group by a.productid
order by a.productid
```
![2_3_1](http://jbcdn2.b0.upaiyun.com/2016/08/da04866eaa69c793d3aecaa30802d830.png)
图2-3-1
![2_3_2](http://jbcdn2.b0.upaiyun.com/2016/08/aaaf0e8095e49997ad36dd2cbac0671a.png)
图2-3-2
而并行和串行的区别联系,大家可以理解成下图2-4
主线程在串行中实际就是他的执行线程
![2_4](http://jbcdn2.b0.upaiyun.com/2016/08/3aa4a4bc1a2cd94f576627b768146c52.png)
图2-4
图2-3中,并行的表扫描后聚合,实际上是等于两个串行的表扫描聚合.这个提到并行表扫描table scan(range scan)就稍微展开下:在SQL Server 2005及以前版本中并行扫描实际上是
” parallel page supplier”及每个线程扫描的单位是数据页,扫描多少有数据页中含有的数据行多少决定,而在SQL Server 2008及以后是”Range Enumerators” 每个线程扫描的数据实际上是由数据的区间分布决定的,至此也就有了我们在08及以后版本中经常看到的“access_methods_dataset_parent”闩锁等待,你可能会疑问怎么高版本还带来新问题,实际上在 ” parallel page supplier”中的问题也是不少的,比如在快照隔离级别下的数据页扫描确认问题等等,总得来说还是更高效了.这里需要大家注意的是并行扫描只支持前滚扫描(forward),不支持反向扫描(backward)感兴趣的朋友可以根据下面代码自行测试.
/*************************并行扫描相关测试*********************************/
—-并行扫描中只有前滚扫描才可以并行

MySQL
```
USE [AdventureWorks2008R2]
GO
select color,COUNT(1) from [bigProduct]
group by Color option(querytraceon 8649)--- Parallel scan only forward
go
CREATE NONCLUSTERED INDEX [inx_color] ON [dbo].[bigProduct]
(
         [Color] desc----asc can parallel desc can not
)WITH ( DROP_EXISTING = ON) ON [PRIMARY]
---修改索引排序规则(asc,desc)
GO
select color,COUNT(1) from [bigProduct]
group by Color option(querytraceon 8649)--- Parallel scan only forward
```
/*************************并行扫描相关测试*********************************/
看到这里不少朋友可能有疑惑了,上文中又是串行,又是并行,又是多tasks,到底什么时候用串行,什么时候并行,用多少个tasks啊,实际上SQL Server实例级有两个设置:并行阈值”cost threshold for parallelism” 即当Subtree cost(前面提到的估计资源消耗)大于设定值时优化器才会触发并行优化,进而可能生成并行执行计划.二:最大并行度(max degree of parallelism)用于设置分支(branches)中到底可以多少个CPU同时工作.这里给大家画个简单的图,大家就一目了然了.如图2-4
![2_5](http://jbcdn2.b0.upaiyun.com/2016/08/a2cdfcaf25336872ac3e76cf4ded288a.png)
图2-5
至此,并行相关的基本概念已经介绍完毕,接下来让我们深入并行..
Exchanges
顾名思义,就是交换,在并行里指的是threads间的数据交换,这也是在只有并行执行的情况下才会有的操作符.具体到SQL Server中有三种exchanges 操作,分别为Gather Streams, Repartition Streams,以及Distribute Streams其对应起到的作用就是聚集,重定向,分发Threads间的数据,操作符如图3-1所示
![3_1](http://jbcdn2.b0.upaiyun.com/2016/08/88bce1618d99cd1227173ce4fc15380a.png)
图3-1
而实际上上述运算符每一个都是两个运算符的”合集”这里就是我们之前提到的生产者与消费者操作.操作符中右边为生产者,将生成的数据放入packets中,而左边为消费者,从packets中将数据取出.如图3-2所示
![3_2_1](http://jbcdn2.b0.upaiyun.com/2016/08/1fa2b7c72ade5cd502c5ae59cf98a712.png)
图3-2-1
![3_2_2](http://jbcdn2.b0.upaiyun.com/2016/08/fc8b54214c009d15a76c5b9d1b1dc004.png)
图3-2-2
这里解释下我们在SQL Server中常见的CXPACKET等待,不少同学都将这个等待看做并行中某些线程工作快,某些线程工作慢而产生的差异,这个对,但说法不专业,实际上根据生产者消费者模型来看分析是:
CXPACKET Waits=Class eXchange PACKET
针对生产者:所有的 Buffer packets 都已经被填满,无法继续生产(填充)
针对消费者:所有的Buffer packets都是空的,没有数据可以消费
在生产者与消费者的模型中,数据是通过一定规则由生产者流动到消费者那里,在SQL Server有如下5种规则
Broadcast：广播,当数据量较小时将生产者的所有数据都广播到所有的消费者中.阿里的朋友在分布式中间件中小表广播应该有不少应用吧J
Hash:通过哈希函数将制定的数据一个或多个栏位哈希化(简单的如取模),根据不同的值填充到不同的Buffer packets中供消费者使用.
Round Robin 顾名思义,将数据顺序逐条地添加到每个Buffer packets中.
Demand 之前的都是发送数据,而这个是消费者从生产者中拉数据,如某个消费者拉某个常量的数据,这个只在分区表中采用.
Range 顾名思义,根据数据中某些栏位的分布,分别填充到不同的Buffer packets中,这个一般在并行索引,统计信息重建时采用.
有了生产者和消费者的数据使用方式,你可能觉得还差点什么吧?没错,数据是否是有序的呢,SQL Server中exchange分别Merge Exchange 和Non-Merge Exchange,分别对应数据在exchage时是否要求有序,当然大家也都知道了有序的成本Merge Exchange明显高于无序
如图3-3所示
![3_3](http://jbcdn2.b0.upaiyun.com/2016/08/0fd1a145a25d0aef293dd03bf229e4a1.png)
图3-3
Parallel joins
最后我们来谈谈SQL Server中的并行Join.SQL Server中对于三种基本join: nested loop join ,merge join, hash Join都支持,我们分别说明下他们的优缺点,在今后的并行优化时,你可能用到.在这里我就不复述三类join链接的基本算法了,不懂的同学wikipedia下
Parallel merge join 如图4-1
将参与join的key(两个表)都进行hash化,然后同时进行匹配.
并行merge join几乎无优点,缺点倒是不少,首先是需要merge exchange,其次增加CPU本身对性能帮助实际不大(参考相应算法),而且merge join还会增加并行死锁的几率.所以实际生产中我们还是应尽量避开merge join.
![4_1](http://jbcdn2.b0.upaiyun.com/2016/08/e6d8d3060a9604ef13732af8fda05ebb.png)
图4-1
Parallel hash join
Build阶段,如果数据量小,所有broadcast到所有threads中 如图4-2-1,否则就将join key 哈希化,进行匹配,如图4-2-2
这里应注意parallel hash join是Non-merge exchanges,且随着CPU的增加,性能是线性扩展的(具体参考相应算法),但在parallel hash join中应注意可能由于统计信息问题,复杂join导致的hash join在构建,探测阶段都有可能因为内存不足而溢出的现象,从而拖慢查询.关于溢出可以使用跟踪,扩展事件捕捉等.
![4_2_1](http://jbcdn2.b0.upaiyun.com/2016/08/6d2286d60072fe391343b0e16d24aebc.png)
图4-2-1
![4_2_2](http://jbcdn2.b0.upaiyun.com/2016/08/7306debf81708579d3f560a925c8cd04.png)
图4-2-2
Parallel nested loop join
外表多个threads同时运行(如扫描数据),而内表则在每个thread上串行同时进行匹配.
Parallel nested loop join在实际生产中可能为我们解决许多一些棘手问题,比如他会减少并行计划中的exchange使用,使用更少的内存等等.但也应注意可能由于数据分布倾斜,nested loop预读等情形导致的性能提升有限,而消耗反而相比串行提高等问题.而且SQL Server的优化器本身是”不喜欢并行循环嵌套的”,有时我们需要特定的写法才能实现他.如图4-3
![4_3](http://jbcdn2.b0.upaiyun.com/2016/08/d4e3fc05b9e2b6663ae34f2d97b4deec.png)
图4-3
最后我们再提下Bitmap过滤吧,SQL Server是没有位图索引的,这个也颇为诟病,但实际SQL在执行时是有可能用到位图过滤的.
简单说下SQL Server位图过滤实现方式(具体大家也可以wikipedia bloom filter)
实现方式:通过构建一个长度X的位数组(bit array)(所有位为0),将要匹配的集合通过哈希函数映射到位数组中的相应点中(相应位为1),当判断一个值是否存在时找bit array中对应位是否为1就可以了.这个过程由SQL Server内部自己完成. 如图4-4-1,图4-4-2我将一个现有数组哈希化,然后在其中搜索 ”悟空..”是否在数组中
![4_4_1](http://jbcdn2.b0.upaiyun.com/2016/08/ef435619d079a191132c3c35a4b824c9.png)
图4-4-1
![4_4_2](http://jbcdn2.b0.upaiyun.com/2016/08/1bccdfb62c35b2209820131a57a6e34e.png)
图4-4-2
好了,SQL Server 相关的并行知识就给大家介绍这么多吧,后面有时间给大家带来相关的实践应用.
