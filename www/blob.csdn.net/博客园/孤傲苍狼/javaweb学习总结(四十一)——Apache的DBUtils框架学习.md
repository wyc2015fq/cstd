# javaweb学习总结(四十一)——Apache的DBUtils框架学习 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [javaweb学习总结(四十一)——Apache的DBUtils框架学习](https://www.cnblogs.com/xdp-gacl/p/4007225.html)



## 一、commons-dbutils简介　

　　commons-dbutils 是 Apache 组织提供的一个开源 JDBC工具类库，它是对JDBC的简单封装，学习成本极低，并且使用dbutils能极大简化jdbc编码的工作量，同时也不会影响程序的性能。因此dbutils成为很多不喜欢hibernate的公司的首选。

　　commons-dbutilsAPI介绍：
- org.apache.commons.dbutils.QueryRunner
- org.apache.commons.dbutils.ResultSetHandler

　　工具类
- org.apache.commons.dbutils.DbUtils

## 二、QueryRunner类使用讲解

　　该类简单化了SQL查询，它与ResultSetHandler组合在一起使用可以完成大部分的数据库操作，能够大大减少编码量。
　　QueryRunner类提供了两个构造方法：
- 默认的构造方法
- 需要一个 javax.sql.DataSource 来作参数的构造方法。

### 2.1、QueryRunner类的主要方法

　　public Object query(Connection conn, String sql, Object[] params, ResultSetHandler rsh) throws SQLException：执行一个查询操作，在这个查询中，对象数组中的每个元素值被用来作为查询语句的置换参数。该方法会自行处理 PreparedStatement 和 ResultSet 的创建和关闭。
　　public Object query(String sql, Object[] params, ResultSetHandler rsh) throws SQLException:　几乎与第一种方法一样；唯一的不同在于它不将数据库连接提供给方法，并且它是从提供给构造方法的数据源(DataSource) 或使用的setDataSource 方法中重新获得 Connection。
　　public Object query(Connection conn, String sql, ResultSetHandler rsh) throws SQLException : 执行一个不需要置换参数的查询操作。
　　public int update(Connection conn, String sql, Object[] params) throws SQLException:用来执行一个更新（插入、更新或删除）操作。
　　public int update(Connection conn, String sql) throws SQLException：用来执行一个不需要置换参数的更新操作。

### 2.2、使用QueryRunner类实现CRUD

```
1 package me.gacl.test;
  2 
  3 import java.util.Date;
  4 import java.util.List;
  5 import java.io.File;
  6 import java.io.FileReader;
  7 import java.io.IOException;
  8 import java.sql.SQLException;
  9 import javax.sql.rowset.serial.SerialClob;
 10 import me.gacl.domain.User;
 11 import me.gacl.util.JdbcUtils;
 12 import org.apache.commons.dbutils.QueryRunner;
 13 import org.apache.commons.dbutils.handlers.BeanHandler;
 14 import org.apache.commons.dbutils.handlers.BeanListHandler;
 15 import org.junit.Test;
 16 
 17 /**
 18 * @ClassName: DBUtilsCRUDTest
 19 * @Description:使用dbutils框架的QueryRunner类完成CRUD,以及批处理
 20 * @author: 孤傲苍狼
 21 * @date: 2014-10-5 下午4:56:44
 22 *
 23 */ 
 24 public class QueryRunnerCRUDTest {
 25 
 26     /*
 27      *测试表
 28      create table users(
 29          id int primary key auto_increment, 
 30          name varchar(40),
 31          password varchar(40), 
 32          email varchar(60), 
 33          birthday date 
 34      );
 35      */
 36     
 37     @Test
 38     public void add() throws SQLException {
 39         //将数据源传递给QueryRunner，QueryRunner内部通过数据源获取数据库连接
 40         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 41         String sql = "insert into users(name,password,email,birthday) values(?,?,?,?)";
 42         Object params[] = {"孤傲苍狼","123", "gacl@sina.com", new Date()};
 43         //Object params[] = {"白虎神皇","123", "gacl@sina.com", "1988-05-07"};
 44         qr.update(sql, params);
 45     }
 46     
 47     @Test
 48     public void delete() throws SQLException {
 49 
 50         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 51         String sql = "delete from users where id=?";
 52         qr.update(sql, 1);
 53 
 54     }
 55 
 56     @Test
 57     public void update() throws SQLException {
 58         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 59         String sql = "update users set name=? where id=?";
 60         Object params[] = { "ddd", 5};
 61         qr.update(sql, params);
 62     }
 63 
 64     @Test
 65     public void find() throws SQLException {
 66         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 67         String sql = "select * from users where id=?";
 68         Object params[] = {2};
 69         User user = (User) qr.query(sql, params, new BeanHandler(User.class));
 70         System.out.println(user.getBirthday());
 71     }
 72 
 73     @Test
 74     public void getAll() throws SQLException {
 75         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 76         String sql = "select * from users";
 77         List list = (List) qr.query(sql, new BeanListHandler(User.class));
 78         System.out.println(list.size());
 79     }
 80 
 81     /**
 82     * @Method: testBatch
 83     * @Description:批处理
 84     * @Anthor:孤傲苍狼
 85     *
 86     * @throws SQLException
 87     */ 
 88     @Test
 89     public void testBatch() throws SQLException {
 90         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 91         String sql = "insert into users(name,password,email,birthday) values(?,?,?,?)";
 92         Object params[][] = new Object[10][];
 93         for (int i = 0; i < 10; i++) {
 94             params[i] = new Object[] { "aa" + i, "123", "aa@sina.com",
 95                     new Date() };
 96         }
 97         qr.batch(sql, params);
 98     }
 99     
100     //用dbutils完成大数据（不建议用）
101     /***************************************************************************
102      create table testclob
103      (
104          id int primary key auto_increment,
105          resume text
106      );
107      **************************************************************************/
108     @Test
109     public void testclob() throws SQLException, IOException{
110         QueryRunner runner = new QueryRunner(JdbcUtils.getDataSource());
111         String sql = "insert into testclob(resume) values(?)";  //clob
112         //这种方式获取的路径，其中的空格会被使用“%20”代替
113         String path  = QueryRunnerCRUDTest.class.getClassLoader().getResource("data.txt").getPath();
114         //将“%20”替换回空格
115         path = path.replaceAll("%20", " ");
116         FileReader in = new FileReader(path);
117         char[] buffer = new char[(int) new File(path).length()];
118         in.read(buffer);
119         SerialClob clob = new SerialClob(buffer);
120         Object params[] = {clob};
121         runner.update(sql, params);
122     }
123 }
```

