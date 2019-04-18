# Hive中Order by和Sort by的区别是什么? - z69183787的专栏 - CSDN博客
2016年10月26日 12:11:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7053
Hive基于HADOOP来执行分布式程序的，和普通单机程序不同的一个特点就是最终的数据会产生多个子文件，每个reducer节点都会处理partition给自己的那份数据产生结果文件，这导致了在HADOOP环境下很难对数据进行全局排序，如果在HADOOP上进行order by全排序，会导致所有的数据集中在一台reducer节点上，然后进行排序，这样很可能会超过单个节点的磁盘和内存存储能力导致任务失败。
一种替代的方案则是放弃全局有序，而是分组有序，比如不求全百度最高的点击词排序，而是求每种产品线的最高点击词排序。
使用order by会引发全局排序
select * from baidu_click order by click desc;
使用distribute和sort进行分组排序
select * from baidu_click distribute by product_line sort by click desc;
distribute by + sort by就是该替代方案，被distribute by设定的字段为KEY，数据会被HASH分发到不同的reducer机器上，然后sort by会对同一个reducer机器上的每组数据进行局部排序。
![image](http://www.crazyant.net/wp-content/uploads/2014/06/image_thumb.png)
order by是全局有序而distribute+sort是分组有序
distribute+sort的结果是按组有序而全局无序的，输入数据经过了以下两个步骤的处理：
1) 根据KEY字段被HASH，相同组的数据被分发到相同的reducer节点；
2) 对每个组内部做排序
由于每组数据是按KEY进行HASH后的存储并且组内有序，其还可以有两种用途：
1) 直接作为HBASE的输入源，导入到HBASE；
2) 在distribute+sort后再进行orderby阶段，实现间接的全局排序；
不过即使是先distribute by然后sort by这样的操作，如果某个分组数据太大也会超出reduce节点的存储限制，常常会出现137内存溢出的错误，对大数据量的排序都是应该避免的。
本文地址：[http://www.crazyant.net/1456.html](http://www.crazyant.net/1456.html)
