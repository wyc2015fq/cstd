# spring整合mybatis sqlSession释放问题 - z69183787的专栏 - CSDN博客
2017年10月31日 14:20:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2553
[http://blog.csdn.net/u011686226/article/details/52704158](http://blog.csdn.net/u011686226/article/details/52704158)
MapperFactoryBean
![](https://img-blog.csdn.net/20160930105746082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
MapperFactoryBean实现了FactoryBean，所以
MapperFactoryBean.getObject -> SqlSessionTemplate.getMapper() -> Configuration.getMapper() -> MapperRegistry.getMapper() ->
mapperProxyFactory.newInstance(sqlSession);
最后用jdk动态代理生成了代理类，代理的接口是MapperProxyFactory的mapperInterface（就是Mapper），InvocationHandler是MapperProxy。
public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
    if(Object.class.equals(method.getDeclaringClass())) {
        try {
            return method.invoke(this, args);
} catch (Throwable var5) {
            throw ExceptionUtil.unwrapThrowable(var5);
}
    } else {
        MapperMethod mapperMethod = this.cachedMapperMethod(method);
        return mapperMethod.execute(this.sqlSession, args);
}
}invoke调用了sqlSession，这个sqlSession实际上是SqlSessionTemplate。这个template的构造函数
public SqlSessionTemplate(SqlSessionFactory sqlSessionFactory, ExecutorType executorType,
PersistenceExceptionTranslator exceptionTranslator) {
  notNull(sqlSessionFactory, "Property 'sqlSessionFactory' is required");
notNull(executorType, "Property 'executorType' is required");
  this.sqlSessionFactory = sqlSessionFactory;
  this.executorType = executorType;
  this.exceptionTranslator = exceptionTranslator;
  this.sqlSessionProxy = (SqlSession) newProxyInstance(
      SqlSessionFactory.class.getClassLoader(),
      new Class[] { SqlSession.class },
      new SqlSessionInterceptor());
}也初始化了一个代理sqlSessionProxy ，每次查询或者删除都是使用这个代理类的方法
public <T> T selectOne(String statement) {
  return this.sqlSessionProxy.<T> selectOne(statement);
}这个代理类使用了SqlSessionInterceptor，SqlSessionInterceptor会尝试获取当前线程的sqlsession,如果没有就使用sqlsessionfactory创建。
private class SqlSessionInterceptor implements InvocationHandler {
  @Override
public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
    SqlSession sqlSession = getSqlSession(
        SqlSessionTemplate.this.sqlSessionFactory,
SqlSessionTemplate.this.executorType,
SqlSessionTemplate.this.exceptionTranslator);
    try {
      Object result = method.invoke(sqlSession, args);
      if (!isSqlSessionTransactional(sqlSession, SqlSessionTemplate.this.sqlSessionFactory)) {
        // force commit even on non-dirty sessions because some databases require
        // a commit/rollback before calling close()
sqlSession.commit(true);
}
      return result;
} catch (Throwable t) {
      Throwable unwrapped = unwrapThrowable(t);
      if (SqlSessionTemplate.this.exceptionTranslator != null && unwrapped instanceof PersistenceException) {
        // release the connection to avoid a deadlock if the translator is no loaded. See issue #22
closeSqlSession(sqlSession, SqlSessionTemplate.this.sqlSessionFactory);
sqlSession = null;
Throwable translated = SqlSessionTemplate.this.exceptionTranslator.translateExceptionIfPossible((PersistenceException) unwrapped);
        if (translated != null) {
          unwrapped = translated;
}
      }
      throw unwrapped;
} finally {
      if (sqlSession != null) {
        closeSqlSession(sqlSession, SqlSessionTemplate.this.sqlSessionFactory);
}
    }
  }
}完成了每次请求sqlSession的获取和释放。
也就是我们写的dao最后一个代理，每次涉及数据库操作时会调用SqlSessionTemplate，SqlSessionTemplate也有一个代理类，在在代理类里面获取sqlsession，然后发生实际的操作。
http://www.cnblogs.com/daxin/p/3544188.html
http://ju.outofmemory.cn/entry/62780
http://fangjian0423.github.io/2014/09/06/MapperScannerConfigurer-analysis/
