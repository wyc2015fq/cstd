# Github Mybatis深入学习之XML配置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月08日 16:50:44[boonya](https://me.csdn.net/boonya)阅读数：4583








      MyBatis的配置的灵活性也是其一大亮点，这样适合不同的配置需求，显得其有包涵有涵养。所以，这也是许多公司选择它作为持久化框架的原因。

原文地址：[http://mybatis.github.io/mybatis-3/configuration.html](http://mybatis.github.io/mybatis-3/configuration.html)

MyBatis包含了设置和属性来影响MyBatis的行为，其高级文档结构如下：


- **configuration**- **[properties](http://mybatis.github.io/mybatis-3/configuration.html#properties)（属性）**
- **[settings](http://mybatis.github.io/mybatis-3/configuration.html#settings)（配置）**
- **[typeAliases](http://mybatis.github.io/mybatis-3/configuration.html#typeAliases)（类型别名）**
- **[typeHandlers](http://mybatis.github.io/mybatis-3/configuration.html#typeHandlers)（类型处理器）**
- **[objectFactory](http://mybatis.github.io/mybatis-3/configuration.html#objectFactory)（对象工厂）**
- **[plugins](http://mybatis.github.io/mybatis-3/configuration.html#plugins)（插件）**
- **[environments](http://mybatis.github.io/mybatis-3/configuration.html#environments)（环境集合属性对象）**- **environment（环境子属性对象）**- **transactionManager（事务管理）**
- **dataSource（数据源）**


- **[databaseIdProvider](http://mybatis.github.io/mybatis-3/configuration.html#databaseIdProvider)（数据库ID提供者）**
- **[mappers](http://mybatis.github.io/mybatis-3/configuration.html#mappers)（映射器）**

下面就来一一作一介绍：


**1、属性（properties）**

       属性都是可以配置在一个典型的Java属性文件实例，通过属性元素的子元素或通过外部化，拥有可替换的特点。例如：




```
<properties resource="org/mybatis/example/config.properties">
  <property name="username" value="dev_user"/>
  <property name="password" value="F2Fa3!33TYyg"/>
</properties>
```
然后，可以使用这些属性可以在整个配置文件值来替换，需要进行动态配置。例如：
<dataSourcetype="POOLED"><propertyname="driver"value="${driver}"/><propertyname="url"value="${url}"/><propertyname="username"value="${username}"/><propertyname="password"value="${password}"/></dataSource>在属性元素设置的值，在这个例子中的用户名和密码将被替换。 config.properties文件中包含的值将被替换的驱动程序和URL属性。这提供了很多配置的选项。

属性也可以被传递到SqlSessionBuilder.build（）方法。例如：





```java
SqlSessionFactory factory = sqlSessionFactoryBuilder.build(reader, props);

// ... or ...

SqlSessionFactory factory = sqlSessionFactoryBuilder.build(reader, environment, props);
```
如果一个属性存在于一个以上的这些地方的MyBatis的加载按以下顺序（属性文件配置加载顺序）：


（1）、在body内优先读取指定的属性的属性元素。

（2）、从classpath资源或URL属性的属性元素加载的属性第二被读取，并覆盖已经指定的任何重复的属性，

（3）、传递的属性作为方法参数最后被读取，并覆盖任何重复的特性，可能已加载从属性的身体和资源/ url属性。


因此，最高优先级的属性是那些作为方法参数，其次是资源/ url属性，最后指定的属性的属性元素在body内传递。

**2、配置（setting）**

        配置是极其重要的调整，以及在运行时修改的MyBatis的行为方式。下表描述的是它们的设置、含义和默认值。




|Setting(设置)|Description（描述）|Valid Values(验证值组)|Default(默认值)|
|----|----|----|----|
|cacheEnabled|在全局范围内启用或禁用缓存配置任何映射器在此配置下。|true | false|true|
|lazyLoadingEnabled|在全局范围内启用或禁用延迟加载。禁用时，所有协会将热加载。|true | false|true|
|aggressiveLazyLoading|启用时，有延迟加载属性的对象将被完全加载后调用懒惰的任何属性。否则，每一个属性是按需加载。|true | false|true|
|multipleResultSetsEnabled|允许或不允许从一个单独的语句（需要兼容的驱动程序）要返回多个结果集。|true | false|true|
|useColumnLabel|使用列标签，而不是列名。在这方面，不同的驱动有不同的行为。参考驱动文档或测试两种方法来决定你的驱动程序的行为如何。|true | false|true|
|useGeneratedKeys|允许JDBC支持生成的密钥。兼容的驱动程序是必需的。此设置强制生成的键被使用，如果设置为true，一些驱动会不兼容性，但仍然可以工作。|true | false|False|
|autoMappingBehavior|指定MyBatis的应如何自动映射列到字段/属性。NONE自动映射。 PARTIAL只会自动映射结果没有嵌套结果映射定义里面。 FULL会自动映射的结果映射任何复杂的（包含嵌套或其他）。|NONE, PARTIAL, FULL|PARTIAL|
|defaultExecutorType|配置默认执行人。SIMPLE执行人确实没有什么特别的。 REUSE执行器重用准备好的语句。 BATCH执行器重用语句和批处理更新。|SIMPLE REUSE BATCH|SIMPLE|
|defaultStatementTimeout|设置驱动程序等待一个数据库响应的秒数。|Any positive integer|Not Set (null)|
|safeRowBoundsEnabled|允许使用嵌套的语句RowBounds。|true | false|False|
|mapUnderscoreToCamelCase|从经典的数据库列名A_COLUMN启用自动映射到骆驼标识的经典的Java属性名aColumn。|true | false|False|
|localCacheScope|MyBatis的使用本地缓存，以防止循环引用，并加快反复嵌套查询。默认情况下（SESSION）会话期间执行的所有查询缓存。如果localCacheScope=STATMENT本地会话将被用于语句的执行，只是没有将数据共享之间的两个不同的调用相同的SqlSession。|SESSION | STATEMENT|SESSION|
|jdbcTypeForNull|指定为空值时，没有特定的JDBC类型的参数的JDBC类型。有些驱动需要指定列的JDBC类型，但其他像NULL，VARCHAR或OTHER的工作与通用值。|JdbcType enumeration. Most common are: NULL, VARCHAR and OTHER|OTHER|
|lazyLoadTriggerMethods|指定触发延迟加载的对象的方法。|A method name list separated by commas|equals,clone,hashCode,toString|
|defaultScriptingLanguage|指定所使用的语言默认为动态SQL生成。|A type alias or fully qualified class name.|org.apache.ibatis.scripting.xmltags.XMLDynamicLanguageDriver|
|callSettersOnNulls|指定如果setter方法​​或地图的put方法时，将调用检索到的值是null。它是有用的，当你依靠Map.keySet（）或null初始化。注意原语（如整型，布尔等）不会被设置为null。|true | false|false|
|logPrefix|指定的前缀字串，MyBatis将会增加记录器的名称。|Any String|Not set|
|logImpl|指定MyBatis的日志实现使用。如果此设置是不存在的记录的实施将自动查找。|SLF4J | LOG4J | LOG4J2 | JDK_LOGGING | COMMONS_LOGGING | STDOUT_LOGGING | NO_LOGGING|Not set|
|proxyFactory|指定代理工具，MyBatis将会使用创建懒加载能力的对象。|CGLIB | JAVASSIST|CGLIB|

下面是一个完整的setting元素的设置示例：



```
<settings>
  <setting name="cacheEnabled" value="true"/>
  <setting name="lazyLoadingEnabled" value="true"/>
  <setting name="multipleResultSetsEnabled" value="true"/>
  <setting name="useColumnLabel" value="true"/>
  <setting name="useGeneratedKeys" value="false"/>
  <setting name="autoMappingBehavior" value="PARTIAL"/>
  <setting name="defaultExecutorType" value="SIMPLE"/>
  <setting name="defaultStatementTimeout" value="25"/>
  <setting name="safeRowBoundsEnabled" value="false"/>
  <setting name="mapUnderscoreToCamelCase" value="false"/>
  <setting name="localCacheScope" value="SESSION"/>
  <setting name="jdbcTypeForNull" value="OTHER"/>
  <setting name="lazyLoadTriggerMethods" value="equals,clone,hashCode,toString"/>
</settings>
```



**3、类型别名**

    A型别名是一个用较短的名称代表Java类型。这是唯一的XML配置有关，只是存在，以减少多余的类完全限定名。例如：




```
<typeAliases>
  <typeAlias alias="Author" type="domain.blog.Author"/>
  <typeAlias alias="Blog" type="domain.blog.Blog"/>
  <typeAlias alias="Comment" type="domain.blog.Comment"/>
  <typeAlias alias="Post" type="domain.blog.Post"/>
  <typeAlias alias="Section" type="domain.blog.Section"/>
  <typeAlias alias="Tag" type="domain.blog.Tag"/>
</typeAliases>
```
根据这样的结构，Blog现在可以在任何地方使用，domain.blog.Blog可能。您也可以指定包MyBatis将会搜索bean。例如：





```
<typeAliases>
  <package name="domain.blog"/>
</typeAliases>
```
每个bean在domain.blog发现，如果没有找到注解，将注册使用小写开头的非限定类的bean的名称作为别名。临屋区将为domain.blog.Author被登记author。 如果@Alias注解的发现，它的值将被用作别名。请看下面的例子：

```java
@Alias("author")
public class Author {
    ...
}
```
有许多内置类型，对于普通的Java类型的别名。他们都是大小写不敏感，注意由于重载的名字需要特殊处理原语。
|Alias|Mapped Type|
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



**4、类型处理器**

        每当MyBatis的一个PreparedStatement对象，或设置一个参数，从ResultSet检索值（一个类型处理器的用于检索的价值），来匹配Java类型。下面的表格描述了默认的类型处理器。



|Type Handler|Java Types|JDBC Types|
|----|----|----|
|BooleanTypeHandler|java.lang.Boolean, boolean|Any compatible BOOLEAN|
|ByteTypeHandler|java.lang.Byte, byte|Any compatible NUMERIC or BYTE|
|ShortTypeHandler|java.lang.Short, short|Any compatible NUMERIC or SHORT INTEGER|
|IntegerTypeHandler|java.lang.Integer, int|Any compatible NUMERIC or INTEGER|
|LongTypeHandler|java.lang.Long, long|Any compatible NUMERIC or LONG INTEGER|
|FloatTypeHandler|java.lang.Float, float|Any compatible NUMERIC or FLOAT|
|DoubleTypeHandler|java.lang.Double, double|Any compatible NUMERIC or DOUBLE|
|BigDecimalTypeHandler|java.math.BigDecimal|Any compatible NUMERIC or DECIMAL|
|StringTypeHandler|java.lang.String|CHAR, VARCHAR|
|ClobTypeHandler|java.lang.String|CLOB, LONGVARCHAR|
|NStringTypeHandler|java.lang.String|NVARCHAR, NCHAR|
|NClobTypeHandler|java.lang.String|NCLOB|
|ByteArrayTypeHandler|byte[]|Any compatible byte stream type|
|BlobTypeHandler|byte[]|BLOB, LONGVARBINARY|
|DateTypeHandler|java.util.Date|TIMESTAMP|
|DateOnlyTypeHandler|java.util.Date|DATE|
|TimeOnlyTypeHandler|java.util.Date|TIME|
|SqlTimestampTypeHandler|java.sql.Timestamp|TIMESTAMP|
|SqlDateTypeHandler|java.sql.Date|DATE|
|SqlTimeTypeHandler|java.sql.Time|TIME|
|ObjectTypeHandler|Any|OTHER, or unspecified|
|EnumTypeHandler|Enumeration Type|VARCHAR any string compatible type, as the code is stored (not index).|
|EnumOrdinalTypeHandler|Enumeration Type|Any compatible NUMERIC or DOUBLE, as the position is stored (not the code itself).|

你可以重写类型处理器或创建您自己处理不支持的或非标准的类型。要做到这一点，只需继承org.apache.ibatis.type.BaseTypeHandler类和选择性映射新的类型处理器类的JDBC类型。例如：




```java
// ExampleTypeHandler.java
@MappedJdbcTypes(JdbcType.VARCHAR)
public class ExampleTypeHandler extends BaseTypeHandler<String> {

  @Override
  public void setNonNullParameter(PreparedStatement ps, int i, String parameter, JdbcType jdbcType) throws SQLException {
    ps.setString(i, parameter);
  }

  @Override
  public String getNullableResult(ResultSet rs, String columnName) throws SQLException {
    return rs.getString(columnName);
  }

  @Override
  public String getNullableResult(ResultSet rs, int columnIndex) throws SQLException {
    return rs.getString(columnIndex);
  }

  @Override
  public String getNullableResult(CallableStatement cs, int columnIndex) throws SQLException {
    return cs.getString(columnIndex);
  }
}
```

```
<!-- mybatis-config.xml -->
<typeHandlers>
  <typeHandler handler="org.mybatis.example.ExampleTypeHandler"/>
</typeHandlers>
```
使用这样的类型处理器将会覆盖已经存在的Java的String类型属性和VARCHAR参数及结果的类型处理器。注意MyBatis不会审视数据库元数据的类型来确定的，所以你必须指定，这是一个VARCHAR字段的参数和结果映射到正确的类型处理。由于这样事实是MyBatis直到执行该语句，它不知道的数据类型。
MyBatis会知道你要处理这个类型处理器通过内省其泛型类型的Java类型，但你可以重写此行为由两个手段：



（1）、添加javaType属性类型处理器的元素（例如：javaType=“String”）

（2）、添加注释类型处理器类指定Java类型的列表，将它与一个@ MappedTypes。此注释将被忽略，如果javaType属性也被指定。


通过下列两种方式可以指定相关联的JDBC类型：


（1）、添加jdbcType属性到类型处理器的元素（例如：jdbcType为VARCHAR）。

（2）、添加注释联想到指定的JDBC类型列表类型处理器类的一个@ MappedJdbcTypes。此注释将被忽略，而jdbcType属性将被指定。


最后你可以让MyBatis搜索你的类型处理器：




```
<!-- mybatis-config.xml -->
<typeHandlers>
  <package name="org.mybatis.example"/>
</typeHandlers>
```
注意当使用JDBC类型自动发现功能时只能被指定的注解。

您可以创建一个能够处理超过一个类通用的的类型处理器。为了这个目的，MyBatis将会作为参数，并通过实际的类构造类型处理器时，接收类添加一个构造。

```java
//GenericTypeHandler.java
public class GenericTypeHandler<E extends MyObject> extends BaseTypeHandler<E> {

  private Class<E> type;

  public GenericTypeHandler(Class<E> type) {
    if (type == null) throw new IllegalArgumentException("Type argument cannot be null");
    this.type = type;
  }
  ...
```
EnumTypeHandler EnumOrdinalTypeHandler泛型类型处理器。在下一节中，我们将了解他们。



**5、处理枚举（Handling Enums）**

如果你想使用枚举映射，你会需要使用EnumTypeHandler或EnumOrdinalTypeHandler。

例如，让我们说，我们需要存储的舍入模式，应该用一些数字，如果它需要四舍五入。默认情况下，的MyBatis使用EnumTypeHandler的的枚举值转换成自己的名字。

**注意：EnumTypeHandler是在这个意义特殊，它不像其他的处理程序，它并不只是处理一个特定的类，而是任何扩展Enum的类。**

然而，我们可能不希望存储姓名。我们的DBA可能整数代码，而不是坚持。这是一样容易：添加EnumOrdinalTypeHandler到你的配置文件的类型处理器，现在每个与RoundingMode将被映射到一个整数，使用其序数值。




```
<!-- mybatis-config.xml -->
<typeHandlers>
  <typeHandler handler="org.apache.ibatis.type.EnumOrdinalTypeHandler" javaType="java.math.RoundingMode"/>
</typeHandlers>
```
但是如果你想相同的枚举映射到一个字符串在一个地方和另一个整数？自动映射器会自动使用EnumOrdinalTypeHandler的，所以如果我们想回去使用简单陈旧的普通EnumTypeHandler的，我们必须告诉它，明确设置类型处理程序使用这些SQL语句。（映射文件没有涉及到下一节，因此，如果这是你第一次通过阅读文档，你可能现在想跳过这些但以后会回来了解它的。）





```
<!DOCTYPE mapper
    PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
    "http://mybatis.org/dtd/mybatis-3-mapper.dtd">

<!DOCTYPE mapper
    PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN"
    "http://mybatis.org/dtd/mybatis-3-mapper.dtd">

<mapper namespace="org.apache.ibatis.submitted.rounding.Mapper">
	<resultMap type="org.apache.ibatis.submitted.rounding.User" id="usermap">
		<id column="id" property="id"/>
		<result column="name" property="name"/>
		<result column="funkyNumber" property="funkyNumber"/>
		<result column="roundingMode" property="roundingMode"/>
	</resultMap>

	<select id="getUser" resultMap="usermap">
		select * from users
	</select>
	<insert id="insert" parameterType="org.apache.ibatis.submitted.rounding.User">
	    insert into users (id, name, funkyNumber, roundingMode) values (
	    	#{id}, #{name}, #{funkyNumber}, #{roundingMode}
	    )
	</insert>
	
	<resultMap type="org.apache.ibatis.submitted.rounding.User" id="usermap2">
		<id column="id" property="id"/>
		<result column="name" property="name"/>
		<result column="funkyNumber" property="funkyNumber"/>
		<result column="roundingMode" property="roundingMode" typeHandler="org.apache.ibatis.type.EnumTypeHandler"/>
	</resultMap>
	<select id="getUser2" resultMap="usermap2">
		select * from users2
	</select>
	<insert id="insert2" parameterType="org.apache.ibatis.submitted.rounding.User">
	    insert into users2 (id, name, funkyNumber, roundingMode) values (
	    	#{id}, #{name}, #{funkyNumber}, #{roundingMode, typeHandler=org.apache.ibatis.type.EnumTypeHandler}
	    )
	</insert>

</mapper>
```



请注意，这迫使我们使用一个resultMap代替resultType在select语句中。


**6、对象工厂（objectFactory）**

     MyBatis每次创建结果对象的一个新实例，它使用一个ObjectFactory实例实现。默认的ObjectFactory做多一点的实例化目标类的默认构造函数，或一个参数的构造函数，如果存在参数映射。如果你想重写默认的ObjectFactory，你可以创建你自己的。例如：




```java
// ExampleObjectFactory.java
public class ExampleObjectFactory extends DefaultObjectFactory {
  public Object create(Class type) {
    return super.create(type);
  }
  public Object create(Class type, List<Class> constructorArgTypes, List<Object> constructorArgs) {
    return super.create(type, constructorArgTypes, constructorArgs);
  }
  public void setProperties(Properties properties) {
    super.setProperties(properties);
  }
}
```

```
<!-- mybatis-config.xml -->
<objectFactory type="org.mybatis.example.ExampleObjectFactory">
  <property name="someProperty" value="100"/>
</objectFactory>
```
ObjectFactory接口非常简单。它包含两个创建方法，处理默认构造函数和其他处理带参数构造。最后，setProperties方法可以被用来配置ObjectFactory的。body内的objectFactory元素中定义的属性将被传递给setProperties方法的ObjectFactory实例在初始化之后。


**7、插件（plugins）**

    MyBatis允许你调用映射语句的执行范围​​内的某些点拦截。默认情况下，MyBatis允许使用插件来拦截方法调用：


Executor (update, query, flushStatements, commit, rollback, getTransaction, close, isClosed)ParameterHandler (getParameterObject, setParameters)ResultSetHandler (handleResultSets, handleOutputParameters)StatementHandler (prepare, parameterize, batch, update, query)

这些类中方法的详细看签名，每一个完整的方法，可用每个MyBatis的释放与源代码来被发现。你应该理解你覆盖方法的行为，假设你正在做的事情不仅仅是监控呼叫。如果您尝试修改或覆盖一个给定的方法的行为，你很可能会打破MyBatis的核心。这是低层次的类和方法，所以使用插件持谨慎态度。



使用插件是非常简单的，通过他们所提供的力量。简单实现拦截器接口，确保你想拦截的指定签名。




```java
// ExamplePlugin.java
@Intercepts({@Signature(
  type= Executor.class,
  method = "update",
  args = {MappedStatement.class,Object.class})})
public class ExamplePlugin implements Interceptor {
  public Object intercept(Invocation invocation) throws Throwable {
    return invocation.proceed();
  }
  public Object plugin(Object target) {
    return Plugin.wrap(target, this);
  }
  public void setProperties(Properties properties) {
  }
}
```

```
<!-- mybatis-config.xml -->
<plugins>
  <plugin interceptor="org.mybatis.example.ExamplePlugin">
    <property name="someProperty" value="100"/>
  </plugin>
</plugins>
```
该插件在上面会拦截所有叫做“update”的方法在Executor实例，这是一个内部负责低层次映射语句的执行对象。
注意重写配置类：除了修改MyBatis核心的行为插件，你也可以完全覆盖配置类。简单扩展并覆盖任何方法里面，并把它传递到的sqlSessionFactoryBuilder.build（myConfig）方法的调用。再次提醒，虽然这可能会严重影响MyBatis的行为，所以谨慎使用


**8、environments标签**

       MyBatis的可以配置多个environment。这是有助于您应用到多个数据库的SQL映射为任意数量的原因。例如，你可能会为您的开发，测试和生产环境中有不同的配置。或者，你可能有多个生产数据库，共享相同的架构，你想使用相同的SQL映射。有许多用例。

       一个重要的事情要记住，虽然：虽然您可以配置多种environment，但你只能选择一个SqlSessionFactory对应的实例。

所以，如果你想连接两个数据库，你需要SqlSessionFactory的创建两个实例。三个数据库，你需要三个实例，并依此类推。这真的很容易，要记住：



**每个数据库对应一个SqlSessionFactory**


要指定创建哪种环境，你只需把它传递到SqlSessionFactoryBuilder作为一个可选的参数。接受环境中的两个签名是：




```java
SqlSessionFactory factory = sqlSessionFactoryBuilder.build(reader, environment);
SqlSessionFactory factory = sqlSessionFactoryBuilder.build(reader, environment,properties);
```
如果环境被省略，则加载默认的环境，如下所示：

```java
SqlSessionFactory factory = sqlSessionFactoryBuilder.build(reader);
SqlSessionFactory factory = sqlSessionFactoryBuilder.build(reader,properties);
```
environment元素定义了如何配置环境：

```
<environments default="development">
  <environment id="development">
    <transactionManager type="JDBC">
      <property name="..." value="..."/>
    </transactionManager>
    <dataSource type="POOLED">
      <property name="driver" value="${driver}"/>
      <property name="url" value="${url}"/>
      <property name="username" value="${username}"/>
      <property name="password" value="${password}"/>
    </dataSource>
  </environment>
</environments>
```
请注意这里的关键部分：

（1）、默认的环境ID（例如，default=“development”）。

（2）、的环境ID为每个定义的环境（例如：ID=“development”）。

（3）、事务管理器的配置（如type=“JDBC”）

（4）、数据源配置（例如type=“POOLED”）


默认的环境和环境ID是自我解释。将它们命名为任何你喜欢的，只是确保默认匹配其中之一。

**事务管理(transactionManager)**


TransactionManager的有两种类型（即类型=“[JDBC|MANAGED]”）中所包含的MyBatis：

JDBC - 这个配置直接简单使用了JDBC的提交和回滚。它依赖于从数据源的连接来管理事务范围。

MANAGED - 这个配置几乎没做什么。它从来不提交或回滚一个连接。相反，它让容器管理的整个生命周期的交易（如JEE应用服务器的上下文）。默认情况下，它会关闭连接。然而一些容器并没有想到这一点，因此如果你需要阻止它关闭连接，设置“形象尤其”属性设置为false。例如：




```
<transactionManager type="MANAGED">
  <property name="closeConnection" value="false"/>
</transactionManager>
```
注意：如果你打算使用MyBatis与Spring是无需配置任何的TransactionManager，因为Spring模块将覆盖任何先前设置的配置设置自己的。


TransactionManager的类型都不需要任何属性。但是，它们都是类型别名，所以换句话说，你可以把你自己的完全合格的类名或类型别名，是指你自己的实施的TransacFactory接口，而不是使用它们。




```java
public interface TransactionFactory {
  void setProperties(Properties props);
  Transaction newTransaction(Connection conn);
  Transaction newTransaction(DataSource dataSource, TransactionIsolationLevel level, boolean autoCommit);
}
```
任何在XML中配置的属性将被传递给setProperties（）方法实例化后。您的实现还需要创建一个交易执行，这也是一个非常简单的接口：

```java
public interface Transaction {
  Connection getConnection() throws SQLException;
  void commit() throws SQLException;
  void rollback() throws SQLException;
  void close() throws SQLException;
}
```
使用这两个接口，你可以完全自定义MyBatis对事务的处理。



dataSourcedataSource元素源配置使用标准的JDBC DataSource接口的JDBC连接对象。

许多MyBatis的应用程序配置数据源的例子中。然而，它不是必需的。虽然实现，为了方便使用延迟加载，这个数据源是必需的。有三种内建的数据源类型（即type=“？”）：


**（1）、NNPOOLED**

UNPOOLED - 这个数据源的实现简单地打开和关闭连接每次请求。虽然它慢一点，这是一个不错的选择，简单的应用程序，不需要及时的可用连接的性能。在此表演区，不同的数据库也不同，所以对一些人来说可能是不太重要的游泳池，这个配置将是理想的。 UNPOOLED的DataSource配置只有五个属性：

driver - 这是完全合格的Java类的JDBC驱动程序（不DataSource类，如果你的驱动器包括一个）。

url - 这是你的数据库实例的JDBC URL。

username- 数据库的用户名登录。

password - 数据库的密码登录。

defaultTransactionIsolationLevel - 默认的事务隔离级别连接。

或者，您可以通过数据库驱动程序的属性。要做到这一点，与驱动程序前缀的属性，例如：

driver.encoding=UTF8

这将传递属性编码，UTF8的价值，数据库驱动程序通过的DriverManager.getConnection（URL，driverProperties）方法。


**（2）、POOLED**

     POOLED- 此实现数据源连接池的JDBC连接对象，以避免初始连接和认证所需的时间来创建一个新的连接实例。这是一个流行的做法，并发Web应用程序，以实现最快的响应。除了向（UNPOOLED）上述物业，也有许多其他属性，可用于配置POOLED的数据源：

poolMaximumActiveConnections - 这是有效的（即在使用）的连接，可以在任何给定时间存在的数量。默认值：10

poolMaximumIdleConnections - 的空闲连接的数目，可以存在于任何给定的时间。

poolMaximumCheckoutTime - 在一个连接可以“检查”出来之前，池中将被强制返回的时间量。默认值：20000 （即20秒）

poolTimeToWait - 这是一个低级别设置给连接池一个打印日志状态，并重新尝试连接的情况下，它采取不同寻常的长（避免失败永远默默地如果池配置错误）收购​​的机会。默认值：20000 （即20秒）

poolPingQuery - 平安查询发送到数据库以验证连接处于良好的工作秩序，并准备好接受请求。默认值是“ NO PING QUERY SET ” ，这将导致大多数的数据库驱动程序失败，并一个体面错误消息。

poolPingEnabled - 这是开启或禁用侦测查询。如果启用，则还必须与一个有效的SQL语句（最好是很快速的）设置poolPingQuery属性。默认值：false 。

poolPingConnectionsNotUsedFor - 此配置的频率将被用于poolPingQuery ， 。这可以被设置匹配标准的数据库连接超时，以避免不必要的侦测。默认值：0 （即所有的连接都被侦测每一次 - 但只有当poolPingEnabled的当然是真实的） 。


**（3）、JNDI**

JNDI - 这个数据源的实现是为了使用如EJB容器或应用服务器，可以配置数据源，集中或外部的引用放到一个JNDI上下文。这个数据源配置只需要两个属性：

initial_context - 这个属性是用于上下文查找的InitialContext（即（initial_context）initialContext.lookup）。此属性是可选的，如果省略，然后将一个数据源属性抬头，直接以初始。

data_source - 这是可以找到参考的数据源实例的上下文路径。这将是抬头对返回的环境由initial_context的查找，或直接initial_context没有反对的InitialContext。

其他数据源配置相似，它可以直接向初始属性发送与环保这些属性的前缀，例如：

env.encoding=UTF8

这将发送的财产编码与UTF8实例化后的InitialContext的构造的价值。**9、数据库ID提供者（databaseIdProvider）**

      MyBatis的是能够执行不同的语句，这取决于您的数据库供应商。多数据库厂商的支持的基础上的映射语句的databaseID的属性。 MyBatis将会加载与没有databaseId属性相匹配的当前用的databaseID的所有陈述。如果情况，如果发现相同的语句与和的databaseID无后者将被丢弃。为了使多厂商支持的MyBatis-config.xml文件中添加databaseIdProvider如下：



`<databaseIdProvider type="DB_VENDOR" />`DB_VENDOR实施databaseIdProvider套作为databaseId由DatabaseMetaData：＃getDatabaseProductName（）返回的字符串。通常字符串太长，不同版本的同一产品也返回不同的值，所以你可能想将它翻译成一个较短的添加属性，比如如下：





```
<databaseIdProvider type="DB_VENDOR">
  <property name="SQL Server" value="sqlserver"/>
  <property name="DB2" value="db2"/>
  <property name="Oracle" value="oracle" />
</databaseIdProvider>
```
当属性类型为DB_VENDOR databaseIdProvider搜索发现在返回的数据库产品名称或“空”的第一个关键，如果没有匹配的属性对应的属性值。在这种情况下，如果getDatabaseProductName（）返回“甲骨文公司（DataDirect产品）”的的databaseID将被设置为“oracle”。

您可以建立您的自己的DatabaseIdProvider，实现接口org.apache.ibatis.mapping.DatabaseIdProvider的MyBatis-config.xml中注册：

```java
public interface DatabaseIdProvider {

  void setProperties(Properties p);

  String getDatabaseId(DataSource dataSource) throws SQLException;
}
```
**10、映射器（mappers）**


现在MyBatis的行为是配置上面的配置元素，我们已经准备好来定义映射的SQL语句。但首先，我们需要告诉MyBatis在哪里可以找到它们的。 Java不提供一个很好的手段自动发现，在这方面，所以做到这一点的最好办法是简单地告诉MyBatis在哪里可以找到映射文件。您可以使用相对于类路径的资源引用，或文字，完全合格的URL引用（包括file:/ / /URL）。例如：


```
<!-- Using classpath relative resources -->
<mappers>
  <mapper resource="org/mybatis/builder/AuthorMapper.xml"/>
  <mapper resource="org/mybatis/builder/BlogMapper.xml"/>
  <mapper resource="org/mybatis/builder/PostMapper.xml"/>
</mappers>
```

```
<!-- Using url fully qualified paths -->
<mappers>
  <mapper url="file:///var/mappers/AuthorMapper.xml"/>
  <mapper url="file:///var/mappers/BlogMapper.xml"/>
  <mapper url="file:///var/mappers/PostMapper.xml"/>
</mappers>
```

```
<!-- Using mapper interface classes -->
<mappers>
  <mapper class="org.mybatis.builder.AuthorMapper"/>
  <mapper class="org.mybatis.builder.BlogMapper"/>
  <mapper class="org.mybatis.builder.PostMapper"/>
</mappers>
```

```
<!-- Register all interfaces in a package as mappers -->
<mappers>
  <package name="org.mybatis.builder"/>
</mappers>
```
这些语句简单告诉MyBatis去哪里从这里开始（多种配置方式）。其余的细节都在每个SQL映射文件，这正是下一节将讨论什么。