## 三、ResultSetHandler接口使用讲解

　　该接口用于处理java.sql.ResultSet，将数据按要求转换为另一种形式。
　　ResultSetHandler接口提供了一个单独的方法：Object handle (java.sql.ResultSet .rs)

### 3.1、ResultSetHandler接口的实现类
- ArrayHandler：把结果集中的**第一行**数据转成对象数组。
- ArrayListHandler：把结果集中的每一行数据都转成一个数组，再存放到List中。
- BeanHandler：将结果集中的**第一行**数据封装到一个对应的JavaBean实例中。
- BeanListHandler：将结果集中的每一行数据都封装到一个对应的JavaBean实例中，存放到List里。
- ColumnListHandler：将结果集中某一列的数据存放到List中。
- KeyedHandler(name)：将结果集中的每一行数据都封装到一个Map里，再把这些map再存到一个map里，其key为指定的key。
- MapHandler：将结果集中的第一行数据封装到一个Map里，key是列名，value就是对应的值。
- MapListHandler：将结果集中的每一行数据都封装到一个Map里，然后再存放到List

### 3.2、测试dbutils各种类型的处理器

```
1 package me.gacl.test;
  2 
  3 import java.sql.SQLException;
  4 import java.util.Arrays;
  5 import java.util.List;
  6 import java.util.Map;
  7 import me.gacl.util.JdbcUtils;
  8 import org.apache.commons.dbutils.QueryRunner;
  9 import org.apache.commons.dbutils.handlers.ArrayHandler;
 10 import org.apache.commons.dbutils.handlers.ArrayListHandler;
 11 import org.apache.commons.dbutils.handlers.ColumnListHandler;
 12 import org.apache.commons.dbutils.handlers.KeyedHandler;
 13 import org.apache.commons.dbutils.handlers.MapHandler;
 14 import org.apache.commons.dbutils.handlers.MapListHandler;
 15 import org.apache.commons.dbutils.handlers.ScalarHandler;
 16 import org.junit.Test;
 17 
 18 /**
 19 * @ClassName: ResultSetHandlerTest
 20 * @Description:测试dbutils各种类型的处理器
 21 * @author: 孤傲苍狼
 22 * @date: 2014-10-6 上午8:39:14
 23 *
 24 */ 
 25 public class ResultSetHandlerTest {
 26 
 27     @Test
 28     public void testArrayHandler() throws SQLException{
 29         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 30         String sql = "select * from users";
 31         Object result[] = (Object[]) qr.query(sql, new ArrayHandler());
 32         System.out.println(Arrays.asList(result));  //list  toString()
 33     }
 34     
 35     @Test
 36     public void testArrayListHandler() throws SQLException{
 37         
 38         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 39         String sql = "select * from users";
 40         List<Object[]> list = (List) qr.query(sql, new ArrayListHandler());
 41         for(Object[] o : list){
 42             System.out.println(Arrays.asList(o));
 43         }
 44     }
 45     
 46     @Test
 47     public void testColumnListHandler() throws SQLException{
 48         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 49         String sql = "select * from users";
 50         List list = (List) qr.query(sql, new ColumnListHandler("id"));
 51         System.out.println(list);
 52     }
 53     
 54     @Test
 55     public void testKeyedHandler() throws Exception{
 56         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 57         String sql = "select * from users";
 58         
 59         Map<Integer,Map> map = (Map) qr.query(sql, new KeyedHandler("id"));
 60         for(Map.Entry<Integer, Map> me : map.entrySet()){
 61             int  id = me.getKey();
 62             Map<String,Object> innermap = me.getValue();
 63             for(Map.Entry<String, Object> innerme : innermap.entrySet()){
 64                 String columnName = innerme.getKey();
 65                 Object value = innerme.getValue();
 66                 System.out.println(columnName + "=" + value);
 67             }
 68             System.out.println("----------------");
 69         }
 70     }
 71     
 72     @Test
 73     public void testMapHandler() throws SQLException{
 74         
 75         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 76         String sql = "select * from users";
 77         
 78         Map<String,Object> map = (Map) qr.query(sql, new MapHandler());
 79         for(Map.Entry<String, Object> me : map.entrySet())
 80         {
 81             System.out.println(me.getKey() + "=" + me.getValue());
 82         }
 83     }
 84     
 85     
 86     @Test
 87     public void testMapListHandler() throws SQLException{
 88         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
 89         String sql = "select * from users";
 90         List<Map> list = (List) qr.query(sql, new MapListHandler());
 91         for(Map<String,Object> map :list){
 92             for(Map.Entry<String, Object> me : map.entrySet())
 93             {
 94                 System.out.println(me.getKey() + "=" + me.getValue());
 95             }
 96         }
 97     }
 98     
 99     @Test
100     public void testScalarHandler() throws SQLException{
101         QueryRunner qr = new QueryRunner(JdbcUtils.getDataSource());
102         String sql = "select count(*) from users";  //[13]  list[13]
103         int count = ((Long)qr.query(sql, new ScalarHandler(1))).intValue();
104         System.out.println(count);
105     }
106 }
```

