# sql server2000中怎么对存储过程进行加密和解密 - 深之JohnChen的专栏 - CSDN博客

2005年09月01日 14:07:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3041


**1.** >>加密
From the Books Online:

Use the WITH ENCRYPTION option:

IF EXISTS (SELECT name FROM sysobjects
WHERE name = 'encrypt_this' AND type = 'P')
DROP PROCEDURE encrypt_this
GO
USE pubs
GO
CREATE PROCEDURE encrypt_this
WITH ENCRYPTION
AS
SELECT * 
FROM authors
GO

EXEC sp_helptext encrypt_this 

2. 关键字字节不受限制,比SQLexplorer安全,查看所有SYSCOMMENTS内加密的代码

create PROCEDURE sp_decrypt(@objectName varchar(50))
AS
begin
set nocount on
--CSDN：j9988 copyright:2004.01.05 
--V3.1
--破解字节不受限制，适用于SQLSERVER2000存储过程，函数，视图，触发器
--发现有错，请E_MAIL：CSDNj9988@tom.com
begin tran
declare @objectname1 varchar(100),@orgvarbin varbinary(8000)
declare @sql1 nvarchar(4000),@sql2 varchar(8000),@sql3 nvarchar(4000),@sql4 nvarchar(4000)
DECLARE @OrigSpText1 nvarchar(4000), @OrigSpText2 nvarchar(4000) , @OrigSpText3 nvarchar(4000), @resultsp nvarchar(4000)
declare @i int,@status int,@type varchar(10),@parentid int
declare @colid int,@n int,@q int,@j int,@k int,@encrypted int,@number int
select @type=xtype,@parentid=parent_obj from sysobjects where id=object_id(@ObjectName)

create table #temp(number int,colid int,ctext varbinary(8000),encrypted int,status int)
insert #temp SELECT number,colid,ctext,encrypted,status FROM syscomments WHERE id = object_id(@objectName)
select @number=max(number) from #temp
set @k=0

while @k<=@number 
begin
if exists(select 1 from syscomments where id=object_id(@objectname) and number=@k)
begin
if @type='P'
set @sql1=(case when @number>1 then 'ALTER PROCEDURE '+ @objectName +';'+rtrim(@k)+' WITH ENCRYPTION AS '
else 'ALTER PROCEDURE '+ @objectName+' WITH ENCRYPTION AS '
end)

if @type='TR'
set @sql1='ALTER TRIGGER '+@objectname+' ON '+OBJECT_NAME(@parentid)+' WITH ENCRYPTION FOR INSERT AS PRINT 1 '

if @type='FN' or @type='TF' or @type='IF'
set @sql1=(case @type when 'TF' then 
'ALTER FUNCTION '+ @objectName+'(@a char(1)) returns @b table(a varchar(10)) with encryption as begin insert @b select @a return end '
when 'FN' then
'ALTER FUNCTION '+ @objectName+'(@a char(1)) returns char(1) with encryption as begin return @a end'
when 'IF' then
'ALTER FUNCTION '+ @objectName+'(@a char(1)) returns table with encryption as return select @a as a'
end)

if @type='V'
set @sql1='ALTER VIEW '+@objectname+' WITH ENCRYPTION AS SELECT 1 as f'

set @q=len(@sql1)
set @sql1=@sql1+REPLICATE('-',4000-@q)
select @sql2=REPLICATE('-',8000)
set @sql3='exec(@sql1'
select @colid=max(colid) from #temp where number=@k 
set @n=1
while @n<=CEILING(1.0*(@colid-1)/2) and len(@sQL3)<=3996
begin 
set @sql3=@sql3+'+@'
set @n=@n+1
end
set @sql3=@sql3+')'
exec sp_executesql @sql3,N'@Sql1 nvarchar(4000),@ varchar(8000)',@sql1=@sql1,@=@sql2

end
set @k=@k+1
end

set @k=0
while @k<=@number 
begin

if exists(select 1 from syscomments where id=object_id(@objectname) and number=@k)
begin
select @colid=max(colid) from #temp where number=@k 
set @n=1

while @n<=@colid
begin
select @OrigSpText1=ctext,@encrypted=encrypted,@status=status FROM #temp WHERE colid=@n and number=@k

SET @OrigSpText3=(SELECT ctext FROM syscomments WHERE id=object_id(@objectName) and colid=@n and number=@k)
if @n=1
begin
if @type='P'
SET @OrigSpText2=(case when @number>1 then 'CREATE PROCEDURE '+ @objectName +';'+rtrim(@k)+' WITH ENCRYPTION AS '
else 'CREATE PROCEDURE '+ @objectName +' WITH ENCRYPTION AS '
end)

if @type='FN' or @type='TF' or @type='IF'--刚才有错改一下
SET @OrigSpText2=(case @type when 'TF' then 
'CREATE FUNCTION '+ @objectName+'(@a char(1)) returns @b table(a varchar(10)) with encryption as begin insert @b select @a return end '
when 'FN' then
'CREATE FUNCTION '+ @objectName+'(@a char(1)) returns char(1) with encryption as begin return @a end'
when 'IF' then
'CREATE FUNCTION '+ @objectName+'(@a char(1)) returns table with encryption as return select @a as a'
end)

if @type='TR'
set @OrigSpText2='CREATE TRIGGER '+@objectname+' ON '+OBJECT_NAME(@parentid)+' WITH ENCRYPTION FOR INSERT AS PRINT 1 '

if @type='V'
set @OrigSpText2='CREATE VIEW '+@objectname+' WITH ENCRYPTION AS SELECT 1 as f'

set @q=4000-len(@OrigSpText2)
set @OrigSpText2=@OrigSpText2+REPLICATE('-',@q)
end
else
begin
SET @OrigSpText2=REPLICATE('-', 4000)
end
SET @i=1

SET @resultsp = replicate(N'A', (datalength(@OrigSpText1) / 2))

WHILE @i<=datalength(@OrigSpText1)/2
BEGIN

SET @resultsp = stuff(@resultsp, @i, 1, NCHAR(UNICODE(substring(@OrigSpText1, @i, 1)) ^
(UNICODE(substring(@OrigSpText2, @i, 1)) ^
UNICODE(substring(@OrigSpText3, @i, 1)))))
SET @i=@i+1
END
set @orgvarbin=cast(@OrigSpText1 as varbinary(8000))
set @resultsp=(case when @encrypted=1 
then @resultsp 
else convert(nvarchar(4000),case when @status&2=2 then uncompress(@orgvarbin) else @orgvarbin end)
end)
print @resultsp

set @n=@n+1

end

end
set @k=@k+1
end

drop table #temp
rollback tran
end 



