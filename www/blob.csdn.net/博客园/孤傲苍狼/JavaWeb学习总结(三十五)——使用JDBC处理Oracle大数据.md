# JavaWeb学习总结(三十五)——使用JDBC处理Oracle大数据 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [JavaWeb学习总结(三十五)——使用JDBC处理Oracle大数据](https://www.cnblogs.com/xdp-gacl/p/4009399.html)



## 一、Oracle中大数据处理

　　在Oracle中，LOB（Large Object，大型对象）类型的字段现在用得越来越多了。因为这种类型的字段，容量大（最多能容纳4GB的数据），且一个表中可以有多个这种类型的字段，很灵活，适用于数据 量非常大的业务领域（如图象、档案等）。

　　LOB类型分为BLOB和CLOB两种：BLOB即二进制大型对象（Binary Large Object），适用于存贮非文本的字节流数据（如程序、图象、影音等）。而CLOB，即字符型大型对象（Character Large Object），则与字符集相关，适于存贮文本型的数据（如历史档案、大部头著作等）。

## 二、搭建测试环境

### 2.1、建立两个测试用的数据库表

　　建表SQL语句为：

```
1 CREATE TABLE TEST_CLOB ( ID NUMBER(3), CLOBCOL CLOB)
2 
3 CREATE TABLE TEST_BLOB ( ID NUMBER(3), BLOBCOL BLOB)
```

### 2.2、搭建测试项目架构

![](data:image/png;base64,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)

### 2.3、编写db.properties配置文件

```
1 oracleDb_Driver=oracle.jdbc.driver.OracleDriver
2 oracleDb_Url=jdbc:oracle:thin:@localhost:1521:GACL
3 oracleDb_UserName=GACL_XDP
4 oracleDb_Password=P
```

### 2.4、编写JdbcUtils工具类

```
1 package me.gacl.utils;
 2 
 3 import java.io.InputStream;
 4 import java.sql.Connection;
 5 import java.sql.DriverManager;
 6 import java.sql.ResultSet;
 7 import java.sql.SQLException;
 8 import java.sql.Statement;
 9 import java.util.Properties;
10 
11 public class JdbcUtils {
12 
13     private static String oracleDb_Driver = null;
14     private static String oracleDb_Url = null;
15     private static String oracleDb_UserName = null;
16     private static String oracleDb_Password = null;
17     
18     static{
19         try{
20             //读取db.properties文件中的数据库连接信息
21             InputStream in = JdbcUtils.class.getClassLoader().getResourceAsStream("db.properties");
22             Properties prop = new Properties();
23             prop.load(in);
24             
25             //获取数据库连接驱动
26             oracleDb_Driver = prop.getProperty("oracleDb_Driver");
27             //获取数据库连接URL地址
28             oracleDb_Url = prop.getProperty("oracleDb_Url");
29             //获取数据库连接用户名
30             oracleDb_UserName = prop.getProperty("oracleDb_UserName");
31             //获取数据库连接密码
32             oracleDb_Password = prop.getProperty("oracleDb_Password");
33             
34             //加载数据库驱动
35             Class.forName(oracleDb_Driver);
36             
37         }catch (Exception e) {
38             throw new ExceptionInInitializerError(e);
39         }
40     }
41     
42     /**
43     * @Method: getOracleConnection
44     * @Description: 获取Oracle数据库连接对象
45     * @Anthor:孤傲苍狼
46     *
47     * @return Connection数据库连接对象
48     * @throws SQLException
49     */ 
50     public static Connection getOracleConnection() throws SQLException{
51         return DriverManager.getConnection(oracleDb_Url, oracleDb_UserName,oracleDb_Password);
52     }
53     
54     /**
55     * @Method: release
56     * @Description: 释放资源，
57     *     要释放的资源包括Connection数据库连接对象，负责执行SQL命令的Statement对象，存储查询结果的ResultSet对象
58     * @Anthor:孤傲苍狼
59     *
60     * @param conn
61     * @param st
62     * @param rs
63     */ 
64     public static void release(Connection conn,Statement st,ResultSet rs){
65         if(rs!=null){
66             try{
67                 //关闭存储查询结果的ResultSet对象
68                 rs.close();
69             }catch (Exception e) {
70                 e.printStackTrace();
71             }
72             rs = null;
73         }
74         if(st!=null){
75             try{
76                 //关闭负责执行SQL命令的Statement对象
77                 st.close();
78             }catch (Exception e) {
79                 e.printStackTrace();
80             }
81         }
82         
83         if(conn!=null){
84             try{
85                 //关闭Connection数据库连接对象
86                 conn.close();
87             }catch (Exception e) {
88                 e.printStackTrace();
89             }
90         }
91     }
92 }
```

