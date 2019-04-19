# Linq中的group by多表多字段 - 左直拳的马桶_日用桶 - CSDN博客
2015年03月11日 11:02:44[左直拳](https://me.csdn.net/leftfist)阅读数：27645
按我的理解，linq与sql对应，只不过，一个对应在代码里，一个对应在数据库里。之所以要使用linq，是让分工越来越精细的程序员可以基本不管数据库这一块，只须关注代码即可。
不过，linq与sql还是会有一点区别。比如这个group by。
在sql中，如果有group by，那么select的字段只能包含分组内容，或者count、sum、avg这些统计字段。
但在linq里面，是：group 你想要什么字段 by 分组字段
比如：
```
var q =
    from p in db.Products
    group p by p.CategoryID into g
    select g;
```
但这只是最简单的情形，玩具级别。实际应用中，多表多字段参与分组比较常见：
```
from a in TableA
	join b in TableB on a.Id equals b.aId
	where ((b.Type == 1 || b.Type == 2 || b.Type == 5) && b.State == 1)
	group new { a.Id, b.Name,b,CreateDate } by new { a.Id, b.Name } into g
	select (new Class1 { Id = g.Key.Id, Name = g.Key.Name ?? "" });
class Class1
{
	public int Id { get; set; }
	publid string Name { get; set; }
}
```
参考文章：
[http://www.cnblogs.com/aspnet2008/archive/2008/12/23/1358494.html](http://www.cnblogs.com/aspnet2008/archive/2008/12/23/1358494.html)
