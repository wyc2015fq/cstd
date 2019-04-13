
# mybatis源码分析3 - sqlSession的创建 - 谢杨易的博客 - CSDN博客

2018年01月09日 12:03:12[谢杨易](https://me.csdn.net/u013510838)阅读数：712



## 1 引言和主要类
初始化mybatis，也就是创建完单例SqlSessionFactory后，就进入到了mybatis的运行阶段。mybatis每次的运行都是通过SqlSession对象来进行，它是运行时的核心。不同于SqlSessionFactory，SqlSession不是线程安全的，故一般建议放在局部作用域中定义, 且使用完后close掉。我们使用mybatis创建SqlSession十分简单，代码如下
```python
// 读入xml配置文件
```
```python
String resource =
```
```python
"main/resources/SqlMapConfig.xml"
```
```python
;
InputStream inputStream = Resources.getResourceAsStream(resource);
```
```python
// 创建SqlSessionFactory，初始化mybatis的关键所在，上一节分析过了
```
```python
SqlSessionFactory sessionFactory =
```
```python
new
```
```python
SqlSessionFactoryBuilder().build(inputStream);
```
```python
// 利用SqlSessionFactory来创建SqlSession，这一节重点分析
```
```python
SqlSession session = sessionFactory.openSession();
```
创建SqlSession十分简单，调用SqlSessionFactory的openSession()方法即可。但这个方法背后其实相当复杂，mybatis为我们做了很多幕后工作。我们先介绍下openSession()过程中要用到的主要类。
DefaultSqlSessionFactory：SqlSessionFactory的默认实现类，上一节我们讲到过。
Transaction：数据库事务，有JdbcTransaction和ManagedTransaction两个实现
Executor：调度器，sqlSession的select update等方法的真正实现，mybatis运行的核心。
DefaultSqlSession：sqlSession的默认实现类，其方法几乎均由Executor代理实现。
## 2 流程分析
我们从DefaultSqlSessionFactory这个默认工厂类的openSession()方法说起。
```python
// SqlSessionFactory单例创建并开启一个sqlSession实例。sqlSession线程不安全，一般存放在局部作用域中，用完close即可。
```
```python
public
```
```python
SqlSession
```
```python
openSession
```
```python
() {
```
```python
return
```
```python
openSessionFromDataSource(configuration.getDefaultExecutorType(),
```
```python
null
```
```python
,
```
```python
false
```
```python
);
}
```
```python
// 开启sqlSession
```
```python
private
```
```python
SqlSession
```
```python
openSessionFromDataSource
```
```python
(ExecutorType execType, TransactionIsolationLevel level,
```
```python
boolean
```
```python
autoCommit) {
    Transaction tx =
```
```python
null
```
```python
;
```
```python
try
```
```python
{
```
```python
// 获取configuration的environment，它代表了运行的数据库环境，
```
```python
// 由配置文件中的environments节点的environment子节点生成，创建SqlSessionFactory时指定其id，默认为default
```
```python
final
```
```python
Environment environment = configuration.getEnvironment();
```
```python
// environment实例中取出transactionFactory成员变量，然后实例化它。
```
```python
// JdbcTransactionFactory创建JdbcTransaction，使用JDBC代理管理commit等事务
```
```python
// ManagedTransactionFactory创建ManagedTransaction，自身不对事务进行处理，完全交给容器，如Spring
```
```python
final
```
```python
TransactionFactory transactionFactory = getTransactionFactoryFromEnvironment(environment);
      tx = transactionFactory.newTransaction(environment.getDataSource(), level, autoCommit);
```
```python
// 由事务transaction创建调度器Executor，SqlSession的几乎所有方法都是通过代理模式由Executor真正实现
```
```python
// Executor代表调度器，由他来调度StatementHandler ParameterHandler ResultSetHandler。四者合称SqlSession四大组件
```
```python
// ExecutorType在XML配置文件的settings节点中设置(defaultExecutorType), 可以取SIMPLE REUSE BATCH，默认为SIMPLE
```
```python
// SIMPLE表示简易执行器，REUSE为一种执行器重用预处理语句，BATCH则为批量专用的执行器。
```
```python
final
```
```python
Executor executor = configuration.newExecutor(tx, execType);
```
```python
// 构造SqlSession实例，mybatis默认的实现类为DefaultSqlSession
```
```python
return
```
```python
new
```
```python
DefaultSqlSession(configuration, executor, autoCommit);
    }
```
```python
catch
```
```python
(Exception e) {
      closeTransaction(tx);
```
```python
// may have fetched a connection so lets call close()
```
```python
throw
```
```python
ExceptionFactory.wrapException(
```
```python
"Error opening session.  Cause: "
```
```python
+ e, e);
    }
```
```python
finally
```
```python
{
      ErrorContext.instance().reset();
    }
  }
```
mybatis先由初始化阶段创建在configuration中的environment变量来创建数据库事务，然后根据XML配置文件的settings节点中设置的defaultExecutorType来创建调度器Executor，最后就可以创建得到DefaultSqlSession实例了。下面我们来分析事务的创建
### 2.1 数据库事务Transaction的创建
Transaction有两个实现类，即JdbcTransaction和ManagedTransaction，他们分别由JdbcTransactionFactory和ManagedTransactionFactory工厂类来创建实例。JdbcTransaction本地事务由数据库本身来管理事务，其getConnection()从DataSource中得到， commit() rollback() close() 则由java.sql.Connection实例代理实现。
ManagedTransaction由容器来管理事务，其getConnection()从DataSource中得到，commit() rollback()均为空实现，不做任何事情，完全托管给容器。
下面以JDBCTransaction为例来看Transaction事务的创建
```python
// 工厂类来创建事务实例
```
```python
// @Param DataSource： 数据库源
```
```python
// @Param level: 事务隔离级别,定义并发事务的处理方式。如A读时，B在写。
```
```python
// @param autoCommit： 是否自动提交
```
```python
public
```
```python
Transaction
```
```python
newTransaction
```
```python
(DataSource ds, TransactionIsolationLevel level,
```
```python
boolean
```
```python
autoCommit) {
```
```python
return
```
```python
new
```
```python
JdbcTransaction(ds, level, autoCommit);
  }
```
### 2.2 调度器Executor的创建
Executor是SqlSession的核心，其select update commit rollback close等方法均由Executor代理实现。Executor代表调度器，由他来调度StatementHandler ParameterHandler ResultSetHandler。四者合称mybatis运行时四大组件。四大组件均可由用户通过插件注入，也都有默认实现。下面看创建Executor的源码
```python
// 创建sqlsession执行器Executor
```
```python
public
```
```python
Executor
```
```python
newExecutor
```
```python
(Transaction transaction, ExecutorType executorType) {
```
```python
// executorType通过settings节点中的defaultExecutorType来设置，没有设置则默认为SIMPLE
```
```python
executorType = executorType ==
```
```python
null
```
```python
? defaultExecutorType : executorType;
  executorType = executorType ==
```
```python
null
```
```python
? ExecutorType.SIMPLE : executorType;
  Executor executor;
```
```python
// 根据executorType分别创建BatchExecutor ReuseExecutor SimpleExecutor
```
```python
if
```
```python
(ExecutorType.BATCH == executorType) {
    executor =
```
```python
new
```
```python
BatchExecutor(
```
```python
this
```
```python
, transaction);
  }
```
```python
else
```
```python
if
```
```python
(ExecutorType.REUSE == executorType) {
    executor =
```
```python
new
```
```python
ReuseExecutor(
```
```python
this
```
```python
, transaction);
  }
```
```python
else
```
```python
{
    executor =
```
```python
new
```
```python
SimpleExecutor(
```
```python
this
```
```python
, transaction);
  }
```
```python
// 如果打开了缓存，使用CachingExecutor包装下之前创建的executor，简单理解就是为executor添加了cache功能
```
```python
if
```
```python
(cacheEnabled) {
    executor =
```
```python
new
```
```python
CachingExecutor(executor);
  }
```
```python
// 将执行器executor设置到plugins节点中设置的所有插件中，作为插件的目标执行器。
```
```python
executor = (Executor) interceptorChain.pluginAll(executor);
```
```python
return
```
```python
executor;
}
```
```python
public
```
```python
Object
```
```python
pluginAll
```
```python
(Object target) {
```
```python
// 遍历所有插件，将传入的target，作为插件的目标执行器。插件通过配置XML文件的plugins节点设置。
```
```python
for
```
```python
(Interceptor interceptor : interceptors) {
      target = interceptor.plugin(target);
    }
```
```python
return
```
```python
target;
  }
```
Executor根据executorType分别创建BatchExecutor ReuseExecutor SimpleExecutor三个实现类，他们的基类为BaseExecutor。下一节我们分析selectList方法时，以SimpleExecutor这个默认的调度器为例分析。
### 2.3 SqlSession实例的创建
SqlSession的默认实现为DefaultSqlSession，其创建十分简单，构造方法如下
```python
public
```
```python
DefaultSqlSession
```
```python
(Configuration configuration, Executor executor,
```
```python
boolean
```
```python
autoCommit) {
```
```python
this
```
```python
.configuration = configuration;
```
```python
this
```
```python
.executor = executor;
```
```python
this
```
```python
.dirty =
```
```python
false
```
```python
;
```
```python
this
```
```python
.autoCommit = autoCommit;
}
```
其中executor成员最为关键，DefaultSqlSession的大部分方法均是通过它来代理实现。比如select update方法。而delete和insert方法均调用update方法来实现。
```python
// select方法通过executor实现，典型的代理模式
```
```python
// selectOne selectList均调用它实现
```
```python
public
```
```python
void
```
```python
select
```
```python
(String statement, Object parameter, RowBounds rowBounds, ResultHandler handler) {
```
```python
try
```
```python
{
    MappedStatement ms = configuration.getMappedStatement(statement);
```
```python
// 利用调度器Executor代理实现
```
```python
executor.query(ms, wrapCollection(parameter), rowBounds, handler);
  }
```
```python
catch
```
```python
(Exception e) {
```
```python
throw
```
```python
ExceptionFactory.wrapException(
```
```python
"Error querying database.  Cause: "
```
```python
+ e, e);
  }
```
```python
finally
```
```python
{
    ErrorContext.instance().reset();
  }
}
```
```python
// update方法通过executor实现，典型的代理模式
```
```python
// insert update等DML操作均调用它实现
```
```python
public
```
```python
int
```
```python
update
```
```python
(String statement, Object parameter) {
```
```python
try
```
```python
{
      dirty =
```
```python
true
```
```python
;
      MappedStatement ms = configuration.getMappedStatement(statement);
```
```python
// 利用调度器Executor代理实现
```
```python
return
```
```python
executor.update(ms, wrapCollection(parameter));
    }
```
```python
catch
```
```python
(Exception e) {
```
```python
throw
```
```python
ExceptionFactory.wrapException(
```
```python
"Error updating database.  Cause: "
```
```python
+ e, e);
    }
```
```python
finally
```
```python
{
      ErrorContext.instance().reset();
    }
  }
```
```python
public
```
```python
int
```
```python
insert
```
```python
(String statement, Object parameter) {
```
```python
return
```
```python
update(statement, parameter);
}
```
```python
public
```
```python
int
```
```python
delete
```
```python
(String statement, Object parameter) {
```
```python
return
```
```python
update(statement, parameter);
}
```
## 3 总结
构建SqlSession的核心是创建调度器Executor，它是mybatis运行的核心。通过调度StatementHandler ParameterHandler ResultSetHandler来完成各种操作。四者合称SqlSession四大组件。后面一节我们以sqlSession.selectOne()方法为例来分析mybatis的CRUD操作的背后原理。
### 相关文章
[mybatis源码分析1 - 框架](http://blog.csdn.net/u013510838/article/details/79055792)
[mybatis源码分析2 - SqlSessionFactory的创建](http://blog.csdn.net/u013510838/article/details/78995542)
[mybatis源码分析3 - sqlSession的创建](http://blog.csdn.net/u013510838/article/details/79011489)
[mybatis源码分析4 - sqlSession读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79016238)
[mybatis源码分析5 - mapper读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79023822)
[mybatis源码分析6 - mybatis-spring容器初始化](http://blog.csdn.net/u013510838/article/details/79053519)
[mybatis源码分析7 - mybatis-spring读写数据库全过程](http://blog.csdn.net/u013510838/article/details/79054007)

