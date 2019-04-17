# JavaWeb —— 数据库编程（JDBC） - Alex_McAvoy的博客 - CSDN博客





2018年11月01日 23:42:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：54
所属专栏：[JavaWeb 学习笔记](https://blog.csdn.net/column/details/34361.html)









# 【概述】

JDBC，是一套面向对象的应用程序接口，制定了统一的访问各种关系数据库的接口，为各数据库厂商提供了标准接口的实现。

通过 JDBC 技术，可以用纯 Java 与标准 SQL 语句编写完整的数据库应用程序，且真正的实现了软件的跨平台性。

JDBC 是一种底层的 API，在访问数据库时需要在业务逻辑中直接嵌入 SQL 语句，由于 SQL 语句是面向关系的，依赖于关系模型，因此 JDBC 传承了简单直接的优点，但需要注意的是，JDBC 不能直接访问数据库，必须依赖于数据库厂商提供的 JDBC 驱动程序来完成。

驱动程序完成工作：
-  同数据库建立连接
- 向数据库发送 SQL 语句
- 处理从数据库返回的结果

# 【常用类与接口】

## 1.Driver 接口

每种数据库的驱动程序都提供了一个实现 java.sql.Driver 接口的类，在加载某一驱动程序的 Driver 类时，其应创建自己的实例并向 java.sql.DriverManager 类注册该实例。

通常情况下，通过 java.lang.Class 类的静态方法 forName(String className) 加载与欲连接数据库的 Driver 类，该方法的入口参数为欲加载类的完整路径，加载成功后，会将 Driver 类实例注册到 DriverManager 类中，若加载失败，将抛出异常。

```java
//装载MySQL驱动
Class.forName("com.mysql.jdbc.Driver");
//装载Oracle驱动
Class.forName("oracle.jdbc.driver.OracleDriver");
```

## 2.DriverManager 类

DriverManager 类是管理 JDBC 驱动程序的基本服务，是 JDBC 的管理层，作用于用户与驱动程序之间负责追踪可用的驱动程序，并在数据库和驱动程序之间建立连接，除此之外，该类也常被用于处理驱动程序登录时间限制、登录与追踪消息的显示等工作。

成功加载 Driver 类实例并在 DriverManager 类中注册后，DriverManager 类即可用于建立数据库连接，当调用 DriverManager 类的 getConnection() 方法请求建立数据库时，DriverManager 类将定位一个适当的 Driver 类，并检查是否可以建立连接，如果可以则建立连接，否则将抛出异常。

该类提供的常用方法：
- getConnection(String url,String user,String password)：获取数据库连接，参数分别为要连接数据库的 URL、用户名、密码，返回类型为 java.sql.Connectoin
- setLoginTimeout(int seconds)：设置每次等待建立数据库连接的最长时间
- setLogWriter(PrintWriter out)：设置日志输出对象
- println(String message)：输出指定消息到当前 JDBC 日志流

```java
//连接MySql数据库
Connection conn = DriverManager.getConnection("jdbc:mysql://host:port/database", "user", "password");

//连接Oracle数据库
Connection conn = DriverManager.getConnection("jdbc:oracle:thin:@host:port:database", "user", "password");

//连接SqlServer数据库
Connection conn = DriverManager.getConnection("jdbc:microsoft:sqlserver://host:port; DatabaseName=database", "user", "password");
```

## 3.Connection 接口

java.sql.Connectoin 接口用于与特定数据库的连接，在连接的上下文中可以执行 SQL 语句并返回结果，还可通过 gerMetaData() 方法获得由数据库提供的相关信息，如数据表、存储过程、连接功能等信息。

该接口提供的常用方法： 
- createStatement()：创建一个 Statement 实例，执行无参的 SQL 语句时使用。
- prepareStatement() ：创建一个 PrepareSatement 实例，并对 SQL 语句进行预编译处理，执行含参的 SQL 语句时使用。
- prepareCall()：创建一个 CallableStatement 实例，调用数据库存储过程时使用。
- setReadOnly()：设置当前 Connection 实例的读取模式，默认为非只读模式，有一 boolean 型参数入口，true 表示开启只读模式，false 表示关闭只读模式。
- isReadOnly()：查看当前 Connection 实例是否处于只读模式，若是则返回 true，否则返回 false
- setAutoCommit()：设置当前 Connection 实例的自动提交模式，默认为 true，即自动将更改同步到数据库中，若设为 false，则需 commit() 或 rollback() 方法手动更改同步到数据库中。
- getAutoCommit()：查看当前 Connection 实例是否处于自动提交模式，若是则返回 true，否则返回 false
- commit() ：将上一次提交或回滚以来进行的所有更改同步到数据库，并释放 Connection 实例当前拥有的所有数据库锁定。
- rollback() ：取消当前事物中的所有更改，并释放当前 Connection 实例拥有的所有数据库锁定。
- setSavepoint()：在当前事物中创建一 Savepoint 实例，前提条件是当前的 Connection 实例不能处于自动提交模式，否则抛出异常。
- releaseSavapoint()：从当前事物中移除指定的 Savepoint 实例
- close()：释放 Connection 实例占用的数据库和 JDBC 资源，即关闭数据库连接。
- isClosed()：查看当前 Connection 实例是否被关闭，若是则返回 true，否则返回 false

## 4.Statement 接口

java.sql.Statement 接口用来执行静态 SQL 语句并返回执行结果。

其有三个 Statement 接口：
- Statement：由 createStatement() 创建，用于发送简单的不带参数的 SQL 语句。
- PreparedStatement ：继承自 Statement 接口，由 preparedStatement() 创建，用于发送含有一或多个参数的 SQL 语句，其效率要比 Statement 效率更高，并且可以防止 SQL 注入，因此一般使用 PreparedStatement
- CallableStatement：继承自 PreparedStatement 接口，由方法 prepareCall 创建，用于执行 SQL 的存储过程。0.21

### **1）Statement 接口**
- execute(String sql)：运行指定语句，返回是否有结果。
- executeQuery(String sql)：运行静态 SELECT 语句，返回 ResultSet 实例。
- executeUpdate(String sql)：运行 INSERT/UPDATE/DELETE 语句，返回更新的行数。
- addBatch(String sql) ：把多条 SQL 语句放到一个批处理(Batch)中，若驱动程序不支持批量处理则抛出异常。
- executeBatch()：执行批处理(Batch) 的 SQL 语句，若全部执行成功，返回由更新计数组成的数组，数组元素排序与语句添加顺序对应，若驱动程序不支持批量处理或未能成功执行 Batch 中的 SQL 语句之一则抛出异常。

	数组元素情况：大于等于 0 ：语句执行成功，数为更新的行数；-2：SQL 语句执行成功，但未得到受影响的行数；-3：SQL 语句执行失败，仅当执行失败后继续执行后面的 SQL 语句时出现
- clearBatch()：清楚位于 Batch 中的所有 SQL 语句，若驱动程序不支持批量处理则抛出异常。
- close()：立即释放 Statement 实例占用的数据库和 JDBC 资源。

### **2）PreparedStatement 接口**

PreparedStatement 接口常用于执行动态的 SQL 语句，即包含参数的 SQL 语句，通过 PreparedStatement 实例执行的动态 SQL 语句将被预编译并保存到 PreparedStatement 实例中，从而可以反复且高效的执行该 SQL 语句。

PreparedStatement 接口的方法除继承自 Statement 接口方法之外，还有用于为参数赋值的 setXXX(int i,XXX j) 方法：将指定参数设为 XXX 型，对应参数的 SQL 类型设为 XXX 的相应类型（JDBC 定义了从 Object 类到 SQL 类型的标准映射关系，在向数据库发送时将转换为相应的 SQL 类型），其中第一个参数 i 为欲赋值参数的索引值(从1开始)

当为参数赋值时，建议利用与参数类型匹配的方法，如：setDouble(int i,double x)，将指定参数 i 设为 double 型，对应参数的 SQL 类型为 DOUBLE

除 setXXX(int i,XXX j) 方法外，还可使用 setObject(int i,Object obj) 方法为各种类型的参数赋值。

若想清除所有参数的值，可以使用 clearParameters() 方法

### 3）CallableStatement 接口

CallableStatement 接口继承自 PreparedStatement 接口，用于执行 SQL 的存储过程。

为参数赋值的方法使用了自 PreparedStatement 接口中继承来的 setXXX() 方法，但在执行之前，需要注册所有的 OUT 参数类型，它们的值是在执行后通过 getXXX() 方法得来的。 

## 5.ResultSet 接口

java.sql.ResultSet 接口类似于一个线性表，通过该接口实例可以获得检索结果集，以及对应数据表的相关信息（列名、类型等），ResultSet 实例通过执行查询数据库的语句形成。

ResultSet 实例具有指向当前数据行的指针，最初指针指向第一行记录，通过 next() 方法可以将指针移动到下一行，若存在下一行，返回 true，否则返回 false，因此可以通过 while 循环来迭代 ResultSet 结果集。

默认情况下，ResultSet 实例不可更新，只能移动指针，所以只能迭代一次，并且只能按从前向后的顺序，如果需要，可以生成可滚动、可更新的 ResultSet 实例。

ResultSet 提供了从当前行检索不同类型列值的 getXXX() 方法，均有两个重载，分别是根据列的索引编号和列的名称指定列，进行检索，其中以列的索引编号检索效率较高，对于不同的 getXXX() 方法，JDBC 驱动程序会将基础数据类类型转换为与 getXXX() 方法相应的 Java 类型并返回。

此外，其提供了用于更新的 updateXXX() 方法，也均有两个重载，分别根据列的索引编号和列的名称指定列，进行更新。该方法可以用来更新当前行的指定列，也可以用来初始化要插入行的指定列，但该方法并未将操作同步到数据库，需要自行 updateRow() 或 insertRow() 方法来完成同步操作。

ResultSet 接口还提供了对结果集进行滚动的方法：
- next()：移动到下一行
- Previous()：移动到前一行
- absolute(int row)：移动到指定行
- beforeFirst()：移动 resultSet 的最前面。
- afterLast() ：移动到 resultSet 的最后面。

使用后依次关闭对象及连接：ResultSet → Statement → Connection

# 【使用 JDBC 步骤】

**基本过程**

加载 JDBC 驱动程序（Driver、DriverManager，仅需一次） → 建立数据库连接（Connection） → 创建执行 SQL 的语句（Statement） → 处理执行结果（ResultSet） → 释放资源

## 1.加载 JDBC 驱动

在与数据库建立连接之前，必须先加载欲连接数据库的驱动程序到 JVM 中，即加载数据库的驱动程序提供的实现 java.sql.Driver 接口的类，加载方法是通过 java.lang.Class 类的静态方法 forName(String className) 加载。

加载成功后，会将加载的驱动类注册给 DriverManager 类，若加载失败，将抛出未找到驱动类的异常(ClassNotFoundException)，因此要在加载数据库驱动类时捕捉可能抛出的异常。

由于仅需在第一次访问数据库时加载，故将负责加载数据库驱动的代码放在 static 代码块中，避免重复加载数据库驱动，造成对资源的浪费，同时还提高访问数据库的速度。

```java
public class JDBC{
    static{
        try{
            Class.forName("com.micrsoft.sqlserver.jdbc.SQLServerDriver");
        } catch(ClassNotFoundException e){
            System.out.println("数据库驱动加载失败");
            e.printStackTrace();
        }
    }
}
```

## 2.创建数据库连接

java.sql.DriverManager 类是 JDBC 的管理层，其负责建立和管理数据库的连接。

通过 DriverManager 的静态方法 getConnection(String url,String user,String password) 可以建立数据库连接，其返回类型是 java.sql.Connectoin

URL 用于标识数据库的位置，通过 URL 地址告诉 JDBC 程序连接哪个数据库，URL 的写法为：

![](https://img-blog.csdnimg.cn/20181107235241954.png)

其他参数如：useUnicode=true&characterEncoding=utf8

```java
private static final String URL="jdbc:mysql://host:port/database";
private static final String USER="root";
private static final String PASSWORD="123456";
try{
    Connection conn = DriverManager.getConnection(URL,USER,PASSWORD);
} catch(SQLException e){
    System.out.println("创建数据库连接失败");
    e.printStackTrace();
}
```

## 3.执行 SQL 语句

建立数据库连接(Connection)的目的是与数据库进行通信，实现方法为执行 SQL 语句，但通过 Connection 实例部分执行 SQL 语句，还需要通过 Connection 实例创建 Statement 实例，根据具体需要选择不同的 Statement、PreparedStatement、CallableStatement 实例进行操作。

```java
String id="5";
String sql="delete from table where id="+id;
Statement st=conn.createStatement();  
st.executeQuery(sql);
```

## 4.处理执行结果

在执行完 SQL 语句后，需要对其结果进行处理，此时需要用到 ResultSet 接口来进行操作。

```java
ResultSet rs = ps.executeQuery();  
While(rs.next()){  
    rs.getString(“col_name”);  
    rs.getInt(1);  
    //...
}
```

## 5.释放资源

建立 Connection、Statement、ResultSet 实例时，均需占用一定的数据库和 JDBC 资源，所以每次访问数据库后，应及时销毁这些实例，释放它们所占用的资源。

释放资源的方法是运行各个实例的 close() 方法，建议按照以下的顺序来结束：

```java
resultSet.close();
statement.close();
connection.close();
```





