
# java入门教程-12.8Java数据库之插入记录 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:52:20[seven-soft](https://me.csdn.net/softn)阅读数：135


插入数据表记录有3种方案
## 一.使用Statement对象
实现插入数据表记录的SQL语句的语法是：
insert into 表名(字段名1，字段名2,……)value (字段值1，字段值2，……)
例如：
insert into ksInfo(考号，姓名，成绩，地址，简历)value(‘200701’,’张大卫’534，’上海欧阳路218弄4-1202’,’’)
实现同样功能的Java程序代码是：
sql = “insert intoksIno(考号，姓名，成绩，地址，简历)”;
sql= = sq1+ “value(‘”+txtNo.getTxt()+’,’”+txtName.getText(0”’,”;
sql = sql+txtScore.getText();
sql=sql+”,’”+txtAddr.getText()+”’,’”+txtResume.getText()+”’)”;
stmt.executeUpdate(sql);
## 二.使用ResultSet对象
使用ResultSet对象的方法moveToInsertRow()将数据表游标移到插入位置，输入数据后，用方法insertRow()插入记录。例如，以下示意代码：
String sql= “select * from ksInfo”;//生成SQL语句
ResultSet rs = stmt.executeQuery(sql);//获取数据表结果集
rs.moveToInsertRow();//将数据表游标移到插入记录位置
rs.updateString(1,’200701’);//向考号字段填入数据
rs.updateString(2,’张大卫’);//向名字字段填入数据
rs.updateInt(3,534);//向成绩字段填入数据
rs.updateString(4,’上海欧阳路218弄4-1202’);//向地址字段填入数据
rs.updateString(5,’’);//向简历字段填入数据
try{rs.insertRow();}catch(Exception e){};//完成插入
## 三.使用PrepareStatement对象
与使用Statement对象的方法类似，只是创建SQL语句时暂时用参数？表示值，然后由SQL语句对象生成PrepareStatement对象，插入时通过设定实际参数，实现记录的更新。示意代码如下：
sql = “insert into ksInfo(考号，姓名，成绩，地址，简历)value (?,?,?,?,’’)”;
PrepareStatement pStmt = connect.prepareStatement(sql);
pStmt.setString(1,’200701’);//向考号字段填入数据
pStmt. setString (2,’张大卫’);//向名字字段填入数据
pStmt.setInt(3,534);//向成绩字段填入数据
pStmt. setString (4,’上海欧阳路218弄4-1202’);//向地址字段填入数据
pStmt. setString (5,’’);//向简历字段填入数据
pStmt.executeUpdate();

