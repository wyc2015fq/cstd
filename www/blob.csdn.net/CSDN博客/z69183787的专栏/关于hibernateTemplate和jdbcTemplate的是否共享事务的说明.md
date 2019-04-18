# 关于hibernateTemplate和jdbcTemplate的是否共享事务的说明 - z69183787的专栏 - CSDN博客
2015年01月04日 19:57:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1915
1、hibernateTemplate 与 jdbcTemplate 事务为共享。
2、但hibernate中的HQL方法，比如save update等 ，若不调用flush()，则数据只存在于缓冲区，未提交到数据库中，此时事务并未提交，
故共享事务中的 jdbctemplate无法查询到该数据；调用flush后，数据也可以共享。举例如下：
1、hibernateTemplate.save(obj)
2、jdbctemplate.queryforList(sql)
第二行中的select语句无法查询到第一行语句中save的数据。
1、hibernateTemplate.save(obj);flush();
2、jdbctemplate.queryforList(sql)
第二行即可查询到。
事务共享：jdbctemplate hibernatetemplate 共享数据事务，即与数据库同步，但hibernateTemplate有自身的缓存设置，所以在不调用 flush的情况下，可能产生数据丢失。
但若hibernateTemplate中调用 find 或 其他方法后，自动调用flush同步数据。而调用jdbctemplate时不会刷新hibernate的缓存。
调用flush后，新增数据只会存在于当前事务中，其他事务访问该表均无变化。
