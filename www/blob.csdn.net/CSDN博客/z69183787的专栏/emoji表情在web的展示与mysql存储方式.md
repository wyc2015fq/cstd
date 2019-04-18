# emoji表情在web的展示与mysql存储方式 - z69183787的专栏 - CSDN博客
2017年01月07日 13:03:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1907
[iOS](http://lib.csdn.net/base/ios)，[Android](http://lib.csdn.net/base/android)，wp上emoji表情输入与显示
首先解决emoji表情在[MySQL](http://lib.csdn.net/base/mysql)中存储报错问题：
Caused by: [Java](http://lib.csdn.net/base/javase).sql.SQLException: Incorrect string value: '\xF0\x9F\x98\x97\xF0\x9F...'
 for column 'CONTENT' at row 1
一、主要问题为解决字符集，需要将mysql的字符集修改成utf8mb4
1、先去修改表字段字符集为utf8mb4
 ALTER TABLE UGC_REVIEW_CONTENT MODIFY `CONTENT` TEXT CHARACTER SET utf8mb4 COLLATE  utf8mb4_unicode_ci COMMENT '评论内容';
2、修改表字符集utf8mb4
ALTER TABLE UGC_REVIEW_CONTENT  CHARSET=utf8mb4
3、再去修改[数据库](http://lib.csdn.net/base/mysql)的字符集utf8mb4
  在mysq安装目录下，my.cnf文件中新增如下配置：
[client]
 default-character-set = utf8mb4
[mysql]
 default-character-set=utf8mb4
[mysqld]
 character-set-client-handshake = FALSE
 character-set-server = utf8mb4
 collation-server = utf8mb4_unicode_ci
 init_connect='SET NAMES utf8mb4'
 重启mysql服务器
 [root@mysqlvm4 ~]# service mysql restart
 Shutting down MySQL....                                    [确定]
 Starting MySQL.............................................[确定]..
 [root@mysqlvm4 ~]#
查看db的字符集 
mysql> show variables like '%char%';
 +--------------------------+----------------------------------+
 | Variable_name            | Value                            |
 +--------------------------+----------------------------------+
 | character_set_client     | utf8mb4                          |
 | character_set_connection | utf8mb4                          |
 | character_set_database   | utf8mb4                          |
 | character_set_filesystem | binary                           |
 | character_set_results    | utf8mb4                          |
 | character_set_server     | utf8mb4                          |
 | character_set_system     | utf8                             |
 | character_sets_dir       | /usr/local/mysql/share/charsets/ |
 +--------------------------+----------------------------------+
 8 rows in set (0.00 sec)
 mysql> 
 到此为止，mysql能支持存储emoji表情了
 二、解决emoji表情在web端能显示的原因
 所需js库，按顺序依赖
emoji-list-with-image.js 存储emoji编码和表情图片数据，格式如['e415','1f604','iVBORw0KGg...']
punycode.js 编码解析库
emoji.js  解析emoji编码 
示例用法：在含有emoji表情的content中调用以下函数,即可展示emoji表情
**[javascript]**[view
 plain](http://blog.csdn.net/qq_21397183/article/details/45502065?ref=myread#)[copy](http://blog.csdn.net/qq_21397183/article/details/45502065?ref=myread#)
- //解析存储的emoji表情
- function parse(arg) {  
- if (typeof ioNull !='undefined') {  
- return  ioNull.emoji.parse(arg);      
-     }  
- return'';  
- };  
![](http://static.blog.csdn.net/images/save_snippets.png)
emoji.js中包含了getList方法，emoji.js依赖其余两个js文件
其中js文件下载地址http://www.it165[.NET](http://lib.csdn.net/base/dotnet)/uploadfile/files/2014/0904/emoji-lib.zip
