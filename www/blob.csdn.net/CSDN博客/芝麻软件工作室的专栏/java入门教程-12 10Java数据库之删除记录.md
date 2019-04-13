
# java入门教程-12.10Java数据库之删除记录 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月29日 07:54:18[seven-soft](https://me.csdn.net/softn)阅读数：180个人分类：[Java																](https://blog.csdn.net/softn/article/category/6242590)



删除数据表也有3种方案
## 一.使用Statement对象
删除数据表记录的SQL语句的语法是：
delete from 表名 where 特定条件
例如 ：
delete from ksInfo where 姓名 = ‘张大卫’
先创建一个SQL语句，然后调用Statement对象的executeUpdate()方法：
stmt.executeUpdate(sql);
## 二.使用ResultSet对象
先创建一个SQL语句，然后调用Statement对象的executeUpdate()方法。例如：
String sql = “select * from ksInfo where 姓名 = ‘张大卫’”;//生成SQL语句
ResultSet rs = stmt.executeQuery(sql);//获取数据表结果集
if(rs.next()){
rs.deleteRow();try{ rs.updateRow();}catch(Exception e){}
}
## 三.使用PrepareStatement对象
创建SQL语句时，暂时用参数？表示值，然后由SQL语句对象生成PrepareStatement对象，接着设定实际参数实现特定记录的删除。例如，以下示意代码：
sql = “delete form ksInfo where 姓名=?”;
PrepareStatement pStmt = connect.prepareStatement(sql);
pStmt.setString(2,’张大卫’);//给名字字段指定数据
pStmt.executeUpdate();

