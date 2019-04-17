# PostgreSQ 连接问题 FATAL: no pg_hba.conf entry for host - DoubleLi - 博客园






PostgreSQ[数据库](http://lib.csdn.net/base/mysql)为了安全，它不会监听除本地以外的所有连接请求，当用户通过JDBC访问是，会报一些如下的异常：

org.postgresql.util.PSQLException: FATAL: no pg_hba.conf entry for host



要解决这个问题，只需要在PostgreSQL数据库的安装目录下找到/data/pg_hba.conf,找到“# IPv4 local connections:”

在其下加上请求连接的机器IP

host all all 127.0.0.1/32 md5

32是子网掩码的网段；md5是密码验证方法，可以改（见文件pg_hba.conf上的说明）......





proxool建立数据库连接池代码



/*建立连接池*/

Properties info = new Properties();
info.setProperty(ProxoolConstants.USER_PROPERTY, this.username);
info.setProperty(ProxoolConstants.PASSWORD_PROPERTY, this.password); 
info.setProperty(ProxoolConstants.PROTOTYPE_COUNT_PROPERTY, this.prototypeConnections);
info.setProperty(ProxoolConstants.MINIMUM_CONNECTION_COUNT_PROPERTY, this.poolMinConnections);
info.setProperty(ProxoolConstants.MAXIMUM_CONNECTION_COUNT_PROPERTY, this.poolMaxConnections);

info.setProperty(ProxoolConstants.HOUSE_KEEPING_SLEEP_TIME_PROPERTY, waitTime);

url = "proxool." + this.dbType + ":" + this.driver + ":"+this.dbURL;
ProxoolFacade.registerConnectionPool(url, info);



/*取连接，其中 this.dbType是连接池的关联名称*/

DriverManager.getConnection("proxool." + this.dbType);









