​       

# [javaweb学习总结(四十)——编写自己的JDBC框架](https://www.cnblogs.com/xdp-gacl/p/4006830.html)

## 一、元数据介绍

　　元数据指的是"数据库"、"表"、"列"的定义信息。

### 1.1、DataBaseMetaData元数据

　　Connection.getDatabaseMetaData()获得代表DatabaseMetaData元数据的DatabaseMetaData对象。
　　DataBaseMetaData对象的常用方法：

- getURL()：返回一个String类对象，代表数据库的URL。
- getUserName()：返回连接当前数据库管理系统的用户名。
- getDatabaseProductName()：返回数据库的产品名称。
- getDatabaseProductVersion()：返回数据库的版本号。
- getDriverName()：返回驱动驱动程序的名称。
- getDriverVersion()：返回驱动程序的版本号。
- isReadOnly()：返回一个boolean值，指示数据库是否只允许读操作。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1     /**
 2     * @Method: testDataBaseMetaData
 3     * @Description: 获取数据库的元信息
 4     * @Anthor:孤傲苍狼
 5     *
 6     * @throws SQLException
 7     */ 
 8     @Test
 9     public void testDataBaseMetaData() throws SQLException {
10         Connection conn = JdbcUtils.getConnection();
11         DatabaseMetaData metadata = conn.getMetaData();
12         //getURL()：返回一个String类对象，代表数据库的URL
13         System.out.println(metadata.getURL());
14         //getUserName()：返回连接当前数据库管理系统的用户名
15         System.out.println(metadata.getUserName());
16         //getDatabaseProductName()：返回数据库的产品名称
17         System.out.println(metadata.getDatabaseProductName());
18         //getDatabaseProductVersion()：返回数据库的版本号
19         System.out.println(metadata.getDatabaseProductVersion());
20         //getDriverName()：返回驱动驱动程序的名称
21         System.out.println(metadata.getDriverName());
22         //getDriverVersion()：返回驱动程序的版本号
23         System.out.println(metadata.getDriverVersion());
24         //isReadOnly()：返回一个boolean值，指示数据库是否只允许读操作
25         System.out.println(metadata.isReadOnly());
26         JdbcUtils.release(conn, null, null);
27     }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　运行结果如下：

　　![img](data:image/png;base64,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)

### 1.2、ParameterMetaData元数据

　　PreparedStatement.getParameterMetaData() 获得代表PreparedStatement元数据的ParameterMetaData对象。 
　　Select * from user where name=? And password=?
　　ParameterMetaData对象的常用方法：

- getParameterCount()： 获得指定参数的个数
- getParameterType(int param)：获得指定参数的sql类型，MySQL数据库驱动不支持

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1     /**
 2     * @Method: testParameterMetaData
 3     * @Description: 获取参数元信息
 4     * @Anthor:孤傲苍狼
 5     *
 6     * @throws SQLException
 7     */ 
 8     @Test
 9     public void testParameterMetaData() throws SQLException {
10         Connection conn = JdbcUtils.getConnection();
11         String sql = "select * from user wherer name=? and password=?";
12         //将SQL预编译一下
13         PreparedStatement st = conn.prepareStatement(sql);
14         ParameterMetaData pm = st.getParameterMetaData();
15         //getParameterCount() 获得指定参数的个数
16         System.out.println(pm.getParameterCount());
17         //getParameterType(int param)：获得指定参数的sql类型，MySQL数据库驱动不支持
18         System.out.println(pm.getParameterType(1));
19         JdbcUtils.release(conn, null, null);
20     }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 1.3、ResultSetMetaData元数据

　　ResultSet. getMetaData() 获得代表ResultSet对象元数据的ResultSetMetaData对象。 
　　ResultSetMetaData对象的常用方法：

- getColumnCount() 返回resultset对象的列数
- getColumnName(int column) 获得指定列的名称
- getColumnTypeName(int column)获得指定列的类型

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1     /**
 2     * @Method: testResultSetMetaData
 3     * @Description: 结果集的元数据
 4     * @Anthor:孤傲苍狼
 5     *
 6     * @throws Exception
 7     */
 8     @Test
 9     public void testResultSetMetaData() throws Exception {
10         Connection conn = JdbcUtils.getConnection();
11         String sql = "select * from account";
12         PreparedStatement st  = conn.prepareStatement(sql);
13         ResultSet rs = st.executeQuery();
14         //ResultSet.getMetaData()获得代表ResultSet对象元数据的ResultSetMetaData对象
15         ResultSetMetaData metadata = rs.getMetaData();
16         //getColumnCount() 返回resultset对象的列数
17         System.out.println(metadata.getColumnCount());
18         //getColumnName(int column) 获得指定列的名称
19         System.out.println(metadata.getColumnName(1));
20         //getColumnTypeName(int column)获得指定列的类型
21         System.out.println(metadata.getColumnTypeName(1));
22         JdbcUtils.release(conn, st, rs);
23     }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

