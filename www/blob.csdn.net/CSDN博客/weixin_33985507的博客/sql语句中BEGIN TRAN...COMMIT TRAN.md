# sql语句中BEGIN TRAN...COMMIT TRAN - weixin_33985507的博客 - CSDN博客
2014年10月26日 17:18:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：14
BEGIN TRAN标记事务開始 
COMMIT TRAN 提交事务 
一般把DML语句(select ,delete,update,insert语句)放在BEGIN TRAN...COMMIT TRAN 之间作为一个事务处理 
也就是那些语句运行过程中假设遇到错误，不管哪句遇到错误，全部语句都会回滚到BEGIN TRAN之前的状态。
