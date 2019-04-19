# SQL SERVER的排名函数 - 左直拳的马桶_日用桶 - CSDN博客
2011年02月25日 10:50:00[左直拳](https://me.csdn.net/leftfist)阅读数：1276
ROW_NUMBER() :给整体结果集中的行标明序号，依次加1
RANK():先分组（也可以不分组），然后每组的行各自标明序号，依次加1，假如该组中排序列的值相同，就会有一些行拥有相同的序号，但接下来的行的序号会不连贯。比如有两行的序号是并列第1，则第3行的序号是3而不是2。
DENSE_RANK():接近RANK()，区别是虽然也有相同序号，但排名是连贯的。比如有两行的序号是并列第1，则第3行的序号是2而不是3。
NTILE(n):将结果集分成n等份，然后每等份里面的行标明序号，同一份里的序号都是相同的（比如说，分成4等份，份1里所有行的序号都是1，份2里所有行的序号都是2，以此类推）；或者先分组，然后每组分成n等份，等份里面再标明序号。（什么情况下会用到呢？）
USE AdventureWorks;
GO
SELECT c.FirstName, c.LastName
    ,ROW_NUMBER()
 OVER (ORDER BY a.PostalCode) AS 'Row Number
'
    ,RANK()
 OVER (ORDER BY a.PostalCode) AS 'Rank
'
    ,DENSE_RANK()
 OVER (ORDER BY a.PostalCode) AS 'Dense Rank'
    ,NTILE(4)
 OVER (ORDER BY a.PostalCode) AS 'Quartile'
    ,s.SalesYTD, a.PostalCode
FROM Sales.SalesPerson s 
    INNER JOIN Person.Contact c ON s.SalesPersonID = c.ContactID
    INNER JOIN Person.Address a ON a.AddressID = c.ContactID
WHERE TerritoryID IS NOT NULL AND SalesYTD <> 0;|FirstName|LastName|Row Number|Rank|Dense Rank|Quartile|SalesYTD|PostalCode|
|----|----|----|----|----|----|----|----|
|Maciej|Dusza|1|1|1|1|4557045|98027|
|Shelley|Dyck|2|1|1|1|5200475|98027|
|Linda|Ecoffey|3|1|1|1|3857164|98027|
|Carla|Eldridge|4|1|1|1|1764939|98027|
|Carol|Elliott|5|1|1|2|2811013|98027|
|Jauna|Elson|6|6|2|2|3018725|98055|
|Michael|Emanuel|7|6|2|2|3189356|98055|
|Terry|Eminhizer|8|6|2|3|3587378|98055|
|Gail|Erickson|9|6|2|3|5015682|98055|
|Mark|Erickson|10|6|2|3|3827950|98055|
|Martha|Espinoza|11|6|2|4|1931620|98055|
|Janeth|Esteves|12|6|2|4|2241204|98055|
|Twanna|Evans|13|6|2|4|1758386|98055|
