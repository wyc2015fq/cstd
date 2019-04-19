# Qt中SQL语句update同时更新多字段及设置字段值为空的方法 - xqhrs232的专栏 - CSDN博客
2018年05月27日 16:04:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：271
原文地址::[https://blog.csdn.net/ckzhb/article/details/79853566](https://blog.csdn.net/ckzhb/article/details/79853566)
Qt中往往需要对数据库进行操作，常出现根据某变量更改相关字段的内容。一般地，可以采用名称绑定和位置绑定两种方法。
Qt与MySQL连接方法：[https://blog.csdn.net/ckzhb/article/details/79630035](https://blog.csdn.net/ckzhb/article/details/79630035)
本文以update举例，其他insert等语句操作类似。
方法一：名称绑定
[cpp][view plain](https://blog.csdn.net/ckzhb/article/details/79853566#)[copy](https://blog.csdn.net/ckzhb/article/details/79853566#)
- 已知：定义Qstring类型变量a b c d e f g h。  
- QSqlQuery query5;  
- query5.prepare(QString("UPDATE rfid_match SET CKH=:ckh,WLMS=:wlms,CCLX=:cclx,CW=:cw,PC=:pc,GC=:gc,JLDW=:jldw WHERE WZSFM='%1'").arg(a));  
- query5.bindValue(":ckh",b);  
- query5.bindValue(":wlms",c);  
- query5.bindValue(":cclx",d);  
- query5.bindValue(":cw",e);  
- query5.bindValue(":pc",f);  
- query5.bindValue(":gc",g);  
- query5.bindValue(":jldw",h);  
- query5.exec();  
注意：第三行中的WHERE='%1'中不要忘记%1两边的单引号，因为表中字段类型是varchar型。
方法二：位置绑定
[cpp][view plain](https://blog.csdn.net/ckzhb/article/details/79853566#)[copy](https://blog.csdn.net/ckzhb/article/details/79853566#)
- 已知：定义Qstring类型变量a b c d e f g h。  
- QSqlQuery query5;  
- query5.prepare(QString("update rfid_match set CKH=?,WLMS=?,"
- "CCLX=?,CW=?,PC=?,GC=?,JLDW=? WHERE WZSFM='%1'").arg(a));  
- query5.addBindValue(b);  
- query5.addBindValue(c);  
- query5.addBindValue(d);  
- query5.addBindValue(e);  
- query5.addBindValue(f);  
- query5.addBindValue(g);  
- query5.addBindValue(h);  
- query5.exec();    
提示：
将数据库中某字段值设置为空用null，如下所示
[cpp][view plain](https://blog.csdn.net/ckzhb/article/details/79853566#)[copy](https://blog.csdn.net/ckzhb/article/details/79853566#)
- query.exec(QString("update rfid_match set RFID2=null where WZSFM='%1'").arg(model->item(modelReIndex,4)->text()));  
//=====================================================================
备注：：
举例如下
1》
 QString  idstr;
query1.prepare(QString("UPDATE Tb_abc_Info SET PRINT_FLAG=:print_flag WHERE id='%1'").arg(idstr));              query1.bindValue(":print_flag",2);              query1.exec();
2》
 QString  idstr;
query1.prepare(QString("update Tb_abc_Info set print_Flag=? WHERE id='%1'").arg(idstr));           query1.addBindValue(2);           query1.exec();
