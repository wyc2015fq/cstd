# Oracle 查看当前连接数与会话数情况 - z69183787的专栏 - CSDN博客
2014年04月14日 10:46:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2785
select  b.MACHINE, b.PROGRAM , count(*) from v$process a, v$session b where a.ADDR = b.PADDR and  b.USERNAME is not null   group by  b.MACHINE  , b.PROGRAM order by count(*) desc
**select INST_ID, count(*) from gv$session  g group by g.INST_ID**
