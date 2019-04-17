# Github Mybatis深入学习之SQL生成类 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月11日 11:20:10[boonya](https://me.csdn.net/boonya)阅读数：4936








   MyBatis是怎样生成SQL语句的，我们对此非常好奇。所以还是来了解了解吧，原文地址：[http://mybatis.github.io/mybatis-3/statement-builders.html](http://mybatis.github.io/mybatis-3/statement-builders.html)

**1、问题（problem）**

         一个Java开发人员永远不会有最讨厌的事情之一是在Java代码中嵌入SQL。通常这样做是因为SQL可以动态生成 - 否则，你可能在一个文件或一个存储过程。正如你已经看到的，MyBatis有一个有力的回答，动态SQL生成XML映射功能。然而，有时有必要建立一个SQL语句的字符串里面的Java代码。在这种情况下，MyBatis有更多的功能来帮助你摆脱烦恼，减少自己典型的乱七八糟的加号，引号，换行，格式化的问题和嵌套的条件来处理多余的逗号和连词。事实上，在Java中动态生成SQL代码是一个真正的噩梦。例如：




```java
String sql = "SELECT P.ID, P.USERNAME, P.PASSWORD, P.FULL_NAME, "
"P.LAST_NAME,P.CREATED_ON, P.UPDATED_ON " +
"FROM PERSON P, ACCOUNT A " +
"INNER JOIN DEPARTMENT D on D.ID = P.DEPARTMENT_ID " +
"INNER JOIN COMPANY C on D.COMPANY_ID = C.ID " +
"WHERE (P.ID = A.ID AND P.FIRST_NAME like ?) " +
"OR (P.LAST_NAME like ?) " +
"GROUP BY P.ID " +
"HAVING (P.LAST_NAME like ?) " +
"OR (P.FIRST_NAME like ?) " +
"ORDER BY P.ID, P.FULL_NAME";
```



**2、解决方案（solution）**

         MyBatis的3提供了一个方便的实用程序工具类以解决此问题。随着SQL类，你只需创建一个实例，让你一次调用方法而不是一步就建立一个SQL语句。SQL类中重写时，上面的例子问题看起来像这样：




```java
private String selectPersonSql() {
  return new SQL() {{
    SELECT("P.ID, P.USERNAME, P.PASSWORD, P.FULL_NAME");
    SELECT("P.LAST_NAME, P.CREATED_ON, P.UPDATED_ON");
    FROM("PERSON P");
    FROM("ACCOUNT A");
    INNER_JOIN("DEPARTMENT D on D.ID = P.DEPARTMENT_ID");
    INNER_JOIN("COMPANY C on D.COMPANY_ID = C.ID");
    WHERE("P.ID = A.ID");
    WHERE("P.FIRST_NAME like ?");
    OR();
    WHERE("P.LAST_NAME like ?");
    GROUP_BY("P.ID");
    HAVING("P.LAST_NAME like ?");
    OR();
    HAVING("P.FIRST_NAME like ?");
    ORDER_BY("P.ID");
    ORDER_BY("P.FULL_NAME");
  }}.toString();
}
```
关于那个例子有什么特别之处呢？好吧，如果你仔细看，它并不担心意外复制“AND”关键字，或选择之间的“WHERE”和“AND”或根本没有。 SQL类需要关心理解的是其“WHERE”需要去“AND”应使用所有的字符串连接。


**3、SQL类**

       这里有一些例子：



```java
// Anonymous inner class
public String deletePersonSql() {
  return new SQL() {{
    DELETE_FROM("PERSON");
    WHERE("ID = ${id}");
  }}.toString();
}

// Builder / Fluent style
public String insertPersonSql() {
  String sql = new SQL()
    .INSERT_INTO("PERSON");
    .VALUES("ID, FIRST_NAME", "${id}, ${firstName}");
    .VALUES("LAST_NAME", "${lastName}")
    .toString();
  return sql;
}

// With conditionals (note the final parameters, required for the anonymous inner class to access them)
public String selectPersonLike(final String id, final String firstName, final String lastName) {
  return new SQL() {{
    SELECT("P.ID, P.USERNAME, P.PASSWORD, P.FIRST_NAME, P.LAST_NAME");
    FROM("PERSON P");
    if (id != null) {
      WHERE("P.ID like ${id}");
    }
    if (firstName != null) {
      WHERE("P.FIRST_NAME like ${firstName}");
    }
    if (lastName != null) {
      WHERE("P.LAST_NAME like ${lastName}");
    }
    ORDER_BY("P.LAST_NAME");
  }}.toString();
}

public String deletePersonSql() {
  return new SQL() {{
    DELETE_FROM("PERSON");
    WHERE("ID = ${id}");
  }}.toString();
}

public String insertPersonSql() {
  return new SQL() {{
    INSERT_INTO("PERSON");
    VALUES("ID, FIRST_NAME", "${id}, ${firstName}");
    VALUES("LAST_NAME", "${lastName}");
  }}.toString();
}

public String updatePersonSql() {
  return new SQL() {{
    UPDATE("PERSON");
    SET("FIRST_NAME = ${firstName}");
    WHERE("ID = ${id}");
  }}.toString();
}
```
|方法|描述|
|----|----|
|SELECT(String)|开始或附加SELECT子句。可以称为不止一次和参数将被追加到theSELECT条款。参数通常是一个逗号分隔的列表的列和别名，但可以是任何可以接受的驱动。|
|SELECT_DISTINCT(String)|启动或附加到一个SELECT子句，还增加了DISTINCT关键字生成的查询。可以称为不止一次和参数将被追加到SELECT子句。参数通常是一个逗号分隔的列表的列和别名，但可以是任何可接受的驱动。|
|FROM(String)|启动或追加到FROM子句。可以调用超过一次，将被追加的fromClause的参数。这些参数通常是一个表名和别名，或任何可接受的驱动程序。|
|· JOIN(String)· INNER_JOIN(String)· LEFT_OUTER_JOIN(String)RIGHT_OUTER_JOIN(String)|JOIN子句添加一个新的合适的类型，这取决于调用的方法。该参数可以包括一个标准的联接组成的列和条件下加入。|
|WHERE(String)|追加一个新的WHERE子句条件，并使用“AND”串连。可以多次调用，每次用and这会导致它连接新的条件。使用OR（）分裂ANOR。|
|OR()|拆分当前WHERE子句条件与OR。可以不止一次被调用，但调用不止一次在一排会产生额外的SQL。|
|AND()|拆分当前WHERE子句条件与AND。可以不止一次被调用，但调用不止一次在一排会产生额外SQL。因为同时具有自动串连，这是一个非常罕见的使用方法，只是真正包含完整性。|
|GROUP_BY(String)|追加一个新的GROUP BY子句元素，串连一个逗号。可以多次调用，这会导致它连接在新的条件，每次用逗号。|
|HAVING(String)|追加一个新的HAVING子句条件，串连。可以多次调用，这会导致它连接新的条件下，每次用and。使用OR（）分裂anOR。|
|ORDER_BY(String)|追加一个新的ORDER BY子句元素，串连一个逗号。可以多次调用，这会导致它连接在新的条件，每次用逗号。|
|DELETE_FROM(String)|启动DELETE语句指定表中删除。一般来说，这应遵循​​由WHERE语句！|
|INSERT_INTO(String)|启动Insert语句插入到指定表。此应遵循由一个或多个VALUES（）调用。|
|SET(String)|追加一个更新语句“SET”列表。|
|UPDATE(String)|始一个更新(UPDATE)语句，并指定表更新。这应遵循的一个或多个SET（）调用，通常一个WHERE（）调用。|
|VALUES(String, String)|追加的insert语句。第一个参数是要插入的列（次），第二个参数的值（s）。|
**4、SqlBuilder和SelectBuilder（不推荐使用）**


         在3.2版本之前，我们使用了一点不同的方法，通过使用一个ThreadLocal变量来掩盖一些语言的限制，使Java的DSL的有点麻烦。然而，这种方法现在已经过时，现代框架温暖了人们使用的建设者型模式和匿名内部类，伴随这样想法。因此，SelectBuilder和的SqlBuilder类已被废弃了。

下面的方法适用，只的过时的SqlBuilder和SelectBuilder类的。


|方法|描述|
|----|----|
|BEGIN()/RESET()|这些方法清除的SelectBuilder类ThreadLocal的状态，并准备要建一个新的语句。 BEGIN（）读取最好的时候，开始一个新语句。 RESET（）读取时最好清除语句在执行过程中，由于某种原因，（也许如果逻辑需要一个完全不同的说法，在某些条件下）。|
|SQL()|这将返回生成的SQL（）和重置SelectBuilder的状态（如果START（）或RESET（）被调用）。因此，这种方法只能被调用一次！|
        SelectBuilder和SqlBuilder类没那么神奇，但重要的是要知道他们是如何工作的。 SelectBuilder SqlBuilder使用静态导入一个ThreadLocal变量，使一个干净的语法，可以轻松地用条件交错的组合。要使用它们，你需要的静态导入是这样的（一个或另一个，但不能同时）类中的方法：



```java
import static org.apache.ibatis.jdbc.SelectBuilder.*;
```

```java
import static org.apache.ibatis.jdbc.SqlBuilder.*;
```
这使我们能够创造这样的方法：





```java
/* DEPRECATED */
public String selectBlogsSql() {
  BEGIN(); // Clears ThreadLocal variable
  SELECT("*");
  FROM("BLOG");
  return SQL();
}
```

```java
/* DEPRECATED */
private String selectPersonSql() {
  BEGIN(); // Clears ThreadLocal variable
  SELECT("P.ID, P.USERNAME, P.PASSWORD, P.FULL_NAME");
  SELECT("P.LAST_NAME, P.CREATED_ON, P.UPDATED_ON");
  FROM("PERSON P");
  FROM("ACCOUNT A");
  INNER_JOIN("DEPARTMENT D on D.ID = P.DEPARTMENT_ID");
  INNER_JOIN("COMPANY C on D.COMPANY_ID = C.ID");
  WHERE("P.ID = A.ID");
  WHERE("P.FIRST_NAME like ?");
  OR();
  WHERE("P.LAST_NAME like ?");
  GROUP_BY("P.ID");
  HAVING("P.LAST_NAME like ?");
  OR();
  HAVING("P.FIRST_NAME like ?");
  ORDER_BY("P.ID");
  ORDER_BY("P.FULL_NAME");
  return SQL();
}
```







