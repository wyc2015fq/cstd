# SQL Server 不可见字符处理 - 文章 - 伯乐在线
原文出处： [一百零七个](http://www.cnblogs.com/zzry/p/5729404.html)
**前言**
问题描述：在表列里有肉眼不可见字符，导致一些更新或插入失败。 几年前第一次碰见这种问题是在读取考勤机人员信息时碰见的，折腾了一点时间，现在又碰到了还有点新发现就顺便一起记录下。
如下图所示 golds字段 看上去5个字符，长度则为44 ，可判断有特殊字符存在此列中。
![](http://jbcdn2.b0.upaiyun.com/2016/08/6a83c02957444de689ebb9372d21a218.png)
**基础知识准备**
贴几个常用函数
**1、获取字符的ASCII码 ASCII**
ASCII码是对字符的标准编码。要获取字符的ASCII码就可以通过调用ASCII函数来实现。
语法结构：
ASCII(espression)
这里的expression是一个返回char或varchar数据类型的表达式，**ASCII函数仅对表达式最左侧的字符返回ASCII码值**。–**这个是重点**
返回值：int数据类型。
示例：
select ASCII(‘f’) –输出 102
注意如果是多个字符的字符串，也只是返回第一个字符的ASCII码值。
**2、获取ASCII码对应的字符 Char**
语法结构：
char(integer_expression)
返回值类型：char型字符
示例：
select char(102) — 输出f
**3、获取字符的unicode编码 Unicode**
unicode函数可以获得字符的unicode编码。
语法结构：
unicode(‘ncharacter_expression’)
这里的ncharacter_expression是nchar或nvarchar类型的表达式。函数仅会返回第一个字符的unicode编码。
返回值：int类型数据
示例：
SELECT Unicode(‘飞’) — 输出 39134
**4、获取unicode编码对应的字符nchar**
nchar能够根据unicode标准的定义，返回具有指定的整数代码的unicode字符。
语法结构：
nchar(integer_expression)
返回值：unicode字符
示例：
SELECT nchar(39134) — 输出 飞
**5、获取字符串长度LEN**
len函数用于获取字符串的长度(字符数)，但不包括右边的空格。左边的空格和右边的空格计算在内。
语法结构：
len( string_expression )
参数说明：
string_expression：要计算长度的字符串。
返回值：expression数据类型为varchar(max)、nvarchar(max)或varbinary(max)，则为bigint。否则为int。
示例：
select len(‘天下之大，无奇不有’) — 输出 9
**6、REPLACE**
用另一个字符串值替换出现的所有指定字符串值。
语法如下：
REPLACE ( string_expression1 , string_expression2 , string_expression3 )
参数
string_expression1:要搜索的字符串表达式。string_expression1 可以是字符或二进制数据类型。
string_expression2:要查找的子字符串。string_expression2 可以是字符或二进制数据类型。
string_expression3:替换字符串。string_expression3 可以是字符或二进制数据类型。
SELECT REPLACE(‘abcde’,’abc’,’xxx’) –xxxde
**7 、指定位置搜索字符串中的内容CHARINDEX**
charindex函数用于在指定的字符串中搜索特定的字符串，并可以指定开始搜索的位置，返回第一次找到目标字符串的字符数。
语法结构：
charindex ( expression1 , expression2 [ , start_location] )
参数说明：
expression1：一个字符串数据类型的表达式，其中包含要查找的字符的序列。
expression2：一个字符串数据类型的表达式，通常是一个为指定序列搜索的列。
start_location：开始在expression2中搜索expression1是的字符位置。如果start_location未被指定、是一个负数或零，则将从expression2的开头开始搜索。　　　　　　start_location可以是bingint类型。
返回值：如果expression2的数据类型为varchar(max)、nvarchar(max)或varbinary(max)，则为bigint，否则为int。
示例：
select charindex(‘456′,’123456789’) — 输出 4 从123456789中检索456出现的位置
select charindex(‘456′,’123456789’,4) — 输出 4
**问题处理**
以前言中描述的例子为例（个人习惯处理方式供参考，如果你的肉眼自带反隐功能可略过下文）
先查看左右边字符和肉眼所见是否吻合结果如下图
![](http://jbcdn2.b0.upaiyun.com/2016/08/9a16f4ecc94dc650938e873d07132dd9.png)
初步断定不可见字符在右边，因为第三个的查询结果不是0而是空（可能是字符串空吧”）
接着查看它的ascii编码值
![](http://jbcdn2.b0.upaiyun.com/2016/08/bcd7244d7f8e9687624db065052c4736.png)
查看ascii表，**ASCII 非打印控制字符**：ASCII 表上的数字 0–31 分配给了控制字符，用于控制像打印机等一些外围设备。
![](http://jbcdn2.b0.upaiyun.com/2016/08/f30cdcb1f98425c560fae98773243e22.png)
得到结论1：右边第一个不可见字符是个控制字符（对应char(0),为什么是char(0)上面贴的基础知识有讲）
一般到这里就结束了，查到了是什么字符，然后用replace函数替换成字符串空完事，下面将其处理掉时发现失败
上面查到了是char（0）在作祟，而且我循环输出golds字段右边的字符发现有39个char（0），方便理解，简化问题，改为下图对比着看
![](http://jbcdn2.b0.upaiyun.com/2016/08/9c66914e62167a216875f6bc505e797e.png)
初步断定在@a里没找到
继续追踪原因
![](http://jbcdn2.b0.upaiyun.com/2016/08/74538da9779e17fa45f0320168c5862c.png)
原来如此，charindex都找不到它，得right一下才把这根刺挑出来。当是char（1）等等的时候是可以的，replace(@a,char(1) ,’a’)
这样是成功的，char（0）对应空字元，不对应数据库的NULL哦。
既然这样，那就用截取字符串的方法把需要的信息拿出来，框架大概如下，套个循环。当然也有可能有更好的方法，仅供参考

MySQL
```
loop
UPDATE  TblName  SET 
 ColName = LEFT(ColName,LEN(ColName)-1)    
 WHERE ASCII(RIGHT( ColName ,1))=0  
end loop
```
下面共享个通用处理指定表中所有列中所有不可见字符的处理脚本，实测可用。

MySQL
```
SET NOCOUNT ON
 DECLARE @TblName  VARCHAR(100)
 DECLARE @UpdateString NVARCHAR(1000)
 DECLARE @SelectString NVARCHAR(1000)
 DECLARE @COlName VARCHAR(100)
 DECLARE @COUNT  INT
 SET @TblName = '表名称'--指定想要修改的表名
 --定义游标取出指定表内的数据类型是VARCHAR,char,nVARCHAR的字段名称
 DECLARE cur_ColName  CURSOR
 FOR
 SELECT col.name
 FROM syscolumns AS col
 inner join sysobjects  AS obj  ON col.ID = obj.ID 
 INNER join systypes  AS typ  ON col.xtype = typ.xtype
 WHERE obj.xtype ='U'
 AND obj.name = @TblName
 AND typ.name IN ('VARCHAR','CHAR','NVARCHAR','NCHAR')
 FOR READ ONLY
 --打开游标
 OPEN cur_ColName
 FETCH NEXT FROM cur_ColName INTO @ColName
 IF @@FETCH_STATUS0
 BEGIN 
 PRINT '没有对应表或字段，'PRINT '请确认当前数据库内有' + @TblName + '表，' PRINT '或该表内有VARCHAR、CHAR、NVARCHAR、NCHAR类型的字段！' GOTO LABCLOSE
 END--循环修改
 WHILE @@FETCH_STATUS=0
 BEGIN 
 --拼修改字符串 
 --去掉左边的不可见字符 
 SET @SelectString = 'SELECT @COU=COUNT(*)     
 FROM ' + @TblName +'    
 WHERE ASCII(LEFT(' + @ColName +',1))'+ @ColName + ' IS NOT NULL' 
 EXEC sp_executesql @SelectString,N'@COU INT OUTPUT',
 @COUNT OUTPUT WHILE @COUNT>0 
 BEGIN  
 SET @UpdateString =   
 ' UPDATE ' + @TblName +   
 ' SET ' + @ColName + '=RIGHT(' + @ColName + ',LEN(' + @ColName + ')-1)    
 WHERE ASCII(LEFT(' + @ColName + ',1))' + @ColName + ' IS NOT NULL'  
 EXEC sp_executesql @UpdateString  
 EXEC sp_executesql @SelectString,N'@COU INT OUTPUT',@COUNT OUTPUT 
 END 
 --去掉右边的不可见字符 
 SET @SelectString = 'SELECT @COU=COUNT(*)     
 FROM ' + @TblName +'    
 WHERE ASCII(RIGHT(' + @ColName +',1))'+ @ColName + ' IS NOT NULL' 
 EXEC sp_executesql @SelectString,N'@COU INT OUTPUT',
 @COUNT OUTPUT WHILE @COUNT>0 
 BEGIN  
 SET @UpdateString =   ' UPDATE ' + @TblName +   ' SET ' 
 + @ColName + '=LEFT(' + @ColName + ',LEN(' + @ColName + ')-1)    
 WHERE ASCII(RIGHT(' + @ColName + ',1))' + @ColName + ' IS NOT NULL'  
 EXEC SP_EXECUTESQL @UpdateString  
 EXEC sp_executesql @SelectString,N'@COU INT OUTPUT',
 @COUNT OUTPUT 
 END 
 PRINT 'column: ' + @ColName + '－－－ok' 
 FETCH NEXT FROM cur_ColName INTO @ColName
 END
 --关闭、释放游标
 LABCLOSE: CLOSE cur_ColName  
 DEALLOCATE cur_ColName
 GO
```
