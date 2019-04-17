# jdbc篇第4课：封装Dbc - PeterBishop - CSDN博客





2018年11月27日 19:10:15[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：48
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课开始我们开始讲封装jdbc



  分析:

在前面几课中，我们都要先建立连接，获取Connection对象，而且代码是重复的，我们可以将这部分代码提取出来



  实现:
package com.tool;



import java.sql.Connection;

import java.sql.DriverManager;

import java.sql.SQLException;



//Database Connection，负责连接数据库，返回一个Connection对象

public class Dbc {



    //不允许调用构造器

    private Dbc(){}



    public static Connection getConnection(String driver,String url,String user, String password) throws ClassNotFoundException, SQLException {

        Class.*forName*(driver);

        return DriverManager.*getConnection*(url,user,password);

    }

}


  测试代码:
public static void main(String[] args) throws SQLException, ClassNotFoundException {

    String driver = "com.mysql.jdbc.Driver";

    String url = "jdbc:mysql://localhost:3306/teach";

    String user = "root";

    String password = "root";

    Connection conn = Dbc.*getConnection*(driver, url, user, password);



    //测试下连接是否真的可用

    Statement statement = conn.createStatement();

    String sql = "select * from tbl_employee where id = 1";

    ResultSet resultSet = statement.executeQuery(sql);

    resultSet.next();

    System.*out*.println(resultSet.getInt(1));

    System.*out*.println(resultSet.getString(2));

    //连接确实可用

}


  结果:

1

xiaoye







