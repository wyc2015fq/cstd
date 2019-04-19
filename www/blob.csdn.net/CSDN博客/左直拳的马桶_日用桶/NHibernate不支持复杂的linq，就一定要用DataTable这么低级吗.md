# NHibernate不支持复杂的linq，就一定要用DataTable这么低级吗 - 左直拳的马桶_日用桶 - CSDN博客
2016年08月26日 20:00:22[左直拳](https://me.csdn.net/leftfist)阅读数：1563
有些linq，好不容易写出来，正想扬眉吐屁一番，不料用NHibernate一执行，却报错，说是不支持，我靠。
只好捏着鼻子写一大段sql，交给它。这种直接执行SQL的情况，我看我同事写的，全部都是返回DataTable类型。 
DataTable类型，我以前用得太多了。直接行、列这样子处理，感觉就是在直接操作数据库，有点不符合当代潮流。太low了。
怎么ORM起来呢？其实也可以这样的：
```
string sql = @"select p.*,l.message,l.sendorderid from targetunit u
 join targetperson p on u.id=p.unitid
 join (select u.type,l.sendorderid,o.message from sendlist l
 join targetUnit u on l.type=u.type
 join SendOrder o on l.sendorderid=o.id) l on u.type=l.type
 where not exists(select 1 from sendlog g where g.sendorderid=l.sendorderid and g.mobile=p.mobile and g.ok=1)";
DataSet ds = personRepository.QueryDataSet(sql, null);
return ds.Tables[0].AsEnumerable().Select(dataRow => new SendListVM
{
    ID = Convert.ToInt64(dataRow[0]),
    NAME = dataRow[1].ToString(),
    MOBILE = dataRow[2].ToString(),
    UNITID = Convert.ToInt64(dataRow[3]),
    POSITION = dataRow[4].ToString(),
    MESSAGE = dataRow[5].ToString(),
    SENDORDERID = Convert.ToInt64(dataRow[6]),
}).OrderBy( m => m.SENDORDERID).ToList();
```
