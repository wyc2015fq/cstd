# Spring JDBC Framework详解——批量JDBC操作、ORM映射 - z69183787的专栏 - CSDN博客
2012年12月28日 10:38:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1551
**一、Spring JDBC 概述**
     Spring 提供了一个强有力的模板类JdbcTemplate简化JDBC操作，DataSource,JdbcTemplate都可以以Bean的方式定义在想xml配置文件，JdbcTemplate创建只需注入一个DataSource，应用程序Dao层只需要继承JdbcDaoSupport, 或者注入JdbcTemplate，便可以获取JdbcTemplate，JdbcTemplate是一个线程安全的类，多个Dao可以注入一个JdbcTemplate；
<!--         Oracle数据源           -->
<bean id="dataSource" class="org.apache.commons.dbcp.BasicDataSource" destroy-method="close">
<property name="driverClassName" value="oracle.jdbc.driver.OracleDriver"/>
<property name="url" value="jdbc:oracle:thin:@oracle.devcake.co.uk:1521:INTL"/>
<property name="username" value="sa"/>
<property name="password" value=""/>
</bean>
<bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
<property name="dataSource" ref="dataSource"/>
</bean>
<!--  set注入方式获取jdbcTemplate -->
<bean id="customerDao" class="JdbcCustomerDao" >
<property name="jdbcTemplate" ref="jdbcTemplate"/>
</bean>
<!-- 注入dataSource，customerDao通过继承JdbcDaoSupport ,使用this.getJdbcTemplate()获取JdbcTemplate   -->
<bean id="customerDao" class="JdbcCustomerDao" >
<property name="dataSource" ref="dataSource"/>
</bean>
然后将jdbcTemplate对象注入自定义的Dao、或者继承JdbcDaoSupport，例如：
```java
public class JdbcCustomerDao extends JdbcDaoSupport implements CustomerDao {
}
public class JdbcCustomerDao implements CustomerDao {
private JdbcTemplate jdbcTemplate
public void setJdbcTemplate()JdbcTemplate jdbcTemplate{
this.jdbcTemplate=jdbcTemplate
}
}
```
**二、 JdbcTemplate 提供以下主要方法简化JDBC操作：**
**2.1、List query(String sql,Ojbect[] args,RowMapper rowMapper)**
     说明：常用的查询，sql待执行的sql语句，args是sql语句的参数，rowMapper负责将每一行记录转化为java对象存放在list，并最终返回，例如：
```java
public List<Book> queryByAuthor(String author) {
String sql = "select * from book where author=?";
Collection c = getJdoTemplate().find(sql,
new Object[] { author },new BookRowMapper());
List<Book> books = new ArrayList<Book>();
books.addAll(c);
return books;
}
class BookRowMapper implements RowMapper{
public Object mapRow(ResultSet res, int index) throws SQLException {
Book book = new Book();
book.setId(rs.getInt("id"));
//省略set
return book；
}
}
```
   更新、删除、其他查询操作类似，举例如下，详细细节请参考spring api：
```java
//返回值为一个长整形
public long getAverageAge() {
return getJdbcTemplate().queryForLong("SELECT AVG(age) FROM employee");
}
//返回一个整数
public int getTotalNumberOfEmployees() {
return getJdbcTemplate().queryForInt("SELECT COUNT(0) FROM employees");
}
//更新操作
this.jdbcTemplate.update(
"insert into t_actor (first_name, surname) values (?, ?)",
new Object[] {"Leonor", "Watling"});
```
 2.**2、spring 2.5新功能，另类的jdbc ORM：BeanPropertyRowMapper**
      上面我们检索时必须实现RowMapper，将结果集转化为java对象。Spring2.5 简化了这一操作，使得我们不必再实现RowMapper，实现此功能的俩个神奇东东便是：ParameterizedRowMapper，ParameterizedBeanPropertyRowMapper，貌似通过java反射机制实现了将resultset字段映射到java对象，但是数据表的列必须和java对象的属性对应，没有研究源码，有点类似于apache 的BeanUtil，不知为何这部分在spring开发参考手册没有，难道不是经典。
```java
//使用ParameterizedBeanPropertyRowMapper
@SuppressWarnings({"unchecked"})
public List<Customer> getAll() {
return getJdbcTemplate().query("select * from t_customer", ParameterizedBeanPropertyRowMapper.newInstance(Customer.class));
}
//使用BeanPropertyRowMapper
@SuppressWarnings({"unchecked"})
public List<Customer> getAll() {
return getJdbcTemplate().query("select * from t_customer", new BeanPropertyRowMapper(Customer.class));
}
```
注意：ParameterizedBeanPropertyRowMapper是BeanPropertyRowMapper子类。另外表的字段名称必须和实体类的成员变量名称一致；
**2.3、spring之JDBC批量操作**
jdbcTemplate.batchUpdate(final String[] sql) ，API解释：Issue multiple SQL updates on a single JDBC Statement using batching，翻译过来大致为：解决多个sql的插入、更新、删除操作在一个Statement中。性能一般。
   jdbcTemplate.batchUpdate(String sql, final BatchPreparedStatementSetter pss),类似于JDBC的PreparedStatement，性能较上着有所提高。
   我们举例说明如何使用，示例如下:
```java
final int count = 2000;
final List<String> firstNames = new ArrayList<String>(count);
final List<String> lastNames = new ArrayList<String>(count);
for (int i = 0; i < count; i++) {
firstNames.add("First Name " + i);
lastNames.add("Last Name " + i);
}
jdbcTemplate.batchUpdate(
"insert into customer (id, first_name, last_name, last_login, comments) values (?, ?, ?, ?, ?)",
new BatchPreparedStatementSetter() {
//为prepared statement设置参数。这个方法将在整个过程中被调用的次数
public void setValues(PreparedStatement ps, int i) throws SQLException {
ps.setLong(1, i + 10);
ps.setString(2, firstNames.get(i));
ps.setString(3, lastNames.get(i));
ps.setNull(4, Types.TIMESTAMP);
ps.setNull(5, Types.CLOB);
}
//返回更新的结果集条数
public int getBatchSize() {
return count;
}
});
}
```
  BatchSqlUpdate类是SqlUpdate 的子类，适用于插入、删除、更新批量操作，内部使用PreparedStatement，所以效率很高，批量语句达到设定的batchSize，或者手动调用flush才会执行批量操作。注意：此类是非线程安全的，必须为每个使用者创建一个实例，或者在同一个线程中使用前调用reset。
   下面我们举例说明如何使用BatchSqlUpdate，来执行批量操作。示例如下：
```java
class BatchInsert extends BatchSqlUpdate {
private static final String SQL = "insert into t_customer (id, first_name, last_name, last_login, "
+ "comments) values (?, ?, ?, ?, null)";
BatchInsert(DataSource dataSource) {
super(dataSource, SQL);
declareParameter(new SqlParameter(Types.INTEGER));
declareParameter(new SqlParameter(Types.VARCHAR));
declareParameter(new SqlParameter(Types.VARCHAR));
declareParameter(new SqlParameter(Types.TIMESTAMP));
setBatchSize(10);
}
}
```
```java
int count = 5000;
for (int i = 0; i < count; i++) {
batchInsert.update(new Object[] { i + 100L, "a" + i, "b" + i, null });
}
```
 至此，spring JDBC主要的应用基本上都简单罗列一番，所有代码均为文章举例，不是很严谨，仅为演示每一种用法，抛砖引玉，希望有独特见解的拍砖，有问题的请指明问题所在,谢谢