##  三、DbUtils类使用讲解

　　DbUtils ：提供如关闭连接、装载JDBC驱动程序等常规工作的工具类，里面的所有方法都是静态的。主要方法如下：
　　public static void close(…) throws java.sql.SQLException：　DbUtils类提供了三个重载的关闭方法。这些方法检查所提供的参数是不是NULL，如果不是的话，它们就关闭Connection、Statement和ResultSet。
　　public static void closeQuietly(…): 这一类方法不仅能在Connection、Statement和ResultSet为NULL情况下避免关闭，还能隐藏一些在程序中抛出的SQLEeception。
　　public static void commitAndCloseQuietly(Connection conn)： 用来提交连接，然后关闭连接，并且在关闭连接时不抛出SQL异常。 
　　public static boolean loadDriver(java.lang.String driverClassName)：这一方装载并注册JDBC驱动程序，如果成功就返回true。使用该方法，你不需要捕捉这个异常ClassNotFoundException。

## 四、JDBC开发中的事务处理

　　在开发中，对数据库的多个表或者对一个表中的多条数据执行更新操作时要保证对多个更新操作要么同时成功，要么都不成功，这就涉及到对多个更新操作的事务管理问题了。比如银行业务中的转账问题，A用户向B用户转账100元，假设A用户和B用户的钱都存储在Account表，那么A用户向B用户转账时就涉及到同时更新Account表中的A用户的钱和B用户的钱，用SQL来表示就是：

```
1 update account set money=money-100 where name='A'
2 update account set money=money+100 where name='B'
```

### 4.1、在数据访问层(Dao)中处理事务

　　对于这样的同时更新一个表中的多条数据的操作，那么必须保证要么同时成功，要么都不成功，所以需要保证这两个update操作在同一个事务中进行。在开发中，我们可能会在AccountDao写一个转账处理方法，如下：

```
1 /**
 2     * @Method: transfer
 3     * @Description:这个方法是用来处理两个用户之间的转账业务
 4     * 在开发中，DAO层的职责应该只涉及到CRUD，
 5     * 而这个transfer方法是处理两个用户之间的转账业务的，已经涉及到具体的业务操作，应该在业务层中做，不应该出现在DAO层的
 6     * 所以在开发中DAO层出现这样的业务处理方法是完全错误的
 7     * @Anthor:孤傲苍狼
 8     *
 9     * @param sourceName
10     * @param targetName
11     * @param money
12     * @throws SQLException
13     */ 
14     public void transfer(String sourceName,String targetName,float money) throws SQLException{
15         Connection conn = null;
16         try{
17             conn = JdbcUtils.getConnection();
18             //开启事务
19             conn.setAutoCommit(false);
20             /**
21              * 在创建QueryRunner对象时，不传递数据源给它，是为了保证这两条SQL在同一个事务中进行，
22              * 我们手动获取数据库连接，然后让这两条SQL使用同一个数据库连接执行
23              */
24             QueryRunner runner = new QueryRunner();
25             String sql1 = "update account set money=money-100 where name=?";
26             String sql2 = "update account set money=money+100 where name=?";
27             Object[] paramArr1 = {sourceName};
28             Object[] paramArr2 = {targetName};
29             runner.update(conn,sql1,paramArr1);
30             //模拟程序出现异常让事务回滚
31             int x = 1/0;
32             runner.update(conn,sql2,paramArr2);
33             //sql正常执行之后就提交事务
34             conn.commit();
35         }catch (Exception e) {
36             e.printStackTrace();
37             if(conn!=null){
38                 //出现异常之后就回滚事务
39                 conn.rollback();
40             }
41         }finally{
42             //关闭数据库连接
43             conn.close();
44         }
45     }
```

