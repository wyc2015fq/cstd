
# mybatis源码分析6 - mybatis-spring容器初始化 - 谢杨易的博客 - CSDN博客

2018年01月13日 20:23:36[谢杨易](https://me.csdn.net/u013510838)阅读数：602



## 1 引言
使用 MyBatis-Spring 模块，我们可以在Spring中使用mybatis，让Spring容器来管理sqlSessionFactory单例的创建。如以下代码
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
"sqlSessionFactory"
```
```python
class
```
```python
=
```
```python
"org.mybatis.spring.SqlSessionFactoryBean"
```
```python
>
```
```python
<!--指定数据源，不用再在mybatis的XML配置文件中指定environment了-->
```
```python
<
```
```python
property
```
```python
name
```
```python
=
```
```python
"dataSource"
```
```python
ref
```
```python
=
```
```python
"dataSource"
```
```python
/>
```
```python
<!--指定configuration对象，它是创建sqlSessionFactory的核心，包含mybatis几乎全部的配置信息-->
```
```python
<
```
```python
property
```
```python
name
```
```python
=
```
```python
"configuration"
```
```python
>
```
```python
<
```
```python
bean
```
```python
class
```
```python
=
```
```python
"org.apache.ibatis.session.Configuration"
```
```python
>
```
```python
<
```
```python
property
```
```python
name
```
```python
=
```
```python
"mapUnderscoreToCamelCase"
```
```python
value
```
```python
=
```
```python
"true"
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
</
```
```python
property
```
```python
>
```
```python
<!--数据库映射mapper文件的位置-->
```
```python
<
```
```python
property
```
```python
name
```
```python
=
```
```python
"mapperLocations"
```
```python
value
```
```python
=
```
```python
"classpath*:com/xxt/ibatis/dbcp/**/*.xml"
```
```python
/>
```
```python
<!--或指定指定sqlMapConfig总配置文件位置configLocation，建议采用这种mybatis配置单独放在另一个XML中的方式-->
```
```python
<
```
```python
property
```
```python
name
```
```python
=
```
```python
"configLocation"
```
```python
value
```
```python
=
```
```python
"classpath:sqlMapConfig.xml"
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
我们只需要指定两个属性即可，一是dataSource数据库源，二是configuration对象或configLocation配置文件所在位置。那么有这两个属性是如何创建sqlSessionFactory对象的呢，这一节我们详细分析。
## 2 sqlSessionFactory对象注入的流程
创建sqlSessionFactory bean时，指定的实现类是SqlSessionFactoryBean类，它是一个FactoryBean。我们知道，对于FactoryBean，Spring为我们创建的不是FactoryBean本身的对象，二是它的getObject()方法返回的对象。故我们从SqlSessionFactoryBean的getObject()方法来分析。
```python
// 工厂bean，它返回的不是FactoryBean本身，而是它的getObject方法返回的bean
```
```python
public
```
```python
SqlSessionFactory
```
```python
getObject
```
```python
()
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
(
```
```python
this
```
```python
.sqlSessionFactory ==
```
```python
null
```
```python
) {
    afterPropertiesSet();
  }
```
```python
// getObject最终返回的还是一个SqlSessionFactory对象
```
```python
return
```
```python
this
```
```python
.sqlSessionFactory;
}
```
上面是典型的单例模式，我们到afterPropertiesSet()方法中去看。
```python
public
```
```python
void
```
```python
afterPropertiesSet
```
```python
()
```
```python
throws
```
```python
Exception {
```
```python
// 各种报错
```
```python
notNull(dataSource,
```
```python
"Property 'dataSource' is required"
```
```python
);
  notNull(sqlSessionFactoryBuilder,
```
```python
"Property 'sqlSessionFactoryBuilder' is required"
```
```python
);
  state((configuration ==
```
```python
null
```
```python
&& configLocation ==
```
```python
null
```
```python
) || !(configuration !=
```
```python
null
```
```python
&& configLocation !=
```
```python
null
```
```python
),
```
```python
"Property 'configuration' and 'configLocation' can not specified with together"
```
```python
);
```
```python
// 创建sqlSessionFactory
```
```python
this
```
```python
.sqlSessionFactory = buildSqlSessionFactory();
}
```
afterPropertiesSet先做dataSource等属性值的校验，注入sqlSessionFactory的时候，必须传入dataSource属性的。然后调用buildSqlSessionFactory()方法来创建sqlSessionFactory，它是一个关键方法，我们详细分析。
```python
// 创建SqlSessionFactory实例
```
```python
protected
```
```python
SqlSessionFactory
```
```python
buildSqlSessionFactory
```
```python
()
```
```python
throws
```
```python
IOException {
```
```python
// 包含了几乎所有mybatis配置信息，创建sqlSessionFactory最重要的变量，之前分析mybatis初始化的时候讲到过
```
```python
Configuration configuration;
```
```python
// 先读取sqlSessionFactory bean注入时，用来设置mybatis配置信息Configuration的属性
```
```python
// 有configuration属性或者configLocation属性两种。
```
```python
XMLConfigBuilder xmlConfigBuilder =
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
(
```
```python
this
```
```python
.configuration !=
```
```python
null
```
```python
) {
```
```python
// 注入的是configuration属性时，它是一个bean
```
```python
configuration =
```
```python
this
```
```python
.configuration;
```
```python
// 合并configurationProperties变量到configuration的variables成员中。mybatis初始化的章节讲到过这个合并
```
```python
// configurationProperties包含的是一些动态化常量，比如数据库的username和password等信息
```
```python
// configurationProperties属性同样在sqlSessionFactory bean注入时设置进来
```
```python
if
```
```python
(configuration.getVariables() ==
```
```python
null
```
```python
) {
      configuration.setVariables(
```
```python
this
```
```python
.configurationProperties);
    }
```
```python
else
```
```python
if
```
```python
(
```
```python
this
```
```python
.configurationProperties !=
```
```python
null
```
```python
) {
      configuration.getVariables().putAll(
```
```python
this
```
```python
.configurationProperties);
    }
  }
```
```python
else
```
```python
if
```
```python
(
```
```python
this
```
```python
.configLocation !=
```
```python
null
```
```python
) {
```
```python
// 注入的是configLocation属性时，它是一个String，描述了mybatis xml配置文件的位置
```
```python
// 此时使用mybatis的配置文件来配置其他属性，利用配置文件生成Configuration对象
```
```python
// 和原生mybatis一样，也是先创建XMLConfigBuilder对象，然后利用它来解析mybatis配置文件，然后将配置文件中的属性设置到configuration的相关成员变量中去
```
```python
// 此处只是创建XMLConfigBuilder和configuration对象，还没有做解析
```
```python
xmlConfigBuilder =
```
```python
new
```
```python
XMLConfigBuilder(
```
```python
this
```
```python
.configLocation.getInputStream(),
```
```python
null
```
```python
,
```
```python
this
```
```python
.configurationProperties);
    configuration = xmlConfigBuilder.getConfiguration();
  }
```
```python
else
```
```python
{
```
```python
// configuration属性和configLocation属性都没有注入时，只能直接构造mybatis默认的Configuration对象了
```
```python
LOGGER.debug(() ->
```
```python
"Property 'configuration' or 'configLocation' not specified, using default MyBatis Configuration"
```
```python
);
    configuration =
```
```python
new
```
```python
Configuration();
```
```python
// 同样合并configurationProperties属性到configuration变量的variables变量中
```
```python
if
```
```python
(
```
```python
this
```
```python
.configurationProperties !=
```
```python
null
```
```python
) {
      configuration.setVariables(
```
```python
this
```
```python
.configurationProperties);
    }
  }
```
```python
// 注入了objectFactory属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(
```
```python
this
```
```python
.objectFactory !=
```
```python
null
```
```python
) {
    configuration.setObjectFactory(
```
```python
this
```
```python
.objectFactory);
  }
```
```python
// 注入了objectWrapperFactory属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(
```
```python
this
```
```python
.objectWrapperFactory !=
```
```python
null
```
```python
) {
    configuration.setObjectWrapperFactory(
```
```python
this
```
```python
.objectWrapperFactory);
  }
```
```python
// 注入了vfs属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(
```
```python
this
```
```python
.vfs !=
```
```python
null
```
```python
) {
    configuration.setVfsImpl(
```
```python
this
```
```python
.vfs);
  }
```
```python
// 注入了typeAliasesPackage属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(hasLength(
```
```python
this
```
```python
.typeAliasesPackage)) {
    String[] typeAliasPackageArray = tokenizeToStringArray(
```
```python
this
```
```python
.typeAliasesPackage,
        ConfigurableApplicationContext.CONFIG_LOCATION_DELIMITERS);
```
```python
for
```
```python
(String packageToScan : typeAliasPackageArray) {
      configuration.getTypeAliasRegistry().registerAliases(packageToScan,
              typeAliasesSuperType ==
```
```python
null
```
```python
? Object.class : typeAliasesSuperType);
      LOGGER.debug(() ->
```
```python
"Scanned package: '"
```
```python
+ packageToScan +
```
```python
"' for aliases"
```
```python
);
    }
  }
```
```python
// 注入了typeAliases属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(!isEmpty(
```
```python
this
```
```python
.typeAliases)) {
```
```python
for
```
```python
(Class<?> typeAlias :
```
```python
this
```
```python
.typeAliases) {
      configuration.getTypeAliasRegistry().registerAlias(typeAlias);
      LOGGER.debug(() ->
```
```python
"Registered type alias: '"
```
```python
+ typeAlias +
```
```python
"'"
```
```python
);
    }
  }
```
```python
// 注入了plugins属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(!isEmpty(
```
```python
this
```
```python
.plugins)) {
```
```python
for
```
```python
(Interceptor plugin :
```
```python
this
```
```python
.plugins) {
      configuration.addInterceptor(plugin);
      LOGGER.debug(() ->
```
```python
"Registered plugin: '"
```
```python
+ plugin +
```
```python
"'"
```
```python
);
    }
  }
```
```python
// 注入了typeHandlersPackage属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(hasLength(
```
```python
this
```
```python
.typeHandlersPackage)) {
    String[] typeHandlersPackageArray = tokenizeToStringArray(
```
```python
this
```
```python
.typeHandlersPackage,
        ConfigurableApplicationContext.CONFIG_LOCATION_DELIMITERS);
```
```python
for
```
```python
(String packageToScan : typeHandlersPackageArray) {
      configuration.getTypeHandlerRegistry().register(packageToScan);
      LOGGER.debug(() ->
```
```python
"Scanned package: '"
```
```python
+ packageToScan +
```
```python
"' for type handlers"
```
```python
);
    }
  }
```
```python
// 注入了typeHandlers属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(!isEmpty(
```
```python
this
```
```python
.typeHandlers)) {
```
```python
for
```
```python
(TypeHandler<?> typeHandler :
```
```python
this
```
```python
.typeHandlers) {
      configuration.getTypeHandlerRegistry().register(typeHandler);
      LOGGER.debug(() ->
```
```python
"Registered type handler: '"
```
```python
+ typeHandler +
```
```python
"'"
```
```python
);
    }
  }
```
```python
// 注入了databaseIdProvider属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(
```
```python
this
```
```python
.databaseIdProvider !=
```
```python
null
```
```python
) {
```
```python
//fix #64 set databaseId before parse mapper xmls
```
```python
try
```
```python
{
      configuration.setDatabaseId(
```
```python
this
```
```python
.databaseIdProvider.getDatabaseId(
```
```python
this
```
```python
.dataSource));
    }
```
```python
catch
```
```python
(SQLException e) {
```
```python
throw
```
```python
new
```
```python
NestedIOException(
```
```python
"Failed getting a databaseId"
```
```python
, e);
    }
  }
```
```python
// 注入了cache属性时，添加到configuration变量的cache map中
```
```python
if
```
```python
(
```
```python
this
```
```python
.cache !=
```
```python
null
```
```python
) {
    configuration.addCache(
```
```python
this
```
```python
.cache);
  }
```
```python
// 使用configLocation属性时，解析mybatis xml配置文件，和直接使用原生mybatis的new SqlSessionFactoryBuild().build()方式几乎相同
```
```python
if
```
```python
(xmlConfigBuilder !=
```
```python
null
```
```python
) {
```
```python
try
```
```python
{
```
```python
// 利用前面创建的xmlConfigBuilder来解析XML配置文件，并将解析后的键值对设置到configuration变量中
```
```python
xmlConfigBuilder.parse();
      LOGGER.debug(() ->
```
```python
"Parsed configuration file: '"
```
```python
+
```
```python
this
```
```python
.configLocation +
```
```python
"'"
```
```python
);
    }
```
```python
catch
```
```python
(Exception ex) {
```
```python
throw
```
```python
new
```
```python
NestedIOException(
```
```python
"Failed to parse config resource: "
```
```python
+
```
```python
this
```
```python
.configLocation, ex);
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
// 创建transactionFactory，用来创建transaction事务，Spring使用AOP来创建事务
```
```python
if
```
```python
(
```
```python
this
```
```python
.transactionFactory ==
```
```python
null
```
```python
) {
```
```python
this
```
```python
.transactionFactory =
```
```python
new
```
```python
SpringManagedTransactionFactory();
  }
```
```python
// 设置configuration的environment变量，
```
```python
// 采用Spring注入方式时，直接指定了sqlSessionFactory下的dataSource数据库源，一般不需要在mybaits配置文件中设置environments了
```
```python
configuration.setEnvironment(
```
```python
new
```
```python
Environment(
```
```python
this
```
```python
.environment,
```
```python
this
```
```python
.transactionFactory,
```
```python
this
```
```python
.dataSource));
```
```python
// 注入了mapperLocations属性时，一般不建议在sqlSessionFactory中注入，而是放到mybatis配置文件中。
```
```python
if
```
```python
(!isEmpty(
```
```python
this
```
```python
.mapperLocations)) {
```
```python
for
```
```python
(Resource mapperLocation :
```
```python
this
```
```python
.mapperLocations) {
```
```python
if
```
```python
(mapperLocation ==
```
```python
null
```
```python
) {
```
```python
continue
```
```python
;
      }
```
```python
try
```
```python
{
```
```python
// 读取mapper配置文件，并解析
```
```python
XMLMapperBuilder xmlMapperBuilder =
```
```python
new
```
```python
XMLMapperBuilder(mapperLocation.getInputStream(),
            configuration, mapperLocation.toString(), configuration.getSqlFragments());
        xmlMapperBuilder.parse();
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
NestedIOException(
```
```python
"Failed to parse mapping resource: '"
```
```python
+ mapperLocation +
```
```python
"'"
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
      }
      LOGGER.debug(() ->
```
```python
"Parsed mapper file: '"
```
```python
+ mapperLocation +
```
```python
"'"
```
```python
);
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
"Property 'mapperLocations' was not specified or no matching resources found"
```
```python
);
  }
```
```python
// configuration变量创建并初始化好之后，就可以创建sqlSessionFactory对象了
```
```python
// sqlSessionFactoryBuilder的build创建DefaultSqlSessionFactory对象，默认的SqlSessionFactory
```
```python
// 这个过程之前讲解mybatis初始化的章节时，讲过了的
```
```python
return
```
```python
this
```
```python
.sqlSessionFactoryBuilder.build(configuration);
}
```
这个方法比较长，详细内容读者可以逐行看上面代码和注释，注释应该已经十分详尽了。我们总结下这个方法的流程。
先读取mybatis配置信息，它通过sqlSessionFactory注入时，传入的configuration对象或者configLocation String来分析配置信息。
1）传入的是configuration属性时，合并configurationProperties属性到configuration对象中去即可。
2）传入的是configLocation属性时，它是一个String，描述了mybatis xml配置文件的位置。先创建XMLConfigBuilder对象和configuration对象，后面几步会解析mybatis配置文件，然后将配置文件中的属性设置到configuration的相关成员变量中去（这个过程和原生mybatis相同）
3）configuration属性和configLocation属性都没有注入时，只能直接构造mybatis默认的Configuration对象了
再读取创建sqlSessionFactory bean时，传入的其他属性，如objectFactory objectWrapperFactory vfs typeAliasesPackage typeAliases plugins typeHandlersPackage typeHandlers databaseIdProvider等。如果我们使用配置文件位置信息configLocation来解析mybatis配置信息的话，这些属性均不需要传入。如果采用configuration对象的方式，或者configLocation和configuration都没有传入的话，则需要这些属性了。一般建议采用configLocation的方式，将mybatis的配置信息和Spring配置信息相分离。
使用configLocation属性时，解析mybatis xml配置文件，和直接使用原生mybatis的new SqlSessionFactoryBuild().build()方式几乎相同。
创建transactionFactory，用来创建transaction事务，Spring使用AOP来创建事务
设置configuration的environment变量，利用传入的dataSource属性
读取创建sqlSessionFactory bean时，传入的mapperLocations属性。如果采用configLocation指定mybatis配置文件位置的方式，则一般不需要在Spring中配置mapperLocations
sqlSessionFactoryBuilder的build创建DefaultSqlSessionFactory对象
这个方法很关键，且流程很长。大家最重要的是要知道，创建sqlSessionFactory时指定mybatis配置信息，有三种方式。一是直接configuration对象，包含了配置信息各项参数。二是configLocation字符串，指定了配置文件的位置。三是configuration和configLocation均没有配置，完全依靠Spring配置文件中指定objectFactory typeHandlers 等属性。明白了这一点，上面的代码就会比较清晰了。
为了将Spring配置信息和mybatis配置信息相分离，从而让各个XML各司其职，也避免Spring配置文件过于膨胀，我们一般采用configLocation的方式。这种方式和原生mybatis创建sqlSessionFactory的过程极其类似，都是通过XMLConfigBuilder解析XML配置文件，并将解析到的键值对设置到Configuration对象的相关变量中去。这一过程我们在前面讲解mybatis初始化的章节中已经详细介绍了，故此处不详细讲解了。最后我们看sqlSessionFactoryBuilder.build()方法。
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
这个方法十分简单，构造sqlSessionFactory的默认实现类DefaultSqlSessionFactory，并传入前面创建并解析好的configuration对象即可。configuration包含了几乎所有的mybatis配置信息，十分重要。
## 3 总结
Spring容器中sqlSessionFactory的创建其实是十分简单的，特别是采用了configLocation方式的时候。创建过程基本是依赖原生mybatis的执行流程的。从这儿也可以看出代码分层有利于代码适配。这也是我们平时自己设计框架时要要注意的地方，尽量让层次分明，模块解耦，这样才能简易的适配不同的环境，从而提高可移植性。
下一节我们分析mybatis-spring中，sqlSession是如何操作数据库的
### 相关文章
[mybatis源码分析1 - 框架](http://blog.csdn.net/u013510838/article/details/79055792)
[mybatis源码分析2 - SqlSessionFactory的创建](http://blog.csdn.net/u013510838/article/details/78995542)
[mybatis源码分析3 - sqlSession的创建](http://blog.csdn.net/u013510838/article/details/79011489)
[mybatis源码分析4 - sqlSession读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79016238)
[mybatis源码分析5 - mapper读写数据库完全解析](http://blog.csdn.net/u013510838/article/details/79023822)
[mybatis源码分析6 - mybatis-spring容器初始化](http://blog.csdn.net/u013510838/article/details/79053519)
[mybatis源码分析7 - mybatis-spring读写数据库全过程](http://blog.csdn.net/u013510838/article/details/79054007)

