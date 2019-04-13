
# JDBC中DAO模式的例子示范 - 李昆鹏的博客 - CSDN博客


2018年06月15日 23:36:01[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：327


----------------------------------------JDBC中DAO模式的例子示范---------------------------------------
## 1　JdbcUtils的作用
你也看到了，连接数据库的四大参数是：驱动类、url、用户名，以及密码。这些参数都与特定数据库关联，如果将来想更改数据库，那么就要去修改这四大参数，那么为了不去修改代码，我们写一个JdbcUtils类，让它从配置文件中读取配置参数，然后创建连接对象。

### 2　DAO模式
DAO（Data Access Object）模式就是写一个类，把访问数据库的代码封装起来。DAO在数据库与业务逻辑（Service）之间。
l  实体域（JavaBean），即操作的对象，例如我们操作的表是user表，那么就需要先写一个User类；
l  DAO模式需要先提供一个DAO接口；
l  然后再提供一个DAO接口的实现类；
l  再编写一个DAO工厂，Service通过工厂来获取DAO实现。
**代码示例：**
**DBAutil****类  主要功能，建立数据库连接，返回执行sql****语句对象，释放资源**
|package|com.rl.jdbc.utils;
|import|java.io.InputStream;
|import|java.sql.Connection;
|import|java.sql.DriverManager;
|import|java.sql.PreparedStatement;
|import|java.sql.ResultSet;
|import|java.sql.SQLException;
|import|java.sql.Statement;
|import|java.util.Properties;
|public|class|DBUtils {

|/**
|*|获得|connction|方法
|*|@return
|*/
|public|static|Connection getConn(){
|//|获得字节输入流对象
|InputStream|in|= DBUtils.|class|.getClassLoader().getResourceAsStream(|"db.propertise"|);
|Properties|pps|=|new|Properties();
|Connection|conn|=|null|;
|try|{
|//|将文件加载到|properties|集合中
|pps|.load(|in|);
|//|取得|properties|中的配置文件参数
|String|driverClass|=|pps|.getProperty(|"driverClass"|);
|String|url|=|pps|.getProperty(|"url"|);
|String|username|=|pps|.getProperty(|"username"|);
|String|password|=|pps|.getProperty(|"password"|);
|//|加载|Oracle|驱动
|Class.|forName|(|driverClass|);
|//|获得|connection|对象
|conn|= DriverManager.|getConnection|(|url|,|username|,|password|);
|}|catch|(Exception|e|) {
|e|.printStackTrace();
|}
|return|conn|;
|}
|/**
|*|获得执行|sql|语句返回的预编译|PreparedStatement|对象
|*/
|public|static|PreparedStatement getPrepare(String|sql|){
|PreparedStatement|ps|=|null|;
|try|{
|Connection|conn|=|getConn|();
|ps|=|conn|.prepareStatement(|sql|);
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|return|ps|;
|}
|/**
|*|关闭修改数据库所用到的资源
|*/
|public|static|void|closeUpdate(PreparedStatement|ps|){
|try|{
|if|(|ps|!=|null|){
|Connection|conn|=|ps|.getConnection();
|ps|.close();
|if|(|conn|!=|null|)
|conn|.close();
|}
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|/**
|*|关闭查询数据库时所用到的资源
|*/
|public|static|void|closeQuery(ResultSet|rs|){
|try|{
|if|(|rs|!=|null|){
|Statement|st|=|rs|.getStatement();
|rs|.close();
|if|(|st|!=|null|){
|Connection|conn|=|st|.getConnection();
|st|.close();
|if|(|conn|!=|null|){
|conn|.close();
|}
|}
|}
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|}
|
**Person类，用于插入，数据库中的表单到java中的数据共享。**
|package|com.rl.jdbc.utils;
|import|java.util.Date;
|public|class|Person {
|private|Integer|id|;
|private|String|name|;
|private|String|address|;
|private|Date|birthday|;
|public|Integer getId() {
|return|id|;
|}
|public|void|setId(Integer|id|) {
|this|.|id|=|id|;
|}
|public|String getName() {
|return|name|;
|}
|public|void|setName(String|name|) {
|this|.|name|=|name|;
|}
|public|String getAddress() {
|return|address|;
|}
|public|void|setAddress(String|address|) {
|this|.|address|=|address|;
|}
|public|Date getBirthday() {
|return|birthday|;
|}
|public|void|setBirthday(Date|birthday|) {
|this|.|birthday|=|birthday|;
|}
|@Override
|public|String toString() {
|return|"Person [id="|+|id|+|", name="|+|name|+|", address="|+|address|+|", birthday="|+|birthday|+|"]\n"|;
|}
|}
|
**PersonDAO接口，定义person类于数据库中的一些方法**
|package com.rl.jdbc.dao;
|import java.util.List;
|import com.rl.jdbc.utils.Person;
|public interface PersonDAO {
|public  void savePerson(Person p);
|public  void updatePerson(Person p);
|public  Person getPersonById(Integer id);
|public  List<Person> listPerson();
|}
|
**PersonDaoImpl****类主要是实现PersonDAO****接口中的方法**
|package com.rl.jdbc.dao.impl;
|import java.sql.Date;
|import java.sql.PreparedStatement;
|import java.sql.ResultSet;
|import java.sql.SQLException;
|import java.util.ArrayList;
|import java.util.List;
|import com.rl.jdbc.dao.PersonDAO;
|import com.rl.jdbc.utils.DBUtils;
|import com.rl.jdbc.utils.Person;
|public class PersonDaoImpl implements  PersonDAO {
|/**
|* 將person的屬性值寫入數據庫中
|*/
|@Override
|public  void savePerson(Person p) {
|//要執行的sql语句
|String  sql = "insert into person values(personid.nextval,?,?,?)";
|//获得预编译对象
|PreparedStatement  ps = DBUtils.getPrepare(sql);
|try  {
|ps.setString(1,  p.getName());
|ps.setString(2,  p.getAddress());
|ps.setDate(3,  new Date(p.getBirthday().getTime()));
|int  count = ps.executeUpdate();
|System.out.println("数据库插入数据");
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally{
|//关闭资源
|DBUtils.closeUpdate(ps);
|}
|}
|/**
|* 对数据库中的person表进行修改
|*/
|@Override
|public  void updatePerson(Person p) {
|String  sql = "update person t set t.id = ?,t.name = ?,t.address = ?, t.birthday  = ? ";
|PreparedStatement  ps = DBUtils.getPrepare(sql);
|try  {
|ps.setInt(1,  p.getId());
|ps.setString(2,  p.getName());
|ps.setString(3,  p.getAddress());
|ps.setDate(4,  new Date(p.getBirthday().getTime()));
|int  count = ps.executeUpdate();
|System.out.println("数据已经修改成功");
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally{
|DBUtils.closeUpdate(ps);
|}
|}
|/**
|* 按指定的ID获取person表的一行数据，并传入到返回的一个person对象属性中
|*/
|@Override
|public  Person getPersonById(Integer id) {
|String  sql = "select * from person t where t.id = ?";
|PreparedStatement  ps = DBUtils.getPrepare(sql);
|ResultSet  rs = null;
|Person  p = new Person();
|try  {
|ps.setInt(1,  id);
|//获得结果集对象
|rs  = ps.executeQuery();
|//游标向下移动
|rs.next();
|//获取结果集对象中的值
|Integer  personid = rs.getInt("id");
|String  name = rs.getString("name");
|String  address = rs.getString("address");
|java.util.Date  birthday = rs.getDate("birthday");
|//将结果集中的集写入到Person对象中
|p.setId(personid);
|p.setName(name);
|p.setAddress(address);
|p.setBirthday(birthday);
|System.out.println("数据已写入Person对象中");
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally{
|DBUtils.closeQuery(rs);
|}
|return  p;
|}
|/**
|* 查询Person表所有的数据放入返回的Person集合中
|*/
|@Override
|public  List<Person> listPerson() {
|String  sql = "select * from person";
|PreparedStatement  ps = DBUtils.getPrepare(sql);
|ResultSet  rs = null;
|//创建Person对象集合
|List<Person>  listPerson = new ArrayList<Person>();
|Person  p = new Person();
|try  {
|//获得结果集对象
|rs  = ps.executeQuery();
|//定义计算插入数据数量的变量
|int  i = 0;
|//循环游标向下移动
|while(rs.next()){
|//获取结果集对象中的值
|Integer  personid = rs.getInt("id");
|String  name = rs.getString("name");
|String  address = rs.getString("address");
|java.util.Date  birthday = rs.getDate("birthday");
|//将结果集中的集写入到Person对象中
|p.setId(personid);
|p.setName(name);
|p.setAddress(address);
|p.setBirthday(birthday);
|listPerson.add(p);
|i++;
|System.out.println(i  + "条数据已写入Person对象中并且添加到集合中");
|}
|}  catch (SQLException e) {
|e.printStackTrace();
|}  finally{
|DBUtils.closeQuery(rs);
|}
|return  listPerson;
|}
|}
|
**PersonTest类为了测试以上类功能是否能正常运转**
|package|com.rl.jdbc.test;
|import|java.util.Date;
|import|java.util.List;
|import|com.rl.jdbc.dao.impl.PersonDaoImpl;
|import|com.rl.jdbc.utils.Person;
|public|class|PersonTest {
|public|static|void|main(String[]|args|) {
|PersonDaoImpl|pd|=|new|PersonDaoImpl();
|Person|p|=|new|Person();
|//p.setId(1);
|p|.setName(|"|李昆鹏|"|);
|p|.setAddress(|"|地球之上|"|);
|p|.setBirthday(|new|Date());
|//|插入数据
|//pd.savePerson(p);
|//|修改数据
|//pd.updatePerson(p);
|//|给出指定的|ID|从数据库中获取数据到|person|对象
|//p = pd.getPersonById(1);
|//System.out.println(p);
|//|将数据库|Person|表中所有的数据写入|Person|集合中
|List<Person>|list|=|pd|.listPerson();
|System.|out|.println(|list|);
|}
|}
|



