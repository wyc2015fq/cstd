
# java入门教程-12.9Java数据库之修改记录 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:53:14[seven-soft](https://me.csdn.net/softn)阅读数：134个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



修改数据表记录也有3种方案。
## 一.使用Statement对象
实现修改数据表记录的SQL语句的语法是：
update表名 set 字段名1 = 字段值1，字段名2 = 字段值2，……where特定条件
例如：
update ksInfo set 姓名 = ‘张小卫’where 姓名 = ‘张大卫’
先创建一个SQL语句，然砶调用Statement对象的executeUpdate()方法。例如，
sql = “update ksInfo set 姓名 = ‘”+txtName.getText();
sql = sql + “,成绩=”+txtScore.getText();
sql = sql +”,地址=’”+txtAddr.getText();
sql= sql+”’,,简历=’”+txtResume.getText()+”’where 考号=”+txtNo.getText();
stmt.executeUpdate(sql);
## 二.使用ResultSet对象
先建立ResultSet对象，然后直接设定记录的字段值，修改数据表的记录。例如，
String sql = “select * from ksInfo where 姓名=’张大卫’”;//生成SQL语句
ResultSet rs = stmt.executeQuery(sql);//获取数据表结果集
if(rs.next()){
rs.updateString(2,’张小卫’);
try{rs.updateRow();}catch(Exception e){}
}
## 三.使用PrepareStatement对象
创建SQL语句时，暂时用参数？表示值，然后由SQL语句对象生成PrepareStatement对象，接着通过设定实际参数实现记录的更新。示意代码：
sql = “update ksInfo set 姓名=? where 姓名 = ‘张大卫’;
PrepareStatement pStmt = connect.prepareStatement(sql);
pStmt.setString(2,’张小卫’);//向名字字段填入数据
pStmt.executeUpdate();