　　然后我们在AccountService中再写一个同名方法，在方法内部调用AccountDao的transfer方法处理转账业务，如下：

```
1 public void transfer(String sourceName,String targetName,float money) throws SQLException{
2         AccountDao dao = new AccountDao();
3         dao.transfer(sourceName, targetName, money);
4 }
```

　　上面AccountDao的这个transfer方法可以处理转账业务，并且保证了在同一个事务中进行，但是AccountDao的这个transfer方法是处理两个用户之间的转账业务的，已经涉及到具体的业务操作，应该在业务层中做，不应该出现在DAO层的，在开发中，DAO层的职责应该只涉及到基本的CRUD，不涉及具体的业务操作，所以在开发中DAO层出现这样的业务处理方法是一种不好的设计。

### 4.2、在业务层(BusinessService)处理事务

　　由于上述AccountDao存在具体的业务处理方法，导致AccountDao的职责不够单一，下面我们对AccountDao进行改造，让AccountDao的职责只是做CRUD操作，将事务的处理挪到业务层(BusinessService)，改造后的AccountDao如下：

```
1 package me.gacl.dao;
 2 
 3 import java.sql.Connection;
 4 import java.sql.SQLException;
 5 import org.apache.commons.dbutils.QueryRunner;
 6 import org.apache.commons.dbutils.handlers.BeanHandler;
 7 import me.gacl.domain.Account;
 8 import me.gacl.util.JdbcUtils;
 9 
10 /*account测试表
11 create table account(
12     id int primary key auto_increment,
13     name varchar(40),
14     money float
15 )character set utf8 collate utf8_general_ci;
16 
17 insert into account(name,money) values('A',1000);
18 insert into account(name,money) values('B',1000);
19 insert into account(name,money) values('C',1000);
20 
21 */
22 
23 /**
24 * @ClassName: AccountDao
25 * @Description: 针对Account对象的CRUD
26 * @author: 孤傲苍狼
27 * @date: 2014-10-6 下午4:00:42
28 *
29 */ 
30 public class AccountDao {
31 
32     //接收service层传递过来的Connection对象
33     private Connection conn = null;
34     
35     public AccountDao(Connection conn){
36         this.conn = conn;
37     }
38     
39     public AccountDao(){
40         
41     }
42     
43     /**
44     * @Method: update
45     * @Description:更新
46     * @Anthor:孤傲苍狼
47     *
48     * @param account
49     * @throws SQLException
50     */ 
51     public void update(Account account) throws SQLException{
52         
53         QueryRunner qr = new QueryRunner();
54         String sql = "update account set name=?,money=? where id=?";
55         Object params[] = {account.getName(),account.getMoney(),account.getId()};
56         //使用service层传递过来的Connection对象操作数据库
57         qr.update(conn,sql, params);
58         
59     }
60     
61     /**
62     * @Method: find
63     * @Description:查找
64     * @Anthor:孤傲苍狼
65     *
66     * @param id
67     * @return
68     * @throws SQLException
69     */ 
70     public Account find(int id) throws SQLException{
71         QueryRunner qr = new QueryRunner();
72         String sql = "select * from account where id=?";
73         //使用service层传递过来的Connection对象操作数据库
74         return (Account) qr.query(conn,sql, id, new BeanHandler(Account.class));
75     }
76 }
```

　　接着对AccountService(业务层)中的transfer方法的改造，在业务层(BusinessService)中处理事务

```
1 package me.gacl.service;
 2 
 3 import java.sql.Connection;
 4 import java.sql.SQLException;
 5 import me.gacl.dao.AccountDao;
 6 import me.gacl.domain.Account;
 7 import me.gacl.util.JdbcUtils;
 8 
 9 /**
10 * @ClassName: AccountService
11 * @Description: 业务逻辑处理层
12 * @author: 孤傲苍狼
13 * @date: 2014-10-6 下午5:30:15
14 *
15 */ 
16 public class AccountService {
17     
18     /**
19     * @Method: transfer
20     * @Description:这个方法是用来处理两个用户之间的转账业务
21     * @Anthor:孤傲苍狼
22     *
23     * @param sourceid
24     * @param tartgetid
25     * @param money
26     * @throws SQLException
27     */ 
28     public void transfer(int sourceid,int tartgetid,float money) throws SQLException{
29         Connection conn = null;
30         try{
31             //获取数据库连接
32             conn = JdbcUtils.getConnection();
33             //开启事务
34             conn.setAutoCommit(false);
35             //将获取到的Connection传递给AccountDao，保证dao层使用的是同一个Connection对象操作数据库
36             AccountDao dao = new AccountDao(conn);
37             Account source = dao.find(sourceid);
38             Account target = dao.find(tartgetid);
39             
40             source.setMoney(source.getMoney()-money);
41             target.setMoney(target.getMoney()+money);
42             
43             dao.update(source);
44             //模拟程序出现异常让事务回滚
45             int x = 1/0;
46             dao.update(target);
47             //提交事务
48             conn.commit();
49         }catch (Exception e) {
50             e.printStackTrace();
51             //出现异常之后就回滚事务
52             conn.rollback();
53         }finally{
54             conn.close();
55         }
56     }
57 }
```

