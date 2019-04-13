
# SQLite的SQL语法 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月03日 10:52:10[seven-soft](https://me.csdn.net/softn)阅读数：516标签：[sqlite																](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)[sql																](https://so.csdn.net/so/search/s.do?q=sql&t=blog)[null																](https://so.csdn.net/so/search/s.do?q=null&t=blog)[function																](https://so.csdn.net/so/search/s.do?q=function&t=blog)[insert																](https://so.csdn.net/so/search/s.do?q=insert&t=blog)[语言																](https://so.csdn.net/so/search/s.do?q=语言&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=insert&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=function&t=blog)个人分类：[SQLite																](https://blog.csdn.net/softn/article/category/1137974)
[
																								](https://so.csdn.net/so/search/s.do?q=function&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=null&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=sql&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=sqlite&t=blog)

## 表达式
|expr|::=|expr|binary-op|expr||
|expr|[|NOT|]|like-op|expr|[|ESCAPE|expr|]||
|unary-op|expr||
|(|expr|)||
|column-name||
|table-name|.|column-name||
|database-name|.|table-name|.|column-name||
|literal-value||
|parameter||
|function-name|(|expr-list|||*|)||
|expr|ISNULL||
|expr|NOTNULL||
|expr|[|NOT|]|BETWEEN|expr|AND|expr||
|expr|[|NOT|]|IN|(|value-list|)||
|expr|[|NOT|]|IN|(|select-statement|)||
|expr|[|NOT|]|IN|[|database-name|.|]|table-name||
|[|EXISTS|]|(|select-statement|)||
|CASE|[|expr|]|(|WHEN|expr|THEN|expr|)+|[|ELSE|expr|]|END||
|CAST|(|expr|AS|type|)|
|like-op|::=|LIKE|||GLOB|||REGEXP|
这一节与其它的各节有所不同。我们讨论的不是一个单一的SQL命令，而是作为其他大部分命令的一部分的表达式。
SQLite支持如下的二元运算符，按优先级由高至低排列：
> IN
AND   
OR
所支持的一元运算符：
注意等号和“不等”号的两个变种。等号可以是=或==. “不等”号可以是!=或<>.||为“连接符”——它将两个字符串连接起来。%输出左边部分以右边部分为模取模得到的余数。
二元运算符的结果均为数字，除了||连接符，它给出字符串结果。
文本值(literal value)是一个整数或浮点数。可以使用科学计数法。"."符号总是被当作小数点即使本地设定中用","来表示小数点 ——用","表示小数点会造成歧义。字符串常量由字符串加单引号(')构成。字符串内部的单引号可像Pascal 中一样用两个单引号来表示。C风格的加反斜线的表示法由于不是标准SQL而不被支持。 BLOB文本是以"x"或"X"开头的含有十六进制文本信息的文本值 For example:
> X'53514697465'
文本值同样可以为"NULL".
表达式中插入文本值占位符的参数可以使用sqlite3_bindAPI函数在运行时插入。参数可以是如下几种形式：
> ?
> NNN
> 问号跟随数字
> NNN
> 为第NNN个参数占位。NNN需介于1和999之间。
|
> ?
> 不加数字的问号为下一个参数占位。
|
> :
> AAAA
> 冒号带一个标识符名称为一个名为AAAA的参数占位。命名的参数同样可以使用序号占位，被赋予的参数序号为下一个尚未被使用的序号。建议不要混合使用命名代表的参数和序号代表的参数以免引起混淆。
|
> $
> AAAA
> $符号带一个标识符名称也可以为一个名为AAAA的参数占位。在这一情况下标识符名称可以包括一个或更多的"::" 以及包含任何文本的"(...)"后缀。该语法是Tcl编程语言中的一个可变形式。
|
不使用sqlite3_bind赋值的参数被视为NULL.
LIKE运算符进行模式匹配比较。运算符右边为进行匹配的模式而左边为需进行匹配的字符串。 模式中的百分号%匹配结果中的零或任意多个字符。下划线_匹配任意单个字符。其他的任意字符匹配本身或等同的大/小写字符。 (即不区分大小写的匹配)。 (一个bug: SQLite仅对7-bit拉丁字符支持不区分大小写匹配。这是由于 LIKE运算符对8-bit iso8859字符或UTF-8字符是大小写敏感的。例如，表达式**'a' LIKE 'A'**的值为真而**'æ' LIKE 'Æ'**为假)。
如果使用可选的ESCAPE子句，则跟随ESCAPE关键字的必须是一个有一个字符的字符串。这一字符(逃逸字符)可用于LIKE模式中，以代替百分号或下划线。逃逸字符后跟百分号，下划线或它本身代表字符串中的百分号，下划线或逃逸字符。插入的LIKE运算符功能通过调用用户函数like(*X*,*Y*)来实现。
当使用可选的ESCAPE子句，它对函数给出第三个参数，LIKE的功能可以通过重载SQL函数like()进行改变。GLOB运算符与LIKE相似，但它使用Unix文件 globbing 语法作为通配符。还有一点不同是GLOB对大小写敏感。 GLOB和LIKE都可以前缀NOT关键字构成相反的意思。插入的GLOB运算符功能通过调用用户函数[
](http://localhost/Htmleditor.asp#globFunc)glob(*X*,*Y*)可以通过重载函数改变GLOB的功能。
REGEXP运算符是用户函数regexp()的一个特殊的代表符号。缺省情况下regexp()函数不被定义，所以使用REGEXP运算符会报错。当运行时存在用户定义的"regexp"函数的定义，则调用该函数以实现REGEXP运算符功能。
字段名可以是CREATE TABLE语句定义的任何名字或如下几个特殊标识符之一"**ROWID**", "**OID**", 以及"**_ROWID_**".这些特殊标识符均代表每个表每一行关联的那个唯一随机整数键("row key")。仅仅在CREATE TABLE语句没有对这些特殊标识符的真实字段予以定义的情况下，它们才代表"row key"。它们与只读字段类似，可以像任何正常字段一样使用，除了在UPDATE或INSERT语句中(即是说你不能添加或更改row
 key)。 "SELECT * ..."不返回row key.
SELECT语句可以在表达式中出现，作为IN运算符的右边运算量，作为一个纯量，或作为EXISTS运算符的运算量。当作纯量或IN的运算量时，SELECT语句的结果仅允许有一个字段，可以使用复合的SELECT(用UNION或 EXCEPT等关键字连接)。作为EXISTS运算符的运算量时，SELECT结果中的字段被忽略，在结果为空时表达式为假，反之为真。若SELECT表达式代表的查询中不含有引用值的部分，则它将在处理其它事务之前被计算，并且结果在必要时会被重复使用。若SELECT表达式含从其它查询中得到的变量，在每一次使用时该表达式均被重新计算。
当SELECT作为IN运算符的右运算量，在左边的运算量是SELECT产生的任意一个值时，表达式返回TRUE。IN运算符前可以加NOT 构成相反的意思。
当SELECT与表达式一同出现且不在IN的右边，则SELECT结果的第一行作为表达式中使用的值。SELECT返回的结果在第一行以后的部分被忽略。返回结果为空时SELECT语句的值为NULL.
CAST表达式将的数据类型改为声明的类型。 可以是CREATE TABLE语句字段定义部分定义的对该字段有效的任意非空数据类型。
表达式支持简单函数和聚集函数。简单函数直接从输入获得结果，可用于任何表达式中。聚集函数使用结果集中的所有行计算结果，仅用于SELECT语句中。
T下面这些函数是缺省可用的。可以使用C语言写出其它的函数然后使用sqlite3_create_function()API函数添加到数据库引擎中。
注意同时重载like()的两参数和三参数版本，否则在使用/不使用 ESCAPE子句时，LIKE运算符的实现可能使用的是不同的代码。|abs(|X|)|返回参数|X|的绝对值。|
|coalesce(|X|,|Y|,...)|返回第一个非空参数的副本。若所有的参数均为NULL，返回NULL。至少2个参数。|
|glob(|X|,|Y|)|用于实现SQLite的 "|X GLOB Y|"语法。可使用|[
](http://localhost/Htmleditor.asp#)|sqlite3_create_function()|重载该函数从而改变|GLOB|运算符的功能。|
|ifnull(|X|,|Y|)|返回第一个非空参数的副本。 若两个参数均为NULL，返回NULL。与上面的|coalesce()|类似。|
|last_insert_rowid()|返回当前数据库连接最后插入行的ROWID。|sqlite_last_insert_rowid()|API函数同样可用于得到该值。|
|length(|X|)|返回|X|的长度，以字符计。如果SQLite被配置为支持UTF-8，则返回UTF-8字符数而不是字节数。|
|like(|X|,|Y|[,|Z|])|用于实现SQL语法"|X LIKE Y [ESCAPE Z]|".若使用可选的ESCAPE子句，则函数被赋予三个参数，否则只有两个。可使用|sqlite3_create_function()|重载该函数从而改变|LIKE|运算符的功能。|
|lower(|X|)|返回|X|字符串的所有字符小写化版本。这一转换使用C语言库的|tolower()|函数，对UTF-8字符不能提供好的支持。|
|max(|X|,|Y|,...)|返回最大值。参数可以不仅仅为数字，可以为字符串。大小顺序由常用的排序法则决定。注意，|max()|在有2个或更多参数时为简单函数，但当仅给出一个参数时它变为聚集函数。|
|min(|X|,|Y|,...)|返回最小值。与max()类似。|
|nullif(|X|,|Y|)|当两参数不同时返回X，否则返回NULL.|
|quote(|X|)|返回参数的适于插入其它SQL语句中的值。字符串会被添加单引号，在内部的引号前会加入逃逸符号。 BLOB被编码为十六进制文本。当前的VACUUM使用这一函数实现。在使用触发器实现撤销/重做功能时这一函数也很有用。|
|random(*)|返回介于-2147483648和 +2147483647之间的随机整数。|
|round(|X|)
|round(|X|,|Y|)|将|X|四舍五入，保留小数点后|Y|位。若忽略|Y|参数，则默认其为0。|
|soundex(|X|)|计算字符串|X|的soundex编码。参数为NULL时返回字符串"?000".缺省的SQLite是不支持该函数的，当编译时选项 -DSQLITE_SOUNDEX=1 时该函数才可用。|
|sqlite_version(*)|返回所运行的SQLite库的版本号字符串。如 "2.8.0"。|
|substr(|X|,|Y|,|Z|)|返回输入字符串|X|中以第|Y|个字符开始，|Z|个字符长的子串。|X|最左端的字符序号为1。若|Y|为负，则从右至左数起。若SQLite配置支持UTF-8，则“字符”代表的是UTF-8字符而非字节。|
|typeof(|X|)|返回表达式|X|的类型。返回值可能为"null", "integer", "real", "text", 以及 "blob". SQLite的类型处理参见|SQLite3的数据类型|.|
|upper(|X|)|返回|X|字符串的所有字符大写化版本。这一转换使用C语言库的|toupper()|函数，对UTF-8字符不能提供好的支持。|
以下是缺省可用的聚集函数列表。可以使用C语言写出其它的聚集函数然后使用sqlite3_create_function()API函数添加到数据库引擎中。
在单参数聚集函数中，参数可以加前缀DISTINCT。这时重复参数会被过滤掉，然后才穿入到函数中。例如，函数"count(distinct X)"返回字段X的不重复非空值的个数，而不是字段X的全部非空值。
|avg(|X|)|返回一组中非空的|X|的平均值。非数字值作0处理。avg()的结果总是一个浮点数，即使所有的输入变量都是整数。|
|count(|X|)
|count(*)|返回一组中|X|是非空值的次数的第一种形式。第二种形式(不带参数)返回该组中的行数。|
|max(|X|)|返回一组中的最大值。大小由常用排序法决定。|
|min(|X|)|返回一组中最小的非空值。大小由常用排序法决定。仅在所有值为空时返回NULL。|
|sum(|X|)
|total(|X|)|返回一组中所有非空值的数字和。若没有非空行，sum()返回NULL而total()返回0.0. NULL通常情况下并不是对于“没有行”的和的一个有意义的结果，但SQL标准如此要求，且大部分其它SQL数据库引擎这样定义sum()，所以SQLite 也如此定义以保证兼容。我们提供非标准的total()函数作为解决该SQL语言设计问题的一个简易方法。|total()的返回值式中为浮点数。sum()可以为整数，当所有非空输入均为整数时，和是精确的。 若sum()的任意一个输入既非整数也非NULL或计算中产生整数类型的溢出时，sum()返回接近真和的浮点数。
|


