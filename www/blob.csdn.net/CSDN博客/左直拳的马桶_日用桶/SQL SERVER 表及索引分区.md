# SQL SERVER 表及索引分区 - 左直拳的马桶_日用桶 - CSDN博客
2010年06月10日 14:41:00[左直拳](https://me.csdn.net/leftfist)阅读数：2047
表分区应该是处理海量数据一个最好的方法。名义上一个表，但分散存储于不同的物理文件上，这跟小表并没有多大区别。
不过，分区表的主键如何处理却不得不谨慎。
分区表的依据列最好就是主键。比如用户表，UserId是自动增长的标识列，主键，又以之作为划分分区的依据，每50万一个分区。在这里，主键与分区依据列完美结合，护发去头皮。
但有时候主键不一定是分区依据列。比如说，用户设置表。用户表与用户设置表是一对多的关系，一个用户有好多设置，即每个用户会有若干条设置记录。这种情况，UserId应该是分区依据列，但显然不能是主键，因为不唯一。
通常情况下，主键默认是聚集索引。而聚集索引是数据的一部分，就是说，聚集索引和表数据肯定是同一个文件组。如果主键不是分区依据列，又是聚集索引，这个表还怎么分区？我就试过，辛辛苦苦将一个巨大表分好区，此时尚未建主键，后来将主键建到一个独立的文件组，发现表数据也全部挪到该独立文件组，之前的分区努力化为泡影。
我的感觉是，这种情况下，主键不能定为聚集索引，应该做成非聚集索引。
除了依据列，其他索引不要建立在这个分区上（即所谓对齐），全部指定独立文件组，甚至自己分区。否则，系统会偷偷给你搭售依据列。
---------------------------------------------------------------------------------------
（以下内容抄自SQL SERVER 2008 联机帮助）
对聚集索引进行分区
对聚集索引进行分区时，聚集键必须包含分区依据列。对非唯一的聚集索引进行分区时，如果未在聚集键中明确指定分区依据列，默认情况下 SQL Server 将在聚集索引键列表中添加分区依据列。如果聚集索引是唯一的，则必须明确指定聚集索引键包含分区依据列。
 对非聚集索引进行分区
对唯一的非聚集索引进行分区时，索引键必须包含分区依据列。对非唯一的非聚集索引进行分区时，默认情况下 SQL Server 将分区依据列添加为索引的非键（包含性）列，以确保索引与基表对齐。如果索引中已经存在分区依据列，SQL Server 将不会向索引中添加分区依据列。
---------------------------------------------------------------------------------------
CREATE TABLE [dbo].[User_News_Log](
    [LogId] [int] IDENTITY(1,1) NOT NULL,
    [WebUserId] [int] NOT NULL,
    [Code] [varchar](20) NOT NULL,
    [Content] [varchar](700) NOT NULL,
    [CreateDate] [smalldatetime] NOT NULL,
--非聚集索引主键
 CONSTRAINT [PK_user_news] PRIMARY KEY NonCLUSTERED 
(
    [LogId] ASC  
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [UserNewsIndex]
) 
--分区表
ON UserNewsSCM(CreateDate)
--页级压缩
WITH 
(
  DATA_COMPRESSION = PAGE ON PARTITIONS (1 TO 5)
);
GO
也由此可知，想将现有未分区表转为分区表，有个方法就是将聚集索引建到分区上。运行完毕，你会欣喜地发现，表已经分区了。
附：一个现有巨大表分区、压缩、主键再分区的例子
use [Compare]
go
-------------------------------------------------------------------
-- 数据
-------------------------------------------------------------------
--文件组
ALTER DATABASE [Compare] ADD FILEGROUP [Compare0]
GO
ALTER DATABASE [Compare] ADD FILEGROUP [Compare1]
GO
ALTER DATABASE [Compare] ADD FILEGROUP [Compare2]
GO
--
文件
ALTER DATABASE [Compare]
ADD FILE 
(NAME = N'Compare0',FILENAME = N'C:/Compare/Compare0.ndf',SIZE = 3MB,FILEGROWTH = 1MB)
TO FILEGROUP [Compare0]
GO
ALTER DATABASE [Compare]
ADD FILE 
(NAME = N'Compare1',FILENAME = N'C:/Compare/Compare1.ndf',SIZE = 3MB,FILEGROWTH = 1MB)
TO FILEGROUP [Compare1]
GO
ALTER DATABASE [Compare]
ADD FILE 
(NAME = N'Compare2',FILENAME = N'C:/Compare/Compare2.ndf',SIZE = 3MB,FILEGROWTH = 1MB)
TO FILEGROUP [Compare2]
GO
-- 分区函数
CREATE PARTITION FUNCTION ComparePFN(INT) AS RANGE LEFT FOR VALUES (50000,100000); 
GO
--分区方案
CREATE PARTITION SCHEME CompareSCM
AS PARTITION ComparePFN
TO (
[Compare0]
,[Compare1]
,[Compare2]
);
GO
--将聚集索引建到分区方案，数据自动跟着转移
CREATE CLUSTERED INDEX [cix_cp_wk] ON [dbo].[CatchMirror] 
(
    [WebSiteKeyWordId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = OFF) ON CompareSCM(WebSiteKeyWordId)
GO
-----------------------------------------------------------------
-- 行压缩
-----------------------------------------------------------------
ALTER TABLE [CatchMirror]
REBUILD PARTITION = ALL 
WITH
(
DATA_COMPRESSION = ROW ON PARTITIONS(1 TO 3) 
);
GO
-------------------------------------------------------------------
-- 分区索引（主键）
-------------------------------------------------------------------
ALTER DATABASE [Compare] ADD FILEGROUP [CompareIndex0]
GO
ALTER DATABASE [Compare] ADD FILEGROUP [CompareIndex1]
GO
ALTER DATABASE [Compare] ADD FILEGROUP [CompareIndex2]
GO
ALTER DATABASE [Compare]
ADD FILE 
(NAME = N'CompareIndex0',FILENAME = N'C:/Compare/CompareIndex0.ndf',SIZE = 3MB,FILEGROWTH = 1MB)
TO FILEGROUP [CompareIndex0]
GO
ALTER DATABASE [Compare]
ADD FILE 
(NAME = N'CompareIndex1',FILENAME = N'C:/Compare/CompareIndex1.ndf',SIZE = 3MB,FILEGROWTH = 1MB)
TO FILEGROUP [CompareIndex1]
GO
ALTER DATABASE [Compare]
ADD FILE 
(NAME = N'CompareIndex2',FILENAME = N'C:/Compare/CompareIndex2.ndf',SIZE = 3MB,FILEGROWTH = 1MB)
TO FILEGROUP [CompareIndex2]
GO
CREATE PARTITION FUNCTION CompareIndexPFN(INT) AS RANGE LEFT FOR VALUES (200000000,400000000); 
GO
CREATE PARTITION SCHEME CompareIndexSCM
AS PARTITION CompareIndexPFN
TO (
[CompareIndex0]
,[CompareIndex1]
,[CompareIndex2]
);
GO
--主键，非聚集，分区，并启用行压缩
ALTER TABLE [dbo].[CatchMirror] ADD  CONSTRAINT [PK_CatchMirror] PRIMARY KEY NONCLUSTERED 
(
    [CatchMirrorId] ASC
)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON,DATA_COMPRESSION = ROW
) ON CompareIndexSCM(CatchMirrorId)
GO
后记：
应用分区对性能的改善是显著的。未分区和压缩前，服务器硬盘队列达到30以上；分区和压缩后，硬盘队列10以下，足足改善了3倍。
补充一句：如果表已经应用了页级压缩，聚集索引就不要再应用行压缩，否则整个表又变成行压缩。
**2、拆分分区：**
拆分分区，除非分区方案里有多余的文件组，否则要先添加文件组，然后修改分区方案，再修改分区函数。
ALTER PARTITION SCHEME UserSCM NEXT USED [user5];
ALTER PARTITION FUNCTION UserPFN() SPLIT RANGE('5')
**3、分区依据列为字符型**
-- 分区函数
CREATE PARTITION FUNCTION UserPFN(VARCHAR(50)) AS RANGE LEFT FOR VALUES (
'1','4','7','a','c','e','h','k','o','r','u','x','z');
GO
**4、分区依据列为日期型**
-- 分区函数
CREATE PARTITION FUNCTION UserPFN(SMALLDATETIME) AS RANGE LEFT FOR VALUES ('2008-01-01','2009-01-01','2010-01-01','2011-01-01','2012-01-01');
GO
