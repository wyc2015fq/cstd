# mysql存储表情微信昵称 - 三少GG - CSDN博客
2017年10月12日 10:51:03[三少GG](https://me.csdn.net/scut1135)阅读数：1782
1，去修改表字符集utf8mb4：
ALTER TABLE UGC_REVIEW_CONTENT  CHARSET=utf8mb4 COMMENT='晒单/推荐的评论内容';
执行完后，再在手机端的app测试，依然报同样的错误。
2，再去修改数据库的字符集utf8mb4: 
vim my.cnf
init-connect='SET NAMES utf8mb4'
character-set-server=utf8mb4
重启mysql数据库
[root@mysqlvm4 ~]# service mysql restart
Shutting down MySQL....                                    [确定]
Starting MySQL.............................................[确定]..
[root@mysqlvm4 ~]# 
查看db的字符集 
mysql> show variables like '%char%';
+--------------------------+----------------------------------+
| Variable_name            | Value                            |
+--------------------------+----------------------------------+
| character_set_client     | utf8mb4                          |
| character_set_connection | utf8mb4                          |
| character_set_database   | utf8mb4                          |
| character_set_filesystem | binary                           |
| character_set_results    | utf8mb4                          |
| character_set_server     | utf8mb4                          |
| character_set_system     | utf8                             |
| character_sets_dir       | /usr/local/mysql/share/charsets/ |
+--------------------------+----------------------------------+
8 rows in set (0.00 sec)
mysql> 
在手机app端重新评论，输入表情符号，点击提交提示评论成功。
问题分析总结：
    ㈠ 原因
        普通的字符串或者表情都是占位3个字节，所以utf8足够用了，但是移动端的表情符号占位是4个字节，普通的utf8就不够用了，为了应对无线互联网的机遇和挑战、避免 emoji 表情符号带来的问题、涉及无线相关的 MySQL 数据库建议都提前采用 utf8mb4 字符集，这必须要作为移动互联网行业的一个技术选型的要点
    ㈡ 限制
        需要 >= MySQL 5.5.3版本、从库也必须是5.5的了、低版本不支持这个字符集、复制报错。

.一般来说**Mysql(小于5.5.3)**字符集设置为utf8,指定连接的字符集也为utf8，django中save unicode string是木有问题的。但是，当字符串中有**特殊字符（如emoji表情符号，以及其他凡是转成utf8要占用4字节的字符）**，就会有问题，会报错**Incorrect string value: '\xF0\x9F\x92\x90</...' for column 'xxx' at row 1**
大家都知道Unicode是一个标准，utf8是unicode一个实现方式, 某些Unicode字符转成utf8可能4字节，而在MySQl5.5.3之前，utf8最长只有3字节。
mysql> show character set;
+------------+----------------------------+------------------------+----------+
| Charset  | Description                 | Default collation   | Maxlen |
+------------+----------------------------+------------------------+----------+
| utf8          | UTF-8 Unicode          | utf8_general_ci    |      3       |
+------------+----------------------------+------------------------+----------+
所以呢，这个需要4字节才能表示的Unicode字符会被截断，存不进去。
