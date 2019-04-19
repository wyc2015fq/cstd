# mysql savepoint 示例 - Arthur的随笔 - CSDN博客
2011年09月08日 13:53:28[largetalk](https://me.csdn.net/largetalk)阅读数：8421
```
SET SQL_SAFE_UPDATES=0;
update books set free =1, new=1;
commit;
SET AUTOCOMMIT=1;
begin; 
select free, new from books limit 1;
update books set free = 0;
select free, new from books limit 1;
SAVEPOINT book1;
update books set new = 0;
select free, new from books limit 1;
rollback to book1;
select free, new from books limit 1;
rollback;
select free, new from books limit 1;
```
```
mysql> update books set free =1, new=1;
Query OK, 0 rows affected (0.06 sec)
Rows matched: 79  Changed: 0  Warnings: 0
mysql> commit;
Query OK, 0 rows affected (0.00 sec)
mysql> SET AUTOCOMMIT=1;
Query OK, 0 rows affected (0.00 sec)
mysql> begin; 
Query OK, 0 rows affected (0.00 sec)
mysql> select free, new from books limit 1;
+------+-----+
| free | new |
+------+-----+
|    1 |   1 |
+------+-----+
1 row in set (0.00 sec)
mysql> update books set free = 0;
Query OK, 79 rows affected (0.01 sec)
Rows matched: 79  Changed: 79  Warnings: 0
mysql> select free, new from books limit 1;
+------+-----+
| free | new |
+------+-----+
|    0 |   1 |
+------+-----+
1 row in set (0.00 sec)
mysql> SAVEPOINT book1;
Query OK, 0 rows affected (0.00 sec)
mysql> update books set new = 0;
Query OK, 79 rows affected (0.00 sec)
Rows matched: 79  Changed: 79  Warnings: 0
mysql> select free, new from books limit 1;
+------+-----+
| free | new |
+------+-----+
|    0 |   0 |
+------+-----+
1 row in set (0.00 sec)
mysql> rollback to book1;
Query OK, 0 rows affected (0.00 sec)
mysql> select free, new from books limit 1;
+------+-----+
| free | new |
+------+-----+
|    0 |   1 |
+------+-----+
1 row in set (0.00 sec)
mysql> rollback;
Query OK, 0 rows affected (0.05 sec)
mysql> select free, new from books limit 1;
+------+-----+
| free | new |
+------+-----+
|    1 |   1 |
+------+-----+
1 row in set (0.00 sec)
```
