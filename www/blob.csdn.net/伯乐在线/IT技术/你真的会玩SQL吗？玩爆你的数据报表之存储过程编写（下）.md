# 你真的会玩SQL吗？玩爆你的数据报表之存储过程编写（下） - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [欢醉](http://www.jobbole.com/members/huanzui2336) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
上一篇 [你真的会玩SQL吗？玩爆你的数据报表之存储过程编写（上）](http://blog.jobbole.com/95227/) 已经讲到了列转行的数据。
对于留下的作业不知道有没有思路？
这里接下来讲怎么做：
从表#tempSaleDtl2**行转列**，按**类型聚合** 求出每个产品每个类型(面积、金额……)的合计 放入表#tempSaleDtl3


MySQL
```
SELECT ProductGUID,type,typecode,
    MAX(CASE YearMonth WHEN '9999-13' THEN val ELSE 0 END) AS '项目合计',
    MAX(CASE YearMonth WHEN @Year+'-00' THEN val ELSE 0 END) AS '以前年度合计',
    MAX(CASE YearMonth WHEN @Year+'-13' THEN val ELSE 0 END) AS '2011年合计',
    MAX(CASE YearMonth WHEN @Year+'-01' THEN val ELSE 0 END) AS '2011-01',
    MAX(CASE YearMonth WHEN @Year+'-02' THEN val ELSE 0 END) AS '2011-02',
    MAX(CASE YearMonth WHEN @Year+'-03' THEN val ELSE 0 END) AS '2011-03',
    MAX(CASE YearMonth WHEN @Year+'-04' THEN val ELSE 0 END) AS '2011-04',
    MAX(CASE YearMonth WHEN @Year+'-05' THEN val ELSE 0 END) AS '2011-05',
    MAX(CASE YearMonth WHEN @Year+'-06' THEN val ELSE 0 END) AS '2011-06',
    MAX(CASE YearMonth WHEN @Year+'-07' THEN val ELSE 0 END) AS '2011-07',
    MAX(CASE YearMonth WHEN @Year+'-08' THEN val ELSE 0 END) AS '2011-08',
    MAX(CASE YearMonth WHEN @Year+'-09' THEN val ELSE 0 END) AS '2011-09',
    MAX(CASE YearMonth WHEN @Year+'-10' THEN val ELSE 0 END) AS '2011-10',
    MAX(CASE YearMonth WHEN @Year+'-11' THEN val ELSE 0 END) AS '2011-11',
    MAX(CASE YearMonth WHEN @Year+'-12' THEN val ELSE 0 END) AS '2011-12',
    MAX(CASE YearMonth WHEN '9999-12' THEN val ELSE 0 END) AS '以后年度合计'
into #tempSaleDtl3
FROM #tempSaleDtl2
GROUP BY ProductGUID,type,typecode
ORDER BY ProductGUID,typecode
```
来看看这里的数据与前一步的对应关系：
![](http://jbcdn2.b0.upaiyun.com/2015/11/bb036d1f821caac1b5bd68d92730eb141.png)
（[查看大图](http://ww1.sinaimg.cn/large/5e4d414cgw1eyfrzb68izj213w0lhqlc.jpg)）
结果部分数据如图：
![](http://jbcdn2.b0.upaiyun.com/2015/11/3df9950443b3da3d44d0cd770d10544a.png)
（[查看大图](http://ww4.sinaimg.cn/large/5e4d414cgw1eyfrzby8jfj213u0edql3.jpg)）
其中列名为了显示方便，这里用了**2011，可以将列名变了year-01**……。
关于行转列的知识前系列也提过，不理解的请自觉前去复习。
小技巧是用到了**MAX聚合**，关于这点前面聚合的文章中有提到。整个的数据结构现在越来越趋近于最后的结果了，year~继续。
来看看结果中还差什么？**项目！**
这里用 “–”来代替数据：


MySQL
```
--从Project表中加入项目数据select ProjectName as orderCode,ProjectGUID,ProjectName,
        '--' AS '项目合计',
        '--' AS '以前年度合计',
        '--' AS '2011年合计',
        '--' AS '2011-01',
        '--' AS '2011-02',
        '--' AS '2011-03',
        '--' AS '2011-04',
        '--' AS '2011-05',
        '--' AS '2011-06',
        '--' AS '2011-07',
        '--' AS '2011-08',
        '--' AS '2011-09',
        '--' AS '2011-10',
        '--' AS '2011-11',
        '--' AS '2011-12',
        '--' AS '以后年度合计'
    from Project 
    where ProjectGUID=@ProjectGUID
```
还有**产品**：

```
select Project.ProjectName+'.'+a.ProductCode as orderCode,a.ProductGUID,a.ProductName,
        '--' AS '项目合计',
        '--' AS '以前年度合计',
        '--' AS '2011年合计',
        '--' AS '2011-01',
        '--' AS '2011-02',
        '--' AS '2011-03',
        '--' AS '2011-04',
        '--' AS '2011-05',
        '--' AS '2011-06',
        '--' AS '2011-07',
        '--' AS '2011-08',
        '--' AS '2011-09',
        '--' AS '2011-10',
        '--' AS '2011-11',
        '--' AS '2011-12',
        '--' AS '以后年度合计'
    from #product a
    left join Project on a.ProjectGUID=Project.ProjectGUID
```
部分数据如图：
![](http://jbcdn2.b0.upaiyun.com/2015/11/beff6c7b0d9f5727afacd5699a051bf0.png)
（[查看大图](http://ww1.sinaimg.cn/large/5e4d414cgw1eyfrzcgdlij213m0as165.jpg)）
最后从表#tempSaleDtl3 联接产品表#product 与项目表查询出最后的显示：


MySQL
```
--从产品表和Project表、#tempSaleDtl3中加入类型行数据
    select c.ProjectName+'.'+b.ProductCode+'.'+a.typecode as orderCode,a.ProductGUID, a.[type],
        cast(a.[项目合计] as varchar(20)),
        cast(a.[以前年度合计] as varchar(20)),
        cast(a.[2011年合计] as varchar(20)),
        cast(a.[2011-01] as varchar(20)),
        cast(a.[2011-02] as varchar(20)),
        cast(a.[2011-03] as varchar(20)),
        cast(a.[2011-04] as varchar(20)),
        cast(a.[2011-05] as varchar(20)),
        cast(a.[2011-06] as varchar(20)),
        cast(a.[2011-07] as varchar(20)),
        cast(a.[2011-08] as varchar(20)),
        cast(a.[2011-09] as varchar(20)),
        cast(a.[2011-10] as varchar(20)),
        cast(a.[2011-11] as varchar(20)),
        cast(a.[2011-12] as varchar(20)),
        cast(a.[以后年度合计] as varchar(20))
    from #tempSaleDtl3 a
    left join #product b on a.ProductGUID=b.ProductGUID
    left join Project c on b.ProjectGUID=c.ProjectGUID
```
以上三块数据加在一起，用**union ALL**，然后排个序就得到最开始效果图中的结果，再次来看看：
![](http://jbcdn2.b0.upaiyun.com/2015/11/548d2e4a4d69bc80d01e01d30c6bcd5e1.png)
（[查看大图](http://ww1.sinaimg.cn/large/5e4d414cgw1eyfrzdj9clj21430c9h15.jpg)）
其中注意orderCode，这里用到层级，前台显示时可以更方便显示层级关系。
最后完整的code:


MySQL
```
--从Project表中加入项目数据
select * from (
    select ProjectName as orderCode,ProjectGUID,ProjectName,
        '--' AS '项目合计',
        '--' AS '以前年度合计',
        '--' AS '2011年合计',
        '--' AS '2011-01',
        '--' AS '2011-02',
        '--' AS '2011-03',
        '--' AS '2011-04',
        '--' AS '2011-05',
        '--' AS '2011-06',
        '--' AS '2011-07',
        '--' AS '2011-08',
        '--' AS '2011-09',
        '--' AS '2011-10',
        '--' AS '2011-11',
        '--' AS '2011-12',
        '--' AS '以后年度合计'
    from Project 
    where ProjectGUID=@ProjectGUID
----项目1    8FA659C8-3DA9-4330-B277-9B517E67606D    项目1    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --
    union all
--从产品表和Project表中加入合计行数据
    select Project.ProjectName+'.'+a.ProductCode as orderCode,a.ProductGUID,a.ProductName,
        '--' AS '项目合计',
        '--' AS '以前年度合计',
        '--' AS '2011年合计',
        '--' AS '2011-01',
        '--' AS '2011-02',
        '--' AS '2011-03',
        '--' AS '2011-04',
        '--' AS '2011-05',
        '--' AS '2011-06',
        '--' AS '2011-07',
        '--' AS '2011-08',
        '--' AS '2011-09',
        '--' AS '2011-10',
        '--' AS '2011-11',
        '--' AS '2011-12',
        '--' AS '以后年度合计'
    from #product a
    left join Project on a.ProjectGUID=Project.ProjectGUID
----项目1    8FA659C8-3DA9-4330-B277-9B517E67606D    项目1    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --
----项目1.00    00000000-0000-0000-0000-000000000000    合计    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --
----项目1.产品1    18908255-DB67-4EA3-A231-8BB39D5B748B    产品1    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --
----项目1.产品2    EDB216A9-EBB8-4F2C-AE4E-0A989EC7A993    产品2    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --
----项目1.产品3    7040241F-5A66-4F17-AACA-7CBE2FB3BCB9    产品3    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --
----项目1.产品4    108778CD-47C0-4258-9CB6-1FBE90CDEBDA    产品4    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --
----项目1.产品5    43F7B9BA-EF91-4A38-A048-090179F33C9B    产品5    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --
    union ALL
    --从产品表和Project表、#tempSaleDtl3中加入类型行数据
    select c.ProjectName+'.'+b.ProductCode+'.'+a.typecode as orderCode,a.ProductGUID, a.[type],
        cast(a.[项目合计] as varchar(20)),
        cast(a.[以前年度合计] as varchar(20)),
        cast(a.[2011年合计] as varchar(20)),
        cast(a.[2011-01] as varchar(20)),
        cast(a.[2011-02] as varchar(20)),
        cast(a.[2011-03] as varchar(20)),
        cast(a.[2011-04] as varchar(20)),
        cast(a.[2011-05] as varchar(20)),
        cast(a.[2011-06] as varchar(20)),
        cast(a.[2011-07] as varchar(20)),
        cast(a.[2011-08] as varchar(20)),
        cast(a.[2011-09] as varchar(20)),
        cast(a.[2011-10] as varchar(20)),
        cast(a.[2011-11] as varchar(20)),
        cast(a.[2011-12] as varchar(20)),
        cast(a.[以后年度合计] as varchar(20))
    from #tempSaleDtl3 a
    left join #product b on a.ProductGUID=b.ProductGUID
    left join Project c on b.ProjectGUID=c.ProjectGUID
) t1
order by orderCode
```
最重要的重要的！！最后不要忘了删除临时表：


MySQL
```
drop table #product
drop table #TempAllSaleDtl
DROP TABLE #TempSaleDtl
DROP TABLE #tempSaleDtl2
DROP TABLE #tempSaleDtl3
DROP TABLE #ProductSaleArea
```
至此 数据报表系列已结束，以上也只是提供一下思路，再次提醒一下各位在作以上每一步数据处理时请用**数据集合的思维**来思考，可能对于一般人来说长篇幅的存储过程编写没有写过，文中涉及到的技巧和思路是值得借鉴的，其中不理解的可以回过头来将之前系列的各部分基础知识点复习一下，然后一步步将结果打印出来测试，在脑袋里留下个前一步的数据，然后再思考下一步的数据怎么处理。如果你能毫无鸭梨的完全写出来，那恭喜你，你已经进阶为小大师，将会迎娶白富美，走上人生巅峰……想想是不是有点小激动……
最终的SQL，其中两个参数 需要查询的项目ID @ProjectGUID和需要查询的年份 @Year 可以由外部传入，可以自己写个存储过程，在这里就不演示了：


MySQL
```
DBCC DROPCLEANBUFFERS 
DBCC FREEPROCCACHE 
--SET STATISTICS IO ON
--SET STATISTICS TIME ON
DECLARE @ProjectGUID UNIQUEIDENTIFIER
SET @ProjectGUID='8FA659C8-3DA9-4330-B277-9B517E67606D'
DECLARE @Year CHAR(4)
SET @Year='2011'
/*临时表说明
  #product：用项目过滤后，将“合计”作为一个产品的集合
  #TempAllSaleDtl：通过项目过滤后的销售明细，所有月的
  #ProductSaleArea：各个产品的总面积，用于计算比例
  #TempSaleDtl：通过日期过滤，且加工过后的销售明细，包括增加累积列，以前年度、以后年度、项目合计的记录
  #tempSaleDtl2：列转行后的数据集
  #tempSaleDtl3：行转列后的数据集
*/
select ProductGUID,ProductName,ProjectGUID,ProductCode into #product from(
    select ProductGUID,ProductName,ProjectGUID,ProductName as ProductCode from Product where ProjectGUID=@ProjectGUID
    union all
    select '00000000-0000-0000-0000-000000000000','合计',@ProjectGUID,'00' as ProductCode
) a
--SELECT * FROM #product
----查找项目所有产品的销售明细：#TempAllSaleDtl
SELECT ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice
INTO #TempAllSaleDtl 
FROM dbo.SaleDtl WHERE ProductGUID IN (
    SELECT ProductGUID FROM dbo.Product WHERE ProjectGUID=@ProjectGUID 
)
--SELECT * FROM #TempAllSaleDtl
--ORDER BY ProductGUID,YearMonth
--根据现有数据统计，向#TempAllSaleDtl添加总合计记录
insert into #TempAllSaleDtl(ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice)
select '00000000-0000-0000-0000-000000000000',YearMonth,SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea) 
from #TempAllSaleDtl
group by YearMonth
--SELECT * FROM #TempAllSaleDtl
--ORDER BY ProductGUID,YearMonth
--查找某年的销售明细：#TempSaleDtl
SELECT ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,
    SalePrice AS ljSaleArea,
    SalePrice AS blSaleArea,
    SalePrice AS ljSaleAmount
INTO #TempSaleDtl 
FROM #TempAllSaleDtl 
WHERE LEFT([YearMonth],4)=@Year 
--SELECT * FROM #TempSaleDtl
--ORDER BY ProductGUID,YearMonth
--获取项目各个产品的总销售面积：#ProductSaleArea
SELECT ProductGUID,SUM(SaleArea) AS all_SaleArea INTO #ProductSaleArea 
FROM #TempAllSaleDtl
GROUP BY ProductGUID
--SELECT * FROM #ProductSaleArea
--ORDER BY ProductGUID
--添加2011合计列的记录(本年度的各产品的所有面积、金额、均价总合计)
insert into #TempSaleDtl(
    ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,ljSaleArea, blSaleArea,ljSaleAmount)
select ProductGUID,@Year+'-13',SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea),0,0,0 
from #TempSaleDtl
group by ProductGUID
--SELECT * FROM #TempSaleDtl
--ORDER BY ProductGUID,YearMonth
--以前年度列记录（本年度以前的各产品的所有面积、金额、均价总合计 操作与上一步类似）
insert into #TempSaleDtl(
    ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,ljSaleArea, blSaleArea,ljSaleAmount)
select ProductGUID,@Year+'-00',SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea),0,0,0 
from #TempAllSaleDtl where YearMonth @Year+'-00'
group by ProductGUID
--SELECT * FROM #TempSaleDtl
--ORDER BY ProductGUID,YearMonth
--以后年度列记录（本年度以后的各产品的所有面积、金额、均价总合计 操作与上一步类似）
insert into #TempSaleDtl(
    ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,ljSaleArea, blSaleArea,ljSaleAmount)
select ProductGUID,'9999-12',SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea),0,0,0 
from #TempAllSaleDtl where YearMonth >cast((cast(@Year as int) +1) as CHAR(4))+'-00'
group by ProductGUID
--SELECT * FROM #TempSaleDtl
--ORDER BY ProductGUID,YearMonth
--项目合计列记录(各产品取所有的合计。与上面的区别在于没有添加 here YearMonth >cast((cast(@Year as int) +1) as CHAR(4))+'-00')
insert into #TempSaleDtl(
    ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,ljSaleArea, blSaleArea,ljSaleAmount)
select ProductGUID,'9999-13',SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea),0,0,0 
from #TempAllSaleDtl
group by ProductGUID
--SELECT * FROM #TempSaleDtl
--ORDER BY ProductGUID,YearMonth
--更新销售明细TempSaleDtl的累积销售面积、累积销售面积比例，累积销售金额
UPDATE #TempSaleDtl SET 
    ljSaleArea=b.sum_SaleArea,
    ljSaleAmount=b.sum_SaleAmount,
    blSaleArea=b.sum_SaleArea/c.all_SaleArea
FROM #TempSaleDtl
left JOIN (
    SELECT n.ProductGUID,n.YearMonth,SUM(m.SaleArea) AS sum_SaleArea,SUM(m.SaleAmount) AS sum_SaleAmount 
    FROM #TempAllSaleDtl m
    INNER JOIN #TempSaleDtl n ON m.YearMonthn.YearMonth AND m.ProductGUID=n.ProductGUID
    GROUP BY n.ProductGUID,n.YearMonth 
) b ON #TempSaleDtl.ProductGUID=b.ProductGUID AND #TempSaleDtl.YearMonth=b.YearMonth
LEFT JOIN #ProductSaleArea c ON c.ProductGUID=#TempSaleDtl.ProductGUID
--SELECT * FROM #TempSaleDtl
--ORDER BY ProductGUID,YearMonth
--列转行，转换后的表只有 产品、统计类型、日期，值4列；(每个产品对应的0-12、13 月对应的值)
SELECT * INTO #tempSaleDtl2 FROM (
    SELECT ProductGUID,'销售套数' AS type,'01' AS typecode,YearMonth,MAX(SaleNum) AS val FROM #TempSaleDtl
    GROUP BY ProductGUID,YearMonth
    UNION ALL
    SELECT ProductGUID,'销售面积' AS type,'02' AS typecode,YearMonth,MAX(SaleArea) AS val FROM #TempSaleDtl
    GROUP BY ProductGUID,YearMonth
    UNION ALL
    SELECT ProductGUID,'销售均价' AS type,'03' AS typecode,YearMonth,MAX(SalePrice) AS val FROM #TempSaleDtl
    GROUP BY ProductGUID,YearMonth
    UNION ALL
    SELECT ProductGUID,'销售金额' AS type,'04' AS typecode,YearMonth,MAX(SaleAmount) AS val FROM #TempSaleDtl
    GROUP BY ProductGUID,YearMonth
    UNION ALL
    SELECT ProductGUID,'累计销售面积' AS type, '05' AS typecode,YearMonth,SUM(ljSaleArea) FROM #TempSaleDtl
    GROUP BY ProductGUID,YearMonth
    UNION ALL
    SELECT ProductGUID,'累计销售面积比例' AS type, '06' AS typecode,YearMonth,SUM(blSaleArea) FROM #TempSaleDtl
    GROUP BY ProductGUID,YearMonth
    UNION ALL
    SELECT ProductGUID,'累计销售金额' AS type, '07' AS typecode,YearMonth,SUM(ljSaleAmount) FROM #TempSaleDtl
    GROUP BY ProductGUID,YearMonth
) t
--SELECT * FROM #tempSaleDtl2
--ORDER BY ProductGUID,yearmonth
--行转列，按日期聚合
SELECT ProductGUID,type,typecode,
    MAX(CASE YearMonth WHEN '9999-13' THEN val ELSE 0 END) AS '项目合计',
    MAX(CASE YearMonth WHEN @Year+'-00' THEN val ELSE 0 END) AS '以前年度合计',
    MAX(CASE YearMonth WHEN @Year+'-13' THEN val ELSE 0 END) AS '2011年合计',
    MAX(CASE YearMonth WHEN @Year+'-01' THEN val ELSE 0 END) AS '2011-01',
    MAX(CASE YearMonth WHEN @Year+'-02' THEN val ELSE 0 END) AS '2011-02',
    MAX(CASE YearMonth WHEN @Year+'-03' THEN val ELSE 0 END) AS '2011-03',
    MAX(CASE YearMonth WHEN @Year+'-04' THEN val ELSE 0 END) AS '2011-04',
    MAX(CASE YearMonth WHEN @Year+'-05' THEN val ELSE 0 END) AS '2011-05',
    MAX(CASE YearMonth WHEN @Year+'-06' THEN val ELSE 0 END) AS '2011-06',
    MAX(CASE YearMonth WHEN @Year+'-07' THEN val ELSE 0 END) AS '2011-07',
    MAX(CASE YearMonth WHEN @Year+'-08' THEN val ELSE 0 END) AS '2011-08',
    MAX(CASE YearMonth WHEN @Year+'-09' THEN val ELSE 0 END) AS '2011-09',
    MAX(CASE YearMonth WHEN @Year+'-10' THEN val ELSE 0 END) AS '2011-10',
    MAX(CASE YearMonth WHEN @Year+'-11' THEN val ELSE 0 END) AS '2011-11',
    MAX(CASE YearMonth WHEN @Year+'-12' THEN val ELSE 0 END) AS '2011-12',
    MAX(CASE YearMonth WHEN '9999-12' THEN val ELSE 0 END) AS '以后年度合计'
into #tempSaleDtl3
FROM #tempSaleDtl2
GROUP BY ProductGUID,type,typecode
ORDER BY ProductGUID,typecode
--SELECT * FROM #tempSaleDtl3
--ORDER BY ProductGUID,typecode
--从Project表中加入项目数据
select * from (
    select ProjectName as orderCode,ProjectGUID,ProjectName,
        '--' AS '项目合计',
        '--' AS '以前年度合计',
        '--' AS '2011年合计',
        '--' AS '2011-01',
        '--' AS '2011-02',
        '--' AS '2011-03',
        '--' AS '2011-04',
        '--' AS '2011-05',
        '--' AS '2011-06',
        '--' AS '2011-07',
        '--' AS '2011-08',
        '--' AS '2011-09',
        '--' AS '2011-10',
        '--' AS '2011-11',
        '--' AS '2011-12',
        '--' AS '以后年度合计'
    from Project 
    where ProjectGUID=@ProjectGUID
----项目1    8FA659C8-3DA9-4330-B277-9B517E67606D    项目1    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --    --
    union all
--从产品表和Project表中加入合计行数据
    select Project.ProjectName+'.'+a.ProductCode as orderCode,a.ProductGUID,a.ProductName,
        '--' AS '项目合计',
        '--' AS '以前年度合计',
        '--' AS '2011年合计',
        '--' AS '2011-01',
        '--' AS '2011-02',
        '--' AS '2011-03',
        '--' AS '2011-04',
        '--' AS '2011-05',
        '--' AS '2011-06',
        '--' AS '2011-07',
        '--' AS '2011-08',
        '--' AS '2011-09',
        '--' AS '2011-10',
        '--' AS '2011-11',
        '--' AS '2011-12',
        '--' AS '以后年度合计'
    from #product a
    left join Project on a.ProjectGUID=Project.ProjectGUID
    union ALL
    --从产品表和Project表、#tempSaleDtl3中加入类型行数据
    select c.ProjectName+'.'+b.ProductCode+'.'+a.typecode as orderCode,a.ProductGUID, a.[type],
        cast(a.[项目合计] as varchar(20)),
        cast(a.[以前年度合计] as varchar(20)),
        cast(a.[2011年合计] as varchar(20)),
        cast(a.[2011-01] as varchar(20)),
        cast(a.[2011-02] as varchar(20)),
        cast(a.[2011-03] as varchar(20)),
        cast(a.[2011-04] as varchar(20)),
        cast(a.[2011-05] as varchar(20)),
        cast(a.[2011-06] as varchar(20)),
        cast(a.[2011-07] as varchar(20)),
        cast(a.[2011-08] as varchar(20)),
        cast(a.[2011-09] as varchar(20)),
        cast(a.[2011-10] as varchar(20)),
        cast(a.[2011-11] as varchar(20)),
        cast(a.[2011-12] as varchar(20)),
        cast(a.[以后年度合计] as varchar(20))
    from #tempSaleDtl3 a
    left join #product b on a.ProductGUID=b.ProductGUID
    left join Project c on b.ProjectGUID=c.ProjectGUID
) t1
order by orderCode
drop table #product
drop table #TempAllSaleDtl
DROP TABLE #TempSaleDtl
DROP TABLE #tempSaleDtl2
DROP TABLE #tempSaleDtl3
DROP TABLE #ProductSaleArea
```
