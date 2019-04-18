# VC++ ado事务处理 - 深之JohnChen的专栏 - CSDN博客

2017年09月03日 11:27:19[byxdaz](https://me.csdn.net/byxdaz)阅读数：1075


一、ADO中Connection对象事务处理

ADO中的事务处理也很简单，只需分别在适当的位置调用Connection对象的三个方法即可，这三个方法是：

　　（1）、在事务开始时调用

 pCnn->BeginTrans();

　　（2）、在事务结束并成功时调用

 pCnn->CommitTrans ();

　　（3）、在事务结束并失败时调用

pCnn->RollbackTrans ();

在使用事务处理时，应尽量减小事务的范围，即减小从事务开始到结束（提交或回滚）之间的时间间隔，以便提高系统效率。需要时也可在调用BeginTrans()方法之前，先设置Connection对象的IsolationLevel属性值，详细内容参见MSDN中有关ADO的技术资料。

二、 sql语句中BEGIN TRAN...COMMIT TRAN

BEGIN TRAN标记事务开始 

COMMIT TRAN 提交事务 

一般把DML语句(select ,delete,update,insert语句)放在BEGIN TRAN...COMMIT TRAN 之间作为一个事务处理 

也就是那些语句执行过程中如果遇到错误，无论哪句遇到错误，所有语句都会回滚到BEGIN TRAN之前的状态。

