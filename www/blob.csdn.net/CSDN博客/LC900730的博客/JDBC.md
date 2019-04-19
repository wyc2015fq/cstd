# JDBC - LC900730的博客 - CSDN博客
2017年07月27日 15:35:01[lc900730](https://me.csdn.net/LC900730)阅读数：166标签：[java																[jdbc](https://so.csdn.net/so/search/s.do?q=jdbc&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## 主要使用步骤
```
//注册驱动程序
//连接数据库，返回连接对象
//创建statement
//准备sql
//发送sql语句，执行sql语句，得到返回结果
//输出
//关闭
```
## jdbc
是sun公司设计的一套通用的 java语言操作不同数据库的接口 
驱动程序就是针对接口的实现
```
方式一：使用驱动管理器
    Connection connection=null;
    Class.forName("com.mysql.cj.jdbc.Driver").newInstance();
    connection=DriverManager.getConnection("jdbc:mysql://localhost:3306/test?useSSL=false","root","qqaazz");
System.out.println(connection);
方式二：接口的connect
Driver driver=new com.mysql.jdbc.Driver();
        String url="jdbc:mysql://localhost:3306/test";
        String user="root";
        String password="qqaazz";
        Properties prop=new Properties();
        prop.setProperty("user", user);
        prop.setProperty("password", password);
        Connection conn=driver.connect(url, prop);
        System.out.println(conn);
```
## JDBC接口核心的API
```
java.sql.*和javax.sql.*
```
Driver接口：表示java驱动程序接口，所有的具体数据库厂商要来实现此接口。
```
connect：连接数据库的方法；
```
DriverManage类：驱动管理器类，用于管理所有注册的驱动程序；
```
registerDriver(driver):注册驱动类对象
Conncetion getConnection(url,user,password):获取连接对象
```
Connection接口：表示java程序和数据库的连接对象
```
Statement createStatement():创建Statement对象
PreparedStatement prepareStatement(String sql)
    创建PreparedStatement对象   
CallableStatement prepareCall(String sql)
    创建CallableStatement对象
```
Statement接口：用于执行静态的sql语句 
    int executeUpdate(String sql):执行静态更新的sql语句(DDL,DML) 
    ResultSet executeQuery(String sql):执行静态查询sql语句(DQL)
PreparedStatement接口：用于执行预编译sql语句
ResultSet: 
具有指向其当前数据行的光标。最初光标被置于第一行之前，next方法将光标移到到下一行；因为该方法在Resultset对象没有下一行时返回false，所以可以在while循环中使用它来迭代结果集。 
ResultSet接口用于从当前行中获取列值的获取方法(getBoolean,getLong等)。可以使用列索引较为高效，列从1开始编号，为了获得最大 的可移植性，应该按照从左到右顺序读取每行中的结果集列，每列只能读取一次。
### 预处理
Statement
```
int executeUpdate(String sql)
ResultSet executeQuery(String sql)
```
PrepareStatement接口
```
int executeUpdate(String sql)
ResultSet executeQuery(String sql)
```
```java
PreparedStatement stmt=null;
String sql="insert into student(NAME,gender)    values(?,?)";
stmt=conn.prepareStatement(sql);
stmt.setString(1,"张三");//1代表参数的位置
stmt.setstring(2,"男");
```
检查语法是否正确； 
检查是否有足够的权限； 
把sql加入到sql缓存区
```
Map<String,执行任务>
<"select * from student where id=1",执行任务>
```
数据库取出sql缓冲区的执行任务开始执行
PreparedStatement执行sql语句 
        PreparedStatement vs Statement 
        1.语法不同，PreparedStatement可以使用预编译的sql，而Statement只能使用静态的sql; 
        2.效率不同，PreparedStatement可以使用sql缓冲区，效率比Statement高 
        3.安全性不同，PreparedStatement可以有效防止sql注入，而Statement不能防止sql注入。
### callableStatement
```
创建一个存储过程：
    DElimiter $
    CREATE PROCEDURE pro_findById(In SID INT)
    BEGIN 
        SELECT * FROM student where id=sid;
    END
    $
```
### 接收输入参数 的存储过程
1.mysql调用存储过程  ：CALL pro_findById(4); 
2.JAVA中调用存储过程； 
CallableStatement接口： 
    //获取数据库连接 
    //准备sql
```
String sql="CALL pro_findById(?);"
    stmt=conn.prepareCall(sql);
    stmt.setInt(1,4);
    //所有调用存储过程都需要使用executeQuery()
    ResultSet res=stmt.executeQuery();
    while(res.next()){
        int id=res.getInt("id");
        String name=res.getString("name");
        System.out.println(id+name);
    }
```
### 输入输出参数的存储过程
```
DElimiter $
CREATE PROCEDURE pro_findById2(IN sid INT,OUT sname VARCHAR(20))
BEGIN 
    SELECT NAME into sname FROM student where id=sid;
END
$
CALL pro_findById2(5,@name);
SELECT @name;
```
registerOutParameter(int paramterIndex,int sqlType) 
按照顺序位置parameterIndex将OUT参数注册为JDBC类型sqlType，所有OUT参数都必须在执行存储过程前注册。sqlType为OUT参数指定的JDBC类型确定必须用于get方法，以读取该参数值的Java类型
创建存储过程的时候参数有类型
```
driver=new com.mysql.jdbc.Driver();
            conn=driver.connect(url, prop);
            String sql="CALL pro_findById2(?,?)";
            //执行预编译
            stmt=conn.prepareCall(sql);
            //设置参数值
            stmt.setInt(1,4);
            //注册输出参数
            /**
             * 参数1：位置
             * 参数2：存储过程中的输出参数jdbc类型 
             * */
            stmt.registerOutParameter(2,java.sql.Types.VARCHAR);
//          stmt.setString("李四");
            //发送参数执行
            stmt.executeQuery();
            //得到输出参数的值
            //callableStatement中的getXXX()
            **//getXX主要是获取存储过程中的输出参数**
            String result=stmt.getString(2);   //这个2是因为registerParameter注册的是2
            System.out.println(result);
```
    在Java项目中，  . 代表项目的根目录 
    在web项目中，代表java命令运行开始，从tomcat的bin目录开始 
类路径加载方式 
/代表classpath的根目录 
JdbcUtil.class.getResourceAsStream(“/”)
如果是java项目，classpath从bin开始； 
web项目下，classpath从WEB-INF/classes目录
无论java项目还是java web ，src下写的文件会自动拷贝到类路径下面
## 事务
Connection接口： 
    setAutoCommit(boolean autoCommit);设置事务可以自动提交；false表示手动提交
