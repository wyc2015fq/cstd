# 理解Mysql prepare预处理语句：mysql_stmt_init(MYSQL *mysql); - Koma Hub - CSDN博客
2019年03月13日 22:43:56[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：33
个人分类：[MySQL](https://blog.csdn.net/Rong_Toa/article/category/7349084)
MySQL 5.1对服务器一方的预制语句提供支持。如果您使用合适的客户端编程界面，则这种支持可以发挥在MySQL 4.1中实施的高效客户端/服务器二进制协议的优势。候选界面包括MySQL C API客户端库（用于C程序）、MySQL Connector/J（用于Java程序）和MySQL Connector/NET。例如，C API可以提供一套能组成预制语句API的函数调用。其它语言界面可以对使用了二进制协议（通过在C客户端库中链接）的预制语句提供支持。对预制语句，还有一个SQL界面可以利用。与在整个预制语句API中使用二进制协议相比，本界面效率没有那么高，但是它不要求编程，因为在SQL层级，可以**直接利用本界面**：
- · 当您无法利用编程界面时，您可以使用本界面。
- · 有些程序允许您发送SQL语句到将被执行的服务器中，比如mysql客户端程序。您可以从这些程序中使用本界面。
- · 即使客户端正在使用旧版本的客户端库，您也可以使用本界面。唯一的要求是，您能够连接到一个支持预制语句SQL语法的服务器上。
- 预制语句的SQL语法在以下情况下使用：
- · 在编代码前，您想要测试预制语句在您的应用程序中运行得如何。或者也许一个应用程序在执行预制语句时有问题，您想要确定问题是什么。
- · 您想要创建一个测试案例，该案例描述了您使用预制语句时出现的问题，以便您编制程序错误报告。
- · 您需要使用预制语句，但是您无法使用支持预制语句的编程API。
预制语句的SQL语法基于三个SQL语句：
|12345|`PREPARE``stmt_name ``FROM``preparable_stmt;`` ``EXECUTE``stmt_name [USING @var_name [, @var_name] ...];`` ``{``DEALLOCATE``| ``DROP``} ``PREPARE``stmt_name;`|
PREPARE语句用于预备一个语句，并赋予它名称stmt_name，借此在以后引用该语句。语句名称对案例不敏感。preparable_stmt可以是一个文字字符串，也可以是一个包含了语句文本的用户变量。该文本必须展现一个单一的SQL语句，而不是多个语句。使用本语句，‘?'字符可以被用于制作参数，以指示当您执行查询时，数据值在哪里与查询结合在一起。‘?'字符不应加引号，即使您想要把它们与字符串值结合在一起，也不要加引号。参数制作符只能被用于数据值应该出现的地方，不用于SQL关键词和标识符等。
如果带有此名称的预制语句已经存在，则在新的语言被预备以前，它会被隐含地解除分配。这意味着，如果新语句包含一个错误并且不能被预备，则会返回一个错误，并且不存在带有给定名称语句。
预制语句的范围是客户端会话。在此会话内，语句被创建。其它客户端看不到它。
在预备了一个语句后，您可使用一个EXECUTE语句（该语句引用了预制语句名称）来执行它。如果预制语句包含任何参数制造符，则您必须提供一个列举了用户变量（其中包含要与参数结合的值）的USING子句。参数值只能有用户变量提供，USING子句必须准确地指明用户变量。用户变量的数目与语句中的参数制造符的数量一样多。
您可以多次执行一个给定的预制语句，在每次执行前，把不同的变量传递给它，或把变量设置为不同的值。
要对一个预制语句解除分配，需使用DEALLOCATE PREPARE语句。尝试在解除分配后执行一个预制语句会导致错误。
如果您终止了一个客户端会话，同时没有对以前已预制的语句解除分配，则服务器会自动解除分配。
以下SQL语句可以被用在预制语句中：CREATE TABLE, DELETE, DO, INSERT, REPLACE, SELECT, SET, UPDATE和多数的SHOW语句。目前不支持其它语句。
以下例子显示了预备一个语句的两种方法。该语句用于在给定了两个边的长度时，计算三角形的斜边。
第一个例子显示如何通过使用文字字符串来创建一个预制语句，以提供语句的文本：
|12345678910|`mysql> ``PREPARE``stmt1 ``FROM``'SELECT SQRT(POW(?,2) + POW(?,2)) AS hypotenuse'``;``mysql> ``SET``@a = 3;``mysql> ``SET``@b = 4;``mysql> ``EXECUTE``stmt1 USING @a, @b;``+``------------+``| hypotenuse |``+``------------+``|     5      |``+``------------+``mysql> ``DEALLOCATE``PREPARE``stmt1;`|
第二个例子是相似的，不同的是提供了语句的文本，作为一个用户变量：
|1234567891011|`mysql> ``SET``@s = ``'SELECT SQRT(POW(?,2) + POW(?,2)) AS hypotenuse'``;``mysql> ``PREPARE``stmt2 ``FROM``@s;``mysql> ``SET``@a = 6;``mysql> ``SET``@b = 8;``mysql> ``EXECUTE``stmt2 USING @a, @b;``+``------------+``| hypotenuse |``+``------------+``|     10     |``+``------------+``mysql> ``DEALLOCATE``PREPARE``stmt2;`|
对于已预备的语句，您可以使用位置保持符。以下语句将从tb1表中返回一行：
|12345|`mysql> ``SET``@a=1;``mysql> ``PREPARE``STMT ``FROM``"SELECT * FROM tbl LIMIT ?"``;``mysql> ``EXECUTE``STMT USING @a;`|
以下语句将从tb1表中返回第二到第六行：
|12345|`mysql> ``SET``@skip=1; ``SET``@numrows=5;``mysql> ``PREPARE``STMT ``FROM``"SELECT * FROM tbl LIMIT ?, ?"``;``mysql> ``EXECUTE``STMT USING @skip, @numrows;`|
预制语句的SQL语法不能被用于带嵌套的风格中。也就是说，被传递给PREPARE的语句本身不能是一个PREPARE, EXECUTE或DEALLOCATE PREPARE语句。
预制语句的SQL语法与使用预制语句API调用不同。例如，您不能使用mysql_stmt_prepare() C API函数来预备一个PREPARE, EXECUTE或DEALLOCATE PREPARE语句。
预制语句的SQL语法可以在已存储的过程中使用，但是不能在已存储的函数或触发程序中使用。
以上就是本文的全部内容，希望对大家的学习有所帮助。
转自：[https://www.jb51.net/article/81378.htm](https://www.jb51.net/article/81378.htm)
