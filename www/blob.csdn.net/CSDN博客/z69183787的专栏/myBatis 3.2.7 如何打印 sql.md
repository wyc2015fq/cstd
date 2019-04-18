# myBatis 3.2.7 如何打印 sql - z69183787的专栏 - CSDN博客
2016年07月12日 20:39:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：882
**在Loggers节点下添加如下内容<!-- show sql--><logger name="你的mybatis mapper接口package路径" level="DEBUG"/><root level="DEBUG">            <appender-ref ref="console" />        </root>**
该文中使用的log框架为logback
# myBatis3.0.6左右的版本时
打印sql的时候只需要配置如下属性：
```
<logger name="java.sql.Connection" level="DEBUG" />
<logger name="java.sql.Statement" level="DEBUG" />
<logger name="java.sql.PreparedStatement" level="DEBUG" />
```
源码解析：
PreparedStatementLogger里面看这个log.isDebugEnabled()
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public Object invoke(Object proxy, Method method, Object[] params) throws Throwable {
    try {
      if (EXECUTE_METHODS.contains(method.getName())) {
        if (log.isDebugEnabled()) {
          log.debug("==>  Executing: " + removeBreakingWhitespace(sql));
          log.debug("==> Parameters: " + getParameterValueString());
        }
        clearColumnInfo();
        if ("executeQuery".equals(method.getName())) {
          ResultSet rs = (ResultSet) method.invoke(statement, params);
          if (rs != null) {
            return ResultSetLogger.newInstance(rs);
          } else {
            return null;
          }
        } else {
          return method.invoke(statement, params);
        }
      }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这个log定义的是PreparedStatement
```
private static final Log log = LogFactory.getLog(PreparedStatement.class);
```
# 在myBatis3.2.7左右版本
更改了打印Sql的模式，它将sql打印细化到了每一个mapperStatement的每一个方法上。
如果你打算有一个全局配置打印所有的sql，则需要如下配置
在mybatis的configuration中增加setting配置
```
<settings>
        <setting name="logPrefix" value="dao."/>
</settings>
```
然后增加配置
```
<logger name="dao" level="DEBUG"/>
```
源码解析：
ConnectionLogger
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public Object invoke(Object proxy, Method method, Object[] params)
      throws Throwable {
    try {
      if (Object.class.equals(method.getDeclaringClass())) {
        return method.invoke(this, params);
      }    
      if ("prepareStatement".equals(method.getName())) {
        if (isDebugEnabled()) {
          debug(" Preparing: " + removeBreakingWhitespace((String) params[0]), true);
        }        
        PreparedStatement stmt = (PreparedStatement) method.invoke(connection, params);
        stmt = PreparedStatementLogger.newInstance(stmt, statementLog, queryStack);
        return stmt;
      }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
其中的isDebugEnabled()指的是
```
protected boolean isDebugEnabled() {
    return statementLog.isDebugEnabled();
}
```
注意这里的statementLog，看SimpleExecutor的prepareStatement(handler, ms.getStatementLog());
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
public <E> List<E> doQuery(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) throws SQLException {
    Statement stmt = null;
    try {
      Configuration configuration = ms.getConfiguration();
      StatementHandler handler = configuration.newStatementHandler(wrapper, ms, parameter, rowBounds, resultHandler, boundSql);
      stmt = prepareStatement(handler, ms.getStatementLog());
      return handler.<E>query(stmt, resultHandler);
    } finally {
      closeStatement(stmt);
    }
  }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
这个statementLog是ms.getStatementLog()而来的。而MappedStatement的StatementLog
```
String logId = id;
 if (configuration.getLogPrefix() != null) logId = configuration.getLogPrefix() + id;
 mappedStatement.statementLog = LogFactory.getLog(logId);
```
这里可以看到，logPrefix决定了所有log前缀，所以只需要配置logPrefix就行了
