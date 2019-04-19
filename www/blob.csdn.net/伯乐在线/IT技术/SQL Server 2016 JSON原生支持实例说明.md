# SQL Server 2016 JSON原生支持实例说明 - 文章 - 伯乐在线
原文出处： [DB乐之者](http://www.cnblogs.com/wenBlog/p/5817842.html)
# 背景
Microsoft SQL Server 对于数据平台的开发者来说越来越友好。比如已经原生支持XML很多年了，在这个趋势下，如今也能在SQLServer2016中使用内置的JSON。尤其对于一些大数据很数据接口的解析环节来说这显得非常有价值。与我们现在所做比如在SQL中使用CLR或者自定义的函数来解析JSON相比较，新的内置JSON会大大提高性能，同时优化了编程以及增删查改等方法。
那么是否意味着我们可以丢弃XML，然后开始使用JSON？当然不是，这取决于数据输出处理的目的。如果有一个外部的通过XML与外部交互数据的服务并且内外的架构是一致的，那么应该是使用XML数据类型以及原生的函数。如果是针对微型服务架构或者动态元数据和数据存储，那么久应该利用最新的JSON函数。
## 实例
当使用查询这些已经有固定架构的JSON的数据表时，使用“[FOR JSON](https://msdn.microsoft.com/en-us/library/dn921882.aspx)” 提示在你的T-SQL脚本后面，用这种方式以便于格式化输出。一下实例我使用了SQLServer 2016 Worldwide Importers sample database，可以在GitHub上直接下载下来（[下载地址](https://github.com/Microsoft/sql-server-samples/releases/)）。看一下视图**Website.customers。**我们查询一个数据并格式化输出JSON格式：


MySQL
```
SELECT [CustomerID]
      ,[CustomerName]
      ,[CustomerCategoryName]
      ,[PrimaryContact]
      ,[AlternateContact]
      ,[PhoneNumber]
      ,[FaxNumber]
      ,[BuyingGroupName]
      ,[WebsiteURL]
      ,[DeliveryMethod]
      ,[CityName]
      
 ,DeliveryLocation.ToString() as DeliveryLocation
      ,[DeliveryRun]
      ,[RunPosition]
  FROM [WideWorldImporters].[Website].[Customers]
  WHERE CustomerID=1
  FOR JSON AUTO
```
请注意我们有一个地理数据类型列（DeliveryLocation），这需要引入两个重要的变通方案（标黄）:
首先，需要转换一个string字符，否则就会报错:
`FOR JSON cannot serialize CLR objects. Cast CLR types explicitly into one of the supported types in FOR JSON queries.`
其次，JSON采用键值对的语法因此必须指定一个别名来转换数据，如果失败会出现下面的错误:
`Column expressions and data sources without names or aliases cannot be formatted as JSON text using FOR JSON clause. Add alias to the unnamed column or table.`
确认了这些，改写的格式化输出如下:


MySQL
```
[
    {
        "CustomerID": 1,
        "CustomerName": "Tailspin Toys (Head Office)",
        "CustomerCategoryName": "Novelty Shop",
        "PrimaryContact": "Waldemar Fisar",
        "AlternateContact": "Laimonis Berzins",
        "PhoneNumber": "(308) 555-0100",
        "FaxNumber": "(308) 555-0101",
        "BuyingGroupName": "Tailspin Toys",
        "WebsiteURL": "http://www.tailspintoys.com",
        "DeliveryMethod": "Delivery Van",
        "CityName": "Lisco",
        "DeliveryLocation": "POINT (-102.6201979 41.4972022)",
        "DeliveryRun": "",
        "RunPosition": ""
    }
]
```
当然也可以使用JSON作为输入型DML语句，例如INSERT/UPDATE/DELETE 语句中使用“[OPENJSON](https://blogs.msdn.microsoft.com/sqlserverstorageengine/2015/09/22/openjson-the-easiest-way-to-import-json-text-into-table/)”。因此可以在所有的数据操作上加入JSON提示。
如果不了解数据结构或者想让其更加灵活，那么可以将数据存储为一个JSON格式的字符类型，改列的类型可以使NVARCHAR 类型。**Application.People** 表中的**CustomFields** 列就是典型这种情况。可以用如下语句看一下表格格式这个列的内容:


MySQL
```
declare json nvarchar(max)
SELECT @json=[CustomFields]
FROM [WideWorldImporters].[Application].[People]
where PersonID=8
select * from openjson(json)
```
结果集在表格结果中的显示:
![](http://jbcdn2.b0.upaiyun.com/2016/09/a3d4e0f36a7a8d52235877edf1048b29.png)
用另一种方式来查询这条记录，前提是需要知道在JSON数据结构和关键的名字，使用[JSON_VALUE](https://msdn.microsoft.com/en-us/library/dn921898.aspx) 和[JSON_QUERY](https://msdn.microsoft.com/en-us/library/dn921884.aspx) 函数:


MySQL
```
SELECT
       JSON_QUERY([CustomFields],'$.OtherLanguages') as OtherLanguages,
       JSON_VALUE([CustomFields],'$.HireDate') as HireDate,
       JSON_VALUE([CustomFields],'$.Title') as Title,
       JSON_VALUE([CustomFields],'$.PrimarySalesTerritory') as PrimarySalesTerritory,
       JSON_VALUE([CustomFields],'$.CommissionRate') as CommissionRate
  FROM [WideWorldImporters].[Application].[People]
  where PersonID=8
```
在表格结果集中展示表格格式的结果:
![](http://jbcdn2.b0.upaiyun.com/2016/09/c3d0623c02bbb151cbe39613c0caef36.png)
这个地方最关心就是查询条件和添加索引。设想一下我们打算去查询所有2011年以后雇佣的人，你可以运行下面的查询语句:


MySQL
```
SELECT personID,fullName,JSON_VALUE(CustomFields,'$.HireDate') as hireDate
FROM [WideWorldImporters].[Application].[People]
where IsEmployee=1
and year(cast(JSON_VALUE(CustomFields,'$.HireDate') as date))>2011
```
切记JSON_VALUE 返回一个单一的文本值（nvarchar(4000)）。需要转换返回值到一个时间字段中，然后分离年来筛选查询条件。实际执行计划如下:
![](http://jbcdn2.b0.upaiyun.com/2016/09/22a12a79e0bcfa22b3ed10de0375ac1a.png)
为了验证如何对JSON内容创建索引，需要创建一个计算列。为了举例说明，Application.People 表标记版本，并且加入计算列，当系统版本为ON的时候不支持。我们这里使用Sales.Invoices表，其中**ReturnedDeliveryData** 中插入json数据。接下来获取数据，感受一下:


MySQL
```
SELECT TOP 100 [InvoiceID]
      ,[CustomerID]
      ,JSON_QUERY([ReturnedDeliveryData],'$.Events')
  FROM [WideWorldImporters].[Sales].[Invoices]
```
发现结果集第一个event都是“Ready for collection”:
![](http://jbcdn2.b0.upaiyun.com/2016/09/b76e27333ee19c979b647b7c9435c830.png)
然后获取2016年3月的发票数据:


MySQL
```
SELECT [InvoiceID]
      ,[CustomerID]
      ,CONVERT(datetime, CONVERT(varchar,JSON_VALUE([ReturnedDeliveryData],'$.Events[0].EventTime')),126)
  FROM [WideWorldImporters].[Sales].[Invoices]
  WHERE CONVERT(datetime, CONVERT(varchar,JSON_VALUE([ReturnedDeliveryData],'$.Events[0].EventTime')),126)
       BETWEEN '20160301' AND '20160331'
```
实际执行计划如下:
![](http://jbcdn2.b0.upaiyun.com/2016/09/2296d80c7c31b180d1b6b7621bee6cc4.png)
加入一个计算列叫做“ReadyDate”, 准备好集合表达式的结果:


MySQL
```
ALTER TABLE [WideWorldImporters].[Sales].[Invoices]
ADD ReadyDate AS CONVERT(datetime, CONVERT(varchar,JSON_VALUE([ReturnedDeliveryData],'$.Events[0].EventTime')),126)
```
之后，重新执行查询，但是使用新的计算列作为条件:


MySQL
```
SELECT [InvoiceID]
      ,[CustomerID]
      ,ReadyDate
  FROM [WideWorldImporters].[Sales].[Invoices]
  WHERE ReadyDate BETWEEN '20160301' AND '20160331'
```
执行计划是一样的，除了SSMS建议的缺失索引:
![](http://jbcdn2.b0.upaiyun.com/2016/09/f372bccf6c3fca27cebe647195194fa6.png)
因此，根据建议在计算列上建立索引来帮助查询，建立索引如下:


MySQL
```
/*
The Query Processor estimates that implementing the following index could improve the query cost by 99.272%.
*/
CREATE NONCLUSTERED INDEX IX_Invoices_ReadyDate
ON [Sales].[Invoices] ([ReadyDate])
INCLUDE ([InvoiceID],[CustomerID])
GO
```
我们重新执行查询验证执行计划:
![](http://jbcdn2.b0.upaiyun.com/2016/09/8602c543f0478d66920c9772982fcb43.png)
有了索引之后，大大提升了性能，并且查询JSON的速度和表列是一样快的。
## 总结:
本篇通过对SQL2016 中的新增的内置JSON进行了简单介绍，主要有如下要点：
- JSON能在SQLServer2016中高效的使用，但是JSON并不是原生数据类型;
- 如果使用JSON格式必须为输出结果是表达式的提供别名;
- JSON_VALUE 和 JSON_QUERY  函数转移和获取Varchar格式的数据，因此必须将数据转译成你需要的类型。
- 在计算列的帮助下查询JSON可以使用索引进行优化。
