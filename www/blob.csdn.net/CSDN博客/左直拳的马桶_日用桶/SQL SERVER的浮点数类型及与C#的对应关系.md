# SQL SERVER的浮点数类型及与C#的对应关系 - 左直拳的马桶_日用桶 - CSDN博客
2013年06月19日 16:36:43[左直拳](https://me.csdn.net/leftfist)阅读数：4339
**SQL SERVER:**
float 与 real
7位数或15位数。这里说的位数，不是指小数位，而是包括整数和小数在内的位数。
float的位数是多少，要看float[(n)]里的n数值是多少。
|*n *value|精度|存储大小|
|----|----|----|
|**1-24**|7 位数|4 字节|
|**25-53**|15 位数|8 字节|
注意：SQL Server 将 *n* 视为下列两个可能值之一。如果 **1**<=n<=**24**，则将 *n* 视为 **24**。如果 **25**<=n<=**53**，则将 *n* 视为 **53**。(是不是很怪异)
如果不写这个n，默认n是53，即15位。
**real** 的 ISO 同义词为 **float(24),**也就是7位了。
```
declare @f float(18) = 12345.0123456789
	,@f2 float(18) = 1234567.0123456789
	,@f3 float(18) = 123456789.0123456789
	,@f4 float(53) = 123456789.01234567890123456789;
declare @r real = 1234567.01234567890123456789;
select @f as [f],@f2 as [f2],@f3 as [f3],@f4 as [f4],@r as [real];
```
f                 f2                 f3                         f4                                real
12345.0112345671.234568E+08123456789.0123461234567
decimal和numeric
带固定精度和小数位数的数值数据类型。
- **decimal**[ **(***p*[ **,***s*] **)**] 和 **numeric**[ **(***p*[ **,***s*] **)**]
固定精度和小数位数。使用最大精度时，有效值从 - 10^38 +1 到 10^38 - 1。**decimal** 的 ISO 同义词为 **dec** 和 **dec(***p*, *s***)**。**numeric** 在功能上等价于 **decimal**。
- p（精度）
最多可以存储的十进制数字的总位数，包括小数点左边和右边的位数。该精度必须是从 1 到最大精度 38 之间的值。默认精度为 18。
- *s *（小数位数）
小数点右边可以存储的十进制数字的最大位数。小数位数必须是从 0 到 *p* 之间的值。仅在指定精度后才可以指定小数位数。默认的小数位数为 0；因此，0 <= *s* <= *p*。最大存储大小基于精度而变化。
|精度|存储字节数|
|----|----|
|1 - 9|5|
|10-19|9|
|20-28|13|
|29-38|17|
可见，decimal和numeric可以指定小数位数。如果不指定，默认是不带小数。
```
declare @d decimal = 12.0123456789,@d2 decimal(5,3) = 12.0123456789;
select @d as [d],@d2 as [d2];
```
dd21212.012
money和smallmoney
代表货币或货币值的数据类型。
|数据类型|范围|存储|
|----|----|----|
|**money**|-922,337,203,685,477.5808 到 922,337,203,685,477.5807|8 字节|
|**smallmoney **|-214,748.3648 到 214,748.3647|4 字节|
注释
**money** 和 **smallmoney** 数据类型精确到它们所代表的货币单位的万分之一。
也就是说，money和smallmoney的小数位固定是4位。
```
declare @m money = 123456789.0123456789;
select @m as [money];
```
money
123456789.0123
C#
float
float关键字表示存储 32 位浮点值的简单类型。 下表显示了 float 类型的精度和大致范围。
|类型|大致范围|精度|.NET Framework 类型|
|----|----|----|----|
|float|-3.4 × 1038 到 +3.4 × 1038|7 位|[System.Single](http://msdn.microsoft.com/zh-cn/library/vstudio/system.single.aspx)|
double
double关键字表示存储 64 位浮点值的简单类型。 下表显示了 double 类型的精度和大致范围。
|类型|大致范围|精度|.NET Framework 类型|
|----|----|----|----|
|double|±5.0 × 10−324 到 ±1.7 × 10308|15 到 16 位|[System.Double](http://msdn.microsoft.com/zh-cn/library/vstudio/system.double.aspx)|
decimal
decimal关键字表示 128 位数据类型。 同浮点型相比，decimal 类型具有更高的精度和更小的范围，这使它适合于财务和货币计算。decimal类型的大致范围和精度如下表所示。
|类型|大致范围|精度|.NET Framework 类型|
|----|----|----|----|
|decimal|(-7.9 x 1028 - 7.9 x 1028) / (100 - 28)|28 到 29 位有效位|[System.Decimal](http://msdn.microsoft.com/zh-cn/library/vstudio/system.decimal.aspx)|
结论
如果只是用于价格、款项之类的数据，SQL SERVER这边可以用smallmoney、money、或者精确一些，用decimal[(p[,s])]来限定整数位数和小数位数。与之相对应，C#这里可以用decimal来赋值。C#里面，decimal类型的话，数值后面要带m或M，如0.01m。
=======================================================================================
网上有高人做了一个列表
<Language From="SQL" To="C#">
   <Type From="bigint" To="long" />
   <Type From="binary" To="object" />
   <Type From="bit" To="bool" />
   <Type From="char" To="string" />
   <Type From="datetime" To="DateTime" />
   <Type From="decimal" To="decimal" />
   <Type From="float" To="double" />
   <Type From="image" To="byte[]" />
   <Type From="int" To="int" />
   <Type From="money" To="decimal" />
   <Type From="nchar" To="string" />
   <Type From="ntext" To="string" />
   <Type From="numeric" To="decimal" />
   <Type From="nvarchar" To="string" />
   <Type From="real" To="float" />
   <Type From="smalldatetime" To="DateTime" />
   <Type From="smallint" To="short" />
   <Type From="smallmoney" To="decimal" />
   <Type From="text" To="string" />
   <Type From="timestamp" To="byte[]" />
   <Type From="tinyint" To="byte" />
   <Type From="uniqueidentifier" To="Guid" />
   <Type From="varbinary" To="byte[]" />
   <Type From="varchar" To="string" />
   <Type From="xml" To="string" />
   <Type From="sql_variant" To="object" />
</Language>
<Language From="SQL" To="C# System Types">
   <Type From="bigint" To="System.Int64" />
   <Type From="binary" To="System.Object" />
   <Type From="bit" To="System.Boolean" />
   <Type From="char" To="System.String" />
   <Type From="datetime" To="System.DateTime" />
   <Type From="decimal" To="System.Decimal" />
   <Type From="float" To="System.Double" />
   <Type From="image" To="System.Byte[]" />
   <Type From="int" To="System.Int32" />
   <Type From="money" To="System.Decimal" />
   <Type From="nchar" To="System.String" />
   <Type From="ntext" To="System.String" />
   <Type From="numeric" To="System.Decimal" />
   <Type From="nvarchar" To="System.String" />
   <Type From="real" To="System.Single" />
   <Type From="smalldatetime" To="System.DateTime" />
   <Type From="smallint" To="System.Int16" />
   <Type From="smallmoney" To="System.Decimal" />
   <Type From="text" To="System.String" />
   <Type From="timestamp" To="System.Byte[]" />
   <Type From="tinyint" To="System.Byte" />
   <Type From="uniqueidentifier" To="System.Guid" />
   <Type From="varbinary" To="System.Byte[]" />
   <Type From="varchar" To="System.String" />
   <Type From="xml" To="System.String" />
   <Type From="sql_variant" To="System.Object" />
</Language>
<DbTarget From="SQL" To="SqlClient">
   <Type From="bigint" To="SqlDbType.BigInt" />
   <Type From="binary" To="SqlDbType.Binary" />
   <Type From="bit" To="SqlDbType.Bit" />
   <Type From="char" To="SqlDbType.Char" />
   <Type From="datetime" To="SqlDbType.DateTime" />
   <Type From="decimal" To="SqlDbType.Decimal" />
   <Type From="float" To="SqlDbType.Float" />
   <Type From="image" To="SqlDbType.Image" />
   <Type From="int" To="SqlDbType.Int" />
   <Type From="money" To="SqlDbType.Money" />
   <Type From="nchar" To="SqlDbType.NChar" />
   <Type From="ntext" To="SqlDbType.NText" />
   <Type From="numeric" To="SqlDbType.Decimal" />
   <Type From="nvarchar" To="SqlDbType.NVarChar" />
   <Type From="real" To="SqlDbType.Real" />
   <Type From="smalldatetime" To="SqlDbType.SmallDateTime" />
   <Type From="smallint" To="SqlDbType.SmallInt" />
   <Type From="smallmoney" To="SqlDbType.SmallMoney" />
   <Type From="text" To="SqlDbType.Text" />
   <Type From="timestamp" To="SqlDbType.Timestamp" />
   <Type From="tinyint" To="SqlDbType.TinyInt" />
   <Type From="uniqueidentifier" To="SqlDbType.UniqueIdentifier" />
   <Type From="varbinary" To="SqlDbType.VarBinary" />
   <Type From="varchar" To="SqlDbType.VarChar" />
   <Type From="xml" To="SqlDbType.Xml" />
   <Type From="sql_variant" To="SqlDbType.Variant" />
</DbTarget>
<DbTarget From="SQLCE" To="SqlServerCe">
   <Type From="bigint" To="SqlDbType.BigInt" />
   <Type From="binary" To="SqlDbType.Binary" />
   <Type From="bit" To="SqlDbType.Bit" />
   <Type From="char" To="SqlDbType.Char" />
   <Type From="datetime" To="SqlDbType.DateTime" />
   <Type From="decimal" To="SqlDbType.Decimal" />
   <Type From="float" To="SqlDbType.Float" />
   <Type From="image" To="SqlDbType.Image" />
   <Type From="int" To="SqlDbType.Int" />
   <Type From="money" To="SqlDbType.Money" />
   <Type From="nchar" To="SqlDbType.NChar" />
   <Type From="ntext" To="SqlDbType.NText" />
   <Type From="numeric" To="SqlDbType.Decimal" />
   <Type From="nvarchar" To="SqlDbType.NVarChar" />
   <Type From="real" To="SqlDbType.Real" />
   <Type From="smalldatetime" To="SqlDbType.SmallDateTime" />
   <Type From="smallint" To="SqlDbType.SmallInt" />
   <Type From="smallmoney" To="SqlDbType.SmallMoney" />
   <Type From="text" To="SqlDbType.Text" />
   <Type From="timestamp" To="SqlDbType.Timestamp" />
   <Type From="tinyint" To="SqlDbType.TinyInt" />
   <Type From="uniqueidentifier" To="SqlDbType.UniqueIdentifier" />
   <Type From="varbinary" To="SqlDbType.VarBinary" />
   <Type From="varchar" To="SqlDbType.VarChar" />
   <Type From="xml" To="SqlDbType.Xml" />
   <Type From="sql_variant" To="SqlDbType.Variant" />
</DbTarget>
（源文章：http://www.cnblogs.com/eng308/archive/2010/06/01/1748824.html）
