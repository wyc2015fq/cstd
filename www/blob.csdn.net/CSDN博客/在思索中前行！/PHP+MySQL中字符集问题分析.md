# PHP+MySQL中字符集问题分析 - 在思索中前行！ - CSDN博客





2014年10月13日 22:50:49[_Tham](https://me.csdn.net/txl16211)阅读数：666标签：[PHH+mysql 字符集																[MysqL字符编码](https://so.csdn.net/so/search/s.do?q=MysqL字符编码&t=blog)](https://so.csdn.net/so/search/s.do?q=PHH+mysql 字符集&t=blog)
个人分类：[简明现代魔法](https://blog.csdn.net/txl16211/article/category/2631505)








 Character set顾名思义，就是字符、以及字符对应的编码的集合。例如简体中文字符集gb2312就包括简体中文中的所有规定汉字，以及每个汉字对应的代码。

 Collation，是指比较字符的规则的集合。有了比较规则，才能够将一组数据排序——例如按照英文字母顺序排序、汉字按照拼音顺序排序等等。显然，针对同样一组字符集可以有不同的排序标准、规则。例如汉字可以按照拼音排序，也可以按照笔画多少排序。尤其是Unicode的字符集，由于其可以包含不同种类的语言，所以可以按照各种语言的排序方法排序。此外，完全按照字符在字符集里的编码进行比较的方式称为binary比较。

 到了这里我们就容易理解了。举例来说，MySQL支持的gb2312字符集中，有gb2312_bin和gb2312_general_ci两种collation。很显然前者是binary比较规则，后者是一般的中文字符比较规则。

 每种字符集都有其默认的collation。对于utf8字符集来说，其默认collation是utf8_general_ci。要获得MySQL里面支持的字符集和默认collation列表，可以使用SHOW CHARACTER SET语句：



|`1`|```mysql> SHOW````CHARACTER``SET``;`|
|----|----|


|`2`|`+``----------+-----------------------------+---------------------+`|
|----|----|


|`3`|```| Charset  | Description                 |````Default````collation   |```|
|----|----|


|`4`|`+``----------+-----------------------------+---------------------+`|
|----|----|


|`5`|```| big5     | Big5 Traditional Chinese    | big5_chinese_ci     |```|
|----|----|


|`6`|```| dec8     |````DEC````West European           | dec8_swedish_ci     |```|
|----|----|


|`7`|```| cp850    | DOS West European           | cp850_general_ci    |```|
|----|----|


|`8`|`...`|
|----|----|




 其次，是MySQL中，在哪些地方需要这些字符集和collation。总体上分，在MySQL的体系中有三处字符集和collation：服务器（数据），连接，客户端。乍一看体系清楚明了，其实并不是这样。下面就一一介绍。

###  服务器（数据）端

 服务器（数据）端的字符集和collation，可以分成四级逐层指定——server, database, table, column。当MySQL存取位于某一列（column）的数据时，如果column的字符集和collation没有指定，就会向上追溯table的；如果table也没有指定字符集和collation，就以database的字符集和collation作为默认值；如果database仍旧没有指定，那么就以服务器的字符集和collation作为默认值。

 那么server的字符集和collation的默认值又是从哪里来的呢？答案是，配置文件（my.ini）和mysqld（或者mysqld-nt）的命令行参数中都可以指定。如果不幸的，你根本没有在my.ini或者命令行中指定，那么MySQL就会使用编译MySQL时指定的默认字符集——latin1。

 但是，需要注意的是，如果安装MySQL时选择了多语言支持（一般用中文的都会选择吧），安装程序会自动在配置文件中设置default-character-set=utf8。

 这样，所有创建的数据库、表，除非明确指出使用其它字符集，都会默认的使用utf作为数据的字符集（同时使用utf8_general_ci作为默认collation，因为它是utf8的默认collation）。

 相关系统变量：



|`1`|`character_set_server：服务器的字符集`|
|----|----|


|`2`|`collation_server：服务器的collation`|
|----|----|


|`3`|`character_set_database：数据库字符集`|
|----|----|


|`4`|`collation_database：数据库的collation`|
|----|----|




### 客户端

对于客户端传送来的literal string（例如INSERT，UPDATE语句当中的值），MySQL需要知道它们是什么编码。同时，MySQL返回给客户端的值（例如SELECT语句的返回值），也可以按照指定的编码返回。

相关系统变量



|`1`|`character_set_client：客户端发送过来文字的字符集`|
|----|----|


|`2`|`character_set_results：发送给客户端的结果所使用的字符集`|
|----|----|




### 连接

用于连接的字符集和collation，是指MySQL在接受到客户端发送来的文本之后，转换成何种字符集，用什么规则进行比较。需要注意的是，如果是将文本和数据库中某个column的值比较，将优先使用该column的字符集和collation。

相关系统变量



|`1`|`character_set_connection：用于连接的字符集`|
|----|----|


|`2`|`collation_connection：用于连接的collation`|
|----|----|




有了上面的预备知识，我们就开始分析最初的问题：本来是应该作为UTF-8字符保存的数据，为什么到了数据库中就变成了“乱码”？而且这些乱码居然还能毫无问题地被wordpress显示？为什么一旦导入到租用的主机那里就不能正常显示了呢？

首先让我们来看一下，我家里的服务器上，MySQL的系统变量（System Variables）是如何设置的。

注意：因为一些系统变量是根据客户端不同而不同的，所以用mysql命令行登陆所看到的和PHP下看到的并不相同。此外，似乎也不能用PMA查看——似乎在PMA中也已经更改了默认的系统变量。因此，要查看PHP作为客户端时的默认系统变量，可以编写一个类似下面的PHP小程序：



|`1`|`mysql_connect(localhost, ``$user``, ``$pass``);`|
|----|----|


|`2`|`$query``=````"SHOW VARIABLES"````;`|
|----|----|


|`3`|`$result``=mysql_query(``$query``);`|
|----|----|




其中$result就包含着所有系统变量。在我家里的服务器上得到了如下结果（以下只列出跟字符集有关的系统变量）：



|`01`|```character_set_client  latin1```|
|----|----|


|`02`|```character_set_connection  latin1```|
|----|----|


|`03`|```character_set_database  utf8```|
|----|----|


|`04`|```character_set_filesystem  binary```|
|----|----|


|`05`|```character_set_results  latin1```|
|----|----|


|`06`|```character_set_server  utf8```|
|----|----|


|`07`|```character_set_system  utf8```|
|----|----|


|`08`|```collation_connection  latin1_swedish_ci```|
|----|----|


|`09`|```collation_database  utf8_general_ci```|
|----|----|


|`10`|```collation_server  utf8_general_ci```|
|----|----|




可见，默认的客户端编码、默认的连接编码是latin1——这也就是说，虽然实际上wordpress传递给MySQL的文本都是用UTF-8编码的，但是由于上述系统变量设置不当，这些UTF-8编码的文本被MySQL当作是latin1编码的，并且由于数据库本身是utf8，因此把这些“latin1文本”又转换成了utf8。这样，一个汉字居然需要6bytes（一个汉字作为UTF-8是3bytes，被当作latin1进行了转换，每个latin1字符转换成2bytes的UTF-8编码）。这就不难理解为什么数据库存储的是“乱码”了。

那么为什么这些“乱码”在wordpress显示时没问题呢？这是因为，character_set_result也是latin1，也就是说MySQL在取出数据交给wordpress时，把这些数据从utf8转换回latin1，然后wordpress将这些latin1又当作了utf8——正好是上面的逆过程。

那么，为什么到另一台服务器上面就无法正常显示了呢？请看看那台租用主机的系统变量设置：



|`1`|```character_set_client  ujis```|
|----|----|


|`2`|```character_set_connection  ujis```|
|----|----|


|`3`|```character_set_database  ujis```|
|----|----|


|`4`|```character_set_results  ujis```|
|----|----|


|`5`|```character_set_server  ujis```|
|----|----|


|`6`|```character_set_system  utf8```|
|----|----|


|`7`|```collation_connection  ujis_japanese_ci```|
|----|----|


|`8`|```collation_database  ujis_japanese_ci```|
|----|----|


|`9`|```collation_server  ujis_japanese_ci```|
|----|----|




可见，其默认的客户端编码是ujis。也就是说，MySQL把utf8数据取出后，将会转换成ujis并传递给wordpress。这经历了latin1 – utf8 – ujis转换的原本是utf8的字符，早已面目全非了……

### 解决方案

解决方案在很多论坛、网页上已经有提到了，在wordpress的trac也已经有人提出过。

但是在解决问题之前，我却很想知道一个问题的答案，那就是：这到底是MySQL的问题，还是PHP（特别是php_mysql extension）的问题，还是wordpress的问题？甚至是用户配置的问题？我倾向于认为这是一个wordpress的问题。因为无论MySQL还是PHP都不知道wordpress使用了什么字符编码，所以无法更改客户端字符集；而作为一般的wordpress用户，要求他们设置字符编码——可以，但是必须要提供一个用户界面，而不是直接修改源程序。

那么解决方案（或者说只是一个workaround）就是，修改wordpress的\wp-uncludes\wp-db.php。在第40多行的function wpdb中，在$this->select($dbname);之前添加一句：



|`1`|`$this``->query(````"SET NAMES latin1"````);`|
|----|----|




SET NAMES语句的功能就是，执行了SET NAMES ‘x’相当于下面三条语句的功能。



|`1`|```SET character_set_client = x;```|
|----|----|


|`2`|```SET character_set_results = x;```|
|----|----|


|`3`|```SET character_set_connection = x;```|
|----|----|




这样，在默认客户端字符集是ujis的租用主机上，导入的wordpress文章也能正常显示了。当然，这不是彻底的解决方案——这只是“将错就错”，反正数据库里面存储的已经是被当作latin1而转换成utf8的utf8了，那么就将其转换回所谓的latin1就是了。这样做将使其他程序无法读取wordpress的数据，并且更重要的是，数据库中存储的“utf8数据”无法真正按照utf8应有的排序规则来排序。

那么最彻底的做法，就是在安装wordpress时就添加上面所说的SET NAMES语句，并且设置客户端的字符集为utf8：



|`1`|`$this``->query(````"SET NAMES utf8"````);`|
|----|----|




但是这样做的话，已经被当作latin1写到数据库里面的文章就会无法正常显示了。要让他们正常显示，必须经过utf8 – latin1的转换。如果数量较多，可以考虑编写一个程序进行转换；数量较少的话……手动转换吧。

BTW，国内高手们汉化的中文版的wordpress中已经添加好这一句了，上面的信息只适用于那些使用英文wordpress的朋友，以及喜欢追根问底的朋友。




