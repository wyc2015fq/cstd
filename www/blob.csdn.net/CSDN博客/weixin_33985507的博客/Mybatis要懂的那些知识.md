# Mybatis要懂的那些知识 - weixin_33985507的博客 - CSDN博客
2019年01月27日 11:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
- mybatis-config.xml里<environments>标签可以放置多个environment，这里可以切换test和develop数据源
- databaseIdProvider提供多种数据库，在xml映射文件里选择databaseId即可使用对应的数据库
- Oracle的自增是使用Sequence实现的，若要获取插入数据的主键，在xml映射里写上selectKey标签，select seq.nextval from dual;
- 哪些地方不能使用占位符？比如分表时表名，orderby，desc
- @Mapkey("id")指定返回类型为map时，将id作为key
## 1. mybatis参数绑定的过程
我们写一个UserMapper接口，Mybatis会为改接口创建一个MapperProxy对象。
### 1.1 MapperRegistry
在继续之前，先来了解一下MapperRegistry。
- MapperRegistry是mapper的注册类，提供了注册一个包下面的所有Mapper接口。内部维护了一个`private final Map<Class<?>, MapperProxyFactory<?>> knownMappers = new HashMap<Class<?>, MapperProxyFactory<?>>();`map以mapper接口类为key，value是接口类对应的代理工厂。
`void addMapper(Class<T> type)`方法简单的将type放进knownMappers里面。不过，这里面还有
```java
MapperAnnotationBuilder parser = new MapperAnnotationBuilder(config, type);
        parser.parse();
```
这样一个解析操作，暂不做分析。
- `<T> T getMapper(Class<T> type, SqlSession sqlSession)`该方法是本类最重要的一个方法了，获取Mapper接口的代理对象也正是从该方法获取的。
该方法实现就是，knownMappers.get(type)得到代理对象工厂mapperProxyFactory，返回的是`mapperProxyFactory.newInstance(sqlSession)`。
进入该方法发现过程也比较简单，构造了一个MapperProxy对象
```java
public T newInstance(SqlSession sqlSession) {
    final MapperProxy<T> mapperProxy = new MapperProxy<T>(sqlSession, mapperInterface, methodCache);
    return newInstance(mapperProxy);
  }
```
再进去看
```java
protected T newInstance(MapperProxy<T> mapperProxy) {
    return (T) Proxy.newProxyInstance(mapperInterface.getClassLoader(), new Class[] { mapperInterface }, mapperProxy);
  }
```
发现这就是标准的JDK面向接口的动态代理啊。要问对应的InvocationHandler在哪里？mapperProxy就是啦。
至此，Mybatis如何获取Mapper接口的代理对象已经解决。
再梳理一下整个流程：
> 
sqlsession.getMapper->configuration.getMapper->mapperRegistry.getMapper->获取Mapper代理对象
## 1.2
创建好MapperProxy对象之后，调用Mapper.xx方法时就会走MapperProxy的invoke方法
```java
public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
    try {
      if (Object.class.equals(method.getDeclaringClass())) {//如果方法是在Object里面的，放行
        return method.invoke(this, args);
      } else if (isDefaultMethod(method)) {
        return invokeDefaultMethod(proxy, method, args);
      }
    } catch (Throwable t) {
      throw ExceptionUtil.unwrapThrowable(t);
    }
    final MapperMethod mapperMethod = cachedMapperMethod(method);//缓存method方法
    return mapperMethod.execute(sqlSession, args); // 执行
  }
```
发现关键点在于`mapperMethod.execute(sqlSession, args)`方法
