# MySQL 的 20+ 条最佳实践 - 文章 - 伯乐在线
原文出处： [Burak Guzel](https://code.tutsplus.com/tutorials/top-20-mysql-best-practices--net-7855)   译文出处：[开源中国](https://www.oschina.net/translate/top-20-mysql-best-practices?print)
数据库操作是当今 Web 应用程序中的主要瓶颈。 不仅是 DBA（数据库管理员）需要为各种性能问题操心，程序员为做出准确的结构化表，优化查询性能和编写更优代码，也要费尽心思。 在本文中，我列出了一些针对程序员的 MySQL 优化技术。
在我们开始学习之前，我补充一点：你可以在 Envato Market 上找到大量的 MySQL 脚本和实用程序。![](http://jbcdn2.b0.upaiyun.com/2016/11/7156fed7754ec432b2de696b4a98390f.png)
## 1.优化查询的查询缓存
大部分MySQL服务器都有查询缓存功能。这是提高性能的最有效的方法之一，这是由数据库引擎私下处理的。当同一个查询被多次执行，结果会直接从缓存里提取，这样速度就很快。
主要的问题是，这对程序员来说太简单了，不容易看到，我们很多人都容易忽略。我们实际上是可以组织查询缓存执行任务的。


```
// query cache does NOT work
$r = mysql_query("SELECT username FROM user WHERE signup_date >= CURDATE()");
 
// query cache works!
$today = date("Y-m-d");
$r = mysql_query("SELECT username FROM user WHERE signup_date >= '$today'");
```
查询缓存在第一行不执行的原因在于CURDTE()功能的使用。这适用于所有的非确定性功能，就像NOW()和RAND()等等。。。因为功能返回的结果是可变的。MySQL决定禁用查询器的查询缓存。我们所需要做的是通过添加一额外一行PHP，在查询前阻止它发生。
## **2. EXPLAIN你的选择查询**
使用EXPLAIN关键词可以帮助了解MySQL是怎样运行你的查询的。这有助于发现瓶颈和查询或表结构的其它问题。
EXPLAIN的查询结果会展示哪一个索引被使用过，表示怎样扫描和储存的，等等。。。
选择一个SELECT查询（一个有连接的复杂查询会更好），在它的前面添加关键词EXPLAIN，这样就可以直接使用数据库了。结果会以一个漂亮的表来展示。例如，就好比我执行连接时忘了添加一栏的索引：
![](http://jbcdn2.b0.upaiyun.com/2016/11/75994426b06426f1ed9fd694b90f886f.jpg)
现在它只会从表2里面扫描9和16行，而非扫描7883行。经验法则是乘以所有“行”那一栏的数字，你的查询性能会跟结果数字成比例的。
## 3. 获取唯一行时使用LIMIT 1
有时当你查表时，你已经知道你正在查找的结果只有一行。你可能正在获取唯一记录，或者你可能只是查询是否存在满足你的WHERE子句条件的记录。
在这种情况下，将LIMIT 1添加到查询条件中可以提高性能。这样，数据库引擎将在找到刚刚第一个记录之后停止扫描记录，而不是遍历整个表或索引。


```
// do I have any users from Alabama?
 
// what NOT to do:
$r = mysql_query("SELECT * FROM user WHERE state = 'Alabama'");
if (mysql_num_rows($r) > 0) {
    // ...
}
 
 
// much better:
$r = mysql_query("SELECT 1 FROM user WHERE state = 'Alabama' LIMIT 1");
if (mysql_num_rows($r) > 0) {
    // ...
}
```
## 4. 索引搜索字段
索引不仅仅是为了主键或唯一键。如果你会在你的表中按照任何列搜索，你就都应该索引它们。
![](http://jbcdn2.b0.upaiyun.com/2016/11/1f8eec732aad14824ab983dffe0be72a.jpg)
正如你所看到的，这个规则也适用于如 “last_name LIKE ‘a%'”的部分字符串搜索。当从字符串的开头搜索时，MySQL就可以使用那一列的索引。
你也应该明白什么样搜索可以不使用有规律的索引。例如，当搜索一个单词时（例如，”WHERE post_content LIKE ‘%apple%'”），你将不会看到普通索引的好处。你最好使用 mysql 全文搜索或者构建你自己的索引解决方案。
## 5. 索引并对连接使用同样的字段类型
如果你的应用程序包含许多连接查询, 你需要确保连接的字段在两张表上都建立了索引。 这会影响MySQL如何内部优化连接操作。
此外,被连接的字段,需要使用同样类型。例如, 如果你使用一个DECIMAL字段, 连接另一张表的INT字段, MySQL将无法使用至少一个索引。 即使字符编码也需要使用相同的字符类型。


```
// looking for companies in my state
$r = mysql_query("SELECT company_name FROM users
    LEFT JOIN companies ON (users.state = companies.state)
    WHERE users.id = $user_id");
 
// both state columns should be indexed
// and they both should be the same type and character encoding
// or MySQL might do full table scans
```
## 6. 不要ORDER BY RAND()
起初这是一个听起来挺酷的技巧, 让许多菜鸟程序员陷入了这个陷阱。但你可能不知道，一旦你开始在查询中使用它，你创建了非常可怕的查询瓶颈。
如果你真的需要对结果随机排序, 这有一个更好的方法。补充一些额外代码,你将可以防止当数据成指数级增长时造成的瓶颈。关键问题是，MySQL必须在排序之前对表中的每一行执行RAND()操作（这需要处理能力），并且仅仅给出一行。


```
// what NOT to do:
$r = mysql_query("SELECT username FROM user ORDER BY RAND() LIMIT 1");
 
 
// much better:
 
$r = mysql_query("SELECT count(*) FROM user");
$d = mysql_fetch_row($r);
$rand = mt_rand(0,$d[0] - 1);
 
$r = mysql_query("SELECT username FROM user LIMIT $rand, 1");
```
所以挑选一个小于结果数的随机数，并将其用作LIMIT子句中的偏移量。
## 7. 避免使用SELECT *
从数据表中读取的数据越多，查询操作速度就越慢。它增加了磁盘操作所需的时间。此外，当数据库服务器与Web服务器分开时，由于必须在服务器之间传输数据，将会有更长的网络延迟。
这是一个好习惯：当你使用SELECT语句时总是指定你需要的列。


```
// not preferred
$r = mysql_query("SELECT * FROM user WHERE user_id = 1");
$d = mysql_fetch_assoc($r);
echo "Welcome {$d['username']}";
 
// better:
$r = mysql_query("SELECT username FROM user WHERE user_id = 1");
$d = mysql_fetch_assoc($r);
echo "Welcome {$d['username']}";
 
// the differences are more significant with bigger result sets
```
## 8. 几乎总是有一个id字段
在每个以id列为PRIMARY KEY的数据表中，优先选择AUTO_INCREMENT或者INT。 也可以优选使用UNSIGNED，因为该值不能为负的。
即使你拥有一个具有唯一用户名字段的用户表，也不要将其作为主键。 VARCHAR字段作为主键（检索）速度较慢。通过内部ID引用所有的用户数据，你的代码中将更加结构化。
有些后台操作是由MySQL引擎本身完成的，它在内部使用主键字段。当数据库设置越复杂（集群，分区等…），这就变得更加重要了。
这个规则的一个可能的例外是“关联表”，用于两个表之间的多对多类型的关联。例如，“posts_tags”表中包含两列：post_id，tag_id，用于保存表名为“post”和“tags”的两个表之间的关系。这些表可以具有包含两个id字段的PRIMARY键。
## 9. 相比VARCHAR优先使用ENUM
ENUM枚举类型是非常快速和紧凑的。在内部它们像TINYINT一样存储，但它们可以包含和显示字符串值。这使他们成为某些领域的完美候选。
如果有一个字段只包含几种不同的值，请使用ENUM而不是VARCHAR。例如，它可以是名为“status”的列，并且只包含诸如“active”，“inactive”，“pending”，“expired”等的值…
关于如何重构你的数据表，甚至有一种方法是可以从MySQL本身得到“建议”。 当你有一个VARCHAR字段，它实际上建议你将该列类型更改为ENUM。这通过调用PROCEDURE ANALYZE()来完成。
## 10. 使用PROCEDURE ANALYSE()获取建议
PROCEDURE ANALYSE() 将使用MySQL分析列结构和表中的实际数据，为你提供一些建议。它只有在数据表中有实际数据时才有用，因为这在分析决策时很重要。
例如，如果你创建了一个INT类型的主键，但没有太多行，MySQL则可能建议您改用MEDIUMINT。或者如果你使用VARCHAR字段，如果表里只有很少的取值，你可能会得到一个建议是将其转换为ENUM。
你也可以在其中一个表视图中单击phpmyadmin中的“建议表结构”链接来执行此操作。
![](http://jbcdn2.b0.upaiyun.com/2016/11/8c78973a896d5d4491ca383f76ef14ae.jpg)
请记住，这些只是建议。 如果你的数据表变得越来越大，他们甚至可能不是正确的建议。至于如何修改最终是你来决定。
## 11. 如果可以的话使用NOT NULL
除非你有非常重要的理由使用NULL值，否则你应该设置你的列为NOT NULL。
首先，问一下你自己在空字符串值和NULL值之间（对应INT字段：0 vs. NULL）是否有任何的不同.如果没有理由一起使用这两个，那么你就不需要一个NULL字段（你知道在Oracle中NULL和空字符串是一样的吗？）。
NULL列需要额外的空间，他们增加了你的比较语句的复杂度。如果可以的话尽量避免它们。当然，我理解一些人，他们也许有非常重要的理由使用NULL值，这不总是一件坏事。
摘自MySQL 文档：
> 
 “NULL列在行记录它们的值是否为NULL时需要额外的空间。例如MyISAM 表，每一个NULL列拥有额外的一个比特，聚集在最近的字节。”
## 12. 预处理语句
使用预处理语句有诸多好处，包括更高的性能和更好的安全性。
预处理语句默认情况下会过滤绑定到它的变量，这对于避免SQL注入攻击极为有效。当然你也可以指定要过滤的变量。但这些方法更容易出现人为错误，也更容易被程序员遗忘。这在使用框架或 ORM 的时候会出现一些问题。
既然我们关注性能，那就应该说说这个方面的好处。当在应用中多次使用同一个查询的时候，它的好处特别明显。既然向同一个预备好的语句中传入不同的参数值，MySQL 对这个语句也只会进行一次解析。
同时，最新版本的 MySQL 在传输预备好的语句时会采用二进制形式，这样做的作用非常明显，而且对减少网络延迟很有帮助。
曾经有一段时间，许多程序员为了一个重要的原因则避免使用预处理语句。这个原因就是，它们不会被MySQL 缓存。不过在 5.1 版本的某个时候，查询缓存也得到的支持。
想在 PHP 中使用预处理语句，你可以看看 mysqli 扩展 或使用数据抽象层，如 PDO。


```
// create a prepared statement
if ($stmt = $mysqli->prepare("SELECT username FROM user WHERE state=?")) {
 
    // bind parameters
    $stmt->bind_param("s", $state);
 
    // execute
    $stmt->execute();
 
    // bind result variables
    $stmt->bind_result($username);
 
    // fetch value
    $stmt->fetch();
 
    printf("%s is from %sn", $username, $state);
 
    $stmt->close();
}
```
##  13. 无缓冲查询
通常当你从脚本执行一个查询，在它可以继续后面的任务之前将需要等待查询执行完成。你可以使用无缓冲的查询来改变这一情况。
在PHP 文档中对  mysql_unbuffered_query() f函数有一个很好的解释：
> 
“mysql_unbuffered_query() 发送SQL查询语句到MySQL不会像 mysql_query()那样自动地取并缓冲结果行。这让产生大量结果集的查询节省了大量的内存，在第一行已经被取回时你就可以立即在结果集上继续工作，而不用等到SQL查询被执行完成。”
然而，它有一定的局限性。你必须在执行另一个查询之前读取所有的行或调用mysql_free_result() 。另外你不能在结果集上使用mysql_num_rows() 或 mysql_data_seek() 。
## 14. 使用 UNSIGNED INT 存储IP地址
很多程序员没有意识到可以使用整数类型的字段来存储 IP 地址，所以一直使用 VARCHAR(15) 类型的字段。使用 INT 只需要 4 个字节的空间，而且字段长度固定。
必须确保列是 UNSINGED INT 类型，因为 IP 地址可能会用到 32 位无符号整型数据的每一个位。
在查询中可以使用 INET_ATON() 来把一个IP转换为整数，用 INET_NTOA() 来进行相反的操作。在 PHP 也有类似的函数，ip2long() 和 long2ip()。


```
$r = "UPDATE users SET ip = INET_ATON('{$_SERVER['REMOTE_ADDR']}') WHERE user_id = $user_id";
```
## 15. 固定长度（静态）的表会更快
（译者注：这里提到的表的长度，实际是指表头的长度，即表中每条数据占用的空间大小，而不是指表的数据量）
如果表中所有列都是“固定长度”，那么这个表被认为是“静态”或“固定长度”的。不固定的列类型包括 VARCHAR、TEXT、BLOB等。即使表中只包含一个这些类型的列，这个表就不再是固定长度的，MySQL 引擎会以不同的方式来处理它。
固定长度的表会提高性能，因为 MySQL 引擎在记录中检索的时候速度会更快。如果想读取表中的某一地，它可以直接计算出这一行的位置。如果行的大小不固定，那就需要在主键中进行检索。
它们也易于缓存，崩溃后容易重建。不过它们也会占用更多空间。例如，如果你把一个 VARCHAR(20) 的字符改为 CHAR(20) 类型，它会总是占用 20 个字节，不管里面存的是什么内容。
你可以使用“垂直分区”技术，将长度变化的列拆分到另一张表中。来看看：
## 16. 垂直分区
垂直分区是为了优化表结构而对其进行纵向拆分的行为。
**示例 1:** 你可能会有一张用户表，包含家庭住址，而这个不是一个常用数据。这时候你可以选择把表拆分开，将住址信息保存到另一个表中。这样你的主用户表就会更小。如你所知，表越小越快。
**示例 2**: 表中有一个 “last_login” 字段，用户每次登录网站都会更新这个字段，而每次更新都会导致这个表缓存的查询数据被清空。这种情况下你可以将那个字段放到另一张表里，保持用户表更新量最小。
不过你也需要确保不会经常联合查询分开后的两张表，要不然你就得忍受由这带来的性能下降。
## 17. 拆分大型DELETE或INSERT语句
如果你需要在网站上执行大型DELETE或INSERT查询，则需要注意不要影响网络流量。当执行大型语句时，它会锁表并使你的Web应用程序停止。
Apache运行许多并行进程/线程。 因此它执行脚本效率很高。所以服务器不期望打开过多的连接和进程，这很消耗资源，特别是内存。
如果你锁表很长时间（如30秒或更长），在一个高流量的网站，会导致进程和查询堆积，处理这些进程和查询可能需要很长时间，最终甚至使你的网站崩溃。
如果你的维护脚本需要删除大量的行，只需使用LIMIT子句，以避免阻塞。


```
while (1) {
    mysql_query("DELETE FROM logs WHERE log_date
```
## 18.越少的列越快
对于数据库引擎，磁盘可能是最重要的瓶颈。更小更紧凑的数据、减少磁盘传输量，通常有助于性能提高。
MySQL文档[Storage Requirements](http://dev.mysql.com/doc/refman/5.7/en/storage-requirements.html) 有所有数据类型清单。
如果已知表具有很少的行，则没有理由是主键类型为INT，可以用MEDIUMINT、SMALLINT代替，甚至在某些情况下使用TINYINT。 如果不需要完整时间记录，请使用DATE而不是DATETIME。
确保留下合理的扩展空间，不然你可能会像Slashdot这样。
## 19. 选择正确的存储引擎
MySQL有两个主要存储引擎，MyISAM和InnoDB。 每个都有自己的优点和缺点。
MyISAM适用于读取繁重的应用程序，但是当有很多写入时它不能很好地扩展。 即使你正在更新一行的一个字段，整个表也被锁定，并且在语句执行完成之前，其他进程甚至无法读取该字段。 MyISAM在计算SELECT COUNT（*）的查询时非常快。
InnoDB是一个更复杂的存储引擎，对于大多数小的应用程序，它比MyISAM慢。 但它支持基于行的锁定，使其更好地扩展。 它还支持一些更高级的功能，比如事务。
- [MyISAM存储引擎](http://dev.mysql.com/doc/refman/5.7/en/myisam-storage-engine.html)
- [InnoDB存储引擎](http://dev.mysql.com/doc/refman/5.7/en/innodb-storage-engine.html)
## 20. 使用对象关系映射器（ORM, Object Relational Mapper）
通过使用ORM（对象关系映射器），你可以获得一定的性能提升。ORM可以完成的一切事情，手动编码也可完成。但这可能意味着需要太多额外的工作，并且需要高水平的专业知识。
ORM以“延迟加载”著称。这意味着它们仅在需要时获取实际值。但是你需要小心处理他们，否则你可能最终创建了许多微型查询，这会降低数据库性能。
ORM还可以将多个查询批处理到事务中，其操作速度比向数据库发送单个查询快得多。
目前我最喜欢的PHP-ORM是Doctrine。
## 21. 小心使用持久连接
持久连接意味着减少重建连接到MySQL的成本。 当持久连接被创建时，它将保持打开状态直到脚本完成运行。 因为Apache重用它的子进程，下一次进程运行一个新的脚本时，它将重用相同的MySQL连接。
- PHP：mysql_pconnect()
理论上看起来不错。 但从我个人（和许多其他人）的经验看来，这个功能可能会导致更多麻烦。 你可能会出现连接数限制问题、内存问题等等。
Apache总是并行运行的，它创建许多子进程。 这是持久连接在这种环境中不能很好工作的主要原因。 在你考虑使用mysql_pconnect()之前，请咨询你的系统管理员。
