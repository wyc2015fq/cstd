# Linq入门 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月06日 00:40:11[左直拳](https://me.csdn.net/leftfist)阅读数：1492
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
看起来，Linq是.NET独创的，java里面，或者其他开发语言里面，似乎没有这个东西。微软独创的东西，命运都不大好说，也许不久也会被微软自己抛弃。
但是目前来讲，年轻的.NET程序员写出来的代码，到处都是Linq，Lambda表达式。看着这些代码，我有一种生疏感，敬畏之心油然而生，难道自己真的是老了吗？
原因是自己对Linq不熟。
但其实，读了网上一篇文章以后，感觉比较好理解了：
[http://www.cnblogs.com/liulun/archive/2013/02/26/2909985.html](http://www.cnblogs.com/liulun/archive/2013/02/26/2909985.html)
读这篇文章，最大的感悟是，Linq 与 接口IEnumerable有很大关系：
*IEnumerable接口是LINQ特性的核心接口只有实现了IEnumerable接口的集合才能执行相关的LINQ操作，比如select,where等*
也就是说，可linq操作是支持遍历的。linq操作，本质上，就是个遍历操作。
以前对一个集合或者数组，我们要从中找出一些元素，就要用循环，for、while、或者foreach。那现在有了linq以后，可以写得更简洁一点。仅此而已。但是，现在的程序员，因为分工愈来愈精细，对数据库操作不甚了解，sql估计懒得写，就全部ORM，linq起来。
老实说，LINQ 跟 SQL 真的比较像，最显著的区别，只是select部分从开头移到了尾部而已。
from v in arr where v > 3 select v
以下虚线里的内容为抄自上面那篇参考文章：
======================================================================
先看一段伪代码：　　　　　　
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42439921#)[copy](http://blog.csdn.net/leftfist/article/details/42439921#)
- from [type] id in source  
- [join [type] id in source on expr equals expr [into subGroup]]  
- [from [type] id in source | let id = expr | where condition]  
- [orderby ordering,ordering,ordering...]  
- select expr | group expr by key  
- [into id query]  
<1>第一行的解释：
type是可选的，
id是集合中的一项，
source是一个集合，
如果集合中的类型与type指定的类型不同则导致强制转化
<2>第二行的解释：
一个查询表达式中可以有0个或多个join子句，
这里的source可以不等于第一句中的source
expr可以是一个表达式
[into subGroup] subGroup是一个中间变量，
它继承自IGrouping，代表一个分组，也就是说“一对多”里的“多”
可以通过这个变量得到这一组包含的对象个数，以及这一组对象的键
比如：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42439921#)[copy](http://blog.csdn.net/leftfist/article/details/42439921#)
- from c in db.Customers  
-         　　　　join o in db.Orders on c.CustomerID  
-         　　　　equals o.CustomerID into orders  
-         　　　　select new
-         　　　　{  
-             　　　　c.ContactName,  
-             　　　　OrderCount = orders.Count()  
-         　　　　};  
- 
<3>第三行的解释：　　　　　
一个查询表达式中可以有1个或多个from子句
一个查询表达式中可以有0个或多个let子句，let子句可以创建一个临时变量
比如：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42440225#)[copy](http://blog.csdn.net/leftfist/article/details/42440225#)
- from u in users  
- 　let number = Int32.Parse(u.Username.Substring(u.Username.Length - 1))  
- 　where u.ID < 9 && number % 2 == 0  
- 　select u  
一个查询表达式中可以有0个或多个where子句，where子句可以指定查询条件
<4>第四行的解释：
一个查询表达式可以有0个或多个排序方式
每个排序方式以逗号分割
<5>第五行的解释：
一个查询表达式必须以select或者group by结束
select后跟要检索的内容
group by 是对检索的内容进行分组
比如：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42440225#)[copy](http://blog.csdn.net/leftfist/article/details/42440225#)
- from p in db.Products    
- group p by p.CategoryID into g    
- select new {  g.Key, NumProducts = g.Count()};   
<6>第六行的解释：
最后一个into子句起到的作用是
将前面语句的结果作为后面语句操作的数据源
比如：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42440225#)[copy](http://blog.csdn.net/leftfist/article/details/42440225#)
- from p in db.Employees  
- 　select new
- 　{  
-  　　　　LastName = p.LastName,  
-  　　　　TitleOfCourtesy = p.TitleOfCourtesy  
- 　} into EmployeesList  
- 　orderby EmployeesList.TitleOfCourtesy ascending  
- 　select EmployeesList;  
=====================================================================
请观看一些代码：
示例1：
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42439579#)[copy](http://blog.csdn.net/leftfist/article/details/42439579#)
- var query = (from a in _DbContext.Set<Meta_View>()//_DbContext.Set<Meta_View>()是一个List对象，别管它
-                     where a.ProjectID == projectId && a.Creator == account  
-                     select (new ViewTag  
-                     {  
-                         ViewId = a.ID  
-                         ,Name = a.Name  
-                         ,IsValid = a.IsValid  
-                         ,Seq = a.Seq  
-                         ,ChangeType = (byte)ViewTag.EChangeType.NoChanged  
-                     }));  
- return query.ToList<ViewTag>();  
示例2
**[csharp]**[view plain](http://blog.csdn.net/leftfist/article/details/42439579#)[copy](http://blog.csdn.net/leftfist/article/details/42439579#)
- IEnumerable<Meta_View> dbViews = _DbContext.Set<Meta_View>()  
-     .Where(m => m.ProjectID.Equals(projectId) && m.Creator.Equals(account));  
