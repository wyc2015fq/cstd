# mysql中数据类型的长度 INTEGER, INT, SMALLINT, TINYINT, MEDIUMINT, BIGINT - z69183787的专栏 - CSDN博客
2018年02月27日 10:49:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：787
[https://dev.mysql.com/doc/refman/5.7/en/numeric-type-overview.html](https://dev.mysql.com/doc/refman/5.7/en/numeric-type-overview.html)
[https://www.cnblogs.com/stringzero/p/5707467.html](https://www.cnblogs.com/stringzero/p/5707467.html)
Table 11.1 Required Storage and Range for Integer Types Supported by MySQL
|Type|Storage (Bytes)|Minimum Value Signed|Minimum Value Unsigned|Maximum Value Signed|Maximum Value Unsigned|
|----|----|----|----|----|----|
|`TINYINT`|1|`-128`|`0`|`127`|`255`|
|`SMALLINT`|2|`-32768`|`0`|`32767`|`65535`|
|`MEDIUMINT`|3|`-8388608`|`0`|`8388607`|`16777215`|
|`INT`|4|`-2147483648`|`0`|`2147483647`|`4294967295`|
|`BIGINT`|8|`-2^63`|`0`|`2^63-1`|`2^64-1`|
因为最近要登记一些长度较大的数值，今天才仔细的查看了bigint的范围。
以前都忽略了mysql中数据长度的上限问题。
在mysql中创建表时，varchar类型必须指定长度，int类型可以不指定长度。当然，在mysql中使用界面创建表（不使用sql语句创建表），若不指定长度，它会为你指定默认长度，下面对mysql中常见的几种数据类型的默认长度和最大长度进行了总结。
|数据类型|默认长度|最大长度|
|----|----|----|
|varchar|255|65535|
|char|255|65535|
|double|0|255|
|floa t|0|255|
|int|11|255|
|tinyint|4|255|
|bigint|20|255|
|date|0|0|
|datetime|0|0|
|text|0|0|
数字类型
|类型|大小|范围（有符号）|范围（无符号）|用途|
|----|----|----|----|----|
|TINYINT|1 字节|(-128，127)|(0，255)|小整数值|
|SMALLINT|2 字节|(-32 768，32 767)|(0，65 535)|大整数值|
|MEDIUMINT|3 字节|(-8 388 608，8 388 607)|(0，16 777 215)|大整数值|
|INT或INTEGER|4 字节|(-2 147 483 648，2 147 483 647)|(0，4 294 967 295)|大整数值|
|BIGINT|8 字节|(-9 233 372 036 854 775 808，9 223 372 036 854 775 807)|(0，18 446 744 073 709 551 615)|极大整数值|
|FLOAT|4 字节|(-3.402 823 466 E+38，1.175 494 351 E-38)，0，(1.175 494 351 E-38，3.402 823 466 351 E+38)|0，(1.175 494 351 E-38，3.402 823 466 E+38)|单精度浮点数值|
|DOUBLE|8 字节|(1.797 693 134 862 315 7 E+308，2.225 073 858 507 201 4 E-308)，0，(2.225 073 858 507 201 4 E-308，1.797 693 134 862 315 7 E+308)|0，(2.225 073 858 507 201 4 E-308，1.797 693 134 862 315 7 E+308)|双精度浮点数值|
|DECIMAL|对DECIMAL(M,D) ，如果M>D，为M+2否则为D+2|依赖于M和D的值|依赖于M和D的值|小数值|
字符类型
|CHAR|0-255字节|定长字符串|
|----|----|----|
|VARCHAR|0-255字节|变长字符串|
|TINYBLOB|0-255字节|不超过 255 个字符的二进制字符串|
|TINYTEXT|0-255字节|短文本字符串|
|BLOB|0-65 535字节|二进制形式的长文本数据|
|TEXT|0-65 535字节|长文本数据|
|MEDIUMBLOB|0-16 777 215字节|二进制形式的中等长度文本数据|
|MEDIUMTEXT|0-16 777 215字节|中等长度文本数据|
|LOGNGBLOB|0-4 294 967 295字节|二进制形式的极大文本数据|
|LONGTEXT|0-4 294 967 295字节|极大文本数据|
枚举、集合
ENUM （最多65535个成员） 64KB
SET （最多64个成员） 64KB
时间类型
|类型|大小(字节)|范围|格式|用途|
|----|----|----|----|----|
|DATE|3|1000-01-01/9999-12-31|YYYY-MM-DD|日期值|
|TIME|3|'-838:59:59'/'838:59:59'|HH:MM:SS|时间值或持续时间|
|YEAR|1|1901/2155|YYYY|年份值|
|DATETIME|8|1000-01-01 00:00:00/9999-12-31 23:59:59|YYYY-MM-DD HH:MM:SS|混合日期和时间值|
|TIMESTAMP|8|1970-01-01 00:00:00/2037 年某时|YYYYMMDD HHMMSS|混合日期和时间值，时间戳|
