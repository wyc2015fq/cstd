
# JDBC数据库变更（增删改查） - 李昆鹏的博客 - CSDN博客


2018年06月15日 23:26:12[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：40标签：[JDBC																](https://so.csdn.net/so/search/s.do?q=JDBC&t=blog)个人分类：[JDBC																](https://blog.csdn.net/weixin_41547486/article/category/7739465)


------------------------------------------------------JDBC数据库变更（增删改查）-----------------------------------------
**JDBC应用示例**
下面开始编写第一个JDBC程序
|public void show() {
|//4|大参数,driverClassName(|每个数据库不一样) / url(针对不同的数据库连接格式不同) /
|//username(|数据库用户名) / password(|数据库密码)
|String driverClassNameString = "oracle.jdbc.OracleDriver";
|String url = "jdbc:oracle:thin:@127.0.0.1:1521:orcl";
|String username = "scott";
|String password = "tiger";
|Connection con=null;
|ResultSet rs=null;
|Statement statement=null;
|try{
|//|注册驱动类
|Class.forName(driverClassNameString);
|//|得到连接器,|基础
|con =  DriverManager.getConnection(url,username,password);
|//|语句集
|statement = con.createStatement();
|//|执行内容字符串化
|//|插入数据
|//           String sql = "insert into stu  values(null,'john',99,99,99);";
|//|改表名
|//           String sql = "rename table stu  to student;";
|//|删除
|//           String sql = "delete from  student where id=5;";
|//|修改信息 update
|//           int i =  statement.executeUpdate(sql);
|//           System.out.println("|插入到"+i);
|//|用到查询就需要返回字符集
|String sql = "select * from student;";
|rs = statement.executeQuery(sql);
|while(rs.next()) {
|System.out.println(rs.getString(1) + ",  " + rs.getString(2)
|+ ", " +  rs.getString(3)+","+rs.getString(4)+","+rs.getString(5));
|}
|}catch (Exception e) {
|throw new RuntimeException(e);
|}finally {
|try{
|if(rs != null) rs.close();
|if(statement != null) statement.close();
|if(con != null) con.close();
|}catch(Exception e) {
|e.printStackTrace();
|}
|}
|}
|
|

## 1  引入数据库的驱动jar包：
Oracle：ojdbc14-10.2.0.2.0.jar
Mysql: mysql-connector-java-5.1.13-bin.jar；
## 2. 获取连接
获取连接需要两步，一是使用DriverManager来注册驱动，二是使用DriverManager来获取Connection对象。
Ø**注册驱动**
看清楚了，注册驱动就只有一句话：Class.forName(“oracle.jdbc.OracleDriver”)。
Ø**获取连接**
获取连接的也只有一句代码：DriverManager.getConnection(url,username,password)，其中username和password是登录数据库的用户名和密码。
url查对复杂一点，它是用来找到要连接数据库“网址”，就好比你要浏览器中查找百度时，也需要提供一个url。下面是mysql的url：
jdbc:oracle:thin:@127.0.0.1:1521:orcl
JDBC规定url的格式由三部分组成，每个部分中间使用冒号分隔。
l  第一部分是jdbc，这是固定的；
l  第二部分是数据库名称，那么连接mysql数据库，第二部分当然是mysql了；
l  第三部分是由数据库厂商规定的，我们需要了解每个数据库厂商的要求，mysql的第三部分分别由数据库服务器的IP地址（localhost）、端口号（3306），以及DATABASE名称(mydb1)组成。
下面是获取连接的语句：
Connection con = DriverManager.getConnection(“jdbc:oracle:thin:@127.0.0.1:1521:orcl
”,”username”,”password”);
## 3　获取Statement
在得到Connectoin之后，说明已经与数据库连接上了，下面是通过Connection获取Statement对象的代码：
Statement stmt =con.createStatement();
Statement是用来向数据库发送要执行的SQL语句的！
## 4　发送SQL增、删、改、查语句
String sql = “insertinto user value(’zhangSan’, ’123’)”;
int m =stmt.executeUpdate(sql);
其中int类型的返回值表示执行这条SQL语句所影响的行数，我们知道，对insert来说，最后只能影响一行，而update和delete可能会影响0~n行。
如果SQL语句执行失败，那么executeUpdate()会抛出一个SQLException。
发送SQL查询语句
String sql = “select* from user”;
ResultSet rs =stmt.executeQuery(sql);
请注册，执行查询使用的不是executeUpdate()方法，而是executeQuery()方法。executeQuery()方法返回的是ResultSet，ResultSet封装了查询结果，我们称之为结果集。
## 5　如果是查询读取结果集中的数据
ResultSet就是一张二维的表格，它内部有一个“行光标”，光标默认的位置在“第一行上方”，我们可以调用rs对象的next()方法把“行光标”向下移动一行，当第一次调用next()方法时，“行光标”就到了第一行记录的位置，这时就可以使用ResultSet提供的getXXX(int col)方法来获取指定列的数据了：
rs.next();//光标移动到下一行
rs.getInt(1);//获取第一行第一列的数据
当你使用rs.getInt(1)方法时，你必须可以肯定第1列的数据类型就是int类型，如果你不能肯定，那么最好使用rs.getObject(1)。在ResultSet类中提供了一系列的getXXX()方法，比较常用的方法有：
ObjectgetObject(int col)
StringgetString(int col)
int getInt(intcol)
doublegetDouble(int col)
## 6　关闭资源
与IO流一样，使用后的东西都需要关闭！关闭的顺序是先得到的后关闭，后得到的先关闭。
rs.close();
stmt.close();
con.close();
## 7 示例代码
所谓规范化代码就是无论是否出现异常，都要关闭ResultSet、Statement，以及Connection，如果你还记得IO流的规范化代码，那么下面的代码你就明白什么意思了。
|@Test
|public|void|query() {
|Connection con =|null|;
|Statement stmt =|null|;
|ResultSet rs =|null|;
|try|{
|con =|getConnection|();
|stmt = con.createStatement();
|String sql =|"select * from user"|;
|rs = stmt.executeQuery(sql);
|while|(rs.next()) {
|String username =  rs.getString(1);
|String password = rs.getString(2);
|System.|out|.println(username  +|", "|+ password);
|}
|}|catch|(Exception e) {
|throw|new|RuntimeException(e);
|}|finally|{
|try|{
|if|(rs !=|null|) rs.close();
|if|(stmt !=|null|) stmt.close();
|if|(con !=|null|) con.close();
|}|catch|(SQLException e) {}
|}
|}
|
Jdbc增删改代码示例：
|package|com.rl.jdbc;
|import|java.sql.Connection;
|import|java.sql.DriverManager;
|import|java.sql.SQLException;
|import|java.sql.Statement;
|public|class|JDBCAdd {
|/**
|*|@param|args
|*/
|public|static|void|main(String[]|args|) {
|//|设置|oracle|数据库驱动的类
|String|driverClass|=|"oracle.jdbc.OracleDriver"|;
|//|定义连接|oracle|的|url
|String|url|=|"jdbc:oracle:thin:@127.0.0.1:1521:orcl"|;
|//|用户名
|String|username|=|"scott"|;
|//|密码
|String|password|=|"tiger"|;
|//|定义|connection|连接
|Connection|conn|=|null|;
|//|定义数据库的|sql|执行对象
|Statement|stmt|=|null|;
|//|定义要执行的|sql|增加语句
|String|addsql|=|"insert into  person(id,name,address,birthday) "
|+|"values(personid.nextval,'|李昆鹏|','|地球|',to_date('1988-02-22','yyyy-mm-dd'))"|;
|//|定义要执行的|sql|修改语句
|String|updatesql|=|"update person p set p.address = '|中国|' where p.id = 1"|;
|//|定义要执行的|sql|删除语句
|String|deletesql|=|"delete person p where p.id =  1"|;
|try|{
|//|注册数据库的驱动程序
|Class.|forName|(|driverClass|);
|//|获得数据库的连接
|conn|= DriverManager.|getConnection|(|url|,|username|,|password|);
|//|创建|sql|对象
|stmt|=|conn|.createStatement();
|//|执行|sql|,|返回响应行数
|int|count|=|stmt|.executeUpdate(|deletesql|);
|System.|out|.println(|"|当前|sql|的更新条数：|"|+|count|);
|}|catch|(ClassNotFoundException|e|) {
|e|.printStackTrace();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}|finally|{
|try|{
|if|(|stmt|!=|null|)
|stmt|.close();
|if|(|conn|!=|null|)
|conn|.close();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|}
|}
|
JDBC查询代码示例：
|package|com.rl.jdbc;
|import|java.sql.Connection;
|import|java.sql.DriverManager;
|import|java.sql.ResultSet;
|import|java.sql.ResultSetMetaData;
|import|java.sql.SQLException;
|import|java.sql.Statement;
|import|java.util.ArrayList;
|import|java.util.List;
|public|class|JDBCUpdate {
|/**
|*|@param|args
|*/
|public|static|void|main(String[]|args|) {
|//|设置|oracle|数据库驱动的类
|String|driverClass|=|"oracle.jdbc.OracleDriver"|;
|//|定义连接|oracle|的|url
|String|url|=|"jdbc:oracle:thin:@127.0.0.1:1521:orcl"|;
|//|用户名
|String|username|=|"scott"|;
|//|密码
|String|password|=|"tiger"|;
|//|定义|connection|连接
|Connection|conn|=|null|;
|//|定义数据库的|sql|执行对象
|Statement|stmt|=|null|;
|//|定义查询结果的|ResultSet|对象
|ResultSet|rs|=|null|;
|//|定义|sql|查询语句
|String|querySql|=|"select * from person"|;
|try|{
|//|注册数据库的驱动程序
|Class.|forName|(|driverClass|);
|//|获得数据库的连接
|conn|= DriverManager.|getConnection|(|url|,|username|,|password|);
|//|创建|sql|对象
|stmt|=|conn|.createStatement();
|//|执行查询|sql
|rs|=|stmt|.executeQuery(|querySql|);
|/*
|while(rs.next()){
|//|通过列名获得查询数据
|String id =  rs.getString("id");
|String name =  rs.getString("name");
|String address =  rs.getString("address");
|String birthday =  rs.getString("birthday");
|System.out.println("id:"+id+"\t|姓名|:"+name+"\t|地址|:"+address+"\t|生日|"+birthday);
|//|通过列的顺序获得查询数据
|String id1 =  rs.getString(1);
|String name1 =  rs.getString(2);
|String address1 =  rs.getString(3);
|String birthday1 =  rs.getString(4);
|System.out.println("id:"+id1+"\t|姓名|:"+name1+"\t|地址|:"+address1+"\t|生日|"+birthday1);
|//|通过不同的类型使用不同的方法
|Integer id2 =  rs.getInt("id");
|String name2 =  rs.getString("name");
|String address2 =  rs.getString("address");
|Date birthday2 =  rs.getDate("birthday");
|System.out.println("id:"+id2+"\t|姓名|:"+name2+"\t|地址|:"+address2+"\t|生日|"+birthday2);
|//|通过|Object|类型接收和查询方法
|Object id3 =  rs.getObject("id");
|Object name3 =  rs.getObject("name");
|Object address3 =  rs.getObject("address");
|Object birthday3 =  rs.getObject("birthday");
|System.out.println("id:"+id3+"\t|姓名|:"+name3+"\t|地址|:"+address3+"\t|生日|"+birthday3);
|System.out.println("------------------------------------------------------");
|//|获得|object|对象所对应的具体的类型
|System.out.println(id3.getClass()+"\t"+name3.getClass()+"\t"+address3.getClass()+"\t"+birthday3.getClass());
|}
|*/
|//|得到查询的元数据，包括表名，列的数量，列名。。。。。。
|ResultSetMetaData|rsmd|=|rs|.getMetaData();
|//|得到查询的列数
|Integer|cols|=|rsmd|.getColumnCount();
|System.|out|.println(|"|列数|:"|+|cols|);
|//|创建一个集合来存储列名
|List<String>|listColumn|=|new|ArrayList<String>();
|for|(|int|i|=0;|i|<|cols|;|i|++){
|//|获得每一列的列名
|String|columnName|=|rsmd|.getColumnName(|i|+1);
|//|将列名添加到集合中
|listColumn|.add(|columnName|);
|//|打印列名
|System.|out|.print(|columnName|+|"\t"|);
|}
|System.|out|.println();
|//|根据列名来获得列下的值
|while|(|rs|.next()){
|for|(String|cname|:|listColumn|){
|String|val|=|rs|.getString(|cname|);
|System.|out|.print(|val|+|"\t"|);
|}
|System.|out|.println();
|}
|}|catch|(ClassNotFoundException|e|) {
|e|.printStackTrace();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}|finally|{
|try|{
|if|(|stmt|!=|null|)
|stmt|.close();
|if|(|conn|!=|null|)
|conn|.close();
|if|(|rs|!=|null|)
|rs|.close();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|}
|}
|


