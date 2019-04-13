
# JDBC大数据 - 李昆鹏的博客 - CSDN博客


2018年06月15日 23:32:26[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：119


-----------------------------------------JDBC大数据-------------------------------------------
# 大数据
## 1　什么是大数据
所谓大数据，就是大的字节数据，或大的字符数据。标准SQL中提供了如下类型来保存大数据类型：
|类型
|长度
|
|tinyblob
|2|8-|-1B（256B）
|
|blob
|2|16|-1B（64K）
|
|mediumblob
|2|24|-1B（16M）
|
|longblob
|2|32|-1B（4G）
|
|tinyclob
|2|8-|-1B|（256B|）
|
|clob
|2|16|-1B|（64K|）
|
|mediumclob
|2|24|-1B|（16M|）
|
|longclob
|2|32|-1B|（4G|）
|
在mysql中没有提供tinyclob、clob、mediumclob、longclob四种类型，而是使用如下四种类型来处理文本大数据：
|类型
|长度
|
|tinytext
|2|8-|-1B（256B）
|
|text
|2|16|-1B（64K）
|
|mediumtext
|2|24|-1B（16M）
|
|longtext
|2|32|-1B（4G）
|
首先我们需要创建一张表，表中要有一个mediumblob（16M）类型的字段。
|CREATE TABLE tab_bin(
|id   INT        PRIMARY  KEY AUTO_INCREMENT,
|filename VARCHAR(100),
|data      MEDIUMBLOB
|);
|
向数据库插入二进制数据需要使用PreparedStatement为原setBinaryStream(int, InputSteam)方法来完成。
|con = JdbcUtils.|getConnection|();
|String sql =|"insert into tab_bin(filename,data) values(?,  ?)"|;
|pstmt = con.prepareStatement(sql);
|pstmt.setString(1,|"a.jpg"|);
|InputStream in =|new|FileInputStream(|"f:\\a.jpg"|);
|pstmt.setBinaryStream(2, in);
|pstmt.executeUpdate();
|
读取二进制数据，需要在查询后使用ResultSet类的getBinaryStream()方法来获取输入流对象。也就是说，PreparedStatement有setXXX()，那么ResultSet就有getXXX()。
|con = JdbcUtils.|getConnection|();
|String sql =|"select filename,data from tab_bin where  id=?"|;
|pstmt = con.prepareStatement(sql);
|pstmt.setInt(1, 1);
|rs = pstmt.executeQuery();
|rs.next();
|String filename = rs.getString(|"filename"|);
|OutputStream out =|new|FileOutputStream(|"F:\\"|+ filename);
|InputStream in =  rs.getBinaryStream(|"data"|);
|IOUtils.|copy|(in, out);
|out.close();
|
还有一种方法，就是把要存储的数据包装成Blob类型，然后调用PreparedStatement的setBlob()方法来设置数据
|con = JdbcUtils.|getConnection|();
|String sql =|"insert into tab_bin(filename,data) values(?, ?)"|;
|pstmt = con.prepareStatement(sql);
|pstmt.setString(1,|"a.jpg"|);
|File file =|new|File(|"f:\\a.jpg"|);
|byte|[] datas = FileUtils.|getBytes|(file);|//|获取文件中的数据
|Blob blob =|new|SerialBlob(datas);|//|创建|Blob|对象
|pstmt.setBlob(2, blob);|//|设置|Blob|类型的参数
|pstmt.executeUpdate();
|
|con = JdbcUtils.|getConnection|();
|String sql =|"select filename,data from tab_bin where id=?"|;
|pstmt = con.prepareStatement(sql);
|pstmt.setInt(1, 1);
|rs = pstmt.executeQuery();
|rs.next();
|String filename = rs.getString(|"filename"|);
|File file =|new|File(|"F:\\"|+ filename) ;
|Blob blob = rs.getBlob(|"data"|);
|byte|[] datas = blob.getBytes(0, (|int|)file.length());
|FileUtils.|writeByteArrayToFile|(file,  datas);
|
二进制和字符数据的JDBC存取代码示例：
|package|com.rl.jdbc;
|import|java.io.FileInputStream;
|import|java.io.FileNotFoundException;
|import|java.io.FileOutputStream;
|import|java.io.FileReader;
|import|java.io.FileWriter;
|import|java.io.IOException;
|import|java.io.InputStream;
|import|java.io.OutputStream;
|import|java.io.Reader;
|import|java.io.Writer;
|import|java.sql.Clob;
|import|java.sql.Connection;
|import|java.sql.DriverManager;
|import|java.sql.PreparedStatement;
|import|java.sql.ResultSet;
|import|java.sql.SQLException;
|public|class|JDBCAdd3 {
|/**
|*|@param|args
|*/
|public|static|void|main(String[]|args|) {
|//saveLob();
|getLob|();
|}
|/**
|*|将二进制和文本存储到数据库中
|*/
|public|static|void|saveLob(){
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
|//|定义数据库预编译的|sql|执行对象
|PreparedStatement|ps|=|null|;
|//|定义要执行的|sql|增加语句
|String|addsql|=|"insert into lob_test values(?,?)  "|;
|try|{
|//|注册数据库的驱动程序
|Class.|forName|(|driverClass|);
|//|获得数据库的连接
|conn|= DriverManager.|getConnection|(|url|,|username|,|password|);
|//|创建预编译|sql|对象
|ps|=|conn|.prepareStatement(|addsql|);
|//|创建二进制输入流
|InputStream|in|=|new|FileInputStream(|"E:/java|学习课堂资料|/java|第二阶段资料|/javaweb|阶段资料|/JDBC|基础经典剖析资料|/jdjc.jpg"|);
|//|创建文本输入流
|Reader|reader|=|new|FileReader(|"E:/java|学习课堂资料|/java|第二阶段资料|/javaweb|阶段资料|/JDBC|基础经典剖析资料|/person.sql"|);
|//|设置二进制参数
|ps|.setBinaryStream(1,|in|);
|//|设置文本参数
|ps|.setClob(2,|reader|);
|int|count|=|ps|.executeUpdate();
|System.|out|.println(|"|当前|sql|的更新条数：|"|+|count|);
|}|catch|(ClassNotFoundException|e|) {
|e|.printStackTrace();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}|catch|(FileNotFoundException|e|) {
|//|TODO|Auto-generated catch block
|e|.printStackTrace();
|}|finally|{
|try|{
|if|(|ps|!=|null|)
|ps|.close();
|if|(|conn|!=|null|)
|conn|.close();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|}
|/**
|*|从数据库中取出二进制和文本到硬盘上
|*/
|public|static|void|getLob(){
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
|//|定义数据库预编译的|sql|执行对象
|PreparedStatement|ps|=|null|;
|//|定义查询数据库结果集对象
|ResultSet|rs|=|null|;
|//|定义要执行的|sql|查询语句
|String|querysql|=|"select * from lob_test "|;
|//|定义二进制和字符输入输出流
|InputStream|in|=|null|;
|Reader|reader|=|null|;
|OutputStream|out|=|null|;
|Writer|writer|=|null|;
|try|{
|//|注册数据库的驱动程序
|Class.|forName|(|driverClass|);
|//|获得数据库的连接
|conn|= DriverManager.|getConnection|(|url|,|username|,|password|);
|//|创建预编译|sql|对象
|ps|=|conn|.prepareStatement(|querysql|);
|//|获得结果集对象
|rs|=|ps|.executeQuery();
|while|(|rs|.next()){
|//|获得二进制输入流
|in|=|rs|.getBinaryStream(1);
|//|获得大文本对象
|Clob|clob|=|rs|.getClob(2);
|//|获得|reader|对象
|reader|=|clob|.getCharacterStream();
|//|把图片写入到硬盘上
|out|=|new|FileOutputStream(|"E:/jdbc.jpg"|);
|byte|[]|bytes|=|new|byte|[1024];
|int|len|= 0;
|while|((|len|=|in|.read(|bytes|)) != -1){
|out|.write(|bytes|, 0,|len|);
|}
|out|.flush();
|System.|out|.println(|"|二进制数据写入成功！|"|);
|//|把文本写入到硬盘上
|writer|=|new|FileWriter(|"E:/person.sql"|);
|char|[]|chars|=|new|char|[1024];
|int|len1|= 0;
|while|((|len1|=|reader|.read(|chars|)) != -1){
|writer|.write(|chars|,0,|len1|);
|}
|writer|.flush();
|System.|out|.println(|"|文本文件写入成功|"|);
|}
|}|catch|(Exception|e|) {
|e|.printStackTrace();
|}|finally|{
|try|{
|if|(|writer|!=|null|)
|writer|.close();
|if|(|out|!=|null|)
|out|.close();
|if|(|reader|!=|null|)
|reader|.close();
|if|(|in|!=|null|)
|in|.close();
|if|(|ps|!=|null|)
|ps|.close();
|if|(|conn|!=|null|)
|conn|.close();
|}|catch|(IOException|e|) {
|e|.printStackTrace();
|}|catch|(SQLException|e|) {
|e|.printStackTrace();
|}
|}
|}
|}
|


