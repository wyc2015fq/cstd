# 如何转义emoji表情，让它可以存入utf8的数据库？ - z69183787的专栏 - CSDN博客
2017年01月07日 12:48:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2902
给一个标准的解决方案：
- 
mysql的版本必须为v5.5.3或更高
- 
把数据库的编码改成`utf8mb4 -- UTF-8 Unicode`
- 
然后需要存储emoji表情的字段选择`utf8mb4_general_ci`
- 
数据库连接也需要改为`utf8mb4`
设置完成后，应该可以看到如下类似字符集设置结果。那么可以直接的存入数据库，无需做任何额外的事情了。
```
mysql> SHOW VARIABLES WHERE Variable_name LIKE 'character\_set\_%' OR Variable_name LIKE 'collation%';  
+--------------------------+--------------------+  
| Variable_name            | Value              |  
+--------------------------+--------------------+  
| character_set_client     | utf8mb4            |  
| character_set_connection | utf8mb4            |  
| character_set_database   | utf8mb4            |  
| character_set_filesystem | binary             |  
| character_set_results    | utf8mb4            |  
| character_set_server     | utf8mb4            |  
| character_set_system     | utf8               |  
| collation_connection     | utf8mb4_unicode_ci |  
| collation_database       | utf8mb4_unicode_ci |  
| collation_server         | utf8mb4_unicode_ci |  
+--------------------------+--------------------+  
 rows in set (0.00 sec)
```
