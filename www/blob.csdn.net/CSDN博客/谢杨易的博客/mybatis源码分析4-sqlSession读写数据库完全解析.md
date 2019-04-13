
# mybatis源码分析4 - sqlSession读写数据库完全解析 - 谢杨易的博客 - CSDN博客

2018年01月09日 18:55:46[谢杨易](https://me.csdn.net/u013510838)阅读数：668



## 1 引言和主要类
创建完sqlSession实例后，我们就可以进行数据库操作了。比如通过selectOne()方法查询数据库，如代码
```python
// 读取XML配置文件
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
// 创建sqlSessionFactory单例，初始化mybatis容器
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
// 创建sqlSession实例，用它来进行数据库操作，mybatis运行时的门面
```
```python
SqlSession session = sessionFactory.openSession();
```
```python
// 进行数据库查询操作
```
```python
User user = session.selectOne(
```
```python
"test.findUserById"
```
```python
,
```
```python
1
```
```python
);
```
```python
// 访问数据库,statement为mapper.xml中的id
```
创建sqlSessionFactory单例和sqlSession实例在前两节中分析过了，下面我们着重来分析sqlSession操作数据库的过程。
sqlSession操作数据库有两种方法，可以直接使用select update insert delete等方法；也可以通过getMapper()先获取mapper动态代理实例，然后再进行数据库操作。相比而言mapper方式更灵活且不易出错，是mybatis推荐的方式。本节我们分析直接使用select等方法的流程，下一节再分析mapper方式。
本节以selectOne()方法的实现过程为例来分析sqlSession操作数据库的流程，涉及的主要类如下。
DefaultSqlSession：SqlSession的默认实现，其方法基本都是利用Executor代理实现。
Executor：mybatis运行的核心，调度器。调度mybatis其他三大组件的执行。
StatementHandler：SQL语句执行器，cache的管理等
ParameterHandler：入参处理器，statementType为PREPARE是需要使用到它，来解析入参到preparedStatement中
ResultSetHandler：结果集映射处理器，将数据库操作原始结果（主要是查询操作），映射为Java POJO。这正是ORM要解决的关键问题。
## 2 流程
### 2.1 DefaultSqlSession的selectOne()
先从DefaultSqlSession的selectOne()方法看起。
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
(String statement, Object parameter) {
```
```python
// selectOne本质上是调用selectList实现，如果结果集大于一个，则报TooManyResultsException。
```
```python
List<T> list =
```
```python
this
```
```python
.<T>selectList(statement, parameter);
```
```python
if
```
```python
(list.size() ==
```
```python
1
```
```python
) {
```
```python
return
```
```python
list.get(
```
```python
0
```
```python
);
  }
```
```python
else
```
```python
if
```
```python
(list.size() >
```
```python
1
```
```python
) {
```
```python
throw
```
```python
new
```
```python
TooManyResultsException(
```
```python
"Expected one result (or null) to be returned by selectOne(), but found: "
```
```python
+ list.size());
  }
```
```python
else
```
```python
{
```
```python
return
```
```python
null
```
```python
;
  }
}
```
```python
public
```
```python
<E> List<E>
```
```python
selectList
```
```python
(String statement, Object parameter) {
```
```python
return
```
```python
this
```
```python
.selectList(statement, parameter, RowBounds.DEFAULT);
}
```
```python
// 由sql语句的标示statement和入参parameter，查询满足条件的数据列表
```
```python
// @Param statement: mapper.xml中mapper节点下的select delete update insert等子节点的id属性
```
```python
// @Param parameter: 传入sql语句的入参
```
```python
// @Param rowBounds: 逻辑分页，包含offset和limit两个主要成员变量。mybatis分页逻辑为舍弃offset之前条目，取剩下的limit条。默认DEFAULT不分页
```
```python
public
```
```python
<E> List<E>
```
```python
selectList
```
```python
(String statement, Object parameter, RowBounds rowBounds) {
```
```python
try
```
```python
{
```
```python
// 从mappers节点初始化阶段创建好的mappedStatements这个Map中，找到key为当前要找到的sql的id的那条
```
```python
MappedStatement ms = configuration.getMappedStatement(statement);
```
```python
// 通过执行器Executor作为总调度来执行查询语句，后面以BaseExecutor来分析。
```
```python
// BatchExecutor ReuseExecutor SimpleExecutor均继承了BaseExecutor
```
```python
return
```
```python
executor.query(ms, wrapCollection(parameter), rowBounds, Executor.NO_RESULT_HANDLER);
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
selectOne()方法其实是通过调用selectList()实现的，因为二者本质是完全相同的，只是前者返回一个对象，而后者为列表List而已。selectList()采用代理模式，使用调度器Executor的query()方法实现。上一节着重讲过Executor是SqlSession各个方法的具体实现，是mybatis运行的核心，通过调度StatementHandler ParameterHandler ResultSetHandler三个组件来完成sqlSession操作数据库的整个过程。Executor的实现类有SimpleExecutor ReuseExecutor BatchExecutor等，它们的基类都是BaseExecutor。下面来分析BaseExecutor的query
### 2.2 BaseExecutor的query() 调度器开始数据库query
```python
// BaseExecutor的查找方法
```
```python
public
```
```python
<E> List<E>
```
```python
query
```
```python
(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler)
```
```python
throws
```
```python
SQLException {
```
```python
// 从MappedStatement中找到boundSql成员变量，前面SqlSessionFactory创建部分讲到过Mapper解析时的三大组件：MappedStatement SqlSource BoundSql
```
```python
// 其中BoundSql通过sql执行语句和入参，来组装最终查询数据库用到的sql。
```
```python
BoundSql boundSql = ms.getBoundSql(parameter);
```
```python
// 创建CacheKey，用作缓存的key，不用深入理解。
```
```python
// sql的id，逻辑分页rowBounds的offset和limit，boundSql的sql语句均相同时（主要是动态sql的存在），也就是组装后的SQL语句完全相同时，才认为是同一个cacheKey
```
```python
CacheKey key = createCacheKey(ms, parameter, rowBounds, boundSql);
```
```python
// 真正的查询语句执行处，关键代码
```
```python
return
```
```python
query(ms, parameter, rowBounds, resultHandler, key, boundSql);
}
```
```python
// 查找方法
```
```python
public
```
```python
<E> List<E>
```
```python
query
```
```python
(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, CacheKey key, BoundSql boundSql)
```
```python
throws
```
```python
SQLException {
    ErrorContext.instance().resource(ms.getResource()).activity(
```
```python
"executing a query"
```
```python
).object(ms.getId());
```
```python
// 调度器已经close了则报错
```
```python
if
```
```python
(closed) {
```
```python
throw
```
```python
new
```
```python
ExecutorException(
```
```python
"Executor was closed."
```
```python
);
    }
```
```python
// flush cache, 即写入并清空cache。之后就只能从数据库中读取了，这样可以防止脏cache
```
```python
// localCache和localOutputParameterCache为BaseExecutor的成员变量，它们构成了mybatis的一级缓存，也就是sqlSession级别的缓存，默认是开启的。
```
```python
if
```
```python
(queryStack ==
```
```python
0
```
```python
&& ms.isFlushCacheRequired()) {
      clearLocalCache();
    }
```
```python
// 从缓存或数据库中查询结果list
```
```python
List<E> list;
```
```python
try
```
```python
{
```
```python
// queryStack用来记录当前有几条同样的查询语句在同时执行，也就是并发
```
```python
queryStack++;
```
```python
// 未定义resultHandler时，先尝试从缓存中取。
```
```python
list = resultHandler ==
```
```python
null
```
```python
? (List<E>) localCache.getObject(key) :
```
```python
null
```
```python
;
```
```python
if
```
```python
(list !=
```
```python
null
```
```python
) {
```
```python
// 缓存命中时，直接从本地缓存中取出即可，不做详细分析
```
```python
handleLocallyCachedOutputParameters(ms, key, parameter, boundSql);
      }
```
```python
else
```
```python
{
```
```python
// 缓存未命中，必须从数据库中查询。后面详细分析
```
```python
list = queryFromDatabase(ms, parameter, rowBounds, resultHandler, key, boundSql);
      }
    }
```
```python
finally
```
```python
{
      queryStack--;
    }
```
```python
// 当前所有查询语句都结束时，开始处理延迟加载。从缓存中取出执行结果，因为前面已经有过本查询语句了。
```
```python
if
```
```python
(queryStack ==
```
```python
0
```
```python
) {
```
```python
for
```
```python
(DeferredLoad deferredLoad : deferredLoads) {
```
```python
// 延迟加载从缓存中获取结果
```
```python
deferredLoad.load();
      }
```
```python
// issue #601
```
```python
deferredLoads.clear();
```
```python
if
```
```python
(configuration.getLocalCacheScope() == LocalCacheScope.STATEMENT) {
```
```python
// statement级别的缓存，只缓存id相同的sql。当所有查询语句和延迟加载的查询语句均执行完毕后，可清空cache。这样可节约内存
```
```python
clearLocalCache();
      }
    }
```
```python
return
```
```python
list;
  }
```
调度器的query方法先从MappedStatement中获取BoundSql，它包含了sql语句和入参对象等变量，再构造缓存的key，即cacheKey。然后先尝试从缓存中取，缓存未命中则直接从数据库中查询。最后处理延迟加载，直接从缓存中取出查询数据即可。下面我们着重分析直接从数据库中查询的过程，也即queryFromDatabase()方法。
```python
// 直接从数据库中查询
```
```python
private
```
```python
<E> List<E>
```
```python
queryFromDatabase
```
```python
(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, CacheKey key, BoundSql boundSql)
```
```python
throws
```
```python
SQLException {
  List<E> list;
```
```python
// 先利用占位符将本次查询设置到本地cache中，个人理解是防止后面延迟加载时cache为空
```
```python
localCache.putObject(key, EXECUTION_PLACEHOLDER);
```
```python
try
```
```python
{
```
```python
// 真正的数据库查询，后面详细分析
```
```python
list = doQuery(ms, parameter, rowBounds, resultHandler, boundSql);
  }
```
```python
finally
```
```python
{
```
```python
// 查到了结果后，将前面的占位符的cache删掉
```
```python
localCache.removeObject(key);
  }
```
```python
// 将查询结果放到本地cache中缓存起来
```
```python
localCache.putObject(key, list);
```
```python
if
```
```python
(ms.getStatementType() == StatementType.CALLABLE) {
    localOutputParameterCache.putObject(key, parameter);
  }
```
```python
return
```
```python
list;
}
```
doQuery()进行真正的数据库查询，它由SimpleExecutor等具体类来实现。我们以SimpleExecutor为例分析。
```python
// 数据库查询
```
```python
public
```
```python
<E> List<E>
```
```python
doQuery
```
```python
(MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql)
```
```python
throws
```
```python
SQLException {
  Statement stmt =
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
    Configuration configuration = ms.getConfiguration();
```
```python
// 创建StatementHandler，用来执行sql语句。SimpleExecutor创建的是RoutingStatementHandler。
```
```python
// 它的是一个门面类，几乎所有方法都是通过代理来实现。代理则由配置XML settings节点的statementType区分。故仅仅是一个分发和路由。后面详细分析
```
```python
StatementHandler handler = configuration.newStatementHandler(wrapper, ms, parameter, rowBounds, resultHandler, boundSql);
```
```python
// 构造Statement，后面详细分析
```
```python
stmt = prepareStatement(handler, ms.getStatementLog());
```
```python
// 通过语句执行器的query方法进行查询, 查询结果通过resultHandler处理后返回。后面详细分析
```
```python
return
```
```python
handler.<E>query(stmt, resultHandler);
  }
```
```python
finally
```
```python
{
    closeStatement(stmt);
  }
}
```
doQuery流程为
先创建StatementHandler语句处理器。前面讲过StatementHandler是mybatis四大组件之一，负责sql语句的执行。根据XML配置文件的settings节点的statementType子元素，来创建不同的实现类，如SimpleStatementHandler，PreparedStatementHandler，CallableStatementHandler。他们的基类统一为BaseStatementHandler，外观类为RoutingStatementHandler（后面详细分析）。
创建完StatementHandler后，调用prepareStatement进行初始化，
然后调用实现类的query方法进行查询。
### 2.3 StatementHandler的query()， 语句处理器进行查询
下面我们来看StatementHandler是如何执行的，先看StatementHandler的创建过程。
#### 2.3.1 StatementHandler的创建过程
```python
// 创建RoutingStatementHandler，它是StatementHandler的外观类，也是StatementHandler的一个实现类
```
```python
public
```
```python
StatementHandler
```
```python
newStatementHandler
```
```python
(Executor executor, MappedStatement mappedStatement, Object parameterObject, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) {
```
```python
// 直接构造一个RoutingStatementHandler
```
```python
StatementHandler statementHandler =
```
```python
new
```
```python
RoutingStatementHandler(executor, mappedStatement, parameterObject, rowBounds, resultHandler, boundSql);
```
```python
// 将statementHandler，添加为插件的目标执行器。插件通过配置XML文件的plugins节点设置。
```
```python
statementHandler = (StatementHandler) interceptorChain.pluginAll(statementHandler);
```
```python
return
```
```python
statementHandler;
}
```
```python
// RoutingStatementHandler的构造器，根据statementType变量来创建不同的StatementHandler实现，作为它的代理
```
```python
// RoutingStatementHandler的几乎所有方法都是通过这些代理实现的，典型的代理模式。
```
```python
public
```
```python
RoutingStatementHandler
```
```python
(Executor executor, MappedStatement ms, Object parameter, RowBounds rowBounds, ResultHandler resultHandler, BoundSql boundSql) {
```
```python
// 根据statementType创建不同的StatementHandler实现类。statementType是在xml配置文件的settngs节点的statementType子元素中设置的。
```
```python
switch
```
```python
(ms.getStatementType()) {
```
```python
case
```
```python
STATEMENT:
```
```python
// 直接操作sql，不进行预编译。此时直接进行字符串拼接构造sql String
```
```python
delegate =
```
```python
new
```
```python
SimpleStatementHandler(executor, ms, parameter, rowBounds, resultHandler, boundSql);
```
```python
break
```
```python
;
```
```python
case
```
```python
PREPARED:
```
```python
// 默认的类型。预处理，需要进行预编译。可以使用参数替换，会将#转换为？，再设置对应的参数的值。
```
```python
delegate =
```
```python
new
```
```python
PreparedStatementHandler(executor, ms, parameter, rowBounds, resultHandler, boundSql);
```
```python
break
```
```python
;
```
```python
case
```
```python
CALLABLE:
```
```python
// 执行存储过程
```
```python
delegate =
```
```python
new
```
```python
CallableStatementHandler(executor, ms, parameter, rowBounds, resultHandler, boundSql);
```
```python
break
```
```python
;
```
```python
default
```
```python
:
```
```python
throw
```
```python
new
```
```python
ExecutorException(
```
```python
"Unknown statement type: "
```
```python
+ ms.getStatementType());
    }
  }
```
#### 2.3.2 StatementHandler的初始化
StatementHandler的初始化如下
```python
// 通过事务构造sql执行语句statement，如JdbcTransaction
```
```python
private
```
```python
Statement
```
```python
prepareStatement
```
```python
(StatementHandler handler, Log statementLog)
```
```python
throws
```
```python
SQLException {
  Statement stmt;
```
```python
// 开启数据库连接，创建Connection对象。JdbcTransaction事务直接通过JDBC创建connection
```
```python
Connection connection = getConnection(statementLog);
```
```python
// 初始化statement并设置期相关变量，不同的StatementHandler实现不同。后面以RoutingStatementHandler为例分析
```
```python
stmt = handler.prepare(connection, transaction.getTimeout());
```
```python
// 设置parameterHandler，对于SimpleStatementHandler来说不用处理
```
```python
handler.parameterize(stmt);
```
```python
return
```
```python
stmt;
}
```
StatementHandler初始化步骤如下：
先开启一个数据库连接connection，
然后初始化statementHandler，
最后进行参数预处理。
先开启数据库连接connection，直接获取数据源dataSource的connection，即通过数据库本身来开启连接。
```python
// JdbcTransaction和ManagedTransaction都是直接调用dataSource的getConnection
```
```python
protected
```
```python
Connection
```
```python
getConnection
```
```python
(Log statementLog)
```
```python
throws
```
```python
SQLException {
  Connection connection = transaction.getConnection();
```
```python
if
```
```python
(statementLog.isDebugEnabled()) {
```
```python
return
```
```python
ConnectionLogger.newInstance(connection, statementLog, queryStack);
  }
```
```python
else
```
```python
{
```
```python
return
```
```python
connection;
  }
}
```
再进行初始化statementHandler，调用基类BaseStatementHandler的prepare方法完成
```python
// 初始化statementHandler
```
```python
public
```
```python
Statement
```
```python
prepare
```
```python
(Connection connection, Integer transactionTimeout)
```
```python
throws
```
```python
SQLException {
```
```python
// 典型的代理模式，不同的statementType创建不同的Statement，但这儿他们都调用到他们的基类BaseStatementHandler中的prepare方法
```
```python
return
```
```python
delegate.prepare(connection, transactionTimeout);
}
```
```python
// BaseStatementHandler初始化statement并设置期相关变量，不同的StatementHandler实现不同。
```
```python
public
```
```python
Statement
```
```python
prepare
```
```python
(Connection connection, Integer transactionTimeout)
```
```python
throws
```
```python
SQLException {
    ErrorContext.instance().sql(boundSql.getSql());
    Statement statement =
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
// 初始化statement，由具体的StatementHandler来实现。比如SimpleStatementHandler通过JDBC connection的createStatement来创建
```
```python
statement = instantiateStatement(connection);
```
```python
// 设置timeout（超时时间）和fetchSize（获取数据库的行数）
```
```python
setStatementTimeout(statement, transactionTimeout);
      setFetchSize(statement);
```
```python
return
```
```python
statement;
    }
```
```python
catch
```
```python
(SQLException e) {
      closeStatement(statement);
```
```python
throw
```
```python
e;
    }
```
```python
catch
```
```python
(Exception e) {
      closeStatement(statement);
```
```python
throw
```
```python
new
```
```python
ExecutorException(
```
```python
"Error preparing statement.  Cause: "
```
```python
+ e, e);
    }
}
```
最后进行参数预处理。不同的statementHandler实现类有不同的参数预处理方式。
SimpleStatementHandler不进行任何参数预处理，它的sql直接通过字符串拼接而成。
PreparedStatementHandler进行预处理，会将\#转换为？，然后设置对应的变量到sql String中。
```python
// RoutingStatementHandler的parameterize方法，通过代理模式实现
```
```python
public
```
```python
void
```
```python
parameterize
```
```python
(Statement statement)
```
```python
throws
```
```python
SQLException {
```
```python
// 又是代理模式,由具体的statementHandler实现类来实现
```
```python
delegate.parameterize(statement);
}
```
```python
// SimpleStatementHandler不做参数预处理
```
```python
public
```
```python
void
```
```python
parameterize
```
```python
(Statement statement)
```
```python
throws
```
```python
SQLException {
```
```python
// N/A
```
```python
}
```
```python
// PreparedStatementHandler进行参数预处理，通过parameterHandler实现
```
```python
public
```
```python
void
```
```python
parameterize
```
```python
(Statement statement)
```
```python
throws
```
```python
SQLException {
```
```python
// parameterHandler可以由用户通过插件方式实现，mybatis默认为DefaultParameterHandler。这个方法我们不进行详细分析了。
```
```python
parameterHandler.setParameters((PreparedStatement) statement);
}
```
#### 2.3.3 statementHandler的query()进行数据库查询
创建和初始化statementHandler后，就可以调用它的query()方法来执行语句查询了。先看SimpleStatementHandler的query过程。
```python
// SimpleStatementHandler的query操作过程
```
```python
public
```
```python
<E> List<E>
```
```python
query
```
```python
(Statement statement, ResultHandler resultHandler)
```
```python
throws
```
```python
SQLException {
```
```python
// 获取存放在boundSql中的sql执行语句
```
```python
String sql = boundSql.getSql();
```
```python
// 通过JDBC sql的statement，直接执行sql语句。入参在statement预编译时进行了转换并设置到statement中了。
```
```python
statement.execute(sql);
```
```python
// resultSetHandler处理查询结果，并返回。这一部分十分复杂，但也体现了mybatis的设计精巧之处，可以兼容很多复杂场景下的数据库结果转换。如数据库列名和Java POJO属性名不同时的映射，关联数据库的映射等。
```
```python
return
```
```python
resultSetHandler.<E>handleResultSets(statement);
}
```
PrepareStatementHandler的query操作过程如下
```python
// PrepareStatementHandler的query操作过程
```
```python
public
```
```python
<E> List<E>
```
```python
query
```
```python
(Statement statement, ResultHandler resultHandler)
```
```python
throws
```
```python
SQLException {
```
```python
// PREPARE方式下，sql statement进行了预编译，并注入了入参。它是一个PreparedStatement类型
```
```python
PreparedStatement ps = (PreparedStatement) statement;
```
```python
// 直接调用JDBC PreparedStatement的execute方法操作数据库。大家应该对这儿很熟悉了，JDBC的操作
```
```python
ps.execute();
```
```python
// 结果集处理，后面详细分析
```
```python
return
```
```python
resultSetHandler.<E> handleResultSets(ps);
}
```
query先从boundSql中获取具体执行语句，然后通过JDBC的statement直接执行SQL语句。这两步完成后，就从数据库中查找到了结果集了。从这儿可见，mybatis最底层还是通过JDBC来操作数据库的。
mybatis对结果集的处理十分复杂，我们下面详细分析。
### 2.4 ResultSetHandler处理数据库结果集
通过JDBC完成数据库的操作后，我们就拿到了原始的数据库结果了。此时要将数据库结果集ResultSet转换为Java POJO。这一步通过mybatis四大组件之一的ResultSetHandler来实现。ResultSetHandler默认实现类为DefaultResultSetHandler，用户也可以通过插件的方式覆盖它。插件在xml配置文件的plugins子节点下添加。下面详细分析DefaultResultSetHandler的handleResultSets()方法。
```python
// DefaultResultSetHandler通过handleResultSets处理数据库结果集，处理后作为真正的结果返回。此处的关键是处理resultMap映射
```
```python
public
```
```python
List<Object>
```
```python
handleResultSets
```
```python
(Statement stmt)
```
```python
throws
```
```python
SQLException {
    ErrorContext.instance().activity(
```
```python
"handling results"
```
```python
).object(mappedStatement.getId());
```
```python
final
```
```python
List<Object> multipleResults =
```
```python
new
```
```python
ArrayList<Object>();
```
```python
int
```
```python
resultSetCount =
```
```python
0
```
```python
;
```
```python
// 1 从JDBC操作数据库后的statement中取出结果集ResultSet
```
```python
ResultSetWrapper rsw = getFirstResultSet(stmt);
```
```python
// 2 获取resultMaps， mapper.xml中设置，并在mybatis初始化阶段存入mappedStatement中。
```
```python
// resultMap定义了jdbc列到Java属性的映射关系，可以解决列名和Java属性名不一致，关联数据库映射等诸多问题。
```
```python
// 它是mybatis中比较复杂的地方，同时也大大扩展了mybatis的功能
```
```python
List<ResultMap> resultMaps = mappedStatement.getResultMaps();
```
```python
int
```
```python
resultMapCount = resultMaps.size();
    validateResultMapsCount(rsw, resultMapCount);
```
```python
// 3 一条条处理resultSet
```
```python
while
```
```python
(rsw !=
```
```python
null
```
```python
&& resultMapCount > resultSetCount) {
```
```python
// 取出一条resultMap，即结果映射
```
```python
ResultMap resultMap = resultMaps.get(resultSetCount);
```
```python
// 进行数据库列到Java属性的映射,后面详细分析
```
```python
handleResultSet(rsw, resultMap, multipleResults,
```
```python
null
```
```python
);
```
```python
// 取出下一条resultSet
```
```python
rsw = getNextResultSet(stmt);
```
```python
// 清空nestedResultObjects，即嵌套的Result结果集
```
```python
cleanUpAfterHandlingResultSet();
      resultSetCount++;
    }
```
```python
// 4 处理嵌套的resultMap，即映射结果中的某些子属性也需要resultMap映射时
```
```python
String[] resultSets = mappedStatement.getResultSets();
```
```python
if
```
```python
(resultSets !=
```
```python
null
```
```python
) {
```
```python
while
```
```python
(rsw !=
```
```python
null
```
```python
&& resultSetCount < resultSets.length) {
```
```python
// 取出父ResultMapping，用于嵌套情况
```
```python
ResultMapping parentMapping = nextResultMaps.get(resultSets[resultSetCount]);
```
```python
if
```
```python
(parentMapping !=
```
```python
null
```
```python
) {
```
```python
// 通过嵌套ResultMap的id，取出ResultMap
```
```python
String nestedResultMapId = parentMapping.getNestedResultMapId();
          ResultMap resultMap = configuration.getResultMap(nestedResultMapId);
```
```python
// 处理ResultSet，后面详细分析
```
```python
handleResultSet(rsw, resultMap,
```
```python
null
```
```python
, parentMapping);
        }
        rsw = getNextResultSet(stmt);
        cleanUpAfterHandlingResultSet();
        resultSetCount++;
      }
    }
```
```python
// 5 构造成List，将处理后的结果集返回
```
```python
return
```
```python
collapseSingleResultList(multipleResults);
}
```
handleResultSets流程如下
从JDBC操作数据库后的statement中取出结果集ResultSet
获取resultMaps, 它们定义了数据库结果集到Java POJO的映射关系
一条条处理resultSet，调用handleResultSet做数据库列到Java属性的映射
处理嵌套的resultMap，即映射结果中的某些子属性也需要resultMap映射时
构造成List，将处理后的结果集返回
这其中的关键是handleResultSet()方法进行数据库列到Java属性的映射，也是ORM关键所在。我们接着分析。
```python
// 通过resultMap映射，处理数据库结果集resultSet
```
```python
private
```
```python
void
```
```python
handleResultSet
```
```python
(ResultSetWrapper rsw, ResultMap resultMap, List<Object> multipleResults, ResultMapping parentMapping)
```
```python
throws
```
```python
SQLException {
```
```python
try
```
```python
{
```
```python
if
```
```python
(parentMapping !=
```
```python
null
```
```python
) {
```
```python
// parentMapping不为空，表示处理的是嵌套resultMap中的子resultMap。handleRowValues后面详细分析
```
```python
handleRowValues(rsw, resultMap,
```
```python
null
```
```python
, RowBounds.DEFAULT, parentMapping);
    }
```
```python
else
```
```python
{
```
```python
// 非嵌套resultMap
```
```python
if
```
```python
(resultHandler ==
```
```python
null
```
```python
) {
```
```python
// 用户没有自定义resultHandler时，采用DefaultResultHandler。并将最终的处理结果添加到multipleResults中
```
```python
DefaultResultHandler defaultResultHandler =
```
```python
new
```
```python
DefaultResultHandler(objectFactory);
        handleRowValues(rsw, resultMap, defaultResultHandler, rowBounds,
```
```python
null
```
```python
);
        multipleResults.add(defaultResultHandler.getResultList());
      }
```
```python
else
```
```python
{
```
```python
// 用户定义了resultHandler时，采用用户自定义的resultHandler
```
```python
handleRowValues(rsw, resultMap, resultHandler, rowBounds,
```
```python
null
```
```python
);
      }
    }
  }
```
```python
finally
```
```python
{
```
```python
// issue #228 (close resultsets)
```
```python
closeResultSet(rsw.getResultSet());
  }
}
```
handleResultSet针对嵌套resultMap和非嵌套resultMap做了分别处理，但都是调用的handleRowValues()方法，接着看。
```python
public
```
```python
void
```
```python
handleRowValues
```
```python
(ResultSetWrapper rsw, ResultMap resultMap, ResultHandler<?> resultHandler, RowBounds rowBounds, ResultMapping parentMapping)
```
```python
throws
```
```python
SQLException {
```
```python
if
```
```python
(resultMap.hasNestedResultMaps()) {
```
```python
// 有嵌套resultMap时
```
```python
ensureNoRowBounds();
    checkResultHandler();
    handleRowValuesForNestedResultMap(rsw, resultMap, resultHandler, rowBounds, parentMapping);
  }
```
```python
else
```
```python
{
```
```python
// 无嵌套resultMap时
```
```python
handleRowValuesForSimpleResultMap(rsw, resultMap, resultHandler, rowBounds, parentMapping);
  }
}
```
嵌套resultMap的处理比较麻烦，这儿不分析了，我们看非嵌套的，即handleRowValuesForSimpleResultMap。
```python
// 非嵌套ResultMap的处理方法。根据resultMap一行行处理数据库结果集到Java属性的映射
```
```python
private
```
```python
void
```
```python
handleRowValuesForSimpleResultMap
```
```python
(ResultSetWrapper rsw, ResultMap resultMap, ResultHandler<?> resultHandler, RowBounds rowBounds, ResultMapping parentMapping)
```
```python
throws
```
```python
SQLException {
  DefaultResultContext<Object> resultContext =
```
```python
new
```
```python
DefaultResultContext<Object>();
```
```python
// mybatis的逻辑分页规则为跳过rowBounds中offset之前的部分，取limit行数的数据
```
```python
// skipRows方法会跳过rowBounds中offset之前的部分。
```
```python
skipRows(rsw.getResultSet(), rowBounds);
```
```python
// 一行行处理数据库结果集，直到取出的行数等于rowBounds的limit变量（逻辑分页），或者所有行都取完了。
```
```python
while
```
```python
(shouldProcessMoreRows(resultContext, rowBounds) && rsw.getResultSet().next()) {
```
```python
// 处理resultMap中的discriminator，使用结果值来决定使用哪个结果映射。可以将不同的数据库结果映射成不同的Java类型。此处不详细分析了
```
```python
ResultMap discriminatedResultMap = resolveDiscriminatedResultMap(rsw.getResultSet(), resultMap,
```
```python
null
```
```python
);
```
```python
// 处理这一行数据, 得到映射后的Java结果
```
```python
Object rowValue = getRowValue(rsw, discriminatedResultMap);
```
```python
// 使用resultHandler处理得到的Java结果，这才是最终返回的Java属性值。
```
```python
// 用户可自定义resultHandler，否则使用DefaultResultHandler。
```
```python
// 用户可使用ResultSetHandler插件来自定义结果处理方式，此处体现了mybatis设计精巧之处
```
```python
storeObject(resultHandler, resultContext, rowValue, parentMapping, rsw.getResultSet());
  }
}
```
```python
// 逻辑分页rowBounds。skipRows方法会跳过rowBounds中offset之前的部分
```
```python
private
```
```python
void
```
```python
skipRows
```
```python
(ResultSet rs, RowBounds rowBounds)
```
```python
throws
```
```python
SQLException {
```
```python
if
```
```python
(rs.getType() != ResultSet.TYPE_FORWARD_ONLY) {
```
```python
if
```
```python
(rowBounds.getOffset() != RowBounds.NO_ROW_OFFSET) {
        rs.absolute(rowBounds.getOffset());
      }
    }
```
```python
else
```
```python
{
```
```python
for
```
```python
(
```
```python
int
```
```python
i =
```
```python
0
```
```python
; i < rowBounds.getOffset(); i++) {
        rs.next();
      }
    }
 }
```
```python
// 逻辑分页rowBounds。shouldProcessMoreRows取limit条数据库查询结果
```
```python
private
```
```python
boolean
```
```python
shouldProcessMoreRows
```
```python
(ResultContext<?> context, RowBounds rowBounds)
```
```python
throws
```
```python
SQLException {
```
```python
return
```
```python
!context.isStopped() && context.getResultCount() < rowBounds.getLimit();
}
```
mybatis的逻辑分页规则为跳过rowBounds中offset之前的部分，取limit行数的数据。通过skipRows()和shouldProcessMoreRows()两个方法共同完成这个功能。遍历resultSet，通过getRowValue()方法处理一行行数据。最后调用resultHandler来处理转换后的结果。
storeObject结果处理的代码如下
```python
// 利用resultHandler处理经过resultMap映射的Java结果
```
```python
private
```
```python
void
```
```python
storeObject
```
```python
(ResultHandler<?> resultHandler, DefaultResultContext<Object> resultContext, Object rowValue, ResultMapping parentMapping, ResultSet rs)
```
```python
throws
```
```python
SQLException {
```
```python
if
```
```python
(parentMapping !=
```
```python
null
```
```python
) {
```
```python
// 嵌套的resultMap，也就是子resultSet结果。链接到父resultSet中，由父resultMap一起处理。不详细分析了
```
```python
linkToParents(rs, parentMapping, rowValue);
    }
```
```python
else
```
```python
{
```
```python
// 不是嵌套时，直接调用resultHandler进行最后的处理，后面详细看
```
```python
callResultHandler(resultHandler, resultContext, rowValue);
    }
}
```
```python
private
```
```python
void
```
```python
callResultHandler
```
```python
(ResultHandler<?> resultHandler, DefaultResultContext<Object> resultContext, Object rowValue) {
```
```python
// 构建resultContext上下文，然后利用resultHandler处理。后面以DefaultResultHandler来分析
```
```python
resultContext.nextResultObject(rowValue);
  ((ResultHandler<Object>) resultHandler).handleResult(resultContext);
}
```
```python
// ResultHandler对映射后的结果做最后的处理
```
```python
public
```
```python
void
```
```python
handleResult
```
```python
(ResultContext<? extends Object> context) {
```
```python
// DefaultResultHandler对经过resultMap映射后的Java结果不做任何处理，仅仅添加到list中，最后将list返回给selectList()等方法。
```
```python
list.add(context.getResultObject());
}
```
## 3 总结
mybatis操作数据库的流程，也是它的四大组件Executor StatementHandler ParameterHandler ResultSetHandler的执行过程。其中Executor是调度器，StatementHandler为SQL语句执行器，ParameterHandler为入参执行器，ResultSetHandler为结果集映射执行器。四大组件分层合理，运行流程清晰，都有默认实现，同时用户也可以利用plugin来覆盖它。这些无一不体现了mybatis的灵活和设计精巧，值得我们平时设计构架时学习。
### 相关文章
[mybatis源码分析1 - 框架](http://blog.csdn.net/u013510838/article/details/79055792)
[mybatis源码分析2 - SqlSessionFactory的创建](http://blog.csdn.net/u013510838/article/details/78995542)
[mybatis源码分析3 - sqlSession的创建](http://blog.csdn.net/u013510838/article/details/79011489)
[mybatis源码分析4 - sqlSession读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79016238)
[mybatis源码分析5 - mapper读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79023822)
[mybatis源码分析6 - mybatis-spring容器初始化](http://blog.csdn.net/u013510838/article/details/79053519)
[mybatis源码分析7 - mybatis-spring读写数据库全过程](http://blog.csdn.net/u013510838/article/details/79054007)

