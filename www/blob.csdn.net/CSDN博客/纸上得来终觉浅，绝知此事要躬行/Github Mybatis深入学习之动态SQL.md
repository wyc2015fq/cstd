# Github Mybatis深入学习之动态SQL - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月09日 21:42:32[boonya](https://me.csdn.net/boonya)阅读数：3749








       动态SQL说的内容是根据输入条件决定的，所以需要对条件进行逻辑判断和处理。Mybatis提供了这样的标签来实现动态SQL的XML配置处理，实现了SQL语句和业务逻辑处理的兼容，而无需在代码中进行处理，完成了SQL的完美映射。

        原文地址：[http://mybatis.github.io/mybatis-3/dynamic-sql.html](http://mybatis.github.io/mybatis-3/dynamic-sql.html)

        MyBatis的最强大的功能之一就是动态SQL功能。如果您有任何经验与JDBC或任何类似的框架，你就明白条件地串联SQL字符串在一起，一定不要忘记空格或列的列表的末尾省略了一个逗号是多么痛苦。动态SQL可以彻底处理这种痛苦。当工作时绝不会使用动态SQL党，却肯定MyBatis提高的情况提供了强大的动态SQL语言，可以用在任何SQL映射语句。动态SQL元素应该与使用JSTL或任何类似的基于XML的文本处理器相似。 MyBatis的在以前的版本中，有很多元素的认识和了解。的MyBatis3大大提升，现在有不到一半的这些元素。
 MyBatis的采用功能强大的基于OGNL表达式来消除其他元素：


**if****choose (when, otherwise)****trim (where, set)****foreach**


**1、逻辑语句（if）**

最常见的事情在动态SQL有条件包括where子句中的一部分。例如：



```
<select id="findActiveBlogWithTitleLike"
     parameterType="Blog" resultType="Blog">
  SELECT * FROM BLOG
  WHERE state = ‘ACTIVE’
  <if test="title != null">
    AND title like #{title}
  </if>
</select>
```
这条语句会提供一个可选的文本查找功能。如果你没有传递title，那么所有激活的博客将被返回。但是，如果你传递一个标题，它会寻找这样的（敏锐的眼睛，是在这种情况下，你的参数值需要包括任何遮掩或通配符）标题。




```
<select id="findActiveBlogLike"
     parameterType="Blog" resultType="Blog">
  SELECT * FROM BLOG WHERE state = ‘ACTIVE’
  <if test="title != null">
    AND title like #{title}
  </if>
  <if test="author != null and author.name != null">
    AND author_name like #{author.name}
  </if>
</select>
```
**2、情形语句(choose, when, otherwise)**

有时候，我们不希望所有申请条件句，相反，我们要在众多选项中只能选择一个的情况下。类似Java中的switch语句，MyBatis的提供choose元素。让我们用上面的例子，但现在让我们只搜索标题。如果提供了，那么只能由作者提供；如果没有提供，我们只返回功能的博客（或许是战略性管理员选择列表，而不是返回大量没有意义的随机博客列表）。




```
<select id="findActiveBlogLike"
     parameterType="Blog" resultType="Blog">
  SELECT * FROM BLOG WHERE state = ‘ACTIVE’
  <choose>
    <when test="title != null">
      AND title like #{title}
    </when>
    <when test="author != null and author.name != null">
      AND author_name like #{author.name}
    </when>
    <otherwise>
      AND featured = 1
    </otherwise>
  </choose>
</select>
```
**3、条件语句（trim, where, set）**


前面的例子已经方便地演示了一个臭名昭著的动态SQL的挑战。考虑会发生什么，如果我们回到我们的“如果”的例子，但这次我们将“ACTIVE= 1”，以及一个动态的条件。




```
<select id="findActiveBlogLike"
     parameterType="Blog" resultType="Blog">
  SELECT * FROM BLOG
  WHERE
  <if test="state != null">
    state = #{state}
  </if>
  <if test="title != null">
    AND title like #{title}
  </if>
  <if test="author != null and author.name != null">
    AND author_name like #{author.name}
  </if>
</select>
```
如果没有满足条件，会发生什么事，你会像这样结束SQL？

```
SELECT * FROM BLOG
WHERE
```
这会失败。如果只满足第二个条件是什么，你会像这样结束SQL？

```
SELECT * FROM BLOG
WHERE
AND title like ‘someTitle’
```
这也将失败。不会轻易用条件来解决这个问题，如果你曾经不得不写它，那么你可能永远都不想再次这样做。MyBatis有一个简单的答案，这将可能在90％的情况下工作。在那些情况下，你不会那么自定义它。有了一个简单的变化，一切正常：

```
<select id="findActiveBlogLike"
     parameterType="Blog" resultType="Blog">
  SELECT * FROM BLOG
  <where>
    <if test="state != null">
         state = #{state}
    </if>
    <if test="title != null">
        AND title like #{title}
    </if>
    <if test="author != null and author.name != null">
        AND author_name like #{author.name}
    </if>
  </where>
</select>
```
其中元素知道只有插入“WHERE”如果有包含标签返回的任何内容。此外，如果该内容与“AND”或“OR”开头，它知道剥离其关闭。如果where元素的行为只要你喜欢的不完全相同，您可以自定义自己的装饰元素。例如，trim相当于where元素：

```
<trim prefix="WHERE" prefixOverrides="AND |OR ">
  ...
</trim>
```
prefixOverrides属性需要一个管道分隔的列表的文本重写，其中空白有关。其结果是除去指定任何在prefixOverrides属性，和插入任何前缀属性。有一个类似的解决方案和动态更新语句称为set。 set元素可以用来动态地包含列更新，脱离其他的。例如：

```
<update id="updateAuthorIfNecessary"
       parameterType="domain.blog.Author">
  update Author
    <set>
      <if test="username != null">username=#{username},</if>
      <if test="password != null">password=#{password},</if>
      <if test="email != null">email=#{email},</if>
      <if test="bio != null">bio=#{bio}</if>
    </set>
  where id=#{id}
</update>
```
在这里，set元素会动态前置SET关键字，还可以消除任何无关的逗号，应用条件后，可能落后的价值分配。如果你好奇相等的trim元素看起来会像什么，那就是：

```
<trim prefix="SET" suffixOverrides=",">
  ...
</trim>
```
请注意，在这种情况下，我们要覆盖后缀，同时我们还附加前缀。
**3、遍历（foreach）**


另一种常见的动态SQL的必要性是需要遍历一个集合，通常是构建在IN条件。例如：


```
<select id="selectPostIn" resultType="domain.blog.Post">
  SELECT *
  FROM POST P
  WHERE ID in
  <foreach item="item" index="index" collection="list"
      open="(" separator="," close=")">
        #{item}
  </foreach>
</select>
```
       foreach元素是非常强大的，它允许你指定一个集合，申报项目和索引变量，可用于身体内的元素。它还允许你打开和关闭指定的字符串，添加分隔符，在迭代之间放置。元素是很智能的，它不会偶然地附加多余的分隔符。
 注意：您可以传递一个List实例或者数组作为参数对象MyBatis的。当你这样做的时候，MyBatis将会自动包裹在Map和keyname。键入List实例名称“列表”和“数组”的名称将键入的数组实例。这包XML配置文件和XML映射文件的讨论。下一节将详细讨论Java API，所以，你可以得到最出你已经创建的映射。



**4、绑定（bind）**

bind元素可让你创建一个变量出OGNL表达式的值，并将其绑定到上下文。例如：



```
<select id="selectBlogsLike" parameterType="Blog" resultType="Blog">
  <bind name="pattern" value="'%' + _parameter.getTitle() + '%'" />
  SELECT * FROM BLOG
  WHERE title LIKE #{pattern}
</select>
```
**5、多数据库厂商支持（Multi-db vendor support）**

如果一个databaseIdProvider是配置了“_databaseId”的变量是动态代码，所以你可以建立不同的语句，根据数据库供应商。看看下面的例子：





```
<insert id="insert" parameterType="org.myapp.domain.User">
  <selectKey keyProperty="id" resultType="int" order="BEFORE">
    <if test="_databaseId == 'oracle'">
      select seq_users.nextval from dual
    </if>
    <if test="_databaseId == 'db2'">
      select nextval for seq_users from sysibm.sysdummy1"
    </if>
  </selectKey>
  insert into users values (#{id}, #{name})
</insert>
```
**6、可插拔动态SQL的脚本语言（Pluggable Scripting Languages For Dynamic SQL）**

从3.2版的MyBatis支持可插拔的脚本语言，所以你可以插入一个语言驱动程序，并使用该语言编写动态SQL查询。有两个内置的语言：




|别名|驱动|
|----|----|
|xml|XmlLanguageDriver|
|raw|RawLanguageDriver|
XML语言是默认的。它是能够执行，我们在前面的章节中看到所有的动态标签。实际上是在原始语言的语言的情况下。当使用此设置MyBatis的执行参数替换语句传递到数据库的驱动程序。正如你可能猜到的，原始的语言是速度远远超过可扩展标记语言（XML）。您可以更改默认语言为您的项目通过配置MyBatis-config.xml文件：



```
<settings>
  <setting name="defaultScriptingLanguage" value="raw"/>
</settings>
```
您也可以指定你的语言要使用在一个特定的语句中加入lang属性，如下：

```
<select id="selectBlog" lang="raw">
  SELECT * FROM BLOG
</select>
```
或者，如果你使用的是映射器，使用@Lang注解：

```java
public interface Mapper {
  @Lang(RawLanguageDriver.class)
  @Select("SELECT * FROM BLOG")
  List<Blog> selectBlog();
}
```
注：您可以使用Apache Velocity作为动态语言。有看看在MyBatis的速度的项目细节。您也可以实现你自己的语言驱动程序实现以下接口：

```java
public interface LanguageDriver {
  ParameterHandler createParameterHandler(MappedStatement mappedStatement, Object parameterObject, BoundSql boundSql);
  SqlSource createSqlSource(Configuration configuration, XNode script, Class<?> parameterType);
  SqlSource createSqlSource(Configuration configuration, String script, Class<?> parameterType);
}
```






