# 使用Log4j2打印Mybatis SQL语句以及结果集 - z69183787的专栏 - CSDN博客
2016年10月25日 18:02:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12775
                
配置log4j2.xml
实现用Log4j2来打印Mybatis的SQL很简单,先配置一个name为consolePrint的附加器,指定输出格式 
然后在loggers下配置一个logger,name指向项目持久层接口的package,也就是和Mybatis配置文件对应的接口包,再定义输出方式就可以了
```
<?xml version="1.0" encoding="UTF-8"?>
<Configuration status="INFO" monitorInterval="1800">
    <appenders>
        <Console name="consolePrint" target="SYSTEM_OUT">
            <PatternLayout pattern="%d{HH:mm:ss} [%t] %-5level %logger{36} - %msg%n" />
        </Console>
    </appenders>
    <loggers>
        <!-- 将业务dao接口填写进去,并用控制台输出即可 -->
        <logger name="com.amayadream.freemarker.dao" level="DEBUG" additivity="false">
            <appender-ref ref="consolePrint"/>
        </logger>
        <root level="info">
            <appender-ref ref="consolePrint" />
        </root>
    </loggers>
</Configuration>
```
这里要注意,如果将level定义为DEBUG则只会打印出SQL语句,SQL参数以及结果条数,例如下面
```
23:02:58 [http-apr-8090-exec-17] DEBUG com.amayadream.freemarker.dao.IPhysicalDao.showColumns - ==>  Preparing: select t.column_name, nvl(c.comments, t.column_name) as comments, t.data_type, t.data_length, t.data_precision, t.data_scale, nullable from user_tab_columns t left join user_col_comments c on c.table_name = t.table_name and c.column_name = t.column_name where t.table_name = ? 
23:02:58 [http-apr-8090-exec-17] DEBUG com.amayadream.freemarker.dao.IPhysicalDao.showColumns - ==> Parameters: WEBCHAT_LOG(String)
23:02:58 [http-apr-8090-exec-17] DEBUG com.amayadream.freemarker.dao.IPhysicalDao.showColumns - <==      Total: 6
```
如果将level定义为TRACE则会打印出SQL语句,SQL参数以及结果集还有结果条数,例如下面:
```
23:00:36 [http-apr-8090-exec-11] DEBUG com.amayadream.freemarker.dao.IPhysicalDao.showColumns - ==>  Preparing: select t.column_name, nvl(c.comments, t.column_name) as comments, t.data_type, t.data_length, t.data_precision, t.data_scale, nullable from user_tab_columns t left join user_col_comments c on c.table_name = t.table_name and c.column_name = t.column_name where t.table_name = ? 
23:00:36 [http-apr-8090-exec-11] DEBUG com.amayadream.freemarker.dao.IPhysicalDao.showColumns - ==> Parameters: WEBCHAT_LOG(String)
23:00:36 [http-apr-8090-exec-11] TRACE com.amayadream.freemarker.dao.IPhysicalDao.showColumns - <==    Columns: COLUMN_NAME, COMMENTS, DATA_TYPE, DATA_LENGTH, DATA_PRECISION, DATA_SCALE, NULLABLE
23:00:36 [http-apr-8090-exec-11] TRACE com.amayadream.freemarker.dao.IPhysicalDao.showColumns - <==        Row: ID, 日志编号, VARCHAR2, 32, null, null, N
23:00:36 [http-apr-8090-exec-11] TRACE com.amayadream.freemarker.dao.IPhysicalDao.showColumns - <==        Row: USERID, 用户名, VARCHAR2, 32, null, null, N
23:00:36 [http-apr-8090-exec-11] TRACE com.amayadream.freemarker.dao.IPhysicalDao.showColumns - <==        Row: TIME, 时间, VARCHAR2, 32, null, null, Y
23:00:36 [http-apr-8090-exec-11] TRACE com.amayadream.freemarker.dao.IPhysicalDao.showColumns - <==        Row: TYPE, 类型, VARCHAR2, 32, null, null, Y
23:00:36 [http-apr-8090-exec-11] TRACE com.amayadream.freemarker.dao.IPhysicalDao.showColumns - <==        Row: DETAIL, 详情, VARCHAR2, 300, null, null, Y
23:00:36 [http-apr-8090-exec-11] TRACE com.amayadream.freemarker.dao.IPhysicalDao.showColumns - <==        Row: IP, ip地址, VARCHAR2, 32, null, null, Y
23:00:36 [http-apr-8090-exec-11] DEBUG com.amayadream.freemarker.dao.IPhysicalDao.showColumns - <==      Total: 6
```