　　程序经过这样改造之后就比刚才好多了，AccountDao只负责CRUD，里面没有具体的业务处理方法了，职责就单一了，而AccountService则负责具体的业务逻辑和事务的处理，需要操作数据库时，就调用AccountDao层提供的CRUD方法操作数据库。

### 4.3、使用ThreadLocal进行更加优雅的事务处理

　　上面的在businessService层这种处理事务的方式依然不够优雅，为了能够让事务处理更加优雅，我们使用ThreadLocal类进行改造，**ThreadLocal一个容器，向这个容器存储的对象，在当前线程范围内都可以取得出来，向ThreadLocal里面存东西就是向它里面的Map存东西的，然后ThreadLocal把这个Map挂到当前的线程底下，这样Map就只属于这个线程了**

　　ThreadLocal类的使用范例如下：

```
1 package me.gacl.test;
 2 
 3 public class ThreadLocalTest {
 4 
 5     public static void main(String[] args) {
 6         //得到程序运行时的当前线程
 7         Thread currentThread = Thread.currentThread();
 8         System.out.println(currentThread);
 9         //ThreadLocal一个容器，向这个容器存储的对象，在当前线程范围内都可以取得出来
10         ThreadLocal<String> t = new ThreadLocal<String>();
11         //把某个对象绑定到当前线程上 对象以键值对的形式存储到一个Map集合中，对象的的key是当前的线程，如： map(currentThread,"aaa")
12         t.set("aaa");
13         //获取绑定到当前线程中的对象
14         String value = t.get();
15         //输出value的值是aaa
16         System.out.println(value);
17     }
18 }
```

　　使用使用ThreadLocal类进行改造数据库连接工具类JdbcUtils，改造后的代码如下：

```
1 package me.gacl.util;
  2 
  3 import java.sql.Connection;
  4 import java.sql.SQLException;
  5 import javax.sql.DataSource;
  6 import com.mchange.v2.c3p0.ComboPooledDataSource;
  7 
  8 /**
  9 * @ClassName: JdbcUtils2
 10 * @Description: 数据库连接工具类
 11 * @author: 孤傲苍狼
 12 * @date: 2014-10-4 下午6:04:36
 13 *
 14 */ 
 15 public class JdbcUtils2 {
 16     
 17     private static ComboPooledDataSource ds = null;
 18     //使用ThreadLocal存储当前线程中的Connection对象
 19     private static ThreadLocal<Connection> threadLocal = new ThreadLocal<Connection>();
 20     
 21     //在静态代码块中创建数据库连接池
 22     static{
 23         try{
 24             //通过代码创建C3P0数据库连接池
 25             /*ds = new ComboPooledDataSource();
 26             ds.setDriverClass("com.mysql.jdbc.Driver");
 27             ds.setJdbcUrl("jdbc:mysql://localhost:3306/jdbcstudy");
 28             ds.setUser("root");
 29             ds.setPassword("XDP");
 30             ds.setInitialPoolSize(10);
 31             ds.setMinPoolSize(5);
 32             ds.setMaxPoolSize(20);*/
 33             
 34             //通过读取C3P0的xml配置文件创建数据源，C3P0的xml配置文件c3p0-config.xml必须放在src目录下
 35             //ds = new ComboPooledDataSource();//使用C3P0的默认配置来创建数据源
 36             ds = new ComboPooledDataSource("MySQL");//使用C3P0的命名配置来创建数据源
 37             
 38         }catch (Exception e) {
 39             throw new ExceptionInInitializerError(e);
 40         }
 41     }
 42     
 43     /**
 44     * @Method: getConnection
 45     * @Description: 从数据源中获取数据库连接
 46     * @Anthor:孤傲苍狼
 47     * @return Connection
 48     * @throws SQLException
 49     */ 
 50     public static Connection getConnection() throws SQLException{
 51         //从当前线程中获取Connection
 52         Connection conn = threadLocal.get();
 53         if(conn==null){
 54             //从数据源中获取数据库连接
 55             conn = getDataSource().getConnection();
 56             //将conn绑定到当前线程
 57             threadLocal.set(conn);
 58         }
 59         return conn;
 60     }
 61     
 62     /**
 63     * @Method: startTransaction
 64     * @Description: 开启事务
 65     * @Anthor:孤傲苍狼
 66     *
 67     */ 
 68     public static void startTransaction(){
 69         try{
 70             Connection conn =  threadLocal.get();
 71             if(conn==null){
 72                 conn = getConnection();
 73                  //把 conn绑定到当前线程上
 74                 threadLocal.set(conn);
 75             }
 76             //开启事务
 77             conn.setAutoCommit(false);
 78         }catch (Exception e) {
 79             throw new RuntimeException(e);
 80         }
 81     }
 82     
 83     /**
 84     * @Method: rollback
 85     * @Description:回滚事务
 86     * @Anthor:孤傲苍狼
 87     *
 88     */ 
 89     public static void rollback(){
 90         try{
 91             //从当前线程中获取Connection
 92             Connection conn = threadLocal.get();
 93             if(conn!=null){
 94                 //回滚事务
 95                 conn.rollback();
 96             }
 97         }catch (Exception e) {
 98             throw new RuntimeException(e);
 99         }
100     }
101     
102     /**
103     * @Method: commit
104     * @Description:提交事务
105     * @Anthor:孤傲苍狼
106     *
107     */ 
108     public static void commit(){
109         try{
110             //从当前线程中获取Connection
111             Connection conn = threadLocal.get();
112             if(conn!=null){
113                 //提交事务
114                 conn.commit();
115             }
116         }catch (Exception e) {
117             throw new RuntimeException(e);
118         }
119     }
120     
121     /**
122     * @Method: close
123     * @Description:关闭数据库连接(注意，并不是真的关闭，而是把连接还给数据库连接池)
124     * @Anthor:孤傲苍狼
125     *
126     */ 
127     public static void close(){
128         try{
129             //从当前线程中获取Connection
130             Connection conn = threadLocal.get();
131             if(conn!=null){
132                 conn.close();
133                  //解除当前线程上绑定conn
134                 threadLocal.remove();
135             }
136         }catch (Exception e) {
137             throw new RuntimeException(e);
138         }
139     }
140     
141     /**
142     * @Method: getDataSource
143     * @Description: 获取数据源
144     * @Anthor:孤傲苍狼
145     * @return DataSource
146     */ 
147     public static DataSource getDataSource(){
148         //从数据源中获取数据库连接
149         return ds;
150     }
151 }
```

