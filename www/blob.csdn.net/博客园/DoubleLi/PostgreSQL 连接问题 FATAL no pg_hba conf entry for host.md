# PostgreSQL 连接问题 FATAL: no pg_hba.conf entry for host - DoubleLi - 博客园






The server doesn't grant access to the database: the server reports 
FATAL: no pg_hba.conf entry for host "192.168.0.123", user "postgres", database "postgres" FATAL: no pg_hba.conf entry for host "192.168.0.123", user "postgres", database "postgres"



PostgreSQL[数据库](http://lib.csdn.net/base/mysql)为了安全，它不会监听除本地以外的所有连接请求，当用户通过JDBC访问是，会报一些如下的异常：

org.postgresql.util.PSQLException: FATAL: no pg_hba.conf entry for host



要解决这个问题，只需要在PostgreSQL数据库的安装目录下找到/data/pg_hba.conf,找到“# IPv4 local connections:”

在其下加上请求连接的机器IP

host all all 127.0.0.1/32 md5

32是子网掩码的网段；md5是密码验证方法，可以改为trust









