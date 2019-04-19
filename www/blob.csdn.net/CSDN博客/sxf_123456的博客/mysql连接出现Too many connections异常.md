# mysql连接出现Too many connections异常 - sxf_123456的博客 - CSDN博客
2018年10月23日 10:55:44[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：70
异常:
com.mysql.jdbc.exceptions.jdbc4.MySQLNonTransientConnectionException: Too many connections
    at sun.reflect.NativeConstructorAccessorImpl.newInstance0(Native Method)
    at sun.reflect.NativeConstructorAccessorImpl.newInstance(NativeConstructorAccessorImpl.java:62)
    at sun.reflect.DelegatingConstructorAccessorImpl.newInstance(DelegatingConstructorAccessorImpl.java:45)
    at java.lang.reflect.Constructor.newInstance(Constructor.java:423)
    at com.mysql.jdbc.Util.handleNewInstance(Util.java:411)
    at com.mysql.jdbc.Util.getInstance(Util.java:386)
    at com.mysql.jdbc.SQLError.createSQLException(SQLError.java:1013)
    at com.mysql.jdbc.MysqlIO.checkErrorPacket(MysqlIO.java:3597)
    at com.mysql.jdbc.MysqlIO.checkErrorPacket(MysqlIO.java:3529)
    at com.mysql.jdbc.MysqlIO.checkErrorPacket(MysqlIO.java:935)
解决办法:
1、对每调用一个conn,对其进行close操作
2、通过修改数据库的配置参数
   (1)如果数据库文件使用的my.cnf文件
    找到文件 /etc/my.cnf
    编辑器打开，修改max_connections的值为10000。实际MySQL服务器允许的最大连接数16384。
    set-variable = max_connections=10000
   (2)如果数据库文件使用的my.ini文件
    找到文件/bin/my.ini（不一定在这里，根据你的数据库情况）
     在[mysqld]里面增加
     max_connections=10000
     如果原来就有，那么可以修改max_connections的值。
  (3)重启mysql
