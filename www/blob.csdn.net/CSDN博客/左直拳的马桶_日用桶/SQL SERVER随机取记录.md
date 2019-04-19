# SQL SERVER随机取记录 - 左直拳的马桶_日用桶 - CSDN博客
2012年04月24日 17:06:39[左直拳](https://me.csdn.net/leftfist)阅读数：1230
SQL SERVER里实现随机取记录可这样写：
select top 10 
**newid()** as row, productid, productname
from Products
order by row
参考文章
[http://www.builder.com.cn/2006/0724/301184.shtml](http://www.builder.com.cn/2006/0724/301184.shtml)
