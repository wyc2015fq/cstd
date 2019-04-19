# MyBatis 完全使用指南 - 文章 - 伯乐在线
原文出处： [TheAlchemist](http://www.jianshu.com/p/1c7c7d1bba33)
![](http://jbcdn2.b0.upaiyun.com/2016/11/0e99c9b9af9550a379ed828b5aa5e9eb.png)
MyBatis作为一个轻量的SQL映射框架，确实很简单，但是知识点挺多，实际使用中还是会有时想不起来某个标签该怎么写，所以整理了这篇文章，以备查询。由于MyBatis如此简单，使得这一篇文章基本把实际使用中常碰到的事情都涵盖了，包括：
- MyBatis中的一些概念
- MyBatis包含的内容
- SQL映射
- 动态SQL
### 一、 MyBatis中的一些概念
![](http://jbcdn2.b0.upaiyun.com/2016/11/9cf560fa27578b9ee7bae162fd89d838.jpg)
MyBatis的架构
#### 1. `MyBatis`是个什么东西
在使用MyBatis的项目里，日志中往往会出现很多拼接SQL语句的log，这其实说明了MyBatis底层还是使用JDBC来实现的。在JDBC的基础上，为了性能考虑，所有的语句都是基于`SqlSession`。顾名思义，维护这个类的实例其实就是维护了一个对于某个数据库的连接的会话，在这个会话里可以有缓存啊什么之类的。
#### 2. `mapper`接口是怎么映射成SQL语句的
目前多数开发者还是会使用XML来进行MyBatis的配置，包括MyBatis的核心配置和SQL映射配置。其实和注解一样，XML本身只不过是一个元数据的载体，最终起作用的还是MyBatis的核心类。其中有这样几个比较重要的：
- `SqlSessionFactoryBuilder`，用来创建`SqlSessionFactory`的实例，之后就没有用了，其生命周期只是在初始化的时候有作用。
- `SqlSessionFactory`，MyBatis最基础的类，用来创建会话（即`SqlSession`的实例），其生命周期与整个系统的生命周期相同，在系统运行的任何时候都可以使用它查询到当前数据库的配置信息等。
- `SqlSession`，真正的和数据库之间的会话，线程不安全，所以其生命周期和使用它的线程相同。
- 各种`Mapper`，承载了实际的业务逻辑，其生命周期比较短，由`SqlSession`创建。
#### 3. Spring环境中MyBatis的初始化过程
实际情况中MyBatis往往是在Spring的环境中使用的，MyBatis本身并不依赖Spring，但是使用Spring可以极大的提高开发效率，由于Spring进行了控制反转，所以其中MyBatis的初始化过程和正常过程稍稍有些不同：
- Spring发现需要创建`SqlSessionFactory`实例，会在classpath下找到MyBatis的核心配置文件，使用它来初始化一个`SqlSessionFactory`实例。当然，这一步完全可以使用代码来完成，或者使用注解，就更加清晰明了。
- 往往mapper类也会作为bean注入到代码中去的，那么Spring会使用上一步中的`SqlSessionFactory`实例来创建`SqlSession`的实例。
- 然后再使用`SqlSession`尝试创建各个mapper对象。
- 于此同时，MyBatis会扫描classpath下的mapper映射XML文件（此路径可以自定义），对于每一个mapper接口，它的「类全名」会作为命名空间，来和映射文件中的mapper标签进行匹配。
- 对于每一个映射文件中的一个执行语句标签（如select、delete），MyBatis会把他们映射到`SqlSession`的方法上，创建mapper接口的一个实现类。
- 如果mapper接口和其映射文件一一匹配，则bean创建成功。
### 二、MyBatis包含的内容
MyBatis本身就是一个简单的ORM框架，提供了SQL语句到方法、关系型数据表到对象的映射。实际使用中与开发者相关的有两个东西：
- MyBatis核心配置
缓存、数据源、日志等关系到MyBatis其本身行为的一些配置。
- mapper接口的映射
针对于具体业务逻辑的SQL映射。
### 三、SQL映射
MyBatis也可以使用注解来实现映射，对于简单的语句，使用注解可能会更加清晰简单，但是其真正强大的地方还是XML。
#### 1. select
|属性|描述|
|----|----|
|id|此命名空间内的标识符|
|parameterType|参数的类的全名或者alias，可选。默认为空。|
|parameterMap|**Deprecated**|
|resultType|返回结果的类型全名或alias，如果结果是集合，此类型表示的是集合的成员类型。|
|resultMap|使用指定的resultMap来映射结果集。resultMap 和 resultType只能二选一。|
|flushCache|如果为true，每次调用，一级缓存和二级缓存都会回写。select语句中默认为false。|
|useCache|如果为true，结果将在二级缓存中缓存。select语句中默认为true|
|timeout|设置超时，若超时则抛出异常。|
|fetchSize|尝试在获取数据时分批获取。|
|statementType|STATEMENT，PREPARED或者CALLABLE. 分别对应JDBC中的Statement，PreparedStatement和CallableStatement respectively。默认PREPARED.|
|resultSetType|FORWARD_ONLY，SCROLL_SENSITIVE或者SCROLL_INSENSITIVE。默认为空。|
|databaseId|使用特定的databaseIdProvider|
|resultOrdered|嵌套查询时使用。|
|resultSets|多返回集合时使用。|
#### 2. 修改语句（insert，update，DELETE）
这3种语句的属性基本和上边select的一样，INSERT和UPDATE的语句有几个特殊的属性如下：
|属性|描述|
|----|----|
|useGeneratedKeys|将使用JDBC的getGeneratedKeys方法来获取主键的值。默认为false。|
|keyProperty|主键。MyBatis会将生成的主键赋给这个列。联合主键使用逗号隔开。|
|keyColumn|特定数据库需要使用。|
#### 3. SQl语句段（sql标签）
标签可以定义一段sql语句段，可以在其他语句中引入使用。sql标签可以包含参数。示例如下：


```
<sql id="userColumns">${alias}.id,${alias}.username,${alias}.password </sql>
<select id="selectUsers" resultType="map">
      select
        <include refid="userColumns"><property name="alias" value="t1"/></include>,
        <include refid="userColumns"><property name="alias" value="t2"/></include>
      from some_table t1
        cross join some_table t2
</select>
```
参数（property）也可以在refid或者include进来的sql中使用。示例如下：


```
<sql id="sometable">
      ${prefix}Table
</sql>
<sql id="someinclude">
      from
    <include refid="${include_target}"/>
</sql>
<select id="select" resultType="map">
  select
    field1, field2, field3
  <include refid="someinclude">
    <property name="prefix" value="Some"/>
    <property name="include_target" value="sometable"/>
  </include>
</select>
```
#### 4. `#{}`和`${}`的区别
- `#{}`在底层实现上使用`?`做占位符来生成PreparedStatement，然后将参数传入，大多数情况都应使用这个，它更快、更安全。
- `${}`将传入的数据直接显示生成在sql中。如：order by ${user_id}，如果传入的值是111,那么解析成sql时的值为order by 111, 如果传入的值是id，则解析成的sql为order by id.
#### 5. 类型别名
什么地方都少不了这种小技巧，可以让你少打很多字。


```
<typeAlias type="com.someapp.model.User" alias="User"/>
```
#### 6. ResultMap
官方文档上说这个特性是整个MyBatis中**最强大**的特性（没有之一）。其实它做的就是映射结果集。
MyBatis底层使用JDBC，所以查询出来的结果是ResultSet，如果resultType是一个对象，MyBatis底层就会创建一个resultMap，并根据字段名一一对应上这个对象。如果你有一个查询，它的结果集非常复杂，可以使用resultMap来做映射。
#### cache和cach-ref
使用cache标签在映射文件内（某命名空间内）实现二级缓存，其所有属性都有缺省值，所以单单一个标签就可以生效。cach-ref可以在两个映射文件之间共享缓存


```
<cache
        eviction="LRU"//缓存移除策略
        flushInterval="60000"//默认不设置，不定时刷新
        size="1024"
        readOnly="false"/>
```
### 四、动态SQL
#### 1. if
条件判断，参数为test
#### 2. choose (嵌套when，otherwise)
多选一，参数为test
#### 3. trim (where，set)
- trim标签可以定制其他标签，并且去头去尾；
- where标签会自动生成where语句，并且可以去除头部的and或者or；
- set标签可以自动生成set语句，并且可以去除尾部的逗号。
#### 4. foreach
循环集合输出，可以循环数组，Iterable对象，或者Map，如果是Map，则index会是key。


```
<foreach item="item" index="index" collection="list"
  open="(" separator="," close=")">
    #{item}
</foreach>
```
#### 5. bind
声明一个使用OGNL表达式生成的变量，在sql语句中使用。
