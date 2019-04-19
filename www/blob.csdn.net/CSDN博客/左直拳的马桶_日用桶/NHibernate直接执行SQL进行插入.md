# NHibernate直接执行SQL进行插入 - 左直拳的马桶_日用桶 - CSDN博客
2016年08月26日 20:42:18[左直拳](https://me.csdn.net/leftfist)阅读数：3053
有时候，需要用NHibernate直接执行SQL进行数据insert或update。
怎么写呢？简单一点的，可以直接拼凑出来的SQL，这样写：
```
using NHibernate;
StringBuilder sb = new StringBuilder($@"insert into {tablename}(id,report_Id) values({id},{reportid})");
ISQLQuery query = Session.CreateSQLQuery(sb.ToString());
return query.ExecuteUpdate() > 0;
```
如果怕注入攻击，带参数，酱紫写：
```
StringBuilder strSQL = new StringBuilder("insert into SDE.HaiDiDianLanGuanDaoP (ObjectID,StartName,UseMode,PermitGUID,IsRight,UseModeName,Sat_Marker,shape) ");
strSQL.Append(" values ");
strSQL.AppendFormat("((select sde.version_user_ddl.next_row_id('SDE', (SELECT registration_id SEQ FROM sde.table_registry WHERE owner = 'SDE' and table_name ='{0}' )) OBJECTID from dual)", "HaiDiDianLanGuanDaoP".ToUpper());
strSQL.Append(",:StartName,:UseMode,:PermitGUID,:IsRight,:UseModeName,:Sat_Marker,SDE.ST_TransForm(SDE.ST_Buffer(SDE.ST_TransForm(decode(to_char(substr(:shape,1,2)),'',null,sde.st_geometry(:shape, 0)), 102028), 10), 4326))");
ISQLQuery query = Session.CreateSQLQuery(strSQL.ToString());
query.SetParameter("StartName", daoL.STARTNAME);
query.SetParameter("UseMode", daoL.USEMODE);
query.SetParameter("PermitGUID", daoL.PERMITGUID);
query.SetParameter("IsRight", daoL.ISRIGHT);
query.SetParameter("UseModeName", daoL.USEMODENAME);
query.SetParameter("Sat_Marker", daoL.SAT_MARKER);
query.SetParameter("shape", daoL.SHAPE);
return query.ExecuteUpdate() > 0;
```
