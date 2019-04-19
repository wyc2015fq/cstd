# SQL性能优化前期准备-清除缓存、开启IO统计 - 文章 - 伯乐在线
原文出处： [VicentRen](http://www.cnblogs.com/Ren_Lei/p/5669662.html)
如果需要进行SQl Server下的SQL性能优化，需要准备以下内容：
## **一、SQL查询分析器设置：**
1、开启实际执行计划跟踪。
![](http://jbcdn2.b0.upaiyun.com/2016/08/bd0598820becf4a11c721fd3d459d663.png)
2、每次执行需优化SQL前，带上清除缓存的设置SQL。
平常在进行SQL Server性能优化时，为了确保真实还原性能问题，我们需要关闭SQL Server自身的执行计划及缓存。可以通过以下设置清除缓存。


MySQL
```
1 DBCC DROPCLEANBUFFERS  --清除缓冲区
2 DBCC FREEPROCCACHE  --删除计划高速缓存中的元素
```
3、开启查询IO读取统计、查询时间统计。


MySQL
```
SET STATISTICS TIME ON --执行时间
SET STATISTICS IO ON --IO读取
```
开启设置后，执行SQL效果如下：
![](http://jbcdn2.b0.upaiyun.com/2016/08/4dd7bc4be4b92a23d7157fb6d1544fc7.png)
![](http://jbcdn2.b0.upaiyun.com/2016/08/5ab147c17d4cbf5d5e6507ab47581f77.png)
![](http://jbcdn2.b0.upaiyun.com/2016/08/d556287fb0388384013b8a4dc7b034c7.png)
针对其中的每个图标节点，鼠标滑上去的时候，可以看到具体的执行信息。如下图：
![](http://jbcdn2.b0.upaiyun.com/2016/08/af1764b6b7c4e37c7ad4755fb48e6199.png)
可以通过查看谓词、对象、输出列表，分析问题点或者创建优化索引等。
当然你也可以换一种查看方式，点击右键选择显示执行计划XML。
还有一点特别说明的是：当你SQL很长逻辑关系很复杂的时候，执行计划会是一个很大的网状关系图，你会发现在右下角有一个加号的按钮，点击后一个缩略图。通过缩略图你可以很方便的定位执行节点，用起来还比较好用。
## **二、针对SQL Server Profile，SQL查询跟踪器进行分析。**
1、打开方式：SQL Server查询分析器->工具，SQL Profile。打开方式截图：
![](http://jbcdn2.b0.upaiyun.com/2016/08/068771cb985970ced7f51a750044236e.png)
2、连接&特殊设置：
打开后界面如下图：
![](http://jbcdn2.b0.upaiyun.com/2016/08/c54b9fe3ff5edc4c26ada10c39552e0d.png)
设置正确连接信息后，点击连接，弹出如下界面。按照图中操作步骤进行设置。
![](http://jbcdn2.b0.upaiyun.com/2016/08/51de551ffb37cf3d1c8de661a32b6554.png)
其中DatabaseId、HostName可以在查询分析器中进行查询，脚本如下：


MySQL
```
1 SELECT DB_ID()
2 SELECT DB_NAME()
3 SELECT HOST_ID()
4 SELECT HOST_NAME()
```
实际上HostName就是你的本机计算机名。
最终设置完之后点击运行。正常跟踪的效果如图：
![](http://jbcdn2.b0.upaiyun.com/2016/08/ce251d51e139317f197d76289cb89daf.png)
重点关注其中的Duration、Writes、Reads、CPU，分析对象是TextData，及执行的语句。其中Duration为毫秒数，1000即为1秒。
——————————————————————————————————————————
**应用总结&建议**：
上面应用配合方式是：
1、先通过SQL查询跟踪器，跟踪出你所以执行的SQL，然后定位其中Duration比较的SQL 或者超过性能标准的SQl（比如页面访问3s、5s、8s）、报表30s等。
2、将问题SQL在查询分析器中进行分析，主要通过执行计划及IO统计定位耗时占比高及IO读取大的地方，然后逐步的调整SQL逻辑关系（比如添加业务条件过滤缩小集合，建立索引、调整like匹配等），优化后再重新进行跟踪看看是否有效果，最终达到SQL的优化目的。
写到这里，基本上我常用的SQL性能优化的方式就已经讲完了，希望给大家能提供帮助。
