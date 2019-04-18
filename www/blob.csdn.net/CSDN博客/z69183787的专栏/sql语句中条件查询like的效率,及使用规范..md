# sql语句中条件查询like的效率,及使用规范. - z69183787的专栏 - CSDN博客
2013年08月29日 13:37:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10144
引用“
我在做数据库查询的时候，发现了一个奇怪的问题，在使用like关键字的时候，速度其慢，效率极低。不明白是什么原因，请大家讲解一下，为什么like慢，而其他的两个快。3个查询不同的地方用红色做了标识。
我数据表中的数据量如下FDAEnterprise 数据513条，TYHJL里面的数据为22516条。
我有如下一个查询语句：查询所花费的时间是13秒。
  select top 15  
FDAEnterprise.cName,FDAEnterprise.cEnterpriseCode,FDAEnterprise.cAddress,count(*) as NoteNum  
from TYHJL join FDAEnterprise on  
TYHJL.cEnterpriseCode=FDAEnterprise.cEnterpriseCode   
where  
FDAEnterprise.cName like '%大药房%' 
and  
yh_xq<='2050-01-01'  
and  
Yh_datetime > '1900-01-01'  
and  
Yh_datetime <'2050-01-01'
  group by  
FDAEnterprise.cName ,FDAEnterprise.cEnterpriseCode,FDAEnterprise.cAddress   
  order by  
FDAEnterprise.cEnterpriseCode
后来经过修改，改为如下语句，查询所花时间大概是0.25秒
select top 15  
FDAEnterprise.cName,FDAEnterprise.cEnterpriseCode,FDAEnterprise.cAddress,count(*) as NoteNum  
from TYHJL join FDAEnterprise on  
TYHJL.cEnterpriseCode=FDAEnterprise.cEnterpriseCode   
where
(select cName from FDAEnterprise where TYHJL.cEnterpriseCode=FDAEnterprise.cEnterpriseCode)like '%大药房%' 
and  
yh_xq<='2050-01-01'  
and  
Yh_datetime > '1900-01-01'  
and  
Yh_datetime <'2050-01-01'
group by  
FDAEnterprise.cName ,FDAEnterprise.cEnterpriseCode,FDAEnterprise.cAddress   
order by  
FDAEnterprise.cEnterpriseCode
最后我又进行了修改，改为如下的语句，查询耗时大概0.14秒
select top 15  
FDAEnterprise.cName,FDAEnterprise.cEnterpriseCode,FDAEnterprise.cAddress,count(*) as NoteNum  
from TYHJL join FDAEnterprise on  
TYHJL.cEnterpriseCode=FDAEnterprise.cEnterpriseCode   
where  
charindex('大药房',FDAEnterprise.cName)<>0 
and  
yh_xq<='2050-01-01'  
and  
Yh_datetime > '1900-01-01'  
and  
Yh_datetime <'2050-01-01'
group by
FDAEnterprise.cName ,FDAEnterprise.cEnterpriseCode,FDAEnterprise.cAddress   
order by  
FDAEnterprise.cEnterpriseCode
”
**第一种like的内容如果前后都使用了%的话，是无法使用索引的，这应该是慢的主要原因第二种先select了一次，相当于用hash索引的方式过滤了一遍，结果集小了很多第三种使用了文本索引，所以速度比较快在sql语句里，"like"，"not in","not exists"这几个是比较慢的，原因在于它们的实现机制。例如，如果我们在一个“字符串”里找查“另一个字符串”，而且这个字符串有通配符(%)，不管是用顺序查找还是二分查找，效率都不见得太高。like比较符，如果用的是like "大药房%",这种方式会用索引扫描，但如果是 like "%大药房%",这种方式会用表扫描，该字段上的索引不起作用，速度会非常慢，如果你的记录很多，该字段很大的话。**
