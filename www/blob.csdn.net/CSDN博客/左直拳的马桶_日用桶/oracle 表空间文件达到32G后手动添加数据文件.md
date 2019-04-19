# oracle 表空间文件达到32G后手动添加数据文件 - 左直拳的马桶_日用桶 - CSDN博客
2018年12月27日 17:07:32[左直拳](https://me.csdn.net/leftfist)阅读数：371
客户忽然打电话来说数据库疑似出了问题，数据插不进去。我远程登录过去一看，还真是，按照错误提示一查，估计是数据库文件大小超标了。
```
"ORA-01654: 索引。。。无法通过8192（在表空间。。。中）扩展"
```
原来，oracle单个数据库文件上限是32g。虽然我们设了参数是自动扩展，但上限就是32G，到天花板了。一查果真是
```
SELECT UPPER(F.TABLESPACE_NAME) "表空间名",
D.TOT_GROOTTE_MB "表空间大小(M)",
D.TOT_GROOTTE_MB - F.TOTAL_BYTES "已使用空间(M)",
TO_CHAR(ROUND((D.TOT_GROOTTE_MB - F.TOTAL_BYTES) / D.TOT_GROOTTE_MB * 100,2),'990.99') "使用比",
F.TOTAL_BYTES "空闲空间(M)",
F.MAX_BYTES "最大块(M)"
FROM (SELECT TABLESPACE_NAME,
ROUND(SUM(BYTES) / (1024 * 1024), 2) TOTAL_BYTES,
ROUND(MAX(BYTES) / (1024 * 1024), 2) MAX_BYTES
FROM SYS.DBA_FREE_SPACE
GROUP BY TABLESPACE_NAME) F,
(SELECT DD.TABLESPACE_NAME,
ROUND(SUM(DD.BYTES) / (1024 * 1024), 2) TOT_GROOTTE_MB
FROM SYS.DBA_DATA_FILES DD
GROUP BY DD.TABLESPACE_NAME) D
WHERE D.TABLESPACE_NAME = F.TABLESPACE_NAME
ORDER BY 4 DESC;
```
解决之道，就是手动给该表空间添加数据文件：
```
--爷我一下子给你10G，够爽快吧。
ALTER TABLESPACE "WORK" ADD DATAFILE 'D:\网站\db\work02.DBF' SIZE 10G AUTOEXTEND ON NEXT 50M MAXSIZE UNLIMITED;
```
如果当初建立表空间之初，就设立多个文件，应该也可以。
