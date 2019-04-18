# 在命令行下进行Oracle用户解锁 - z69183787的专栏 - CSDN博客
2014年04月12日 20:53:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：725
个人分类：[Oracle](https://blog.csdn.net/z69183787/article/category/2175483)

转：http://database.51cto.com/art/200910/158576.htm
在DBA的日常工作中，经常遇到为Oracle用户解锁的操作；这篇文章给出在命令行下进行Oracle用户解锁的操作方法，通过几条简单的解锁语句就能完成此项工作。下面是具体的过程：
默认的scott用户是被锁定的，先解锁就能登陆上了。 使用下面的语句解锁scott:
*alter user scott account unlock;*
解锁之后可能会要求你该密码：
*alter user scott identified by tiger;*
再登陆：
*sqlplus scott/tiger*
就能登陆了
Oracle锁定和解锁用户的命令
*SQL> conn /as sysdba 已连接。 //scott 用户在没有设定tieger默认密码时，登陆不上*
**锁定用户的方法：**
*SQL> alter user test account lock;*
用户已更改。
*-------test用户登陆数据库，失败了。 C:\>sqlplus test/test*
*ERROR: ORA-28000: the account is locked*
**Oracle用户解锁的方法：**
*SQL> conn /as sysdba SQL> alter user test account unlock;*
用户已更改。这样就完成了Oracle的用户解锁操作。
