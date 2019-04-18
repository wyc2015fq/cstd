# Mysql-Emoji表情的存取 - z69183787的专栏 - CSDN博客
2018年04月24日 15:38:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：193
日前，在项目中遇到了一个很尴尬的问题，首先贴上一些背景和概况； 
Mysql版本：5.6.24-72.2-log 
字符集：
```
mysql> SHOW VARIABLES WHERE Variable_name LIKE 'character_set_%' OR Variable_name LIKE 'collation%';
+--------------------------+-----------------------------------------+
| Variable_name            | Value                                   |
+--------------------------+-----------------------------------------+
| character_set_client     | utf8mb4                                 |
| character_set_connection | utf8mb4                                 |
| character_set_database   | utf8mb4                                 |
| character_set_filesystem | binary                                  |
| character_set_results    | utf8mb4                                 |
| character_set_server     | utf8                                    |
| character_set_system     | utf8                                    |
| character_sets_dir       | /usr/local/mysql-5.6.24/share/charsets/ |
| collation_connection     | utf8mb4_general_ci                      |
| collation_database       | utf8mb4_general_ci                      |
| collation_server         | utf8_general_ci                         |
+--------------------------+-----------------------------------------+
11 rows in set
mysql> SHOW VARIABLES LIKE '%colla%';
+----------------------+--------------------+
| Variable_name        | Value              |
+----------------------+--------------------+
| collation_connection | utf8mb4_general_ci |
| collation_database   | utf8mb4_general_ci |
| collation_server     | utf8_general_ci    |
+----------------------+--------------------+
3 rows in set
mysql>
```
可以看到 **character_set_server** 服务器字符集是utf-8，这也就是导致最后emoji表情没有存入的关键，但其实我们遇到的事情更奇怪。 
应用A Mysql驱动库版本 mysql:mysql-connector-java:5.1.13 
应用B Mysql驱动库版本 mysql:mysql-connector-java:5.1.34 + 
大家都知道，为了存入四字节UTF-8字符集的emoji表情，数据表的字段需要变更为utf8mb4。前戏都准备好了，奇怪的问题出现了，A应用可以正常存入emoji表情，B应用却报了   java.sql.SQLException: Incorrect string value: ‘\xF0\x9F\x90\xBC\xF0\x9F…’ for column ‘Ext1’ at row 1，无法存入emoji表情。 
接着，从其他同事那边拿到了传说中的解决方案：不同的连接池可能参数不同，但核心思想是在数据库连接池配置时增加一个配置项，以Druid为例：
```xml
<property name="connectionInitSqls" value="set names utf8mb4"/>
```
在物理连接建立时，设定字符集为utf8mb4。但上线后发现还是不行。
之后和DBA同学交流了一下，得出了2个解决方案： 
1、修改mysql配置文件my.cnf（windows为my.ini），设置默认的编码集为utf8mb4，我们的数据库少了 **character_set_server**（方式大家可以自行搜索），之前还是utf-8，但这种方式需要重启，比较麻烦。 
2、使用5.1.13的驱动版本
相信大家都觉得奇怪，为什么只有5.1.13可以正常存入emoji呢？，查询官网后发现，是一个错进错出的CASE。 
以下来自Mysql官网 
[5.1.13](https://dev.mysql.com/doc/relnotes/connector-j/5.1/en/news-5-1-13.html)
5.1.13版本的功能改进： 
Connector/J did not support utf8mb4 for servers 5.5.2 and newer. 
Connector/J now auto-detects servers configured with character_set_server=utf8mb4 or treats the Java encoding utf-8 passed using characterEncoding=… as utf8mb4 in the SET NAMES= calls it makes when establishing the connection. (Bug #54175) 
大概意思就是如果连接字符串使用 
jdbc:mysql://xxxxx:3306/DB?characterEncoding=UTF8&socketTimeout=60000 
UTF8的字符集，当与mysql建立连接时，会自动适配为utf8mb4，其实和上面设置initSql的解决方案雷同（但不知道为什么上面没用）。
[5.1.14](https://dev.mysql.com/doc/relnotes/connector-j/5.1/en/news-5-1-14.html)
5.1.14版本的BugFix： 
Connector/J mapped both 3-byte and 4-byte UTF8 encodings to the same Java UTF8 encoding. 
使用相同的java编码格式UTF-8来处理3和4字节的UTF-8编码
To use 3-byte UTF8 with Connector/J set characterEncoding=utf8 and set useUnicode=true in the connection string.
To use 4-byte UTF8 with Connector/J configure the MySQL server with character_set_server=utf8mb4. Connector/J will then use that setting as long as characterEncoding has not been set in the connection string. This is equivalent to autodetection of the character set. (Bug #58232)
针对四字节的UTF-8，如果设置了编码格式为UTF-8，就不会自动开启自动检测，整个链接过程都是UTF-8格式，会导致无法处理Emoji。但如果未设置编码格式，则会自动监测数据库自身的编码格式（character_set_server），恰好这时数据库编码是utfbmb4的话，emoji表情就可以存入了。
我们的问题就在这里，链接字符串中有characterEncoding=UTF-8，并且我们的数据库character_set_server也是UTF-8，所以导致全程使用了UTF-8的编码集来处理链接及数据请求，而5.1.13的自动适配反而帮助我们搞定了emoji的存储。
