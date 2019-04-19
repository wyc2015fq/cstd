# SQL SERVER定期转移海量数据方案 - 左直拳的马桶_日用桶 - CSDN博客
2010年08月19日 10:12:00[左直拳](https://me.csdn.net/leftfist)阅读数：2443
SQL SERVER定期转移海量数据方案
【关键词】
1、    分区切换
2、    BCP
3、    BULK INSERT
4、    Insert
【背景】
有个表比较巨大，每天新增约500万条记录。只保留最新7天数据，每天定期移走过期记录，归并到历史库。采用INSERT，DELETE的方法，消耗时间越来越长，平均达到45分钟，期间该表和历史库根本无法访问。
【方案】
1、    采用分区切换，快速清除旧数据
将表按日进行分区，每日一个区。
比如今天是2010年8月8日，有
--分区函数
CREATE PARTITION FUNCTION [CrPFN](smalldatetime) AS RANGE LEFT FOR VALUES (N'2010-08-01T00:00:00.000', N'2010-08-02T00:00:00.000', N'2010-08-03T00:00:00.000', N'2010-08-04T00:00:00.000', N'2010-08-05T00:00:00.000', N'2010-08-06T00:00:00.000', N'2010-08-07T00:00:00.000', N'2010-08-08T00:00:00.000', N'2010-08-09T00:00:00.000')
GO
--分区方案
CREATE PARTITION SCHEME [crSCM] AS PARTITION [CrPFN] TO ([cr0], [cr1], [cr2], [cr3], [cr4], [cr5], [cr6], [cr7], [cr8], [cr9])
GO
巨大表应用此分区方案，数据按天划分，分布于[cr0]……[cr9]这10个文件组。另外，按照巨大表相同的结构，分别在这10个文件组中建立一个不分区的表：[Temp_0]……[Temp_9]，每个文件组上有一个。
分区切换步骤：
1）    分区切换，将最老分区数据切换到同一文件组的单表（秒杀，几百万条记录瞬时完成，不用1秒）
@day7ago    SMALLDATETIME --当前日期的7天前
@day2later SMALLDATETIME --当前日期的2天后
ALTER TABLE [BigTable] SWITCH PARTITION $PARTITION.CrPFN(@day7ago) TO [Temp_0];
2）    将最老分区与次老分区合并，也就是8天前分区与7天前分区合并。由于8天前分区的数据已经切换掉，为空，所以合并也瞬时完成。
ALTER PARTITION FUNCTION CrPFN() MERGE RANGE (CONVERT(VARCHAR(10),@day7ago,120));
3）    修改分区方案，将原先最老分区对应的文件组插入到分区队列末尾，以重新使用。
ALTER PARTITION SCHEME [CrSCM]    NEXT USED [cr0];
4）    拆分最新日期的分区，边界值推进到当前日期的2天后
ALTER PARTITION FUNCTION CrPFN() SPLIT RANGE (CONVERT(VARCHAR(10),@day2later,120));
因为最新日期所在分区还没有数据（因为是未来日期），所以拆分起来也是瞬间完成。这也正是我不只设置7个分区，而是10个分区，永远将边界值往未来移两天的原因。
5）    将移到单表的数据归并入历史库
如何归并，是下面一个话题。
6）    清空单表
TRUNCATE TABLE [Temp_0];
总结：
1）    永远只有10个分区，循环使用。最老的分区被合并出局后，马上又插到分区队列末尾。
2）    关键是如何找出分区位于哪个文件组，然后才能知道应该切换给哪个单表，才能放在作业里自动执行。这个方法详见拙作：
# [查看分区在哪个文件组](http://blog.csdn.net/leftfist/archive/2010/08/04/5788814.aspx)
3）    分区表与单表务必对齐。
首先是结构一定要一致。比如字段类型、是否可以为null，等等。
其次是索引。分区表有什么索引，单表也要有什么索引。
注意分区表里面的索引，如果要应用本文说的方法，无论聚集索引还是非聚集索引，必须全部都是分区的。其分区方案对应的分区函数应该跟表的分区函数同为一个，分区方案可以不同。
分区表和单表，包括索引，是否为压缩也必须一致。
2、    bcp + BULK INSERT，将数据导入历史库
用bcp将数据库导出到文本文件，然后用BULK INSERT。
    DECLARE @shell VARCHAR(500);
    SET @shell = 'bcp [myDb].dbo.[' + @Table + '] out c:/data.txt -T -n';
    EXEC xp_cmdshell @shell;
    BULK INSERT [History].dbo.[Table1] FROM 'c:/data.txt';
EXEC xp_cmdshell 'del c:/data.txt';
总结：
1）    bcp导出很快。400百万条记录20秒内完成。
2）    BULK INSERT号称导入数据很快，但在我这里却没有见到效果。如果目标表是空表，400百万条数据可在1分半钟内导入成功。但我的历史表超过5亿条记录，里面两个索引，超过20分钟不见反应，比直接INSERT还要慢（INSERT大约17分钟），我失去了耐心，直接按取消了。
3）    其实BULK INSERT运行的这20分钟里，已经插入了100多万条，但这样的速度比直接用INSERT还要慢，让人无法接受。我估计是索引的缘故，如果将索引全部删掉，应该会快吧？但重建的时间会更长。究竟这个BULK INSERT是个什么机制，在这里会比INSERT更慢？有个网友说它会在tempDB里处理，然后再插进源表，但我在运行期间，观察了一下系统的tempDB，发现它的文件没有变化。
4）    失败。请高手赐教。
