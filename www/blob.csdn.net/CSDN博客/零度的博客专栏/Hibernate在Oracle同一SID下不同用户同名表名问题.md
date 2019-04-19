# Hibernate在Oracle同一SID下不同用户同名表名问题 - 零度的博客专栏 - CSDN博客
2016年12月15日 11:15:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：1156
                
一般来说[hibernate](http://lib.csdn.net/base/javaee)配置[数据库](http://lib.csdn.net/base/mysql)连接只需要下面几行配置
**[html]**[view plain](http://blog.csdn.net/u012454773/article/details/52980632#)[copy](http://blog.csdn.net/u012454773/article/details/52980632#)[print](http://blog.csdn.net/u012454773/article/details/52980632#)[?](http://blog.csdn.net/u012454773/article/details/52980632#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1958066/fork)
- ##Oracle 9i/10g    
- hibernate.dialect org.hibernate.dialect.Oracle9Dialect    
- hibernate.connection.driver_class oracle.jdbc.driver.OracleDriver    
- hibernate.connection.username xxxx    
- hibernate.connection.password xxxx    
- hibernate.connection.url jdbc:oracle:thin:@127.0.0.1:1521:SID    
![](http://static.blog.csdn.net/images/save_snippets.png)
```
##Oracle 9i/10g  
hibernate.dialect org.hibernate.dialect.Oracle9Dialect  
hibernate.connection.driver_class oracle.jdbc.driver.OracleDriver  
hibernate.connection.username xxxx  
hibernate.connection.password xxxx  
hibernate.connection.url jdbc:oracle:thin:@127.0.0.1:1521:SID
```
Oracle是允许不同用户在同一个SID中创建同名的表的,但是如果按照上面的配置
Hibernate的SchemaUpdate操作是不会认帐的
SchemaUpdate指的hibernate配置成
**[html]**[view plain](http://blog.csdn.net/u012454773/article/details/52980632#)[copy](http://blog.csdn.net/u012454773/article/details/52980632#)[print](http://blog.csdn.net/u012454773/article/details/52980632#)[?](http://blog.csdn.net/u012454773/article/details/52980632#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1958066/fork)
- hibernate.hbm2ddl.auto update  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
`hibernate.hbm2ddl.auto update`例如：
    假设,有模型Entity,已用A用户运行过配置有hibernate的系统或者程序，则,此时数据库中有A.Entity表存在了。将hibernate配置用户改为B，然后运行系统或者程序。将会发现hibernate不会为我们创建B.Entity表。
    如果Entity模型在以A用户运行过后经过修改，同时会发现hibernate既不会创建B.Entity表，也不会更改A.Entity表的结构
如果想让hibernate执行上述描述的特殊操作方式需要在hibernate的配置上指明用户的Schema名，即在hibernate配置添加上

**[html]**[view plain](http://blog.csdn.net/u012454773/article/details/52980632#)[copy](http://blog.csdn.net/u012454773/article/details/52980632#)[print](http://blog.csdn.net/u012454773/article/details/52980632#)[?](http://blog.csdn.net/u012454773/article/details/52980632#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1958066/fork)
- hibernate.default_schema xxxx    
![](http://static.blog.csdn.net/images/save_snippets_01.png)
`hibernate.default_schema xxxx  `做个说明，Oracle中表的完整定位为username.tablename，而添加这个hibernate.default_schema的配置就是使hibernate在查找表的时候加上这个username的前缀
oracle中Schema含义解释参考：[Oracle中User与Schema的简单理解](http://blog.csdn.net/u012454773/article/details/52980596)