## 三、JDBC处理Oracle大数据

### 3.1、JDBC处理CLOB数据

```
1 package me.gacl.demo;
  2 
  3 import java.io.BufferedReader;
  4 import java.io.BufferedWriter;
  5 import java.io.FileReader;
  6 import java.io.FileWriter;
  7 import java.sql.Connection;
  8 import java.sql.PreparedStatement;
  9 import java.sql.ResultSet;
 10 import org.junit.Test;
 11 import me.gacl.utils.JdbcUtils;
 12 
 13 /**
 14 * @ClassName: JdbcOperaOracleClob
 15 * @Description:Oracle中字符型大型对象（Character Large Object）数据处理
 16 * @author: 孤傲苍狼
 17 * @date: 2014-10-7 下午3:53:19
 18 *
 19 */ 
 20 public class JdbcOperaOracleClob {
 21 
 22     /**
 23     CREATE TABLE TEST_CLOB ( ID NUMBER(3), CLOBCOL CLOB)
 24      */
 25     /**
 26     * @Method: clobInsert
 27     * @Description:往数据库中插入一个新的CLOB对象
 28     * @Anthor:孤傲苍狼
 29     *
 30     * @throws Exception
 31     */ 
 32     @Test
 33     public void clobInsert() throws Exception {
 34         Connection conn = null;
 35         PreparedStatement stmt = null;
 36         ResultSet rs = null;
 37         conn = JdbcUtils.getOracleConnection();
 38         boolean defaultCommit = conn.getAutoCommit();
 39         /*开启事务，设定不自动提交 */
 40         conn.setAutoCommit(false);
 41         try {
 42             /* 插入一个空的CLOB对象 */
 43             String sql = "INSERT INTO TEST_CLOB VALUES (?, EMPTY_CLOB())";
 44             stmt = conn.prepareStatement(sql);
 45             stmt.setInt(1, 1);
 46             stmt.executeUpdate();
 47             /* 查询此CLOB对象并锁定 */
 48             sql = "SELECT CLOBCOL FROM TEST_CLOB WHERE ID=? FOR UPDATE";
 49             stmt = conn.prepareStatement(sql);
 50             stmt.setInt(1, 1);
 51             rs = stmt.executeQuery();
 52             if (rs.next()) {
 53                 /* 取出此CLOB对象 */
 54                 oracle.sql.CLOB clob = (oracle.sql.CLOB) rs.getClob("CLOBCOL");
 55                 /* 向CLOB对象中写入数据 */
 56                 BufferedWriter out = new BufferedWriter(clob.getCharacterOutputStream());
 57                 //这种方式获取的路径，其中的空格会被使用“%20”代替
 58                 String path = JdbcOperaClob.class.getClassLoader().getResource("data.txt").getPath();
 59                 //将“%20”替换回空格
 60                 path = path.replaceAll("%20", " ");
 61                 BufferedReader in = new BufferedReader(new FileReader(path));
 62                 int c;
 63                 while ((c = in.read()) != -1) {
 64                     out.write(c);
 65                 }
 66                 in.close();
 67                 out.close();
 68             }
 69             /* 正式提交 */
 70             conn.commit();
 71             System.out.println("插入成功");
 72         } catch (Exception ex) {
 73             /* 出错回滚 */
 74             conn.rollback();
 75             throw ex;
 76         }finally{
 77             /* 恢复原提交状态 */
 78             conn.setAutoCommit(defaultCommit);
 79             JdbcUtils.release(conn,stmt,rs);
 80         }
 81         
 82     }
 83     
 84     /**
 85     * @Method: clobRead
 86     * @Description: CLOB对象读取
 87     * @Anthor:孤傲苍狼
 88     *
 89     * @throws Exception
 90     */
 91     @Test
 92     public void clobRead() throws Exception {
 93         Connection conn = null;
 94         PreparedStatement stmt = null;
 95         ResultSet rs = null;
 96         conn = JdbcUtils.getOracleConnection();
 97         boolean defaultCommit = conn.getAutoCommit();
 98         conn.setAutoCommit(false);
 99         try {
100             /* 查询CLOB对象 */
101             String sql = "SELECT * FROM TEST_CLOB WHERE ID=?";
102             stmt = conn.prepareStatement(sql);
103             stmt.setInt(1, 1);
104             rs = stmt.executeQuery();
105             if (rs.next()) {
106                 /* 获取CLOB对象 */
107                 oracle.sql.CLOB clob = (oracle.sql.CLOB) rs.getClob("CLOBCOL");
108                 /* 以字符形式输出 */
109                 BufferedReader in = new BufferedReader(clob.getCharacterStream());
110                 BufferedWriter out = new BufferedWriter(new FileWriter("D:\\2.txt"));
111                 int c;
112                 while ((c = in.read()) != -1) {
113                     out.write(c);
114                 }
115                 out.close();
116                 in.close();
117             }
118         } catch (Exception ex) {
119             conn.rollback();
120             throw ex;
121         }finally{
122             /* 恢复原提交状态 */
123             conn.setAutoCommit(defaultCommit);
124             JdbcUtils.release(conn,stmt,rs);
125         }
126     }
127     
128     /**
129     * @Method: clobModify
130     * @Description:修改CLOB对象（是在原CLOB对象基础上进行覆盖式的修改）
131     * @Anthor:孤傲苍狼
132     *
133     * @throws Exception
134     */ 
135     @Test
136     public void clobModify() throws Exception {
137         Connection conn = null;
138         PreparedStatement stmt = null;
139         ResultSet rs = null;
140         conn = JdbcUtils.getOracleConnection();
141         boolean defaultCommit = conn.getAutoCommit();
142         // 开启事务
143         conn.setAutoCommit(false);
144         try {
145             /* 查询CLOB对象并锁定 */
146             String sql = "SELECT CLOBCOL FROM TEST_CLOB WHERE ID=? FOR UPDATE";
147             stmt = conn.prepareStatement(sql);
148             stmt.setInt(1, 1);
149             rs = stmt.executeQuery();
150             if (rs.next()) {
151                 /* 获取此CLOB对象 */
152                 oracle.sql.CLOB clob = (oracle.sql.CLOB) rs.getClob("CLOBCOL");
153                 /* 进行覆盖式修改 */
154                 BufferedWriter out = new BufferedWriter(clob.getCharacterOutputStream());
155                 // 这种方式获取的路径，其中的空格会被使用“%20”代替
156                 String path = JdbcOperaClob.class.getClassLoader().getResource("data2.txt").getPath();
157                 // 将“%20”替换回空格
158                 path = path.replaceAll("%20", " ");
159                 BufferedReader in = new BufferedReader(new FileReader(path));
160                 int c;
161                 while ((c = in.read()) != -1) {
162                     out.write(c);
163                 }
164                 in.close();
165                 out.close();
166             }
167             /*提交事务 */
168             conn.commit();
169         } catch (Exception ex) {
170             /*出错回滚事务 */
171             conn.rollback();
172             throw ex;
173         }finally{
174             /*恢复原提交状态 */
175             conn.setAutoCommit(defaultCommit);
176             JdbcUtils.release(conn,stmt,rs);
177         }
178     }
179 
180     /**
181     * @Method: clobReplace
182     * @Description:替换CLOB对象（将原CLOB对象清除，换成一个全新的CLOB对象）
183     * @Anthor:孤傲苍狼
184     *
185     * @throws Exception
186     */ 
187     @Test
188     public void clobReplace() throws Exception {
189         Connection conn = null;
190         PreparedStatement stmt = null;
191         ResultSet rs = null;
192         conn = JdbcUtils.getOracleConnection();
193         boolean defaultCommit = conn.getAutoCommit();
194         // 开启事务
195         conn.setAutoCommit(false);
196         try {
197             /* 清空原CLOB对象 */
198             String sql = "UPDATE TEST_CLOB SET CLOBCOL=EMPTY_CLOB() WHERE ID=?";
199             stmt = conn.prepareStatement(sql);
200             stmt.setInt(1, 1);
201             stmt.executeUpdate();
202 
203             /* 查询CLOB对象并锁定 */
204             sql = "SELECT CLOBCOL FROM TEST_CLOB WHERE ID=? FOR UPDATE";
205             stmt = conn.prepareStatement(sql);
206             stmt.setInt(1, 1);
207             rs = stmt.executeQuery();
208             if (rs.next()) {
209                 /* 获取此CLOB对象 */
210                 oracle.sql.CLOB clob = (oracle.sql.CLOB) rs.getClob("CLOBCOL");
211                 /* 更新数据 */
212                 BufferedWriter out = new BufferedWriter(clob.getCharacterOutputStream());
213                 // 这种方式获取的路径，其中的空格会被使用“%20”代替
214                 String path = JdbcOperaClob.class.getClassLoader().getResource("db.properties").getPath();
215                 // 将“%20”替换回空格
216                 path = path.replaceAll("%20", " ");
217                 BufferedReader in = new BufferedReader(new FileReader(path));
218                 int c;
219                 while ((c = in.read()) != -1) {
220                     out.write(c);
221                 }
222                 in.close();
223                 out.close();
224             }
225             /* 正式提交 */
226             conn.commit();
227         } catch (Exception ex) {
228             /* 出错回滚 */
229             conn.rollback();
230             throw ex;
231         } finally {
232             /* 恢复原提交状态 */
233             conn.setAutoCommit(defaultCommit);
234             JdbcUtils.release(conn, stmt, rs);
235         }
236     }
237 }
```

