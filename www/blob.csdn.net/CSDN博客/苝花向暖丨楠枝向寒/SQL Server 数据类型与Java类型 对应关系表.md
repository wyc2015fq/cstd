# SQL Server 数据类型与Java类型 对应关系表 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月10日 13:57:53[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：305


|SQL Server 类型|JDBC 类型 (java.sql.Types)|Java 语言类型|
|----|----|----|
|bigint|BIGINT|long|
|timestampbinary|BINARY|byte[]|
|bit|BIT|boolean|
|char|CHAR|String|
|decimalmoneysmallmoney|DECIMAL|java.math.BigDecimal|
|float|DOUBLE|double|
|int|INTEGER|int|
|imagevarbinary(max)|LONGVARBINARY|byte[]|
|varchar(max)text|LONGVARCHAR|String|
|nchar|CHARNCHAR (Java SE 6.0)|String|
|nvarchar|VARCHARNVARCHAR (Java SE 6.0)|String|
|nvarchar(max)ntext|LONGVARCHARLONGNVARCHAR (Java SE 6.0)|String|
|numeric|NUMERIC|java.math.BigDecimal|
|real|REAL|float|
|smallint|SMALLINT|short|
|datetimesmalldatetime|TIMESTAMP|java.sql.Timestamp|
|varbinaryudt|VARBINARY|byte[]|
|varchar|VARCHAR|String|
|tinyint|TINYINT|short|
|uniqueidentifier|CHAR|String|
|xml|LONGVARCHARSQLXML (Java SE 6.0)|StringSQLXML|
|time|TIME (1)|java.sql.Time (1)|
|date|DATE|java.sql.Date|
|datetime2|TIMESTAMP|java.sql.Timestamp|
|datetimeoffset (2)|microsoft.sql.Types.DATETIMEOFFSET|microsoft.sql.DateTimeOffset|

