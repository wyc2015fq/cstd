# oracle多表关联多字段update - 左直拳的马桶_日用桶 - CSDN博客
2016年06月13日 18:32:31[左直拳](https://me.csdn.net/leftfist)阅读数：6526
多表关联多字段update
有代码有J8:
```
update spatial_references
set(
auth_name, 
auth_srid, 
falsex, 
falsey, 
xyunits, 
falsez, 
zunits, 
falsem, 
munits, 
xycluster_tol, 
zcluster_tol, 
mcluster_tol, 
object_flags, 
srtext
)=(select  
auth_name, 
auth_srid, 
falsex, 
falsey, 
xyunits, 
falsez, 
zunits, 
falsem, 
munits, 
xycluster_tol, 
zcluster_tol, 
mcluster_tol, 
object_flags, 
srtext
from spatial_references where srid=0)
where srid=2;
```
=============================================== 
2016.07.09 
以上这条语句不够实用。今天有同事问到，结果露怯，于是发粪涂墙，狂搜资料，又写了一条实用一点的：
`update qq q set 证书GUID=(select zs.guid from 证书表 zs where zs.code=q.证书编号 and rownum=1)`
更完整一点的：
```
update qq q set 证书GUID=(select zs.guid from 证书表 zs where zs.code=q.证书编号 and rownum=1)
where exists(select 1 from 证书表 zs where zs.code=q.证书编号 and zs.guid<>q.证书GUID)
```
```
update nodeconfiginfo n set (qh_name,qh_parentid,qh_orderid)=(select qh_name,qh_parentid,qh_orderid
from SeaBoardConfig s where s.qh_id=n.qh_id)
where exists(select 1 from SeaBoardConfig s where s.qh_id=n.qh_id);
```
这条语句，SET里有个循环；然后外层也有个循环。这两个循环没有什么直接关系。我不知道这条SQL语句是如何执行的，猜想应该是嵌套循环：
```
foreach()
{//外层循环
    foreach()
    {//set循环
    }
}
```
如果没有外层循环的话，那么会全表执行。
