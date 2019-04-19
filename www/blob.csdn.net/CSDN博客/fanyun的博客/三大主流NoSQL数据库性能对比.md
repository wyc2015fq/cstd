# 三大主流NoSQL数据库性能对比 - fanyun的博客 - CSDN博客
2019年01月31日 19:58:09[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：43
近年来，NoSQL数据库凭借其易扩展、高性能、高可用、数据模型灵活等特色吸引到了大量新兴互联网公司的青睐，包括国内的淘宝、新浪、京东商城、360、搜狗等都已经在局部尝试NoSQL解决方案。目前市场上有很多优秀的NoSQL产品，如MongoDB 、SequoiaDB、Cassandra、HBase等，每个产品都有其特性，但也有各自的缺陷。
　　近日，知名独立基准测评机构Bankmark，针对目前市面上主流的NoSQL数据库SequoiaDB、MongoDB以及Cassandra三款NoSQL数据库产品做了性能对比测试并发布测试报告。**在所有的测试中，三款****NoSQL数据库产品的表现各有千秋，没有那款产品能在所有测试场景中完败对手，就整体表现而言SequoiaDB与Cassandra不相上下表现上佳，而MongoDB表现却不理想属于垫底的存在。**
**测试机构：**
Bankmark是一家德国的独立基准测评机构，业内著名的NoSQL性能测试团队。
**测试产品：**
**MongoDB**(目前名气最大，用的人最多的NoSQL数据库，占据了非关系型数据库的主流位置。)
**SequoiaDB**(由前IBM DB2团队的研发人员创建，据称在性能和功能上能够与MongoDB做正面抗衡。)
**Cassandra**(是Facebook的数据库系统的开源分支，支持者众多，而Cassandra能一直稳压HBase(高贵的血统，与Hadoop天然集成)长期霸占列存储第一更是其优越性的强有力证明。)
**测试环境：**
Bankmark使用了 Yahoo Cloud Serving Benchmark(YCSB)测试框架作为测试工具。
　　所有的测试都在物理硬件上进行，没有使用任何虚拟化的层级。
　　所有的数据库测试都在一个10节点的集群上进行，其中10台[服务器](http://product.it168.com/list/b/0402_1.shtml)(DellPowerEdge R520(5台)、Dell PowerEdge R720(5台))作为数据库系统的主机，5台(HPProLiant BL465c)刀片机作为YCSB客户端。
**测试结果：**
　　从报告结果来看，没有那款产品能在所有测试场景中全部击败对手，在“大部分[内存](http://product.it168.com/list/b/0205_1.shtml)环境”的设定下，SequoiaDB的性能明显优于其他的产品，除了在Cassandra的强项“读多写少”。在“全[内存](http://product.pcpop.com/Memory/10734_1.html)环境”下测试结果表明，SequoiaDB拥有更快速的读请求性能，而Cassandra在写请求下表现要好于SequoiaDB。然而，在所有的测试案例中，MongoDB几乎都是最慢的。更多内容请见测试报告（[NoSQL Performance Test](http://wenku.it168.com/d_001578073.shtml)）
　　大部分内存环境(Mostly-in-memory)：原始数据大小为总RAM大小的1/4
　　全内存环境(in-memory)：原始数据大小为总RAM大小的1/2
**测试场景****1： 2亿条记录(每个节点2000万)**
**在本实验中，全内存环境下****(原(始)数据大小大约占系统总内存的45%左右 )**
**1、单条数据导入**
![](https://img-blog.csdn.net/20160408102502988?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲单条记录载入测试
　　从上图结果中可以看到，在全内存环境下，单条记录载入操作，Cassandra最高，总吞吐量可达每秒近7万，Mongodb最差。
**2、批量数据导入(1000条记录一批次)**
![](https://img-blog.csdn.net/20160408102523714?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲批量记录载入测试
　　在该测试中，数据为每批次1000条，测试结果显示，SequoiaDB可以达到每秒钟近6.5万的导入速度。
**3、Zipfian分布，100%读**
![](https://img-blog.csdn.net/20160408102813825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲单纯随机查询测试
　　单纯随机查询的测试中，SequoiaDB表现最为突出，Mongodb次之。
**4、Zipfian分布，50%读，50%更新**
![](https://img-blog.csdn.net/20160408102845809?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲查询更新平衡测试
　　在查询更新平衡测试中（50%读，50%更新），Sequoiadb表现最佳，Mongodb表现最不理想。
**5、Zipfian分布，5%读，95%更新**
![](https://img-blog.csdn.net/20160408102905508?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲更新为主的测试
　　在以更新为主的测试中(5%查询，95%更新)，Cassandra表现最佳。
**6、Zipfian分布，95%读，5%更新**
![](https://img-blog.csdn.net/20160408102953513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲查询为主的测试
　　在以查询为主的测试中(95%查询，5%更新)，mongodb表现最优秀，Cassandra最差
**7、latest分布，95%读，5%插入**
![](https://img-blog.csdn.net/20160408103022666?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲查询最新测试
　　从图中可以看出，查询最新测试中，SequoiaDB对于刚刚写入至内存中便读取的场景性能最佳，Cassandra最差
**测试场景****2： 1亿条记录(每个节点1000万)**
**在本案例****/实验中，大部分内存环境下(原(始)数据大小大约占系统总内存的22%左右)**
**1、单条记录导入**
![](https://img-blog.csdn.net/20160408103111995?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲单条记录导入测试
　　从上图结果中可以看到，在大部分内存环境下，单条记录载入操作，Sequoiadb最佳。这与在全内存环境下结果发生了变化。
**2、批量数据导入**
![](https://img-blog.csdn.net/20160408103130808?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲批量载入测试
　　测试结果显示，SequoiaDB可以达到每秒钟7万多的导入速度。
**3、Zipfian分布，100%读**
![](https://img-blog.csdn.net/20160408103155231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲单纯随机查询的测试
　　单纯随机查询的测试中，SequoiaDB表现依然是最为突出，Cassandra次之。。
**4、Zipfian分布，50%读，50%更新**
![](https://img-blog.csdn.net/20160408103215889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲查询更新平衡的测试
　　在查询更新平衡测试中，50%读，50%更新，Sequoiadb依然表现最佳
**5、Zipfian分布，5%读，95%更新**
![](https://img-blog.csdn.net/20160408103238419?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲更新为主的测试
　　在以更新为主的场景测试中(5%查询，95%更新)，Cassandra秒杀对手。
**6、Zipfian分布，95%读，5%更新**
![](https://img-blog.csdn.net/20160408103306420?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲查询为主的测试
　　在以查询为主的场景中(95%查询，5%更新)，Cassandra表现最佳,Mongodb反而表现最差
**7、 latest分布，95%读，5%插入**
![](https://img-blog.csdn.net/20160408103336077?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
▲查询最新的测试
　　从图中可以看出，与全内存环境不同的结果，SequoiaDB依然最快，Cassandra超过Mongodb仅次于SequoiaDB。