　　对AccountDao进行改造，数据库连接对象不再需要service层传递过来，而是直接从JdbcUtils2提供的getConnection方法去获取，改造后的AccountDao如下：

```
1 package me.gacl.dao;
 2 
 3 import java.sql.Connection;
 4 import java.sql.SQLException;
 5 import org.apache.commons.dbutils.QueryRunner;
 6 import org.apache.commons.dbutils.handlers.BeanHandler;
 7 import me.gacl.domain.Account;
 8 import me.gacl.util.JdbcUtils;
 9 import me.gacl.util.JdbcUtils2;
10 
11 /*
12 create table account(
13     id int primary key auto_increment,
14     name varchar(40),
15     money float
16 )character set utf8 collate utf8_general_ci;
17 
18 insert into account(name,money) values('A',1000);
19 insert into account(name,money) values('B',1000);
20 insert into account(name,money) values('C',1000);
21 
22 */
23 
24 /**
25 * @ClassName: AccountDao
26 * @Description: 针对Account对象的CRUD
27 * @author: 孤傲苍狼
28 * @date: 2014-10-6 下午4:00:42
29 *
30 */ 
31 public class AccountDao2 {
32 
33     public void update(Account account) throws SQLException{
34         
35         QueryRunner qr = new QueryRunner();
36         String sql = "update account set name=?,money=? where id=?";
37         Object params[] = {account.getName(),account.getMoney(),account.getId()};
38         //JdbcUtils2.getConnection()获取当前线程中的Connection对象
39         qr.update(JdbcUtils2.getConnection(),sql, params);
40         
41     }
42     
43     public Account find(int id) throws SQLException{
44         QueryRunner qr = new QueryRunner();
45         String sql = "select * from account where id=?";
46         //JdbcUtils2.getConnection()获取当前线程中的Connection对象
47         return (Account) qr.query(JdbcUtils2.getConnection(),sql, id, new BeanHandler(Account.class));
48     }
49 }
```

　　对AccountService进行改造，service层不再需要传递数据库连接Connection给Dao层，改造后的AccountService如下：

```
1 package me.gacl.service;
 2 
 3 import java.sql.SQLException;
 4 import me.gacl.dao.AccountDao2;
 5 import me.gacl.domain.Account;
 6 import me.gacl.util.JdbcUtils2;
 7 
 8 public class AccountService2 {
 9     
10     /**
11     * @Method: transfer
12     * @Description:在业务层处理两个账户之间的转账问题
13     * @Anthor:孤傲苍狼
14     *
15     * @param sourceid
16     * @param tartgetid
17     * @param money
18     * @throws SQLException
19     */ 
20     public void transfer(int sourceid,int tartgetid,float money) throws SQLException{
21         try{
22             //开启事务，在业务层处理事务，保证dao层的多个操作在同一个事务中进行
23             JdbcUtils2.startTransaction();
24             AccountDao2 dao = new AccountDao2();
25             
26             Account source = dao.find(sourceid);
27             Account target = dao.find(tartgetid);
28             source.setMoney(source.getMoney()-money);
29             target.setMoney(target.getMoney()+money);
30             
31             dao.update(source);
32             //模拟程序出现异常让事务回滚
33             int x = 1/0;
34             dao.update(target);
35             
36             //SQL正常执行之后提交事务
37             JdbcUtils2.commit();
38         }catch (Exception e) {
39             e.printStackTrace();
40             //出现异常之后就回滚事务
41             JdbcUtils2.rollback();
42         }finally{
43             //关闭数据库连接
44             JdbcUtils2.close();
45         }
46     }
47 }
```

