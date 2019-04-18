# Hibernate Session中flush、clear、evict方法的区别 - z69183787的专栏 - CSDN博客
2013年12月27日 10:22:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1106
flush：
 该方法进行清理缓存的操作，执行一系列的SQL语句，但不提交事务。由此可见该方法提前将SQL缓冲区的SQL语句发送到数据库中。另外执行flush方法时Hibernate会对当前所操作的对象加锁，且该锁为行级别锁，防止其他事务对当前对象脏读。（强制数据库与DB同步）
      flush方法是可以设置的，也就是 fulsh 什么时候执行是可以设置的。
在session.beginTransaction 前设置 FlushMode
      session.setFlushMode(FlushMode.Always|AUTO|COMMIT|NEVER|MANUAL)
      FlushMode有 5 个值可选：
      Always:任何代码都会 Flush 
      AUTO:默认方式 – 自动 
      Commit:COMMIT时 
      Never:始终不 
      MANUAL:手动方式
      设置FlushMode 有个好处就是可以节省开销，比如默认 session 只做查询时，就可以不让他与数据库同步了。
clear：
该方法会强制清空当前session中的缓存 。
evict：
 清除指定的缓存对象。session.evict(ohj)
