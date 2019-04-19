# 哇，union的优先级很高嘛 - 左直拳的马桶_日用桶 - CSDN博客
2013年06月15日 17:56:42[左直拳](https://me.csdn.net/leftfist)阅读数：2469
原来，union的优先级很高。比如
```
select stdname,[subject]='化学',化学 as score  from #student2 
union all 
select stdname,[subject]='数学',数学 as score  from #student2 
union all 
select stdname,[subject]='物理',物理 as score  from #student2 
union all 
select stdname,[subject]='语文',语文 as score  from #student2 
order by stdname,[subject]
```
等同于
```
select * from(
select stdname,[subject]='化学',化学 as score  from #student2 
union all 
select stdname,[subject]='数学',数学 as score  from #student2 
union all 
select stdname,[subject]='物理',物理 as score  from #student2 
union all 
select stdname,[subject]='语文',语文 as score  from #student2 
) as a
order by stdname,[subject]
```
