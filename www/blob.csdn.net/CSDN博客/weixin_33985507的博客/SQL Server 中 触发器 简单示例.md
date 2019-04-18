# SQL Server 中 触发器 简单示例 - weixin_33985507的博客 - CSDN博客
2007年03月09日 10:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
注意 对 inserted,deleted 的查询 使用
-------------------------
--当person表 新增 一条person记录时 
--将 该记录的主键personName 保存在changeTable表中
USE TestDB
IF EXISTS (SELECT name FROM sysobjects
      WHERE name = 'personInsert' AND type = 'TR')
   DROP TRIGGER personInsert
GO
CREATE TRIGGER personInsert
ON person
FOR INSERT
AS
   INSERT INTO TestDB.dbo.changeTable
  (changeTableName, changeType, changeTablePK, changeTablePKValue)
  SELECT 'person','INSERT','personName',personName FROM INSERTED
GO
--当person表 修改 一条person记录时 
--将 该记录的主键personName 保存在changeTable表中
USE TestDB
IF EXISTS (SELECT name FROM sysobjects
      WHERE name = 'personUpdate' AND type = 'TR')
   DROP TRIGGER personUpdate
GO
CREATE TRIGGER personUpdate
ON person
FOR UPDATE
AS
   INSERT INTO TestDB.dbo.changeTable
  (changeTableName, changeType, changeTablePK, changeTablePKValue)
  SELECT 'person','UPDATE','personName',personName FROM INSERTED
GO
--当person表 删除 一条person记录时 
--将 该记录的主键personName 保存在changeTable表中
USE TestDB
IF EXISTS (SELECT name FROM sysobjects
      WHERE name = 'personDelete' AND type = 'TR')
   DROP TRIGGER personDelete
GO
CREATE TRIGGER personUpdate
ON person
FOR DELETE
AS
   INSERT INTO TestDB.dbo.changeTable
  (changeTableName, changeType, changeTablePK, changeTablePKValue)
  SELECT 'person','DELETE','personName',personName FROM DELETED
GO
