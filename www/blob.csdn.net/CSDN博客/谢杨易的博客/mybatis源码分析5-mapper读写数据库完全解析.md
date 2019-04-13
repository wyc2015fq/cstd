
# mybatis源码分析5 - mapper读写数据库完全解析 - 谢杨易的博客 - CSDN博客

2018年01月10日 14:56:19[谢杨易](https://me.csdn.net/u013510838)阅读数：870



## 1 引言和主要类
上一节讲解了sqlSession读写数据库的整个流程和四大组件的执行过程，相信大家对mybatis操作数据库有了一定的了解。上一节还提到过，其实我们还可以通过mapper方式读写数据库，并且mybatis建议使用mapper方式，而不是直接通过sqlSession的selectList update等方法。使用mapper方式的例子如下
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
// 获取mapper接口动态代理对象
```
```python
UserMapper mapper = session.getMapper(UserMapper.class);
```
```python
// 利用动态代理调用mapper的相关方法
```
```python
User user = mapper.findUserById(
```
```python
1
```
```python
);
```
mapper使用起来十分方便，它的优点如下
只用定义接口，无需实现类。也不需要继承任何类，十分灵活
通过接口的方法进行调用，可以有效避免String拼写错误
返回值不用局限于sqlSession有限的那几个方法，实现了完全的自定义。
下面我们来分析mapper方式是如何读写数据库的。
## 2 流程
mapper操作数据库分为两个步骤，先getMapper()获取动态代理对象，然后利用mapper对象操作数据库。我们一步步分析。
### 2.1 getMapper()获取动态代理对象流程
先从DefaultSqlSession的getMapper()分析。
```python
// mapper动态代理方式执行SQL命令的入口
```
```python
public
```
```python
<T> T
```
```python
getMapper
```
```python
(Class<T> type) {
```
```python
return
```
```python
configuration.<T>getMapper(type,
```
```python
this
```
```python
);
}
```
这儿是总入口，没啥好说的，再看Configuration类的getMapper()
```python
// mapper方式执行SQL命令，先获取mapper对象
```
```python
public
```
```python
<T> T
```
```python
getMapper
```
```python
(Class<T> type, SqlSession sqlSession) {
```
```python
return
```
```python
mapperRegistry.getMapper(type, sqlSession);
}
```
这也没啥好说的，看MapperRegistry类
```python
// 动态代理方式创建mapper对象
```
```python
public
```
```python
<T> T
```
```python
getMapper
```
```python
(Class<T> type, SqlSession sqlSession) {
```
```python
// 先从mybatis初始化，也就是创建SqlSessionFactory时，创建的MapperProxyFactory的map中取出当前mapper接口对应的MapperProxyFactory
```
```python
// MapperProxyFactory为mapper动态代理的工厂类
```
```python
final
```
```python
MapperProxyFactory<T> mapperProxyFactory = (MapperProxyFactory<T>) knownMappers.get(type);
```
```python
if
```
```python
(mapperProxyFactory ==
```
```python
null
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
BindingException(
```
```python
"Type "
```
```python
+ type +
```
```python
" is not known to the MapperRegistry."
```
```python
);
  }
```
```python
try
```
```python
{
```
```python
// 利用工厂类创建mapper动态代理对象
```
```python
return
```
```python
mapperProxyFactory.newInstance(sqlSession);
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
new
```
```python
BindingException(
```
```python
"Error getting mapper instance. Cause: "
```
```python
+ e, e);
  }
}
```
先从mybatis初始化时创建的MapperProxyFactory的Map中，取出当前mapper接口类的MapperProxyFactory。利用这个工厂类构造MapperProxy。下面详细看这个过程
```python
// 创建mapper动态代理对象
```
```python
public
```
```python
T
```
```python
newInstance
```
```python
(SqlSession sqlSession) {
```
```python
// 构造MapperProxy对象，然后创建我们定义的mapper接口对应的对象
```
```python
final
```
```python
MapperProxy<T> mapperProxy =
```
```python
new
```
```python
MapperProxy<T>(sqlSession, mapperInterface, methodCache);
```
```python
return
```
```python
newInstance(mapperProxy);
}
```
```python
// 动态代理的方式，反射生成mapper接口对象
```
```python
protected
```
```python
T
```
```python
newInstance
```
```python
(MapperProxy<T> mapperProxy) {
```
```python
return
```
```python
(T) Proxy.newProxyInstance(mapperInterface.getClassLoader(),
```
```python
new
```
```python
Class[] { mapperInterface }, mapperProxy);
}
```
先创建MapperProxy类，然后创建Java动态代理对象，将mapperProxy作为它的InvocationHandler。执行动态代理方法时，会回调InvocationHandler的invoke()方法。对Java动态代理不清楚的同学，建议复习一下这个内容。
Java的动态代理类Proxy的newProxyInstance()生成mapper接口的动态代理对象后，getMapper()方法就执行完毕了。接下来就是动态代理执行方法调用的过程了。下面详细分析。
### 2.2 mapper 接口方法调用流程
mapper接口方法调用，是通过动态代理的方式执行的。Proxy动态代理执行方法调用时，调用到invocationHandler的invoke方法。我们传入的invocationHandler是创建的MapperProxy对象，故方法调用的入口为MapperProxy的invoke()方法。下面详细分析。
```python
// 调用mapper接口中方法时的入口，Proxy会回调InvocationHandler的invoke方法
```
```python
// @Param proxy: Java反射的动态代理对象，getMapper()中通过Java反射 Proxy.newProxyInstance()生成的动态代理
```
```python
// @Param method: 要调用的接口方法
```
```python
// @Param args: 方法入参
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
try
```
```python
{
```
```python
// mapper接口类中的抽象方法不会执行这儿的if和else
```
```python
if
```
```python
(Object.class.equals(method.getDeclaringClass())) {
```
```python
// mapper是一个接口，而非实现类，不会走到这儿
```
```python
return
```
```python
method.invoke(
```
```python
this
```
```python
, args);
      }
```
```python
else
```
```python
if
```
```python
(isDefaultMethod(method)) {
```
```python
// mapper中方法为抽象方法，也不会走到这儿
```
```python
return
```
```python
invokeDefaultMethod(proxy, method, args);
      }
    }
```
```python
catch
```
```python
(Throwable t) {
```
```python
throw
```
```python
ExceptionUtil.unwrapThrowable(t);
    }
```
```python
// 生成mapperMethod，先从cache中取，没有则创建一个MapperMethod。
```
```python
final
```
```python
MapperMethod mapperMethod = cachedMapperMethod(method);
```
```python
// 执行execute，通过mapperMethod的method方法名，从XML中找到匹配的SQL语句，最终利用sqlSession执行数据库操作
```
```python
return
```
```python
mapperMethod.execute(sqlSession, args);
  }