　　这样在service层对事务的处理看起来就更加优雅了。ThreadLocal类在开发中使用得是比较多的，**程序运行中产生的数据要想在一个线程范围内共享，只需要把数据使用ThreadLocal进行存储即可**。

### 4.4、ThreadLocal + Filter 处理事务

　　上面介绍了JDBC开发中事务处理的3种方式，下面再介绍的一种使用ThreadLocal + Filter进行统一的事务处理，这种方式主要是使用过滤器进行统一的事务处理，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201410/070808249216296.png)

**1、编写一个事务过滤器TransactionFilter**

代码如下：

```
1 package me.gac.web.filter;
 2 
 3 import java.io.IOException;
 4 import java.sql.Connection;
 5 import java.sql.SQLException;
 6 import javax.servlet.Filter;
 7 import javax.servlet.FilterChain;
 8 import javax.servlet.FilterConfig;
 9 import javax.servlet.ServletException;
10 import javax.servlet.ServletRequest;
11 import javax.servlet.ServletResponse;
12 import javax.servlet.http.HttpServletRequest;
13 import javax.servlet.http.HttpServletResponse;
14 import me.gacl.util.JdbcUtils;
15 
16 /**
17 * @ClassName: TransactionFilter
18 * @Description:ThreadLocal + Filter 统一处理数据库事务
19 * @author: 孤傲苍狼
20 * @date: 2014-10-6 下午11:36:58
21 *
22 */ 
23 public class TransactionFilter implements Filter {
24 
25     @Override
26     public void init(FilterConfig filterConfig) throws ServletException {
27 
28     }
29 
30     @Override
31     public void doFilter(ServletRequest request, ServletResponse response,
32             FilterChain chain) throws IOException, ServletException {
33 
34         Connection connection = null;
35         try {
36             //1、获取数据库连接对象Connection
37             connection = JdbcUtils.getConnection();
38             //2、开启事务
39             connection.setAutoCommit(false);
40             //3、利用ThreadLocal把获取数据库连接对象Connection和当前线程绑定
41             ConnectionContext.getInstance().bind(connection);
42             //4、把请求转发给目标Servlet
43             chain.doFilter(request, response);
44             //5、提交事务
45             connection.commit();
46         } catch (Exception e) {
47             e.printStackTrace();
48             //6、回滚事务
49             try {
50                 connection.rollback();
51             } catch (SQLException e1) {
52                 e1.printStackTrace();
53             }
54             HttpServletRequest req = (HttpServletRequest) request;
55             HttpServletResponse res = (HttpServletResponse) response;
56             //req.setAttribute("errMsg", e.getMessage());
57             //req.getRequestDispatcher("/error.jsp").forward(req, res);
58             //出现异常之后跳转到错误页面
59             res.sendRedirect(req.getContextPath()+"/error.jsp");
60         }finally{
61             //7、解除绑定
62             ConnectionContext.getInstance().remove();
63             //8、关闭数据库连接
64             try {
65                 connection.close();
66             } catch (SQLException e) {
67                 e.printStackTrace();
68             }
69         }
70     }
71 
72     @Override
73     public void destroy() {
74 
75     }
76 }
```

　　我们在TransactionFilter中把获取到的数据库连接使用ThreadLocal绑定到当前线程之后，在DAO层还需要从ThreadLocal中取出数据库连接来操作数据库，因此需要编写一个ConnectionContext类来存储ThreadLocal，ConnectionContext类的代码如下：

```
1 package me.gac.web.filter;
 2 
 3 import java.sql.Connection;
 4 
 5 /**
 6 * @ClassName: ConnectionContext
 7 * @Description:数据库连接上下文
 8 * @author: 孤傲苍狼
 9 * @date: 2014-10-7 上午8:36:01
10 *
11 */ 
12 public class ConnectionContext {
13 
14     /**
15      * 构造方法私有化，将ConnectionContext设计成单例
16      */
17     private ConnectionContext(){
18         
19     }
20     //创建ConnectionContext实例对象
21     private static ConnectionContext connectionContext = new ConnectionContext();
22     
23     /**
24     * @Method: getInstance
25     * @Description:获取ConnectionContext实例对象
26     * @Anthor:孤傲苍狼
27     *
28     * @return
29     */ 
30     public static ConnectionContext getInstance(){
31         return connectionContext;
32     }
33     
34     /**
35     * @Field: connectionThreadLocal
36     *         使用ThreadLocal存储数据库连接对象
37     */ 
38     private ThreadLocal<Connection> connectionThreadLocal = new ThreadLocal<Connection>();
39     
40     /**
41     * @Method: bind
42     * @Description:利用ThreadLocal把获取数据库连接对象Connection和当前线程绑定
43     * @Anthor:孤傲苍狼
44     *
45     * @param connection
46     */ 
47     public void bind(Connection connection){
48         connectionThreadLocal.set(connection);
49     }
50     
51     /**
52     * @Method: getConnection
53     * @Description:从当前线程中取出Connection对象
54     * @Anthor:孤傲苍狼
55     *
56     * @return
57     */ 
58     public Connection getConnection(){
59         return connectionThreadLocal.get();
60     }
61     
62     /**
63     * @Method: remove
64     * @Description: 解除当前线程上绑定Connection
65     * @Anthor:孤傲苍狼
66     *
67     */ 
68     public void remove(){
69         connectionThreadLocal.remove();
70     }
71 }
```

