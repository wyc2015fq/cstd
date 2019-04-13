
# JDBC封装DBUtil和DAO模式 - 李昆鹏的博客 - CSDN博客


2018年06月15日 23:34:40[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：382


------------------------------------- JDBC封装DBUtil和DAO模式----------------------------------------------
# JdbcUtils工具类

## 1　JdbcUtils的作用
你也看到了，连接数据库的四大参数是：驱动类、url、用户名，以及密码。这些参数都与特定数据库关联，如果将来想更改数据库，那么就要去修改这四大参数，那么为了不去修改代码，我们写一个JdbcUtils类，让它从配置文件中读取配置参数，然后创建连接对象。

## 2　JdbcUtils代码
JdbcUtils.java
|public|class|JdbcUtils {
|private|static|final|String|dbconfig|=|"dbconfig.properties"|;
|private|static|Properties|prop|=|new|Properties();
|static|{
|try|{
|InputStream in = Thread.|currentThread|().getContextClassLoader().getResourceAsStream(|dbconfig|);
|prop|.load(in);
|Class.|forName|(|prop|.getProperty(|"driverClassName"|));
|}|catch|(IOException e) {
|throw|new|RuntimeException(e);
|}
|}
|public|static|Connection getConnection() {
|try|{
|return|DriverManager.|getConnection|(|prop|.getProperty(|"url"|),
|prop|.getProperty(|"username"|),|prop|.getProperty(|"password"|));
|}|catch|(Exception e) {
|throw|new|RuntimeException(e);
|}
|}
|}
|
dbconfig.properties
|driverClassName=|oracle.jdbc.OracleDriver
|url=|jdbc:oracle:thin:@127.0.0.1:1521:orcl
|username=|scott
|password=|tiger
|

## 3 UserDao

### 1　DAO模式
DAO（Data Access Object）模式就是写一个类，把访问数据库的代码封装起来。DAO在数据库与业务逻辑（Service）之间。
l  实体域（JavaBean），即操作的对象，例如我们操作的表是user表，那么就需要先写一个User类；
l  DAO模式需要先提供一个DAO接口；
l  然后再提供一个DAO接口的实现类；
l  再编写一个DAO工厂，Service通过工厂来获取DAO实现。

### 2　代码
User.java
|public|class|User {
|private|String|uid|;
|private|String|username|;
|private|String|password|;
|…
|}
|
UserDao.java
|public|interface|UserDao {
|public|void|add(User user);
|public|void|mod(User user);
|public|void|del(String uid);
|public|User find(String uid);
|public|List<User> findAll();
|}
|
UserDaoImpl.java
|public|class|UserDaoImpl|implements|UserDao {
|public|void|add(User user) {
|Connection con =|null|;
|PreparedStatement pstmt =|null|;
|try|{
|con = JdbcUtils.|getConnection|();
|String sql =|"insert into user value(?,?,?)"|;
|pstmt = con.prepareStatement(sql);
|pstmt.setString(1, user.getUid());
|pstmt.setString(2,  user.getUsername());
|pstmt.setString(3,  user.getPassword());
|pstmt.executeUpdate();
|}|catch|(Exception e) {
|throw|new|RuntimeException(e);
|}|finally|{
|try|{
|if|(pstmt !=|null|) pstmt.close();
|if|(con !=|null|) con.close();
|}|catch|(SQLException e) {}
|}
|}
|public|void|mod(User user) {
|Connection con =|null|;
|PreparedStatement pstmt =|null|;
|try|{
|con = JdbcUtils.|getConnection|();
|String sql =|"update user set username=?, password=? where  uid=?"|;
|pstmt = con.prepareStatement(sql);
|pstmt.setString(1,  user.getUsername());
|pstmt.setString(2,  user.getPassword());
|pstmt.setString(3, user.getUid());
|pstmt.executeUpdate();
|}|catch|(Exception e) {
|throw|new|RuntimeException(e);
|}|finally|{
|try|{
|if|(pstmt !=|null|) pstmt.close();
|if|(con !=|null|) con.close();
|}|catch|(SQLException e) {}
|}
|}
|public|void|del(String uid) {
|Connection con =|null|;
|PreparedStatement pstmt =|null|;
|try|{
|con = JdbcUtils.|getConnection|();
|String sql =|"delete from user where uid=?"|;
|pstmt = con.prepareStatement(sql);
|pstmt.setString(1, uid);
|pstmt.executeUpdate();
|}|catch|(Exception e) {
|throw|new|RuntimeException(e);
|}|finally|{
|try|{
|if|(pstmt !=|null|) pstmt.close();
|if|(con !=|null|) con.close();
|}|catch|(SQLException e) {}
|}
|}
|public|User load(String uid) {
|Connection con =|null|;
|PreparedStatement pstmt =|null|;
|ResultSet rs =|null|;
|try|{
|con = JdbcUtils.|getConnection|();
|String sql =|"select * from user where uid=?"|;
|pstmt = con.prepareStatement(sql);
|pstmt.setString(1, uid);
|rs = pstmt.executeQuery();
|if|(rs.next()) {
|return|new|User(rs.getString(1), rs.getString(2),  rs.getString(3));
|}
|return|null|;
|}|catch|(Exception e) {
|throw|new|RuntimeException(e);
|}|finally|{
|try|{
|if|(pstmt !=|null|) pstmt.close();
|if|(con !=|null|) con.close();
|}|catch|(SQLException e) {}
|}
|}
|public|List<User> findAll() {
|Connection con =|null|;
|PreparedStatement pstmt =|null|;
|ResultSet rs =|null|;
|try|{
|con = JdbcUtils.|getConnection|();
|String sql =|"select * from user"|;
|pstmt = con.prepareStatement(sql);
|rs = pstmt.executeQuery();
|List<User> userList =|new|ArrayList<User>();
|while|(rs.next()) {
|userList.add(|new|User(rs.getString(1), rs.getString(2), rs.getString(3)));
|}
|return|userList;
|}|catch|(Exception e) {
|throw|new|RuntimeException(e);
|}|finally|{
|try|{
|if|(pstmt !=|null|) pstmt.close();
|if|(con !=|null|) con.close();
|}|catch|(SQLException e) {}
|}
|}
|}
|
UserDaoFactory.java
|public|class|UserDaoFactory {
|private|static|UserDao|userDao|;
|static|{
|try|{
|InputStream in = Thread.|currentThread|().getContextClassLoader()
|.getResourceAsStream(|"dao.properties"|);
|Properties prop =|new|Properties();
|prop.load(in);
|String className =  prop.getProperty(|"com.rl.jdbc.UserDao"|);
|Class clazz = Class.|forName|(className);
|userDao|= (UserDao)  clazz.newInstance();
|}|catch|(Exception e) {
|throw|new|RuntimeException(e);
|}
|}
|public|static|UserDao getUserDao() {
|return|userDao|;
|}
|}
|
dao.properties
|com.rl.jdbc.UserDao=|com.rl.jdbc.UserDaoImpl
|


