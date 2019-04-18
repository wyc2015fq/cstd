# 使用Replace into 生成全局唯一主键（flickr） - z69183787的专栏 - CSDN博客
2017年03月13日 15:17:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2024
`CREATE``TABLE```Sequence_test_table``
```
(
 `id`
```
`bigint``(20) ``NOT``NULL``AUTO_INCREMENT,`
` ```stub` ``char``(1) ``NOT``NULL``,`
`   ```UpdateTime` ``timestamp``NOT``NULL``DEFAULT``CURRENT_TIMESTAMP``ON``UPDATE``CURRENT_TIMESTAMP``,`
`PRIMARY``KEY``(`id`),``UNIQUE``KEY`
```
`idx_stub`
 (`stub`),
```
`KEY`
```
`IX_UpdateTime`
 (`UpdateTime`)
```
```
)
 ENGINE=InnoDB
```
`DEFAULT``CHARSET=utf8;`
通过使用mysql的自增id来产生相应的id,stub字段只是用于存储1字符数据的字段（供replace into 删除数据使用），无实际意义。
sqlMap.xml
折叠原码
```
```xml
<
```
```xml
sqlMap
```
```xml
namespace
```
```xml
=
```
```xml
"IdGenerator"
```
```xml
>
```
```xml
```
```xml
<
```
```xml
select
```
```xml
id
```
```xml
=
```
```xml
"count"
```
```xml
parameterClass
```
```xml
=
```
```xml
"map"
```
```xml
resultClass
```
```xml
=
```
```xml
"java.lang.Integer"
```
```xml
>
```
```xml
```
```xml
SELECT
 COUNT(*) FROM Sequence_$bizType$_table
```
```xml
```
```xml
</
```
```xml
select
```
```xml
>
```
```xml
```
```xml
<
```
```xml
insert
```
```xml
id
```
```xml
=
```
```xml
"generate"
```
```xml
parameterClass
```
```xml
=
```
```xml
"map"
```
```xml
>
```
```xml
```
```xml
REPLACE
 INTO Sequence_$bizType$_table (stub) VALUES ('1');
```
```xml
```
```xml
<
```
```xml
selectKey
```
```xml
resultClass
```
```xml
=
```
```xml
"java.lang.Integer"
```
```xml
keyProperty
```
```xml
=
```
```xml
"Id"
```
```xml
>
```
```xml
```
```xml
SELECT
 LAST_INSERT_ID() AS id
```
```xml
```
```xml
</
```
```xml
selectKey
```
```xml
>
```
```xml
```
```xml
</
```
```xml
insert
```
```xml
>
```
```xml
</
```
```xml
sqlMap
```
```xml
>
```
```xml
```
```
传入相应bizType获取对应生成的ID

REPLACE的运行与INSERT很相似。只有一点例外，假如表中的一个旧记录与一个用于PRIMARY
KEY或一个UNIQUE索引的新记录具有相同的值，则在新记录被插入之前，旧记录被删除。 
注意，除非表有一个PRIMARY KEY或UNIQUE索引，否则，使用一个REPLACE语句没有意义。该
语句会与INSERT相同，因为没有索引被用于确定是否新行复制了其它的行。
所有列的值均取自在REPLACE语句中被指定的值。所有缺失的列被设置为各自的默认值，这
和INSERT一样。您不能从当前行中引用值，也不能在新行中使用值。如果您使用一个例如“
SET col_name = col_name + 1”的赋值，则对位于右侧的列名称的引用会被作为DEFAULT
(col_name)处理。因此，该赋值相当于SET col_name = DEFAULT(col_name) + 1。
为了能够使用REPLACE，您必须同时拥有表的INSERT和DELETE权限。
REPLACE语句会返回一个数，来指示受影响的行的数目。该数是被删除和被插入的行数的和
。如果对于一个单行REPLACE该数为1，则一行被插入，同时没有行被删除。如果该数大于1
，则在新行被插入前，有一个或多个旧行被删除。如果表包含多个唯一索引，并且新行复制
了在不同的唯一索引中的不同旧行的值，则有可能是一个单一行替换了多个旧行。
受影响的行数可以容易地确定是否REPLACE只添加了一行，或者是否REPLACE也替换了其它行
：检查该数是否为1（添加）或更大（替换）。
1. 尝试把新行插入到表中
2. 当因为对于主键或唯一关键字出现重复关键字错误而造成插入失败时：
a. 从表中删除含有重复关键字值的冲突行
b. 再次尝试把新行插入到表中
REPLACE [LOW_PRIORITY | DELAYED]
[INTO] tbl_name [(col_name,...)]
VALUES ({expr | DEFAULT},…),(…),…
或：
REPLACE [LOW_PRIORITY | DELAYED]
[INTO] tbl_name
SET col_name={expr | DEFAULT}, …
或：
REPLACE [LOW_PRIORITY | DELAYED]
[INTO] tbl_name [(col_name,...)]
SELECT …
REPLACE INTO `table` (`unique_column`,`num`) VALUES ('$unique_value',$num);跟INSERT INTO `table` (`unique_column`,`num`) VALUES('$unique_value',$num) ON DUPLICATE UPDATE num=$num;还是有些区别的.
区别就是replace into的时候会删除老记录。如果表中有一个自增的主键。
那么就要出问题了。
首先，因为新纪录与老记录的主键值不同，所以其他表中所有与本表老数据主键id建立的关联全部会被破坏。
其次，就是，频繁的REPLACE INTO 会造成新纪录的主键的值迅速增大。
总有一天。达到最大值后就会因为数据太大溢出了。就没法再插入新纪录了。数据表满了，不是因为空间不够了，而是因为主键的值没法再增加了。
