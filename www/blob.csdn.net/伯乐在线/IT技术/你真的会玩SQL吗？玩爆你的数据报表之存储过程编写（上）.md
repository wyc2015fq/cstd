# 你真的会玩SQL吗？玩爆你的数据报表之存储过程编写（上） - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [欢醉](http://www.jobbole.com/members/huanzui2336) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
本系列之前的所有知识均为本章作准备，若看不懂本章可先回头温习下之前的系列。在之前还是先提一下中心思想：**SQL数据处理是集合思维，不要用逻辑思维来思考。**
在项目中经常需要从基础数据中提取**数据进行处理**后显示给老板或客户一些**报表**，这时数据量大，涉及表多，简单的表处理SQL无法满足，且需要重复使用，这时就要使用存储过程来处理大数据和**复杂的业务逻辑**。可能会有人提出在后台读出DataSet加载到内在中用逻辑来处理，但之前说过，**逻辑处理远远没有数据库集合处理快**，且占用了宝贵的内存，运用好可以减少网络流量、可提高数据库系统的安全性。
存储过程的编写最重要的是**思路清晰**，能知道自己想要的结果和写出的SQL能运行出什么样的结果，这需要基本功非常扎实，过程中会用到**联表查询、更新、临时表、数据聚合、行列转换、简单的函数**……等知识。
接下来不多说，直接上需求实例：
**需求：统计某个项目下各个产品的具体销售情况**
数据表：
![](http://jbcdn2.b0.upaiyun.com/2015/11/75b7adec0a12b08cd7e63c61ecd47549.png)
表结构如下：
![](http://jbcdn2.b0.upaiyun.com/2015/11/012756740915b42134485f0ffa2b00be.png)
由于数据库数据经过翻倍，数据库用例数据数量有点大，请要下载的可以在此下载，然后自行**还原数据库**，传送门：**链接：[http://pan.baidu.com/s/1o6MUnay](http://pan.baidu.com/s/1o6MUnay) 密码：swi4**
需要经过编写SQL显示数据库中销售记录是按**每个产品、每月一条记录**存储的，需要展示如下图。
总的显示**一个项目**，然后再按每个产品进行分组展示，每个产品有**7个属性行统计数据**，**再将所有产品分别进行合计，放到各自产品上面**。
![](http://jbcdn2.b0.upaiyun.com/2015/11/548d2e4a4d69bc80d01e01d30c6bcd5e.png)
（[查看大图](http://ww2.sinaimg.cn/large/5e4d414cgw1eyeju2meg6j21430c9h15.jpg)）
![](http://jbcdn2.b0.upaiyun.com/2015/11/ef83f5b2f036ea6146ab9603fc7cf76a.png)
（[查看大图](http://ww2.sinaimg.cn/large/5e4d414cgw1eyeju2xpsnj21ci0e8h55.jpg)）
**部分业务名词解释：**
横项 **总项目**数据= 产品1 + 产品2 + 产品3 + 产品4……
**“项目合计”列**：整个项目的，即以前年度合计+2011年合计+以后年度合计。如 产品1的“销售套数”的项目合计**5555** = 2011 前年度合计**3030** + 指定查询年2011年的**505** + 2011年以后的**2020 ，销售面积等以此类推。**
** “以前年度合计”列**：2011年以前所有年的合计。
**“以后年度合计”列**：2011年以后所有年的合计。
**“累计”**，统计所在周期+以前合计，如2011-01累计销售面积指2011-01年以前（**包括2011-01**）的销售面积之和。
**“累计销售比例”**：累计销售面积/项目总销售面积。如 产品1 的2011-02的“累计销售面积比例”**0.55** = 2011-02的“累计销售面积” **127200.00** / 产品1的项目合计的 “累计销售面积” **229900.00**
**“累计销售面积”**：到当前统计时间为止的所有销售面积，如产品1的2011-02月的“累计销售面积”**127200.00** = 2011-1月”累计销售的面积”**126400.00** + 2011-02月的“销售面积“ **800.00**，其它概念以此类推。
在这里有个特别的是** “累计销售面积”的”以后年度合计“**，如产品1的 2011年的 **”以后年度合计“** 的 “累计销售面积”**229900.00** = 2011年的  “累计销售面积”**146300.00** + 2011年后的 ”销售面积”** 83600.00 ，在这里你会发现229900和产品1的项目合计的“累计销售面积”相同，这个是正确的，项目合计中的累积面积并不等于 以前年+当年+以后年，请理解一下这个滑动聚合概念。**
其中需要传入两个参数：**项目ID**和**年份**
**下面来理一理整体的思路：**
如果只统计一个产品显示以上的数据该如何写呢？你可以先试一下。
**先将数据表拆分：**
**横向**：总项目合计+ 每个产品中每个子项（如 销售套数，销售面积等）+每个产品累计销售面积
**竖向**：项目、产品基本信息+当年每月各项累积+以前年度合计+以后年度合计+项目总合计
**核心数据表**：销售明细表，进行分析核心数据列：销售面积、销售均价、销售金额
**核心操作**：行、列互转，滑动聚合统计
**由于涉及到的知识过于庞大，流程过于繁多，导致整个篇幅过长，因此在这里分为上、下篇来讲解。**
**那来看看整个流程思路，先过滤数据：**
- 查找该项目的**所有产品**放进临时表A
- 查找该项目的所有产品的**销售明细**放进临时表B
- 从临时表B中查找**指定年的销售明细**放进临时表C
- 从表C统计**当前年度合计列**，各产品的所有面积、金额、均价总合计 放入表C
- 从表B统计**以前年度**的各产品的所有面积、金额、均价总合计 操作与上一步类似 放入表C
- 从表B统计**以后年度**的各产品的所有面积、金额、均价总合计 操作与上一步类似 放入表C
- 从表B统计各产品取**所有的合计** 放入表C
- 从表C统计**累积销售面积、累积销售面积比例，累积销售金额** 更新表C
- 从表C **列转行**，转换后的表只有 产品、统计类型、日期，值4列；(每个产品对应的0-12、13 月对应的值) 放入表D
- 从表D **行转列**，按类型聚合 求出每个产品每个类型(面积、金额……)的合计 放入表E
- 从表E 联接产品表A 与敷项目表查询出最后的显示
以上只是大概思路，过程中会讲一些技巧。


MySQL
```
/*临时表说明
  #product：用项目过滤后，将“合计”作为一个产品的集合
  #TempAllSaleDtl：通过项目过滤后的销售明细，所有月的
  #ProductSaleArea：各个产品的总面积，用于计算比例
  #TempSaleDtl：通过日期过滤，且加工过后的销售明细，包括增加累积列，以前年度、以后年度、项目合计的记录
  #tempSaleDtl2：列转行后的数据集
  #tempSaleDtl3：行转列后的数据集
*/
```
设置要查询的参数，以下示例为了好说明，特用**2011年作统一说明**：


MySQL
```
DECLARE @ProjectGUID UNIQUEIDENTIFIER
SET @ProjectGUID='8FA659C8-3DA9-4330-B277-9B517E67606D'--要查询的项目
DECLARE @Year CHAR(4)
SET @Year='2011'--要统一的年份
```
查找该项目的**所有产品**放进临时表#product，这里将“合计”作为一个产品的集合也插入产品表#product：


MySQL
```
select ProductGUID,ProductName,ProjectGUID,ProductCode into #product from(
    select ProductGUID,ProductName,ProjectGUID,ProductName as ProductCode from Product where ProjectGUID=@ProjectGUID
    union all
    select '00000000-0000-0000-0000-000000000000','合计',@ProjectGUID,'00' as ProductCode
) a
```
查找该项目的所有产品的**销售明细**放进临时表#TempAllSaleDtl，以作备用：


MySQL
```
SELECT ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice
INTO #TempAllSaleDtl 
FROM dbo.SaleDtl WHERE ProductGUID IN (
    SELECT ProductGUID FROM dbo.Product WHERE ProjectGUID=@ProjectGUID 
)
```
根据#TempAllSaleDtl现有数据统计，向#TempAllSaleDtl添加**总合计记录**


MySQL
```
--根据现有数据统计，向#TempAllSaleDtl添加总合计记录
insert into #TempAllSaleDtl(ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice)
select '00000000-0000-0000-0000-000000000000',YearMonth,SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea) 
from #TempAllSaleDtl
group by YearMonth
```
添加的部分数据如图：
![](http://jbcdn2.b0.upaiyun.com/2015/11/9b7c5b3d613ec724c46d60b53bc1d6a7.png)
从临时表#TempAllSaleDtl 中查找**指定年**的销售明细放进临时表#TempSaleDtl ，注意 这个时候就已经包含了 “合计”产品00的数据：


MySQL
```
--查找某年的销售明细：#TempSaleDtl
SELECT ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,
    SalePrice AS ljSaleArea,
    SalePrice AS blSaleArea,
    SalePrice AS ljSaleAmount
INTO #TempSaleDtl 
FROM #TempAllSaleDtl 
WHERE LEFT([YearMonth],4)=@Year
```
从#TempAllSaleDtl 中统计项目各个产品的总销售面积放入表：#ProductSaleArea，主要用作计算 **项目累计销售面积比例**


MySQL
```
--获取项目各个产品的总销售面积：#ProductSaleArea
SELECT ProductGUID,SUM(SaleArea) AS all_SaleArea INTO #ProductSaleArea 
FROM #TempAllSaleDtl
GROUP BY ProductGUID
```
![](http://jbcdn2.b0.upaiyun.com/2015/11/7032eb0f100d242f12c0dff8b9622342.png)
从表#TempSaleDtl 统计**当前年度合计列**，各产品的所有面积、金额、均价总合计 放入表#TempSaleDtl，注意这里 SUM(SaleAmount)/SUM(SaleArea) 计算销售单价：


MySQL
```
--添加2011合计列的记录(本年度的各产品的所有面积、金额、均价总合计)
insert into #TempSaleDtl(
    ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,ljSaleArea, blSaleArea,ljSaleAmount)
select ProductGUID,@Year+'-13',SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea),0,0,0 
from #TempSaleDtl
group by ProductGUID
```
部分数据如图，这里有个技巧是用**2011-13代表2011整个年份：**
![](http://jbcdn2.b0.upaiyun.com/2015/11/e427874477a965d5ceced462a5f4db70.png)
从表#TempAllSaleDtl 统计**以前年度**的**各产品**的所有面积、金额、均价总合计 操作与上一步类似 放入表#TempSaleDtl


MySQL
```
--以前年度列记录（本年度以前的各产品的所有面积、金额、均价总合计 操作与上一步类似）
insert into #TempSaleDtl(
    ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,ljSaleArea, blSaleArea,ljSaleAmount)
select ProductGUID,@Year+'-00',SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea),0,0,0 
from #TempAllSaleDtl where YearMonth @Year+'-00'
group by ProductGUID
```
部分数据如图，这里有个技巧是用**2011-00代表2011年以前年份：**
![](http://jbcdn2.b0.upaiyun.com/2015/11/ae2fca9fd0c5a47e809a882b64dfe95f.png)
从表#TempAllSaleDtl 统计**以后年度**的**各产品**的所有面积、金额、均价总合计 操作与上一步类似 放入表#TempSaleDtl


MySQL
```
--以后年度列记录（本年度以后的各产品的所有面积、金额、均价总合计 操作与上一步类似）
insert into #TempSaleDtl(
    ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,ljSaleArea, blSaleArea,ljSaleAmount)
select ProductGUID,'9999-12',SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea),0,0,0 
from #TempAllSaleDtl where YearMonth >cast((cast(@Year as int) +1) as CHAR(4))+'-00'
group by ProductGUID
```
部分数据如图，这里有个技巧是用**9999-12代表2011年以后年份：**
**![](http://jbcdn2.b0.upaiyun.com/2015/11/b23fdab25131fb0802e3505ee86107d5.png)**
从表#TempAllSaleDtl 统计**各产品取所有的合计** 放入表#TempSaleDtl


MySQL
```
--项目合计列记录(各产品取所有的合计。与上面的区别在于没有添加 here YearMonth >cast((cast(@Year as int) +1) as CHAR(4))+'-00')
insert into #TempSaleDtl(
    ProductGUID,YearMonth,SaleAmount,SaleArea,SaleNum,SalePrice,ljSaleArea, blSaleArea,ljSaleAmount)
select ProductGUID,'9999-13',SUM(SaleAmount),SUM(SaleArea),SUM(SaleNum),SUM(SaleAmount)/SUM(SaleArea),0,0,0 
from #TempAllSaleDtl
group by ProductGUID
```
部分数据如图，这里有个技巧是用**9999-13代表所有年份：**
**![](http://jbcdn2.b0.upaiyun.com/2015/11/bc50f1604d71cf431a874b0e453c27ee.png)**
以上数据中我们的 累积销售面积、累积销售面积比例，累积销售金额三项 之前都用0代替，现在我们来统计。
从表#TempSaleDtl 与 #TempAllSaleDtl统计**累积销售面积、累积销售面积比例，累积销售金额** 更新表#TempSaleDtl


MySQL
```
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
```
注意这里用到了**滑动累计聚合 m.YearMonth滑动累计聚合**请看**之前的**系列） ，利用子查询统计出 **每个产品到当月为止**的累计销售面积，累积销售金额，再联接 #ProductSaleArea 更新每个产品的累积销售面积比例。
部分数据如图：
![](http://jbcdn2.b0.upaiyun.com/2015/11/ee0b118fa840fa1f9ccbe82b5b6f7295.png)
从表#TempSaleDtl 列转行，转换后的表只有 产品、统计类型、日期，值4列；(每个产品对应的0-12、13 月对应的值) 放入表#tempSaleDtl2


MySQL
```
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
```
部分数据如图：
![](http://jbcdn2.b0.upaiyun.com/2015/11/e339a2ab4c02bc82055aeda169c5a2bb.png)
这里用到的**列转行**，共有7列，技巧为用**code来代表每个类型**，也用于显示排序，最终数据为每个产品每个月都有7行数据。这里是不是有了最终结果的雏形？
至此 **你真的会玩SQL吗？玩爆你的数据报表之存储过程编写 **上篇先写到这，对于看不懂的建议先建立数据库，然后自己一步步试着理下思路，试着写。
这里留个作业，**如何将上面的数据转化为下图中的格式呢?**
![](http://jbcdn2.b0.upaiyun.com/2015/11/bb036d1f821caac1b5bd68d92730eb14.png)
**敬请期待下篇，未完待续……**