### 3.2、JDBC处理BLOB数据

　　Oracle定义了一个BLOB字段用于保存二进制数据，但这个字段并不能存放真正的二进制数据，只能向这个字段存一个指针，然后把数据放到指针所指向的Oracle的LOB段中， LOB段是在数据库内部表的一部分。因而在操作Oracle的Blob之前，必须获得指针（定位器）才能进行Blob数据的读取和写入。
　　如何获得表中的Blob指针呢？ 可以先使用insert语句向表中插入一个空的blob（调用oracle的函数empty_blob()），这将创建一个blob的指针，然后再把这个empty的blob的指针查询出来，这样就可得到BLOB对象，从而读写blob数据了。

　　1、插入空blob：insert into testblob(id,image) values(?,empty_blob())
　　2、获得blob的cursor：

　　　　select image from testblob where id=? for update 注意: 必 须加for update锁定该行，直至该行被修改完毕，保证不产生并发冲突。

　　　　Blob b = rs.getBlob("image");

　　3、利用 io和获取到的cursor往数据库读写数据
　　注意：**以上操作需开启事务。**

**　　BLOB对象的存取范例**

```
1 package me.gacl.demo;
  2 
  3 import java.io.BufferedInputStream;
  4 import java.io.BufferedOutputStream;
  5 import java.io.FileOutputStream;
  6 import java.sql.Connection;
  7 import java.sql.PreparedStatement;
  8 import java.sql.ResultSet;
  9 import org.junit.Test;
 10 import me.gacl.utils.JdbcUtils;
 11 
 12 /**
 13 * @ClassName: JdbcOperaOracleBlob
 14 * @Description:Oracle中大数据处理
 15 * @author: 孤傲苍狼
 16 * @date: 2014-10-7 下午3:53:19
 17 *
 18 */ 
 19 public class JdbcOperaOracleBlob {
 20 
 21     /**
 22      * @Method: blobInsert
 23      * @Description: 向数据库中插入一个新的BLOB对象
 24      * @Anthor:孤傲苍狼
 25      * 
 26      * @throws Exception
 27      */
 28     @Test
 29     public void blobInsert() throws Exception {
 30         Connection conn = null;
 31         PreparedStatement stmt = null;
 32         ResultSet rs = null;
 33         boolean defaultCommit = true;
 34         try {
 35             conn = JdbcUtils.getOracleConnection();
 36             //得到数据库事务处理的默认提交方式
 37             defaultCommit = conn.getAutoCommit();
 38             //1、开启事务
 39             conn.setAutoCommit(false);
 40             //2、插入一个空的BLOB对象
 41             String sql = "INSERT INTO TEST_BLOB VALUES (?, EMPTY_BLOB())";
 42             stmt = conn.prepareStatement(sql);
 43             stmt.setInt(1, 1);
 44             stmt.executeUpdate();
 45             //3、查询此BLOB对象并锁定。注意: 必 须加for update锁定该行，直至该行被修改完毕，保证不产生并发冲突
 46             sql = "SELECT BLOBCOL FROM TEST_BLOB WHERE ID=? FOR UPDATE";
 47             stmt = conn.prepareStatement(sql);
 48             stmt.setInt(1, 1);
 49             rs = stmt.executeQuery();
 50             if (rs.next()) {
 51                 //4、取出此BLOB对象 ，并强制转换成Oracle的BLOB对象
 52                 oracle.sql.BLOB blob = (oracle.sql.BLOB) rs.getBlob("BLOBCOL");
 53                 //5、使用IO向BLOB对象中写入数据
 54                 BufferedOutputStream out = new BufferedOutputStream(blob.getBinaryOutputStream());
 55                 BufferedInputStream in = new BufferedInputStream(JdbcOperaOracleBlob.class.getClassLoader().getResourceAsStream("01.jpg"));
 56                 int c;
 57                 while ((c = in.read()) != -1) {
 58                     out.write(c);
 59                 }
 60                 in.close();
 61                 out.close();
 62             }
 63             //6、提交事务
 64             conn.commit();
 65         } catch (Exception ex) {
 66             //7、出错回滚事务
 67             conn.rollback();
 68             throw ex;
 69         } finally {
 70             //8、恢复数据库事务处理的默认提交方式
 71             conn.setAutoCommit(defaultCommit);
 72             //释放资源
 73             JdbcUtils.release(conn, stmt, rs);
 74         }
 75 
 76     }
 77 
 78     /**
 79      * @Method: blobModify
 80      * @Description:修改BLOB对象（是在原BLOB对象基础上进行覆盖式的修改）
 81      * @Anthor:孤傲苍狼
 82      * 
 83      * @throws Exception
 84      */
 85     @Test
 86     public void blobModify() throws Exception {
 87         Connection conn = null;
 88         PreparedStatement stmt = null;
 89         ResultSet rs = null;
 90         boolean defaultCommit = true;
 91         try {
 92             conn = JdbcUtils.getOracleConnection();
 93             //得到数据库事务处理的默认提交方式
 94             defaultCommit = conn.getAutoCommit();
 95             //1、开启事务
 96             conn.setAutoCommit(false);
 97             //2、查询此BLOB对象并锁定。注意: 必 须加for update锁定该行，直至该行被修改完毕，保证不产生并发冲突
 98             String sql = "SELECT BLOBCOL FROM TEST_BLOB WHERE ID=? FOR UPDATE";
 99             stmt = conn.prepareStatement(sql);
100             stmt.setInt(1, 1);
101             rs = stmt.executeQuery();
102             if (rs.next()) {
103                 //3、取出此BLOB对象 ，并强制转换成Oracle的BLOB对象
104                 oracle.sql.BLOB blob = (oracle.sql.BLOB) rs.getBlob("BLOBCOL");
105                 //4、使用IO向BLOB对象中写入数据
106                 BufferedOutputStream out = new BufferedOutputStream(blob.getBinaryOutputStream());
107                 BufferedInputStream in = new BufferedInputStream(JdbcOperaOracleBlob.class.getClassLoader().getResourceAsStream("02.jpg"));
108                 int c;
109                 while ((c = in.read()) != -1) {
110                     out.write(c);
111                 }
112                 in.close();
113                 out.close();
114             }
115             //5、提交事务
116             conn.commit();
117         } catch (Exception ex) {
118             //6、出错回滚事务
119             conn.rollback();
120             throw ex;
121         } finally {
122             //8、恢复数据库事务处理的默认提交方式
123             conn.setAutoCommit(defaultCommit);
124             //释放资源
125             JdbcUtils.release(conn, stmt, rs);
126         }
127     }
128 
129     /**
130      * @Method: blobReplace
131      * @Description:替换BLOB对象（将原BLOB对象清除，换成一个全新的BLOB对象）
132      * @Anthor:孤傲苍狼
133      * 
134      * @throws Exception
135      */
136     @Test
137     public void blobReplace() throws Exception {
138         Connection conn = null;
139         PreparedStatement stmt = null;
140         ResultSet rs = null;
141         boolean defaultCommit = true;
142         try {
143             conn = JdbcUtils.getOracleConnection();
144             //得到数据库事务处理的默认提交方式
145             defaultCommit = conn.getAutoCommit();
146             //1、开启事务
147             conn.setAutoCommit(false);
148             //2、清空原BLOB对象
149             String sql = "UPDATE TEST_BLOB SET BLOBCOL=EMPTY_BLOB() WHERE ID=?";
150             stmt = conn.prepareStatement(sql);
151             stmt.setInt(1, 1);
152             stmt.executeUpdate();
153             //3、查询此BLOB对象并锁定。注意: 必 须加for update锁定该行，直至该行被修改完毕，保证不产生并发冲突
154             sql = "SELECT BLOBCOL FROM TEST_BLOB WHERE ID=? FOR UPDATE";
155             stmt = conn.prepareStatement(sql);
156             stmt.setInt(1, 1);
157             rs = stmt.executeQuery();
158             if (rs.next()) {
159                 //4、取出此BLOB对象 ，并强制转换成Oracle的BLOB对象
160                 oracle.sql.BLOB blob = (oracle.sql.BLOB) rs.getBlob("BLOBCOL");
161                 //5、使用IO向BLOB对象中写入数据
162                 BufferedOutputStream out = new BufferedOutputStream(blob.getBinaryOutputStream());
163                 BufferedInputStream in = new BufferedInputStream(JdbcOperaOracleBlob.class.getClassLoader().getResourceAsStream("01.jpg"));
164                 int c;
165                 while ((c = in.read()) != -1) {
166                     out.write(c);
167                 }
168                 in.close();
169                 out.close();
170             }
171             //6、提交事务
172             conn.commit();
173         } catch (Exception ex) {
174             //7、出错回滚事务
175             conn.rollback();
176             throw ex;
177         } finally {
178             //8、恢复数据库事务处理的默认提交方式
179             conn.setAutoCommit(defaultCommit);
180             //释放资源
181             JdbcUtils.release(conn, stmt, rs);
182         }
183     }
184 
185     /**
186      * @Method: blobRead
187      * @Description:BLOB对象读取
188      * @Anthor:孤傲苍狼
189      * 
190      * @throws Exception
191      */
192     @Test
193     public void blobRead() throws Exception {
194         Connection conn = null;
195         PreparedStatement stmt = null;
196         ResultSet rs = null;
197         boolean defaultCommit = true;
198         try {
199             conn = JdbcUtils.getOracleConnection();
200             //得到数据库事务处理的默认提交方式
201             defaultCommit = conn.getAutoCommit();
202             //1、开启事务
203             conn.setAutoCommit(false);
204             //2、查询BLOB对象
205             String sql = "SELECT BLOBCOL FROM TEST_BLOB WHERE ID=?";
206             stmt = conn.prepareStatement(sql);
207             stmt.setInt(1, 1);
208             rs = stmt.executeQuery();
209             if (rs.next()) {
210                 //3、取出此BLOB对象 ，并强制转换成Oracle的BLOB对象
211                 oracle.sql.BLOB blob = (oracle.sql.BLOB) rs.getBlob("BLOBCOL");
212                 //4、以二进制流的形式输出
213                 BufferedOutputStream out = new BufferedOutputStream(new FileOutputStream("D:/1.jpg"));
214                 BufferedInputStream in = new BufferedInputStream(blob.getBinaryStream());
215                 int c;
216                 while ((c = in.read()) != -1) {
217                     out.write(c);
218                 }
219                 in.close();
220                 out.close();
221             }
222             //5、提交事务
223             conn.commit();
224         } catch (Exception ex) {
225             //6、出错回滚事务
226             conn.rollback();
227             throw ex;
228         } finally {
229             //8、恢复数据库事务处理的默认提交方式
230             conn.setAutoCommit(defaultCommit);
231             //释放资源
232             JdbcUtils.release(conn, stmt, rs);
233         }
234 
235     }
236 }
```

