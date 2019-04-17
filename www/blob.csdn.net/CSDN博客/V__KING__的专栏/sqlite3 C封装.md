# sqlite3 C封装 - V__KING__的专栏 - CSDN博客





2018年11月08日 17:46:22[v__king__](https://me.csdn.net/V__KING__)阅读数：89








二次封装：[https://blog.csdn.net/lifan1314521/article/details/51250350](https://blog.csdn.net/lifan1314521/article/details/51250350)
- 
回调函数

sqlite3_exec()有回调函数

sqlite3_step()， sqlite3_perpare()， sqlite3_finalize() 可以不用回调函数一步一步的去取数据去的字段。- 
速度效率上sqlite3_step快

参考：[https://blog.csdn.net/zongshiwujie/article/details/7394101](https://blog.csdn.net/zongshiwujie/article/details/7394101)

关于不同插入方法对插入速度的影响参考：[https://www.sqlite.org/faq.html#q19](https://www.sqlite.org/faq.html#q19)

中的“`(19) INSERT is really slow - I can only do few dozen INSERTs per second`”
> 
Actually, SQLite will easily do 50,000 or more INSERT statements per second on an average desktop computer. But it will only do a few dozen transactions per second. Transaction speed is limited by the rotational speed of your disk drive. A transaction normally requires two complete rotations of the disk platter, which on a 7200RPM disk drive limits you to about 60 transactions per second.

Transaction speed is limited by disk drive speed because (by default) SQLite actually waits until the data really is safely stored on the disk surface before the transaction is complete. That way, if you suddenly lose power or if your OS crashes, your data is still safe. For details, read about atomic commit in SQLite…


> 
By default, each INSERT statement is its own transaction. But if you surround multiple INSERT statements with BEGIN…COMMIT then all the inserts are grouped into a single transaction. The time needed to commit the transaction is amortized over all the enclosed insert statements and so the time per insert statement is greatly reduced.


> 
Another option is to run PRAGMA synchronous=OFF. This command will cause SQLite to not wait on data to reach the disk surface, which will make write operations appear to be much faster. But if you lose power in the middle of a transaction, your database file might go corrupt.

- 对sql事务理解：
[https://blog.csdn.net/qyx0714/article/details/70157749](https://blog.csdn.net/qyx0714/article/details/70157749)







