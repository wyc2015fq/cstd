# J2EE规范——JDBC - Big Smile - CSDN博客
2017年01月11日 11:38:43[王啸tr1912](https://me.csdn.net/tr1912)阅读数：437标签：[j2ee																[jdbc](https://so.csdn.net/so/search/s.do?q=jdbc&t=blog)](https://so.csdn.net/so/search/s.do?q=j2ee&t=blog)
个人分类：[J2EE](https://blog.csdn.net/tr1912/article/category/6675351)
# 一、什么是JDBC
         JDBC是以统一方式访问数据库的API。
        它提供了独立于平台的数据库访问,也就是说,有了JDBC API,我们就不必为访问Oracle数据库专门写一个程序,为访问Sybase数据库又专门写一个程序等等,只需要用JDBC API写一个程序就够了,它可以向相应数据库发送SQL调用.JDBC是Java应用程序与各种不同数据库之间进行对话的方法的机制.简单地说,它做了三件事:与数据库建立连接--发送操作数据库的语句--处理结果。
# 二、使用步骤
1.注册驱动 (只做一次)
方式一：Class.forName(“com.MySQL.jdbc.Driver”);
推荐这种方式，不会对具体的驱动类产生依赖。
方式二：DriverManager.registerDriver(com.mysql.jdbc.Driver);
会造成DriverManager中产生两个一样的驱动，并会对具体的驱动类产生依赖。
方式三：System.setProperty(“jdbc.drivers”, “driver1:driver2”);
虽然不会对具体的驱动类产生依赖；但注册不太方便，所以很少使用。 
驱动类型(四种类型)
2.建立连接(Connection)
```java
Connection conn = DriverManager.getConnection(url, user, password);
```
3.创建执行SQL的语句(Statement)
```java
Statement  
Statement st = conn.createStatement();  
st.executeQuery(sql);  
PreparedStatement  
String sql = “select * from table_name where col_name=?”;  
PreparedStatement ps = conn.preparedStatement(sql);  
ps.setString(1, “col_value”);  
ps.executeQuery();
```
4.处理执行结果(ResultSet)
```java
ResultSet rs = statement.executeQuery(sql);  
While(rs.next()){  
    rs.getString(“col_name”);  
    rs.getInt(“col_name”);  
    //…  
}
```
5.释放资源
        释放ResultSet, Statement,Connection.
        数据库连接（Connection）是非常稀有的资源，用完后必须马上释放，如果Connection不能及时正确的关闭将导致系统宕机。Connection的使用原则是尽量晚创建，尽量早的释放。
# 三、与ODBC比较
        以上是JAVA代码和数据库连接的一种方法，其实我们也可以通过Windows自带的数据源工具来建立一个数据源，然后通过连接数据源的方法来连接数据库。其实纵观这两种连接的方法，都是有一个公司为我们封装的一种数据库连接的方法，只不过他们遵循的标准不是特别的一样，内部调用可能也是不太一样，这里我们在回忆一下我们还在VB里面使用的数据源然后转型到.net里面的ADO.net的时候的发现，真的是一切都不一样了，从调用数据的方法，到连接的关闭，相信JAVA的JDBC能让我更加兴奋。