## 四、使用JDBC处理Oracle大数据总结

　　通过JDBC操纵Oracle数据库的LOB字段，不外乎插入、修改、替换、读取四种方式，掌握起来并不难。观察上述**程序对LOB类型字段的存取**，我们可以看出，**较之其它类型字段，有下面几个显著不同的特点**：

**1、必须取消自动提交**。

　　存取操作开始前，必须用setAutoCommit(false)取消自动提交。其它类型字段则无此特殊要求。这是因为存取LOB类型字段时，通常要进行多次操作可以完成。不这样的话，Oracle将抛出“读取违反顺序”的错误。

**2、插入方式不同**。

　　LOB数据不能象其它类型数据一样直接插入（INSERT）。插入前必须先插入一个空的LOB对象，CLOB类型 的空对象为EMPTY_CLOB()，BLOB类型的空对象为EMPTY_BLOB()。之后通过SELECT命令查询得到先前插入的记录并锁定，继而将 空对象修改为所要插入的LOB对象。

**3、修改方式不同**。

　　其它类型的字段修改时，用UPDATE … SET…命令即可。而LOB类型字段，则只能用SELECT … FOR UPDATE命令将记录查询出来并锁定，然后才能修改。且修改也有两种改法：一是在原数据基础上的修改（即覆盖式修改），执行SELECT … FOR UPDATE后再改数据；二是替换（先将原数据清掉，再修改），先执行UPDATE命令将LOB字段之值设为空的LOB对象，然后进行第一种改法。建议使 用替换的方法，以实现与其它字段UPDATE操作后一样的效果。

