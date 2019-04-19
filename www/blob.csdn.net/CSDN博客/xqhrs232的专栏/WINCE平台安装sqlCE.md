# WINCE平台安装sqlCE - xqhrs232的专栏 - CSDN博客
2015年06月19日 09:46:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1793
原文地址::[http://blog.csdn.net/yjg428/article/details/7513860](http://blog.csdn.net/yjg428/article/details/7513860)
相关文章
1、[wince 安装 sql server ce 3.5](http://blog.csdn.net/lanruoshui/article/details/5733573)----[http://blog.csdn.net/lanruoshui/article/details/5733573](http://blog.csdn.net/lanruoshui/article/details/5733573)
2、[在wince7.0上安装sqlce3.5](http://blog.csdn.net/ren062/article/details/22266617)----[http://blog.csdn.net/ren062/article/details/22266617](http://blog.csdn.net/ren062/article/details/22266617)
3、NETCFv2.wce5.armv4i.cab的安装----[http://bbs.csdn.net/topics/360074008](http://bbs.csdn.net/topics/360074008)
4、Wince 的下怎么安装sqlce数据库----[http://bbs.csdn.net/topics/300075672](http://bbs.csdn.net/topics/300075672)
5、[WINCE6.0 CAB文件的制作与安装](http://blog.csdn.net/loongembedded/article/details/8272350)----[http://blog.csdn.net/loongembedded/article/details/8272350](http://blog.csdn.net/loongembedded/article/details/8272350)
# NETCFv2.wce5.armv
WINCE平台安装sqlCE
首先到微软官方下载sqlCE 3.0
下载地址：[http://www.microsoft.com/downloads/details.aspx?displaylang=zh-cn&FamilyID=e9aa3f8d-363d-49f3-ae89-64e1d149e09b](http://www.microsoft.com/downloads/details.aspx?displaylang=zh-cn&FamilyID=e9aa3f8d-363d-49f3-ae89-64e1d149e09b)
先在PC机上安装sdk。
当然由于需要将CAB包安装到嵌入式目标跑平台，Microsoft ActiveSync也需要安装，Microsoft ActiveSync微软官方也可以下载到最新版本是V4.2的。
然后找到此目录：\Microsoft SQL Server Compact Edition\v3.1\SDK\bin\wce500\，然后针对自己的嵌入式目标平台来选择。
我的平台是arm，所以选择armv4i
将armv4i这个文件夹通过Microsoft ActiveSync复制到目标平台
一次双击安装：(当然，我这里只是针对wince 5.0，如果你的平台式ppc等，需要选择ppc的CAB)
sqlce30.wce5.armv4i.CAB
sqlce30.repl.wce5.armv4i.CAB
sqlce30.dev.CHS.wce5.armv4i.CAB
由于这些包安装需要依赖于很多dll文件，所以，不能只拷贝这3个文件到目标平台，最好是把armv4i全部拷贝过来。
安装的过程中，不要指定安装路径，直接安装在目标平台\windows目录下，因为，当用编程方式操作数据库的时候，默认会在\windows目录下找。
到此为止，sqlCE就在嵌入式目标平台上安装成功了。
你可能注意到了，Microsoft的帮助文件中，提到，如果使用visual studio 2005编写程序数据库程序，VS 2005会自动将sqlCE安装到嵌入式目标平台。经过我自己的考察和研究，Microsoft描述的都是针对用VS 2005 创建C#或者VB工程。如果你使用embedded VC++ 即 EVC或者 VS中创建VC ++工程，那么，他是不会自动下载安装的。
所以，如果你使用VC++来编写数据库应用程序的话，需要手动去安装，即我上面描述的方法。
而且，也不是Microsoft提到的那样每次都去安装，只要安装一次就ok，如果你安装路径是\windows。
sqlCE优于wince 自带数据库的特点：
类别 对象 最大大小限制 
存储
列名
128 个字符
表中的列数
1024
行大小
8060 字节
数据库密码
40 个字符
数据库大小
4 GB 1 
数据库大小增量
增量为 1 页或 16 页（取决于表大小）
页大小
4 KB
会话数
256
BLOB（ntext 和 image）列的大小
2 GB
表名
128 个字符
表大小
512 MB
查询
SQL 语句中的字符数
无限制
游标中的列数
1024
ORDER BY、GROUP BY 或 DISTINCT 子句中的列数
10242 
嵌套子查询的层数
无限制
命名的参数
支持
查询中操作数的数量
无限制
联接中表的数量
无限制
索引 
BLOB 列
无法索引
索引键中的字节数
5123 
索引中的列数
16
每个表的索引数
249
约束
PRIMARY KEY、UNIQUE、默认约束和 FOREIGN KEY
支持 4 
每个表的约束数
249
上表中，我们最为关心的数据，应该是支持的列数和数据量，wince自带数据库最多支持4个字段，而且数据量很有限，而且操作非常复杂。
SQL Server Compact Edition 支持下列数据类型：
数据类型 说明 
bigint 
整数数据，从 –2^63 (–9,223,372,036,854,775,808) 到 2^63–1 (9,223,372,036,854,775,807)。存储大小为 8 字节。
integer 
整数数据，从 –2^31 (-2,147,483,648) 到 2^31–1 (2,147,483,647)。
存储大小为 4 字节。
smallint 
整数数据，从 –32,768 到 32,767。存储大小为 2 字节。
tinyint 
整数数据，从 0 到 255。存储大小为 1 字节。
bit 
整数数据，值为 1 或 0。
存储大小为 1 位。
numeric (p, s) 
精度和小数位数固定的数值数据，取值范围从 –10^38+1 到 10^38–1。p 变量指定精度，取值范围从 1 到 38。s 变量指定小数位数，取值范围从 0 到 p。
存储大小为 19 字节。
money 
货币数据值，从 (–2^63/10000) (–922,337,203,685,477.5808) 到 2^63–1 (922,337,203,685,477.5807)，准确度为货币单位的万分之一。存储大小为 8 字节。
float 
浮点数数据，从 –1.79E +308 到 1.79E+308
存储大小为 8 字节。
real 
浮点精度数字数据，从 –3.40E+38 到 3.40E+38。
存储大小为 4 字节。
datetime 
日期和时间数据，从 1753 年 1 月 1 日到 9999 年 12 月 31 日，准确度为三百分之一秒或 3.33 毫秒。值被圆整到 .000、.003 或 .007 毫秒增量。
存储为两个 4 字节整数。前 4 个字节存储早于或晚于 base date 1900 年 1 月 1 日的天数。基准日期是系统的参照日期。不允许 datetime 的值早于 1753 年 1 月 1 日。后 4 个字节存储一天之中的具体时间，被表示为从午夜算起的毫秒数。秒数的有效范围是 0–59。
格式 示例 
YYYY/MM/DD HH:MM:SS
1947/08/15 03:33:20
MM/DD/YYYY HH:MM:SS
04/15/1947 03:33:20
DD MMM YYYY HH:MM:SS
15 Jan 1947 03:33:20
DD MMMM YYYY H:MM:SS
15 January 1947 03:33:20
national character(n)
Synonym:nchar(n) 
固定长度的 Unicode 数据，最大长度为 4000 个字符。默认长度 = 1。存储大小（以字节计）是输入的字符数的两倍。
national character varying(n)
Synonym:nvarchar(n) 
可变长度的 Unicode 数据，长度值范围为从 1 到 4000 个字符。默认长度 = 1。存储大小（以字节计）是输入的字符数的两倍。
ntext¹ 
可变长度的 Unicode 数据，最大长度为 (2^30–2)/2 (536,870,911) 个字符。存储大小（以字节计）是输入的字符数的两倍。
注意： 
字符串函数中不再支持 ntext。 
nchar 
n 个字符组成的固定长度的 Unicode 字符数据。n 必须是从 1 到 4,000 的值。存储大小是 n 字节的两倍。
binary(n) 
固定长度的二进制数据，最大长度为 8000 字节。默认长度 = 1。
存储大小是固定的，是在类型中声明的以字节为单位的长度。
varbinary(n) 
可变长度的二进制数据，最大长度为 8000 字节。默认长度 = 1。
存储大小可变。它表示值的长度（以字节为单位）。
image¹ 
可变长度的二进制数据，最大长度为 2^30–1 (1,073,741,823) 字节。
存储大小是值的以字节为单位的长度。
uniqueidentifier 
全局唯一标识符 (GUID)。存储大小为 16 字节。
IDENTITY [(s, i)] 
这是数据列的一个属性，而不是一个独特的数据类型。
只有整数数据类型的数据列可用于标识列。一个表只能有一个标识列。可以指定种子和增量，但不能更新列。
s (seed) = 起始值
i (increment) = 增量值
ROWGUIDCOL 
这是数据列的一个属性，而不是一个独特的数据类型。它是一个表中使用 uniqueidentifier 数据类型定义的列。一个表只能有一个 ROWGUIDCOL 列。
¹在 SQL Server Compact Edition 中，当字节数超过 256 时，Ntext 和 image 数据将存储于新的数据页中。由于 SQL Server Compact Edition 数据库可以按页而不是字节进行压缩，因此这会影响数据库的压缩程度。
4i.cab的安装
