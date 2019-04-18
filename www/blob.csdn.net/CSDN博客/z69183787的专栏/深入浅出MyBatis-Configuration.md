# 深入浅出MyBatis-Configuration - z69183787的专栏 - CSDN博客
2015年10月06日 19:56:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：587
个人分类：[对象映射框架-Ibatis/Mybatis](https://blog.csdn.net/z69183787/article/category/2261201)
# [http://blog.csdn.net/hupanfeng/article/details/9080545](http://blog.csdn.net/hupanfeng/article/details/9080545)
# Configuration是干嘛的
Configuration就像是Mybatis的总管，Mybatis的所有配置信息都存放在这里，此外，它还提供了设置这些配置信息的方法。Configuration可以从配置文件里获取属性值，也可以通过程序直接设置。Configuration里可供配置的属性有：
1.       properties属性
1)       配置示例
**[html]**[view
 plain](http://blog.csdn.net/hupanfeng/article/details/9080545#)[copy](http://blog.csdn.net/hupanfeng/article/details/9080545#)
- <propertiesresource="org/mybatis/example/config.properties">
- <propertyname="username"value="dev_user"/>
- <propertyname="password"value="F2Fa3!33TYyg"/>
- </properties>
2)       用途
properties里配置的属性将被存放在Configuration的variables变量里，供Mybatis使用。此外在配置文件中，也可以使用这些属性，用来对配置项实现动态配置。比如：
**[html]**[view
 plain](http://blog.csdn.net/hupanfeng/article/details/9080545#)[copy](http://blog.csdn.net/hupanfeng/article/details/9080545#)
- <dataSourcetype="POOLED">
- <propertyname="driver"value="${driver}"/>
- <propertyname="url"value="${url}"/>
- <propertyname="username"value="${username}"/>
- <propertyname="password"value="${password}"/>
- </dataSource>
3)       注意事项
resource里也可指定属性值，并且resource里属性值的优先级高于property子节点里配置的值，也就是说resource的同名属性将会覆盖于property子节点里的值。
2.        settings设置
1)       配置示例
**[html]**[view
 plain](http://blog.csdn.net/hupanfeng/article/details/9080545#)[copy](http://blog.csdn.net/hupanfeng/article/details/9080545#)
- <settings>
- <settingname="cacheEnabled"value="true"/>
- <settingname="lazyLoadingEnabled"value="true"/>
- <settingname="multipleResultSetsEnabled"value="true"/>
- <settingname="useColumnLabel"value="true"/>
- <settingname="useGeneratedKeys"value="false"/>
- <settingname="enhancementEnabled"value="false"/>
- <settingname="defaultExecutorType"value="SIMPLE"/>
- <settingname="defaultStatementTimeout"value="25000"/>
- </settings>
2)       用途
setting节点里配置的值会直接改写Configuration对应的变量值，这些变量描述的是Mybatis的全局运行方式，如果对这些属性的含义不熟悉的话建议不要配置，使用默认值即可。下面这个表格描述了各个配置项的含义和默认值：
|设置参数|描述|有效值|默认值|
|----|----|----|----|
|cacheEnabled|这个配置使全局的映射器启用或禁用缓存。|true | false|true|
|lazyLoadingEnabled|全局启用或禁用延迟加载。当禁用时，所有关联对象都会即时加载。|true | false|true|
|aggressiveLazyLoading|当启用时，有延迟加载属性的对象在被调用时将会完全加载任意属性。否则，每种属性将会按需要加载。|true | false|true|
|multipleResultSetsEnabled|允许或不允许多种结果集从一个单独的语句中返回（需要适合的驱动）。|true | false|true|
|useColumnLabel|使用列标签代替列名。不同的驱动在这方便表现不同。参考驱动文档或充分测试两种方法来决定所使用的驱动。|true | false|true|
|useGeneratedKeys|允许JDBC支持生成的键。需要适合的驱动。如果设置为true则这个设置强制生成的键被使用，尽管一些驱动拒绝兼容但仍然有效（比如Derby）。|true | false|false|
|autoMappingBehavior|指定MyBatis如何自动映射列到字段/属性。PARTIAL只会自动映射简单，没有嵌套的结果。FULL会自动映射任意复杂的结果（嵌套的或其他情况）。|NONE,PARTIAL,FULL|PARTIAL|
3)       注意事项
谨慎修改这些配置值。
3.       typeAliases类型命名
1)       配置示例
**[html]**[view
 plain](http://blog.csdn.net/hupanfeng/article/details/9080545#)[copy](http://blog.csdn.net/hupanfeng/article/details/9080545#)
- <typeAliases>
- <typeAliasalias="Author"type="domain.blog.Author"/>
- <typeAliasalias="Blog"type="domain.blog.Blog"/>
- <typeAliasalias="Comment"type="domain.blog.Comment"/>
- <typeAliasalias="Post"type="domain.blog.Post"/>
- <typeAliasalias="Section"type="domain.blog.Section"/>
- <typeAliasalias="Tag"type="domain.blog.Tag"/>
- </typeAliases>
2)       用途
别名是为Java类型命名一个短的名字。它只用在XML配置文件里，用来减少类完全限定名的多余部分。例如在配置文件里需要使用domain.blog.Author的地方可以简单的使用Author替换。
3)       注意事项
Mybatis里有一些默认的别名，不要覆盖这些别名：
|别名|映射的类型|
|----|----|
|_byte|byte|
|_long|long|
|_short|short|
|_int|int|
|_integer|int|
|_double|double|
|_float|float|
|_boolean|boolean|
|string|String|
|byte|Byte|
|long|Long|
|short|Short|
|int|Integer|
|integer|Integer|
|double|Double|
|float|Float|
|boolean|Boolean|
|date|Date|
|decimal|BigDecimal|
|bigdecimal|BigDecimal|
|object|Object|
|map|Map|
|hashmap|HashMap|
|list|List|
|arraylist|ArrayList|
|collection|Collection|
|iterator|Iterator|
4.       typeHandlers类型处理器
1)       配置示例
**[html]**[view
 plain](http://blog.csdn.net/hupanfeng/article/details/9080545#)[copy](http://blog.csdn.net/hupanfeng/article/details/9080545#)
- <typeHandlers>
- <typeHandlerjavaType="String"jdbcType="VARCHAR"handler="org.mybatis.example.ExampleTypeHandler"/>
- </typeHandlers>
2)       用途
无论是MyBatis在预处理语句中设置一个参数，还是从结果集中取出一个值时，类型处理器被用来将获取的值以合适的方式转换成Java类型。
3)       注意事项
Mybatis已经有了若干个类型处理器了，编写自己的类型处理器时勿重新发明轮子。
|类型处理器|**Java**类型|**JDBC**类型|
|----|----|----|
|BooleanTypeHandler|Boolean，boolean|任何兼容的布尔值|
|ByteTypeHandler|Byte，byte|任何兼容的数字或字节类型|
|ShortTypeHandler|Short，short|任何兼容的数字或短整型|
|IntegerTypeHandler|Integer，int|任何兼容的数字和整型|
|LongTypeHandler|Long，long|任何兼容的数字或长整型|
|FloatTypeHandler|Float，float|任何兼容的数字或单精度浮点型|
|DoubleTypeHandler|Double，double|任何兼容的数字或双精度浮点型|
|BigDecimalTypeHandler|BigDecimal|任何兼容的数字或十进制小数类型|
|StringTypeHandler|String|CHAR和VARCHAR类型|
|ClobTypeHandler|String|CLOB和LONGVARCHAR类型|
|NStringTypeHandler|String|NVARCHAR和NCHAR类型|
|NClobTypeHandler|String|NCLOB类型|
|ByteArrayTypeHandler|byte[]|任何兼容的字节流类型|
|BlobTypeHandler|byte[]|BLOB和LONGVARBINARY类型|
|DateTypeHandler|Date（java.util）|TIMESTAMP类型|
|DateOnlyTypeHandler|Date（java.util）|DATE类型|
|TimeOnlyTypeHandler|Date（java.util）|TIME类型|
|SqlTimestampTypeHandler|Timestamp（java.sql）|TIMESTAMP类型|
|SqlDateTypeHandler|Date（java.sql）|DATE类型|
|SqlTimeTypeHandler|Time（java.sql）|TIME类型|
|ObjectTypeHandler|Any|其他或未指定类型|
|EnumTypeHandler|Enumeration类型|VARCHAR-任何兼容的字符串类型，作为代码存储（而不是索引）|
5.       objectFactory对象工厂
略（不常用）
6.       plugins插件
1)       配置示例
**[html]**[view
 plain](http://blog.csdn.net/hupanfeng/article/details/9080545#)[copy](http://blog.csdn.net/hupanfeng/article/details/9080545#)
- <plugins>
- <plugininterceptor="org.mybatis.example.ExamplePlugin">
- <propertyname="someProperty"value="100"/>
- </plugin>
- </plugins>
2)       用途
插件可以改变某些类的执行， MyBatis允许被插件来拦截的类有：
**[java]**[view
 plain](http://blog.csdn.net/hupanfeng/article/details/9080545#)[copy](http://blog.csdn.net/hupanfeng/article/details/9080545#)
- Executor (update, query, flushStatements, commit, rollback, getTransaction, close, isClosed)   
- ParameterHandler (getParameterObject, setParameters)   
- ResultSetHandler (handleResultSets, handleOutputParameters)   
- StatementHandler (prepare, parameterize, batch, update, query)  
3)       注意事项
插件可以改变MyBatis底层的行为，因此功能非常强大。但在对MyBatis深入理解前不要急着编写自己的插件，因为若插件编写不当，将导致整个MyBatis都不能正常工作。**本系列的后面会有专门的章节介绍插件机制以及两个有用的插件，欢迎持续关注。**
7.       environments环境
environments里可以配置多个environment，每个environment对应一个数据库环境。一个完整的定义如下：
**[html]**[view
 plain](http://blog.csdn.net/hupanfeng/article/details/9080545#)[copy](http://blog.csdn.net/hupanfeng/article/details/9080545#)
- <environmentsdefault="development">
- <environmentid="development">
- <transactionManagertype="JDBC">
- </transactionManager>
- <dataSourcetype="POOLED">
- <propertyname="driver"value="${driver}"/>
- <propertyname="url"value="${url}"/>
- <propertyname="username"value="${username}"/>
- <propertyname="password"value="${password}"/>
- </dataSource>
- </environment>
- </environments>
environments里的default属性表示默认的数据库环境，与某个environment的id相对应。
1)       environment环境变量
environment通过id属性与其他数据库环境区别。它有两个子节点：
a)       transactionManager事务管理器
在MyBatis中有两种事务管理器类型（也就是type=”[JDBC|MANAGED]”）：
l **JDBC**–这个配置直接简单使用了JDBC的提交和回滚设置。它依赖于从数据源得到的连接来管理事务范围。
l **MANAGED**–这个配置几乎没做什么。它从来不提交或回滚一个连接。而它会让容器来管理事务的整个生命周期（比如Spring或JEE应用服务器的上下文）
b)       dataSource数据源
在MyBatis中有三种数据源类型（也就是type=”[UNPOOLED | POOLED| JNDI]”）：
l **UNPOOLED –**这个数据源的实现是每次被请求时简单打开和关闭连接，需要配置的属性：
driver – 这是JDBC驱动的Java类的完全限定名
url – 这是数据库的JDBC URL地址。
username – 登录数据库的用户名。
password – 登录数据库的密码。
defaultTransactionIsolationLevel – 默认的连接事务隔离级别。
l **POOLED –**mybatis实现的简单的数据库连接池类型，它使得数据库连接可被复用，不必在每次请求时都去创建一个物理的连接。
l **JNDI** – 通过jndi从tomcat之类的容器里获取数据源。
8.       mapper映射器
1)       配置示例
**[html]**[view
 plain](http://blog.csdn.net/hupanfeng/article/details/9080545#)[copy](http://blog.csdn.net/hupanfeng/article/details/9080545#)
- <mappers>
- <mapperresource="org/mybatis/builder/AuthorMapper.xml"/>
- <mapperresource="org/mybatis/builder/BlogMapper.xml"/>
- <mapperresource="org/mybatis/builder/PostMapper.xml"/>
- </mappers>
2)       用途
Mappers用于告诉Mybatis去哪里寻找sql映射文件。sql映射文件是Mybatis最灵活的地方。sql映射文件在后续的章节里会进一步详细描述，欢迎持续关注。
# 如何构建configuration
前面介绍了Configuration的各个属性以及在文件里配置他们的方法。这一节介绍如何从文件里解析这些配置项以及简化配置的方法。Configuration在SqlSessionFactoryBuilder创建SqlSessionFactory时创建，通过XMLConfigBuilder的parse方法创建。下面一个个的介绍：
1.       properties属性
通过XMLConfigBuilder的propertiesElement方法解析，解析后的结果存放在Configuration的variables变量里。解析顺序，先解析子节点里的属性值，再解析resource属性指定的配置文件里的值。后者会覆盖前者的值。
2.        settings设置
通过XMLConfigBuilder的settingsElement方法解析，解析前校验属性是否是可配置的，只要有一个不可配置，整个mybatis就会异常退出，所以配置这些属性务必小心。
3.       typeAliases类型命名
通过XMLConfigBuilder的typeAliasesElement方法解析,在该方法内部调用TypeAliasRegistry的registerAlias方法完成注册，并将注册的别名存入本地缓存中。XMLConfigBuilder调用的registerAlias方法并没有什么特别的地方，但TypeAliasRegistry却提供了批量注册别名的方法，该方法只需要一个包名参数。该方法会在指定的包路径下扫描可注册的类（接口和内部类除外），并以类的简单名为key完整名为value注册别名。**这个方法极大的简化了配置，后续章节将要介绍的spring与mybatis的整合模块里就使用了这个方法使得配置量大大减少。**
4.       typeHandlers类型处理器
通过XMLConfigBuilder的typeHandlerElement方法解析,在该方法内部调用TypeHandlerRegistry的register方法完成注册，并将注册的类型处理器存入本地缓存中。同typeAliases一样，TypeHandlerRegistry也提供了批量注册的方法，该方法同样只需要一个包名参数。该方法会在指定的包路径下扫描可注册的类（接口、抽象类和内部类除外），不过只注册那些配置了MappedTypes注解的类。**这个方法极大的简化了配置，后续章节将要介绍的spring与mybatis的整合模块里就使用了这个方法使得配置量大大减少。**
5.       objectFactory对象工厂
略（不常用）
6.       plugins插件
通过XMLConfigBuilder的pluginElement方法解析,在该方法内部实例化插件对象后存入Configuration的interceptorChain变量里。
7.       environments环境
通过XMLConfigBuilder的environmentsElement方法解析，在该方法内，先判断默认是否指定了环境ID，没有的话就使用默认的环境ID吗，然后在各个环境里取ID对应的项。也就是说一个Configuration只会保存一个数据库环境，如果要配置多数据库环境的话需要创建多个Configuration对象。在改方法内先解析事务工厂、再解析数据源、然后再解析数据库环境，在解析数据库环境的过程中会访问一次数据库，以取得数据库类型信息。
8.        mapper映射器
通过XMLConfigBuilder的mapperElement方法解析，在该方法内部通过调用XMLMapperBuilder的parse方法完成。**mapper的解析是mybatis的核心功能，涉及的流程较复杂，在下一章将专门分析，敬请期待。**