## 二、使用元数据封装简单的JDBC框架

　　系统中所有实体对象都涉及到基本的CRUD操作
　　所有实体的CUD操作代码基本相同，仅仅发送给数据库的SQL语句不同而已，因此可以把CUD操作的所有相同代码抽取到工具类的一个update方法中，并定义参数接收变化的SQL语句。
　　实体的R操作，除SQL语句不同之外，根据操作的实体不同，对ResultSet的映射也各不相同，因此可义一个query方法，除以参数形式接收变化的SQL语句外，可以使用策略模式由qurey方法的调用者决定如何把ResultSet中的数据映射到实体对象中。

### 2.1、封装通用的update方法和qurey方法

　　定义一个JdbcUtils工具类，工具类负责获取数据库连接，释放资源，执行SQL的update和query操作，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.util;
  2 
  3 import java.io.InputStream;
  4 import java.sql.Connection;
  5 import java.sql.DriverManager;
  6 import java.sql.PreparedStatement;
  7 import java.sql.ResultSet;
  8 import java.sql.SQLException;
  9 import java.sql.Statement;
 10 import java.util.Properties;
 11 
 12 public class JdbcUtils {
 13 
 14     private static String driver = null;
 15     private static String url = null;
 16     private static String username = null;
 17     private static String password = null;
 18     
 19     static{
 20         try{
 21             //读取db.properties文件中的数据库连接信息
 22             InputStream in = JdbcUtils.class.getClassLoader().getResourceAsStream("db.properties");
 23             Properties prop = new Properties();
 24             prop.load(in);
 25             
 26             //获取数据库连接驱动
 27             driver = prop.getProperty("driver");
 28             //获取数据库连接URL地址
 29             url = prop.getProperty("url");
 30             //获取数据库连接用户名
 31             username = prop.getProperty("username");
 32             //获取数据库连接密码
 33             password = prop.getProperty("password");
 34             
 35             //加载数据库驱动
 36             Class.forName(driver);
 37             
 38         }catch (Exception e) {
 39             throw new ExceptionInInitializerError(e);
 40         }
 41     }
 42     
 43     /**
 44     * @Method: getConnection
 45     * @Description: 获取数据库连接对象
 46     * @Anthor:孤傲苍狼
 47     *
 48     * @return Connection数据库连接对象
 49     * @throws SQLException
 50     */ 
 51     public static Connection getConnection() throws SQLException{
 52         return DriverManager.getConnection(url, username,password);
 53     }
 54     
 55     /**
 56     * @Method: release
 57     * @Description: 释放资源，
 58     *     要释放的资源包括Connection数据库连接对象，负责执行SQL命令的Statement对象，存储查询结果的ResultSet对象
 59     * @Anthor:孤傲苍狼
 60     *
 61     * @param conn
 62     * @param st
 63     * @param rs
 64     */ 
 65     public static void release(Connection conn,Statement st,ResultSet rs){
 66         if(rs!=null){
 67             try{
 68                 //关闭存储查询结果的ResultSet对象
 69                 rs.close();
 70             }catch (Exception e) {
 71                 e.printStackTrace();
 72             }
 73             rs = null;
 74         }
 75         if(st!=null){
 76             try{
 77                 //关闭负责执行SQL命令的Statement对象
 78                 st.close();
 79             }catch (Exception e) {
 80                 e.printStackTrace();
 81             }
 82         }
 83         
 84         if(conn!=null){
 85             try{
 86                 //关闭Connection数据库连接对象
 87                 conn.close();
 88             }catch (Exception e) {
 89                 e.printStackTrace();
 90             }
 91         }
 92     }
 93     
 94     /**
 95     * @Method: update
 96     * @Description: 万能更新
 97     * 所有实体的CUD操作代码基本相同，仅仅发送给数据库的SQL语句不同而已，
 98     * 因此可以把CUD操作的所有相同代码抽取到工具类的一个update方法中，并定义参数接收变化的SQL语句
 99     * @Anthor:孤傲苍狼
100     * @param sql 要执行的SQL
101     * @param params 执行SQL时使用的参数
102     * @throws SQLException
103     */ 
104     public static void update(String sql,Object params[]) throws SQLException{
105         Connection conn = null;
106         PreparedStatement st = null;
107         ResultSet rs = null;
108         try{
109             conn = getConnection();
110             st = conn.prepareStatement(sql);
111             for(int i=0;i<params.length;i++){
112                 st.setObject(i+1, params[i]);
113             }
114             st.executeUpdate();
115             
116         }finally{
117             release(conn, st, rs);
118         }
119     }
120     
121     /**
122     * @Method: query
123     * @Description:万能查询
124     * 实体的R操作，除SQL语句不同之外，根据操作的实体不同，对ResultSet的映射也各不相同，
125     * 因此可义一个query方法，除以参数形式接收变化的SQL语句外，可以使用策略模式由qurey方法的调用者决定如何把ResultSet中的数据映射到实体对象中。
126     * @Anthor:孤傲苍狼
127     *
128     * @param sql 要执行的SQL
129     * @param params 执行SQL时使用的参数
130     * @param rsh 查询返回的结果集处理器
131     * @return
132     * @throws SQLException
133     */ 
134     public static Object query(String sql,Object params[],ResultSetHandler rsh) throws SQLException{
135         
136         Connection conn = null;
137         PreparedStatement st = null;
138         ResultSet rs = null;
139         
140         try{
141             conn = getConnection();
142             st = conn.prepareStatement(sql);
143             for(int i=0;i<params.length;i++){
144                 st.setObject(i+1, params[i]);
145             }
146             rs = st.executeQuery();
147             /**
148              * 对于查询返回的结果集处理使用到了策略模式，
149              * 在设计query方法时，query方法事先是无法知道用户对返回的查询结果集如何进行处理的，即不知道结果集的处理策略，
150              * 那么这个结果集的处理策略就让用户自己提供，query方法内部就调用用户提交的结果集处理策略进行处理
151              * 为了能够让用户提供结果集的处理策略，需要对用户暴露出一个结果集处理接口ResultSetHandler
152              * 用户只要实现了ResultSetHandler接口，那么query方法内部就知道用户要如何处理结果集了
153              */
154             return rsh.handler(rs);
155             
156         }finally{
157             release(conn, st, rs);
158         }
159     }
160 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在设计query方法时，对于查询返回的结果集处理使用到了策略模式，query方法事先是无法知道用户对返回的查询结果集如何进行处理的，即不知道结果集的处理策略，  那么这个结果集的处理策略就让用户自己提供，query方法内部就调用用户提交的结果集处理策略进行处理，  为了能够让用户提供结果集的处理策略，需要对用户暴露出一个结果集处理接口ResultSetHandler，  结果集处理器接口ResultSetHandler的定义如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.util;
 2 
 3 import java.sql.ResultSet;
 4 
 5 /**
 6 * @ClassName: ResultSetHandler
 7 * @Description:结果集处理器接口
 8 * @author: 孤傲苍狼
 9 * @date: 2014-10-5 下午12:01:27
10 *
11 */ 
12 public interface ResultSetHandler {
13     
14     /**
15     * @Method: handler
16     * @Description: 结果集处理方法
17     * @Anthor:孤傲苍狼
18     *
19     * @param rs 查询结果集
20     * @return
21     */ 
22     public Object handler(ResultSet rs);
23 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　用户只要实现了ResultSetHandler接口，那么就是针对查询结果集写了一个处理器，在query方法内部就调用用户自己写的处理器处理结果集。

### 2.2、编写常用的结果集处理器

　　为了提高框架的易用性，我们可以事先就针对结果集写好一些常用的处理器，比如将结果集转换成bean对象的处理器，将结果集转换成bean对象的list集合的处理器。

#### **2.2.1、BeanHandler——将结果集转换成bean对象的处理器**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.util;
 2 
 3 import java.lang.reflect.Field;
 4 import java.sql.ResultSet;
 5 import java.sql.ResultSetMetaData;
 6 
 7 /**
 8 * @ClassName: BeanHandler
 9 * @Description: 将结果集转换成bean对象的处理器
10 * @author: 孤傲苍狼
11 * @date: 2014-10-5 下午12:00:33
12 *
13 */ 
14 public class BeanHandler implements ResultSetHandler {
15     private Class<?> clazz;
16     public BeanHandler(Class<?> clazz){
17         this.clazz = clazz;
18     }
19     public Object handler(ResultSet rs) {
20         try{
21             if(!rs.next()){
22                 return null;
23             }
24             Object bean = clazz.newInstance();
25             //得到结果集元数据
26             ResultSetMetaData metadata = rs.getMetaData();
27             int columnCount = metadata.getColumnCount();//得到结果集中有几列数据
28             for(int i=0;i<columnCount;i++){
29                 String coulmnName = metadata.getColumnName(i+1);//得到每列的列名
30                 Object coulmnData = rs.getObject(i+1);
31                 Field f = clazz.getDeclaredField(coulmnName);//反射出类上列名对应的属性
32                 f.setAccessible(true);
33                 f.set(bean, coulmnData);
34             }
35             return bean;
36         }catch (Exception e) {
37             throw new RuntimeException(e);
38         }
39     }
40 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

#### **2.2.2、BeanListHandler——将结果集转换成bean对象的list集合的处理器**

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.util;
 2 
 3 import java.lang.reflect.Field;
 4 import java.sql.ResultSet;
 5 import java.sql.ResultSetMetaData;
 6 import java.util.ArrayList;
 7 import java.util.List;
 8 
 9 /**
10 * @ClassName: BeanListHandler
11 * @Description: 将结果集转换成bean对象的list集合的处理器
12 * @author: 孤傲苍狼
13 * @date: 2014-10-5 下午12:00:06
14 *
15 */ 
16 public class BeanListHandler implements ResultSetHandler {
17     private Class<?> clazz;
18     public BeanListHandler(Class<?> clazz){
19         this.clazz = clazz;
20     }
21     
22     public Object handler(ResultSet rs) {
23         try{
24             List<Object> list = new ArrayList<Object>();
25             while(rs.next()){
26                 Object bean = clazz.newInstance();
27                 
28                 ResultSetMetaData  metadata = rs.getMetaData();
29                 int count = metadata.getColumnCount();
30                 for(int i=0;i<count;i++){
31                     String name = metadata.getColumnName(i+1);
32                     Object value = rs.getObject(name);
33                     
34                     Field f = bean.getClass().getDeclaredField(name);
35                     f.setAccessible(true);
36                     f.set(bean, value);
37                 }
38                 list.add(bean);
39             }
40             return list.size()>0?list:null;
41         
42         }catch (Exception e) {
43             throw new RuntimeException(e);
44         }
45     }
46 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　当框架自身提供的结果集处理器不满足用户的要求时，那么用户就可以自己去实现ResultSetHandler接口，编写满足自己业务要求的结果集处理器。

　　有了上述的JdbcUtils框架之后，针对单个实体对象CRUD操作就非常方便了，如下所示：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.dao;
 2 
 3 import java.sql.SQLException;
 4 import java.util.List;
 5 import me.gacl.domain.Account;
 6 import me.gacl.util.BeanHandler;
 7 import me.gacl.util.BeanListHandler;
 8 import me.gacl.util.JdbcUtils;
 9 
10 public class AccountDao {
11 
12     public void add(Account account) throws SQLException{
13         String sql = "insert into account(name,money) values(?,?)";
14         Object params[] = {account.getName(),account.getMoney()};
15         JdbcUtils.update(sql, params);
16     }
17     
18     
19     public void delete(int id) throws SQLException{
20         String sql = "delete from account where id=?";
21         Object params[] = {id};
22         JdbcUtils.update(sql, params);
23     }
24     
25     public void update(Account account) throws SQLException{
26         
27         String sql = "update account set name=?,money=? where id=?";
28         Object params[] = {account.getName(),account.getMoney(),account.getId()};
29         JdbcUtils.update(sql, params);
30         
31     }
32     
33     public Account find(int id) throws SQLException{
34         String sql = "select * from account where id=?";
35         Object params[] = {id};
36         return (Account) JdbcUtils.query(sql, params, new BeanHandler(Account.class));
37     }
38     
39     public List<Account> getAll() throws SQLException{
40         String sql = "select * from account";
41         Object params[] = {};
42         return (List<Account>) JdbcUtils.query(sql, params,new BeanListHandler(Account.class));
43     }
44 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 　　编写的这个JDBC框架就是模拟Apache的DBUtils框架的实现，下一篇将具体介绍Apache的DBUtils框架。



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)