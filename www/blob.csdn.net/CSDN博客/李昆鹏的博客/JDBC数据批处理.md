
# JDBC数据批处理 - 李昆鹏的博客 - CSDN博客


2018年06月15日 23:33:24[李昆鹏](https://me.csdn.net/weixin_41547486)阅读数：29标签：[JDBC																](https://so.csdn.net/so/search/s.do?q=JDBC&t=blog)个人分类：[JDBC																](https://blog.csdn.net/weixin_41547486/article/category/7739465)


--------------------------------------------------JDBC数据批处理-----------------------------------------------------
# 批处理
## PreparedStatement批处理
批处理就是一批一批的处理，而不是一个一个的处理！
当你有10条SQL语句要执行时，一次向服务器发送一条SQL语句，这么做效率上很差！处理的方案是使用批处理，即一次向服务器发送多条SQL语句，然后由服务器一次性处理。
PreparedStatement的批处理有所不同，因为每个PreparedStatement对象都绑定一条SQL模板。所以向PreparedStatement中添加的不是SQL语句，而是给“?”赋值。
|con = JdbcUtils.|getConnection|();
|String sql =|"insert into stu values(?,?,?,?)"|;
|pstmt = con.prepareStatement(sql);
|for|(|int|i = 0; i < 10; i++) {
|pstmt.setString(1,|"S_10"|+ i);
|pstmt.setString(2,|"stu"|+ i);
|pstmt.setInt(3, 20 + i);
|pstmt.setString(4, i % 2 == 0 ?|"male"|:|"female"|);
|pstmt.addBatch();
|}
|pstmt.executeBatch();
|
批量增加数据代码示例：
|package|com.rl.jdbc;
|import|java.sql.Connection;
|import|java.sql.Date;
|import|java.sql.DriverManager;
|import|java.sql.PreparedStatement;
|import|java.sql.SQLException;
|public|class|JDBCAdd4 {
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
|PreparedStatement|ps|=|null|;
|//|定义要执行的|sql|增加语句
|String|addsql|=|"insert into person  values(personid.nextval,?,?,?) "|;
|try|{
|//|注册数据库的驱动程序
|Class.|forName|(|driverClass|);
|//|获得数据库的连接
|conn|= DriverManager.|getConnection|(|url|,|username|,|password|);
|//|创建|sql|对象
|ps|=|conn|.prepareStatement(|addsql|);
|//|向|person|表插入|100|条数据
|for|(|int|i|= 0;|i|< 100;|i|++) {
|ps|.setString(1,|"|李昆鹏|"|);
|ps|.setString(2,|"|地球|"|);
|ps|.setDate(3,|new|Date(System.|currentTimeMillis|()));
|//|批量数据插入
|ps|.addBatch();
|}
|//|执行批量|sql|语句
|int|[]|a|=|ps|.executeBatch();
|for|(|int|i|= 0;|i|<|a|.|length|;|i|++) {
|System.|out|.println(|"|更新新数据条数：|"|+|a|[|i|]);
|}
|}|catch|(ClassNotFoundException|e|) {
|e|.printStackTrace();
|}|catch|(SQLException|e|) {
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
|}
|