```
先创建MapperMethod对象，然后利用它的execute来执行。我们分两小节分别分析。
#### 2.2.1 MapperMethod的创建
先看MapperMethod的创建过程cachedMapperMethod方法。
```python
// 生成mapperMethod，先从cache中取，没有则创建一个MapperMethod
```
```python
private
```
```python
MapperMethod
```
```python
cachedMapperMethod
```
```python
(Method method) {
```
```python
// 先从cache中取
```
```python
MapperMethod mapperMethod = methodCache.get(method);
```
```python
// 未命中，则创建一个MapperMethod
```
```python
if
```
```python
(mapperMethod ==
```
```python
null
```
```python
) {
```
```python
// 创建MapperMethod用来执行mapper接口的方法。
```
```python
// mapperInterface: 用户定义的mapper接口类
```
```python
// method：要执行的mapper接口中的方法
```
```python
// Configuration: XML配置信息
```
```python
mapperMethod =
```
```python
new
```
```python
MapperMethod(mapperInterface, method, sqlSession.getConfiguration());
    methodCache.put(method, mapperMethod);
  }
```
```python
return
```
```python
mapperMethod;
}
```
MapperMethod对象是mapper方法调用的核心对象，它先尝试从cache中取，没有则创建。下面看MapperMethod的构造方法。
```python
// MapperMethod的构造方法，创建SqlCommand和MethodSignature两个主要成员变量。二者也是动态代理的关键
```
```python
// @Param mapperInterface: 用户定义的mapper接口类
```
```python
// @Param method：要执行的mapper接口中的方法
```
```python
// @Param Configuration: XML配置信息
```
```python
public
```
```python
MapperMethod(
```
```python
Class
```
```python
<?> mapperInterface,
```
```python
Method
```
```python
method
```
```python
,
```
```python
Configuration
```
```python
config
```
```python
)
```
```python
{
  // 创建SqlCommand，包含SQL语句的id和type两个关键字段，type为insert update delete等类型。
  this.command = new SqlCommand(config, mapperInterface, method);
  // 创建MethodSignature，包含方法的返回值returnType等关键字段
  this.method = new MethodSignature(config, mapperInterface, method);
}
```
MapperMethod包含两个主要成员变量，SqlCommand和MethodSignature。SqlCommand是mapper.xml中SQL语句的描述，包含了SQL语句的id和type。MethodSignature是mapper接口中方法的描述，包含了方法返回值等关键信息。有了它们，mapper.xml配置文件和mapper接口方法基本就描述清楚了。后面会使用到这两个变量来执行方法调用。下面我们分别看这两个关键对象是如何创建的。
先看SqlCommand的构造过程。
```python
public
```
```python
static
```
```python
class
```
```python
SqlCommand
```
```python
{
```
```python
private
```
```python
final
```
```python
String name;
```
```python
// sql语句的id，mapper接口类名.方法名，拼接而成
```
```python
private
```
```python
final
```
```python
SqlCommandType type;
```
```python
// sql语句的标签，insert select update delete等
```
```python
// SqlCommand构造方法，描述了mapper.xml配置信息中一条SQL语句
```
```python
public
```
```python
SqlCommand
```
```python
(Configuration configuration, Class<?> mapperInterface, Method method) {
```
```python
// 先获取mapper接口方法的方法名，和方法声明所在的类，也就是mapper接口类
```
```python
final
```
```python
String methodName = method.getName();
```
```python
final
```
```python
Class<?> declaringClass = method.getDeclaringClass();
```
```python
// 拼接mapperInterface和methodName，构成id，从mybatis初始化阶段生成的mappedStatements这个map中获取当前方法对应的mappedStatement
```
```python
// mybatis初始化一节讲过mappedStatement，它是通过解析mapper.xml生成的，描述了其中的一条CRUD语句。
```
```python
// mappedStatement是mapper.xml中SQL操作语句的核心，包含sqlSource和BoundSql等重要对象
```
```python
MappedStatement ms = resolveMappedStatement(mapperInterface, methodName, declaringClass,
        configuration);
```
```python
if
```
```python
(ms ==
```
```python
null
```
```python
) {
```
```python
// 找不到此sql语句时
```
```python
if
```
```python
(method.getAnnotation(Flush.class) !=
```
```python
null
```
```python
) {
        name =
```
```python
null
```
```python
;
        type = SqlCommandType.FLUSH;
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
BindingException(
```
```python
"Invalid bound statement (not found): "
```
```python
+ mapperInterface.getName() +
```
```python
"."
```
```python
+ methodName);
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
// 找到此SQL语句对应的MappedStatement时。
```
```python
// name为SQL语句的id，mapper接口类名.方法名，拼接而成。也就是上面分析的statementId。
```
```python
name = ms.getId();
```
```python
// sqlCommandType为mapper.xml中SQL语句的标签，如select insert delete update等
```
```python
type = ms.getSqlCommandType();
```
```python
if
```
```python
(type == SqlCommandType.UNKNOWN) {
```
```python
throw
```
```python
new
```
```python
BindingException(
```
```python
"Unknown execution method for: "
```
```python
+ name);
      }
    }
  }
```
```python
// 从mybatis初始化时创建的mappedStatements中找到本SQL语句对应的mappedStatement
```
```python
private
```
```python
MappedStatement
```
```python
resolveMappedStatement
```
```python
(Class<?> mapperInterface, String methodName,
        Class<?> declaringClass, Configuration configuration) {
```
```python
// 以mapper接口类名.方法名，作为id，这样可以唯一对应一条SQL操作。
```
```python
// mapper.xml中namespace对应接口类名，SQL语句id对应mapper接口中方法名。
```
```python
String statementId = mapperInterface.getName() +
```
```python
"."
```
```python
+ methodName;
```
```python
// 本SQL语句在mybatis初始化阶段已经添加了时
```
```python
if
```
```python
(configuration.hasStatement(statementId)) {
```
```python
// 直接从初始化mybatis阶段，生成的mappedStatements总map表中，获取当前SQL语句的mappedStatement
```
```python
return
```
```python
configuration.getMappedStatement(statementId);
      }
```
```python
else
```
```python
if
```
```python
(mapperInterface.equals(declaringClass)) {
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
```
```python
for
```
```python
(Class<?> superInterface : mapperInterface.getInterfaces()) {
```
```python
if
```
```python
(declaringClass.isAssignableFrom(superInterface)) {
          MappedStatement ms = resolveMappedStatement(superInterface, methodName,
              declaringClass, configuration);
```
```python
if
```
```python
(ms !=
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
ms;
          }
        }
      }
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
MethodSignature构造方法我们就不详细分析了，有兴趣的同学可以自行分析。
#### 2.2.2 MapperMethod的执行
下面来看MapperMethod的方法执行，也就是execute方法。
```python
// 执行mapper接口中的方法
```
```python
public
```
```python
Object
```
```python
execute
```
```python
(SqlSession sqlSession, Object[] args) {
  Object result;
```
```python
// 按照SQL语句的不同type，如insert update等，调用sqlSession的insert update等对应方法，执行数据库操作
```
```python
// 下面重点分析Select
```
```python
switch
```
```python
(command.getType()) {
```
```python
case
```
```python
INSERT: {
      Object param = method.convertArgsToSqlCommandParam(args);
      result = rowCountResult(sqlSession.insert(command.getName(), param));
```
```python
break
```
```python
;
    }
```
```python
case
```
```python
UPDATE: {
      Object param = method.convertArgsToSqlCommandParam(args);
      result = rowCountResult(sqlSession.update(command.getName(), param));
```
```python
break
```
```python
;
    }
```
```python
case
```
```python
DELETE: {
      Object param = method.convertArgsToSqlCommandParam(args);
      result = rowCountResult(sqlSession.delete(command.getName(), param));
```
```python
break
```
```python
;
    }
```
```python
case
```
```python
SELECT:
```
```python
// 根据select返回值的不同，调用对应的不同方法来查询，其实本质上大同小异，最后都是调用了sqlSession的select方法
```
```python
if
```
```python
(method.returnsVoid() && method.hasResultHandler()) {
```
```python
// 无需返回值时
```
```python
executeWithResultHandler(sqlSession, args);
        result =
```
```python
null
```
```python
;
      }
```
```python
else
```
```python
if
```
```python
(method.returnsMany()) {
```
```python
// 返回值为Collection集合类型或者数组类型时，下面重点分析
```
```python
result = executeForMany(sqlSession, args);
      }
```
```python
else
```
```python
if
```
```python
(method.returnsMap()) {
```
```python
// 返回值为Map类型时
```
```python
result = executeForMap(sqlSession, args);
      }
```
```python
else
```
```python
if
```
```python
(method.returnsCursor()) {
```
```python
// 返回值为Cursor类型时
```
```python
result = executeForCursor(sqlSession, args);
      }
```
```python
else
```
```python
{
```
```python
// 返回值为其他类型时，认为不是集合类型，故调用selectOne返回一个对象。
```
```python
Object param = method.convertArgsToSqlCommandParam(args);
        result = sqlSession.selectOne(command.getName(), param);
      }
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
FLUSH:
      result = sqlSession.flushStatements();
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
BindingException(
```
```python
"Unknown execution method for: "
```
```python
+ command.getName());
  }
```
```python
if
```
```python
(result ==
```
```python
null
```
```python
&& method.getReturnType().isPrimitive() && !method.returnsVoid()) {
```
```python
throw
```
```python
new
```
```python
BindingException(
```
```python
"Mapper method '"
```
```python
+ command.getName() 
        +
```
```python
" attempted to return null from a method with a primitive return type ("
```
```python
+ method.getReturnType() +
```
```python
")."
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
execute按照SQL语句的不同type，如insert update等，调用sqlSession的insert update等对应方法。由此可见，最终mapper方式还是通过sqlSession来完成数据库insert select等操作的。我们以select这个type为例分析。不同的select返回值类型，如集合Collection，Map等对应不同的执行方法，但其实本质都是调用selectList方法。我们以最常见的返回Collection为例分析。
返回值为Collection集合类型或者数组类型时，调用executeForMany()来执行方法调用。如下。
```python
// select返回值为集合类型时，如List
```
```python
private
```
```python
<E> Object
```
```python
executeForMany
```
```python
(SqlSession sqlSession, Object[] args) {
  List<E> result;
```
```python
// 调用paramNameResolver，处理入参
```
```python
Object param = method.convertArgsToSqlCommandParam(args);
```
```python
// 调用sqlSession的selectList()方法，查询数据库。是否定义了逻辑分页时，会有些细微差别
```
```python
// selectList方法前面一节已经讲解过了，这儿不详细讲了。
```
```python
// 从这儿可见，mapper方式最终还是通过sqlSession操作数据库。
```
```python
// mapper方式比直接操作方式更灵活，且不易出错。mybatis建议大家使用mapper方式。
```
```python
if
```
```python
(method.hasRowBounds()) {
```
```python
// 定义了逻辑分页rowBounds时
```
```python
RowBounds rowBounds = method.extractRowBounds(args);
    result = sqlSession.<E>selectList(command.getName(), param, rowBounds);
  }
```
```python
else
```
```python
{
```
```python
// 未定义rowBounds时
```
```python
result = sqlSession.<E>selectList(command.getName(), param);
  }
```
```python
// 由于selectList返回的是List类型的集合，而mapper接口中的方法返回值可能为Set Queue等其他集合类型，故此处需要做转换
```
```python
if
```
```python
(!method.getReturnType().isAssignableFrom(result.getClass())) {
```
```python
if
```
```python
(method.getReturnType().isArray()) {
```
```python
return
```
```python
convertToArray(result);
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
convertToDeclaredCollection(sqlSession.getConfiguration(), result);
    }
  }
```
```python
return
```
```python
result;
}
```
可以很清晰的看到，最终我们还是通过sqlSession的selectList方法完成查询操作的。selectList()方法的流程上一节已经讲解过了，这儿就不说了。这样，mapper动态代理就完成了它的方法调用了。
## 3 总结
sqlSession的mapper动态代理方法调用，比直接通过sqlSession的selectList，update等方法更优。mapper方式是sqlSession selectList等直接调用方式的一层封装。正因为有了这层封装，使得mybatis在增加灵活性之余，还能提高健壮性。这也充分体现了mybatis的设计精巧，值得我们学习。
### 相关文章
[mybatis源码分析1 - 框架](http://blog.csdn.net/u013510838/article/details/79055792)
[mybatis源码分析2 - SqlSessionFactory的创建](http://blog.csdn.net/u013510838/article/details/78995542)
[mybatis源码分析3 - sqlSession的创建](http://blog.csdn.net/u013510838/article/details/79011489)
[mybatis源码分析4 - sqlSession读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79016238)
[mybatis源码分析5 - mapper读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79023822)
[mybatis源码分析6 - mybatis-spring容器初始化](http://blog.csdn.net/u013510838/article/details/79053519)
[mybatis源码分析7 - mybatis-spring读写数据库全过程](http://blog.csdn.net/u013510838/article/details/79054007)

