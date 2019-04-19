# sum和case - 左直拳的马桶_日用桶 - CSDN博客
2017年08月22日 21:36:14[左直拳](https://me.csdn.net/leftfist)阅读数：415
近期写SQL统计，用到了SUM，还有case，用得麻溜麻溜的。 
自我欣赏一下：
```
select 所在重点区域,
sum(case b when 1 then 1 else 0 end) Builded,
sum(case b when 0 then 1 else 0 end) Building,
sum(case b when -1 then 1 else 0 end) Will
from(
select 所在重点区域,1 b from sde.station_gh where 建设状况='已建'
union all
select 所在重点区域,0 b from sde.station_gh where 建设状况='在建'
union all
select 所在重点区域,-1 b from sde.station_gh where 建设状况='拟建'
) t
group by 所在重点区域
select 监测目标,
sum(case b when 1 then 1 else 0 end) Builded,
sum(case b when 0 then 1 else 0 end) Building,
sum(case b when -1 then 1 else 0 end) Will
from(
select 监测目标,1 b from sde.station_gh where 建设状况='已建'
union all
select 监测目标,0 b from sde.station_gh where 建设状况='在建'
union all
select 监测目标,-1 b from sde.station_gh where 建设状况='拟建'
) t
group by 监测目标
```
