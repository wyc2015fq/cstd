
# Redis 事务 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月27日 10:25:07[seven-soft](https://me.csdn.net/softn)阅读数：141个人分类：[Redis																](https://blog.csdn.net/softn/article/category/6332853)



Redis 事务可以一次执行多个命令， 并且带有以下两个重要的保证：
事务是一个单独的隔离操作：事务中的所有命令都会序列化、按顺序地执行。事务在执行的过程中，不会被其他客户端发送来的命令请求所打断。
事务是一个原子操作：事务中的命令要么全部被执行，要么全部都不执行。
一个事务从开始到执行会经历以下三个阶段：
开始事务。
命令入队。
执行事务。
---

## 实例
以下是一个事务的例子， 它先以**MULTI**开始一个事务， 然后将多个命令入队到事务中， 最后由**EXEC**命令触发事务， 一并执行事务中的所有命令：
redis127.0.0.1:6379>MULTI
OK
redis127.0.0.1:6379>SET book-name"Mastering C++ in 21 days"QUEUED
redis127.0.0.1:6379>GET book-name
QUEUED
redis127.0.0.1:6379>SADD tag"C++""Programming""Mastering Series"QUEUED
redis127.0.0.1:6379>SMEMBERS tag
QUEUED
redis127.0.0.1:6379>EXEC1)OK2)"Mastering C++ in 21 days"3)(integer)34)1)"Mastering Series"2)"C++"3)"Programming"
---

## Redis 事务命令
下表列出了 redis 事务的相关命令：
|序号|命令及描述|
|1|[DISCARD](http://www.runoob.com/redis/transactions-discard.html)
|取消事务，放弃执行事务块内的所有命令。|
|2|[EXEC](http://www.runoob.com/redis/transactions-exec.html)
|执行所有事务块内的命令。|
|3|[MULTI](http://www.runoob.com/redis/transactions-multi.html)
|标记一个事务块的开始。|
|4|[UNWATCH](http://www.runoob.com/redis/transactions-unwatch.html)
|取消 WATCH 命令对所有 key 的监视。|
|5|[WATCH key [key ...]](http://www.runoob.com/redis/transactions-watch.html)
|监视一个(或多个) key ，如果在事务执行之前这个(或这些) key 被其他命令所改动，那么事务将被打断。|

