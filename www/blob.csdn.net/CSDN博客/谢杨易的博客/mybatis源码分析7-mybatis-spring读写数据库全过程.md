
# mybatis源码分析7 - mybatis-spring读写数据库全过程 - 谢杨易的博客 - CSDN博客

2018年01月13日 22:00:26[谢杨易](https://me.csdn.net/u013510838)阅读数：529标签：[数据库																](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)[spring																](https://so.csdn.net/so/search/s.do?q=spring&t=blog)[mybatis																](https://so.csdn.net/so/search/s.do?q=mybatis&t=blog)[线程安全																](https://so.csdn.net/so/search/s.do?q=线程安全&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=mybatis&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=spring&t=blog)个人分类：[Java																](https://blog.csdn.net/u013510838/article/category/7025289)
[
																								](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=数据库&t=blog)


## 1 引言
mybatis-spring中，我们利用Spring容器注入的方式创建了sqlSessionFactory，从而完成了mybatis的初始化。那么如何来读写数据库呢？最简单的方式是，和mybatis中一样，利用sqlSessionFactory的openSession来创建sqlSession，然后利用它来select或update，或者mapper方式。这种方式每次都需要手动openSession创建sqlSession对象，和Spring将对象创建和管理交给容器的理念不相符。那么有同学肯定就会说，直接用Spring容器注入sqlSession不就行了吗。但是很不幸，sqlSession是线程不安全的。那么我们该如何做呢？Spring给出了完美的解决方案，sqlSessionTemplete，一个线程安全的SqlSession实现类。使用它的例子如下
```python
<!--Spring配置文件中声明SqlSessionTemplate-->
```
```python
<
```
```python
bean
```
```python
id
```
```python
=
```
```python
"sqlSession"
```
```python
class
```
```python
=
```
```python
"org.mybatis.spring.SqlSessionTemplate"
```
```python
>
```
```python
<
```
```python
constructor-arg
```
```python
index
```
```python
=
```
```python
"0"
```
```python
ref
```
```python
=
```
```python
"sqlSessionFactory"
```
```python
/>
```
```python
</
```
```python
bean
```
```python
>
```
```python
public
```
```python
class
```
```python
UserDaoImpl
```
```python
implements
```
```python
UserDao
```
```python
{
```
```python
// sqlSession由Spring容器注入
```
```python
private
```
```python
SqlSession sqlSession;
```
```python
public
```
```python
void
```
```python
setSqlSession
```
```python
(SqlSession sqlSession) {
```
```python
this
```
```python
.sqlSession = sqlSession;
    }
```
```python
// 使用sqlSession的selectOne方法操作数据库
```
```python
public
```
```python
User
```
```python
getUser
```
```python
(String userId) {
```
```python
return
```
```python
(User) sqlSession.selectOne(
```
```python
"org.mybatis.spring.sample.mapper.UserMapper.getUser"
```
```python
, userId);
    }
}
```
有了SqlSessionTemplate这个线程安全的sqlSession实现类后，我们就可以将sqlSession创建交给容器来处理了，不需要每次数据库操作都openSession()和close()了。然后和使用原生mybatis一样，可以使用sqlSession的select等直接CRUD方法，或者mapper方式，进行数据库读写了。这儿就有两个问题了
容器是如何创建SqlSessionTemplate对象的？
SqlSessionTemplate是如何解决线程不安全问题的？
带着这两个问题，我们一步步来揭开Spring容器中读写数据库的全过程。
## 2 Spring容器读写数据库流程
我们先来看第一个问题，容器是如何创建SqlSessionTemplate对象的。
### 2.1 容器创建SqlSessionTemplate对象的过程
来看SqlSessionTemplate的构造方法。
```python
// spring bean注入时构造方法
```
```python
public
```
```python
SqlSessionTemplate
```
```python
(SqlSessionFactory sqlSessionFactory) {
```
```python
this
```
```python
(sqlSessionFactory, sqlSessionFactory.getConfiguration().getDefaultExecutorType());
}
```
```python
public
```
```python
SqlSessionTemplate
```
```python
(SqlSessionFactory sqlSessionFactory, ExecutorType executorType) {
```
```python
this
```
```python
(sqlSessionFactory, executorType,
```
```python
new
```
```python
MyBatisExceptionTranslator(
            sqlSessionFactory.getConfiguration().getEnvironment().getDataSource(),
```
```python
true
```
```python
));
}
```
```python
// 最终调用的构造方法
```
```python
public
```
```python
SqlSessionTemplate
```
```python
(SqlSessionFactory sqlSessionFactory, ExecutorType executorType,
      PersistenceExceptionTranslator exceptionTranslator) {
```
```python
// 属性校验，XML属性配置中必须传入sqlSessionFactory
```
```python
notNull(sqlSessionFactory,
```
```python
"Property 'sqlSessionFactory' is required"
```
```python
);
    notNull(executorType,
```
```python
"Property 'executorType' is required"
```
```python
);
```
```python
this
```
```python
.sqlSessionFactory = sqlSessionFactory;
```
```python
this
```
```python
.executorType = executorType;
```
```python
this
```
```python
.exceptionTranslator = exceptionTranslator;
```
```python
// 这儿是关键，创建sqlSession动态代理。
```
```python
// SqlSessionTemplate的几乎所有操作，如select update delete都是通过这个代理完成的
```
```python
// 故最终还是调用的sqlSession的select update delete等方法
```
```python
// 方法调用是，触发InvocationHandler，也就是这儿的SqlSessionInterceptor的invoke方法
```
```python
this
```
```python
.sqlSessionProxy = (SqlSession) newProxyInstance(
        SqlSessionFactory.class.getClassLoader(),
```
```python
new
```
```python
Class[] { SqlSession.class },
```
```python
new
```
```python
SqlSessionInterceptor());
}
```
我们看第三个构造方法即可，首先进行参数校验，确保sqlSessionFactory等属性已经构造好了，然后创建Proxy动态代理，传入的InvocationHandler是SqlSessionInterceptor，之后sqlSessionTemplete的方法调用，如select update delete，都会由SqlSessionInterceptor的invoke方法完成。不清楚动态代理的同学建议好好复习下Java反射。
sqlSessionTemplete的selectOne update等方法均通过sqlSessionProxy代理完成。如下
```python
public
```
```python
<T> T
```
```python
selectOne
```
```python
(String statement) {
```
```python
// sqlSessionTemplete的select update等方法都是通过sqlSessionProxy代理完成的。
```
```python
return
```
```python
this
```
```python
.sqlSessionProxy.selectOne(statement);
}
```
容器创建sqlSessionTemplete对象其实很简单，最关键的一点是创建了sqlSessionProxy动态代理，其数据库操作均是通过这个动态代理完成的。接下来我们详细分析这个动态代理的运行过程，以及它是如何保证线程安全的。
### 2.2 SqlSessionTemplate线程安全地操作数据库
接着上面说，sqlSessionTemplete对数据库的操作，都是通过代理模式，由sqlSessionProxy完成的。而sqlSessionProxy是一个动态代理，其方法调用，都是通过回调内部的InvocationHandler的invoke方法完成的。我们创建sqlSessionProxy动态代理时，传入的InvocationHandler是SqlSessionInterceptor对象，故select update等数据库操作，都是经过它的invoke方法完成的，我们下面详细分析。
```python
private
```
```python
class
```
```python
SqlSessionInterceptor
```
```python
implements
```
```python
InvocationHandler
```
```python
{
```
```python
@Override
```
```python
// 动态代理执行方法调用时，回调InvocationHandler的invoke方法.
```
```python
// 故使用sqlSessionTemplete执行数据库select insert等操作时，从invoke方法进入
```
```python
public
```
```python
Object
```
```python
invoke
```
```python
(Object proxy, Method method, Object[] args)
```
```python
throws
```
```python
Throwable {
```
```python
// 获取sqlSession实例，底层处理了线程同步问题，故SqlSessionTemplete是线程安全的
```
```python
// 从sessionHolder中获取，或者通过sqlSessionFactory的openSession()方法创建。
```
```python
// 这就是为什么SqlSessionTemplete是线程安全的原因所在了，这儿也充分体现了mybatis-spring的设计精妙
```
```python
SqlSession sqlSession = getSqlSession(
        SqlSessionTemplate.
```
```python
this
```
```python
.sqlSessionFactory,
        SqlSessionTemplate.
```
```python
this
```
```python
.executorType,
        SqlSessionTemplate.
```
```python
this
```
```python
.exceptionTranslator);
```
```python
try
```
```python
{
```
```python
// 方法的反射调用，第一个入参为调用者对象，第二个参数为入参列表。不清楚的同学复习下Java反射
```
```python
// 故其实就是调用sqlSession对象的method方法，入参为args。
```
```python
Object result = method.invoke(sqlSession, args);
```
```python
// 如果不是由事务来管理，则强制sqlSession commit一次，因为有些数据库在close前必须commit
```
```python
if
```
```python
(!isSqlSessionTransactional(sqlSession, SqlSessionTemplate.
```
```python
this
```
```python
.sqlSessionFactory)) {
        sqlSession.commit(
```
```python
true
```
```python
);
      }
```
```python
return
```
```python
result;
    }
```
```python
catch
```
```python
(Throwable t) {
```
```python
// 异常处理，关闭sqlSession
```
```python
Throwable unwrapped = unwrapThrowable(t);
```
```python
if
```
```python
(SqlSessionTemplate.
```
```python
this
```
```python
.exceptionTranslator !=
```
```python
null
```
```python
&& unwrapped
```
```python
instanceof
```
```python
PersistenceException) {
```
```python
// release the connection to avoid a deadlock if the translator is no loaded. See issue #22
```
```python
closeSqlSession(sqlSession, SqlSessionTemplate.
```
```python
this
```
```python
.sqlSessionFactory);
        sqlSession =
```
```python
null
```
```python
;
        Throwable translated = SqlSessionTemplate.
```
```python
this
```
```python
.exceptionTranslator.translateExceptionIfPossible((PersistenceException) unwrapped);
```
```python
if
```
```python
(translated !=
```
```python
null
```
```python
) {
          unwrapped = translated;
        }
      }
```
```python
throw
```
```python
unwrapped;
    }
```
```python
finally
```
```python
{
```
```python
if
```
```python
(sqlSession !=
```
```python
null
```
```python
) {
        closeSqlSession(sqlSession, SqlSessionTemplate.
```
```python
this
```
```python
.sqlSessionFactory);
      }
    }
  }
}
```
invoke方法的步骤主要为
getSqlSession()方法获取sqlSession实例，如何保证sqlSession线程安全的，也是隐含的这个方法中，我们后面会详细分析。
method.invoke(), 执行方法调用。如select update等方法。这是Java反射的方法调用通用方式。
我们下面来详细分析getSqlSession()方法。
```python
// 获取sqlSession实例，它保证了线程安全
```
```python
public
```
```python
static
```
```python
SqlSession
```
```python
getSqlSession
```
```python
(SqlSessionFactory sessionFactory, ExecutorType executorType, PersistenceExceptionTranslator exceptionTranslator) {
  notNull(sessionFactory, NO_SQL_SESSION_FACTORY_SPECIFIED);
  notNull(executorType, NO_EXECUTOR_TYPE_SPECIFIED);
```
```python
// 先从同步事务管理器的 ThreadLocal map中，取出sqlSessionFactory对应的SqlSessionHolder，它是sqlSession的包装类
```
```python
// 同一线程的同一SqlSessionFactory，才对应同一个sqlSession对象
```
```python
// ThreadLocal的存在保证了sqlSession的线程安全
```
```python
SqlSessionHolder holder = (SqlSessionHolder) TransactionSynchronizationManager.getResource(sessionFactory);
```
```python
// 从sessionHolder缓存中取出SqlSession对象，获取到后就可以将sqlSession返回了。
```
```python
SqlSession session = sessionHolder(executorType, holder);
```
```python
if
```
```python
(session !=
```
```python
null
```
```python
) {
```
```python
return
```
```python
session;
  }
```
```python
// 没有获取到session时，创建sqlSession实例，还是通过sqlSessionFactory的openSession()方法
```
```python
LOGGER.debug(() ->
```
```python
"Creating a new SqlSession"
```
```python
);
  session = sessionFactory.openSession(executorType);
```
```python
// 将构造好的sqlSession封装到SqlSessionHolder缓存中，然后添加到同步事务管理器的threadLocal队列中
```
```python
// 不同线程下ThreadLocal有不同的实例，这是由于它的存在，保证了sqlSession是线程安全的
```
```python
registerSessionHolder(sessionFactory, executorType, exceptionTranslator, session);
```
```python
return
```
```python
session;
}
```
getSqlSession先从事务管理器的map中取出当前sqlSessionFactory对应的SqlSessionHolder，它是sqlSession的包装类。然后从sessionHolder缓存中取出SqlSession对象并返回。如果map中取不到SqlSession（比如之前根本就没有创建过），那么就需要先openSession来创建SqlSession了。然后将sqlSession添加到事务管理器的map中。这个过程理解起来其实不复杂，简单来说就是缓存命中则从缓存中取，未命中则创建并写入到缓存中。创建sqlSession还是使用的mybatis原生方法openSession(), 那么是如何保证线程安全的呢。关键就在registerSessionHolder()方法中。
```python
// 构造SqlSessionHolder，并将它添加到事务管理器的threadLocal队列中，然后注册并开启事务同步功能，这样才能保证获取sqlSession时是线程安全的了
```
```python
private
```
```python
static
```
```python
void
```
```python
registerSessionHolder
```
```python
(SqlSessionFactory sessionFactory, ExecutorType executorType,
    PersistenceExceptionTranslator exceptionTranslator, SqlSession session) {
  SqlSessionHolder holder;
```
```python
// 开启了事务同步功能时
```
```python
if
```
```python
(TransactionSynchronizationManager.isSynchronizationActive()) {
    Environment environment = sessionFactory.getConfiguration().getEnvironment();
```
```python
if
```
```python
(environment.getTransactionFactory()
```
```python
instanceof
```
```python
SpringManagedTransactionFactory) {
      LOGGER.debug(() ->
```
```python
"Registering transaction synchronization for SqlSession ["
```
```python
+ session +
```
```python
"]"
```
```python
);
```
```python
// 构造SqlSessionHolder缓存对象，并添加到事务管理器的map中，之后每次从map中取，而不用再创建sqlSession了
```
```python
holder =
```
```python
new
```
```python
SqlSessionHolder(session, executorType, exceptionTranslator);
      TransactionSynchronizationManager.bindResource(sessionFactory, holder);
```
```python
// 注册事务同步，TransactionSynchronizationManager管理了一个TransactionSynchronization队列，
```
```python
// 它是一个ThreadLocal，不同线程下有不同的实例，这就是解决线程安全问题的关键所在,
```
```python
// 代码为 ThreadLocal<Set<TransactionSynchronization>> synchronizations = new NamedThreadLocal("Transaction synchronizations");
```
```python
TransactionSynchronizationManager.registerSynchronization(
```
```python
new
```
```python
SqlSessionSynchronization(holder, sessionFactory));
```
```python
// 开启事务同步功能
```
```python
holder.setSynchronizedWithTransaction(
```
```python
true
```
```python
);
```
```python
// 加锁
```
```python
holder.requested();
    }
```
```python
else
```
```python
{
```
```python
if
```
```python
(TransactionSynchronizationManager.getResource(environment.getDataSource()) ==
```
```python
null
```
```python
) {
        LOGGER.debug(() ->
```
```python
"SqlSession ["
```
```python
+ session +
```
```python
"] was not registered for synchronization because DataSource is not transactional"
```
```python
);
      }
```
```python
else
```
```python
{
```
```python
throw
```
```python
new
```
```python
TransientDataAccessResourceException(
```
```python
"SqlSessionFactory must be using a SpringManagedTransactionFactory in order to use Spring transaction synchronization"
```
```python
);
      }
    }
  }
```
```python
else
```
```python
{
    LOGGER.debug(() ->
```
```python
"SqlSession ["
```
```python
+ session +
```
```python
"] was not registered for synchronization because synchronization is not active"
```
```python
);
  }
```
registerSessionHolder()方法先构造SqlSessionHolder缓存对象，并添加到事务管理器的map中缓存起来。然后创建SqlSessionSynchronization对象并添加到事务管理器TransactionSynchronizationManager中（这儿是线程安全的关键所在）。最后开启事务同步功能并申请锁。
我们来看registerSynchronization()方法
```python
// synchronizations是一个ThreadLocal，每个线程下都有不同的实例
```
```python
private
```
```python
static
```
```python
final
```
```python
ThreadLocal<Set<TransactionSynchronization>> synchronizations =
```
```python
new
```
```python
NamedThreadLocal(
```
```python
"Transaction synchronizations"
```
```python
);
```
```python
public
```
```python
static
```
```python
void
```
```python
registerSynchronization
```
```python
(TransactionSynchronization synchronization)
```
```python
throws
```
```python
IllegalStateException {
    Assert.notNull(synchronization,
```
```python
"TransactionSynchronization must not be null"
```
```python
);
```
```python
if
```
```python
(!isSynchronizationActive()) {
```
```python
throw
```
```python
new
```
```python
IllegalStateException(
```
```python
"Transaction synchronization is not active"
```
```python
);
    }
```
```python
else
```
```python
{
```
```python
// 从ThreadLocal中获取本线程下的synchronizations队列，然后将新创建的对象添加进去
```
```python
((Set)synchronizations.get()).add(synchronization);
    }
}
```
看到registerSynchronization()方法是应该就恍然大悟了吧，原来是利用ThreadLocal这个线程安全类来实现的呀。不同线程下，ThreadLocal会创建不同对象，故对象只在固定线程下可见，也就是我们通常所说的线程作用域。这样就完全保证了不同线程不可能调用同一个对象了，也就是线程安全了。Spring利用ThreadLocal这个大杀器完美解决了sqlSession线程不安全问题了。
## 3 总结
mybatis-Spring读写数据库，完美解决了两大问题。一是sqlSession对象创建和管理完全交给Spring容器。二是解决了sqlSession线程不安全问题。至于读写数据库，其实还是通过原生mybatis，sqlSession的select update等方法或者mapper方式。由此可见mybatis-spring仅仅是扩展了mybatis的功能，并适配到Spring容器中而已，并没有去侵入mybatis代码并从本质上去改变他的运行方式。这些都是我们设计框架时可以借鉴的。
### 相关文章
[mybatis源码分析1 - 框架](http://blog.csdn.net/u013510838/article/details/79055792)
[mybatis源码分析2 - SqlSessionFactory的创建](http://blog.csdn.net/u013510838/article/details/78995542)
[mybatis源码分析3 - sqlSession的创建](http://blog.csdn.net/u013510838/article/details/79011489)
[mybatis源码分析4 - sqlSession读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79016238)
[mybatis源码分析5 - mapper读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79023822)
[mybatis源码分析6 - mybatis-spring容器初始化](http://blog.csdn.net/u013510838/article/details/79053519)
[mybatis源码分析7 - mybatis-spring读写数据库全过程](http://blog.csdn.net/u013510838/article/details/79054007)

