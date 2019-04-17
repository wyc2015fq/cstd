# Github Mybatis深入学习之映射器XML - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月09日 16:42:13[boonya](https://me.csdn.net/boonya)阅读数：5653








 mapper.xml文件的配置是mybatis实体对象和查询接口封装SQL操作的配置文件，其中还包括了查询、修改和删除等操作，并且包含对象的关联映射、数据集合的关联映射、以及对象的缓存设置和缓存引用等操作，了解这些对手动设置和学习对象的关系很有帮助。

 MyBatis的真正的力量是在映射语句。这是奇迹发生的地方。对于所有的力量，映射XML文件相对简单。当然如果你比较它们相当于JDBC代码，你会立即看到节省了95％的代码。MyBatis的构建就是聚焦于SQL的，它最好留出自己的方式。原文地址：[http://mybatis.github.io/mybatis-3/sqlmap-xml.html](http://mybatis.github.io/mybatis-3/sqlmap-xml.html)

映射XML文件只有几个一流的元素（它们应该被定义的顺序）：
- cache -配置给定命名空间的缓存。
- cache-ref -缓存配置从另一个命名空间的参考。
- resultMap -最复杂，最强大的元素，介绍了如何将你的对象从数据库结果集。
- ~~parameterMap的~~~~ -已过时！老派的参数映射。内联参数是优选的，这些元素在将来可能会被删除。这里没有记录。~~
- sql -可以重用的SQL块，也可以被其他语句引用。
- insert -一个映射INSERT语句。
- update -映射的UPDATE语句。
- delete -映射DELETE语句。
- select -一个映射的SELECT语句。
**1、选择（select）查询语句**
select语句是您使用MyBatis最流行的元素之一。把数据库中有价值的数据筛选，直到帮你把它找回来，所以许多应用程序查询远远超过他们修改数据。对于每次插入，更新或者删除，有许多选择。查询是MyBatis的构建原则之一，原因是这么多的重点和精力放在查询和结果映射，查询元素是非常简单的。例如：

```
<select id="selectPerson" parameterType="int" resultType="hashmap">
  SELECT * FROM PERSON WHERE ID = #{id}
</select>
```

这条语句被称作selectPerson的，需要一个参数类型为int（或Integer），并返回一个HashMap的键是列名映射到行值。注意参数注释：
`#{id}`


这告诉MyBatis使创建一个PreparedStatement参数。使用JDBC，这样的参数可以由一个“？” 在SQL传递到一个新的预处理，这样的事情：

```java
// Similar JDBC code, NOT MyBatis…
String selectPerson = "SELECT * FROM PERSON WHERE ID=?";
PreparedStatement ps = conn.prepareStatement(selectPerson);
ps.setInt(1,id);
```
当然，还有很多更需要单独的JDBC代码来提取结果并将它们映射到一个对象的一个​​实例，这是做什么MyBatis节省你时间。有很多了解参数和结果映射。这些细节保证自己的节，在本节后面。select元素有更多的属性，使您可以配置每个语句应该如何行为的细节。



```
<select
  id="selectPerson"
  parameterType="int"
  parameterMap="deprecated"
  resultType="hashmap"
  resultMap="personResultMap"
  flushCache="false"
  useCache="true"
  timeout="10000"
  fetchSize="256"
  statementType="PREPARED"
  resultSetType="FORWARD_ONLY">
```
select的属性



|属性|描述|
|----|----|
|ID|此命名空间中的唯一标识符，可以用来引用这条语句。|
|parameterType|类的完全限定名或别名将传入这条语句的参数。|
|parameterMap|这是一个废弃的方法引用外部parameterMap。使用内联参数映射和parameterMap属性。|
|resultType|从这个声明中，将返回的期望类型的类的完全限定名或别名。请注意，在集合的情况下，这应该是集合包含类型，不是类型的集合本身。使用resultType或 resultMap，而不是两个。|
|resultMap|的命名参照一个外部的resultMap。结果映射MyBatis的是最强大的功能，可以解决他们一个很好的了解，许多复杂映射的情形。使用resultMap或与resultType，而不是两个。|
|flushCache|设置为true，将导致每当这个语句被称作本地和第二级缓存被刷新。默认值：false。|
|useCache|设置为true，将导致这句话的结果是二级缓存缓存。默认： true。|
|timeout|这设定的秒数，将驱动程序等待数据库返回请求之前，抛出一个异常。默认unset（取决于驱动程序）。|
|fetchSize|这是一个驱动暗示试图将导致驱动程序返回的结果行的编号在此设置的大小等于批次。默认的设置（取决于驱动程序）。|
|statementType|任何一个STATEMENT，PREPARED或CALLABLE。这导致MyBatis的使用PreparedStatement或CallableStatement的分别。默认值：准备。|
|resultSetType|任何一个FORWARD_ONLY SCROLL_SENSITIVE的SCROLL_INSENSITIVE。默认的设置（取决于驱动程序）。|
|databaseId|有配置的databaseIdProvider，MyBatis将会加载与没有databaseId 属性相匹配的当前用的databaseID的所有陈述。如果情况，如果发现相同的语句与和的databaseID无后者将被丢弃。|
|resultOrdered|这是只适用于嵌套结果select语句：如果这是真的，它是假定嵌套结果载列或组合在一起，这样，当一个新的主要结果行返回到以前的结果行，没有提及会再发生。这允许嵌套结果来填补内存更友好。默认： false。|
|resultSets|这是只适用于多个结果集。它列出了将语句返回的结果集，并给出每一个名字。名称由逗号分隔。|

**2、插入(insert)、更新(update)和删除(delete)**

数据修改语句插入，更新和删除，在其实施是非常相似的：

```
<insert
  id="insertAuthor"
  parameterType="domain.blog.Author"
  flushCache="true"
  statementType="PREPARED"
  keyProperty=""
  keyColumn=""
  useGeneratedKeys=""
  timeout="20">

<update
  id="insertAuthor"
  parameterType="domain.blog.Author"
  flushCache="true"
  statementType="PREPARED"
  timeout="20">

<delete
  id="insertAuthor"
  parameterType="domain.blog.Author"
  flushCache="true"
  statementType="PREPARED"
  timeout="20">
```



**insert,update和delete属性**
|插入，更新和删除属性| |
|----|----|
|属性|描述|
|ID|此命名空间中的唯一标识符，可以用来引用这条语句。|
|parameterType|类的完全限定名或别名将传入这条语句的参数。|
|parameterMap|这是一个废弃的方法引用外部parameterMap。使用内联参数的映射和parameterType属性。|
|flushCache|设置为true，将导致第二级和本地缓存刷新每当这个语句被称作。默认：true的插入，更新和删除语句。|
|timeout|这设置最大秒数将驱动程序等待数据库返回请求之前，抛出一个异常。默认的unset（取决于驱动程序）。|
|statementType|任何一个STATEMENT，PREPARED或CALLABLE。这导致MyBatis的使用PreparedStatement或CallableStatement的分别。默认值：准备。|
|useGeneratedKeys|（插入）这就告诉MyBatis使用JDBC getGeneratedKeys方法来检索数据库（如自动递增字段在关系型数据库，如MySQL或SQL Server）内部产生的密钥。默认值：false|
|keyProperty|（插入）标识的属性到MyBatis将会设置键返回值getGeneratedKeys，或由selectKey元素的子元素的insert语句。默认：未设置。|
|keyColumn|（插入）设置生成的键的表中的列名。只需要在特定的数据库（如PostgreSQL）键列时，是不是第一个表中的列。|
|databaseId|有配置的databaseIdProvider，MyBatis将会加载与没有databaseId 属性相匹配的当前用的databaseID的所有陈述。如果情况，如果发现相同的语句与和的databaseID无后者将被丢弃。|



以下是一些例子，插入，更新和删除语句。

```
<insert id="insertAuthor" parameterType="domain.blog.Author">
  insert into Author (id,username,password,email,bio)
  values (#{id},#{username},#{password},#{email},#{bio})
</insert>

<update id="updateAuthor" parameterType="domain.blog.Author">
  update Author set
    username = #{username},
    password = #{password},
    email = #{email},
    bio = #{bio}
  where id = #{id}
</update>

<delete id="deleteAuthor" parameterType="int">
  delete from Author where id = #{id}
</delete>
```

 如前所述，插入一点点更加丰富的，它有一些属性和子元素，从而使其处理以多种方式中的密钥生成。首先，如果你的数据库支持自动生成主键字段（如MySQL和SQL Server），那么你可以简单地设置useGeneratedKeys =“true”，并设置目标属性keyProperty就大功告成了。例如，如果使用了上面的作者表的ID自动生成的列类型，声明将作如下修改：

```
<insert id="insertAuthor" parameterType="domain.blog.Author" useGeneratedKeys="true"
    keyProperty="id">
  insert into Author (username,password,email,bio)
  values (#{username},#{password},#{email},#{bio})
</insert>
```



MyBatis有另一种方式来处理密钥生成的数据库不支持自动生成的列类型，或者还不支持的JDBC驱动程序支持自动生成的键。下面是一个简单的例子（傻），会生成一个随机ID（可能你不会这么做，但是这展示的灵活性和MyBatis的真的不介意）：

```
<insert id="insertAuthor" parameterType="domain.blog.Author">
  <selectKey keyProperty="id" resultType="int" order="BEFORE">
    select CAST(RANDOM()*1000000 as INTEGER) a from SYSIBM.SYSDUMMY1
  </selectKey>
  insert into Author
    (id, username, password, email,bio, favourite_section)
  values
    (#{id}, #{username}, #{password}, #{email}, #{bio}, #{favouriteSection,jdbcType=VARCHAR})
</insert>
```



在上面的例子中，selectKey将首先运行，作者 ID会被设置，然后插入语句会被调用。这给你一个类似的行为在你的数据库中自动生成的密钥，无需复杂的Java代码。selectKey元素描述如下：


```
<selectKey
  keyProperty="id"
  resultType="int"
  order="BEFORE"
  statementType="PREPARED">
```






|**selectKey元素属性**| |
|----|----|
|属性|描述|
|keyProperty|selectKey元素语句的结果应设置目标属性。|
|resultType|不同的结果。的MyBatis通常可以算出来，但它不会伤害它添加到务必。MyBatis允许任何简单类型，被用来作为关键，包括字符串。|
|order|这可以设置BEFORE或AFTER。如果设置BEFORE，那么它会首先选择主键，设置keyProperty，然后执行插入语句。如果设置后，执行插入语句，然后selectKey语句-这是很常见的数据库，如Oracle，可能已经嵌入序列调用INSERT语句。|
|statementType|同上，MyBatis的支持声STATEMENT，PREPARED和CALLABLE语句类型分别映射到声明的PreparedStatement和CallableStatement的。|

**3、SQL**

这个元素可以被用来定义可重用的SQL代码段，可以包含在其他语句。例如：



`<sql id="userColumns"> id,username,password </sql>`


SQL片段可以被包含在另一份声明中，例如：




```
<select id="selectUsers" parameterType="int" resultType="hashmap">
  select <include refid="userColumns"/>
  from some_table
  where id = #{id}
</select>
```
**4、参数(parameters)**


在所有过去的表格中，你已经看到了一些简单参数的例子。MyBatis中参数是非常强大的元素。对于简单的情况下，大概90％的情况下，是不太多的，例如：




```
<select id="selectUsers" parameterType="int" resultType="User">
  select id, username, password
  from users
  where id = #{id}
</select>
```



上面的例子演示了一个非常简单的命名参数映射。参数类型被设置为 int类型，因此这个参数可以任意取名。原始或简单数据类型，如 整数和字符串没有相关属性，从 ​​而将取代参数的值完全。不过，如果你在一个复杂的对象传递，那么其行为是一个有点不同。例如：




```
<insert id="insertUser" parameterType="User" >
  insert into users (id, username, password)
  values (#{id}, #{username}, #{password})
</insert>
```
如果User类型的参数对象传递到了语句，ID，用户名和密码会被抬起头来，他们的价值观传递给一个PreparedStatement参数。


这是好的和简单的传递参数​​到语句。但也有很多其他功能参数映射。首先，像MyBatis的其他部分，参数可以指定一个更具体的数据类型。
`#{property,javaType=int,jdbcType=NUMERIC}`
MyBatis的其他地区一样，给javaType几乎总是可以从参数对象中决定，除非对象是一个HashMap的。然后给javaType应该被确定来保证使用正确 类型处理器。

注：所有可能为空的列，如果为null被当作值来传递所需的JDBC JDBC类型。您可以调查自己通过阅读预处理语句（）方法的JavaDoc 。

要进一步自定义类型的处理，也可以指定一个特定的类型处理器 类（或别名），例如：
`#{age,javaType=int,jdbcType=NUMERIC,typeHandler=MyTypeHandler}`


所以，它已经似乎变得越来越冗长，但事实是，你很少设置任何这些。对于数值类型也有一个数值范围确定小数点后多少有关。
`#{height,javaType=double,jdbcType=NUMERIC,numericScale=2}`


最后，模式属性允许你指定IN，OUT或INOUT参数。如果参数为 OUT或INOUT，参数对象属性的实际价值将被改变，就像你会想到，如果你要求输出参数。如果模式= OUT（或INOUT）和 jdbcType为CURSOR（也就是Oracle的REFCURSOR），你必须指定一个resultMap的映射结果集到的参数类型。的javaType属性是可选的，它会被自动设置ResultSet的一个光标的jdbcType如果左边的空白。
`#{department, mode=OUT, jdbcType=CURSOR, javaType=ResultSet, resultMap=departmentResultMap}`


MyBatis也支持更高级的数据类型，比如结构，但是你必须告诉语句类型名称注册out参数时。比如（再次提示，不要在实践中，像这样换行）：
`#{middleInitial, mode=OUT, jdbcType=STRUCT, jdbcTypeName=MY_TYPE, resultMap=departmentResultMap}`


尽管所有这些强大的选项，大部分的时间你只需指定属性名，MyBatis的休息。在大多数，你会指定的jdbcType为空的列。

```
#{firstName}
#{middleInitial,jdbcType=VARCHAR}
#{lastName}
```



**5、字符串替换**

默认情况下，使用＃{}语法会导致MyBatis使生成的PreparedStatement性能和安全设定值，对PreparedStatement的参数（例如？）。虽然这是更安全，速度更快，而且几乎总是首选，有时你只是想直接注入到SQL语句中的字符串未修改。例如，对于ORDER BY，你可能会使用这样的事情：
`ORDER BY ${columnName}`


这里的MyBatis不会修改或转义字符串。注：这不是安全地接受用户的输入，并提供一份声明中这样未修改。这将导致潜在的SQL注入攻击，因此，你应该允许用户输入这些字段中，或总是执行自己的逃逸和检查。

**6、结果映射（resultMap）**

resultMap的元素MyBatis中是最重要，最强大的元素。是什么让你远离JDBC要求的检索数据从结果 s，并且在某些情况下，可以让你做的事情，甚至不支持JDBC 代码的90％ 。事实上，写一个复杂语句联合映射类似的等同的代码，也许可以跨过上千行的代码。的结果映射 s 的设计是这样的简单语句不需要明确的结果映射，更复杂的语句不需要多描述它们的关系是绝对必要的。您已经看到了，没有一个明确的结果映射的简单映射语句的例子。例如：




```
<select id="selectUsers" parameterType="int" resultType="hashmap">
  select id, username, hashedPassword
  from some_table
  where id = #{id}
</select>
```



这样一个语句简单导致所有列被自动映射到一个HashMap的键，与resultType属性指定。虽然在很多情况下很有用，一个HashMap中没有一个很好的域模型。它更可能是你的应用程序将使用JavaBeans或域模型的POJO（简单Java对象）。MyBatis的支持。请考虑以下的JavaBean：

```java
package com.someapp.model;
public class User {
  private int id;
  private String username;
  private String hashedPassword;
  
  public int getId() {
    return id;
  }
  public void setId(int id) {
    this.id = id;
  }
  public String getUsername() {
    return username;
  }
  public void setUsername(String username) {
    this.username = username;
  }
  public String getHashedPassword() {
    return hashedPassword;
  }
  public void setHashedPassword(String hashedPassword) {
    this.hashedPassword = hashedPassword;
  }
}
```
基于JavaBeans规范，上面的类有3个属性：ID，用户名，hashedPassword。这些精确匹配的select语句中的列名。这样的一个JavaBean可以被映射到ResultSet中一样容易的HashMap。
```
<select id="selectUsers" parameterType="int" resultType="com.someapp.model.User">
  select id, username, hashedPassword
  from some_table
  where id = #{id}
</select>
```



要记住类型别名是你的朋友。使用它们，这样你就不必继续键入你的类的完全限定路径。例如：

```
<!-- In Config XML file -->
<typeAlias type="com.someapp.model.User" alias="User"/>

<!-- In SQL Mapping XML file -->
<select id="selectUsers" parameterType="int" resultType="User">
  select id, username, hashedPassword
  from some_table
  where id = #{id}
</select>
```



在这些情况下，MyBatis会自动创建一个幕后自动映射列到JavaBean属性基于名称的ResultMap。如果列名没有精确匹配，你可以在列名上使用select字句的别名（一个标准的SQL功能）来匹配标签。例如：

```
<select id="selectUsers" parameterType="int" resultType="User">
  select
    user_id             as "id",
    user_name           as "userName",
    hashed_password     as "hashedPassword"
  from some_table
  where id = #{id}
</select>
```



ResultMap Ş 伟大的事情是，你已经学到了很多关于他们，但你有没有见过一个！这些简单的情况下，不要求任何超过你在这里看到的。只是出于示例的原因，让我们来看看这是什么最后的示例看起来像一个外部的结果，因为这是另一种方式来解决列名不匹配。

```
<resultMap id="userResultMap" type="User">
  <id property="id" column="user_id" />
  <result property="username" column="user_name"/>
  <result property="password" column="hashed_password"/>
</resultMap>
```
声明引用它使用的resultMap属性就行了（注意我们去掉 与resultType属性）。例如：

```
<select id="selectUsers" parameterType="int" resultMap="userResultMap">
  select user_id, user_name, hashed_password
  from some_table
  where id = #{id}
</select>
```



**7、高级结果映射**

        MyBatis创建的一个想法：数据库并不总是你想要什么，或者需要他们。虽然我们最喜欢的数据库最好是第三范式或BCNF模式，他们都没有。，这将是巨大的，如果可能有一个单独的数据库映射到所有使用它的应用，它不是。结果映射MyBatis的这个问题提供了答案。例如，我们如何映射下面这个语句？

```
<!-- Very Complex Statement -->
<select id="selectBlogDetails" parameterType="int" resultMap="detailedBlogResultMap">
  select
       B.id as blog_id,
       B.title as blog_title,
       B.author_id as blog_author_id,
       A.id as author_id,
       A.username as author_username,
       A.password as author_password,
       A.email as author_email,
       A.bio as author_bio,
       A.favourite_section as author_favourite_section,
       P.id as post_id,
       P.blog_id as post_blog_id,
       P.author_id as post_author_id,
       P.created_on as post_created_on,
       P.section as post_section,
       P.subject as post_subject,
       P.draft as draft,
       P.body as post_body,
       C.id as comment_id,
       C.post_id as comment_post_id,
       C.name as comment_name,
       C.comment as comment_text,
       T.id as tag_id,
       T.name as tag_name
  from Blog B
       left outer join Author A on B.author_id = A.id
       left outer join Post P on B.id = P.blog_id
       left outer join Comment C on P.id = C.post_id
       left outer join Post_Tag PT on PT.post_id = P.id
       left outer join Tag T on PT.tag_id = T.id
  where B.id = #{id}
</select>
```



你可能想把它映射到一个智能的对象模型由一个作者写的博客，有很多文章，其中每一个可能为零或许多评论和标签。下面是一个完整的例子（假设作者，博客，帖子，评论和标签都是类型的别名）的复杂结果。看看它，但不要担心，我们将要经过的每一步。尽管它可能看起来令人生畏，它实际上是非常简单的。



```
<!-- Very Complex Result Map -->
<resultMap id="detailedBlogResultMap" type="Blog">
  <constructor>
    <idArg column="blog_id" javaType="int"/>
  </constructor>
  <result property="title" column="blog_title"/>
  <association property="author" javaType="Author">
    <id property="id" column="author_id"/>
    <result property="username" column="author_username"/>
    <result property="password" column="author_password"/>
    <result property="email" column="author_email"/>
    <result property="bio" column="author_bio"/>
    <result property="favouriteSection" column="author_favourite_section"/>
  </association>
  <collection property="posts" ofType="Post">
    <id property="id" column="post_id"/>
    <result property="subject" column="post_subject"/>
    <association property="author" javaType="Author"/>
    <collection property="comments" ofType="Comment">
      <id property="id" column="comment_id"/>
    </collection>
    <collection property="tags" ofType="Tag" >
      <id property="id" column="tag_id"/>
    </collection>
    <discriminator javaType="int" column="draft">
      <case value="1" resultType="DraftPost"/>
    </discriminator>
  </collection>
</resultMap>
```



**resultMap元素有很多子元素和一个值得讨论的结构。下面是resultMap的元素的概念图。**
- constructor -用于注入结果到后一类的构造函数实例- idArg - ID参数标记结果作为ID，将有助于提高整体性能
- ARG -注入到构造一个正常的结果

- id -一个ID结果;标记结果作为ID，将有助于提高整体性能
- result -注入到字段或JavaBean属性的正常结果
- association -一个复杂的类型关联，许多结果将包成这种类型- 嵌套结果映射-关联是resultMap的 Ş自己，或者可以参考一

- collection -复杂类型的集合- 嵌套结果映射-集合的resultMap Ş本身，也可以是指一个

- discriminator -使用结果值来决定使用 这结果- case -这种情形结果地图- 嵌套结果映射 - 这种情形结果也映射本身，因此可以包含很多这些相同的元素，或者它可以参照一个外部的结果。



**resultMap的属性**




|属性|描述|
|----|----|
|id|此命名空间中的唯一标识符，可以用来引用此结果地图。|
|type|一个完全合格的Java类名，或一个类型别名（见上表内置类型别名的列表）。|
|autoMapping|如果存在，MyBatis将会启用或禁用自动映射此结果映射。此属性覆盖全球autoMappingBehavior的。默认：未设置状态。|

最佳实践通常逐步建立结果。单元测试的真正帮助在这里。如果您尝试建立一个以上一次像一个巨大的resultMap，有可能你会得到它错了，这将是艰苦的工作。从简单的开始，并在同一时间一步一步的发展。和单元测试！使用该框架的缺点是，他们有时有点一个黑盒子（开源与否）。你最好的赌注，以确保你的行为，你打算实现，编写单元测试。这也有助于让他们提交时的错误。



**8、ID和result**
``
<id property="id" column="post_id"/>

<result property="subject" column="post_subject"/>



这些都是最基本的结果映射。这两个ID， 结果到一个单一的一个简单的数据类型（字符串，整型，双，日期等）的属性或字段映射一个单独列的值。两者之间唯一的区别是ID 标记的结果作为标识符的属性时要使用比较对象实例。这有助于提高性能一般，但特别是在性能嵌套结果映射缓存和（即加盟映射）。每个人都有一些属性：


|Id和result属性| |
|----|----|
|属性|描述|
|property|映射到列结果的字段或属性。如果存在匹配的JavaBeans属性为给定的名称，然后将被使用。否则MyBatis将会寻找给定的名称进行了实地。在这两种情况下，你可以使用通常点式的复杂属性导航使用。例如，你可以映射到一些简单的如：用户名，或更复杂的东西，如：address.street.number。|
|column|从数据库中的列名，或重命名的列标签。这是相同的字符串，通常会被传递给resultSet.getString（columnName）方法的方法。|
|javaType|一个完全合格的Java类名，或一个类型别名（见上表内置类型别名的列表）。MyBatis通常可以断定类型，如果你映射到一个JavaBean。然而，如果你映射到的是HashMap，那么你应该明确地指定javaType来​​保证所需的行为。|
|jdbcType|JDBC类型支持的类型列表如下表。为空的列的JDBC类型是仅仅需要对插入，更新或删除。这是JDBC的需要，而不是MyBatis的。所以，即使你直接使用JDBC编程，你需要指定这个类型 - 但只有为空的值。|
|typeHandler|本文档中我们讨论了以前默认的类型处理器。使用这个属性，你可以覆盖默认的类型处理器的映射，映射的基础上。该值可以是一个完全合格的类名，一个类型处理器的实现，或者是类型别名。|

**9、支持的JDBC类型**

对于未来的参考，MyBatis的支持下面的JDBC类型通过包含的jdbcType枚举。


|BIT|FLOAT|CHAR|TIMESTAMP|OTHER|UNDEFINED|
|----|----|----|----|----|----|
|TINYINT|REAL|VARCHAR|BINARY|BLOG|NVARCHAR|
|SMALLINT|DOUBLE|LONGVARCHAR|VARBINARY|CLOB|NCHAR|
|INTEGER|NUMERIC|DATE|LONGVARBINARY|BOOLEAN|NCLOB|
|BIGINT|DECIMAL|TIME|NULL|CURSOR|ARRAY|

**10、构造器（constructor）**
<constructor>
   <idArg column="id" javaType="int"/>
   <arg column="username" javaType="String"/>
</constructor>虽然性能将大多数数据传输对象（DTO）类型类，可能是最您的域模型，可能会有一些情况下，你想使用一成不变的类。表通常包含引用或查找数据，适合对一成不变的类很少或从来没有改变。构造函数注入允许你设置值类的实例时，不暴露的公共方法。 MyBatis也支持私有属性和私有JavaBeans属性来实现这一目标，但有些人喜欢构造函数注入。该构造元素支持这个。考虑下面的构造函数：
```java
public class User {
   //...
   public User(int id, String username) {
     //...
  }
//...
}
```
为了注入结果到构造，MyBatis的需要确定其参数类型的构造。 Java没有办法射参数名称。因此，创建一个构造元素时，保证参数的顺序，数据类型指定。



```
<constructor>
   <idArg column="id" javaType="int"/>
   <arg column="username" javaType="String"/>
</constructor>
```
**构造器的属性描述：**



|属性|描述|
|----|----|
|column|从数据库中的列名，或重命名的列标签。这是相同的字符串，通常会被通过toresultSet.getString（columnName）方法。|
|javaType|一个完全合格的Java类名，或一个类型别名（见上表内置类型别名的列表）。 MyBatis通常可以断定类型，如果你映射到一个JavaBean。然而，如果你映射到的是HashMap，那么你应该明确地指定javaType来保证所需的行为。|
|jdbcType|JDBC类型支持的类型列表如下表。为空的列的JDBC类型是仅仅需要对插入，更新或删除。这是JDBC的需要，而不是MyBatis的。所以，即使你直接使用JDBC编程，你需要指定这个类型 - 但只有为空的值。|
|typeHandler|本文档中我们讨论了以前默认的类型处理器。使用这个属性，你可以覆盖默认的类型处理器的映射，映射的基础上。该值可以是一个完全合格的类名，一个类型处理器的实现，或者是类型别名。|
|select|另外一个映射语句的ID，将加载这个属性映射需要的复杂类型。检索列属性中指定列的值将被传递给目标select语句作为参数。见协元素为多。|
|resultMap|这是一个ResultMap嵌套结果这种说法，可以映射到一个合适的对象图的ID。这是一个替代另一个select语句调用。它允许你加入多个表，汇集成一个单一的ResultSet。这样的结果将包含重复的，重复组数据，需要分解和正确映射到一个嵌套的对象图。为了推动这项工作的MyBatis让你“链”结果地图一起，处理嵌套结果。请参阅以下Association元素更多。|
**11、关联关系（association）**
```
<association property="author" javaType="Author">
  <id property="id" column="author_id"/>
  <result property="username" column="author_username"/>
</association>
```
          关联元素处理“has-one”类型的关系。例如，在我们的例子中，一个博客有一个作者。一个关联映射作品大多像任何其他的结果。你指定了目标属性，java类型的的（MyBatis可以自己弄清楚的大部分时间），而且jdbcType如果必要和类型，如果你想覆盖的检索结果值。关联中不同的是，你需要告诉MyBatis如何加载关联。 MyBatis的两种不同的方式可以这样做：
嵌套查询：通过执行另外一个映射SQL语句返回预期的复杂类型。
嵌套结果：使用嵌套结果映射来处理重复的联合结果的子集。
让我们来看看的**元素属性**。正如你所看到的，它不同于一个正常的结果映射只能由select和resultMap属性。
|属性|描述|
|----|----|
|property|映射到列结果的字段或属性。如果存在匹配的JavaBeans属性为给定的名称，然后将被使用。否则MyBatis将会寻找给定的名称进行了实地。在这两种情况下，你可以使用通常点式的复杂属性导航使用。例如，你可以映射到一些简单的如：用户名，或更复杂的东西，如：address.street.number。|
|javaType|一个完全合格的Java类名，或一个类型别名（见上表内置类型别名的列表）。 MyBatis通常可以断定类型，如果你映射到一个JavaBean。然而，如果你映射到的是HashMap，那么你应该明确地指定javaType来保证所需的行为。|
|jdbcType|JDBC类型支持的类型列表如下表。为空的列的JDBC类型是仅仅需要对插入，更新或删除。这是JDBC的需要，而不是MyBatis的。所以，即使你直接使用JDBC编程，你需要指定这个类型 - 但只有为空的值。|
|typeHandler|本文档中我们讨论了以前默认的类型处理器。使用这个属性，你可以覆盖默认的类型处理器的映射，映射的基础上。该值可以是一个完全合格的类名，一个类型处理器的实现，或者是类型别名。|
**嵌套查询关联（Nested Select for Association）**



|属性|描述|
|----|----|
|column|从数据库中的列名或持有作为输入参数的值将被传递到嵌套的语句命名的列标签。这是相同的字符串，通常会被传递给resultSet.getString（columnName）方法的方法。注意：要处理复合键，您可以指定多个列名通过使用嵌套的select语句的语法列=“{名字为prop1= col1中，为prop2= col2的}”。 prop1和prop2的对参数设置为目标对象，这将导致嵌套SELECT语句。|
|select|另外一个映射语句的ID，将加载这个属性映射需要的复杂类型。检索列属性中指定列的值将被传递给目标select语句作为参数。一个详细的例子如下表。注意：要处理复合键，您可以指定多个列名通过使用嵌套的select语句的语法列=“{名字为prop1= col1中，为prop2= col2的}”。 prop1和prop2的对参数设置为目标对象，这将导致嵌套SELECT语句。|
例如：
```
<resultMap id="blogResult" type="Blog">
  <association property="author" column="author_id" javaType="Author" select="selectAuthor"/>
</resultMap>

<select id="selectBlog" parameterType="int" resultMap="blogResult">
  SELECT * FROM BLOG WHERE ID = #{id}
</select>

<select id="selectAuthor" parameterType="int" resultType="Author">
  SELECT * FROM AUTHOR WHERE ID = #{id}
</select>
```
我们有两个查询语句：一个来加载博客，另外一个来加载作者博客的结果说明，selectAuthor语句应该被用来加载它的author属性。所有其他属性将被自动加载，假设它们的列和属性名相匹配。虽然这种方法很简单，它不会执行以及大型数据集或列表。这个问题被称为“N +1选择问题” 。概括地说， N +1查询问题可以是这样引起的：

    （1）、你执行一个SQL语句检索记录（ “+1” ）的列表。

    （2）、对于返回的每条记录，你执行select语句加载细节（ “N”） 。

这个问题会导致在数百或上千的SQL语句来执行。这并不总是可取的。有利的一面是时MyBatis能延迟加载这样的查询，因此你可能幸免这些语句的成本。但是，如果你加载一个列表，然后立即遍历访问嵌套的数据，你将调用所有懒惰的负载，这样的行为可能会很糟糕。所以，是另一种方式。
**嵌套结果关联（Nested Results for Association）**



|属性|描述|
|----|----|
|resultMap|这是一个ResultMap可以映射到一个合适的对象图中的嵌套的结果，此关联的ID。这是一个替代另一个select语句调用。它允许你加入多个表，汇集成一个单一的ResultSet。这样的结果将包含重复的，重复组数据，需要分解和正确映射到一个嵌套的对象图。为了推动这项工作的MyBatis让你“链”结果地图一起，处理嵌套结果。一个例子会很容易跟进，如下表。|
|columnPrefix|连接多个表时，你将不得不使用列的别名，以避免重复ResultSet中列名。指定columnPrefix允许你这样的列映射到了外部的结果。请参阅本节后面的例子解释。|
|notNullColumn|默认情况下，创建子对象时，如果至少有一个孩子的属性映射到列不为null。有了这个属性，你可以改变这种行为，藉由指定的列必须有一个值，所以MyBatis将会创建一个子对象，如果任何这些列不为空。可以指定多个列名使用逗号作为分隔符。默认值：取消。|
你已经看到了一个非常复杂的嵌套关联的例子。下面是一个简单的例子来说明它如何工作。而是执行一个单独的语句中，我们将联合博客表和作者表在一起，像这样：
```
<select id="selectBlog" parameterType="int" resultMap="blogResult">
  select
    B.id            as blog_id,
    B.title         as blog_title,
    B.author_id     as blog_author_id,
    A.id            as author_id,
    A.username      as author_username,
    A.password      as author_password,
    A.email         as author_email,
    A.bio           as author_bio
  from Blog B left outer join Author A on B.author_id = A.id
  where B.id = #{id}
</select>
```
注意联接，小心确保所有的结果是一个独特而清晰的名字别名。这使得映射容易得多。现在，我们可以映射的结果：

```
<resultMap id="blogResult" type="Blog">
  <id property="id" column="blog_id" />
  <result property="title" column="blog_title"/>
  <association property="author" resultMap="authorResult" />
</resultMap>

<resultMap id="authorResult" type="Author">
  <id property="id" column="author_id"/>
  <result property="username" column="author_username"/>
  <result property="password" column="author_password"/>
  <result property="email" column="author_email"/>
  <result property="bio" column="author_bio"/>
</resultMap>
```
        在上面的例子中，你可以看到在博客的“作者”关联代表着“authorResult”结果映射来加载作者实例。非常重要：id元素扮演非常重要的角色，在嵌套结果映射。你应该总是指定一个或多个属性，可以用来唯一标识结果。事实是时MyBatis仍然可以工作，如果你离开它，但在严重的性能问题。选择的属性越少越好，可以唯一地标识结果。主键是一个显而易见的选择（即使复合）。

       现在，上面的例子中使用了外部的结果元素来映射关联。这使得Author结果可重复使用。但是，如果你没有需要重用它，或者如果你更喜欢合作，找到你的结果映射到一个单一的描述的结果，你可以嵌套结果映射。这是同样的例子，使用这种方法：

```
<resultMap id="blogResult" type="Blog">
  <id property="id" column="blog_id" />
  <result property="title" column="blog_title"/>
  <association property="author" javaType="Author">
    <id property="id" column="author_id"/>
    <result property="username" column="author_username"/>
    <result property="password" column="author_password"/>
    <result property="email" column="author_email"/>
    <result property="bio" column="author_bio"/>
  </association>
</resultMap>
```
如果博客有一个合著者？ select语句看起来像这样：

```
<select id="selectBlog" parameterType="int" resultMap="blogResult">
  select
    B.id            as blog_id,
    B.title         as blog_title,
    A.id            as author_id,
    A.username      as author_username,
    A.password      as author_password,
    A.email         as author_email,
    A.bio           as author_bio,
    CA.id           as co_author_id,
    CA.username     as co_author_username,
    CA.password     as co_author_password,
    CA.email        as co_author_email,
    CA.bio          as co_author_bio
  from Blog B
  left outer join Author A on B.author_id = A.id
  left outer join Author CA on B.co_author_id = CA.id
  where B.id = #{id}
</select>
```
回想一下，作者resultMap的定义如下。

```
<resultMap id="authorResult" type="Author">
  <id property="id" column="author_id"/>
  <result property="username" column="author_username"/>
  <result property="password" column="author_password"/>
  <result property="email" column="author_email"/>
  <result property="bio" column="author_bio"/>
</resultMap>
```
因为结果中的列名不同resultMap的定义的列，你需要指定columnPrefix重用resultMap的合著者映射结果。


```
<resultMap id="blogResult" type="Blog">
  <id property="id" column="blog_id" />
  <result property="title" column="blog_title"/>
  <association property="author"
    resultMap="authorResult" />
  <association property="coAuthor"
    resultMap="authorResult"
    columnPrefix="co_" />
</resultMap>
```
**多结果集关联（Multiple ResultSets for Association）**
|属性|描述|
|----|----|
|column|当使用多个结果集该属性指定的列（以逗号分隔），将与theforeignColumn识别家长和孩子的关系。|
|foreignColumn|标识包含国外的值将被对父类型列中的attibute指定的列的值匹配的键的列的名称。|
|resultSet|标识该复杂类型将被加载的结果集的名称。|
从3.2.3版的MyBatis提供了另一种方式来解决N +1问题。有些数据库允许返回多个结果集的存储过程或一次执行多个语句，并返回一个结果每每一个。这可以用来打一次数据库，并返回相关数据，而无需使用连接。在这个例子中，存储过程执行以下的查询和返回两个结果集。首先将包含博客和第二作者。


```
SELECT * FROM BLOG WHERE ID = #{id}

SELECT * FROM AUTHOR WHERE ID = #{id}
```
必须把这个名字给每个结果集加入映射的语句，结果集属性列表以逗号分隔的名字。



```
<select id="selectBlog" resultSets="blogs,authors" resultMap="blogResult">
  {call getBlogsAndAuthors(#{id,jdbcType=INTEGER,mode=IN})}
</select>
```
现在，我们可以指定的数据，以填补关联的“作者”是“作者”的结果集：

```
<resultMap id="blogResult" type="Blog">
  <id property="id" column="id" />
  <result property="title" column="title"/>
  <association property="author" javaType="Author" resultSet="authors" column="author_id" foreignColumn="id">
    <id property="id" column="id"/>
    <result property="username" column="username"/>
    <result property="password" column="password"/>
    <result property="email" column="email"/>
    <result property="bio" column="bio"/>
  </association>
</resultMap>
```
上面你已经看到如何处理“has one”类型关联。但是“has many”？这是下一节的主题。


**12、集合（collection）**


```
<collection property="posts" ofType="domain.blog.Post">
  <id property="id" column="post_id"/>
  <result property="subject" column="post_subject"/>
  <result property="body" column="post_body"/>
</collection>
```
集合元素的工作方式几乎相同的关联。事实上，它是如此的相似，文档的异同是多余的。因此，让我们专注于差异。要继续我们上面的例子中，一个博客只有一个作者。但是博客有很多文章。在博客类，这将代表类似：

```java
private List<Post> posts;
```
要映射嵌套结果集合到List中，我们使用集合元素。就像关联元素，我们可以使用一个嵌套的选择，从连接或者嵌套结果。


**嵌套查询集合（Nested Select for Collection）**

首先，让我们来看看使用嵌套选择加载文章的博客。




```
<resultMap id="blogResult" type="Blog">
  <collection property="posts" javaType="ArrayList" column="id" ofType="Post" select="selectPostsForBlog"/>
</resultMap>

<select id="selectBlog" parameterType="int" resultMap="blogResult">
  SELECT * FROM BLOG WHERE ID = #{id}
</select>

<select id="selectPostsForBlog" parameterType="int" resultType="Post">
  SELECT * FROM POST WHERE BLOG_ID = #{id}
</select>
```
有一些东西，你会立即通知，但在大多数情况下，它看起来非常相似，我们了解上述关联元素。首先，你会发现，我们正在使用的集合元素。然后，你会发现，有一个新的的的“ofType”属性。此属性是必要的JavaBean（或字段）属性类型和集合包含的类型来区分。所以，你可以阅读下面的这样的映射：


`<collection property="posts" javaType="ArrayList" column="id" ofType="Post" select="selectPostsForBlog"/>`读为：“在一个ArrayList类型发表的帖子的集合。”javaType属性是不需要的，因为在大多数情况下，MyBatis会为你算出来。所以你经常可以缩短下来简单：
`<collection property="posts" column="id" ofType="Post" select="selectPostsForBlog"/>`**嵌套集合（Nested Results for Collection）**

这一点，你大概可以猜测集合嵌套结果如何工作，因为它是作为一个关联完全相同，但除了具有相同的应用ofType属性。首先，让我们来看看在SQL：

```
<select id="selectBlog" parameterType="int" resultMap="blogResult">
  select
  B.id as blog_id,
  B.title as blog_title,
  B.author_id as blog_author_id,
  P.id as post_id,
  P.subject as post_subject,
  P.body as post_body,
  from Blog B
  left outer join Post P on B.id = P.blog_id
  where B.id = #{id}
</select>
```
同样，我们已经联合了博客表和文章表，并已采取谨慎措施，确保质量，结果列标签的简单映射。现在其博客文章映射集合映射是非常简单的：



```
<resultMap id="blogResult" type="Blog">
  <id property="id" column="blog_id" />
  <result property="title" column="blog_title"/>
  <collection property="posts" ofType="Post">
    <id property="id" column="post_id"/>
    <result property="subject" column="post_subject"/>
    <result property="body" column="post_body"/>
  </collection>
</resultMap>
```
同样，请记住这里的id元素的重要性，如果你还没有阅读上面的关联部分。另外，如果你喜欢更长的形式允许你的结果映射的更多重用，你可以使用以下的替代映射：




```
<resultMap id="blogResult" type="Blog">
  <id property="id" column="blog_id" />
  <result property="title" column="blog_title"/>
  <collection property="posts" ofType="Post" resultMap="blogPostResult" columnPrefix="post_"/>
</resultMap>

<resultMap id="blogPostResult" type="Post">
  <id property="id" column="id"/>
  <result property="subject" column="subject"/>
  <result property="body" column="body"/>
</resultMap>
```
**多个结果集合（Multiple ResultSets for Collection）**

正如我们所做的关联，我们可以调用一个存储过程执行两个查询，并返回两个结果集，一个博客和帖子：

```
SELECT * FROM BLOG WHERE ID = #{id}

SELECT * FROM POST WHERE BLOG_ID = #{id}
```
必须把这个名字给每个结果集加入映射的语句，结果集属性列表以逗号分隔的名字。

```
<select id="selectBlog" resultSets="blogs,posts" parameterType="int" resultMap="blogResult">
  {call getBlogsAndPosts(#{id,jdbcType=INTEGER,mode=IN})}
</select>
```
我们指定的数据包含在结果集名为“posts”，“posts”集合将被填掉：

```
<resultMap id="blogResult" type="Blog">
  <id property="id" column="id" />
  <result property="title" column="title"/>
  <collection property="posts" ofType="Post" resultSet="posts" column="id" foreignColumn="blog_id">
    <id property="id" column="id"/>
    <result property="subject" column="subject"/>
    <result property="body" column="body"/>
  </collection>
</resultMap>
```
注：关联和集合映射到的深度，广度或组合没有任何限制。它们映射时，你应该考虑性能。您的应用程序的单元测试和性能测试去发现最好的方法为您的应用程序很长的路要走。好事是myBatis让你改变了主意后，你的代码很少（如果有的话）的影响。

高级关联和集合映射是一个深度的主题。文档只能给你这么远。随着一点点的做法，它都将变得清晰非常快。
**13、鉴别（discriminator）**

```
<discriminator javaType="int" column="draft">
  <case value="1" resultType="DraftPost"/>
</discriminator>
```
        有时一个单独的数据库查询可能返回很多不同（但是希望有些关联）数据类型的结果集。鉴别元素被设计用来处理这种情况下，和其他人，包括类的继承层次结构。鉴别是非常简单的理解，因为它的表现很像Java中的switch语句。

        定义鉴别器指定了column和javaType属性。列是MyBatis将会寻找比较值的。 JavaType是要求，以确保平等测试的合适类型（尽管字符串可能会工作几乎任何情况下）。例如：

```
<resultMap id="vehicleResult" type="Vehicle">
  <id property="id" column="id" />
  <result property="vin" column="vin"/>
  <result property="year" column="year"/>
  <result property="make" column="make"/>
  <result property="model" column="model"/>
  <result property="color" column="color"/>
  <discriminator javaType="int" column="vehicle_type">
    <case value="1" resultMap="carResult"/>
    <case value="2" resultMap="truckResult"/>
    <case value="3" resultMap="vanResult"/>
    <case value="4" resultMap="suvResult"/>
  </discriminator>
</resultMap>
```
在这个例子中，MyBatis会从结果集中检索每个记录并比较其车辆类型值。如果它匹配任何鉴别个案，那么它会使用的情况下指定resultMap的。这样做完全是，所以换句话说，剩余的结果将被忽略（除非它被扩展，这是我们谈论在第二）。如果没有匹配的情况下，那么MyBatis仅仅使用鉴别器块外定义的结果。所以，如果carResult声明如下：




```
<resultMap id="carResult" type="Car">
  <result property="doorCount" column="door_count" />
</resultMap>
```
那么只有doorCount的属性将被加载。这样做是为了让完全独立的群体鉴别的情况下，甚至是那些没有任何关系和父结果。在这种情况下，我们当然知道，有一个汽车和汽车之间的关系，如Car是一个Vehicle。因此，我们希望剩下的属性也被加载。一个简单的变化的结果映射我们去。




```
<resultMap id="carResult" type="Car" extends="vehicleResult">
  <result property="doorCount" column="door_count" />
</resultMap>
```
现在，所有的属性，无论从vehicleResult carResult将被加载。尽管曾经有些人会发现这个外部定义的地图有点乏味。因此，那些喜欢更简洁的映射风格的另一种语法。例如：

```
<resultMap id="vehicleResult" type="Vehicle">
  <id property="id" column="id" />
  <result property="vin" column="vin"/>
  <result property="year" column="year"/>
  <result property="make" column="make"/>
  <result property="model" column="model"/>
  <result property="color" column="color"/>
  <discriminator javaType="int" column="vehicle_type">
    <case value="1" resultType="carResult">
      <result property="doorCount" column="door_count" />
    </case>
    <case value="2" resultType="truckResult">
      <result property="boxSize" column="box_size" />
      <result property="extendedCab" column="extended_cab" />
    </case>
    <case value="3" resultType="vanResult">
      <result property="powerSlidingDoor" column="power_sliding_door" />
    </case>
    <case value="4" resultType="suvResult">
      <result property="allWheelDrive" column="all_wheel_drive" />
    </case>
  </discriminator>
</resultMap>
```
注：请记住，这些都是结果映射，如果你不指定任何结果，那么MyBatis将自动匹配为你列和属性。所以大部分这些例子更详细的比他们真正需要的是。也就是说，大多数的数据库是很复杂的，它是不可能的，我们就可以依靠它的所有情况。
**14、自动映射（Auto-mapping）**

       正如你在前面的章节中已经看到，在简单的情况下， MyBatis的自动映射可以为您和他人的结果，你将需要建立一个结果地图。但是，正如你将看到在本节中，你也可以混合两种策略。让我们更深入地了解如何自动映射工作。当自动测绘成果MyBatis将会得到具有相同名称的忽略大小写的属性列名和期待。这意味着，如果一列名为ID和名为id的财产被发现， MyBatis会设置的ID列的值的id属性。一般数据库列命名使用大写字母和下划线字与字之间经常的驼峰命名covention的遵循Java属性。为了使它们之间的自动映射设置mapUnderscoreToCamelCase设置为true
 。

       自动映射工作，即使有一个具体结果地图。当发生这种情况时，每个结果地图， ResultSet中没有手动映射，是目前所有列将自动映射，然后手动映射将被处理。在下面的示例ID和用户名，列将自动映射和hashed_password的列将被映射。

```
<select id="selectUsers" parameterType="int" resultType="User">
  select
    user_id             as "id",
    user_name           as "userName",
    hashed_password
  from some_table
  where id = #{id}
</select>
```

```
<resultMap id="userResultMap" type="User">
  <result property="password" column="hashed_password"/>
</resultMap>
```



有三个自动映射水平：
- NONE -  禁用自动映射。只有手动映射属性将被设置。
- PARTIAL - 自动映射的结果，除了那些有嵌套结果映射内部定义（加入）。
- FULL -  自动映射一切。
默认值是局部的，它是如此的一个原因。当使用FULL的自动映射将进行处理的连接结果时，连接在同一行中的几个不同的实体，因此，这可能会导致不希望的映射检索数据。要了解风险看看下面的示例：

```
<select id="selectBlog" parameterType="int" resultMap="blogResult">
  select
    B.id,
    B.title,
    A.username,
  from Blog B left outer join Author A on B.author_id = A.id
  where B.id = #{id}
</select>
```

```
<resultMap id="blogResult" type="Blog">
  <association property="author" javaType="Author" resultMap="authorResult"/>
</resultMap>

<resultMap id="authorResult" type="Author">
  <result property="username" column="author_username"/>
</resultMap>
```
有了这个结果地图，博客和作者都将被自动映射。但是请注意，作者有一个id属性中有一列名为id的博客的ID，所以作者的ID将充满ResultSet中，是不是你所期望的。因此，谨慎使用FULL选项。
**15、缓存（cache）**
        MyBatis包含一个功能强大的交易查询缓存功能，这是非常可配置和可定制的。 MyBatis 3中的缓存实现已作出很多修改，使其更加强大而且易于配置。默认情况下，只是局部的session缓存，仅用于缓存数据在会话期间启用。要启用全球第二级缓存，你只需要你的SQL映射文件中添加一行：

`<cache/>`字面上看就是这样。这个简单语句的效果如下：

（1）、select语句中的映射语句文件中的所有结果将被缓存。

（2）、所有插入，更新和删除映射语句文件中的报表将刷新缓存。

（3）、缓存将使用最近最少使用算法（LRU）驱逐。

（4）、任何形式的基于时间排程（即没有刷新间隔），缓存不会冲洗。

（5）、缓存会存储列表或对象（无论查询方法返回什么）的1024个引用。

（6）、缓存会被视为作为一个读/写高速缓存，这意味着对象检索不是共享的，可以安全地被调用者修改，而不干扰其他调用者或线程所做的潜在修改。

所有这些属性是可以修改的通过缓存元素属性。例如：

```
<cache
  eviction="FIFO"
  flushInterval="60000"
  size="512"
  readOnly="true"/>
```
这个更高级的配置创建了一个FIFO缓存结果对象或列表，每60秒刷新一次，最多可存储512个引用，而且返回的对象被认为是只读，修改它们会导致调用者之间的冲突在不同的线程中。可用的收回策略有：
LRU - 最近最少使用的：移除长时间未使用的对象。

FIFO - 先进先出的顺序，他们进入了高速缓存中删除对象。

SOFT - 软引用：移除基于垃圾收集器状态和软引用规则的对象。

WEAK- 弱引用：更积极地移除基于垃圾收集器状态​​和弱引用规则的对象。

       默认的是LRU。flushInterval（刷新间隔）可以被设置为任意正整数，它们代表一个合理的时间量，以毫秒为单位指定。默认为不设置，因此没有刷新间隔，缓存仅仅调用语句时刷新。大小可以设置为任意正整数，记住你缓存的对象和你的环境可用的内存资源的大小。默认值是1024。

         只读属性可以设置为true或false。只读缓存缓存对象的实例将返回相同的对所有主叫方。因此，这样的对象不应当被修改。这提供了一个显着的性能优势。的读写缓存会返回缓存对象的拷贝（通过序列）。这是慢一些，但是安全，因此默认是假的。
注：第二级缓存是事务性的。这意味着它是一个的SqlSession的完成与提交或回滚，但没有插入/删除/更新与FLUSHCACHE= true其中执行的，当它完成时更新。 

**使用自定义缓存（Using a Custom Cache）**

除了自定义缓存在这些方面，你也可以完全覆盖缓存行为实现自己的缓存，或其他第三方缓存解决方案创建适配器。



`<cache type="com.domain.something.MyCustomCache"/>`此示例演示如何使用自定义的缓存实现。在type属性指定的类必须实现org.apache.ibatis.cache.Cache的接口，并提供了一个构造函数，得到一个字符串ID作为参数。这个接口是一个更复杂的MyBatis框架中，但简单的，因为它做什么。



```java
public interface Cache {
  String getId();
  int getSize();
  void putObject(Object key, Object value);
  Object getObject(Object key);
  boolean hasKey(Object key);
  Object removeObject(Object key);
  void clear();
  ReadWriteLock getReadWriteLock();
}
```
        要配置您的缓存和公有的JavaBeans属性，只需添加到你的缓存实现，并通过通过cache元素的属性，例如，下面的调用一个方法叫做setCacheFile（String file）你的缓存实现：

```
<cache type="com.domain.something.MyCustomCache">
  <property name="cacheFile" value="/tmp/my-custom-cache.tmp"/>
</cache>
```
 您可以使用所有简单类型的JavaBeans的属性，MyBatis会进行转换。重要的是要记住，缓存配置和缓存实例是绑定在SQL映射文件的命名空间。因此，同一个命名空间作为缓存中的所有语句都受其约束。语句可以修改它们如何相互作用与缓存，或完全排除它们通过一个语句的语句的基础上使用两种简单的属性。默认情况下，报表的配置是这样的：

```
<select ... flushCache="false" useCache="true"/>
<insert ... flushCache="true"/>
<update ... flushCache="true"/>
<delete ... flushCache="true"/>
```
由于默认情况下，你显然不应该显式配置方式一份声明中说。相反，只有设置flushCache和useCache属性，如果你想改变默认的行为。例如，在某些情况下，您可能要排除特定语句的结果从缓存中，或者你可能想选择语句来刷新缓存。同样的，你可能有一些更新语句执行时不需要刷新缓存。
**缓存引用（cache-ref）**

回想一下，从上一节这个特殊命名空间的唯一缓存会被使用或刷新同一个命名空间内的语句。有可能来的时候，你要在命名空间中共享相同的缓存配置和实例。在这种情况下，你可以通过使用缓存-ref元素引用另外一个缓存。




`<cache-ref namespace="com.someone.application.data.SomeMapper"/>`





