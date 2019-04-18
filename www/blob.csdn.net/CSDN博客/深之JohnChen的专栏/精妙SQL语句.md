# 精妙SQL语句 - 深之JohnChen的专栏 - CSDN博客

2005年09月01日 17:14:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1487标签：[sql																[date																[table																[存储																[join																[insert](https://so.csdn.net/so/search/s.do?q=insert&t=blog)](https://so.csdn.net/so/search/s.do?q=join&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=table&t=blog)](https://so.csdn.net/so/search/s.do?q=date&t=blog)](https://so.csdn.net/so/search/s.do?q=sql&t=blog)
个人分类：[数据库编程](https://blog.csdn.net/byxdaz/article/category/184468)


精妙SQL语句介绍

　　如何从一位菜鸟蜕变成为高手，灵活使用的SQL语句是必不可少的。本文收集了部分比较经典，常用的SQL语句供大家参考，希望对大家有所帮助。 

        说明：存储过程的使用,CREATE PROC 创建存储过程,SQL2000中用sp_xxx和xp_xxx存储过程;一般来说,sp_xxx是一般的存储过程,而xp_xxx是扩展的存储过程。使用这些系统存储过程时，一般使用USE MASTER然后在使用sp_xxx或者xp_xxx。

　　说明：复制表(只复制结构,源表名：a 新表名：b)　　 

　　SQL: select * into b from a where 1<>1 

　　说明：拷贝表(拷贝数据,源表名：a 目标表名：b)　　 

　　SQL: insert into b(a, b, c) select d,e,f from b; 

　　说明：显示文章、提交人和最后回复时间　　 

　　SQL: select a.title,a.username,b.adddate from table a,(select max(adddate) adddate from table where table.title=a.title) b 

　　说明：外连接查询(表名1：a 表名2：b)　　 

　　SQL: select a.a, a.b, a.c, b.c, b.d, b.f from a LEFT OUT JOIN b ON a.a = b.c 

　　说明：日程安排提前五分钟提醒　　 

　　SQL: select * from 日程安排 where datediff('minute',f开始时间,getdate())>5　　 

　　说明：两张关联表，删除主表中已经在副表中没有的信息 

　　SQL: 　　 

　　delete from info where not exists ( select * from infobz where info.infid=infobz.infid 

　　说明：-- 

　　SQL: 　　 

　　SELECT A.NUM, A.NAME, B.UPD_DATE, B.PREV_UPD_DATE 

　　 FROM TABLE1, 

　　 (SELECT X.NUM, X.UPD_DATE, Y.UPD_DATE PREV_UPD_DATE 

　　 FROM (SELECT NUM, UPD_DATE, INBOUND_QTY, STOCK_ONHAND 

　　 FROM TABLE2 

　　 WHERE TO_CHAR(UPD_DATE,'YYYY/MM') = TO_CHAR(SYSDATE, 'YYYY/MM')) X, 

　　 (SELECT NUM, UPD_DATE, STOCK_ONHAND 

　　 FROM TABLE2 

　　 WHERE TO_CHAR(UPD_DATE,'YYYY/MM') = 

　　 TO_CHAR(TO_DATE(TO_CHAR(SYSDATE, 'YYYY/MM') ¦¦ '/01','YYYY/MM/DD') - 1, 'YYYY/MM') Y, 

　　 WHERE X.NUM = Y.NUM （+） 

　　 AND X.INBOUND_QTY + NVL(Y.STOCK_ONHAND,0) <> X.STOCK_ONHAND B 

　　WHERE A.NUM = B.NUM 

　　说明：-- 

　　SQL: 　　 

　　select * from studentinfo where not exists(select * from student where studentinfo.id=student.id) and 系名称='"&strdepartmentname&"' and 专业名称='"&strprofessionname&"' order by 性别,生源地,高考总成绩 

　　说明： 

　　从数据库中去一年的各单位电话费统计(电话费定额贺电化肥清单两个表来源） 

　　SQL: 　 

　　SELECT a.userper, a.tel, a.standfee, TO_CHAR(a.telfeedate, 'yyyy') AS telyear, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '01', a.factration)) AS JAN, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '02', a.factration)) AS FRI, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '03', a.factration)) AS MAR, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '04', a.factration)) AS APR, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '05', a.factration)) AS MAY, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '06', a.factration)) AS JUE, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '07', a.factration)) AS JUL, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '08', a.factration)) AS AGU, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '09', a.factration)) AS SEP, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '10', a.factration)) AS OCT, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '11', a.factration)) AS NOV, 

　　 SUM(decode(TO_CHAR(a.telfeedate, 'mm'), '12', a.factration)) AS DEC 

　　FROM (SELECT a.userper, a.tel, a.standfee, b.telfeedate, b.factration 

　　 FROM TELFEESTAND a, TELFEE b 

　　 WHERE a.tel = b.telfax) a 

　　GROUP BY a.userper, a.tel, a.standfee, TO_CHAR(a.telfeedate, 'yyyy') 

　　说明：四表联查问题：　　 

　　SQL: select * from a left inner join b on a.a=b.b right inner join c on a.a=c.c inner join d on a.a=d.d where ..... 

　　说明：得到表中最小的未使用的ID号 

　　SQL:　 

　　SELECT (CASE WHEN EXISTS(SELECT * FROM Handle b WHERE b.HandleID = 1) THEN MIN(HandleID) + 1 ELSE 1 END) as HandleID 

　　 FROM Handle 

　　 WHERE NOT HandleID IN (SELECT a.HandleID - 1 FROM Handle a) 