　　在DAO层想获取数据库连接时，就可以使用ConnectionContext.getInstance().getConnection()来获取，如下所示：

```
1 package me.gacl.dao;
 2 
 3 import java.sql.SQLException;
 4 import org.apache.commons.dbutils.QueryRunner;
 5 import org.apache.commons.dbutils.handlers.BeanHandler;
 6 
 7 import me.gac.web.filter.ConnectionContext;
 8 import me.gacl.domain.Account;
 9 
10 /*
11 create table account(
12     id int primary key auto_increment,
13     name varchar(40),
14     money float
15 )character set utf8 collate utf8_general_ci;
16 
17 insert into account(name,money) values('A',1000);
18 insert into account(name,money) values('B',1000);
19 insert into account(name,money) values('C',1000);
20 
21 */
22 
23 /**
24 * @ClassName: AccountDao
25 * @Description: 针对Account对象的CRUD
26 * @author: 孤傲苍狼
27 * @date: 2014-10-6 下午4:00:42
28 *
29 */ 
30 public class AccountDao3 {
31 
32     public void update(Account account) throws SQLException{
33         
34         QueryRunner qr = new QueryRunner();
35         String sql = "update account set name=?,money=? where id=?";
36         Object params[] = {account.getName(),account.getMoney(),account.getId()};
37         //ConnectionContext.getInstance().getConnection()获取当前线程中的Connection对象
38         qr.update(ConnectionContext.getInstance().getConnection(),sql, params);
39         
40     }
41     
42     public Account find(int id) throws SQLException{
43         QueryRunner qr = new QueryRunner();
44         String sql = "select * from account where id=?";
45         //ConnectionContext.getInstance().getConnection()获取当前线程中的Connection对象
46         return (Account) qr.query(ConnectionContext.getInstance().getConnection(),sql, id, new BeanHandler(Account.class));
47     }
48 }
```

　　businessService层也不用处理事务和数据库连接问题了，这些统一在TransactionFilter中统一管理了，businessService层只需要专注业务逻辑的处理即可，如下所示：

```
1 package me.gacl.service;
 2 
 3 import java.sql.SQLException;
 4 import me.gacl.dao.AccountDao3;
 5 import me.gacl.domain.Account;
 6 
 7 public class AccountService3 {
 8     
 9     /**
10     * @Method: transfer
11     * @Description:在业务层处理两个账户之间的转账问题
12     * @Anthor:孤傲苍狼
13     *
14     * @param sourceid
15     * @param tartgetid
16     * @param money
17     * @throws SQLException
18     */ 
19     public void transfer(int sourceid, int tartgetid, float money)
20             throws SQLException {
21         AccountDao3 dao = new AccountDao3();
22         Account source = dao.find(sourceid);
23         Account target = dao.find(tartgetid);
24         source.setMoney(source.getMoney() - money);
25         target.setMoney(target.getMoney() + money);
26         dao.update(source);
27         // 模拟程序出现异常让事务回滚
28         int x = 1 / 0;
29         dao.update(target);
30     }
31 }
```

　　Web层的Servlet调用businessService层的业务方法处理用户请求，需要注意的是：调用businessService层的方法出异常之后，继续将异常抛出，这样在TransactionFilter就能捕获到抛出的异常，继而执行事务回滚操作，如下所示：

```
1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 import java.sql.SQLException;
 5 import javax.servlet.ServletException;
 6 import javax.servlet.http.HttpServlet;
 7 import javax.servlet.http.HttpServletRequest;
 8 import javax.servlet.http.HttpServletResponse;
 9 import me.gacl.service.AccountService3;
10 
11 public class AccountServlet extends HttpServlet {
12 
13     public void doGet(HttpServletRequest request, HttpServletResponse response)
14             throws ServletException, IOException {
15         AccountService3 service = new AccountService3();
16         try {
17             service.transfer(1, 2, 100);
18         } catch (SQLException e) {
19             e.printStackTrace();
20             //注意：调用service层的方法出异常之后，继续将异常抛出，这样在TransactionFilter就能捕获到抛出的异常，继而执行事务回滚操作
21             throw new RuntimeException(e);
22         }
23     }
24 
25     public void doPost(HttpServletRequest request, HttpServletResponse response)
26             throws ServletException, IOException {
27         doGet(request, response);
28     }
29 }
```