**4、存取时应使用由数据库JDBC驱动程序提供的LOB操作类**。

　　对于Oracle数据库，应使用oracle.sql.CLOB和oracle.sql.BLOB。不使用由数据库JDBC驱动程序提供的LOB类时，程序运行时易于出现“抽象方法调用”的错误，这是因为JDBC所定义的java.sql.Clob与 java.sql.Blob接口，其中的一些方法并未在数据库厂家提供的驱动程序中真正实现。

**5、存取手段与文件操作相仿**。

　　对于BLOB类型，应用InputStream/OutputStream类，此类不进行编码转换，逐个字节存取。oracle.sql.BLOB类相应提供了getBinaryStream()和getBinaryOutputStream()两个方法，前一个 方法用于读取Oracle的BLOB字段，后一个方法用于将数据写入Oracle的BLOB字段。

　　对于CLOB类型，应用Reader/Writer类，此类进行编码转换。oracle.sql.CLOB类相应 提供了getCharacterStream()和getCharacterOutputStream()两个方法，前一个方法用于读取Oracle的 CLOB字段，后一个方法用于将数据写入Oracle的CLOB字段。

　　需要说明的是，为了大幅提高程序执行效率，对BLOB/CLOB字段的读写操作，应该使用缓冲操作类（带 Buffered前缀），即：BufferedInputStream，BufferedOutputStream，BufferedReader，BufferedWriter。 例程中全部使用了缓冲操作类。









