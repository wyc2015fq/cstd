# 学习笔记—— 一些UPDATE语句 - 左直拳的马桶_日用桶 - CSDN博客
2014年03月12日 16:41:51[左直拳](https://me.csdn.net/leftfist)阅读数：2354
个人分类：[sql server																[学习笔记](https://blog.csdn.net/leftfist/article/category/1790155)](https://blog.csdn.net/leftfist/article/category/136470)
UPDATE语句原来还有许多种写法，有的还很复杂，孤陋寡闻的我甚至闻所未闻。幸甚至哉，记而志之。
**0、UPDATE 表名 SET 字段。。。 FROM 。。。的方式**
USE AdventureWorks2012;
GO
UPDATE Production.ScrapReason
SET Name += ' - tool malfunction'
FROM Production.ScrapReason AS sr
JOIN Production.WorkOrder AS wo 
     ON sr.ScrapReasonID = wo.ScrapReasonID
     AND wo.ScrappedQty > 300;
就是需要联合其他表进行一些条件过滤的时候，可以采用这种方式。删除语句更厉害，有delete from ... from ... 的格式。第一个from是delete语句必须的，第二个from是用于联合其他表进行条件过滤。
WITH w AS(        
        SELECT Id,ROW_NUMBER() OVER(ORDER BY Id DESC) AS [Row]
        FROM [t1]
        WHERE Code=@Code
    )
    DELETE FROM t1 FROM t1 s INNER JOIN w ON s.Id=w.Id
    WHERE w.Row>100;
这种格式中，表名也可以采用别名：
USE AdventureWorks2012;
GO
UPDATE sr
SET sr.Name += ' - tool malfunction'
FROM Production.ScrapReason AS sr
JOIN Production.WorkOrder AS wo 
     ON sr.ScrapReasonID = wo.ScrapReasonID
     AND wo.ScrappedQty > 300;
**1、使用TOP语句**
USE AdventureWorks2012;
GO
UPDATE TOP (10) HumanResources.Employee
SET VacationHours = VacationHours * 1.25 ;
GO
**2、使用 WITH common_table_expression 子句**
USE AdventureWorks2012;
GO
WITH Parts(AssemblyID, ComponentID, PerAssemblyQty, EndDate, ComponentLevel)AS
(
    SELECT b.ProductAssemblyID, b.ComponentID, b.PerAssemblyQty,
        b.EndDate, 0 AS ComponentLevel
    FROM Production.BillOfMaterials AS b
    WHERE b.ProductAssemblyID = 800
          AND b.EndDate IS NULL
    UNION ALL
    SELECT bom.ProductAssemblyID, bom.ComponentID, p.PerAssemblyQty,
        bom.EndDate, ComponentLevel + 1
    FROM Production.BillOfMaterials AS bom 
        INNER JOIN Parts AS p
        ON bom.ProductAssemblyID = p.ComponentID
        AND bom.EndDate IS NULL
)
UPDATE Production.BillOfMaterials
SET PerAssemblyQty = c.PerAssemblyQty * 2
FROM Production.BillOfMaterials AS c
JOIN Parts AS d ON c.ProductAssemblyID = d.AssemblyID
WHERE d.ComponentLevel = 0; 
parts是这个公用表表达式里的表名，括号里的是字段别名。
**3、使用包含 .WRITE 的 UPDATE**
USE AdventureWorks2012;
GO
UPDATE Production.Document
SET DocumentSummary .WRITE(N'Carefully inspect and maintain the tires and crank arms.',0,NULL)
WHERE Title = N'Crank Arm and Tire Maintenance';
.WRITE (expression,@Offset,@Length)
指定修改 column_name 值的一部分。 expression 替换 @Length 单位（从 column_name 的 @Offset 开始）。 只有varchar(max)、nvarchar(max) 或 varbinary(max) 列才能使用此子句来指定。 column_name 不能为 NULL，也不能由表名或表别名限定。
expression 是复制到 column_name 的值。 expression 必须运算或隐式转换为 column_name 类型。 如果将 expression 设置为 NULL，则忽略@Length，并将 column_name 中的值按指定的 @Offset 截断。
@Offset 是 column_name 值中的起点，从该点开始编写 expression。 @Offset 是基于零的序号位置，数据类型为 bigint，不能为负数。 如果@Offset 为 NULL，则更新操作将在现有 column_name 值的结尾追加 expression，并忽略 @Length。 如果 @Offset 大于 column_name 值的长度，则数据库引擎将返回错误。 如果 @Offset 加上 @Length 超出了列中基础值的限度，则将删除到值的最后一个字符。 如果 @Offset 加上 LEN(expression) 大于声明的基础大小，则将出现错误。
@Length 是指列中某个部分的长度，从 @Offset 开始，该长度由 expression 替换。 @Length 是 bigint 并且不能为负数。 如果 @Length 为 NULL，则更新操作将删除从 @Offset 到 column_name 值的结尾的所有数据。
**4、使用包含 OPENROWSET 的 UPDATE 修改 varbinary(max) 列**
以下示例将 varbinary(max) 列中存储的现有图像替换为新图像。 将 OPENROWSET 函数和 BULK 选项一起使用以将图像加载到列中。 此示例假定指定的文件路径中存在名为 Tires.jpg 的文件。
USE AdventureWorks2012;
GO
UPDATE Production.ProductPhoto
SET ThumbNailPhoto = (
    SELECT *
    FROM OPENROWSET(BULK 'c:\Tires.jpg', SINGLE_BLOB) AS x )
WHERE ProductPhotoID = 1;
GO
**5、捕获 UPDATE 语句的结果**
USE AdventureWorks2012;
GO
DECLARE @MyTableVar table(
    EmpID int NOT NULL,
    OldVacationHours int,
    NewVacationHours int,
    ModifiedDate datetime);
UPDATE TOP (10) HumanResources.Employee
SET VacationHours = VacationHours * 1.25,
    ModifiedDate = GETDATE() 
OUTPUT inserted.BusinessEntityID,
       deleted.VacationHours,
       inserted.VacationHours,
       inserted.ModifiedDate
INTO @MyTableVar;
--Display the result set of the table variable.
SELECT EmpID, OldVacationHours, NewVacationHours, ModifiedDate
FROM @MyTableVar;
GO
--Display the result set of the table.
SELECT TOP (10) BusinessEntityID, VacationHours, ModifiedDate
FROM HumanResources.Employee;
GO
inserted、deleted都是触发器中常用到的系统表。
inserted：当有新记录被INSERT，或原表中有记录被UPDATE，那么这些新记录会拷贝一份到inserted;
deleted：当记录被update、delete，那么会从原表中拷贝一份到deleted表，然后原表中的记录被修改、删除；
**6、UPDATE使用CASE语句**
UPDATE [Table1]
        SET [StatuID] = CASE WHEN  [StatuID] - 1 < 0 THEN 0 ELSE [StatuID] - 1 END
        WHERE [IndexId] = @IndexId;
**7、在 TRY…CATCH 块中使用 UPDATE**
以下示例在 TRY…CATCH 块中使用 UPDATE 语句来处理在执行更新操作期间可能遇到的执行错误。
USE AdventureWorks2012;
GO
BEGIN TRANSACTION;
BEGIN TRY
    -- Intentionally generate a constraint violation error.
    UPDATE HumanResources.Department
    SET Name = N'MyNewName'
    WHERE DepartmentID BETWEEN 1 AND 2;
END TRY
BEGIN CATCH
    SELECT 
         ERROR_NUMBER() AS ErrorNumber
        ,ERROR_SEVERITY() AS ErrorSeverity
        ,ERROR_STATE() AS ErrorState
        ,ERROR_PROCEDURE() AS ErrorProcedure
        ,ERROR_LINE() AS ErrorLine
        ,ERROR_MESSAGE() AS ErrorMessage;
    IF @@TRANCOUNT > 0
        ROLLBACK TRANSACTION;
END CATCH;
IF @@TRANCOUNT > 0
    COMMIT TRANSACTION;
GO
这种语句，主要是看看里面这些系统函数的用法。
**8、使用 WHERE CURRENT OF 子句**
以下示例使用 WHERE CURRENT OF 子句来只更新游标位于其上的行。 如果游标基于某个联接，则只修改 UPDATE 语句中指定的 table_name。其他参与该游标的表不会受到影响。
USE AdventureWorks2012;
GO
DECLARE complex_cursor CURSOR FOR
    SELECT a.BusinessEntityID
    FROM HumanResources.EmployeePayHistory AS a
    WHERE RateChangeDate <> 
         (SELECT MAX(RateChangeDate)
          FROM HumanResources.EmployeePayHistory AS b
          WHERE a.BusinessEntityID = b.BusinessEntityID) ;
OPEN complex_cursor;
FETCH FROM complex_cursor;
UPDATE HumanResources.EmployeePayHistory
SET PayFrequency = 2 
WHERE CURRENT OF complex_cursor;
CLOSE complex_cursor;
DEALLOCATE complex_cursor;
GO按我的理解，游标相当于DataReader，逐行读取，并且保持数据库连接。针对游标中的行进行修改，会直接投射到数据库中，改游标其实就相当于改数据库。
UPDATE语句的写法还有很多，以上只是摘录一二而已。
