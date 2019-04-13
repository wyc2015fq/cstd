
# mybatis源码分析2 - SqlSessionFactory的创建 - 谢杨易的博客 - CSDN博客

2018年01月07日 16:29:00[谢杨易](https://me.csdn.net/u013510838)阅读数：878



## 1 主要类
初始化mybatis的过程，其实就是创建SqlSessionFactory单例的过程。下面是一个简单的初始化例子。
```python
String resource =
```
```python
"main/resources/SqlMapConfig.xml"
```
```python
;
InputStream inputStream = Resources.getResourceAsStream(resource);
SqlSessionFactory sessionFactory =
```
```python
new
```
```python
SqlSessionFactoryBuilder().build(inputStream);
```
初始化流程大致分为如下几步
mybatis读取全局xml配置文件，解析XML中各个节点元素
将节点元素键值对，设置到Configuration实例的相关变量中
由Configuration实例创建SqlSessionFactory单例对象
我们先来分析初始化过程中涉及的主要类
1.**SqlSessionFactoryBuilder**：用来创建SqlSessionFactory实例，典型的builder链式创建模式。
2.**XMLConfigBuilder**：主要有三个作用：
```python
-
```
```python
解析
```
```python
XML
```
```python
文件，生成
```
```python
XNode
```
```python
对象。
```
```python
XNode
```
```python
是
```
```python
XML
```
```python
文件中元素节点的描述。
```
```python
-
```
```python
创建并初始化
```
```python
Configuration
```
```python
实例
```
```python
-
```
```python
利用root根
```
```python
XNode
```
```python
对象，解析其各个子节点xNode，获取其中的属性或子元素，生成键值对。然后设置到
```
```python
Configuration
```
```python
实例的相关变量中
```
3.**Configuration**：它是一个数据类，包含了几乎所有的mybatis配置信息，他们会影响mybatis的执行流程。初始化阶段最重要的就是创建并设置Configuration的相关变量。
4.**SqlSessionFactory**：创建SqlSession实例的工厂类，一般我们都把它作为单例使用。它的默认实现为DefaultSqlSessionFactory
## 2 流程
我们一般通过new SqlSessionFactoryBuilder().build()来创建SqlSessionFactory对象。下面是SqlSessionFactoryBuilder的build()方法
```python
// 创建SqlSessionFactory单例
```
```python
// @Param inputStream: 读取的全局XML配置文件的输入流，配置信息都在这个文件中
```
```python
// @param environment：指定的此SqlSessionFactory的数据库环境，默认为default
```
```python
// @param properties: 设置一些动态化常量，会和XML中的properties 中常量合并在一起
```
```python
public
```
```python
SqlSessionFactory
```
```python
build
```
```python
(InputStream inputStream, String environment, Properties properties) {
```
```python
try
```
```python
{
```
```python
// 先构建XML文件解析器
```
```python
XMLConfigBuilder parser =
```
```python
new
```
```python
XMLConfigBuilder(inputStream, environment, properties);
```
```python
// 解析得到Configuration对象，这个对象包含了几乎所有的mybatis配置文件中的信息，十分关键。
```
```python
// 然后利用Configuration对象构建SqlSessionFactory单例
```
```python
return
```
```python
build(parser.parse());
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
"Error building SqlSession."
```
```python
, e);
  }
```
```python
finally
```
```python
{
    ErrorContext.instance().reset();
```
```python
try
```
```python
{
      inputStream.close();
    }
```
```python
catch
```
```python
(IOException e) {
```
```python
// Intentionally ignore. Prefer previous error.
```
```python
}
  }
}
```
build方法比较简单，从中可以分析出mybatis初始化的三大流程，即读取XML文件，设置Configuration中相关变量，创建SqlSessionFactory.下面我们一步步分析。
### 2.1 读取XML文件，解析节点元素
mybatis初始化第一步，就是读取全局XML配置文件，解析文件节点，生成xNode对象。下面来分析这个过程，先来看XMLConfigBuilder的实例化过程。
```python
// new XMLConfigBuilder()的过程。
```
```python
public
```
```python
XMLConfigBuilder
```
```python
(InputStream inputStream, String environment, Properties props) {
```
```python
// 利用XML配置文件输入流，创建XPathParser解析对象
```
```python
this
```
```python
(
```
```python
new
```
```python
XPathParser(inputStream,
```
```python
true
```
```python
, props,
```
```python
new
```
```python
XMLMapperEntityResolver()), environment, props);
}
```
```python
private
```
```python
XMLConfigBuilder
```
```python
(XPathParser parser, String environment, Properties props) {
```
```python
// 先创建Configuration实例对象，也会做一些基本的初始化。主要是注册一些别名供后面解析XML用，如JDBC
```
```python
super
```
```python
(
```
```python
new
```
```python
Configuration());
  ErrorContext.instance().resource(
```
```python
"SQL Mapper Configuration"
```
```python
);
```
```python
// 将new SqlSessionFactoryBuilder().build()中传入的Properties键值对，设置到configuration对象的variables变量中，它会和后面解析properties 子节点得到的键值对做合并。他们主要作用是配置常量动态化。
```
```python
this
```
```python
.configuration.setVariables(props);
```
```python
this
```
```python
.parsed =
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
.environment = environment;
```
```python
this
```
```python
.parser = parser;
}
```
实例化XMLConfigBuilder后，configuration对象就创建好了，它的成员变量暂时还没有设置好。接下来就是读取XML文件，生成xNode节点了。xNode链式节点是一个XML文件的Java描述，每个xNode都对应XML中的一个节点元素。下面来分析parser.parse()，代码如下
```python
public
```
```python
Configuration
```
```python
parse
```
```python
() {
```
```python
if
```
```python
(parsed) {
```
```python
throw
```
```python
new
```
```python
BuilderException(
```
```python
"Each XMLConfigBuilder can only be used once."
```
```python
);
  }
  parsed =
```
```python
true
```
```python
;
```
```python
// 利用parser，创建xNode对象
```
```python
// parseConfiguration将解析后的键值对设置到Configuration实例的相关变量中,下一节详细讲解
```
```python
parseConfiguration(parser.evalNode(
```
```python
"/configuration"
```
```python
));
```
```python
return
```
```python
configuration;
}
```
解析XML文件，生成xNode链式节点的过程就在parser.evalNode(“/configuration”)中。它通过Apache的XPath解析器，读取并解析各个节点。这一部分我们不用仔细分析，只需要知道XNode对象中包含了XML的配置信息即可。
### 2.2 设置Configuration实例的相关变量
读取并解析了XML文件后，接下来就是将解析的XML配置信息，设置到configuration实例的相关变量中了。接着上一节的parseConfiguration()方法讲。
```python
// 解析由XML文件生成的XNode对象，生成Configuration对象。这个是关键方法
```
```python
private
```
```python
void
```
```python
parseConfiguration
```
```python
(XNode root) {
```
```python
try
```
```python
{
```
```python
// 解析properties  节点，存放到Configuration对象的variables变量中，用来将配置变量动态化。
```
```python
// 如配置dataSource的username password
```
```python
propertiesElement(root.evalNode(
```
```python
"properties"
```
```python
));
```
```python
// 解析settings  节点，会改写Configuration中的相关值。
```
```python
// 这些值决定了mybatis的运行方式，如CacheEnabled lazyLoadingEnabled等属性
```
```python
Properties settings = settingsAsProperties(root.evalNode(
```
```python
"settings"
```
```python
));
    loadCustomVfs(settings);
```
```python
// 解析typeAliases  节点，定义别名，一般用来为Java全路径类型取一个比较短的名字
```
```python
typeAliasesElement(root.evalNode(
```
```python
"typeAliases"
```
```python
));
```
```python
// 解析plugins  节点， 定义插件，用来拦截某些类，从而改变这些类的执行
```
```python
// 如四大基本插件，Executor ParameterHandler StatementHandler ResultSetHandler
```
```python
pluginElement(root.evalNode(
```
```python
"plugins"
```
```python
));
```
```python
// 解析objectFactory  节点，定义对象工厂，不常用。对象工厂用来创建mybatis返回的结果对象
```
```python
objectFactoryElement(root.evalNode(
```
```python
"objectFactory"
```
```python
));
```
```python
// 解析objectWrapperFactory  节点, 包装Object实例，不常用
```
```python
objectWrapperFactoryElement(root.evalNode(
```
```python
"objectWrapperFactory"
```
```python
));
```
```python
// 解析reflectorFactory  节点，创建Reflector类反射，不常用
```
```python
reflectorFactoryElement(root.evalNode(
```
```python
"reflectorFactory"
```
```python
));
```
```python
// 将解析并读取settings 节点后得到的键值对，设置到Configuration实例的相关变量中
```
```python
// 这些键值对决定了mybatis的运行方式，如果没设置，则采用默认值
```
```python
settingsElement(settings);
```
```python
// 解析environments  节点， 定义数据库环境。
```
```python
// 可以配置多个environment，每个对应一个dataSource和transactionManager
```
```python
environmentsElement(root.evalNode(
```
```python
"environments"
```
```python
));
```
```python
// 解析databaseIdProvider  节点，定义数据库厂商标识。
```
```python
// mybatis可以根据不同的厂商执行不同的语句
```
```python
databaseIdProviderElement(root.evalNode(
```
```python
"databaseIdProvider"
```
```python
));
```
```python
// 解析typeHandlers  节点， 定义类型处理器，用来将数据库中获取的值转换为Java类型
```
```python
typeHandlerElement(root.evalNode(
```
```python
"typeHandlers"
```
```python
));
```
```python
// 解析mappers  节点， 定义映射器，也就是SQL映射语句。mappers中定义好映射文件的位置即可。
```
```python
mapperElement(root.evalNode(
```
```python
"mappers"
```
```python
));
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
BuilderException(
```
```python
"Error parsing SQL Mapper Configuration. Cause: "
```
```python
+ e, e);
  }
}
```
parseConfiguration逐一读取XML配置信息的子节点，然后将它们设置到configuration变量中，比如properties，typeAliases，mappers等。下面我们逐一分析。先看properties节点
```python
// 读取并解析properties 节点， 合并到configuration的variables变量中
```
```python
private
```
```python
void
```
```python
propertiesElement
```
```python
(XNode context)
```
```python
throws
```
```python
Exception {
```
```python
if
```
```python
(context !=
```
```python
null
```
```python
) {
```
```python
// 获取properties 中的resource或者url属性，二者必须定义一个，否则定义有误
```
```python
Properties defaults = context.getChildrenAsProperties();
    String resource = context.getStringAttribute(
```
```python
"resource"
```
```python
);
    String url = context.getStringAttribute(
```
```python
"url"
```
```python
);
```
```python
if
```
```python
(resource !=
```
```python
null
```
```python
&& url !=
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
BuilderException(
```
```python
"The properties element cannot specify both a URL and a resource based property file reference.  Please specify one or the other."
```
```python
);
    }
```
```python
// 从resource或者url中读取资源，生成Properties对象。Properties是一个 Hashtable，保存了<key,value>的键值对
```
```python
if
```
```python
(resource !=
```
```python
null
```
```python
) {
      defaults.putAll(Resources.getResourceAsProperties(resource));
    }
```
```python
else
```
```python
if
```
```python
(url !=
```
```python
null
```
```python
) {
      defaults.putAll(Resources.getUrlAsProperties(url));
    }
```
```python
// 将configuration本来就存在的Hashtable，variables变量，和从resource或者url中加载的Hashtable合并，更新到variables变量中。
```
```python
Properties vars = configuration.getVariables();
```
```python
if
```
```python
(vars !=
```
```python
null
```
```python
) {
      defaults.putAll(vars);
    }
    parser.setVariables(defaults);
    configuration.setVariables(defaults);
  }
}
```
propertiesElement用来解析properties节点，合并到configuration的variables变量中。variables变量是由new SqlSessionFactoryBuilder().build(Reader reader, String environment, Properties properties)时传入的初始properties初始化的，详细可参见前面的分析。下面来分析settings 节点。
```python
// 读取并解析settings元素，生成Properties键值对
```
```python
private
```
```python
Properties
```
```python
settingsAsProperties
```
```python
(XNode context) {
```
```python
if
```
```python
(context ==
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
new
```
```python
Properties();
  }
  Properties props = context.getChildrenAsProperties();
```
```python
// Check that all settings are known to the configuration class
```
```python
MetaClass metaConfig = MetaClass.forClass(Configuration.class, localReflectorFactory);
```
```python
for
```
```python
(Object key : props.keySet()) {
```
```python
if
```
```python
(!metaConfig.hasSetter(String.valueOf(key))) {
```
```python
throw
```
```python
new
```
```python
BuilderException(
```
```python
"The setting "
```
```python
+ key +
```
```python
" is not known.  Make sure you spelled it correctly (case sensitive)."
```
```python
);
    }
  }
```
```python
return
```
```python
props;
}
```
```python
// 获取用户自定义的vfs的实现，配置在settings元素中。
```
```python
// settings中放置自定义vfs实现类的全限定名，以逗号分隔
```
```python
private
```
```python
void
```
```python
loadCustomVfs
```
```python
(Properties props)
```
```python
throws
```
```python
ClassNotFoundException {
  String value = props.getProperty(
```
```python
"vfsImpl"
```
```python
);
```
```python
if
```
```python
(value !=
```
```python
null
```
```python
) {
```
```python
// vfs实现类可以有多个，其实现类全限定名以逗号分隔
```
```python
String[] clazzes = value.split(
```
```python
","
```
```python
);
```
```python
for
```
```python
(String clazz : clazzes) {
```
```python
if
```
```python
(!clazz.isEmpty()) {
```
```python
// 反射加载自定义vfs实现类，并设置到Configuration实例中
```
```python
Class<? extends VFS> vfsImpl = (Class<? extends VFS>)Resources.classForName(clazz);
        configuration.setVfsImpl(vfsImpl);
      }
    }
  }
}
```
这一步还没有将settings配置信息设置到configuration中，会通过后面的settingsElement(settings)方法进行设置。之所以放在后面，是因为解析settings时，会利用到其他一些节点元素的解析结果，比如typeAliases的别名，plugin的拦截器等。settingsElement()源码如下
```python
// 将解析并读取settings节点后得到的键值对，设置到Configuration实例的相关变量中
```
```python
private
```
```python
void
```
```python
settingsElement
```
```python
(Properties props)
```
```python
throws
```
```python
Exception {
```
```python
// 设置configuration的autoMappingBehavior变量， 指定mybatis应如何自动映射数据库列到POJO对象属性。
```
```python
// 为NONE表示取消自动映射，PARTIAL表示只映射非嵌套结果集，FULL表示映射所有结果集，即使有嵌套。
```
```python
configuration.setAutoMappingBehavior(AutoMappingBehavior.valueOf(props.getProperty(
```
```python
"autoMappingBehavior"
```
```python
,
```
```python
"PARTIAL"
```
```python
)));
```
```python
// 设置autoMappingUnknownColumnBehavior，mybatis自动映射时对未定义列应如何处理。
```
```python
// 为NONE则不做任何处理，WARNING输出提醒日志，FAILING映射失败，抛出SqlSessionException
```
```python
configuration.setAutoMappingUnknownColumnBehavior(AutoMappingUnknownColumnBehavior.valueOf(props.getProperty(
```
```python
"autoMappingUnknownColumnBehavior"
```
```python
,
```
```python
"NONE"
```
```python
)));
```
```python
// 设置cacheEnabled, 是否开启二级缓存，也就是SqlSessionFactory级别的缓存。一级缓存，也即sqlSession内的HashMap，是默认开启的。
```
```python
configuration.setCacheEnabled(booleanValueOf(props.getProperty(
```
```python
"cacheEnabled"
```
```python
),
```
```python
true
```
```python
));
```
```python
// 设置proxyFactory，指定mybatis创建延迟加载对象所用到的动态代理工具。可为CGLIB 或 JAVASSIST
```
```python
configuration.setProxyFactory((ProxyFactory) createInstance(props.getProperty(
```
```python
"proxyFactory"
```
```python
)));
```
```python
// 设置lazyLoadingEnabled，延迟加载，开启时所有关联对象都会延迟加载，除非关联对象中设置了fetchType来覆盖它。
```
```python
configuration.setLazyLoadingEnabled(booleanValueOf(props.getProperty(
```
```python
"lazyLoadingEnabled"
```
```python
),
```
```python
false
```
```python
));
```
```python
// 设置aggressiveLazyLoading，开启时，调用对象内任何方法都会加载对象内所有属性，默认为false，即按需加载
```
```python
configuration.setAggressiveLazyLoading(booleanValueOf(props.getProperty(
```
```python
"aggressiveLazyLoading"
```
```python
),
```
```python
false
```
```python
));
```
```python
// 设置 multipleResultSetsEnabled，开启时，允许单一语句返回多结果
```
```python
configuration.setMultipleResultSetsEnabled(booleanValueOf(props.getProperty(
```
```python
"multipleResultSetsEnabled"
```
```python
),
```
```python
true
```
```python
));
```
```python
// 设置useColumnLabel，使用列标签代替列名
```
```python
configuration.setUseColumnLabel(booleanValueOf(props.getProperty(
```
```python
"useColumnLabel"
```
```python
),
```
```python
true
```
```python
));
```
```python
// 设置useGeneratedKeys，允许JDBC自动生成主键
```
```python
configuration.setUseGeneratedKeys(booleanValueOf(props.getProperty(
```
```python
"useGeneratedKeys"
```
```python
),
```
```python
false
```
```python
));
```
```python
// 设置defaultExecutorType，默认执行器
```
```python
// SIMPLE 普通执行器；REUSE 重用预处理语句；BATCH 批量更新
```
```python
configuration.setDefaultExecutorType(ExecutorType.valueOf(props.getProperty(
```
```python
"defaultExecutorType"
```
```python
,
```
```python
"SIMPLE"
```
```python
)));
```
```python
// 设置defaultStatementTimeout，超时时间，也就是数据库驱动等待数据库响应的时间，单位为秒
```
```python
configuration.setDefaultStatementTimeout(integerValueOf(props.getProperty(
```
```python
"defaultStatementTimeout"
```
```python
),
```
```python
null
```
```python
));
```
```python
// 设置defaultFetchSize，每次返回的数据库结果集的行数。使用它可避免内存溢出
```
```python
configuration.setDefaultFetchSize(integerValueOf(props.getProperty(
```
```python
"defaultFetchSize"
```
```python
),
```
```python
null
```
```python
));
```
```python
// 设置mapUnderscoreToCamelCase，开启自动驼峰命名规则映射，即将数据库列名a_column映射为Java属性aColumn
```
```python
configuration.setMapUnderscoreToCamelCase(booleanValueOf(props.getProperty(
```
```python
"mapUnderscoreToCamelCase"
```
```python
),
```
```python
false
```
```python
));
```
```python
// 设置safeRowBoundsEnabled，允许在嵌套语句中使用分页RowBounds
```
```python
configuration.setSafeRowBoundsEnabled(booleanValueOf(props.getProperty(
```
```python
"safeRowBoundsEnabled"
```
```python
),
```
```python
false
```
```python
));
```
```python
// 设置localCacheScope，本地缓存的作用域。本地缓存用来加速嵌套查询和防止循环引用
```
```python
// 为SESSION，则缓存本sqlSession中的所有查询语句。为STATEMENT，则相同sqlSession的同一个调用语句才做缓存。
```
```python
configuration.setLocalCacheScope(LocalCacheScope.valueOf(props.getProperty(
```
```python
"localCacheScope"
```
```python
,
```
```python
"SESSION"
```
```python
)));
```
```python
// 设置jdbcTypeForNull，没有为参数提供特定的JDBC类型时，Java NULL对应的JDBC类型
```
```python
// 可为 NULL、VARCHAR 或 OTHER
```
```python
configuration.setJdbcTypeForNull(JdbcType.valueOf(props.getProperty(
```
```python
"jdbcTypeForNull"
```
```python
,
```
```python
"OTHER"
```
```python
)));
```
```python
// 设置lazyLoadTriggerMethods，指定哪些方法会触发延迟加载关联对象。方法名之间用逗号隔开，如equals,clone,hashCode,toString
```
```python
configuration.setLazyLoadTriggerMethods(stringSetValueOf(props.getProperty(
```
```python
"lazyLoadTriggerMethods"
```
```python
),
```
```python
"equals,clone,hashCode,toString"
```
```python
));
```
```python
// 设置safeResultHandlerEnabled，允许在嵌套语句中使用分页（ResultHandler）
```
```python
configuration.setSafeResultHandlerEnabled(booleanValueOf(props.getProperty(
```
```python
"safeResultHandlerEnabled"
```
```python
),
```
```python
true
```
```python
));
```
```python
// 设置defaultScriptingLanguage，指定用来生成动态SQL语句的默认语言
```
```python
configuration.setDefaultScriptingLanguage(resolveClass(props.getProperty(
```
```python
"defaultScriptingLanguage"
```
```python
)));
```
```python
// 设置defaultEnumTypeHandler，指定Enum对应的默认的TypeHandler. 它是一个Java全限定名。如org.apache.ibatis.type.EnumTypeHandler
```
```python
@SuppressWarnings
```
```python
(
```
```python
"unchecked"
```
```python
)
  Class<? extends TypeHandler> typeHandler = (Class<? extends TypeHandler>)resolveClass(props.getProperty(
```
```python
"defaultEnumTypeHandler"
```
```python
));
  configuration.setDefaultEnumTypeHandler(typeHandler);
```
```python
// 设置callSettersOnNulls，指定结果集中null值是否调用Java映射对象的setter
```
```python
configuration.setCallSettersOnNulls(booleanValueOf(props.getProperty(
```
```python
"callSettersOnNulls"
```
```python
),
```
```python
false
```
```python
));
```
```python
// 设置useActualParamName，允许使用方法签名中的形参名作为SQL语句的参数名称。使用这个属性时，工程必须支持Java8
```
```python
configuration.setUseActualParamName(booleanValueOf(props.getProperty(
```
```python
"useActualParamName"
```
```python
),
```
```python
true
```
```python
));
```
```python
// 设置returnInstanceForEmptyRow，当某一行的所有列都为空时，mybatis返回一个null实例对象。
```
```python
configuration.setReturnInstanceForEmptyRow(booleanValueOf(props.getProperty(
```
```python
"returnInstanceForEmptyRow"
```
```python
),
```
```python
false
```
```python
));
```
```python
// 设置logPrefix，指定mybatis添加到日志中的前缀。设置后每一条日志都会添加这个前缀
```
```python
configuration.setLogPrefix(props.getProperty(
```
```python
"logPrefix"
```
```python
));
```
```python
// 设置logImpl，指定mybatis使用的日志系统，如LOG4J
```
```python
@SuppressWarnings
```
```python
(
```
```python
"unchecked"
```
```python
)
  Class<? extends Log> logImpl = (Class<? extends Log>)resolveClass(props.getProperty(
```
```python
"logImpl"
```
```python
));
  configuration.setLogImpl(logImpl);
```
```python
// 设置configurationFactory，指定生成Configuration对象的工厂类。工厂类必须包含getConfiguration()方法
```
```python
configuration.setConfigurationFactory(resolveClass(props.getProperty(
```
```python
"configurationFactory"
```
```python
)));
}
```
分析完settings节点后，我们来分析typeAliases节点，源码如下
```python
// 读取并解析typeAliases元素，并设置到Configuration的typeAliasRegistry中
```
```python
private
```
```python
void
```
```python
typeAliasesElement
```
```python
(XNode parent) {
```
```python
if
```
```python
(parent !=
```
```python
null
```
```python
) {
```
```python
for
```
```python
(XNode child : parent.getChildren()) {
```
```python
if
```
```python
(
```
```python
"package"
```
```python
.equals(child.getName())) {
```
```python
// 元素为package时，mybatis会在搜索包名下需要的Java bean。使用bean的首字母的小写的非限定名来作为它的别名
```
```python
String typeAliasPackage = child.getStringAttribute(
```
```python
"name"
```
```python
);
        configuration.getTypeAliasRegistry().registerAliases(typeAliasPackage);
      }
```
```python
else
```
```python
{
```
```python
// 元素为typeAlias时，读取单个typeAlias 的alias和type元素，它们分别是别名和原名
```
```python
String alias = child.getStringAttribute(
```
```python
"alias"
```
```python
);
```
```python
// 别名
```
```python
String type = child.getStringAttribute(
```
```python
"type"
```
```python
);
```
```python
// 原名
```
```python
try
```
```python
{
```
```python
// 反射加载type对应的原类型，然后以alias作为key，class对象作为value放入TYPE_ALIASES这个Map中。
```
```python
// 这样使用到别名的时候就可以使用真实的class类来替换
```
```python
// 这个Map定义了很多默认的别名映射，如string byte int
```
```python
Class<?> clazz = Resources.classForName(type);
```
```python
if
```
```python
(alias ==
```
```python
null
```
```python
) {
            typeAliasRegistry.registerAlias(clazz);
          }
```
```python
else
```
```python
{
            typeAliasRegistry.registerAlias(alias, clazz);
          }
        }
```
```python
catch
```
```python
(ClassNotFoundException e) {
```
```python
throw
```
```python
new
```
```python
BuilderException(
```
```python
"Error registering typeAlias for '"
```
```python
+ alias +
```
```python
"'. Cause: "
```
```python
+ e, e);
        }
      }
    }
  }
}
```
typeAliasesElement会读取并解析typeAliases元素，并设置到Configuration的typeAliasRegistry中。它的子元素可以为package 或者typeAlias。下面分析plugins 节点
```python
// 读取并解析plugin元素，并添加到Configuration的interceptorChain中
```
```python
private
```
```python
void
```
```python
pluginElement
```
```python
(XNode parent)
```
```python
throws
```
```python
Exception {
```
```python
if
```
```python
(parent !=
```
```python
null
```
```python
) {
```
```python
// 遍历plugins中的每个plugin
```
```python
for
```
```python
(XNode child : parent.getChildren()) {
```
```python
// 读取plugin中的interceptor属性，它声明了插件的实现类的全限定名
```
```python
String interceptor = child.getStringAttribute(
```
```python
"interceptor"
```
```python
);
```
```python
// 读取plugin中的property元素，它声明了插件类的构造参数。
```
```python
Properties properties = child.getChildrenAsProperties();
```
```python
// 有了实现类的全限定名和构造参数后，就可以反射创建插件对象实例了，并初始化它
```
```python
Interceptor interceptorInstance = (Interceptor) resolveClass(interceptor).newInstance();
      interceptorInstance.setProperties(properties);
```
```python
// 将创建并初始化好的插件对象实例添加到Configuration对象中
```
```python
configuration.addInterceptor(interceptorInstance);
    }
  }
}
```
pluginElement会读取并解析plugin元素，并添加到Configuration的interceptorChain中。读取它的plugin子节点，解析其中的interceptor属性和property子元素，来反射创建并初始化插件实例。下面来分析environments 节点。
```python
// 读取并解析environments元素，并设置到Configuration实例中
```
```python
private
```
```python
void
```
```python
environmentsElement
```
```python
(XNode context)
```
```python
throws
```
```python
Exception {
```
```python
if
```
```python
(context !=
```
```python
null
```
```python
) {
```
```python
// environment为SqlSessionFactoryBuilder().build(inputStream,environment)时传入的String，
```
```python
// 它指定了SqlSessionFactory所使用的数据库环境。不声明的话则采用XML中default元素对应的数据库环境
```
```python
if
```
```python
(environment ==
```
```python
null
```
```python
) {
      environment = context.getStringAttribute(
```
```python
"default"
```
```python
);
    }
```
```python
// 遍历各个子environment
```
```python
for
```
```python
(XNode child : context.getChildren()) {
```
```python
// 先获取environment的id元素
```
```python
String id = child.getStringAttribute(
```
```python
"id"
```
```python
);
```
```python
// 判断id是否等于上面指定的environment String。因为我们只需要加载我们指定的environment即可。
```
```python
// 这儿就可以明白为啥XML中可以配置多个数据库环境，运行时可以由我们动态选择了。
```
```python
if
```
```python
(isSpecifiedEnvironment(id)) {
```
```python
// 获取transactionManager元素，创建TransactionFactory实例并初始化它。
```
```python
TransactionFactory txFactory = transactionManagerElement(child.evalNode(
```
```python
"transactionManager"
```
```python
));
```
```python
// 获取dataSource元素, 创建DataSourceFactory实例并初始化它
```
```python
DataSourceFactory dsFactory = dataSourceElement(child.evalNode(
```
```python
"dataSource"
```
```python
));
        DataSource dataSource = dsFactory.getDataSource();
```
```python
// builder设计模式创建Environment对象，它包含id，transactionFactory，dataSource成员变量
```
```python
Environment.Builder environmentBuilder =
```
```python
new
```
```python
Environment.Builder(id)
            .transactionFactory(txFactory)
            .dataSource(dataSource);
```
```python
// 将创建好的Environment对象设置到configuration实例中
```
```python
configuration.setEnvironment(environmentBuilder.build());
      }
    }
  }
}
```
environmentsElement()读取并解析environments元素，并设置到Configuration实例中。他会根据new SqlSessionFactoryBuilder().build(Reader reader, String environment, Properties properties)时指定的数据库环境environment，来找到对应的environments 子节点，然后读取它的transactionManager 和dataSource 子节点，创建并初始化它们，然后设置到Configuration实例的environment变量中。下面来看typeHandlers 子节点。
```python
// 读取并解析typeHandler元素，并添加到typeHandlerRegistry变量中
```
```python
private
```
```python
void
```
```python
typeHandlerElement
```
```python
(XNode parent)
```
```python
throws
```
```python
Exception {
```
```python
if
```
```python
(parent !=
```
```python
null
```
```python
) {
```
```python
for
```
```python
(XNode child : parent.getChildren()) {
```
```python
if
```
```python
(
```
```python
"package"
```
```python
.equals(child.getName())) {
```
```python
// 使用package子元素时，mybatis自动检索包名下添加了相关注解的typeHandler，实例化它
```
```python
String typeHandlerPackage = child.getStringAttribute(
```
```python
"name"
```
```python
);
        typeHandlerRegistry.register(typeHandlerPackage);
      }
```
```python
else
```
```python
{
```
```python
// 使用typeHandler子元素时，分别读取javaType， jdbcType， handler三个元素。
```
```python
// javaType对应Java类型 jdbcType对应数据库中的类型，handler则为类处理器
```
```python
// 比如我们将varchar转变为String，就需要一个typeHandler
```
```python
String javaTypeName = child.getStringAttribute(
```
```python
"javaType"
```
```python
);
        String jdbcTypeName = child.getStringAttribute(
```
```python
"jdbcType"
```
```python
);
        String handlerTypeName = child.getStringAttribute(
```
```python
"handler"
```
```python
);
```
```python
// 利用javaTypeName， jdbcTypeName， handlerName创建实例化对象，可以为别名或者全限定名。
```
```python
// typeAlias这个时候就派上用场了。
```
```python
Class<?> javaTypeClass = resolveClass(javaTypeName);
        JdbcType jdbcType = resolveJdbcType(jdbcTypeName);
        Class<?> typeHandlerClass = resolveClass(handlerTypeName);
```
```python
// 将实例化好的对象添加到typeHandlerRegistry中，mybatis运行时就需要用到它了。此时是注册阶段。
```
```python
if
```
```python
(javaTypeClass !=
```
```python
null
```
```python
) {
```
```python
if
```
```python
(jdbcType ==
```
```python
null
```
```python
) {
            typeHandlerRegistry.register(javaTypeClass, typeHandlerClass);
          }
```
```python
else
```
```python
{
            typeHandlerRegistry.register(javaTypeClass, jdbcType, typeHandlerClass);
          }
        }
```
```python
else
```
```python
{
          typeHandlerRegistry.register(typeHandlerClass);
        }
      }
    }
  }
}
```
typeHandlerElement() 读取并解析typeHandler元素，并添加到typeHandlerRegistry变量中。它的子元素可以为package 或typeHandler 。根据javaType jdbcType handler 三个属性指定的全限定类名或别名，创建并初始化javaTypeClass JdbcType typeHandlerClass，然后设置到typeHandlerRegistry变量中。下面来分析mappers节点。mappers定义了SQL语句映射关系，要复杂很多。
```python
// 读取并解析mappers元素，并添加到configuration实例的mapperRegistry变量中
```
```python
private
```
```python
void
```
```python
mapperElement
```
```python
(XNode parent)
```
```python
throws
```
```python
Exception {
```
```python
if
```
```python
(parent !=
```
```python
null
```
```python
) {
```
```python
for
```
```python
(XNode child : parent.getChildren()) {
```
```python
if
```
```python
(
```
```python
"package"
```
```python
.equals(child.getName())) {
```
```python
// 子元素为package时，mybatis将包名下所有的接口认为是mapper类。创建其类对象并添加到mapperRegistry中。
```
```python
// 此时一般是注解方式，不需要使用XML mapper文件。
```
```python
String mapperPackage = child.getStringAttribute(
```
```python
"name"
```
```python
);
        configuration.addMappers(mapperPackage);
      }
```
```python
else
```
```python
{
```
```python
// 子元素为mapper时，读取子元素的resource或url或class属性。
```
```python
String resource = child.getStringAttribute(
```
```python
"resource"
```
```python
);
        String url = child.getStringAttribute(
```
```python
"url"
```
```python
);
        String mapperClass = child.getStringAttribute(
```
```python
"class"
```
```python
);
```
```python
if
```
```python
(resource !=
```
```python
null
```
```python
&& url ==
```
```python
null
```
```python
&& mapperClass ==
```
```python
null
```
```python
) {
```
```python
// resource属性不为空时，读取resource对应的XML资源，并解析它
```
```python
ErrorContext.instance().resource(resource);
          InputStream inputStream = Resources.getResourceAsStream(resource);
          XMLMapperBuilder mapperParser =
```
```python
new
```
```python
XMLMapperBuilder(inputStream, configuration, resource, configuration.getSqlFragments());
          mapperParser.parse();
        }
```
```python
else
```
```python
if
```
```python
(resource ==
```
```python
null
```
```python
&& url !=
```
```python
null
```
```python
&& mapperClass ==
```
```python
null
```
```python
) {
```
```python
// url属性不为空时，读取url对应的xml资源，并解析它
```
```python
ErrorContext.instance().resource(url);
          InputStream inputStream = Resources.getUrlAsStream(url);
          XMLMapperBuilder mapperParser =
```
```python
new
```
```python
XMLMapperBuilder(inputStream, configuration, url, configuration.getSqlFragments());
          mapperParser.parse();
        }
```
```python
else
```
```python
if
```
```python
(resource ==
```
```python
null
```
```python
&& url ==
```
```python
null
```
```python
&& mapperClass !=
```
```python
null
```
```python
) {
```
```python
// class属性不为空时，直接创建class对应的类对象，并添加到configuration中。
```
```python
// 仅使用mapperClass，而不使用XML mapper文件，一般是注解方式。我们一般不建议采用注解方式。
```
```python
// 后面会详细分析注解方式
```
```python
Class<?> mapperInterface = Resources.classForName(mapperClass);
          configuration.addMapper(mapperInterface);
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
BuilderException(
```
```python
"A mapper element may only specify a url, resource or class, but not more than one."
```
```python
);
        }
      }
    }
  }
}
```
mapperElement会读取并解析mappers元素，并添加到configuration实例的mapperRegistry变量中。子元素可以为package 或mapper 。我们知道，mapper配置可以采用XML方式或者注解方式。注解方式对应package子元素或mapper子元素下的class方式。XML方式对应mapper子元素下的resource或url方式。先来看注解方式，从上面的configuration.addMapper(mapperInterface);看起
```python
// configuration.addMapper()使用代理方式，调用到mapperRegistry中。
```
```python
// configuration包含了几乎所有的配置信息，是配置的门面，十分复杂。
```
```python
// 故采用外观模式和代理模式，将真正实现下沉到各个子系统中。这样通过分层可以解耦和降低复杂度。
```
```python
public
```
```python
<T>
```
```python
void
```
```python
addMapper(Class<T>
```
```python
type
```
```python
) {
```
```python
mapperRegistry.addMapper(
```
```python
type
```
```python
);
```
```python
}
```
```python
// 添加mapperClass到map中保存起来，并解析它的注解
```
```python
public
```
```python
<T>
```
```python
void
```
```python
addMapper(Class<T>
```
```python
type
```
```python
) {
```
```python
// 我们定义的mybatis的mapper类必须是一个接口
```
```python
if
```
```python
(
```
```python
type
```
```python
.
```
```python
isInterface
```
```python
(
```
```python
)) {
```
```python
// 已经添加过了的mapper就不再添加了
```
```python
if
```
```python
(hasMapper(
```
```python
type
```
```python
)) {
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
+
```
```python
type
```
```python
+ "
```
```python
is
```
```python
already
```
```python
known
```
```python
to
```
```python
the
```
```python
MapperRegistry
```
```python
.");
```
```python
}
```
```python
// 防止并发重入
```
```python
boolean loadCompleted =
```
```python
false
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
// 将新解析的mapper接口类添加到map中
```
```python
knownMappers.put(
```
```python
type
```
```python
,
```
```python
new
```
```python
MapperProxyFactory
```
```python
<
```
```python
T
```
```python
>(
```
```python
type
```
```python
));
```
```python
// 解析Mapper接口的各项注解，比如@Select，这是最关键的地方
```
```python
MapperAnnotationBuilder parser =
```
```python
new
```
```python
MapperAnnotationBuilder(config,
```
```python
type
```
```python
);
```
```python
parser.parse();
      loadCompleted =
```
```python
true
```
```python
;
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
(!loadCompleted) {
        knownMappers.remove(
```
```python
type
```
```python
);
```
```python
}
    }
  }
}
```
```python
// 遍历Mapper接口类的每个方法，解析其注解，生成MappedStatement，SqlSource和BoundSql三大主要对象
```
```python
public
```
```python
void
```
```python
parse() {
```
```python
// type为mapper接口类
```
```python
String resource =
```
```python
type
```
```python
.
```
```python
toString
```
```python
(
```
```python
);
```
```python
if
```
```python
(!configuration.isResourceLoaded(resource)) {
```
```python
// 根据mapper接口类名，获取XML资源文件，并加载。Spring-mybatis会使用到，仅使用mybatis时貌似不会用到
```
```python
loadXmlResource();
      configuration.addLoadedResource(resource);
      assistant.setCurrentNamespace(
```
```python
type
```
```python
.
```
```python
getName
```
```python
(
```
```python
));
```
```python
// 解析CacheNamespace注解，Mapper接口类注解
```
```python
parseCache();
```
```python
// 解析CacheNamespaceRef注解，Mapper接口类注解
```
```python
parseCacheRef();
```
```python
// 遍历mapper接口的各个方法，然后解析方法上的注解
```
```python
Method[] methods =
```
```python
type
```
```python
.
```
```python
getMethods
```
```python
(
```
```python
);
```
```python
for
```
```python
(Method method : methods) {
```
```python
try
```
```python
{
```
```python
// 解析mapper，此处是关键
```
```python
if
```
```python
(!method.isBridge()) {
            parseStatement(method);
          }
        } catch (IncompleteElementException e) {
          configuration.addIncompleteMethod(
```
```python
new
```
```python
MethodResolver(this, method));
        }
      }
    }
    parsePendingMethods();
  }
```
注解方式下，解析Mapper接口类时，我们先将Mapper接口添加到Map中保存起来，然后解析类注解@CacheNamespace @CacheNamespaceRef。之后就开始最关键的步骤，遍历Mapper的各个方法，解析方法上的注解。下面我们接着分析。
```python
void
```
```python
parseStatement(Method method) {
```
```python
// 反射获取入参类型和@Lang注解，为构建SqlSource做准备
```
```python
Class<?> parameterTypeClass = getParameterType(method);
  LanguageDriver languageDriver = getLanguageDriver(method);
```
```python
// 这一步十分关键，是创建SqlSource和其内的BoundSql对象的关键所在。
```
```python
// SqlSource是MappedStatement的一个属性，并用来提供BoundSql对象
```
```python
// BoundSql用来建立sql语句，它包含sql String，入参parameterObject和入参映射parameterMappings。它利用sql语句和入参，组装成最终的访问数据库的SQL语句，包括动态SQL。这是mybatis Mapper映射的最核心的地方。
```
```python
// 获取@Select @Insert @Update @Delete等注解，或者@SelectProvider @InsertProvider @UpdateProvider @DeleteProvider等注解
```
```python
// 然后解析这些注解，利用注解的value，也就是sql String解析生成SqlSource对象。后面详细分析
```
```python
SqlSource sqlSource = getSqlSourceFromAnnotations(method, parameterTypeClass, languageDriver);
```
```python
// 后面代码比较多，大多是解析方法上其他注解，如@Options @SelectKey @ResultMap。这里省略
```
```python
if
```
```python
(sqlSource !=
```
```python
null
```
```python
) {
    ...
  }
  ...
```
```python
// 利用上面解析得到的sqlSource等变量构建mappedStatement
```
```python
assistant.addMappedStatement(...);
```
下面来看sqlSource和BoundSql是如何创建的。
```python
// 根据mapper的方法上的注解，解析得到sqlSource对象。
```
```python
private
```
```python
SqlSource
```
```python
getSqlSourceFromAnnotations
```
```python
(Method method, Class<?> parameterType, LanguageDriver languageDriver) {
```
```python
try
```
```python
{
```
```python
// 解析获取 @Select @Insert @Delete @Update四者中的一个
```
```python
Class<? extends Annotation> sqlAnnotationType = getSqlAnnotationType(method);
```
```python
// 解析获取 @SelectProvider @InsertProvider @DeleteProvider @UpdateProvider四者中的一个
```
```python
Class<? extends Annotation> sqlProviderAnnotationType = getSqlProviderAnnotationType(method);
```
```python
if
```
```python
(sqlAnnotationType !=
```
```python
null
```
```python
) {
```
```python
if
```
```python
(sqlProviderAnnotationType !=
```
```python
null
```
```python
) {
```
```python
// 静态SQL注解和sqlProvider注解不可能同时存在
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
"You cannot supply both a static SQL and SqlProvider to method named "
```
```python
+ method.getName());
      }
```
```python
// 反射获取静态sql注解
```
```python
Annotation sqlAnnotation = method.getAnnotation(sqlAnnotationType);
```
```python
// 获取注解中的value,也就是sql String
```
```python
final
```
```python
String[] strings = (String[]) sqlAnnotation.getClass().getMethod(
```
```python
"value"
```
```python
).invoke(sqlAnnotation);
```
```python
// 利用sql String和入参来组装SqlSource对象，由这两者就可以生成最终访问数据库的SQL语句了
```
```python
// 后面详细分析
```
```python
return
```
```python
buildSqlSourceFromStrings(strings, parameterType, languageDriver);
    }
```
```python
else
```
```python
if
```
```python
(sqlProviderAnnotationType !=
```
```python
null
```
```python
) {
```
```python
// 反射获取SQLProvider注解，构建ProviderSqlSource类型的SqlSource。这一部分我们就不详细展开了，读者可自行分析
```
```python
Annotation sqlProviderAnnotation = method.getAnnotation(sqlProviderAnnotationType);
```
```python
return
```
```python
new
```
```python
ProviderSqlSource(assistant.getConfiguration(), sqlProviderAnnotation, type, method);
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
BuilderException(
```
```python
"Could not find value method on SQL annotation.  Cause: "
```
```python
+ e, e);
  }
}
```
```python
// 利用sql String和入参来组装SqlSource对象，由这两者就可以生成最终访问数据库的SQL语句了
```
```python
private
```
```python
SqlSource
```
```python
buildSqlSourceFromStrings
```
```python
(String[] strings, Class<?> parameterTypeClass, LanguageDriver languageDriver) {
```
```python
// 将sql String 拼装起来，构成一整个sql。此时sql中还没有注入入参
```
```python
final
```
```python
StringBuilder sql =
```
```python
new
```
```python
StringBuilder();
```
```python
for
```
```python
(String fragment : strings) {
      sql.append(fragment);
      sql.append(
```
```python
" "
```
```python
);
    }
```
```python
// 构建sqlSource，由具体LanguageDriver实现类完成，如XMLLanguageDriver。后面不详细分析了
```
```python
return
```
```python
languageDriver.createSqlSource(configuration, sql.toString().trim(), parameterTypeClass);
  }
```
### 2.3 由Configuration实例创建SqlSessionFactory单例对象
读取XNode中的XML配置信息，并设置到Configuration实例的相关变量中后，就可以由Configuration实例创建SqlSessionFactory单例对象了。这个过程比较简单，源码如下。
```python
// 利用Configuration实例创建DefaultSqlSessionFactory对象
```
```python
public
```
```python
SqlSessionFactory
```
```python
build
```
```python
(Configuration config) {
```
```python
return
```
```python
new
```
```python
DefaultSqlSessionFactory(config);
}
```
```python
// 默认的SqlSessionFactory实现类。
```
```python
public
```
```python
class
```
```python
DefaultSqlSessionFactory
```
```python
implements
```
```python
SqlSessionFactory
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
Configuration configuration;
```
```python
public
```
```python
DefaultSqlSessionFactory
```
```python
(Configuration configuration) {
```
```python
this
```
```python
.configuration = configuration;
  }
}
```
创建SqlSessionFactory对象的过程十分简单，利用Configuration实例构造DefaultSqlSessionFactory对象即可。DefaultSqlSessionFactory是SqlSessionFactory接口的默认实现类。
## 3 总结
SqlSessionFactory的创建，其实也就是mybatis的初始化过程。其中的重中之重就是Configuration实例的相关变量的设置。mybatis运行时会读取这些变量，来决定执行流程。这也是mybatis设计的精巧之处，代码配置化将设置与运行相分离，配置信息XML化也大大降低了Java代码复杂度，提高了可读性和可维护性。这些精巧的构思都是我们设计框架时可以吸取的宝贵经验。
### 相关文章
[mybatis源码分析1 - 框架](http://blog.csdn.net/u013510838/article/details/79055792)
[mybatis源码分析2 - SqlSessionFactory的创建](http://blog.csdn.net/u013510838/article/details/78995542)
[mybatis源码分析3 - sqlSession的创建](http://blog.csdn.net/u013510838/article/details/79011489)
[mybatis源码分析4 - sqlSession读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79016238)
[mybatis源码分析5 - mapper读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79023822)
[mybatis源码分析6 - mybatis-spring容器初始化](http://blog.csdn.net/u013510838/article/details/79053519)
[mybatis源码分析7 - mybatis-spring读写数据库全过程](http://blog.csdn.net/u013510838/article/details/79054007)

