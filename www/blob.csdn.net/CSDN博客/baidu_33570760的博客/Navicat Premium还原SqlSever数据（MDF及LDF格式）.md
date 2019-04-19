# Navicat Premium还原SqlSever数据（MDF及LDF格式） - baidu_33570760的博客 - CSDN博客
2017年08月24日 15:22:59[carman_风](https://me.csdn.net/baidu_33570760)阅读数：1815
## 转载自：http://blog.csdn.net/bruce_shan/article/details/54956764
## 1.还原.mdf格式数据库文件
1.新建SQL server连接
2.执行查询语句 
```bash
exec sp_attach_db '数据库名','E:\A.mdf','E:\A_log.ldf'
```
## 2.使用客户端进行备份还原（.bak数据库文件）
![这里写图片描述](https://img-blog.csdn.net/20170209210312988?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQnJ1Y2Vfc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170209210326107?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQnJ1Y2Vfc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170209210342483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQnJ1Y2Vfc2hhbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**实际上也看到了备份过程中执行的sql 语句，粘贴如下：**
```
BACKUP DATABASE [数据库名]
TO
  DISK = N'e:\DataBase\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Backup\数据库名.bak'
WITH
  DESCRIPTION = N'描述信息',
  NAME = N'数据库名 - Backup',
  NOFORMAT, NOINIT, SKIP,
  STATS = 5
```
*标注：实际使用过程中，第一种方式还原使用起来很快（Affected rows: 0 Time: 0.106s），第二种方式还原很慢，备份还好，还原特别慢，建议采用第一种方式还原数据库*
