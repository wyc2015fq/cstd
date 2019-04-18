# Greenplum数据库（GPDB）初识 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年04月27日 14:25:16[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4844








1、特性

Greenplum数据库（GPDB），分布式数据库，数据仓库快速查询工具，特性：

1）GPDB完全支持ANSI SQL 2008标准和SQL OLAP 2003 扩展，从应用编程接口上讲，它支持ODBC和JDBC；和 NoSQL、NewSQL和Hadoop 等不支持 SQL不一样 ；


2）支持分布式事务，支持ACID；保证数据的强一致性。


3）分布式数据库，拥有良好的线性扩展能力。


我的一个简单理解就是：Hadoop和GPDB都是分布式集群（分布式存储和分布式计算），不同的是GPDB是支持SQL标准，或者说处理和存储结构化数据，适用OLAP和OLTP，适用数据集市和数据仓库的建设。




2、架构

![](https://img-blog.csdn.net/20170427141138678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


数据库由Master Severs和Segment Severs通过Interconnect互联组成。


1）Master主机负责：建立与客户端的连接和管理；SQL的解析并形成执行计划；执行计划向Segment的分发收集Segment的执行结果；Master不存储业务数据，只存储数据字典。  


2）Segment主机负责：业务数据的存储和存取；用户查询SQL的执行。 


Master类似于Hadoop平台的Namenode，而Segment则类似于Hadoop平台的Datanode。




 greenplum使用mpp架构。


![](https://img-blog.csdn.net/20170427141338965?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








 greenplum部署体系


![](https://img-blog.csdn.net/20170427141524888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





3、Master节点：

![](https://img-blog.csdn.net/20170427141614887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


master node高可用，类似于hadoop的namenode和second namenode，实现主备的高可用。


![](https://img-blog.csdn.net/20170427141646482?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





4、Segments节点

![](https://img-blog.csdn.net/20170427141727811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5、管理

1）数据装载和性能监控

![](https://img-blog.csdn.net/20170427141951126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2）并行备份和恢复

![](https://img-blog.csdn.net/20170427142035940?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


3）数据访问流程

![](https://img-blog.csdn.net/20170427142125456?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


4）查询流程：分查询创建和查询分发，计算后将结果返回

![](https://img-blog.csdn.net/20170427142211609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


5）分布存储

![](https://img-blog.csdn.net/20170427142240521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





数据的分布，分为hash分布和随机分布两种。


![](https://img-blog.csdn.net/20170427142328735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在线数据分布：均匀分布

![](https://img-blog.csdn.net/20170427142507424?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)










