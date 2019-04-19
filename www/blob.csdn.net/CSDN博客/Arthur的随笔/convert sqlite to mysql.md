# convert sqlite to mysql - Arthur的随笔 - CSDN博客
2012年07月24日 23:07:49[largetalk](https://me.csdn.net/largetalk)阅读数：2151
尼玛，开始数据库不选好后面改太操蛋了，关键运行中得数据还不能丢，让人崩溃阿
1. schema先
sqlite3 xxxx.db
>.output schema.sql
>.schema
mysql和sqlite差距老大了，写一个sed脚本改schema,sql，一边写一边测，不具有可复制性
```
#!/bin/sed -f 
#filename : schema.sed
#author: authur
#date : 2012-7-24
s/\(CREATE TABLE \)"\(\S*\)"/\1\2/
/\bid\b[^,]*PRIMARY KEY/s/PRIMARY KEY/AUTO_INCREMENT PRIMARY KEY/g
/[CREATE TABLE|integer|varchar]/s/"/`/g
/CREATE TABLE/s/\(text[^,]*\)DEFAULT[^,]*,/\1,/g
/CREATE TABLE.*workshop_blog/s/DEFAULT 'interview'//g
/CREATE TABLE.*workshop_topic/s/DEFAULT 'faq'//g
s/djangoratings_vote_content_type_id__object_id__cookie__user_id__key__ip_address/djratings_vote_ctype_id_object_id_cookie_user_id_key_ip_address/
s/djangoratings_vote_content_type_id__object_id__key__user_id__ip_address/djratings_vote_ctype_id_object_id_key_user_id_ip_address/
```
./schema.sed schema.sql > schema-mysql.sql
2. 数据
数据就更让人崩溃了，mysql utf 字符集不支持大小写敏感，还有数据格式，一堆错误阿
sqlite3 xxxx.db
>.output dump.sql
>.dump
首先修改schema.sed为如下
```
#!/bin/sed -f 
#filename : schema.sed
#author: authur
#date : 2012-7-24
s/\(CREATE TABLE \)"\(\S*\)"/\1\2/
/\bid\b[^,]*PRIMARY KEY/s/PRIMARY KEY/AUTO_INCREMENT PRIMARY KEY/g
/[CREATE TABLE|integer|varchar]/s/"/`/g
/CREATE TABLE/s/\(text[^,]*\)DEFAULT[^,]*,/\1,/g
/CREATE TABLE.*workshop_blog/s/DEFAULT 'interview'//g
/CREATE TABLE.*workshop_topic/s/DEFAULT 'faq'//g
s/djangoratings_vote_content_type_id__object_id__cookie__user_id__key__ip_address/djratings_vote_ctype_id_object_id_cookie_user
_id_key_ip_address/
s/djangoratings_vote_content_type_id__object_id__key__user_id__ip_address/djratings_vote_ctype_id_object_id_key_user_id_ip_addr
ess/
s/PRAGMA foreign_keys=OFF;/SET foreign_key_checks = 0;/
s/BEGIN TRANSACTION;/START TRANSACTION;/
s/\(INSERT INTO \)"\(\S*\)"/\1\2/
```
然后./schema.sed dump.sql > dump-mysql.sql
到这里如果你认为大功已经告成你就错了，数据能不能导还说不定呢
首先尝试把dump-mysql.sql 导入mysql，如果发现因为大小写不敏感导致duplicate key或其他错误你就试试下面方法：
设置mysql collation， 在my.cnf里添加
[mysqld]
character-set-server=latin1
collation-server=latin1_general_cs #_cs结尾得都行， cs就是case sensitive, ci 就是 case insensitive, utf8 没有 _cs结尾得
再重启mysql试试。
如果有用中文做unique key, 导入基本也有问题， 做unique key好像只能区别出前两个字，尼玛。这怎么解决呢，看业务得吧，我比较粗暴点，把每个key前面加个随机字符，自己看着办吧。
`update workshop_tag set name = substr(abs(random()), 1, 10) || name;`
然后重新dump， 用schema.sed 处理一次
处理了这两个问题，我现在数据已经可以导入到mysql了，中间有没有隐藏问题谁也说不定阿，先这样用着呗。
ps。 一切做好备份，别转不成也回不去了就麻烦了。
