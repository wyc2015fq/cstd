# Ubuntu下eclipse部署mysql jdbc驱动 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年10月01日 18:41:01[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：5385








1.mysql jdbc驱动包下载地址：[http://dev.mysql.com/downloads/connector/j/](http://dev.mysql.com/downloads/connector/j/)

   版本：mysql-connector-java-5.1.17.zip

2.下载后任意目录解压到/mysql-connector-java-5.1.17/mysql-connector-java-5.1.17-bin.jar；

    /mysql-connector-java-5.1.17/目录下还有docs和src文件夹，src文件夹里有lib目录；

3.前提：mysql和eclipse环境都安装好，参照前面blog关于ubuntu安装j2ee环境和mysql的文章；

4.打开eclispe，进入菜单windows->prefrences，进入Java->Build Path->user libraries，选择新建，命名为mysql，路径为：/mysql-connector-java-5.1.17/mysql-connector-java-5.1.17-bin.jar；

5.创建java project，命名为mysqlcon，右键其，选择Build Path->Add libraries，从列表中选择user library，在next界面中选择上面部署的mysql安装包即可；

6.开发，mysqlcon工程下新建class，命名为my，包为cn，即cn.my的java文件：
Package cn;import java.sql.*;publicclass my{
  publicstaticvoid main(String args[]) {
    try {
      Class.forName("com.mysql.jdbc.Driver");     //加载MYSQL JDBC驱动程序   
      System.out.println("Success loading Mysql Driver!");
    }
    catch (Exception e) {
      System.out.print("Error loading Mysql Driver!");
      e.printStackTrace();
    }
    try {
      Connection connect = DriverManager.getConnection(
          "jdbc:mysql://localhost:3306/dbname","root","password");
      System.out.println("Success connect Mysql server!");      //select code
      Statement stmt = connect.createStatement();
      ResultSet rs = stmt.executeQuery("select * from t_auth");//t_auth为表的名称while (rs.next()) {
        System.out.println(rs.getString("auth_path"));//auth_path是表t_auth的一个字段      }      //insert into code      PreparedStatement Statement=connect.prepareStatement("INSERT INTO t_auth VALUES(?,?)");
      Statement.setString(1,"2");
      Statement.setString(2,"系统管理");
      Statement.executeUpdate();  
    }
    catch (Exception e) {
      System.out.print("get data error!");
      e.printStackTrace();
    }
  }
}
能正确显示和插入即成功！



